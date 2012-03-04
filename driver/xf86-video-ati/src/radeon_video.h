#ifndef __RADEON_VIDEO_H__
#define __RADEON_VIDEO_H__

#include "xf86i2c.h"
#include "fi1236.h"
#include "msp3430.h"
#include "tda9885.h"
#include "uda1380.h"
#include "i2c_def.h"

#include "generic_bus.h"
#include "theatre.h"

#include "xf86Crtc.h"

#include "bicubic_table.h"

#include <xf86xvmc.h>

#define ClipValue(v,min,max) ((v) < (min) ? (min) : (v) > (max) ? (max) : (v))

/* Xvideo port struct */
typedef struct {
   uint32_t	 transform_index;
   uint32_t	 gamma; /* gamma value x 1000 */
   int           brightness;
   int           saturation;
   int           hue;
   int           contrast;
   int           red_intensity;
   int           green_intensity;
   int           blue_intensity;

	/* overlay composition mode */
   int		 alpha_mode; /* 0 = key mode, 1 = global mode */
   int		 ov_alpha;
   int		 gr_alpha;

     /* i2c bus and devices */
   I2CBusPtr     i2c;
   uint32_t      radeon_i2c_timing;
   uint32_t      radeon_M;
   uint32_t      radeon_N;
   uint32_t      i2c_status;
   uint32_t      i2c_cntl;

   FI1236Ptr     fi1236;
   uint8_t       tuner_type;
   MSP3430Ptr    msp3430;
   TDA9885Ptr    tda9885;
   UDA1380Ptr    uda1380;

   /* VIP bus and devices */
   GENERIC_BUS_Ptr  VIP;
   TheatrePtr       theatre;

   Bool          video_stream_active;
   int           encoding;
   uint32_t      frequency;
   int           volume;
   Bool          mute;
   int           sap_channel;
   int           v;
   uint32_t      adjustment; /* general purpose variable */

#define METHOD_BOB      0
#define METHOD_SINGLE   1
#define METHOD_WEAVE    2
#define METHOD_ADAPTIVE 3

   int           overlay_deinterlacing_method;

   int           capture_vbi_data;

   int           dec_brightness;
   int           dec_saturation;
   int           dec_hue;
   int           dec_contrast;

   Bool          doubleBuffer;
   unsigned char currentBuffer;
   RegionRec     clip;
   uint32_t      colorKey;
   uint32_t      videoStatus;
   Time          offTime;
   Time          freeTime;
   Bool          autopaint_colorkey;
   xf86CrtcPtr   desired_crtc;

   int           size;

   void         *video_memory;
   int           video_offset;

   int           planeu_offset;
   int           planev_offset;

   /* bicubic filtering */
   Bool          bicubic_enabled;
   uint32_t      bicubic_src_offset;
   int           bicubic_state;
#define BICUBIC_OFF  0
#define BICUBIC_ON   1
#define BICUBIC_AUTO 2

   Atom          device_id, location_id, instance_id;

    /* textured video */
    Bool textured;
    DrawablePtr pDraw;
    PixmapPtr pPixmap;

    uint32_t src_offset;
    uint32_t src_pitch;
    uint8_t *src_addr;

    int id;
    int src_w, src_h, dst_w, dst_h;
    int w, h;
    int drw_x, drw_y;
    int src_x, src_y;
    int vsync;
    Bool is_planar;
    int vtx_count;
    int hw_align;

    struct radeon_bo *src_bo[2];
} RADEONPortPrivRec, *RADEONPortPrivPtr;

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

#define RTFSaturation(a)   (1.0 + ((a)*1.0)/1000.0)
#define RTFBrightness(a)   (((a)*1.0)/2000.0)
#define RTFIntensity(a)   (((a)*1.0)/2000.0)
#define RTFContrast(a)   (1.0 + ((a)*1.0)/1000.0)
#define RTFHue(a)   (((a)*3.1416)/1000.0)

void RADEONInitI2C(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv);
void RADEONResetI2C(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv);

void RADEONVIP_init(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv);
void RADEONVIP_reset(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv);

int  RADEONSetPortAttribute(ScrnInfoPtr, Atom, INT32, pointer);
int  RADEONGetPortAttribute(ScrnInfoPtr, Atom ,INT32 *, pointer);
void RADEONFreeVideoMemory(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv);
void RADEONStopVideo(ScrnInfoPtr, pointer, Bool);
void RADEONQueryBestSize(ScrnInfoPtr, Bool, short, short, short, short,
			 unsigned int *, unsigned int *, pointer);
int  RADEONQueryImageAttributes(ScrnInfoPtr, int, unsigned short *,
			unsigned short *,  int *, int *);

XF86VideoAdaptorPtr
RADEONSetupImageTexturedVideo(ScreenPtr pScreen);

XF86MCAdaptorPtr
RADEONCreateAdaptorXvMC(ScreenPtr pScreen, char *xv_adaptor_name);

void
RADEONCopyData(ScrnInfoPtr pScrn,
	       unsigned char *src, unsigned char *dst,
	       unsigned int srcPitch, unsigned int dstPitch,
	       unsigned int h, unsigned int w, unsigned int bpp);

void
RADEONCopyMungedData(ScrnInfoPtr pScrn,
		     unsigned char *src1, unsigned char *src2,
		     unsigned char *src3, unsigned char *dst1,
		     unsigned int srcPitch, unsigned int srcPitch2,
		     unsigned int dstPitch, unsigned int h, unsigned int w);

#endif
