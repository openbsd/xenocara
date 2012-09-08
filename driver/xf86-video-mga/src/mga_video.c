#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "compiler.h"
#include "xf86Pci.h"
#include "xf86fbman.h"
#include "regionstr.h"

#include "mga_reg.h"
#include "mga.h"
#include "mga_macros.h"
#include "xf86xv.h"
#include <X11/extensions/Xv.h>

#ifdef USE_XAA
#include "xaa.h"
#include "xaalocal.h"
#endif

#include "dixstruct.h"
#include "fourcc.h"

#define OFF_DELAY 	250  /* milliseconds */
#define FREE_DELAY 	15000

#define OFF_TIMER 	0x01
#define FREE_TIMER	0x02
#define CLIENT_VIDEO_ON	0x04

#define TIMER_MASK      (OFF_TIMER | FREE_TIMER)

#define MGA_MAX_PORTS	32

static void MGAInitOffscreenImages(ScreenPtr);

static XF86VideoAdaptorPtr MGASetupImageVideoOverlay(ScreenPtr);
static int  MGASetPortAttributeOverlay(ScrnInfoPtr, Atom, INT32, pointer);
static int  MGAGetPortAttributeOverlay(ScrnInfoPtr, Atom ,INT32 *, pointer);

static XF86VideoAdaptorPtr MGASetupImageVideoTexture(ScreenPtr);
static int  MGASetPortAttributeTexture(ScrnInfoPtr, Atom, INT32, pointer);
static int  MGAGetPortAttributeTexture(ScrnInfoPtr, Atom ,INT32 *, pointer);

static void MGAStopVideo(ScrnInfoPtr, pointer, Bool);
static void MGAQueryBestSize(ScrnInfoPtr, Bool, short, short, short, short, 
			unsigned int *, unsigned int *, pointer);
static int  MGAPutImage(ScrnInfoPtr, short, short, short, short, short, 
			short, short, short, int, unsigned char*, short, 
			short, Bool, RegionPtr, pointer, DrawablePtr);
static int  MGAQueryImageAttributes(ScrnInfoPtr, int, unsigned short *, 
			unsigned short *,  int *, int *);
static void MGAFreeMemory(ScrnInfoPtr pScrn, void *mem_struct);

static void MGAResetVideoOverlay(ScrnInfoPtr);

static void MGAVideoTimerCallback(ScrnInfoPtr pScrn, Time time);

static XF86VideoAdaptorPtr MGASetupImageVideoILOAD(ScreenPtr);
static int MGAPutImageILOAD(ScrnInfoPtr, short, short, short, short, short, 
			    short, short, short, int, unsigned char*, short,
			    short, Bool, RegionPtr, pointer, DrawablePtr);

#define MAKE_ATOM(a) MakeAtom(a, sizeof(a) - 1, TRUE)

static Atom xvBrightness, xvContrast, xvColorKey, xvDoubleBuffer;

#ifdef USE_EXA
static void
MGAVideoSave(ScreenPtr pScreen, ExaOffscreenArea *area)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    MGAPtr pMga = MGAPTR(pScrn);
    MGAPortPrivPtr pPriv = pMga->portPrivate;

    if (pPriv->video_memory == area)
        pPriv->video_memory = NULL;
}
#endif /* USE_EXA */

void MGAInitVideo(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    XF86VideoAdaptorPtr *adaptors, *newAdaptors = NULL;
    XF86VideoAdaptorPtr newAdaptor = NULL;
    MGAPtr pMga = MGAPTR(pScrn);
    int num_adaptors;

    if ((pScrn->bitsPerPixel != 8) && !pMga->NoAccel &&
	(pMga->SecondCrtc == FALSE) &&
	((pMga->Chipset == PCI_CHIP_MGA2164) ||
	 (pMga->Chipset == PCI_CHIP_MGA2164_AGP) ||     
/*	 (pMga->Chipset == PCI_CHIP_MGA2064) ||     */
	 (pMga->Chipset == PCI_CHIP_MGAG200) ||     
	 (pMga->Chipset == PCI_CHIP_MGAG200_PCI) ||
	 (pMga->Chipset == PCI_CHIP_MGAG400) ||
	 (pMga->Chipset == PCI_CHIP_MGAG550))) {
	if ((pMga->Chipset == PCI_CHIP_MGA2164) ||
/*	    (pMga->Chipset == PCI_CHIP_MGA2064) ||   */  
	    (pMga->Chipset == PCI_CHIP_MGA2164_AGP)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using MGA 2164W ILOAD video\n");
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
		       "This is an experimental driver and may not work on your machine.\n");

	    newAdaptor = MGASetupImageVideoILOAD(pScreen);
	    pMga->TexturedVideo = TRUE; 
	    /* ^^^ this is not really true but the ILOAD scaler shares 
	     * much more code with the textured video than the overlay 
	     */
	} else if (pMga->TexturedVideo && (pScrn->bitsPerPixel != 24)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using texture video\n");
	    newAdaptor = MGASetupImageVideoTexture(pScreen);
	    pMga->TexturedVideo = TRUE;
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using overlay video\n");
	    newAdaptor = MGASetupImageVideoOverlay(pScreen);
	    pMga->TexturedVideo = FALSE;
	}

	MGAInitOffscreenImages(pScreen);
    }

    num_adaptors = xf86XVListGenericAdaptors(pScrn, &adaptors);

    if(newAdaptor) {
	if(!num_adaptors) {
	    num_adaptors = 1;
	    adaptors = &newAdaptor;
	} else {
	    /* need to free this someplace */
	    newAdaptors = malloc((num_adaptors + 1) * sizeof(XF86VideoAdaptorPtr *));
	    if(newAdaptors) {
		memcpy(newAdaptors, adaptors, num_adaptors * 
					sizeof(XF86VideoAdaptorPtr));
		newAdaptors[num_adaptors] = newAdaptor;
		adaptors = newAdaptors;
		num_adaptors++;
	    }
	}
    }

    if(num_adaptors)
        xf86XVScreenInit(pScreen, adaptors, num_adaptors);

    free(newAdaptors);
}

/* client libraries expect an encoding */
static XF86VideoEncodingRec DummyEncoding[2] =
{
 {   /* overlay limit */
   0,
   "XV_IMAGE",
   1024, 1024,
   {1, 1}
 },
 {  /* texture limit */
   0,
   "XV_IMAGE",
   2046, 2046,
   {1, 1}
 }
};

#define NUM_FORMATS 6

static XF86VideoFormatRec Formats[NUM_FORMATS] = 
{
   {15, TrueColor}, {16, TrueColor}, {24, TrueColor},
   {15, DirectColor}, {16, DirectColor}, {24, DirectColor}
};

#define NUM_ATTRIBUTES_OVERLAY 4

static XF86AttributeRec Attributes[NUM_ATTRIBUTES_OVERLAY] =
{
   {XvSettable | XvGettable, 0, (1 << 24) - 1, "XV_COLORKEY"},
   {XvSettable | XvGettable, -128, 127, "XV_BRIGHTNESS"},
   {XvSettable | XvGettable, 0, 255, "XV_CONTRAST"},
   {XvSettable | XvGettable, 0, 1, "XV_DOUBLE_BUFFER"}
};

#define NUM_IMAGES 4

static XF86ImageRec Images[NUM_IMAGES] =
{
	XVIMAGE_YUY2,
	XVIMAGE_YV12,
	XVIMAGE_I420,
	XVIMAGE_UYVY
};

static void 
MGAResetVideoOverlay(ScrnInfoPtr pScrn) 
{
    MGAPtr pMga = MGAPTR(pScrn);
    MGAPortPrivPtr pPriv = pMga->portPrivate;

    CHECK_DMA_QUIESCENT(pMga, pScrn);
   
    outMGAdac(0x51, 0x01); /* keying on */
    outMGAdac(0x52, 0xff); /* full mask */
    outMGAdac(0x53, 0xff);
    outMGAdac(0x54, 0xff);

    outMGAdac(0x55, (pPriv->colorKey & pScrn->mask.red) >> 
		    pScrn->offset.red);
    outMGAdac(0x56, (pPriv->colorKey & pScrn->mask.green) >> 
		    pScrn->offset.green);
    outMGAdac(0x57, (pPriv->colorKey & pScrn->mask.blue) >> 
		    pScrn->offset.blue);

    OUTREG(MGAREG_BESLUMACTL, ((pPriv->brightness & 0xff) << 16) |
			       (pPriv->contrast & 0xff));
}


static XF86VideoAdaptorPtr
MGAAllocAdaptor(ScrnInfoPtr pScrn, Bool doublebuffer)
{
    XF86VideoAdaptorPtr adapt;
    MGAPtr pMga = MGAPTR(pScrn);
    MGAPortPrivPtr pPriv;
    int i;

    if(!(adapt = xf86XVAllocateVideoAdaptorRec(pScrn)))
	return NULL;

    if(!(pPriv = calloc(1, sizeof(MGAPortPrivRec) +
			(sizeof(DevUnion) * MGA_MAX_PORTS)))) 
    {
	free(adapt);
	return NULL;
    }

    adapt->pPortPrivates = (DevUnion*)(&pPriv[1]);

    for(i = 0; i < MGA_MAX_PORTS; i++)
	adapt->pPortPrivates[i].val = i;

    xvBrightness = MAKE_ATOM("XV_BRIGHTNESS");
    xvContrast   = MAKE_ATOM("XV_CONTRAST");
    xvColorKey   = MAKE_ATOM("XV_COLORKEY");
    xvDoubleBuffer = MAKE_ATOM("XV_DOUBLE_BUFFER");   

    pPriv->colorKey = pMga->videoKey;
    pPriv->videoStatus = 0;
    pPriv->brightness = 0;
    pPriv->contrast = 128;
    pPriv->lastPort = -1;
    pPriv->doubleBuffer = doublebuffer;       
    pPriv->currentBuffer = 0;         

    pMga->adaptor = adapt;
    pMga->portPrivate = pPriv;

    return adapt;
}

static XF86VideoAdaptorPtr 
MGASetupImageVideoOverlay(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    MGAPtr pMga = MGAPTR(pScrn);
    XF86VideoAdaptorPtr adapt;

    adapt = MGAAllocAdaptor(pScrn, TRUE);
    if (adapt == NULL)
	return NULL;

    adapt->type = XvWindowMask | XvInputMask | XvImageMask;
    adapt->flags = VIDEO_OVERLAID_IMAGES | VIDEO_CLIP_TO_VIEWPORT;
    adapt->name = "Matrox G-Series Backend Scaler";
    adapt->nEncodings = 1;
    adapt->pEncodings = &DummyEncoding[0];
    adapt->nFormats = NUM_FORMATS;
    adapt->pFormats = Formats;
    adapt->nPorts = 1;
    adapt->pAttributes = Attributes;
    if (pMga->Chipset == PCI_CHIP_MGAG400 || 
	pMga->Chipset == PCI_CHIP_MGAG550) {
	adapt->nImages = 4;
	adapt->nAttributes = 4;
    } else {
	adapt->nImages = 3;
	adapt->nAttributes = 1;
    }
    adapt->pImages = Images;
    adapt->PutVideo = NULL;
    adapt->PutStill = NULL;
    adapt->GetVideo = NULL;
    adapt->GetStill = NULL;
    adapt->StopVideo = MGAStopVideo;
    adapt->SetPortAttribute = MGASetPortAttributeOverlay;
    adapt->GetPortAttribute = MGAGetPortAttributeOverlay;
    adapt->QueryBestSize = MGAQueryBestSize;
    adapt->PutImage = MGAPutImage;
    adapt->QueryImageAttributes = MGAQueryImageAttributes;

    /* gotta uninit this someplace */
    REGION_NULL(pScreen, &(pMga->portPrivate->clip));

    MGAResetVideoOverlay(pScrn);

    return adapt;
}


static XF86VideoAdaptorPtr 
MGASetupImageVideoTexture(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    XF86VideoAdaptorPtr adapt;
    MGAPtr pMga = MGAPTR(pScrn);

    adapt = MGAAllocAdaptor(pScrn, FALSE);
    if (adapt == NULL)
	return NULL;

    adapt->type = XvWindowMask | XvInputMask | XvImageMask;
    adapt->flags = 0;
    adapt->name = "Matrox G-Series Texture Engine";
    adapt->nEncodings = 1;
    adapt->pEncodings = &DummyEncoding[1];
    adapt->nFormats = NUM_FORMATS;
    adapt->pFormats = Formats;
    adapt->nPorts = MGA_MAX_PORTS;
    adapt->pAttributes = NULL;
    adapt->nAttributes = 0;
    adapt->pImages = Images;
    if (pMga->Chipset == PCI_CHIP_MGAG400 ||
	pMga->Chipset == PCI_CHIP_MGAG550)
	adapt->nImages = 4;
    else
	adapt->nImages = 3;
    adapt->PutVideo = NULL;
    adapt->PutStill = NULL;
    adapt->GetVideo = NULL;
    adapt->GetStill = NULL;
    adapt->StopVideo = MGAStopVideo;
    adapt->SetPortAttribute = MGASetPortAttributeTexture;
    adapt->GetPortAttribute = MGAGetPortAttributeTexture;
    adapt->QueryBestSize = MGAQueryBestSize;
    adapt->PutImage = MGAPutImage;
    adapt->QueryImageAttributes = MGAQueryImageAttributes;

    return adapt;
}


static void 
MGAStopVideo(ScrnInfoPtr pScrn, pointer data, Bool shutdown)
{
  MGAPtr pMga = MGAPTR(pScrn);
  MGAPortPrivPtr pPriv = pMga->portPrivate;

  if(pMga->TexturedVideo) return;

  REGION_EMPTY(pScrn->pScreen, &pPriv->clip);   

  if(shutdown) {
     if(pPriv->videoStatus & CLIENT_VIDEO_ON)
	OUTREG(MGAREG_BESCTL, 0);
     if (pPriv->video_memory) {
         MGAFreeMemory(pScrn, pPriv->video_memory);
         pPriv->video_memory = NULL;
     }
     pPriv->videoStatus = 0;
  } else {
     if(pPriv->videoStatus & CLIENT_VIDEO_ON) {
	pPriv->videoStatus |= OFF_TIMER;
	pPriv->offTime = currentTime.milliseconds + OFF_DELAY; 
     }
  }
}

static int 
MGASetPortAttributeOverlay(
  ScrnInfoPtr pScrn, 
  Atom attribute,
  INT32 value, 
  pointer data
){
  MGAPtr pMga = MGAPTR(pScrn);
  MGAPortPrivPtr pPriv = pMga->portPrivate;

  CHECK_DMA_QUIESCENT(pMga, pScrn);

  if(attribute == xvBrightness) {
	if((value < -128) || (value > 127))
	   return BadValue;
	pPriv->brightness = value;
	OUTREG(MGAREG_BESLUMACTL, ((pPriv->brightness & 0xff) << 16) |
			           (pPriv->contrast & 0xff));
  } else
  if(attribute == xvContrast) {
	if((value < 0) || (value > 255))
	   return BadValue;
	pPriv->contrast = value;
	OUTREG(MGAREG_BESLUMACTL, ((pPriv->brightness & 0xff) << 16) |
			           (pPriv->contrast & 0xff));
  } else
  if(attribute == xvColorKey) {
	pPriv->colorKey = value;
	outMGAdac(0x55, (pPriv->colorKey & pScrn->mask.red) >> 
		    pScrn->offset.red);
	outMGAdac(0x56, (pPriv->colorKey & pScrn->mask.green) >> 
		    pScrn->offset.green);
	outMGAdac(0x57, (pPriv->colorKey & pScrn->mask.blue) >> 
		    pScrn->offset.blue);
	REGION_EMPTY(pScrn->pScreen, &pPriv->clip);   
  } else
  if(attribute == xvDoubleBuffer) {
	if((value < 0) || (value > 1))
          return BadValue;
	pPriv->doubleBuffer = value;  
  } else return BadMatch;

  return Success;
}

static int 
MGAGetPortAttributeOverlay(
  ScrnInfoPtr pScrn, 
  Atom attribute,
  INT32 *value, 
  pointer data
){
  MGAPtr pMga = MGAPTR(pScrn);
  MGAPortPrivPtr pPriv = pMga->portPrivate;

  if(attribute == xvBrightness) {
	*value = pPriv->brightness;
  } else
  if(attribute == xvContrast) {
	*value = pPriv->contrast;
  } else
  if(attribute == xvDoubleBuffer) {
        *value = pPriv->doubleBuffer ? 1 : 0;
  } else
  if(attribute == xvColorKey) {
	*value = pPriv->colorKey;
  } else return BadMatch;

  return Success;
}


static int 
MGASetPortAttributeTexture(
  ScrnInfoPtr pScrn, 
  Atom attribute,
  INT32 value, 
  pointer data
) {
  return BadMatch;
}


static int 
MGAGetPortAttributeTexture(
  ScrnInfoPtr pScrn, 
  Atom attribute,
  INT32 *value, 
  pointer data
){
  return BadMatch;
}

static void 
MGAQueryBestSize(
  ScrnInfoPtr pScrn, 
  Bool motion,
  short vid_w, short vid_h, 
  short drw_w, short drw_h, 
  unsigned int *p_w, unsigned int *p_h, 
  pointer data
){
  *p_w = drw_w;
  *p_h = drw_h; 
}


static void
MGACopyData(
  unsigned char *src,
  unsigned char *dst,
  int srcPitch,
  int dstPitch,
  int h,
  int w
){
    w <<= 1;
    while(h--) {
	/* XXX Maybe this one needs big-endian fixes, too? -ReneR */
	memcpy(dst, src, w);
	src += srcPitch;
	dst += dstPitch;
    }
}

static void
MGACopyMungedData(
   unsigned char *src1,
   unsigned char *src2,
   unsigned char *src3,
   unsigned char *dst1,
   int srcPitch,
   int srcPitch2,
   int dstPitch,
   int h,
   int w
){
   CARD32 *dst;
   CARD8 *s1, *s2, *s3;
   int i, j;

   w >>= 1;

   for(j = 0; j < h; j++) {
        dst = (CARD32*)dst1;
        s1 = src1;  s2 = src2;  s3 = src3;
        i = w;
        while(i > 4) {
#if X_BYTE_ORDER == X_LITTLE_ENDIAN
           dst[0] = s1[0] | (s1[1] << 16) | (s3[0] << 8) | (s2[0] << 24);
           dst[1] = s1[2] | (s1[3] << 16) | (s3[1] << 8) | (s2[1] << 24);
           dst[2] = s1[4] | (s1[5] << 16) | (s3[2] << 8) | (s2[2] << 24);
           dst[3] = s1[6] | (s1[7] << 16) | (s3[3] << 8) | (s2[3] << 24);
#else
           dst[0] = (s1[0] << 16) | s1[1] | (s3[0] << 24) | (s2[0] << 8);
           dst[1] = (s1[2] << 16) | s1[3] | (s3[1] << 24) | (s2[1] << 8);
           dst[2] = (s1[4] << 16) | s1[5] | (s3[2] << 24) | (s2[2] << 8);
           dst[3] = (s1[6] << 16) | s1[7] | (s3[3] << 24) | (s2[3] << 8);
#endif
           dst += 4; s2 += 4; s3 += 4; s1 += 8;
           i -= 4;
        }

        while(i--) {
#if X_BYTE_ORDER == X_LITTLE_ENDIAN
           dst[0] = s1[0] | (s1[1] << 16) | (s3[0] << 8) | (s2[0] << 24);
#else
           dst[0] = (s1[0] << 16) | s1[1] | (s3[0] << 24) | (s2[0] << 8);
#endif
           dst++; s2++; s3++;
           s1 += 2;
        }

        dst1 += dstPitch;
        src1 += srcPitch;
        if(j & 1) {
            src2 += srcPitch2;
            src3 += srcPitch2;
        }
   }
}


static CARD32
MGAAllocateMemory(
   ScrnInfoPtr pScrn,
   void **mem_struct,
   int size
){
   MGAPtr pMga = MGAPTR(pScrn);
   ScreenPtr pScreen = xf86ScrnToScreen(pScrn);
   int offset = 0;

#ifdef USE_EXA
   if (pMga->Exa) {
       ExaOffscreenArea *area = *mem_struct;

	if (area) {
	    if (area->size >= size)
		return area->offset;

	    exaOffscreenFree(pScrn->pScreen, area);
	}

	area = exaOffscreenAlloc(pScrn->pScreen, size, 64, TRUE, MGAVideoSave,
				 NULL);
	*mem_struct = area;

	if (!area)
	    return 0;

	offset = area->offset;
   }
#endif /* USE_EXA */
#ifdef USE_XAA
   FBLinearPtr linear = *mem_struct;
   int cpp = pMga->CurrentLayout.bitsPerPixel / 8;

   /* XAA allocates in units of pixels at the screen bpp, so adjust size
    * appropriately.
    */
   size = (size + cpp - 1) / cpp;

   if (!pMga->Exa) {
       if (linear) {
           if (linear->size >= size)
               return linear->offset * cpp;

           if (xf86ResizeOffscreenLinear(linear, size))
               return linear->offset * cpp;

           xf86FreeOffscreenLinear(linear);
       }


       linear = xf86AllocateOffscreenLinear(pScreen, size, 16,
                                            NULL, NULL, NULL);
       *mem_struct = linear;

       if (!linear) {
           int max_size;

           xf86QueryLargestOffscreenLinear(pScreen, &max_size, 16,
                                           PRIORITY_EXTREME);

           if (max_size < size)
               return 0;

           xf86PurgeUnlockedOffscreenAreas(pScreen);

           linear = xf86AllocateOffscreenLinear(pScreen, size, 16,
                                                NULL, NULL, NULL);
           *mem_struct = linear;

           if (!linear)
               return 0;
       }

       offset = linear->offset * cpp;
   }
#endif /* USE_XAA */

   return offset;
}

static void
MGAFreeMemory(ScrnInfoPtr pScrn, void *mem_struct)
{
    MGAPtr pMga = MGAPTR(pScrn);

#ifdef USE_EXA
    if (pMga->Exa) {
	ExaOffscreenArea *area = mem_struct;

	if (area)
	    exaOffscreenFree(pScrn->pScreen, area);
    }
#endif /* USE_EXA */
#ifdef USE_XAA
    if (!pMga->Exa) {
	FBLinearPtr linear = mem_struct;

	if (linear)
	    xf86FreeOffscreenLinear(linear);
    }
#endif /* USE_XAA */
}

static void
MGADisplayVideoOverlay(
    ScrnInfoPtr pScrn,
    int id,
    int offset,
    short width, short height,
    int pitch, 
    int x1, int y1, int x2, int y2,
    BoxPtr dstBox,
    short src_w, short src_h,
    short drw_w, short drw_h
){
    MGAPtr pMga = MGAPTR(pScrn);
    int tmp, hzoom, intrep;
    int maxOverlayClock;

    CHECK_DMA_QUIESCENT(pMga, pScrn);

    /* got 48 scanlines to do it in */
    tmp = INREG(MGAREG_VCOUNT) + 48;
    /* FIXME always change it in vertical retrace use CrtcV ?*/
    if(tmp > pScrn->currentMode->CrtcVTotal)
	tmp -= 49; /* too bad */
    else
        tmp = pScrn->currentMode->CrtcVTotal -1;

    tmp = pScrn->currentMode->VDisplay +1;
    /* enable accelerated 2x horizontal zoom when pixelclock >135MHz */

    if ((pMga->ChipRev >= 0x80) || (pMga->Chipset == PCI_CHIP_MGAG550)) {
	/* G450, G550 */
	maxOverlayClock = 234000;
    } else {
	maxOverlayClock = 135000;
    }

    hzoom = (pScrn->currentMode->Clock > maxOverlayClock) ? 1 : 0;

    switch(id) {
    case FOURCC_UYVY:
	OUTREG(MGAREG_BESGLOBCTL, 0x000000c0 | (3 * hzoom) | (tmp << 16));
	break;
    case FOURCC_YUY2:
    default:
	OUTREG(MGAREG_BESGLOBCTL, 0x00000080 | (3 * hzoom) | (tmp << 16));
	break;
    }

    OUTREG(MGAREG_BESA1ORG, offset);

    if(y1 & 0x00010000)
	OUTREG(MGAREG_BESCTL, 0x00040c41);
    else 
	OUTREG(MGAREG_BESCTL, 0x00040c01);
 
    OUTREG(MGAREG_BESHCOORD, (dstBox->x1 << 16) | (dstBox->x2 - 1));
    OUTREG(MGAREG_BESVCOORD, (dstBox->y1 << 16) | (dstBox->y2 - 1));

    OUTREG(MGAREG_BESHSRCST, x1 & 0x03fffffc);
    OUTREG(MGAREG_BESHSRCEND, (x2 - 0x00010000) & 0x03fffffc);
    OUTREG(MGAREG_BESHSRCLST, (width - 1) << 16);
   
    OUTREG(MGAREG_BESPITCH, pitch >> 1);

    OUTREG(MGAREG_BESV1WGHT, y1 & 0x0000fffc);
    OUTREG(MGAREG_BESV1SRCLST, height - 1 - (y1 >> 16));

    intrep = ((drw_h == src_h) || (drw_h < 2)) ? 0 : 1;
    tmp = ((src_h - intrep) << 16)/(drw_h - intrep);
    if(tmp >= (32 << 16))
	tmp = (32 << 16) - 1;
    OUTREG(MGAREG_BESVISCAL, tmp & 0x001ffffc);

    intrep = ((drw_w == src_w) || (drw_w < 2)) ? 0 : 1;
    tmp = (((src_w - intrep) << 16)/(drw_w - intrep)) << hzoom;
    if(tmp >= (32 << 16))
	tmp = (32 << 16) - 1;
    OUTREG(MGAREG_BESHISCAL, tmp & 0x001ffffc);

}


/**
 * \todo
 * Starting with at least the G200, the chip can handle non-mipmapped
 * non-power-of-two textures.  However, the code in this routine forces the
 * texture dimensions to be powers of two.  That should simplify the code and
 * may improve performance slightly.
 */
static void
MGADisplayVideoTexture(
    ScrnInfoPtr pScrn,
    int id, int offset,
    int nbox, BoxPtr pbox,
    int width, int height, int pitch,
    short src_x, short src_y,
    short src_w, short src_h,
    short drw_x, short drw_y,
    short drw_w, short drw_h
){
    MGAPtr pMga = MGAPTR(pScrn);
    int log2w = 0, log2h = 0, i, incx, incy, padw, padh;
    
    pitch >>= 1;

    i = 12;
    while(--i) {
	if(width & (1 << i)) {
	    log2w = i;
	    if(width & ((1 << i) - 1)) 
		log2w++;
	    break;		
	}
    }

    i = 12;
    while(--i) {
	if(height & (1 << i)) {
	    log2h = i;
	    if(height & ((1 << i) - 1)) 
		log2h++;		
	    break;		
	}
    }

    padw = 1 << log2w;
    padh = 1 << log2h;
    incx = (src_w << 20)/(drw_w * padw);
    incy = (src_h << 20)/(drw_h * padh);
   
    CHECK_DMA_QUIESCENT(pMga, pScrn);

    WAITFIFO(15);
    OUTREG(MGAREG_TMR0, incx);  /* sx inc */
    OUTREG(MGAREG_TMR1, 0);  /* sy inc */
    OUTREG(MGAREG_TMR2, 0);  /* tx inc */
    OUTREG(MGAREG_TMR3, incy);  /* ty inc */
    OUTREG(MGAREG_TMR4, 0x00000000); 
    OUTREG(MGAREG_TMR5, 0x00000000);
    OUTREG(MGAREG_TMR8, 0x00010000);
    OUTREG(MGAREG_TEXORG, offset);
    OUTREG(MGAREG_TEXWIDTH,  log2w | (((8 - log2w) & 63) << 9) | 
				((width - 1) << 18));
    OUTREG(MGAREG_TEXHEIGHT, log2h | (((8 - log2h) & 63) << 9) | 
				((height - 1) << 18));
    if(id == FOURCC_UYVY)
	OUTREG(MGAREG_TEXCTL, 0x1A00010b | ((pitch & 0x07FF) << 9));
    else
	OUTREG(MGAREG_TEXCTL, 0x1A00010a | ((pitch & 0x07FF) << 9));
    OUTREG(MGAREG_TEXCTL2, 0x00000014);
    OUTREG(MGAREG_DWGCTL, 0x000c7076);   
    OUTREG(MGAREG_TEXFILTER, 0x01e00020);
    OUTREG(MGAREG_ALPHACTRL, 0x00000001);

    padw = (src_x << 20)/padw;
    padh = (src_y << 20)/padh;

    while(nbox--) {
	WAITFIFO(4);
	OUTREG(MGAREG_TMR6, (incx * (pbox->x1 - drw_x)) + padw);
	OUTREG(MGAREG_TMR7, (incy * (pbox->y1 - drw_y)) + padh);
	OUTREG(MGAREG_FXBNDRY, (pbox->x2 << 16) | (pbox->x1 & 0xffff));
	OUTREG(MGAREG_YDSTLEN + MGAREG_EXEC, 
				(pbox->y1 << 16) | (pbox->y2 - pbox->y1));
	pbox++;
    }

    MGA_MARK_SYNC(pMga, pScrn);
}

static int 
MGAPutImage( 
  ScrnInfoPtr pScrn, 
  short src_x, short src_y, 
  short drw_x, short drw_y,
  short src_w, short src_h, 
  short drw_w, short drw_h,
  int id, unsigned char* buf, 
  short width, short height, 
  Bool Sync,
  RegionPtr clipBoxes, pointer data,
  DrawablePtr pDraw
){
   MGAPtr pMga = MGAPTR(pScrn);
   MGAPortPrivPtr pPriv = pMga->portPrivate;
   INT32 x1, x2, y1, y2;
   unsigned char *dst_start;
   int new_size, offset, offset2 = 0, offset3 = 0;
   int srcPitch, srcPitch2 = 0, dstPitch;
   int top, left, npixels, nlines;
   BoxRec dstBox;
   CARD32 tmp;

   /* Clip */
   x1 = src_x;
   x2 = src_x + src_w;
   y1 = src_y;
   y2 = src_y + src_h;

   dstBox.x1 = drw_x;
   dstBox.x2 = drw_x + drw_w;
   dstBox.y1 = drw_y;
   dstBox.y2 = drw_y + drw_h;

   if(!xf86XVClipVideoHelper(&dstBox, &x1, &x2, &y1, &y2,
			     clipBoxes, width, height))
	return Success;

   if(!pMga->TexturedVideo) {
	dstBox.x1 -= pScrn->frameX0;
	dstBox.x2 -= pScrn->frameX0;
	dstBox.y1 -= pScrn->frameY0;
	dstBox.y2 -= pScrn->frameY0;
   }

   dstPitch = ((width << 1) + 15) & ~15;
   new_size = dstPitch * height;
   
   switch(id) {
   case FOURCC_YV12:
   case FOURCC_I420:
	srcPitch = (width + 3) & ~3;
	offset2 = srcPitch * height;
	srcPitch2 = ((width >> 1) + 3) & ~3;
	offset3 = (srcPitch2 * (height >> 1)) + offset2;
	break;
   case FOURCC_UYVY:
   case FOURCC_YUY2:
   default:
	srcPitch = (width << 1);
	break;
   }  

   pPriv->video_offset = MGAAllocateMemory(pScrn, &pPriv->video_memory,
					   pPriv->doubleBuffer ?
                                           (new_size << 1) : new_size);
   if (!pPriv->video_offset)
	return BadAlloc;

   pPriv->currentBuffer ^= 1;

    /* copy data */
   top = y1 >> 16;
   left = (x1 >> 16) & ~1;
   npixels = ((((x2 + 0xffff) >> 16) + 1) & ~1) - left;
   left <<= 1;

   offset = pPriv->video_offset;
   if(pPriv->doubleBuffer)
        offset += pPriv->currentBuffer * new_size;
   dst_start = pMga->FbStart + offset + left + (top * dstPitch);

   if (pMga->TexturedVideo && ((long)data != pPriv->lastPort))
       MGA_SYNC(pMga, pScrn);

   switch(id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	top &= ~1;
	tmp = ((top >> 1) * srcPitch2) + (left >> 2);
	offset2 += tmp;
	offset3 += tmp;
	if(id == FOURCC_I420) {
	   tmp = offset2;
	   offset2 = offset3;
	   offset3 = tmp;
	}
	nlines = ((((y2 + 0xffff) >> 16) + 1) & ~1) - top;
	MGACopyMungedData(buf + (top * srcPitch) + (left >> 1), 
			  buf + offset2, buf + offset3, dst_start,
			  srcPitch, srcPitch2, dstPitch, nlines, npixels);
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	buf += (top * srcPitch) + left;
	nlines = ((y2 + 0xffff) >> 16) - top;
	MGACopyData(buf, dst_start, srcPitch, dstPitch, nlines, npixels);
        break;
    }

    if(pMga->TexturedVideo) {
	pPriv->lastPort = (long)data;
	MGADisplayVideoTexture(pScrn, id, offset, 
		REGION_NUM_RECTS(clipBoxes), REGION_RECTS(clipBoxes),
		width, height, dstPitch, src_x, src_y, src_w, src_h,
		drw_x, drw_y, drw_w, drw_h);
	pPriv->videoStatus = FREE_TIMER;
	pPriv->freeTime = currentTime.milliseconds + FREE_DELAY;
    } else {
    /* update cliplist */
	if(!REGION_EQUAL(pScrn->pScreen, &pPriv->clip, clipBoxes)) {
	    REGION_COPY(pScrn->pScreen, &pPriv->clip, clipBoxes);
	    /* draw these */
	    xf86XVFillKeyHelper(pScrn->pScreen, pPriv->colorKey, clipBoxes);
	}

	offset += top * dstPitch;
	MGADisplayVideoOverlay(pScrn, id, offset, width, height, dstPitch,
	     x1, y1, x2, y2, &dstBox, src_w, src_h, drw_w, drw_h);

	pPriv->videoStatus = CLIENT_VIDEO_ON;
    }
    pMga->VideoTimerCallback = MGAVideoTimerCallback;

    return Success;
}


static int 
MGAQueryImageAttributes(
    ScrnInfoPtr pScrn, 
    int id, 
    unsigned short *w, unsigned short *h, 
    int *pitches, int *offsets
){
    MGAPtr pMga = MGAPTR(pScrn);
    int size, tmp;

    if(pMga->TexturedVideo) {
	if(*w > 2046) *w = 2046;
	if(*h > 2046) *h = 2046;
    } else {
	if(*w > 1024) *w = 1024;
	if(*h > 1024) *h = 1024;
    }

    *w = (*w + 1) & ~1;
    if(offsets) offsets[0] = 0;

    switch(id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	*h = (*h + 1) & ~1;
	size = (*w + 3) & ~3;
	if(pitches) pitches[0] = size;
	size *= *h;
	if(offsets) offsets[1] = size;
	tmp = ((*w >> 1) + 3) & ~3;
	if(pitches) pitches[1] = pitches[2] = tmp;
	tmp *= (*h >> 1);
	size += tmp;
	if(offsets) offsets[2] = size;
	size += tmp;
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	size = *w << 1;
	if(pitches) pitches[0] = size;
	size *= *h;
	break;
    }

    return size;
}

static void
MGAVideoTimerCallback(ScrnInfoPtr pScrn, Time time)
{
    MGAPtr pMga = MGAPTR(pScrn);
    MGAPortPrivPtr pPriv = pMga->portPrivate;

    if(pPriv->videoStatus & TIMER_MASK) {
	if(pPriv->videoStatus & OFF_TIMER) {
	    if(pPriv->offTime < time) {
		OUTREG(MGAREG_BESCTL, 0);
		pPriv->videoStatus = FREE_TIMER;
		pPriv->freeTime = time + FREE_DELAY;
	    }
	} else {  /* FREE_TIMER */
	    if(pPriv->freeTime < time) {
		if (pPriv->video_memory) {
                   MGAFreeMemory(pScrn, pPriv->video_memory);
		   pPriv->video_memory = NULL;
		}
		pPriv->videoStatus = 0;
	        pMga->VideoTimerCallback = NULL;
	    }
        }
    } else  /* shouldn't get here */
	pMga->VideoTimerCallback = NULL;
}


/****************** Offscreen stuff ***************/

typedef struct {
  void *surface_memory;
  Bool isOn;
} OffscreenPrivRec, * OffscreenPrivPtr;

static int 
MGAAllocateSurface(
    ScrnInfoPtr pScrn,
    int id,
    unsigned short w, 	
    unsigned short h,
    XF86SurfacePtr surface
){
    void *surface_memory = NULL;
    int pitch, size, bpp, offset;
    OffscreenPrivPtr pPriv;

    if((w > 1024) || (h > 1024))
	return BadAlloc;

    w = (w + 1) & ~1;
    pitch = ((w << 1) + 15) & ~15;
    bpp = pScrn->bitsPerPixel >> 3;
    size = ((pitch * h) + bpp - 1) / bpp;

    offset = MGAAllocateMemory(pScrn, &surface_memory, size);
    if (!offset)
	return BadAlloc;

    surface->width = w;
    surface->height = h;

    if(!(surface->pitches = malloc(sizeof(int)))) {
        MGAFreeMemory(pScrn, surface_memory);
	return BadAlloc;
    }
    if(!(surface->offsets = malloc(sizeof(int)))) {
	free(surface->pitches);
        MGAFreeMemory(pScrn, surface_memory);
	return BadAlloc;
    }
    if(!(pPriv = malloc(sizeof(OffscreenPrivRec)))) {
	free(surface->pitches);
	free(surface->offsets);
        MGAFreeMemory(pScrn, surface_memory);
	return BadAlloc;
    }

    pPriv->surface_memory = surface_memory;
    pPriv->isOn = FALSE;

    surface->pScrn = pScrn;
    surface->id = id;   
    surface->pitches[0] = pitch;
    surface->offsets[0] = offset;
    surface->devPrivate.ptr = (pointer)pPriv;

    return Success;
}

static int 
MGAStopSurface(
    XF86SurfacePtr surface
){
    OffscreenPrivPtr pPriv = (OffscreenPrivPtr)surface->devPrivate.ptr;

    if(pPriv->isOn) {
	ScrnInfoPtr pScrn = surface->pScrn;
	MGAPtr pMga = MGAPTR(pScrn);
	OUTREG(MGAREG_BESCTL, 0);
	pPriv->isOn = FALSE;
    }

    return Success;
}


static int 
MGAFreeSurface(
    XF86SurfacePtr surface
){
    ScrnInfoPtr pScrn = surface->pScrn;
    OffscreenPrivPtr pPriv = (OffscreenPrivPtr)surface->devPrivate.ptr;

    if(pPriv->isOn)
	MGAStopSurface(surface);
    MGAFreeMemory(pScrn, pPriv->surface_memory);
    free(surface->pitches);
    free(surface->offsets);
    free(surface->devPrivate.ptr);

    return Success;
}

static int
MGAGetSurfaceAttribute(
    ScrnInfoPtr pScrn,
    Atom attribute,
    INT32 *value
){
    return MGAGetPortAttributeOverlay(pScrn, attribute, value, 0);
}

static int
MGASetSurfaceAttribute(
    ScrnInfoPtr pScrn,
    Atom attribute,
    INT32 value
){
    return MGASetPortAttributeOverlay(pScrn, attribute, value, 0);
}


static int 
MGADisplaySurface(
    XF86SurfacePtr surface,
    short src_x, short src_y, 
    short drw_x, short drw_y,
    short src_w, short src_h, 
    short drw_w, short drw_h,
    RegionPtr clipBoxes
){
    OffscreenPrivPtr pPriv = (OffscreenPrivPtr)surface->devPrivate.ptr;
    ScrnInfoPtr pScrn = surface->pScrn;
    MGAPtr pMga = MGAPTR(pScrn);
    MGAPortPrivPtr portPriv = pMga->portPrivate;
    INT32 x1, y1, x2, y2;
    BoxRec dstBox;

    x1 = src_x;
    x2 = src_x + src_w;
    y1 = src_y;
    y2 = src_y + src_h;

    dstBox.x1 = drw_x;
    dstBox.x2 = drw_x + drw_w;
    dstBox.y1 = drw_y;
    dstBox.y2 = drw_y + drw_h;

    if(!xf86XVClipVideoHelper(&dstBox, &x1, &x2, &y1, &y2, clipBoxes, 
			      surface->width, surface->height))
    {
	return Success;
    }

    dstBox.x1 -= pScrn->frameX0;
    dstBox.x2 -= pScrn->frameX0;
    dstBox.y1 -= pScrn->frameY0;
    dstBox.y2 -= pScrn->frameY0;

    MGAResetVideoOverlay(pScrn);

    MGADisplayVideoOverlay(pScrn, surface->id, surface->offsets[0], 
	     surface->width, surface->height, surface->pitches[0],
	     x1, y1, x2, y2, &dstBox, src_w, src_h, drw_w, drw_h);

    xf86XVFillKeyHelper(pScrn->pScreen, portPriv->colorKey, clipBoxes);

    pPriv->isOn = TRUE;
    /* we've prempted the XvImage stream so set its free timer */
    if(portPriv->videoStatus & CLIENT_VIDEO_ON) {
	REGION_EMPTY(pScrn->pScreen, &portPriv->clip);   
	UpdateCurrentTime();
	portPriv->videoStatus = FREE_TIMER;
	portPriv->freeTime = currentTime.milliseconds + FREE_DELAY;
	pMga->VideoTimerCallback = MGAVideoTimerCallback;
    }

    return Success;
}


static void 
MGAInitOffscreenImages(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    MGAPtr pMga = MGAPTR(pScrn);
    int num = (pMga->Chipset == PCI_CHIP_MGAG400 || pMga->Chipset == PCI_CHIP_MGAG550) ? 2 : 1;
    XF86OffscreenImagePtr offscreenImages;

    /* need to free this someplace */
    if(!(offscreenImages = malloc(num * sizeof(XF86OffscreenImageRec))))
	return;

    offscreenImages[0].image = &Images[0];
    offscreenImages[0].flags = VIDEO_OVERLAID_IMAGES | 
			       VIDEO_CLIP_TO_VIEWPORT;
    offscreenImages[0].alloc_surface = MGAAllocateSurface;
    offscreenImages[0].free_surface = MGAFreeSurface;
    offscreenImages[0].display = MGADisplaySurface;
    offscreenImages[0].stop = MGAStopSurface;
    offscreenImages[0].setAttribute = MGASetSurfaceAttribute;
    offscreenImages[0].getAttribute = MGAGetSurfaceAttribute;
    offscreenImages[0].max_width = 1024;
    offscreenImages[0].max_height = 1024;
    offscreenImages[0].num_attributes = (num == 1) ? 1 : 4;
    offscreenImages[0].attributes = Attributes;

    if(num == 2) {
	offscreenImages[1].image = &Images[3];
	offscreenImages[1].flags = VIDEO_OVERLAID_IMAGES | 
				   VIDEO_CLIP_TO_VIEWPORT;
	offscreenImages[1].alloc_surface = MGAAllocateSurface;
	offscreenImages[1].free_surface = MGAFreeSurface;
	offscreenImages[1].display = MGADisplaySurface;
	offscreenImages[1].stop = MGAStopSurface;
	offscreenImages[1].setAttribute = MGASetSurfaceAttribute;
	offscreenImages[1].getAttribute = MGAGetSurfaceAttribute;
	offscreenImages[1].max_width = 1024;
	offscreenImages[1].max_height = 1024;
	offscreenImages[1].num_attributes = 4;
	offscreenImages[1].attributes = Attributes;
    }

    xf86XVRegisterOffscreenImages(pScreen, offscreenImages, num);
}


/* Matrox MGA 2164W Xv extension support.
*  The extension is implemented as a HOST->FB image load in YUV format. 
*  I decided not to use real hardware overlay since on the Millennium II
*  it would limit the size of the frame buffer to 4Mb (even on a 16Mb
*  card) due to an hardware limitation.
*  Author: Gabriele Gorla (gorlik@yahoo.com)
*  Based on the MGA-Gxxx Xv extension by: Mark Vojkovich
   */

/* This code is still in alpha stage. Only YUV->RGB conversion
   and horizontal scaling are hardware accelerated.
   All 4 FOURCC formats supported by X should be supported.
   It has been tested only on my DEC XP1000 at 1024x768x32 under
   linux 2.6.18 with X.org 7.1.1 (debian alpha)

   Bug reports and success/failure stories are greatly appreciated.
*/

/* #define DEBUG_MGA2164 */
/* #define CUSTOM_MEMCOPY */
#define MGA2164_SWFILTER


static XF86VideoAdaptorPtr
MGASetupImageVideoILOAD(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    XF86VideoAdaptorPtr adapt;
    MGAPtr pMga = MGAPTR(pScrn);

    adapt = MGAAllocAdaptor(pScrn, FALSE);
    if (adapt == NULL)
	return NULL;

    adapt->type = XvWindowMask | XvInputMask | XvImageMask;
    adapt->flags = 0;
    adapt->name = "Matrox Millennium II ILOAD Video Engine";
    adapt->nEncodings = 1;
    adapt->pEncodings = &DummyEncoding[1];
    adapt->nFormats = NUM_FORMATS;
    adapt->pFormats = Formats;
    adapt->nPorts = MGA_MAX_PORTS;
    adapt->pAttributes = NULL;
    adapt->nAttributes = 0;
    
    /* number of supported color formats */
    adapt->pImages = Images;
    adapt->nImages = 4;

    adapt->PutVideo = NULL;
    adapt->PutStill = NULL;
    adapt->GetVideo = NULL;
    adapt->GetStill = NULL;
    adapt->StopVideo = MGAStopVideo;
    
    adapt->SetPortAttribute = MGASetPortAttributeTexture;
    adapt->GetPortAttribute = MGAGetPortAttributeTexture;
    adapt->QueryBestSize = MGAQueryBestSize;
    adapt->PutImage = MGAPutImageILOAD;
    adapt->QueryImageAttributes = MGAQueryImageAttributes;

    REGION_INIT(pScreen, &(pMga->portPrivate->clip), NullBox, 0);

    return adapt;
}

/* this function is optimized for alpha. It might be better also for 
other load/store risc architectures but I never tested on anything else 
than my ev56 */
static void CopyMungedScanline_AXP(CARD32 *fb_ptr, short src_w,
				   CARD32 *tsp, CARD32 *tpu, CARD32 *tpv)
{
    CARD32 k,y0,y1,u,v;
  
    for(k=src_w/8;k;k--) {
	y0=*tsp;
	y1=*(tsp+1);
	u=*tpu;
	v=*tpv;

	*(fb_ptr)=(y1&0x000000ff)|((y1&0x0000ff00)<<8) |
	    (v&0x00ff0000)<<8 | (u&0x00ff0000)>>8;
	*(fb_ptr+1)=(y1&0x000000ff)|((y1&0x0000ff00)<<8) |
	    (v&0x00ff0000)<<8 | (u&0x00ff0000)>>8;

	*(fb_ptr+2)=(y0&0x000000ff)|((y0&0x0000ff00)<<8) |
	    (v&0x000000ff)<<24 | (u&0x000000ff)<<8;
	*(fb_ptr+3)=(y0&0x000000ff)|((y0&0x0000ff00)<<8) |
	    (v&0x000000ff)<<24 | (u&0x000000ff)<<8;

	/*correct below*/
	/*    *(fb_ptr)=(y0&0x000000ff)|((y0&0x0000ff00)<<8) |
	      (v&0x000000ff)<<24 | (u&0x000000ff)<<8;
	      *(fb_ptr+1)=((y0&0x00ff0000)>>16)|((y0&0xff000000)>>8) |
	      (v&0x0000ff00)<<16 | (u&0x0000ff00);
	      *(fb_ptr+2)=(y1&0x000000ff)|((y1&0x0000ff00)<<8) |
	      (v&0x00ff0000)<<8 | (u&0x00ff0000)>>8;
	      *(fb_ptr+3)=((y1&0x00ff0000)>>16)|((y1&0xff000000)>>8) |
	      (v&0xff000000) | (u&0xff000000)>>16; */

	tsp+=2; tpu++; tpv++;    
	fb_ptr+=4;
    }
}

static void CopyMungedScanline_AXP2(CARD32 *fb_ptr, short src_w,
				    CARD32 *tsp, CARD32 *tpu, CARD32 *tpv)
{
    CARD8 *y, *u, *v;
    int k;
    y=(CARD8 *)tsp;
    u=(CARD8 *)tpu;
    v=(CARD8 *)tpv;

    for(k=src_w/8;k;k--) {
	fb_ptr[0]=y[0] | y[1]<<16 | v[0]<<24 | u[0]<<8;
	fb_ptr[1]=y[2] | y[3]<<16 | v[1]<<24 | u[1]<<8;
	fb_ptr[2]=y[4] | y[5]<<16 | v[2]<<24 | u[2]<<8;
	fb_ptr[3]=y[6] | y[7]<<16 | v[3]<<24 | u[3]<<8;

	y+=8; u+=4; v+=4;
	fb_ptr+=4;
    }
}


static void CopyMungedScanlineFilter_AXP(CARD32 *fb_ptr, short src_w,
					 CARD32 *tsp1, CARD32 *tpu1, CARD32 *tpv1,
					 CARD32 *tsp2, CARD32 *tpu2, CARD32 *tpv2,
					 int beta, int xds )
{
    unsigned int k,y0_1,y1_1,y0_2,y1_2,u,v;
    int yf[8], uf[4], vf[4];
    int oneminbeta = 0xff - beta;

    for(k=xds*src_w/8;k;k--) {
	y0_1=*tsp1;
	y1_1=*(tsp1+1);
	y0_2=*tsp2;
	y1_2=*(tsp2+1);
	u=*tpu1;
	v=*tpv1;

	tsp1+=2; tsp2+=2; tpu1++; tpv1++; 
	yf[0] = ((y0_1&0x000000ff)*oneminbeta + (y0_2&0x000000ff)*beta )>>8;
	yf[1] = (((y0_1&0x0000ff00)>>8)*oneminbeta + ((y0_2&0x0000ff00)>>8)*beta )>>8;
	yf[2] = (((y0_1&0x00ff0000)>>16)*oneminbeta + ((y0_2&0x00ff0000)>>16)*beta )>>8;
	yf[3] = (((y0_1&0xff000000)>>24)*oneminbeta + ((y0_2&0xff000000)>>24)*beta )>>8;
	yf[4] = ((y1_1&0x000000ff)*oneminbeta + (y1_2&0x000000ff)*beta )>>8;
	yf[5] = (((y1_1&0x0000ff00)>>8)*oneminbeta + ((y1_2&0x0000ff00)>>8)*beta )>>8;
	yf[6] = (((y1_1&0x00ff0000)>>16)*oneminbeta + ((y1_2&0x00ff0000)>>16)*beta )>>8;
	yf[7] = (((y1_1&0xff000000)>>24)*oneminbeta + ((y1_2&0xff000000)>>24)*beta )>>8;

	/* FIXME: there is still no filtering on u and v */
	uf[0]=(u&0x000000ff);
	uf[1]=(u&0x0000ff00)>>8;
	uf[2]=(u&0x00ff0000)>>16;
	uf[3]=(u&0xff000000)>>24;

	vf[0]=(v&0x000000ff);
	vf[1]=(v&0x0000ff00)>>8;
	vf[2]=(v&0x00ff0000)>>16;
	vf[3]=(v&0xff000000)>>24;

	switch(xds) {
	case 1:
	    *(fb_ptr)=(yf[0]) | (yf[1]<<16) |
		vf[0]<<24 | uf[0]<<8;
	    *(fb_ptr+1)=(yf[2]) | (yf[3]<<16) |
		vf[1]<<24 | uf[1]<<8;
	    *(fb_ptr+2)=(yf[4]) | (yf[5]<<16) |
		vf[2]<<24 | uf[2]<<8;
	    *(fb_ptr+3)=(yf[6]) | (yf[7]<<16) |
		vf[3]<<24 | uf[3]<<8;
	    fb_ptr+=4;
	    break;

	case 2:
	    *(fb_ptr)=(yf[0]+yf[1])/2 | (((yf[2]+yf[3])/2)<<16) |
		((vf[0]+vf[1])/2 )<<24 | ((uf[0]+uf[1])/2)<<8;
	    *(fb_ptr+1)=(yf[4]+yf[5])/2 | ( ((yf[6]+yf[7])/2) <<16) |
		((vf[2]+vf[3])/2 )<<24 | ((uf[2]+uf[3])/2)<<8;
	    fb_ptr+=2;
	    break;

	case 4:
	    *(fb_ptr)=(yf[0]+yf[1]+yf[2]+yf[3])/4 | (((yf[4]+yf[5]+yf[6]+yf[7])/4)<<16) |
		((vf[0]+vf[1]+vf[2]+vf[3])/4 )<<24 | ((uf[0]+uf[1]+uf[2]+uf[3])/4)<<8;
	    fb_ptr+=1;
	    break;

	default:
	    break;
	}
    }
}

static void CopyMungedScanlineFilterDown_AXP(CARD32 *fb_ptr, short src_w,
					     CARD32 *tsp1, CARD32 *tpu1, CARD32 *tpv1,
					     CARD32 *tsp2, CARD32 *tpu2, CARD32 *tpv2,
					     int beta , int xds)
{
    unsigned int k,y0_1,y1_1,y0_2,y1_2,u,v;
    int yf[8], uf[4], vf[4];
  
    for(k=src_w/8;k;k--) {
	y0_1=*tsp1;
	y1_1=*(tsp1+1);
	y0_2=*tsp2;
	y1_2=*(tsp2+1);
	u=*tpu1;
	v=*tpv1;

	tsp1+=2; tsp2+=2; tpu1++; tpv1++;
	yf[0] = ((y0_1&0x000000ff) + (y0_2&0x000000ff))>>8;
	yf[1] = (((y0_1&0x0000ff00)>>8) + ((y0_2&0x0000ff00)>>8))>>8;
	yf[2] = (((y0_1&0x00ff0000)>>16) + ((y0_2&0x00ff0000)>>16))>>8;
	yf[3] = (((y0_1&0x000000ff)>>24) + ((y0_2&0x000000ff)>>24))>>8;
	yf[4] = ((y1_1&0x000000ff) + (y1_2&0x000000ff))>>8;
	yf[5] = (((y1_1&0x0000ff00)>>8) + ((y1_2&0x0000ff00)>>8))>>8;
	yf[6] = (((y1_1&0x00ff0000)>>16) + ((y1_2&0x00ff0000)>>16))>>8;
	yf[7] = (((y1_1&0x000000ff)>>24) + ((y1_2&0x000000ff)>>24))>>8;

	*(fb_ptr)=(yf[0]) | (yf[1]<<16) |
	    (v&0x000000ff)<<24 | (u&0x000000ff)<<8;
	*(fb_ptr+1)=(yf[2]) | (yf[3]<<16) |
	    (v&0x0000ff00)<<16 | (u&0x0000ff00);
	*(fb_ptr+2)=(yf[4]) | (yf[5]<<16) |
	    (v&0x00ff0000)<<8 | (u&0x00ff0000)>>8;
	*(fb_ptr+3)=(yf[6]) | (yf[7]<<16) |
	    (v&0xff000000) | (u&0xff000000)>>16;

	fb_ptr+=4;
    }
}

static void MGACopyScaledILOAD(
			       ScrnInfoPtr pScrn,
			       int id, unsigned char *buf,
			       BoxPtr pbox,
			       int width, int height, int pitch,
			       short src_x, short src_y,
			       short src_w, short src_h,
			       short drw_x, short drw_y,
			       short drw_w, short drw_h
			       )
{
    MGAPtr pMga = MGAPTR(pScrn);
    CARD32 *fb_ptr;
    unsigned char *ubuf, *vbuf, *tbuf;
    CARD32 *pu, *pv;
    int k,l, pl, dl, xds, yds;
    short box_h;
    short scr_pitch = ( pScrn->virtualX + 15) & ~15;

#ifdef DEBUG_MGA2164
    char sbuf[255];

    sprintf(sbuf,"---- PBOX: x1=%d y1=%d w=%d h=%d (x2=%d y2=%d)\n",
	    pbox->x1,pbox->y1,pbox->x2-pbox->x1,pbox->y2-pbox->y1,
	    pbox->x2,pbox->y2);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);

    sprintf(sbuf,"in src: src_x=%d src_y=%d src_w=%d src_h=%d\n",
	    src_x,src_y,src_w,src_h);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);
    sprintf(sbuf,"in drw: drw_x=%d drw_y=%d drw_w=%d drw_h=%d\n",
	    drw_x,drw_y,drw_w,drw_h);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);
#endif

    /* scaling yuv->rgb */

    /* hack to force width and src image to be 8 pixel aligned */
    src_x&=~0x7;
    src_w&=~0x7;

    box_h=pbox->y2-pbox->y1;

    /* compute X down scaling factor */
    if(src_w>drw_w) {
	if(src_w/2<drw_w) {
	    xds=2;
	} else if(src_w/4<drw_w) {
	    xds=4;
	} else { xds=8; }
    } else xds = 1;

    /* prevent crashing when dragging window outside left boundary of screen */
    /* FIXME: need to implement per pixel left start to avoid undesired
       effects when dragging window outside left screen boundary */

    if(drw_x<0) {
	src_x=( -(drw_x*src_w)/drw_w + 0x7)&~0x7;
	src_w-=src_x;
	drw_w+=drw_x;
	drw_x=0;
    }

    src_w/=xds;

    /* compute X down scaling factor */
    if(src_h>drw_h) {
	if(src_h/2<drw_h) {
	    yds=2;
	} else if(src_h/4<drw_h) {
	    yds=4;
	} else { yds=8; }
    } else yds = 1;


#ifdef DEBUG_MGA2164
    char sbuf[255];

    sprintf(sbuf,"---- xds = %d\n",
	    xds);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);
#endif


#ifdef DEBUG_MGA2164
    sprintf(sbuf,"out src: src_x=%d src_y=%d src_w=%d src_h=%d\n",
	    src_x,src_y,src_w,src_h);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);
    sprintf(sbuf,"out drw: drw_x=%d drw_y=%d drw_w=%d drw_h=%d\n",
	    drw_x,drw_y,drw_w,drw_h);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);
#endif

    CHECK_DMA_QUIESCENT(pMga, pScrn);

    /* scaling ILOAD */

    vbuf=buf+width*height;
    ubuf=vbuf+width*height/4;
    pu = (CARD32 *)(ubuf+(src_y/2)*(width/2));
    pv = (CARD32 *)(vbuf+(src_y/2)*(width/2));

    for(pl=-1,dl=0;dl<box_h;dl++) {
	int beta;
	l=(dl+(pbox->y1-drw_y))*src_h/drw_h;
	/* FIXME: check the math */
	beta = ((dl+(pbox->y1-drw_y))*src_h*0xff/drw_h) - ((dl+(pbox->y1-drw_y))*src_h/drw_h*0xff);

#ifdef MGA2164_BLIT_DUP
	if(l!=pl)
#else
	    if(1)
#endif
		{

		    /*
		      #ifdef DEBUG_MGA2164
		      sprintf(sbuf,"new line: scr_dst %d   img_src %d   prev %d\n",
		      dl,l,pl);
		      xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);
		      #endif
		    */

		    OUTREG(MGAREG_DWGCTL, MGADWG_ILOAD_HIQH | MGADWG_BUYUV | MGADWG_SHIFTZERO
			   | MGADWG_SGNZERO | 0xc0000);
    
		    OUTREG(MGAREG_AR0, pbox->x1 + drw_w -1);    /* SRC LINE END   why -1 ? */
		    OUTREG(MGAREG_AR2, ( ( (src_w-1)<<16) / (drw_w-1)) + 1 ); /* ((SRC_X_DIM -1)<<16) / (DST_X_DIM-1) +1 */
		    OUTREG(MGAREG_AR3, pbox->x1 );                            /* SRC LINE START*/
		    OUTREG(MGAREG_AR5, scr_pitch);                            /* DST_Y_INCR = PITCH? */
		    OUTREG(MGAREG_AR6, ((src_w-drw_w)<<16) / (drw_w-1) );     /* */
		    OUTREG(MGAREG_FXBNDRY, drw_x|((drw_x+drw_w-1)<<16) );     /* why -1 ? */
		    OUTREG(MGAREG_CXBNDRY, pbox->x1 | ((pbox->x2-1)<<16 ) );
		    OUTREG(MGAREG_YDST , pbox->y1+dl );                       /* Y_START_POS */
		    OUTREG(MGAREG_LEN + MGAREG_EXEC , 1);                     /* # of LINES */
    
		    /* xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Data finished\n"); */
    
		    fb_ptr=(CARD32 *)pMga->ILOADBase;
  
		    switch(id) {
		    case FOURCC_YV12:
		    case FOURCC_I420:
			tbuf=buf+(l+src_y)*width;
			{
			    CARD32 *tpu=pu+src_x/8+l/2*width/8;
			    CARD32 *tpv=pv+src_x/8+l/2*width/8;
			    CARD32 *tsp=(CARD32 *)(tbuf+src_x), *tsp2;

			    if((l+src_y)<(src_h-1))
				tsp2=(CARD32 *)(tbuf+src_x+width);
			    else
				tsp2=(CARD32 *)(tbuf+src_x);

			    /* it is not clear if waiting is actually good for performance */
			    /*	 WAITFIFO(pMga->FifoSize);*/
			    /* should try to get MGACopyMunged data to work here */
			    /*		CopyMungedScanline_AXP(fb_ptr,src_w,tsp,tpu,tpv); */

			    /* Filter does not work yet */
			    CopyMungedScanlineFilter_AXP(fb_ptr,src_w,tsp,tpu,tpv,tsp2,tpu,tpv, beta, xds); 
			    /*	if(l&1) {
				pu+=width/8;
				pv+=width/8;
				} */
			}
			break;
		    case FOURCC_UYVY:
		    case FOURCC_YUY2:
			tbuf=buf+(l+src_y)*width*2;

#ifndef MGA2164_SWFILTER
			WAITFIFO(pMga->FifoSize/2);
			memcpy(fb_ptr, tbuf+src_x*2, src_w*2);
			fb_ptr+=src_w*2;   /* pointer in the pseudo dma window */
#else
			{
			    CARD32 *tsp=(CARD32 *)(tbuf+src_x*2), *tsp2;

			    if((l+src_y)<(src_h-1))
				tsp2=(CARD32 *)(tbuf+src_x*2+width*2);
			    else
				tsp2=(CARD32 *)(tbuf+src_x*2);
			    /*	  {
				  char sbuf [256];
				  sprintf(sbuf,"dst line: %d   src_line: %d    beta: %x\n",
				  dl, l, beta );
				  xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);
				  }  */

			    WAITFIFO(pMga->FifoSize/4);
			    for(k=xds*src_w/8;k;k--) {
				int oneminbeta = 0xff-beta;
				int y[8], u[4], v[4], ya[4], ua[2], va[2], p;

				switch(yds) {
				case 1:
				    /* upscale y filter */
				    for(p=0;p<4;p++) {
					y[2*p]=(((*(tsp+p)&0x000000ff))*oneminbeta+((*(tsp2+p)&0x000000ff))*beta)>>8;
					y[2*p+1]=(((*(tsp+p)&0x00ff0000)>>16)*oneminbeta+((*(tsp2+p)&0x00ff0000)>>16)*beta)>>8;
					u[p]=(((*(tsp+p)&0x0000ff00)>>8)*oneminbeta+((*(tsp2+p)&0x0000ff00)>>8)*beta)>>8;
					v[p]=(((*(tsp+p)&0xff000000)>>24)*oneminbeta+((*(tsp2+p)&0xff000000)>>24)*beta)>>8;
				    }
				    break;
				    /* downscale y filter */
				case 2:
				case 3:
				case 4:
				default:
				    for(p=0;p<4;p++) {
					y[2*p]=(((*(tsp+p)&0x000000ff)));
					y[2*p+1]=(((*(tsp+p)&0x00ff0000)>>16));
					u[p]=(((*(tsp+p)&0x0000ff00)>>8));
					v[p]=(((*(tsp+p)&0xff000000)>>24));
				    }
				    break;
				}

				switch (xds) {
				case 1: /* simple copy */
				    *(fb_ptr++)=y[0]|y[1]<<16|u[0]<<8|v[0]<<24;
				    *(fb_ptr++)=y[2]|y[3]<<16|u[1]<<8|v[1]<<24;
				    *(fb_ptr++)=y[4]|y[5]<<16|u[2]<<8|v[2]<<24;
				    *(fb_ptr++)=y[6]|y[7]<<16|u[3]<<8|v[3]<<24;
				    break;
				case 2: /* dowscale by 2 */
				    ya[0]=(y[0]+y[1])>>1;
				    ya[1]=(y[2]+y[3])>>1;
				    ya[2]=(y[4]+y[5])>>1;
				    ya[3]=(y[6]+y[7])>>1;
				    ua[0]=(u[0]+u[1])>>1;
				    ua[1]=(u[2]+u[3])>>1;
				    va[0]=(v[0]+v[1])>>1;
				    va[1]=(v[2]+v[3])>>1;
				    *(fb_ptr++)=ya[0]|ya[1]<<16|ua[0]<<8|va[0]<<24;
				    *(fb_ptr++)=ya[2]|ya[3]<<16|ua[1]<<8|va[1]<<24;
				    break;
				case 4: /* downscale by 4 */
				    ya[0]=(y[0]+y[1]+y[2]+y[3])>>2;
				    ya[1]=(y[4]+y[5]+y[6]+y[7])>>2;
				    ua[0]=(u[0]+u[1]+u[2]+u[3])>>2;
				    va[0]=(v[0]+v[1]+v[2]+v[3])>>2;
				    *(fb_ptr++)=ya[0]|ya[1]<<16|ua[0]<<8|va[0]<<24;
				    break;
				case 8:
				default:
				    break;
				}

				/* fb_ptr+=4; */
				tsp+=4; tsp2+=4;
			    }
			}
#endif /* MGA2164_SWFILTER */
			break;
		    default:
			break;
		    }
		    pl=l;
		} else {
		    /* dup lines */

#ifdef DEBUG_MGA2164
		    sprintf(sbuf,"dup line: scr_src %d   scr_dst %d\n",
			    dl-1,dl);
		    xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);
#endif

		    OUTREG(MGAREG_DWGCTL, 0x040C6008);
		    OUTREG(MGAREG_FXBNDRY, pbox->x1|((pbox->x2-1)<<16) );      /* why -1 ? */
		    OUTREG(MGAREG_AR3, (pbox->y1+dl-1)*scr_pitch+pbox->x1 );   /* SRC LINE START*/
		    OUTREG(MGAREG_AR0, (pbox->y1+dl-1)*scr_pitch+pbox->x2 -1); /* SRC LINE END   why -1 ? */
		    OUTREG(MGAREG_AR5, scr_pitch);                             /* DST_Y_INCR = PITCH? */
		    OUTREG(MGAREG_YDST , pbox->y1+dl);                         /* Y_START_POS */
		    OUTREG(MGAREG_LEN + MGAREG_EXEC , 1);                      /* # of LINES */
		}
    }
    OUTREG(MGAREG_CXBNDRY, 0xFFFF0000);
}

static void MGACopyILOAD(
			 ScrnInfoPtr pScrn,
			 int id, unsigned char *buf,
			 BoxPtr pbox,
			 int width, int height, int pitch,
			 short src_x, short src_y,
			 short src_w, short src_h,
			 short drw_x, short drw_y,
			 short drw_w, short drw_h
			 )
{
    MGAPtr pMga = MGAPTR(pScrn);
    CARD32 *fb_ptr;
    CARD8  *ubuf, *vbuf;
    CARD32 *pu, *pv;
    int k,l;
    short clip_x1, clip_x2, tmp_w;

#ifdef DEBUG_MGA2164
    char sbuf[255];
  
    sprintf(sbuf,"---- PBOX: x1=%d y1=%d w=%d h=%d (x2=%d y2=%d)\n",
	    pbox->x1,pbox->y1,pbox->x2-pbox->x1,pbox->y2-pbox->y1,
	    pbox->x2,pbox->y2);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);
  
    sprintf(sbuf,"in src: src_x=%d src_y=%d src_w=%d src_h=%d\n",
	    src_x,src_y,src_w,src_h);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);
    sprintf(sbuf,"in drw: drw_x=%d drw_y=%d drw_w=%d drw_h=%d\n",
	    drw_x,drw_y,drw_w,drw_h);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);
#endif

    /* non-scaling yuv->rgb */

    /* hack to force width and src image to be 8 pixel aligned */
    src_x&=~0x7;
    src_w&=~0x7;
    drw_w&=~0x7;
    tmp_w=drw_w;
    clip_x1=drw_x;
    clip_x2=drw_x+drw_w;

    /* hack for clipping in non scaling version */
    /* this works only if no scaling */
    if(pbox->x1 > drw_x) {              /* left side X clipping*/
	src_x+=((pbox->x1-drw_x)&~0x7);
	src_w-=((pbox->x1-drw_x)&~0x7);
	clip_x1=pbox->x1;
	drw_x+=src_x;
	drw_w=src_w;
    }

    if( (pbox->x2) < (drw_x+drw_w) ) {     /* right side X clipping */
	tmp_w=( (pbox->x2) - drw_x );
	drw_w= tmp_w & (~0x7);
	if(drw_w!=tmp_w) drw_w+=8;
	clip_x2=drw_x+tmp_w-1; /* not sure why needs -1 */
	src_w=drw_w;
    }

    if(pbox->y1 > drw_y) {             /* top side Y clipping */
	src_y+=(pbox->y1-drw_y);
	src_h-=(pbox->y1-drw_y);
	drw_y+=src_y;
	drw_h=src_h;
    }
    if((pbox->y2)<(drw_y+drw_h)) {     /* bottom side Y clipping */
	drw_h=(pbox->y2)-drw_y;
	src_h=drw_h;
    }

    if(drw_x<0) drw_x=0;

#ifdef DEBUG_MGA2164
    sprintf(sbuf,"out src: src_x=%d src_y=%d src_w=%d src_h=%d\n",
	    src_x,src_y,src_w,src_h);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);
    sprintf(sbuf,"out drw: drw_x=%d drw_y=%d drw_w=%d drw_h=%d\n",
	    drw_x,drw_y,drw_w,drw_h);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf);
#endif

    /* ready to draw */
    if(drw_w==0||drw_h==0) return;

    if(drw_w<0||drw_h<0) {
	/* actually until scaling is working this might happen
	   during normal operation */
	/*  sprintf(sbuf,"drw_w or drw_h are negative (this should never
	    happen)\n");
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, sbuf); */
	return;
    }

    CHECK_DMA_QUIESCENT(pMga, pScrn);

    /* non scaling ILOAD */
    WAITFIFO(6);
    OUTREG(MGAREG_AR5, 0);
    OUTREG(MGAREG_DWGCTL, MGADWG_ILOAD | MGADWG_BUYUV | MGADWG_SHIFTZERO
	   | MGADWG_SGNZERO | 0xc0000);
    OUTREG(MGAREG_AR0, (drw_w)-1 );
    OUTREG(MGAREG_AR3, 0);
    OUTREG(MGAREG_CXBNDRY, clip_x1|(clip_x2<<16));
    OUTREG(MGAREG_FXBNDRY, drw_x|((drw_x+drw_w-1)<<16));
    OUTREG(MGAREG_YDSTLEN + MGAREG_EXEC , (drw_y<<16)|drw_h);

    fb_ptr=(CARD32 *)pMga->ILOADBase;
    vbuf=buf+width*height;
    ubuf=vbuf+width*height/4;

    switch(id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	pu = (CARD32 *)(ubuf+(src_y/2)*(width/2));
	pv = (CARD32 *)(vbuf+(src_y/2)*(width/2));
	buf+=src_y*width;

	for(l=0;l<drw_h;l++) {
	    CARD32 *tpu=pu+src_x/8;
	    CARD32 *tpv=pv+src_x/8;
	    CARD32 *tsp=(CARD32 *)(buf+src_x);

	    /* it is not clear if waiting is actually good for performance */
	    /*	WAITFIFO(pMga->FifoSize);*/
	    /* should try to get MGACopyMunged data to work here */
	    CopyMungedScanline_AXP(fb_ptr,src_w,tsp,tpu,tpv);
	    buf+=width;
	    if(l&1) {
		pu+=width/8;
		pv+=width/8;
	    }
	}
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
	buf+=src_y*width*2;
	for(l=0;l<drw_h;l++) {

#ifndef CUSTOM_MEMCOPY
	    WAITFIFO(pMga->FifoSize/2); /* not sure what's the value for best performance */
	    memcpy(fb_ptr, buf+src_x*2, src_w*2);
	    fb_ptr+=src_w*2;
#else
	    CARD32 *tsp=(CARD32 *)(buf+src_x*2);
	    WAITFIFO(pMga->FifoSize/4);
	    for(k=src_w/8;k;k--) {
		*(fb_ptr)=*(tsp);
		*(fb_ptr+1)=*(tsp+1);
		*(fb_ptr+2)=*(tsp+2);
		*(fb_ptr+3)=*(tsp+3);
		fb_ptr+=4; tsp+=4;
	    }
#endif /* CUSTOM_MEMCOPY */
	    buf+=width*2;
	}
	break;
    default:
	break;
    }
    OUTREG(MGAREG_CXBNDRY, 0xFFFF0000);    /* put clipping back to normal */
}

static int
MGAPutImageILOAD(
		 ScrnInfoPtr pScrn,
		 short src_x, short src_y,
		 short drw_x, short drw_y,
		 short src_w, short src_h,
		 short drw_w, short drw_h,
		 int id, unsigned char* buf,
		 short width, short height,
		 Bool Sync,
		 RegionPtr clipBoxes, pointer data,
		 DrawablePtr pDraw
		 ){
    MGAPtr pMga = MGAPTR(pScrn);
    MGAPortPrivPtr pPriv = pMga->portPrivate;
    INT32 x1, x2, y1, y2;
    int dstPitch = 0;
    int bpp;
    BoxRec dstBox;
    int nbox;
    BoxPtr pbox;

    /* Clip */
    x1 = src_x; x2 = src_x + src_w;
    y1 = src_y; y2 = src_y + src_h;

    dstBox.x1 = drw_x; dstBox.x2 = drw_x + drw_w;
    dstBox.y1 = drw_y; dstBox.y2 = drw_y + drw_h;

    if(!xf86XVClipVideoHelper(&dstBox, &x1, &x2, &y1, &y2,
			      clipBoxes, width, height))
	return Success;

    bpp = pScrn->bitsPerPixel >> 3;

#ifdef HAVE_XAA_H
    if( pMga->AccelInfoRec->NeedToSync && ((long)data != pPriv->lastPort) ) {
	MGAStormSync(pScrn);
    }
#endif

    pPriv->lastPort = (long)data;
    nbox=REGION_NUM_RECTS(clipBoxes);
    pbox=REGION_RECTS(clipBoxes);

    while(nbox--) {

	if ( (drw_w==src_w) && (drw_h==src_h) && (drw_x >= 0 ) ) {
	    /* special case 1: non scaling optimization */
	    MGACopyILOAD(pScrn,id,buf,pbox,
			 width, height, dstPitch, src_x, src_y, src_w, src_h,
			 drw_x, drw_y, drw_w, drw_h);
#if 0
	    } else if ( (drw_w>src_w) && (drw_h>src_h) && (drw_x >= 0 ) ) {
		/* special case 2: upscaling for full screen apps */
		/* FIXME: to do */
		MGACopyScaledILOAD(pScrn,id,buf,pbox,
				   width, height, dstPitch, src_x, src_y, src_w, src_h,
				   drw_x, drw_y, drw_w, drw_h);

#endif
	    } else /* generic fallback case */
		MGACopyScaledILOAD(pScrn,id,buf,pbox,
				   width, height, dstPitch, src_x, src_y, src_w, src_h,
				   drw_x, drw_y, drw_w, drw_h);
	/* FIXME: when the generic is perfect I will enable the optimizations */
	pbox++;
    }

#ifdef HAVE_XAA_H
    pMga->AccelInfoRec->NeedToSync = TRUE;
#endif
    pPriv->videoStatus = FREE_TIMER;
    pPriv->freeTime = currentTime.milliseconds + FREE_DELAY;
    pMga->VideoTimerCallback = MGAVideoTimerCallback;

    return Success;
}
