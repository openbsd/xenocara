/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/ati/radeon_video.c,v 1.30 2003/11/10 18:22:18 tsi Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_macros.h"
#include "radeon_probe.h"
#include "radeon_mergedfb.h"
#include "radeon_video.h"

#include "xf86.h"
#include "dixstruct.h"
#include "atipciids.h"
#include "xf86fbman.h"

#include <X11/extensions/Xv.h>
#include "fourcc.h"

#include "theatre_detect.h"
#include "theatre_reg.h"
#include "fi1236.h"
#include "msp3430.h"
#include "tda9885.h"

#define OFF_DELAY       250  /* milliseconds */
#define FREE_DELAY      15000

#define OFF_TIMER       0x01
#define FREE_TIMER      0x02
#define CLIENT_VIDEO_ON 0x04

#define TIMER_MASK      (OFF_TIMER | FREE_TIMER)

/* capture config constants */
#define BUF_TYPE_FIELD          0
#define BUF_TYPE_ALTERNATING    1
#define BUF_TYPE_FRAME          2


#define BUF_MODE_SINGLE         0
#define BUF_MODE_DOUBLE         1
#define BUF_MODE_TRIPLE         2
/* CAP0_CONFIG values */

#define FORMAT_BROOKTREE        0
#define FORMAT_CCIR656          1
#define FORMAT_ZV               2
#define FORMAT_VIP16            3
#define FORMAT_TRANSPORT        4

#define ENABLE_RADEON_CAPTURE_WEAVE (RADEON_CAP0_CONFIG_CONTINUOS \
                        | (BUF_MODE_DOUBLE <<7) \
                        | (BUF_TYPE_FRAME << 4) \
                        | ( (pPriv->theatre !=NULL)?(FORMAT_CCIR656<<23):(FORMAT_BROOKTREE<<23)) \
                        | RADEON_CAP0_CONFIG_HORZ_DECIMATOR \
                        | (pPriv->capture_vbi_data ? RADEON_CAP0_CONFIG_VBI_EN : 0) \
                        | RADEON_CAP0_CONFIG_VIDEO_IN_VYUY422)

#define ENABLE_RADEON_CAPTURE_BOB (RADEON_CAP0_CONFIG_CONTINUOS \
                        | (BUF_MODE_SINGLE <<7)  \
                        | (BUF_TYPE_ALTERNATING << 4) \
                        | ( (pPriv->theatre !=NULL)?(FORMAT_CCIR656<<23):(FORMAT_BROOKTREE<<23)) \
                        | RADEON_CAP0_CONFIG_HORZ_DECIMATOR \
                        | (pPriv->capture_vbi_data ? RADEON_CAP0_CONFIG_VBI_EN : 0) \
                        | RADEON_CAP0_CONFIG_VIDEO_IN_VYUY422)


static void RADEONInitOffscreenImages(ScreenPtr);

static XF86VideoAdaptorPtr RADEONSetupImageVideo(ScreenPtr);
static int  RADEONSetPortAttribute(ScrnInfoPtr, Atom, INT32, pointer);
static int  RADEONGetPortAttribute(ScrnInfoPtr, Atom ,INT32 *, pointer);
static void RADEONStopVideo(ScrnInfoPtr, pointer, Bool);
static void RADEONQueryBestSize(ScrnInfoPtr, Bool, short, short, short, short,
			unsigned int *, unsigned int *, pointer);
static int  RADEONPutImage(ScrnInfoPtr, short, short, short, short, short,
			short, short, short, int, unsigned char*, short,
			short, Bool, RegionPtr, pointer,
			DrawablePtr);
static int  RADEONQueryImageAttributes(ScrnInfoPtr, int, unsigned short *,
			unsigned short *,  int *, int *);
static void RADEONFreeMemory(ScrnInfoPtr pScrn, void *mem_struct);

static void RADEONVideoTimerCallback(ScrnInfoPtr pScrn, Time now);
static int RADEONPutVideo(ScrnInfoPtr pScrn, short src_x, short src_y, short drw_x, short drw_y,
                        short src_w, short src_h, short drw_w, short drw_h, 
			RegionPtr clipBoxes, pointer data, DrawablePtr pDraw);

static void RADEON_board_setmisc(RADEONPortPrivPtr pPriv);
static void RADEON_RT_SetEncoding(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv);
static void RADEON_MSP_SetEncoding(RADEONPortPrivPtr pPriv);
static void RADEON_TDA9885_SetEncoding(RADEONPortPrivPtr pPriv);
static void RADEON_FI1236_SetEncoding(RADEONPortPrivPtr pPriv);


#define MAKE_ATOM(a) MakeAtom(a, sizeof(a) - 1, TRUE)
#define ClipValue(v,min,max) ((v) < (min) ? (min) : (v) > (max) ? (max) : (v))

static Atom xvBrightness, xvColorKey, xvSaturation, xvDoubleBuffer;
static Atom xvRedIntensity, xvGreenIntensity, xvBlueIntensity;
static Atom xvContrast, xvHue, xvColor, xvAutopaintColorkey, xvSetDefaults;
static Atom xvGamma, xvColorspace;
static Atom xvSwitchCRT;
static Atom xvEncoding, xvFrequency, xvVolume, xvMute,
	     xvDecBrightness, xvDecContrast, xvDecHue, xvDecColor, xvDecSaturation,
	     xvTunerStatus, xvSAP, xvOverlayDeinterlacingMethod,
	     xvLocationID, xvDeviceID, xvInstanceID, xvDumpStatus,
	     xvAdjustment;
	     
static Atom xvOvAlpha, xvGrAlpha, xvAlphaMode;


#define GET_PORT_PRIVATE(pScrn) \
   (RADEONPortPrivPtr)((RADEONPTR(pScrn))->adaptor->pPortPrivates[0].ptr)


#ifdef USE_EXA
static void
ATIVideoSave(ScreenPtr pScreen, ExaOffscreenArea *area)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONPortPrivPtr pPriv = info->adaptor->pPortPrivates[0].ptr;

    if (pPriv->video_memory == area)
        pPriv->video_memory = NULL;
}
#endif /* USE_EXA */

void RADEONInitVideo(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    XF86VideoAdaptorPtr *adaptors, *newAdaptors = NULL;
    XF86VideoAdaptorPtr newAdaptor = NULL;
    int num_adaptors;

    newAdaptor = RADEONSetupImageVideo(pScreen);
    RADEONInitOffscreenImages(pScreen);
    num_adaptors = xf86XVListGenericAdaptors(pScrn, &adaptors);

    if(newAdaptor) {
	if(!num_adaptors) {
	    num_adaptors = 1;
	    adaptors = &newAdaptor;
	} else {
	    newAdaptors =  /* need to free this someplace */
		xalloc((num_adaptors + 1) * sizeof(XF86VideoAdaptorPtr*));
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

    if(newAdaptors)
	xfree(newAdaptors);
}

/* client libraries expect an encoding */
static XF86VideoEncodingRec DummyEncoding =
{
   0,
   "XV_IMAGE",
   2048, 2048,
   {1, 1}
};

 /* the picture is interlaced - hence the half-heights */

static XF86VideoEncodingRec
InputVideoEncodings[] =
{
    { 0, "XV_IMAGE",			2048,2048,{1,1}},        
    { 1, "pal-composite",		720, 288, { 1, 50 }},
    { 2, "pal-tuner",			720, 288, { 1, 50 }},
    { 3, "pal-svideo",			720, 288, { 1, 50 }},
    { 4, "ntsc-composite",		640, 240, { 1001, 60000 }},
    { 5, "ntsc-tuner",			640, 240, { 1001, 60000 }},
    { 6, "ntsc-svideo",			640, 240, { 1001, 60000 }},
    { 7, "secam-composite",		720, 288, { 1, 50 }},
    { 8, "secam-tuner",			720, 288, { 1, 50 }},
    { 9, "secam-svideo",		720, 288, { 1, 50 }},
    { 10,"pal_60-composite",		768, 288, { 1, 50 }},
    { 11,"pal_60-tuner",		768, 288, { 1, 50 }},
    { 12,"pal_60-svideo",		768, 288, { 1, 50 }}
};


#define NUM_FORMATS 12

static XF86VideoFormatRec Formats[NUM_FORMATS] =
{
   {8, TrueColor}, {8, DirectColor}, {8, PseudoColor},
   {8, GrayScale}, {8, StaticGray}, {8, StaticColor},
   {15, TrueColor}, {16, TrueColor}, {24, TrueColor},
   {15, DirectColor}, {16, DirectColor}, {24, DirectColor}
};


#if 0
#define NUM_ATTRIBUTES 9+6

static XF86AttributeRec Attributes[NUM_ATTRIBUTES] =
{
   {XvSettable             ,     0,    1, "XV_SET_DEFAULTS"},
   {XvSettable | XvGettable,     0,    1, "XV_AUTOPAINT_COLORKEY"},
   {XvSettable | XvGettable,     0,   ~0, "XV_COLORKEY"},
   {XvSettable | XvGettable,     0,    1, "XV_DOUBLE_BUFFER"},
   {XvSettable | XvGettable, -1000, 1000, "XV_BRIGHTNESS"},
   {XvSettable | XvGettable, -1000, 1000, "XV_CONTRAST"},
   {XvSettable | XvGettable, -1000, 1000, "XV_SATURATION"},
   {XvSettable | XvGettable, -1000, 1000, "XV_COLOR"},
   {XvSettable | XvGettable, -1000, 1000, "XV_HUE"},
   {XvSettable | XvGettable, -1000, 1000, "XV_RED_INTENSITY"},
   {XvSettable | XvGettable, -1000, 1000, "XV_GREEN_INTENSITY"},
   {XvSettable | XvGettable, -1000, 1000, "XV_BLUE_INTENSITY"},
   {XvSettable | XvGettable,     0,    1, "XV_SWITCHCRT"},
   {XvSettable | XvGettable,   100, 10000, "XV_GAMMA"},
   {XvSettable | XvGettable,     0,    1, "XV_COLORSPACE"},
};

#endif

#define NUM_ATTRIBUTES 22
#define NUM_DEC_ATTRIBUTES (NUM_ATTRIBUTES+12)

static XF86AttributeRec Attributes[NUM_DEC_ATTRIBUTES+1] =
{
   {             XvGettable, 0, ~0, "XV_DEVICE_ID"},
   {             XvGettable, 0, ~0, "XV_LOCATION_ID"},
   {             XvGettable, 0, ~0, "XV_INSTANCE_ID"},
   {XvSettable		   , 0, 1, "XV_DUMP_STATUS"},
   {XvSettable             , 0, 1, "XV_SET_DEFAULTS"},
   {XvSettable | XvGettable, 0, 1, "XV_AUTOPAINT_COLORKEY"},
   {XvSettable | XvGettable, 0, ~0,"XV_COLORKEY"},
   {XvSettable | XvGettable, 0, 1, "XV_DOUBLE_BUFFER"},
   {XvSettable | XvGettable,     0,  255, "XV_OVERLAY_ALPHA"},
   {XvSettable | XvGettable,     0,  255, "XV_GRAPHICS_ALPHA"},
   {XvSettable | XvGettable,     0,    1, "XV_ALPHA_MODE"},
   {XvSettable | XvGettable, -1000, 1000, "XV_BRIGHTNESS"},
   {XvSettable | XvGettable, -1000, 1000, "XV_CONTRAST"},
   {XvSettable | XvGettable, -1000, 1000, "XV_SATURATION"},
   {XvSettable | XvGettable, -1000, 1000, "XV_COLOR"},
   {XvSettable | XvGettable, -1000, 1000, "XV_HUE"},
   {XvSettable | XvGettable, -1000, 1000, "XV_RED_INTENSITY"},
   {XvSettable | XvGettable, -1000, 1000, "XV_GREEN_INTENSITY"},
   {XvSettable | XvGettable, -1000, 1000, "XV_BLUE_INTENSITY"},
   {XvSettable | XvGettable,     0,    1, "XV_SWITCHCRT"},
   {XvSettable | XvGettable,   100, 10000, "XV_GAMMA"},
   {XvSettable | XvGettable,     0,    1, "XV_COLORSPACE"},
   
   {XvSettable | XvGettable, -1000, 1000, "XV_DEC_BRIGHTNESS"},
   {XvSettable | XvGettable, -1000, 1000, "XV_DEC_CONTRAST"},
   {XvSettable | XvGettable, -1000, 1000, "XV_DEC_SATURATION"},
   {XvSettable | XvGettable, -1000, 1000, "XV_DEC_HUE"},
   {XvSettable | XvGettable, 0, 2, "XV_OVERLAY_DEINTERLACING_METHOD"},
   {XvSettable | XvGettable, 0, 12, "XV_ENCODING"},
   {XvSettable | XvGettable, 0, -1, "XV_FREQ"},
   {             XvGettable, -1000, 1000, "XV_TUNER_STATUS"},
   {XvSettable | XvGettable, -1000, 1000, "XV_VOLUME"},
   {XvSettable | XvGettable, 0, 1, "XV_MUTE"},
   {XvSettable | XvGettable, 0, 1, "XV_SAP"},
   {XvSettable | XvGettable, 0, 0x1F, "XV_DEBUG_ADJUSTMENT"},   
   { 0, 0, 0, NULL}  /* just a place holder so I don't have to be fancy with commas */
};


#define INCLUDE_RGB_FORMATS 1

#if INCLUDE_RGB_FORMATS

#define NUM_IMAGES 8

/* Note: GUIDs are bogus... - but nothing uses them anyway */

#define FOURCC_RGBA32   0x41424752

#define XVIMAGE_RGBA32(byte_order)   \
        { \
                FOURCC_RGBA32, \
                XvRGB, \
                byte_order, \
                { 'R', 'G', 'B', 'A', \
                  0x00,0x00,0x00,0x10,0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}, \
                32, \
                XvPacked, \
                1, \
                32, 0x00FF0000, 0x0000FF00, 0x000000FF, \
                0, 0, 0, 0, 0, 0, 0, 0, 0, \
                {'A', 'R', 'G', 'B', \
                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, \
                XvTopToBottom \
        }               

#define FOURCC_RGB24    0x00000000

#define XVIMAGE_RGB24(byte_order)   \
        { \
                FOURCC_RGB24, \
                XvRGB, \
                byte_order, \
                { 'R', 'G', 'B', 0, \
                  0x00,0x00,0x00,0x10,0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}, \
                24, \
                XvPacked, \
                1, \
                24, 0x00FF0000, 0x0000FF00, 0x000000FF, \
                0, 0, 0, 0, 0, 0, 0, 0, 0, \
                { 'R', 'G', 'B', \
                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, \
                XvTopToBottom \
        }

#define FOURCC_RGBT16   0x54424752

#define XVIMAGE_RGBT16(byte_order)   \
        { \
                FOURCC_RGBT16, \
                XvRGB, \
                byte_order, \
                { 'R', 'G', 'B', 'T', \
                  0x00,0x00,0x00,0x10,0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}, \
                16, \
                XvPacked, \
                1, \
                16, 0x00007C00, 0x000003E0, 0x0000001F, \
                0, 0, 0, 0, 0, 0, 0, 0, 0, \
                {'A', 'R', 'G', 'B', \
                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, \
                XvTopToBottom \
        }               

#define FOURCC_RGB16    0x32424752

#define XVIMAGE_RGB16(byte_order)   \
        { \
                FOURCC_RGB16, \
                XvRGB, \
                byte_order, \
                { 'R', 'G', 'B', 0x00, \
                  0x00,0x00,0x00,0x10,0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}, \
                16, \
                XvPacked, \
                1, \
                16, 0x0000F800, 0x000007E0, 0x0000001F, \
                0, 0, 0, 0, 0, 0, 0, 0, 0, \
                {'R', 'G', 'B', \
                  0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, \
                XvTopToBottom \
        }               

static XF86ImageRec Images[NUM_IMAGES] =
{
#if X_BYTE_ORDER == X_BIG_ENDIAN
        XVIMAGE_RGBA32(MSBFirst),
        XVIMAGE_RGB24(MSBFirst),
        XVIMAGE_RGBT16(MSBFirst),
        XVIMAGE_RGB16(MSBFirst),
#else
        XVIMAGE_RGBA32(LSBFirst),
        XVIMAGE_RGB24(LSBFirst),
        XVIMAGE_RGBT16(LSBFirst),
        XVIMAGE_RGB16(LSBFirst),
#endif
        XVIMAGE_YUY2,
        XVIMAGE_UYVY,
        XVIMAGE_YV12,
        XVIMAGE_I420
};

#else

#define NUM_IMAGES 4

static XF86ImageRec Images[NUM_IMAGES] =
{
    XVIMAGE_YUY2,
    XVIMAGE_UYVY,
    XVIMAGE_YV12,
    XVIMAGE_I420
};

#endif

/* Reference color space transform data */
typedef struct tagREF_TRANSFORM
{
    float   RefLuma;
    float   RefRCb;
    float   RefRCr;
    float   RefGCb;
    float   RefGCr;
    float   RefBCb;
    float   RefBCr;
} REF_TRANSFORM;

/* Parameters for ITU-R BT.601 and ITU-R BT.709 colour spaces */
static REF_TRANSFORM trans[2] =
{
    {1.1678, 0.0, 1.6007, -0.3929, -0.8154, 2.0232, 0.0}, /* BT.601 */
    {1.1678, 0.0, 1.7980, -0.2139, -0.5345, 2.1186, 0.0}  /* BT.709 */
};


/* Gamma curve definition for preset gammas */
typedef struct tagGAMMA_CURVE_R100
{
    CARD32 GAMMA_0_F_SLOPE;
    CARD32 GAMMA_0_F_OFFSET;
    CARD32 GAMMA_10_1F_SLOPE;
    CARD32 GAMMA_10_1F_OFFSET;
    CARD32 GAMMA_20_3F_SLOPE;
    CARD32 GAMMA_20_3F_OFFSET;
    CARD32 GAMMA_40_7F_SLOPE;
    CARD32 GAMMA_40_7F_OFFSET;
    CARD32 GAMMA_380_3BF_SLOPE;
    CARD32 GAMMA_380_3BF_OFFSET;
    CARD32 GAMMA_3C0_3FF_SLOPE;
    CARD32 GAMMA_3C0_3FF_OFFSET;
    float OvGammaCont;
} GAMMA_CURVE_R100;

typedef struct tagGAMMA_CURVE_R200
{
    CARD32 GAMMA_0_F_SLOPE;
    CARD32 GAMMA_0_F_OFFSET;
    CARD32 GAMMA_10_1F_SLOPE;
    CARD32 GAMMA_10_1F_OFFSET;
    CARD32 GAMMA_20_3F_SLOPE;
    CARD32 GAMMA_20_3F_OFFSET;
    CARD32 GAMMA_40_7F_SLOPE;
    CARD32 GAMMA_40_7F_OFFSET;
    CARD32 GAMMA_80_BF_SLOPE;
    CARD32 GAMMA_80_BF_OFFSET;
    CARD32 GAMMA_C0_FF_SLOPE;
    CARD32 GAMMA_C0_FF_OFFSET;
    CARD32 GAMMA_100_13F_SLOPE;
    CARD32 GAMMA_100_13F_OFFSET;
    CARD32 GAMMA_140_17F_SLOPE;
    CARD32 GAMMA_140_17F_OFFSET;
    CARD32 GAMMA_180_1BF_SLOPE;
    CARD32 GAMMA_180_1BF_OFFSET;
    CARD32 GAMMA_1C0_1FF_SLOPE;
    CARD32 GAMMA_1C0_1FF_OFFSET;
    CARD32 GAMMA_200_23F_SLOPE;
    CARD32 GAMMA_200_23F_OFFSET;
    CARD32 GAMMA_240_27F_SLOPE;
    CARD32 GAMMA_240_27F_OFFSET;
    CARD32 GAMMA_280_2BF_SLOPE;
    CARD32 GAMMA_280_2BF_OFFSET;
    CARD32 GAMMA_2C0_2FF_SLOPE;
    CARD32 GAMMA_2C0_2FF_OFFSET;
    CARD32 GAMMA_300_33F_SLOPE;
    CARD32 GAMMA_300_33F_OFFSET;
    CARD32 GAMMA_340_37F_SLOPE;
    CARD32 GAMMA_340_37F_OFFSET;
    CARD32 GAMMA_380_3BF_SLOPE;
    CARD32 GAMMA_380_3BF_OFFSET;
    CARD32 GAMMA_3C0_3FF_SLOPE;
    CARD32 GAMMA_3C0_3FF_OFFSET;
    float OvGammaCont;
} GAMMA_CURVE_R200;


/* Preset gammas */
static GAMMA_CURVE_R100 gamma_curve_r100[8] = 
{
	/* Gamma 1.0 */
	{0x100, 0x0, 
	 0x100, 0x20, 
	 0x100, 0x40, 
	 0x100, 0x80, 
	 0x100, 0x100, 
	 0x100, 0x100, 
	 1.0},
	/* Gamma 0.85 */
	{0x75,  0x0, 
	 0xA2,  0xF,  
	 0xAC,  0x23, 
	 0xC6,  0x4E, 
	 0x129, 0xD6, 
	 0x12B, 0xD5, 
	 1.0},
	/* Gamma 1.1 */
	{0x180, 0x0, 
	 0x13C, 0x30, 
	 0x13C, 0x57, 
	 0x123, 0xA5, 
	 0xEA,  0x116, 
	 0xEA, 0x116, 
	 0.9913},
	/* Gamma 1.2 */
	{0x21B, 0x0, 
	 0x16D, 0x43, 
	 0x172, 0x71, 
	 0x13D, 0xCD, 
	 0xD9,  0x128, 
	 0xD6, 0x12A, 
	 0.9827},
	/* Gamma 1.45 */
	{0x404, 0x0, 
	 0x1B9, 0x81, 
	 0x1EE, 0xB8, 
	 0x16A, 0x133, 
	 0xB7, 0x14B, 
	 0xB2, 0x14E, 
	 0.9567},
	/* Gamma 1.7 */
	{0x658, 0x0, 
	 0x1B5, 0xCB, 
	 0x25F, 0x102, 
	 0x181, 0x199, 
	 0x9C,  0x165, 
	 0x98, 0x167, 
	 0.9394},
	/* Gamma 2.2 */
	{0x7FF, 0x0, 
	 0x625, 0x100, 
	 0x1E4, 0x1C4, 
	 0x1BD, 0x23D, 
	 0x79,  0x187, 
	 0x76,  0x188, 
	 0.9135},
	/* Gamma 2.5 */
	{0x7FF, 0x0, 
	 0x7FF, 0x100, 
	 0x2AD, 0x200, 
	 0x1A2, 0x2AB, 
	 0x6E,  0x194, 
	 0x67,  0x197, 
	 0.9135}
};

static GAMMA_CURVE_R200 gamma_curve_r200[8] =
 {
	/* Gamma 1.0 */
      {0x00000040, 0x00000000,
       0x00000040, 0x00000020,
       0x00000080, 0x00000040,
       0x00000100, 0x00000080,
       0x00000100, 0x00000100,
       0x00000100, 0x00000100,
       0x00000100, 0x00000200,
       0x00000100, 0x00000200,
       0x00000100, 0x00000300,
       0x00000100, 0x00000300,
       0x00000100, 0x00000400,
       0x00000100, 0x00000400,
       0x00000100, 0x00000500,
       0x00000100, 0x00000500,
       0x00000100, 0x00000600,
       0x00000100, 0x00000600,
       0x00000100, 0x00000700,
       0x00000100, 0x00000700,
       1.0},
	/* Gamma 0.85 */
      {0x0000001D, 0x00000000,
       0x00000028, 0x0000000F,
       0x00000056, 0x00000023,
       0x000000C5, 0x0000004E,
       0x000000DA, 0x000000B0,
       0x000000E6, 0x000000AA,
       0x000000F1, 0x00000190,
       0x000000F9, 0x0000018C,
       0x00000101, 0x00000286,
       0x00000108, 0x00000282,
       0x0000010D, 0x0000038A,
       0x00000113, 0x00000387,
       0x00000118, 0x0000049A,
       0x0000011C, 0x00000498,
       0x00000120, 0x000005B4,
       0x00000124, 0x000005B2,
       0x00000128, 0x000006D6,
       0x0000012C, 0x000006D5,
       1.0},
	/* Gamma 1.1 */
      {0x00000060, 0x00000000,
       0x0000004F, 0x00000030,
       0x0000009C, 0x00000057,
       0x00000121, 0x000000A5,
       0x00000113, 0x00000136,
       0x0000010B, 0x0000013A,
       0x00000105, 0x00000245,
       0x00000100, 0x00000247,
       0x000000FD, 0x00000348,
       0x000000F9, 0x00000349,
       0x000000F6, 0x00000443,
       0x000000F4, 0x00000444,
       0x000000F2, 0x00000538,
       0x000000F0, 0x00000539,
       0x000000EE, 0x00000629,
       0x000000EC, 0x00000629,
       0x000000EB, 0x00000716,
       0x000000E9, 0x00000717,
       0.9913},
	/* Gamma 1.2 */
      {0x00000087, 0x00000000,
       0x0000005B, 0x00000043,
       0x000000B7, 0x00000071,
       0x0000013D, 0x000000CD,
       0x00000121, 0x0000016B,
       0x00000113, 0x00000172,
       0x00000107, 0x00000286,
       0x000000FF, 0x0000028A,
       0x000000F8, 0x00000389,
       0x000000F2, 0x0000038B,
       0x000000ED, 0x0000047D,
       0x000000E9, 0x00000480,
       0x000000E5, 0x00000568,
       0x000000E1, 0x0000056A,
       0x000000DE, 0x0000064B,
       0x000000DB, 0x0000064D,
       0x000000D9, 0x00000728,
       0x000000D6, 0x00000729,
       0.9827},
	/* Gamma 1.45 */
      {0x00000101, 0x00000000,
       0x0000006E, 0x00000081,
       0x000000F7, 0x000000B8,
       0x0000016E, 0x00000133,
       0x00000139, 0x000001EA,
       0x0000011B, 0x000001F9,
       0x00000105, 0x00000314,
       0x000000F6, 0x0000031C,
       0x000000E9, 0x00000411,
       0x000000DF, 0x00000417,
       0x000000D7, 0x000004F6,
       0x000000CF, 0x000004F9,
       0x000000C9, 0x000005C9,
       0x000000C4, 0x000005CC,
       0x000000BF, 0x0000068F,
       0x000000BA, 0x00000691,
       0x000000B6, 0x0000074B,
       0x000000B2, 0x0000074D,
       0.9567},
	/* Gamma 1.7 */
      {0x00000196, 0x00000000,
       0x0000006D, 0x000000CB,
       0x0000012F, 0x00000102,
       0x00000187, 0x00000199,
       0x00000144, 0x0000025b,
       0x00000118, 0x00000273,
       0x000000FE, 0x0000038B,
       0x000000E9, 0x00000395,
       0x000000DA, 0x0000047E,
       0x000000CE, 0x00000485,
       0x000000C3, 0x00000552,
       0x000000BB, 0x00000556,
       0x000000B3, 0x00000611,
       0x000000AC, 0x00000614,
       0x000000A7, 0x000006C1,
       0x000000A1, 0x000006C3,
       0x0000009D, 0x00000765,
       0x00000098, 0x00000767,
       0.9394},
	/* Gamma 2.2 */
      {0x000001FF, 0x00000000,
       0x0000018A, 0x00000100,
       0x000000F1, 0x000001C5,
       0x000001D6, 0x0000023D,
       0x00000124, 0x00000328,
       0x00000116, 0x0000032F,
       0x000000E2, 0x00000446,
       0x000000D3, 0x0000044D,
       0x000000BC, 0x00000520,
       0x000000B0, 0x00000526,
       0x000000A4, 0x000005D6,
       0x0000009B, 0x000005DB,
       0x00000092, 0x00000676,
       0x0000008B, 0x00000679,
       0x00000085, 0x00000704,
       0x00000080, 0x00000707,
       0x0000007B, 0x00000787,
       0x00000076, 0x00000789,
       0.9135},
	/* Gamma 2.5 */
      {0x000001FF, 0x00000000,
       0x000001FF, 0x00000100,
       0x00000159, 0x000001FF,
       0x000001AC, 0x000002AB,
       0x0000012F, 0x00000381,
       0x00000101, 0x00000399,
       0x000000D9, 0x0000049A,
       0x000000C3, 0x000004A5,
       0x000000AF, 0x00000567,
       0x000000A1, 0x0000056E,
       0x00000095, 0x00000610,
       0x0000008C, 0x00000614,
       0x00000084, 0x000006A0,
       0x0000007D, 0x000006A4,
       0x00000077, 0x00000721,
       0x00000071, 0x00000723,
       0x0000006D, 0x00000795,
       0x00000068, 0x00000797,
       0.9135}
};

static void
RADEONSetOverlayGamma(ScrnInfoPtr pScrn, CARD32 gamma)
{
    RADEONInfoPtr    info = RADEONPTR(pScrn);
    unsigned char   *RADEONMMIO = info->MMIO;
    CARD32	    ov0_scale_cntl;

    /* Set gamma */
    RADEONWaitForIdleMMIO(pScrn);
    ov0_scale_cntl = INREG(RADEON_OV0_SCALE_CNTL) & ~RADEON_SCALER_GAMMA_SEL_MASK;
    OUTREG(RADEON_OV0_SCALE_CNTL, ov0_scale_cntl | (gamma << 0x00000005));

    /* Load gamma curve adjustments */
    if (info->ChipFamily >= CHIP_FAMILY_R200) {
    	OUTREG(RADEON_OV0_GAMMA_000_00F,
	    (gamma_curve_r200[gamma].GAMMA_0_F_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_0_F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_010_01F,
	    (gamma_curve_r200[gamma].GAMMA_10_1F_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_10_1F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_020_03F,
	    (gamma_curve_r200[gamma].GAMMA_20_3F_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_20_3F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_040_07F,
	    (gamma_curve_r200[gamma].GAMMA_40_7F_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_40_7F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_080_0BF,
	    (gamma_curve_r200[gamma].GAMMA_80_BF_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_80_BF_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_0C0_0FF,
	    (gamma_curve_r200[gamma].GAMMA_C0_FF_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_C0_FF_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_100_13F,
	    (gamma_curve_r200[gamma].GAMMA_100_13F_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_100_13F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_140_17F,
	    (gamma_curve_r200[gamma].GAMMA_140_17F_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_140_17F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_180_1BF,
	    (gamma_curve_r200[gamma].GAMMA_180_1BF_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_180_1BF_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_1C0_1FF,
	    (gamma_curve_r200[gamma].GAMMA_1C0_1FF_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_1C0_1FF_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_200_23F,
	    (gamma_curve_r200[gamma].GAMMA_200_23F_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_200_23F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_240_27F,
	    (gamma_curve_r200[gamma].GAMMA_240_27F_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_240_27F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_280_2BF,
	    (gamma_curve_r200[gamma].GAMMA_280_2BF_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_280_2BF_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_2C0_2FF,
	    (gamma_curve_r200[gamma].GAMMA_2C0_2FF_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_2C0_2FF_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_300_33F,
	    (gamma_curve_r200[gamma].GAMMA_300_33F_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_300_33F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_340_37F,
	    (gamma_curve_r200[gamma].GAMMA_340_37F_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_340_37F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_380_3BF,
	    (gamma_curve_r200[gamma].GAMMA_380_3BF_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_380_3BF_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_3C0_3FF,
	    (gamma_curve_r200[gamma].GAMMA_3C0_3FF_OFFSET << 0x00000000) |
	    (gamma_curve_r200[gamma].GAMMA_3C0_3FF_SLOPE << 0x00000010));
    } else {
    	OUTREG(RADEON_OV0_GAMMA_000_00F,
	    (gamma_curve_r100[gamma].GAMMA_0_F_OFFSET << 0x00000000) |
	    (gamma_curve_r100[gamma].GAMMA_0_F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_010_01F,
	    (gamma_curve_r100[gamma].GAMMA_10_1F_OFFSET << 0x00000000) |
	    (gamma_curve_r100[gamma].GAMMA_10_1F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_020_03F,
	    (gamma_curve_r100[gamma].GAMMA_20_3F_OFFSET << 0x00000000) |
	    (gamma_curve_r100[gamma].GAMMA_20_3F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_040_07F,
	    (gamma_curve_r100[gamma].GAMMA_40_7F_OFFSET << 0x00000000) |
	    (gamma_curve_r100[gamma].GAMMA_40_7F_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_380_3BF,
	    (gamma_curve_r100[gamma].GAMMA_380_3BF_OFFSET << 0x00000000) |
	    (gamma_curve_r100[gamma].GAMMA_380_3BF_SLOPE << 0x00000010));
    	OUTREG(RADEON_OV0_GAMMA_3C0_3FF,
	    (gamma_curve_r100[gamma].GAMMA_3C0_3FF_OFFSET << 0x00000000) |
	    (gamma_curve_r100[gamma].GAMMA_3C0_3FF_SLOPE << 0x00000010));
    }

}


/****************************************************************************
 * SetTransform                                                             *
 *  Function: Calculates and sets color space transform from supplied       *
 *            reference transform, gamma, brightness, contrast, hue and     *
 *            saturation.                                                   *
 *    Inputs: bright - brightness                                           *
 *            cont - contrast                                               *
 *            sat - saturation                                              *
 *            hue - hue                                                     *
 *            red_intensity - intensity of red component                    *
 *            green_intensity - intensity of green component                *
 *            blue_intensity - intensity of blue component                  *
 *            ref - index to the table of refernce transforms               *
 *            user_gamma - gamma value x 1000 (e.g., 1200 = gamma of 1.2)   *
 *   Outputs: NONE                                                          *
 ****************************************************************************/

static void RADEONSetTransform (ScrnInfoPtr pScrn,
				float	    bright,
				float	    cont,
				float	    sat,
				float	    hue,
				float	    red_intensity,
				float	    green_intensity,
				float	    blue_intensity,
				CARD32	    ref,
				CARD32      user_gamma)
{
    RADEONInfoPtr    info = RADEONPTR(pScrn);
    unsigned char   *RADEONMMIO = info->MMIO;
    float	    OvHueSin, OvHueCos;
    float	    CAdjLuma, CAdjOff;
    float	    CAdjRCb, CAdjRCr;
    float	    CAdjGCb, CAdjGCr;
    float	    CAdjBCb, CAdjBCr;
    float	    RedAdj,GreenAdj,BlueAdj;
    float	    OvLuma, OvROff, OvGOff, OvBOff;
    float	    OvRCb, OvRCr;
    float	    OvGCb, OvGCr;
    float	    OvBCb, OvBCr;
    float	    Loff = 64.0;
    float	    Coff = 512.0f;

    CARD32	    dwOvLuma, dwOvROff, dwOvGOff, dwOvBOff;
    CARD32	    dwOvRCb, dwOvRCr;
    CARD32	    dwOvGCb, dwOvGCr;
    CARD32	    dwOvBCb, dwOvBCr;
    CARD32	    gamma = 0;

    if (ref >= 2)
	return;

    /* translate from user_gamma (gamma x 1000) to radeon gamma table index value */
    if (user_gamma <= 925)       /* 0.85 */
	gamma = 1;
    else if (user_gamma <= 1050) /* 1.0  */
	gamma = 0;
    else if (user_gamma <= 1150) /* 1.1  */
	gamma = 2;
    else if (user_gamma <= 1325) /* 1.2  */
	gamma = 3;
    else if (user_gamma <= 1575) /* 1.45 */
	gamma = 4;
    else if (user_gamma <= 1950) /* 1.7  */
	gamma = 5;
    else if (user_gamma <= 2350) /* 2.2  */
	gamma = 6;
    else if (user_gamma > 2350)  /* 2.5  */
	gamma = 7;

    if (gamma >= 8) 
	return;

    OvHueSin = sin(hue);
    OvHueCos = cos(hue);

    CAdjLuma = cont * trans[ref].RefLuma;
    CAdjOff = cont * trans[ref].RefLuma * bright * 1023.0;
    RedAdj = cont * trans[ref].RefLuma * red_intensity * 1023.0;
    GreenAdj = cont * trans[ref].RefLuma * green_intensity * 1023.0;
    BlueAdj = cont * trans[ref].RefLuma * blue_intensity * 1023.0;

    CAdjRCb = sat * -OvHueSin * trans[ref].RefRCr;
    CAdjRCr = sat * OvHueCos * trans[ref].RefRCr;
    CAdjGCb = sat * (OvHueCos * trans[ref].RefGCb - OvHueSin * trans[ref].RefGCr);
    CAdjGCr = sat * (OvHueSin * trans[ref].RefGCb + OvHueCos * trans[ref].RefGCr);
    CAdjBCb = sat * OvHueCos * trans[ref].RefBCb;
    CAdjBCr = sat * OvHueSin * trans[ref].RefBCb;

#if 0 /* default constants */
    CAdjLuma = 1.16455078125;

    CAdjRCb = 0.0;
    CAdjRCr = 1.59619140625;
    CAdjGCb = -0.39111328125;
    CAdjGCr = -0.8125;
    CAdjBCb = 2.01708984375;
    CAdjBCr = 0;
#endif

    OvLuma = CAdjLuma * gamma_curve_r100[gamma].OvGammaCont;
    OvRCb = CAdjRCb * gamma_curve_r100[gamma].OvGammaCont;
    OvRCr = CAdjRCr * gamma_curve_r100[gamma].OvGammaCont;
    OvGCb = CAdjGCb * gamma_curve_r100[gamma].OvGammaCont;
    OvGCr = CAdjGCr * gamma_curve_r100[gamma].OvGammaCont;
    OvBCb = CAdjBCb * gamma_curve_r100[gamma].OvGammaCont;
    OvBCr = CAdjBCr * gamma_curve_r100[gamma].OvGammaCont;
    OvROff = CAdjOff * gamma_curve_r100[gamma].OvGammaCont - 
	OvLuma * Loff - (OvRCb + OvRCr) * Coff;
    OvGOff = CAdjOff * gamma_curve_r100[gamma].OvGammaCont - 
	OvLuma * Loff - (OvGCb + OvGCr) * Coff;
    OvBOff = CAdjOff * gamma_curve_r100[gamma].OvGammaCont - 
	OvLuma * Loff - (OvBCb + OvBCr) * Coff;
#if 0 /* default constants */
    OvROff = -888.5;
    OvGOff = 545;
    OvBOff = -1104;
#endif

    OvROff = ClipValue(OvROff, -2048.0, 2047.5);
    OvGOff = ClipValue(OvGOff, -2048.0, 2047.5);
    OvBOff = ClipValue(OvBOff, -2048.0, 2047.5);
    dwOvROff = ((INT32)(OvROff * 2.0)) & 0x1fff;
    dwOvGOff = ((INT32)(OvGOff * 2.0)) & 0x1fff;
    dwOvBOff = ((INT32)(OvBOff * 2.0)) & 0x1fff;

    if(info->ChipFamily == CHIP_FAMILY_RADEON)
    {
	dwOvLuma =(((INT32)(OvLuma * 2048.0))&0x7fff)<<17;
	dwOvRCb = (((INT32)(OvRCb * 2048.0))&0x7fff)<<1;
	dwOvRCr = (((INT32)(OvRCr * 2048.0))&0x7fff)<<17;
	dwOvGCb = (((INT32)(OvGCb * 2048.0))&0x7fff)<<1;
	dwOvGCr = (((INT32)(OvGCr * 2048.0))&0x7fff)<<17;
	dwOvBCb = (((INT32)(OvBCb * 2048.0))&0x7fff)<<1;
	dwOvBCr = (((INT32)(OvBCr * 2048.0))&0x7fff)<<17;
    }
    else
    {
	dwOvLuma = (((INT32)(OvLuma * 256.0))&0xfff)<<20;
	dwOvRCb = (((INT32)(OvRCb * 256.0))&0xfff)<<4;
	dwOvRCr = (((INT32)(OvRCr * 256.0))&0xfff)<<20;
	dwOvGCb = (((INT32)(OvGCb * 256.0))&0xfff)<<4;
	dwOvGCr = (((INT32)(OvGCr * 256.0))&0xfff)<<20;
	dwOvBCb = (((INT32)(OvBCb * 256.0))&0xfff)<<4;
	dwOvBCr = (((INT32)(OvBCr * 256.0))&0xfff)<<20;
    }

    /* set gamma */
    RADEONSetOverlayGamma(pScrn, gamma);

    /* color transforms */
    OUTREG(RADEON_OV0_LIN_TRANS_A, dwOvRCb | dwOvLuma);
    OUTREG(RADEON_OV0_LIN_TRANS_B, dwOvROff | dwOvRCr);
    OUTREG(RADEON_OV0_LIN_TRANS_C, dwOvGCb | dwOvLuma);
    OUTREG(RADEON_OV0_LIN_TRANS_D, dwOvGOff | dwOvGCr);
    OUTREG(RADEON_OV0_LIN_TRANS_E, dwOvBCb | dwOvLuma);
    OUTREG(RADEON_OV0_LIN_TRANS_F, dwOvBOff | dwOvBCr);
}

static void RADEONSetOverlayAlpha(ScrnInfoPtr pScrn, int ov_alpha, int gr_alpha, int alpha_mode)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (alpha_mode == 0) { /* key mode */
    	OUTREG(RADEON_OV0_KEY_CNTL, 
		RADEON_GRAPHIC_KEY_FN_EQ | /* what does this do? */
		RADEON_VIDEO_KEY_FN_FALSE | /* what does this do? */
		RADEON_CMP_MIX_OR);
    	/* crtc 1 */
    	OUTREG(RADEON_DISP_MERGE_CNTL,
		(RADEON_DISP_ALPHA_MODE_KEY & 
		RADEON_DISP_ALPHA_MODE_MASK) |
		((gr_alpha << 0x00000010) & 
		RADEON_DISP_GRPH_ALPHA_MASK) |
		((ov_alpha << 0x00000018) & 
		RADEON_DISP_OV0_ALPHA_MASK));
    	/* crtc 2 */
    	OUTREG(RADEON_DISP2_MERGE_CNTL,
		(RADEON_DISP_ALPHA_MODE_KEY & 
		RADEON_DISP_ALPHA_MODE_MASK) |
		((gr_alpha << 0x00000010) & 
		RADEON_DISP_GRPH_ALPHA_MASK) |
		((ov_alpha << 0x00000018) & 
		RADEON_DISP_OV0_ALPHA_MASK));
    } else { /* global mode */
    	OUTREG(RADEON_OV0_KEY_CNTL, 
		RADEON_GRAPHIC_KEY_FN_FALSE |   /* what does this do? */
		RADEON_VIDEO_KEY_FN_FALSE |   /* what does this do? */
		RADEON_CMP_MIX_AND);
    	/* crtc 2 */
    	OUTREG(RADEON_DISP2_MERGE_CNTL,
		(RADEON_DISP_ALPHA_MODE_GLOBAL & 
		RADEON_DISP_ALPHA_MODE_MASK) |
		((gr_alpha << 0x00000010) & 
		RADEON_DISP_GRPH_ALPHA_MASK) |
		((ov_alpha << 0x00000018) & 
		RADEON_DISP_OV0_ALPHA_MASK));
    	/* crtc 1 */
    	OUTREG(RADEON_DISP_MERGE_CNTL,
		(RADEON_DISP_ALPHA_MODE_GLOBAL & 
		RADEON_DISP_ALPHA_MODE_MASK) |
		((gr_alpha << 0x00000010) & 
		RADEON_DISP_GRPH_ALPHA_MASK) |
		((ov_alpha << 0x00000018) & 
		RADEON_DISP_OV0_ALPHA_MASK));
    }
     /* per-pixel mode - RADEON_DISP_ALPHA_MODE_PER_PIXEL */
     /* not yet supported */
}

static void RADEONSetColorKey(ScrnInfoPtr pScrn, CARD32 colorKey)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32 min, max;
    CARD8 r, g, b;

    if (info->CurrentLayout.depth > 8)
    {
	CARD32	rbits, gbits, bbits;

	rbits = (colorKey & pScrn->mask.red) >> pScrn->offset.red;
	gbits = (colorKey & pScrn->mask.green) >> pScrn->offset.green;
	bbits = (colorKey & pScrn->mask.blue) >> pScrn->offset.blue;

	r = rbits << (8 - pScrn->weight.red);
	g = gbits << (8 - pScrn->weight.green);
	b = bbits << (8 - pScrn->weight.blue);
    }
    else
    {
	CARD32	bits;

	bits = colorKey & ((1 << info->CurrentLayout.depth) - 1);
	r = bits;
	g = bits;
	b = bits;
    }
    min = (r << 16) | (g << 8) | (b);
    max = (0xff << 24) | (r << 16) | (g << 8) | (b);

    RADEONWaitForFifo(pScrn, 2);
    OUTREG(RADEON_OV0_GRAPHICS_KEY_CLR_HIGH, max);
    OUTREG(RADEON_OV0_GRAPHICS_KEY_CLR_LOW, min);
}

void
RADEONResetVideo(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr   info      = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONPortPrivPtr pPriv = info->adaptor->pPortPrivates[0].ptr;
    char tmp[200];

    /* this function is called from ScreenInit. pScreen is used 
       by XAA internally, but not valid until ScreenInit finishs.
    */
    if (info->accelOn && pScrn->pScreen)
	RADEON_SYNC(info, pScrn);

    /* this is done here because each time the server is reset these
       could change.. Otherwise they remain constant */
    xvInstanceID = MAKE_ATOM("XV_INSTANCE_ID");
    xvDeviceID = MAKE_ATOM("XV_DEVICE_ID");
    xvLocationID = MAKE_ATOM("XV_LOCATION_ID");
    xvDumpStatus = MAKE_ATOM("XV_DUMP_STATUS");
 
    xvBrightness   = MAKE_ATOM("XV_BRIGHTNESS");
    xvSaturation   = MAKE_ATOM("XV_SATURATION");
    xvColor        = MAKE_ATOM("XV_COLOR");
    xvContrast     = MAKE_ATOM("XV_CONTRAST");
    xvColorKey     = MAKE_ATOM("XV_COLORKEY");
    xvDoubleBuffer = MAKE_ATOM("XV_DOUBLE_BUFFER");
    xvHue          = MAKE_ATOM("XV_HUE");
    xvRedIntensity   = MAKE_ATOM("XV_RED_INTENSITY");
    xvGreenIntensity = MAKE_ATOM("XV_GREEN_INTENSITY");
    xvBlueIntensity  = MAKE_ATOM("XV_BLUE_INTENSITY");
    xvGamma          = MAKE_ATOM("XV_GAMMA");
    xvColorspace     = MAKE_ATOM("XV_COLORSPACE");

    xvAutopaintColorkey = MAKE_ATOM("XV_AUTOPAINT_COLORKEY");
    xvSetDefaults       = MAKE_ATOM("XV_SET_DEFAULTS");
    xvSwitchCRT         = MAKE_ATOM("XV_SWITCHCRT");

    xvOvAlpha	      = MAKE_ATOM("XV_OVERLAY_ALPHA");
    xvGrAlpha	      = MAKE_ATOM("XV_GRAPHICS_ALPHA");
    xvAlphaMode       = MAKE_ATOM("XV_ALPHA_MODE");

    xvOverlayDeinterlacingMethod = MAKE_ATOM("XV_OVERLAY_DEINTERLACING_METHOD");

    xvDecBrightness   = MAKE_ATOM("XV_DEC_BRIGHTNESS");
    xvDecSaturation   = MAKE_ATOM("XV_DEC_SATURATION");
    xvDecColor        = MAKE_ATOM("XV_DEC_COLOR");
    xvDecContrast     = MAKE_ATOM("XV_DEC_CONTRAST");
    xvDecHue          = MAKE_ATOM("XV_DEC_HUE");

    xvEncoding        = MAKE_ATOM("XV_ENCODING");
    xvFrequency       = MAKE_ATOM("XV_FREQ");
    xvTunerStatus     = MAKE_ATOM("XV_TUNER_STATUS");
    xvVolume          = MAKE_ATOM("XV_VOLUME");
    xvMute            = MAKE_ATOM("XV_MUTE");
    xvSAP             = MAKE_ATOM("XV_SAP");

    xvAdjustment      = MAKE_ATOM("XV_DEBUG_ADJUSTMENT");

    sprintf(tmp, "RXXX:%d.%d.%d", info->PciInfo->vendor, info->PciInfo->chipType, info->PciInfo->chipRev);
    pPriv->device_id = MAKE_ATOM(tmp);
    sprintf(tmp, "PCI:%02d:%02d.%d", info->PciInfo->bus, info->PciInfo->device, info->PciInfo->func);
    pPriv->location_id = MAKE_ATOM(tmp);
    sprintf(tmp, "INSTANCE:%d", pScrn->scrnIndex);
    pPriv->instance_id = MAKE_ATOM(tmp);

    OUTREG(RADEON_OV0_SCALE_CNTL, RADEON_SCALER_SOFT_RESET);
    OUTREG(RADEON_OV0_AUTO_FLIP_CNTL, 0);   /* maybe */
    OUTREG(RADEON_OV0_EXCLUSIVE_HORZ, 0);
    OUTREG(RADEON_OV0_FILTER_CNTL, RADEON_FILTER_PROGRAMMABLE_COEF);
    OUTREG(RADEON_OV0_KEY_CNTL, RADEON_GRAPHIC_KEY_FN_EQ |
				RADEON_VIDEO_KEY_FN_FALSE |
				RADEON_CMP_MIX_OR);
    OUTREG(RADEON_OV0_TEST, 0);
    OUTREG(RADEON_FCP_CNTL, RADEON_FCP0_SRC_GND);
    OUTREG(RADEON_CAP0_TRIG_CNTL, 0);
    RADEONSetColorKey(pScrn, pPriv->colorKey);

    if (info->ChipFamily == CHIP_FAMILY_RADEON) {

	OUTREG(RADEON_OV0_LIN_TRANS_A, 0x12a00000);
	OUTREG(RADEON_OV0_LIN_TRANS_B, 0x1990190e);
	OUTREG(RADEON_OV0_LIN_TRANS_C, 0x12a0f9c0);
	OUTREG(RADEON_OV0_LIN_TRANS_D, 0xf3000442);
	OUTREG(RADEON_OV0_LIN_TRANS_E, 0x12a02040);
	OUTREG(RADEON_OV0_LIN_TRANS_F, 0x175f);

    } else {

	OUTREG(RADEON_OV0_LIN_TRANS_A, 0x12a20000);
	OUTREG(RADEON_OV0_LIN_TRANS_B, 0x198a190e);
	OUTREG(RADEON_OV0_LIN_TRANS_C, 0x12a2f9da);
	OUTREG(RADEON_OV0_LIN_TRANS_D, 0xf2fe0442);
	OUTREG(RADEON_OV0_LIN_TRANS_E, 0x12a22046);
	OUTREG(RADEON_OV0_LIN_TRANS_F, 0x175f);
    }
	/*
	 * Set default Gamma ramp:
	 *
	 * Of 18 segments for gamma curve, all segments in R200 (and
	 * newer) are programmable, while only lower 4 and upper 2
	 * segments are programmable in the older Radeons.
	 */

    RADEONSetOverlayGamma(pScrn, 0); /* gamma = 1.0 */

    if(pPriv->VIP!=NULL){
        RADEONVIP_reset(pScrn,pPriv);
        }
    
    if(pPriv->theatre != NULL) {
        xf86_InitTheatre(pPriv->theatre);
/*      xf86_ResetTheatreRegsForNoTVout(pPriv->theatre); */
        }
    
    if(pPriv->i2c != NULL){
        RADEONResetI2C(pScrn, pPriv);
        }
}

static void RADEONSetupTheatre(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONPLLPtr  pll = &(info->pll);
    TheatrePtr t;

    CARD8 a;
    int i;
              
    pPriv->theatre = NULL;

    if(!info->MM_TABLE_valid && 
       !((info->RageTheatreCrystal>=0) &&
           (info->RageTheatreTunerPort>=0) && (info->RageTheatreCompositePort>=0) &&
           (info->RageTheatreSVideoPort>=0)))
    {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "no multimedia table present, disabling Rage Theatre.\n");
       return;
    }

    /* Go and find Rage Theatre, if it exists */
    
    switch(info->Chipset){
    	case PCI_CHIP_RADEON_LY:
	case PCI_CHIP_RADEON_LZ:
	        xf86DrvMsg(pScrn->scrnIndex,X_INFO,"Detected Radeon Mobility M6, not scanning for Rage Theatre\n");
		break;
	case PCI_CHIP_RADEON_LW:
	        xf86DrvMsg(pScrn->scrnIndex,X_INFO,"Detected Radeon Mobility M7, not scanning for Rage Theatre\n");
		break;
	default:
	    pPriv->theatre=xf86_DetectTheatre(pPriv->VIP);
	}

    if(pPriv->theatre==NULL)return;
    
    /* just a matter of convenience */
    t=pPriv->theatre; 
        
    t->video_decoder_type=info->video_decoder_type;
        
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "video decoder type is 0x%04x (BIOS value) versus 0x%04x (current PLL setting)\n",
         t->video_decoder_type, pll->xclk);
        
    if(info->MM_TABLE_valid){
         for(i=0;i<5;i++){
                a=info->MM_TABLE.input[i];
                
                switch(a & 0x3){
                        case 1:
                                t->wTunerConnector=i;
                                xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Tuner is on port %d\n",i);
                                break;
                        case 2:  if(a & 0x4){
                                   t->wComp0Connector=RT_COMP2;
                                   } else {
                                   t->wComp0Connector=RT_COMP1;
                                   }
                xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Composite connector is port %ld\n", t->wComp0Connector);
                                  break;
                        case 3:  if(a & 0x4){
                                   t->wSVideo0Connector=RT_YCR_COMP4;
                                   } else {
                                   t->wSVideo0Connector=RT_YCF_COMP4;
                                   }
                xf86DrvMsg(pScrn->scrnIndex, X_INFO, "SVideo connector is port %ld\n", t->wSVideo0Connector);
                                   break;
                        default:
                                break;
                        }
                }
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Rage Theatre: Connectors (detected): tuner=%ld, composite=%ld, svideo=%ld\n",
    	     t->wTunerConnector, t->wComp0Connector, t->wSVideo0Connector);
        
         }

    if(info->RageTheatreTunerPort>=0)t->wTunerConnector=info->RageTheatreTunerPort;
    if(info->RageTheatreCompositePort>=0)t->wComp0Connector=info->RageTheatreCompositePort;
    if(info->RageTheatreSVideoPort>=0)t->wSVideo0Connector=info->RageTheatreSVideoPort;
        
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "RageTheatre: Connectors (using): tuner=%ld, composite=%ld, svideo=%ld\n",
    	t->wTunerConnector, t->wComp0Connector, t->wSVideo0Connector);

    switch((info->RageTheatreCrystal>=0)?info->RageTheatreCrystal:pll->reference_freq){
                case 2700:
                        t->video_decoder_type=RT_FREF_2700;
                        break;
                case 2950:
                        t->video_decoder_type=RT_FREF_2950;
                        break;
                default:
                        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                "Unsupported reference clock frequency, Rage Theatre disabled\n");
                        t->theatre_num=-1;
			xfree(pPriv->theatre);
			pPriv->theatre = NULL;
			return;
                }
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "video decoder type used: 0x%04x\n", t->video_decoder_type);
}

static XF86VideoAdaptorPtr
RADEONAllocAdaptor(ScrnInfoPtr pScrn)
{
    XF86VideoAdaptorPtr adapt;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONPortPrivPtr pPriv;
    CARD32 dot_clock;

    if(!(adapt = xf86XVAllocateVideoAdaptorRec(pScrn)))
	return NULL;

    if(!(pPriv = xcalloc(1, sizeof(RADEONPortPrivRec) + sizeof(DevUnion))))
    {
	xfree(adapt);
	return NULL;
    }

    adapt->pPortPrivates = (DevUnion*)(&pPriv[1]);
    adapt->pPortPrivates[0].ptr = (pointer)pPriv;

    pPriv->colorKey = info->videoKey;
    pPriv->doubleBuffer = TRUE;
    pPriv->videoStatus = 0;
    pPriv->brightness = 0;
    pPriv->transform_index = 0;
    pPriv->saturation = 0;
    pPriv->contrast = 0;
    pPriv->red_intensity = 0;
    pPriv->green_intensity = 0;
    pPriv->blue_intensity = 0;
    pPriv->hue = 0;
    pPriv->currentBuffer = 0;
    pPriv->autopaint_colorkey = TRUE;
    pPriv->gamma = 1000;
    if (info->OverlayOnCRTC2)
	pPriv->crt2 = TRUE;
    else
	pPriv->crt2 = FALSE;
	
    pPriv->ov_alpha = 255;
    pPriv->gr_alpha = 255;
    pPriv->alpha_mode = 0;

       /* TV-in stuff */
    pPriv->video_stream_active = FALSE;
    pPriv->encoding = 4;
    pPriv->frequency = 1000;
    pPriv->volume = -1000;
    pPriv->mute = TRUE;
    pPriv->v = 0;
    pPriv->overlay_deinterlacing_method = METHOD_BOB;
    pPriv->capture_vbi_data = 0;
    pPriv->dec_brightness = 0;
    pPriv->dec_saturation = 0;
    pPriv->dec_contrast = 0;
    pPriv->dec_hue = 0;


    /*
     * Unlike older Mach64 chips, RADEON has only two ECP settings:
     * 0 for PIXCLK < 175Mhz, and 1 (divide by 2)
     * for higher clocks, sure makes life nicer
     */

    /* Figure out which head we are on */
    if ((info->MergedFB && info->OverlayOnCRTC2) || info->IsSecondary)
	dot_clock = info->ModeReg.dot_clock_freq_2;
    else
	dot_clock = info->ModeReg.dot_clock_freq;

    if(dot_clock < 17500)
        pPriv->ecp_div = 0;
    else
        pPriv->ecp_div = 1;


#if 0
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Dotclock is %g Mhz, setting ecp_div to %d\n", info->ModeReg.dot_clock_freq/100.0, pPriv->ecp_div);
#endif

    OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, (INPLL(pScrn, RADEON_VCLK_ECP_CNTL) &
					 0xfffffCff) | (pPriv->ecp_div << 8));

    /* I suspect we may need a usleep after writing to the PLL.  if you play a video too soon
       after switching crtcs in mergedfb clone mode you get a temporary one pixel line of colorkey 
       on the right edge video output.  */


    if ((info->ChipFamily == CHIP_FAMILY_RS100) || 
	(info->ChipFamily == CHIP_FAMILY_RS200) ||
	(info->ChipFamily == CHIP_FAMILY_RS300)) {
        /* Force the overlay clock on for integrated chips
	 */
        OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL,
	       (INPLL(pScrn, RADEON_VCLK_ECP_CNTL) | (1<<18)));
    }
    
    /* overlay scaler line length differs for different revisions 
       this needs to be maintained by hand  */
    switch(info->ChipFamily){
    	case CHIP_FAMILY_R200:
	case CHIP_FAMILY_R300:
		pPriv->overlay_scaler_buffer_width=1920;
		break;
	default:
		pPriv->overlay_scaler_buffer_width=1536;
    	}

    /* Decide on tuner type */
    if((info->tunerType<0) && (info->MM_TABLE_valid)) {
        pPriv->tuner_type = info->MM_TABLE.tuner_type;
    	} else
        pPriv->tuner_type = info->tunerType;
        
    /* Initialize I2C bus */
    RADEONInitI2C(pScrn, pPriv);
    if(pPriv->i2c != NULL)RADEON_board_setmisc(pPriv);
    

    #if 0  /* this is just here for easy debugging - normally off */
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Scanning I2C Bus\n");
    for(i=0;i<255;i+=2)
        if(RADEONProbeAddress(pPriv->i2c, i))
                xf86DrvMsg(pScrn->scrnIndex, X_INFO, "     found device at address 0x%02x\n", i);
    #endif

    /* Initialize VIP bus */
    RADEONVIP_init(pScrn, pPriv);
    info->adaptor = adapt;

	if(!xf86LoadSubModule(pScrn,"theatre_detect")) 
	{
		xf86DrvMsg(pScrn->scrnIndex,X_ERROR,"Unable to load Rage Theatre detect module\n");
		goto skip_theatre;
    }
	xf86LoaderReqSymbols(TheatreDetectSymbolsList, NULL);

	RADEONSetupTheatre(pScrn, pPriv);

	/*
	 * Now load the correspondind theatre chip based on what has been detected. 
	 */
	if (pPriv->theatre)
	{
		xf86DrvMsg(pScrn->scrnIndex,X_INFO,"Going to load the corresponding theatre module\n");
		switch (pPriv->theatre->theatre_id)
		{
			case RT100_ATI_ID:
			{
				if(!xf86LoadSubModule(pScrn,"theatre")) 
				{
					xf86DrvMsg(pScrn->scrnIndex,X_ERROR,"Unable to load Rage Theatre module\n");
					xfree(pPriv->theatre);
					goto skip_theatre;
				}
				break;
			}
			case RT200_ATI_ID:
			{
				if(!xf86LoadSubModule(pScrn,"theatre200")) 
				{
					xf86DrvMsg(pScrn->scrnIndex,X_ERROR,"Unable to load Rage Theatre module\n");
					xfree(pPriv->theatre);
					goto skip_theatre;
				}
				pPriv->theatre->microc_path = info->RageTheatreMicrocPath;
				pPriv->theatre->microc_type = info->RageTheatreMicrocType;
				break;
			}
			default:
			{
				xf86DrvMsg(pScrn->scrnIndex,X_ERROR,"Unknown Theatre chip\n");
				xfree(pPriv->theatre);
				goto skip_theatre;
			}
		}
		xf86LoaderReqSymbols(TheatreSymbolsList, NULL);
	}
    
	if(pPriv->theatre!=NULL)
	{
		xf86_InitTheatre(pPriv->theatre);
		if(pPriv->theatre->mode == MODE_UNINITIALIZED)
		{
			Xfree(pPriv->theatre);
			pPriv->theatre = NULL;
			xf86DrvMsg(pScrn->scrnIndex,X_INFO,"Rage Theatre disabled\n");
			/* Here the modules must be unloaded */
			goto skip_theatre;
		}
	}
  
    if(pPriv->theatre!=NULL){	
        xf86_ResetTheatreRegsForNoTVout(pPriv->theatre);
        xf86_RT_SetTint(pPriv->theatre, pPriv->dec_hue);
        xf86_RT_SetSaturation(pPriv->theatre, pPriv->dec_saturation);
        xf86_RT_SetSharpness(pPriv->theatre, RT_NORM_SHARPNESS);
        xf86_RT_SetContrast(pPriv->theatre, pPriv->dec_contrast);
        xf86_RT_SetBrightness(pPriv->theatre, pPriv->dec_brightness);  
	
        RADEON_RT_SetEncoding(pScrn, pPriv);	
	}
	
skip_theatre:

    return adapt;
}

static XF86VideoAdaptorPtr
RADEONSetupImageVideo(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RADEONPortPrivPtr pPriv;
    XF86VideoAdaptorPtr adapt;

    if(!(adapt = RADEONAllocAdaptor(pScrn)))
	return NULL;

    adapt->type = XvWindowMask | XvInputMask | XvImageMask;
    adapt->flags = VIDEO_OVERLAID_IMAGES | VIDEO_CLIP_TO_VIEWPORT;
    adapt->name = "ATI Radeon Video Overlay";
    adapt->nEncodings = 1;
    adapt->pEncodings = &DummyEncoding;
    adapt->nFormats = NUM_FORMATS;
    adapt->pFormats = Formats;
    adapt->nPorts = 1;
    adapt->nAttributes = NUM_ATTRIBUTES;
    adapt->pAttributes = Attributes;
    adapt->nImages = NUM_IMAGES;
    adapt->pImages = Images;
    adapt->PutVideo = NULL;
    adapt->PutStill = NULL;
    adapt->GetVideo = NULL;
    adapt->GetStill = NULL;
    adapt->StopVideo = RADEONStopVideo;
    adapt->SetPortAttribute = RADEONSetPortAttribute;
    adapt->GetPortAttribute = RADEONGetPortAttribute;
    adapt->QueryBestSize = RADEONQueryBestSize;
    adapt->PutImage = RADEONPutImage;
    adapt->QueryImageAttributes = RADEONQueryImageAttributes;

    pPriv = (RADEONPortPrivPtr)(adapt->pPortPrivates[0].ptr);
    REGION_NULL(pScreen, &(pPriv->clip));

    if(pPriv->theatre != NULL) 
    {
	/* video decoder is present, extend capabilities */
       adapt->nEncodings = 13;
       adapt->pEncodings = InputVideoEncodings;
       adapt->type |= XvVideoMask;
       adapt->nAttributes = NUM_DEC_ATTRIBUTES;    
       adapt->PutVideo = RADEONPutVideo;
    }

    RADEONResetVideo(pScrn);

    return adapt;
}

static void
RADEONStopVideo(ScrnInfoPtr pScrn, pointer data, Bool cleanup)
{
  RADEONInfoPtr info = RADEONPTR(pScrn);
  unsigned char *RADEONMMIO = info->MMIO;
  RADEONPortPrivPtr pPriv = (RADEONPortPrivPtr)data;

  REGION_EMPTY(pScrn->pScreen, &pPriv->clip);

  if(cleanup) {
     if(pPriv->videoStatus & CLIENT_VIDEO_ON) {
	RADEONWaitForFifo(pScrn, 2);
	OUTREG(RADEON_OV0_SCALE_CNTL, 0);
     }
     if(pPriv->video_stream_active){
        RADEONWaitForFifo(pScrn, 2);
        OUTREG(RADEON_FCP_CNTL, RADEON_FCP0_SRC_GND);
        OUTREG(RADEON_CAP0_TRIG_CNTL, 0);
        RADEONResetVideo(pScrn);
        pPriv->video_stream_active = FALSE;
        if(pPriv->msp3430 != NULL) xf86_MSP3430SetVolume(pPriv->msp3430, MSP3430_FAST_MUTE);
		if(pPriv->uda1380 != NULL) xf86_uda1380_mute(pPriv->uda1380, TRUE);
        if(pPriv->i2c != NULL) RADEON_board_setmisc(pPriv);
     }
     if (pPriv->video_memory != NULL) {
	 RADEONFreeMemory(pScrn, pPriv->video_memory);
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
RADEONSetPortAttribute(ScrnInfoPtr  pScrn,
		       Atom	    attribute,
		       INT32	    value,
		       pointer	    data)
{
    RADEONInfoPtr	info = RADEONPTR(pScrn);
    RADEONPortPrivPtr	pPriv = (RADEONPortPrivPtr)data;
    Bool		setTransform = FALSE;
    Bool		setAlpha = FALSE;
    unsigned char *RADEONMMIO = info->MMIO;

    RADEON_SYNC(info, pScrn);

#define RTFSaturation(a)   (1.0 + ((a)*1.0)/1000.0)
#define RTFBrightness(a)   (((a)*1.0)/2000.0)
#define RTFIntensity(a)   (((a)*1.0)/2000.0)
#define RTFContrast(a)   (1.0 + ((a)*1.0)/1000.0)
#define RTFHue(a)   (((a)*3.1416)/1000.0)

    if(attribute == xvAutopaintColorkey)
    {
	pPriv->autopaint_colorkey = ClipValue (value, 0, 1);
    }
    else if(attribute == xvSetDefaults)
    {
	pPriv->autopaint_colorkey = TRUE;
	pPriv->brightness = 0;
	pPriv->saturation = 0;
	pPriv->contrast = 0;
	pPriv->hue = 0;
	pPriv->red_intensity = 0;
	pPriv->green_intensity = 0;
	pPriv->blue_intensity = 0;
	pPriv->gamma = 1000;
	pPriv->transform_index = 0;
	pPriv->doubleBuffer = FALSE;
	pPriv->ov_alpha = 255;
	pPriv->gr_alpha = 255;
	pPriv->alpha_mode = 0;

        /* It is simpler to call itself */
        RADEONSetPortAttribute(pScrn, xvDecBrightness, 0, data);
        RADEONSetPortAttribute(pScrn, xvDecSaturation, 0, data);
        RADEONSetPortAttribute(pScrn, xvDecContrast,   0, data);
        RADEONSetPortAttribute(pScrn, xvDecHue,   0, data);

        RADEONSetPortAttribute(pScrn, xvVolume,   -1000, data);
        RADEONSetPortAttribute(pScrn, xvMute,   1, data);
        RADEONSetPortAttribute(pScrn, xvSAP,   0, data);
        RADEONSetPortAttribute(pScrn, xvDoubleBuffer,   1, data);

	setTransform = TRUE;
	setAlpha = TRUE;
    }
    else if(attribute == xvBrightness)
    {
	pPriv->brightness = ClipValue (value, -1000, 1000);
	setTransform = TRUE;
    }
    else if((attribute == xvSaturation) || (attribute == xvColor))
    {
	pPriv->saturation = ClipValue (value, -1000, 1000);
	setTransform = TRUE;
    }
    else if(attribute == xvContrast)
    {
	pPriv->contrast = ClipValue (value, -1000, 1000);
	setTransform = TRUE;
    }
    else if(attribute == xvHue)
    {
	pPriv->hue = ClipValue (value, -1000, 1000);
	setTransform = TRUE;
    }
    else if(attribute == xvRedIntensity)
    {
	pPriv->red_intensity = ClipValue (value, -1000, 1000);
	setTransform = TRUE;
    }
    else if(attribute == xvGreenIntensity)
    {
	pPriv->green_intensity = ClipValue (value, -1000, 1000);
	setTransform = TRUE;
    }
    else if(attribute == xvBlueIntensity)
    {
	pPriv->blue_intensity = ClipValue (value, -1000, 1000);
	setTransform = TRUE;
    }
    else if(attribute == xvGamma) 
    {
	pPriv->gamma = ClipValue (value, 100, 10000);
	setTransform = TRUE;
    } 
    else if(attribute == xvColorspace) 
    {
	pPriv->transform_index = ClipValue (value, 0, 1);
	setTransform = TRUE;
    } 
    else if(attribute == xvDoubleBuffer)
    {
	pPriv->doubleBuffer = ClipValue (value, 0, 1);
    }
    else if(attribute == xvColorKey)
    {
	pPriv->colorKey = value;
	RADEONSetColorKey (pScrn, pPriv->colorKey);
	REGION_EMPTY(pScrn->pScreen, &pPriv->clip);
    } 
    else if(attribute == xvSwitchCRT) 
    {
	pPriv->crt2 = ClipValue (value, 0, 1);
	pPriv->crt2 = value;
	if (pPriv->crt2)
	    info->OverlayOnCRTC2 = TRUE;
	else
	    info->OverlayOnCRTC2 = FALSE; 
    }
    else if(attribute == xvOvAlpha) 
    {
	pPriv->ov_alpha = ClipValue (value, 0, 255);
	setAlpha = TRUE;
    }
    else if(attribute == xvGrAlpha) 
    {
	pPriv->gr_alpha = ClipValue (value, 0, 255);
	setAlpha = TRUE;
    } 
    else if(attribute == xvAlphaMode) 
    {
	pPriv->alpha_mode = ClipValue (value, 0, 1);
	setAlpha = TRUE;
    } 
    else if(attribute == xvDecBrightness) 
    {
        pPriv->dec_brightness = value;
        if(pPriv->theatre!=NULL) xf86_RT_SetBrightness(pPriv->theatre, pPriv->dec_brightness);  
    } 
    else if((attribute == xvDecSaturation) || (attribute == xvDecColor)) 
    {
        if(value<-1000)value = -1000;
        if(value>1000)value = 1000;
        pPriv->dec_saturation = value;
        if(pPriv->theatre != NULL)xf86_RT_SetSaturation(pPriv->theatre, value);
    } 
    else if(attribute == xvDecContrast) 
    {
        pPriv->dec_contrast = value;
        if(pPriv->theatre != NULL)xf86_RT_SetContrast(pPriv->theatre, value);
    } 
    else if(attribute == xvDecHue) 
    {
        pPriv->dec_hue = value;
        if(pPriv->theatre != NULL)xf86_RT_SetTint(pPriv->theatre, value);
    } 
    else if(attribute == xvEncoding) 
    {
        pPriv->encoding = value;
        if(pPriv->video_stream_active)
        {
           if(pPriv->theatre != NULL) RADEON_RT_SetEncoding(pScrn, pPriv);
           if(pPriv->msp3430 != NULL) RADEON_MSP_SetEncoding(pPriv);
           if(pPriv->tda9885 != NULL) RADEON_TDA9885_SetEncoding(pPriv);
	   if(pPriv->fi1236 != NULL) RADEON_FI1236_SetEncoding(pPriv);
           if(pPriv->i2c != NULL) RADEON_board_setmisc(pPriv);
        /* put more here to actually change it */
        }
   } 
   else if(attribute == xvFrequency) 
   {
        pPriv->frequency = value;
        /* mute volume if it was not muted before */
        if((pPriv->msp3430!=NULL)&& !pPriv->mute)xf86_MSP3430SetVolume(pPriv->msp3430, MSP3430_FAST_MUTE);
		if((pPriv->uda1380!=NULL)&& !pPriv->mute)xf86_uda1380_mute(pPriv->uda1380, TRUE);
        if(pPriv->fi1236 != NULL) xf86_TUNER_set_frequency(pPriv->fi1236, value);
/*        if(pPriv->theatre != NULL) RADEON_RT_SetEncoding(pScrn, pPriv);  */
        if((pPriv->msp3430 != NULL) && (pPriv->msp3430->recheck))
                xf86_InitMSP3430(pPriv->msp3430);
        if((pPriv->msp3430 != NULL)&& !pPriv->mute) xf86_MSP3430SetVolume(pPriv->msp3430, MSP3430_VOLUME(pPriv->volume));
		if((pPriv->uda1380 != NULL)&& !pPriv->mute) xf86_uda1380_setvolume(pPriv->uda1380, pPriv->volume);
   } 
   else if(attribute == xvMute) 
   {
        pPriv->mute = value;
        if(pPriv->msp3430 != NULL) xf86_MSP3430SetVolume(pPriv->msp3430, pPriv->mute ? MSP3430_FAST_MUTE : MSP3430_VOLUME(pPriv->volume));
        if(pPriv->i2c != NULL) RADEON_board_setmisc(pPriv);
		if(pPriv->uda1380 != NULL) xf86_uda1380_mute(pPriv->uda1380, pPriv->mute);
   } 
   else if(attribute == xvSAP) 
   {
        pPriv->sap_channel = value;
        if(pPriv->msp3430 != NULL) xf86_MSP3430SetSAP(pPriv->msp3430, pPriv->sap_channel?4:3);
   } 
   else if(attribute == xvVolume) 
   {
        if(value<-1000)value = -1000;
        if(value>1000)value = 1000;
        pPriv->volume = value;  
        pPriv->mute = FALSE;
        if(pPriv->msp3430 != NULL) xf86_MSP3430SetVolume(pPriv->msp3430, MSP3430_VOLUME(value));
        if(pPriv->i2c != NULL) RADEON_board_setmisc(pPriv);
		if(pPriv->uda1380 != NULL) xf86_uda1380_setvolume(pPriv->uda1380, value);
   } 
   else if(attribute == xvOverlayDeinterlacingMethod) 
   {
        if(value<0)value = 0;
        if(value>2)value = 2;
        pPriv->overlay_deinterlacing_method = value;    
        switch(pPriv->overlay_deinterlacing_method){
                case METHOD_BOB:
                        OUTREG(RADEON_OV0_DEINTERLACE_PATTERN, 0xAAAAA);
                        break;
                case METHOD_SINGLE:
                        OUTREG(RADEON_OV0_DEINTERLACE_PATTERN, 0xEEEEE | (9<<28));
                        break;
                case METHOD_WEAVE:
                        OUTREG(RADEON_OV0_DEINTERLACE_PATTERN, 0x0);
                        break;
                default:
                        OUTREG(RADEON_OV0_DEINTERLACE_PATTERN, 0xAAAAA);
                }                       
   } 
   else if(attribute == xvDumpStatus) 
   {
  	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Current mode flags 0x%08x: %s%s\n",
		pScrn->currentMode->Flags,
		pScrn->currentMode->Flags & V_INTERLACE ? " interlaced" : "" ,
		pScrn->currentMode->Flags & V_DBLSCAN ? " doublescan" : ""
		);
	if(pPriv->tda9885 != NULL){
		xf86_tda9885_getstatus(pPriv->tda9885);
		xf86_tda9885_dumpstatus(pPriv->tda9885);
		}
	if(pPriv->fi1236!=NULL){
		xf86_fi1236_dump_status(pPriv->fi1236);
		}
   } 
   else if(attribute == xvAdjustment) 
   {
  	pPriv->adjustment=value;
        xf86DrvMsg(pScrn->scrnIndex,X_ERROR,"Setting pPriv->adjustment to %ld\n", pPriv->adjustment);
  	if(pPriv->tda9885!=0){
		pPriv->tda9885->top_adjustment=value;
		RADEON_TDA9885_SetEncoding(pPriv);
		}
   }
   else 
	return BadMatch;

    if (setTransform)
    {
	RADEONSetTransform(pScrn,
			   RTFBrightness(pPriv->brightness),
			   RTFContrast(pPriv->contrast),
			   RTFSaturation(pPriv->saturation),
			   RTFHue(pPriv->hue),
			   RTFIntensity(pPriv->red_intensity),
			   RTFIntensity(pPriv->green_intensity),
			   RTFIntensity(pPriv->blue_intensity),
			   pPriv->transform_index,
			   pPriv->gamma);
    }

    if (setAlpha)
    {
	RADEONSetOverlayAlpha(pScrn, pPriv->ov_alpha, pPriv->gr_alpha, pPriv->alpha_mode);
    }
	
    return Success;
}

static int
RADEONGetPortAttribute(ScrnInfoPtr  pScrn,
		       Atom	    attribute,
		       INT32	    *value,
		       pointer	    data)
{
    RADEONInfoPtr	info = RADEONPTR(pScrn);
    RADEONPortPrivPtr	pPriv = (RADEONPortPrivPtr)data;

    if (info->accelOn) RADEON_SYNC(info, pScrn);

    if(attribute == xvAutopaintColorkey)
	*value = pPriv->autopaint_colorkey;
    else if(attribute == xvBrightness)
	*value = pPriv->brightness;
    else if((attribute == xvSaturation) || (attribute == xvColor))
	*value = pPriv->saturation;
    else if(attribute == xvContrast)
	*value = pPriv->contrast;
    else if(attribute == xvHue)
	*value = pPriv->hue;
    else if(attribute == xvRedIntensity)
	*value = pPriv->red_intensity;
    else if(attribute == xvGreenIntensity)
	*value = pPriv->green_intensity;
    else if(attribute == xvBlueIntensity)
	*value = pPriv->blue_intensity;
    else if(attribute == xvGamma)
	*value = pPriv->gamma;
    else if(attribute == xvColorspace)
	*value = pPriv->transform_index;
    else if(attribute == xvDoubleBuffer)
	*value = pPriv->doubleBuffer ? 1 : 0;
    else if(attribute == xvColorKey)
	*value = pPriv->colorKey;
    else if(attribute == xvSwitchCRT)
	*value = pPriv->crt2 ? 1 : 0;
    else if(attribute == xvOvAlpha)
	*value = pPriv->ov_alpha;
    else if(attribute == xvGrAlpha)
	*value = pPriv->gr_alpha;
    else if(attribute == xvAlphaMode)
	*value = pPriv->alpha_mode;
    else if(attribute == xvDecBrightness)
        *value = pPriv->dec_brightness;
    else if((attribute == xvDecSaturation) || (attribute == xvDecColor))
        *value = pPriv->dec_saturation;
    else if(attribute == xvDecContrast)
        *value = pPriv->dec_contrast;
    else if(attribute == xvDecHue)
        *value = pPriv->dec_hue;
    else if(attribute == xvEncoding)
        *value = pPriv->encoding;
    else if(attribute == xvFrequency)
        *value = pPriv->frequency;
    else 
    if(attribute == xvTunerStatus) {
        if(pPriv->fi1236==NULL){
                *value=TUNER_OFF;
                } else
                {
                *value = xf86_TUNER_get_afc_hint(pPriv->fi1236);
                }
       } 
    else if(attribute == xvMute)
        *value = pPriv->mute;
    else if(attribute == xvSAP)
        *value = pPriv->sap_channel;
    else if(attribute == xvVolume)
        *value = pPriv->volume;
    else if(attribute == xvOverlayDeinterlacingMethod)
        *value = pPriv->overlay_deinterlacing_method;
    else if(attribute == xvDeviceID)
        *value = pPriv->device_id;
    else if(attribute == xvLocationID)
        *value = pPriv->location_id;
    else if(attribute == xvInstanceID)
        *value = pPriv->instance_id;
    else if(attribute == xvAdjustment)
  	*value = pPriv->adjustment;
    else
	return BadMatch;

    return Success;
}

static void
RADEONQueryBestSize(
  ScrnInfoPtr pScrn,
  Bool motion,
  short vid_w, short vid_h,
  short drw_w, short drw_h,
  unsigned int *p_w, unsigned int *p_h,
  pointer data
){
   if(vid_w > (drw_w << 4))
	drw_w = vid_w >> 4;
   if(vid_h > (drw_h << 4))
	drw_h = vid_h >> 4;

  *p_w = drw_w;
  *p_h = drw_h;
}

static struct {
	double range;
	signed char coeff[5][4];
	} TapCoeffs[]=
	{
        {0.25, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13,   13,    3}, }},
        {0.26, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.27, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.28, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.29, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.30, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.31, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.32, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.33, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.34, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.35, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.36, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.37, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.38, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.39, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.40, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.41, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.42, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.43, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.44, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.45, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.46, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.47, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.48, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.49, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.50, {{ 7,    16,  9,  0}, { 7,   16,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 3,   13, 13,  3}, }},
        {0.51, {{ 7,    17,  8,  0}, { 6,   17,  9,  0}, { 5,   15, 11,  1}, { 4,   15, 12,  1}, { 2,   14, 14,  2}, }},
        {0.52, {{ 7,    17,  8,  0}, { 6,   17,  9,  0}, { 5,   16, 11,  0}, { 3,   15, 13,  1}, { 2,   14, 14,  2}, }},
        {0.53, {{ 7,    17,  8,  0}, { 6,   17,  9,  0}, { 5,   16, 11,  0}, { 3,   15, 13,  1}, { 2,   14, 14,  2}, }},
        {0.54, {{ 7,    17,  8,  0}, { 6,   17,  9,  0}, { 4,   17, 11,  0}, { 3,   15, 13,  1}, { 2,   14, 14,  2}, }},
        {0.55, {{ 7,    18,  7,  0}, { 6,   17,  9,  0}, { 4,   17, 11,  0}, { 3,   15, 13,  1}, { 1,   15, 15,  1}, }},
        {0.56, {{ 7,    18,  7,  0}, { 5,   18,  9,  0}, { 4,   17, 11,  0}, { 2,   17, 13,  0}, { 1,   15, 15,  1}, }},
        {0.57, {{ 7,    18,  7,  0}, { 5,   18,  9,  0}, { 4,   17, 11,  0}, { 2,   17, 13,  0}, { 1,   15, 15,  1}, }},
        {0.58, {{ 7,    18,  7,  0}, { 5,   18,  9,  0}, { 4,   17, 11,  0}, { 2,   17, 13,  0}, { 1,   15, 15,  1}, }},
        {0.59, {{ 7,    18,  7,  0}, { 5,   18,  9,  0}, { 4,   17, 11,  0}, { 2,   17, 13,  0}, { 1,   15, 15,  1}, }},
        {0.60, {{ 7,    18,  8, -1}, { 6,   17, 10, -1}, { 4,   17, 11,  0}, { 2,   17, 13,  0}, { 1,   15, 15,  1}, }},
        {0.61, {{ 7,    18,  8, -1}, { 6,   17, 10, -1}, { 4,   17, 11,  0}, { 2,   17, 13,  0}, { 1,   15, 15,  1}, }},
        {0.62, {{ 7,    18,  8, -1}, { 6,   17, 10, -1}, { 4,   17, 11,  0}, { 2,   17, 13,  0}, { 1,   15, 15,  1}, }},
        {0.63, {{ 7,    18,  8, -1}, { 6,   17, 10, -1}, { 4,   17, 11,  0}, { 2,   17, 13,  0}, { 1,   15, 15,  1}, }},
        {0.64, {{ 7,    18,  8, -1}, { 6,   17, 10, -1}, { 4,   17, 12, -1}, { 2,   17, 13,  0}, { 1,   15, 15,  1}, }},
        {0.65, {{ 7,    18,  8, -1}, { 6,   17, 10, -1}, { 4,   17, 12, -1}, { 2,   17, 13,  0}, { 0,   16, 16,  0}, }},
        {0.66, {{ 7,    18,  8, -1}, { 6,   18, 10, -2}, { 4,   17, 12, -1}, { 2,   17, 13,  0}, { 0,   16, 16,  0}, }},
        {0.67, {{ 7,    20,  7, -2}, { 5,   19, 10, -2}, { 3,   18, 12, -1}, { 2,   17, 13,  0}, { 0,   16, 16,  0}, }},
        {0.68, {{ 7,    20,  7, -2}, { 5,   19, 10, -2}, { 3,   19, 12, -2}, { 1,   18, 14, -1}, { 0,   16, 16,  0}, }},
        {0.69, {{ 7,    20,  7, -2}, { 5,   19, 10, -2}, { 3,   19, 12, -2}, { 1,   18, 14, -1}, { 0,   16, 16,  0}, }},
        {0.70, {{ 7,    20,  7, -2}, { 5,   20,  9, -2}, { 3,   19, 12, -2}, { 1,   18, 14, -1}, { 0,   16, 16,  0}, }},
        {0.71, {{ 7,    20,  7, -2}, { 5,   20,  9, -2}, { 3,   19, 12, -2}, { 1,   18, 14, -1}, { 0,   16, 16,  0}, }},
        {0.72, {{ 7,    20,  7, -2}, { 5,   20,  9, -2}, { 2,   20, 12, -2}, { 0,   19, 15, -2}, {-1,   17, 17, -1}, }},
        {0.73, {{ 7,    20,  7, -2}, { 4,   21,  9, -2}, { 2,   20, 12, -2}, { 0,   19, 15, -2}, {-1,   17, 17, -1}, }},
        {0.74, {{ 6,    22,  6, -2}, { 4,   21,  9, -2}, { 2,   20, 12, -2}, { 0,   19, 15, -2}, {-1,   17, 17, -1}, }},
        {0.75, {{ 6,    22,  6, -2}, { 4,   21,  9, -2}, { 1,   21, 12, -2}, { 0,   19, 15, -2}, {-1,   17, 17, -1}, }},
        {0.76, {{ 6,    22,  6, -2}, { 4,   21,  9, -2}, { 1,   21, 12, -2}, { 0,   19, 15, -2}, {-1,   17, 17, -1}, }},
        {0.77, {{ 6,    22,  6, -2}, { 3,   22,  9, -2}, { 1,   22, 12, -3}, { 0,   19, 15, -2}, {-2,   18, 18, -2}, }},
        {0.78, {{ 6,    21,  6, -1}, { 3,   22,  9, -2}, { 1,   22, 12, -3}, { 0,   19, 15, -2}, {-2,   18, 18, -2}, }},
        {0.79, {{ 5,    23,  5, -1}, { 3,   22,  9, -2}, { 0,   23, 12, -3}, {-1,   21, 15, -3}, {-2,   18, 18, -2}, }},
        {0.80, {{ 5,    23,  5, -1}, { 3,   23,  8, -2}, { 0,   23, 12, -3}, {-1,   21, 15, -3}, {-2,   18, 18, -2}, }},
        {0.81, {{ 5,    23,  5, -1}, { 2,   24,  8, -2}, { 0,   23, 12, -3}, {-1,   21, 15, -3}, {-2,   18, 18, -2}, }},
        {0.82, {{ 5,    23,  5, -1}, { 2,   24,  8, -2}, { 0,   23, 12, -3}, {-1,   21, 15, -3}, {-3,   19, 19, -3}, }},
        {0.83, {{ 5,    23,  5, -1}, { 2,   24,  8, -2}, { 0,   23, 11, -2}, {-2,   22, 15, -3}, {-3,   19, 19, -3}, }},
        {0.84, {{ 4,    25,  4, -1}, { 1,   25,  8, -2}, { 0,   23, 11, -2}, {-2,   22, 15, -3}, {-3,   19, 19, -3}, }},
        {0.85, {{ 4,    25,  4, -1}, { 1,   25,  8, -2}, { 0,   23, 11, -2}, {-2,   22, 15, -3}, {-3,   19, 19, -3}, }},
        {0.86, {{ 4,    24,  4,  0}, { 1,   25,  7, -1}, {-1,   24, 11, -2}, {-2,   22, 15, -3}, {-3,   19, 19, -3}, }},
        {0.87, {{ 4,    24,  4,  0}, { 1,   25,  7, -1}, {-1,   24, 11, -2}, {-2,   22, 15, -3}, {-3,   19, 19, -3}, }},
        {0.88, {{ 3,    26,  3,  0}, { 0,   26,  7, -1}, {-1,   24, 11, -2}, {-3,   23, 15, -3}, {-3,   19, 19, -3}, }},
        {0.89, {{ 3,    26,  3,  0}, { 0,   26,  7, -1}, {-1,   24, 11, -2}, {-3,   23, 15, -3}, {-3,   19, 19, -3}, }},
        {0.90, {{ 3,    26,  3,  0}, { 0,   26,  7, -1}, {-2,   25, 11, -2}, {-3,   23, 15, -3}, {-3,   19, 19, -3}, }},
        {0.91, {{ 3,    26,  3,  0}, { 0,   27,  6, -1}, {-2,   25, 11, -2}, {-3,   23, 15, -3}, {-3,   19, 19, -3}, }},
        {0.92, {{ 2,    28,  2,  0}, { 0,   27,  6, -1}, {-2,   25, 11, -2}, {-3,   23, 15, -3}, {-3,   19, 19, -3}, }},
        {0.93, {{ 2,    28,  2,  0}, { 0,   26,  6,  0}, {-2,   25, 10, -1}, {-3,   23, 15, -3}, {-3,   19, 19, -3}, }},
        {0.94, {{ 2,    28,  2,  0}, { 0,   26,  6,  0}, {-2,   25, 10, -1}, {-3,   23, 15, -3}, {-3,   19, 19, -3}, }},
        {0.95, {{ 1,    30,  1,  0}, {-1,   28,  5,  0}, {-3,   26, 10, -1}, {-3,   23, 14, -2}, {-3,   19, 19, -3}, }},
        {0.96, {{ 1,    30,  1,  0}, {-1,   28,  5,  0}, {-3,   26, 10, -1}, {-3,   23, 14, -2}, {-3,   19, 19, -3}, }},
        {0.97, {{ 1,    30,  1,  0}, {-1,   28,  5,  0}, {-3,   26, 10, -1}, {-3,   23, 14, -2}, {-3,   19, 19, -3}, }},
        {0.98, {{ 1,    30,  1,  0}, {-2,   29,  5,  0}, {-3,   27,  9, -1}, {-3,   23, 14, -2}, {-3,   19, 19, -3}, }},
        {0.99, {{ 0,    32,  0,  0}, {-2,   29,  5,  0}, {-3,   27,  9, -1}, {-4,   24, 14, -2}, {-3,   19, 19, -3}, }},
        {1.00, {{ 0,    32,  0,  0}, {-2,   29,  5,  0}, {-3,   27,  9, -1}, {-4,   24, 14, -2}, {-3,   19, 19, -3}, }}
    };

static void
RADEONCopyData(
  ScrnInfoPtr pScrn,
  unsigned char *src,
  unsigned char *dst,
  unsigned int srcPitch,
  unsigned int dstPitch,
  unsigned int h,
  unsigned int w,
  unsigned int bpp
){
    RADEONInfoPtr info = RADEONPTR(pScrn);
#ifdef XF86DRI

    if ( info->directRenderingEnabled && info->DMAForXv )
    {
	CARD8 *buf;
	CARD32 bufPitch, dstPitchOff;
	int x, y;
	unsigned int hpass;

	/* Get the byte-swapping right for big endian systems */
	if ( bpp == 2 )
	{
	    w *= 2;
	    bpp = 1;
	}

	RADEONHostDataParams( pScrn, dst, dstPitch, bpp, &dstPitchOff, &x, &y );

	while ( (buf = RADEONHostDataBlit( pScrn, bpp, w, dstPitchOff, &bufPitch,
					   x, &y, &h, &hpass )) )
	{
	    RADEONHostDataBlitCopyPass( pScrn, bpp, buf, src, hpass, bufPitch,
					srcPitch );
	    src += hpass * srcPitch;
	}

	FLUSH_RING();

	return;
    }
    else
#endif /* XF86DRI */
    {
#if X_BYTE_ORDER == X_BIG_ENDIAN
	unsigned char *RADEONMMIO = info->MMIO;
	unsigned int swapper = info->ModeReg.surface_cntl &
		~(RADEON_NONSURF_AP0_SWP_32BPP | RADEON_NONSURF_AP1_SWP_32BPP |
		  RADEON_NONSURF_AP0_SWP_16BPP | RADEON_NONSURF_AP1_SWP_16BPP);

	switch(bpp) {
	case 2:
	    swapper |= RADEON_NONSURF_AP0_SWP_16BPP
		    |  RADEON_NONSURF_AP1_SWP_16BPP;
	    break;
	case 4:
	    swapper |= RADEON_NONSURF_AP0_SWP_32BPP
		    |  RADEON_NONSURF_AP1_SWP_32BPP;
	    break;
	}
	OUTREG(RADEON_SURFACE_CNTL, swapper);
#endif
	w *= bpp;

	while (h--) {
	    memcpy(dst, src, w);
	    src += srcPitch;
	    dst += dstPitch;
	}

#if X_BYTE_ORDER == X_BIG_ENDIAN
	/* restore byte swapping */
	OUTREG(RADEON_SURFACE_CNTL, info->ModeReg.surface_cntl);
#endif
    }
}

#ifdef XF86DRI
static void RADEON_420_422(
    unsigned int *d,
    unsigned char *s1,
    unsigned char *s2,
    unsigned char *s3,
    unsigned int n
)
{
    while ( n ) {
	*(d++) = s1[0] | (s1[1] << 16) | (s3[0] << 8) | (s2[0] << 24);
	s1+=2; s2++; s3++;
	n--;
    }
}
#endif

static void
RADEONCopyRGB24Data(
  ScrnInfoPtr pScrn,
  unsigned char *src,
  unsigned char *dst,
  unsigned int srcPitch,
  unsigned int dstPitch,
  unsigned int h,
  unsigned int w
){
    CARD32 *dptr;
    CARD8 *sptr = 0;
    int i,j;
    RADEONInfoPtr info = RADEONPTR(pScrn);
#ifdef XF86DRI

    if ( info->directRenderingEnabled && info->DMAForXv )
    {
	CARD32 bufPitch, dstPitchOff;
	int x, y;
	unsigned int hpass;

	/* XXX Fix endian flip on R300 */

	RADEONHostDataParams( pScrn, dst, dstPitch, 4, &dstPitchOff, &x, &y );

	while ( (dptr = ( CARD32* )RADEONHostDataBlit( pScrn, 4, w, dstPitch,
						       &bufPitch, x, &y, &h,
						       &hpass )) )
	{
	    for( j = 0; j < hpass; j++ )
	    {
		sptr = src;

		for ( i = 0 ; i < w; i++, sptr += 3 )
		{
		    *dptr++ = (sptr[0] << 24) | (sptr[1] << 16) | sptr[2];
		}

		src += hpass * srcPitch;
		dptr += hpass * bufPitch;
	    }
	}

	FLUSH_RING();

	return;
    }
    else
#endif /* XF86DRI */
    {
#if X_BYTE_ORDER == X_BIG_ENDIAN
	unsigned char *RADEONMMIO = info->MMIO;
	OUTREG(RADEON_SURFACE_CNTL, (info->ModeReg.surface_cntl
				   | RADEON_NONSURF_AP0_SWP_32BPP)
				  & ~RADEON_NONSURF_AP0_SWP_16BPP);
#endif

	for(j=0;j<h;j++){
	    dptr=(CARD32 *)(dst+j*dstPitch);
	    sptr=src+j*srcPitch;

	    for(i=w;i>0;i--){
	      dptr[0]=((sptr[0])<<24)|((sptr[1])<<16)|(sptr[2]);
	      dptr++;
	      sptr+=3;
	    }
	}

#if X_BYTE_ORDER == X_BIG_ENDIAN
	/* restore byte swapping */
	OUTREG(RADEON_SURFACE_CNTL, info->ModeReg.surface_cntl);
#endif
    }
}

static void
RADEONCopyMungedData(
   ScrnInfoPtr pScrn,
   unsigned char *src1,
   unsigned char *src2,
   unsigned char *src3,
   unsigned char *dst1,
   unsigned int srcPitch,
   unsigned int srcPitch2,
   unsigned int dstPitch,
   unsigned int h,
   unsigned int w
){
    RADEONInfoPtr info = RADEONPTR(pScrn);
#ifdef XF86DRI

    if ( info->directRenderingEnabled && info->DMAForXv )
    {
	CARD8 *buf;
	CARD32 y = 0, bufPitch, dstPitchOff;
	int blitX, blitY;
	unsigned int hpass;

	/* XXX Fix endian flip on R300 */

	RADEONHostDataParams( pScrn, dst1, dstPitch, 4, &dstPitchOff, &blitX, &blitY );

	while ( (buf = RADEONHostDataBlit( pScrn, 4, w/2, dstPitchOff, &bufPitch,
					   blitX, &blitY, &h, &hpass )) )
	{
	    while ( hpass-- )
	    {
		RADEON_420_422( (unsigned int *) buf, src1, src2, src3,
				bufPitch / 4 );
		src1 += srcPitch;
		if ( y & 1 )
		{
		    src2 += srcPitch2;
		    src3 += srcPitch2;
		}
		buf += bufPitch;
		y++;
	    }
	}

	FLUSH_RING();
    }
    else
#endif /* XF86DRI */
    {
	CARD32 *dst;
	CARD8 *s1, *s2, *s3;
	int i, j;

#if X_BYTE_ORDER == X_BIG_ENDIAN
	unsigned char *RADEONMMIO = info->MMIO;
	OUTREG(RADEON_SURFACE_CNTL, (info->ModeReg.surface_cntl
				   | RADEON_NONSURF_AP0_SWP_32BPP)
				  & ~RADEON_NONSURF_AP0_SWP_16BPP);
#endif

	w /= 2;

	for( j = 0; j < h; j++ )
	{
	    dst = (pointer)dst1;
	    s1 = src1;  s2 = src2;  s3 = src3;
	    i = w;
	    while( i > 4 )
	    {
		dst[0] = s1[0] | (s1[1] << 16) | (s3[0] << 8) | (s2[0] << 24);
		dst[1] = s1[2] | (s1[3] << 16) | (s3[1] << 8) | (s2[1] << 24);
		dst[2] = s1[4] | (s1[5] << 16) | (s3[2] << 8) | (s2[2] << 24);
		dst[3] = s1[6] | (s1[7] << 16) | (s3[3] << 8) | (s2[3] << 24);
		dst += 4; s2 += 4; s3 += 4; s1 += 8;
		i -= 4;
	    }
	    while( i-- )
	    {
		dst[0] = s1[0] | (s1[1] << 16) | (s3[0] << 8) | (s2[0] << 24);
		dst++; s2++; s3++;
		s1 += 2;
	    }

	    dst1 += dstPitch;
	    src1 += srcPitch;
	    if( j & 1 )
	    {
		src2 += srcPitch2;
		src3 += srcPitch2;
	    }	
	}
#if X_BYTE_ORDER == X_BIG_ENDIAN
	/* restore byte swapping */
	OUTREG(RADEON_SURFACE_CNTL, info->ModeReg.surface_cntl);
#endif
    }
}

/* Allocates memory, either by resizing the allocation pointed to by mem_struct,
 * or by freeing mem_struct (if non-NULL) and allocating a new space.  The size
 * is measured in bytes, and the offset from the beginning of card space is
 * returned.
 */
static CARD32
RADEONAllocateMemory(
   ScrnInfoPtr pScrn,
   void **mem_struct,
   int size
){
    ScreenPtr pScreen;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    int offset = 0;

    pScreen = screenInfo.screens[pScrn->scrnIndex];
#ifdef USE_EXA
    if (info->useEXA) {
	ExaOffscreenArea *area = *mem_struct;

	if (area != NULL) {
	    if (area->size >= size)
		return area->offset;

	    exaOffscreenFree(pScrn->pScreen, area);
	}

	area = exaOffscreenAlloc(pScrn->pScreen, size, 64, TRUE, ATIVideoSave,
				 NULL);
	*mem_struct = area;
	if (area == NULL)
	    return 0;
	offset = area->offset;
    }
#endif /* USE_EXA */
#ifdef USE_XAA
    if (!info->useEXA) {
	FBLinearPtr linear = *mem_struct;
	int cpp = info->CurrentLayout.bitsPerPixel / 8;

	/* XAA allocates in units of pixels at the screen bpp, so adjust size
	 * appropriately.
	 */
	size = (size + cpp - 1) / cpp;

	if (linear) {
	    if(linear->size >= size)
		return linear->offset * cpp;

	    if(xf86ResizeOffscreenLinear(linear, size))
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

	    if(max_size < size)
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
RADEONFreeMemory(
   ScrnInfoPtr pScrn,
   void *mem_struct
){
    RADEONInfoPtr info = RADEONPTR(pScrn);

#ifdef USE_EXA
    if (info->useEXA) {
	ExaOffscreenArea *area = mem_struct;

	if (area != NULL)
	    exaOffscreenFree(pScrn->pScreen, area);
    }
#endif /* USE_EXA */
#ifdef USE_XAA
    if (!info->useEXA) {
	FBLinearPtr linear = mem_struct;

	if (linear != NULL)
	    xf86FreeOffscreenLinear(linear);
    }
#endif /* USE_XAA */
}

static void
RADEONDisplayVideo(
    ScrnInfoPtr pScrn,
    RADEONPortPrivPtr pPriv, 
    int id,
    int offset1, int offset2,
    int offset3, int offset4,
    short width, short height,
    int pitch,
    int left, int right, int top,
    BoxPtr dstBox,
    short src_w, short src_h,
    short drw_w, short drw_h,
    int deinterlacing_method
){
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32 v_inc, h_inc, h_inc_uv, step_by_y, step_by_uv, tmp;
    double h_inc_d;
    int p1_h_accum_init, p23_h_accum_init;
    int p1_v_accum_init;
    int ecp_div;
    int v_inc_shift;
    int y_mult;
    int x_off;
    int y_off;
    CARD32 scaler_src;
    CARD32 dot_clock;
    DisplayModePtr overlay_mode;
    int is_rgb;
    int i;
    CARD32 scale_cntl;
    double dsr;
    int tap_set;

    is_rgb=0;
    switch(id){
        case FOURCC_RGBA32:
        case FOURCC_RGB24:
        case FOURCC_RGBT16:
        case FOURCC_RGB16:
            is_rgb=1;
            break;
        default:
	    break;
        }

    /* Unlike older Mach64 chips, RADEON has only two ECP settings: 0 for PIXCLK < 175Mhz, and 1 (divide by 2)
       for higher clocks, sure makes life nicer

       Here we need to find ecp_div again, as the user may have switched resolutions */

    /* Figure out which head we are on for dot clock */
    if ((info->MergedFB && info->OverlayOnCRTC2) || info->IsSecondary)
        dot_clock = info->ModeReg.dot_clock_freq_2;
    else
        dot_clock = info->ModeReg.dot_clock_freq;

    if (dot_clock < 17500)
        ecp_div = 0;
    else
	ecp_div = 1;

    OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL,
	   (INPLL(pScrn, RADEON_VCLK_ECP_CNTL) & 0xfffffCff) | (ecp_div << 8));

    /* I suspect we may need a usleep after writing to the PLL.  if you play a video too soon
       after switching crtcs in mergedfb clone mode you get a temporary one pixel line of colorkey 
       on the right edge video output.  */

    v_inc_shift = 20;
    y_mult = 1;

    if (info->MergedFB) {
        if (info->OverlayOnCRTC2)
	    overlay_mode = ((RADEONMergedDisplayModePtr)info->CurrentLayout.mode->Private)->CRT2;
	else
	    overlay_mode = ((RADEONMergedDisplayModePtr)info->CurrentLayout.mode->Private)->CRT1;
	if (overlay_mode->Flags & V_INTERLACE)
	    v_inc_shift++;
    	if (overlay_mode->Flags & V_DBLSCAN) {
	    v_inc_shift--;
	    y_mult = 2;
	}
    	if (overlay_mode->Flags & RADEON_USE_RMX) {
	    v_inc = ((src_h * overlay_mode->CrtcVDisplay / info->PanelYRes) << v_inc_shift) / drw_h;
    	} else {
	    v_inc = (src_h << v_inc_shift) / drw_h;
    	}
    } else {
	if (pScrn->currentMode->Flags & V_INTERLACE)
	    v_inc_shift++;
    	if (pScrn->currentMode->Flags & V_DBLSCAN) {
	    v_inc_shift--;
	    y_mult = 2;
	}
    	if (pScrn->currentMode->Flags & RADEON_USE_RMX) {
	    v_inc = ((src_h * pScrn->currentMode->CrtcVDisplay / info->PanelYRes) << v_inc_shift) / drw_h;
    	} else {
	    v_inc = (src_h << v_inc_shift) / drw_h;
    	}
    }

    h_inc = (1 << (12 + ecp_div));

    step_by_y = 1;
    step_by_uv = step_by_y;

    /* if the source width was larger than what would fit in overlay scaler increase step_by values */
    i=src_w;
    while(i>pPriv->overlay_scaler_buffer_width){
    	step_by_y++;
	step_by_uv++;
	h_inc >>=1;
	i=i/2;
    	}


    h_inc_d = src_w;
    h_inc_d = h_inc_d/drw_w;
    /* we could do a tad better  - but why
       bother when this concerns downscaling and the code is so much more
       hairy */
    while(h_inc*h_inc_d >= (2 << 12)) {
        if(!is_rgb && (((h_inc+h_inc/2)*h_inc_d)<(2<<12))){
                step_by_uv = step_by_y+1;
                break;
                }
        step_by_y++;
        step_by_uv = step_by_y;
        h_inc >>= 1;
        }
    h_inc_uv = h_inc>>(step_by_uv-step_by_y);
    h_inc = h_inc * h_inc_d;
    h_inc_uv = h_inc_uv * h_inc_d;
    /* pPriv->overlay_scaler_buffer_width is magic number - maximum line length the overlay scaler can fit 
       in the buffer for 2 tap filtering */
    /* the only place it is documented in is in ATI source code */
    /* we need twice as much space for 4 tap filtering.. */
    /* under special circumstances turn on 4 tap filtering */
    /* disable this code for now as it has a DISASTROUS effect on image quality when upscaling
       at least on rv250 (only as long as the drw_w*2 <=... requirement is still met of course) */
#if 0
    if(!is_rgb && (step_by_y==1) && (step_by_uv==1) && (h_inc < (1<<12))
       && (deinterlacing_method!=METHOD_WEAVE)
       && (drw_w*2 <= pPriv->overlay_scaler_buffer_width)){
        step_by_y=0;
        step_by_uv=1;
        h_inc_uv = h_inc;
        }
#endif

    /* keep everything in 16.16 */

    offset1 += ((left >> 16) & ~7) << 1;
    offset2 += ((left >> 16) & ~7) << 1;
    offset3 += ((left >> 16) & ~7) << 1;
    offset4 += ((left >> 16) & ~7) << 1;

    if (info->IsSecondary) {
	offset1 += info->FbMapSize;
	offset2 += info->FbMapSize;
    	offset3 += info->FbMapSize;
    	offset4 += info->FbMapSize;
    }

    tmp = (left & 0x0003ffff) + 0x00028000 + (h_inc << 3);
    p1_h_accum_init = ((tmp <<  4) & 0x000f8000) |
		      ((tmp << 12) & 0xf0000000);

    tmp = ((left >> 1) & 0x0001ffff) + 0x00028000 + (h_inc_uv << 2);
    p23_h_accum_init = ((tmp <<  4) & 0x000f8000) |
		       ((tmp << 12) & 0x70000000);

    tmp = (top & 0x0000ffff) + 0x00018000;
    p1_v_accum_init = ((tmp << 4) & 0x03ff8000) | 
    	(((deinterlacing_method!=METHOD_WEAVE)&&!is_rgb)?0x03:0x01);

    left = (left >> 16) & 7;

    RADEONWaitForFifo(pScrn, 2);
    OUTREG(RADEON_OV0_REG_LOAD_CNTL, RADEON_REG_LD_CTL_LOCK);
    if (info->accelOn) RADEON_SYNC(info, pScrn);
    while(!(INREG(RADEON_OV0_REG_LOAD_CNTL) & RADEON_REG_LD_CTL_LOCK_READBACK));

    RADEONWaitForFifo(pScrn, 10);
    OUTREG(RADEON_OV0_H_INC, h_inc | ((h_inc_uv >> 1) << 16));
    OUTREG(RADEON_OV0_STEP_BY, step_by_y | (step_by_uv << 8));

    x_off = 8;
    y_off = 0;

    if (IS_R300_VARIANT ||
        (info->ChipFamily == CHIP_FAMILY_R200))
	x_off = 0;

    /* needed to make the overlay work on crtc1 in leftof and above modes */
    if (info->MergedFB) {
	RADEONScrn2Rel srel =
	    ((RADEONMergedDisplayModePtr)info->CurrentLayout.mode->Private)->CRT2Position;
	overlay_mode = ((RADEONMergedDisplayModePtr)info->CurrentLayout.mode->Private)->CRT2;
	if (srel == radeonLeftOf) {
    	    x_off -= overlay_mode->CrtcHDisplay;
	    /* y_off -= pScrn->frameY0; */
	}
	if (srel == radeonAbove) {
    	    y_off -= overlay_mode->CrtcVDisplay;
	    /* x_off -= pScrn->frameX0; */
	}
    }

    /* Put the hardware overlay on CRTC2:
     *
     * Since one hardware overlay can not be displayed on two heads
     * at the same time, we might need to consider using software
     * rendering for the second head.
     */

    if ((info->MergedFB && info->OverlayOnCRTC2) || info->IsSecondary) {
        x_off = 0;
        OUTREG(RADEON_OV1_Y_X_START, ((dstBox->x1 + x_off) |
                                      ((dstBox->y1*y_mult) << 16)));
        OUTREG(RADEON_OV1_Y_X_END,   ((dstBox->x2 + x_off) |
                                      ((dstBox->y2*y_mult) << 16)));
        scaler_src = RADEON_SCALER_CRTC_SEL;
    } else {
	OUTREG(RADEON_OV0_Y_X_START, ((dstBox->x1 + x_off) |
				      (((dstBox->y1*y_mult) + y_off) << 16)));
	OUTREG(RADEON_OV0_Y_X_END,   ((dstBox->x2 + x_off) |
				      (((dstBox->y2*y_mult) + y_off) << 16)));
	scaler_src = 0;
    }

    /* program the tap coefficients for better downscaling quality.
       Could do slightly better by using hardcoded coefficients for one axis
       in case only the other axis is downscaled (see RADEON_OV0_FILTER_CNTL) */
    dsr=(double)(1<<0xC)/h_inc;
    if(dsr<0.25)dsr=0.25;
    if(dsr>1.0)dsr=1.0;
    tap_set=(int)((dsr-0.25)*100);
    for(i=0;i<5;i++){
	    OUTREG(RADEON_OV0_FOUR_TAP_COEF_0+i*4, (TapCoeffs[tap_set].coeff[i][0] &0xf) | 
	    	((TapCoeffs[tap_set].coeff[i][1] &0x7f)<<8) | 
	    	((TapCoeffs[tap_set].coeff[i][2] &0x7f)<<16) | 
	    	((TapCoeffs[tap_set].coeff[i][3] &0xf)<<24));
		}

    RADEONWaitForFifo(pScrn, 10);
    OUTREG(RADEON_OV0_V_INC, v_inc);
    OUTREG(RADEON_OV0_P1_BLANK_LINES_AT_TOP, 0x00000fff | ((src_h - 1) << 16));
    OUTREG(RADEON_OV0_VID_BUF_PITCH0_VALUE, pitch);
    OUTREG(RADEON_OV0_VID_BUF_PITCH1_VALUE, pitch);
    OUTREG(RADEON_OV0_P1_X_START_END, (src_w + left - 1) | (left << 16));
    left >>= 1; src_w >>= 1;
    OUTREG(RADEON_OV0_P2_X_START_END, (src_w + left - 1) | (left << 16));
    OUTREG(RADEON_OV0_P3_X_START_END, (src_w + left - 1) | (left << 16));
    OUTREG(RADEON_OV0_VID_BUF0_BASE_ADRS, offset1 & 0xfffffff0);
    OUTREG(RADEON_OV0_VID_BUF1_BASE_ADRS, offset2 & 0xfffffff0);
    OUTREG(RADEON_OV0_VID_BUF2_BASE_ADRS, offset3 & 0xfffffff0);

    RADEONWaitForFifo(pScrn, 9);
    OUTREG(RADEON_OV0_VID_BUF3_BASE_ADRS, offset4 & 0xfffffff0);
    OUTREG(RADEON_OV0_VID_BUF4_BASE_ADRS, offset1 & 0xfffffff0);
    OUTREG(RADEON_OV0_VID_BUF5_BASE_ADRS, offset2 & 0xfffffff0);
    OUTREG(RADEON_OV0_P1_V_ACCUM_INIT, p1_v_accum_init);
    OUTREG(RADEON_OV0_P1_H_ACCUM_INIT, p1_h_accum_init);
    OUTREG(RADEON_OV0_P23_H_ACCUM_INIT, p23_h_accum_init);

   scale_cntl = RADEON_SCALER_ADAPTIVE_DEINT | RADEON_SCALER_DOUBLE_BUFFER 
        | RADEON_SCALER_ENABLE | RADEON_SCALER_SMART_SWITCH | (0x7f<<16) | scaler_src;
   switch(id){
        case FOURCC_UYVY:
		scale_cntl |= RADEON_SCALER_SOURCE_YVYU422;
        case FOURCC_RGB24:
        case FOURCC_RGBA32:
		scale_cntl |= RADEON_SCALER_SOURCE_32BPP | RADEON_SCALER_LIN_TRANS_BYPASS;
		break;
        case FOURCC_RGB16:
		scale_cntl |= RADEON_SCALER_SOURCE_16BPP | RADEON_SCALER_LIN_TRANS_BYPASS;
		break;
        case FOURCC_RGBT16:
		scale_cntl |= RADEON_SCALER_SOURCE_15BPP | RADEON_SCALER_LIN_TRANS_BYPASS;
		break;
        case FOURCC_YUY2:
        case FOURCC_YV12:
        case FOURCC_I420:
        default:
		scale_cntl |= RADEON_SCALER_SOURCE_VYUY422
			| ((info->ChipFamily >= CHIP_FAMILY_R200) ? RADEON_SCALER_TEMPORAL_DEINT : 0);
		break;
    }
    OUTREG(RADEON_OV0_SCALE_CNTL, scale_cntl);
    OUTREG(RADEON_OV0_REG_LOAD_CNTL, 0);
}


static int
RADEONPutImage(
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
   RADEONInfoPtr info = RADEONPTR(pScrn);
   RADEONPortPrivPtr pPriv = (RADEONPortPrivPtr)data;
   INT32 xa, xb, ya, yb;
   unsigned char *dst_start;
   int new_size, offset, s2offset, s3offset;
   int srcPitch, srcPitch2, dstPitch;
   int top, left, npixels, nlines, bpp;
   BoxRec dstBox;
   CARD32 tmp;

   /*
    * s2offset, s3offset - byte offsets into U and V plane of the
    *                      source where copying starts.  Y plane is
    *                      done by editing "buf".
    *
    * offset - byte offset to the first line of the destination.
    *
    * dst_start - byte address to the first displayed pel.
    *
    */

   /* make the compiler happy */
   s2offset = s3offset = srcPitch2 = 0;

   if(src_w > (drw_w << 4))
	drw_w = src_w >> 4;
   if(src_h > (drw_h << 4))
	drw_h = src_h >> 4;

   /* Clip */
   xa = src_x;
   xb = src_x + src_w;
   ya = src_y;
   yb = src_y + src_h;

   dstBox.x1 = drw_x;
   dstBox.x2 = drw_x + drw_w;
   dstBox.y1 = drw_y;
   dstBox.y2 = drw_y + drw_h;

   if (info->MergedFB)
	RADEONChooseOverlayCRTC(pScrn, &dstBox);

   if(!xf86XVClipVideoHelper(&dstBox, &xa, &xb, &ya, &yb,
			     clipBoxes, width, height))
	return Success;

   if (info->MergedFB && info->OverlayOnCRTC2) {
	dstBox.x1 -= info->CRT2pScrn->frameX0;
	dstBox.x2 -= info->CRT2pScrn->frameX0;
	dstBox.y1 -= info->CRT2pScrn->frameY0;
	dstBox.y2 -= info->CRT2pScrn->frameY0;
   } else {
	dstBox.x1 -= pScrn->frameX0;
	dstBox.x2 -= pScrn->frameX0;
	dstBox.y1 -= pScrn->frameY0;
	dstBox.y2 -= pScrn->frameY0;
   }

   bpp = pScrn->bitsPerPixel >> 3;

   switch(id) {
   case FOURCC_RGB24:
   	dstPitch=(width*4+0x0f)&(~0x0f);
	srcPitch=width*3;
	break;
   case FOURCC_RGBA32:
   	dstPitch=(width*4+0x0f)&(~0x0f);
	srcPitch=width*4;
	break;
   case FOURCC_RGB16:
   case FOURCC_RGBT16:
   	dstPitch=(width*2+0x0f)&(~0x0f);
	srcPitch=(width*2+3)&(~0x03);
	break;
   case FOURCC_YV12:
   case FOURCC_I420:
	dstPitch = ((width << 1) + 63) & ~63;
	srcPitch = (width + 3) & ~3;
	s2offset = srcPitch * height;
	srcPitch2 = ((width >> 1) + 3) & ~3;
	s3offset = (srcPitch2 * (height >> 1)) + s2offset;
	break;
   case FOURCC_UYVY:
   case FOURCC_YUY2:
   default:
	dstPitch = ((width << 1) + 63) & ~63;
	srcPitch = (width << 1);
	break;
   }

   new_size = dstPitch * height;
   pPriv->video_offset = RADEONAllocateMemory(pScrn, &pPriv->video_memory,
					      (pPriv->doubleBuffer ?
					       (new_size * 2) : new_size));
   if (pPriv->video_offset == 0)
      return BadAlloc;

   pPriv->currentBuffer ^= 1;

    /* copy data */
   top = ya >> 16;
   left = (xa >> 16) & ~1;
   npixels = ((((xb + 0xffff) >> 16) + 1) & ~1) - left;

   offset = (pPriv->video_offset) + (top * dstPitch);

   if(pPriv->doubleBuffer)
	offset += pPriv->currentBuffer * new_size;

   dst_start = info->FB + offset;

   switch(id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	top &= ~1;
	dst_start += left << 1;
	tmp = ((top >> 1) * srcPitch2) + (left >> 1);
	s2offset += tmp;
	s3offset += tmp;
	if(id == FOURCC_I420) {
	   tmp = s2offset;
	   s2offset = s3offset;
	   s3offset = tmp;
	}
	nlines = ((((yb + 0xffff) >> 16) + 1) & ~1) - top;
	RADEONCopyMungedData(pScrn, buf + (top * srcPitch) + left,
			     buf + s2offset, buf + s3offset, dst_start,
			     srcPitch, srcPitch2, dstPitch, nlines, npixels);
	break;
    case FOURCC_RGBT16:
    case FOURCC_RGB16:
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	left <<= 1;
	buf += (top * srcPitch) + left;
	nlines = ((yb + 0xffff) >> 16) - top;
	dst_start += left;
	RADEONCopyData(pScrn, buf, dst_start, srcPitch, dstPitch, nlines, npixels, 2);
	break;
    case FOURCC_RGBA32:
	buf += (top * srcPitch) + left*4;
	nlines = ((yb + 0xffff) >> 16) - top;
	dst_start += left*4;
	RADEONCopyData(pScrn, buf, dst_start, srcPitch, dstPitch, nlines, npixels, 4);
    	break;
    case FOURCC_RGB24:
	buf += (top * srcPitch) + left*3;
	nlines = ((yb + 0xffff) >> 16) - top;
	dst_start += left*4;
	RADEONCopyRGB24Data(pScrn, buf, dst_start, srcPitch, dstPitch, nlines, npixels);
    	break;
    }

    /* update cliplist */
    if(!REGION_EQUAL(pScrn->pScreen, &pPriv->clip, clipBoxes))
    {
	REGION_COPY(pScrn->pScreen, &pPriv->clip, clipBoxes);
	/* draw these */
	if(pPriv->autopaint_colorkey)
	    xf86XVFillKeyHelper(pScrn->pScreen, pPriv->colorKey, clipBoxes);
    }

    RADEONDisplayVideo(pScrn, pPriv, id, offset, offset, offset, offset, width, height, dstPitch,
		     xa, xb, ya, &dstBox, src_w, src_h, drw_w, drw_h, METHOD_BOB);

    pPriv->videoStatus = CLIENT_VIDEO_ON;

    info->VideoTimerCallback = RADEONVideoTimerCallback;

    return Success;
}


static int
RADEONQueryImageAttributes(
    ScrnInfoPtr pScrn,
    int id,
    unsigned short *w, unsigned short *h,
    int *pitches, int *offsets
){
    int size, tmp;

    if(*w > 2048) *w = 2048;
    if(*h > 2048) *h = 2048;

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
    case FOURCC_RGBA32:
	size = *w << 2;
	if(pitches) pitches[0] = size;
	size *= *h;
	break;
    case FOURCC_RGB24:
	size = *w * 3;
	if(pitches) pitches[0] = size;
	size *= *h;
	break;
    case FOURCC_RGBT16:
    case FOURCC_RGB16:
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
RADEONVideoTimerCallback(ScrnInfoPtr pScrn, Time now)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONPortPrivPtr pPriv = info->adaptor->pPortPrivates[0].ptr;

    if(pPriv->videoStatus & TIMER_MASK) {
	if(pPriv->videoStatus & OFF_TIMER) {
	    if(pPriv->offTime < now) {
		unsigned char *RADEONMMIO = info->MMIO;
		OUTREG(RADEON_OV0_SCALE_CNTL, 0);
		pPriv->videoStatus = FREE_TIMER;
		pPriv->freeTime = now + FREE_DELAY;
	    }
	} else {  /* FREE_TIMER */
	    if(pPriv->freeTime < now) {
		if (pPriv->video_memory != NULL) {
		    RADEONFreeMemory(pScrn, pPriv->video_memory);
		    pPriv->video_memory = NULL;
		}
		pPriv->videoStatus = 0;
		info->VideoTimerCallback = NULL;
	    }
	}
    } else  /* shouldn't get here */
	info->VideoTimerCallback = NULL;
}

/****************** Offscreen stuff ***************/
typedef struct {
  void *surface_memory;
  Bool isOn;
} OffscreenPrivRec, * OffscreenPrivPtr;

static int
RADEONAllocateSurface(
    ScrnInfoPtr pScrn,
    int id,
    unsigned short w,
    unsigned short h,
    XF86SurfacePtr surface
){
    int offset, pitch, size;
    OffscreenPrivPtr pPriv;
    void *surface_memory = NULL;
    if((w > 1024) || (h > 1024))
	return BadAlloc;

    w = (w + 1) & ~1;
    pitch = ((w << 1) + 15) & ~15;
    size = pitch * h;

    offset = RADEONAllocateMemory(pScrn, &surface_memory, size);
    if (offset == 0)
	return BadAlloc;

    surface->width = w;
    surface->height = h;

    if(!(surface->pitches = xalloc(sizeof(int)))) {
	RADEONFreeMemory(pScrn, surface_memory);
	return BadAlloc;
    }
    if(!(surface->offsets = xalloc(sizeof(int)))) {
	xfree(surface->pitches);
	RADEONFreeMemory(pScrn, surface_memory);
	return BadAlloc;
    }
    if(!(pPriv = xalloc(sizeof(OffscreenPrivRec)))) {
	xfree(surface->pitches);
	xfree(surface->offsets);
	RADEONFreeMemory(pScrn, surface_memory);
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
RADEONStopSurface(
    XF86SurfacePtr surface
){
  OffscreenPrivPtr pPriv = (OffscreenPrivPtr)surface->devPrivate.ptr;
  RADEONInfoPtr info = RADEONPTR(surface->pScrn);
  unsigned char *RADEONMMIO = info->MMIO;

  if(pPriv->isOn) {
	OUTREG(RADEON_OV0_SCALE_CNTL, 0);
	pPriv->isOn = FALSE;
  }
  return Success;
}


static int
RADEONFreeSurface(
    XF86SurfacePtr surface
){
    ScrnInfoPtr pScrn = surface->pScrn;
    OffscreenPrivPtr pPriv = (OffscreenPrivPtr)surface->devPrivate.ptr;

    if(pPriv->isOn)
	RADEONStopSurface(surface);
    RADEONFreeMemory(pScrn, pPriv->surface_memory);
    xfree(surface->pitches);
    xfree(surface->offsets);
    xfree(surface->devPrivate.ptr);

    return Success;
}

static int
RADEONGetSurfaceAttribute(
    ScrnInfoPtr pScrn,
    Atom attribute,
    INT32 *value
){
   return RADEONGetPortAttribute(pScrn, attribute, value,
		(pointer)(GET_PORT_PRIVATE(pScrn)));
}

static int
RADEONSetSurfaceAttribute(
    ScrnInfoPtr pScrn,
    Atom attribute,
    INT32 value
){
   return RADEONSetPortAttribute(pScrn, attribute, value,
		(pointer)(GET_PORT_PRIVATE(pScrn)));
}


static int
RADEONDisplaySurface(
    XF86SurfacePtr surface,
    short src_x, short src_y,
    short drw_x, short drw_y,
    short src_w, short src_h,
    short drw_w, short drw_h,
    RegionPtr clipBoxes
){
    OffscreenPrivPtr pPriv = (OffscreenPrivPtr)surface->devPrivate.ptr;
    ScrnInfoPtr pScrn = surface->pScrn;

    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONPortPrivPtr portPriv = info->adaptor->pPortPrivates[0].ptr;

    INT32 xa, ya, xb, yb;
    BoxRec dstBox;

    if (src_w > (drw_w << 4))
	drw_w = src_w >> 4;
    if (src_h > (drw_h << 4))
	drw_h = src_h >> 4;

    xa = src_x;
    xb = src_x + src_w;
    ya = src_y;
    yb = src_y + src_h;

    dstBox.x1 = drw_x;
    dstBox.x2 = drw_x + drw_w;
    dstBox.y1 = drw_y;
    dstBox.y2 = drw_y + drw_h;

    if (info->MergedFB)
        RADEONChooseOverlayCRTC(pScrn, &dstBox);

    if (!xf86XVClipVideoHelper(&dstBox, &xa, &xb, &ya, &yb, clipBoxes, 
			       surface->width, surface->height))
	return Success;

    if (info->MergedFB && info->OverlayOnCRTC2) {
	dstBox.x1 -= info->CRT2pScrn->frameX0;
	dstBox.x2 -= info->CRT2pScrn->frameX0;
	dstBox.y1 -= info->CRT2pScrn->frameY0;
	dstBox.y2 -= info->CRT2pScrn->frameY0;
    } else {
	dstBox.x1 -= pScrn->frameX0;
	dstBox.x2 -= pScrn->frameX0;
	dstBox.y1 -= pScrn->frameY0;
	dstBox.y2 -= pScrn->frameY0;
    }

#if 0
    /* this isn't needed */
    RADEONResetVideo(pScrn);
#endif
    RADEONDisplayVideo(pScrn, portPriv, surface->id,
		       surface->offsets[0], surface->offsets[0],
		       surface->offsets[0], surface->offsets[0],
		       surface->width, surface->height, surface->pitches[0],
		       xa, xb, ya, &dstBox, src_w, src_h, drw_w, drw_h, METHOD_BOB);

    if (portPriv->autopaint_colorkey)
	xf86XVFillKeyHelper(pScrn->pScreen, portPriv->colorKey, clipBoxes);

    pPriv->isOn = TRUE;
    /* we've prempted the XvImage stream so set its free timer */
    if (portPriv->videoStatus & CLIENT_VIDEO_ON) {
	REGION_EMPTY(pScrn->pScreen, &portPriv->clip);
	UpdateCurrentTime();
	portPriv->videoStatus = FREE_TIMER;
	portPriv->freeTime = currentTime.milliseconds + FREE_DELAY;
	info->VideoTimerCallback = RADEONVideoTimerCallback;
    }

    return Success;
}


static void
RADEONInitOffscreenImages(ScreenPtr pScreen)
{
/*  ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn); */
    XF86OffscreenImagePtr offscreenImages;
    /* need to free this someplace */

    if (!(offscreenImages = xalloc(sizeof(XF86OffscreenImageRec))))
	return;

    offscreenImages[0].image = &Images[0];
    offscreenImages[0].flags = VIDEO_OVERLAID_IMAGES |
			       VIDEO_CLIP_TO_VIEWPORT;
    offscreenImages[0].alloc_surface = RADEONAllocateSurface;
    offscreenImages[0].free_surface = RADEONFreeSurface;
    offscreenImages[0].display = RADEONDisplaySurface;
    offscreenImages[0].stop = RADEONStopSurface;
    offscreenImages[0].setAttribute = RADEONSetSurfaceAttribute;
    offscreenImages[0].getAttribute = RADEONGetSurfaceAttribute;
    offscreenImages[0].max_width = 2048;
    offscreenImages[0].max_height = 2048;
    offscreenImages[0].num_attributes = NUM_ATTRIBUTES;
    offscreenImages[0].attributes = Attributes;

    xf86XVRegisterOffscreenImages(pScreen, offscreenImages, 1);
}

         /* TV-in functions */

static int
RADEONPutVideo(
  ScrnInfoPtr pScrn,
  short src_x, short src_y,
  short drw_x, short drw_y,
  short src_w, short src_h,
  short drw_w, short drw_h,
  RegionPtr clipBoxes, pointer data,
  DrawablePtr pDraw
){
   RADEONInfoPtr info = RADEONPTR(pScrn);
   RADEONPortPrivPtr pPriv = (RADEONPortPrivPtr)data;
   unsigned char *RADEONMMIO = info->MMIO;
   INT32 xa, xb, ya, yb, top;
   unsigned int pitch, new_size, alloc_size;
   unsigned int offset1, offset2, offset3, offset4, s2offset, s3offset;
   unsigned int vbi_offset0, vbi_offset1;
   int srcPitch, srcPitch2, dstPitch;
   int bpp;
   BoxRec dstBox;
   CARD32 id, display_base;
   int width, height;
   int mult;
   int vbi_line_width, vbi_start, vbi_end;

    RADEON_SYNC(info, pScrn);
   /*
    * s2offset, s3offset - byte offsets into U and V plane of the
    *                      source where copying starts.  Y plane is
    *                      done by editing "buf".
    *
    * offset - byte offset to the first line of the destination.
    *
    * dst_start - byte address to the first displayed pel.
    *
    */

   /* make the compiler happy */
   s2offset = s3offset = srcPitch2 = 0;

   if(src_w > (drw_w << 4))
        drw_w = src_w >> 4;
   if(src_h > (drw_h << 4))
        drw_h = src_h >> 4;

   /* Clip */
   xa = src_x;
   xb = src_x + src_w;
   ya = src_y;
   yb = src_y + src_h;

   dstBox.x1 = drw_x;
   dstBox.x2 = drw_x + drw_w;
   dstBox.y1 = drw_y;
   dstBox.y2 = drw_y + drw_h;

   width = InputVideoEncodings[pPriv->encoding].width;
   height = InputVideoEncodings[pPriv->encoding].height;

   vbi_line_width = 798*2;
   if(width<=640)
	   vbi_line_width = 0x640; /* 1600 actually */
	   else
	   vbi_line_width = 2000; /* might need adjustment */

   if (info->MergedFB)
        RADEONChooseOverlayCRTC(pScrn, &dstBox);
        
   if(!xf86XVClipVideoHelper(&dstBox, &xa, &xb, &ya, &yb, clipBoxes, width, height))
        return Success;

   if (info->MergedFB && info->OverlayOnCRTC2) {
	dstBox.x1 -= info->CRT2pScrn->frameX0;
	dstBox.x2 -= info->CRT2pScrn->frameX0;
	dstBox.y1 -= info->CRT2pScrn->frameY0;
	dstBox.y2 -= info->CRT2pScrn->frameY0;
   } else {
	dstBox.x1 -= pScrn->frameX0;
	dstBox.x2 -= pScrn->frameX0;
	dstBox.y1 -= pScrn->frameY0;
	dstBox.y2 -= pScrn->frameY0;
   }

   bpp = pScrn->bitsPerPixel >> 3;
   pitch = bpp * pScrn->displayWidth;

   switch(pPriv->overlay_deinterlacing_method){
        case METHOD_BOB:
        case METHOD_SINGLE:
                mult=2;
                break;
        case METHOD_WEAVE:
        case METHOD_ADAPTIVE:
                mult=4;
                break;
        default:
                xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Internal error: PutVideo\n");
                mult=4;
        }

   id = FOURCC_YUY2;
   
   top = ya>>16;

   switch(id) {
   case FOURCC_YV12:
   case FOURCC_I420:
        top &= ~1;
        dstPitch = ((width << 1) + 15) & ~15;
        srcPitch = (width + 3) & ~3;
        s2offset = srcPitch * height;
        srcPitch2 = ((width >> 1) + 3) & ~3;
        s3offset = (srcPitch2 * (height >> 1)) + s2offset;
        break;
   case FOURCC_UYVY:
   case FOURCC_YUY2:
   default:
        dstPitch = ((width<<1) + 15) & ~15;
        srcPitch = (width<<1);
        break;
   }

   new_size = dstPitch * height;
   new_size = new_size + 0x1f; /* for aligning */
   alloc_size = new_size * mult;
   if (pPriv->capture_vbi_data)
      alloc_size += 2 * 2 * vbi_line_width * 21;

   pPriv->video_offset = RADEONAllocateMemory(pScrn, &pPriv->video_memory,
					      (pPriv->doubleBuffer ?
					       (new_size * 2) : new_size));
   if (pPriv->video_offset == 0)
      return BadAlloc;

/* I have suspicion that capture engine must be active _before_ Rage Theatre
   is being manipulated with.. */

   RADEONWaitForIdleMMIO(pScrn);
   display_base=INREG(RADEON_DISPLAY_BASE_ADDR);   

/*   RADEONWaitForFifo(pScrn, 15); */

   switch(pPriv->overlay_deinterlacing_method){
        case METHOD_BOB:
        case METHOD_SINGLE:
           offset1 = (pPriv->video_offset + 0xf) & (~0xf);
           offset2 = (pPriv->video_offset + new_size + 0xf) & (~0xf);
           offset3 = offset1;
           offset4 = offset2;
           break;
        case METHOD_WEAVE:
           offset1 = (pPriv->video_offset + 0xf) & (~0xf);
           offset2 = offset1+dstPitch;
           offset3 = (pPriv->video_offset + 2 * new_size + 0xf) & (~0xf);
           offset4 = offset3+dstPitch;
           break;
        default:
           offset1 = (pPriv->video_offset + 0xf) & (~0xf);
           offset2 = (pPriv->video_offset + new_size + 0xf) & (~0xf);
           offset3 = offset1;
           offset4 = offset2;
        }

   OUTREG(RADEON_CAP0_BUF0_OFFSET,        offset1+display_base);
   OUTREG(RADEON_CAP0_BUF0_EVEN_OFFSET,   offset2+display_base);
   OUTREG(RADEON_CAP0_BUF1_OFFSET,        offset3+display_base);
   OUTREG(RADEON_CAP0_BUF1_EVEN_OFFSET,   offset4+display_base);

   OUTREG(RADEON_CAP0_ONESHOT_BUF_OFFSET, offset1+display_base);

   if(pPriv->capture_vbi_data){
        if ((pPriv->encoding==2)||(pPriv->encoding==8)) {
            /* PAL, SECAM */
            vbi_start = 5;
            vbi_end = 21;
        } else {
            /* NTSC */
            vbi_start = 8;
            vbi_end = 20;
        }

        vbi_offset0 = (pPriv->video_offset + mult * new_size * bpp + 0xf) & (~0xf);
        vbi_offset1 = vbi_offset0 + dstPitch*20;
        OUTREG(RADEON_CAP0_VBI0_OFFSET, vbi_offset0+display_base);
        OUTREG(RADEON_CAP0_VBI1_OFFSET, vbi_offset1+display_base);
        OUTREG(RADEON_CAP0_VBI2_OFFSET, 0);
        OUTREG(RADEON_CAP0_VBI3_OFFSET, 0);
        OUTREG(RADEON_CAP0_VBI_V_WINDOW, vbi_start | (vbi_end<<16));
        OUTREG(RADEON_CAP0_VBI_H_WINDOW, 0 | (vbi_line_width)<<16);
        }
   
   OUTREG(RADEON_CAP0_BUF_PITCH, dstPitch*mult/2);
   OUTREG(RADEON_CAP0_H_WINDOW, (2*width)<<16);
   OUTREG(RADEON_CAP0_V_WINDOW, (((height)+pPriv->v-1)<<16)|(pPriv->v-1));
   if(mult==2){
           OUTREG(RADEON_CAP0_CONFIG, ENABLE_RADEON_CAPTURE_BOB);
           } else {
           OUTREG(RADEON_CAP0_CONFIG, ENABLE_RADEON_CAPTURE_WEAVE);
           }
   OUTREG(RADEON_CAP0_DEBUG, 0);
   
   OUTREG(RADEON_VID_BUFFER_CONTROL, (1<<16) | 0x01);
   OUTREG(RADEON_TEST_DEBUG_CNTL, 0);
   
   if(! pPriv->video_stream_active)
   {

      RADEONWaitForIdleMMIO(pScrn);
      OUTREG(RADEON_VIDEOMUX_CNTL, INREG(RADEON_VIDEOMUX_CNTL)|1 ); 
      OUTREG(RADEON_CAP0_PORT_MODE_CNTL, (pPriv->theatre!=NULL)? 1: 0);
      OUTREG(RADEON_FCP_CNTL, RADEON_FCP0_SRC_PCLK);
      OUTREG(RADEON_CAP0_TRIG_CNTL, 0x11);
      if(pPriv->theatre != NULL) 
      {
         RADEON_RT_SetEncoding(pScrn, pPriv); 
      }
      if(pPriv->msp3430 != NULL) RADEON_MSP_SetEncoding(pPriv);
      if(pPriv->tda9885 != NULL) RADEON_TDA9885_SetEncoding(pPriv);
      if(pPriv->fi1236 != NULL) RADEON_FI1236_SetEncoding(pPriv);
      if(pPriv->i2c != NULL)RADEON_board_setmisc(pPriv);
   }

   
   /* update cliplist */
   if(!REGION_EQUAL(pScrn->pScreen, &pPriv->clip, clipBoxes)) {
        REGION_COPY(pScreen, &pPriv->clip, clipBoxes);
        /* draw these */
        if(pPriv->autopaint_colorkey)
	     	xf86XVFillKeyHelper(pScrn->pScreen, pPriv->colorKey, clipBoxes);
   }

   RADEONDisplayVideo(pScrn, pPriv, id, offset1+top*srcPitch, offset2+top*srcPitch, offset3+top*srcPitch, offset4+top*srcPitch, width, height, dstPitch*mult/2,
                     xa, xb, ya, &dstBox, src_w, src_h*mult/2, drw_w, drw_h, pPriv->overlay_deinterlacing_method);

   RADEONWaitForFifo(pScrn, 1);
   OUTREG(RADEON_OV0_REG_LOAD_CNTL,  RADEON_REG_LD_CTL_LOCK);
   RADEONWaitForIdleMMIO(pScrn);
   while(!(INREG(RADEON_OV0_REG_LOAD_CNTL) & RADEON_REG_LD_CTL_LOCK_READBACK));


   switch(pPriv->overlay_deinterlacing_method){
        case METHOD_BOB:
           OUTREG(RADEON_OV0_DEINTERLACE_PATTERN, 0xAAAAA);
           OUTREG(RADEON_OV0_AUTO_FLIP_CNTL,0 /*| RADEON_OV0_AUTO_FLIP_CNTL_SOFT_BUF_ODD*/
                |RADEON_OV0_AUTO_FLIP_CNTL_SHIFT_ODD_DOWN);
           break;
        case METHOD_SINGLE:
           OUTREG(RADEON_OV0_DEINTERLACE_PATTERN, 0xEEEEE | (9<<28));
           OUTREG(RADEON_OV0_AUTO_FLIP_CNTL, RADEON_OV0_AUTO_FLIP_CNTL_SOFT_BUF_ODD
                |RADEON_OV0_AUTO_FLIP_CNTL_SHIFT_ODD_DOWN);
           break;
        case METHOD_WEAVE:
           OUTREG(RADEON_OV0_DEINTERLACE_PATTERN, 0x11111 | (9<<28));
           OUTREG(RADEON_OV0_AUTO_FLIP_CNTL, 0  |RADEON_OV0_AUTO_FLIP_CNTL_SOFT_BUF_ODD 
                | RADEON_OV0_AUTO_FLIP_CNTL_P1_FIRST_LINE_EVEN 
                /* |RADEON_OV0_AUTO_FLIP_CNTL_SHIFT_ODD_DOWN */
                /*|RADEON_OV0_AUTO_FLIP_CNTL_SHIFT_EVEN_DOWN */
                |RADEON_OV0_AUTO_FLIP_CNTL_FIELD_POL_SOURCE);
           break;
        default:
           OUTREG(RADEON_OV0_DEINTERLACE_PATTERN, 0xAAAAA);
           OUTREG(RADEON_OV0_AUTO_FLIP_CNTL, RADEON_OV0_AUTO_FLIP_CNTL_SOFT_BUF_ODD
                |RADEON_OV0_AUTO_FLIP_CNTL_SHIFT_ODD_DOWN);
        }
                
   
   RADEONWaitForIdleMMIO(pScrn);
   OUTREG (RADEON_OV0_AUTO_FLIP_CNTL, (INREG (RADEON_OV0_AUTO_FLIP_CNTL) ^ RADEON_OV0_AUTO_FLIP_CNTL_SOFT_EOF_TOGGLE ));
   OUTREG (RADEON_OV0_AUTO_FLIP_CNTL, (INREG (RADEON_OV0_AUTO_FLIP_CNTL) ^ RADEON_OV0_AUTO_FLIP_CNTL_SOFT_EOF_TOGGLE ));

   OUTREG(RADEON_OV0_REG_LOAD_CNTL, 0);

#if 0
   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "OV0_FLAG_CNTL=0x%08x\n", INREG(RADEON_OV0_FLAG_CNTL));
/*   OUTREG(RADEON_OV0_FLAG_CNTL, 8); */
   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "OV0_VID_BUFFER_CNTL=0x%08x\n", INREG(RADEON_VID_BUFFER_CONTROL));
   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CAP0_BUF_STATUS=0x%08x\n", INREG(RADEON_CAP0_BUF_STATUS));

/*   OUTREG(RADEON_OV0_SCALE_CNTL, 0x417f1B00); */
#endif

   pPriv->videoStatus = CLIENT_VIDEO_ON;
   pPriv->video_stream_active = TRUE;

   info->VideoTimerCallback = RADEONVideoTimerCallback;

   return Success;
}
        /* miscellaneous TV-in helper functions */

static void RADEON_board_setmisc(RADEONPortPrivPtr pPriv)
{
    /* Adjust PAL/SECAM constants for FI1216MF tuner */
    if((((pPriv->tuner_type & 0xf)==5) ||
        ((pPriv->tuner_type & 0xf)==11)||
        ((pPriv->tuner_type & 0xf)==14))
        && (pPriv->fi1236!=NULL))
    {
        if((pPriv->encoding>=1)&&(pPriv->encoding<=3)) /*PAL*/
        {
           pPriv->fi1236->parm.band_low = 0xA1;
           pPriv->fi1236->parm.band_mid = 0x91;
           pPriv->fi1236->parm.band_high = 0x31;
        }
        if((pPriv->encoding>=7)&&(pPriv->encoding<=9)) /*SECAM*/
        {
           pPriv->fi1236->parm.band_low = 0xA3;
           pPriv->fi1236->parm.band_mid = 0x93;
           pPriv->fi1236->parm.band_high = 0x33;
        }
    }
    
}

static void RADEON_RT_SetEncoding(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
int width, height;
RADEONWaitForIdleMMIO(pScrn);

/* Disable VBI capture for anything but TV tuner */
switch(pPriv->encoding){
	case 2:
	case 5:
	case 8:
		pPriv->capture_vbi_data=1;
		break;
	default:
		pPriv->capture_vbi_data=0;
	}

switch(pPriv->encoding){
        case 1:
                xf86_RT_SetConnector(pPriv->theatre,DEC_COMPOSITE, 0);
                xf86_RT_SetStandard(pPriv->theatre,DEC_PAL | extPAL);
                pPriv->v=25;
                break;
        case 2:
                xf86_RT_SetConnector(pPriv->theatre,DEC_TUNER,0);
                xf86_RT_SetStandard(pPriv->theatre,DEC_PAL | extPAL);
                pPriv->v=25;
                break;
        case 3:
                xf86_RT_SetConnector(pPriv->theatre,DEC_SVIDEO,0);
                xf86_RT_SetStandard(pPriv->theatre,DEC_PAL | extPAL);
                pPriv->v=25;
                break;
        case 4:
                xf86_RT_SetConnector(pPriv->theatre, DEC_COMPOSITE,0);
                xf86_RT_SetStandard(pPriv->theatre,DEC_NTSC | extNONE);
                pPriv->v=23;
                break;
        case 5:
                xf86_RT_SetConnector(pPriv->theatre, DEC_TUNER, 0);
                xf86_RT_SetStandard(pPriv->theatre,DEC_NTSC | extNONE);
                pPriv->v=23;
                break;
        case 6:
                xf86_RT_SetConnector(pPriv->theatre, DEC_SVIDEO, 0);
                xf86_RT_SetStandard(pPriv->theatre,DEC_NTSC | extNONE);
                pPriv->v=23;
                break;
        case 7:
                xf86_RT_SetConnector(pPriv->theatre, DEC_COMPOSITE, 0);
                xf86_RT_SetStandard(pPriv->theatre,DEC_SECAM | extNONE);
                pPriv->v=25;
                break;
        case 8:
                xf86_RT_SetConnector(pPriv->theatre, DEC_TUNER, 0);
                xf86_RT_SetStandard(pPriv->theatre,DEC_SECAM | extNONE);
                pPriv->v=25;
                break;
        case 9:
                xf86_RT_SetConnector(pPriv->theatre, DEC_SVIDEO, 0);
                xf86_RT_SetStandard(pPriv->theatre,DEC_SECAM | extNONE);
                pPriv->v=25;
                break;
        case 10:
                xf86_RT_SetConnector(pPriv->theatre,DEC_COMPOSITE, 0);
                xf86_RT_SetStandard(pPriv->theatre,DEC_PAL | extPAL_60);
                pPriv->v=25;
                break;
        case 11:
                xf86_RT_SetConnector(pPriv->theatre,DEC_TUNER,0);
                xf86_RT_SetStandard(pPriv->theatre,DEC_PAL | extPAL_60);
                pPriv->v=25;
                break;
        case 12:
                xf86_RT_SetConnector(pPriv->theatre,DEC_SVIDEO,0);
                xf86_RT_SetStandard(pPriv->theatre,DEC_PAL | extPAL_60);
                pPriv->v=25;
                break;
        default:
                pPriv->v=0;
                return;
        }       
xf86_RT_SetInterlace(pPriv->theatre, 1);
width = InputVideoEncodings[pPriv->encoding].width;
height = InputVideoEncodings[pPriv->encoding].height;
xf86_RT_SetOutputVideoSize(pPriv->theatre, width, height*2, 0, pPriv->capture_vbi_data);   
}

static void RADEON_MSP_SetEncoding(RADEONPortPrivPtr pPriv)
{
xf86_MSP3430SetVolume(pPriv->msp3430, MSP3430_FAST_MUTE);
switch(pPriv->encoding){
        case 1:
                pPriv->msp3430->standard = MSP3430_PAL;
                pPriv->msp3430->connector = MSP3430_CONNECTOR_3;
                break;
        case 2:
                pPriv->msp3430->standard = MSP3430_PAL;
                pPriv->msp3430->connector = MSP3430_CONNECTOR_1;
                break;
        case 3:
                pPriv->msp3430->standard = MSP3430_PAL;
                pPriv->msp3430->connector = MSP3430_CONNECTOR_2;
                break;
        case 4:
                pPriv->msp3430->standard = MSP3430_NTSC;
                pPriv->msp3430->connector = MSP3430_CONNECTOR_3;
                break;
        case 5:
                pPriv->msp3430->standard = MSP3430_NTSC;
                pPriv->msp3430->connector = MSP3430_CONNECTOR_1;
                break;
        case 6:
                pPriv->msp3430->standard = MSP3430_NTSC;
                pPriv->msp3430->connector = MSP3430_CONNECTOR_2;
                break;
        case 7:
                pPriv->msp3430->standard = MSP3430_SECAM;
                pPriv->msp3430->connector = MSP3430_CONNECTOR_3;
                break;
        case 8:
                pPriv->msp3430->standard = MSP3430_SECAM;
                pPriv->msp3430->connector = MSP3430_CONNECTOR_1;
                break;
        case 9:
                pPriv->msp3430->standard = MSP3430_SECAM;
                pPriv->msp3430->connector = MSP3430_CONNECTOR_2;
                break;
        case 10:
                pPriv->msp3430->standard = MSP3430_SECAM;
                pPriv->msp3430->connector = MSP3430_CONNECTOR_3;
                break;
        case 11:
                pPriv->msp3430->standard = MSP3430_SECAM;
                pPriv->msp3430->connector = MSP3430_CONNECTOR_1;
                break;
        case 12:
                pPriv->msp3430->standard = MSP3430_SECAM;
                pPriv->msp3430->connector = MSP3430_CONNECTOR_2;
                break;
        default:
                return;
        }
xf86_InitMSP3430(pPriv->msp3430);
xf86_MSP3430SetVolume(pPriv->msp3430, pPriv->mute ? MSP3430_FAST_MUTE : MSP3430_VOLUME(pPriv->volume));
}

static void RADEON_TDA9885_SetEncoding(RADEONPortPrivPtr pPriv)
{
TDA9885Ptr t=pPriv->tda9885;

switch(pPriv->encoding){
                /* PAL */
        case 1:
        case 2:
        case 3:
                t->standard_video_if=2;
                t->standard_sound_carrier=1;
					 t->modulation=2; /* negative FM */
                break;
                /* NTSC */
        case 4:
        case 5:
        case 6:
                t->standard_video_if=1;
                t->standard_sound_carrier=0;
					 t->modulation=2; /* negative FM */
                break;
                /* SECAM */
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
                t->standard_video_if=0;
                t->standard_sound_carrier=3;
                t->modulation=0; /* positive AM */
                break;
        default:
                return;
        }       
xf86_tda9885_setparameters(pPriv->tda9885); 
xf86_tda9885_getstatus(pPriv->tda9885);
xf86_tda9885_dumpstatus(pPriv->tda9885);
}

static void RADEON_FI1236_SetEncoding(RADEONPortPrivPtr pPriv)
{
/* at the moment this only affect MT2032 */
switch(pPriv->encoding){
                /* PAL */
        case 1:
        case 2:
        case 3:
		pPriv->fi1236->video_if=38.900;
                break;
                /* NTSC */
        case 4:
        case 5:
        case 6:
		pPriv->fi1236->video_if=45.7812;
		pPriv->fi1236->video_if=45.750;
		pPriv->fi1236->video_if=45.125;
                break;
                /* SECAM */
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
		pPriv->fi1236->video_if=58.7812;
                break;
        default:
                return;
        }       
}

