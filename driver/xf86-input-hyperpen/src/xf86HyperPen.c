/*
 * xf86HyperPen
 *
 * Based on the xf86Summa driver.
 * 
 * Modified for the Aiptek HyperPen 6000 / Tevion MD 9310
 *   (c) 2000 Roland Jansen <roland@lut.rwth-aachen.de>
 * 
 * 
 * 
 * added button and 19200 bps stuff from the DigitalEdge driver on sourceforge
 *   (c) 2000 Christian Herzog <daduke@dataway.ch>
 * 
 * 
 */

/* xf86Summa
 * Copyright 1996 by Steven Lang <tiger@tyger.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Steven Lang not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  Steven Lang makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * STEVEN LANG DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL STEVEN LANG BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTIONS, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
/* $XFree86: xc/programs/Xserver/hw/xfree86/input/hyperpen/xf86HyperPen.c,v 1.9 2003/09/24 03:16:59 dawes Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorgVersion.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <misc.h>
#include <xf86.h>
#define NEED_XF86_TYPES
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

static const char *default_options[] =
{
	"BaudRate",	"9600",
	"DataBits",	"8",
	"StopBits",	"1",
	"Parity",	"Odd",
	"FlowControl",	"Xoff",
	"VTime",	"10",
	"VMin",		"7",
	NULL
};

static InputDriverPtr hypDrv;



/*
** Debugging macros
*/
#ifdef DBG
#undef DBG
#endif
#ifdef DEBUG
#undef DEBUG
#endif

#ifndef INI_DEBUG_LEVEL
#define INI_DEBUG_LEVEL 0
#endif

static int debug_level = INI_DEBUG_LEVEL;
#define DEBUG 1
#if DEBUG
#define 	DBG(lvl, f) 	{if ((lvl) <= debug_level) f;}
#else
#define 	DBG(lvl, f)
#endif

/*
** Device records
*/
#define ABSOLUTE_FLAG		1
#define STYLUS_FLAG		2
#define INVX_FLAG		4
#define INVY_FLAG		8
#define BAUD_19200_FLAG		16	

int stylus;

typedef struct 
{
	char	*hypDevice;	/* device file name */
	int	hypButTrans;	/* button translation flags */
	int	hypOldX;	/* previous X position */
	int	hypOldY;	/* previous Y position */
	int	hypOldZ;	/* previous Z position */
	int	hypOldProximity; /* previous proximity */
	int 	hypOldPush;           /* previous buttons state */
	int 	hypOldButtons;           /* previous buttons state */
	int 	hypOldBarrel;         /* previous buttons state */
	int 	hypOldBarrel1;                /* previous buttons state */
	int 	hypOldPressure;               /* previous pen pressure */
	int	hypMaxX;	/* max X value */
	int	hypMaxY;	/* max Y value */
	int	hypMaxZ;	/* max Z value */
	int	hypXSize;	/* active area X size */
	int	hypXOffset;	/* active area X offset */
	int	hypYSize;	/* active area Y size */
	int	hypYOffset;	/* active area Y offset */
	int	hypRes;		/* resolution in lines per inch */
	int	flags;		/* various flags */
	int	hypIndex;	/* number of bytes read */
	int	modelid;	/* model id */
	int 	PT;             /* pressure threshold */
        int     AutoPT;         /* automatically set PT*/
        int     PMax;           /* maximum pressure read from tablet */
	unsigned char hypData[7];	/* data read on the device */
} HyperPenDeviceRec, *HyperPenDevicePtr;

/*
** List of model IDs
*/
static struct MODEL_ID {
	unsigned id;
	char* name;
} models[] = {
	{0x32, "HyperPen 3000"},
	{0x43, "HyperPen 4000"},
	{0x54, "HyperPen 5000"},
	{0x64, "HyperPen 6000"},
	{0, NULL}
};

/*
** Configuration data
*/
#define HYPERPEN_SECTION_NAME "HyperPen"


/*
** Contants and macro
*/
#define BUFFER_SIZE	256		/* size of reception buffer */
#define XI_NAME 	"HYPERPEN"	/* X device name for the stylus */

#define SYSCALL(call) while(((call) == -1) && (errno == EINTR))


#define SS_RESET	"\0"	/* Reset */
#define SS_GETID	"\5"	/* Get Model ID */
#define SS_CONFIG	"a"	/* Send configuration (max coords) */
#define SS_PROMPT_MODE	"D"	/* Prompt mode */
#define SS_STREAM_MODE	"@"	/* Stream mode */
#define SS_RATE         "\xb5"  /* 19200 bps */
#define SS_ABSOLUTE     "F"     /* absolute mode */
#define SS_RELATIVE     "E"     /* relative mode */
#define SS_MACROKEY	"U"	/* enable F-keys */
#define SS_MACRO_4K	"\2"	/* start F-keys for Hyperpen 4000 */
#define SS_MACRO_56K	"\1"	/* start F-keys for Hyperpen 5000/6000 */
static const char * ss_initstr = SS_STREAM_MODE; 

#define PHASING_BIT	0x80
#define PROXIMITY_BIT	0x40
#define XSIGN_BIT	0x10
#define YSIGN_BIT	0x08
#define BUTTON_BITS	0x07
#define COORD_BITS	0x7f
#define XAXIS_BITS	0x03
#define YAXIS_BITS	0x0C
#define ZMASK_BIT       0x70
#define TIP_BITS        0x01
#define F_BIT           0x20


/* macro from counts/inch to counts/meter */
#define LPI2CPM(res)	(res * 1000 / 25.4)

/*
** External declarations
*/


/*
** xf86HypConvert
** Convert valuators to X and Y.
*/
static Bool
xf86HypConvert(LocalDevicePtr	local,
	       int		first,
	       int		num,
	       int		v0,
	       int		v1,
	       int		v2,
	       int		v3,
	       int		v4,
	       int		v5,
	       int*		x,
	       int*		y)
{
    HyperPenDevicePtr	priv = (HyperPenDevicePtr) local->private;

    if (first != 0 || num == 1)
      return FALSE;
    *x = (v0 * screenInfo.screens[0]->width) / priv->hypXSize;
    *y = (v1 * screenInfo.screens[0]->height) / priv->hypYSize;
    if (priv->flags & INVX_FLAG)
    	*x = screenInfo.screens[0]->width - *x;
    if (*x < 0)
	*x = 0;
    if (priv->flags & INVY_FLAG)
    	*y = screenInfo.screens[0]->height - *y;
    if (*y < 0)
	*y = 0;
    if (*x > screenInfo.screens[0]->width)
	*x = screenInfo.screens[0]->width;
    if (*y > screenInfo.screens[0]->height)
	*y = screenInfo.screens[0]->height;



    return TRUE;
}

/*
** xf86HypReverseConvert
** Convert X and Y to valuators.
*/
static Bool
xf86HypReverseConvert(LocalDevicePtr	local,
		      int		x,
		      int		y,
		      int		*valuators)
{
    HyperPenDevicePtr	priv = (HyperPenDevicePtr) local->private;
    valuators[0] = ((x * priv->hypXSize) / screenInfo.screens[0]->width);
    valuators[1] = ((y * priv->hypYSize) / screenInfo.screens[0]->height);



    return TRUE;
}

/*
** xf86HypReadInput
** Reads from the HyperPen and posts any new events to the server.
*/
static void
xf86HypReadInput(LocalDevicePtr local)
{
    HyperPenDevicePtr	priv = (HyperPenDevicePtr) local->private;
    int			len, loop;
    int			is_absolute;
    int f_keys, f_key, tip;
    int x, y, bx, by, barrel, barrel1, prox, pressure, button, push;
    int hw_pressure;

    DeviceIntPtr	device;
    unsigned char	buffer[BUFFER_SIZE];
  



    SYSCALL(len = read(local->fd, buffer, sizeof(buffer)));

    if (len <= 0) {
	Error("error reading HyperPen device");
	return;
    } else {

    }

    for(loop=0; loop<len; loop++) {

	if ((priv->hypIndex == 0) && !(buffer[loop] & PHASING_BIT)) { /* magic bit is not OK */
	    DBG(6, ErrorF("xf86HypReadInput bad magic number 0x%x\n", buffer[loop]));;
	    continue;
	}

	priv->hypData[priv->hypIndex++] = buffer[loop];

	if (priv->hypIndex == (priv->flags & ABSOLUTE_FLAG? 7 : 5)) {
/* the packet is OK */
/* reset char count for next read */
	    priv->hypIndex = 0;

	    prox = (priv->hypData[0] & PROXIMITY_BIT)? 0: 1;	
	    tip = (priv->hypData[0] & TIP_BITS)? 1:0;		
	    button = (priv->hypData[0] & BUTTON_BITS);		

	    f_keys = (priv->hypData[0] & F_BIT);		
	    pressure = (int) priv->hypData[6] + (((int) priv->hypData[5] & ZMASK_BIT) << 3);	

	    if ((tip==0) && (button==0) && (pressure>2) && (pressure != 1022)) {priv->flags |= STYLUS_FLAG; stylus=1;} else
	    if ((tip==0) && (button==0) && (pressure==0)) {priv->flags &= ~STYLUS_FLAG; stylus=0; pressure = 1019;}	

is_absolute = stylus;	

	x = priv->hypData[1] + (priv->hypData[2] << 7) + ((priv->hypData[5] & XAXIS_BITS) << 14); 
	y = priv->hypData[3] + (priv->hypData[4] << 7) + ((priv->hypData[5] & YAXIS_BITS) << 12);

	    if ((f_keys) && (tip)) (f_key = ((x >> 7) + 1) >> 1); else f_key =0; 

	    x -= priv->hypXOffset;
	    y = priv->hypYSize - y + priv->hypYOffset;
	    if (x < 0) x = 0;
	    if (y < 0) y = 0;
	    if (x > priv->hypXSize) x = priv->hypXSize;
	    if (y > priv->hypYSize) y = priv->hypYSize;	
	    bx=x;	
	    by=y;

	    if (!is_absolute) {		
		x -= priv->hypOldX;
		y -= priv->hypOldY;
	    } 




	
	
	hw_pressure=pressure; 
	

	if (!priv->PMax) priv->PMax=1000; 
      	if (pressure>1020) pressure=priv->PT;	
	if (priv->AutoPT)
	  {
	   if ((pressure>1) && !(tip)) priv->PT = pressure;
	   pressure = 511 * (pressure - priv->PT - 10) / (priv->PMax - priv->PT);
	  }
	else
	  {
	    pressure = 511 * (pressure - priv->PT) / (priv->PMax - priv->PT);
	    button &= ~1;
	    if (pressure>0) button |=1;

	  };


	if (pressure > 511) pressure = 511;
	if (pressure < 0) pressure = 0;

	

	push = button & 1;	
	barrel = button & 2;
	barrel1 = button & 4;
	
	DBG(6, ErrorF("hw-press=%d\ttip=%d\tbarrel=%d\tbarrel1=%d\tpush=%d\tpressure=%d\tPT=%d\tbuttons=%d\tf-key=%d\n",hw_pressure, priv->hypData[0] & TIP_BITS, barrel, barrel1, push, pressure, priv->PT, priv->hypData[0] & BUTTON_BITS,f_key));
	    
	    device = local->dev;

/* coordonates are ready we can send events */

	    if ((prox) && !(f_keys)) {
		if (!(priv->hypOldProximity)) 
		        xf86PostProximityEvent(device, 1, 0, 3, x, y, pressure);

		if ((is_absolute && ((priv->hypOldX != x) || 
                                     (priv->hypOldY != y) ||
                                     (priv->hypOldZ != pressure)))
		       || (!is_absolute && (x || y))) {
		    if (is_absolute || priv->hypOldProximity) {
			xf86PostMotionEvent(device, is_absolute, 0, 3, x, y, pressure);
		    }
		}



	if (priv->hypOldBarrel1 != barrel1) {
		int delta;
		delta = barrel1 - priv->hypOldBarrel1;
		if (priv->hypOldBarrel1 != barrel1) {



			xf86PostButtonEvent(device, is_absolute, 2, (delta > 0), 0, 3, x, y, pressure);
		}
	}

	if (priv->hypOldBarrel != barrel) {
		int delta;
		delta = barrel - priv->hypOldBarrel;
		if (priv->hypOldBarrel != barrel) {

			xf86PostButtonEvent(device, is_absolute, 3, (delta > 0), 0, 3, x, y, pressure);
		}
	}

	if ((priv->hypOldPush != push) && !barrel && !barrel1) {
		int delta;
		delta = push - priv->hypOldPush;
		if (priv->hypOldPush != push) {

			xf86PostButtonEvent(device, is_absolute, 1, (delta > 0), 0, 3, x, y, pressure);
		}
	}

		
	    	priv->hypOldX = bx;
	    	priv->hypOldY = by;
		priv->hypOldPush = push;
		priv->hypOldBarrel = barrel;
		priv->hypOldBarrel1 = barrel1;
		priv->hypOldProximity = prox;


	    } else { /* !PROXIMITY */
/* Any changes in buttons are ignored when !proximity */
                if (priv->hypOldProximity)
                    xf86PostProximityEvent(device, 0, 0, 3, x, y, pressure);
		priv->hypOldProximity = 0;
	    }
	}
    }


}

/*
** xf86HypControlProc
** It really does do something.  Honest!
*/
static void
xf86HypControlProc(DeviceIntPtr	device, PtrCtrl *ctrl)
{
    DBG(2, ErrorF("xf86HypControlProc\n"));
}

/*
** xf86HypWriteAndRead
** Write data, and get the response.
*/
static char *
xf86HypWriteAndRead(int fd, char *data, char *buffer, int len, int cr_term)
{
    int err, numread = 0;

    SYSCALL(err = write(fd, data, strlen(data)));
    if (err == -1) {
	Error("HyperPen write");
	return NULL;
    }

    while (numread < len) {
	err = xf86WaitForInput(fd, 100000);
	if (err == -1) {
	    Error("HyperPen select");
	    return NULL;
	}
	if (!err) {
	    ErrorF("Timeout while reading HyperPen tablet. No tablet connected ???\n");
	    return NULL;
	}

	SYSCALL(err = read(fd, buffer + numread++, 1));
	if (err == -1) {
	    Error("HyperPen read");
	    return NULL;
	}
	if (!err) {
	    --numread;
	    break;
	}
	if (cr_term && buffer[numread - 1] == '\r') {
	    buffer[numread - 1] = 0;
	    break;
	}
    }
    buffer[numread] = 0;
    return buffer;
}

/*
** xf86HypOpen
** Open and initialize the tablet, as well as probe for any needed data.
*/

#define WAIT(t)                                                 \
    err = xf86WaitForInput(-1, ((t) * 1000));                   \
    if (err == -1) {                                            \
        ErrorF("HyperPen select error : %s\n", strerror(errno));   \
        return !Success;                                        \
    }

static Bool
xf86HypOpen(LocalDevicePtr local)
{
    char		buffer[256];
    int			err, idx;
    int			i, n;
    double		res100;
    double		sratio, tratio;
    HyperPenDevicePtr	priv = (HyperPenDevicePtr)local->private;

    DBG(1, ErrorF("opening %s\n", priv->hypDevice));

    local->fd = xf86OpenSerial(local->options);
    if (local->fd == -1) {
	Error(priv->hypDevice);
	return !Success;
    }
    DBG(2, ErrorF("%s opened as fd %d\n", priv->hypDevice, local->fd));

   if (xf86SetSerialSpeed(local->fd, 9600) < 0)
      return !Success;

    DBG(1, ErrorF("initializing HyperPen tablet\n"));

/* Send reset to the tablet */
    
     write(local->fd, SS_RESET, strlen(SS_RESET));
     WAIT(1000);

/* Put it in prompt mode so it doesn't say anything before we're ready */
      SYSCALL(err = write(local->fd, SS_PROMPT_MODE, strlen(SS_PROMPT_MODE)));
      if (err == -1) {
	Error("HyperPen write");
	return !Success;
      }
/* Clear any pending input */
    tcflush(local->fd, TCIFLUSH);
 
    DBG(2, ErrorF("reading model\n"));

    if (!xf86HypWriteAndRead(local->fd, SS_GETID, buffer, 1, 0))
	return !Success;
    
    priv->modelid=buffer[0];

    for (n = -1, i = 0; models[i].id != 0; i++)
		if (models[i].id == priv->modelid)
			n = i;

    if (xf86Verbose)
	ErrorF("%s HyperPen Model: 0x%x (%s)\n", 
		XCONFIG_PROBED, priv->modelid, n==-1? "UNKNOWN":models[n].name);


	/* enable F-Keys */
        SYSCALL(err = write(local->fd, SS_MACROKEY, strlen(SS_MACROKEY)));
	    if (err == -1) {
		            ErrorF("HyperPen write error : %s\n", strerror(errno));
			            return !Success;
				        }

	    DBG(6, ErrorF("prepared F-keys\n"));
	    
	    /* start sequence depends on model ID */
	    if (priv->modelid == 0x43) SYSCALL(err = write(local->fd, SS_MACRO_4K, strlen(SS_MACRO_4K))); else SYSCALL(err = write(local->fd, SS_MACRO_56K, strlen(SS_MACRO_56K)));

	    if (err == -1) {
		            ErrorF("HyperPen write error : %s\n", strerror(errno));
			            return !Success;
				        }

	    DBG(6, ErrorF("started F-keys\n"));



    priv->hypRes = 500;
    res100 = priv->hypRes / 100;

    DBG(2, ErrorF("reading max coordinates\n"));

    if (!xf86HypWriteAndRead(local->fd, SS_CONFIG, buffer, 5, 0))
	return !Success;
    
    priv->hypMaxX = (buffer[1] & 0x7f) | (buffer[2] << 7);
    priv->hypMaxY = (buffer[3] & 0x7f) | (buffer[4] << 7);
    priv->hypMaxZ = 512;
    
if (xf86Verbose)
	ErrorF("%s HyperPen max tablet size %d.%02dinx%d.%02din, %dx%d "
	       "lines of resolution\n", XCONFIG_PROBED, 
	       priv->hypMaxX / priv->hypRes,
	       (priv->hypMaxX * 100 / priv->hypRes) % 100,
	       priv->hypMaxY / priv->hypRes,
	       (priv->hypMaxY * 100 / priv->hypRes) % 100,
	       priv->hypMaxX, priv->hypMaxY);

    if (priv->hypXOffset >= 0 && priv->hypYOffset >= 0) {
	priv->hypXOffset *= res100;
	priv->hypYOffset *= res100;
	priv->hypMaxX -= priv->hypXOffset;
	priv->hypMaxY -= priv->hypYOffset;
    }

    if (priv->hypXSize > 0 && priv->hypYSize > 0) {
	if ((priv->hypXSize * res100) <= priv->hypMaxX &&
	    (priv->hypYSize * res100) <= priv->hypMaxY) {
	    priv->hypXSize *= res100;
	    priv->hypYSize *= res100;
	} else {
	    ErrorF("%s HyperPen active area bigger than tablet, "
		   "assuming maximum\n", XCONFIG_PROBED);
	    priv->hypXSize = priv->hypMaxX;
	    priv->hypYSize = priv->hypMaxY;
	}
    } else {
	priv->hypXSize = priv->hypMaxX;
	priv->hypYSize = priv->hypMaxY;
    }

    /* map tablet area by screen aspect ratio */
    sratio = (double)screenInfo.screens[0]->height /
	     (double)screenInfo.screens[0]->width;
    tratio = (double)priv->hypMaxY / (double)priv->hypMaxX;

    if (tratio <= 1.0) {		/* tablet horizontal > vertical */
	priv->hypXSize = (double)priv->hypYSize / sratio;
	if (priv->hypXSize > priv->hypMaxX) priv->hypXSize = priv->hypMaxX;
    }
    else {
	priv->hypYSize = (double)priv->hypXSize / sratio;
	if (priv->hypYSize > priv->hypMaxY) priv->hypYSize = priv->hypMaxY;
    }
    ErrorF("%s HyperPen using tablet area %d by %d, at res %d lpi\n",
	XCONFIG_PROBED, priv->hypXSize, priv->hypYSize, priv->hypRes);

    if (priv->flags & BAUD_19200_FLAG) {
/* Send 19200 baud to the tablet */
        SYSCALL(err = write(local->fd, SS_RATE, strlen(SS_RATE)));
	    if (err == -1) {
		            ErrorF("HyperPen write error : %s\n", strerror(errno));
			            return !Success;
				        }

	    DBG(6, ErrorF("tablet set to 19200 bps\n"));


	        /* Wait 10 mSecs */
	        WAIT(10);

		    /* Set the speed of the serial link to 19200 */
		   if (xf86SetSerialSpeed(local->fd, 19200) < 0) {
			          return !Success;
				     }

		       DBG(6, ErrorF("set serial speed to 19200\n"));
    }


    
/* Sets up the tablet mode to increment, stream, and such */
      for (idx = 0; ss_initstr[idx]; idx++) {
	buffer[idx] = ss_initstr[idx];
      }

      buffer[idx] = 0;

      SYSCALL(err = write(local->fd, buffer, idx));

      

      if (err == -1) {
	Error("HyperPen write");
	return !Success;
      }

      if (err <= 0) {
	SYSCALL(close(local->fd));
	return !Success;
      }
    return Success;
}

/*
** xf86HypOpenDevice
** Opens and initializes the device driver stuff or something.
*/
static int
xf86HypOpenDevice(DeviceIntPtr pHyp)
{
    LocalDevicePtr	local = (LocalDevicePtr)pHyp->public.devicePrivate;
    HyperPenDevicePtr	priv = (HyperPenDevicePtr)PRIVATE(pHyp);

    if (xf86HypOpen(local) != Success) {
	if (local->fd >= 0) {
	    SYSCALL(close(local->fd));
	}
	local->fd = -1;
    }

/* Set the real values */
    InitValuatorAxisStruct(pHyp,
			   0,
			   0, /* min val */
			   priv->hypXSize, /* max val */
			   LPI2CPM(priv->hypRes), /* resolution */
			   0, /* min_res */
			   LPI2CPM(priv->hypRes)); /* max_res */
    InitValuatorAxisStruct(pHyp,
			   1,
			   0, /* min val */
			   priv->hypYSize, /* max val */
			   LPI2CPM(priv->hypRes), /* resolution */
			   0, /* min_res */
			   LPI2CPM(priv->hypRes)); /* max_res */
    InitValuatorAxisStruct(pHyp,
			   2,
			   0, /* min val */
			   511, /* max val */
			   512, /* resolution */
			   0, /* min_res */
			   512); /* max_res */
    return (local->fd != -1);
}

/*
** xf86HypProc
** Handle requests to do stuff to the driver.
*/
static int
xf86HypProc(DeviceIntPtr pHyp, int what)
{
    CARD8		map[25];
    int			nbaxes;
    int			nbbuttons;
    int			loop;
    LocalDevicePtr	local = (LocalDevicePtr)pHyp->public.devicePrivate;
    HyperPenDevicePtr	priv = (HyperPenDevicePtr)PRIVATE(pHyp);



    switch (what) {
	case DEVICE_INIT:
	    DBG(1, ErrorF("xf86HypProc pHyp=%p what=INIT\n", (void *)pHyp));

	    nbaxes = 3;			/* X, Y, Z */
	    nbbuttons = (priv->flags & STYLUS_FLAG)? 3 : 4;

	    for(loop=1; loop<=nbbuttons; loop++) map[loop] = loop;

	    if (InitButtonClassDeviceStruct(pHyp,
					    nbbuttons,
					    map) == FALSE) {
		ErrorF("unable to allocate Button class device\n");
		return !Success;
	    }

	    if (InitFocusClassDeviceStruct(pHyp) == FALSE) {
		ErrorF("unable to init Focus class device\n");
		return !Success;
	    }

	    if (InitPtrFeedbackClassDeviceStruct(pHyp,
		   xf86HypControlProc) == FALSE) {
		ErrorF("unable to init ptr feedback\n");
		return !Success;
	    }

	    if (InitProximityClassDeviceStruct(pHyp) == FALSE) {
		ErrorF("unable to init proximity class device\n"); 
		return !Success;
	    }

	    if (InitValuatorClassDeviceStruct(pHyp,
		   nbaxes,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 3
		   xf86GetMotionEvents,
#endif
		   local->history_size,
		   (priv->flags & ABSOLUTE_FLAG)? Absolute: Relative)
		   == FALSE) {
		ErrorF("unable to allocate Valuator class device\n"); 
		return !Success;
	    }
/* allocate the motion history buffer if needed */
	    xf86MotionHistoryAllocate(local);
/* open the device to gather informations */
	    xf86HypOpenDevice(pHyp);
	    break;

	case DEVICE_ON:
	    DBG(1, ErrorF("xf86HypProc pHyp=%p what=ON\n", (void *)pHyp));

	    if ((local->fd < 0) && (!xf86HypOpenDevice(pHyp))) {
		return !Success;
	    }
	    xf86AddEnabledDevice(local);
	    pHyp->public.on = TRUE;
	    break;

	case DEVICE_OFF:
	    DBG(1, ErrorF("xf86HypProc  pHyp=%p what=%s\n", (void *)pHyp,
		   (what == DEVICE_CLOSE) ? "CLOSE" : "OFF"));
	    if (local->fd >= 0)
		xf86RemoveEnabledDevice(local);
	    pHyp->public.on = FALSE;
	    break;

	case DEVICE_CLOSE:
	    DBG(1, ErrorF("xf86HypProc  pHyp=%p what=%s\n", (void *)pHyp,
		   (what == DEVICE_CLOSE) ? "CLOSE" : "OFF"));
	    SYSCALL(close(local->fd));
	    local->fd = -1;
	    break;

	default:
	    ErrorF("unsupported mode=%d\n", what);
	    return !Success;
	    break;
    }
    DBG(2, ErrorF("END   xf86HypProc Success what=%d dev=%p priv=%p\n",
	   what, (void *)pHyp, (void *)priv));
    return Success;
}

/*
** xf86HypClose
** It...  Uh...  Closes the physical device?
*/
static void
xf86HypClose(LocalDevicePtr local)
{
    if (local->fd >= 0) {
	SYSCALL(close(local->fd));
    }
    local->fd = -1;
}

/*
** xf86HypChangeControl
** When I figure out what it does, it will do it.
*/
static int
xf86HypChangeControl(LocalDevicePtr local, xDeviceCtl *control)
{
    xDeviceResolutionCtl	*res;

    res = (xDeviceResolutionCtl *)control;
	
    if ((control->control != DEVICE_RESOLUTION) ||
	   (res->num_valuators < 1))
	return (BadMatch);

    return(Success);
}

/*
** xf86HypSwitchMode
** Switches the mode.  For now just absolute or relative, hopefully
** more on the way.
*/
static int
xf86HypSwitchMode(ClientPtr client, DeviceIntPtr dev, int mode)
{
    LocalDevicePtr	local = (LocalDevicePtr)dev->public.devicePrivate;
    HyperPenDevicePtr	priv = (HyperPenDevicePtr)(local->private);
    char		newmode;

    DBG(3, ErrorF("xf86HypSwitchMode dev=%p mode=%d\n", (void *)dev, mode));

    switch(mode) {
	case Absolute:
	    priv->flags |= ABSOLUTE_FLAG;

	    break;

	case Relative:
	    priv->flags &= ~ABSOLUTE_FLAG;

	    break;

	default:
	    DBG(1, ErrorF("xf86HypSwitchMode dev=%p invalid mode=%d\n",
		   (void *)dev, mode));
	    return BadMatch;
    }
    SYSCALL(write(local->fd, &newmode, 1));
    return Success;
}

/*
** xf86HypAllocate
** Allocates the device structures for the HyperPen.
*/
static LocalDevicePtr
xf86HypAllocate(void)
{
    LocalDevicePtr	local = xf86AllocateInput(hypDrv, 0);
    HyperPenDevicePtr	priv = (HyperPenDevicePtr)xalloc(sizeof(HyperPenDeviceRec));
#if defined (sun) && !defined(i386)
    char		*dev_name = getenv("HYPERPEN_DEV");
#endif

    local->name = XI_NAME;
    local->type_name = "HyperPen Tablet";
    local->flags = 0; /*XI86_NO_OPEN_ON_INIT;*/
    local->device_control = xf86HypProc;
    local->read_input = xf86HypReadInput;
    local->control_proc = xf86HypChangeControl;
    local->close_proc = xf86HypClose;
    local->switch_mode = xf86HypSwitchMode;
    local->conversion_proc = xf86HypConvert;
    local->reverse_conversion_proc = xf86HypReverseConvert;
    local->fd = -1;
    local->atom = 0;
    local->dev = NULL;
    local->private = priv;
    local->private_flags = 0;
    local->history_size  = 0;

#if defined(sun) && !defined(i386)
    if (dev_name) {
	priv->hypDevice = (char *)xalloc(strlen(dev_name) + 1);
	strcpy(priv->hypDevice, dev_name);
	ErrorF("xf86HypOpen port changed to '%s'\n", priv->hypDevice);
    } else {
	priv->hypDevice = "";
    }
#else
    priv->hypDevice = "";         /* device file name */
#endif
    priv->hypOldX = -1;           /* previous X position */
    priv->hypOldY = -1;           /* previous Y position */
    priv->hypOldProximity = 0;    /* previous proximity */
    priv->hypOldButtons = 0;      /* previous buttons state */
    priv->hypMaxX = -1;           /* max X value */
    priv->hypMaxY = -1;           /* max Y value */
    priv->hypXSize = -1;	  /* active area X */
    priv->hypXOffset = 0;	  /* active area X offset */
    priv->hypYSize = -1;	  /* active area Y */
    priv->hypYOffset = 0;	  /* active area Y offset */
    priv->flags = ABSOLUTE_FLAG;  /* various flags -- default abs format */
    priv->hypIndex = 0;           /* number of bytes read */
    priv->hypRes = 0;		  /* resolution */
    stylus=0;

    return local;
}


/*
 * xf86HypUninit --
 *
 * called when the driver is unloaded.
 */
static void
xf86HypUninit(InputDriverPtr	drv,
	      LocalDevicePtr	local,
	      int flags)
{
    HyperPenDevicePtr	priv = (HyperPenDevicePtr) local->private;
    
    DBG(1, ErrorF("xf86HypUninit\n"));
    
    xf86HypProc(local->dev, DEVICE_OFF);
    
    xfree (priv);
    xf86DeleteInput(local, 0);    
}


/*
 * xf86HypInit --
 *
 * called when the module subsection is found in XF86Config
 */
static InputInfoPtr
xf86HypInit(InputDriverPtr	drv,
	    IDevPtr		dev,
	    int			flags)
{
    LocalDevicePtr	local = NULL;
    HyperPenDevicePtr	priv = NULL;
    char		*s;

    hypDrv = drv;

    local = xf86HypAllocate();
    local->conf_idev = dev;

    xf86CollectInputOptions(local, default_options, NULL);   
    xf86OptionListReport( local->options );

    if (local)
	priv = (HyperPenDevicePtr) local->private;

    if (!local || !priv) {
	goto SetupProc_fail;
    }
    
    local->name = dev->identifier;
    priv->AutoPT = 1;
    
    /* Serial Device is mandatory */
    priv->hypDevice = xf86FindOptionValue(local->options, "Device");

    if (!priv->hypDevice) {
	xf86Msg (X_ERROR, "%s: No Device specified.\n", dev->identifier);
	goto SetupProc_fail;
    }

    /* Process the common options. */
    xf86ProcessCommonOptions(local, local->options);

    /* Optional configuration */

    xf86Msg(X_CONFIG, "%s serial device is %s\n", dev->identifier,
	    priv->hypDevice);

    debug_level = xf86SetIntOption(local->options, "DebugLevel", 0);
    if (debug_level > 0) {
	xf86Msg(X_CONFIG, "%s: debug level set to %d\n", dev->identifier, debug_level);
    }



    s = xf86FindOptionValue(local->options, "Mode");

    if (s && (xf86NameCmp(s, "absolute") == 0)) {
	priv->flags = priv->flags | ABSOLUTE_FLAG;
    }
    else if (s && (xf86NameCmp(s, "relative") == 0)) {
	priv->flags = priv->flags & ~ABSOLUTE_FLAG;
    }
    else if (s) {
	xf86Msg(X_ERROR, "%s: invalid Mode (should be absolute or relative). "
		"Using default.\n", dev->identifier);
    }
    xf86Msg(X_CONFIG, "%s is in %s mode\n", local->name,
	    (priv->flags & ABSOLUTE_FLAG) ? "absolute" : "relative");	    


    s = xf86FindOptionValue(local->options, "Cursor");

    if (s && (xf86NameCmp(s, "stylus") == 0)) {
	priv->flags = priv->flags | STYLUS_FLAG;
    }
    else if (s && (xf86NameCmp(s, "puck") == 0)) {
	priv->flags = priv->flags & ~STYLUS_FLAG;
    }
    else if (s) {
	xf86Msg(X_ERROR, "%s: invalid Cursor (should be stylus or puck). "
		"Using default.\n", dev->identifier);
    }
    xf86Msg(X_CONFIG, "%s is in cursor-mode %s\n", local->name,
	    (priv->flags & STYLUS_FLAG) ? "cursor" : "puck");	    

    priv->hypXSize = xf86SetIntOption(local->options, "XSize", 0);
    if (priv->hypXSize != 0) {
	    xf86Msg(X_CONFIG, "%s: XSize = %d\n", 
		    dev->identifier, priv->hypXSize);
    }

    priv->hypYSize = xf86SetIntOption(local->options, "YSize", 0);
    if (priv->hypYSize != 0) {
	    xf86Msg(X_CONFIG, "%s: YSize = %d\n", 
		    dev->identifier, priv->hypYSize);
    }

    priv->PT = xf86SetIntOption(local->options, "PMin", 0);
    if (priv->PT > 2) {
	    xf86Msg(X_CONFIG, "%s: PMin = %d\n", 
		    dev->identifier, priv->PT);
	    priv->AutoPT = 0;
    }
    else
	xf86Msg(X_ERROR, "%s: invalid PMin value (should be > 2)."
		"Using default.\n", dev->identifier);
      

    priv->PMax = xf86SetIntOption(local->options, "PMax", 0);
    if (priv->PMax > 3) {
	    xf86Msg(X_CONFIG, "%s: PMax = %d\n", 
		    dev->identifier, priv->PMax);
    }
    else
	xf86Msg(X_ERROR, "%s: invalid PMax value (should be > 3)."
		"Using default.\n", dev->identifier);
      

    priv->hypXOffset = xf86SetIntOption(local->options, "XOffset", 0);
    if (priv->hypXOffset != 0) {
	    xf86Msg(X_CONFIG, "%s: XOffset = %d\n", 
		    dev->identifier, priv->hypXOffset);
    }

    priv->hypYOffset = xf86SetIntOption(local->options, "YOffset", 0);
    if (priv->hypYOffset != 0) {
	    xf86Msg(X_CONFIG, "%s: YOffset = %d\n", 
		    dev->identifier, priv->hypYOffset);
    }

    if (xf86SetBoolOption(local->options, "InvX", FALSE)) {
	    priv->flags |= INVX_FLAG;
    	    xf86Msg(X_CONFIG, "%s: InvX\n", dev->identifier); 
    }

    if (xf86SetBoolOption(local->options, "InvY", FALSE)) {
	    priv->flags |= INVY_FLAG;
    	    xf86Msg(X_CONFIG, "%s: InvY\n", dev->identifier); 
    }

    {
	int val;
	val = xf86SetIntOption(local->options, "BaudRate", 0);
	switch (val) {
	case 19200:
	    priv->flags |= BAUD_19200_FLAG;
	    break;
	case 9600:
	    priv->flags &= ~BAUD_19200_FLAG;
	    break;
	default:
    	    xf86Msg(X_CONFIG, "%s: Illegal speed value (must be 9600 or 19200)\n", dev->identifier); 
	    break;
 	}
    }

    /* mark the device configured */
    local->flags |= XI86_POINTER_CAPABLE | XI86_CONFIGURED;

    /* return the LocalDevice */
    return (local);

  SetupProc_fail:
    if (priv)
	xfree(priv);
    if (local)
	xfree(local);
    return NULL;
}

_X_EXPORT InputDriverRec HYPERPEN = {
    1,				/* driver version */
    "hyperpen",			/* driver name */
    NULL,			/* identify */
    xf86HypInit,		/* pre-init */
    xf86HypUninit,		/* un-init */
    NULL,			/* module */
    0				/* ref count */
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
 * xf86HypUnplug --
 *
 * called when the module subsection is found in XF86Config
 */
static void
xf86HypUnplug(pointer	p)
{
    DBG(1, ErrorF("xf86HypUnplug\n"));
}

/*
 * xf86HypPlug --
 *
 * called when the module subsection is found in XF86Config
 */
static pointer
xf86HypPlug(pointer	module,
	    pointer	options,
	    int		*errmaj,
	    int		*errmin)
{
    DBG(1, ErrorF("xf86HypPlug\n"));
	
    xf86AddInputDriver(&HYPERPEN, module, 0);

    return module;
}

static XF86ModuleVersionInfo xf86HypVersionRec =
{
    "hyperpen",
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

_X_EXPORT XF86ModuleData hyperpenModuleData = {
    &xf86HypVersionRec,
    xf86HypPlug,
    xf86HypUnplug
};

#endif /* XFree86LOADER */

/* end of xf86HyperPen.c */
