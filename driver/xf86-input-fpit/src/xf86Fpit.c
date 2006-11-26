/*
 * Copyright 1995, 1999 by Patrick Lecoanet, France. <lecoanet@cena.dgac.fr>
 * Copyright 2002, Red Hat <alan@redhat.com>
 *
 * This driver is a merge of the Elographics driver (from Patrick Lecoanet) and
 * the driver for Fujitsu Pen Computers from Rob Tsuk and John Apfelbaum.
 * 
 * Modified for Stylistic 3400 passive pen support by David Clay
 * Fixed processing of all packets
 * Detangled and simplified if-statement logic
 * Fixed hover-mode pointer movement
 * Added Passive parameter for passive displays
 * Added switch 3 for "right" mouse button
 * I might have broken active pen support. I can't test it.
 *   January 2005 <dave at claysrus.com>
 *
 * Stylistic 500, 1000, 1200, 2300 Support fixed by John Apfelbaum
 *   June 2001 <johnapf@linuxlsate.com>
 *
 * Richard Miller-Smith <Richard.Miller-Smith@philips.com>
 *
 * Fixed up for XFree86 4.2, cleaned up the junk, cured crash on pointer
 * setting.
 *                                                                            
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is  hereby granted without fee, provided that
 * the  above copyright   notice appear  in   all  copies and  that both  that
 * copyright  notice   and   this  permission   notice  appear  in  supporting
 * documentation, and that   the  name of  Patrick  Lecoanet not  be  used  in
 * advertising or publicity pertaining to distribution of the software without
 * specific,  written      prior  permission.     The Authors   make  no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.                   
 *                                                                            
 * PATRICK LECOANET DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT  SHALL PATRICK LECOANET BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA  OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT  SHALL RED HAT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA  OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */
/* $XFree86: xc/programs/Xserver/hw/xfree86/input/fpit/xf86Fpit.c,v 1.6 2004/04/26 22:48:21 dawes Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xf86Version.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#  include <misc.h>
#  include <xf86.h>
#  if !defined(DGUX)
#  endif
#  include <xf86_OSproc.h>
#  include <xf86Xinput.h>
#  include <exevents.h>

#  include <xf86Module.h>


/*
 ***************************************************************************
 *
 * Default constants.
 *
 ***************************************************************************
 */
#define FPIT_LINK_SPEED		B19200	/* 19200 Baud                           */
#define FPIT_PORT		"/dev/ttyS3"

#define FPIT_MAX_X		4100
#define FPIT_MIN_X		0
#define FPIT_MAX_Y		4100
#define FPIT_MIN_Y		0

#define PHASING_BIT	0x80
#define PROXIMITY_BIT	0x20 /* DMC: This was 0x40 but the chart says its bit 5 which is 0x20 */
/*#define TABID_BIT	0x20  */
#define XSIGN_BIT	0x10
#define YSIGN_BIT	0x08
#define BUTTON_BITS	0x07
#define COORD_BITS	0x7f

/* DMC: Added these */
#define SW1	0x01
#define SW2	0x02
#define SW3	0x04


/*
 ***************************************************************************
 *
 * Protocol constants.
 *
 ***************************************************************************
 */
#define FPIT_PACKET_SIZE		5
#define BUFFER_SIZE               (FPIT_PACKET_SIZE*20)

/*
 ***************************************************************************
 *
 * Device private records.
 *
 ***************************************************************************
 */
typedef struct {
	char *fpitDev;		/* device file name */
	int screen_width;
	int screen_height;
	int screen_no;
	int fpitInc;		/* increment between transmits */
	int fpitButTrans;	/* button translation flags */
	int fpitOldX;		/* previous X position */
	int fpitOldY;		/* previous Y position */
	int fpitOldProximity;	/* previous proximity */
	int fpitOldButtons;	/* previous buttons state */
	int fpitMinX;		/* min X value */
	int fpitMinY;		/* min Y value */
	int fpitMaxX;		/* max X value */
	int fpitMaxY;		/* max Y value */
	int fpitInvX;		/* Invert X axis */
	int fpitInvY;		/* Invert Y axis */
	int fpitRes;		/* resolution in lines per inch */
	int flags;		/* various flags */
	int fpitIndex;		/* number of bytes read */
	int fpitBaud;		/* Baud rate of device */
	unsigned char fpitData[BUFFER_SIZE];	/* data read on the device */
	int fpitSwapXY;		/* swap X and Y values */
	int fpitPassive;	/* translate passive buttons */
} FpitPrivateRec, *FpitPrivatePtr;




/*
 ***************************************************************************
 *
 * xf86FpitConvert --
 *	Convert extended valuators to x and y suitable for core motion
 *	events. Return True if ok and False if the requested conversion
 *	can't be done for the specified valuators.
 *
 ***************************************************************************
 */
static Bool xf86FpitConvert(LocalDevicePtr local, int first, int num, int v0, int v1, int v2, int v3, int v4, int v5, int *x, int *y)
{
	FpitPrivatePtr priv = (FpitPrivatePtr) local->private;
	if (first != 0 || num != 2) {
		return FALSE;
	}

	if (priv->fpitSwapXY != 0) {
		*x = xf86ScaleAxis(v1, 0, priv->screen_width, priv->fpitMinY, priv->fpitMaxY);
		*y = xf86ScaleAxis(v0, 0, priv->screen_height, priv->fpitMinX, priv->fpitMaxX);
	} else {
		*x = xf86ScaleAxis(v0, 0, priv->screen_width, priv->fpitMinX, priv->fpitMaxX);
		*y = xf86ScaleAxis(v1, 0, priv->screen_height, priv->fpitMinY, priv->fpitMaxY);
	}
	return TRUE;
}

/*
** xf86FpitReadInput
** Reads from the Fpit and posts any new events to the server.
*/
static void xf86FpitReadInput(LocalDevicePtr local)
{
	FpitPrivatePtr priv = (FpitPrivatePtr) local->private;
	int len, loop;
	int is_core_pointer;
	int x, y, buttons, prox;
	DeviceIntPtr device;
	int conv_x, conv_y;
	
  do { /* keep reading blocks until there are no more */

	/* Read data into buffer */
	len = xf86ReadSerial(local->fd, priv->fpitData+priv->fpitIndex, BUFFER_SIZE-priv->fpitIndex);
	if (len <= 0) {
		Error("error reading FPIT device");
		priv->fpitIndex = 0;
		return;
	}


	/* Since the Fujitsu only delivers data in an absolute mode, we
	   can look through the data backwards to find the last full and valid
	   position. (This may make cursor movement a bit faster) */

	/* DMC: We want to process ALL packets! This way, all points will come
			through and drawing curves are smoother. Also we won't miss any
			button events.
	*/

	priv->fpitIndex += len;

	/* process each packet in this block */
	for (loop=0;loop+FPIT_PACKET_SIZE<=priv->fpitIndex;loop++) { 
		if (!(priv->fpitData[loop] & 0x80)) continue; /* we don't have a start bit yet */

/* Format of 5 bytes data packet for Fpit Tablets
     Byte 1
       bit 7  Phasing bit always 1
       bit 6  Switch status change
       bit 5  Proximity
       bit 4  Always 0
       bit 3  Test data
       bit 2  Sw3 (2nd side sw) 
       bit 1  Sw2 (1st side sw) 
       bit 0  Sw1 (Pen tip sw) 

     Byte 2
       bit 7  Always 0
       bits 6-0 = X6 - X0

     Byte 3
       bit 7  Always 0
       bits 6-0 = X13 - X7

     Byte 4
       bit 7  Always 0
       bits 6-0 = Y6 - Y0

     Byte 5
       bit 7  Always 0
       bits 6-0 = Y13 - Y7
*/

		x = (int) (priv->fpitData[loop + 1] & 0x7f) + ((int) (priv->fpitData[loop + 2] & 0x7f) << 7);
		y = (int) (priv->fpitData[loop + 3] & 0x7f) + ((int) (priv->fpitData[loop + 4] & 0x7f) << 7);
		/* Add in any offsets */
		if (priv->fpitInvX)
			x = priv->fpitMaxX - x + priv->fpitMinX;
		if (priv->fpitInvY)
			y = priv->fpitMaxY - y + priv->fpitMinY;
		prox = (priv->fpitData[loop] & PROXIMITY_BIT) ? 0 : 1;
		buttons = (priv->fpitData[loop] & BUTTON_BITS);
		device = local->dev;
		is_core_pointer = xf86IsCorePointer(device);

		xf86FpitConvert(local, 0, 2, x, y, 0, 0, 0, 0, &conv_x, &conv_y);
		xf86XInputSetScreen(local, priv->screen_no, conv_x, conv_y);

		/* coordonates are ready we can send events */

		if (prox!=priv->fpitOldProximity) /* proximity changed */
			if (!is_core_pointer) xf86PostProximityEvent(device, prox, 0, 2, x, y);

		if (priv->fpitOldX != x || priv->fpitOldY != y) /* position changed */
			xf86PostMotionEvent(device, 1, 0, 2, x, y);

		if (priv->fpitPassive) {
			/*
				For passive pen (Stylistic 3400, et al.):
				sw1 = 1 if pen is moving
				sw1 = 0 if pen is not moving
				sw2 = 0 if pen is contacting the pad
				sw2 = 1 if pen was lifted from the pad
				sw3 = 1 if right mouse-button icon was chosen
			*/
			/* convert the pen button bits to actual mouse buttons */
			if (buttons & SW2) buttons=0; /* the pen was lifted, so no buttons are pressed */
			else if (buttons & SW3) buttons=SW3; /* the "right mouse" button was pressed, so send down event */
			else if (prox) buttons=SW1; /* the "left mouse" button was pressed and we are not hovering, so send down event */
			else buttons=0; /* We are in hover mode, so no buttons */
		}
		else { /* the active pen's buttons map directly to the mouse buttons */
			if (!prox) buttons=0; /* We are in hover mode, so no buttons */
		}
	
		/* DBG(2, ErrorF("%02d/%02d Prox=%d SW:%x Buttons:%x->%x (%d, %d)\n",
			loop,priv->fpitIndex,prox,priv->fpitData[loop]&BUTTON_BITS,priv->fpitOldButtons,buttons,x,y));*/

		if (priv->fpitOldButtons != buttons) {
			int delta;
			delta = buttons ^ priv->fpitOldButtons; /* set delta to the bits that have changed */
			while (delta) {
				int id;
				id = ffs(delta);
				delta &= ~(1 << (id - 1));
				xf86PostButtonEvent(device, 1, id, (buttons & (1 << (id - 1))), 0, 2, x, y);
				/* DBG(1, ErrorF("Button %d %s\n",id,(buttons & (1 << (id - 1)))?"DOWN":"UP"));*/
			}
			priv->fpitOldButtons = buttons;
		}
		priv->fpitOldX = x;
		priv->fpitOldY = y;
		priv->fpitOldProximity = prox;

		loop+=FPIT_PACKET_SIZE-1; /* advance to the next packet */
	} /* for each packet */

	/* remove from the data buffer all that we have processed */
	if (loop<priv->fpitIndex) memmove(priv->fpitData, priv->fpitData+loop,priv->fpitIndex-loop);
	priv->fpitIndex-=loop;

	/* DMC: My system did not read the pen-up event until another event was
			posted, the result was the button sticking down even though
			I had lifted the pen. So I am checking the device for more data
			and then retrieving it. This fixed it for me. I don't know if this is just my system. */

  } while (xf86WaitForInput(local->fd,0)>0); /* go back and check for more data (we don't want to block for I/O!) */

	return;
}

static void xf86FpitPtrCtrl(DeviceIntPtr device, PtrCtrl *ctrl)
{
	/* I have no clue what this does, except that registering it stops the 
	   X server segfaulting in ProcGetPointerMapping()
	   Ho Hum.
	*/
}


/*
 ***************************************************************************
 *
 * xf86FpitControl --
 *
 ***************************************************************************
 */
static Bool xf86FpitControl(DeviceIntPtr dev, int mode)
{
	LocalDevicePtr local = (LocalDevicePtr) dev->public.devicePrivate;
	FpitPrivatePtr priv = (FpitPrivatePtr) (local->private);
	unsigned char map[] = {
		0, 1, 2, 3 /* DMC: changed this so we can use all three buttons */
	};


	switch (mode) {
	case DEVICE_INIT:
		{

			if (priv->screen_no >= screenInfo.numScreens || priv->screen_no < 0) {
				priv->screen_no = 0;
			}
			priv->screen_width = screenInfo.screens[priv->screen_no]->width;
			priv->screen_height = screenInfo.screens[priv->screen_no]->height;
			/*
			 * Device reports button press for up to 3 buttons.
			 */
			if (InitButtonClassDeviceStruct(dev, 3, map) == FALSE) {
				ErrorF("Unable to allocate Fpit touchscreen ButtonClassDeviceStruct\n");
				return !Success;
			}

			if (InitFocusClassDeviceStruct(dev) == FALSE) {
				ErrorF("Unable to allocate Fpit touchscreen FocusClassDeviceStruct\n");
				return !Success;
			}

			if(InitPtrFeedbackClassDeviceStruct(dev, xf86FpitPtrCtrl) == FALSE) {
				ErrorF("Unable to allocate PtrFeedBackClassDeviceStruct\n");
			}
	      
			/*
			 * Device reports motions on 2 axes in absolute coordinates.
			 * Axes min and max values are reported in raw coordinates.
			 * Resolution is computed roughly by the difference between
			 * max and min values scaled from the approximate size of the
			 * screen to fit one meter.
			 */
			if (InitValuatorClassDeviceStruct(dev, 2, xf86GetMotionEvents, local->history_size, Absolute) == FALSE) {
				ErrorF("Unable to allocate Elographics touchscreen ValuatorClassDeviceStruct\n");
				return !Success;
			} else {
				InitValuatorAxisStruct(dev, 0, priv->fpitMinX, priv->fpitMaxX, 9500, 0 /* min_res */ ,
						       9500 /* max_res */ );
				InitValuatorAxisStruct(dev, 1, priv->fpitMinY, priv->fpitMaxY, 10500, 0 /* min_res */ ,
						       10500 /* max_res */ );
			}

			if (InitFocusClassDeviceStruct(dev) == FALSE) {
				ErrorF("Unable to allocate Fpit touchscreen FocusClassDeviceStruct\n");
			}
			/*
			 * Allocate the motion events buffer.
			 */
			xf86MotionHistoryAllocate(local);
			/*
			 * This once has caused the server to crash after doing an xalloc & strcpy ??
			 */
			return Success;
		}

	case DEVICE_ON:
		if (local->fd < 0) {
			local->fd = xf86OpenSerial(local->options);
			if (local->fd < 0) {
				Error("Unable to open Fpit touchscreen device");
				return !Success;
			}

			xf86AddEnabledDevice(local);
			dev->public.on = TRUE;
		}
		return Success;

		/*
		 * Deactivate the device. After this, the device will not emit
		 * events until a subsequent DEVICE_ON. Thus, we can momentarily
		 * close the port.
		 */
	case DEVICE_OFF:
		dev->public.on = FALSE;
		if (local->fd >= 0) {
			xf86RemoveEnabledDevice(local);
		}
		xf86CloseSerial(local->fd);
		local->fd = -1;
		return Success;
		/*
		 * Final close before server exit. This is used during server shutdown.
		 * Close the port and free all the resources.
		 */
	case DEVICE_CLOSE:
		dev->public.on = FALSE;
		if (local->fd >= 0) {
			RemoveEnabledDevice(local->fd);
		}
		xf86CloseSerial(local->fd);
		local->fd = -1;
		return Success;
	default:
		ErrorF("unsupported mode=%d\n", mode);
		return !Success;
	}
}

/*
 ***************************************************************************
 *
 * xf86FpitAllocate --
 *
 ***************************************************************************
 */
static LocalDevicePtr xf86FpitAllocate(InputDriverPtr drv)
{
	LocalDevicePtr local;
	FpitPrivatePtr priv;
	priv = xalloc(sizeof(FpitPrivateRec));
	if (!priv)
		return NULL;
	local = xf86AllocateInput(drv, 0);
	if (!local) {
		xfree(priv);
		return NULL;
	}

	priv->fpitDev = strdup(FPIT_PORT);
	priv->screen_no = 0;
	priv->screen_width = -1;
	priv->screen_height = -1;
	priv->fpitMinX = FPIT_MIN_X;
	priv->fpitMaxX = FPIT_MAX_X;
	priv->fpitMinY = FPIT_MIN_Y;
	priv->fpitMaxY = FPIT_MAX_Y;
	priv->fpitOldX = priv->fpitOldY = -1;
	priv->fpitOldButtons = 0;
	priv->fpitOldProximity = 0;
	priv->fpitIndex = 0;
	priv->fpitSwapXY = 0;
	priv->fpitPassive = 0;
	local->name = XI_TOUCHSCREEN;
	local->flags = 0 /* XI86_NO_OPEN_ON_INIT */ ;
	local->device_control = xf86FpitControl;
	local->read_input = xf86FpitReadInput;
	local->control_proc = NULL;
	local->close_proc = NULL;
	local->switch_mode = NULL;
	local->conversion_proc = xf86FpitConvert;
	local->reverse_conversion_proc = NULL;
	local->fd = -1;
	local->atom = 0;
	local->dev = NULL;
	local->private = priv;
	local->type_name = "Fujitsu Stylistic";
	local->history_size = 0;
	return local;
}

static void xf86FpitUninit(InputDriverPtr drv, LocalDevicePtr local, int flags)
{
	FpitPrivatePtr priv = (FpitPrivatePtr) local->private;
	xf86FpitControl(local->dev, DEVICE_OFF);
	xfree(priv->fpitDev);
	xfree(priv);
	xfree(local->name);
	xfree(local);
	xf86DeleteInput(local, 0);
}

static const char *default_options[] = {
	"BaudRate", "19200", "StopBits", "0", "DataBits", "8", "Parity", "None", "Vmin", "10", "Vtime", "1", "FlowControl", "None", NULL
};

static InputInfoPtr xf86FpitInit(InputDriverPtr drv, IDevPtr dev, int flags)
{
	LocalDevicePtr local = NULL;
	FpitPrivatePtr priv = NULL;
	char *str;

	local = xf86FpitAllocate(drv);
	if (!local)
		return NULL;

	priv = local->private;
	local->conf_idev = dev;
	xf86CollectInputOptions(local, default_options, NULL);
	/* Process the common options. */
	xf86ProcessCommonOptions(local, local->options);
	str = xf86FindOptionValue(local->options, "Device");
	if (!str) {
		xf86Msg(X_ERROR, "%s: No Device specified in FPIT module config.\n", dev->identifier);
		if (priv) {
			if (priv->fpitDev) {
				xfree(priv->fpitDev);
			}
			xfree(priv);
		}
		return local;
	}
	priv->fpitDev = strdup(str);
	local->name = xf86SetStrOption(local->options, "DeviceName", XI_TOUCHSCREEN);
	xf86Msg(X_CONFIG, "FPIT device name: %s\n", local->name);
	priv->screen_no = xf86SetIntOption(local->options, "ScreenNo", 0);
	xf86Msg(X_CONFIG, "Fpit associated screen: %d\n", priv->screen_no);
	priv->fpitMaxX = xf86SetIntOption(local->options, "MaximumXPosition", 4100);
	xf86Msg(X_CONFIG, "FPIT maximum x position: %d\n", priv->fpitMaxX);
	priv->fpitMinX = xf86SetIntOption(local->options, "MinimumXPosition", 0);
	xf86Msg(X_CONFIG, "FPIT minimum x position: %d\n", priv->fpitMinX);
	priv->fpitMaxY = xf86SetIntOption(local->options, "MaximumYPosition", 4100);
	xf86Msg(X_CONFIG, "FPIT maximum y position: %d\n", priv->fpitMaxY);
	priv->fpitMinY = xf86SetIntOption(local->options, "MinimumYPosition", 0);
	xf86Msg(X_CONFIG, "FPIT minimum y position: %d\n", priv->fpitMinY);
	priv->fpitInvX = xf86SetBoolOption(local->options, "InvertX", 0);
	priv->fpitInvY = xf86SetBoolOption(local->options, "InvertY", 0);
	priv->fpitSwapXY = xf86SetBoolOption(local->options, "SwapXY", 0);
	priv->fpitPassive = xf86SetBoolOption(local->options, "Passive", 0);
	str = xf86SetStrOption(local->options, "Rotate", 0);
	if (!xf86NameCmp(str, "CW")) {
		priv->fpitInvX = 1;
		priv->fpitInvY = 1;
		priv->fpitSwapXY = 1;
	} else if (!xf86NameCmp(str, "CCW")) {
		priv->fpitInvX = 0;
		priv->fpitInvY = 0;
		priv->fpitSwapXY = 1;
	}
	xf86Msg(X_CONFIG, "FPIT invert X axis: %s\n", priv->fpitInvX ? "Yes" : "No");
	xf86Msg(X_CONFIG, "FPIT invert Y axis: %s\n", priv->fpitInvY ? "Yes" : "No");
	xf86Msg(X_CONFIG, "FPIT swap X and Y axis: %s\n", priv->fpitSwapXY ? "Yes" : "No");
	xf86Msg(X_CONFIG, "FPIT Passive button mode: %s\n", priv->fpitPassive ? "Yes" : "No");
	/* mark the device configured */
	local->flags |= XI86_CONFIGURED;
	return local;
}

_X_EXPORT InputDriverRec FPIT = {
	1,			/* driver version */
	"fpit",			/* driver name */
	NULL,			/* identify */
	xf86FpitInit,		/* pre-init */
	xf86FpitUninit,		/* un-init */
	NULL,			/* module */
	0			/* ref count */
};

#ifdef XFree86LOADER
static pointer Plug(pointer module, pointer options, int *errmaj, int *errmin)
{
	xf86AddInputDriver(&FPIT, module, 0);
	return module;
}

static void Unplug(pointer p)
{
}

static XF86ModuleVersionInfo version_rec = {
	"fpit", MODULEVENDORSTRING, MODINFOSTRING1, MODINFOSTRING2, XORG_VERSION_CURRENT, 1, 1, 0, ABI_CLASS_XINPUT, ABI_XINPUT_VERSION, MOD_CLASS_XINPUT, 
	{0, 0, 0, 0}
};

/*
 * This is the entry point in the module. The name
 * is setup after the pattern <module_name>ModuleData.
 * Do not change it.
 */
_X_EXPORT XF86ModuleData fpitModuleData = {
	&version_rec, Plug, Unplug
};
#endif
