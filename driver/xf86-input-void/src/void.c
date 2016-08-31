/*
 * Copyright 1999 by Frederic Lepied, France. <Lepied@XFree86.org>
 *                                                                            
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is  hereby granted without fee, provided that
 * the  above copyright   notice appear  in   all  copies and  that both  that
 * copyright  notice   and   this  permission   notice  appear  in  supporting
 * documentation, and that   the  name of  Frederic   Lepied not  be  used  in
 * advertising or publicity pertaining to distribution of the software without
 * specific,  written      prior  permission.     Frederic  Lepied   makes  no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.                   
 *                                                                            
 * FREDERIC  LEPIED DISCLAIMS ALL   WARRANTIES WITH REGARD  TO  THIS SOFTWARE,
 * INCLUDING ALL IMPLIED   WARRANTIES OF MERCHANTABILITY  AND   FITNESS, IN NO
 * EVENT  SHALL FREDERIC  LEPIED BE   LIABLE   FOR ANY  SPECIAL, INDIRECT   OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA  OR PROFITS, WHETHER  IN  AN ACTION OF  CONTRACT,  NEGLIGENCE OR OTHER
 * TORTIOUS  ACTION, ARISING    OUT OF OR   IN  CONNECTION  WITH THE USE    OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

/* Input device which doesn't output any event. This device can be used
 * as a core pointer or as a core keyboard.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <misc.h>
#include <xf86.h>
#define NEED_XF86_TYPES 1
#if !defined(DGUX)
#include <xisb.h>
#endif
#include <xf86_OSproc.h>
#include <xf86Xinput.h>
#include <exevents.h>		/* Needed for InitValuator/Proximity stuff */
#include <X11/keysym.h>
#include <mipointer.h>

#include <xf86Module.h>

#include <X11/Xatom.h>
#include <xserver-properties.h>

#define MAXBUTTONS 3

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 12
#error "XINPUT ABI 12 required."
#endif
/******************************************************************************
 * Function/Macro keys variables
 *****************************************************************************/

static void
BellProc(
    int percent,
    DeviceIntPtr pDev,
    pointer ctrl,
    int unused)
{
    return;
}

static void
KeyControlProc(
    DeviceIntPtr pDev,
    KeybdCtrl *ctrl)
{
    return;
}

static void
PointerControlProc(
    DeviceIntPtr dev,
    PtrCtrl *ctrl)
{
    return;
}

/*
 * xf86VoidControlProc --
 *
 * called to change the state of a device.
 */
static int
xf86VoidControlProc(DeviceIntPtr device, int what)
{
    InputInfoPtr pInfo;
    unsigned char map[MAXBUTTONS + 1];
    unsigned char i;
    Bool result;
    Atom btn_labels[MAXBUTTONS] = {0};
    Atom axes_labels[2] = {0};

    axes_labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_ABS_X);
    axes_labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_ABS_Y);

    btn_labels[0] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_LEFT);
    btn_labels[1] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_MIDDLE);
    btn_labels[2] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_RIGHT);

    pInfo = device->public.devicePrivate;
    
    switch (what)
    {
    case DEVICE_INIT:
	device->public.on = FALSE;

	for (i = 0; i < MAXBUTTONS; i++) {
	    map[i + 1] = i + 1;
	}
	
	if (InitButtonClassDeviceStruct(device,
					MAXBUTTONS,
					btn_labels,
					map) == FALSE) {
	  ErrorF("unable to allocate Button class device\n");
	  return !Success;
	}

	result = InitKeyboardDeviceStruct(device, NULL,
					  BellProc, KeyControlProc);
	if (!result) {
	  ErrorF("unable to init keyboard device\n");
	  return !Success;
	}

	if (InitValuatorClassDeviceStruct(device,
					  2,
					  axes_labels,
					  0,
					  Absolute) == FALSE) {
	  InitValuatorAxisStruct(device,
				 0,
				 axes_labels[0],
				 0, /* min val */1, /* max val */
				 1, /* resolution */
				 0, /* min_res */
				 1, /* max_res */
				 Absolute);
	  InitValuatorAxisStruct(device,
				 1,
				 axes_labels[1],
				 0, /* min val */1, /* max val */
				 1, /* resolution */
				 0, /* min_res */
				 1, /* max_res */
				 Absolute);
	  ErrorF("unable to allocate Valuator class device\n"); 
	  return !Success;
	}
	else {
	  /* allocate the motion history buffer if needed */
	  xf86MotionHistoryAllocate(pInfo);
	}
	if (InitPtrFeedbackClassDeviceStruct(device, PointerControlProc) == FALSE) {
	  ErrorF("unable to init pointer feedback class device\n"); 
	  return !Success;
	}
	break;

    case DEVICE_ON:
	device->public.on = TRUE;
	break;

    case DEVICE_OFF:
    case DEVICE_CLOSE:
	device->public.on = FALSE;
	break;

    default:
	return BadValue;
    }
    return Success;
}

/*
 * xf86VoidUninit --
 *
 * called when the driver is unloaded.
 */
static void
xf86VoidUninit(InputDriverPtr	drv,
	       InputInfoPtr	pInfo,
	       int		flags)
{
    xf86VoidControlProc(pInfo->dev, DEVICE_OFF);
}

/*
 * xf86VoidInit --
 *
 * called when the module subsection is found in XF86Config
 */
static int
xf86VoidInit(InputDriverPtr	drv,
	     InputInfoPtr	pInfo,
	     int		flags)
{
    /* Initialise the InputInfoRec. */
    pInfo->type_name = "Void";
    pInfo->device_control = xf86VoidControlProc;
    pInfo->read_input = NULL;
    pInfo->control_proc = NULL;
    pInfo->switch_mode = NULL;
    pInfo->fd = -1;

    return Success;
}

_X_EXPORT InputDriverRec VOID = {
    1,				/* driver version */
    "void",			/* driver name */
    NULL,			/* identify */
    xf86VoidInit,		/* pre-init */
    xf86VoidUninit,		/* un-init */
    NULL,			/* module */
};

/*
 ***************************************************************************
 *
 * Dynamic loading functions
 *
 ***************************************************************************
 */
/*
 * xf86VoidUnplug --
 *
 * called when the module subsection is found in XF86Config
 */
static void
xf86VoidUnplug(pointer	p)
{
}

/*
 * xf86VoidPlug --
 *
 * called when the module subsection is found in XF86Config
 */
static pointer
xf86VoidPlug(pointer	module,
	    pointer	options,
	    int		*errmaj,
	    int		*errmin)
{
    xf86AddInputDriver(&VOID, module, 0);

    return module;
}

static XF86ModuleVersionInfo xf86VoidVersionRec =
{
    "void",
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, PACKAGE_VERSION_PATCHLEVEL,
    ABI_CLASS_XINPUT,
    ABI_XINPUT_VERSION,
    MOD_CLASS_XINPUT,
    {0, 0, 0, 0}		/* signature, to be patched into the file by */
				/* a tool */
};

_X_EXPORT XF86ModuleData voidModuleData = {
    &xf86VoidVersionRec,
    xf86VoidPlug,
    xf86VoidUnplug
};

