/*
 * Copyright 1990,91 by Thomas Roell, Dinkelscherben, Germany.
 * Copyright 1993 by David Dawes <dawes@xfree86.org>
 * Copyright 2002 by SuSE Linux AG, Author: Egbert Eich
 * Copyright 1994-2002 by The XFree86 Project, Inc.
 * Copyright 2002 by Paul Elliott
 * Copyright 2002-2006 by VMware, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of copyright holders not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.  The copyright holders
 * make no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

/*
 * vmmouse.c --
 *
 * 	This is a modified version of the mouse input driver
 * 	provided in Xserver/hw/xfree86/input/mouse/mouse.c
 *
 *      Although all data is read using the vmmouse protocol, notification
 *      is still done through the PS/2 port, so all the basic code for
 *      interacting with the port is retained.
 *
 */


/*****************************************************************************
 *	Standard Headers
 ****************************************************************************/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <X11/X.h>
#include <X11/Xproto.h>

#include "xf86.h"

#ifdef XINPUT
#include <X11/extensions/XI.h>
#include <X11/extensions/XIproto.h>
#include "extnsionst.h"
#include "extinit.h"
#else
#include "inputstr.h"
#endif

#include "xf86Xinput.h"
#include "xf86_OSproc.h"
#include "xf86OSmouse.h"
#include "xf86Priv.h"
#include "compiler.h"

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
#include <xserver-properties.h>
#include "exevents.h"
#endif

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 18
#define LogMessageVerbSigSafe xf86MsgVerb
#endif

#include "xisb.h"
#include "mipointer.h"

#ifndef HAVE_XORG_SERVER_1_5_0
#include <xf86_ansic.h>
#include <xf86_libc.h>
#endif

/*****************************************************************************
 *	Local Headers
 ****************************************************************************/
#include "vmmouse_client.h"

/*
 * This is the only way I know to turn a #define of an integer constant into
 * a constant string.
 */
#define VMW_INNERSTRINGIFY(s) #s
#define VMW_STRING(str) VMW_INNERSTRINGIFY(str)

/*
 * So that the file compiles unmodified when dropped into an xfree source tree.
 */
#ifndef XORG_VERSION_CURRENT
#define XORG_VERSION_CURRENT XF86_VERSION_CURRENT
#endif

/*
 * Version constants
 */
#define VMMOUSE_DRIVER_VERSION \
   (PACKAGE_VERSION_MAJOR * 65536 + PACKAGE_VERSION_MINOR * 256 + PACKAGE_VERSION_PATCHLEVEL)
#define VMMOUSE_DRIVER_VERSION_STRING \
    VMW_STRING(PACKAGE_VERSION_MAJOR) "." VMW_STRING(PACKAGE_VERSION_MINOR) \
    "." VMW_STRING(PACKAGE_VERSION_PATCHLEVEL)

/*
 * Standard four digit version string expected by VMware Tools installer.
 * As the driver's version is only  {major, minor, patchlevel},
 * The fourth digit may describe the commit number relative to the
 * last version tag as output from `git describe`
 */
#ifdef __GNUC__
#ifdef VMW_SUBPATCH
const char vm_mouse_version[] __attribute__((section(".modinfo"),unused)) =
    "version=" VMMOUSE_DRIVER_VERSION_STRING "." VMW_STRING(VMW_SUBPATCH);
#else
const char vm_mouse_version[] __attribute__((section(".modinfo"),unused)) =
    "version=" VMMOUSE_DRIVER_VERSION_STRING ".0";
#endif /*VMW_SUBPATCH*/
#endif


/*****************************************************************************
 *	static function header
 ****************************************************************************/
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
static int VMMousePreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags);
#else
static InputInfoPtr VMMousePreInit(InputDriverPtr drv, IDevPtr dev, int flags);
static void VMMouseCloseProc(InputInfoPtr pInfo);
static Bool VMMouseConvertProc(InputInfoPtr pInfo, int first, int num, int v0, int v1, int v2,
			       int v3, int v4, int v5, int *x, int *y);
#endif
static void VMMouseUnInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags);
static void MouseCommonOptions(InputInfoPtr pInfo);
static void GetVMMouseMotionEvent(InputInfoPtr pInfo);
static void VMMousePostEvent(InputInfoPtr pInfo, int buttons, int dx, int dy, int dz, int dw);
static void VMMouseDoPostEvent(InputInfoPtr pInfo, int buttons, int dx, int dy);
static Bool VMMouseDeviceControl(DeviceIntPtr device, int mode);
static int  VMMouseControlProc(InputInfoPtr pInfo, xDeviceCtl * control);
static void VMMouseReadInput(InputInfoPtr pInfo);
static int  VMMouseSwitchMode(ClientPtr client, DeviceIntPtr dev, int mode);
static void MouseCtrl(DeviceIntPtr device, PtrCtrl *ctrl);

/******************************************************************************
 *		Definitions
 *****************************************************************************/
typedef struct {
   int                 screenNum;
   Bool                vmmouseAvailable;
   VMMOUSE_INPUT_DATA  vmmousePrevInput;
   Bool                isCurrRelative;
   Bool                absoluteRequested;
} VMMousePrivRec, *VMMousePrivPtr;

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 7
static const char *reqSymbols[] = {
   "InitPointerDeviceStruct",
   "LoaderSymbol",
   "LoadSubModule",
   "miPointerGetMotionBufferSize",
   "miPointerGetMotionEvents",
   "screenInfo",
   "Xcalloc",
   "xf86AddEnabledDevice",
   "xf86AddInputDriver",
   "xf86AddModuleInfo",
   "xf86AllocateInput",
   "xf86BlockSIGIO",
   "xf86CloseSerial",
   "xf86CollectInputOptions",
   "xf86ffs",
   "xf86FlushInput",
   "xf86GetAllowMouseOpenFail",
   "xf86GetMotionEvents",
   "xf86InitValuatorAxisStruct",
   "xf86InitValuatorDefaults",
   "xf86LoaderCheckSymbol",
   "xf86MotionHistoryAllocate",
   "xf86Msg",
   "xf86NameCmp",
   "xf86OpenSerial",
   "xf86OSMouseInit",
   "xf86PostButtonEvent",
   "xf86PostMotionEvent",
   "xf86ProcessCommonOptions",
   "xf86RemoveEnabledDevice",
   "xf86SetIntOption",
   "xf86SetStrOption",
   "xf86sprintf",
   "xf86sscanf",
   "xf86UnblockSIGIO",
   "xf86usleep",
   "xf86XInputSetScreen",
   "Xfree",
   "XisbBlockDuration",
   "XisbFree",
   "XisbNew",
   "XisbRead",
   "Xstrdup",
   NULL
};
#endif

InputDriverRec VMMOUSE = {
   1,
   "vmmouse",
   NULL,
   VMMousePreInit,
   VMMouseUnInit,
   NULL
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 11
       ,
   0
#endif
};

static char reverseMap[32] = { 0,  4,  2,  6,  1,  5,  3,  7,
			       8, 12, 10, 14,  9, 13, 11, 15,
			      16, 20, 18, 22, 17, 21, 19, 23,
			      24, 28, 26, 30, 25, 29, 27, 31};

#define reverseBits(map, b)	(((b) & ~0x0f) | map[(b) & 0x0f])

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 14

static InputOption*
input_option_new(InputOption *list, char *key, char *value)
{
   InputOption *new;

   new = calloc(1, sizeof(InputOption));
   new->key = key;
   new->value = value;
   new->next = list;
   return new;
}

static void
input_option_free_list(InputOption **opts)
{
   InputOption *tmp = *opts;
   while(*opts)
   {
      tmp = (*opts)->next;
      free((*opts)->key);
      free((*opts)->value);
      free((*opts));
      *opts = tmp;
   }
}
#endif

static int
VMMouseInitPassthru(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
{
   InputAttributes *attrs = NULL;
   InputOption *input_options = NULL;
   pointer options;
   DeviceIntPtr dev;
   int rc;

   options = xf86OptionListDuplicate(pInfo->options);
   options = xf86ReplaceStrOption(options, "Driver", "mouse");

   while(options) {
      input_options = input_option_new(input_options,
                                       xf86OptionName(options),
                                       xf86OptionValue(options));
      options = xf86NextOption(options);
   }

   rc = NewInputDeviceRequest(input_options, attrs, &dev);

   input_option_free_list(&input_options);

   return rc;
}

#else /* if ABI_XINPUT_VERSION < 12 */
static InputInfoPtr
VMMouseInitPassthru(InputDriverPtr drv, IDevPtr dev, int flags)
{
   InputDriverRec *passthruMouse;
   passthruMouse = (InputDriverRec *)LoaderSymbol("MOUSE");
   if(passthruMouse != NULL) {
      return (passthruMouse->PreInit)(drv, dev, flags);
   } else {
      return NULL;
   }
}
#endif

/*
 *----------------------------------------------------------------------
 *
 * VMMousePreInit --
 *	This function collect all the information that is necessary to
 *	determine the configuration of the hardware and to prepare the
 *	device for being used
 *
 * Results:
 * 	An InputInfoPtr object which points to vmmouse's information,
 *	if the absolute pointing device available
 *	Otherwise, an InputInfoPtr of regular mouse
 *
 * Side effects:
 * 	VMMouse was initialized with necessary information
 *
 *----------------------------------------------------------------------
 */

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 12
static int
VMMouseNewPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags);

static InputInfoPtr
VMMousePreInit(InputDriverPtr drv, IDevPtr dev, int flags)
{
   InputInfoPtr pInfo;

#ifndef NO_MOUSE_MODULE
{
   OSMouseInfoPtr osInfo = NULL;

   /*
    * let Xserver init the mouse first
    */
   osInfo = xf86OSMouseInit(0);
   if (!osInfo)
      return FALSE;
}
#endif

   /*
    * enable hardware access
    */
   if (!xorgHWAccess) {
      if (xf86EnableIO())
          xorgHWAccess = TRUE;
      else
          return NULL;
   }

   /*
    * try to enable vmmouse here
    */
   if (!VMMouseClient_Enable()) {
      /*
       * vmmouse failed
       * Fall back to normal mouse module
       */
      xf86Msg(X_ERROR, "VMWARE(0): vmmouse enable failed\n");
      return VMMouseInitPassthru(drv, dev, flags);
   } else {
      /*
       * vmmouse is available
       */
      xf86Msg(X_INFO, "VMWARE(0): vmmouse is available\n");
      /*
       * Disable the absolute pointing device for now
       * It will be enabled during DEVICE_ON phase
       */
      VMMouseClient_Disable();
   }

   if (!(pInfo = xf86AllocateInput(drv, 0))) {
      return NULL;
   }

   pInfo->name = dev->identifier;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) == 0
   pInfo->motion_history_proc = xf86GetMotionEvents;
#endif
   pInfo->close_proc = VMMouseCloseProc;
   pInfo->conversion_proc = VMMouseConvertProc;
   pInfo->reverse_conversion_proc = NULL;
   pInfo->fd = -1;
   pInfo->dev = NULL;
   pInfo->private_flags = 0;
   pInfo->always_core_feedback = 0;
   pInfo->conf_idev = dev;
   pInfo->flags = XI86_POINTER_CAPABLE | XI86_SEND_DRAG_EVENTS;

   /* Collect the options, and process the common options. */
   xf86CollectInputOptions(pInfo, NULL, NULL);
   xf86ProcessCommonOptions(pInfo, pInfo->options);

   if (VMMouseNewPreInit(drv, pInfo, flags) == Success)
       pInfo->flags |= XI86_CONFIGURED;

   return pInfo;
}

static int
VMMouseNewPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
#else /* if ABI_XINPUT_VERSION >= 12 */
static int
VMMousePreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
#endif
{
   MouseDevPtr pMse = NULL;
   VMMousePrivPtr mPriv = NULL;
   int rc = Success;

   /* Enable hardware access. */
   if (!xorgHWAccess) {
      if (xf86EnableIO())
          xorgHWAccess = TRUE;
      else {
          rc = BadValue;
          goto error;
      }
   }

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
   /* For ABI < 12, we need to return the wrapped driver's pInfo (see
    * above). ABI 12, we call NIDR and are done */
   if (!VMMouseClient_Enable()) {
      xf86Msg(X_ERROR, "VMWARE(0): vmmouse enable failed\n");
      return VMMouseInitPassthru(drv, pInfo, flags);
   } else {
      xf86Msg(X_INFO, "VMWARE(0): vmmouse is available\n");
      VMMouseClient_Disable();
   }
#endif

   mPriv = calloc (1, sizeof (VMMousePrivRec));

   if (!mPriv) {
      rc = BadAlloc;
      goto error;
   }

   mPriv->absoluteRequested = FALSE;
   mPriv->vmmouseAvailable = TRUE;

   /* Settup the pInfo */
   pInfo->type_name = XI_MOUSE;
   pInfo->device_control = VMMouseDeviceControl;
   pInfo->read_input = VMMouseReadInput;
   pInfo->control_proc = VMMouseControlProc;
   pInfo->switch_mode = VMMouseSwitchMode;

   /* Allocate the MouseDevRec and initialise it. */
   if (!(pMse = calloc(sizeof(MouseDevRec), 1))) {
      rc = BadAlloc;
      goto error;
   }

   pInfo->private = pMse;
   pMse->Ctrl = MouseCtrl;
   pMse->PostEvent = VMMousePostEvent;
   pMse->CommonOptions = MouseCommonOptions;
   pMse->mousePriv = mPriv;


   /* Check if the device can be opened. */
   pInfo->fd = xf86OpenSerial(pInfo->options);
   if (pInfo->fd == -1) {
      if (xf86GetAllowMouseOpenFail())
	 xf86Msg(X_WARNING, "%s: cannot open input device\n", pInfo->name);
      else {
	 xf86Msg(X_ERROR, "%s: cannot open input device\n", pInfo->name);
	 rc = BadValue;
	 goto error;
      }
   }
   xf86CloseSerial(pInfo->fd);
   pInfo->fd = -1;

   /* Process the options */
   pMse->CommonOptions(pInfo);

   /* set up the current screen num */
   mPriv->screenNum = xf86SetIntOption(pInfo->options, "ScreenNumber", 0);

   return Success;

error:
   pInfo->private = NULL;
   if (mPriv)
      free(mPriv);
   if (pMse)
      free(pMse);

   return rc;
}


/*
 *----------------------------------------------------------------------
 *
 * MouseCtrl --
 *     Alter the control paramters for the mouse.
 *
 * Results:
 * 	None
 *
 * Side effects:
 * 	None
 *
 *----------------------------------------------------------------------
 */

static void
MouseCtrl(DeviceIntPtr device, PtrCtrl *ctrl)
{
    InputInfoPtr pInfo;
    MouseDevPtr pMse;

    pInfo = device->public.devicePrivate;
    pMse = pInfo->private;

#ifdef EXTMOUSEDEBUG
    xf86Msg(X_INFO, "VMMOUSE(0): MouseCtrl pMse=%p\n", pMse);
#endif

    pMse->num       = ctrl->num;
    pMse->den       = ctrl->den;
    pMse->threshold = ctrl->threshold;
}


/*
 *----------------------------------------------------------------------
 *
 * VMMouseDoPostEvent --
 *	Post the mouse button event and mouse motion event to Xserver
 *
 * Results:
 * 	None
 *
 * Side effects:
 * 	Mouse location and button status was updated
 *
 *----------------------------------------------------------------------
 */

static void
VMMouseDoPostEvent(InputInfoPtr pInfo, int buttons, int dx, int dy)
{
    MouseDevPtr pMse;
    VMMousePrivPtr mPriv;
    int truebuttons;
    int id, change;
    Bool mouseMoved = FALSE;

    pMse = pInfo->private;
    mPriv = (VMMousePrivPtr)pMse->mousePriv;

    /*
     * The following truebuttons/reverseBits and lastButtons are
     * used to compare the current buttons and the previous buttons
     * to find the button changes during two mouse events
     */
    truebuttons = buttons;

    buttons = reverseBits(reverseMap, buttons);

    if (mPriv->isCurrRelative) {
       mouseMoved = dx || dy;
    } else {
       mouseMoved = (dx != mPriv->vmmousePrevInput.X) ||
                    (dy != mPriv->vmmousePrevInput.Y) ||
                    (mPriv->vmmousePrevInput.Flags & VMMOUSE_MOVE_RELATIVE);
    }
    if (mouseMoved) {

#ifdef CALL_CONVERSION_PROC
        /*
         * Xservers between 1.3.99.0 - 1.4.0.90 do not call conversion_proc, so
         * we need to do the conversion from device to screen space.
         */
        VMMouseConvertProc(pInfo, 0, 2, dx, dy, 0, 0, 0, 0, &dx, &dy);
#endif
        xf86PostMotionEvent(pInfo->dev, !mPriv->isCurrRelative, 0, 2, dx, dy);
    }

    if (truebuttons != pMse->lastButtons) {
       change = buttons ^ reverseBits(reverseMap, pMse->lastButtons);
       while (change) {
	  id = ffs(change);
	  change &= ~(1 << (id - 1));
	  xf86PostButtonEvent(pInfo->dev, 0, id,
			      (buttons & (1 << (id - 1))), 0, 0);
       }
       pMse->lastButtons = truebuttons;
    }
}


/*
 *----------------------------------------------------------------------
 *
 * VMMousePostEvent --
 *	Prepare the mouse status according to the Z axis mapping
 *	before we post the event to Xserver
 *
 * Results:
 * 	None
 *
 * Side effects:
 * 	Buttons was updated according to Z axis mapping
 *
 *----------------------------------------------------------------------
 */

static void
VMMousePostEvent(InputInfoPtr pInfo, int buttons, int dx, int dy, int dz, int dw)
{
    MouseDevPtr pMse;
    int zbutton = 0;
    VMMousePrivPtr mPriv;

    pMse = pInfo->private;
    mPriv = (VMMousePrivPtr)pMse->mousePriv;
    /* Map the Z axis movement. */
    /* XXX Could this go in the conversion_proc? */
    switch (pMse->negativeZ) {
    case MSE_NOZMAP:	/* do nothing */
	break;
    case MSE_MAPTOX:
	if (dz != 0) {
	   if(mPriv->isCurrRelative)
	      dx = dz;
	   else
	      dx += dz;
	    dz = 0;
	}
	break;
    case MSE_MAPTOY:
	if (dz != 0) {
	   if(mPriv->isCurrRelative)
	      dy = dz;
	   else
	      dy += dz;
	    dz = 0;
	}
	break;
    default:	/* buttons */
	buttons &= ~(pMse->negativeZ | pMse->positiveZ
		   | pMse->negativeW | pMse->positiveW);
	if (dw < 0 || dz < -1) {
	    zbutton = pMse->negativeW;
	}
	else if (dz < 0) {
	    zbutton = pMse->negativeZ;
	}
	else if (dw > 0 || dz > 1) {
	    zbutton = pMse->positiveW;
	}
	else if (dz > 0) {
	    zbutton = pMse->positiveZ;
	}
	buttons |= zbutton;
	dz = 0;
	break;
    }

    VMMouseDoPostEvent(pInfo, buttons, dx, dy);

    /*
     * If dz has been mapped to a button `down' event, we need to cook up
     * a corresponding button `up' event.
     */
    if (zbutton) {
	buttons &= ~zbutton;
	if(mPriv->isCurrRelative)
	   VMMouseDoPostEvent(pInfo, buttons, 0, 0);
	else
	   VMMouseDoPostEvent(pInfo, buttons, dx, dy);
    }
}


/*
 *----------------------------------------------------------------------
 *
 * FlushButtons --
 *
 * 	FlushButtons -- reset button states.
 *
 * Results:
 * 	None
 *
 * Side effects:
 * 	None
 *
 *----------------------------------------------------------------------
 */

static void
FlushButtons(MouseDevPtr pMse)
{
    pMse->lastButtons = 0;
}


/*
 *----------------------------------------------------------------------
 *
 * MouseCommonOptions --
 *	Process acceptable mouse options. Currently we only process
 *	"Buttons" and "ZAxisMapping" options.
 *	More options can be added later on
 *
 * Results:
 * 	None
 *
 * Side effects:
 * 	The buttons was setup according to the options
 *
 *----------------------------------------------------------------------
 */

static void
MouseCommonOptions(InputInfoPtr pInfo)
{
   MouseDevPtr pMse;
   char *s;

   pMse = pInfo->private;

   pMse->buttons = xf86SetIntOption(pInfo->options, "Buttons", 0);
   if (!pMse->buttons) {
      pMse->buttons = MSE_DFLTBUTTONS;
   }

   /*
    * "emulate3Buttons" and "Drag Lock" is not supported
    */

   /*
    * Process option for ZAxisMapping
    */
   s = xf86SetStrOption(pInfo->options, "ZAxisMapping", "4 5");
   if (s) {
      int b1 = 0, b2 = 0, b3 = 0, b4 = 0;
      char *msg = NULL;

      if (!xf86NameCmp(s, "x")) {
	 pMse->negativeZ = pMse->positiveZ = MSE_MAPTOX;
	 pMse->negativeW = pMse->positiveW = MSE_MAPTOX;
	 msg = xstrdup("X axis");
      } else if (!xf86NameCmp(s, "y")) {
	 pMse->negativeZ = pMse->positiveZ = MSE_MAPTOY;
	 pMse->negativeW = pMse->positiveW = MSE_MAPTOY;
	 msg = xstrdup("Y axis");
      } else if (sscanf(s, "%d %d %d %d", &b1, &b2, &b3, &b4) >= 2 &&
		 b1 > 0 && b1 <= MSE_MAXBUTTONS &&
		 b2 > 0 && b2 <= MSE_MAXBUTTONS) {
	 msg = xstrdup("buttons XX and YY");
	 if (msg)
	    sprintf(msg, "buttons %d and %d", b1, b2);
	 pMse->negativeZ = pMse->negativeW = 1 << (b1-1);
	 pMse->positiveZ = pMse->positiveW = 1 << (b2-1);
	 if (b1 > pMse->buttons) pMse->buttons = b1;
	 if (b2 > pMse->buttons) pMse->buttons = b2;

	 /*
	  * Option "ZAxisMapping" "N1 N2 N3 N4" not supported
	  */
	 pMse->negativeW = pMse->positiveW = MSE_NOZMAP;
      } else {
	 pMse->negativeZ = pMse->positiveZ = MSE_NOZMAP;
	 pMse->negativeW = pMse->positiveW = MSE_NOZMAP;
      }
      if (msg) {
	 xf86Msg(X_CONFIG, "%s: ZAxisMapping: %s\n", pInfo->name, msg);
	 free(msg);
      } else {
	 xf86Msg(X_WARNING, "%s: Invalid ZAxisMapping value: \"%s\"\n",
		 pInfo->name, s);
      }
   }
}


/*
 *----------------------------------------------------------------------
 *
 * VMMouseUnInit --
 * 	This function was supposed to be called by Xserver to do Un-Init.
 *	But it was unused now
 *
 * Results:
 * 	None
 *
 * Side effects:
 * 	None
 *
 *----------------------------------------------------------------------
 */

static void
VMMouseUnInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
{
   MouseDevPtr pMse = pInfo->private;

   xf86Msg(X_INFO, "VMWARE(0): VMMouseUnInit\n");

   if (pMse) {
       VMMousePrivPtr mPriv = (VMMousePrivPtr)pMse->mousePriv;
       free(mPriv);
   }

   xf86DeleteInput(pInfo, flags);
}


/*
 *----------------------------------------------------------------------
 *
 * VMMouseDeviceControl --
 * 	This function was called by Xserver during DEVICE_INIT, DEVICE_ON,
 *	DEVICE_OFF and DEVICE_CLOSE phase
 *
 * Results:
 * 	TRUE, if sucessful
 *	FALSE, if failed
 *
 * Side effects:
 * 	Absolute pointing device is enabled during DEVICE_ON
 *	Absolute pointing device is disabled during DEVICE_OFF
 *	and DEVICE_CLOSE
 *
 *----------------------------------------------------------------------
 */

static Bool
VMMouseDeviceControl(DeviceIntPtr device, int mode)
{
   InputInfoPtr pInfo;
   MouseDevPtr pMse;
   unsigned char map[MSE_MAXBUTTONS + 1];
   int i;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
   Atom btn_labels[MSE_MAXBUTTONS] = {0};
   Atom axes_labels[2] = { 0, 0 };
#endif

   pInfo = device->public.devicePrivate;
   pMse = pInfo->private;
   pMse->device = device;

   switch (mode){
   case DEVICE_INIT:
      device->public.on = FALSE;
      /*
       * [KAZU-241097] We don't know exactly how many buttons the
       * device has, so setup the map with the maximum number.
       */
      for (i = 0; i < MSE_MAXBUTTONS; i++)
	 map[i + 1] = i + 1;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
      btn_labels[0] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_LEFT);
      btn_labels[1] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_MIDDLE);
      btn_labels[2] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_RIGHT);
      btn_labels[3] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_WHEEL_UP);
      btn_labels[4] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_WHEEL_DOWN);
      btn_labels[5] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_HWHEEL_LEFT);
      btn_labels[6] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_HWHEEL_RIGHT);
      /* other buttons are unknown */

#ifdef ABS_VALUATOR_AXES
      axes_labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_ABS_X);
      axes_labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_ABS_Y);
#else
      axes_labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_X);
      axes_labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_Y);
#endif /* ABS_VALUATOR_AXES */
#endif

      InitPointerDeviceStruct((DevicePtr)device, map,
			      min(pMse->buttons, MSE_MAXBUTTONS),
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
				btn_labels,
#endif
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) == 0
				miPointerGetMotionEvents,
#elif GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 3
                                GetMotionHistory,
#endif
                                pMse->Ctrl,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) == 0
				miPointerGetMotionBufferSize()
#else
                                GetMotionHistorySize(), 2
#endif
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
				, axes_labels
#endif
                                );

      /* X valuator */
#ifdef ABS_VALUATOR_AXES
      xf86InitValuatorAxisStruct(device, 0,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
				axes_labels[0],
#endif
				0, 65535, 10000, 0, 10000
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
                                , Absolute
#endif
                                );
#else
      xf86InitValuatorAxisStruct(device, 0,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
				axes_labels[0],
#endif
				0, -1, 1, 0, 1
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
                                , Relative
#endif
                                );
#endif
      xf86InitValuatorDefaults(device, 0);
      /* Y valuator */
#ifdef ABS_VALUATOR_AXES
      xf86InitValuatorAxisStruct(device, 1,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
				axes_labels[1],
#endif
				0, 65535, 10000, 0, 10000
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
                                , Absolute
#endif
                                );
#else
      xf86InitValuatorAxisStruct(device, 1,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
				axes_labels[1],
#endif
				0, -1, 1, 0, 1
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
                                , Relative
#endif
                                );
#endif
      xf86InitValuatorDefaults(device, 1);
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) == 0
      xf86MotionHistoryAllocate(pInfo);
#endif

      xf86Msg(X_INFO, "VMWARE(0): VMMOUSE DEVICE_INIT\n");
#ifdef EXTMOUSEDEBUG
      xf86Msg(X_INFO, "assigning %p atom=%d name=%s\n", device, pInfo->atom,
	     pInfo->name);
#endif
      break;

   case DEVICE_ON:
      xf86Msg(X_INFO, "VMWARE(0): VMMOUSE DEVICE_ON\n");
      pInfo->fd = xf86OpenSerial(pInfo->options);
      if (pInfo->fd == -1)
	 xf86Msg(X_WARNING, "%s: cannot open input device\n", pInfo->name);
      else {
	 pMse->buffer = XisbNew(pInfo->fd, 64);
	 if (!pMse->buffer) {
	    xf86CloseSerial(pInfo->fd);
	    pInfo->fd = -1;
	 } else {
	    VMMousePrivPtr mPriv = (VMMousePrivPtr)pMse->mousePriv;
	    if (mPriv != NULL) {
	       /*
		* enable absolute pointing device here
		*/
	       if (!VMMouseClient_Enable()) {
		  xf86Msg(X_ERROR, "VMWARE(0): vmmouse enable failed\n");
		  mPriv->vmmouseAvailable = FALSE;
		  device->public.on = FALSE;
		  return FALSE;
	       } else {
		  mPriv->vmmouseAvailable = TRUE;
		  xf86Msg(X_INFO, "VMWARE(0): vmmouse enabled\n");
	       }
	    }
	    xf86FlushInput(pInfo->fd);
	    xf86AddEnabledDevice(pInfo);
	 }
      }
      pMse->lastButtons = 0;
      device->public.on = TRUE;
      FlushButtons(pMse);
      break;
   case DEVICE_OFF:
   case DEVICE_CLOSE:
      xf86Msg(X_INFO, "VMWARE(0): VMMOUSE DEVICE_OFF/CLOSE\n");

      if (pInfo->fd != -1) {
	 VMMousePrivPtr mPriv = (VMMousePrivPtr)pMse->mousePriv;
	 if( mPriv->vmmouseAvailable ) {
	    VMMouseClient_Disable();
            mPriv->vmmouseAvailable = FALSE;
            mPriv->absoluteRequested = FALSE;
	 }

	 xf86RemoveEnabledDevice(pInfo);
	 if (pMse->buffer) {
	    XisbFree(pMse->buffer);
	    pMse->buffer = NULL;
	 }
	 xf86CloseSerial(pInfo->fd);
	 pInfo->fd = -1;
      }
      device->public.on = FALSE;
      usleep(300000);
      break;

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) * 100 + GET_ABI_MINOR(ABI_XINPUT_VERSION) >= 1901
   case  DEVICE_ABORT:
      if (pInfo->fd != -1) {
	 VMMousePrivPtr mPriv = (VMMousePrivPtr)pMse->mousePriv;
	 if( mPriv->vmmouseAvailable )
	    VMMouseClient_Disable();
         break;
      }
#endif
   }

   return Success;
}


/*
 *----------------------------------------------------------------------
 *
 * VMMouseReadInput --
 * 	This function was called by Xserver when there is data available
 *	in the input device
 *
 * Results:
 * 	None
 *
 * Side effects:
 * 	Input data in regular PS/2 fd was cleared
 *	Real mouse data was read from the absolute pointing device
 *	and posted to Xserver
 *
 *----------------------------------------------------------------------
 */

static void
VMMouseReadInput(InputInfoPtr pInfo)
{
   MouseDevPtr pMse;
   VMMousePrivPtr mPriv;
   int c;
   int len = 0;

   pMse = pInfo->private;
   mPriv = pMse->mousePriv;

   if (!mPriv->absoluteRequested) {
      /*
       * We can request for absolute mode, but it depends on
       * host whether it will send us absolute or relative
       * position.
       */
      VMMouseClient_RequestAbsolute();
      mPriv->absoluteRequested = TRUE;
      LogMessageVerbSigSafe(X_INFO, -1, "VMWARE(0): vmmouse enable absolute mode\n");
   }

   /*
    * First read the bytes in input device to clear the regular PS/2 fd so
    * we don't get called again.
    */
   /*
    * Set blocking to -1 on the first call because we know there is data to
    * read. Xisb automatically clears it after one successful read so that
    * succeeding reads are preceeded by a select with a 0 timeout to prevent
    * read from blocking indefinitely.
    */
   XisbBlockDuration(pMse->buffer, -1);
   while ((c = XisbRead(pMse->buffer)) >= 0) {
      len++;
      /*
       * regular PS packet consists of 3 bytes
       * We read 3 bytes to drain the PS/2 packet
       */
      if(len < 3) continue;
      len = 0;
      /*
       * Now get the real data from absolute pointing device
       */
      GetVMMouseMotionEvent(pInfo);
   }
   /*
    * There maybe still vmmouse data available
    */
   GetVMMouseMotionEvent(pInfo);
}


/*
 *----------------------------------------------------------------------
 *
 * GetVMMouseMotionEvent --
 * 	Read all the mouse data available from the absolute
 * 	pointing device	and post it to the Xserver
 *
 * Results:
 * 	None
 *
 * Side effects:
 *	Real mouse data was read from the absolute pointing
 *	device and posted to Xserver
 *
 *----------------------------------------------------------------------
 */

static void
GetVMMouseMotionEvent(InputInfoPtr pInfo){
   MouseDevPtr pMse;
   VMMousePrivPtr mPriv;
   int buttons, dx, dy, dz, dw;
   VMMOUSE_INPUT_DATA  vmmouseInput;
   int numPackets;

   pMse = pInfo->private;
   mPriv = (VMMousePrivPtr)pMse->mousePriv;
   while((numPackets = VMMouseClient_GetInput(&vmmouseInput))){
      int ps2Buttons = 0;
      if (numPackets == VMMOUSE_ERROR) {
         VMMouseClient_Disable();
         VMMouseClient_Enable();
         VMMouseClient_RequestAbsolute();
         LogMessageVerbSigSafe(X_INFO, -1, "VMWARE(0): re-requesting absolute mode after reset\n");
         break;
      }

      if(vmmouseInput.Buttons & VMMOUSE_MIDDLE_BUTTON)
	 ps2Buttons |= 0x04; 			/* Middle*/
      if(vmmouseInput.Buttons & VMMOUSE_RIGHT_BUTTON)
	 ps2Buttons |= 0x02; 			/* Right*/
      if(vmmouseInput.Buttons & VMMOUSE_LEFT_BUTTON)
	 ps2Buttons |= 0x01; 			/* Left*/

      buttons = (ps2Buttons & 0x04) >> 1 |	/* Middle */
	 (ps2Buttons & 0x02) >> 1 |       	/* Right */
	 (ps2Buttons & 0x01) << 2;       	/* Left */

      dx = vmmouseInput.X;
      dy = vmmouseInput.Y;
      dz = (char)vmmouseInput.Z;
      dw = 0;
      /*
       * Get the per package relative or absolute information.
       */
      mPriv->isCurrRelative = vmmouseInput.Flags & VMMOUSE_MOVE_RELATIVE;
      /* post an event */
      pMse->PostEvent(pInfo, buttons, dx, dy, dz, dw);
      mPriv->vmmousePrevInput = vmmouseInput;
   }
}


/*
 *----------------------------------------------------------------------
 *
 * VMMouseControlProc --
 *	This function is unused
 *
 * Results:
 * 	None
 *
 * Side effects:
 * 	None
 *
 *----------------------------------------------------------------------
 */

static int
VMMouseControlProc(InputInfoPtr pInfo, xDeviceCtl * control)
{
   xf86Msg(X_INFO, "VMWARE(0): VMMouseControlProc\n");
   return (Success);
}


/*
 *----------------------------------------------------------------------
 *
 *  VMMouseCloseProc --
 *	This function is unused
 *
 * Results:
 * 	None
 *
 * Side effects:
 * 	None
 *
 *----------------------------------------------------------------------
 */

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 12
static void
VMMouseCloseProc(InputInfoPtr pInfo)
{
   xf86Msg(X_INFO, "VMWARE(0): VMMouseCloseProc\n");
}
#endif


/*
 *----------------------------------------------------------------------
 *
 *  VMMouseSwitchProc --
 *	This function is unused
 *
 * Results:
 * 	None
 *
 * Side effects:
 * 	None
 *
 *----------------------------------------------------------------------
 */

static int
VMMouseSwitchMode(ClientPtr client, DeviceIntPtr dev, int mode)
{
   xf86Msg(X_INFO, "VMWARE(0): VMMouseSwitchMode\n");
   return (Success);
}


/*
 *----------------------------------------------------------------------
 *
 * VMMouseConvertProc  --
 * 	This function was called by Xserver to convert valuators to X and Y
 *
 * Results:
 * 	TRUE
 *
 * Side effects:
 * 	X and Y was converted according to current Screen dimension
 *
 *----------------------------------------------------------------------
 */

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 12
static Bool
VMMouseConvertProc(InputInfoPtr pInfo, int first, int num, int v0, int v1, int v2,
	     int v3, int v4, int v5, int *x, int *y)
{
   MouseDevPtr pMse;
   VMMousePrivPtr mPriv;
   double factorX, factorY;

   pMse = pInfo->private;
   mPriv = pMse->mousePriv;

   if (first != 0 || num != 2)
      return FALSE;

   if(mPriv->isCurrRelative) {
      *x = v0;
      *y = v1;
   } else {
      factorX = ((double) screenInfo.screens[mPriv->screenNum]->width) / (double) 65535;
      factorY = ((double) screenInfo.screens[mPriv->screenNum]->height) / (double) 65535;

      *x = v0 * factorX + 0.5;
      *y = v1 * factorY + 0.5;

      if (mPriv->screenNum != -1) {
	 xf86XInputSetScreen(pInfo, mPriv->screenNum, *x, *y);
      }
   }
   return TRUE;
}
#endif


#ifdef XFree86LOADER

/*
 *----------------------------------------------------------------------
 *
 * VMMouseUnplug  --
 * 	This function was called by Xserver when unplug
 *
 * Results:
 * 	None
 *
 * Side effects:
 * 	None
 *
 *----------------------------------------------------------------------
 */

static void
VMMouseUnplug(pointer p)
{
   xf86Msg(X_INFO, "VMWARE(0): VMMouseUnplug\n");
}


/*
 *----------------------------------------------------------------------
 *
 * VMMousePlug  --
 * 	This function was called when Xserver load vmmouse module. It will
 * 	integrate the  module infto the XFree86 loader architecutre.
 *
 * Results:
 * 	TRUE
 *
 * Side effects:
 * 	Regular mouse module was loaded as a submodule. In case
 * 	absolute pointing device is not available, we can always fall back
 *	to the regular mouse module
 *
 *----------------------------------------------------------------------
 */

static pointer
VMMousePlug(pointer	module,
	    pointer	options,
	    int		*errmaj,
	    int		*errmin)
{
   static Bool Initialised = FALSE;

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 7
   xf86LoaderReqSymLists(reqSymbols, NULL);
#endif

   if (!Initialised)
      Initialised = TRUE;

   xf86Msg(X_INFO, "VMWARE(0): VMMOUSE module was loaded\n");
   xf86AddInputDriver(&VMMOUSE, module, 0);

#ifndef NO_MOUSE_MODULE
{
   char *name;
   /*
    * Load the normal mouse module as submodule
    * If we fail in PreInit later, this allows us to fall back to normal mouse module
    */
#ifndef NORMALISE_MODULE_NAME
   name = xstrdup("mouse");
#else
   /* Normalise the module name */
   name = xf86NormalizeName("mouse");
#endif

   if (!LoadSubModule(module, name, NULL, NULL, NULL, NULL, errmaj, errmin)) {
      LoaderErrorMsg(NULL, name, *errmaj, *errmin);
   }
   free(name);
}
#endif

   return module;
}

static XF86ModuleVersionInfo VMMouseVersionRec = {
   "vmmouse",
   MODULEVENDORSTRING,
   MODINFOSTRING1,
   MODINFOSTRING2,
   XORG_VERSION_CURRENT,
   PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, PACKAGE_VERSION_PATCHLEVEL,
   ABI_CLASS_XINPUT,
   ABI_XINPUT_VERSION,
   MOD_CLASS_XINPUT,
   {0, 0, 0, 0}		/* signature, to be patched into the file by a tool */
};

/*
 * The variable contains the necessary information to load and initialize the module
 */
_X_EXPORT XF86ModuleData vmmouseModuleData = {
   &VMMouseVersionRec,
   VMMousePlug,
   VMMouseUnplug
};
#endif /* XFree86LOADER */
