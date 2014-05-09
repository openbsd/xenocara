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
#include "synproto.h"
#include "synapticsstr.h"
#include <xf86.h>
#include <mtdev.h>

#ifndef INPUT_PROP_BUTTONPAD
#define INPUT_PROP_BUTTONPAD 0x02
#endif
#ifndef INPUT_PROP_SEMI_MT
#define INPUT_PROP_SEMI_MT 0x03
#endif

#define SYSCALL(call) while (((call) == -1) && (errno == EINTR))

#define LONG_BITS (sizeof(long) * 8)
#define NBITS(x) (((x) + LONG_BITS - 1) / LONG_BITS)
#define OFF(x)   ((x) % LONG_BITS)
#define LONG(x)  ((x) / LONG_BITS)
#define TEST_BIT(bit, array) ((array[LONG(bit)] >> OFF(bit)) & 1)

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
    struct mtdev *mtdev;
    int axis_map[MT_ABS_SIZE];
    int cur_slot;
    ValuatorMask **last_mt_vals;
    int num_touches;
    int *tracking_ids;
};

struct eventcomm_proto_data *
EventProtoDataAlloc(void)
{
    struct eventcomm_proto_data *proto_data;

    proto_data = calloc(1, sizeof(struct eventcomm_proto_data));
    if (!proto_data)
        return NULL;

    proto_data->st_to_mt_scale[0] = 1;
    proto_data->st_to_mt_scale[1] = 1;

    return proto_data;
}

static int
last_mt_vals_slot(const SynapticsPrivate * priv)
{
    struct eventcomm_proto_data *proto_data =
        (struct eventcomm_proto_data *) priv->proto_data;
    int value = proto_data->cur_slot;

    return value < priv->num_slots ? value : -1;
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

    mtdev_close_delete(proto_data->mtdev);
    proto_data->mtdev = NULL;
    proto_data->num_touches = 0;

    free(proto_data->tracking_ids);
    proto_data->tracking_ids = NULL;
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

    proto_data->mtdev = mtdev_new_open(pInfo->fd);
    if (!proto_data->mtdev) {
        xf86IDrvMsg(pInfo, X_WARNING,
                    "failed to create mtdev instance, ignoring touch events\n");
        return;
    }

    proto_data->cur_slot = proto_data->mtdev->caps.slot.value;
    proto_data->num_touches = 0;

    proto_data->last_mt_vals = calloc(priv->num_slots, sizeof(ValuatorMask *));
    if (!proto_data->last_mt_vals) {
        xf86IDrvMsg(pInfo, X_WARNING,
                    "failed to allocate MT last values mask array\n");
        UninitializeTouch(pInfo);
        return;
    }

    proto_data->tracking_ids = calloc(priv->num_slots, sizeof(int));
    if (!proto_data->tracking_ids) {
        xf86IDrvMsg(pInfo, X_WARNING, "failed to allocate tracking ID array\n");
        UninitializeTouch(pInfo);
        return;
    }

    for (i = 0; i < priv->num_slots; i++) {
        int j;

        proto_data->tracking_ids[i] = -1;

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

    if (para->grab_event_device) {
        /* Try to grab the event device so that data don't leak to /dev/input/mice */
        int ret;

        SYSCALL(ret = ioctl(pInfo->fd, EVIOCGRAB, (pointer) 1));
        if (ret < 0) {
            xf86IDrvMsg(pInfo, X_WARNING, "can't grab event device, errno=%d\n",
                        errno);
            return FALSE;
        }
    }

    proto_data->need_grab = FALSE;

    InitializeTouch(pInfo);

    return TRUE;
}

static Bool
EventDeviceOffHook(InputInfoPtr pInfo)
{
    UninitializeTouch(pInfo);
    SYSCALL(ioctl(pInfo->fd, EVIOCGRAB, (pointer) 0));

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
 * @param fd The file descriptor to an event device.
 * @param test_grab If true, test whether an EVIOCGRAB is possible on the
 * device. A failure to grab the event device returns in a failure.
 *
 * @return TRUE if the device is a touchpad or FALSE otherwise.
 */
static Bool
event_query_is_touchpad(int fd, BOOL test_grab)
{
    int ret = FALSE, rc;
    unsigned long evbits[NBITS(EV_MAX)] = { 0 };
    unsigned long absbits[NBITS(ABS_MAX)] = { 0 };
    unsigned long keybits[NBITS(KEY_MAX)] = { 0 };

    if (test_grab) {
        SYSCALL(rc = ioctl(fd, EVIOCGRAB, (pointer) 1));
        if (rc < 0)
            return FALSE;
    }

    /* Check for ABS_X, ABS_Y, ABS_PRESSURE and BTN_TOOL_FINGER */

    SYSCALL(rc = ioctl(fd, EVIOCGBIT(0, sizeof(evbits)), evbits));
    if (rc < 0)
        goto unwind;
    if (!TEST_BIT(EV_SYN, evbits) ||
        !TEST_BIT(EV_ABS, evbits) || !TEST_BIT(EV_KEY, evbits))
        goto unwind;

    SYSCALL(rc = ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absbits)), absbits));
    if (rc < 0)
        goto unwind;
    if (!TEST_BIT(ABS_X, absbits) || !TEST_BIT(ABS_Y, absbits))
        goto unwind;

    SYSCALL(rc = ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybits)), keybits));
    if (rc < 0)
        goto unwind;

    /* we expect touchpad either report raw pressure or touches */
    if (!TEST_BIT(ABS_PRESSURE, absbits) && !TEST_BIT(BTN_TOUCH, keybits))
        goto unwind;
    /* all Synaptics-like touchpad report BTN_TOOL_FINGER */
    if (!TEST_BIT(BTN_TOOL_FINGER, keybits))
        goto unwind;
    if (TEST_BIT(BTN_TOOL_PEN, keybits))
        goto unwind;            /* Don't match wacom tablets */

    ret = TRUE;

 unwind:
    if (test_grab)
        SYSCALL(ioctl(fd, EVIOCGRAB, (pointer) 0));

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
    {0x05ac, 0x223, PRODUCT_ANY, MODEL_UNIBODY_MACBOOK},
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
event_query_model(int fd, enum TouchpadModel *model_out,
                  unsigned short *vendor_id, unsigned short *product_id)
{
    struct input_id id;
    int rc;
    struct model_lookup_t *model_lookup;

    SYSCALL(rc = ioctl(fd, EVIOCGID, &id));
    if (rc < 0)
        return FALSE;

    for (model_lookup = model_lookup_table; model_lookup->vendor;
         model_lookup++) {
        if (model_lookup->vendor == id.vendor &&
            (model_lookup->product_start == PRODUCT_ANY ||
             model_lookup->product_start <= id.product) &&
            (model_lookup->product_end == PRODUCT_ANY ||
             model_lookup->product_end >= id.product))
            *model_out = model_lookup->model;
    }

    *vendor_id = id.vendor;
    *product_id = id.product;

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
event_get_abs(InputInfoPtr pInfo, int fd, int code,
              int *min, int *max, int *fuzz, int *res)
{
    int rc;
    struct input_absinfo abs = { 0 };

    SYSCALL(rc = ioctl(fd, EVIOCGABS(code), &abs));
    if (rc < 0) {
        xf86IDrvMsg(pInfo, X_ERROR, "%s EVIOCGABS error on %d (%s)\n",
                    __func__, code, strerror(errno));
        return errno;
    }

    *min = abs.minimum;
    *max = abs.maximum;
    /* We dont trust a zero fuzz as it probably is just a lazy value */
    if (fuzz && abs.fuzz > 0)
        *fuzz = abs.fuzz;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
    if (res)
        *res = abs.resolution;
#endif

    return 0;
}

/* Query device for axis ranges */
static void
event_query_axis_ranges(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    struct eventcomm_proto_data *proto_data = priv->proto_data;
    unsigned long absbits[NBITS(ABS_MAX)] = { 0 };
    unsigned long keybits[NBITS(KEY_MAX)] = { 0 };
    char buf[256] = { 0 };
    int rc;

    /* The kernel's fuzziness concept seems a bit weird, but it can more or
     * less be applied as hysteresis directly, i.e. no factor here. */
    event_get_abs(pInfo, pInfo->fd, ABS_X, &priv->minx, &priv->maxx,
                  &priv->synpara.hyst_x, &priv->resx);

    event_get_abs(pInfo, pInfo->fd, ABS_Y, &priv->miny, &priv->maxy,
                  &priv->synpara.hyst_y, &priv->resy);

    priv->has_pressure = FALSE;
    priv->has_width = FALSE;
    SYSCALL(rc = ioctl(pInfo->fd, EVIOCGBIT(EV_ABS, sizeof(absbits)), absbits));
    if (rc >= 0) {
        priv->has_pressure = (TEST_BIT(ABS_PRESSURE, absbits) != 0);
        priv->has_width = (TEST_BIT(ABS_TOOL_WIDTH, absbits) != 0);
    }
    else
        xf86IDrvMsg(pInfo, X_ERROR, "failed to query ABS bits (%s)\n",
                    strerror(errno));

    if (priv->has_pressure)
        event_get_abs(pInfo, pInfo->fd, ABS_PRESSURE, &priv->minp, &priv->maxp,
                      NULL, NULL);

    if (priv->has_width)
        event_get_abs(pInfo, pInfo->fd, ABS_TOOL_WIDTH,
                      &priv->minw, &priv->maxw, NULL, NULL);

    if (priv->has_touch) {
        int st_minx = priv->minx;
        int st_maxx = priv->maxx;
        int st_miny = priv->miny;
        int st_maxy = priv->maxy;

        event_get_abs(pInfo, pInfo->fd, ABS_MT_POSITION_X, &priv->minx,
                      &priv->maxx, &priv->synpara.hyst_x, &priv->resx);
        event_get_abs(pInfo, pInfo->fd, ABS_MT_POSITION_Y, &priv->miny,
                      &priv->maxy, &priv->synpara.hyst_y, &priv->resy);

        proto_data->st_to_mt_offset[0] = priv->minx - st_minx;
        proto_data->st_to_mt_scale[0] =
            (priv->maxx - priv->minx) / (st_maxx - st_minx);
        proto_data->st_to_mt_offset[1] = priv->miny - st_miny;
        proto_data->st_to_mt_scale[1] =
            (priv->maxy - priv->miny) / (st_maxy - st_miny);
    }

    SYSCALL(rc = ioctl(pInfo->fd, EVIOCGBIT(EV_KEY, sizeof(keybits)), keybits));
    if (rc >= 0) {
        priv->has_left = (TEST_BIT(BTN_LEFT, keybits) != 0);
        priv->has_right = (TEST_BIT(BTN_RIGHT, keybits) != 0);
        priv->has_middle = (TEST_BIT(BTN_MIDDLE, keybits) != 0);
        priv->has_double = (TEST_BIT(BTN_TOOL_DOUBLETAP, keybits) != 0);
        priv->has_triple = (TEST_BIT(BTN_TOOL_TRIPLETAP, keybits) != 0);

        if ((TEST_BIT(BTN_0, keybits) != 0) ||
            (TEST_BIT(BTN_1, keybits) != 0) ||
            (TEST_BIT(BTN_2, keybits) != 0) || (TEST_BIT(BTN_3, keybits) != 0))
            priv->has_scrollbuttons = 1;
    }

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

    if (!event_query_is_touchpad
        (pInfo->fd, (proto_data) ? proto_data->need_grab : TRUE))
        return FALSE;

    xf86IDrvMsg(pInfo, X_PROBED, "touchpad found\n");

    return TRUE;
}

static Bool
SynapticsReadEvent(InputInfoPtr pInfo, struct input_event *ev)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    struct eventcomm_proto_data *proto_data = priv->proto_data;
    int rc = TRUE;
    ssize_t len;

    if (proto_data->mtdev)
        len = mtdev_get(proto_data->mtdev, pInfo->fd, ev, 1) *
            sizeof(struct input_event);
    else
        len = read(pInfo->fd, ev, sizeof(*ev));
    if (len <= 0) {
        /* We use X_NONE here because it doesn't alloc */
        if (errno != EAGAIN)
            LogMessageVerbSigSafe(X_ERROR, 0, "%s: Read error %d\n", pInfo->name,
                                  errno);
        rc = FALSE;
    }
    else if (len % sizeof(*ev)) {
        LogMessageVerbSigSafe(X_ERROR, 0, "%s: Read error, invalid number of bytes.",
                              pInfo->name);
        rc = FALSE;
    }
    return rc;
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
        int slot_index = last_mt_vals_slot(priv);

        if (slot_index < 0)
            return;

        if (hw->slot_state[slot_index] == SLOTSTATE_OPEN_EMPTY)
            hw->slot_state[slot_index] = SLOTSTATE_UPDATE;
        if (ev->code == ABS_MT_TRACKING_ID) {
            int old_tracking_id = proto_data->tracking_ids[slot_index];

            /* We don't have proper SYN_DROPPED handling in
               synaptics < 1.8. This is a poor man's version that covers the
               worst bug we're seeing: touch points starting/stopping during
               SYN_DROPPED. There can only be one touchpoint per slot,
               identified by the tracking ID. Make sure that we only ever
               have a single touch point open per slot.
             */
            if (ev->value != -1 && old_tracking_id != -1) {
                /* Our touch terminated during SYN_DROPPED, now we have a
                   new touch starting in the same slot but ours is still
                   open. Do nothing, just continue with the old touch */
            } else if (ev->value == -1 && old_tracking_id == -1) {
                /* A new touch started during SYN_DROPPED, now we have that
                   touch terminating. Do nothing, we don't have that touch
                   open */
            } else if (ev->value >= 0) {
                hw->slot_state[slot_index] = SLOTSTATE_OPEN;
                proto_data->num_touches++;
                valuator_mask_copy(hw->mt_mask[slot_index],
                                   proto_data->last_mt_vals[slot_index]);
            }
            else if (hw->slot_state[slot_index] != SLOTSTATE_EMPTY) {
                hw->slot_state[slot_index] = SLOTSTATE_CLOSE;
                proto_data->num_touches--;
            }

            proto_data->tracking_ids[slot_index] = ev->value;
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

    SynapticsResetTouchHwState(hw, FALSE);

    /* Reset cumulative values if buttons were not previously pressed */
    if (!hw->left && !hw->right && !hw->middle) {
        hw->cumulative_dx = hw->x;
        hw->cumulative_dy = hw->y;
    }

    while (SynapticsReadEvent(pInfo, &ev)) {
        switch (ev.type) {
        case EV_SYN:
            switch (ev.code) {
            case SYN_REPORT:
                hw->numFingers = count_fingers(pInfo, comm);
                hw->millis = 1000 * ev.time.tv_sec + ev.time.tv_usec / 1000;
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
                    break;
                case ABS_Y:
                    hw->y = apply_st_scaling(proto_data, ev.value, 1);
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
    struct mtdev *mtdev;
    int i;
    int rc;
    uint8_t prop;

    priv->max_touches = 0;
    priv->num_mt_axes = 0;

#ifdef EVIOCGPROP
    SYSCALL(rc = ioctl(pInfo->fd, EVIOCGPROP(sizeof(prop)), &prop));
    if (rc >= 0 && BitIsOn(&prop, INPUT_PROP_SEMI_MT)) {
        xf86IDrvMsg(pInfo, X_INFO,
                    "ignoring touch events for semi-multitouch device\n");
        priv->has_semi_mt = TRUE;
    }

    if (rc >= 0 && BitIsOn(&prop, INPUT_PROP_BUTTONPAD)) {
        xf86IDrvMsg(pInfo, X_INFO, "found clickpad property\n");
        para->clickpad = TRUE;
    }
#endif

    mtdev = mtdev_new_open(pInfo->fd);
    if (!mtdev) {
        xf86IDrvMsg(pInfo, X_WARNING,
                    "failed to open mtdev when querying touch capabilities\n");
        return;
    }

    for (i = 0; i < MT_ABS_SIZE; i++) {
        if (mtdev->caps.has_abs[i]) {
            switch (i) {
                /* X and Y axis info is handed by synaptics already */
            case ABS_MT_POSITION_X - ABS_MT_TOUCH_MAJOR:
            case ABS_MT_POSITION_Y - ABS_MT_TOUCH_MAJOR:
                /* Skip tracking ID info */
            case ABS_MT_TRACKING_ID - ABS_MT_TOUCH_MAJOR:
                break;
            default:
                priv->num_mt_axes++;
                break;
            }
            priv->has_touch = TRUE;
        }
    }

    if (priv->has_touch) {
        int axnum;

        static const char *labels[] = {
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
        };

        if (mtdev->caps.slot.maximum > 0)
            priv->max_touches = mtdev->caps.slot.maximum + 1;

        priv->touch_axes = malloc(priv->num_mt_axes *
                                  sizeof(SynapticsTouchAxisRec));
        if (!priv->touch_axes) {
            priv->has_touch = FALSE;
            goto out;
        }

        axnum = 0;
        for (i = 0; i < MT_ABS_SIZE; i++) {
            if (mtdev->caps.has_abs[i]) {
                switch (i) {
                    /* X and Y axis info is handed by synaptics already, we just
                     * need to map the evdev codes to the valuator numbers */
                case ABS_MT_POSITION_X - ABS_MT_TOUCH_MAJOR:
                    proto_data->axis_map[i] = 0;
                    break;

                case ABS_MT_POSITION_Y - ABS_MT_TOUCH_MAJOR:
                    proto_data->axis_map[i] = 1;
                    break;

                    /* Skip tracking ID info */
                case ABS_MT_TRACKING_ID - ABS_MT_TOUCH_MAJOR:
                    break;

                default:
                    priv->touch_axes[axnum].label = labels[i];
                    priv->touch_axes[axnum].min = mtdev->caps.abs[i].minimum;
                    priv->touch_axes[axnum].max = mtdev->caps.abs[i].maximum;
                    /* Kernel provides units/mm, X wants units/m */
                    priv->touch_axes[axnum].res =
                        mtdev->caps.abs[i].resolution * 1000;
                    /* Valuators 0-3 are used for X, Y, and scrolling */
                    proto_data->axis_map[i] = 4 + axnum;
                    axnum++;
                    break;
                }
            }
        }
    }

 out:
    mtdev_close_delete(mtdev);
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

    proto_data = EventProtoDataAlloc();
    priv->proto_data = proto_data;

    for (i = 0; i < MT_ABS_SIZE; i++)
        proto_data->axis_map[i] = -1;
    proto_data->cur_slot = -1;

    if (event_query_is_touchpad(pInfo->fd, proto_data->need_grab)) {
        event_query_touch(pInfo);
        event_query_axis_ranges(pInfo);
    }
    event_query_model(pInfo->fd, &priv->model, &priv->id_vendor,
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

        SYSCALL(fd = open(device, O_RDONLY));
        if (fd >= 0) {
            touchpad_found = event_query_is_touchpad(fd, TRUE);

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
            sprintf(fname, "%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);
            SYSCALL(fd = open(fname, O_RDONLY));
            if (fd < 0)
                continue;

            if (event_query_is_touchpad(fd, TRUE)) {
                touchpad_found = TRUE;
                xf86IDrvMsg(pInfo, X_PROBED, "auto-dev sets device to %s\n",
                            fname);
                pInfo->options =
                    xf86ReplaceStrOption(pInfo->options, "Device", fname);
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
