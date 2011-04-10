/*
 * Copyright © 2004-2007 Peter Osterlund
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
#include "eventcomm.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include "synproto.h"
#include "synaptics.h"
#include "synapticsstr.h"
#include <xf86.h>


#define SYSCALL(call) while (((call) == -1) && (errno == EINTR))

#define LONG_BITS (sizeof(long) * 8)
#define NBITS(x) (((x) + LONG_BITS - 1) / LONG_BITS)

/**
 * Protocol-specific data.
 */
struct eventcomm_proto_data
{
    /**
     * Do we need to grab the event device?
     * Note that in the current flow, this variable is always false and
     * exists for readability of the code.
     */
    BOOL need_grab;
};

static void
EventDeviceOnHook(InputInfoPtr pInfo, SynapticsParameters *para)
{
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    struct eventcomm_proto_data *proto_data = (struct eventcomm_proto_data*)priv->proto_data;

    if (!proto_data) {
	proto_data = calloc(1, sizeof(struct eventcomm_proto_data));
	priv->proto_data = proto_data;
    }

    if (para->grab_event_device) {
	/* Try to grab the event device so that data don't leak to /dev/input/mice */
	int ret;
	SYSCALL(ret = ioctl(pInfo->fd, EVIOCGRAB, (pointer)1));
	if (ret < 0) {
	    xf86Msg(X_WARNING, "%s can't grab event device, errno=%d\n",
		    pInfo->name, errno);
	}
    }

    proto_data->need_grab = FALSE;
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
    unsigned long evbits[NBITS(EV_MAX)] = {0};
    unsigned long absbits[NBITS(ABS_MAX)] = {0};
    unsigned long keybits[NBITS(KEY_MAX)] = {0};

    if (test_grab)
    {
        SYSCALL(rc = ioctl(fd, EVIOCGRAB, (pointer)1));
        if (rc < 0)
            return FALSE;
    }

    /* Check for ABS_X, ABS_Y, ABS_PRESSURE and BTN_TOOL_FINGER */

    SYSCALL(rc = ioctl(fd, EVIOCGBIT(0, sizeof(evbits)), evbits));
    if (rc < 0)
	goto unwind;
    if (!BitIsOn(evbits, EV_SYN) ||
	!BitIsOn(evbits, EV_ABS) ||
	!BitIsOn(evbits, EV_KEY))
	goto unwind;

    SYSCALL(rc = ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absbits)), absbits));
    if (rc < 0)
	goto unwind;
    if (!BitIsOn(absbits, ABS_X) ||
	!BitIsOn(absbits, ABS_Y))
	goto unwind;

    SYSCALL(rc = ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybits)), keybits));
    if (rc < 0)
	goto unwind;

    /* we expect touchpad either report raw pressure or touches */
    if (!BitIsOn(absbits, ABS_PRESSURE) && !BitIsOn(keybits, BTN_TOUCH))
	goto unwind;
    /* all Synaptics-like touchpad report BTN_TOOL_FINGER */
    if (!BitIsOn(keybits, BTN_TOOL_FINGER))
	goto unwind;
    if (BitIsOn(keybits, BTN_TOOL_PEN))
	goto unwind;			    /* Don't match wacom tablets */

    ret = TRUE;

unwind:
    if (test_grab)
        SYSCALL(ioctl(fd, EVIOCGRAB, (pointer)0));

    return (ret == TRUE);
}

typedef struct {
	short vendor;
	short product;
	enum TouchpadModel model;
} model_lookup_t;
#define PRODUCT_ANY 0x0000

static model_lookup_t model_lookup_table[] = {
	{0x0002, 0x0007, MODEL_SYNAPTICS},
	{0x0002, 0x0008, MODEL_ALPS},
	{0x05ac, PRODUCT_ANY, MODEL_APPLETOUCH},
	{0x0, 0x0, 0x0}
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
event_query_model(int fd, enum TouchpadModel *model_out)
{
    short id[4];
    int rc;
    model_lookup_t *model_lookup;

    SYSCALL(rc = ioctl(fd, EVIOCGID, id));
    if (rc < 0)
        return FALSE;

    for(model_lookup = model_lookup_table; model_lookup->vendor; model_lookup++) {
        if(model_lookup->vendor == id[ID_VENDOR] &&
           (model_lookup->product == id[ID_PRODUCT] || model_lookup->product == PRODUCT_ANY))
            *model_out = model_lookup->model;
    }

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
event_get_abs(int fd, int code, int *min, int *max, int *fuzz, int *res)
{
    int rc;
    struct input_absinfo abs =  {0};

    SYSCALL(rc = ioctl(fd, EVIOCGABS(code), &abs));
    if (rc < 0) {
	xf86Msg(X_ERROR, "%s: EVIOCGABS error on %d (%s)\n",
		__func__, code, strerror(rc));
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
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    struct input_absinfo abs = {0};
    unsigned long absbits[NBITS(ABS_MAX)] = {0};
    unsigned long keybits[NBITS(KEY_MAX)] = {0};
    char buf[256] = {0};
    int rc;

    /* The kernel's fuzziness concept seems a bit weird, but it can more or
     * less be applied as hysteresis directly, i.e. no factor here. */
    event_get_abs(pInfo->fd, ABS_X, &priv->minx, &priv->maxx,
		  &priv->synpara.hyst_x, &priv->resx);

    event_get_abs(pInfo->fd, ABS_Y, &priv->miny, &priv->maxy,
		  &priv->synpara.hyst_y, &priv->resy);

    priv->has_pressure = FALSE;
    priv->has_width = FALSE;
    SYSCALL(rc = ioctl(pInfo->fd, EVIOCGBIT(EV_ABS, sizeof(absbits)), absbits));
    if (rc >= 0)
    {
	priv->has_pressure = (BitIsOn(absbits, ABS_PRESSURE) != 0);
	priv->has_width = (BitIsOn(absbits, ABS_TOOL_WIDTH) != 0);
    }
    else
	xf86Msg(X_ERROR, "%s: failed to query ABS bits (%s)\n", pInfo->name,
		strerror(errno));

    if (priv->has_pressure)
	event_get_abs(pInfo->fd, ABS_PRESSURE, &priv->minp, &priv->maxp,
		      NULL, NULL);

    if (priv->has_width)
	event_get_abs(pInfo->fd, ABS_TOOL_WIDTH,
		      &priv->minw, &priv->maxw,
		      NULL, NULL);

    SYSCALL(rc = ioctl(pInfo->fd, EVIOCGBIT(EV_KEY, sizeof(keybits)), keybits));
    if (rc >= 0)
    {
	priv->has_left = (BitIsOn(keybits, BTN_LEFT) != 0);
	priv->has_right = (BitIsOn(keybits, BTN_RIGHT) != 0);
	priv->has_middle = (BitIsOn(keybits, BTN_MIDDLE) != 0);
	priv->has_double = (BitIsOn(keybits, BTN_TOOL_DOUBLETAP) != 0);
	priv->has_triple = (BitIsOn(keybits, BTN_TOOL_TRIPLETAP) != 0);

	if ((BitIsOn(keybits, BTN_0) != 0) ||
	    (BitIsOn(keybits, BTN_1) != 0) ||
	    (BitIsOn(keybits, BTN_2) != 0) ||
	    (BitIsOn(keybits, BTN_3) != 0))
	    priv->has_scrollbuttons = 1;
    }

    /* Now print the device information */
    xf86Msg(X_PROBED, "%s: x-axis range %d - %d\n", pInfo->name,
	    priv->minx, priv->maxx);
    xf86Msg(X_PROBED, "%s: y-axis range %d - %d\n", pInfo->name,
	    priv->miny, priv->maxy);
    if (priv->has_pressure)
	xf86Msg(X_PROBED, "%s: pressure range %d - %d\n", pInfo->name,
		priv->minp, priv->maxp);
    else
	xf86Msg(X_INFO,
		"%s: device does not report pressure, will use touch data.\n",
		pInfo->name);
    if (priv->has_width)
	xf86Msg(X_PROBED, "%s: finger width range %d - %d\n", pInfo->name,
		abs.minimum, abs.maximum);
    else
	xf86Msg(X_INFO,
		"%s: device does not report finger width.\n", pInfo->name);

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

    xf86Msg(X_PROBED, "%s: buttons:%s\n", pInfo->name, buf);
}

static Bool
EventQueryHardware(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    struct eventcomm_proto_data *proto_data = priv->proto_data;

    if (!event_query_is_touchpad(pInfo->fd, (proto_data) ? proto_data->need_grab : TRUE))
	return FALSE;

    xf86Msg(X_PROBED, "%s: touchpad found\n", pInfo->name);

    return TRUE;
}

static Bool
SynapticsReadEvent(InputInfoPtr pInfo, struct input_event *ev)
{
    int rc = TRUE;
    ssize_t len;

    len = read(pInfo->fd, ev, sizeof(*ev));
    if (len <= 0)
    {
        /* We use X_NONE here because it doesn't alloc */
        if (errno != EAGAIN)
            xf86MsgVerb(X_NONE, 0, "%s: Read error %s\n", pInfo->name, strerror(errno));
        rc = FALSE;
    } else if (len % sizeof(*ev)) {
        xf86MsgVerb(X_NONE, 0, "%s: Read error, invalid number of bytes.", pInfo->name);
        rc = FALSE;
    }
    return rc;
}

/**
 * Count the number of fingers based on the CommData information.
 * The CommData struct contains the event information based on previous
 * struct input_events, now we're just counting based on that.
 *
 * @param comm Assembled information from previous events.
 * @return The number of fingers currently set.
 */
static int count_fingers(const struct CommData *comm)
{
    int fingers = 0;

    if (comm->oneFinger)
	fingers = 1;
    else if (comm->twoFingers)
	fingers = 2;
    else if (comm->threeFingers)
	fingers = 3;

    return fingers;
}


Bool
EventReadHwState(InputInfoPtr pInfo,
		 struct CommData *comm, struct SynapticsHwState *hwRet)
{
    struct input_event ev;
    Bool v;
    struct SynapticsHwState *hw = &(comm->hwState);
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    SynapticsParameters *para = &priv->synpara;

    while (SynapticsReadEvent(pInfo, &ev)) {
	switch (ev.type) {
	case EV_SYN:
	    switch (ev.code) {
	    case SYN_REPORT:
		hw->numFingers = count_fingers(comm);
		*hwRet = *hw;
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
	    switch (ev.code) {
	    case ABS_X:
		hw->x = ev.value;
		break;
	    case ABS_Y:
		hw->y = ev.value;
		break;
	    case ABS_PRESSURE:
		hw->z = ev.value;
		break;
	    case ABS_TOOL_WIDTH:
		hw->fingerWidth = ev.value;
		break;
	    }
	    break;
	}
    }
    return FALSE;
}

/* filter for the AutoDevProbe scandir on /dev/input */
static int EventDevOnly(const struct dirent *dir) {
	return strncmp(EVENT_DEV_NAME, dir->d_name, 5) == 0;
}

/**
 * Probe the open device for dimensions.
 */
static void
EventReadDevDimensions(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    struct eventcomm_proto_data *proto_data = priv->proto_data;

    if (event_query_is_touchpad(pInfo->fd, (proto_data) ? proto_data->need_grab : TRUE))
	event_query_axis_ranges(pInfo);
    event_query_model(pInfo->fd, &priv->model);
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
	if (fd >= 0)
	{
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
		xf86Msg(X_ERROR, "Couldn't open %s\n", DEV_INPUT_EVENT);
		return FALSE;
    }
    else if (i == 0) {
		xf86Msg(X_ERROR, "%s The /dev/input/event* device nodes seem to be missing\n",
				pInfo->name);
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
			    xf86Msg(X_PROBED, "%s auto-dev sets device to %s\n",
				    pInfo->name, fname);
			    pInfo->options =
			    	xf86ReplaceStrOption(pInfo->options, "Device", fname);
			}
			SYSCALL(close(fd));
		}
		free(namelist[i]);
    }

    free(namelist);

    if (!touchpad_found) {
	xf86Msg(X_ERROR, "%s no synaptics event device found\n", pInfo->name);
	return FALSE;
    }

    return TRUE;
}

struct SynapticsProtocolOperations event_proto_operations = {
    EventDeviceOnHook,
    NULL,
    EventQueryHardware,
    EventReadHwState,
    EventAutoDevProbe,
    EventReadDevDimensions
};
