/*****************************************************************************
 * VIA Unichrome XvMC extension client lib.
 *
 * Copyright (c) 2004 Thomas Hellström. All rights reserved.
 * Copyright (c) 2003 Andreas Robinson. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHOR(S) OR COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Low-level functions that deal directly with the hardware. In the future,
 * these functions might be implemented in a kernel module. Also, some of them
 * would benefit from DMA.
 *
 * Authors:
 *  Andreas Robinson 2003. (Initial decoder interface functions).
 *  Thomas Hellstrom 2004, 2005 (Blitting functions, AGP and locking, Unichrome Pro Video AGP).
 *  Ivor Hewitt 2005 (Unichrome Pro modifications and merging).
 */

/* IH
 * I've left the proReg or-ing in case we need/want to implement the V1/V3
 * register toggle too, which also moves the register locations.
 * The CN400 has dual mpeg decoders, not sure at the moment whether these
 * are also operated through independent registers also.
 */

#undef VIDEO_DMA
#define HQV_USE_IRQ
#define UNICHROME_PRO

#include "viaXvMCPriv.h"
#include "viaLowLevel.h"
#include "driDrawable.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

typedef enum
{ ll_init, ll_agpBuf, ll_pciBuf, ll_timeStamp, ll_llBuf }
LLState;

typedef struct
{
    drm_via_mem_t mem;
    unsigned offset;
    unsigned stride;
    unsigned height;
} LowLevelBuffer;

struct _XvMCLowLevel;

typedef struct _ViaCommandBuffer
{
    CARD32 *buf;
    CARD32 waitFlags;
    unsigned pos;
    unsigned bufSize;
    int mode;
    int header_start;
    int rindex;
    void (*flushFunc) (struct _ViaCommandBuffer * cb,
	struct _XvMCLowLevel * xl);
} ViaCommandBuffer;

typedef struct _XvMCLowLevel
{
    ViaCommandBuffer agpBuf, pciBuf, *videoBuf;
    int use_agp;
    int fd;
    drm_context_t *drmcontext;
    drmLockPtr hwLock;
    drmAddress mmioAddress;
    drmAddress fbAddress;
    unsigned fbStride;
    unsigned fbDepth;
    unsigned width;
    unsigned height;
    int performLocking;
    unsigned errors;
    drm_via_mem_t tsMem;
    CARD32 tsOffset;
    volatile CARD32 *tsP;
    CARD32 curTimeStamp;
    CARD32 lastReadTimeStamp;
    int agpSync;
    CARD32 agpSyncTimeStamp;
    unsigned chipId;

    /*
     * Data for video-engine less display
     */

    XvMCRegion sRegion;
    XvMCRegion dRegion;
    LowLevelBuffer scale;
    LowLevelBuffer back;
    Bool downScaling;
    CARD32 downScaleW;
    CARD32 downScaleH;
    CARD32 upScaleW;
    CARD32 upScaleH;
    unsigned fetch;
    unsigned line;
    LLState state;
} XvMCLowLevel;

/*
 * For Other architectures than i386 these might have to be modified for
 * bigendian etc.
 */

#define MPEGIN(xl,reg)							\
    *((volatile CARD32 *)(((CARD8 *)(xl)->mmioAddress) + 0xc00 + (reg)))

#define VIDIN(ctx,reg)							\
    *((volatile CARD32 *)(((CARD8 *)(ctx)->mmioAddress) + (reg)))

#define REGIN(ctx,reg)							\
    *((volatile CARD32 *)(((CARD8 *)(ctx)->mmioAddress) + 0x0000 + (reg)))

#define HQV_CONTROL             0x3D0
#define HQV_SRC_OFFSET          0x3CC
#define HQV_SRC_STARTADDR_Y     0x3D4
#define HQV_SRC_STARTADDR_U     0x3D8
#define HQV_SRC_STARTADDR_V     0x3DC
#define HQV_MINIFY_DEBLOCK      0x3E8

#define REG_HQV1_INDEX      0x00001000

#define HQV_SW_FLIP         0x00000010
#define HQV_FLIP_STATUS     0x00000001
#define HQV_SUBPIC_FLIP     0x00008000
#define HQV_FLIP_ODD        0x00000020
#define HQV_DEINTERLACE     0x00010000
#define HQV_FIELD_2_FRAME   0x00020000
#define HQV_FRAME_2_FIELD   0x00040000
#define HQV_FIELD_UV        0x00100000
#define HQV_DEBLOCK_HOR     0x00008000
#define HQV_DEBLOCK_VER     0x80000000
#define HQV_YUV420          0xC0000000
#define HQV_YUV422          0x80000000
#define HQV_ENABLE          0x08000000
#define HQV_GEN_IRQ         0x00000080

#define HQV_SCALE_ENABLE    0x00000800
#define HQV_SCALE_DOWN      0x00001000

#define V_COMPOSE_MODE          0x298
#define V1_COMMAND_FIRE         0x80000000
#define V3_COMMAND_FIRE         0x40000000

/* SUBPICTURE Registers */
#define SUBP_CONTROL_STRIDE     0x3C0
#define SUBP_STARTADDR          0x3C4
#define RAM_TABLE_CONTROL       0x3C8
#define RAM_TABLE_READ          0x3CC

/* SUBP_CONTROL_STRIDE              0x3c0 */
#define SUBP_HQV_ENABLE             0x00010000
#define SUBP_IA44                   0x00020000
#define SUBP_AI44                   0x00000000
#define SUBP_STRIDE_MASK            0x00001fff
#define SUBP_CONTROL_MASK           0x00070000

/* RAM_TABLE_CONTROL                0x3c8 */
#define RAM_TABLE_RGB_ENABLE        0x00000007

#define VIA_REG_STATUS          0x400
#define VIA_REG_GEMODE          0x004
#define VIA_REG_SRCBASE         0x030
#define VIA_REG_DSTBASE         0x034
#define VIA_REG_PITCH           0x038
#define VIA_REG_SRCCOLORKEY     0x01C
#define VIA_REG_KEYCONTROL      0x02C
#define VIA_REG_SRCPOS          0x008
#define VIA_REG_DSTPOS          0x00C
#define VIA_REG_GECMD           0x000
#define VIA_REG_DIMENSION       0x010  /* width and height */
#define VIA_REG_FGCOLOR         0x018

#define VIA_VR_QUEUE_BUSY       0x00020000	/* Virtual Queue is busy */
#define VIA_CMD_RGTR_BUSY       0x00000080	/* Command Regulator is busy */
#define VIA_2D_ENG_BUSY         0x00000002	/* 2D Engine is busy */
#define VIA_3D_ENG_BUSY         0x00000001	/* 3D Engine is busy */
#define VIA_GEM_8bpp            0x00000000
#define VIA_GEM_16bpp           0x00000100
#define VIA_GEM_32bpp           0x00000300
#define VIA_GEC_BLT             0x00000001
#define VIA_PITCH_ENABLE        0x80000000
#define VIA_GEC_INCX            0x00000000
#define VIA_GEC_DECY            0x00004000
#define VIA_GEC_INCY            0x00000000
#define VIA_GEC_DECX            0x00008000
#define VIA_GEC_FIXCOLOR_PAT    0x00002000

#define VIA_BLIT_CLEAR 0x00
#define VIA_BLIT_COPY 0xCC
#define VIA_BLIT_FILL 0xF0
#define VIA_BLIT_SET 0xFF

#define VIA_SYNCWAITTIMEOUT 50000      /* Might be a bit conservative */
#define VIA_DMAWAITTIMEOUT 150000
#define VIA_VIDWAITTIMEOUT 50000
#define VIA_XVMC_DECODERTIMEOUT 50000  /*(microseconds) */

#define VIA_AGP_HEADER5 0xFE040000
#define VIA_AGP_HEADER6 0xFE050000

typedef struct
{
    CARD32 data;
    Bool set;
} HQVRegister;

#define H1_ADDR(val) (((val) >> 2) | 0xF0000000)
#define WAITFLAGS(cb, flags)			\
    (cb)->waitFlags |= (flags)
#define BEGIN_RING_AGP(cb, xl, size)					\
    do {								\
	if ((cb)->pos > ((cb)->bufSize-(size))) {			\
	    cb->flushFunc(cb, xl);					\
	}								\
    } while(0)
#define OUT_RING_AGP(cb, val) do{			\
	(cb)->buf[(cb)->pos++] = (val);	\
  } while(0);

#define OUT_RING_QW_AGP(cb, val1, val2)			\
    do {						\
	(cb)->buf[(cb)->pos++] = (val1);	\
	(cb)->buf[(cb)->pos++] = (val2);	\
    } while (0)

#define BEGIN_HEADER5_AGP(cb, xl, index)	\
    do {					\
	BEGIN_RING_AGP(cb, xl, 8);		\
	(cb)->mode = VIA_AGP_HEADER5;		\
        (cb)->rindex = (index);			\
	(cb)->header_start = (cb)->pos;		\
	(cb)->pos += 4;				\
    } while (0)

#define BEGIN_HEADER6_AGP(cb, xl)		\
    do {					\
	BEGIN_RING_AGP(cb, xl, 8);		\
	(cb)->mode = VIA_AGP_HEADER6;	\
	(cb)->header_start = (cb)->pos; \
	(cb)->pos += 4;			\
    } while (0)

#define BEGIN_HEADER5_DATA(cb, xl, size, index)				\
    do {								\
	if ((cb)->pos > ((cb)->bufSize - ((size) + 16))) {		\
	    cb->flushFunc(cb, xl);					\
	    BEGIN_HEADER5_AGP(cb, xl, index);				\
	} else if ((cb)->mode && (((cb)->mode != VIA_AGP_HEADER5) ||	\
				  ((cb)->rindex != index))) {		\
	    finish_header_agp(cb);					\
	    BEGIN_HEADER5_AGP((cb), xl, (index));			\
	} else if (cb->mode != VIA_AGP_HEADER5) {			\
	    BEGIN_HEADER5_AGP((cb), xl, (index));			\
	}								\
    }while(0)

#define BEGIN_HEADER6_DATA(cb, xl, size)				\
    do{									\
	if ((cb)->pos > (cb->bufSize-(((size) << 1) + 16))) {		\
	    cb->flushFunc(cb, xl);					\
	    BEGIN_HEADER6_AGP(cb, xl);					\
	} else	if ((cb)->mode && ((cb)->mode != VIA_AGP_HEADER6)) {	\
	    finish_header_agp(cb);					\
	    BEGIN_HEADER6_AGP(cb, xl);					\
	}								\
	else if ((cb->mode != VIA_AGP_HEADER6)) {			\
	    BEGIN_HEADER6_AGP(cb, (xl));				\
	}								\
    }while(0)

#define HQV_SHADOW_BASE 0x3CC
#define HQV_SHADOW_SIZE 13

#define SETHQVSHADOW(shadow, offset, value)				\
    do {								\
	HQVRegister *r = (shadow) + (((offset) - HQV_SHADOW_BASE) >> 2); \
	r->data = (value);						\
	r->set = TRUE;							\
    } while(0)

#define GETHQVSHADOW(shadow, offset)  ((shadow)[(offset - HQV_SHADOW_BASE) >> 2].data)

#define LL_HW_LOCK(xl)							\
    do {								\
	DRM_LOCK((xl)->fd,(xl)->hwLock,*(xl)->drmcontext,0);		\
    } while(0);
#define LL_HW_UNLOCK(xl)					\
    do {							\
	DRM_UNLOCK((xl)->fd,(xl)->hwLock,*(xl)->drmcontext);	\
    } while(0);

static HQVRegister hqvShadow[HQV_SHADOW_SIZE];

static void
initHQVShadow(HQVRegister * r)
{
    int i;

    for (i = 0; i < HQV_SHADOW_SIZE; ++i) {
	r->data = 0;
	r++->set = FALSE;
    }
}

#if 0
static void
setHQVHWDeinterlacing(HQVRegister * shadow, Bool on, Bool motionDetect,
    CARD32 stride, CARD32 height)
{
    CARD32 tmp = GETHQVSHADOW(shadow, 0x3E4);

    if (!on) {
	tmp &= ~((1 << 0) | (1 << 12) | (1 << 27) | (1 << 31));
	SETHQVSHADOW(shadow, 0x3E4, tmp);
	return;
    }

    tmp = (1 << 31) |
	(4 << 28) |
	(1 << 27) |
	(3 << 25) | (1 << 18) | (2 << 14) | (8 << 8) | (8 << 1) | (1 << 0);

    if (motionDetect)
	tmp |= (1 << 12);

    SETHQVSHADOW(shadow, 0x3E4, tmp);

    tmp = GETHQVSHADOW(shadow, 0x3DC);
    tmp |= (stride * height * 1536) / 1024 & 0x7ff;

    SETHQVSHADOW(shadow, 0x3DC, tmp);

    tmp = GETHQVSHADOW(shadow, 0x3D0);
    tmp |= (1 << 23);

    SETHQVSHADOW(shadow, 0x3D0, tmp);
}

#endif

static void
setHQVDeblocking(HQVRegister * shadow, Bool on, Bool lowPass)
{
    CARD32 tmp = GETHQVSHADOW(shadow, 0x3DC);

    if (!on) {
	tmp &= ~(1 << 27);
	SETHQVSHADOW(shadow, 0x3DC, tmp);
	return;
    }

    tmp |= (8 << 16) | (1 << 27);
    if (lowPass)
	tmp |= (1 << 26);
    SETHQVSHADOW(shadow, 0x3DC, tmp);

    tmp = GETHQVSHADOW(shadow, 0x3D4);
    tmp |= (6 << 27);
    SETHQVSHADOW(shadow, 0x3D4, tmp);

    tmp = GETHQVSHADOW(shadow, 0x3D8);
    tmp |= (19 << 27);
    SETHQVSHADOW(shadow, 0x3D8, tmp);
}

static void
setHQVStartAddress(HQVRegister * shadow, unsigned yOffs, unsigned uOffs,
    unsigned stride, unsigned format)
{
    CARD32 tmp = GETHQVSHADOW(shadow, 0x3D4);

    tmp |= yOffs & 0x03FFFFF0;
    SETHQVSHADOW(shadow, 0x3D4, tmp);
    tmp = GETHQVSHADOW(shadow, 0x3D8);
    tmp |= uOffs & 0x03FFFFF0;
    SETHQVSHADOW(shadow, 0x3D8, tmp);
    tmp = GETHQVSHADOW(shadow, 0x3F8);
    tmp |= (stride & 0x1FF8);
    SETHQVSHADOW(shadow, 0x3F8, tmp);
    tmp = GETHQVSHADOW(shadow, 0x3D0);

    if (format == 0) {
	/*
	 * NV12
	 */
	tmp |= (0x0C << 28);
    } else if (format == 1) {
	/*
	 * RGB16
	 */
	tmp |= (0x02 << 28);
    } else if (format == 2) {
	/*
	 * RGB32
	 */
	;
    }
    SETHQVSHADOW(shadow, 0x3D0, tmp);
}

#if 0

static void
setHQVColorSpaceConversion(HQVRegister * shadow, unsigned depth, Bool on)
{
    CARD32 tmp = GETHQVSHADOW(shadow, 0x3DC);

    if (!on) {
	tmp &= ~(1 << 28);
	SETHQVSHADOW(shadow, 0x3DC, tmp);
	return;
    }

    if (depth == 32)
	tmp |= (1 << 29);
    tmp |= (1 << 28);
    tmp &= ~(1 << 15);
    SETHQVSHADOW(shadow, 0x3DC, tmp);
}

static void
setHQVFetchLine(HQVRegister * shadow, unsigned fetch, unsigned lines)
{
    SETHQVSHADOW(shadow, 0x3E0,
	((lines - 1) & 0x7FF) | (((fetch - 1) & 0x1FFF) << 16));
}

static void
setHQVScale(HQVRegister * shadow, unsigned horizontal, unsigned vertical)
{
    SETHQVSHADOW(shadow, 0x3E8,
	(horizontal & 0xFFFF) | ((vertical & 0xFFFF) << 16));
}

static void
setHQVSingleDestination(HQVRegister * shadow, unsigned offset,
    unsigned stride)
{
    CARD32 tmp = GETHQVSHADOW(shadow, 0x3D0);

    tmp |= (1 << 6);

    SETHQVSHADOW(shadow, 0x3D0, tmp);
    SETHQVSHADOW(shadow, 0x3EC, offset & 0x03FFFFF8);
    SETHQVSHADOW(shadow, 0x3F4, stride & 0x1FF8);
}
#endif

static void
setHQVDeinterlacing(HQVRegister * shadow, CARD32 frameType)
{
    CARD32 tmp = GETHQVSHADOW(shadow, 0x3D0);

    if ((frameType & XVMC_FRAME_PICTURE) == XVMC_TOP_FIELD) {
	tmp |= HQV_FIELD_UV |
	    HQV_DEINTERLACE | HQV_FIELD_2_FRAME | HQV_FRAME_2_FIELD;
    } else if ((frameType & XVMC_FRAME_PICTURE) == XVMC_BOTTOM_FIELD) {
	tmp |= HQV_FIELD_UV |
	    HQV_DEINTERLACE |
	    HQV_FIELD_2_FRAME | HQV_FRAME_2_FIELD | HQV_FLIP_ODD;
    }
    SETHQVSHADOW(shadow, 0x3D0, tmp);
}

static void
setHQVTripleBuffer(HQVRegister * shadow, Bool on)
{
    CARD32 tmp = GETHQVSHADOW(shadow, 0x3D0);

    if (on)
	tmp |= (1 << 26);
    else
	tmp &= ~(1 << 26);
    SETHQVSHADOW(shadow, 0x3D0, tmp);
}

static void
finish_header_agp(ViaCommandBuffer * cb)
{
    int numDWords, i;

    CARD32 *hb;

    if (!cb->mode)
	return;
    numDWords = cb->pos - cb->header_start - 4;
    hb = cb->buf + cb->header_start;
    switch (cb->mode) {
    case VIA_AGP_HEADER5:
	hb[0] = VIA_AGP_HEADER5 | cb->rindex;
	hb[1] = numDWords;
	hb[2] = 0x00F50000;	       /* SW debug flag. (?) */
	break;
    default:
	hb[0] = VIA_AGP_HEADER6;
	hb[1] = numDWords >> 1;
	hb[2] = 0x00F60000;	       /* SW debug flag. (?) */
	break;
    }
    hb[3] = 0;
    if (numDWords & 3) {
	for (i = 0; i < (4 - (numDWords & 3)); ++i)
	    OUT_RING_AGP(cb, 0x00000000);
    }
    cb->mode = 0;
}

void
hwlLock(void *xlp, int videoLock)
{
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    LL_HW_LOCK(xl);
}

void
hwlUnlock(void *xlp, int videoLock)
{
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    LL_HW_UNLOCK(xl);
}

static unsigned
timeDiff(struct timeval *now, struct timeval *then)
{
    return (now->tv_usec >= then->tv_usec) ?
	now->tv_usec - then->tv_usec :
	1000000 - (then->tv_usec - now->tv_usec);
}

void
setAGPSyncLowLevel(void *xlp, int val, CARD32 timeStamp)
{
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    xl->agpSync = val;
    xl->agpSyncTimeStamp = timeStamp;
}

CARD32
viaDMATimeStampLowLevel(void *xlp)
{
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    if (xl->use_agp) {
	viaBlit(xl, 32, xl->tsOffset, 1, xl->tsOffset, 1, 1, 1, 0, 0,
	    VIABLIT_FILL, xl->curTimeStamp);
	return xl->curTimeStamp++;
    }
    return 0;
}

static void
viaDMAWaitTimeStamp(XvMCLowLevel * xl, CARD32 timeStamp, int doSleep)
{
    struct timeval now, then;
    struct timezone here;
    struct timespec sleep, rem;

    if (xl->use_agp && (xl->lastReadTimeStamp - timeStamp > (1 << 23))) {
	sleep.tv_nsec = 1;
	sleep.tv_sec = 0;
	here.tz_minuteswest = 0;
	here.tz_dsttime = 0;
	gettimeofday(&then, &here);

	while (((xl->lastReadTimeStamp = *xl->tsP) - timeStamp) > (1 << 23)) {
	    gettimeofday(&now, &here);
	    if (timeDiff(&now, &then) > VIA_DMAWAITTIMEOUT) {
		if (((xl->lastReadTimeStamp =
			    *xl->tsP) - timeStamp) > (1 << 23)) {
		    xl->errors |= LL_DMA_TIMEDOUT;
		    break;
		}
	    }
	    if (doSleep)
		nanosleep(&sleep, &rem);
	}
    }
}

static int
viaDMAInitTimeStamp(XvMCLowLevel * xl)
{
    int ret = 0;

    if (xl->use_agp) {
	xl->tsMem.context = *(xl->drmcontext);
	xl->tsMem.size = 64;
	xl->tsMem.type = VIA_MEM_VIDEO;
	if ((ret = drmCommandWriteRead(xl->fd, DRM_VIA_ALLOCMEM,
		    &xl->tsMem, sizeof(xl->tsMem))) < 0)
	    return ret;
	if (xl->tsMem.size != 64)
	    return -1;
	xl->tsOffset = (xl->tsMem.offset + 31) & ~31;
	xl->tsP = (CARD32 *) xl->fbAddress + (xl->tsOffset >> 2);
	xl->curTimeStamp = 1;
	*xl->tsP = 0;
    }
    return 0;
}

static int
viaDMACleanupTimeStamp(XvMCLowLevel * xl)
{

    if (!(xl->tsMem.size) || !xl->use_agp)
	return 0;
    return drmCommandWrite(xl->fd, DRM_VIA_FREEMEM, &xl->tsMem,
	sizeof(xl->tsMem));
}

static CARD32
viaMpegGetStatus(XvMCLowLevel * xl)
{
    return MPEGIN(xl, 0x54);
}

static int
viaMpegIsBusy(XvMCLowLevel * xl, CARD32 mask, CARD32 idle)
{
    CARD32 tmp = viaMpegGetStatus(xl);

    /*
     * Error detected.
     * FIXME: Are errors really shown when error concealment is on?
     */

    if (tmp & 0x70)
	return 0;

    return (tmp & mask) != idle;
}

static void
syncDMA(XvMCLowLevel * xl, unsigned int doSleep)
{

    /*
     * Ideally, we'd like to have an interrupt wait here, but, according to second hand
     * information, the hardware does not support this, although earlier S3 chips do that.
     * It is therefore not implemented into the DRM, and we'll do a user space wait here.
     */

    struct timeval now, then;
    struct timezone here;
    struct timespec sleep, rem;

    sleep.tv_nsec = 1;
    sleep.tv_sec = 0;
    here.tz_minuteswest = 0;
    here.tz_dsttime = 0;
    gettimeofday(&then, &here);
    while (!(REGIN(xl, VIA_REG_STATUS) & VIA_VR_QUEUE_BUSY)) {
	gettimeofday(&now, &here);
	if (timeDiff(&now, &then) > VIA_DMAWAITTIMEOUT) {
	    if (!(REGIN(xl, VIA_REG_STATUS) & VIA_VR_QUEUE_BUSY)) {
		xl->errors |= LL_DMA_TIMEDOUT;
		break;
	    }
	}
	if (doSleep)
	    nanosleep(&sleep, &rem);
    }
    while (REGIN(xl, VIA_REG_STATUS) & VIA_CMD_RGTR_BUSY) {
	gettimeofday(&now, &here);
	if (timeDiff(&now, &then) > VIA_DMAWAITTIMEOUT) {
	    if (REGIN(xl, VIA_REG_STATUS) & VIA_CMD_RGTR_BUSY) {
		xl->errors |= LL_DMA_TIMEDOUT;
		break;
	    }
	}
	if (doSleep)
	    nanosleep(&sleep, &rem);
    }
}

#ifdef HQV_USE_IRQ
static void
syncVideo(XvMCLowLevel * xl, unsigned int doSleep)
{
    int proReg = REG_HQV1_INDEX;

    /*
     * Wait for HQV completion using completion interrupt. Nothing strange here.
     * Note that the interrupt handler clears the HQV_FLIP_STATUS bit, so we 
     * can't wait on that one.
     */

    if ((VIDIN(xl, HQV_CONTROL | proReg) & (HQV_SW_FLIP | HQV_SUBPIC_FLIP))) {
	drm_via_irqwait_t irqw;

	irqw.request.irq = 1;
	irqw.request.type = VIA_IRQ_ABSOLUTE;
	if (drmCommandWriteRead(xl->fd, DRM_VIA_WAIT_IRQ, &irqw,
		sizeof(irqw)) < 0)
	    xl->errors |= LL_VIDEO_TIMEDOUT;
    }
}
#else
static void
syncVideo(XvMCLowLevel * xl, unsigned int doSleep)
{
    /*
     * Wait for HQV completion. Nothing strange here. We assume that the HQV
     * Handles syncing to the V1 / V3 engines by itself. It should be safe to
     * always wait for SUBPIC_FLIP completion although subpictures are not
     * always used.
     */

    struct timeval now, then;
    struct timezone here;
    struct timespec sleep, rem;

    int proReg = REG_HQV1_INDEX;

    sleep.tv_nsec = 1;
    sleep.tv_sec = 0;
    here.tz_minuteswest = 0;
    here.tz_dsttime = 0;
    gettimeofday(&then, &here);
    while ((VIDIN(xl,
		HQV_CONTROL | proReg) & (HQV_SW_FLIP | HQV_SUBPIC_FLIP))) {
	gettimeofday(&now, &here);
	if (timeDiff(&now, &then) > VIA_SYNCWAITTIMEOUT) {
	    if ((VIDIN(xl,
			HQV_CONTROL | proReg) & (HQV_SW_FLIP |
			HQV_SUBPIC_FLIP))) {
		xl->errors |= LL_VIDEO_TIMEDOUT;
		break;
	    }
	}
	if (doSleep)
	    nanosleep(&sleep, &rem);
    }
}
#endif

static void
syncAccel(XvMCLowLevel * xl, unsigned int mode, unsigned int doSleep)
{
    struct timeval now, then;
    struct timezone here;
    struct timespec sleep, rem;
    CARD32 mask = ((mode & LL_MODE_2D) ? VIA_2D_ENG_BUSY : 0) |
	((mode & LL_MODE_3D) ? VIA_3D_ENG_BUSY : 0);

    sleep.tv_nsec = 1;
    sleep.tv_sec = 0;
    here.tz_minuteswest = 0;
    here.tz_dsttime = 0;
    gettimeofday(&then, &here);
    while (REGIN(xl, VIA_REG_STATUS) & mask) {
	gettimeofday(&now, &here);
	if (timeDiff(&now, &then) > VIA_SYNCWAITTIMEOUT) {
	    if (REGIN(xl, VIA_REG_STATUS) & mask) {
		xl->errors |= LL_ACCEL_TIMEDOUT;
		break;
	    }
	}
	if (doSleep)
	    nanosleep(&sleep, &rem);
    }
}

static void
syncMpeg(XvMCLowLevel * xl, unsigned int mode, unsigned int doSleep)
{
    /*
     * Ideally, we'd like to have an interrupt wait here, but from information from VIA
     * at least the MPEG completion interrupt is broken on the CLE266, which was
     * discovered during validation of the chip.
     */

    struct timeval now, then;
    struct timezone here;
    struct timespec sleep, rem;
    CARD32 busyMask = 0;
    CARD32 idleVal = 0;
    CARD32 ret;

    sleep.tv_nsec = 1;
    sleep.tv_sec = 0;
    here.tz_minuteswest = 0;
    here.tz_dsttime = 0;
    gettimeofday(&then, &here);
    if (mode & LL_MODE_DECODER_SLICE) {
	busyMask = VIA_SLICEBUSYMASK;
	idleVal = VIA_SLICEIDLEVAL;
    }
    if (mode & LL_MODE_DECODER_IDLE) {
	busyMask |= VIA_BUSYMASK;
	idleVal = VIA_IDLEVAL;
    }
    while (viaMpegIsBusy(xl, busyMask, idleVal)) {
	gettimeofday(&now, &here);
	if (timeDiff(&now, &then) > VIA_XVMC_DECODERTIMEOUT) {
	    if (viaMpegIsBusy(xl, busyMask, idleVal)) {
		xl->errors |= LL_DECODER_TIMEDOUT;
	    }
	    break;
	}
	if (doSleep)
	    nanosleep(&sleep, &rem);
    }

    ret = viaMpegGetStatus(xl);
    if (ret & 0x70) {
	xl->errors |= ((ret & 0x70) >> 3);
    }
    return;
}

static void
pciFlush(ViaCommandBuffer * cb, XvMCLowLevel * xl)
{
    int ret;
    drm_via_cmdbuffer_t b;
    unsigned mode = cb->waitFlags;

    finish_header_agp(cb);
    b.buf = (char *)cb->buf;
    b.size = cb->pos * sizeof(CARD32);
    if (xl->performLocking)
	hwlLock(xl, 0);
    if (((mode == LL_MODE_VIDEO) && (xl->videoBuf == &xl->agpBuf)) ||
	((mode != LL_MODE_VIDEO) && (mode != 0)))
	syncDMA(xl, 0);
    if ((mode & LL_MODE_2D) || (mode & LL_MODE_3D)) {
	syncAccel(xl, mode, 0);
    }
    if (mode & LL_MODE_VIDEO) {
	syncVideo(xl, 1);
    }
    if (mode & (LL_MODE_DECODER_SLICE | LL_MODE_DECODER_IDLE)) {
	syncMpeg(xl, mode, 0);
    }
    ret = drmCommandWrite(xl->fd, DRM_VIA_PCICMD, &b, sizeof(b));
    if (xl->performLocking)
	hwlUnlock(xl, 0);
    if (ret) {
	xl->errors |= LL_PCI_COMMAND_ERR;
    }
    cb->pos = 0;
    cb->waitFlags = 0;
}

static void
agpFlush(ViaCommandBuffer * cb, XvMCLowLevel * xl)
{
    drm_via_cmdbuffer_t b;
    int ret;
    int i;

    finish_header_agp(cb);
    if (xl->use_agp) {
	b.buf = (char *)cb->buf;
	b.size = cb->pos * sizeof(CARD32);
	if (xl->agpSync) {
	    syncXvMCLowLevel(xl, LL_MODE_DECODER_IDLE, 1,
		xl->agpSyncTimeStamp);
	    xl->agpSync = 0;
	}
	if (xl->performLocking)
	    hwlLock(xl, 0);
	do {
	    ret = drmCommandWrite(xl->fd, DRM_VIA_CMDBUFFER, &b, sizeof(b));
	} while (-EAGAIN == ret);
	if (xl->performLocking)
	    hwlUnlock(xl, 0);

	if (ret) {
	    xl->errors |= LL_AGP_COMMAND_ERR;
	    for (i = 0; i < cb->pos; i += 2) {
		printf("0x%x, 0x%x\n", (unsigned)cb->buf[i],
		    (unsigned)cb->buf[i + 1]);
	    }
	    exit(-1);
	} else {
	    cb->pos = 0;
	}
	cb->waitFlags &= LL_MODE_VIDEO;	/* FIXME: Check this! */
    } else {
	unsigned mode = cb->waitFlags;

	b.buf = (char *)cb->buf;
	b.size = cb->pos * sizeof(CARD32);
	if (xl->performLocking)
	    hwlLock(xl, 0);
	if (((mode == LL_MODE_VIDEO) && (cb == &xl->agpBuf)) ||
	    ((mode != LL_MODE_VIDEO) && (mode != 0)))
	    syncDMA(xl, 0);
	if ((mode & LL_MODE_2D) || (mode & LL_MODE_3D))
	    syncAccel(xl, mode, 0);
	if (mode & LL_MODE_VIDEO)
	    syncVideo(xl, 1);
	if (mode & (LL_MODE_DECODER_SLICE | LL_MODE_DECODER_IDLE))
	    syncMpeg(xl, mode, 0);
	ret = drmCommandWrite(xl->fd, DRM_VIA_PCICMD, &b, sizeof(b));
	if (xl->performLocking)
	    hwlUnlock(xl, 0);
	if (ret) {
	    xl->errors |= LL_PCI_COMMAND_ERR;
	}
	cb->pos = 0;
	cb->waitFlags = 0;
    }
}

#if 0				       /* Needs debugging */
static void
uploadHQVDeinterlace(XvMCLowLevel * xl, unsigned offset, HQVRegister * shadow,
    CARD32 cur_offset, CARD32 prev_offset, CARD32 stride,
    Bool top_field_first, CARD32 height)
{
    CARD32 tmp;
    ViaCommandBuffer *cb = &xl->agpBuf;

    BEGIN_HEADER6_DATA(cb, xl, 9);
    tmp = GETHQVSHADOW(shadow, 0x3F8);
    tmp &= ~(3 << 30);
    tmp |= (1 << 30);
    OUT_RING_QW_AGP(cb, 0x3F8 + offset, tmp);
    OUT_RING_QW_AGP(cb, 0x3D4 + offset, prev_offset +
	((top_field_first) ? stride : 0));
    OUT_RING_QW_AGP(cb, 0x3D8 + offset, prev_offset + stride * height);
    tmp &= ~(3 << 30);
    tmp |= (2 << 30);
    OUT_RING_QW_AGP(cb, 0x3F8 + offset, tmp);
    OUT_RING_QW_AGP(cb, 0x3D4 + offset, cur_offset +
	((top_field_first) ? 0 : stride));
    OUT_RING_QW_AGP(cb, 0x3D8 + offset, cur_offset + stride * height);
    tmp |= (3 << 30);
    OUT_RING_QW_AGP(cb, 0x3F8 + offset, tmp);
    OUT_RING_QW_AGP(cb, 0x3D4 + offset, cur_offset +
	((top_field_first) ? stride : 0));
    OUT_RING_QW_AGP(cb, 0x3D8 + offset, cur_offset + stride * height);
}

#endif

static void
uploadHQVShadow(XvMCLowLevel * xl, unsigned offset, HQVRegister * shadow,
    Bool flip)
{
    int i;
    CARD32 tmp;
    ViaCommandBuffer *cb = xl->videoBuf;

    BEGIN_HEADER6_DATA(cb, xl, HQV_SHADOW_SIZE);
    WAITFLAGS(cb, LL_MODE_VIDEO);

    if (shadow[0].set)
	OUT_RING_QW_AGP(cb, 0x3CC + offset, 0);

    for (i = 2; i < HQV_SHADOW_SIZE; ++i) {
	if (shadow[i].set) {
	    OUT_RING_QW_AGP(cb, offset + HQV_SHADOW_BASE + (i << 2),
		shadow[i].data);
	    shadow[i].set = FALSE;
	}
    }

    /*
     * Finally the control register for flip.
     */

    if (flip) {
	tmp = GETHQVSHADOW(shadow, 0x3D0);
	OUT_RING_QW_AGP(cb, offset + HQV_CONTROL,
	    HQV_ENABLE | HQV_GEN_IRQ | HQV_SUBPIC_FLIP | HQV_SW_FLIP | tmp);
    }
    shadow[0].set = FALSE;
    shadow[1].set = FALSE;
}

unsigned
flushXvMCLowLevel(void *xlp)
{
    unsigned errors;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    if (xl->pciBuf.pos)
	pciFlush(&xl->pciBuf, xl);
    if (xl->agpBuf.pos)
	agpFlush(&xl->agpBuf, xl);
    errors = xl->errors;
    if (errors)
	printf("Error 0x%x\n", errors);
    xl->errors = 0;
    return errors;
}

void
flushPCIXvMCLowLevel(void *xlp)
{
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    if (xl->pciBuf.pos)
	pciFlush(&xl->pciBuf, xl);
    if ((!xl->use_agp && xl->agpBuf.pos))
	agpFlush(&xl->agpBuf, xl);
}

void
viaMpegSetSurfaceStride(void *xlp, ViaXvMCContext * ctx)
{
    CARD32 y_stride = ctx->yStride;
    CARD32 uv_stride = y_stride >> 1;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;
    ViaCommandBuffer *cb = &xl->agpBuf;

    BEGIN_HEADER6_DATA(cb, xl, 1);
    OUT_RING_QW_AGP(cb, 0xc50, (y_stride >> 3) | ((uv_stride >> 3) << 16));
    WAITFLAGS(cb, LL_MODE_DECODER_IDLE);
}

void
viaVideoSetSWFLipLocked(void *xlp, unsigned yOffs, unsigned uOffs,
    unsigned vOffs, unsigned yStride, unsigned uvStride)
{
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    initHQVShadow(hqvShadow);
    setHQVStartAddress(hqvShadow, yOffs, vOffs, yStride, 0);
    if (xl->videoBuf == &xl->agpBuf)
	syncDMA(xl, 1);
    syncVideo(xl, 1);
    uploadHQVShadow(xl, REG_HQV1_INDEX, hqvShadow, FALSE);
    xl->videoBuf->flushFunc(xl->videoBuf, xl);
}

void
viaVideoSWFlipLocked(void *xlp, unsigned flags, Bool progressiveSequence)
{
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    setHQVDeinterlacing(hqvShadow, flags);
    setHQVDeblocking(hqvShadow,
	((flags & XVMC_FRAME_PICTURE) == XVMC_FRAME_PICTURE), TRUE);
    setHQVTripleBuffer(hqvShadow, TRUE);
    if (xl->videoBuf == &xl->agpBuf)
	syncDMA(xl, 1);
    syncVideo(xl, 1);
    uploadHQVShadow(xl, REG_HQV1_INDEX, hqvShadow, TRUE);
    xl->videoBuf->flushFunc(xl->videoBuf, xl);
}

void
viaMpegSetFB(void *xlp, unsigned i,
    unsigned yOffs, unsigned uOffs, unsigned vOffs)
{
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;
    ViaCommandBuffer *cb = &xl->agpBuf;

    i *= (4 * 2);
    BEGIN_HEADER6_DATA(cb, xl, 2);
    OUT_RING_QW_AGP(cb, 0xc28 + i, yOffs >> 3);
    OUT_RING_QW_AGP(cb, 0xc2c + i, vOffs >> 3);

    WAITFLAGS(cb, LL_MODE_DECODER_IDLE);
}

void
viaMpegBeginPicture(void *xlp, ViaXvMCContext * ctx,
    unsigned width, unsigned height, const XvMCMpegControl * control)
{

    unsigned j, mb_width, mb_height;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;
    ViaCommandBuffer *cb = &xl->agpBuf;

    mb_width = (width + 15) >> 4;

    mb_height =
	((control->mpeg_coding == XVMC_MPEG_2) &&
	(control->flags & XVMC_PROGRESSIVE_SEQUENCE)) ?
	2 * ((height + 31) >> 5) : (((height + 15) >> 4));

    BEGIN_HEADER6_DATA(cb, xl, 72);
    WAITFLAGS(cb, LL_MODE_DECODER_IDLE);

    OUT_RING_QW_AGP(cb, 0xc00,
	((control->picture_structure & XVMC_FRAME_PICTURE) << 2) |
	((control->picture_coding_type & 3) << 4) |
	((control->flags & XVMC_ALTERNATE_SCAN) ? (1 << 6) : 0));

    if (!(ctx->intraLoaded)) {
	OUT_RING_QW_AGP(cb, 0xc5c, 0);
	for (j = 0; j < 64; j += 4) {
	    OUT_RING_QW_AGP(cb, 0xc60,
		ctx->intra_quantiser_matrix[j] |
		(ctx->intra_quantiser_matrix[j + 1] << 8) |
		(ctx->intra_quantiser_matrix[j + 2] << 16) |
		(ctx->intra_quantiser_matrix[j + 3] << 24));
	}
	ctx->intraLoaded = 1;
    }

    if (!(ctx->nonIntraLoaded)) {
	OUT_RING_QW_AGP(cb, 0xc5c, 1);
	for (j = 0; j < 64; j += 4) {
	    OUT_RING_QW_AGP(cb, 0xc60,
		ctx->non_intra_quantiser_matrix[j] |
		(ctx->non_intra_quantiser_matrix[j + 1] << 8) |
		(ctx->non_intra_quantiser_matrix[j + 2] << 16) |
		(ctx->non_intra_quantiser_matrix[j + 3] << 24));
	}
	ctx->nonIntraLoaded = 1;
    }

    if (!(ctx->chromaIntraLoaded)) {
	OUT_RING_QW_AGP(cb, 0xc5c, 2);
	for (j = 0; j < 64; j += 4) {
	    OUT_RING_QW_AGP(cb, 0xc60,
		ctx->chroma_intra_quantiser_matrix[j] |
		(ctx->chroma_intra_quantiser_matrix[j + 1] << 8) |
		(ctx->chroma_intra_quantiser_matrix[j + 2] << 16) |
		(ctx->chroma_intra_quantiser_matrix[j + 3] << 24));
	}
	ctx->chromaIntraLoaded = 1;
    }

    if (!(ctx->chromaNonIntraLoaded)) {
	OUT_RING_QW_AGP(cb, 0xc5c, 3);
	for (j = 0; j < 64; j += 4) {
	    OUT_RING_QW_AGP(cb, 0xc60,
		ctx->chroma_non_intra_quantiser_matrix[j] |
		(ctx->chroma_non_intra_quantiser_matrix[j + 1] << 8) |
		(ctx->chroma_non_intra_quantiser_matrix[j + 2] << 16) |
		(ctx->chroma_non_intra_quantiser_matrix[j + 3] << 24));
	}
	ctx->chromaNonIntraLoaded = 1;
    }

    OUT_RING_QW_AGP(cb, 0xc90,
	((mb_width * mb_height) & 0x3fff) |
	((control->flags & XVMC_PRED_DCT_FRAME) ? (1 << 14) : 0) |
	((control->flags & XVMC_TOP_FIELD_FIRST) ? (1 << 15) : 0) |
	((control->mpeg_coding == XVMC_MPEG_2) ? (1 << 16) : 0) |
	((mb_width & 0xff) << 18));

    OUT_RING_QW_AGP(cb, 0xc94,
	((control->flags & XVMC_CONCEALMENT_MOTION_VECTORS) ? 1 : 0) |
	((control->flags & XVMC_Q_SCALE_TYPE) ? 2 : 0) |
	((control->intra_dc_precision & 3) << 2) |
	(((1 + 0x100000 / mb_width) & 0xfffff) << 4) |
	((control->flags & XVMC_INTRA_VLC_FORMAT) ? (1 << 24) : 0));

    OUT_RING_QW_AGP(cb, 0xc98,
	(((control->FHMV_range) & 0xf) << 0) |
	(((control->FVMV_range) & 0xf) << 4) |
	(((control->BHMV_range) & 0xf) << 8) |
	(((control->BVMV_range) & 0xf) << 12) |
	((control->flags & XVMC_SECOND_FIELD) ? (1 << 20) : 0) |
	(0x0a6 << 16));

}

void
viaMpegReset(void *xlp)
{
    int i, j;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;
    ViaCommandBuffer *cb = &xl->agpBuf;

    BEGIN_HEADER6_DATA(cb, xl, 99);
    WAITFLAGS(cb, LL_MODE_DECODER_IDLE);

    OUT_RING_QW_AGP(cb, 0xcf0, 0);

    for (i = 0; i < 6; i++) {
	OUT_RING_QW_AGP(cb, 0xcc0, 0);
	OUT_RING_QW_AGP(cb, 0xc0c, 0x43 | 0x20);
	for (j = 0xc10; j < 0xc20; j += 4)
	    OUT_RING_QW_AGP(cb, j, 0);
    }

    OUT_RING_QW_AGP(cb, 0xc0c, 0x1c3);
    for (j = 0xc10; j < 0xc20; j += 4)
	OUT_RING_QW_AGP(cb, j, 0);

    for (i = 0; i < 19; i++)
	OUT_RING_QW_AGP(cb, 0xc08, 0);

    OUT_RING_QW_AGP(cb, 0xc98, 0x400000);

    for (i = 0; i < 6; i++) {
	OUT_RING_QW_AGP(cb, 0xcc0, 0);
	OUT_RING_QW_AGP(cb, 0xc0c, 0x1c3 | 0x20);
	for (j = 0xc10; j < 0xc20; j += 4)
	    OUT_RING_QW_AGP(cb, j, 0);
    }
    OUT_RING_QW_AGP(cb, 0xcf0, 0);

}

void
viaMpegWriteSlice(void *xlp, CARD8 * slice, int nBytes, CARD32 sCode)
{
    int i, n, r;
    CARD32 *buf;
    int count;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;
    ViaCommandBuffer *cb = &xl->agpBuf;

    if (xl->errors & (LL_DECODER_TIMEDOUT |
	    LL_IDCT_FIFO_ERROR | LL_SLICE_FIFO_ERROR | LL_SLICE_FAULT))
	return;

    n = nBytes >> 2;
    if (sCode)
	nBytes += 4;
    r = nBytes & 3;
    buf = (CARD32 *) slice;

    if (r)
	nBytes += 4 - r;

    nBytes += 8;

    BEGIN_HEADER6_DATA(cb, xl, 2);
    WAITFLAGS(cb, LL_MODE_DECODER_IDLE);
    OUT_RING_QW_AGP(cb, 0xc9c, nBytes);

    if (sCode)
	OUT_RING_QW_AGP(cb, 0xca0, sCode);

    i = 0;
    count = 0;

    do {
	count += (LL_AGP_CMDBUF_SIZE - 20);
	count = (count > n) ? n : count;
	BEGIN_HEADER5_DATA(cb, xl, (count - i), 0xca0);

	for (; i < count; i++) {
	    OUT_RING_AGP(cb, *buf++);
	}
	finish_header_agp(cb);
    } while (i < n);

    BEGIN_HEADER5_DATA(cb, xl, 3, 0xca0);

    if (r) {
	OUT_RING_AGP(cb, *buf & ((1 << (r << 3)) - 1));
    }
    OUT_RING_AGP(cb, 0);
    OUT_RING_AGP(cb, 0);
    finish_header_agp(cb);
}

void
viaVideoSubPictureOffLocked(void *xlp)
{

    CARD32 stride;
    int proReg = REG_HQV1_INDEX;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;
    ViaCommandBuffer *cb = xl->videoBuf;

    if (xl->videoBuf == &xl->agpBuf)
	syncDMA(xl, 1);
    stride = VIDIN(xl, proReg | SUBP_CONTROL_STRIDE);
    WAITFLAGS(cb, LL_MODE_VIDEO);
    BEGIN_HEADER6_DATA(cb, xl, 1);
    OUT_RING_QW_AGP(cb, proReg | SUBP_CONTROL_STRIDE,
	stride & ~SUBP_HQV_ENABLE);
}

void
viaVideoSubPictureLocked(void *xlp, ViaXvMCSubPicture * pViaSubPic)
{

    unsigned i;
    CARD32 cWord;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;
    int proReg = REG_HQV1_INDEX;
    ViaCommandBuffer *cb = xl->videoBuf;

    if (xl->videoBuf == &xl->agpBuf)
	syncDMA(xl, 1);
    WAITFLAGS(cb, LL_MODE_VIDEO);
    BEGIN_HEADER6_DATA(cb, xl, VIA_SUBPIC_PALETTE_SIZE + 2);
    for (i = 0; i < VIA_SUBPIC_PALETTE_SIZE; ++i) {
	OUT_RING_QW_AGP(cb, proReg | RAM_TABLE_CONTROL,
	    pViaSubPic->palette[i]);
    }

    cWord = (pViaSubPic->stride & SUBP_STRIDE_MASK) | SUBP_HQV_ENABLE;
    cWord |= (pViaSubPic->ia44) ? SUBP_IA44 : SUBP_AI44;
    OUT_RING_QW_AGP(cb, proReg | SUBP_STARTADDR, pViaSubPic->offset);
    OUT_RING_QW_AGP(cb, proReg | SUBP_CONTROL_STRIDE, cWord);
}

void
viaBlit(void *xlp, unsigned bpp, unsigned srcBase,
    unsigned srcPitch, unsigned dstBase, unsigned dstPitch,
    unsigned w, unsigned h, int xdir, int ydir, unsigned blitMode,
    unsigned color)
{

    CARD32 dwGEMode = 0, srcY = 0, srcX, dstY = 0, dstX;
    CARD32 cmd;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;
    ViaCommandBuffer *cb = &xl->agpBuf;

    if (!w || !h)
	return;

    finish_header_agp(cb);

    switch (bpp) {
    case 16:
	dwGEMode |= VIA_GEM_16bpp;
	break;
    case 32:
	dwGEMode |= VIA_GEM_32bpp;
	break;
    default:
	dwGEMode |= VIA_GEM_8bpp;
	break;
    }

    srcX = srcBase & 31;
    dstX = dstBase & 31;
    switch (bpp) {
    case 16:
	dwGEMode |= VIA_GEM_16bpp;
	srcX >>= 2;
	dstX >>= 2;
	break;
    case 32:
	dwGEMode |= VIA_GEM_32bpp;
	srcX >>= 4;
	dstX >>= 4;
	break;
    default:
	dwGEMode |= VIA_GEM_8bpp;
	break;
    }

    BEGIN_RING_AGP(cb, xl, 20);
    WAITFLAGS(cb, LL_MODE_2D);

    OUT_RING_QW_AGP(cb, H1_ADDR(VIA_REG_GEMODE), dwGEMode);
    cmd = 0;

    if (xdir < 0) {
	cmd |= VIA_GEC_DECX;
	srcX += (w - 1);
	dstX += (w - 1);
    }
    if (ydir < 0) {
	cmd |= VIA_GEC_DECY;
	srcY += (h - 1);
	dstY += (h - 1);
    }

    switch (blitMode) {
    case VIABLIT_TRANSCOPY:
	OUT_RING_QW_AGP(cb, H1_ADDR(VIA_REG_SRCCOLORKEY), color);
	OUT_RING_QW_AGP(cb, H1_ADDR(VIA_REG_KEYCONTROL), 0x4000);
	cmd |= VIA_GEC_BLT | (VIA_BLIT_COPY << 24);
	break;
    case VIABLIT_FILL:
	OUT_RING_QW_AGP(cb, H1_ADDR(VIA_REG_FGCOLOR), color);
	cmd |= VIA_GEC_BLT | VIA_GEC_FIXCOLOR_PAT | (VIA_BLIT_FILL << 24);
	break;
    default:
	OUT_RING_QW_AGP(cb, H1_ADDR(VIA_REG_KEYCONTROL), 0x0);
	cmd |= VIA_GEC_BLT | (VIA_BLIT_COPY << 24);
    }

    OUT_RING_QW_AGP(cb, H1_ADDR(VIA_REG_SRCBASE), (srcBase & ~31) >> 3);
    OUT_RING_QW_AGP(cb, H1_ADDR(VIA_REG_DSTBASE), (dstBase & ~31) >> 3);
    OUT_RING_QW_AGP(cb, H1_ADDR(VIA_REG_PITCH), VIA_PITCH_ENABLE |
	(srcPitch >> 3) | (((dstPitch) >> 3) << 16));
    OUT_RING_QW_AGP(cb, H1_ADDR(VIA_REG_SRCPOS), ((srcY << 16) | srcX));
    OUT_RING_QW_AGP(cb, H1_ADDR(VIA_REG_DSTPOS), ((dstY << 16) | dstX));
    OUT_RING_QW_AGP(cb, H1_ADDR(VIA_REG_DIMENSION),
	(((h - 1) << 16) | (w - 1)));
    OUT_RING_QW_AGP(cb, H1_ADDR(VIA_REG_GECMD), cmd);
}

unsigned
syncXvMCLowLevel(void *xlp, unsigned int mode, unsigned int doSleep,
    CARD32 timeStamp)
{
    unsigned errors;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    if (mode == 0) {
	errors = xl->errors;
	xl->errors = 0;
	return errors;
    }

    if ((mode & (LL_MODE_VIDEO | LL_MODE_3D)) || !xl->use_agp) {
	if (xl->performLocking)
	    hwlLock(xl, 0);
	if ((xl->videoBuf == &xl->agpBuf) || (mode != LL_MODE_VIDEO))
	    syncDMA(xl, doSleep);
	if (mode & LL_MODE_3D)
	    syncAccel(xl, mode, doSleep);
	if (mode & LL_MODE_VIDEO)
	    syncVideo(xl, doSleep);
	if (xl->performLocking)
	    hwlUnlock(xl, 0);
    } else {
	viaDMAWaitTimeStamp(xl, timeStamp, doSleep);
    }

    if (mode & (LL_MODE_DECODER_SLICE | LL_MODE_DECODER_IDLE))
	syncMpeg(xl, mode, doSleep);

    errors = xl->errors;
    xl->errors = 0;

    return errors;
}

static int
updateLowLevelBuf(XvMCLowLevel * xl, LowLevelBuffer * buf,
    unsigned width, unsigned height)
{
    unsigned stride, size;
    drm_via_mem_t *mem = &buf->mem;
    int ret;

    stride = (width + 31) & ~31;
    size = stride * height + (xl->fbDepth >> 3);

    if (size != mem->size) {
	if (mem->size)
	    drmCommandWrite(xl->fd, DRM_VIA_FREEMEM, mem, sizeof(*mem));
	mem->context = *(xl->drmcontext);
	mem->size = size;
	mem->type = VIA_MEM_VIDEO;

	if (((ret = drmCommandWriteRead(xl->fd, DRM_VIA_ALLOCMEM, mem,
			sizeof(*mem))) < 0) || mem->size != size) {
	    mem->size = 0;
	    return -1;
	}
    }

    buf->offset = (mem->offset + 31) & ~31;
    buf->stride = stride;
    buf->height = height;
    return 0;
}

static void
cleanupLowLevelBuf(XvMCLowLevel * xl, LowLevelBuffer * buf)
{
    drm_via_mem_t *mem = &buf->mem;

    if (mem->size)
	drmCommandWrite(xl->fd, DRM_VIA_FREEMEM, mem, sizeof(*mem));
    mem->size = 0;
}

static void *
releaseXvMCLowLevel(XvMCLowLevel * xl)
{
    switch (xl->state) {
    case ll_llBuf:
	cleanupLowLevelBuf(xl, &xl->scale);
    case ll_timeStamp:
	viaDMACleanupTimeStamp(xl);
    case ll_pciBuf:
	free(xl->pciBuf.buf);
    case ll_agpBuf:
	free(xl->agpBuf.buf);
    case ll_init:
	free(xl);
    default:
	;
    }
    return NULL;
}

void *
initXvMCLowLevel(int fd, drm_context_t * ctx,
    drmLockPtr hwLock, drmAddress mmioAddress,
    drmAddress fbAddress, unsigned fbStride, unsigned fbDepth,
    unsigned width, unsigned height, int useAgp, unsigned chipId)
{
    XvMCLowLevel *xl;
    
    if (chipId != PCI_CHIP_VT3259 && chipId != PCI_CHIP_VT3364) {
	fprintf(stderr, "You are using an XvMC driver for the wrong chip.\n");
	fprintf(stderr, "Chipid is 0x%04x.\n", chipId);
	return NULL;
    }

    xl = (XvMCLowLevel *) malloc(sizeof(XvMCLowLevel));
    if (!xl)
	return NULL;
    xl->state = ll_init;

    xl->agpBuf.buf = (CARD32 *) malloc(LL_AGP_CMDBUF_SIZE * sizeof(CARD32));
    if (!xl->agpBuf.buf)
	return releaseXvMCLowLevel(xl);
    xl->state = ll_agpBuf;
    xl->agpBuf.bufSize = LL_AGP_CMDBUF_SIZE;
    xl->agpBuf.flushFunc = &agpFlush;
    xl->agpBuf.pos = 0;
    xl->agpBuf.mode = 0;
    xl->agpBuf.waitFlags = 0;

    xl->pciBuf.buf = (CARD32 *) malloc(LL_PCI_CMDBUF_SIZE * sizeof(CARD32));
    if (!xl->pciBuf.buf)
	return releaseXvMCLowLevel(xl);
    xl->state = ll_pciBuf;
    xl->pciBuf.bufSize = LL_PCI_CMDBUF_SIZE;
    xl->pciBuf.flushFunc = &pciFlush;
    xl->pciBuf.pos = 0;
    xl->pciBuf.mode = 0;
    xl->pciBuf.waitFlags = 0;

    xl->use_agp = useAgp;
    xl->fd = fd;
    xl->drmcontext = ctx;
    xl->hwLock = hwLock;
    xl->mmioAddress = mmioAddress;
    xl->fbAddress = fbAddress;
    xl->fbDepth = fbDepth;
    xl->fbStride = fbStride;
    xl->width = width;
    xl->height = height;
    xl->performLocking = 1;
    xl->errors = 0;
    xl->agpSync = 0;
    xl->chipId = chipId;

    if (viaDMAInitTimeStamp(xl))
	return releaseXvMCLowLevel(xl);
    xl->state = ll_timeStamp;

    xl->scale.mem.size = 0;
    xl->back.mem.size = 0;

    if (updateLowLevelBuf(xl, &xl->scale, width, height))
	return releaseXvMCLowLevel(xl);
    xl->state = ll_llBuf;

#ifdef VIDEO_DMA
    xl->videoBuf = &xl->agpBuf;
#else
    xl->videoBuf = &xl->pciBuf;
#endif

    return xl;
}

void
setLowLevelLocking(void *xlp, int performLocking)
{
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    xl->performLocking = performLocking;
}

void
closeXvMCLowLevel(void *xlp)
{
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    releaseXvMCLowLevel(xl);
}

#if 0				       /* Under development */
static CARD32
computeDownScaling(int dst, int *src)
{
    CARD32 value = 0x800;

    while (*src > dst) {
	*src >>= 1;
	value--;
    }
    return value;
}

static void
computeHQVScaleAndFilter(XvMCLowLevel * xl)
{
    int srcW, srcH;
    const XvMCRegion *src = &xl->sRegion, *back = &xl->dRegion;

    xl->downScaling = FALSE;

    if (back->w < src->w || back->h < src->h) {

	xl->downScaling = TRUE;
	srcW = src->w;
	srcH = src->h;

	xl->downScaleW = (back->w >= srcW) ? 0 :
	    HQV_SCALE_ENABLE | HQV_SCALE_DOWN |
	    (computeDownScaling(back->w, &srcW));

	xl->downScaleH = (back->h >= srcH) ? 0 :
	    HQV_SCALE_ENABLE | HQV_SCALE_DOWN |
	    (computeDownScaling(back->h, &srcH));

    }

    xl->upScaleW =
	(back->w == srcW) ? 0 : (0x800 * srcW / back->w) | HQV_SCALE_ENABLE;
    xl->upScaleH =
	(back->h == srcH) ? 0 : (0x800 * srcH / back->h) | HQV_SCALE_ENABLE;
}

static int
setupBackBuffer(XvMCLowLevel * xl)
{
    return updateLowLevelBuf(xl, &xl->back, xl->dRegion.w, xl->dRegion.h);
}

#endif
