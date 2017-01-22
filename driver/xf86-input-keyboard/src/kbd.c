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
#include <xorg-server.h>

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

#include "exevents.h"
#include <X11/Xatom.h>
#include "xserver-properties.h"

#include "xkbstr.h"
#include "xkbsrv.h"

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 23
#define HAVE_THREADED_INPUT	1
#endif

#define CAPSFLAG	1
#define NUMFLAG		2
#define SCROLLFLAG	4
#define MODEFLAG	8
#define COMPOSEFLAG	16
/* Used to know when the first DEVICE_ON after a DEVICE_INIT is called */
#define INITFLAG	(1U << 31)

static int KbdPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags);
static int KbdProc(DeviceIntPtr device, int what);
static void KbdCtrl(DeviceIntPtr device, KeybdCtrl *ctrl);
static void KbdBell(int percent, DeviceIntPtr dev, pointer ctrl, int unused);
static void PostKbdEvent(InputInfoPtr pInfo, unsigned int key, Bool down);

static void InitKBD(InputInfoPtr pInfo, Bool init);
static void UpdateLeds(InputInfoPtr pInfo);

static const char *kbdDefaults[] = {
#ifdef __NetBSD__
#ifdef DEFAULT_TO_WSKBD
    "Protocol",		"wskbd",
#else
    "Protocol",		"standard",
#endif
#else /* NetBSD */
    "Protocol",		"standard",
#endif /* NetBSD */
    "XkbRules",		XKB_DFLT_RULES,
    "XkbModel",		"pc105",
    "XkbLayout",	"us",
    NULL
};

static char *xkb_rules;
static char *xkb_model;
static char *xkb_layout;
static char *xkb_variant;
static char *xkb_options;

_X_EXPORT InputDriverRec KBD = {
    1,
    "kbd",
    NULL,
    KbdPreInit,
    NULL,
    NULL
};

_X_EXPORT InputDriverRec KEYBOARD = {
    1,
    "keyboard",
    NULL,
    KbdPreInit,
    NULL,
    NULL
};

static XF86ModuleVersionInfo xf86KbdVersionRec = {
    "kbd",
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

static pointer
xf86KbdPlug(pointer module, pointer options, int *errmaj, int *errmin)
{
    xf86AddInputDriver(&KBD, module, 0);
    return module;
}

_X_EXPORT XF86ModuleData kbdModuleData = {
    &xf86KbdVersionRec,
    xf86KbdPlug,
    NULL
};

static int
KbdPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
{
    KbdDevPtr pKbd;
    char *s;
    const char **defaults;
    int rc = Success;

    /* Initialise the InputInfoRec. */
    pInfo->type_name = XI_KEYBOARD;
    pInfo->device_control = KbdProc;
    /*
     * We don't specify our own read_input function. We expect
     * an OS specific readInput() function to handle this.
     */
    pInfo->read_input = NULL;
    pInfo->control_proc = NULL;
    pInfo->switch_mode = NULL;
    pInfo->fd = -1;
    pInfo->dev = NULL;

    defaults = kbdDefaults;
    xf86CollectInputOptions(pInfo, defaults);
    xf86ProcessCommonOptions(pInfo, pInfo->options); 

    if (!(pKbd = calloc(sizeof(KbdDevRec), 1))) {
        rc = BadAlloc;
        goto out;
    }

    pInfo->private = pKbd;
    pKbd->PostEvent = PostKbdEvent;

    if (!xf86OSKbdPreInit(pInfo)) {
        rc = BadAlloc;
        goto out;
    }

    if (!pKbd->OpenKeyboard(pInfo)) {
        rc = BadMatch;
        goto out;
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
        free(s);
    }

    xkb_rules = xf86SetStrOption(pInfo->options, "XkbRules", NULL);
    xkb_model = xf86SetStrOption(pInfo->options, "XkbModel", NULL);
    xkb_layout = xf86SetStrOption(pInfo->options, "XkbLayout", NULL);
    xkb_variant = xf86SetStrOption(pInfo->options, "XkbVariant", NULL);
    xkb_options = xf86SetStrOption(pInfo->options, "XkbOptions", NULL);

    pKbd->CustomKeycodes = xf86SetBoolOption(pInfo->options, "CustomKeycodes",
                                             FALSE);

out:
  return rc;
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
  KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;

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
  XkbRMLVOSet rmlvo;
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
# ifdef XI_PROP_DEVICE_NODE
         {
             const char *device_node =
                 xf86CheckStrOption(pInfo->options, "Device", NULL);

             if (device_node)
             {
                 Atom prop_device = MakeAtom(XI_PROP_DEVICE_NODE,
                                             strlen(XI_PROP_DEVICE_NODE), TRUE);
                 XIChangeDeviceProperty(device, prop_device, XA_STRING, 8,
                                        PropModeReplace, strlen(device_node),
                                        device_node, FALSE);
             }
         }
# endif /* XI_PROP_DEVICE_NODE */
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
#if HAVE_THREADED_INPUT
	xf86AddEnabledDevice(pInfo);
#else
	AddEnabledDevice(pInfo->fd);
#endif
    }

    device->public.on = TRUE;
    InitKBD(pInfo, FALSE);
    break;

  case DEVICE_CLOSE:
  case DEVICE_OFF:

    /*
     * Restore original keyboard directness and translation.
     */
    if (pInfo->fd != -1) {
#if HAVE_THREADED_INPUT
      xf86RemoveEnabledDevice(pInfo);
#else
      RemoveEnabledDevice(pInfo->fd);
#endif
    }
    pKbd->KbdOff(pInfo, what);
    device->public.on = FALSE;
    break;

  default:
    return BadValue;
  }
  return (Success);
}

static void
PostKbdEvent(InputInfoPtr pInfo, unsigned int scanCode, Bool down)
{

  KbdDevPtr    pKbd = (KbdDevPtr) pInfo->private;
  DeviceIntPtr device = pInfo->dev;
  KeyClassRec  *keyc = device->key;
  int state;

#ifdef DEBUG
  LogMessageVerbSigSafe(X_INFO, -1, "kbd driver rec scancode: 0x%x %s\n", scanCode, down ? "down" : "up");
#endif

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

  state = XkbStateFieldFromRec(&keyc->xkbInfo->state);

  if (((state & AltMask) == AltMask) && (scanCode == KEY_SysReqest))
    scanCode = KEY_Print;
  else if (scanCode == KEY_Break)
    scanCode = KEY_Pause;

  xf86PostKeyboardEvent(device, scanCode + MIN_KEYCODE, down);
}
