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

#ifndef XFree86LOADER
#include <unistd.h>
#include <errno.h>
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

#ifdef XFree86LOADER
#include <xf86Module.h>
#endif

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
#include <X11/Xatom.h>
#include <xserver-properties.h>
#endif

#define MAXBUTTONS 3

/******************************************************************************
 * Function/Macro keys variables
 *****************************************************************************/
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 5
static KeySym void_map[] = 
{
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	XK_BackSpace,	XK_Tab,		XK_Linefeed,	NoSymbol,
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	NoSymbol,	NoSymbol,	NoSymbol,	XK_Escape,
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	XK_space,	XK_exclam,	XK_quotedbl,	XK_numbersign,
	XK_dollar,	XK_percent,	XK_ampersand,	XK_apostrophe,
	XK_parenleft,	XK_parenright,	XK_asterisk,	XK_plus,
	XK_comma,	XK_minus,	XK_period,	XK_slash,
	XK_0,		XK_1,		XK_2,		XK_3,
	XK_4,		XK_5,		XK_6,		XK_7,
	XK_8,		XK_9,		XK_colon,	XK_semicolon,
	XK_less,	XK_equal,	XK_greater,	XK_question,
	XK_at,		XK_A,		XK_B,		XK_C,
	XK_D,		XK_E,		XK_F,		XK_G,
	XK_H,		XK_I,		XK_J,		XK_K,
	XK_L,		XK_M,		XK_N,		XK_O,
	XK_P,		XK_Q,		XK_R,		XK_S,
	XK_T,		XK_U,		XK_V,		XK_W,
	XK_X,		XK_Y,		XK_Z,		XK_bracketleft,
	XK_backslash,	XK_bracketright,XK_asciicircum,	XK_underscore,
	XK_grave,	XK_a,		XK_b,		XK_c,
	XK_d,		XK_e,		XK_f,		XK_g,
	XK_h,		XK_i,		XK_j,		XK_k,
	XK_l,		XK_m,		XK_n,		XK_o,
	XK_p,		XK_q,		XK_r,		XK_s,
	XK_t,		XK_u,		XK_v,		XK_w,
	XK_x,		XK_y,		XK_z,		XK_braceleft,
	XK_bar,		XK_braceright,	XK_asciitilde,	XK_BackSpace,
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	NoSymbol,	NoSymbol,	NoSymbol,	NoSymbol,
	XK_nobreakspace,XK_exclamdown,	XK_cent,	XK_sterling,
	XK_currency,	XK_yen,		XK_brokenbar,	XK_section,
	XK_diaeresis,	XK_copyright,	XK_ordfeminine,	XK_guillemotleft,
	XK_notsign,	XK_hyphen,	XK_registered,	XK_macron,
	XK_degree,	XK_plusminus,	XK_twosuperior,	XK_threesuperior,
	XK_acute,	XK_mu,		XK_paragraph,	XK_periodcentered,
	XK_cedilla,	XK_onesuperior,	XK_masculine,	XK_guillemotright,
	XK_onequarter,	XK_onehalf,	XK_threequarters,XK_questiondown,
	XK_Agrave,	XK_Aacute,	XK_Acircumflex,	XK_Atilde,
	XK_Adiaeresis,	XK_Aring,	XK_AE,		XK_Ccedilla,
	XK_Egrave,	XK_Eacute,	XK_Ecircumflex,	XK_Ediaeresis,
	XK_Igrave,	XK_Iacute,	XK_Icircumflex,	XK_Idiaeresis,
	XK_ETH,		XK_Ntilde,	XK_Ograve,	XK_Oacute,
	XK_Ocircumflex,	XK_Otilde,	XK_Odiaeresis,	XK_multiply,
	XK_Ooblique,	XK_Ugrave,	XK_Uacute,	XK_Ucircumflex,
	XK_Udiaeresis,	XK_Yacute,	XK_THORN,	XK_ssharp,
	XK_agrave,	XK_aacute,	XK_acircumflex,	XK_atilde,
	XK_adiaeresis,	XK_aring,	XK_ae,		XK_ccedilla,
	XK_egrave,	XK_eacute,	XK_ecircumflex,	XK_ediaeresis,
	XK_igrave,	XK_iacute,	XK_icircumflex,	XK_idiaeresis,
	XK_eth,		XK_ntilde,	XK_ograve,	XK_oacute,
	XK_ocircumflex,	XK_otilde,	XK_odiaeresis,	XK_division,
	XK_oslash,	XK_ugrave,	XK_uacute,	XK_ucircumflex,
	XK_udiaeresis,	XK_yacute,	XK_thorn,	XK_ydiaeresis
};

/* minKeyCode = 8 because this is the min legal key code */
static KeySymsRec void_keysyms = {
  /* map	minKeyCode	maxKeyCode	width */
  void_map,	8,		255,		1
};
#endif	/* GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 5 */

static const char *DEFAULTS[] = {
    NULL
};

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
    int i;
    Bool result;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
    Atom btn_labels[MAXBUTTONS] = {0};
    Atom axes_labels[2] = {0};

    axes_labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_X);
    axes_labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_Y);

    btn_labels[0] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_LEFT);
    btn_labels[1] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_MIDDLE);
    btn_labels[2] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_RIGHT);
#endif

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
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
					btn_labels,
#endif
					map) == FALSE) {
	  ErrorF("unable to allocate Button class device\n");
	  return !Success;
	}

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 5
	result = InitKeyboardDeviceStruct(device, NULL,
					  BellProc, KeyControlProc);
#else
	result = InitKeyboardDeviceStruct((DevicePtr)device, &void_keysyms,
					  NULL, BellProc, KeyControlProc);
#endif
	if (!result) {
	  ErrorF("unable to init keyboard device\n");
	  return !Success;
	}

	if (InitValuatorClassDeviceStruct(device, 
					  2,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
					  axes_labels,
#endif
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 3
					  xf86GetMotionEvents,
#endif
					  0,
					  Absolute) == FALSE) {
	  InitValuatorAxisStruct(device,
				 0,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
				 axes_labels[0],
#endif
				 0, /* min val */1, /* max val */
				 1, /* resolution */
				 0, /* min_res */
				 1); /* max_res */
	  InitValuatorAxisStruct(device,
				 1,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
				 axes_labels[1],
#endif
				 0, /* min val */1, /* max val */
				 1, /* resolution */
				 0, /* min_res */
				 1); /* max_res */
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
static InputInfoPtr
xf86VoidInit(InputDriverPtr	drv,
	     IDevPtr		dev,
	     int		flags)
{
    InputInfoPtr pInfo;

    if (!(pInfo = xf86AllocateInput(drv, 0)))
	return NULL;

    /* Initialise the InputInfoRec. */
    pInfo->name = dev->identifier;
    pInfo->type_name = "Void";
    pInfo->flags = XI86_KEYBOARD_CAPABLE | XI86_POINTER_CAPABLE | XI86_SEND_DRAG_EVENTS;
    pInfo->device_control = xf86VoidControlProc;
    pInfo->read_input = NULL;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) == 0
    pInfo->motion_history_proc = xf86GetMotionEvents;
    pInfo->history_size = 0;
#endif    
    pInfo->control_proc = NULL;
    pInfo->close_proc = NULL;
    pInfo->switch_mode = NULL;
    pInfo->conversion_proc = NULL;
    pInfo->reverse_conversion_proc = NULL;
    pInfo->fd = -1;
    pInfo->dev = NULL;
    pInfo->private_flags = 0;
    pInfo->always_core_feedback = NULL;
    pInfo->conf_idev = dev;

    /* Collect the options, and process the common options. */
    xf86CollectInputOptions(pInfo, DEFAULTS, NULL);
    xf86ProcessCommonOptions(pInfo, pInfo->options);
    
    /* Mark the device configured */
    pInfo->flags |= XI86_CONFIGURED;

    /* Return the configured device */
    return (pInfo);
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
#ifdef XFree86LOADER
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

#endif /* XFree86LOADER */
