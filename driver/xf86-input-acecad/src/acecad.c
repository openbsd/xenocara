/*
 * Copyright (c) 2001 Edouard TISSERANT <tissered@esstin.u-nancy.fr>
 * Parts inspired from Shane Watts <shane@bofh.asn.au> XFree86 3 Acecad Driver
 * Thanks to Emily, from AceCad, For giving me documents.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 */

#include "config.h"

#include <xorgVersion.h>
#define XORG_VERSION_BOTCHED XORG_VERSION_NUMERIC(1,4,0,0,0)
#if XORG_VERSION_CURRENT >= XORG_VERSION_BOTCHED
#define XORG_BOTCHED_INPUT 1
#else
#define XORG_BOTCHED_INPUT 0
#endif

/*****************************************************************************
 *	Standard Headers
 ****************************************************************************/

#ifdef LINUX_INPUT
#include <asm/types.h>
#include <linux/input.h>
#ifndef EV_SYN
#define EV_SYN EV_RST
#define SYN_REPORT 0
#endif
#ifdef BUS_PCI
#undef BUS_PCI
#endif
#ifdef BUS_ISA
#undef BUS_ISA
#endif
#endif

#include <misc.h>
#include <xf86.h>
#ifndef NEED_XF86_TYPES
#define NEED_XF86_TYPES
#endif
#include <xf86_OSproc.h>
#include <xisb.h>
#include <xf86Xinput.h>
#include <exevents.h>
#include <xf86Module.h>

#include <string.h>
#include <stdio.h>

#include <errno.h>
#ifdef LINUX_INPUT
#include <fcntl.h>
#ifdef LINUX_SYSFS
#include <sysfs/libsysfs.h>
#include <dlfcn.h>
#endif
#endif

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
#include <X11/Xatom.h>
#include <xserver-properties.h>
#endif

/* Previously found in xf86Xinput.h */
#ifdef DBG
#undef DBG
#endif
#define DBG(lvl, f) {if ((lvl) <= xf86GetVerbosity()) f;}

/*****************************************************************************
 *	Local Headers
 ****************************************************************************/
#include "acecad.h"

/*****************************************************************************
 *	Variables without includable headers
 ****************************************************************************/

/*****************************************************************************
 *	Local Variables
 ****************************************************************************/

/* max number of input events to read in one read call */
#define MAX_EVENTS 50

_X_EXPORT InputDriverRec ACECAD =
{
	1,
	"acecad",
	NULL,
	AceCadPreInit,
	NULL,
	NULL,
	0
};

static XF86ModuleVersionInfo VersionRec =
{
	"acecad",
	MODULEVENDORSTRING,
	MODINFOSTRING1,
	MODINFOSTRING2,
	XORG_VERSION_CURRENT,
	PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, PACKAGE_VERSION_PATCHLEVEL,
	ABI_CLASS_XINPUT,
	ABI_XINPUT_VERSION,
	MOD_CLASS_XINPUT,
	{0, 0, 0, 0}
};


_X_EXPORT XF86ModuleData acecadModuleData = {
	&VersionRec,
	SetupProc,
	TearDownProc
};

/*****************************************************************************
 *	Function Definitions
 ****************************************************************************/

static pointer
SetupProc(	pointer module,
		pointer options,
		int *errmaj,
		int *errmin )
{
	xf86AddInputDriver(&ACECAD, module, 0);
	return module;
}

static void
TearDownProc( pointer p )
{
}

static const char *default_options[] =
{
	"BaudRate", "9600",
	"StopBits", "1",
	"DataBits", "8",
	"Parity", "Odd",
	"Vmin", "1",
	"Vtime", "10",
	"FlowControl", "Xoff",
	NULL
};

#ifdef LINUX_INPUT
static int
IsUSBLine(int fd)
{
    int version;
    int err;

    SYSCALL(err = ioctl(fd, EVIOCGVERSION, &version));

    if (!err) {
        xf86MsgVerb(X_PROBED, 4, "Kernel Input driver version is %d.%d.%d\n",
                version >> 16, (version >> 8) & 0xff, version & 0xff);
        return 1;
    } else {
        xf86MsgVerb(X_PROBED, 4, "No Kernel Input driver found\n");
        return 0;
    }
}

/* Heavily inspired by synaptics/eventcomm.c */

#define DEV_INPUT_EVENT "/dev/input/event"
#define EV_DEV_NAME_MAXLEN 64
#define SET_EVENT_NUM(str, num) \
    snprintf(str, EV_DEV_NAME_MAXLEN, "%s%d", DEV_INPUT_EVENT, num)

static Bool
fd_query_acecad(int fd, char *ace_name) {
    char name[256] = "Unknown";
    int cmp_at = strlen(ace_name);
    if (cmp_at > 255)
        cmp_at = 255;
    ioctl(fd, EVIOCGNAME(sizeof(name)), name);
    name[cmp_at] = '\0';
    if (xf86NameCmp(name, ace_name) == 0)
        return TRUE;
    return FALSE;
}

static char ace_name_default[7] = "acecad";

#ifdef LINUX_SYSFS
static char usb_bus_name[4] = "usb";
static char acecad_driver_name[11] = "usb_acecad";
#endif

static Bool
AceCadAutoDevProbe(LocalDevicePtr local, int verb)
{
    /* We are trying to find the right eventX device */
    int i = 0;
    Bool have_evdev = FALSE;
    int noent_cnt = 0;
    const int max_skip = 10;
    char *ace_name = xf86FindOptionValue(local->options, "Name");
    char fname[EV_DEV_NAME_MAXLEN];
    int np;

#ifdef LINUX_SYSFS
    struct sysfs_bus *usb_bus = NULL;
    struct sysfs_driver *acecad_driver = NULL;
    struct sysfs_device *candidate = NULL;
    char *link = NULL;
    struct dlist *devs = NULL;
    struct dlist *links = NULL;

    xf86MsgVerb(X_INFO, verb, "%s: querying sysfs for Acecad tablets\n", local->name);
    usb_bus = sysfs_open_bus(usb_bus_name);
    if (usb_bus) {
	xf86MsgVerb(X_PROBED, 4, "%s: usb bus opened\n", local->name);
	acecad_driver = sysfs_get_bus_driver(usb_bus, acecad_driver_name);
	if (acecad_driver) {
	    xf86MsgVerb(X_PROBED, 4, "%s: usb_acecad driver opened\n", local->name);
	    devs = sysfs_get_driver_devices(acecad_driver);
	    if (devs) {
		xf86MsgVerb(X_PROBED, 4, "%s: usb_acecad devices retrieved\n", local->name);
		dlist_for_each_data(devs, candidate, struct sysfs_device) {
		    xf86MsgVerb(X_PROBED, 4, "%s: device %s at %s\n", local->name, candidate->name, candidate->path);
		    links = sysfs_open_link_list(candidate->path);
		    dlist_for_each_data(links, link, char) {
			if (sscanf(link, "input:event%d", &i) == 1) {
			    xf86MsgVerb(X_PROBED, 4, "%s: device %s at %s: %s\n", local->name, candidate->name, candidate->path, link);
			    break;
			}
		    }
		    sysfs_close_list(links);
		    if (i > 0) /* We found something */
			break;
		}
	    } else
		xf86MsgVerb(X_WARNING, 4, "%s: no usb_acecad devices found\n", local->name);
	} else
	    xf86MsgVerb(X_WARNING, 4, "%s: usb_acecad driver not found\n", local->name);
    } else
	xf86MsgVerb(X_WARNING, 4, "%s: usb bus not found\n", local->name);
    sysfs_close_bus(usb_bus);

    if (i > 0) {
	/* We found something */
	np = SET_EVENT_NUM(fname, i);
	if (np < 0 || np >= EV_DEV_NAME_MAXLEN) {
	    xf86MsgVerb(X_WARNING, verb, "%s: unable to manage event device %d\n", local->name, i);
	} else {
	    goto ProbeFound;
	}
    } else
	xf86MsgVerb(X_WARNING, verb, "%s: no Acecad devices found via sysfs\n", local->name);

#endif

    if (!ace_name)
        ace_name = ace_name_default;

    xf86MsgVerb(X_INFO, verb, "%s: probing event devices for Acecad tablets\n", local->name);
    for (i = 0; ; i++) {
        int fd = -1;
        Bool is_acecad;

        np = SET_EVENT_NUM(fname, i);
        if (np < 0 || np >= EV_DEV_NAME_MAXLEN) {
            xf86MsgVerb(X_WARNING, verb, "%s: too many devices, giving up %d\n", local->name, i);
            break;
        }
        SYSCALL(fd = open(fname, O_RDONLY));
        if (fd < 0) {
            if (errno == ENOENT) {
                if (++noent_cnt >= max_skip)
                    break;
                else
                    continue;
            } else {
                continue;
            }
        }
        noent_cnt = 0;
        have_evdev = TRUE;
        is_acecad = fd_query_acecad(fd, ace_name);
        SYSCALL(close(fd));
        if (is_acecad) {
            goto ProbeFound;
        }
    }
    xf86MsgVerb(X_WARNING, verb, "%s: no Acecad event device found (checked %d nodes, no device name started with '%s')\n",
            local->name, i + 1, ace_name);
    if (i <= max_skip)
        xf86MsgVerb(X_WARNING, verb, "%s: The /dev/input/event* device nodes seem to be missing\n",
                local->name);
    if (i > max_skip && !have_evdev)
        xf86MsgVerb(X_WARNING, verb, "%s: The evdev kernel module seems to be missing\n", local->name);
    return FALSE;

ProbeFound:
    xf86Msg(X_PROBED, "%s auto-dev sets device to %s\n",
            local->name, fname);
    xf86ReplaceStrOption(local->options, "Device", fname);
    return TRUE;
}

#endif

static InputInfoPtr
AceCadPreInit(InputDriverPtr drv, IDevPtr dev, int flags)
{
    LocalDevicePtr local = xf86AllocateInput(drv, 0);
    AceCadPrivatePtr priv = xcalloc (1, sizeof(AceCadPrivateRec));
    int speed;
    int msgtype;
    char *s;

    if ((!local) || (!priv))
        goto SetupProc_fail;

    memset(priv, 0, sizeof(AceCadPrivateRec));

    local->name = dev->identifier;
    local->type_name = XI_TABLET;
    local->flags = XI86_POINTER_CAPABLE | XI86_SEND_DRAG_EVENTS;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) == 0
    local->motion_history_proc = xf86GetMotionEvents;
#endif
    local->control_proc = NULL;
    local->close_proc = CloseProc;
    local->switch_mode = NULL;
    local->conversion_proc = ConvertProc;
    local->reverse_conversion_proc = ReverseConvertProc;
    local->dev = NULL;
    local->private = priv;
    local->private_flags = 0;
    local->conf_idev = dev;
    local->device_control = DeviceControl;
    /*local->always_core_feedback = 0;*/

    xf86CollectInputOptions(local, default_options, NULL);

    xf86OptionListReport(local->options);

    priv->acecadInc = xf86SetIntOption(local->options, "Increment", 0 );

    s = xf86FindOptionValue(local->options, "Device");
    if (!s || (s && (xf86NameCmp(s, "auto-dev") == 0))) {
#ifdef LINUX_INPUT
        priv->flags |= AUTODEV_FLAG;
        if (!AceCadAutoDevProbe(local, 0))
        {
            xf86Msg(X_ERROR, "%s: unable to find device\n", local->name);
            goto SetupProc_fail;
        }
#else
        xf86Msg(X_NOT_IMPLEMENTED, "%s: device autodetection not implemented, sorry\n", local->name);
        goto SetupProc_fail;
#endif
    }

    local->fd = xf86OpenSerial (local->options);
    if (local->fd == -1)
    {
        xf86Msg(X_ERROR, "%s: unable to open device\n", local->name);
        goto SetupProc_fail;
    }
    xf86ErrorFVerb( 6, "tty port opened successfully\n" );

#ifdef LINUX_INPUT
    if (IsUSBLine(local->fd)) {
        priv->flags |= USB_FLAG;

        local->read_input = USBReadInput;

        if (USBQueryHardware(local) != Success)
        {
            xf86Msg(X_ERROR, "%s: unable to query/initialize hardware (not an %s?).\n", local->name, local->type_name);
            goto SetupProc_fail;
        }
    } else
#endif
    {
        local->read_input = ReadInput;

        msgtype = X_DEFAULT;
        if (xf86FindOptionValue(local->options, "ReportSpeed")) {
            msgtype = X_CONFIG;
            speed = xf86SetIntOption(local->options, "ReportSpeed", 85 );
        } else {
            speed = 85;
        }

        switch (speed)
        {
            case 120:
                priv->acecadReportSpeed = 'Q';
                break;
            case 85:
                priv->acecadReportSpeed = 'R';
                break;
            case 10:
                priv->acecadReportSpeed = 'S';
                break;
            case 2:
                priv->acecadReportSpeed = 'T';
                break;
            default:
                priv->acecadReportSpeed = 'R';
                speed = 85;
                xf86Msg(X_ERROR, "%s: ReportSpeed value %d invalid. Possible values: 120, 85, 10, 2. Defaulting to 85\n", local->name, speed);
                msgtype = X_DEFAULT;
        }

        xf86Msg(msgtype, "%s report %d points/s\n", local->name, speed);

        priv->buffer = XisbNew (local->fd, 200);

        /*
         * Verify that hardware is attached and fuctional
         */
        if (QueryHardware(priv) != Success)
        {
            xf86Msg(X_ERROR, "%s: unable to query/initialize hardware (not an %s?).\n", local->name, local->type_name);
            goto SetupProc_fail;
        }
    }

    s = xf86FindOptionValue(local->options, "Mode");
    msgtype = s ? X_CONFIG : X_DEFAULT;
    if (!(s && (xf86NameCmp(s, "relative") == 0)))
    {
        priv->flags |= ABSOLUTE_FLAG;
    }

    xf86Msg(msgtype, "%s is in %s mode\n", local->name, (priv->flags & ABSOLUTE_FLAG) ? "absolute" : "relative");
    DBG (9, XisbTrace (priv->buffer, 1));

    local->history_size = xf86SetIntOption(local->options , "HistorySize", 0);

    xf86ProcessCommonOptions(local, local->options);

    local->flags |= XI86_CONFIGURED;

    if (local->fd != -1)
    {
        RemoveEnabledDevice (local->fd);
        if (priv->buffer)
        {
            XisbFree(priv->buffer);
            priv->buffer = NULL;
        }
        xf86CloseSerial(local->fd);
    }
    RemoveEnabledDevice (local->fd);
    local->fd = -1;
    return local;

    /*
     * If something went wrong, cleanup and return NULL
     */
SetupProc_fail:
    if ((local) && (local->fd))
        xf86CloseSerial (local->fd);
    if ((priv) && (priv->buffer))
        XisbFree (priv->buffer);
    if (priv) {
        xfree (priv);
	if (local)
		local->private = NULL;
    }
    xf86DeleteInput(local, 0);
    return NULL;
}

static Bool
DeviceControl (DeviceIntPtr dev, int mode)
{
    Bool RetValue;

    switch (mode)
    {
        case DEVICE_INIT:
            DeviceInit(dev);
            RetValue = Success;
            break;
        case DEVICE_ON:
            RetValue = DeviceOn(dev);
            break;
        case DEVICE_OFF:
            RetValue = DeviceOff(dev);
            break;
        case DEVICE_CLOSE:
            RetValue = DeviceClose(dev);
            break;
        default:
            RetValue = BadValue;
    }

    return RetValue;
}

static Bool
DeviceOn (DeviceIntPtr dev)
{
    char buffer[256];
    LocalDevicePtr local = (LocalDevicePtr) dev->public.devicePrivate;
    AceCadPrivatePtr priv = (AceCadPrivatePtr) (local->private);

    xf86MsgVerb(X_INFO, 4, "%s Device On\n", local->name);

    local->fd = xf86OpenSerial(local->options);
    if (local->fd == -1)
    {
        xf86Msg(X_WARNING, "%s: cannot open input device %s: %s\n", local->name, xf86FindOptionValue(local->options, "Device"), strerror(errno));
        priv->flags &= ~AVAIL_FLAG;
#ifdef LINUX_INPUT
        if ((priv->flags & AUTODEV_FLAG) && AceCadAutoDevProbe(local, 4))
            local->fd = xf86OpenSerial(local->options);
        if (local->fd == -1)
#endif
            return !Success;
    }
    priv->flags |= AVAIL_FLAG;


    if (!(priv->flags & USB_FLAG)) {
        priv->buffer = XisbNew(local->fd, 200);
        if (!priv->buffer)
        {
            xf86CloseSerial(local->fd);
            local->fd = -1;
            return !Success;
        }

        /* Rets qu'a l'envoyer a la tablette */
        sprintf(buffer, "%s%c%c%c%c", acecad_initstr, priv->acecadReportSpeed, ACECAD_INCREMENT, 32 + priv->acecadInc, (priv->flags & ABSOLUTE_FLAG)? ACECAD_ABSOLUTE: ACECAD_RELATIVE);
        XisbWrite (priv->buffer, (unsigned char *)buffer, strlen(buffer));
    }

    xf86FlushInput(local->fd);
    xf86AddEnabledDevice (local);
    dev->public.on = TRUE;
    return Success;
}

static Bool
DeviceOff (DeviceIntPtr dev)
{
    LocalDevicePtr local = (LocalDevicePtr) dev->public.devicePrivate;
    AceCadPrivatePtr priv = (AceCadPrivatePtr) (local->private);

    xf86MsgVerb(X_INFO, 4, "%s Device Off\n", local->name);

    if (local->fd != -1)
    {
        RemoveEnabledDevice (local->fd);
        if (priv->buffer)
        {
            XisbFree(priv->buffer);
            priv->buffer = NULL;
        }
        xf86CloseSerial(local->fd);
    }


    xf86RemoveEnabledDevice (local);
    dev->public.on = FALSE;
    return Success;
}

static Bool
DeviceClose (DeviceIntPtr dev)
{
    LocalDevicePtr local = (LocalDevicePtr) dev->public.devicePrivate;

    xf86MsgVerb(X_INFO, 4, "%s Device Close\n", local->name);

    return Success;
}

static void
ControlProc(DeviceIntPtr dev, PtrCtrl *ctrl)
{
    LocalDevicePtr local = (LocalDevicePtr) dev->public.devicePrivate;

    xf86MsgVerb(X_INFO, 4, "%s Control Proc\n", local->name);
}

static Bool
DeviceInit (DeviceIntPtr dev)
{
    int rx, ry;
    LocalDevicePtr local = (LocalDevicePtr) dev->public.devicePrivate;
    AceCadPrivatePtr priv = (AceCadPrivatePtr) (local->private);
    unsigned char map[] = {0, 1, 2, 3};
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
    Atom btn_labels[3];
    Atom axes_labels[3];

    btn_labels[0] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_LEFT);
    btn_labels[1] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_MIDDLE);
    btn_labels[2] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_RIGHT);

    if ((priv->flags & ABSOLUTE_FLAG))
    {
        axes_labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_ABS_X);
        axes_labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_ABS_Y);
        axes_labels[2] = XIGetKnownProperty(AXIS_LABEL_PROP_ABS_PRESSURE);
    } else
    {
        axes_labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_X);
        axes_labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_Y);
        axes_labels[2] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_Z);
    }
#endif

    xf86MsgVerb(X_INFO, 4, "%s Init\n", local->name);

    /* 3 boutons */
    if (InitButtonClassDeviceStruct (dev, 3,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
                btn_labels,
#endif
                map) == FALSE)
    {
        xf86Msg(X_ERROR, "%s: unable to allocate ButtonClassDeviceStruct\n", local->name);
        return !Success;
    }

    if (InitFocusClassDeviceStruct (dev) == FALSE)
    {
        xf86Msg(X_ERROR, "%s: unable to allocate FocusClassDeviceStruct\n", local->name);
        return !Success;
    }

    if (InitPtrFeedbackClassDeviceStruct(dev, ControlProc) == FALSE) {
        xf86Msg(X_ERROR, "%s: unable to init ptr feedback\n", local->name);
        return !Success;
    }


    /* 3 axes */
    if (InitValuatorClassDeviceStruct (dev, 3,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
                axes_labels,
#endif
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 3
                xf86GetMotionEvents,
#endif
                local->history_size,
                ((priv->flags & ABSOLUTE_FLAG)? Absolute: Relative)|OutOfProximity)
            == FALSE)
    {
        xf86Msg(X_ERROR, "%s: unable to allocate ValuatorClassDeviceStruct\n", local->name);
        return !Success;
    }
    else
    {

        InitValuatorAxisStruct(dev,
                0,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
                axes_labels[0],
#endif
                0,			/* min val */
#if XORG_BOTCHED_INPUT
                screenInfo.screens[0]->width,
#else
                priv->acecadMaxX,	/* max val */
#endif
                1000,			/* resolution */
                0,			/* min_res */
                1000);			/* max_res */
        InitValuatorAxisStruct(dev,
                1,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
                axes_labels[1],
#endif
                0,			/* min val */
#if XORG_BOTCHED_INPUT
                screenInfo.screens[0]->height,
#else
                priv->acecadMaxY,	/* max val */
#endif
                1000,			/* resolution */
                0,			/* min_res */
                1000);			/* max_res */
        InitValuatorAxisStruct(dev,
                2,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
                axes_labels[2],
#endif
                0,			/* min val */
                priv->acecadMaxZ,	/* max val */
                1000,			/* resolution */
                0,			/* min_res */
                1000);		/* max_res */

    }

    if (InitProximityClassDeviceStruct (dev) == FALSE)
    {
        xf86Msg(X_ERROR, "%s: unable to allocate ProximityClassDeviceStruct\n", local->name);
        return !Success;
    }

    xf86MotionHistoryAllocate (local);


    /* On ne peut pas calculer l'increment avant, faute d'ecran pour
       connaitre la taille... */

    if (priv->acecadInc > 95)
        priv->acecadInc = 95;
    if (priv->acecadInc < 1)
    {
        /* guess the best increment value given video mode */
        rx = priv->acecadMaxX / screenInfo.screens[0]->width;
        ry = priv->acecadMaxY / screenInfo.screens[0]->height;
        if (rx < ry)
            priv->acecadInc = rx;
        else
            priv->acecadInc = ry;
        if (priv->acecadInc < 1)
            priv->acecadInc = 1;
    }

    xf86Msg(X_INFO, "%s Increment: %d\n", local->name, priv->acecadInc);

    return Success;
}

static void
ReadInput (LocalDevicePtr local)
{
    int x, y, z;
    int prox, buttons;
    int is_core_pointer = 0, is_absolute;
    AceCadPrivatePtr priv = (AceCadPrivatePtr) (local->private);

    /*xf86Msg(X_INFO, "ACECAD Tablet Read Input\n");*/

    is_absolute = (priv->flags & ABSOLUTE_FLAG);
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) == 0
    is_core_pointer = xf86IsCorePointer(local->dev);
#endif

    /*
     * set blocking to -1 on the first call because we know there is data to
     * read. Xisb automatically clears it after one successful read so that
     * succeeding reads are preceeded buy a select with a 0 timeout to prevent
     * read from blocking indefinately.
     */
    XisbBlockDuration (priv->buffer, -1);

    while (AceCadGetPacket (priv) == Success)
    {
        x = (int)priv->packet[1] | ((int)priv->packet[2] << 7);
        y = (int)priv->packet[3] | ((int)priv->packet[4] << 7);

        if (!(priv->flags & ABSOLUTE_FLAG))
        {
            x = priv->packet[0] & XSIGN_BIT? x:-x;
            y = priv->packet[0] & YSIGN_BIT? y:-y;
        }
        else
        {
            y = priv->acecadMaxY - y ;
        }


        z = ((int)priv->packet[5] << 2) |
            (((int)priv->packet[6] & 0x01) << 1) |
            (((int)priv->packet[6] & 0x10) >> 4);

        buttons = ((int)priv->packet[0] & 0x07) |
            ((int)priv->packet[6] & 0x02 << 2);

        prox = (priv->packet[0] & PROXIMITY_BIT)? 0: 1;

        if (prox)
        {
            if (!(priv->acecadOldProximity))
                if (!is_core_pointer)
                {
                    /*xf86Msg(X_INFO, "ACECAD Tablet ProxIN %d %d %d\n",x, y, z);*/
                    xf86PostProximityEvent(local->dev, 1, 0, 3 , x, y, z);
                }

            if ((is_absolute && ((priv->acecadOldX != x) || (priv->acecadOldY != y) || (priv->acecadOldZ != z)))
                    || (!is_absolute && (x || y)))
            {
                if (is_absolute || priv->acecadOldProximity)
                {
                    /*xf86Msg(X_INFO, "ACECAD Tablet Motion %d %d %d\n", x, y, z);*/
                    xf86PostMotionEvent(local->dev, is_absolute, 0, 3, x, y, z);
                }
            }

            if (priv->acecadOldButtons != buttons)
            {
                int delta = buttons ^ priv->acecadOldButtons;
                while (delta)
                {
                    int id = ffs(delta);
                    delta &= ~(1 << (id-1));

                    /*xf86Msg(X_INFO, "ACECAD Tablet Button %d 0x%x\n",id,(buttons&(1<<(id-1))));*/
                    xf86PostButtonEvent(local->dev, is_absolute, id, (buttons&(1<<(id-1))), 0, 3, x, y,z);
                }
            }

            priv->acecadOldButtons = buttons;
            priv->acecadOldX = x;
            priv->acecadOldY = y;
            priv->acecadOldZ = z;
            priv->acecadOldProximity = prox;
        }
        else
        {
            if (!is_core_pointer)
                if (priv->acecadOldProximity)
                {
                    /*xf86Msg(X_INFO, "ACECAD Tablet ProxOUT %d %d %d\n",x, y, z);*/
                    xf86PostProximityEvent(local->dev, 0, 0, 3, x,y,z);
                }
            priv->acecadOldProximity = 0;
        }
    }
    /*xf86Msg(X_INFO, "ACECAD Tablet Sortie Read Input\n");*/
}

#ifdef LINUX_INPUT
#define set_bit(byte,nb,bit)	(bit ? byte | (1<<nb) : byte & (~(1<<nb)))
static void
USBReadInput (LocalDevicePtr local)
{
    int len;
    struct input_event * event;
    char eventbuf[sizeof(struct input_event) * MAX_EVENTS];
    AceCadPrivatePtr priv = (AceCadPrivatePtr) (local->private);
    int x = priv->acecadOldX;
    int y = priv->acecadOldY;
    int z = priv->acecadOldZ;
    int report_x, report_y;
    int prox = priv->acecadOldProximity;
    int buttons = priv->acecadOldButtons;
    int is_core_pointer = 0;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) == 0
    is_core_pointer = xf86IsCorePointer(local->dev);
#endif
    /* Is autodev active? */
    int autodev = priv->flags & AUTODEV_FLAG;
    /* Was the device available last time we checked? */
    int avail = priv->flags & AVAIL_FLAG;

    SYSCALL(len = xf86ReadSerial(local->fd, eventbuf, sizeof(eventbuf)));

    if (len <= 0) {
        if (avail) {
            xf86Msg(X_ERROR, "%s: error reading device %s: %s\n", local->name, xf86FindOptionValue(local->options, "Device"), strerror(errno));
        }
        if (NOTAVAIL) {
            priv->flags &= ~AVAIL_FLAG;
            if(autodev) {
                if (AceCadAutoDevProbe(local, 4)) {
                    DeviceOff(local->dev);
                    DeviceOn(local->dev);
                }
            }
        }
        return;
    } else {
        if (!avail) {
            /* If the device wasn't available last time we checked */
            xf86Msg(X_INFO, "%s: device %s is available again\n", local->name, xf86FindOptionValue(local->options, "Device"));
            priv->flags |= AVAIL_FLAG;
        }
    }

    for (event = (struct input_event *)eventbuf;
            event < (struct input_event *)(eventbuf+len); event++) {

        switch (event->type) {
            case EV_SYN: /* 2.6.x */
                if (event->code != SYN_REPORT)
                    xf86Msg(X_ERROR, "%s: unknown EV_SYN code %d\n", local->name, event->code);
                break;
            case EV_ABS:
                switch (event->code) {
                    case ABS_X:
                        x = event->value;
                        break;

                    case ABS_Y:
                        y = event->value;
                        break;

                    case ABS_PRESSURE:
                        z = event->value;
                        break;

                    case ABS_MISC:
                        break;

                }
                break; /* EV_ABS */

            case EV_KEY:
                switch (event->code) {
                    case BTN_TOOL_PEN:
                        prox = event->value;
                        break;

                    case BTN_TOUCH:
                        buttons = set_bit(buttons,0,event->value);
                        break;

                    case BTN_STYLUS:
                        buttons = set_bit(buttons,1,event->value);
                        break;

                    case BTN_STYLUS2:
                        buttons = set_bit(buttons,2,event->value);
                        break;
                }
                break; /* EV_KEY */
            default:
                xf86Msg(X_ERROR, "%s: unknown event type/code %d/%d\n", local->name, event->type, event->code);
        } /* switch event->type */

        /* Linux Kernel 2.6.x sends EV_SYN/SYN_REPORT as an event terminator,
         * whereas 2.4.x sends EV_ABS/ABS_MISC. We have to support both.
         */
        if (!(  (event->type == EV_SYN && event->code == SYN_REPORT) ||
                    (event->type == EV_ABS && event->code == ABS_MISC)
             )) {
            continue;
        }

        if (prox)
        {
#if XORG_BOTCHED_INPUT
            ConvertProc(local, 0, 3, x, y, 0, 0, 0, 0, &report_x, &report_y);
#else
            report_x = x;
            report_y = y;
#endif
            if (!(priv->acecadOldProximity))
                if (!is_core_pointer)
                {
                    xf86PostProximityEvent(local->dev, 1, 0, 3 , report_x, report_y, z);
                }


            xf86PostMotionEvent(local->dev, 1, 0, 3, report_x, report_y, z);

            if (priv->acecadOldButtons != buttons)
            {
                int delta = buttons ^ priv->acecadOldButtons;
                while (delta)
                {
                    int id = ffs(delta);
                    delta &= ~(1 << (id-1));

                    xf86PostButtonEvent(local->dev, 1, id, (buttons&(1<<(id-1))), 0, 3, report_x, report_y, z);
                }
            }
        }
        else
        {
            if (!is_core_pointer)
                if (priv->acecadOldProximity)
                {
                    xf86PostProximityEvent(local->dev, 0, 0, 3, report_x, report_y, z);
                }
            priv->acecadOldProximity = 0;
        }

        priv->acecadOldButtons = buttons;
        priv->acecadOldX = x;
        priv->acecadOldY = y;
        priv->acecadOldZ = z;
        priv->acecadOldProximity = prox;
    }
    /*xf86Msg(X_INFO, "ACECAD Tablet Sortie Read Input\n");*/
}
#endif

static void
CloseProc (LocalDevicePtr local)
{
}

/*
 * The ConvertProc function may need to be tailored for your device.
 * This function converts the device's valuator outputs to x and y coordinates
 * to simulate mouse events.
 */
static Bool
ConvertProc (LocalDevicePtr local, int first, int num,
        int v0, int v1, int v2, int v3, int v4, int v5,
        int *x, int *y)
{
    AceCadPrivatePtr priv = (AceCadPrivatePtr)(local->private);

    /* TODO: should have a structure to hold which screen the
     * pointer is attached to? */
    // xf86Msg(X_INFO, "%s: coordinate conversion in : %d, %d\n", local->name, v0, v1);
    *x = v0 * screenInfo.screens[0]->width / priv->acecadMaxX;
    *y = v1 * screenInfo.screens[0]->height / priv->acecadMaxY;
    // xf86Msg(X_INFO, "%s: coordinate conversion out: %d, %d\n", local->name, *x, *y);
    return TRUE;
}


static Bool
ReverseConvertProc (LocalDevicePtr local,
        int x, int  y,
        int *valuators)
{
    AceCadPrivatePtr priv = (AceCadPrivatePtr)(local->private);

    // xf86Msg(X_INFO, "%s: reverse coordinate conversion in : %d, %d\n", local->name, x, y);
    valuators[0] = x * priv->acecadMaxX / screenInfo.screens[0]->width;
    valuators[1] = y * priv->acecadMaxY / screenInfo.screens[0]->height;
    // xf86Msg(X_INFO, "%s: reverse coordinate conversion out: %d, %d\n", local->name, valuators[0], valuators[1]);

    return TRUE;
}


#define WriteString(str)\
    XisbWrite (priv->buffer, (unsigned char *)(str), strlen(str))


static Bool
QueryHardware (AceCadPrivatePtr priv)
{

    /* Reset */
    WriteString("z0");

    /* Wait */
    milisleep (250);

    /* Prompt Mode in order to not be disturbed */
    WriteString(ACECAD_PROMPT_MODE);

    /* Flush */
    while (XisbRead(priv->buffer) >= 0);

    /* Ask for Config packet*/
    WriteString(ACECAD_CONFIG);

    /* Read the packet */
    XisbBlockDuration (priv->buffer, 1000000);
    NewPacket (priv);

    /*xf86Msg(X_CONFIG, "ACECAD Tablet init envoyé \n");*/

    if ((AceCadGetPacket (priv) == Success))
    {
        priv->acecadMaxX = (int)priv->packet[1] + ((int)priv->packet[2] << 7);
        priv->acecadMaxY = (int)priv->packet[3] + ((int)priv->packet[4] << 7);
        priv->acecadMaxZ = 512;
        xf86Msg(X_PROBED, "ACECAD Tablet MaxX:%d MaxY:%d\n", priv->acecadMaxX, priv->acecadMaxY);
    }
    else
        return !Success;

    /*xf86Msg(X_INFO, "ACECAD Tablet query hardware fini \n");*/
    return Success;
}

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define test_bit(bit, array)	((array[LONG(bit)] >> OFF(bit)) & 1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define LONG(x) ((x)/BITS_PER_LONG)

#ifdef LINUX_INPUT
static Bool
USBQueryHardware (LocalDevicePtr local)
{
    AceCadPrivatePtr	priv = (AceCadPrivatePtr) local->private;
    unsigned long	bit[EV_MAX][NBITS(KEY_MAX)];
    int			i, j;
    int			abs[5];
    char		name[256] = "Unknown";

    ioctl(local->fd, EVIOCGNAME(sizeof(name)), name);
    xf86MsgVerb(X_PROBED, 4, "Kernel Input device name: \"%s\"\n", name);

    memset(bit, 0, sizeof(bit));
    ioctl(local->fd, EVIOCGBIT(0, EV_MAX), bit[0]);

    for (i = 0; i < EV_MAX; i++)
        if (test_bit(i, bit[0])) {
            ioctl(local->fd, EVIOCGBIT(i, KEY_MAX), bit[i]);
            for (j = 0; j < KEY_MAX; j++)
                if (test_bit(j, bit[i])) {
                    if (i == EV_ABS) {
                        ioctl(local->fd, EVIOCGABS(j), abs);
                        switch (j) {
                            case ABS_X:
                                priv->acecadMaxX = abs[2];
                                break;

                            case ABS_Y:
                                priv->acecadMaxY = abs[2];
                                break;

                            case ABS_PRESSURE:
                                priv->acecadMaxZ = abs[2];
                                break;
                        }
                    }
                }
        }

    xf86Msg(X_PROBED, "ACECAD Tablet MaxX:%d MaxY:%d MaxZ:%d\n", priv->acecadMaxX, priv->acecadMaxY, priv->acecadMaxZ);
    return Success;
}
#endif

static void
NewPacket (AceCadPrivatePtr priv)
{
    priv->packeti = 0;
}

static Bool
AceCadGetPacket (AceCadPrivatePtr priv)
{
    int count = 0;
    int c = 0;

    while((c = XisbRead(priv->buffer)) >= 0 )
    {

        /*
         * fail after 500 bytes so the server doesn't hang forever if a
         * device sends bad data.
         */
        if (count++ > 500)
        {
            NewPacket (priv);
            return !Success;
        }

        if (c & PHASING_BIT)
        {
            NewPacket(priv);

            /*xf86Msg(X_CONFIG, "Push %2.2x\n",(char) c);*/
            XisbBlockDuration (priv->buffer, 10000);
            priv->packet[priv->packeti++] = c;
            count = ACECAD_PACKET_SIZE - 1;
            while (count-- && (c = XisbRead(priv->buffer)) >= 0)
            {
                /*xf86Msg(X_INFO, "Push %2.2x\n",(char) c);*/
                priv->packet[priv->packeti++] = c;
            }
            XisbBlockDuration (priv->buffer, 0);
            if(c > 0)
                return Success;
        }
    }
    return !Success;
}
