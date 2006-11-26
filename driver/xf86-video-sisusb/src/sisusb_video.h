/* $XFree86$ */
/* $XdotOrg: driver/xf86-video-sisusb/src/sisusb_video.h,v 1.3 2005/07/09 02:50:34 twini Exp $ */
/*
 * Xv driver for SiS 315 USB
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1) Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2) Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3) The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author:    Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifndef _SISUSB_VIDEO_H_
#define _SISUSB_VIDEO_H_

#include "sisusb_videostr.h"

static 		XF86VideoAdaptorPtr SISUSBSetupImageVideo(ScreenPtr);
static int 	SISUSBSetPortAttribute(ScrnInfoPtr, Atom, INT32, pointer);
static int 	SISUSBGetPortAttribute(ScrnInfoPtr, Atom ,INT32 *, pointer);
static void 	SISUSBQueryBestSize(ScrnInfoPtr, Bool, short, short, short,
			short, unsigned int *,unsigned int *, pointer);
static int 	SISUSBQueryImageAttributes(ScrnInfoPtr,
    			int, UShort *, UShort *, int *, int *);
static void 	SISUSBStopVideo(ScrnInfoPtr, pointer, Bool);
void		SISUSBSetPortDefaults(ScrnInfoPtr pScrn, SISUSBPortPrivPtr pPriv);
#ifdef SIS_ENABLEXV
static int 	SISUSBPutImage( ScrnInfoPtr,
    			short, short, short, short, short, short, short, short,
    			int, UChar *, short, short, Bool, RegionPtr, pointer);
static void 	SISUSBVideoTimerCallback(ScrnInfoPtr pScrn, Time now);
static void     SISUSBInitOffscreenImages(ScreenPtr pScrn);
static FBLinearPtr SISUSBAllocateOverlayMemory(ScrnInfoPtr pScrn, FBLinearPtr linear, int size);
void		SiSUSBUpdateXvGamma(SISUSBPtr pSiSUSB, SISUSBPortPrivPtr pPriv);
#endif

#ifdef XV_SD_DEPRECATED
extern int	SISUSBSetPortUtilAttribute(ScrnInfoPtr pScrn, Atom attribute,
					INT32 value, SISUSBPortPrivPtr pPriv);
extern int	SISUSBGetPortUtilAttribute(ScrnInfoPtr pScrn,  Atom attribute,
					INT32 *value, SISUSBPortPrivPtr pPriv);
#endif

#define OFF_DELAY   	200    /* milliseconds */
#define FREE_DELAY  	30000

#define OFF_TIMER   	0x01
#define FREE_TIMER  	0x02
#define CLIENT_VIDEO_ON 0x04

#define TIMER_MASK      (OFF_TIMER | FREE_TIMER)

#define WATCHDOG_DELAY  200000 /* Watchdog counter for Vertical Restrace waiting */

#define MAKE_ATOM(a) MakeAtom(a, sizeof(a) - 1, TRUE)

#define IMAGE_MIN_WIDTH         32  	/* Minimum and maximum source image sizes */
#define IMAGE_MIN_HEIGHT        24
#define IMAGE_MAX_WIDTH_315   1920
#define IMAGE_MAX_HEIGHT_315  1080
#define IMAGE_MAX_WIDTH_340   1920	/* ? */

#define OVERLAY_MIN_WIDTH       32  	/* Minimum overlay sizes */
#define OVERLAY_MIN_HEIGHT      24

#define DISPMODE_SINGLE1 0x1  		/* CRT1 only */
#define DISPMODE_SINGLE2 0x2  		/* CRT2 only */
#define DISPMODE_MIRROR  0x4  		/* CRT1 + CRT2 MIRROR (see note below) */

#define LINEBUFLIMIT1    384		/* Limits at which line buffers must be merged */
#define LINEBUFLIMIT2    720
#define LINEBUFLIMIT3    576
#define LINEBUFLIMIT4   1280		/* 340 */

#define NUM_FORMATS 3

static XF86VideoFormatRec SISUSBFormats[NUM_FORMATS] =
{
   { 8, PseudoColor},
   {16, TrueColor},
   {24, TrueColor}
};

static char sisxvcolorkey[] 				= "XV_COLORKEY";
static char sisxvbrightness[] 				= "XV_BRIGHTNESS";
static char sisxvcontrast[] 				= "XV_CONTRAST";
static char sisxvsaturation[] 				= "XV_SATURATION";
static char sisxvhue[] 					= "XV_HUE";
static char sisxvautopaintcolorkey[] 			= "XV_AUTOPAINT_COLORKEY";
static char sisxvsetdefaults[] 				= "XV_SET_DEFAULTS";
static char sisxvswitchcrt[] 				= "XV_SWITCHCRT";
static char sisxvtvxposition[] 				= "XV_TVXPOSITION";
static char sisxvtvyposition[] 				= "XV_TVYPOSITION";
static char sisxvgammared[] 				= "XV_GAMMA_RED";
static char sisxvgammagreen[] 				= "XV_GAMMA_GREEN";
static char sisxvgammablue[] 				= "XV_GAMMA_BLUE";
static char sisxvdisablegfx[] 				= "XV_DISABLE_GRAPHICS";
static char sisxvdisablegfxlr[] 			= "XV_DISABLE_GRAPHICS_LR";
static char sisxvdisablecolorkey[] 			= "XV_DISABLE_COLORKEY";
static char sisxvusechromakey[] 			= "XV_USE_CHROMAKEY";
static char sisxvinsidechromakey[] 			= "XV_INSIDE_CHROMAKEY";
static char sisxvyuvchromakey[] 			= "XV_YUV_CHROMAKEY";
static char sisxvchromamin[] 				= "XV_CHROMAMIN";
static char sisxvchromamax[] 				= "XV_CHROMAMAX";
#ifdef XV_SD_DEPRECATED
static char sisxvqueryvbflags[] 			= "XV_QUERYVBFLAGS";
static char sisxvsdgetdriverversion[] 			= "XV_SD_GETDRIVERVERSION";
static char sisxvsdgethardwareinfo[]			= "XV_SD_GETHARDWAREINFO";
static char sisxvsdgetbusid[] 				= "XV_SD_GETBUSID";
static char sisxvsdqueryvbflagsversion[] 		= "XV_SD_QUERYVBFLAGSVERSION";
static char sisxvsdgetsdflags[] 			= "XV_SD_GETSDFLAGS";
static char sisxvsdgetsdflags2[] 			= "XV_SD_GETSDFLAGS2";
static char sisxvsdunlocksisdirect[] 			= "XV_SD_UNLOCKSISDIRECT";
static char sisxvsdsetvbflags[] 			= "XV_SD_SETVBFLAGS";
static char sisxvsdquerydetecteddevices[] 		= "XV_SD_QUERYDETECTEDDEVICES";
static char sisxvsdcrt1status[] 			= "XV_SD_CRT1STATUS";
static char sisxvsdcheckmodeindexforcrt2[] 		= "XV_SD_CHECKMODEINDEXFORCRT2";
static char sisxvsdresultcheckmodeindexforcrt2[] 	= "XV_SD_RESULTCHECKMODEINDEXFORCRT2";
static char sisxvsdredetectcrt2[]			= "XV_SD_REDETECTCRT2DEVICES";
static char sisxvsdsisantiflicker[] 			= "XV_SD_SISANTIFLICKER";
static char sisxvsdsissaturation[] 			= "XV_SD_SISSATURATION";
static char sisxvsdsisedgeenhance[] 			= "XV_SD_SISEDGEENHANCE";
static char sisxvsdsiscolcalibf[] 			= "XV_SD_SISCOLCALIBF";
static char sisxvsdsiscolcalibc[] 			= "XV_SD_SISCOLCALIBC";
static char sisxvsdsiscfilter[] 			= "XV_SD_SISCFILTER";
static char sisxvsdsisyfilter[] 			= "XV_SD_SISYFILTER";
static char sisxvsdchcontrast[] 			= "XV_SD_CHCONTRAST";
static char sisxvsdchtextenhance[] 			= "XV_SD_CHTEXTENHANCE";
static char sisxvsdchchromaflickerfilter[] 		= "XV_SD_CHCHROMAFLICKERFILTER";
static char sisxvsdchlumaflickerfilter[] 		= "XV_SD_CHLUMAFLICKERFILTER";
static char sisxvsdchcvbscolor[] 			= "XV_SD_CHCVBSCOLOR";
static char sisxvsdchoverscan[]				= "XV_SD_CHOVERSCAN";
static char sisxvsdenablegamma[]			= "XV_SD_ENABLEGAMMA";
static char sisxvsdtvxscale[] 				= "XV_SD_TVXSCALE";
static char sisxvsdtvyscale[] 				= "XV_SD_TVYSCALE";
static char sisxvsdgetscreensize[] 			= "XV_SD_GETSCREENSIZE";
static char sisxvsdstorebrir[] 				= "XV_SD_STOREDGAMMABRIR";
static char sisxvsdstorebrig[] 				= "XV_SD_STOREDGAMMABRIG";
static char sisxvsdstorebrib[] 				= "XV_SD_STOREDGAMMABRIB";
static char sisxvsdstorepbrir[] 			= "XV_SD_STOREDGAMMAPBRIR";
static char sisxvsdstorepbrig[] 			= "XV_SD_STOREDGAMMAPBRIG";
static char sisxvsdstorepbrib[] 			= "XV_SD_STOREDGAMMAPBRIB";
static char sisxvsdstorebrir2[]				= "XV_SD_STOREDGAMMABRIR2";
static char sisxvsdstorebrig2[]				= "XV_SD_STOREDGAMMABRIG2";
static char sisxvsdstorebrib2[]				= "XV_SD_STOREDGAMMABRIB2";
static char sisxvsdstorepbrir2[] 			= "XV_SD_STOREDGAMMAPBRIR2";
static char sisxvsdstorepbrig2[] 			= "XV_SD_STOREDGAMMAPBRIG2";
static char sisxvsdstorepbrib2[] 			= "XV_SD_STOREDGAMMAPBRIB2";
static char sisxvsdstoregarc2[]				= "XV_SD_GAMMACRT2R";
static char sisxvsdstoregagc2[]				= "XV_SD_GAMMACRT2G";
static char sisxvsdstoregabc2[]				= "XV_SD_GAMMACRT2B";
static char sisxvsdstorebrirc2[]			= "XV_SD_STOREDGAMMABRIRC2";
static char sisxvsdstorebrigc2[]			= "XV_SD_STOREDGAMMABRIGC2";
static char sisxvsdstorebribc2[]			= "XV_SD_STOREDGAMMABRIBC2";
static char sisxvsdstorepbrirc2[] 			= "XV_SD_STOREDGAMMAPBRIRC2";
static char sisxvsdstorepbrigc2[] 			= "XV_SD_STOREDGAMMAPBRIGC2";
static char sisxvsdstorepbribc2[] 			= "XV_SD_STOREDGAMMAPBRIBC2";
static char sisxvsdhidehwcursor[] 			= "XV_SD_HIDEHWCURSOR";
static char sisxvsdpanelmode[] 				= "XV_SD_PANELMODE";
#ifdef TWDEBUG
static char sisxvsetreg[]				= "XV_SD_SETREG";
#endif
#endif

/***********************************************/
/*               OVERLAY ADAPTOR               */
/***********************************************/

/* client libraries expect an encoding */
static XF86VideoEncodingRec DummyEncoding =
{
   0,
   "XV_IMAGE",
   0, 0,		/* Will be filled in */
   {1, 1}
};

static XF86AttributeRec SISUSBAttributes_315[] =
{
   {XvSettable | XvGettable, 0, (1 << 24) - 1, sisxvcolorkey},
   {XvSettable | XvGettable, -128, 127,        sisxvbrightness},
   {XvSettable | XvGettable, 0, 7,             sisxvcontrast},
   {XvSettable | XvGettable, -7, 7,            sisxvsaturation},
   {XvSettable | XvGettable, -8, 7,            sisxvhue},
   {XvSettable | XvGettable, 0, 1,             sisxvautopaintcolorkey},
   {XvSettable             , 0, 0,             sisxvsetdefaults},
   {XvSettable | XvGettable, -32, 32,          sisxvtvxposition},
   {XvSettable | XvGettable, -32, 32,          sisxvtvyposition},
   {XvSettable | XvGettable, 100, 10000,       sisxvgammared},
   {XvSettable | XvGettable, 100, 10000,       sisxvgammagreen},
   {XvSettable | XvGettable, 100, 10000,       sisxvgammablue},
   {XvSettable | XvGettable, 0, 1,             sisxvdisablegfx},
   {XvSettable | XvGettable, 0, 1,             sisxvdisablegfxlr},
   {XvSettable | XvGettable, 0, 1,             sisxvdisablecolorkey},
   {XvSettable | XvGettable, 0, 1,             sisxvusechromakey},
   {XvSettable | XvGettable, 0, 1,             sisxvinsidechromakey},
   {XvSettable | XvGettable, 0, (1 << 24) - 1, sisxvchromamin},
   {XvSettable | XvGettable, 0, (1 << 24) - 1, sisxvchromamax},
#ifdef XV_SD_DEPRECATED
   {             XvGettable, 0, -1,    	       sisxvqueryvbflags},
   {             XvGettable, 0, -1,    	       sisxvsdgetdriverversion},
   {             XvGettable, 0, -1,    	       sisxvsdgethardwareinfo},
   {             XvGettable, 0, -1,    	       sisxvsdgetbusid},
   {             XvGettable, 0, -1,    	       sisxvsdqueryvbflagsversion},
   {             XvGettable, 0, -1,   	       sisxvsdgetsdflags},
   {             XvGettable, 0, -1,   	       sisxvsdgetsdflags2},
   {XvSettable | XvGettable, 0, -1,   	       sisxvsdunlocksisdirect},
   {XvSettable             , 0, -1,   	       sisxvsdsetvbflags},
   {             XvGettable, 0, -1,    	       sisxvsdquerydetecteddevices},
   {XvSettable | XvGettable, 0, 1,    	       sisxvsdcrt1status},
   {XvSettable             , 0, -1,    	       sisxvsdcheckmodeindexforcrt2},
   {             XvGettable, 0, -1,   	       sisxvsdresultcheckmodeindexforcrt2},
   {XvSettable             , 0, 0,             sisxvsdredetectcrt2},
   {XvSettable | XvGettable, 0, 4,             sisxvsdsisantiflicker},
   {XvSettable | XvGettable, 0, 15,            sisxvsdsissaturation},
   {XvSettable | XvGettable, 0, 15,            sisxvsdsisedgeenhance},
   {XvSettable | XvGettable, -128, 127,        sisxvsdsiscolcalibf},
   {XvSettable | XvGettable, -120, 120,        sisxvsdsiscolcalibc},
   {XvSettable | XvGettable, 0, 1,             sisxvsdsiscfilter},
   {XvSettable | XvGettable, 0, 8,             sisxvsdsisyfilter},
   {XvSettable | XvGettable, 0, 15,            sisxvsdchcontrast},
   {XvSettable | XvGettable, 0, 15,            sisxvsdchtextenhance},
   {XvSettable | XvGettable, 0, 15,            sisxvsdchchromaflickerfilter},
   {XvSettable | XvGettable, 0, 15,            sisxvsdchlumaflickerfilter},
   {XvSettable | XvGettable, 0, 1,             sisxvsdchcvbscolor},
   {XvSettable | XvGettable, 0, 3,             sisxvsdchoverscan},
   {XvSettable | XvGettable, 0, 7,             sisxvsdenablegamma},
   {XvSettable | XvGettable, -16, 16,          sisxvsdtvxscale},
   {XvSettable | XvGettable, -4, 3,            sisxvsdtvyscale},
   {             XvGettable, 0, -1,   	       sisxvsdgetscreensize},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorebrir},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorebrig},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorebrib},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorepbrir},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorepbrig},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorepbrib},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorebrir2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorebrig2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorebrib2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorepbrir2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorepbrig2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorepbrib2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstoregarc2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstoregagc2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstoregabc2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorebrirc2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorebrigc2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorebribc2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorepbrirc2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorepbrigc2},
   {XvSettable | XvGettable, 100, 10000,       sisxvsdstorepbribc2},
   {XvSettable | XvGettable, 0, 1,             sisxvsdhidehwcursor},
   {XvSettable | XvGettable, 0, 15,            sisxvsdpanelmode},
#ifdef TWDEBUG
   {XvSettable             , 0, -1,   	       sisxvsetreg},
#endif
#endif /* XV_SD_DEPRECATED */
   {XvSettable | XvGettable, 0, 1,             sisxvswitchcrt},
   {0                      , 0, 0,             NULL}
};

#define NUM_IMAGES_315 7	    /* basically NV12 only - but does not work */

#define PIXEL_FMT_YV12 FOURCC_YV12  /* 0x32315659 */
#define PIXEL_FMT_UYVY FOURCC_UYVY  /* 0x59565955 */
#define PIXEL_FMT_YUY2 FOURCC_YUY2  /* 0x32595559 */
#define PIXEL_FMT_I420 FOURCC_I420  /* 0x30323449 */
#define PIXEL_FMT_RGB5 0x35315652
#define PIXEL_FMT_RGB6 0x36315652
#define PIXEL_FMT_YVYU 0x55595659   /* 315/330+ only */
#define PIXEL_FMT_NV12 0x3231564e   /* 330+ only */
#define PIXEL_FMT_NV21 0x3132564e   /* 330+ only */

/* TODO: */
#define PIXEL_FMT_RAW8 0x38574152

static XF86ImageRec SISUSBImages[NUM_IMAGES_315] =
{
    XVIMAGE_YUY2, /* If order is changed, SISOffscreenImages must be adapted */
    XVIMAGE_YV12,
    XVIMAGE_UYVY,
    XVIMAGE_I420
    ,
    { /* RGB 555 */
      PIXEL_FMT_RGB5,
      XvRGB,
      LSBFirst,
      {'R','V','1','5',
       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
      16,
      XvPacked,
      1,
      15, 0x7C00, 0x03E0, 0x001F,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0,
      {'R', 'V', 'B',0,
       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      XvTopToBottom
    },
    { /* RGB 565 */
      PIXEL_FMT_RGB6,
      XvRGB,
      LSBFirst,
      {'R','V','1','6',
       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
      16,
      XvPacked,
      1,
      16, 0xF800, 0x07E0, 0x001F,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0,
      {'R', 'V', 'B',0,
       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      XvTopToBottom
    },
    {  /* YVYU */
      PIXEL_FMT_YVYU, \
      XvYUV, \
      LSBFirst, \
      {'Y','V','Y','U',
	0x00,0x00,0x00,0x10,0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71},
      16,
      XvPacked,
      1,
      0, 0, 0, 0,
      8, 8, 8,
      1, 2, 2,
      1, 1, 1,
      {'Y','V','Y','U',
       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      XvTopToBottom
   }
};

typedef struct {
    int pixelFormat;

    CARD16  pitch;
    CARD16  origPitch;

    CARD8   keyOP;
    CARD16  HUSF;
    CARD16  VUSF;
    CARD8   IntBit;
    CARD8   wHPre;

    CARD16  srcW;
    CARD16  srcH;

    BoxRec  dstBox;

    CARD32  PSY;
    CARD32  PSV;
    CARD32  PSU;

    CARD16  SCREENheight;

    CARD16  lineBufSize;

    DisplayModePtr  currentmode;

    CARD8   bobEnable;

    CARD8   planar;
    CARD8   planar_shiftpitch;

    CARD8   contrastCtrl;
    CARD8   contrastFactor;

    CARD16  oldLine, oldtop;

    CARD8   (*VBlankActiveFunc)(SISUSBPtr, SISUSBPortPrivPtr);
#if 0
    CARD32  (*GetScanLineFunc)(SISUSBPtr pSiSUSB);
#endif

} SISUSBOverlayRec, *SISUSBOverlayPtr;

#endif


