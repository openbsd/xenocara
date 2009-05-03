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


#include <xf86.h>
#include <xf86Xinput.h>
#include <X11/extensions/XKBsrv.h>
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include <xf86Optrec.h>

#include "jstk.h"
#include "jstk_key.h"

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 5
#include <xkbsrv.h>
#endif

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
    KeySymsRec keySyms;
    CARD8 modMap[MAP_LENGTH];
    KeySym sym;
    int i, j;
    XkbSrvInfoPtr xkbi;
    XkbControlsPtr ctrls;

    static struct { KeySym keysym; CARD8 mask; } modifiers[] = {
        { XK_Shift_L,           ShiftMask },
        { XK_Shift_R,           ShiftMask },
        { XK_Control_L,         ControlMask },
        { XK_Control_R,         ControlMask },
        { XK_Caps_Lock,         LockMask },
        { XK_Alt_L,             AltMask },
        { XK_Alt_R,             AltMask },
        { XK_Num_Lock,          NumLockMask },
        { XK_Scroll_Lock,       ScrollLockMask },
        { XK_Mode_switch,       AltLangMask }
    };

    DBG(1, xf86Msg(X_CONFIG, "Initializing Keyboard with %d keys\n",
                   priv->keymap.size));
    for (i = 0; i < priv->keymap.size; i++)
    {
	DBG(6, xf86Msg(X_CONFIG, "Keymap [%d]: 0x%08X\n", MIN_KEYCODE+i,(unsigned int)priv->keymap.map[i]));
    }

    memset(modMap, 0, sizeof(modMap));


    keySyms.map        = priv->keymap.map;
    keySyms.mapWidth   = 1;
    keySyms.minKeyCode = MIN_KEYCODE;
/*    keySyms.maxKeyCode = MIN_KEYCODE + priv->keymap.size - 1; */
    keySyms.maxKeyCode = MIN_KEYCODE + sizeof(priv->keymap.map)/sizeof(priv->keymap.map[0]) - 1;

    for (i = 0; i < priv->keymap.size; i++) {
        sym = priv->keymap.map[i];
        for (j = 0; j < sizeof(modifiers)/sizeof(modifiers[0]); j++) {
            if (modifiers[j].keysym == sym)
                modMap[i + MIN_KEYCODE] = modifiers[j].mask;
        }
    }

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 5
    {
        XkbRMLVOSet rmlvo;
        XkbGetRulesDflts(&rmlvo);
    	/* FIXME */
	#warning KEYMAP FOR ABI_XINPUT_VERSION >= 5 BROKEN RIGHT NOW
        if (!InitKeyboardDeviceStruct(pJstk, &rmlvo, NULL, jstkKbdCtrl))
        {
            ErrorF("unable to init keyboard device\n");
            return !Success;
        }
    }
#else
    if (InitKeyboardDeviceStruct((DevicePtr)pJstk, &keySyms, modMap, NULL, jstkKbdCtrl) == FALSE) {
	ErrorF("unable to init keyboard device\n");
        return !Success;
    }
#endif

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
jstkGenerateKeys(LocalDevicePtr device, KEYSCANCODES keys, char pressed)
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
            k = k + MIN_KEYCODE;
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
static Bool
jstkKeyboardDeviceControlProc(DeviceIntPtr       dev,
                              int                what)
{
    JoystickDevPtr priv  = (JoystickDevPtr)XI_PRIVATE(dev);
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
    }

    return Success;
}


/*
 ***************************************************************************
 *
 * jstkKeyboardPreInit --
 *
 * Called manually to create a keyboard device for the joystick
 *
 ***************************************************************************
 */
InputInfoPtr
jstkKeyboardPreInit(InputDriverPtr drv, IDevPtr _dev, int flags)
{
    LocalDevicePtr local = NULL;
    IDevPtr dev = NULL;
    char name[512] = {0};

    local = xf86AllocateInput(drv, 0);
    if (!local) {
        goto SetupProc_fail;
    }

    dev = xcalloc(sizeof(IDevRec), 1);
    strcpy(name, _dev->identifier);
    strcat(name, " (keys)");
    dev->identifier = xstrdup(name);
    dev->driver = xstrdup(_dev->driver);
    dev->commonOptions = (pointer)xf86optionListDup(_dev->commonOptions);
    dev->extraOptions = (pointer)xf86optionListDup(_dev->extraOptions);

    local->name   = dev->identifier;
    local->flags  = XI86_KEYBOARD_CAPABLE;
    local->device_control = jstkKeyboardDeviceControlProc;
    local->read_input = NULL;
    local->close_proc = NULL;
    local->control_proc = NULL;
    local->switch_mode = NULL;
    local->conversion_proc = NULL;
    local->fd = -1;
    local->dev = NULL;
    local->private = NULL;
    local->type_name = XI_JOYSTICK;
    local->history_size = 0;
    local->always_core_feedback = 0;
    local->conf_idev = dev;

    xf86CollectInputOptions(local, NULL, NULL);
    xf86OptionListReport(local->options);
    xf86ProcessCommonOptions(local, local->options);


    /* return the LocalDevice */
    local->flags |= XI86_CONFIGURED;

    return (local);

SetupProc_fail:
    if (local)
        local->private = NULL;
    if (dev) {
        if (dev->identifier) xfree(dev->identifier);
        if (dev->driver) xfree(dev->driver);
        xfree(dev);
    }
    return NULL;
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
                   LocalDevicePtr    local,
                   int               flags)
{
    JoystickDevPtr device = (JoystickDevPtr) local->private;
    DBG(2, ErrorF("jstkKeyboardUnInit.\n"));

    device->keyboard_device = NULL;
    local->private = NULL;

    xf86DeleteInput(local, 0);
}

