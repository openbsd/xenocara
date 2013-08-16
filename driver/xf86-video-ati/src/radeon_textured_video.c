/*
 * Copyright 2008 Alex Deucher
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 * Based on radeon_exa_render.c and kdrive ati_video.c by Eric Anholt, et al.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_probe.h"
#include "radeon_video.h"

#include <X11/extensions/Xv.h>
#include "fourcc.h"

extern void
R600DisplayTexturedVideo(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv);

extern void
EVERGREENDisplayTexturedVideo(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv);


#define IMAGE_MAX_WIDTH		2048
#define IMAGE_MAX_HEIGHT	2048

#define IMAGE_MAX_WIDTH_R500	4096
#define IMAGE_MAX_HEIGHT_R500	4096

#define IMAGE_MAX_WIDTH_R600	8192
#define IMAGE_MAX_HEIGHT_R600	8192

#define IMAGE_MAX_WIDTH_EG	16384
#define IMAGE_MAX_HEIGHT_EG	16384

static Bool
RADEONTilingEnabled(ScrnInfoPtr pScrn, PixmapPtr pPix)
{
    return FALSE;
}

static __inline__ uint32_t F_TO_DW(float val)
{
    union {
	float f;
	uint32_t l;
    } tmp;
    tmp.f = val;
    return tmp.l;
}

/* Borrowed from Mesa */
static __inline__ uint32_t F_TO_24(float val)
{
	float mantissa;
	int exponent;
	uint32_t float24 = 0;

	if (val == 0.0)
		return 0;

	mantissa = frexpf(val, &exponent);

	/* Handle -ve */
	if (mantissa < 0) {
		float24 |= (1 << 23);
		mantissa = mantissa * -1.0;
	}
	/* Handle exponent, bias of 63 */
	exponent += 62;
	float24 |= (exponent << 16);
	/* Kill 7 LSB of mantissa */
	float24 |= (F_TO_DW(mantissa) & 0x7FFFFF) >> 7;

	return float24;
}

static __inline__ uint32_t float4touint(float fr, float fg, float fb, float fa)
{
    unsigned ur = fr * 255.0 + 0.5;
    unsigned ug = fg * 255.0 + 0.5;
    unsigned ub = fb * 255.0 + 0.5;
    unsigned ua = fa * 255.0 + 0.5;
    return (ua << 24) | (ur << 16) | (ug << 8) | ub;
}

/* Parameters for ITU-R BT.601 and ITU-R BT.709 colour spaces
   note the difference to the parameters used in overlay are due
   to 10bit vs. float calcs */
static REF_TRANSFORM trans[2] =
{
    {1.1643, 0.0, 1.5960, -0.3918, -0.8129, 2.0172, 0.0}, /* BT.601 */
    {1.1643, 0.0, 1.7927, -0.2132, -0.5329, 2.1124, 0.0}  /* BT.709 */
};


/* Allocates memory, either by resizing the allocation pointed to by mem_struct,
 * or by freeing mem_struct (if non-NULL) and allocating a new space.  The size
 * is measured in bytes, and the offset from the beginning of card space is
 * returned.
 */
static Bool
radeon_allocate_video_bo(ScrnInfoPtr pScrn,
			 struct radeon_bo **video_bo_p,
			 int size,
			 int align,
			 int domain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_bo *video_bo;

    if (*video_bo_p)
        radeon_bo_unref(*video_bo_p);

    video_bo = radeon_bo_open(info->bufmgr, 0, size, align, domain, 0);

    *video_bo_p = video_bo;

    if (!video_bo)
        return FALSE;

    return TRUE;
}

static void
RADEONFreeVideoMemory(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    if (pPriv->video_memory != NULL) {
	radeon_bo_unref(pPriv->video_memory);
	pPriv->video_memory = NULL;

	if (pPriv->textured) {
	    pPriv->src_bo[0] = NULL;
	    radeon_bo_unref(pPriv->src_bo[1]);
	    pPriv->src_bo[1] = NULL;
	}
    }
}

static void
RADEONStopVideo(ScrnInfoPtr pScrn, pointer data, Bool cleanup)
{
  RADEONPortPrivPtr pPriv = (RADEONPortPrivPtr)data;

  if (pPriv->textured) {
      if (cleanup) {
	  RADEONFreeVideoMemory(pScrn, pPriv);
      }
      return;
  }
}

#define OUT_ACCEL_REG_F(reg, val)	OUT_RING_REG(reg, F_TO_DW(val))

#include "radeon_textured_videofuncs.c"

#undef OUT_ACCEL_REG_F

static void
R600CopyData(
    ScrnInfoPtr pScrn,
    unsigned char *src,
    unsigned char *dst,
    unsigned int srcPitch,
    unsigned int dstPitch,
    unsigned int h,
    unsigned int w,
    unsigned int cpp
){
    if (cpp == 2) {
	w *= 2;
	cpp = 1;
    }

    if (srcPitch == dstPitch)
        memcpy(dst, src, srcPitch * h);
    else {
	while (h--) {
	    memcpy(dst, src, srcPitch);
	    src += srcPitch;
	    dst += dstPitch;
	}
    }
}

static int
RADEONPutImageTextured(ScrnInfoPtr pScrn,
		       short src_x, short src_y,
		       short drw_x, short drw_y,
		       short src_w, short src_h,
		       short drw_w, short drw_h,
		       int id,
		       unsigned char *buf,
		       short width,
		       short height,
		       Bool sync,
		       RegionPtr clipBoxes,
		       pointer data,
		       DrawablePtr pDraw)
{
    ScreenPtr pScreen = pScrn->pScreen;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONPortPrivPtr pPriv = (RADEONPortPrivPtr)data;
    INT32 x1, x2, y1, y2;
    int srcPitch, srcPitch2, dstPitch, dstPitch2 = 0;
    int s2offset, s3offset, tmp;
    int d2line, d3line;
    int top, nlines, size;
    BoxRec dstBox;
    int dst_width = width, dst_height = height;
    int aligned_height;
    int h_align = drmmode_get_height_align(pScrn, 0);
    struct radeon_bo *src_bo;
    int ret;

    /* make the compiler happy */
    s2offset = s3offset = srcPitch2 = 0;

    /* Clip */
    x1 = src_x;
    x2 = src_x + src_w;
    y1 = src_y;
    y2 = src_y + src_h;

    dstBox.x1 = drw_x;
    dstBox.x2 = drw_x + drw_w;
    dstBox.y1 = drw_y;
    dstBox.y2 = drw_y + drw_h;

    if (!xf86XVClipVideoHelper(&dstBox, &x1, &x2, &y1, &y2, clipBoxes, width, height))
	return Success;

    if ((x1 >= x2) || (y1 >= y2))
	return Success;

    /* Bicubic filter setup */
    pPriv->bicubic_enabled = (pPriv->bicubic_state != BICUBIC_OFF);
    if (!(IS_R300_3D || IS_R500_3D)) {
	pPriv->bicubic_enabled = FALSE;
	pPriv->bicubic_state = BICUBIC_OFF;
    }
    if (pPriv->bicubic_enabled && (pPriv->bicubic_state == BICUBIC_AUTO)) {
	/*
	 * Applying the bicubic filter with a scale of less than 200%
	 * results in a blurred picture, so disable the filter.
	 */
	if ((src_w > drw_w / 2) || (src_h > drw_h / 2))
	    pPriv->bicubic_enabled = FALSE;
    }

    if (info->ChipFamily >= CHIP_FAMILY_R600)
	pPriv->hw_align = drmmode_get_base_align(pScrn, 2, 0);
    else
	pPriv->hw_align = 64;

    aligned_height = RADEON_ALIGN(dst_height, h_align);

    switch(id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	srcPitch = RADEON_ALIGN(width, 4);
	srcPitch2 = RADEON_ALIGN(width >> 1, 4);
        if (pPriv->bicubic_state != BICUBIC_OFF) {
	    dstPitch = RADEON_ALIGN(dst_width << 1, pPriv->hw_align);
	    dstPitch2 = 0;
	} else {
	    dstPitch = RADEON_ALIGN(dst_width, pPriv->hw_align);
	    dstPitch2 = RADEON_ALIGN(dstPitch >> 1, pPriv->hw_align);
	}
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	dstPitch = RADEON_ALIGN(dst_width << 1, pPriv->hw_align);
	srcPitch = (width << 1);
	srcPitch2 = 0;
	break;
    }

    size = dstPitch * aligned_height + 2 * dstPitch2 * RADEON_ALIGN(((aligned_height + 1) >> 1), h_align);
    size = RADEON_ALIGN(size, pPriv->hw_align);

    if (size != pPriv->size) {
	RADEONFreeVideoMemory(pScrn, pPriv);
    }

    if (pPriv->video_memory == NULL) {
      Bool ret;
      ret = radeon_allocate_video_bo(pScrn,
				     &pPriv->video_memory,
				     size, pPriv->hw_align,
				     RADEON_GEM_DOMAIN_GTT);
      if (ret == FALSE)
	  return BadAlloc;

      pPriv->src_bo[0] = pPriv->video_memory;
      radeon_allocate_video_bo(pScrn, (void*)&pPriv->src_bo[1], size,
			       pPriv->hw_align,
			       RADEON_GEM_DOMAIN_GTT);
    }

    /* Bicubic filter loading */
    if (pPriv->bicubic_enabled) {
	if (info->bicubic_bo == NULL)
	    pPriv->bicubic_enabled = FALSE;
    }

    if (pDraw->type == DRAWABLE_WINDOW)
	pPriv->pPixmap = (*pScreen->GetWindowPixmap)((WindowPtr)pDraw);
    else
	pPriv->pPixmap = (PixmapPtr)pDraw;

    /* Force the pixmap into framebuffer so we can draw to it. */
    info->exa_force_create = TRUE;
    exaMoveInPixmap(pPriv->pPixmap);
    info->exa_force_create = FALSE;

    /* copy data */
    top = (y1 >> 16) & ~1;
    nlines = ((y2 + 0xffff) >> 16) - top;

    pPriv->currentBuffer ^= 1;
	
    src_bo = pPriv->src_bo[pPriv->currentBuffer];

    ret = radeon_bo_map(src_bo, 1);
    if (ret)
	return BadAlloc;
  
    pPriv->src_addr = src_bo->ptr;
    pPriv->src_pitch = dstPitch;

    pPriv->planeu_offset = dstPitch * aligned_height;
    pPriv->planeu_offset = RADEON_ALIGN(pPriv->planeu_offset, pPriv->hw_align);
    pPriv->planev_offset = pPriv->planeu_offset + dstPitch2 * RADEON_ALIGN(((aligned_height + 1) >> 1), h_align);
    pPriv->planev_offset = RADEON_ALIGN(pPriv->planev_offset, pPriv->hw_align);

    pPriv->size = size;
    pPriv->pDraw = pDraw;

    switch(id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	s2offset = srcPitch * (RADEON_ALIGN(height, 2));
	s3offset = s2offset + (srcPitch2 * ((height + 1) >> 1));
	s2offset += ((top >> 1) * srcPitch2);
	s3offset += ((top >> 1) * srcPitch2);
	if (pPriv->bicubic_state != BICUBIC_OFF) {
	    if (id == FOURCC_I420) {
		tmp = s2offset;
		s2offset = s3offset;
		s3offset = tmp;
	    }
	    RADEONCopyMungedData(pScrn, buf + (top * srcPitch),
				 buf + s2offset, buf + s3offset, pPriv->src_addr + (top * dstPitch),
				 srcPitch, srcPitch2, dstPitch, nlines, width);
	} else {
	    if (id == FOURCC_YV12) {
		tmp = s2offset;
		s2offset = s3offset;
		s3offset = tmp;
	    }
	    d2line = pPriv->planeu_offset + ((top >> 1) * dstPitch2);
	    d3line = pPriv->planev_offset + ((top >> 1) * dstPitch2);

	    if (info->ChipFamily >= CHIP_FAMILY_R600) {
		R600CopyData(pScrn, buf + (top * srcPitch), pPriv->src_addr + (top * dstPitch),
			     srcPitch, dstPitch, nlines, width, 1);
		R600CopyData(pScrn, buf + s2offset,  pPriv->src_addr + d2line,
			     srcPitch2, dstPitch2, (nlines + 1) >> 1, width >> 1, 1);
		R600CopyData(pScrn, buf + s3offset, pPriv->src_addr + d3line,
			     srcPitch2, dstPitch2, (nlines + 1) >> 1, width >> 1, 1);
	    } else {
		RADEONCopyData(pScrn, buf + (top * srcPitch), pPriv->src_addr + (top * dstPitch),
			       srcPitch, dstPitch, nlines, width, 1);
		RADEONCopyData(pScrn, buf + s2offset,  pPriv->src_addr + d2line,
			       srcPitch2, dstPitch2, (nlines + 1) >> 1, width >> 1, 1);
		RADEONCopyData(pScrn, buf + s3offset, pPriv->src_addr + d3line,
			       srcPitch2, dstPitch2, (nlines + 1) >> 1, width >> 1, 1);
	    }
	}
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	if (info->ChipFamily >= CHIP_FAMILY_R600)
	    R600CopyData(pScrn, buf + (top * srcPitch),
			 pPriv->src_addr + (top * dstPitch),
			 srcPitch, dstPitch, nlines, width, 2);
	else
	    RADEONCopyData(pScrn, buf + (top * srcPitch),
			   pPriv->src_addr + (top * dstPitch),
			   srcPitch, dstPitch, nlines, width, 2);
	break;
    }

    /* update cliplist */
    if (!REGION_EQUAL(pScrn->pScreen, &pPriv->clip, clipBoxes)) {
	REGION_COPY(pScrn->pScreen, &pPriv->clip, clipBoxes);
    }

    pPriv->id = id;
    pPriv->src_w = src_w;
    pPriv->src_h = src_h;
    pPriv->src_x = src_x;
    pPriv->src_y = src_y;
    pPriv->drw_x = drw_x;
    pPriv->drw_y = drw_y;
    pPriv->dst_w = drw_w;
    pPriv->dst_h = drw_h;
    pPriv->w = width;
    pPriv->h = height;

    radeon_bo_unmap(pPriv->src_bo[pPriv->currentBuffer]);
    if (info->directRenderingEnabled) {
	if (IS_EVERGREEN_3D)
	    EVERGREENDisplayTexturedVideo(pScrn, pPriv);
	else if (IS_R600_3D)
	    R600DisplayTexturedVideo(pScrn, pPriv);
	else if (IS_R500_3D)
	    R500DisplayTexturedVideo(pScrn, pPriv);
	else if (IS_R300_3D)
	    R300DisplayTexturedVideo(pScrn, pPriv);
	else if (IS_R200_3D)
	    R200DisplayTexturedVideo(pScrn, pPriv);
	else
	    RADEONDisplayTexturedVideo(pScrn, pPriv);
    }

    return Success;
}

/* client libraries expect an encoding */
static XF86VideoEncodingRec DummyEncoding[1] =
{
    {
	0,
	"XV_IMAGE",
	IMAGE_MAX_WIDTH, IMAGE_MAX_HEIGHT,
	{1, 1}
    }
};

static XF86VideoEncodingRec DummyEncodingR500[1] =
{
    {
	0,
	"XV_IMAGE",
	IMAGE_MAX_WIDTH_R500, IMAGE_MAX_HEIGHT_R500,
	{1, 1}
    }
};

static XF86VideoEncodingRec DummyEncodingR600[1] =
{
    {
	0,
	"XV_IMAGE",
	IMAGE_MAX_WIDTH_R600, IMAGE_MAX_HEIGHT_R600,
	{1, 1}
    }
};

static XF86VideoEncodingRec DummyEncodingEG[1] =
{
    {
	0,
	"XV_IMAGE",
	IMAGE_MAX_WIDTH_EG, IMAGE_MAX_HEIGHT_EG,
	{1, 1}
    }
};

#define NUM_FORMATS 3

static XF86VideoFormatRec Formats[NUM_FORMATS] =
{
    {15, TrueColor}, {16, TrueColor}, {24, TrueColor}
};

#define NUM_ATTRIBUTES 2

static XF86AttributeRec Attributes[NUM_ATTRIBUTES+1] =
{
    {XvSettable | XvGettable, 0, 1, "XV_VSYNC"},
    {XvSettable | XvGettable, -1, 1, "XV_CRTC"},
    {0, 0, 0, NULL}
};

#define NUM_ATTRIBUTES_R200 7

static XF86AttributeRec Attributes_r200[NUM_ATTRIBUTES_R200+1] =
{
    {XvSettable | XvGettable, 0, 1, "XV_VSYNC"},
    {XvSettable | XvGettable, -1000, 1000, "XV_BRIGHTNESS"},
    {XvSettable | XvGettable, -1000, 1000, "XV_CONTRAST"},
    {XvSettable | XvGettable, -1000, 1000, "XV_SATURATION"},
    {XvSettable | XvGettable, -1000, 1000, "XV_HUE"},
    {XvSettable | XvGettable, 0, 1, "XV_COLORSPACE"},
    {XvSettable | XvGettable, -1, 1, "XV_CRTC"},
    {0, 0, 0, NULL}
};

#define NUM_ATTRIBUTES_R300 9

static XF86AttributeRec Attributes_r300[NUM_ATTRIBUTES_R300+1] =
{
    {XvSettable | XvGettable, 0, 2, "XV_BICUBIC"},
    {XvSettable | XvGettable, 0, 1, "XV_VSYNC"},
    {XvSettable | XvGettable, -1000, 1000, "XV_BRIGHTNESS"},
    {XvSettable | XvGettable, -1000, 1000, "XV_CONTRAST"},
    {XvSettable | XvGettable, -1000, 1000, "XV_SATURATION"},
    {XvSettable | XvGettable, -1000, 1000, "XV_HUE"},
    {XvSettable | XvGettable, 100, 10000, "XV_GAMMA"},
    {XvSettable | XvGettable, 0, 1, "XV_COLORSPACE"},
    {XvSettable | XvGettable, -1, 1, "XV_CRTC"},
    {0, 0, 0, NULL}
};

#define NUM_ATTRIBUTES_R500 8

static XF86AttributeRec Attributes_r500[NUM_ATTRIBUTES_R500+1] =
{
    {XvSettable | XvGettable, 0, 2, "XV_BICUBIC"},
    {XvSettable | XvGettable, 0, 1, "XV_VSYNC"},
    {XvSettable | XvGettable, -1000, 1000, "XV_BRIGHTNESS"},
    {XvSettable | XvGettable, -1000, 1000, "XV_CONTRAST"},
    {XvSettable | XvGettable, -1000, 1000, "XV_SATURATION"},
    {XvSettable | XvGettable, -1000, 1000, "XV_HUE"},
    {XvSettable | XvGettable, 0, 1, "XV_COLORSPACE"},
    {XvSettable | XvGettable, -1, 1, "XV_CRTC"},
    {0, 0, 0, NULL}
};

#define NUM_ATTRIBUTES_R600 7

static XF86AttributeRec Attributes_r600[NUM_ATTRIBUTES_R600+1] =
{
    {XvSettable | XvGettable, 0, 1, "XV_VSYNC"},
    {XvSettable | XvGettable, -1000, 1000, "XV_BRIGHTNESS"},
    {XvSettable | XvGettable, -1000, 1000, "XV_CONTRAST"},
    {XvSettable | XvGettable, -1000, 1000, "XV_SATURATION"},
    {XvSettable | XvGettable, -1000, 1000, "XV_HUE"},
    {XvSettable | XvGettable, 0, 1, "XV_COLORSPACE"},
    {XvSettable | XvGettable, -1, 1, "XV_CRTC"},
    {0, 0, 0, NULL}
};

static XF86AttributeRec Attributes_eg[NUM_ATTRIBUTES_R600+1] =
{
    {XvSettable | XvGettable, 0, 1, "XV_VSYNC"},
    {XvSettable | XvGettable, -1000, 1000, "XV_BRIGHTNESS"},
    {XvSettable | XvGettable, -1000, 1000, "XV_CONTRAST"},
    {XvSettable | XvGettable, -1000, 1000, "XV_SATURATION"},
    {XvSettable | XvGettable, -1000, 1000, "XV_HUE"},
    {XvSettable | XvGettable, 0, 1, "XV_COLORSPACE"},
    {XvSettable | XvGettable, -1, 5, "XV_CRTC"},
    {0, 0, 0, NULL}
};

static Atom xvBicubic;
static Atom xvVSync;
static Atom xvBrightness, xvContrast, xvSaturation, xvHue;
static Atom xvGamma, xvColorspace;
static Atom xvCRTC;

#define NUM_IMAGES 4

static XF86ImageRec Images[NUM_IMAGES] =
{
    XVIMAGE_YUY2,
    XVIMAGE_YV12,
    XVIMAGE_I420,
    XVIMAGE_UYVY
};

int
RADEONGetTexPortAttribute(ScrnInfoPtr  pScrn,
		       Atom	    attribute,
		       INT32	    *value,
		       pointer	    data)
{
    RADEONInfoPtr	info = RADEONPTR(pScrn);
    RADEONPortPrivPtr	pPriv = (RADEONPortPrivPtr)data;

    if (info->accelOn) RADEON_SYNC(info, pScrn);

    if (attribute == xvBicubic)
	*value = pPriv->bicubic_state;
    else if (attribute == xvVSync)
	*value = pPriv->vsync;
    else if (attribute == xvBrightness)
	*value = pPriv->brightness;
    else if (attribute == xvContrast)
	*value = pPriv->contrast;
    else if (attribute == xvSaturation)
	*value = pPriv->saturation;
    else if (attribute == xvHue)
	*value = pPriv->hue;
    else if (attribute == xvGamma)
	*value = pPriv->gamma;
    else if(attribute == xvColorspace)
	*value = pPriv->transform_index;
    else if(attribute == xvCRTC) {
	int		c;
	xf86CrtcConfigPtr	xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	for (c = 0; c < xf86_config->num_crtc; c++)
	    if (xf86_config->crtc[c] == pPriv->desired_crtc)
		break;
	if (c == xf86_config->num_crtc)
	    c = -1;
	*value = c;
    } else
	return BadMatch;

    return Success;
}

int
RADEONSetTexPortAttribute(ScrnInfoPtr  pScrn,
		       Atom	    attribute,
		       INT32	    value,
		       pointer	    data)
{
    RADEONInfoPtr	info = RADEONPTR(pScrn);
    RADEONPortPrivPtr	pPriv = (RADEONPortPrivPtr)data;

    RADEON_SYNC(info, pScrn);

    if (attribute == xvBicubic)
	pPriv->bicubic_state = ClipValue (value, 0, 2);
    else if (attribute == xvVSync)
	pPriv->vsync = ClipValue (value, 0, 1);
    else if (attribute == xvBrightness)
	pPriv->brightness = ClipValue (value, -1000, 1000);
    else if (attribute == xvContrast)
	pPriv->contrast = ClipValue (value, -1000, 1000);
    else if (attribute == xvSaturation)
	pPriv->saturation = ClipValue (value, -1000, 1000);
    else if (attribute == xvHue)
	pPriv->hue = ClipValue (value, -1000, 1000);
    else if (attribute == xvGamma)
	pPriv->gamma = ClipValue (value, 100, 10000);
    else if(attribute == xvColorspace)
	pPriv->transform_index = ClipValue (value, 0, 1);
    else if(attribute == xvCRTC) {
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	if ((value < -1) || (value > xf86_config->num_crtc))
	    return BadValue;
	if (value < 0)
	    pPriv->desired_crtc = NULL;
	else
	    pPriv->desired_crtc = xf86_config->crtc[value];
    } else
	return BadMatch;

    return Success;
}

Bool radeon_load_bicubic_texture(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr    info = RADEONPTR(pScrn);
    int ret;
    /* Bicubic filter loading */
    ret = radeon_allocate_video_bo(pScrn,
				   &info->bicubic_bo,
				   sizeof(bicubic_tex_512), 64,
				   RADEON_GEM_DOMAIN_VRAM);
    if (ret == FALSE)
	return FALSE;

    /* Upload bicubic filter tex */
    if (info->ChipFamily < CHIP_FAMILY_R600) {
	uint8_t *bicubic_addr;
	int ret;
	ret = radeon_bo_map(info->bicubic_bo, 1);
	if (ret)
	    return FALSE;

	bicubic_addr = info->bicubic_bo->ptr;

	RADEONCopySwap(bicubic_addr, (uint8_t *)bicubic_tex_512, 1024,
#if X_BYTE_ORDER == X_BIG_ENDIAN
		       RADEON_HOST_DATA_SWAP_16BIT
#else
		       RADEON_HOST_DATA_SWAP_NONE
#endif
);
	radeon_bo_unmap(info->bicubic_bo);
    }
    return TRUE;
}

#if 0
/* XXX */
static void radeon_unload_bicubic_texture(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr    info = RADEONPTR(pScrn);

    if (info->bicubic_memory != NULL) {
	radeon_bo_unref(info->bicubic_memory);
	info->bicubic_memory = NULL;
    }

}
#endif

static void
RADEONQueryBestSize(
  ScrnInfoPtr pScrn,
  Bool motion,
  short vid_w, short vid_h,
  short drw_w, short drw_h,
  unsigned int *p_w, unsigned int *p_h,
  pointer data
){
    RADEONPortPrivPtr pPriv = (RADEONPortPrivPtr)data;

    if (!pPriv->textured) {
	if (vid_w > (drw_w << 4))
	    drw_w = vid_w >> 4;
	if (vid_h > (drw_h << 4))
	    drw_h = vid_h >> 4;
    }

  *p_w = drw_w;
  *p_h = drw_h;
}

#define FOURCC_RGB24    0x00000000
#define FOURCC_RGBT16   0x54424752
#define FOURCC_RGB16    0x32424752
#define FOURCC_RGBA32   0x41424752

static int
RADEONQueryImageAttributes(
    ScrnInfoPtr pScrn,
    int id,
    unsigned short *w, unsigned short *h,
    int *pitches, int *offsets
){
    const RADEONInfoRec * const info = RADEONPTR(pScrn);
    int size, tmp;

    if(*w > info->xv_max_width) *w = info->xv_max_width;
    if(*h > info->xv_max_height) *h = info->xv_max_height;

    *w = RADEON_ALIGN(*w, 2);
    if(offsets) offsets[0] = 0;

    switch(id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	*h = RADEON_ALIGN(*h, 2);
	size = RADEON_ALIGN(*w, 4);
	if(pitches) pitches[0] = size;
	size *= *h;
	if(offsets) offsets[1] = size;
	tmp = RADEON_ALIGN(*w >> 1, 4);
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

XF86VideoAdaptorPtr
RADEONSetupImageTexturedVideo(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr    info = RADEONPTR(pScrn);
    RADEONPortPrivPtr pPortPriv;
    XF86VideoAdaptorPtr adapt;
    int i;
    int num_texture_ports = 16;

    adapt = calloc(1, sizeof(XF86VideoAdaptorRec) + num_texture_ports *
		   (sizeof(RADEONPortPrivRec) + sizeof(DevUnion)));
    if (adapt == NULL)
	return NULL;

    xvBicubic         = MAKE_ATOM("XV_BICUBIC");
    xvVSync           = MAKE_ATOM("XV_VSYNC");
    xvBrightness      = MAKE_ATOM("XV_BRIGHTNESS");
    xvContrast        = MAKE_ATOM("XV_CONTRAST");
    xvSaturation      = MAKE_ATOM("XV_SATURATION");
    xvHue             = MAKE_ATOM("XV_HUE");
    xvGamma           = MAKE_ATOM("XV_GAMMA");
    xvColorspace      = MAKE_ATOM("XV_COLORSPACE");
    xvCRTC            = MAKE_ATOM("XV_CRTC");

    adapt->type = XvWindowMask | XvInputMask | XvImageMask;
    adapt->flags = 0;
    adapt->name = "Radeon Textured Video";
    adapt->nEncodings = 1;
    if (IS_EVERGREEN_3D)
	adapt->pEncodings = DummyEncodingEG;
    else if (IS_R600_3D)
	adapt->pEncodings = DummyEncodingR600;
    else if (IS_R500_3D)
	adapt->pEncodings = DummyEncodingR500;
    else
	adapt->pEncodings = DummyEncoding;
    adapt->nFormats = NUM_FORMATS;
    adapt->pFormats = Formats;
    adapt->nPorts = num_texture_ports;
    adapt->pPortPrivates = (DevUnion*)(&adapt[1]);

    pPortPriv =
	(RADEONPortPrivPtr)(&adapt->pPortPrivates[num_texture_ports]);

    if (IS_EVERGREEN_3D) {
	adapt->pAttributes = Attributes_eg;
	adapt->nAttributes = NUM_ATTRIBUTES_R600;
    }
    else if (IS_R600_3D) {
	adapt->pAttributes = Attributes_r600;
	adapt->nAttributes = NUM_ATTRIBUTES_R600;
    }
    else if (IS_R500_3D) {
	adapt->pAttributes = Attributes_r500;
	adapt->nAttributes = NUM_ATTRIBUTES_R500;
    }
    else if (IS_R300_3D) {
	adapt->pAttributes = Attributes_r300;
	adapt->nAttributes = NUM_ATTRIBUTES_R300;
    }
    else if (IS_R200_3D) {
	adapt->pAttributes = Attributes_r200;
	adapt->nAttributes = NUM_ATTRIBUTES_R200;
    }
    else {
	adapt->pAttributes = Attributes;
	adapt->nAttributes = NUM_ATTRIBUTES;
    }
    adapt->pImages = Images;
    adapt->nImages = NUM_IMAGES;
    adapt->PutVideo = NULL;
    adapt->PutStill = NULL;
    adapt->GetVideo = NULL;
    adapt->GetStill = NULL;
    adapt->StopVideo = RADEONStopVideo;
    adapt->SetPortAttribute = RADEONSetTexPortAttribute;
    adapt->GetPortAttribute = RADEONGetTexPortAttribute;
    adapt->QueryBestSize = RADEONQueryBestSize;
    adapt->PutImage = RADEONPutImageTextured;
    adapt->ReputImage = NULL;
    adapt->QueryImageAttributes = RADEONQueryImageAttributes;

    for (i = 0; i < num_texture_ports; i++) {
	RADEONPortPrivPtr pPriv = &pPortPriv[i];

	pPriv->textured = TRUE;
	pPriv->bicubic_state = BICUBIC_OFF;
	pPriv->vsync = TRUE;
	pPriv->brightness = 0;
	pPriv->contrast = 0;
	pPriv->saturation = 0;
	pPriv->hue = 0;
	pPriv->gamma = 1000;
	pPriv->transform_index = 0;
	pPriv->desired_crtc = NULL;

	/* gotta uninit this someplace, XXX: shouldn't be necessary for textured */
	REGION_NULL(pScreen, &pPriv->clip);
	adapt->pPortPrivates[i].ptr = (pointer) (pPriv);
    }

    if (IS_R500_3D || IS_R300_3D)
	radeon_load_bicubic_texture(pScrn);

    info->xv_max_width = adapt->pEncodings->width;
    info->xv_max_height = adapt->pEncodings->height;

    return adapt;
}

