/* 
 * Copyright (c) 1999  Machine Vision Holdings Incorporated
 * Author: David Woodhouse <David.Woodhouse@mvhi.com>
 * CoAuthor: Mayk Langer <langer@vsys.de>
 * 
 * History:
 * 02/01/2009: mjs <mjs@core7.eu>
 * - Added DMC9512 controller protocol support
 *   based on old code from http://www.salt.com.tw/Download/Driver/PenMount/DMC9512/
 * 09/16/2005: Jaya Kumar <jayakumar.xorg@gmail.com> 
 * - Added DMC9000 controller protocol support
 * - DMC9000 support work was sponsored by CIS(M) Sdn Bhd
 * 09/15/2005: Original code from David and Mark
 *
 * Template driver used: Copyright (c) 1998  Metro Link Incorporated
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, cpy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of the Metro Link shall not be
 * used in advertising or otherwise to promote the sale, use or other dealings
 * in this Software without prior written authorization from Metro Link.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define _PENMOUNT_C_

#include <misc.h>
#include <xf86.h>
#define NEED_XF86_TYPES
#include <xf86_OSproc.h>
#include <xf86Xinput.h>
#include <xisb.h>
#include <exevents.h>
#include <string.h>
#include "xf86PM.h"

#define TS_Raw 60
#define TS_Scaled 61

/*
 * Be sure to set vmin appropriately for your device's protocol. You want to
 * read a full packet before returning
 */
static char *default_options[] =
{
	/*	"Device", "/dev/ttyS1",*/
	"BaudRate", "19200",
	"StopBits", "1",
	"DataBits", "8",
	"Parity", "None",
	"Vmin", "3",
	"Vtime", "1",
	"FlowControl", "None",
	NULL,
};

_X_EXPORT InputDriverRec PENMOUNT = {
        1,
        "penmount",
        NULL,
        PenMountPreInit,
        /*PenMountUnInit*/NULL,
        NULL,
        default_options
};

static XF86ModuleVersionInfo VersionRec =
{
	"penmount",
	MODULEVENDORSTRING,
	MODINFOSTRING1,
	MODINFOSTRING2,
	XORG_VERSION_CURRENT,
	PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, PACKAGE_VERSION_PATCHLEVEL,
	ABI_CLASS_XINPUT,
	ABI_XINPUT_VERSION,
	MOD_CLASS_XINPUT,
	{0, 0, 0, 0}				/* signature, to be patched into the file by
								 * a tool */
};

static pointer
PenMountSetupProc(	pointer module,
			pointer options,
			int *errmaj,
			int *errmin )
{
	xf86AddInputDriver(&PENMOUNT, module, 0);
	return (pointer) 1;
}

_X_EXPORT XF86ModuleData penmountModuleData = { 
    &VersionRec,
    PenMountSetupProc,
    NULL
};

/*****************************************************************************
 *	Function Definitions
 ****************************************************************************/

static Bool
ProcessDeviceInit(PenMountPrivatePtr priv, DeviceIntPtr dev, InputInfoPtr pInfo)
{
	unsigned char map[] =
	{0, 1};
	int min_x, min_y, max_x, max_y;
	Atom axis_labels[2] = { 0, 0 };
	Atom btn_label = 0;

	/*
	 * these have to be here instead of in the SetupProc, because when the
	 * SetupProc is run at server startup, screenInfo is not setup yet
	 */
	priv->screen_width = screenInfo.screens[priv->screen_num]->width;
	priv->screen_height = screenInfo.screens[priv->screen_num]->height;
		
	/*
	 * Device reports button press for 1 button.
	 */
	if (InitButtonClassDeviceStruct (dev, 1,
                    &btn_label,
                    map) == FALSE)
		{
			ErrorF ("Unable to allocate PenMount ButtonClassDeviceStruct\n");
			return !Success;
		}
		
	/*
	 * Device reports motions on 2 axes in absolute coordinates.
	 * Axes min and max values are reported in raw coordinates.
	 */
	if (InitValuatorClassDeviceStruct (dev, 2, axis_labels,
					   GetMotionHistorySize(), Absolute) == FALSE)
		{
			ErrorF ("Unable to allocate PenMount ValuatorClassDeviceStruct\n");
			return !Success;
		}
	else
		{
			/* max_* min_* refer to the max/min values we will emit to the core.
			 * When reporting mode is TS_Raw since we do no scaling, it will
			 * just be the values in xorg.conf. If however reporting mode is
			 * TS_Scaled however, xf86ScaleAxis will return a value between 0
			 * screen_width/screen_height, so we must setup the axis accordingly
			 * or there will be strange behaviour
			 */
			if ( priv->reporting_mode == TS_Raw )
				{
					max_x = priv->max_x;
					min_x = priv->min_x;
					max_y = priv->max_y;
					min_y = priv->min_y;
				}
			else
				{
					max_x = priv->screen_width;
					min_x = 0;
					max_y = priv->screen_height;
					min_y = 0;
				}

			InitValuatorAxisStruct (dev, 0, axis_labels[0],
						min_x, max_x,
						9500,
						0 /* min_res */ ,
						9500 /* max_res */,
                                                Absolute);
			InitValuatorAxisStruct (dev, 1, axis_labels[1],
						min_y, max_y,
						10500,
						0 /* min_res */ ,
						10500 /* max_res */,
                                                Absolute);
		}
		
	if (InitProximityClassDeviceStruct (dev) == FALSE)
		{
			ErrorF ("unable to allocate PenMount ProximityClassDeviceStruct\n");
			return !Success;
		}
		
	if (InitPtrFeedbackClassDeviceStruct(dev, PenMountPtrCtrl) == FALSE)
		{
			ErrorF ("unable to allocate PenMount PtrFeedbackClassDeviceStruct\n");
			return !Success;
		}
		
	/* 
	 * Allocate the motion events buffer.
	 */
	xf86MotionHistoryAllocate (pInfo);
	return (Success);
}

static Bool
DMC9000_ProcessDeviceOn(PenMountPrivatePtr priv, DeviceIntPtr dev, InputInfoPtr pInfo)
{
	unsigned char	buf[5] = { 0xF2, 0x00, 0x00, 0x00, 0x00 };
	pInfo->fd = xf86OpenSerial(pInfo->options);
	if (pInfo->fd == -1)
	{
		xf86Msg(X_WARNING, "%s: cannot open input device\n", pInfo->name);
		return (!Success);
	}

	priv->buffer = XisbNew(pInfo->fd, 64);
	if (!priv->buffer) 
	{
		xf86CloseSerial(pInfo->fd);
		pInfo->fd = -1;
		return (!Success);
	}

	XisbBlockDuration (priv->buffer, 500000);
	if ( PenMountSendPacket(priv, buf, 5) == Success )
	{
		/* wait for right response */
		priv->lex_mode = PenMount_Response0;
		if (DMC9000_PenMountGetPacket (priv) == Success )
		{
			if ((priv->packet[0] == 0xF2) &&
				(priv->packet[1] == 0xD9) &&
				(priv->packet[2] == 0x0A)) 
			{
				/* enable the DMC9000 */	
				buf[0] = 0xF1;
				buf[1] = 0x00;
				buf[2] = 0x00;
				buf[3] = 0x00;
				buf[4] = 0x00;
				PenMountSendPacket(priv,buf,5);
			}
		}
	}

	XisbBlockDuration (priv->buffer, -1);
	priv->lex_mode = PenMount_byte0;
	
	xf86FlushInput(pInfo->fd);
	AddEnabledDevice (pInfo->fd);
	dev->public.on = TRUE;
	return (Success);
}

static Bool
ProcessDeviceClose(PenMountPrivatePtr priv, DeviceIntPtr dev, InputInfoPtr pInfo)
{
	if (pInfo->fd != -1)
		{ 
			RemoveEnabledDevice (pInfo->fd);
			if (priv->buffer)
				{
					XisbFree(priv->buffer);
					priv->buffer = NULL;
				}
			xf86CloseSerial(pInfo->fd);
		}
	dev->public.on = FALSE;
	return (Success);
}

static Bool
DMC9000_DeviceControl (DeviceIntPtr dev, int mode)
{
	InputInfoPtr pInfo = dev->public.devicePrivate;
	PenMountPrivatePtr priv = (PenMountPrivatePtr) (pInfo->private);

	switch (mode)
	{
	case DEVICE_INIT:
		return ProcessDeviceInit(priv, dev, pInfo);
	case DEVICE_ON:
		return DMC9000_ProcessDeviceOn(priv, dev, pInfo);
	case DEVICE_OFF:
	case DEVICE_CLOSE:
		return ProcessDeviceClose(priv, dev, pInfo);
	default:
		return (BadValue);
	}

}

static Bool
DMC9512_ProcessDeviceOn(PenMountPrivatePtr priv, DeviceIntPtr dev, InputInfoPtr pInfo)
{
	unsigned char	buf[5] = { 'D', 'G', 0x02, 0x80, 0x00 };

	XisbBlockDuration (priv->buffer, 500000);

	if ( PenMountSendPacket(priv, buf, 5) != Success )
	{
		return !Success;
	}

	/* wait for right response */
	priv->lex_mode = PenMount_Response0;

	if (PenMountGetPacket (priv) != Success )
	{
		return !Success;
	}

	if ( ! (priv->packet[0] == 0xff && priv->packet[1] == 0x70) )
	{
		return !Success;
	}

	xf86Msg(X_NOTICE, "%s: DMC9512: found\n", pInfo->name);
	priv->chip = DMC9512;

	/* disable DMC9512 */
	buf[2] = 0x0a;
	buf[3] = 0x00;
	buf[4] = 0x00;
	PenMountSendPacket(priv,buf,5);
	priv->lex_mode = PenMount_Response0;
	PenMountGetPacket(priv);

	/* set screen width */
	buf[2] = 0x02;
	buf[3] = 0x03; /*(priv->screen_width & 0x0fff) >> 8;*/
	buf[4] = 0xfc; /*priv->screen_width & 0xff;*/
	PenMountSendPacket(priv,buf,5);
	priv->lex_mode = PenMount_Response0;
	PenMountGetPacket(priv);

	/* set screen height */
	buf[2] = 0x02;
	buf[3] = 0x13; /*(priv->screen_height & 0x0fff) >> 8;*/
	buf[4] = 0xfc; /*priv->screen_height & 0xff;*/
	buf[3] |= 0x10;
	PenMountSendPacket(priv,buf,5);
	priv->lex_mode = PenMount_Response0;
	PenMountGetPacket(priv);

	/* Set Calibration Data */
	/* Set X-coordinate of the Left Top corner */
	buf[2] = 0x02;
	buf[3] = 0x40;
	buf[4] = 0x03;
	PenMountSendPacket(priv,buf,5);
	priv->lex_mode = PenMount_Response0;
	PenMountGetPacket(priv);

	/* Set Y-coordinate of the Left Top corner */
	buf[2] = 0x02;
	buf[3] = 0x50;
	buf[4] = 0x03;
	PenMountSendPacket(priv,buf,5);
	priv->lex_mode = PenMount_Response0;
	PenMountGetPacket(priv);

	/* Set X-coordinate of the Right bottom corner */
	buf[2] = 0x02;
	buf[3] = 0x60;
	buf[4] = 0xfc;
	PenMountSendPacket(priv,buf,5);
	priv->lex_mode = PenMount_Response0;
	PenMountGetPacket(priv);

	/* Set Y-coordinate of the Right bottom corner */
	buf[2] = 0x02;
	buf[3] = 0x70;
	buf[4] = 0xfc;
	PenMountSendPacket(priv,buf,5);
	priv->lex_mode = PenMount_Response0;
	PenMountGetPacket(priv);

	/* Set Screen Width Again */
	buf[2] = 0x02;
	buf[3] = 0x03;
	buf[4] = 0xfc;
	PenMountSendPacket(priv,buf,5);
	priv->lex_mode = PenMount_Response0;
	PenMountGetPacket(priv);

	/* Set Screen Height Again */
	buf[2] = 0x02;
	buf[3] = 0x13;
	buf[4] = 0xfc;
	PenMountSendPacket(priv,buf,5);
	priv->lex_mode = PenMount_Response0;
	PenMountGetPacket(priv);

	/* enable DMC9512 */
	buf[2] = 0x0a;
	buf[3] = 0x01;
	buf[4] = 0x00;
	PenMountSendPacket(priv,buf,5);
	priv->lex_mode = PenMount_Response0;
	PenMountGetPacket(priv);

	return Success;
}

static int
PenMountPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
{
	PenMountPrivatePtr priv = calloc (1, sizeof (PenMountPrivateRec));
	char *s;

	if (!priv)
		return BadAlloc;

	priv->min_x = 0;
	priv->max_x = 1024;
	priv->min_y = 768;
	priv->max_y = 0;
	priv->screen_num = 0;
	priv->screen_width = -1;
	priv->screen_height = -1;
	priv->lex_mode = PenMount_byte0;
	priv->swap_xy = 0;
	priv->button_down = FALSE;
	priv->button_number = 1;
	priv->proximity = FALSE;
	priv->chip = CHIP_UNKNOWN;
	priv->pen_down = 0;

	pInfo->type_name = XI_TOUCHSCREEN;
	pInfo->device_control = DeviceControl;
	pInfo->read_input = ReadInput;
	pInfo->control_proc = ControlProc;
	pInfo->switch_mode = SwitchMode;
	pInfo->private = priv;

	xf86OptionListReport( pInfo->options );

	pInfo->fd = xf86OpenSerial (pInfo->options);
	if (pInfo->fd == -1)
	{
		ErrorF ("PenMount driver unable to open device\n");
		goto SetupProc_fail;
	}
	xf86CloseSerial(pInfo->fd);
	/* 
	 * Process the options for your device like this
	 */
	priv->min_x = xf86SetIntOption( pInfo->options, "MinX", 0 );
	priv->max_x = xf86SetIntOption( pInfo->options, "MaxX", 1024 );
	priv->min_y = xf86SetIntOption( pInfo->options, "MinY", 768 );
	priv->max_y = xf86SetIntOption( pInfo->options, "MaxY", 0 );
	priv->screen_num = xf86SetIntOption( pInfo->options, "ScreenNumber", 0 );
	priv->button_number = xf86SetIntOption( pInfo->options, "ButtonNumber", 1 );
	priv->swap_xy = xf86SetIntOption( pInfo->options, "SwapXY", 0 );
	priv->invert_y = xf86SetIntOption( pInfo->options, "InvertY", 0 );
	priv->invert_x = xf86SetIntOption( pInfo->options, "InvertX", 0 );
	priv->buffer = NULL;
	s = xf86FindOptionValue (pInfo->options, "ReportingMode");
	if ((s) && (xf86NameCmp (s, "raw") == 0))
		priv->reporting_mode = TS_Raw;
	else
		priv->reporting_mode = TS_Scaled;

	s = xf86FindOptionValue (pInfo->options, "ControllerModel");
	if ((s) && (xf86NameCmp (s, "DMC9000") == 0)) {
		priv->chip = DMC9000;
		pInfo->device_control = DMC9000_DeviceControl;
		pInfo->read_input = DMC9000_ReadInput;
	} else if ((s) && (xf86NameCmp (s, "DMC9512") == 0)) {
		priv->chip = DMC9512;
	}

	priv->proximity = FALSE;
	priv->button_down = FALSE;
	priv->lex_mode = PenMount_byte0;

	if (QueryHardware (priv) != Success)
	{
		ErrorF ("Unable to query/initialize PenMount hardware.\n");
		goto SetupProc_fail;
	}

	/* this results in an xstrdup that must be freed later */
	pInfo->name = xf86SetStrOption( pInfo->options, "DeviceName", "PenMount");

	return Success;

  SetupProc_fail:
	if ((pInfo) && (pInfo->fd))
		xf86CloseSerial (pInfo->fd);
	if ((pInfo) && (pInfo->name))
		free (pInfo->name);

	if ((priv) && (priv->buffer))
		XisbFree (priv->buffer);
	if (priv)
		free (priv);
	return BadValue;
}

static Bool
DeviceControl (DeviceIntPtr dev, int mode)
{
	InputInfoPtr pInfo = dev->public.devicePrivate;
	PenMountPrivatePtr priv = (PenMountPrivatePtr) (pInfo->private);

	switch (mode)
	{
	case DEVICE_INIT:
		return ProcessDeviceInit(priv, dev, pInfo);
	case DEVICE_ON:
		pInfo->fd = xf86OpenSerial(pInfo->options);
		if (pInfo->fd == -1)
		{
			xf86Msg(X_WARNING, "%s: cannot open input device\n", pInfo->name);
			return (!Success);
		}
		else 
		{
			priv->buffer = XisbNew(pInfo->fd, 64);
			if (!priv->buffer) 
			{
				xf86CloseSerial(pInfo->fd);
				pInfo->fd = -1;
				return (!Success);
			}
/*			if (isatty (pInfo->fd))		check if DMC8910 is found */

			if (priv->chip == DMC9512)
			{
				if (DMC9512_ProcessDeviceOn(priv,dev,pInfo) != Success)
				{
					xf86Msg(X_WARNING, "%s: DMC9512: could not initialize", pInfo->name);
					return !Success;
				}
				// else continue to the code below which does common stuff
				// between 8910/9512 again
			}
			else
			{
/* echo Success Code */
				unsigned char	buf[5] = { 'D', 'G', 0x02, 0x80, 0x00 };

				XisbBlockDuration (priv->buffer, 500000);
				if ( PenMountSendPacket(priv, buf, 5) == Success )
				{
					/* wait for right response */
					priv->lex_mode = PenMount_Response0;
					if (PenMountGetPacket (priv) == Success )
					{
						if (( priv->packet[0] == 0xff ) &&
							( priv->packet[1] == 0x70 ) )
						{
							priv->chip = DMC8910;
						/* disable DMC8910 */
							buf[2] = 0x0b;
							buf[3] = 0x00;
							buf[4] = 0x00;
							PenMountSendPacket(priv,buf,5);
							priv->lex_mode = PenMount_Response0;
							PenMountGetPacket(priv);
						/* set screen width */
							buf[2] = 0x02;
							buf[3] = (priv->screen_width & 0x0fff) >> 8;
							buf[4] = priv->screen_width & 0xff;
							PenMountSendPacket(priv,buf,5);
							priv->lex_mode = PenMount_Response0;
							PenMountGetPacket(priv);
						/* set screen height */
							buf[2] = 0x02;
							buf[3] = (priv->screen_height & 0x0fff) >> 8;
							buf[4] = priv->screen_height & 0xff;
							buf[3] |= 0x10;
							PenMountSendPacket(priv,buf,5);
							priv->lex_mode = PenMount_Response0;
							PenMountGetPacket(priv);
						/* enable DMC8910 */
							buf[2] = 0x0a;
							buf[3] = 0x01;
							buf[4] = 0x00;
							PenMountSendPacket(priv,buf,5);
							priv->lex_mode = PenMount_Response0;
							PenMountGetPacket(priv);
						}
					}
				}
			}
		}

		XisbBlockDuration (priv->buffer, -1);
		priv->lex_mode = PenMount_byte0;
		
		xf86FlushInput(pInfo->fd);
		AddEnabledDevice (pInfo->fd);
		dev->public.on = TRUE;
		return (Success);
		
	case DEVICE_OFF:
	case DEVICE_CLOSE:
		return ProcessDeviceClose(priv, dev, pInfo);
	default:
		return (BadValue);
	}

}


/* 
 * The ReadInput function will have to be tailored to your device
 */
static void
ReadInput (InputInfoPtr pInfo)
{
	PenMountPrivatePtr priv = (PenMountPrivatePtr) (pInfo->private);
	int x,y;
	unsigned char opck[ PENMOUNT_PACKET_SIZE ];

	/* 
	 * set blocking to -1 on the first call because we know there is data to
	 * read. Xisb automatically clears it after one successful read so that
	 * succeeding reads are preceeded buy a select with a 0 timeout to prevent
	 * read from blocking indefinately.
	 */
	XisbBlockDuration (priv->buffer, -1);
	while (1)
	{
		if ( priv->chip != DMC8910 && priv->chip != DMC9512 )
		{
			if ( PenMountGetPacket (priv) != Success)
				break;
			if (( priv->packet[0] & 0x0C ) != 0x0C )
				break;
			if (priv->swap_xy) {
				y = priv->packet[1];
				y |= (priv->packet[0] & 0x10) ? 0x100 : 0;
				y |= (priv->packet[0] & 0x40) ? 0x200 : 0;
				x = priv->packet[2];
				x |= (priv->packet[0] & 0x20) ? 0x100 : 0;
				x |= (priv->packet[0] & 0x80) ? 0x200 : 0;
			} else {
				x = priv->packet[1];
				x |= (priv->packet[0] & 0x10) ? 0x100 : 0;
				x |= (priv->packet[0] & 0x40) ? 0x200 : 0;
				y = priv->packet[2];
				y |= (priv->packet[0] & 0x20) ? 0x100 : 0;
				y |= (priv->packet[0] & 0x80) ? 0x200 : 0;
			}
		}
		else
		{
			memcpy(opck,priv->packet,5);
			if ( PenMountGetPacket (priv) != Success )
				break;
			if (( priv->packet[1] == 0xfd ) && ( priv->packet[2] == 0xfd ))
			{
				priv->pen_down = 1;
				continue;
			}
			if (( priv->packet[1] == 0xfe ) && ( priv->packet[2] == 0xfe ))
			{
				memcpy(priv->packet,opck,5);
				if ( !priv->pen_down )
					continue;
				priv->pen_down = 0;
			}
			if ( priv->swap_xy)
			{
				y = priv->packet[1]*256+priv->packet[2];
				x = priv->packet[3]*256+priv->packet[4];
			}
			else
			{
				x = priv->packet[1]*256+priv->packet[2];
				y = priv->packet[3]*256+priv->packet[4];
			}
			priv->packet[0] = priv->pen_down ? 0x01 : 0x00;
		}

		if (priv->reporting_mode == TS_Scaled)
		{	
			x = xf86ScaleAxis (x, 0, priv->screen_width, priv->min_x,
					   priv->max_x);
                        y = xf86ScaleAxis (y, 0, priv->screen_height, priv->min_y,
					   priv->max_y);
                }        		

		xf86XInputSetScreen (pInfo, priv->screen_num, x, y);

		if ((priv->proximity == FALSE) && (priv->packet[0] & 0x01))
		{
			priv->proximity = TRUE;
			xf86PostProximityEvent (pInfo->dev, 1, 0, 2, x, y);
		}              	
		
             /*
                 * Send events.
                 *
                 * We *must* generate a motion before a button change if pointer
                 * location has changed as DIX assumes this. This is why we always
                 * emit a motion, regardless of the kind of packet processed.
                 */

                xf86PostMotionEvent (pInfo->dev, TRUE, 0, 2, x, y);

                /*
                 * Emit a button press or release.
                 */
                if ((priv->button_down == FALSE) && (priv->packet[0] & 0x01))

                {
                        xf86PostButtonEvent (pInfo->dev, TRUE,
					     priv->button_number, 1, 0, 2, x, y);
                        priv->button_down = TRUE;
                }
                if ((priv->button_down == TRUE) && !(priv->packet[0] & 0x01))
                {
                        xf86PostButtonEvent (pInfo->dev, TRUE,
					     priv->button_number, 0, 0, 2, x, y);
                        priv->button_down = FALSE;
                }
                /*
                 * the untouch should always come after the button release
                 */
                if ((priv->proximity == TRUE) && !(priv->packet[0] & 0x01))
                {
                        priv->proximity = FALSE;
                        xf86PostProximityEvent (pInfo->dev, 0, 0, 2, x, y);
                }
	}
}

static void
DMC9000_ReadInput (InputInfoPtr pInfo)
{
	PenMountPrivatePtr priv = (PenMountPrivatePtr) (pInfo->private);
	int x,y;
	unsigned char opck[ PENMOUNT_PACKET_SIZE ];

	/* 
	 * set blocking to -1 on the first call because we know there is data to
	 * read. Xisb automatically clears it after one successful read so that
	 * succeeding reads are preceeded buy a select with a 0 timeout to prevent
	 * read from blocking indefinately.
	 */
	XisbBlockDuration (priv->buffer, -1);
	while (1)
	{
		unsigned int tmp;
		memcpy(opck,priv->packet,5);
		if ( DMC9000_PenMountGetPacket (priv) != Success )
			break;
		if ( priv->packet[0] == 0xff )
		{
			priv->pen_down = 1;
		}
		if ( priv->packet[0] == 0xbf ) 
		{
			priv->pen_down = 0;
		}
		x = ((((unsigned int) (priv->packet[1]&0x07)) << 7)  | (priv->packet[2]&0x7F));
		y = ((((unsigned int) (priv->packet[3]&0x07)) << 7)  | (priv->packet[4]&0x7F));
		if ( priv->swap_xy)
		{
			tmp = y;
			y = x;
			x = tmp;
		}
		if (priv->invert_y)
		{
			y = priv->max_y - y + priv->min_y;
		}
		if (priv->invert_x)
		{
			x = priv->max_x - x + priv->min_x;
		}
		priv->packet[0] = priv->pen_down ? 0x01 : 0x00;

		if (priv->reporting_mode == TS_Scaled)
		{	
			x = xf86ScaleAxis (x, 0, priv->screen_width, priv->min_x,
					   priv->max_x);
                        y = xf86ScaleAxis (y, 0, priv->screen_height, priv->min_y,
					   priv->max_y);
                }
        		

		xf86XInputSetScreen (pInfo, priv->screen_num, x, y);

		if ((priv->proximity == FALSE) && (priv->packet[0] & 0x01))
		{
			priv->proximity = TRUE;
			xf86PostProximityEvent (pInfo->dev, 1, 0, 2, x, y);
		}              	
		
             /*
                 * Send events.
                 *
                 * We *must* generate a motion before a button change if pointer
                 * location has changed as DIX assumes this. This is why we always
                 * emit a motion, regardless of the kind of packet processed.
                 */

                xf86PostMotionEvent (pInfo->dev, TRUE, 0, 2, x, y);

                /*
                 * Emit a button press or release.
                 */
                if ((priv->button_down == FALSE) && (priv->packet[0] & 0x01))

                {
                        xf86PostButtonEvent (pInfo->dev, TRUE,
					     priv->button_number, 1, 0, 2, x, y);
                        priv->button_down = TRUE;
                }
                if ((priv->button_down == TRUE) && !(priv->packet[0] & 0x01))
                {
                        xf86PostButtonEvent (pInfo->dev, TRUE,
					     priv->button_number, 0, 0, 2, x, y);
                        priv->button_down = FALSE;
                }
                /*
                 * the untouch should always come after the button release
                 */
                if ((priv->proximity == TRUE) && !(priv->packet[0] & 0x01))
                {
                        priv->proximity = FALSE;
                        xf86PostProximityEvent (pInfo->dev, 0, 0, 2, x, y);
                }
	}
}


/* 
 * The ControlProc function may need to be tailored for your device
 */
static int
ControlProc (InputInfoPtr pInfo, xDeviceCtl * control)
{
	xDeviceAbsCalibCtl *c = (xDeviceAbsCalibCtl *) control;
	PenMountPrivatePtr priv = (PenMountPrivatePtr) (pInfo->private);

        priv->min_x = c->min_x;
        priv->max_x = c->max_x;
        priv->min_y = c->min_y;
        priv->max_y = c->max_y;
 
	return (Success);
}

/* 
 * The SwitchMode function may need to be tailored for your device
 */
static int
SwitchMode (ClientPtr client, DeviceIntPtr dev, int mode)
{
	InputInfoPtr pInfo = dev->public.devicePrivate;
        PenMountPrivatePtr priv = (PenMountPrivatePtr) (pInfo->private);


	if ((mode == TS_Raw) || (mode == TS_Scaled))
        {
                priv->reporting_mode = mode;
                return (Success);
        }
        else
                return (!Success);   
}

/* 
 * the QueryHardware fuction should be tailored to your device to
 * verify the device is attached and functional and perform any
 * needed initialization.
 */
static Bool
QueryHardware (PenMountPrivatePtr priv)
{
	/* Maybe once we get the hardware to actually respond correctly to its 
	   configuration 'packets' */

	return (Success);
}

/* 
 * This function should be renamed for your device and tailored to handle
 * your device's protocol.
 */
static Bool
PenMountGetPacket (PenMountPrivatePtr priv)
{
	int count = 0;
	int c;

	while ((c = XisbRead (priv->buffer)) >= 0)
	{
		/* 
		 * fail after 500 bytes so the server doesn't hang forever if a
		 * device sends bad data.
		 */
		if (count++ > 500)
			return (!Success);

		switch (priv->lex_mode)
		{
		case PenMount_byte0:
			if ( priv->chip != DMC8910 && priv->chip != DMC9512 )
			{
				if (!(c & 0x08) )
					return (!Success);
			}
			else
			{
				if ( c != 0xff )
					return (!Success);
			}
			priv->packet[0] = (unsigned char) c;
			priv->lex_mode = PenMount_byte1;
			break;

		case PenMount_byte1:
			priv->packet[1] = (unsigned char) c;
			priv->lex_mode = PenMount_byte2;
			break;
			
		case PenMount_byte2:
			priv->packet[2] = (unsigned char) c;
			priv->lex_mode = PenMount_byte0;
			if ( priv->chip != DMC8910 && priv->chip != DMC9512 )
				return (Success);
			if (( priv->packet[2] == 0xfe ) && ( priv->packet[1] == 0xfe ))
				return (Success);
			if (( priv->packet[2] == 0xfd ) && ( priv->packet[1] == 0xfd ))
				return (Success);
			priv->lex_mode = PenMount_byte3;
			break;

		case PenMount_byte3:
			priv->packet[3] = (unsigned char) c;
			priv->lex_mode = PenMount_byte4;
			break;

		case PenMount_byte4:
			priv->packet[4] = (unsigned char) c;
			priv->lex_mode = PenMount_byte0;
			return (Success);
			break;

		case PenMount_Response0:
			if ( c == 0xff )
				priv->lex_mode = PenMount_Response1;
			priv->packet[0] = (unsigned char) c;
			break;

		case PenMount_Response1:
			priv->packet[1] = (unsigned char) c;
			priv->lex_mode = PenMount_Response2;
			break;
		case PenMount_Response2:
			priv->packet[2] = (unsigned char) c;
			priv->lex_mode = PenMount_byte0;
			return (Success);
			break;
		}
	}
	return (!Success);
}

static Bool
DMC9000_PenMountGetPacket (PenMountPrivatePtr priv)
{
	int count = 0;
	int c;

	while ((c = XisbRead (priv->buffer)) >= 0)
	{
		/* 
		 * fail after 500 bytes so the server doesn't hang forever if a
		 * device sends bad data.
		 */
		if (count++ > 500)
			return (!Success);

		switch (priv->lex_mode)
		{
		case PenMount_byte0:
			if (( c != 0xff ) && ( c != 0xbf))
				return (!Success);
			priv->packet[0] = (unsigned char) c;
			priv->lex_mode = PenMount_byte1;
			break;

		case PenMount_byte1:
			priv->packet[1] = (unsigned char) c;
			priv->lex_mode = PenMount_byte2;
			break;
			
		case PenMount_byte2:
			priv->packet[2] = (unsigned char) c;
			priv->lex_mode = PenMount_byte3;
			break;

		case PenMount_byte3:
			priv->packet[3] = (unsigned char) c;
			priv->lex_mode = PenMount_byte4;
			break;

		case PenMount_byte4:
			priv->packet[4] = (unsigned char) c;
			priv->lex_mode = PenMount_byte0;
			return (Success);
			break;

		case PenMount_Response0:
			if ( c == 0xf2 )
				priv->lex_mode = PenMount_Response1;
			priv->packet[0] = (unsigned char) c;
			break;

		case PenMount_Response1:
			priv->packet[1] = (unsigned char) c;
			priv->lex_mode = PenMount_Response2;
			break;
		case PenMount_Response2:
			priv->packet[2] = (unsigned char) c;
			priv->lex_mode = PenMount_byte0;
			return (Success);
			break;
		}
	}
	return (!Success);
}


static Bool
PenMountSendPacket (PenMountPrivatePtr priv, unsigned char *buf, int len)
{
	int				count = 0;

	while ( len > 0 )
	{
		if ( XisbWrite(priv->buffer, buf, 1) == 1 )
		{
			buf++;
			len--;
			continue;
		}
		if ( count++ > 500 )
			break;
	}
	return (len ? !Success : Success);
}

static void
PenMountPtrCtrl(DeviceIntPtr device, PtrCtrl *ctrl)
{
  /* I have no clue what this does, except that registering it stops the 
     X server segfaulting in ProcGetPointerMapping()
     Ho Hum.
  */
}
