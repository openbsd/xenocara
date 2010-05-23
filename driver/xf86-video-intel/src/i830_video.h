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

#include "xf86.h"
#include "xf86_OSproc.h"

typedef struct {
	uint32_t YBufOffset;
	uint32_t UBufOffset;
	uint32_t VBufOffset;

	int brightness;
	int contrast;
	int saturation;
	xf86CrtcPtr current_crtc;
	xf86CrtcPtr desired_crtc;

	RegionRec clip;
	uint32_t colorKey;

	uint32_t gamma0;
	uint32_t gamma1;
	uint32_t gamma2;
	uint32_t gamma3;
	uint32_t gamma4;
	uint32_t gamma5;

	/* only used by the overlay */
	uint32_t videoStatus;
	Time offTime;
	Time freeTime;
   /** YUV data buffers */
	drm_intel_bo *buf;	/* current buffer to draw into */
	drm_intel_bo *oldBuf;	/* old buffer, may be in use by the overlay hw */
	Bool oldBuf_pinned;	/* only actually pinned when in use by the overlay hw */

	Bool overlayOK;
	int oneLineMode;
	int scaleRatio;
	Bool textured;
	Rotation rotation;	/* should remove intel->rotation later */

	int SyncToVblank;	/* -1: auto, 0: off, 1: on */
} intel_adaptor_private;

static inline intel_adaptor_private *
intel_get_adaptor_private(ScrnInfoPtr scrn)
{
	return intel_get_screen_private(scrn)->adaptor->pPortPrivates[0].ptr;
}

void I915DisplayVideoTextured(ScrnInfoPtr scrn,
			      intel_adaptor_private *adaptor_priv,
			      int id, RegionPtr dstRegion, short width,
			      short height, int video_pitch, int video_pitch2,
			      short src_w, short src_h,
			      short drw_w, short drw_h, PixmapPtr pixmap);

void I965DisplayVideoTextured(ScrnInfoPtr scrn,
			      intel_adaptor_private *adaptor_priv,
			      int id, RegionPtr dstRegion, short width,
			      short height, int video_pitch, int video_pitch2,
			      short src_w, short src_h,
			      short drw_w, short drw_h, PixmapPtr pixmap);

void I830VideoBlockHandler(int i, pointer blockData, pointer pTimeout,
			   pointer pReadmask);
void i965_free_video(ScrnInfoPtr scrn);

int is_planar_fourcc(int id);
