/*
 * Copyright 2007-2011 by Sascha Hlusiak. <saschahlusiak@freedesktop.org>     
 * Copyright 1995-1999 by Frederic Lepied, France. <Lepied@XFree86.org>       
 *                                                                            
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is  hereby granted without fee, provided that
 * the  above copyright   notice appear  in   all  copies and  that both  that
 * copyright  notice   and   this  permission   notice  appear  in  supporting
 * documentation, and that  the  names  of copyright holders not  be  used  in
 * advertising or publicity pertaining to distribution of the software without
 * specific,  written      prior  permission.  The copyright holders  make  no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.                   
 *                                                                            
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED   WARRANTIES OF MERCHANTABILITY  AND   FITNESS, IN NO
 * EVENT  SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT   OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA  OR PROFITS, WHETHER  IN  AN ACTION OF  CONTRACT,  NEGLIGENCE OR OTHER
 * TORTIOUS  ACTION, ARISING    OUT OF OR   IN  CONNECTION  WITH THE USE    OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include <stdio.h>
#include <xorgVersion.h>

#include <misc.h>
#include <xf86.h>
#include <xf86Xinput.h>
#include <exevents.h>		/* Needed for InitValuator/Proximity stuff */
#include <xf86Opt.h>
#include <xf86_OSproc.h>

#include <math.h>
#include <xf86Module.h>


#include "jstk.h"
#include "jstk_axis.h"
#include "jstk_key.h"
#include "jstk_options.h"
#include "jstk_properties.h"
#include <xserver-properties.h>

#ifdef LINUX_BACKEND
    #include "backend_joystick.h"
#endif
#ifdef BSD_BACKEND
    #include "backend_bsd.h"
#endif
#ifdef EVDEV_BACKEND
    #include "backend_evdev.h"
#endif


#if DEBUG
    char debug_level = 0;
#endif

/*
 ***************************************************************************
 *
 * jstkOpenDevice --
 *
 * Called to open the device specified in priv
 * The compiled backends are tried one by one and return the first matching
 *
 * Returns the filedescriptor or -1 in case of error
 *
 ***************************************************************************
 */
static int
jstkOpenDevice(JoystickDevPtr priv, BOOL probe)
{
    int fd;
    fd = -1;

    if (priv->joystick_device->flags & XI86_SERVER_FD)
        priv->fd = priv->joystick_device->fd;

    if (probe == FALSE && priv->open_proc)
        return priv->open_proc(priv, probe);

#ifdef EVDEV_BACKEND
    if (fd == -1)
        fd = jstkOpenDevice_evdev(priv, probe);
#endif
#ifdef LINUX_BACKEND
    if (fd == -1)
        fd = jstkOpenDevice_joystick(priv, probe);
#endif
#ifdef BSD_BACKEND
    if (fd == -1)
        fd = jstkOpenDevice_bsd(priv, probe);
#endif

    return fd;
}

/*
 ***************************************************************************
 *
 * jstkCloseDevice --
 *
 * Called to close the device specified in priv, this is a helper for
 * backend proc_close functions
 *
 ***************************************************************************
 */
void jstkCloseDevice(JoystickDevPtr priv)
{
    if ((priv->fd >= 0)) {
      if (!(priv->joystick_device->flags & XI86_SERVER_FD))
          xf86CloseSerial(priv->fd);
      priv->fd = -1;
    }
}

/*
 ***************************************************************************
 *
 * jstkReadProc --
 *
 * Called when data is available to read from the device
 * Reads the data and process the events
 *
 ***************************************************************************
 */

static void
jstkReadProc(InputInfoPtr pInfo)
{
    JOYSTICKEVENT event;
    int number;
    int i, r;

    JoystickDevPtr priv = pInfo->private;

    do {
        if ((priv->read_proc == NULL) || 
            ((r=priv->read_proc(priv, &event, &number))==0)) {
            xf86Msg(X_WARNING, "JOYSTICK: Read failed. Deactivating device.\n");

            if (pInfo->fd >= 0)
                xf86RemoveEnabledDevice(pInfo);
            return;
        }

        /* A button's status changed */
        if (event == EVENT_BUTTON) {
            DBG(4, ErrorF("Button %d %s. Mapping: %d\n", number, 
                (priv->button[number].pressed == 0) ? "released" : "pressed", 
                priv->button[number].mapping));

            switch (priv->button[number].mapping) {
            case JSTK_MAPPING_BUTTON:
                if (priv->mouse_enabled == TRUE) {
                    xf86PostButtonEvent(pInfo->dev, 0,
                        priv->button[number].buttonnumber,
                        priv->button[number].pressed, 0, 0);
                }
                break;

            case JSTK_MAPPING_X:
            case JSTK_MAPPING_Y:
            case JSTK_MAPPING_ZX:
            case JSTK_MAPPING_ZY:
                if (priv->button[number].pressed == 0) 
                    priv->button[number].currentspeed = 1.0;
                else if (priv->mouse_enabled == TRUE)
                    jstkStartButtonAxisTimer(pInfo, number);
                break;

            case JSTK_MAPPING_KEY:
                if (priv->keys_enabled == TRUE)
                    jstkGenerateKeys(priv->keyboard_device, 
                                     priv->button[number].keys, 
                                     priv->button[number].pressed);
                break;

            case JSTK_MAPPING_SPEED_MULTIPLY:
                priv->amplify = 1.0;
                /* Calculate new amplify value by multiplying them all */
                for (i=0; i<MAXAXES; i++) {
                    if ((priv->button[i].pressed) && 
                        (priv->button[i].mapping == JSTK_MAPPING_SPEED_MULTIPLY))
                        priv->amplify *= priv->button[i].amplify;
                }
                DBG(2, ErrorF("Amplify is now %.3f\n", priv->amplify));
                break;

            case JSTK_MAPPING_DISABLE:
                if (priv->button[number].pressed == 1) {
                    if ((priv->mouse_enabled == TRUE) || 
                        (priv->keys_enabled == TRUE))
                    {
                        priv->mouse_enabled = FALSE;
                        priv->keys_enabled = FALSE;
                        DBG(2, ErrorF("All events disabled\n"));
                    } else {
                        priv->mouse_enabled = TRUE;
                        priv->keys_enabled = TRUE;
                        DBG(2, ErrorF("All events enabled\n"));
                    }
                }
                break;
            case JSTK_MAPPING_DISABLE_MOUSE:
                if (priv->button[number].pressed == 1) {
                    if (priv->mouse_enabled == TRUE) 
                        priv->mouse_enabled = FALSE;
                    else priv->mouse_enabled = TRUE;
                    DBG(2, ErrorF("Mouse events %s\n", 
                        priv->mouse_enabled ? "enabled" : "disabled"));
                }
                break;
            case JSTK_MAPPING_DISABLE_KEYS:
                if (priv->button[number].pressed == 1) {
                    if (priv->keys_enabled == TRUE) 
                        priv->keys_enabled = FALSE;
                    else priv->keys_enabled = TRUE;
                    DBG(2, ErrorF("Keyboard events %s\n", 
                        priv->mouse_enabled ? "enabled" : "disabled"));
                }
                break;

            default:
                break;
            }
        }

        /* An axis was moved */
        if ((event == EVENT_AXIS) && 
            (priv->axis[number].type != JSTK_TYPE_NONE))
        {
            DBG(5, ErrorF("Axis %d moved to %d. Type: %d, Mapping: %d\n", 
                          number,
                          priv->axis[number].value,
                          priv->axis[number].type,
                          priv->axis[number].mapping));

            if (priv->axis[number].valuator != -1)
                xf86PostMotionEvent(pInfo->dev, 1, priv->axis[number].valuator,
                                    1, priv->axis[number].value);

            switch (priv->axis[number].mapping) {
            case JSTK_MAPPING_X:
            case JSTK_MAPPING_Y:
            case JSTK_MAPPING_ZX:
            case JSTK_MAPPING_ZY:
                switch (priv->axis[number].type) {
                case JSTK_TYPE_BYVALUE:
                case JSTK_TYPE_ACCELERATED:
                    if (priv->axis[number].value == 0)
                        priv->axis[number].currentspeed = 1.0;
                    if (priv->mouse_enabled == TRUE)
                        jstkStartAxisTimer(pInfo, number);
                    break;

                case JSTK_TYPE_ABSOLUTE:
                    if (priv->mouse_enabled == TRUE)
                        jstkHandleAbsoluteAxis(pInfo, number);
                    break;
                default:
                    break;
                } /* switch (priv->axis[number].type) */
                break; /* case JSTK_MAPPING_ZY */

            case JSTK_MAPPING_KEY: if (priv->keys_enabled == TRUE) {
                if (priv->axis[number].type == JSTK_TYPE_ACCELERATED) {
                    jstkHandlePWMAxis(pInfo, number);
                } else if (priv->axis[number].type == JSTK_TYPE_BYVALUE) {
                    jstkStartAxisTimer(pInfo, number);
                }
                break;
            }

            case JSTK_MAPPING_NONE:
            default:
                break;
            } /* switch (priv->axis[number].mapping) */
        } /* if (event == EVENT_AXIS) */
    } while (r == 2);
}





static void
jstkPtrCtrlProc(DeviceIntPtr device, PtrCtrl *ctrl)
{
    /* Nothing to do, dix handles all settings */
}


/*
 ***************************************************************************
 *
 * jstkDeviceControlProc --
 *
 * Handles the initialization, etc. of a joystick
 *
 ***************************************************************************
 */

static Bool
jstkDeviceControlProc(DeviceIntPtr       pJstk,
                      int                what)
{
    int i;
    InputInfoPtr     pInfo = (InputInfoPtr)pJstk->public.devicePrivate;
    JoystickDevPtr   priv  = pInfo->private;
    Atom             btn_labels[BUTTONMAP_SIZE+1] = {0}; /* TODO: fillme */
    Atom             axes_labels[MAXAXES] = {0}; /* TODO: fillme */

    switch (what) {
    case DEVICE_INIT: {
        int m;
        char str[32];
        CARD8 buttonmap[BUTTONMAP_SIZE+1];
        DBG(1, ErrorF("jstkDeviceControlProc what=INIT\n"));
        /* Probe device and return if error */
        if (jstkOpenDevice(priv, TRUE) == -1) {
            return !Success;
        } else {
            /* Success. The OpenDevice call already did some initialization
               like priv->num_buttons, priv->num_axes */
            priv->close_proc(priv);
        }

        for (m=0; m<=BUTTONMAP_SIZE; m++) {
            sprintf(str, "Button %d", m);

            buttonmap[m] = m;
            btn_labels[m] = MakeAtom(str, strlen(str), TRUE);
        }



        if (InitButtonClassDeviceStruct(pJstk, BUTTONMAP_SIZE, 
            btn_labels,
            buttonmap) == FALSE) {
            ErrorF("unable to allocate Button class device\n");
            return !Success;
        }
        if (!InitPtrFeedbackClassDeviceStruct(pJstk, jstkPtrCtrlProc))
            return !Success;

        m = 2;
        for (i=0; i<MAXAXES; i++) 
            if (priv->axis[i].valuator != -1)
        {
            DBG(3, ErrorF("Axis %d will be valuator %d\n", i, m));
            sprintf(str, "Axis %d", i + 1);
            priv->axis[i].valuator = m++;
            axes_labels[i] = MakeAtom(str, strlen(str), TRUE);
        }

        if (InitValuatorClassDeviceStruct(pJstk, m, axes_labels,
                                          GetMotionHistorySize(),
                                          Relative) == FALSE) {
            ErrorF("unable to allocate Valuator class device\n"); 
            return !Success;
        } else {
            InitValuatorAxisStruct(pJstk,
                                   0, /* valuator num */
                                   XIGetKnownProperty(AXIS_LABEL_PROP_REL_X),
                                   0, /* min val */
                                   screenInfo.screens[0]->width, /* max val */
                                   1, /* resolution */
                                   0, /* min_res */
                                   1, /* max_res */
                                   Absolute);
            InitValuatorAxisStruct(pJstk,
                                   1, /* valuator num */
                                   XIGetKnownProperty(AXIS_LABEL_PROP_REL_Y),
                                   0, /* min val */
                                   screenInfo.screens[0]->height, /* max val */
                                   1, /* resolution */
                                   0, /* min_res */
                                   1, /* max_res */
                                   Absolute);
            for (i=0; i<MAXAXES; i++) 
                if (priv->axis[i].valuator != -1)
            {
                InitValuatorAxisStruct(pJstk,
                                       priv->axis[i].valuator,
                                       axes_labels[i],
                                       -32768, /* min val */
                                       32767,  /* max val */
                                       1, /* resolution */
                                       0, /* min_res */
                                       1, /* max_res */
                                       Absolute);
            }
            /* allocate the motion history buffer if needed */
            xf86MotionHistoryAllocate(pInfo);
        }

	jstkInitProperties(pJstk, priv);

        break;
    }

    case DEVICE_ON:
        DBG(1, ErrorF("jstkDeviceControlProc  what=ON name=%s\n", 
                      priv->device));

        if (jstkOpenDevice(priv, FALSE) != -1) {
            pJstk->public.on = TRUE;
            pInfo->fd = priv->fd;
            xf86AddEnabledDevice(pInfo);
        } else return !Success;
        break;

    case DEVICE_OFF:
    case DEVICE_CLOSE:
        if (!pJstk->public.on)
            break;
        DBG(1, ErrorF("jstkDeviceControlProc  what=%s\n", 
            (what == DEVICE_CLOSE) ? "CLOSE" : "OFF"));

        if (priv->timerrunning == TRUE) {
            priv->timerrunning = FALSE;
            TimerCancel(priv->timer);
        }
        for (i = 0; i < MAXAXES; i++)
            if (priv->axis[i].timerrunning)
        {
            priv->axis[i].timerrunning = FALSE;
            TimerCancel(priv->axis[i].timer);
        }

        if (pInfo->fd >= 0)
            xf86RemoveEnabledDevice(pInfo);
        if (!(pInfo->flags & XI86_SERVER_FD))
            pInfo->fd = -1;
        if (priv->close_proc)
            priv->close_proc(priv);
        pJstk->public.on = FALSE;
        break;

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) * 100 + GET_ABI_MINOR(ABI_XINPUT_VERSION) >= 1901
    case DEVICE_ABORT:
        break;
#endif

    default:
        ErrorF("unsupported mode=%d\n", what);
        return BadValue;
    } /* switch (what) */
    return Success;
}

/*
 ***************************************************************************
 *
 * jstkCorePreInit --
 *
 * Called when a device will be instantiated
 *
 * This is a tad complicated. NewInputDeviceRequest(), which we use to
 * hotplug a keyboard device,. enables the device, so we need to make sure
 * that all options for the dependent device are set correctly.
 *
 * This means that we parse the keyboard-specific options into the
 * keyboard device's PreInit, and re-use the keyboard's priv field.
 *
 ***************************************************************************
 */

static int
jstkCorePreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
{
    InputInfoPtr        keyboard_device;
    JoystickDevPtr      priv = NULL;
    char                *s;
    int                 i, j;

    s = xf86CheckStrOption(pInfo->options, "_source", NULL);
    if (s && strcmp(s, "_driver/joystick") == 0)
        return jstkKeyboardPreInit(drv, pInfo, flags);

    pInfo->device_control = jstkDeviceControlProc;
    pInfo->read_input = jstkReadProc;
    pInfo->control_proc = NULL;
    pInfo->switch_mode = NULL;
    if (!(pInfo->flags & XI86_SERVER_FD))
        pInfo->fd = -1;
    pInfo->dev = NULL;
    pInfo->type_name = XI_JOYSTICK;

    keyboard_device = jstkKeyboardHotplug(pInfo, flags);
    if (!keyboard_device)
        return BadAlloc;

    pInfo->private = priv = keyboard_device->private;

    priv->fd = -1;
    priv->open_proc = NULL;
    priv->read_proc = NULL;
    priv->close_proc = NULL;
    priv->device = NULL;
    priv->devicedata = NULL;
    priv->timer = NULL;
    priv->timerrunning = FALSE;
    priv->mouse_enabled = TRUE;
    priv->keys_enabled = TRUE;
    priv->amplify = 1.0f;
    priv->joystick_device = pInfo;
    priv->keyboard_device = keyboard_device;
    priv->num_axes    = MAXAXES;
    priv->num_buttons = MAXBUTTONS;

    /* Initialize default mappings */
    for (i=0; i<MAXAXES; i++) {
        priv->axis[i].value        = 0;
        priv->axis[i].oldvalue     = 0;
        priv->axis[i].deadzone     = 5000;
        priv->axis[i].type         = JSTK_TYPE_NONE;
        priv->axis[i].mapping      = JSTK_MAPPING_NONE;
        priv->axis[i].currentspeed = 0.0f;
        priv->axis[i].amplify      = 1.0f;
        priv->axis[i].valuator     = -1;
        priv->axis[i].subpixel     = 0.0f;
        priv->axis[i].timer        = NULL;
        priv->axis[i].timerrunning = FALSE;
        priv->axis[i].key_isdown   = 0;
        for (j=0; j<MAXKEYSPERBUTTON; j++)
            priv->axis[i].keys_low[j] = priv->axis[i].keys_high[j] = 0;
    }
    for (i=0; i<MAXBUTTONS; i++) {
        priv->button[i].pressed      = 0;
        priv->button[i].buttonnumber = 0;
        priv->button[i].mapping      = JSTK_MAPPING_NONE;
        priv->button[i].currentspeed = 1.0f;
        priv->button[i].subpixel     = 0.0f;
        priv->button[i].amplify      = 1.0;
        for (j=0; j<MAXKEYSPERBUTTON; j++)
            priv->button[i].keys[j] = 0;
    }

    /* First three joystick buttons generate mouse clicks */
    priv->button[0].mapping      = JSTK_MAPPING_BUTTON;
    priv->button[0].buttonnumber = 1;
    priv->button[1].mapping      = JSTK_MAPPING_BUTTON;
    priv->button[1].buttonnumber = 2;
    priv->button[2].mapping      = JSTK_MAPPING_BUTTON;
    priv->button[2].buttonnumber = 3;

    /* First two axes are a stick for moving */
    priv->axis[0].type      = JSTK_TYPE_BYVALUE;
    priv->axis[0].mapping   = JSTK_MAPPING_X;
    priv->axis[1].type      = JSTK_TYPE_BYVALUE;
    priv->axis[1].mapping   = JSTK_MAPPING_Y;

    /* Next two axes are a stick for scrolling */
    priv->axis[2].type      = JSTK_TYPE_BYVALUE;
    priv->axis[2].mapping   = JSTK_MAPPING_ZX;
    priv->axis[3].type      = JSTK_TYPE_BYVALUE;
    priv->axis[3].mapping   = JSTK_MAPPING_ZY;

    /* Next two axes are a pad for moving */
    priv->axis[4].type      = JSTK_TYPE_ACCELERATED;
    priv->axis[4].mapping   = JSTK_MAPPING_X;
    priv->axis[5].type      = JSTK_TYPE_ACCELERATED;
    priv->axis[5].mapping   = JSTK_MAPPING_Y;

    /* Joystick device is mandatory */
    priv->device = xf86SetStrOption(pInfo->options, "Device", NULL);
    if (!priv->device)
        priv->device = xf86SetStrOption(pInfo->options, "Path", NULL);

    if (!priv->device) {
        xf86Msg (X_ERROR, "%s: No Device specified.\n", pInfo->name);
        goto SetupProc_fail;
    }

#if DEBUG
    debug_level = xf86SetIntOption(pInfo->options, "DebugLevel", 0);
    if (debug_level > 0) {
        xf86Msg(X_CONFIG, "%s: debug level set to %d\n", 
                pInfo->name, debug_level);
    }
#else
    if (xf86SetIntOption(pInfo->options, "DebugLevel", 0) != 0) {
        xf86Msg(X_WARNING, "%s: DebugLevel: Compiled without debugging support!\n", 
                pInfo->name);
    }
#endif

    priv->mouse_enabled = xf86SetBoolOption(pInfo->options, "StartMouseEnabled", TRUE);
    priv->keys_enabled = xf86SetBoolOption(pInfo->options, "StartKeysEnabled", TRUE);

    /* Process button mapping options */
    for (i=0; i<MAXBUTTONS; i++) {
        char p[64];
        sprintf(p,"MapButton%d",i+1);
        s = xf86SetStrOption(pInfo->options, p, NULL);
        if (s != NULL) {
            jstkParseButtonOption(s, priv, i, pInfo->name);
        }
        DBG(1, xf86Msg(X_CONFIG, "Button %d mapped to %d\n", i+1, 
                       priv->button[i].mapping));
    }

    /* Process button mapping options */
    for (i=0; i<MAXAXES; i++) {
        char p[64];
        sprintf(p,"MapAxis%d",i+1);
        s = xf86SetStrOption(pInfo->options, p, NULL);
        if (s != NULL) {
            jstkParseAxisOption(s, priv, &priv->axis[i], pInfo->name);
        }
        DBG(1, xf86Msg(X_CONFIG, 
                       "Axis %d type is %d, mapped to %d, amplify=%.3f\n", i+1, 
                       priv->axis[i].type,
                       priv->axis[i].mapping,
                       priv->axis[i].amplify));
    }

    return Success;

SetupProc_fail:
    if (priv) {
        free(priv);
        if (keyboard_device)
            keyboard_device->private = NULL;
    }
    if (pInfo)
        pInfo->private = NULL;
    return BadValue;
}



/*
 ***************************************************************************
 *
 * jstkCoreUnInit --
 *
 * Called when a device is unplugged and needs to be removed
 * This is a bit tricky, because the keyboard device and the main device
 * share the same private data, which must be freed only once, which is done
 * by the main device.
 * 
 *
 ***************************************************************************
 */

static void
jstkCoreUnInit(InputDriverPtr    drv,
               InputInfoPtr      pInfo,
               int               flags)
{
    if (pInfo->private) {
	JoystickDevPtr priv = (JoystickDevPtr) pInfo->private;
	if (priv->keyboard_device == pInfo) {
	    /* this is the keyboard device */
	    /* Unlink from private data to notify that the 
	     * keyboard device is no more, but don't free */
	    priv->keyboard_device = NULL;
	} else {
	    /* freeing main device
	       if keyboard still exists, notify keyboard device that it's
	       private data is gone */
	    if (priv->keyboard_device)
		priv->keyboard_device->private = NULL;
	    
	    free (priv);
	}
    }

    pInfo->private = NULL;
    xf86DeleteInput(pInfo, 0);
}




_X_EXPORT InputDriverRec JOYSTICK = {
    1,
    "joystick",
    NULL,
    jstkCorePreInit,
    jstkCoreUnInit,
    NULL,
    NULL,
#ifdef XI86_DRV_CAP_SERVER_FD
    XI86_DRV_CAP_SERVER_FD
#endif
};

/*
 ***************************************************************************
 *
 * jstkPlug --
 *
 * Called when the driver is loaded
 *
 ***************************************************************************
 */

static pointer
jstkDriverPlug(pointer  module,
               pointer  options,
               int      *errmaj,
               int      *errmin)
{
    xf86AddInputDriver(&JOYSTICK, module, 0);
    return module;
}


/*
 ***************************************************************************
 *
 * jstkDriverUnplug --
 *
 * Called when the driver is unloaded
 *
 ***************************************************************************
 */

static void
jstkDriverUnplug(pointer p)
{
}



/*
 ***************************************************************************
 *
 * Module information for X.Org
 *
 ***************************************************************************
 */
static XF86ModuleVersionInfo jstkVersionRec =
{
    "joystick",
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    PACKAGE_VERSION_MAJOR,
    PACKAGE_VERSION_MINOR,
    PACKAGE_VERSION_PATCHLEVEL,
    ABI_CLASS_XINPUT,
    ABI_XINPUT_VERSION,
    MOD_CLASS_XINPUT,
    {0, 0, 0, 0}   /* signature, to be patched into the file by */
                   /* a tool */
};

/*
 ***************************************************************************
 *
 * Exported module Data for X.Org
 *
 ***************************************************************************
 */
_X_EXPORT XF86ModuleData joystickModuleData = {
    &jstkVersionRec,
    jstkDriverPlug,
    jstkDriverUnplug
};

/* vim: set filetype=c.doxygen ts=4 et: */
