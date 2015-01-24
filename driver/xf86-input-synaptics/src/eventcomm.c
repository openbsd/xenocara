/*
 * Copyright © 2004-2007 Peter Osterlund
 * Copyright © 2008-2012 Red Hat, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of Red Hat
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.  Red
 * Hat makes no representations about the suitability of this software
 * for any purpose.  It is provided "as is" without express or implied
 * warranty.
 *
 * THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
 * NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors:
 *      Peter Osterlund (petero2@telia.com)
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include <xserver-properties.h>
#include "eventcomm.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "synproto.h"
#include "synapticsstr.h"
#include <xf86.h>
#include <libevdev/libevdev.h>

#ifndef INPUT_PROP_BUTTONPAD
#define INPUT_PROP_BUTTONPAD 0x02
#endif
#ifndef INPUT_PROP_SEMI_MT
#define INPUT_PROP_SEMI_MT 0x03
#endif
#ifndef INPUT_PROP_TOPBUTTONPAD
#define INPUT_PROP_TOPBUTTONPAD 0x04
#endif
#ifndef ABS_MT_TOOL_Y
#define ABS_MT_TOOL_Y 0x3d
#endif

#define SYSCALL(call) while (((call) == -1) && (errno == EINTR))

#define LONG_BITS (sizeof(long) * 8)
#define NBITS(x) (((x) + LONG_BITS - 1) / LONG_BITS)
#define OFF(x)   ((x) % LONG_BITS)
#define LONG(x)  ((x) / LONG_BITS)
#define TEST_BIT(bit, array) ((array[LONG(bit)] >> OFF(bit)) & 1)

#define ABS_MT_MIN ABS_MT_SLOT
#define ABS_MT_MAX ABS_MT_TOOL_Y
#define ABS_MT_CNT (ABS_MT_MAX - ABS_MT_MIN + 1)

/**
 * Protocol-specific data.
 */
struct eventcomm_proto_data {
    /**
     * Do we need to grab the event device?
     * Note that in the current flow, this variable is always false and
     * exists for readability of the code.
     */
    BOOL need_grab;
    int st_to_mt_offset[2];
    double st_to_mt_scale[2];
    int axis_map[ABS_MT_CNT];
    int cur_slot;
    ValuatorMask **last_mt_vals;
    int num_touches;

    struct libevdev *evdev;
    enum libevdev_read_flag read_flag;

    int have_monotonic_clock;
};

static void
libevdev_log_func(enum libevdev_log_priority priority,
                  void *data,
                  const char *file, int line, const char *func,
                  const char *format, va_list args)
_X_ATTRIBUTE_PRINTF(6, 0);

static void
libevdev_log_func(enum libevdev_log_priority priority,
                  void *data,
                  const char *file, int line, const char *func,
                  const char *format, va_list args)
{
    int verbosity;

    switch(priority) {
        case LIBEVDEV_LOG_ERROR: verbosity = 0; break;
        case LIBEVDEV_LOG_INFO: verbosity = 4; break;
        case LIBEVDEV_LOG_DEBUG: verbosity = 10; break;
    }

    LogVMessageVerbSigSafe(X_NOTICE, verbosity, format, args);
}

static void
set_libevdev_log_handler(void)
{
                              /* be quiet, gcc *handwave* */
    libevdev_set_log_function((libevdev_log_func_t)libevdev_log_func, NULL);
    libevdev_set_log_priority(LIBEVDEV_LOG_DEBUG);
}

struct eventcomm_proto_data *
EventProtoDataAlloc(int fd)
{
    struct eventcomm_proto_data *proto_data;
    int rc;

    set_libevdev_log_handler();

    proto_data = calloc(1, sizeof(struct eventcomm_proto_data));
    if (!proto_data)
        return NULL;

    proto_data->st_to_mt_scale[0] = 1;
    proto_data->st_to_mt_scale[1] = 1;

    rc = libevdev_new_from_fd(fd, &proto_data->evdev);
    if (rc < 0) {
        free(proto_data);
        proto_data = NULL;
    } else
        proto_data->read_flag = LIBEVDEV_READ_FLAG_NORMAL;

    return proto_data;
}

static void
UninitializeTouch(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    struct eventcomm_proto_data *proto_data =
        (struct eventcomm_proto_data *) priv->proto_data;

    if (!priv->has_touch)
        return;

    if (proto_data->last_mt_vals) {
        int i;

        for (i = 0; i < priv->num_slots; i++)
            valuator_mask_free(&proto_data->last_mt_vals[i]);
        free(proto_data->last_mt_vals);
        proto_data->last_mt_vals = NULL;
    }

    proto_data->num_touches = 0;
}

static void
InitializeTouch(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    struct eventcomm_proto_data *proto_data =
        (struct eventcomm_proto_data *) priv->proto_data;
    int i;

    if (!priv->has_touch)
        return;

    proto_data->cur_slot = libevdev_get_current_slot(proto_data->evdev);
    proto_data->num_touches = 0;

    proto_data->last_mt_vals = calloc(priv->num_slots, sizeof(ValuatorMask *));
    if (!proto_data->last_mt_vals) {
        xf86IDrvMsg(pInfo, X_WARNING,
                    "failed to allocate MT last values mask array\n");
        UninitializeTouch(pInfo);
        return;
    }

    for (i = 0; i < priv->num_slots; i++) {
        int j;

        proto_data->last_mt_vals[i] = valuator_mask_new(4 + priv->num_mt_axes);
        if (!proto_data->last_mt_vals[i]) {
            xf86IDrvMsg(pInfo, X_WARNING,
                        "failed to allocate MT last values mask\n");
            UninitializeTouch(pInfo);
            return;
        }

        /* Axes 0-4 are for X, Y, and scrolling. num_mt_axes does not include X
         * and Y. */
        valuator_mask_set(proto_data->last_mt_vals[i], 0, 0);
        valuator_mask_set(proto_data->last_mt_vals[i], 1, 0);
        for (j = 0; j < priv->num_mt_axes; j++)
            valuator_mask_set(proto_data->last_mt_vals[i], 4 + j, 0);
    }
}

static Bool
EventDeviceOnHook(InputInfoPtr pInfo, SynapticsParameters * para)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    struct eventcomm_proto_data *proto_data =
        (struct eventcomm_proto_data *) priv->proto_data;
    int ret;

    set_libevdev_log_handler();

    if (libevdev_get_fd(proto_data->evdev) != -1) {
        struct input_event ev;

        libevdev_change_fd(proto_data->evdev, pInfo->fd);

        /* re-sync libevdev's state, but we don't care about the actual
           events here */
        libevdev_next_event(proto_data->evdev, LIBEVDEV_READ_FLAG_FORCE_SYNC, &ev);
        while (libevdev_next_event(proto_data->evdev,
                    LIBEVDEV_READ_FLAG_SYNC, &ev) == LIBEVDEV_READ_STATUS_SYNC)
            ;

    } else
        libevdev_set_fd(proto_data->evdev, pInfo->fd);


    if (para->grab_event_device) {
        /* Try to grab the event device so that data don't leak to /dev/input/mice */

        ret = libevdev_grab(proto_data->evdev, LIBEVDEV_GRAB);
        if (ret < 0) {
            xf86IDrvMsg(pInfo, X_WARNING, "can't grab event device, errno=%d\n",
                        -ret);
            return FALSE;
        }
    }

    proto_data->need_grab = FALSE;

    ret = libevdev_set_clock_id(proto_data->evdev, CLOCK_MONOTONIC);
    proto_data->have_monotonic_clock = (ret == 0);

    InitializeTouch(pInfo);

    return TRUE;
}

static Bool
EventDeviceOffHook(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    struct eventcomm_proto_data *proto_data = priv->proto_data;

    UninitializeTouch(pInfo);
    libevdev_grab(proto_data->evdev, LIBEVDEV_UNGRAB);
    libevdev_set_log_function(NULL, NULL);
    libevdev_set_log_priority(LIBEVDEV_LOG_INFO); /* reset to default */

    return Success;
}

/**
 * Test if the device on the file descriptior is recognized as touchpad
 * device. Required bits for touchpad recognition are:
 * - ABS_X + ABS_Y for absolute axes
 * - ABS_PRESSURE or BTN_TOUCH
 * - BTN_TOOL_FINGER
 * - BTN_TOOL_PEN is _not_ set
 *
 * @param evdev Libevdev handle
 * @param test_grab If true, test whether an EVIOCGRAB is possible on the
 * device. A failure to grab the event device returns in a failure.
 *
 * @return TRUE if the device is a touchpad or FALSE otherwise.
 */
static Bool
event_query_is_touchpad(struct libevdev *evdev, BOOL test_grab)
{
    int ret = FALSE, rc;

    if (test_grab) {
        rc = libevdev_grab(evdev, LIBEVDEV_GRAB);
        if (rc < 0)
            return FALSE;
    }

    /* Check for ABS_X, ABS_Y, ABS_PRESSURE and BTN_TOOL_FINGER */
    if (!libevdev_has_event_type(evdev, EV_SYN) ||
        !libevdev_has_event_type(evdev, EV_ABS) ||
        !libevdev_has_event_type(evdev, EV_KEY))
        goto unwind;

    if (!libevdev_has_event_code(evdev, EV_ABS, ABS_X) ||
        !libevdev_has_event_code(evdev, EV_ABS, ABS_Y))
        goto unwind;

    /* we expect touchpad either report raw pressure or touches */
    if (!libevdev_has_event_code(evdev, EV_KEY, BTN_TOUCH) &&
        !libevdev_has_event_code(evdev, EV_ABS, ABS_PRESSURE))
        goto unwind;

    /* all Synaptics-like touchpad report BTN_TOOL_FINGER */
    if (!libevdev_has_event_code(evdev, EV_KEY, BTN_TOOL_FINGER) ||
        libevdev_has_event_code(evdev, EV_ABS, BTN_TOOL_PEN)) /* Don't match wacom tablets */
        goto unwind;

    ret = TRUE;

 unwind:
    if (test_grab)
        libevdev_grab(evdev, LIBEVDEV_UNGRAB);

    return (ret == TRUE);
}

#define PRODUCT_ANY 0x0000

struct model_lookup_t {
    short vendor;
    short product_start;
    short product_end;
    enum TouchpadModel model;
};


static struct model_lookup_t model_lookup_table[] = {
    {0x0002, 0x0007, 0x0007, MODEL_SYNAPTICS},
    {0x0002, 0x0008, 0x0008, MODEL_ALPS},
    {0x05ac, PRODUCT_ANY, 0x222, MODEL_APPLETOUCH},
    {0x05ac, 0x223, 0x228, MODEL_UNIBODY_MACBOOK},
    {0x05ac, 0x229, 0x22b, MODEL_APPLETOUCH},
    {0x05ac, 0x22c, PRODUCT_ANY, MODEL_UNIBODY_MACBOOK},
    {0x0002, 0x000e, 0x000e, MODEL_ELANTECH},
    {0x0, 0x0, 0x0, 0x0}
};

/**
 * Check for the vendor/product id on the file descriptor and compare
 * with the built-in model LUT. This information is used in synaptics.c to
 * initialize model-specific dimensions.
 *
 * @param fd The file descriptor to a event device.
 * @param[out] model_out The type of touchpad model detected.
 *
 * @return TRUE on success or FALSE otherwise.
 */
static Bool
event_query_model(struct libevdev *evdev, enum TouchpadModel *model_out,
                  unsigned short *vendor_id, unsigned short *product_id)
{
    int vendor, product;
    struct model_lookup_t *model_lookup;

    vendor = libevdev_get_id_vendor(evdev);
    product = libevdev_get_id_product(evdev);

    for (model_lookup = model_lookup_table; model_lookup->vendor;
         model_lookup++) {
        if (model_lookup->vendor == vendor &&
            (model_lookup->product_start == PRODUCT_ANY ||
             model_lookup->product_start <= product) &&
            (model_lookup->product_end == PRODUCT_ANY ||
             model_lookup->product_end >= product))
            *model_out = model_lookup->model;
    }

    *vendor_id = vendor;
    *product_id = product;

    return TRUE;
}

/**
 * Get absinfo information from the given file descriptor for the given
 * ABS_FOO code and store the information in min, max, fuzz and res.
 *
 * @param fd File descriptor to an event device
 * @param code Event code (e.g. ABS_X)
 * @param[out] min Minimum axis range
 * @param[out] max Maximum axis range
 * @param[out] fuzz Fuzz of this axis. If NULL, fuzz is ignored.
 * @param[out] res Axis resolution. If NULL or the current kernel does not
 * support the resolution field, res is ignored
 *
 * @return Zero on success, or errno otherwise.
 */
static int
event_get_abs(struct libevdev *evdev, int code,
              int *min, int *max, int *fuzz, int *res)
{
    const struct input_absinfo *abs;

    abs = libevdev_get_abs_info(evdev, code);
    *min = abs->minimum;
    *max = abs->maximum;

    /* We dont trust a zero fuzz as it probably is just a lazy value */
    if (fuzz && abs->fuzz > 0)
        *fuzz = abs->fuzz;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
    if (res)
        *res = abs->resolution;
#endif

    return 0;
}

/* Query device for axis ranges */
static void
event_query_axis_ranges(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    struct eventcomm_proto_data *proto_data = priv->proto_data;
    char buf[256] = { 0 };

    /* The kernel's fuzziness concept seems a bit weird, but it can more or
     * less be applied as hysteresis directly, i.e. no factor here. */
    event_get_abs(proto_data->evdev, ABS_X, &priv->minx, &priv->maxx,
                  &priv->synpara.hyst_x, &priv->resx);

    event_get_abs(proto_data->evdev, ABS_Y, &priv->miny, &priv->maxy,
                  &priv->synpara.hyst_y, &priv->resy);

    priv->has_pressure = libevdev_has_event_code(proto_data->evdev, EV_ABS, ABS_PRESSURE);
    priv->has_width = libevdev_has_event_code(proto_data->evdev, EV_ABS, ABS_TOOL_WIDTH);

    if (priv->has_pressure)
        event_get_abs(proto_data->evdev, ABS_PRESSURE, &priv->minp, &priv->maxp,
                      NULL, NULL);

    if (priv->has_width)
        event_get_abs(proto_data->evdev, ABS_TOOL_WIDTH,
                      &priv->minw, &priv->maxw, NULL, NULL);

    if (priv->has_touch) {
        int st_minx = priv->minx;
        int st_maxx = priv->maxx;
        int st_miny = priv->miny;
        int st_maxy = priv->maxy;

        event_get_abs(proto_data->evdev, ABS_MT_POSITION_X, &priv->minx,
                      &priv->maxx, &priv->synpara.hyst_x, &priv->resx);
        event_get_abs(proto_data->evdev, ABS_MT_POSITION_Y, &priv->miny,
                      &priv->maxy, &priv->synpara.hyst_y, &priv->resy);

        proto_data->st_to_mt_offset[0] = priv->minx - st_minx;
        proto_data->st_to_mt_scale[0] =
            (priv->maxx - priv->minx) / (st_maxx - st_minx);
        proto_data->st_to_mt_offset[1] = priv->miny - st_miny;
        proto_data->st_to_mt_scale[1] =
            (priv->maxy - priv->miny) / (st_maxy - st_miny);
    }

    priv->has_left = libevdev_has_event_code(proto_data->evdev, EV_KEY, BTN_LEFT);
    priv->has_right = libevdev_has_event_code(proto_data->evdev, EV_KEY, BTN_RIGHT);
    priv->has_middle = libevdev_has_event_code(proto_data->evdev, EV_KEY, BTN_MIDDLE);
    priv->has_double = libevdev_has_event_code(proto_data->evdev, EV_KEY, BTN_TOOL_DOUBLETAP);
    priv->has_triple = libevdev_has_event_code(proto_data->evdev, EV_KEY, BTN_TOOL_TRIPLETAP);

    if (libevdev_has_event_code(proto_data->evdev, EV_KEY, BTN_0) ||
        libevdev_has_event_code(proto_data->evdev, EV_KEY, BTN_1) ||
        libevdev_has_event_code(proto_data->evdev, EV_KEY, BTN_2) ||
        libevdev_has_event_code(proto_data->evdev, EV_KEY, BTN_3))
        priv->has_scrollbuttons = 1;

    /* Now print the device information */
    xf86IDrvMsg(pInfo, X_PROBED, "x-axis range %d - %d (res %d)\n",
                priv->minx, priv->maxx, priv->resx);
    xf86IDrvMsg(pInfo, X_PROBED, "y-axis range %d - %d (res %d)\n",
                priv->miny, priv->maxy, priv->resy);
    if (priv->has_pressure)
        xf86IDrvMsg(pInfo, X_PROBED, "pressure range %d - %d\n",
                    priv->minp, priv->maxp);
    else
        xf86IDrvMsg(pInfo, X_INFO,
                    "device does not report pressure, will use touch data.\n");
    if (priv->has_width)
        xf86IDrvMsg(pInfo, X_PROBED, "finger width range %d - %d\n",
                    priv->minw, priv->maxw);
    else
        xf86IDrvMsg(pInfo, X_INFO, "device does not report finger width.\n");

    if (priv->has_left)
        strcat(buf, " left");
    if (priv->has_right)
        strcat(buf, " right");
    if (priv->has_middle)
        strcat(buf, " middle");
    if (priv->has_double)
        strcat(buf, " double");
    if (priv->has_triple)
        strcat(buf, " triple");
    if (priv->has_scrollbuttons)
        strcat(buf, " scroll-buttons");

    xf86IDrvMsg(pInfo, X_PROBED, "buttons:%s\n", buf);
}

static Bool
EventQueryHardware(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    struct eventcomm_proto_data *proto_data = priv->proto_data;

    if (!event_query_is_touchpad(proto_data->evdev,
                                 (proto_data) ? proto_data->need_grab : TRUE))
        return FALSE;

    xf86IDrvMsg(pInfo, X_PROBED, "touchpad found\n");

    return TRUE;
}

static Bool
SynapticsReadEvent(InputInfoPtr pInfo, struct input_event *ev)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    struct eventcomm_proto_data *proto_data = priv->proto_data;
    int rc;
    static struct timeval last_event_time;

    rc = libevdev_next_event(proto_data->evdev, proto_data->read_flag, ev);
    if (rc < 0) {
        if (rc != -EAGAIN) {
            LogMessageVerbSigSafe(X_ERROR, 0, "%s: Read error %d\n", pInfo->name,
                    errno);
        } else if (proto_data->read_flag == LIBEVDEV_READ_FLAG_SYNC) {
            proto_data->read_flag = LIBEVDEV_READ_FLAG_NORMAL;
            return SynapticsReadEvent(pInfo, ev);
        }

        return FALSE;
    }

    /* SYN_DROPPED received in normal mode. Create a normal EV_SYN
       so we process what's in the queue atm, then ensure we sync
       next time */
    if (rc == LIBEVDEV_READ_STATUS_SYNC &&
        proto_data->read_flag == LIBEVDEV_READ_FLAG_NORMAL) {
        proto_data->read_flag = LIBEVDEV_READ_FLAG_SYNC;
        ev->type = EV_SYN;
        ev->code = SYN_REPORT;
        ev->value = 0;
        ev->time = last_event_time;
    } else if (ev->type == EV_SYN)
        last_event_time = ev->time;

    return TRUE;
}

static Bool
EventTouchSlotPreviouslyOpen(SynapticsPrivate * priv, int slot)
{
    int i;

    for (i = 0; i < priv->num_active_touches; i++)
        if (priv->open_slots[i] == slot)
            return TRUE;

    return FALSE;
}

static void
EventProcessTouchEvent(InputInfoPtr pInfo, struct SynapticsHwState *hw,
                       struct input_event *ev)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    struct eventcomm_proto_data *proto_data = priv->proto_data;

    if (!priv->has_touch)
        return;

    if (ev->code == ABS_MT_SLOT) {
        proto_data->cur_slot = ev->value;
    }
    else {
        int slot_index = proto_data->cur_slot;

        if (slot_index < 0)
            return;

        if (hw->slot_state[slot_index] == SLOTSTATE_OPEN_EMPTY)
            hw->slot_state[slot_index] = SLOTSTATE_UPDATE;
        if (ev->code == ABS_MT_TRACKING_ID) {
            if (ev->value >= 0) {
                hw->slot_state[slot_index] = SLOTSTATE_OPEN;
                proto_data->num_touches++;
                valuator_mask_copy(hw->mt_mask[slot_index],
                                   proto_data->last_mt_vals[slot_index]);
            }
            else if (hw->slot_state[slot_index] != SLOTSTATE_EMPTY) {
                hw->slot_state[slot_index] = SLOTSTATE_CLOSE;
                proto_data->num_touches--;
            }
        }
        else {
            ValuatorMask *mask = proto_data->last_mt_vals[slot_index];
            int map = proto_data->axis_map[ev->code - ABS_MT_TOUCH_MAJOR];
            int last_val = valuator_mask_get(mask, map);

            valuator_mask_set(hw->mt_mask[slot_index], map, ev->value);
            if (EventTouchSlotPreviouslyOpen(priv, slot_index)) {
                if (ev->code == ABS_MT_POSITION_X)
                    hw->cumulative_dx += ev->value - last_val;
                else if (ev->code == ABS_MT_POSITION_Y)
                    hw->cumulative_dy += ev->value - last_val;
            }

            valuator_mask_set(mask, map, ev->value);
        }
    }
}

/**
 * Count the number of fingers based on the CommData information.
 * The CommData struct contains the event information based on previous
 * struct input_events, now we're just counting based on that.
 *
 * @param comm Assembled information from previous events.
 * @return The number of fingers currently set.
 */
static int
count_fingers(InputInfoPtr pInfo, const struct CommData *comm)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    struct eventcomm_proto_data *proto_data = priv->proto_data;
    int fingers = 0;

    if (comm->oneFinger)
        fingers = 1;
    else if (comm->twoFingers)
        fingers = 2;
    else if (comm->threeFingers)
        fingers = 3;

    if (priv->has_touch && proto_data->num_touches > fingers)
        fingers = proto_data->num_touches;

    return fingers;
}

static inline double
apply_st_scaling(struct eventcomm_proto_data *proto_data, int value, int axis)
{
    return value * proto_data->st_to_mt_scale[axis] +
        proto_data->st_to_mt_offset[axis];
}

Bool
EventReadHwState(InputInfoPtr pInfo,
                 struct CommData *comm, struct SynapticsHwState *hwRet)
{
    struct input_event ev;
    Bool v;
    struct SynapticsHwState *hw = comm->hwState;
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    SynapticsParameters *para = &priv->synpara;
    struct eventcomm_proto_data *proto_data = priv->proto_data;
    Bool sync_cumulative = FALSE;

    set_libevdev_log_handler();

    SynapticsResetTouchHwState(hw, FALSE);

    /* Reset cumulative values if buttons were not previously pressed,
     * or no finger was previously present. */
    if ((!hw->left && !hw->right && !hw->middle) || hw->z < para->finger_low) {
        hw->cumulative_dx = hw->x;
        hw->cumulative_dy = hw->y;
        sync_cumulative = TRUE;
    }

    while (SynapticsReadEvent(pInfo, &ev)) {
        switch (ev.type) {
        case EV_SYN:
            switch (ev.code) {
            case SYN_REPORT:
                hw->numFingers = count_fingers(pInfo, comm);
                if (proto_data->have_monotonic_clock)
                    hw->millis = 1000 * ev.time.tv_sec + ev.time.tv_usec / 1000;
                else
                    hw->millis = GetTimeInMillis();
                SynapticsCopyHwState(hwRet, hw);
                return TRUE;
            }
            break;
        case EV_KEY:
            v = (ev.value ? TRUE : FALSE);
            switch (ev.code) {
            case BTN_LEFT:
                hw->left = v;
                break;
            case BTN_RIGHT:
                hw->right = v;
                break;
            case BTN_MIDDLE:
                hw->middle = v;
                break;
            case BTN_FORWARD:
                hw->up = v;
                break;
            case BTN_BACK:
                hw->down = v;
                break;
            case BTN_0:
                hw->multi[0] = v;
                break;
            case BTN_1:
                hw->multi[1] = v;
                break;
            case BTN_2:
                hw->multi[2] = v;
                break;
            case BTN_3:
                hw->multi[3] = v;
                break;
            case BTN_4:
                hw->multi[4] = v;
                break;
            case BTN_5:
                hw->multi[5] = v;
                break;
            case BTN_6:
                hw->multi[6] = v;
                break;
            case BTN_7:
                hw->multi[7] = v;
                break;
            case BTN_TOOL_FINGER:
                comm->oneFinger = v;
                break;
            case BTN_TOOL_DOUBLETAP:
                comm->twoFingers = v;
                break;
            case BTN_TOOL_TRIPLETAP:
                comm->threeFingers = v;
                break;
            case BTN_TOUCH:
                if (!priv->has_pressure)
                    hw->z = v ? para->finger_high + 1 : 0;
                break;
            }
            break;
        case EV_ABS:
            if (ev.code < ABS_MT_SLOT) {
                switch (ev.code) {
                case ABS_X:
                    hw->x = apply_st_scaling(proto_data, ev.value, 0);
                    if (sync_cumulative)
                        hw->cumulative_dx = hw->x;
                    break;
                case ABS_Y:
                    hw->y = apply_st_scaling(proto_data, ev.value, 1);
                    if (sync_cumulative)
                        hw->cumulative_dy = hw->y;
                    break;
                case ABS_PRESSURE:
                    hw->z = ev.value;
                    break;
                case ABS_TOOL_WIDTH:
                    hw->fingerWidth = ev.value;
                    break;
                }
            }
            else
                EventProcessTouchEvent(pInfo, hw, &ev);
            break;
        }
    }
    return FALSE;
}

/* filter for the AutoDevProbe scandir on /dev/input */
static int
EventDevOnly(const struct dirent *dir)
{
    return strncmp(EVENT_DEV_NAME, dir->d_name, 5) == 0;
}

static void
event_query_touch(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    SynapticsParameters *para = &priv->synpara;
    struct eventcomm_proto_data *proto_data = priv->proto_data;
    struct libevdev *dev = proto_data->evdev;
    int axis;

    priv->max_touches = 0;
    priv->num_mt_axes = 0;

#ifdef EVIOCGPROP
    if (libevdev_has_property(dev, INPUT_PROP_SEMI_MT)) {
        xf86IDrvMsg(pInfo, X_INFO,
                    "ignoring touch events for semi-multitouch device\n");
        priv->has_semi_mt = TRUE;
    }

    if (libevdev_has_property(dev, INPUT_PROP_BUTTONPAD)) {
        xf86IDrvMsg(pInfo, X_INFO, "found clickpad property\n");
        para->clickpad = TRUE;
    }

    if (libevdev_has_property(dev, INPUT_PROP_TOPBUTTONPAD)) {
        xf86IDrvMsg(pInfo, X_INFO, "found top buttonpad property\n");
        para->has_secondary_buttons = TRUE;
    }
#endif


    if (libevdev_has_event_code(dev, EV_ABS, ABS_MT_SLOT)) {
        for (axis = ABS_MT_SLOT + 1; axis <= ABS_MT_MAX; axis++) {
            if (!libevdev_has_event_code(dev, EV_ABS, axis))
                continue;

            priv->has_touch = TRUE;

            /* X and Y axis info is handled by synaptics already and we don't
               expose the tracking ID */
            if (axis == ABS_MT_POSITION_X ||
                axis == ABS_MT_POSITION_Y ||
                axis == ABS_MT_TRACKING_ID)
                continue;

            priv->num_mt_axes++;
        }
    }

    if (priv->has_touch) {
        int axnum;

        static const char *labels[ABS_MT_MAX] = {
            AXIS_LABEL_PROP_ABS_MT_TOUCH_MAJOR,
            AXIS_LABEL_PROP_ABS_MT_TOUCH_MINOR,
            AXIS_LABEL_PROP_ABS_MT_WIDTH_MAJOR,
            AXIS_LABEL_PROP_ABS_MT_WIDTH_MINOR,
            AXIS_LABEL_PROP_ABS_MT_ORIENTATION,
            AXIS_LABEL_PROP_ABS_MT_POSITION_X,
            AXIS_LABEL_PROP_ABS_MT_POSITION_Y,
            AXIS_LABEL_PROP_ABS_MT_TOOL_TYPE,
            AXIS_LABEL_PROP_ABS_MT_BLOB_ID,
            AXIS_LABEL_PROP_ABS_MT_TRACKING_ID,
            AXIS_LABEL_PROP_ABS_MT_PRESSURE,
            AXIS_LABEL_PROP_ABS_MT_DISTANCE,
            AXIS_LABEL_PROP_ABS_MT_TOOL_X,
            AXIS_LABEL_PROP_ABS_MT_TOOL_Y,
        };

        priv->max_touches = libevdev_get_num_slots(dev);
        priv->touch_axes = malloc(priv->num_mt_axes *
                                  sizeof(SynapticsTouchAxisRec));
        if (!priv->touch_axes) {
            priv->has_touch = FALSE;
            return;
        }

        axnum = 0;
        for (axis = ABS_MT_SLOT + 1; axis <= ABS_MT_MAX; axis++) {
            int axis_idx = axis - ABS_MT_TOUCH_MAJOR;

            if (!libevdev_has_event_code(dev, EV_ABS, axis))
                continue;

            switch (axis) {
                /* X and Y axis info is handled by synaptics already, we just
                 * need to map the evdev codes to the valuator numbers */
                case ABS_MT_POSITION_X:
                    proto_data->axis_map[axis_idx] = 0;
                    break;

                case ABS_MT_POSITION_Y:
                    proto_data->axis_map[axis_idx] = 1;
                    break;

                    /* Skip tracking ID info */
                case ABS_MT_TRACKING_ID:
                    break;

                default:
                    if (axis_idx >= sizeof(labels)/sizeof(labels[0])) {
                        xf86IDrvMsg(pInfo, X_ERROR,
                                    "Axis %d out of label range. This is a bug\n",
                                    axis);
                        priv->touch_axes[axnum].label = NULL;
                    } else
                        priv->touch_axes[axnum].label = labels[axis_idx];
                    priv->touch_axes[axnum].min = libevdev_get_abs_minimum(dev, axis);
                    priv->touch_axes[axnum].max = libevdev_get_abs_maximum(dev, axis);
                    /* Kernel provides units/mm, X wants units/m */
                    priv->touch_axes[axnum].res = libevdev_get_abs_resolution(dev, axis) * 1000;
                    /* Valuators 0-3 are used for X, Y, and scrolling */
                    proto_data->axis_map[axis_idx] = 4 + axnum;
                    axnum++;
                    break;
            }
        }
    }
}

/**
 * Probe the open device for dimensions.
 */
static void
EventReadDevDimensions(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    struct eventcomm_proto_data *proto_data = priv->proto_data;
    int i;

    proto_data = EventProtoDataAlloc(pInfo->fd);
    priv->proto_data = proto_data;

    for (i = 0; i < ABS_MT_CNT; i++)
        proto_data->axis_map[i] = -1;
    proto_data->cur_slot = -1;

    if (event_query_is_touchpad(proto_data->evdev, proto_data->need_grab)) {
        event_query_touch(pInfo);
        event_query_axis_ranges(pInfo);
    }
    event_query_model(proto_data->evdev, &priv->model, &priv->id_vendor,
                      &priv->id_product);

    xf86IDrvMsg(pInfo, X_PROBED, "Vendor %#hx Product %#hx\n",
                priv->id_vendor, priv->id_product);
}

static Bool
EventAutoDevProbe(InputInfoPtr pInfo, const char *device)
{
    /* We are trying to find the right eventX device or fall back to
       the psaux protocol and the given device from XF86Config */
    int i;
    Bool touchpad_found = FALSE;
    struct dirent **namelist;

    if (device) {
        int fd = -1;

        if (pInfo->flags & XI86_SERVER_FD)
            fd = pInfo->fd;
        else
            SYSCALL(fd = open(device, O_RDONLY));

        if (fd >= 0) {
            int rc;
            struct libevdev *evdev;

            rc = libevdev_new_from_fd(fd, &evdev);
            if (rc >= 0) {
                touchpad_found = event_query_is_touchpad(evdev, TRUE);
                libevdev_free(evdev);
            }

            if (!(pInfo->flags & XI86_SERVER_FD))
                SYSCALL(close(fd));

            /* if a device is set and not a touchpad (or already grabbed),
             * we must return FALSE.  Otherwise, we'll add a device that
             * wasn't requested for and repeat
             * f5687a6741a19ef3081e7fd83ac55f6df8bcd5c2. */
            return touchpad_found;
        }
    }

    i = scandir(DEV_INPUT_EVENT, &namelist, EventDevOnly, alphasort);
    if (i < 0) {
        xf86IDrvMsg(pInfo, X_ERROR, "Couldn't open %s\n", DEV_INPUT_EVENT);
        return FALSE;
    }
    else if (i == 0) {
        xf86IDrvMsg(pInfo, X_ERROR,
                    "The /dev/input/event* device nodes seem to be missing\n");
        free(namelist);
        return FALSE;
    }

    while (i--) {
        char fname[64];
        int fd = -1;

        if (!touchpad_found) {
            int rc;
            struct libevdev *evdev;

            sprintf(fname, "%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);
            SYSCALL(fd = open(fname, O_RDONLY));
            if (fd < 0)
                continue;

            rc = libevdev_new_from_fd(fd, &evdev);
            if (rc >= 0) {
                touchpad_found = event_query_is_touchpad(evdev, TRUE);
                libevdev_free(evdev);
                if (touchpad_found) {
                    xf86IDrvMsg(pInfo, X_PROBED, "auto-dev sets device to %s\n",
                                fname);
                    pInfo->options = xf86ReplaceStrOption(pInfo->options,
                                                          "Device",
                                                          fname);
                }
            }
            SYSCALL(close(fd));
        }
        free(namelist[i]);
    }

    free(namelist);

    if (!touchpad_found) {
        xf86IDrvMsg(pInfo, X_ERROR, "no synaptics event device found\n");
        return FALSE;
    }

    return TRUE;
}

struct SynapticsProtocolOperations event_proto_operations = {
    EventDeviceOnHook,
    EventDeviceOffHook,
    EventQueryHardware,
    EventReadHwState,
    EventAutoDevProbe,
    EventReadDevDimensions
};
