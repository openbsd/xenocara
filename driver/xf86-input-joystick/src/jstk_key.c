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

#define MIN_KEYCODE 8
#define GLYPHS_PER_KEY 2

#define AltMask		Mod1Mask
#define NumLockMask	Mod2Mask
#define AltLangMask	Mod3Mask
#define KanaMask	Mod4Mask
#define ScrollLockMask	Mod5Mask



#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 5
static KeySym map[] = {
    /* 0x00 */  NoSymbol,       NoSymbol,
    /* 0x01 */  XK_Escape,      NoSymbol,
    /* 0x02 */  XK_1,           XK_exclam,
    /* 0x03 */  XK_2,           XK_at,
    /* 0x04 */  XK_3,           XK_numbersign,
    /* 0x05 */  XK_4,           XK_dollar,
    /* 0x06 */  XK_5,           XK_percent,
    /* 0x07 */  XK_6,           XK_asciicircum,
    /* 0x08 */  XK_7,           XK_ampersand,
    /* 0x09 */  XK_8,           XK_asterisk,
    /* 0x0a */  XK_9,           XK_parenleft,
    /* 0x0b */  XK_0,           XK_parenright,
    /* 0x0c */  XK_minus,       XK_underscore,
    /* 0x0d */  XK_equal,       XK_plus,
    /* 0x0e */  XK_BackSpace,   NoSymbol,
    /* 0x0f */  XK_Tab,         XK_ISO_Left_Tab,
    /* 0x10 */  XK_Q,           NoSymbol,
    /* 0x11 */  XK_W,           NoSymbol,
    /* 0x12 */  XK_E,           NoSymbol,
    /* 0x13 */  XK_R,           NoSymbol,
    /* 0x14 */  XK_T,           NoSymbol,
    /* 0x15 */  XK_Y,           NoSymbol,
    /* 0x16 */  XK_U,           NoSymbol,
    /* 0x17 */  XK_I,           NoSymbol,
    /* 0x18 */  XK_O,           NoSymbol,
    /* 0x19 */  XK_P,           NoSymbol,
    /* 0x1a */  XK_bracketleft, XK_braceleft,
    /* 0x1b */  XK_bracketright,XK_braceright,
    /* 0x1c */  XK_Return,      NoSymbol,
    /* 0x1d */  XK_Control_L,   NoSymbol,
    /* 0x1e */  XK_A,           NoSymbol,
    /* 0x1f */  XK_S,           NoSymbol,
    /* 0x20 */  XK_D,           NoSymbol,
    /* 0x21 */  XK_F,           NoSymbol,
    /* 0x22 */  XK_G,           NoSymbol,
    /* 0x23 */  XK_H,           NoSymbol,
    /* 0x24 */  XK_J,           NoSymbol,
    /* 0x25 */  XK_K,           NoSymbol,
    /* 0x26 */  XK_L,           NoSymbol,
    /* 0x27 */  XK_semicolon,   XK_colon,
    /* 0x28 */  XK_quoteright,  XK_quotedbl,
    /* 0x29 */  XK_quoteleft,	XK_asciitilde,
    /* 0x2a */  XK_Shift_L,     NoSymbol,
    /* 0x2b */  XK_backslash,   XK_bar,
    /* 0x2c */  XK_Z,           NoSymbol,
    /* 0x2d */  XK_X,           NoSymbol,
    /* 0x2e */  XK_C,           NoSymbol,
    /* 0x2f */  XK_V,           NoSymbol,
    /* 0x30 */  XK_B,           NoSymbol,
    /* 0x31 */  XK_N,           NoSymbol,
    /* 0x32 */  XK_M,           NoSymbol,
    /* 0x33 */  XK_comma,       XK_less,
    /* 0x34 */  XK_period,      XK_greater,
    /* 0x35 */  XK_slash,       XK_question,
    /* 0x36 */  XK_Shift_R,     NoSymbol,
    /* 0x37 */  XK_KP_Multiply, NoSymbol,
    /* 0x38 */  XK_Alt_L,	XK_Meta_L,
    /* 0x39 */  XK_space,       NoSymbol,
    /* 0x3a */  XK_Caps_Lock,   NoSymbol,
    /* 0x3b */  XK_F1,          NoSymbol,
    /* 0x3c */  XK_F2,          NoSymbol,
    /* 0x3d */  XK_F3,          NoSymbol,
    /* 0x3e */  XK_F4,          NoSymbol,
    /* 0x3f */  XK_F5,          NoSymbol,
    /* 0x40 */  XK_F6,          NoSymbol,
    /* 0x41 */  XK_F7,          NoSymbol,
    /* 0x42 */  XK_F8,          NoSymbol,
    /* 0x43 */  XK_F9,          NoSymbol,
    /* 0x44 */  XK_F10,         NoSymbol,
    /* 0x45 */  XK_Num_Lock,    NoSymbol,
    /* 0x46 */  XK_Scroll_Lock,	NoSymbol,
    /* These KP keys should have the KP_7 keysyms in the numlock
     * modifer... ? */
    /* 0x47 */  XK_KP_Home,	XK_KP_7,
    /* 0x48 */  XK_KP_Up,	XK_KP_8,
    /* 0x49 */  XK_KP_Prior,	XK_KP_9,
    /* 0x4a */  XK_KP_Subtract, NoSymbol,
    /* 0x4b */  XK_KP_Left,	XK_KP_4,
    /* 0x4c */  XK_KP_Begin,	XK_KP_5,
    /* 0x4d */  XK_KP_Right,	XK_KP_6,
    /* 0x4e */  XK_KP_Add,      NoSymbol,
    /* 0x4f */  XK_KP_End,	XK_KP_1,
    /* 0x50 */  XK_KP_Down,	XK_KP_2,
    /* 0x51 */  XK_KP_Next,	XK_KP_3,
    /* 0x52 */  XK_KP_Insert,	XK_KP_0,
    /* 0x53 */  XK_KP_Delete,	XK_KP_Decimal,
    /* 0x54 */  NoSymbol,	NoSymbol,
    /* 0x55 */  XK_F13,		NoSymbol,
    /* 0x56 */  XK_less,	XK_greater,
    /* 0x57 */  XK_F11,		NoSymbol,
    /* 0x58 */  XK_F12,		NoSymbol,
    /* 0x59 */  XK_F14,		NoSymbol,
    /* 0x5a */  XK_F15,		NoSymbol,
    /* 0x5b */  XK_F16,		NoSymbol,
    /* 0x5c */  XK_F17,		NoSymbol,
    /* 0x5d */  XK_F18,		NoSymbol,
    /* 0x5e */  XK_F19,		NoSymbol,
    /* 0x5f */  XK_F20,		NoSymbol,
    /* 0x60 */  XK_KP_Enter,	NoSymbol,
    /* 0x61 */  XK_Control_R,	NoSymbol,
    /* 0x62 */  XK_KP_Divide,	NoSymbol,
    /* 0x63 */  XK_Print,	XK_Sys_Req,
    /* 0x64 */  XK_Alt_R,	XK_Meta_R,
    /* 0x65 */  NoSymbol,	NoSymbol, /* KEY_LINEFEED */
    /* 0x66 */  XK_Home,	NoSymbol,
    /* 0x67 */  XK_Up,		NoSymbol,
    /* 0x68 */  XK_Prior,	NoSymbol,
    /* 0x69 */  XK_Left,	NoSymbol,
    /* 0x6a */  XK_Right,	NoSymbol,
    /* 0x6b */  XK_End,		NoSymbol,
    /* 0x6c */  XK_Down,	NoSymbol,
    /* 0x6d */  XK_Next,	NoSymbol,
    /* 0x6e */  XK_Insert,	NoSymbol,
    /* 0x6f */  XK_Delete,	NoSymbol,
    /* 0x70 */  NoSymbol,	NoSymbol, /* KEY_MACRO */
    /* 0x71 */  NoSymbol,	NoSymbol,
    /* 0x72 */  NoSymbol,	NoSymbol,
    /* 0x73 */  NoSymbol,	NoSymbol,
    /* 0x74 */  NoSymbol,	NoSymbol,
    /* 0x75 */  XK_KP_Equal,	NoSymbol,
    /* 0x76 */  NoSymbol,	NoSymbol,
    /* 0x77 */  NoSymbol,	NoSymbol,
    /* 0x78 */  XK_F21,		NoSymbol,
    /* 0x79 */  XK_F22,		NoSymbol,
    /* 0x7a */  XK_F23,		NoSymbol,
    /* 0x7b */  XK_F24,		NoSymbol,
    /* 0x7c */  XK_KP_Separator, NoSymbol,
    /* 0x7d */  XK_Meta_L,	NoSymbol,
    /* 0x7e */  XK_Meta_R,	NoSymbol,
    /* 0x7f */  XK_Multi_key,	NoSymbol,
    /* 0x80 */  NoSymbol,	NoSymbol,
    /* 0x81 */  NoSymbol,	NoSymbol,
    /* 0x82 */  NoSymbol,	NoSymbol,
    /* 0x83 */  NoSymbol,	NoSymbol,
    /* 0x84 */  NoSymbol,	NoSymbol,
    /* 0x85 */  NoSymbol,	NoSymbol,
    /* 0x86 */  NoSymbol,	NoSymbol,
    /* 0x87 */  NoSymbol,	NoSymbol,
    /* 0x88 */  NoSymbol,	NoSymbol,
    /* 0x89 */  NoSymbol,	NoSymbol,
    /* 0x8a */  NoSymbol,	NoSymbol,
    /* 0x8b */  NoSymbol,	NoSymbol,
    /* 0x8c */  NoSymbol,	NoSymbol,
    /* 0x8d */  NoSymbol,	NoSymbol,
    /* 0x8e */  NoSymbol,	NoSymbol,
    /* 0x8f */  NoSymbol,	NoSymbol,
    /* 0x90 */  NoSymbol,	NoSymbol,
    /* 0x91 */  NoSymbol,	NoSymbol,
    /* 0x92 */  NoSymbol,	NoSymbol,
    /* 0x93 */  NoSymbol,	NoSymbol,
    /* 0x94 */  NoSymbol,	NoSymbol,
    /* 0x95 */  NoSymbol,	NoSymbol,
    /* 0x96 */  NoSymbol,	NoSymbol,
    /* 0x97 */  NoSymbol,	NoSymbol,
    /* 0x98 */  NoSymbol,	NoSymbol,
    /* 0x99 */  NoSymbol,	NoSymbol,
    /* 0x9a */  NoSymbol,	NoSymbol,
    /* 0x9b */  NoSymbol,	NoSymbol,
    /* 0x9c */  NoSymbol,	NoSymbol,
    /* 0x9d */  NoSymbol,	NoSymbol,
    /* 0x9e */  NoSymbol,	NoSymbol,
    /* 0x9f */  NoSymbol,	NoSymbol,
    /* 0xa0 */  NoSymbol,	NoSymbol,
    /* 0xa1 */  NoSymbol,	NoSymbol,
    /* 0xa2 */  NoSymbol,	NoSymbol,
    /* 0xa3 */  NoSymbol,	NoSymbol,
    /* 0xa4 */  NoSymbol,	NoSymbol,
    /* 0xa5 */  NoSymbol,	NoSymbol,
    /* 0xa6 */  NoSymbol,	NoSymbol,
    /* 0xa7 */  NoSymbol,	NoSymbol,
    /* 0xa8 */  NoSymbol,	NoSymbol,
    /* 0xa9 */  NoSymbol,	NoSymbol,
    /* 0xaa */  NoSymbol,	NoSymbol,
    /* 0xab */  NoSymbol,	NoSymbol,
    /* 0xac */  NoSymbol,	NoSymbol,
    /* 0xad */  NoSymbol,	NoSymbol,
    /* 0xae */  NoSymbol,	NoSymbol,
    /* 0xaf */  NoSymbol,	NoSymbol,
    /* 0xb0 */  NoSymbol,	NoSymbol,
    /* 0xb1 */  NoSymbol,	NoSymbol,
    /* 0xb2 */  NoSymbol,	NoSymbol,
    /* 0xb3 */  NoSymbol,	NoSymbol,
    /* 0xb4 */  NoSymbol,	NoSymbol,
    /* 0xb5 */  NoSymbol,	NoSymbol,
    /* 0xb6 */  NoSymbol,	NoSymbol,
    /* 0xb7 */  NoSymbol,	NoSymbol,
    /* 0xb8 */  NoSymbol,	NoSymbol,
    /* 0xb9 */  NoSymbol,	NoSymbol,
    /* 0xba */  NoSymbol,	NoSymbol,
    /* 0xbb */  NoSymbol,	NoSymbol,
    /* 0xbc */  NoSymbol,	NoSymbol,
    /* 0xbd */  NoSymbol,	NoSymbol,
    /* 0xbe */  NoSymbol,	NoSymbol,
    /* 0xbf */  NoSymbol,	NoSymbol,
    /* 0xc0 */  NoSymbol,	NoSymbol,
    /* 0xc1 */  NoSymbol,	NoSymbol,
    /* 0xc2 */  NoSymbol,	NoSymbol,
    /* 0xc3 */  NoSymbol,	NoSymbol,
    /* 0xc4 */  NoSymbol,	NoSymbol,
    /* 0xc5 */  NoSymbol,	NoSymbol,
    /* 0xc6 */  NoSymbol,	NoSymbol,
    /* 0xc7 */  NoSymbol,	NoSymbol,
    /* 0xc8 */  NoSymbol,	NoSymbol,
    /* 0xc9 */  NoSymbol,	NoSymbol,
    /* 0xca */  NoSymbol,	NoSymbol,
    /* 0xcb */  NoSymbol,	NoSymbol,
    /* 0xcc */  NoSymbol,	NoSymbol,
    /* 0xcd */  NoSymbol,	NoSymbol,
    /* 0xce */  NoSymbol,	NoSymbol,
    /* 0xcf */  NoSymbol,	NoSymbol,
    /* 0xd0 */  NoSymbol,	NoSymbol,
    /* 0xd1 */  NoSymbol,	NoSymbol,
    /* 0xd2 */  NoSymbol,	NoSymbol,
    /* 0xd3 */  NoSymbol,	NoSymbol,
    /* 0xd4 */  NoSymbol,	NoSymbol,
    /* 0xd5 */  NoSymbol,	NoSymbol,
    /* 0xd6 */  NoSymbol,	NoSymbol,
    /* 0xd7 */  NoSymbol,	NoSymbol,
    /* 0xd8 */  NoSymbol,	NoSymbol,
    /* 0xd9 */  NoSymbol,	NoSymbol,
    /* 0xda */  NoSymbol,	NoSymbol,
    /* 0xdb */  NoSymbol,	NoSymbol,
    /* 0xdc */  NoSymbol,	NoSymbol,
    /* 0xdd */  NoSymbol,	NoSymbol,
    /* 0xde */  NoSymbol,	NoSymbol,
    /* 0xdf */  NoSymbol,	NoSymbol,
    /* 0xe0 */  NoSymbol,	NoSymbol,
    /* 0xe1 */  NoSymbol,	NoSymbol,
    /* 0xe2 */  NoSymbol,	NoSymbol,
    /* 0xe3 */  NoSymbol,	NoSymbol,
    /* 0xe4 */  NoSymbol,	NoSymbol,
    /* 0xe5 */  NoSymbol,	NoSymbol,
    /* 0xe6 */  NoSymbol,	NoSymbol,
    /* 0xe7 */  NoSymbol,	NoSymbol,
    /* 0xe8 */  NoSymbol,	NoSymbol,
    /* 0xe9 */  NoSymbol,	NoSymbol,
    /* 0xea */  NoSymbol,	NoSymbol,
    /* 0xeb */  NoSymbol,	NoSymbol,
    /* 0xec */  NoSymbol,	NoSymbol,
    /* 0xed */  NoSymbol,	NoSymbol,
    /* 0xee */  NoSymbol,	NoSymbol,
    /* 0xef */  NoSymbol,	NoSymbol,
    /* 0xf0 */  NoSymbol,	NoSymbol,
    /* 0xf1 */  NoSymbol,	NoSymbol,
    /* 0xf2 */  NoSymbol,	NoSymbol,
    /* 0xf3 */  NoSymbol,	NoSymbol,
    /* 0xf4 */  NoSymbol,	NoSymbol,
    /* 0xf5 */  NoSymbol,	NoSymbol,
    /* 0xf6 */  NoSymbol,	NoSymbol,
    /* 0xf7 */  NoSymbol,	NoSymbol,
};

static struct { KeySym keysym; CARD8 mask; } modifiers[] = {
    { XK_Shift_L,		ShiftMask },
    { XK_Shift_R,		ShiftMask },
    { XK_Control_L,		ControlMask },
    { XK_Control_R,		ControlMask },
    { XK_Caps_Lock,		LockMask },
    { XK_Alt_L,		AltMask },
    { XK_Alt_R,		AltMask },
    { XK_Meta_L,		Mod4Mask },
    { XK_Meta_R,		Mod4Mask },
    { XK_Num_Lock,		NumLockMask },
    { XK_Scroll_Lock,	ScrollLockMask },
    { XK_Mode_switch,	AltLangMask }
};
#endif



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

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 5
    {
        if (!InitKeyboardDeviceStruct(pJstk, &priv->rmlvo, NULL, jstkKbdCtrl))
        {
            ErrorF("unable to init keyboard device\n");
            return !Success;
        }
    }
#else
    KeySymsRec keySyms;
    CARD8 modMap[MAP_LENGTH];
    KeySym sym;
    int i, j;
    XkbComponentNamesRec xkbnames;

    memset(modMap, 0, sizeof(modMap));
    memset(&xkbnames, 0, sizeof(xkbnames));

    keySyms.map        = map;
    keySyms.mapWidth   = GLYPHS_PER_KEY;
    keySyms.minKeyCode = MIN_KEYCODE;
    keySyms.maxKeyCode = MIN_KEYCODE + (sizeof(map) / sizeof(map[0])) / GLYPHS_PER_KEY - 1;

    for (i = 0; i < (sizeof(map) / sizeof(map[0])) / GLYPHS_PER_KEY; i++) {
        sym = map[i * GLYPHS_PER_KEY];
        for (j = 0; j < sizeof(modifiers)/sizeof(modifiers[0]); j++) {
            if (modifiers[j].keysym == sym)
                modMap[i + MIN_KEYCODE] = modifiers[j].mask;
        }
    }

    ErrorF("%s, %s, %s, %s, %s\n", priv->rmlvo.rules, priv->rmlvo.model, priv->rmlvo.layout, priv->rmlvo.variant, priv->rmlvo.options);
    XkbSetRulesDflts(priv->rmlvo.rules, priv->rmlvo.model,
            priv->rmlvo.layout, priv->rmlvo.variant,
            priv->rmlvo.options);
    if (!XkbInitKeyboardDeviceStruct(pJstk, &xkbnames,
                &keySyms, modMap, NULL,
                jstkKbdCtrl))
        return !Success;
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

