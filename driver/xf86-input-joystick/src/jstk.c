/*
 * Copyright 2007-2008 by Sascha Hlusiak. <saschahlusiak@freedesktop.org>     
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

#include <stdio.h>
#include <xorgVersion.h>

#include <misc.h>
#include <xf86.h>
#include <xf86Xinput.h>
#include <exevents.h>		/* Needed for InitValuator/Proximity stuff */
#include <xf86Opt.h>

#include <math.h>
#include <xf86Module.h>


#include "jstk.h"
#include "jstk_axis.h"
#include "jstk_key.h"
#include "jstk_options.h"
#include "jstk_properties.h"

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
 * jstkConvertProc --
 *
 * Convert valuators to X and Y.
 *
 ***************************************************************************
 */

static Bool
jstkConvertProc(LocalDevicePtr	local,
                int             first,
                int             num,
                int             v0,
                int             v1,
                int             v2,
                int             v3,
                int             v4,
                int             v5,
                int*            x,
                int*            y)
{
    if (first != 0 || num != 2)
        return FALSE;

    *x = v0;
    *y = v1;

    return TRUE;
}


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
 * jstkReadProc --
 *
 * Called when data is available to read from the device
 * Reads the data and process the events
 *
 ***************************************************************************
 */

static void
jstkReadProc(LocalDevicePtr local)
{
    JOYSTICKEVENT event;
    int number;
    int i, r;

    JoystickDevPtr priv = local->private;

    do {
        if ((priv->read_proc == NULL) || 
            ((r=priv->read_proc(priv, &event, &number))==0)) {
            xf86Msg(X_WARNING, "JOYSTICK: Read failed. Deactivating device.\n");

            if (local->fd >= 0)
                RemoveEnabledDevice(local->fd);
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
                    xf86PostButtonEvent(local->dev, 0,
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
                    jstkStartButtonAxisTimer(local, number);
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
                xf86PostMotionEvent(local->dev, 1, priv->axis[number].valuator, 
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
                        jstkStartAxisTimer(local, number);
                    break;

                case JSTK_TYPE_ABSOLUTE:
                    if (priv->mouse_enabled == TRUE)
                        jstkHandleAbsoluteAxis(local, number);
                    break;
                default:
                    break;
                } /* switch (priv->axis[number].type) */
                break; /* case JSTK_MAPPING_ZY */

            case JSTK_MAPPING_KEY: if (priv->keys_enabled == TRUE) {
                if (priv->axis[number].type == JSTK_TYPE_ACCELERATED) {
                    jstkHandlePWMAxis(local, number);
                } else if (priv->axis[number].type == JSTK_TYPE_BYVALUE) {
                    jstkStartAxisTimer(local, number);
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
    LocalDevicePtr   local = (LocalDevicePtr)pJstk->public.devicePrivate;
    JoystickDevPtr   priv  = (JoystickDevPtr)XI_PRIVATE(pJstk);

    switch (what) {
    case DEVICE_INIT: {
        int m;
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

        for (m=0; m<=BUTTONMAP_SIZE; m++)
            buttonmap[m] = m;
        if (InitButtonClassDeviceStruct(pJstk, BUTTONMAP_SIZE, 
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
            priv->axis[i].valuator = m++;
        }

        if (InitValuatorClassDeviceStruct(pJstk, 
                                          m,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 3
                                          xf86GetMotionEvents, 
#endif
                                          local->history_size,
                                          Relative) == FALSE) {
            ErrorF("unable to allocate Valuator class device\n"); 
            return !Success;
        } else {
            InitValuatorAxisStruct(pJstk,
                                   0, /* valuator num */
                                   0, /* min val */
                                   screenInfo.screens[0]->width, /* max val */
                                   1, /* resolution */
                                   0, /* min_res */
                                   1); /* max_res */
            InitValuatorAxisStruct(pJstk,
                                   1, /* valuator num */
                                   0, /* min val */
                                   screenInfo.screens[0]->height, /* max val */
                                   1, /* resolution */
                                   0, /* min_res */
                                   1); /* max_res */
            for (i=0; i<MAXAXES; i++) 
                if (priv->axis[i].valuator != -1)
            {
                InitValuatorAxisStruct(pJstk,
                                       priv->axis[i].valuator,
                                       -32768, /* min val */
                                       32767,  /* max val */
                                       1, /* resolution */
                                       0, /* min_res */
                                       1); /* max_res */
            }
            /* allocate the motion history buffer if needed */
            xf86MotionHistoryAllocate(local);
        }


        if (priv->keyboard_device != NULL)
        {
            DBG(2, ErrorF("Activating keyboard device\n"));
            xf86ActivateDevice(priv->keyboard_device);
            priv->keyboard_device->dev->inited = 
                (priv->keyboard_device->device_control(priv->keyboard_device->dev, DEVICE_INIT) == Success);
            xf86EnableDevice(priv->keyboard_device->dev);
            DBG(2, ErrorF("Keyboard device activated\n"));
        }

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 3
	jstkInitProperties(pJstk, priv);
#endif

        break;
    }

    case DEVICE_ON:
        DBG(1, ErrorF("jstkDeviceControlProc  what=ON name=%s\n", 
                      priv->device));

        if (jstkOpenDevice(priv, FALSE) != -1) {
            pJstk->public.on = TRUE;
            local->fd = priv->fd;
            AddEnabledDevice(local->fd);
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

        if (local->fd >= 0)
            RemoveEnabledDevice(local->fd);
        local->fd = -1;
        if (priv->close_proc)
            priv->close_proc(priv);
        pJstk->public.on = FALSE;
        break;

    default:
        ErrorF("unsupported mode=%d\n", what);
        return !Success;
        break;
    } /* switch (what) */
    return Success;
}




_X_EXPORT InputDriverRec JSTK_KEYBOARD = {
    1,
    "joystick_keyboard",
    NULL,
    jstkKeyboardPreInit,
    jstkKeyboardUnInit,
    NULL,
    0
};

/*
 ***************************************************************************
 *
 * jstkCorePreInit --
 *
 * Called when a device will be instantiated
 *
 ***************************************************************************
 */

static InputInfoPtr
jstkCorePreInit(InputDriverPtr drv, IDevPtr dev, int flags)
{
    LocalDevicePtr      local = NULL;
    JoystickDevPtr      priv = NULL;
    char                *s;
    int                 i, j;

    local = xf86AllocateInput(drv, 0);
    if (!local) {
        goto SetupProc_fail;
    }

    local->private = (JoystickDevPtr)xalloc(sizeof(JoystickDevRec));
    priv = (JoystickDevPtr)local->private;

    local->name   = dev->identifier;
    local->flags  = XI86_POINTER_CAPABLE;
    local->flags |= XI86_SEND_DRAG_EVENTS;
    local->device_control = jstkDeviceControlProc;
    local->read_input = jstkReadProc;
    local->close_proc = NULL;
    local->control_proc = NULL;
    local->switch_mode = NULL;
    local->conversion_proc = jstkConvertProc;
    local->fd = -1;
    local->dev = NULL;
    local->private = priv;
    local->type_name = XI_JOYSTICK;
    local->history_size = 0;
    local->always_core_feedback = NULL;
    local->conf_idev = dev;

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
    priv->keyboard_device = NULL;
    priv->keymap.size = 1;
    memset(priv->keymap.map, NoSymbol, sizeof(priv->keymap.map));
    priv->repeat_delay = 0;
    priv->repeat_interval = 0;
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

    xf86CollectInputOptions(local, NULL, NULL);
    xf86OptionListReport(local->options);

    /* Joystick device is mandatory */
    priv->device = xf86SetStrOption(dev->commonOptions, "Device", NULL);
    if (!priv->device)
        priv->device = xf86SetStrOption(dev->commonOptions, "Path", NULL);

    if (!priv->device) {
        xf86Msg (X_ERROR, "%s: No Device specified.\n", local->name);
        goto SetupProc_fail;
    }

    xf86ProcessCommonOptions(local, local->options);

#if DEBUG
    debug_level = xf86SetIntOption(dev->commonOptions, "DebugLevel", 0);
    if (debug_level > 0) {
        xf86Msg(X_CONFIG, "%s: debug level set to %d\n", 
                local->name, debug_level);
    }
#else
    if (xf86SetIntOption(dev->commonOptions, "DebugLevel", 0) != 0) {
        xf86Msg(X_WARNING, "%s: DebugLevel: Compiled without debugging support!\n", 
                local->name);
    }
#endif

    /* Parse option for autorepeat */
    if ((s = xf86SetStrOption(dev->commonOptions, "AutoRepeat", NULL))) {
        int delay, rate;
        if (sscanf(s, "%d %d", &delay, &rate) != 2) {
            xf86Msg(X_ERROR, "%s: \"%s\" is not a valid AutoRepeat value", 
                    local->name, s);
        } else {
            priv->repeat_delay = delay;
            if (rate != 0)
                priv->repeat_interval = 1000/rate;
            else priv->repeat_interval = 0;
            DBG(1, xf86Msg(X_CONFIG, "Autorepeat set to delay=%d, interval=%d\n",
                           priv->repeat_delay,priv->repeat_interval));
        }
        xfree(s);
    }

    priv->mouse_enabled = xf86SetBoolOption(dev->commonOptions, "StartMouseEnabled", TRUE);
    priv->keys_enabled = xf86SetBoolOption(dev->commonOptions, "StartKeysEnabled", TRUE);

    /* Process button mapping options */
    for (i=0; i<MAXBUTTONS; i++) {
        char p[64];
        sprintf(p,"MapButton%d",i+1);
        s = xf86SetStrOption(dev->commonOptions, p, NULL);
        if (s != NULL) {
            jstkParseButtonOption(s, priv, i, local->name);
        }
        DBG(1, xf86Msg(X_CONFIG, "Button %d mapped to %d\n", i+1, 
                       priv->button[i].mapping));
    }

    /* Process button mapping options */
    for (i=0; i<MAXAXES; i++) {
        char p[64];
        sprintf(p,"MapAxis%d",i+1);
        s = xf86SetStrOption(dev->commonOptions, p, NULL);
        if (s != NULL) {
            jstkParseAxisOption(s, priv, &priv->axis[i], local->name);
        }
        DBG(1, xf86Msg(X_CONFIG, 
                       "Axis %d type is %d, mapped to %d, amplify=%.3f\n", i+1, 
                       priv->axis[i].type,
                       priv->axis[i].mapping,
                       priv->axis[i].amplify));
    }

    /* return the LocalDevice */
    local->flags |= XI86_CONFIGURED;

    priv->keyboard_device = jstkKeyboardPreInit(&JSTK_KEYBOARD, dev, flags);
    if (priv->keyboard_device) {
        priv->keyboard_device->private = priv;
    }

    return (local);

SetupProc_fail:
    if (priv)
        xfree(priv);
    if (local)
        local->private = NULL;
    return NULL;
/*    return (local); */ /* Makes X segfault on error */
}



/*
 ***************************************************************************
 *
 * jstkCoreUnInit --
 *
 * Called when a device is unplugged and needs to be removed
 *
 ***************************************************************************
 */

static void
jstkCoreUnInit(InputDriverPtr    drv,
               LocalDevicePtr    local,
               int               flags)
{
    JoystickDevPtr device = (JoystickDevPtr) local->private;

    if (device->keyboard_device != NULL)
    {
        xf86DisableDevice(device->keyboard_device->dev, TRUE);
        device->keyboard_device = NULL;
    }

    xfree (device);
    local->private = NULL;
    xf86DeleteInput(local, 0);
}




_X_EXPORT InputDriverRec JOYSTICK = {
    1,
    "joystick",
    NULL,
    jstkCorePreInit,
    jstkCoreUnInit,
    NULL,
    0
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
    xf86AddInputDriver(&JSTK_KEYBOARD, module, 0);
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
