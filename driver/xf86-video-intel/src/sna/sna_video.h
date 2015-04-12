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

#define FOURCC_XVMC (('C' << 24) + ('M' << 16) + ('V' << 8) + 'X')
#define FOURCC_RGB565 ((16 << 24) + ('B' << 16) + ('G' << 8) + 'R')
#define FOURCC_RGB888 ((24 << 24) + ('B' << 16) + ('G' << 8) + 'R')

/*
 * Below, a dummy picture type that is used in XvPutImage
 * only to do an overlay update.
 * Introduced for the XvMC client lib.
 * Defined to have a zero data size.
 */
#define XVMC_YUV { \
	FOURCC_XVMC, XvYUV, LSBFirst, \
	{'X', 'V', 'M', 'C', 0x00, 0x00, 0x00, 0x10, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71}, \
	12, XvPlanar, 3, 0, 0, 0, 0, 8, 8, 8, 1, 2, 2, 1, 2, 2, \
	{'Y', 'V', 'U', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, \
	XvTopToBottom \
}

#define XVMC_RGB565 { \
	FOURCC_RGB565, XvRGB, LSBFirst, \
	{'P', 'A', 'S', 'S', 'T', 'H', 'R', 'O', 'U', 'G', 'H', 'R', 'G', 'B', '1', '6'}, \
	16, XvPacked, 1, 16, 0x1f<<11, 0x3f<<5, 0x1f<<0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	{'B', 'G', 'R', 'X', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, \
	XvTopToBottom \
}

#define XVMC_RGB888 { \
	FOURCC_RGB888, XvRGB, LSBFirst, \
	{'P', 'A', 'S', 'S', 'T', 'H', 'R', 'O', 'U', 'G', 'H', 'R', 'G', 'B', '2', '4'}, \
	32, XvPacked, 1, 24, 0xff<<16, 0xff<<8, 0xff<<0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	{'B', 'G', 'R', 'X', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, \
	XvTopToBottom \
}

struct sna_video {
	struct sna *sna;

	int brightness;
	int contrast;
	int saturation;
	xf86CrtcPtr desired_crtc;

	uint32_t gamma0;
	uint32_t gamma1;
	uint32_t gamma2;
	uint32_t gamma3;
	uint32_t gamma4;
	uint32_t gamma5;

	unsigned color_key;
	unsigned color_key_changed;
	bool has_color_key;

	/** YUV data buffers */
	struct kgem_bo *old_buf[2];
	struct kgem_bo *buf;
	int width, height, format;

	int alignment;
	bool tiled;
	bool textured;
	int plane;

	struct kgem_bo *bo[4];
	RegionRec clip;

	int SyncToVblank;	/* -1: auto, 0: off, 1: on */
	int AlwaysOnTop;
};

struct sna_video_frame {
	struct kgem_bo *bo;
	uint32_t id;
	uint32_t size;
	uint32_t UBufOffset;
	uint32_t VBufOffset;
	Rotation rotation;

	uint16_t width, height;
	uint16_t pitch[2];

	/* extents */
	BoxRec image;
	BoxRec src;
};

static inline XvScreenPtr to_xv(ScreenPtr screen)
{
	return dixLookupPrivate(&screen->devPrivates, XvGetScreenKey());
}

void sna_video_init(struct sna *sna, ScreenPtr screen);
void sna_video_overlay_setup(struct sna *sna, ScreenPtr screen);
void sna_video_sprite_setup(struct sna *sna, ScreenPtr screen);
void sna_video_textured_setup(struct sna *sna, ScreenPtr screen);
void sna_video_destroy_window(WindowPtr win);
void sna_video_close(struct sna *sna);

XvAdaptorPtr sna_xv_adaptor_alloc(struct sna *sna);
int sna_xv_fixup_formats(ScreenPtr screen,
			 XvFormatPtr formats,
			 int num_formats);
int sna_xv_alloc_port(unsigned long port, XvPortPtr in, XvPortPtr *out);
int sna_xv_free_port(XvPortPtr port);

static inline int xvmc_passthrough(int id)
{
	switch (id) {
	case FOURCC_XVMC:
	case FOURCC_RGB565:
	case FOURCC_RGB888:
		return true;
	default:
		return false;
	}
}

static inline int is_planar_fourcc(int id)
{
	switch (id) {
	case FOURCC_YV12:
	case FOURCC_I420:
	case FOURCC_XVMC:
		return 1;
	default:
		return 0;
	}
}

bool
sna_video_clip_helper(struct sna_video *video,
		      struct sna_video_frame *frame,
		      xf86CrtcPtr *crtc_ret,
		      BoxPtr dst,
		      short src_x, short src_y,
		      short drw_x, short drw_y,
		      short src_w, short src_h,
		      short drw_w, short drw_h,
		      RegionPtr reg);

void
sna_video_frame_init(struct sna_video *video,
		     int id, short width, short height,
		     struct sna_video_frame *frame);

void
sna_video_frame_set_rotation(struct sna_video *video,
			     struct sna_video_frame *frame,
			     Rotation rotation);

struct kgem_bo *
sna_video_buffer(struct sna_video *video,
		 struct sna_video_frame *frame);

bool
sna_video_copy_data(struct sna_video *video,
		    struct sna_video_frame *frame,
		    const uint8_t *buf);

void sna_video_buffer_fini(struct sna_video *video);

void sna_video_free_buffers(struct sna_video *video);

static inline XvPortPtr
sna_window_get_port(WindowPtr window)
{
	return ((void **)__get_private(window, sna_window_key))[2];
}

static inline void
sna_window_set_port(WindowPtr window, XvPortPtr port)
{
	((void **)__get_private(window, sna_window_key))[2] = port;
}

#endif /* SNA_VIDEO_H */
