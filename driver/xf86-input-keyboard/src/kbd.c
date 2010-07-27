/*
 * Copyright (c) 2002 by The XFree86 Project, Inc.
 * Author: Ivan Pascal.
 *
 * Based on the code from
 * xf86Config.c which is
 * Copyright 1991-2002 by The XFree86 Project, Inc.
 * Copyright 1997 by Metro Link, Inc.
 * xf86Events.c and xf86Io.c which are
 * Copyright 1990,91 by Thomas Roell, Dinkelscherben, Germany.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
  
#define NEED_EVENTS
#include <X11/X.h>
#include <X11/Xproto.h>

#include "xf86.h"
#include "atKeynames.h"
#include "xf86Privstr.h"

#include <X11/extensions/XI.h>
#include <X11/extensions/XIproto.h>
#include "extnsionst.h"
#include "extinit.h"
#include "inputstr.h"

#include "xf86Xinput.h"
#include "xf86_OSproc.h"
#include "xf86OSKbd.h"
#include "compiler.h"

#include "xkbstr.h"
#include "xkbsrv.h"

#define CAPSFLAG	1
#define NUMFLAG		2
#define SCROLLFLAG	4
#define MODEFLAG	8
#define COMPOSEFLAG	16
/* Used to know when the first DEVICE_ON after a DEVICE_INIT is called */
#define INITFLAG	(1U << 31)

static InputInfoPtr KbdPreInit(InputDriverPtr drv, IDevPtr dev, int flags);
static int KbdProc(DeviceIntPtr device, int what);
static void KbdCtrl(DeviceIntPtr device, KeybdCtrl *ctrl);
static void KbdBell(int percent, DeviceIntPtr dev, pointer ctrl, int unused);
static void PostKbdEvent(InputInfoPtr pInfo, unsigned int key, Bool down);

static void InitKBD(InputInfoPtr pInfo, Bool init);
static void SetXkbOption(InputInfoPtr pInfo, char *name, char **option);
static void UpdateLeds(InputInfoPtr pInfo);

_X_EXPORT InputDriverRec KBD = {
	1,
	"kbd",
	NULL,
	KbdPreInit,
	NULL,
	NULL,
	0
};

_X_EXPORT InputDriverRec KEYBOARD = {
	1,
	"keyboard",
	NULL,
	KbdPreInit,
	NULL,
	NULL,
	0
};

typedef enum {
    OPTION_ALWAYS_CORE,
    OPTION_SEND_CORE_EVENTS,
    OPTION_CORE_KEYBOARD,
    OPTION_DEVICE,
    OPTION_PROTOCOL,
    OPTION_AUTOREPEAT,
    OPTION_XLEDS,
    OPTION_XKB_RULES,
    OPTION_XKB_MODEL,
    OPTION_XKB_LAYOUT,
    OPTION_XKB_VARIANT,
    OPTION_XKB_OPTIONS,
    OPTION_CUSTOM_KEYCODES
} KeyboardOpts;

/* These aren't actually used ... */
static const OptionInfoRec KeyboardOptions[] = {
    { OPTION_ALWAYS_CORE,	"AlwaysCore",	  OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_SEND_CORE_EVENTS,	"SendCoreEvents", OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CORE_KEYBOARD,	"CoreKeyboard",	  OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_DEVICE,		"Device",	  OPTV_STRING,	{0}, FALSE },
    { OPTION_PROTOCOL,		"Protocol",	  OPTV_STRING,	{0}, FALSE },
    { OPTION_AUTOREPEAT,	"AutoRepeat",	  OPTV_STRING,	{0}, FALSE },
    { OPTION_XLEDS,		"XLeds",	  OPTV_STRING,	{0}, FALSE },
    { OPTION_XKB_RULES,		"XkbRules",	  OPTV_STRING,	{0}, FALSE },
    { OPTION_XKB_MODEL,		"XkbModel",	  OPTV_STRING,	{0}, FALSE },
    { OPTION_XKB_LAYOUT,	"XkbLayout",	  OPTV_STRING,	{0}, FALSE },
    { OPTION_XKB_VARIANT,	"XkbVariant",	  OPTV_STRING,	{0}, FALSE },
    { OPTION_XKB_OPTIONS,	"XkbOptions",	  OPTV_STRING,	{0}, FALSE },
    { OPTION_CUSTOM_KEYCODES,   "CustomKeycodes", OPTV_BOOLEAN,	{0}, FALSE },
    { -1,			NULL,		  OPTV_NONE,	{0}, FALSE }
};

static const char *kbdDefaults[] = {
#ifdef XQUEUE 
    "Protocol",		"Xqueue",
#else
    "Protocol",		"standard",
#endif
    "AutoRepeat",	"500 30",
    "XkbRules",		"xorg",
    "XkbModel",		"pc105",
    "XkbLayout",	"us",
    "CustomKeycodes",	"off",
    NULL
};

static const char *kbd98Defaults[] = {
#ifdef XQUEUE
    "Protocol",		"Xqueue",
#else
    "Protocol",		"standard",
#endif
    "AutoRepeat",	"500 30",
    "XkbRules",		"xfree98",
    "XkbModel",		"pc98",
    "XkbLayout",	"jp",
    "CustomKeycodes",	"off",
    NULL
};

static char *xkb_rules;
static char *xkb_model;
static char *xkb_layout;
static char *xkb_variant;
static char *xkb_options;

static void
SetXkbOption(InputInfoPtr pInfo, char *name, char **option)
{
   char *s;

   if ((s = xf86SetStrOption(pInfo->options, name, NULL))) {
       if (!s[0]) {
           xfree(s);
           *option = NULL;
       } else {
           *option = s;
           xf86Msg(X_CONFIG, "%s: %s: \"%s\"\n", pInfo->name, name, s);
       }
    }
}

static InputInfoPtr
KbdPreInit(InputDriverPtr drv, IDevPtr dev, int flags)
{
    InputInfoPtr pInfo;
    KbdDevPtr pKbd;
    MessageType from = X_DEFAULT;
    char *s;

    if (!(pInfo = xf86AllocateInput(drv, 0)))
	return NULL;

    /* Initialise the InputInfoRec. */
    pInfo->name = dev->identifier;
    pInfo->type_name = XI_KEYBOARD;
    pInfo->flags = XI86_KEYBOARD_CAPABLE;
    pInfo->device_control = KbdProc;
    /*
     * We don't specify our own read_input function. We expect
     * an OS specific readInput() function to handle this.
     */
    pInfo->read_input = NULL;
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

    if (!xf86IsPc98())
        xf86CollectInputOptions(pInfo, kbdDefaults, NULL);
    else
        xf86CollectInputOptions(pInfo, kbd98Defaults, NULL);
    xf86ProcessCommonOptions(pInfo, pInfo->options); 

    if (!(pKbd = xcalloc(sizeof(KbdDevRec), 1)))
        return pInfo;

    pInfo->private = pKbd;
    pKbd->PostEvent = PostKbdEvent;

    if (!xf86OSKbdPreInit(pInfo))
        return pInfo;

    if (!pKbd->OpenKeyboard(pInfo)) {
        return pInfo;
    }

    if ((s = xf86SetStrOption(pInfo->options, "XLeds", NULL))) {
        char *l, *end;
        unsigned int i;
        l = strtok(s, " \t\n");
        while (l) {
    	    i = strtoul(l, &end, 0);
    	    if (*end == '\0')
    	        pKbd->xledsMask |= 1L << (i - 1);
    	    else {
    	        xf86Msg(X_ERROR, "\"%s\" is not a valid XLeds value", l);
    	    }
    	    l = strtok(NULL, " \t\n");
        }
        xfree(s);
    }

    SetXkbOption(pInfo, "XkbRules", &xkb_rules);
    SetXkbOption(pInfo, "XkbModel", &xkb_model);
    SetXkbOption(pInfo, "XkbLayout", &xkb_layout);
    SetXkbOption(pInfo, "XkbVariant", &xkb_variant);
    SetXkbOption(pInfo, "XkbOptions", &xkb_options);

  pKbd->CustomKeycodes = FALSE;
  from = X_DEFAULT; 
  if (xf86FindOption(pInfo->options, "CustomKeycodes")) {
      pKbd->CustomKeycodes = xf86SetBoolOption(pInfo->options, "CustomKeycodes",
                                               pKbd->CustomKeycodes);
     from = X_CONFIG;
  }

  xf86Msg(from, "%s: CustomKeycodes %s\n",
               pInfo->name, pKbd->CustomKeycodes ? "enabled" : "disabled");

  pInfo->flags |= XI86_CONFIGURED;

  return pInfo;
}

static void
KbdBell(int percent, DeviceIntPtr dev, pointer ctrl, int unused)
{
   InputInfoPtr pInfo = (InputInfoPtr) dev->public.devicePrivate;
   KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
   pKbd->Bell(pInfo, percent, ((KeybdCtrl*) ctrl)->bell_pitch,
                              ((KeybdCtrl*) ctrl)->bell_duration);
}

static void
UpdateLeds(InputInfoPtr pInfo)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    unsigned long leds = 0;

    if (pKbd->keyLeds & CAPSFLAG)    leds |= XLED1;
    if (pKbd->keyLeds & NUMFLAG)     leds |= XLED2;
    if (pKbd->keyLeds & SCROLLFLAG ||
        pKbd->keyLeds & MODEFLAG)    leds |= XLED3;
    if (pKbd->keyLeds & COMPOSEFLAG) leds |= XLED4;

    pKbd->leds = (pKbd->leds & pKbd->xledsMask) | (leds & ~pKbd->xledsMask);
    pKbd->SetLeds(pInfo, pKbd->leds);
}

static void
KbdCtrl( DeviceIntPtr device, KeybdCtrl *ctrl)
{
   unsigned long leds;
   InputInfoPtr pInfo = (InputInfoPtr) device->public.devicePrivate;
   KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;

   if ( ctrl->leds & XLED1) {
       pKbd->keyLeds |= CAPSFLAG;
   } else {
       pKbd->keyLeds &= ~CAPSFLAG;
   }
   if ( ctrl->leds & XLED2) {
       pKbd->keyLeds |= NUMFLAG;
   } else {
       pKbd->keyLeds &= ~NUMFLAG;
   }
   if ( ctrl->leds & XLED3) {
       pKbd->keyLeds |= SCROLLFLAG;
   } else {
       pKbd->keyLeds &= ~SCROLLFLAG;
   }
   if ( ctrl->leds & (XCOMP|XLED4) ) {
       pKbd->keyLeds |= COMPOSEFLAG;
   } else {
       pKbd->keyLeds &= ~COMPOSEFLAG;
   }
   leds = ctrl->leds & ~(XCAPS | XNUM | XSCR); /* ??? */
   pKbd->leds = leds;
  pKbd->SetLeds(pInfo, pKbd->leds);
}

static void
InitKBD(InputInfoPtr pInfo, Bool init)
{
  xEvent          kevent;
  KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 1
  DeviceIntPtr    pKeyboard = pInfo->dev;
  KeyClassRec     *keyc = pKeyboard->key;
  KeySym          *map = keyc->curKeySyms.map;
  unsigned int    i;
#endif

  kevent.u.keyButtonPointer.time = GetTimeInMillis();
  kevent.u.keyButtonPointer.rootX = 0;
  kevent.u.keyButtonPointer.rootY = 0;

/* The server does this for us with i-h. */
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 1
  /*
   * Hmm... here is the biggest hack of every time !
   * It may be possible that a switch-vt procedure has finished BEFORE
   * you released all keys neccessary to do this. That peculiar behavior
   * can fool the X-server pretty much, cause it assumes that some keys
   * were not released. TWM may stuck alsmost completly....
   * OK, what we are doing here is after returning from the vt-switch
   * exeplicitely unrelease all keyboard keys before the input-devices
   * are reenabled.
   */
  for (i = keyc->curKeySyms.minKeyCode, map = keyc->curKeySyms.map;
       i < keyc->curKeySyms.maxKeyCode;
       i++, map += keyc->curKeySyms.mapWidth)
     if (KeyPressed(i))
      {
        switch (*map) {
        /* Don't release the lock keys */
        case XK_Caps_Lock:
        case XK_Shift_Lock:
        case XK_Num_Lock:
        case XK_Scroll_Lock:
        case XK_Kana_Lock:
          break;
        default:
          kevent.u.u.detail = i;
          kevent.u.u.type = KeyRelease;
          (* pKeyboard->public.processInputProc)(&kevent, pKeyboard, 1);
        }
      }
#endif

  pKbd->scanPrefix      = 0;

  if (init) {
      pKbd->keyLeds = pKbd->GetLeds(pInfo);
      UpdateLeds(pInfo);
      pKbd->keyLeds |= INITFLAG;
  } else {
      unsigned long leds = pKbd->keyLeds;

      pKbd->keyLeds = pKbd->GetLeds(pInfo);
      UpdateLeds(pInfo);
      if ((pKbd->keyLeds & CAPSFLAG) !=
	  ((leds & INITFLAG) ? 0 : (leds & CAPSFLAG))) {
	  pKbd->PostEvent(pInfo, KEY_CapsLock, TRUE);
	  pKbd->PostEvent(pInfo, KEY_CapsLock, FALSE);
      }
      if ((pKbd->keyLeds & NUMFLAG) !=
	  (leds & INITFLAG ? 0 : leds & NUMFLAG)) {
	  pKbd->PostEvent(pInfo, KEY_NumLock, TRUE);
	  pKbd->PostEvent(pInfo, KEY_NumLock, FALSE);
      }
  }
}

static int
KbdProc(DeviceIntPtr device, int what)
{

  InputInfoPtr pInfo = device->public.devicePrivate;
  KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
  KeySymsRec           keySyms;
  CARD8                modMap[MAP_LENGTH];
  int                  ret;

  switch (what) {
     case DEVICE_INIT:
         ret = pKbd->KbdInit(pInfo, what);
         if (ret != Success)
             return ret;

         pKbd->KbdGetMapping(pInfo, &keySyms, modMap);

         device->public.on = FALSE;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 5
         {
             XkbRMLVOSet rmlvo;
             rmlvo.rules = xkb_rules;
             rmlvo.model = xkb_model;
             rmlvo.layout = xkb_layout;
             rmlvo.variant = xkb_variant;
             rmlvo.options = xkb_options;

             if (!InitKeyboardDeviceStruct(device, &rmlvo, KbdBell, KbdCtrl))
             {
                 xf86Msg(X_ERROR, "%s: Keyboard initialization failed. This "
                         "could be a missing or incorrect setup of "
                         "xkeyboard-config.\n", device->name);

                 return BadValue;
             }
         }
#else
         {
             XkbComponentNamesRec xkbnames;
             memset(&xkbnames, 0, sizeof(xkbnames));
             XkbSetRulesDflts(xkb_rules, xkb_model, xkb_layout,
                              xkb_variant, xkb_options);
             XkbInitKeyboardDeviceStruct(device, &xkbnames, &keySyms,
                                         modMap, KbdBell,
                                         (KbdCtrlProcPtr)KbdCtrl);
         }
#endif /* XINPUT ABI 5*/
         InitKBD(pInfo, TRUE);
         break;
  case DEVICE_ON:
    if (device->public.on)
	break;
    /*
     * Set the keyboard into "direct" mode and turn on
     * event translation.
     */
    if ((ret = pKbd->KbdOn(pInfo, what)) != Success)
	return ret;
    /*
     * Discard any pending input after a VT switch to prevent the server
     * passing on parts of the VT switch sequence.
     */
    if (pInfo->fd >= 0) {
	xf86FlushInput(pInfo->fd);
	AddEnabledDevice(pInfo->fd);
    }

    device->public.on = TRUE;
    InitKBD(pInfo, FALSE);
    break;

  case DEVICE_CLOSE:
  case DEVICE_OFF:

    /*
     * Restore original keyboard directness and translation.
     */
    if (pInfo->fd != -1)
      RemoveEnabledDevice(pInfo->fd);
    pKbd->KbdOff(pInfo, what);
    device->public.on = FALSE;
    break;
  }
  return (Success);
}

static void
PostKbdEvent(InputInfoPtr pInfo, unsigned int scanCode, Bool down)
{

  KbdDevPtr    pKbd = (KbdDevPtr) pInfo->private;
  DeviceIntPtr device = pInfo->dev;
  KeyClassRec  *keyc = device->key;

#ifdef DEBUG
  ErrorF("kbd driver rec scancode: 0x02%x %s\n", scanCode, down?"down":"up");
#endif
	  
  /* Disable any keyboard processing while in suspend */
  if (xf86inSuspend)
      return;

  /*
   * First do some special scancode remapping ...
   */
  if (pKbd->RemapScanCode != NULL) {
     if (pKbd->RemapScanCode(pInfo, (int*) &scanCode))
         return;
  } else {
     if (pKbd->scancodeMap != NULL) {
         TransMapPtr map = pKbd->scancodeMap; 
         if (scanCode >= map->begin && scanCode < map->end)
             scanCode = map->map[scanCode - map->begin];
     }
  }

  /*
   * PC keyboards generate separate key codes for
   * Alt+Print and Control+Pause but in the X keyboard model
   * they need to get the same key code as the base key on the same
   * physical keyboard key.
   */

  if (!xf86IsPc98()) {
    int state;

    state = XkbStateFieldFromRec(&keyc->xkbInfo->state);

    if (((state & AltMask) == AltMask) && (scanCode == KEY_SysReqest))
      scanCode = KEY_Print;
    else if (scanCode == KEY_Break)
      scanCode = KEY_Pause;
  }

  xf86PostKeyboardEvent(device, scanCode + MIN_KEYCODE, down);
}

static void
xf86KbdUnplug(pointer	p)
{
}

static pointer
xf86KbdPlug(pointer	module,
	    pointer	options,
	    int		*errmaj,
	    int		*errmin)
{
    xf86AddInputDriver(&KBD, module, 0);

    return module;
}

static XF86ModuleVersionInfo xf86KbdVersionRec =
{
    "kbd",
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

_X_EXPORT XF86ModuleData kbdModuleData = {
    &xf86KbdVersionRec,
    xf86KbdPlug,
    xf86KbdUnplug
};
