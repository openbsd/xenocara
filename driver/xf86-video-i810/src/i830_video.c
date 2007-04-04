#define VIDEO_DEBUG 0
/***************************************************************************
 
Copyright 2000 Intel Corporation.  All Rights Reserved. 

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the 
"Software"), to deal in the Software without restriction, including 
without limitation the rights to use, copy, modify, merge, publish, 
distribute, sub license, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so, subject to 
the following conditions: 

The above copyright notice and this permission notice (including the 
next paragraph) shall be included in all copies or substantial portions 
of the Software. 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. 
IN NO EVENT SHALL INTEL, AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/
/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/i810/i830_video.c,v 1.11tsi Exp $ */

/*
 * Reformatted with GNU indent (2.2.8), using the following options:
 *
 *    -bad -bap -c41 -cd0 -ncdb -ci6 -cli0 -cp0 -ncs -d0 -di3 -i3 -ip3 -l78
 *    -lp -npcs -psl -sob -ss -br -ce -sc -hnl
 *
 * This provides a good match with the original i810 code and preferred
 * XFree86 formatting conventions.
 *
 * When editing this driver, please follow the existing formatting, and edit
 * with <TAB> characters expanded at 8-column intervals.
 */

/*
 * i830_video.c: i830/i845 Xv driver. 
 *
 * Copyright © 2002 by Alan Hourihane and David Dawes
 *
 * Authors: 
 *	Alan Hourihane <alanh@tungstengraphics.com>
 *	David Dawes <dawes@xfree86.org>
 *
 * Derived from i810 Xv driver:
 *
 * Authors of i810 code:
 * 	Jonathan Bian <jonathan.bian@intel.com>
 *      Offscreen Images:
 *        Matt Sottek <matthew.j.sottek@intel.com>
 */

/*
 * XXX Could support more formats.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Resources.h"
#include "xf86_ansic.h"
#include "compiler.h"
#include "xf86PciInfo.h"
#include "xf86Pci.h"
#include "xf86fbman.h"
#include "regionstr.h"

#include "i830.h"
#include "xf86xv.h"
#include <X11/extensions/Xv.h>
#include "xaa.h"
#include "xaalocal.h"
#include "dixstruct.h"
#include "fourcc.h"

#ifndef USE_USLEEP_FOR_VIDEO
#define USE_USLEEP_FOR_VIDEO 0
#endif

#define OFF_DELAY 	250		/* milliseconds */
#define FREE_DELAY 	15000

#define OFF_TIMER 	0x01
#define FREE_TIMER	0x02
#define CLIENT_VIDEO_ON	0x04

#define TIMER_MASK      (OFF_TIMER | FREE_TIMER)

static void I830InitOffscreenImages(ScreenPtr);

static XF86VideoAdaptorPtr I830SetupImageVideo(ScreenPtr);
static void I830StopVideo(ScrnInfoPtr, pointer, Bool);
static int I830SetPortAttribute(ScrnInfoPtr, Atom, INT32, pointer);
static int I830GetPortAttribute(ScrnInfoPtr, Atom, INT32 *, pointer);
static void I830QueryBestSize(ScrnInfoPtr, Bool,
			      short, short, short, short, unsigned int *,
			      unsigned int *, pointer);
static int I830PutImage(ScrnInfoPtr, short, short, short, short, short, short,
			short, short, int, unsigned char *, short, short,
			Bool, RegionPtr, pointer);
static int I830QueryImageAttributes(ScrnInfoPtr, int, unsigned short *,
				    unsigned short *, int *, int *);

static void I830BlockHandler(int, pointer, pointer, pointer);

#define MAKE_ATOM(a) MakeAtom(a, sizeof(a) - 1, TRUE)

static Atom xvBrightness, xvContrast, xvColorKey, xvPipe;
static Atom xvGamma0, xvGamma1, xvGamma2, xvGamma3, xvGamma4, xvGamma5;

#define IMAGE_MAX_WIDTH		1440
#define IMAGE_MAX_HEIGHT	1080
#define Y_BUF_SIZE		(IMAGE_MAX_WIDTH * IMAGE_MAX_HEIGHT)

#if !VIDEO_DEBUG
#define ErrorF Edummy
static void
Edummy(const char *dummy, ...)
{
}
#endif

/*
 * This is more or less the correct way to initalise, update, and shut down
 * the overlay.  Note OVERLAY_OFF should be used only after disabling the
 * overlay in OCMD and calling OVERLAY_UPDATE.
 *
 * XXX Need to make sure that the overlay engine is cleanly shutdown in
 * all modes of server exit.
 */

#define OVERLAY_UPDATE							\
   do { 								\
      BEGIN_LP_RING(6);							\
      OUT_RING(MI_FLUSH | MI_WRITE_DIRTY_STATE);			\
      OUT_RING(MI_NOOP);						\
      if (!*pI830->overlayOn) {						\
	 OUT_RING(MI_NOOP);						\
	 OUT_RING(MI_NOOP);						\
	 OUT_RING(MI_OVERLAY_FLIP | MI_OVERLAY_FLIP_ON);		\
	 ErrorF("Overlay goes from off to on\n");			\
	 *pI830->overlayOn = TRUE;					\
      } else {								\
	 OUT_RING(MI_WAIT_FOR_EVENT | MI_WAIT_FOR_OVERLAY_FLIP);	\
	 OUT_RING(MI_NOOP);						\
	 OUT_RING(MI_OVERLAY_FLIP | MI_OVERLAY_FLIP_CONTINUE);		\
      }									\
      OUT_RING(pI830->OverlayMem->Physical | 1);			\
      ADVANCE_LP_RING();						\
      ErrorF("OVERLAY_UPDATE\n");					\
   } while(0)

#define OVERLAY_OFF							\
   do { 								\
      if (*pI830->overlayOn) {						\
	 BEGIN_LP_RING(8);						\
	 OUT_RING(MI_FLUSH | MI_WRITE_DIRTY_STATE);			\
	 OUT_RING(MI_NOOP);						\
	 OUT_RING(MI_WAIT_FOR_EVENT | MI_WAIT_FOR_OVERLAY_FLIP);	\
	 OUT_RING(MI_NOOP);						\
	 OUT_RING(MI_OVERLAY_FLIP | MI_OVERLAY_FLIP_OFF);		\
	 OUT_RING(pI830->OverlayMem->Physical);				\
	 OUT_RING(MI_WAIT_FOR_EVENT | MI_WAIT_FOR_OVERLAY_FLIP);	\
	 OUT_RING(MI_NOOP);						\
	 ADVANCE_LP_RING();						\
	 *pI830->overlayOn = FALSE;					\
	 ErrorF("Overlay goes from on to off\n");			\
	 ErrorF("OVERLAY_OFF\n");					\
      }									\
   } while(0)

/*
 * OCMD - Overlay Command Register
 */
#define MIRROR_MODE		(0x3<<17)
#define MIRROR_HORIZONTAL	(0x1<<17)
#define MIRROR_VERTICAL		(0x2<<17)
#define MIRROR_BOTH		(0x3<<17)
#define OV_BYTE_ORDER		(0x3<<14)
#define UV_SWAP			(0x1<<14)
#define Y_SWAP			(0x2<<14)
#define Y_AND_UV_SWAP		(0x3<<14)
#define SOURCE_FORMAT		(0xf<<10)
#define RGB_888			(0x1<<10)
#define	RGB_555			(0x2<<10)
#define	RGB_565			(0x3<<10)
#define	YUV_422			(0x8<<10)
#define	YUV_411			(0x9<<10)
#define	YUV_420			(0xc<<10)
#define	YUV_422_PLANAR		(0xd<<10)
#define	YUV_410			(0xe<<10)
#define TVSYNC_FLIP_PARITY	(0x1<<9)
#define TVSYNC_FLIP_ENABLE	(0x1<<7)
#define BUF_TYPE		(0x1<<5)
#define BUF_TYPE_FRAME		(0x0<<5)
#define BUF_TYPE_FIELD		(0x1<<5)
#define TEST_MODE		(0x1<<4)
#define BUFFER_SELECT		(0x3<<2)
#define BUFFER0			(0x0<<2)
#define BUFFER1			(0x1<<2)
#define FIELD_SELECT		(0x1<<1)
#define FIELD0			(0x0<<1)
#define FIELD1			(0x1<<1)
#define OVERLAY_ENABLE		0x1

/* OCONFIG register */
#define CC_OUT_8BIT		(0x1<<3)
#define OVERLAY_PIPE_MASK	(0x1<<18)		
#define OVERLAY_PIPE_A		(0x0<<18)		
#define OVERLAY_PIPE_B		(0x1<<18)		

/* DCLRKM register */
#define DEST_KEY_ENABLE		(0x1<<31)

/* Polyphase filter coefficients */
#define N_HORIZ_Y_TAPS		5
#define N_VERT_Y_TAPS		3
#define N_HORIZ_UV_TAPS		3
#define N_VERT_UV_TAPS		3
#define N_PHASES		17
#define MAX_TAPS		5

/* Filter cutoff frequency limits. */
#define MIN_CUTOFF_FREQ		1.0
#define MAX_CUTOFF_FREQ		3.0

#define RGB16ToColorKey(c) \
	(((c & 0xF800) << 8) | ((c & 0x07E0) << 5) | ((c & 0x001F) << 3))

#define RGB15ToColorKey(c) \
        (((c & 0x7c00) << 9) | ((c & 0x03E0) << 6) | ((c & 0x001F) << 3))

/* client libraries expect an encoding */
static XF86VideoEncodingRec DummyEncoding[1] = {
   {
      0,
      "XV_IMAGE",
      IMAGE_MAX_WIDTH, IMAGE_MAX_HEIGHT,
      {1, 1}
   }
};

#define NUM_FORMATS 3

static XF86VideoFormatRec Formats[NUM_FORMATS] = {
   {15, TrueColor}, {16, TrueColor}, {24, TrueColor}
};

#define CLONE_ATTRIBUTES 1
static XF86AttributeRec CloneAttributes[CLONE_ATTRIBUTES] = {
   {XvSettable | XvGettable, 0, 1, "XV_PIPE"}
};

#define NUM_ATTRIBUTES 3
static XF86AttributeRec Attributes[NUM_ATTRIBUTES] = {
   {XvSettable | XvGettable, 0, (1 << 24) - 1, "XV_COLORKEY"},
   {XvSettable | XvGettable, -128, 127, "XV_BRIGHTNESS"},
   {XvSettable | XvGettable, 0, 255, "XV_CONTRAST"}
};

#define GAMMA_ATTRIBUTES 6
static XF86AttributeRec GammaAttributes[GAMMA_ATTRIBUTES] = {
   {XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA0"},
   {XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA1"},
   {XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA2"},
   {XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA3"},
   {XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA4"},
   {XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA5"}
};

#define NUM_IMAGES 4

static XF86ImageRec Images[NUM_IMAGES] = {
   XVIMAGE_YUY2,
   XVIMAGE_YV12,
   XVIMAGE_I420,
   XVIMAGE_UYVY
};

typedef struct {
   CARD32 OBUF_0Y;
   CARD32 OBUF_1Y;
   CARD32 OBUF_0U;
   CARD32 OBUF_0V;
   CARD32 OBUF_1U;
   CARD32 OBUF_1V;
   CARD32 OSTRIDE;
   CARD32 YRGB_VPH;
   CARD32 UV_VPH;
   CARD32 HORZ_PH;
   CARD32 INIT_PHS;
   CARD32 DWINPOS;
   CARD32 DWINSZ;
   CARD32 SWIDTH;
   CARD32 SWIDTHSW;
   CARD32 SHEIGHT;
   CARD32 YRGBSCALE;
   CARD32 UVSCALE;
   CARD32 OCLRC0;
   CARD32 OCLRC1;
   CARD32 DCLRKV;
   CARD32 DCLRKM;
   CARD32 SCLRKVH;
   CARD32 SCLRKVL;
   CARD32 SCLRKEN;
   CARD32 OCONFIG;
   CARD32 OCMD;
   CARD32 RESERVED1;			/* 0x6C */
   CARD32 AWINPOS;
   CARD32 AWINSZ;
   CARD32 RESERVED2;			/* 0x78 */
   CARD32 RESERVED3;			/* 0x7C */
   CARD32 RESERVED4;			/* 0x80 */
   CARD32 RESERVED5;			/* 0x84 */
   CARD32 RESERVED6;			/* 0x88 */
   CARD32 RESERVED7;			/* 0x8C */
   CARD32 RESERVED8;			/* 0x90 */
   CARD32 RESERVED9;			/* 0x94 */
   CARD32 RESERVEDA;			/* 0x98 */
   CARD32 RESERVEDB;			/* 0x9C */
   CARD32 FASTHSCALE;			/* 0xA0 */
   CARD32 UVSCALEV;			/* 0xA4 */

   CARD32 RESERVEDC[(0x200 - 0xA8) / 4];		   /* 0xA8 - 0x1FC */
   CARD16 Y_VCOEFS[N_VERT_Y_TAPS * N_PHASES];		   /* 0x200 */
   CARD16 RESERVEDD[0x100 / 2 - N_VERT_Y_TAPS * N_PHASES];
   CARD16 Y_HCOEFS[N_HORIZ_Y_TAPS * N_PHASES];		   /* 0x300 */
   CARD16 RESERVEDE[0x200 / 2 - N_HORIZ_Y_TAPS * N_PHASES];
   CARD16 UV_VCOEFS[N_VERT_UV_TAPS * N_PHASES];		   /* 0x500 */
   CARD16 RESERVEDF[0x100 / 2 - N_VERT_UV_TAPS * N_PHASES];
   CARD16 UV_HCOEFS[N_HORIZ_UV_TAPS * N_PHASES];	   /* 0x600 */
   CARD16 RESERVEDG[0x100 / 2 - N_HORIZ_UV_TAPS * N_PHASES];
} I830OverlayRegRec, *I830OverlayRegPtr;

typedef struct {
   CARD32 YBuf0offset;
   CARD32 UBuf0offset;
   CARD32 VBuf0offset;

   CARD32 YBuf1offset;
   CARD32 UBuf1offset;
   CARD32 VBuf1offset;

   unsigned char currentBuf;

   int brightness;
   int contrast;
   int pipe;

   RegionRec clip;
   CARD32 colorKey;

   CARD32 gamma0;
   CARD32 gamma1;
   CARD32 gamma2;
   CARD32 gamma3;
   CARD32 gamma4;
   CARD32 gamma5;

   CARD32 videoStatus;
   Time offTime;
   Time freeTime;
   FBLinearPtr linear;

   Bool overlayOK;
   int oneLineMode;
   int scaleRatio;
} I830PortPrivRec, *I830PortPrivPtr;

#define GET_PORT_PRIVATE(pScrn) \
   (I830PortPrivPtr)((I830PTR(pScrn))->adaptor->pPortPrivates[0].ptr)

#if VIDEO_DEBUG
static void
CompareOverlay(I830Ptr pI830, CARD32 * overlay, int size)
{
   int i;
   CARD32 val;
   int bad = 0;

   for (i = 0; i < size; i += 4) {
      val = INREG(0x30100 + i);
      if (val != overlay[i / 4]) {
	 ErrorF("0x%05x value doesn't match (0x%08x != 0x%08x)\n",
		0x30100 + i, val, overlay[i / 4]);
	 bad++;
      }
   }
   if (!bad)
      ErrorF("CompareOverlay: no differences\n");
}
#endif

void
I830InitVideo(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   XF86VideoAdaptorPtr *adaptors, *newAdaptors = NULL;
   XF86VideoAdaptorPtr newAdaptor = NULL;
   int num_adaptors;

   DPRINTF(PFX, "I830InitVideo\n");

#if 0
   {
      I830OverlayRegRec tmp;

      ErrorF("sizeof I830OverlayRegRec is 0x%x\n", sizeof(I830OverlayRegRec));
      ErrorF("Reserved C, D, E, F, G are %x, %x, %x, %x, %x\n",
	     (unsigned long)&(tmp.RESERVEDC[0]) - (unsigned long)&tmp,
	     (unsigned long)&(tmp.RESERVEDD[0]) - (unsigned long)&tmp,
	     (unsigned long)&(tmp.RESERVEDE[0]) - (unsigned long)&tmp,
	     (unsigned long)&(tmp.RESERVEDF[0]) - (unsigned long)&tmp,
	     (unsigned long)&(tmp.RESERVEDG[0]) - (unsigned long)&tmp);
   }
#endif

   if (pScrn->bitsPerPixel != 8) {
      newAdaptor = I830SetupImageVideo(pScreen);
      I830InitOffscreenImages(pScreen);
   }

   num_adaptors = xf86XVListGenericAdaptors(pScrn, &adaptors);

   if (newAdaptor) {
      if (!num_adaptors) {
	 num_adaptors = 1;
	 adaptors = &newAdaptor;
      } else {
	 newAdaptors =			/* need to free this someplace */
	       xalloc((num_adaptors + 1) * sizeof(XF86VideoAdaptorPtr *));
	 if (newAdaptors) {
	    memcpy(newAdaptors, adaptors, num_adaptors *
		   sizeof(XF86VideoAdaptorPtr));
	    newAdaptors[num_adaptors] = newAdaptor;
	    adaptors = newAdaptors;
	    num_adaptors++;
	 }
      }
   }

   if (num_adaptors)
      xf86XVScreenInit(pScreen, adaptors, num_adaptors);

   if (newAdaptors)
      xfree(newAdaptors);
}

static void
I830ResetVideo(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   I830PortPrivPtr pPriv = pI830->adaptor->pPortPrivates[0].ptr;
   I830OverlayRegPtr overlay =
	 (I830OverlayRegPtr) (pI830->FbBase + pI830->OverlayMem->Start);

   DPRINTF(PFX, "I830ResetVideo: base: %p, offset: 0x%08x, obase: %p\n",
	   pI830->FbBase, pI830->OverlayMem->Start, overlay);
   /*
    * Default to maximum image size in YV12
    */

   memset(overlay, 0, sizeof(*overlay));
   overlay->YRGB_VPH = 0;
   overlay->UV_VPH = 0;
   overlay->HORZ_PH = 0;
   overlay->INIT_PHS = 0;
   overlay->DWINPOS = 0;
   overlay->DWINSZ = (IMAGE_MAX_HEIGHT << 16) | IMAGE_MAX_WIDTH;
   overlay->SWIDTH = IMAGE_MAX_WIDTH | (IMAGE_MAX_WIDTH << 16);
   overlay->SWIDTHSW = (IMAGE_MAX_WIDTH >> 3) | (IMAGE_MAX_WIDTH << 12);
   overlay->SHEIGHT = IMAGE_MAX_HEIGHT | (IMAGE_MAX_HEIGHT << 15);
   overlay->OCLRC0 = (pPriv->contrast << 18) | (pPriv->brightness & 0xff);
   overlay->OCLRC1 = 0x00000080;	/* saturation: bypass */
   overlay->AWINPOS = 0;
   overlay->AWINSZ = 0;
   overlay->FASTHSCALE = 0;

   /*
    * Enable destination color keying
    */
   switch (pScrn->depth) {
   case 8:
      overlay->DCLRKV = 0;
      overlay->DCLRKM = 0xffffff | DEST_KEY_ENABLE;
      break;
   case 15:
      overlay->DCLRKV = RGB15ToColorKey(pPriv->colorKey);
      overlay->DCLRKM = 0x070707 | DEST_KEY_ENABLE;
      break;
   case 16:
      overlay->DCLRKV = RGB16ToColorKey(pPriv->colorKey);
      overlay->DCLRKM = 0x070307 | DEST_KEY_ENABLE;
      break;
   default:
      overlay->DCLRKV = pPriv->colorKey;
      overlay->DCLRKM = DEST_KEY_ENABLE;
      break;
   }

   overlay->SCLRKVH = 0;
   overlay->SCLRKVL = 0;
   overlay->SCLRKEN = 0;		/* source color key disable */
   overlay->OCONFIG = CC_OUT_8BIT;

   /*
    * Select which pipe the overlay is enabled on.
    */
   overlay->OCONFIG &= ~OVERLAY_PIPE_MASK;
   if (pPriv->pipe == 0)
      overlay->OCONFIG |= OVERLAY_PIPE_A;
   else 
      overlay->OCONFIG |= OVERLAY_PIPE_B;

   overlay->OCMD = YUV_420;

#if 0
   /* 
    * XXX DUMP REGISTER CODE !!!
    * This allows us to dump the complete i845 registers and compare
    * with warm boot situations before we upload our first copy.
    */
   {
      int i;
      for (i = 0x30000; i < 0x31000; i += 4)
	 ErrorF("0x%x 0x%08x\n", i, INREG(i));
   }
#endif
}

#define PFIT_CONTROLS 0x61230
#define PFIT_AUTOVSCALE_MASK 0x200
#define PFIT_ON_MASK 0x80000000
#define PFIT_AUTOSCALE_RATIO 0x61238
#define PFIT_PROGRAMMED_SCALE_RATIO 0x61234

static void
I830SetOneLineModeRatio(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   I830PortPrivPtr pPriv = pI830->adaptor->pPortPrivates[0].ptr;
   CARD32 panelFitControl = INREG(PFIT_CONTROLS);
   int vertScale;

   pPriv->scaleRatio = 0x10000;

   if (panelFitControl & PFIT_ON_MASK) {
      if (panelFitControl & PFIT_AUTOVSCALE_MASK) {
         vertScale = INREG(PFIT_AUTOSCALE_RATIO) >> 16;
      } else {
         vertScale = INREG(PFIT_PROGRAMMED_SCALE_RATIO) >> 16;
      }

      if (vertScale != 0)
         pPriv->scaleRatio = ((double) 0x10000 / (double)vertScale) * 0x10000;
 
      pPriv->oneLineMode = TRUE;

      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Enabling Xvideo one-line mode\n");
   }

   if (pPriv->scaleRatio == 0x10000)
      pPriv->oneLineMode = FALSE;
}

static void
I830UpdateGamma(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   I830PortPrivPtr pPriv = pI830->adaptor->pPortPrivates[0].ptr;

   OUTREG(OGAMC5, pPriv->gamma5);
   OUTREG(OGAMC4, pPriv->gamma4);
   OUTREG(OGAMC3, pPriv->gamma3);
   OUTREG(OGAMC2, pPriv->gamma2);
   OUTREG(OGAMC1, pPriv->gamma1);
   OUTREG(OGAMC0, pPriv->gamma0);
}

static XF86VideoAdaptorPtr
I830SetupImageVideo(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   XF86VideoAdaptorPtr adapt;
   I830PortPrivPtr pPriv;
   XF86AttributePtr att;

   DPRINTF(PFX, "I830SetupImageVideo\n");

   if (!(adapt = xcalloc(1, sizeof(XF86VideoAdaptorRec) +
			 sizeof(I830PortPrivRec) + sizeof(DevUnion))))
      return NULL;

   adapt->type = XvWindowMask | XvInputMask | XvImageMask;
   adapt->flags = VIDEO_OVERLAID_IMAGES | VIDEO_CLIP_TO_VIEWPORT;
   adapt->name = "Intel(R) Video Overlay";
   adapt->nEncodings = 1;
   adapt->pEncodings = DummyEncoding;
   adapt->nFormats = NUM_FORMATS;
   adapt->pFormats = Formats;
   adapt->nPorts = 1;
   adapt->pPortPrivates = (DevUnion *) (&adapt[1]);

   pPriv = (I830PortPrivPtr) (&adapt->pPortPrivates[1]);

   adapt->pPortPrivates[0].ptr = (pointer) (pPriv);
   adapt->nAttributes = NUM_ATTRIBUTES;
   if (pI830->Clone)
      adapt->nAttributes += CLONE_ATTRIBUTES;
   if (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))
      adapt->nAttributes += GAMMA_ATTRIBUTES; /* has gamma */
   adapt->pAttributes = xnfalloc(sizeof(XF86AttributeRec) * adapt->nAttributes);
   /* Now copy the attributes */
   att = adapt->pAttributes;
   memcpy((char *)att, (char*)Attributes, sizeof(XF86AttributeRec)* NUM_ATTRIBUTES);
   att+=NUM_ATTRIBUTES;
   if (pI830->Clone) {
      memcpy((char*)att, (char*)CloneAttributes, sizeof(XF86AttributeRec) * CLONE_ATTRIBUTES);
      att+=CLONE_ATTRIBUTES;
   }
   if (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830)) {
      memcpy((char*)att, (char*)GammaAttributes, sizeof(XF86AttributeRec) * GAMMA_ATTRIBUTES);
      att+=GAMMA_ATTRIBUTES;
   }
   adapt->nImages = NUM_IMAGES;
   adapt->pImages = Images;
   adapt->PutVideo = NULL;
   adapt->PutStill = NULL;
   adapt->GetVideo = NULL;
   adapt->GetStill = NULL;
   adapt->StopVideo = I830StopVideo;
   adapt->SetPortAttribute = I830SetPortAttribute;
   adapt->GetPortAttribute = I830GetPortAttribute;
   adapt->QueryBestSize = I830QueryBestSize;
   adapt->PutImage = I830PutImage;
   adapt->QueryImageAttributes = I830QueryImageAttributes;

   pPriv->colorKey = pI830->colorKey & ((1 << pScrn->depth) - 1);
   pPriv->videoStatus = 0;
   pPriv->brightness = 0;
   pPriv->contrast = 64;
   pPriv->pipe = pI830->pipe; /* default to current pipe */
   pPriv->linear = NULL;
   pPriv->currentBuf = 0;
   pPriv->gamma5 = 0xc0c0c0;
   pPriv->gamma4 = 0x808080;
   pPriv->gamma3 = 0x404040;
   pPriv->gamma2 = 0x202020;
   pPriv->gamma1 = 0x101010;
   pPriv->gamma0 = 0x080808;

   /* gotta uninit this someplace */
   REGION_NULL(pScreen, &pPriv->clip);

   pI830->adaptor = adapt;

   /* With LFP's we need to detect whether we're in One Line Mode, which
    * essentially means a resolution greater than 1024x768, and fix up
    * the scaler accordingly. */
   pPriv->scaleRatio = 0x10000;
   pPriv->oneLineMode = FALSE;

   /*
    * Initialise pPriv->overlayOK.  Set it to TRUE here so that a warning will
    * be generated if I830VideoSwitchModeAfter() sets it to FALSE.
    */
   pPriv->overlayOK = TRUE;
   I830VideoSwitchModeAfter(pScrn, pScrn->currentMode);

   pI830->BlockHandler = pScreen->BlockHandler;
   pScreen->BlockHandler = I830BlockHandler;

   xvBrightness = MAKE_ATOM("XV_BRIGHTNESS");
   xvContrast = MAKE_ATOM("XV_CONTRAST");
   xvColorKey = MAKE_ATOM("XV_COLORKEY");

   /* Allow the pipe to be switched from pipe A to B when in clone mode */
   if (pI830->Clone)
     xvPipe = MAKE_ATOM("XV_PIPE");

   if (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830)) {
     xvGamma0 = MAKE_ATOM("XV_GAMMA0");
     xvGamma1 = MAKE_ATOM("XV_GAMMA1");
     xvGamma2 = MAKE_ATOM("XV_GAMMA2");
     xvGamma3 = MAKE_ATOM("XV_GAMMA3");
     xvGamma4 = MAKE_ATOM("XV_GAMMA4");
     xvGamma5 = MAKE_ATOM("XV_GAMMA5");
   }

   I830ResetVideo(pScrn);

   I830UpdateGamma(pScrn);

   return adapt;
}

static Bool
RegionsEqual(RegionPtr A, RegionPtr B)
{
   int *dataA, *dataB;
   int num;

   num = REGION_NUM_RECTS(A);
   if (num != REGION_NUM_RECTS(B))
      return FALSE;

   if ((A->extents.x1 != B->extents.x1) ||
       (A->extents.x2 != B->extents.x2) ||
       (A->extents.y1 != B->extents.y1) || (A->extents.y2 != B->extents.y2))
      return FALSE;

   dataA = (int *)REGION_RECTS(A);
   dataB = (int *)REGION_RECTS(B);

   while (num--) {
      if ((dataA[0] != dataB[0]) || (dataA[1] != dataB[1]))
	 return FALSE;
      dataA += 2;
      dataB += 2;
   }

   return TRUE;
}

static void
I830StopVideo(ScrnInfoPtr pScrn, pointer data, Bool shutdown)
{
   I830PortPrivPtr pPriv = (I830PortPrivPtr) data;
   I830Ptr pI830 = I830PTR(pScrn);

   I830OverlayRegPtr overlay =
	 (I830OverlayRegPtr) (pI830->FbBase + pI830->OverlayMem->Start);

   DPRINTF(PFX, "I830StopVideo\n");

   REGION_EMPTY(pScrn->pScreen, &pPriv->clip);

   if (shutdown) {
      if (pPriv->videoStatus & CLIENT_VIDEO_ON) {
	 overlay->OCMD &= ~OVERLAY_ENABLE;
	 OVERLAY_UPDATE;

	 OVERLAY_OFF;

         if (pI830->entityPrivate)
            pI830->entityPrivate->XvInUse = -1;
      }
      if (pPriv->linear) {
	 xf86FreeOffscreenLinear(pPriv->linear);
	 pPriv->linear = NULL;
      }
      pPriv->videoStatus = 0;
   } else {
      if (pPriv->videoStatus & CLIENT_VIDEO_ON) {
	 pPriv->videoStatus |= OFF_TIMER;
	 pPriv->offTime = currentTime.milliseconds + OFF_DELAY;
      }
   }

}

static int
I830SetPortAttribute(ScrnInfoPtr pScrn,
		     Atom attribute, INT32 value, pointer data)
{
   I830PortPrivPtr pPriv = (I830PortPrivPtr) data;
   I830Ptr pI830 = I830PTR(pScrn);
   I830OverlayRegPtr overlay =
	 (I830OverlayRegPtr) (pI830->FbBase + pI830->OverlayMem->Start);

   if (attribute == xvBrightness) {
      if ((value < -128) || (value > 127))
	 return BadValue;
      pPriv->brightness = value;
      overlay->OCLRC0 = (pPriv->contrast << 18) | (pPriv->brightness & 0xff);
      if (pPriv->overlayOK)
         OVERLAY_UPDATE;
   } else if (attribute == xvContrast) {
      if ((value < 0) || (value > 255))
	 return BadValue;
      pPriv->contrast = value;
      overlay->OCLRC0 = (pPriv->contrast << 18) | (pPriv->brightness & 0xff);
      if (pPriv->overlayOK)
         OVERLAY_UPDATE;
   } else if (pI830->Clone && attribute == xvPipe) {
      if ((value < 0) || (value > 1))
         return BadValue;
      pPriv->pipe = value;
      /*
       * Select which pipe the overlay is enabled on.
       */
      overlay->OCONFIG &= ~OVERLAY_PIPE_MASK;
      if (pPriv->pipe == 0)
         overlay->OCONFIG |= OVERLAY_PIPE_A;
      else 
         overlay->OCONFIG |= OVERLAY_PIPE_B;
      if (pPriv->overlayOK)
         OVERLAY_UPDATE;
   } else if (attribute == xvGamma0 && (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))) {
      /* Avoid video anomalies, so set gamma registers when overlay is off */
      /* We also clamp the values if they are outside the ranges */
      if (!*pI830->overlayOn) {
         pPriv->gamma0 = value;
	 if (pPriv->gamma1 - pPriv->gamma0 > 0x7d)
	   pPriv->gamma1 = pPriv->gamma0 + 0x7d;
      } else
         return BadRequest;
   } else if (attribute == xvGamma1 && (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))) {
      if (!*pI830->overlayOn) {
         pPriv->gamma1 = value;
         if (pPriv->gamma1 - pPriv->gamma0 > 0x7d)
           pPriv->gamma0 = pPriv->gamma1 - 0x7d;
      } else
         return BadRequest;
   } else if (attribute == xvGamma2 && (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))) {
      if (!*pI830->overlayOn) {
         pPriv->gamma2 = value;
         if (pPriv->gamma3 - pPriv->gamma2 > 0x7d)
            pPriv->gamma3 = pPriv->gamma2 + 0x7d;
      } else
         return BadRequest;
   } else if (attribute == xvGamma3 && (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))) {
      if (!*pI830->overlayOn) {
         pPriv->gamma3 = value;
         if (pPriv->gamma3 - pPriv->gamma2 > 0x7d)
            pPriv->gamma2 = pPriv->gamma3 - 0x7d;
      } else
         return BadRequest;
   } else if (attribute == xvGamma4 && (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))) {
      if (!*pI830->overlayOn) {
         pPriv->gamma4 = value;
         if (pPriv->gamma5 - pPriv->gamma4 > 0x7d)
            pPriv->gamma5 = pPriv->gamma4 + 0x7d;
      } else
         return BadRequest;
   } else if (attribute == xvGamma5 && (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))) {
      if (!*pI830->overlayOn) {
         pPriv->gamma5 = value;
         if (pPriv->gamma5 - pPriv->gamma4 > 0x7d)
            pPriv->gamma4 = pPriv->gamma5 - 0x7d;
      } else
         return BadRequest;
   } else if (attribute == xvColorKey) {
      pPriv->colorKey = value;
      switch (pScrn->depth) {
      case 16:
	 overlay->DCLRKV = RGB16ToColorKey(pPriv->colorKey);
	 break;
      case 15:
	 overlay->DCLRKV = RGB15ToColorKey(pPriv->colorKey);
	 break;
      default:
	 overlay->DCLRKV = pPriv->colorKey;
	 break;
      }
      if (pPriv->overlayOK)
         OVERLAY_UPDATE;
      REGION_EMPTY(pScrn->pScreen, &pPriv->clip);
   } else
      return BadMatch;

   /* We've already confirmed that the overlay is off, ready for updating */
   if ((attribute == xvGamma0 ||
        attribute == xvGamma1 ||
        attribute == xvGamma2 ||
        attribute == xvGamma3 ||
        attribute == xvGamma4 ||
        attribute == xvGamma5) && (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))) {
	I830UpdateGamma(pScrn);
   }

   return Success;
}

static int
I830GetPortAttribute(ScrnInfoPtr pScrn,
		     Atom attribute, INT32 * value, pointer data)
{
   I830Ptr pI830 = I830PTR(pScrn);
   I830PortPrivPtr pPriv = (I830PortPrivPtr) data;

   if (attribute == xvBrightness) {
      *value = pPriv->brightness;
   } else if (attribute == xvContrast) {
      *value = pPriv->contrast;
   } else if (pI830->Clone && attribute == xvPipe) {
      *value = pPriv->pipe;
   } else if (attribute == xvGamma0 && (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))) {
      *value = pPriv->gamma0;
   } else if (attribute == xvGamma1 && (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))) {
      *value = pPriv->gamma1;
   } else if (attribute == xvGamma2 && (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))) {
      *value = pPriv->gamma2;
   } else if (attribute == xvGamma3 && (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))) {
      *value = pPriv->gamma3;
   } else if (attribute == xvGamma4 && (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))) {
      *value = pPriv->gamma4;
   } else if (attribute == xvGamma5 && (IS_I915G(pI830) || IS_I915GM(pI830) || IS_I945G(pI830))) {
      *value = pPriv->gamma5;
   } else if (attribute == xvColorKey) {
      *value = pPriv->colorKey;
   } else
      return BadMatch;

   return Success;
}

static void
I830QueryBestSize(ScrnInfoPtr pScrn,
		  Bool motion,
		  short vid_w, short vid_h,
		  short drw_w, short drw_h,
		  unsigned int *p_w, unsigned int *p_h, pointer data)
{
   if (vid_w > (drw_w << 1))
      drw_w = vid_w >> 1;
   if (vid_h > (drw_h << 1))
      drw_h = vid_h >> 1;

   *p_w = drw_w;
   *p_h = drw_h;
}

static void
I830CopyPackedData(ScrnInfoPtr pScrn,
		   unsigned char *buf,
		   int srcPitch,
		   int dstPitch, int top, int left, int h, int w)
{
   I830Ptr pI830 = I830PTR(pScrn);
   I830PortPrivPtr pPriv = pI830->adaptor->pPortPrivates[0].ptr;
   unsigned char *src, *dst;

   DPRINTF(PFX, "I830CopyPackedData: (%d,%d) (%d,%d)\n"
	   "srcPitch: %d, dstPitch: %d\n", top, left, h, w, srcPitch, dstPitch);

   src = buf + (top * srcPitch) + (left << 1);

   if (pPriv->currentBuf == 0)
      dst = pI830->FbBase + pPriv->YBuf0offset;
   else
      dst = pI830->FbBase + pPriv->YBuf1offset;

   w <<= 1;
   while (h--) {
      memcpy(dst, src, w);
      src += srcPitch;
      dst += dstPitch;
   }
}

static void
I830CopyPlanarData(ScrnInfoPtr pScrn, unsigned char *buf, int srcPitch,
		   int srcPitch2, int dstPitch, int srcH, int top, int left,
		   int h, int w, int id)
{
   I830Ptr pI830 = I830PTR(pScrn);
   I830PortPrivPtr pPriv = pI830->adaptor->pPortPrivates[0].ptr;
   int i;
   unsigned char *src1, *src2, *src3, *dst1, *dst2, *dst3;

   ErrorF("I830CopyPlanarData: srcPitch %d, srcPitch %d, dstPitch %d\n"
	   "nlines %d, npixels %d, top %d, left %d\n", srcPitch, srcPitch2, dstPitch,
	   h, w, top, left);

   /* Copy Y data */
   src1 = buf + (top * srcPitch) + left;
   ErrorF("src1 is %p, offset is %d\n", src1,
	  (unsigned long)src1 - (unsigned long)buf);
   if (pPriv->currentBuf == 0)
      dst1 = pI830->FbBase + pPriv->YBuf0offset;
   else
      dst1 = pI830->FbBase + pPriv->YBuf1offset;

   for (i = 0; i < h; i++) {
      memcpy(dst1, src1, w);
      src1 += srcPitch;
      dst1 += dstPitch << 1;
   }

   /* Copy V data for YV12, or U data for I420 */
   src2 = buf + (srcH * srcPitch) + ((top * srcPitch) >> 2) + (left >> 1);
   ErrorF("src2 is %p, offset is %d\n", src2,
	  (unsigned long)src2 - (unsigned long)buf);
   if (pPriv->currentBuf == 0) {
      if (id == FOURCC_I420)
	 dst2 = pI830->FbBase + pPriv->UBuf0offset;
      else
	 dst2 = pI830->FbBase + pPriv->VBuf0offset;
   } else {
      if (id == FOURCC_I420)
	 dst2 = pI830->FbBase + pPriv->UBuf1offset;
      else
	 dst2 = pI830->FbBase + pPriv->VBuf1offset;
   }

   for (i = 0; i < h / 2; i++) {
      memcpy(dst2, src2, w / 2);
      src2 += srcPitch2;
      dst2 += dstPitch;
   }

   /* Copy U data for YV12, or V data for I420 */
   src3 = buf + (srcH * srcPitch) + ((srcH >> 1) * srcPitch2) +
	 ((top * srcPitch) >> 2) + (left >> 1);
   ErrorF("src3 is %p, offset is %d\n", src3,
	  (unsigned long)src3 - (unsigned long)buf);
   if (pPriv->currentBuf == 0) {
      if (id == FOURCC_I420)
	 dst3 = pI830->FbBase + pPriv->VBuf0offset;
      else
	 dst3 = pI830->FbBase + pPriv->UBuf0offset;
   } else {
      if (id == FOURCC_I420)
	 dst3 = pI830->FbBase + pPriv->VBuf1offset;
      else
	 dst3 = pI830->FbBase + pPriv->UBuf1offset;
   }

   for (i = 0; i < h / 2; i++) {
      memcpy(dst3, src3, w / 2);
      src3 += srcPitch2;
      dst3 += dstPitch;
   }
}

typedef struct {
   CARD8 sign;
   CARD16 mantissa;
   CARD8 exponent;
} coeffRec, *coeffPtr;

static Bool
SetCoeffRegs(double *coeff, int mantSize, coeffPtr pCoeff, int pos)
{
   int maxVal, icoeff, res;
   int sign;
   double c;

   sign = 0;
   maxVal = 1 << mantSize;
   c = *coeff;
   if (c < 0.0) {
      sign = 1;
      c = -c;
   }

   res = 12 - mantSize;
   if ((icoeff = (int)(c * 4 * maxVal + 0.5)) < maxVal) {
      pCoeff[pos].exponent = 3;
      pCoeff[pos].mantissa = icoeff << res;
      *coeff = (double)icoeff / (double)(4 * maxVal);
   } else if ((icoeff = (int)(c * 2 * maxVal + 0.5)) < maxVal) {
      pCoeff[pos].exponent = 2;
      pCoeff[pos].mantissa = icoeff << res;
      *coeff = (double)icoeff / (double)(2 * maxVal);
   } else if ((icoeff = (int)(c * maxVal + 0.5)) < maxVal) {
      pCoeff[pos].exponent = 1;
      pCoeff[pos].mantissa = icoeff << res;
      *coeff = (double)icoeff / (double)(maxVal);
   } else if ((icoeff = (int)(c * maxVal * 0.5 + 0.5)) < maxVal) {
      pCoeff[pos].exponent = 0;
      pCoeff[pos].mantissa = icoeff << res;
      *coeff = (double)icoeff / (double)(maxVal / 2);
   } else {
      /* Coeff out of range */
      return FALSE;
   }

   pCoeff[pos].sign = sign;
   if (sign)
      *coeff = -(*coeff);
   return TRUE;
}

static void
UpdateCoeff(int taps, double fCutoff, Bool isHoriz, Bool isY, coeffPtr pCoeff)
{
   int i, j, j1, num, pos, mantSize;
   double pi = 3.1415926535, val, sinc, window, sum;
   double rawCoeff[MAX_TAPS * 32], coeffs[N_PHASES][MAX_TAPS];
   double diff;
   int tapAdjust[MAX_TAPS], tap2Fix;
   Bool isVertAndUV;

   if (isHoriz)
      mantSize = 7;
   else
      mantSize = 6;

   isVertAndUV = !isHoriz && !isY;
   num = taps * 16;
   for (i = 0; i < num  * 2; i++) {
      val = (1.0 / fCutoff) * taps * pi * (i - num) / (2 * num);
      if (val == 0.0)
	 sinc = 1.0;
      else
	 sinc = sin(val) / val;

      /* Hamming window */
      window = (0.5 - 0.5 * cos(i * pi / num));
      rawCoeff[i] = sinc * window;
   }

   for (i = 0; i < N_PHASES; i++) {
      /* Normalise the coefficients. */
      sum = 0.0;
      for (j = 0; j < taps; j++) {
	 pos = i + j * 32;
	 sum += rawCoeff[pos];
      }
      for (j = 0; j < taps; j++) {
	 pos = i + j * 32;
	 coeffs[i][j] = rawCoeff[pos] / sum;
      }

      /* Set the register values. */
      for (j = 0; j < taps; j++) {
	 pos = j + i * taps;
	 if ((j == (taps - 1) / 2) && !isVertAndUV)
	    SetCoeffRegs(&coeffs[i][j], mantSize + 2, pCoeff, pos);
	 else
	    SetCoeffRegs(&coeffs[i][j], mantSize, pCoeff, pos);
      }

      tapAdjust[0] = (taps - 1) / 2;
      for (j = 1, j1 = 1; j <= tapAdjust[0]; j++, j1++) {
	 tapAdjust[j1] = tapAdjust[0] - j;
	 tapAdjust[++j1] = tapAdjust[0] + j;
      }

      /* Adjust the coefficients. */
      sum = 0.0;
      for (j = 0; j < taps; j++)
	 sum += coeffs[i][j];
      if (sum != 1.0) {
	 for (j1 = 0; j1 < taps; j1++) {
	    tap2Fix = tapAdjust[j1];
	    diff = 1.0 - sum;
	    coeffs[i][tap2Fix] += diff;
	    pos = tap2Fix + i * taps;
	    if ((tap2Fix == (taps - 1) / 2) && !isVertAndUV)
	       SetCoeffRegs(&coeffs[i][tap2Fix], mantSize + 2, pCoeff, pos);
	    else
	       SetCoeffRegs(&coeffs[i][tap2Fix], mantSize, pCoeff, pos);

	    sum = 0.0;
	    for (j = 0; j < taps; j++)
	       sum += coeffs[i][j];
	    if (sum == 1.0)
	       break;
	 }
      }
   }
}

static void
I830DisplayVideo(ScrnInfoPtr pScrn, int id, short width, short height,
		 int dstPitch, int x1, int y1, int x2, int y2, BoxPtr dstBox,
		 short src_w, short src_h, short drw_w, short drw_h)
{
   I830Ptr pI830 = I830PTR(pScrn);
   I830PortPrivPtr pPriv = pI830->adaptor->pPortPrivates[0].ptr;
   I830OverlayRegPtr overlay =
	 (I830OverlayRegPtr) (pI830->FbBase + pI830->OverlayMem->Start);
   unsigned int swidth;
   unsigned int mask, shift, offsety, offsetu;

   ErrorF("I830DisplayVideo: %dx%d (pitch %d)\n", width, height,
	   dstPitch);

   if (!pPriv->overlayOK)
      return;

   if (IS_I915G(pI830) || IS_I915GM(pI830)) {
      shift = 6;
      mask = 0x3f;
   } else {
      shift = 5;
      mask = 0x1f;
   }

   if (pPriv->currentBuf == 0) {
      offsety = pPriv->YBuf0offset;
      offsetu = pPriv->UBuf0offset;
   } else {
      offsety = pPriv->YBuf1offset;
      offsetu = pPriv->UBuf1offset;
   }

#if VIDEO_DEBUG
   CompareOverlay(pI830, (CARD32 *) overlay, 0x100);
#endif

   /* When in dual head with different bpp setups we need to refresh the
    * color key, so let's reset the video parameters and refresh here */
#if 0
   if (pI830->entityPrivate)
#endif
      I830ResetVideo(pScrn);

   switch (id) {
   case FOURCC_YV12:
   case FOURCC_I420:
      swidth = width;

      overlay->SWIDTH = swidth;
      swidth /= 2;
      overlay->SWIDTH |= (swidth & 0x7ff) << 16;

      swidth = ((offsety + width + mask) >> shift) -
	    (offsety >> shift);

      if (IS_I915G(pI830) || IS_I915GM(pI830))
         swidth <<= 1;

      swidth -= 1;

      ErrorF("Y width is %d, swidth is %d\n", width, swidth);

      overlay->SWIDTHSW = swidth << 2;

      swidth = ((offsetu + (width / 2) + mask) >> shift) -
	    (offsetu >> shift);

      if (IS_I915G(pI830) || IS_I915GM(pI830))
         swidth <<= 1;

      swidth -= 1;

      ErrorF("UV width is %d, swidthsw is %d\n", width / 2, swidth);

      overlay->SWIDTHSW |= swidth << 18;

      ErrorF("HEIGHT is %d\n",height);

      overlay->SHEIGHT = height | ((height / 2) << 16);
      break;
   case FOURCC_UYVY:
   case FOURCC_YUY2:
   default:
      swidth = width;
      overlay->SWIDTH = swidth;

      ErrorF("Y width is %d\n", swidth);

      swidth = ((offsety + (width << 1) + mask) >> shift) -
	    (offsety >> shift);

      if (IS_I915G(pI830) || IS_I915GM(pI830))
         swidth <<= 1;

      swidth -= 1;

      ErrorF("swidthsw is %d\n", swidth);

      overlay->SWIDTHSW = swidth << 2;

      ErrorF("HEIGHT is %d\n",height);

      overlay->SHEIGHT = height;
      break;
   }

   if (pPriv->oneLineMode) {
      /* change the coordinates with panel fitting active */
      dstBox->y1 = (((dstBox->y1 - 1) * pPriv->scaleRatio) >> 16) + 1;
      dstBox->y2 = ((dstBox->y2 * pPriv->scaleRatio) >> 16) + 1;
 
      /* Now, alter the height, so we scale to the correct size */
      drw_h = ((drw_h * pPriv->scaleRatio) >> 16) + 1;

      /* Keep the engine happy */
      if (dstBox->y1 < 0) dstBox->y1 = 0;
      if (dstBox->y2 < 0) dstBox->y2 = 0;
   }


   overlay->DWINPOS = (dstBox->y1 << 16) | dstBox->x1;

   overlay->DWINSZ = ((dstBox->y2 - dstBox->y1) << 16) |
	 (dstBox->x2 - dstBox->x1);

   ErrorF("dstBox: x1: %d, y1: %d, x2: %d, y2: %d\n", dstBox->x1, dstBox->y1,
			dstBox->x2, dstBox->y2);

   /* buffer locations */
   overlay->OBUF_0Y = pPriv->YBuf0offset;
   overlay->OBUF_0U = pPriv->UBuf0offset;
   overlay->OBUF_0V = pPriv->VBuf0offset;

   overlay->OBUF_1Y = pPriv->YBuf1offset;
   overlay->OBUF_1U = pPriv->UBuf1offset;
   overlay->OBUF_1V = pPriv->VBuf1offset;

   ErrorF("Buffers: Y0: 0x%08x, U0: 0x%08x, V0: 0x%08x\n", overlay->OBUF_0Y,
	  overlay->OBUF_0U, overlay->OBUF_0V);
   ErrorF("Buffers: Y1: 0x%08x, U1: 0x%08x, V1: 0x%08x\n", overlay->OBUF_1Y,
	  overlay->OBUF_1U, overlay->OBUF_1V);

#if 0
   {
      int i;

      ErrorF("First 32 bytes of Y data:\n");
      for (i = 0; i < 32; i++)
	 ErrorF(" %02x",
		((unsigned char *)pI830->FbBase + pPriv->YBuf0offset)[i]);
      ErrorF("\n");
      ErrorF("First 16 bytes of U data:\n");
      for (i = 0; i < 16; i++)
	 ErrorF(" %02x",
		((unsigned char *)pI830->FbBase + pPriv->UBuf0offset)[i]);
      ErrorF("\n");
      ErrorF("First 16 bytes of V data:\n");
      for (i = 0; i < 16; i++)
	 ErrorF(" %02x",
		((unsigned char *)pI830->FbBase + pPriv->VBuf0offset)[i]);
      ErrorF("\n");
   }
#endif

#if 1
   overlay->OCMD = OVERLAY_ENABLE;
#endif

   ErrorF("pos: 0x%08x, size: 0x%08x\n", overlay->DWINPOS, overlay->DWINSZ);
   ErrorF("dst: %d x %d, src: %d x %d\n", drw_w, drw_h, src_w, src_h);

   /* 
    * Calculate horizontal and vertical scaling factors and polyphase
    * coefficients.
    */

   {
      Bool scaleChanged = FALSE;
      int xscaleInt, xscaleFract, yscaleInt, yscaleFract;
      int xscaleIntUV, xscaleFractUV;
      int yscaleIntUV, yscaleFractUV;
      /* UV is half the size of Y -- YUV420 */
      int uvratio = 2;
      CARD32 newval;
      coeffRec xcoeffY[N_HORIZ_Y_TAPS * N_PHASES];
      coeffRec xcoeffUV[N_HORIZ_UV_TAPS * N_PHASES];
      int i, j, pos;

      /*
       * Y down-scale factor as a multiple of 4096.
       */
      xscaleFract = ((src_w - 1) << 12) / drw_w;
      yscaleFract = ((src_h - 1) << 12) / drw_h;

      /* Calculate the UV scaling factor. */
      xscaleFractUV = xscaleFract / uvratio;
      yscaleFractUV = yscaleFract / uvratio;

      /*
       * To keep the relative Y and UV ratios exact, round the Y scales
       * to a multiple of the Y/UV ratio.
       */
      xscaleFract = xscaleFractUV * uvratio;
      yscaleFract = yscaleFractUV * uvratio;

      /* Integer (un-multiplied) values. */
      xscaleInt = xscaleFract >> 12;
      yscaleInt = yscaleFract >> 12;

      xscaleIntUV = xscaleFractUV >> 12;
      yscaleIntUV = yscaleFractUV >> 12;

      ErrorF("xscale: %x.%03x, yscale: %x.%03x\n", xscaleInt,
	     xscaleFract & 0xFFF, yscaleInt, yscaleFract & 0xFFF);
      ErrorF("UV xscale: %x.%03x, UV yscale: %x.%03x\n", xscaleIntUV,
	     xscaleFractUV & 0xFFF, yscaleIntUV, yscaleFractUV & 0xFFF);

      newval = (xscaleInt << 16) |
	    ((xscaleFract & 0xFFF) << 3) | ((yscaleFract & 0xFFF) << 20);
      if (newval != overlay->YRGBSCALE) {
	 scaleChanged = TRUE;
	 overlay->YRGBSCALE = newval;
      }

      newval = (xscaleIntUV << 16) | ((xscaleFractUV & 0xFFF) << 3) |
	    ((yscaleFractUV & 0xFFF) << 20);
      if (newval != overlay->UVSCALE) {
	 scaleChanged = TRUE;
	 overlay->UVSCALE = newval;
      }

      newval = yscaleInt << 16 | yscaleIntUV;
      if (newval != overlay->UVSCALEV) {
	 scaleChanged = TRUE;
	 overlay->UVSCALEV = newval;
      }

      /* Recalculate coefficients if the scaling changed. */

      /*
       * Only Horizontal coefficients so far.
       */
      if (scaleChanged) {
	 double fCutoffY;
	 double fCutoffUV;

	 fCutoffY = xscaleFract / 4096.0;
	 fCutoffUV = xscaleFractUV / 4096.0;

	 /* Limit to between 1.0 and 3.0. */
	 if (fCutoffY < MIN_CUTOFF_FREQ)
	    fCutoffY = MIN_CUTOFF_FREQ;
	 if (fCutoffY > MAX_CUTOFF_FREQ)
	    fCutoffY = MAX_CUTOFF_FREQ;
	 if (fCutoffUV < MIN_CUTOFF_FREQ)
	    fCutoffUV = MIN_CUTOFF_FREQ;
	 if (fCutoffUV > MAX_CUTOFF_FREQ)
	    fCutoffUV = MAX_CUTOFF_FREQ;

	 UpdateCoeff(N_HORIZ_Y_TAPS, fCutoffY, TRUE, TRUE, xcoeffY);
	 UpdateCoeff(N_HORIZ_UV_TAPS, fCutoffUV, TRUE, FALSE, xcoeffUV);

	 for (i = 0; i < N_PHASES; i++) {
	    for (j = 0; j < N_HORIZ_Y_TAPS; j++) {
	       pos = i * N_HORIZ_Y_TAPS + j;
	       overlay->Y_HCOEFS[pos] = xcoeffY[pos].sign << 15 |
					xcoeffY[pos].exponent << 12 |
					xcoeffY[pos].mantissa;
	    }
	 }
	 for (i = 0; i < N_PHASES; i++) {
	    for (j = 0; j < N_HORIZ_UV_TAPS; j++) {
	       pos = i * N_HORIZ_UV_TAPS + j;
	       overlay->UV_HCOEFS[pos] = xcoeffUV[pos].sign << 15 |
					 xcoeffUV[pos].exponent << 12 |
					 xcoeffUV[pos].mantissa;
	    }
	 }
      }
   }

   switch (id) {
   case FOURCC_YV12:
   case FOURCC_I420:
      ErrorF("YUV420\n");
#if 0
      /* set UV vertical phase to -0.25 */
      overlay->UV_VPH = 0x30003000;
#endif
      ErrorF("UV stride is %d, Y stride is %d\n", dstPitch, dstPitch * 2);
      overlay->OSTRIDE = (dstPitch * 2) | (dstPitch << 16);
      overlay->OCMD &= ~SOURCE_FORMAT;
      overlay->OCMD &= ~OV_BYTE_ORDER;
      overlay->OCMD |= YUV_420;
      break;
   case FOURCC_UYVY:
   case FOURCC_YUY2:
   default:
      ErrorF("YUV422\n");
      overlay->OSTRIDE = dstPitch;
      overlay->OCMD &= ~SOURCE_FORMAT;
      overlay->OCMD |= YUV_422;
      overlay->OCMD &= ~OV_BYTE_ORDER;
      if (id == FOURCC_UYVY)
	 overlay->OCMD |= Y_SWAP;
      break;
   }

   overlay->OCMD &= ~(BUFFER_SELECT | FIELD_SELECT);
   if (pPriv->currentBuf == 0)
      overlay->OCMD |= BUFFER0;
   else
      overlay->OCMD |= BUFFER1;

   ErrorF("OCMD is 0x%08x\n", overlay->OCMD);

   OVERLAY_UPDATE;
}

static FBLinearPtr
I830AllocateMemory(ScrnInfoPtr pScrn, FBLinearPtr linear, int size)
{
   ScreenPtr pScreen;
   FBLinearPtr new_linear;

   DPRINTF(PFX, "I830AllocateMemory\n");

   if (linear) {
      if (linear->size >= size)
	 return linear;

      if (xf86ResizeOffscreenLinear(linear, size))
	 return linear;

      xf86FreeOffscreenLinear(linear);
   }

   pScreen = screenInfo.screens[pScrn->scrnIndex];

   new_linear = xf86AllocateOffscreenLinear(pScreen, size, 4,
					    NULL, NULL, NULL);

   if (!new_linear) {
      int max_size;

      xf86QueryLargestOffscreenLinear(pScreen, &max_size, 4,
				      PRIORITY_EXTREME);

      if (max_size < size)
	 return NULL;

      xf86PurgeUnlockedOffscreenAreas(pScreen);
      new_linear = xf86AllocateOffscreenLinear(pScreen, size, 4,
					       NULL, NULL, NULL);
   }

   return new_linear;
}

static int
I830PutImage(ScrnInfoPtr pScrn,
	     short src_x, short src_y,
	     short drw_x, short drw_y,
	     short src_w, short src_h,
	     short drw_w, short drw_h,
	     int id, unsigned char *buf,
	     short width, short height,
	     Bool sync, RegionPtr clipBoxes, pointer data)
{
   I830Ptr pI830 = I830PTR(pScrn);
   I830PortPrivPtr pPriv = (I830PortPrivPtr) data;
   ScreenPtr pScreen = screenInfo.screens[pScrn->scrnIndex];
   INT32 x1, x2, y1, y2;
   int srcPitch, srcPitch2 = 0, dstPitch;
   int top, left, npixels, nlines, size, loops;
   BoxRec dstBox;

   DPRINTF(PFX, "I830PutImage: src: (%d,%d)(%d,%d), dst: (%d,%d)(%d,%d)\n"
	   "width %d, height %d\n", src_x, src_y, src_w, src_h, drw_x, drw_y,
	   drw_w, drw_h, width, height);

   if (pI830->entityPrivate) {
	 if (pI830->entityPrivate->XvInUse != -1 &&
	     pI830->entityPrivate->XvInUse != pPriv->pipe) {
#ifdef PANORAMIX
		if (!noPanoramiXExtension) {
			return Success; /* faked for trying to share it */
		} else
#endif
		{
			return BadAlloc;
		}
	 }

      pI830->entityPrivate->XvInUse = pPriv->pipe;
   }

   /* overlay limits */
   if(src_w > (drw_w * 7))
      drw_w = src_w * 7;

   if(src_h > (drw_h * 7))
      drw_h = src_h * 7;

   /* Clip */
   x1 = src_x;
   x2 = src_x + src_w;
   y1 = src_y;
   y2 = src_y + src_h;

   dstBox.x1 = drw_x;
   dstBox.x2 = drw_x + drw_w;
   dstBox.y1 = drw_y;
   dstBox.y2 = drw_y + drw_h;

   if (!xf86XVClipVideoHelper(&dstBox, &x1, &x2, &y1, &y2, clipBoxes,
			      width, height))
      return Success;

   dstBox.x1 -= pScrn->frameX0;
   dstBox.x2 -= pScrn->frameX0;
   dstBox.y1 -= pScrn->frameY0;
   dstBox.y2 -= pScrn->frameY0;

   switch (id) {
   case FOURCC_YV12:
   case FOURCC_I420:
      srcPitch = (width + 3) & ~3;
      srcPitch2 = ((width >> 1) + 3) & ~3;
      dstPitch = ((width / 2) + 63) & ~63;	/* of chroma */
      size = dstPitch * height * 3;
      break;
   case FOURCC_UYVY:
   case FOURCC_YUY2:
   default:
      srcPitch = width << 1;
      dstPitch = (srcPitch + 63) & ~63;	/* of chroma */
      size = dstPitch * height;
      break;
   }
   ErrorF("srcPitch: %d, dstPitch: %d, size: %d\n", srcPitch, dstPitch, size);

   /* size is multiplied by 2 because we have two buffers that are flipping */
   if (!(pPriv->linear = I830AllocateMemory(pScrn, pPriv->linear, size * 2 / pI830->cpp)))
      return BadAlloc;

   /* fixup pointers */
   pPriv->YBuf0offset = pScrn->fbOffset + pPriv->linear->offset * pI830->cpp;
   pPriv->UBuf0offset = pPriv->YBuf0offset + (dstPitch * 2 * height);
   pPriv->VBuf0offset = pPriv->UBuf0offset + (dstPitch * height / 2);

   pPriv->YBuf1offset = pPriv->YBuf0offset + size;
   pPriv->UBuf1offset = pPriv->YBuf1offset + (dstPitch * 2 * height);
   pPriv->VBuf1offset = pPriv->UBuf1offset + (dstPitch * height / 2);

   /* XXX We could potentially use MI_WAIT_FOR_OVERLAY here instead
    * of this code....*/

   /* Make sure this buffer isn't in use */
   loops = 0;
   while (loops < 1000000) {
#if USE_USLEEP_FOR_VIDEO
      usleep(10);
#endif
      if (((INREG(DOVSTA) & OC_BUF) >> 20) == pPriv->currentBuf) {
	 break;
      }
      loops++;
   }
   if (loops >= 1000000) {
      ErrorF("loops (1) maxed out for buffer %d\n", pPriv->currentBuf);
#if 0
      pPriv->currentBuf = !pPriv->currentBuf;
#endif
   }

   /* buffer swap */
   if (pPriv->currentBuf == 0)
      pPriv->currentBuf = 1;
   else
      pPriv->currentBuf = 0;

   /* copy data */
   top = y1 >> 16;
   left = (x1 >> 16) & ~1;
   npixels = ((((x2 + 0xffff) >> 16) + 1) & ~1) - left;

   switch (id) {
   case FOURCC_YV12:
   case FOURCC_I420:
      top &= ~1;
      nlines = ((((y2 + 0xffff) >> 16) + 1) & ~1) - top;
      I830CopyPlanarData(pScrn, buf, srcPitch, srcPitch2, dstPitch, height, top, left,
			 nlines, npixels, id);
      break;
   case FOURCC_UYVY:
   case FOURCC_YUY2:
   default:
      nlines = ((y2 + 0xffff) >> 16) - top;
      I830CopyPackedData(pScrn, buf, srcPitch, dstPitch, top, left, nlines,
			 npixels);
      break;
   }

   /* update cliplist */
   if (!RegionsEqual(&pPriv->clip, clipBoxes)) {
      REGION_COPY(pScrn->pScreen, &pPriv->clip, clipBoxes);
      xf86XVFillKeyHelper(pScreen, pPriv->colorKey, clipBoxes);
   }

   I830DisplayVideo(pScrn, id, width, height, dstPitch,
		    x1, y1, x2, y2, &dstBox, src_w, src_h, drw_w, drw_h);

   pPriv->videoStatus = CLIENT_VIDEO_ON;

   return Success;
}

static int
I830QueryImageAttributes(ScrnInfoPtr pScrn,
			 int id,
			 unsigned short *w, unsigned short *h,
			 int *pitches, int *offsets)
{
   int size, tmp;

   DPRINTF(PFX, "I830QueryImageAttributes: w is %d, h is %d\n", *w, *h);

   if (*w > IMAGE_MAX_WIDTH)
      *w = IMAGE_MAX_WIDTH;
   if (*h > IMAGE_MAX_HEIGHT)
      *h = IMAGE_MAX_HEIGHT;

   *w = (*w + 1) & ~1;
   if (offsets)
      offsets[0] = 0;

   switch (id) {
      /* IA44 is for XvMC only */
   case FOURCC_IA44:
   case FOURCC_AI44:
      if (pitches)
	 pitches[0] = *w;
      size = *w * *h;
      break;
   case FOURCC_YV12:
   case FOURCC_I420:
      *h = (*h + 1) & ~1;
      size = (*w + 3) & ~3;
      if (pitches)
	 pitches[0] = size;
      size *= *h;
      if (offsets)
	 offsets[1] = size;
      tmp = ((*w >> 1) + 3) & ~3;
      if (pitches)
	 pitches[1] = pitches[2] = tmp;
      tmp *= (*h >> 1);
      size += tmp;
      if (offsets)
	 offsets[2] = size;
      size += tmp;
      if (pitches)
	 ErrorF("pitch 0 is %d, pitch 1 is %d, pitch 2 is %d\n", pitches[0],
		pitches[1], pitches[2]);
      if (offsets)
	 ErrorF("offset 1 is %d, offset 2 is %d\n", offsets[1], offsets[2]);
      if (offsets)
	 ErrorF("size is %d\n", size);
      break;
   case FOURCC_UYVY:
   case FOURCC_YUY2:
   default:
      size = *w << 1;
      if (pitches)
	 pitches[0] = size;
      size *= *h;
      break;
   }

   return size;
}

static void
I830BlockHandler(int i,
		 pointer blockData, pointer pTimeout, pointer pReadmask)
{
   ScreenPtr pScreen = screenInfo.screens[i];
   ScrnInfoPtr pScrn = xf86Screens[i];
   I830Ptr pI830 = I830PTR(pScrn);
   I830PortPrivPtr pPriv = GET_PORT_PRIVATE(pScrn);
   I830OverlayRegPtr overlay =
	 (I830OverlayRegPtr) (pI830->FbBase + pI830->OverlayMem->Start);

   pScreen->BlockHandler = pI830->BlockHandler;

   (*pScreen->BlockHandler) (i, blockData, pTimeout, pReadmask);

   pScreen->BlockHandler = I830BlockHandler;

   if (pPriv->videoStatus & TIMER_MASK) {
      UpdateCurrentTime();
      if (pPriv->videoStatus & OFF_TIMER) {
	 if (pPriv->offTime < currentTime.milliseconds) {
	    /* Turn off the overlay */
	    overlay->OCMD &= ~OVERLAY_ENABLE;

	    OVERLAY_UPDATE;

	    OVERLAY_OFF;

	    pPriv->videoStatus = FREE_TIMER;
	    pPriv->freeTime = currentTime.milliseconds + FREE_DELAY;
       
            if (pI830->entityPrivate)
               pI830->entityPrivate->XvInUse = -1;
	 }
      } else {				/* FREE_TIMER */
	 if (pPriv->freeTime < currentTime.milliseconds) {
	    if (pPriv->linear) {
	       xf86FreeOffscreenLinear(pPriv->linear);
	       pPriv->linear = NULL;
	    }
	    pPriv->videoStatus = 0;
	 }
      }
   }
}

/***************************************************************************
 * Offscreen Images
 ***************************************************************************/

typedef struct {
   FBLinearPtr linear;
   Bool isOn;
} OffscreenPrivRec, *OffscreenPrivPtr;

static int
I830AllocateSurface(ScrnInfoPtr pScrn,
		    int id,
		    unsigned short w,
		    unsigned short h, XF86SurfacePtr surface)
{
   FBLinearPtr linear;
   int pitch, fbpitch, size, bpp;
   OffscreenPrivPtr pPriv;
   I830Ptr pI830 = I830PTR(pScrn);

   DPRINTF(PFX, "I830AllocateSurface\n");

   if ((w > 1024) || (h > 1024))
      return BadAlloc;

   w = (w + 1) & ~1;
   pitch = ((w << 1) + 15) & ~15;
   bpp = pScrn->bitsPerPixel >> 3;
   fbpitch = bpp * pScrn->displayWidth;
   size = ((pitch * h) + bpp - 1) / bpp;

   if (!(linear = I830AllocateMemory(pScrn, NULL, size)))
      return BadAlloc;

   surface->width = w;
   surface->height = h;

   if (!(surface->pitches = xalloc(sizeof(int)))) {
      xf86FreeOffscreenLinear(linear);
      return BadAlloc;
   }
   if (!(surface->offsets = xalloc(sizeof(int)))) {
      xfree(surface->pitches);
      xf86FreeOffscreenLinear(linear);
      return BadAlloc;
   }
   if (!(pPriv = xalloc(sizeof(OffscreenPrivRec)))) {
      xfree(surface->pitches);
      xfree(surface->offsets);
      xf86FreeOffscreenLinear(linear);
      return BadAlloc;
   }

   pPriv->linear = linear;
   pPriv->isOn = FALSE;

   surface->pScrn = pScrn;
   surface->id = id;
   surface->pitches[0] = pitch;
   surface->offsets[0] = linear->offset * bpp;
   surface->devPrivate.ptr = (pointer) pPriv;

   memset(pI830->FbBase + pScrn->fbOffset + surface->offsets[0], 0, size);

   return Success;
}

static int
I830StopSurface(XF86SurfacePtr surface)
{
   OffscreenPrivPtr pPriv = (OffscreenPrivPtr) surface->devPrivate.ptr;
   ScrnInfoPtr pScrn = surface->pScrn;

   if (pPriv->isOn) {
      I830Ptr pI830 = I830PTR(pScrn);

      I830OverlayRegPtr overlay =
	    (I830OverlayRegPtr) (pI830->FbBase + pI830->OverlayMem->Start);

      overlay->OCMD &= ~OVERLAY_ENABLE;

      OVERLAY_UPDATE;

      OVERLAY_OFF;

      if (pI830->entityPrivate)
         pI830->entityPrivate->XvInUse = -1;

      pPriv->isOn = FALSE;
   }

   return Success;
}

static int
I830FreeSurface(XF86SurfacePtr surface)
{
   OffscreenPrivPtr pPriv = (OffscreenPrivPtr) surface->devPrivate.ptr;

   if (pPriv->isOn) {
      I830StopSurface(surface);
   }
   xf86FreeOffscreenLinear(pPriv->linear);
   xfree(surface->pitches);
   xfree(surface->offsets);
   xfree(surface->devPrivate.ptr);

   return Success;
}

static int
I830GetSurfaceAttribute(ScrnInfoPtr pScrn, Atom attribute, INT32 * value)
{
   return I830GetPortAttribute(pScrn, attribute, value, 0);
}

static int
I830SetSurfaceAttribute(ScrnInfoPtr pScrn, Atom attribute, INT32 value)
{
   return I830SetPortAttribute(pScrn, attribute, value, 0);
}

static int
I830DisplaySurface(XF86SurfacePtr surface,
		   short src_x, short src_y,
		   short drw_x, short drw_y,
		   short src_w, short src_h,
		   short drw_w, short drw_h, RegionPtr clipBoxes)
{
   OffscreenPrivPtr pPriv = (OffscreenPrivPtr) surface->devPrivate.ptr;
   ScrnInfoPtr pScrn = surface->pScrn;
   ScreenPtr pScreen = screenInfo.screens[pScrn->scrnIndex];
   I830Ptr pI830 = I830PTR(pScrn);
   I830PortPrivPtr pI830Priv = GET_PORT_PRIVATE(pScrn);
   INT32 x1, y1, x2, y2;
   INT32 loops = 0;
   BoxRec dstBox;

   DPRINTF(PFX, "I830DisplaySurface\n");

   if (pI830->entityPrivate) {
	 if (pI830->entityPrivate->XvInUse != -1 &&
	     pI830->entityPrivate->XvInUse != pI830Priv->pipe) {
#ifdef PANORAMIX
		if (!noPanoramiXExtension) {
			return Success; /* faked for trying to share it */
		} else
#endif
		{
			return BadAlloc;
		}
	 }

      pI830->entityPrivate->XvInUse = pI830Priv->pipe;
   }

   x1 = src_x;
   x2 = src_x + src_w;
   y1 = src_y;
   y2 = src_y + src_h;

   dstBox.x1 = drw_x;
   dstBox.x2 = drw_x + drw_w;
   dstBox.y1 = drw_y;
   dstBox.y2 = drw_y + drw_h;

   if (!xf86XVClipVideoHelper(&dstBox, &x1, &x2, &y1, &y2, clipBoxes,
			      surface->width, surface->height))
      return Success;

   dstBox.x1 -= pScrn->frameX0;
   dstBox.x2 -= pScrn->frameX0;
   dstBox.y1 -= pScrn->frameY0;
   dstBox.y2 -= pScrn->frameY0;

   /* fixup pointers */
   pI830Priv->YBuf0offset = surface->offsets[0];
   pI830Priv->YBuf1offset = pI830Priv->YBuf0offset;

   /* XXX We could potentially use MI_WAIT_FOR_OVERLAY here instead
    * of this code....*/

   /* wait for the last rendered buffer to be flipped in */
   while (((INREG(DOVSTA) & OC_BUF) >> 20) != pI830Priv->currentBuf) {
#if USE_USLEEP_FOR_VIDEO
      usleep(10);
#endif
      if (loops == 200000) {
	 xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Overlay Lockup\n");
	 break;
      }
      loops++;
   }

   /* buffer swap */
   if (pI830Priv->currentBuf == 0)
      pI830Priv->currentBuf = 1;
   else
      pI830Priv->currentBuf = 0;

   I830DisplayVideo(pScrn, surface->id, surface->width, surface->height,
		    surface->pitches[0], x1, y1, x2, y2, &dstBox,
		    src_w, src_h, drw_w, drw_h);

   xf86XVFillKeyHelper(pScreen, pI830Priv->colorKey, clipBoxes);

   pPriv->isOn = TRUE;
   /* we've prempted the XvImage stream so set its free timer */
   if (pI830Priv->videoStatus & CLIENT_VIDEO_ON) {
      REGION_EMPTY(pScrn->pScreen, &pI830Priv->clip);
      UpdateCurrentTime();
      pI830Priv->videoStatus = FREE_TIMER;
      pI830Priv->freeTime = currentTime.milliseconds + FREE_DELAY;
      pScrn->pScreen->BlockHandler = I830BlockHandler;
   }

   return Success;
}

static void
I830InitOffscreenImages(ScreenPtr pScreen)
{
   XF86OffscreenImagePtr offscreenImages;

   /* need to free this someplace */
   if (!(offscreenImages = xalloc(sizeof(XF86OffscreenImageRec)))) {
      return;
   }

   offscreenImages[0].image = &Images[0];
   offscreenImages[0].flags = VIDEO_OVERLAID_IMAGES | VIDEO_CLIP_TO_VIEWPORT;
   offscreenImages[0].alloc_surface = I830AllocateSurface;
   offscreenImages[0].free_surface = I830FreeSurface;
   offscreenImages[0].display = I830DisplaySurface;
   offscreenImages[0].stop = I830StopSurface;
   offscreenImages[0].setAttribute = I830SetSurfaceAttribute;
   offscreenImages[0].getAttribute = I830GetSurfaceAttribute;
   offscreenImages[0].max_width = 1024;
   offscreenImages[0].max_height = 1024;
   offscreenImages[0].num_attributes = 1;
   offscreenImages[0].attributes = Attributes;

   xf86XVRegisterOffscreenImages(pScreen, offscreenImages, 1);
}

void
I830VideoSwitchModeBefore(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
   I830PortPrivPtr pPriv;

   if (!I830PTR(pScrn)->adaptor) {
      return;
   }

   pPriv = GET_PORT_PRIVATE(pScrn);

   if (!pPriv) {
      xf86ErrorF("pPriv isn't set\n");
      return;
   }

   /* We stop the video when mode switching, just so we don't lockup
    * the engine. The overlayOK will determine whether we can re-enable
    * with the current video on completion of the mode switch.
    */
   I830StopVideo(pScrn, pPriv, TRUE);

   pPriv->oneLineMode = FALSE;
}

void
I830VideoSwitchModeAfter(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
   I830Ptr pI830 = I830PTR(pScrn);
   I830PortPrivPtr pPriv;
   int size, hsize, vsize, active;

   if (!pI830->adaptor) {
      return;
   }
   pPriv = GET_PORT_PRIVATE(pScrn);
   if (!pPriv)
      return;

   if (pPriv->pipe == 0) {
      if (INREG(PIPEACONF) & PIPEACONF_DOUBLE_WIDE) {
         xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	   "Disabling XVideo output because Pipe A is in double-wide mode.\n");
         pPriv->overlayOK = FALSE;
      } else if (!pPriv->overlayOK) {
         xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	   "Re-enabling XVideo output because Pipe A is now in single-wide mode.\n");
         pPriv->overlayOK = TRUE;
      }
   }

   if (pPriv->pipe == 1) {
      if (INREG(PIPEBCONF) & PIPEBCONF_DOUBLE_WIDE) {
         xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	   "Disabling XVideo output because Pipe B is in double-wide mode.\n");
         pPriv->overlayOK = FALSE;
      } else if (!pPriv->overlayOK) {
         xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	   "Re-enabling XVideo output because Pipe B is now in single-wide mode.\n");
         pPriv->overlayOK = TRUE;
      }
   }

   /* Check we have an LFP connected */
   if ((pPriv->pipe == 1 && pI830->operatingDevices & (PIPE_LFP << 8)) ||
       (pPriv->pipe == 0 && pI830->operatingDevices & PIPE_LFP) ) {
      size = pI830->pipe ? INREG(PIPEBSRC) : INREG(PIPEASRC);
      hsize = (size >> 16) & 0x7FF;
      vsize = size & 0x7FF;
      active = pI830->pipe ? (INREG(VTOTAL_B) & 0x7FF) : (INREG(VTOTAL_A) & 0x7FF);

      if (vsize < active && hsize > 1024)
         I830SetOneLineModeRatio(pScrn);
   
      if (pPriv->scaleRatio & 0xFFFE0000) {
         /* Possible bogus ratio, using in-accurate fallback */
         xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	    "Bogus panel fit register, Xvideo positioning may not be accurate.\n");
         xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	    "Using fallback ratio - was 0x%x, now 0x%x\n", pPriv->scaleRatio, (int)(((float)active * 65536)/(float)vsize));
   
   
         pPriv->scaleRatio = (int)(((float)active * 65536) / (float)vsize);
      }
   }
}
