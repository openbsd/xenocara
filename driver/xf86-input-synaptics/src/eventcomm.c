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
#define OFF(x)   ((x) % LONG_BITS)
#define LONG(x)  ((x) / LONG_BITS)
#define TEST_BIT(bit, array) ((array[LONG(bit)] >> OFF(bit)) & 1)

/*****************************************************************************
 *	Function Definitions
 ****************************************************************************/

static void
EventDeviceOnHook(InputInfoPtr pInfo, SynapticsParameters *para)
{
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    BOOL *need_grab;

    if (!priv->proto_data)
        priv->proto_data = calloc(1, sizeof(BOOL));

    need_grab = (BOOL*)priv->proto_data;

    if (para->grab_event_device) {
	/* Try to grab the event device so that data don't leak to /dev/input/mice */
	int ret;
	SYSCALL(ret = ioctl(pInfo->fd, EVIOCGRAB, (pointer)1));
	if (ret < 0) {
	    xf86Msg(X_WARNING, "%s can't grab event device, errno=%d\n",
		    pInfo->name, errno);
	}
    }

    *need_grab = FALSE;
}

static Bool
event_query_is_touchpad(int fd, BOOL grab)
{
    int ret = FALSE, rc;
    unsigned long evbits[NBITS(EV_MAX)] = {0};
    unsigned long absbits[NBITS(ABS_MAX)] = {0};
    unsigned long keybits[NBITS(KEY_MAX)] = {0};

    if (grab)
    {
        SYSCALL(rc = ioctl(fd, EVIOCGRAB, (pointer)1));
        if (rc < 0)
            return FALSE;
    }

    /* Check for ABS_X, ABS_Y, ABS_PRESSURE and BTN_TOOL_FINGER */

    SYSCALL(rc = ioctl(fd, EVIOCGBIT(0, sizeof(evbits)), evbits));
    if (rc < 0)
	goto unwind;
    if (!TEST_BIT(EV_SYN, evbits) ||
	!TEST_BIT(EV_ABS, evbits) ||
	!TEST_BIT(EV_KEY, evbits))
	goto unwind;

    SYSCALL(rc = ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absbits)), absbits));
    if (rc < 0)
	goto unwind;
    if (!TEST_BIT(ABS_X, absbits) ||
	!TEST_BIT(ABS_Y, absbits))
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
	goto unwind;			    /* Don't match wacom tablets */

    ret = TRUE;

unwind:
    if (grab)
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

static void
event_query_info(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    short id[4];
    int rc;
    model_lookup_t *model_lookup;

    SYSCALL(rc = ioctl(pInfo->fd, EVIOCGID, id));
    if (rc < 0)
        return;

    for(model_lookup = model_lookup_table; model_lookup->vendor; model_lookup++) {
        if(model_lookup->vendor == id[ID_VENDOR] &&
           (model_lookup->product == id[ID_PRODUCT] || model_lookup->product == PRODUCT_ANY))
            priv->model = model_lookup->model;
    }
}

/* Query device for axis ranges */
static void
event_query_axis_ranges(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    struct input_absinfo abs = {0};
    unsigned long absbits[NBITS(ABS_MAX)] = {0};
    unsigned long keybits[NBITS(KEY_MAX)] = {0};
    char buf[256];
    int rc;

    SYSCALL(rc = ioctl(pInfo->fd, EVIOCGABS(ABS_X), &abs));
    if (rc >= 0)
    {
	xf86Msg(X_PROBED, "%s: x-axis range %d - %d\n", pInfo->name,
		abs.minimum, abs.maximum);
	priv->minx = abs.minimum;
	priv->maxx = abs.maximum;
	/* The kernel's fuzziness concept seems a bit weird, but it can more or
	 * less be applied as hysteresis directly, i.e. no factor here. Though,
	 * we don't trust a zero fuzz as it probably is just a lazy value. */
	if (abs.fuzz > 0)
	    priv->synpara.hyst_x = abs.fuzz;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	priv->resx = abs.resolution;
#endif
    } else
	xf86Msg(X_ERROR, "%s: failed to query axis range (%s)\n", pInfo->name,
		strerror(errno));

    SYSCALL(rc = ioctl(pInfo->fd, EVIOCGABS(ABS_Y), &abs));
    if (rc >= 0)
    {
	xf86Msg(X_PROBED, "%s: y-axis range %d - %d\n", pInfo->name,
		abs.minimum, abs.maximum);
	priv->miny = abs.minimum;
	priv->maxy = abs.maximum;
	/* don't trust a zero fuzz */
	if (abs.fuzz > 0)
	    priv->synpara.hyst_y = abs.fuzz;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	priv->resy = abs.resolution;
#endif
    } else
	xf86Msg(X_ERROR, "%s: failed to query axis range (%s)\n", pInfo->name,
		strerror(errno));

    priv->has_pressure = FALSE;
    priv->has_width = FALSE;
    SYSCALL(rc = ioctl(pInfo->fd, EVIOCGBIT(EV_ABS, sizeof(absbits)), absbits));
    if (rc >= 0)
    {
	priv->has_pressure = (TEST_BIT(ABS_PRESSURE, absbits) != 0);
	priv->has_width = (TEST_BIT(ABS_TOOL_WIDTH, absbits) != 0);
    }
    else
	xf86Msg(X_ERROR, "%s: failed to query ABS bits (%s)\n", pInfo->name,
		strerror(errno));

    if (priv->has_pressure)
    {
	SYSCALL(rc = ioctl(pInfo->fd, EVIOCGABS(ABS_PRESSURE), &abs));
	if (rc >= 0)
	{
	    xf86Msg(X_PROBED, "%s: pressure range %d - %d\n", pInfo->name,
		    abs.minimum, abs.maximum);
	    priv->minp = abs.minimum;
	    priv->maxp = abs.maximum;
	}
    } else
	xf86Msg(X_INFO,
		"%s: device does not report pressure, will use touch data.\n",
		pInfo->name);

    if (priv->has_width)
    {
	SYSCALL(rc = ioctl(pInfo->fd, EVIOCGABS(ABS_TOOL_WIDTH), &abs));
	if (rc >= 0)
	{
	    xf86Msg(X_PROBED, "%s: finger width range %d - %d\n", pInfo->name,
		    abs.minimum, abs.maximum);
	    priv->minw = abs.minimum;
	    priv->maxw = abs.maximum;
	}
    }

    SYSCALL(rc = ioctl(pInfo->fd, EVIOCGBIT(EV_KEY, sizeof(keybits)), keybits));
    if (rc >= 0)
    {
	buf[0] = 0;
	if ((priv->has_left = (TEST_BIT(BTN_LEFT, keybits) != 0)))
	   strcat(buf, " left");
	if ((priv->has_right = (TEST_BIT(BTN_RIGHT, keybits) != 0)))
	   strcat(buf, " right");
	if ((priv->has_middle = (TEST_BIT(BTN_MIDDLE, keybits) != 0)))
	   strcat(buf, " middle");
	if ((priv->has_double = (TEST_BIT(BTN_TOOL_DOUBLETAP, keybits) != 0)))
	   strcat(buf, " double");
	if ((priv->has_triple = (TEST_BIT(BTN_TOOL_TRIPLETAP, keybits) != 0)))
	   strcat(buf, " triple");

	if ((TEST_BIT(BTN_0, keybits) != 0) ||
	    (TEST_BIT(BTN_1, keybits) != 0) ||
	    (TEST_BIT(BTN_2, keybits) != 0) ||
	    (TEST_BIT(BTN_3, keybits) != 0))
	{
	    priv->has_scrollbuttons = 1;
	    strcat(buf, " scroll-buttons");
	}

	xf86Msg(X_PROBED, "%s: buttons:%s\n", pInfo->name, buf);
    }
}

static Bool
EventQueryHardware(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;
    BOOL *need_grab = (BOOL*)priv->proto_data;

    if (!event_query_is_touchpad(pInfo->fd, (need_grab) ? *need_grab : TRUE))
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

static Bool
EventReadHwState(InputInfoPtr pInfo,
		 struct SynapticsProtocolOperations *proto_ops,
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
		if (comm->oneFinger)
		    hw->numFingers = 1;
		else if (comm->twoFingers)
		    hw->numFingers = 2;
		else if (comm->threeFingers)
		    hw->numFingers = 3;
		else
		    hw->numFingers = 0;
		*hwRet = *hw;
		return TRUE;
	    }
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
    BOOL *need_grab = (BOOL*)priv->proto_data;

    if (event_query_is_touchpad(pInfo->fd, (need_grab) ? *need_grab : TRUE))
	event_query_axis_ranges(pInfo);
    event_query_info(pInfo);
}

static Bool
EventAutoDevProbe(InputInfoPtr pInfo)
{
    /* We are trying to find the right eventX device or fall back to
       the psaux protocol and the given device from XF86Config */
    int i;
    Bool touchpad_found = FALSE;
    struct dirent **namelist;

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
