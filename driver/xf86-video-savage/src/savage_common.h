/* savage_common.h -- Public header for the savage driver (IOCTLs)
 *
 * Copyright 2004  Felix Kuehling
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * VIA, S3 GRAPHICS, AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __SAVAGE_COMMON_H__
#define __SAVAGE_COMMON_H__

#include <inttypes.h>

/* Savage-specific ioctls
 */
#define DRM_SAVAGE_BCI_INIT		0x00
#define DRM_SAVAGE_BCI_CMDBUF           0x01
#define DRM_SAVAGE_BCI_EVENT_EMIT	0x02
#define DRM_SAVAGE_BCI_EVENT_WAIT	0x03

#define SAVAGE_DMA_PCI	1
#define SAVAGE_DMA_AGP	3
typedef struct {
	enum {
		SAVAGE_INIT_BCI = 1,
		SAVAGE_CLEANUP_BCI = 2
	} func;
	unsigned int sarea_priv_offset;

	/* some parameters */
	unsigned int cob_size;
	unsigned int bci_threshold_lo, bci_threshold_hi;
	unsigned int dma_type;

	/* frame buffer layout */
	unsigned int fb_bpp;
	unsigned int front_offset, front_pitch;
	unsigned int back_offset, back_pitch;
	unsigned int depth_bpp;
	unsigned int depth_offset, depth_pitch;

	/* local textures */
	unsigned int texture_offset;
	unsigned int texture_size;

	/* physical locations of non-permanent maps */
	unsigned long status_offset;
	unsigned long buffers_offset;
	unsigned long agp_textures_offset;
	unsigned long cmd_dma_offset;
} drmSAVAGEInit;

typedef union drmSAVAGECmdHeader drmSAVAGECmdHeader;
typedef struct {
				/* command buffer in client's address space */
	drmSAVAGECmdHeader *cmd_addr;
	unsigned int size;	/* size of the command buffer in 64bit units */

	unsigned int dma_idx;	/* DMA buffer index to use */
	int discard;		/* discard DMA buffer when done */
				/* vertex buffer in client's address space */
	unsigned int *vb_addr;
	unsigned int vb_size;	/* size of client vertex buffer in bytes */
	unsigned int vb_stride;	/* stride of vertices in 32bit words */
				/* boxes in client's address space */
	drm_clip_rect_t *box_addr;
	unsigned int nbox;	/* number of clipping boxes */
} drmSAVAGECmdbuf;

#define SAVAGE_WAIT_2D  0x1 /* wait for 2D idle before updating event tag */
#define SAVAGE_WAIT_3D  0x2 /* wait for 3D idle before updating event tag */
#define SAVAGE_WAIT_IRQ 0x4 /* emit or wait for IRQ, not implemented yet */
typedef struct {
	unsigned int count;
	unsigned int flags;
} drmSAVAGEEventEmit, drmSAVAGEEventWait;

/* Commands for the cmdbuf ioctl
 */
#define SAVAGE_CMD_STATE	0  /* a range of state registers */
#define SAVAGE_CMD_DMA_PRIM	1  /* vertices from DMA buffer */
#define SAVAGE_CMD_VTX_PRIM	2  /* vertices from client vertex buffer */
#define SAVAGE_CMD_DMA_IDX	3  /* indexed vertices from DMA buffer */
#define SAVAGE_CMD_VTX_IDX	4  /* indexed vertices client vertex buffer */
#define SAVAGE_CMD_CLEAR	5  /* clear buffers */
#define SAVAGE_CMD_SWAP		6  /* swap buffers */

/* Primitive types
*/
#define SAVAGE_PRIM_TRILIST	0  /* triangle list */
#define SAVAGE_PRIM_TRISTRIP	1  /* triangle strip */
#define SAVAGE_PRIM_TRIFAN	2  /* triangle fan */
#define SAVAGE_PRIM_TRILIST_201	3  /* reorder verts for correct flat
				    * shading on s3d */

/* Skip flags (vertex format)
 */
#define SAVAGE_SKIP_Z		0x01
#define SAVAGE_SKIP_W		0x02
#define SAVAGE_SKIP_C0		0x04
#define SAVAGE_SKIP_C1		0x08
#define SAVAGE_SKIP_S0		0x10
#define SAVAGE_SKIP_T0		0x20
#define SAVAGE_SKIP_ST0		0x30
#define SAVAGE_SKIP_S1		0x40
#define SAVAGE_SKIP_T1		0x80
#define SAVAGE_SKIP_ST1		0xc0
#define SAVAGE_SKIP_ALL_S3D	0x3f
#define SAVAGE_SKIP_ALL_S4	0xff

/* Buffer names for clear command
 */
#define SAVAGE_FRONT		0x1
#define SAVAGE_BACK		0x2
#define SAVAGE_DEPTH		0x4

/* 64-bit command header
 */
union drmSAVAGECmdHeader {
	struct {
		unsigned char cmd;	/* command */
		unsigned char pad0;
		unsigned short pad1;
		unsigned short pad2;
		unsigned short pad3;
	} cmd; /* generic */
	struct {
		unsigned char cmd;
		unsigned char global;	/* need idle engine? */
		unsigned short count;	/* number of consecutive registers */
		unsigned short start;	/* first register */
		unsigned short pad3;
	} state; /* SAVAGE_CMD_STATE */
	struct {
		unsigned char cmd;
		unsigned char prim;	/* primitive type */
		unsigned short skip;	/* vertex format (skip flags) */
		unsigned short count;	/* number of vertices */
		unsigned short start;	/* first vertex in DMA/vertex buffer */
	} prim; /* SAVAGE_CMD_DMA_PRIM, SAVAGE_CMD_VB_PRIM */
	struct {
		unsigned char cmd;
		unsigned char prim;
		unsigned short skip;
		unsigned short count;	/* number of indices that follow */
		unsigned short pad3;
	} idx; /* SAVAGE_CMD_DMA_IDX, SAVAGE_CMD_VB_IDX */
	struct {
		unsigned char cmd;
		unsigned char pad0;
		unsigned short pad1;
		unsigned int flags;
	} clear0; /* SAVAGE_CMD_CLEAR */
	struct {
		unsigned int mask;
		unsigned int value;
	} clear1; /* SAVAGE_CMD_CLEAR data */
};

#endif /* __SAVAGE_COMMON_H__ */
