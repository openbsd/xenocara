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


#include <xorg-server.h>
#include <xf86.h>
#include <xf86Xinput.h>
#include <X11/extensions/XKBsrv.h>
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include <xf86Optrec.h>

#include "jstk.h"
#include "jstk_key.h"

#include <xkbsrv.h>

#define MIN_KEYCODE 8
#define GLYPHS_PER_KEY 2

#define AltMask		Mod1Mask
#define NumLockMask	Mod2Mask
#define AltLangMask	Mod3Mask
#define KanaMask	Mod4Mask
#define ScrollLockMask	Mod5Mask

static void
jstkKbdCtrl(DeviceIntPtr device, KeybdCtrl *ctrl)
{
}


/*
 ***************************************************************************
 *
 * jstkInitKeys --
 *
 * Sets up the keymap, modmap and the keyboard device structs
 *
 ***************************************************************************
 */
static int
jstkInitKeys(DeviceIntPtr pJstk, JoystickDevPtr priv)
{
    XkbSrvInfoPtr xkbi;
    XkbControlsPtr ctrls;

    if (!InitKeyboardDeviceStruct(pJstk, &priv->rmlvo, NULL, jstkKbdCtrl))
    {
        ErrorF("unable to init keyboard device\n");
        return !Success;
    }

    /* Set Autorepeat and Delay */
    if ((priv->repeat_delay || priv->repeat_interval) && 
        pJstk->key && 
        pJstk->key->xkbInfo)
    {
        xkbi = pJstk->key->xkbInfo;
        if (xkbi && xkbi->desc)
        {
            ctrls = xkbi->desc->ctrls;
            ctrls->repeat_delay = priv->repeat_delay;
            ctrls->repeat_interval = priv->repeat_interval;
        }
    }

    return Success;
}



/*
 ***************************************************************************
 *
 * jstkGenerateKeys
 *
 * Generates a series of keydown or keyup events of the specified 
 * KEYSCANCODES
 *
 ***************************************************************************
 */
void
jstkGenerateKeys(InputInfoPtr device, KEYSCANCODES keys, char pressed)
{
    int i;
    unsigned int k;

    if (device == NULL)
        return;
    for (i=0;i<MAXKEYSPERBUTTON;i++) {
        if (pressed != 0) 
            k = keys[i];
        else k = keys[MAXKEYSPERBUTTON - i - 1];

        if (k != 0) {
            DBG(2, ErrorF("Generating key %s event with keycode %d\n", 
                (pressed)?"press":"release", k));
            xf86PostKeyboardEvent(device->dev, k, pressed);
        }
    }
}


/*
 ***************************************************************************
 *
 * jstkKeyboardDeviceControlProc --
 *
 * Handles the initialization, etc. of the keyboard device
 *
 ***************************************************************************
 */
Bool
jstkKeyboardDeviceControlProc(DeviceIntPtr       dev,
                              int                what)
{
    InputInfoPtr pInfo = dev->public.devicePrivate;
    JoystickDevPtr priv  = pInfo->private;
    if (!priv) {
        DBG(2, ErrorF("jstkKeyboardDeviceControlProc: priv == NULL\n"));
        return !Success;
    }
    switch (what) {
    case DEVICE_INIT:
        DBG(2, ErrorF("jstkKeyboardDeviceControlProc what=DEVICE_INIT\n"));
        if (InitFocusClassDeviceStruct(dev) == FALSE) {
            ErrorF("unable to init Focus class device\n");
            return !Success;
        }
        if (jstkInitKeys(dev, priv) != Success)
            return !Success;
        break;
    case DEVICE_ON:
        DBG(2, ErrorF("jstkKeyboardDeviceControlProc what=DEVICE_ON\n"));
        dev->public.on = TRUE;
        break;
    case DEVICE_OFF:
        DBG(2, ErrorF("jstkKeyboardDeviceControlProc what=DEVICE_OFF\n"));
        dev->public.on = FALSE;
        break;
    case DEVICE_CLOSE:
        DBG(2, ErrorF("jstkKeyboardDeviceControlProc what=DEVICE_CLOSE\n"));
        dev->public.on = FALSE;
        break;
    default:
        return BadValue;
    }

    return Success;
}


/*
 ***************************************************************************
 *
 * jstkKeyboardPreInit --
 *
 * See comment in jstkCorePreInit() for details.
 *
 ***************************************************************************
 */

int jstkKeyboardPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
{
    JoystickDevPtr priv = NULL;
    char *s;

    pInfo->private = priv = calloc(1, sizeof(JoystickDevRec));
    if (!priv)
        return BadAlloc;

    pInfo->device_control = jstkKeyboardDeviceControlProc;
    pInfo->read_input = NULL;
    pInfo->control_proc = NULL;
    pInfo->switch_mode = NULL;
    if (!(pInfo->flags & XI86_SERVER_FD))
        pInfo->fd = -1;
    pInfo->type_name = XI_JOYSTICK;

    /* parse keyboard-related options */
    priv->repeat_delay = 0;
    priv->repeat_interval = 0;

    /* Parse option for autorepeat */
    if ((s = xf86SetStrOption(pInfo->options, "AutoRepeat", NULL))) {
        int delay, rate;
        if (sscanf(s, "%d %d", &delay, &rate) != 2) {
            xf86Msg(X_ERROR, "%s: \"%s\" is not a valid AutoRepeat value",
                    pInfo->name, s);
        } else {
            priv->repeat_delay = delay;
            if (rate != 0)
                priv->repeat_interval = 1000/rate;
            else priv->repeat_interval = 0;
            DBG(1, xf86Msg(X_CONFIG, "Autorepeat set to delay=%d, interval=%d\n",
                           priv->repeat_delay,priv->repeat_interval));
        }
        free(s);
    }

    priv->rmlvo.rules = xf86SetStrOption(pInfo->options, "xkb_rules", NULL);
    if (!priv->rmlvo.rules)
	priv->rmlvo.rules = xf86SetStrOption(pInfo->options, "XkbRules", "evdev");

    priv->rmlvo.model = xf86SetStrOption(pInfo->options, "xkb_model", NULL);
    if (!priv->rmlvo.model)
	priv->rmlvo.model = xf86SetStrOption(pInfo->options, "XkbModel", "evdev");

    priv->rmlvo.layout = xf86SetStrOption(pInfo->options, "xkb_layout", NULL);
    if (!priv->rmlvo.layout)
	priv->rmlvo.layout = xf86SetStrOption(pInfo->options, "XkbLayout", "us");

    priv->rmlvo.variant = xf86SetStrOption(pInfo->options, "xkb_variant", NULL);
    if (!priv->rmlvo.variant)
	priv->rmlvo.variant = xf86SetStrOption(pInfo->options, "XkbVariant", "");

    priv->rmlvo.options = xf86SetStrOption(pInfo->options, "xkb_options", NULL);
    if (!priv->rmlvo.options)
	priv->rmlvo.options = xf86SetStrOption(pInfo->options, "XkbOptions", "");

    return Success;
}

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 14
static InputOption*
input_option_new(InputOption* list, char *key, char *value)
{
    InputOption *tmp;

    tmp = calloc(1, sizeof(*tmp));
    tmp->key = key;
    tmp->value = value;
    tmp->next = list;

    return tmp;
}

static void
input_option_free_list(InputOption **list)
{
    InputOption *iopts = *list;

    while(iopts)
    {
        InputOption *tmp = iopts->next;
        free(iopts->key);
        free(iopts->value);
        free(iopts);
        iopts = tmp;
    }

    *list = NULL;
}

#endif

InputInfoPtr
jstkKeyboardHotplug(InputInfoPtr pInfo, int flags)
{
    int rc;
    char name[512] = {0};
    InputAttributes *attrs = NULL;
    InputOption *iopts = NULL;
    DeviceIntPtr dev;
    XF86OptionPtr opts;

    /* duplicate option list, append to name */
    opts = xf86OptionListDuplicate(pInfo->options);
    strcpy(name, pInfo->name);
    strcat(name, " (keys)");
    opts = xf86ReplaceStrOption(opts, "Name", name);
    opts = xf86ReplaceStrOption(opts, "_source", "_driver/joystick");
    opts = xf86AddNewOption(opts, "Driver", pInfo->driver);

    while(opts)
    {
        iopts = input_option_new(iopts,
                                 xf86OptionName(opts),
                                 xf86OptionValue(opts));
        opts = xf86NextOption(opts);
    }

    /* duplicate attribute list */
    attrs = DuplicateInputAttributes(pInfo->attrs);

    rc = NewInputDeviceRequest(iopts, attrs, &dev);

    input_option_free_list(&iopts);

    FreeInputAttributes(attrs);

    return (rc == Success) ? dev->public.devicePrivate : NULL;
}


/*
 ***************************************************************************
 *
 * jstkKeyboardUnInit --
 *
 * Called when the keyboard slave device gets removed
 *
 ***************************************************************************
 */
void
jstkKeyboardUnInit(InputDriverPtr    drv,
                   InputInfoPtr      pInfo,
                   int               flags)
{
    JoystickDevPtr device = (JoystickDevPtr) pInfo->private;
    DBG(2, ErrorF("jstkKeyboardUnInit.\n"));

    device->keyboard_device = NULL;
    pInfo->private = NULL;

    xf86DeleteInput(pInfo, 0);
}

