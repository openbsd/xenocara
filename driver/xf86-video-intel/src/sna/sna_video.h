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

#ifndef SNA_VIDEO_H
#define SNA_VIDEO_H

#include <xf86_OSproc.h>
#include <xf86xv.h>
#include <fourcc.h>

#if defined(XvMCExtension) && defined(ENABLE_XVMC)
#define SNA_XVMC 1
#endif

struct sna_video {
	int brightness;
	int contrast;
	int saturation;
	xf86CrtcPtr desired_crtc;

	RegionRec clip;

	uint32_t gamma0;
	uint32_t gamma1;
	uint32_t gamma2;
	uint32_t gamma3;
	uint32_t gamma4;
	uint32_t gamma5;

	int color_key;
	int color_key_changed;

	/** YUV data buffers */
	struct kgem_bo *old_buf[2];
	struct kgem_bo *buf;

	int alignment;
	bool tiled;
	bool textured;
	Rotation rotation;
	int plane;

	int SyncToVblank;	/* -1: auto, 0: off, 1: on */
};

struct sna_video_frame {
	struct kgem_bo *bo;
	uint32_t id;
	uint32_t size;
	uint32_t UBufOffset;
	uint32_t VBufOffset;

	uint16_t width, height;
	uint16_t pitch[2];

	/* extents */
	BoxRec image;
	BoxRec src;
};

void sna_video_init(struct sna *sna, ScreenPtr screen);
XF86VideoAdaptorPtr sna_video_overlay_setup(struct sna *sna, ScreenPtr screen);
XF86VideoAdaptorPtr sna_video_sprite_setup(struct sna *sna, ScreenPtr screen);
XF86VideoAdaptorPtr sna_video_textured_setup(struct sna *sna, ScreenPtr screen);

#define FOURCC_XVMC     (('C' << 24) + ('M' << 16) + ('V' << 8) + 'X')

static inline int is_planar_fourcc(int id)
{
	switch (id) {
	case FOURCC_YV12:
	case FOURCC_I420:
	case FOURCC_XVMC:
		return 1;
	case FOURCC_UYVY:
	case FOURCC_YUY2:
	default:
		return 0;
	}
}

bool
sna_video_clip_helper(ScrnInfoPtr scrn,
		      struct sna_video *adaptor_priv,
		      struct sna_video_frame *frame,
		      xf86CrtcPtr * crtc_ret,
		      BoxPtr dst,
		      short src_x, short src_y,
		      short drw_x, short drw_y,
		      short src_w, short src_h,
		      short drw_w, short drw_h,
		      RegionPtr reg);

void
sna_video_frame_init(struct sna *sna,
		     struct sna_video *video,
		     int id, short width, short height,
		     struct sna_video_frame *frame);

struct kgem_bo *
sna_video_buffer(struct sna *sna,
		 struct sna_video *video,
		 struct sna_video_frame *frame);

bool
sna_video_copy_data(struct sna *sna,
		    struct sna_video *video,
		    struct sna_video_frame *frame,
		    const uint8_t *buf);

void sna_video_buffer_fini(struct sna *sna,
			   struct sna_video *video);

void sna_video_free_buffers(struct sna *sna, struct sna_video *video);

#endif /* SNA_VIDEO_H */
