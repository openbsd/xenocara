/*
 * Copyright 2000-2002 by Alessandro Rubini <rubini@linux.it>
 *      somehow based on xf86Summa.c:
 * Copyright 1996 by Steven Lang <tiger@tyger.org>
 *
 * This work is sponsored by Gunze-USA.
 *
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the authors not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  The authors make no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL STEVEN LANG BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTIONS, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86Version.h"

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include "misc.h"
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Xinput.h"
#include "exevents.h"		/* Needed for InitValuator/Proximity stuff */
#include "X11/keysym.h"
#include "mipointer.h"

#ifdef XFree86LOADER
#include <xf86Module.h>
#endif

#undef memset
#define memset xf86memset
#undef sleep
#define sleep(t) xf86WaitForInput(-1, 1000 * (t))
#define wait_for_fd(fd) xf86WaitForInput((fd), 1000)
#define tcflush(fd, n) xf86FlushInput((fd))
#undef read
#define read(a,b,c) xf86ReadSerial((a),(b),(c))
#undef write
#define write(a,b,c) xf86WriteSerial((a),(char*)(b),(c))
#undef close
#define close(a) xf86CloseSerial((a))
#define XCONFIG_PROBED "(==)"
#define XCONFIG_GIVEN "(**)"
#define xf86Verbose 1
#undef PRIVATE
#define PRIVATE(x) XI_PRIVATE(x)

/*
 * Be sure to set vmin appropriately for your device's protocol. You want to
 * read a full packet before returning
 */
static const char *default_options[] = {
	"BaudRate",	"9600",
	"StopBits",	"1",
	"DataBits",	"8",
	"Parity",	"None",
	"Vmin",		"1",
	"Vtime",	"10",
	"FlowControl",	"None",
	NULL
};

static InputDriverPtr gnzDrv;

/* Debugging macros */
#ifdef DBG
#undef DBG
#endif
#ifdef DEBUG
#undef DEBUG
#endif

static int      debug_level = 0;
#define DEBUG	1
#if DEBUG
#define 	DBG(lvl, f) 	{if ((lvl) <= debug_level) f;}
#else
#define 	DBG(lvl, f)
#endif

/* Device records */
#define GUNZE_MAXPHYSCOORD	1023
#define GUNZE_MAXCOORD		(64 * 1024 - 1)	/* oversampled, synthetic
						 * value */
#define FLAG_PRE_TAPPING	1	/* this is motion, despite being
					 * pen-down */
#define FLAG_WAS_UP		2	/* last event was a pen-up event */
#define FLAG_RES12BIT		0x100

#define GUNZE_DEFAULT_TAPPING_DELAY	0	/* off */
#define GUNZE_DEFAULT_JITTER_DELAY	50	/* milliseconds */

/*
 * Prototype for a callback to handle delayed release event.
 * This is handy for integrating out the on/off jitter that
 * the Gunze device tends to produce during a user drag (Chris Howe)
 */
static CARD32   touchButtonTimer(OsTimerPtr timer, CARD32 now, pointer arg);

typedef struct {
	char           *gunDevice;	/* device file name */
	int             flags;		/* various flags */
	int             gunType;	/* TYPE_SERIAL, etc */
	int             gunBaud;	/* 9600 or 19200 */
	int             gunDlen;	/* data length (3 or 11) */
	int             gunAvgX;	/* previous X position */
	int             gunAvgY;	/* previous Y position */
	int             gunSmooth;	/* how smooth the motion is */
	int             gunPrevButton;	/* previous button state */
	int             gunBytes;	/* number of bytes read */
	unsigned char   gunData[16];	/* data read on the device */
	int             gunCalib[4];	/* Hmm.... */
	char           *gunConfig;	/* filename for configuration */
	int             gunJitterDelay;	/* time to delay before issuing a
					 * buttonup */
	int             gunTappingDelay;	/* longer hops are not
						 * considered ticks */
	long            gunUpSec;	/* when did pen-up happen */
	long            gunUpUsec;	/* when did pen-up happen */
	OsTimerPtr      timer;
} GunzeDeviceRec, *GunzeDevicePtr;

#define GUNZE_SERIAL_DLEN 11
#define GUNZE_PS2_DLEN     3

/* Configuration data */
#define GUNZE_SECTION_NAME    "GunzeTS"
#define GUNZE_DEFAULT_CFGFILE "/etc/gunzets.calib"

enum devicetypeitems {
	TYPE_UNKNOWN = 0,
	TYPE_SERIAL = 1,
	TYPE_PS2,
	TYPE_USB
};

/* Constants and macros */
#define BUFFER_SIZE	64		/* size of reception buffer */
#define XI_NAME 	"GUNZETS"	/* X device name for the touch screen */
#define MSGID           "xf86Gunze: "

#define SYSCALL(call) while(((call) == -1) && (errno == EINTR))

/*
 * xf86GunzeConvert
 * Convert valuators to X and Y. Since calibration data has already been used,
 * this only requires downscaling to screen size
 */
static Bool
xf86GunzeConvert(LocalDevicePtr local, int first, int num, int v0, int v1,
    int v2, int v3, int v4, int v5, int *x, int *y)
{
	if (first != 0 || num == 1)
		return FALSE;

	*x = v0 * screenInfo.screens[0]->width / (GUNZE_MAXCOORD);
	*y = v1 * screenInfo.screens[0]->height / (GUNZE_MAXCOORD);

	if (*x < 0)
		*x = 0;
	if (*y < 0)
		*y = 0;
	if (*x > screenInfo.screens[0]->width)
		*x = screenInfo.screens[0]->width;
	if (*y > screenInfo.screens[0]->height)
		*y = screenInfo.screens[0]->height;

	DBG(6, ErrorF("Adjusted coords x=%d y=%d\n", *x, *y));
	return TRUE;
}

/*
 * xf86GunzeReadInput
 * Reads from the touch screen, uses calibration data
 * and posts any new events to the server.
 */
static void
xf86GunzeReadInput(LocalDevicePtr local)
{
	GunzeDevicePtr priv = (GunzeDevicePtr)local->private;
	unsigned char *pkt = priv->gunData;
	int len, loop;
	int x, y, button;
	int *calib = priv->gunCalib;
	DeviceIntPtr device;
	unsigned char buffer[BUFFER_SIZE];
	long sec, usec;

	DBG(7, ErrorF("xf86GunzeReadInput BEGIN device=%s fd=%d (bytes %i)\n",
	    priv->gunDevice, local->fd, priv->gunBytes));

	SYSCALL(len = read(local->fd, buffer, sizeof(buffer)));

	if (len <= 0) {
		Error("error reading Gunze touch screen device");
		return;
	}
	/*
         * Serial protocol (11 bytes): <T|R><4-bytes-X><4-bytes-Y><0x0d>
         *
         * Ps/2 protocol (3 bytes):
         *                1  X9 X8 X7 X6 X5 X4 X3
         *                1  X2 X1 X0 Y9 Y8 Y7 Y6
         *                0  TR Y5 Y4 Y3 Y2 Y1 Y0  (TR == 1 touch, 0 release)
         */
	for (loop = 0; loop < len; loop++) {
		/* if first byte, ensure that the packet is syncronized */
		if (priv->gunBytes == 0) {
			int error = 0;
			if (priv->gunDlen == GUNZE_SERIAL_DLEN) {
				/* First byte is 'R' (0x52) or 'T' (0x54) */
				if ((buffer[loop] != 'R')
				    && (buffer[loop] != 'T'))
					error = 1;
			} else {/* PS/2 */
				if (!(buffer[loop] & 0x80)
				    || (len > loop + 1
				    && !(buffer[loop + 1] & 0x80))
				    || (len > loop + 2
				    && (buffer[loop + 2] & 0x80)))
					error = 1;
			}
			if (error) {
				DBG(6, ErrorF("xf86GunzeReadInput: bad first "
				    "byte 0x%x\n", buffer[loop]));;
				continue;
			}
		}
		pkt[priv->gunBytes++] = buffer[loop];

		/*
		 * Hack: sometimes a serial packet gets corrupted. If so,
		 * drop it
		 */
		if (buffer[loop] == 0x0d
		    && priv->gunBytes != priv->gunDlen
		    && priv->gunDlen == GUNZE_SERIAL_DLEN) {
			pkt[priv->gunBytes - 1] = '\0';
			DBG(2, ErrorF("Bad packet \"%s\", dropping it\n", pkt));
			priv->gunBytes = 0;	/* for next time */
			continue;
		}
		/* if whole packet collected, decode it */
		if (priv->gunBytes == priv->gunDlen) {
			priv->gunBytes = 0;	/* for next time */

			if (priv->gunDlen == GUNZE_SERIAL_DLEN) {
				button = (pkt[0] == 'T');
				x = atoi((char *) pkt + 1);
				y = atoi((char *) pkt + 6);
			} else {
				button = (pkt[2] & 0x40);
				x = ((pkt[0] & 0x7f) << 3)
				    | ((pkt[1] & 0x70) >> 4);
				y = ((pkt[1] & 0x0f) << 6)
				    | ((pkt[2] & 0x3f));
			}
			if (priv->flags & FLAG_RES12BIT) {
				/*
				 * support 12-bit by downgrading to 10 bits
				 * (Atsushi Nemoto)
				 */
				x /= 4;
				y /= 4;
			}
			if (x > 1023 || x < 0 || y > 1023 || y < 0) {
				DBG(2, ErrorF("Bad packet \"%s\" -> %i,%i\n",
				    pkt, x, y));
				priv->gunBytes = 0;	/* for next time */
				continue;
			}
			DBG(6, ErrorF("Raw:    %5i, %5i (%i)\n", x, y,
			    !!button));

			/*
		         * Ok, now that we have raw data, turn it to real data
		         * according to calibration, smoothness, tapping and
			 * debouncing
		         */
			/* calibrate and rescale (by multiplying by 64) */
			x = 64 * 128 + 64 * 768
			    * (x - calib[0]) / (calib[2] - calib[0]);
			y = 64 * 128 + 64 * 768
			    * (y - calib[1]) / (calib[3] - calib[1]);
			y = GUNZE_MAXCOORD - y;

			/* smooth it down, unless first touch */
			if (!(priv->flags & FLAG_WAS_UP)) {
				x = (priv->gunAvgX * priv->gunSmooth + x)
				    / (priv->gunSmooth + 1);
				y = (priv->gunAvgY * priv->gunSmooth + y)
				    / (priv->gunSmooth + 1);
			}
			/* FIXME: this isn't coordinated with debouncing */
			if (!button)
				priv->flags |= FLAG_WAS_UP;
			else
				priv->flags &= ~FLAG_WAS_UP;

			DBG(6, ErrorF("Cooked: %5i, %5i (%i)\n", x, y,
			    !!button));

			/* Now send events */
			device = local->dev;
			if ((priv->gunAvgX != x) || (priv->gunAvgY != y)) {
				xf86PostMotionEvent(device, 1 /* absolute */,
				    0, 2, x, y);
				DBG(1, ErrorF("Post motion\n"));
			}
			/*
		         * If we were in an up state and now we are in a down
			 * state consider sending a button down event.
		         */
			if (!priv->gunPrevButton && button) {
				if (priv->timer) {
					/*
				         * We have detected a bounce.
					 * Cancel the timer and do not send
					 * a button event.
				         */
					TimerFree(priv->timer);
					priv->timer = NULL;
				} else {
					int deltams = 0;
					/*
					 * No timer: real touchdown. Is this
					 * pre-tap?
					 */
					xf86getsecs(&sec, &usec);
					deltams = (sec - priv->gunUpSec) * 1000
					    + (usec - priv->gunUpUsec)
					    / 1000;
					if (!priv->gunTappingDelay
					    || (deltams <
					    priv->gunTappingDelay)) {
						xf86PostButtonEvent(device,
						    1 /* absolute */,
						    1 /* button */,
						    1 /* isdown */, 0, 2, x, y);
						DBG(1, ErrorF("Post button\n"));
					} else {
						priv->flags |= FLAG_PRE_TAPPING;
						DBG(1, ErrorF("Pre-tapping\n"));
					}
				}
			} else if (priv->gunPrevButton && !button) {
				/*
				 * The user has let go of the touchpad, but we
				 * dont know if this is a bounce or a real
				 * button up event. Set up a timer to fire off
				 * the button up event unless something
				 * else happens (Chris Howe)
				 *
				 * Unfortunately, this doesn't seem to work
				 * with 3.3.6, so treat the "time is 0" case
				 * specially (ARub)
				 */
				/* tap info */
				xf86getsecs(&priv->gunUpSec, &priv->gunUpUsec);	
				if (priv->gunJitterDelay) {
					priv->timer = TimerSet(priv->timer, 0,
					    priv->gunJitterDelay /* delay */,
					    touchButtonTimer, local);
				} else {
					if (!(priv->flags & FLAG_PRE_TAPPING)) {
						xf86PostButtonEvent(device,
						    1 /* absolute */,
						    1 /* button */,
						    !!button /* isdown */,
						    0, 2, x, y);
						DBG(1, ErrorF("Post button\n"));
					}
					priv->flags &= ~FLAG_PRE_TAPPING;
				}
			}
			/* remember data */
			priv->gunPrevButton = button;
			priv->gunAvgX = x;
			priv->gunAvgY = y;
		}		/* if end-of-packet */
	}
	DBG(7, ErrorF("xf86GunzeReadInput END   device=0x%x priv=0x%x\n",
	    local->dev, priv));
}

/* send a button event after a delay */
static CARD32
touchButtonTimer(OsTimerPtr timer, CARD32 now, pointer arg)
{
	LocalDevicePtr local = (LocalDevicePtr)arg;
	GunzeDevicePtr priv = (GunzeDevicePtr)(local->private);

	xf86PostButtonEvent(local->dev, 1 /* absolute */, 1 /* button */,
	    !!priv->gunPrevButton /* isdown */, 0, 2, priv->gunAvgX,
	    priv->gunAvgY);
	priv->flags &= ~FLAG_PRE_TAPPING;
	DBG(1, ErrorF("Post delayed button-up\n"));
	priv->timer = NULL;

	return 0;
}

/*
 * xf86GunzeControlProc
 * This is called for each device control that is defined at init time.
 * It currently isn't use, but I plan to make tapping, smoothness and
 * on/off available as integer device controls.
 */
static void
xf86GunzeControlProc(DeviceIntPtr device, PtrCtrl * ctrl)
{
	DBG(2, ErrorF("xf86GunzeControlProc\n"));
}

/* Read the configuration file or revert to default (identity) cfg */
static int
xf86GunzeReadCalib(GunzeDevicePtr priv)
{
	int i, err = 1;
	FILE *f;

	f = fopen(priv->gunConfig, "r");
	if (f) {
		char s[80];
		fgets(s, sizeof s, f);	/* discard the comment */
		if (fscanf(f, "%d %d %d %d", priv->gunCalib, priv->gunCalib + 1,
		    priv->gunCalib + 2, priv->gunCalib + 3) == 4)
			err = 0;
		/* Hmm... check */
		for (i = 0; i < 4; i++)
			if (priv->gunCalib[i] & ~1023)
				err++;
		if (abs(priv->gunCalib[0] - priv->gunCalib[2]) < 100)
			err++;
		if (abs(priv->gunCalib[1] - priv->gunCalib[3]) < 100)
			err++;
		fclose(f);
	}
	if (err) {
		ErrorF(MSGID "Calibration data absent or invalid, using "
		    "defaults\n");
		priv->gunCalib[0] = priv->gunCalib[1] = 128;	/* 1/8 */
		priv->gunCalib[2] = priv->gunCalib[3] = 896;	/* 7/8 */
	}
	return 0;
}

/*
 * xf86GunzeOpen
 * Open and initialize the tablet, as well as probe for any needed data.
 */
#define WAIT(t)                                                 \
    err = xf86WaitForInput(-1, ((t) * 1000));                   \
    if (err == -1) {                                            \
       ErrorF("Wacom select error : %s\n", strerror(errno));   \
       return !Success;                                        \
    }

static Bool
xf86GunzeOpen(LocalDevicePtr local)
{
	GunzeDevicePtr priv = (GunzeDevicePtr)local->private;

	DBG(1, ErrorF("opening %s (calibration file is \"%s\"\n",
	    priv->gunDevice, priv->gunConfig));

	xf86GunzeReadCalib(priv);

	/* Is it a serial port or something else? */
	if (priv->gunType == TYPE_UNKNOWN) {
		if (strstr(priv->gunDevice, "tty"))
			priv->gunType = TYPE_SERIAL;
		else
			priv->gunType = TYPE_PS2;
	}
	switch (priv->gunType) {
	case TYPE_SERIAL:
		priv->gunDlen = GUNZE_SERIAL_DLEN;
		break;
	case TYPE_PS2:
	case TYPE_USB:
		priv->gunDlen = GUNZE_PS2_DLEN;
		break;
	}

	if (priv->gunType == TYPE_SERIAL)
		local->fd = xf86OpenSerial(local->options);
	else
		SYSCALL(local->fd = open(priv->gunDevice, O_RDWR | O_NDELAY,
		    0));

	if (local->fd == -1) {
		Error(priv->gunDevice);
		return !Success;
	}
	DBG(2, ErrorF("%s opened as fd %d\n", priv->gunDevice, local->fd));
	DBG(1, ErrorF("initializing Gunze touch screen\n"));

	/* Hmm... close it, so it doens't say anything before we're ready */
	/* FIXME */

	/* Clear any pending input */
	tcflush(local->fd, TCIFLUSH);

	if (xf86Verbose)
		ErrorF("%s Gunze touch screen\n", XCONFIG_PROBED);

	/* FIXME: is there something to write-and-read here? */

	return Success;
}

/*
 * xf86GunzeOpenDevice
 * Opens and initializes the device driver stuff
 */
static int
xf86GunzeOpenDevice(DeviceIntPtr ptr)
{
	LocalDevicePtr local = (LocalDevicePtr)ptr->public.devicePrivate;

	if (xf86GunzeOpen(local) != Success) {
		if (local->fd >= 0)
			SYSCALL(close(local->fd));
		local->fd = -1;
	}
	/* Initialize the axes */
	InitValuatorAxisStruct(ptr, 0, /* X */
	    0, GUNZE_MAXCOORD,	/* min, max val */
	    500000,		/* resolution */
	    0, 500000);		/* min, max_res */
	InitValuatorAxisStruct(ptr, 1,	/* Y */
  	    0, GUNZE_MAXCOORD,	/* min, max val */
	    500000,		/* resolution */
	    0, 500000);		/* min, max_res */
	return (local->fd != -1);
}

/*
 * xf86GunzeProc
 * Handle requests to do stuff to the driver.
 * In order to allow for calibration, the function is called (with DEVICE_OFF
 * and DEVICE_ON commands) also by xf86GunzeChangeControl.
 */
static int
xf86GunzeProc(DeviceIntPtr ptr, int what)
{
	CARD8 map[25];	/* 25? */
	int nbaxes;
	int nbbuttons;
	int loop;
	LocalDevicePtr local = (LocalDevicePtr)ptr->public.devicePrivate;
	GunzeDevicePtr priv = (GunzeDevicePtr)PRIVATE(ptr);

	DBG(2, ErrorF("BEGIN xf86GunzeProc dev=0x%x priv=0x%x what=%d\n", ptr,
	    priv, what));

	switch (what) {
	case DEVICE_INIT:
		DBG(1, ErrorF("xf86GunzeProc ptr=0x%x what=INIT\n", ptr));

		nbaxes = 2;	/* X, Y */
		nbbuttons = 1;	/* one: it's a touch screen */

		for (loop = 1; loop <= nbbuttons; loop++)
			map[loop] = loop;

		if (InitButtonClassDeviceStruct(ptr, nbbuttons, map) == FALSE) {
			ErrorF("unable to allocate Button class device\n");
			return !Success;
		}
		if (InitFocusClassDeviceStruct(ptr) == FALSE) {
			ErrorF("unable to init Focus class device\n");
			return !Success;
		}
		/*
	         * Use default (?) feedback stuff.
	         * I'll use device feedback controls to change parameters at
	         * run time.
	         */
		if (InitPtrFeedbackClassDeviceStruct(ptr, xf86GunzeControlProc)
		    == FALSE) {
			ErrorF("unable to init ptr feedback\n");
			return !Success;
		}
		if (InitProximityClassDeviceStruct(ptr) == FALSE) {
			ErrorF("unable to init proximity class device\n");
			return !Success;
		}
		if (InitValuatorClassDeviceStruct(ptr, nbaxes,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 3
		    xf86GetMotionEvents, 
#endif
		    local->history_size, Absolute)
		    == FALSE) {
			ErrorF("unable to allocate Valuator class device\n");
			return !Success;
		}
		/* allocate the motion history buffer if needed */
		xf86MotionHistoryAllocate(local);

		/* open the device to gather informations */
		xf86GunzeOpenDevice(ptr);
		break;
	case DEVICE_ON:
		DBG(1, ErrorF("xf86GunzeProc ptr=0x%x what=ON\n", ptr));

		if ((local->fd < 0) && (!xf86GunzeOpenDevice(ptr)))
			return !Success;
		tcflush(local->fd, TCIFLUSH);	/* flush pending input */
		AddEnabledDevice(local->fd);
		ptr->public.on = TRUE;
		break;
	case DEVICE_OFF:
		DBG(1, ErrorF("xf86GunzeProc  ptr=0x%x what=OFF\n", ptr));
		if (local->fd >= 0)
			RemoveEnabledDevice(local->fd);
		ptr->public.on = FALSE;
		break;
	case DEVICE_CLOSE:
		DBG(1, ErrorF("xf86GunzeProc  ptr=0x%x what=CLOSE\n", ptr));
		SYSCALL(close(local->fd));
		local->fd = -1;
		break;
	default:
		ErrorF("unsupported mode=%d\n", what);
		return !Success;
		break;
	}
	DBG(2, ErrorF("END   xf86GunzeProc Success what=%d dev=0x%x "
	    "priv=0x%x\n", what, ptr, priv));
	return Success;
}

/*
 * xf86GunzeClose
 * It...  Uh...  Closes the physical device?
 */
static void
xf86GunzeClose(LocalDevicePtr local)
{
	if (local->fd >= 0)
		SYSCALL(close(local->fd));
	local->fd = -1;
}

/*
 * xf86GunzeChangeControl
 * The only supported control is the change of resolution. I currently
 * use it to disable and re-enable the device (used to allow the
 * calibrator to read raw data).
 */
static int
xf86GunzeChangeControl(LocalDevicePtr local, xDeviceCtl * control)
{
	xDeviceResolutionCtl *res;
	GunzeDevicePtr priv = (GunzeDevicePtr)local->private;

	res = (xDeviceResolutionCtl *)control;

	DBG(1, ErrorF(MSGID "change ctrl (valuators: %i)\n",
	    res->num_valuators));
	if ((control->control != DEVICE_RESOLUTION) ||
	    (res->num_valuators < 1))
		return BadMatch;

	/* Hmm... very dirty: use num_valuators: 1 to disable, 2 to enable */
	if (res->num_valuators < 2) {
		/* xf86GunzeProc( (DeviceIntPtr)local, DEVICE_OFF); */
		DBG(1, ErrorF(MSGID "Device OFF\n", local));
		if (local->fd >= 0)
			RemoveEnabledDevice(local->fd);
		/* ptr->public.on = FALSE; */
	} else {
		/* xf86GunzeProc( (DeviceIntPtr)local, DEVICE_ON); */
		DBG(1, ErrorF(MSGID "Device ON\n", local));
		if (local->fd < 0)
			return !Success;
		tcflush(local->fd, TCIFLUSH);	/* flush pending input */
		AddEnabledDevice(local->fd);
		xf86GunzeReadCalib(priv);
		/* ptr->public.on = TRUE; */
	}
	return Success;
}

/*
 * xf86GunzeSwitchMode
 * Switches the mode. Only absolute is allowed.
 */
static int
xf86GunzeSwitchMode(ClientPtr client, DeviceIntPtr dev, int mode)
{
	DBG(3, ErrorF("xf86GunzeSwitchMode dev=0x%x mode=%d\n", dev, mode));

	switch (mode) {
	case Absolute:
		break;
	default:
		DBG(1, ErrorF("xf86GunzeSwitchMode dev=0x%x invalid mode=%d\n",
		    dev, mode));
		return BadMatch;
	}
	return Success;
}

/*
 * xf86GunzeAllocate
 * Allocates the device structures for the Gunze Touch Screen.
 */
static LocalDevicePtr
xf86GunzeAllocate()
{
	LocalDevicePtr local = xf86AllocateInput(gnzDrv, 0);
	GunzeDevicePtr priv = (GunzeDevicePtr)xalloc(sizeof(GunzeDeviceRec));

	local->name = XI_NAME;
	local->type_name = "Gunze Touch Screen";
	local->flags = 0;	/* XI86_NO_OPEN_ON_INIT; */
	local->device_control = xf86GunzeProc;
	local->read_input = xf86GunzeReadInput;
	local->control_proc = xf86GunzeChangeControl;
	local->close_proc = xf86GunzeClose;
	local->switch_mode = xf86GunzeSwitchMode;
	local->conversion_proc = xf86GunzeConvert;
	/*
	 * reverse_conversion is only used by relative devices (for warp
	 * events)
	 */
	local->reverse_conversion_proc = NULL;
	local->fd = -1;
	local->atom = 0;
	local->dev = NULL;
	local->private = priv;
	local->private_flags = 0;
	local->history_size = 0;

	priv->gunDevice = "";	/* device file name */
	priv->gunConfig = GUNZE_DEFAULT_CFGFILE;
	priv->gunDlen = 0;	/* unknown */
	priv->gunType = TYPE_UNKNOWN;
	priv->gunBaud = 9600;
	priv->gunSmooth = 9;	/* quite smooth */
	priv->gunAvgX = -1;	/* previous (avg) X position */
	priv->gunAvgY = -1;	/* previous (avg) Y position */
	priv->gunPrevButton = 0;/* previous buttons state */
	priv->flags = FLAG_WAS_UP;	/* first event is button-down */
	priv->gunBytes = 0;	/* number of bytes read */
	priv->gunTappingDelay = GUNZE_DEFAULT_TAPPING_DELAY;
	xf86getsecs(&priv->gunUpSec, &priv->gunUpUsec);
	priv->timer = NULL;

	return local;
}

/*
 * GunzeTS device association
 * Device section name and allocation function.
 */
DeviceAssocRec gunzets_assoc =
{
	GUNZE_SECTION_NAME,	/* config_section_name */
	xf86GunzeAllocate	/* device_allocate */
};

/*
 * xf86GnzUninit --
 *
 * called when the driver is unloaded.
 */
static void
xf86GnzUninit(InputDriverPtr drv, LocalDevicePtr local, int flags)
{
	GunzeDevicePtr priv = (GunzeDevicePtr)local->private;

	DBG(1, ErrorF("xf86GnzUninit\n"));

	xf86GunzeProc(local->dev, DEVICE_OFF);

	xfree(priv);
	xf86DeleteInput(local, 0);
}

/*
 * xf86GnzInit --
 *
 * called when the module subsection is found in XF86Config
 */
static InputInfoPtr
xf86GnzInit(InputDriverPtr drv, IDevPtr dev, int flags)
{
	LocalDevicePtr local = NULL;
	LocalDevicePtr fakeLocal = NULL;
	GunzeDevicePtr priv = NULL;

	gnzDrv = drv;		/* used by xf86GunzeAllocate() */

	fakeLocal = (LocalDevicePtr)xcalloc(1, sizeof(LocalDeviceRec));
	if (!fakeLocal)
		return NULL;

	fakeLocal->conf_idev = dev;

	/*
	 * Force default serial port options to exist because the serial init
	 * phasis is based on those values.
	 */
	xf86CollectInputOptions(fakeLocal, default_options, NULL);

	local = xf86GunzeAllocate();
	if (!local) {
		xf86Msg(X_ERROR, "%s: Can't allocate touchscreen data\n",
			dev->identifier);
		goto SetupProc_fail;
	}
	priv = (GunzeDevicePtr) (local->private);
	local->options = fakeLocal->options;
	local->conf_idev = dev;
	local->name = dev->identifier;
	xf86OptionListReport(local->options);
	xfree(fakeLocal);
	fakeLocal = NULL;

	/* Device name is mandatory */
	priv->gunDevice = xf86FindOptionValue(local->options, "Device");
	if (!priv->gunDevice) {
		xf86Msg(X_ERROR, "%s: `Device' not specified\n",
		    dev->identifier);
		goto SetupProc_fail;
	}
	priv->gunType = TYPE_UNKNOWN;
	xf86ProcessCommonOptions(local, local->options);
	priv->gunSmooth = xf86SetIntOption(local->options, "Smoothness", 9);
	priv->gunJitterDelay = xf86SetIntOption(local->options, "JitterDelay",
	    GUNZE_DEFAULT_JITTER_DELAY);
	priv->gunTappingDelay = xf86SetIntOption(local->options, "TappingDelay",
	    GUNZE_DEFAULT_TAPPING_DELAY);
	debug_level = xf86SetIntOption(local->options, "DebugLevel", 0);
	if (xf86SetBoolOption(local->options, "Res12bit", 0))
		priv->flags |= FLAG_RES12BIT;

	/* FIXME: device type */
	/* FIXME: configuration file */

	/* mark the device configured */
	local->flags |= XI86_POINTER_CAPABLE | XI86_CONFIGURED;

	return local;

SetupProc_fail:
	if (priv)
		xfree(priv);
	if (local)
		xfree(local);
	return NULL;
}

#ifdef XFree86LOADER
static
#endif
InputDriverRec  GUNZE = {
	1,			/* driver version */
	"gunze",		/* driver name */
	NULL,			/* identify */
	xf86GnzInit,		/* pre-init */
	xf86GnzUninit,		/* un-init */
	NULL,			/* module */
	0			/* ref count */
};

/*
 * Dynamic loading functions
 */
#ifdef XFree86LOADER
/*
 * xf86GnzUnplug --
 *
 * called when the module subsection is found in XF86Config
 */
static void
xf86GnzUnplug(pointer p)
{
	DBG(1, ErrorF("xf86GnzUnplug\n"));
}

/*
 * xf86GnzPlug --
 *
 * called when the module subsection is found in XF86Config
 */
static pointer
xf86GnzPlug(pointer module, pointer options, int *errmaj, int *errmin)
{
	DBG(1, ErrorF("xf86GnzPlug\n"));
	xf86AddInputDriver(&GUNZE, module, 0);
	return module;
}

static XF86ModuleVersionInfo xf86GnzVersionRec =
{
	"gunze",
	"Alessandro Rubini and Gunze USA",
	MODINFOSTRING1,
	MODINFOSTRING2,
	XF86_VERSION_CURRENT,
	1, 4, 0,		/* VERSION */
	ABI_CLASS_XINPUT,
	ABI_XINPUT_VERSION,
	MOD_CLASS_XINPUT,
	{0, 0, 0, 0}		/* signature, to be patched into the file by */
	/* a tool */
};

XF86ModuleData  gunzeModuleData = {&xf86GnzVersionRec, xf86GnzPlug,
xf86GnzUnplug};

#endif				/* XFree86LOADER */
