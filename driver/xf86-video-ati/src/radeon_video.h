#ifndef __RADEON_VIDEO_H__
#define __RADEON_VIDEO_H__

#include "xf86i2c.h"
#include "i2c_def.h"

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

   unsigned char currentBuffer;
   RegionRec     clip;

   Time          offTime;
   Time          freeTime;
   xf86CrtcPtr   desired_crtc;

   int           size;

   struct radeon_bo *video_memory;

   int           planeu_offset;
   int           planev_offset;

   /* bicubic filtering */
   Bool          bicubic_enabled;
   int           bicubic_state;
#define BICUBIC_OFF  0
#define BICUBIC_ON   1
#define BICUBIC_AUTO 2

    /* textured video */
    Bool textured;
    DrawablePtr pDraw;
    PixmapPtr pPixmap;

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

Bool radeon_crtc_is_enabled(xf86CrtcPtr crtc);

#endif
