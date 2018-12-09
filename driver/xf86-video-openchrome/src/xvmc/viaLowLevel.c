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
 * Authors: Andreas Robinson 2003. Thomas Hellström 2004.
 */

#include "viaXvMCPriv.h"
#include "viaLowLevel.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

typedef struct
{
    CARD32 agp_buffer[LL_AGP_CMDBUF_SIZE];
    CARD32 pci_buffer[LL_PCI_CMDBUF_SIZE];
    unsigned agp_pos;
    unsigned pci_pos;
    unsigned flip_pos;
    int use_agp;
    int agp_mode;
    int agp_header_start;
    int agp_index;
    int fd;
    drm_context_t *drmcontext;
    drmLockPtr hwLock;
    drmAddress mmioAddress;
    drmAddress fbAddress;
    unsigned fbStride;
    unsigned fbDepth;
    unsigned width;
    unsigned height;
    unsigned curWaitFlags;
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
#define HQV_SRC_STARTADDR_Y     0x3D4
#define HQV_SRC_STARTADDR_U     0x3D8
#define HQV_SRC_STARTADDR_V     0x3DC
#define HQV_MINIFY_DEBLOCK      0x3E8

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

#define H1_ADDR(val) (((val) >> 2) | 0xF0000000)
#define WAITFLAGS(xl, flags)			\
    (xl)->curWaitFlags |= (flags)
#define BEGIN_RING_AGP(xl,size)						\
    do {								\
	if ((xl)->agp_pos > (LL_AGP_CMDBUF_SIZE-(size))) {		\
	    agpFlush(xl);						\
	}								\
    } while(0)
#define OUT_RING_AGP(xl, val)			\
    (xl)->agp_buffer[(xl)->agp_pos++] = (val)
#define OUT_RING_QW_AGP(xl, val1, val2)			\
    do {						\
	(xl)->agp_buffer[(xl)->agp_pos++] = (val1);	\
	(xl)->agp_buffer[(xl)->agp_pos++] = (val2);	\
    } while (0)

#define LL_HW_LOCK(xl)							\
    do {								\
	DRM_LOCK((xl)->fd,(xl)->hwLock,*(xl)->drmcontext,0);		\
    } while(0);
#define LL_HW_UNLOCK(xl)					\
    do {							\
	DRM_UNLOCK((xl)->fd,(xl)->hwLock,*(xl)->drmcontext);	\
    } while(0);

/*
 * We want to have two concurrent types of thread taking the hardware
 * lock simulataneously. One is the video out thread that needs immediate
 * access to flip an image. The other is everything else which may have
 * the lock for quite some time. This is only so the video out thread can
 * sneak in and display an image while other resources are busy.
 */

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

    if (xl->use_agp && (timeStamp > xl->lastReadTimeStamp)) {
	sleep.tv_nsec = 1;
	sleep.tv_sec = 0;
	here.tz_minuteswest = 0;
	here.tz_dsttime = 0;
	gettimeofday(&then, &here);

	while (timeStamp > (xl->lastReadTimeStamp = *xl->tsP)) {
	    gettimeofday(&now, &here);
	    if (timeDiff(&now, &then) > VIA_DMAWAITTIMEOUT) {
		if ((timeStamp > (xl->lastReadTimeStamp = *xl->tsP))) {
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
	if ((ret = drmCommandWriteRead(xl->fd, DRM_VIA_ALLOCMEM, &xl->tsMem,
		sizeof(xl->tsMem))) < 0)
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

    sleep.tv_nsec = 1;
    sleep.tv_sec = 0;
    here.tz_minuteswest = 0;
    here.tz_dsttime = 0;
    gettimeofday(&then, &here);
    while (VIDIN(xl, HQV_CONTROL) & (HQV_SW_FLIP | HQV_SUBPIC_FLIP)) {
	gettimeofday(&now, &here);
	if (timeDiff(&now, &then) > VIA_SYNCWAITTIMEOUT) {
	    if (VIDIN(xl, HQV_CONTROL) & (HQV_SW_FLIP | HQV_SUBPIC_FLIP)) {
		xl->errors |= LL_VIDEO_TIMEDOUT;
		break;
	    }
	}
	if (doSleep)
	    nanosleep(&sleep, &rem);
    }
}

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
pciFlush(XvMCLowLevel * xl)
{
    int ret;
    drm_via_cmdbuffer_t b;
    unsigned mode = xl->curWaitFlags;

    b.buf = (char *)xl->pci_buffer;
    b.size = xl->pci_pos * sizeof(CARD32);
    if (xl->performLocking)
	hwlLock(xl, 0);
    if ((mode != LL_MODE_VIDEO) && (mode != 0))
	syncDMA(xl, 0);
    if ((mode & LL_MODE_2D) || (mode & LL_MODE_3D))
	syncAccel(xl, mode, 0);
    if (mode & LL_MODE_VIDEO)
	syncVideo(xl, 0);
    if (mode & (LL_MODE_DECODER_SLICE | LL_MODE_DECODER_IDLE))
	syncMpeg(xl, mode, 0);
    ret = drmCommandWrite(xl->fd, DRM_VIA_PCICMD, &b, sizeof(b));
    if (xl->performLocking)
	hwlUnlock(xl, 0);
    if (ret) {
	xl->errors |= LL_PCI_COMMAND_ERR;
    }
    xl->pci_pos = 0;
    xl->curWaitFlags = 0;
}

static void
agpFlush(XvMCLowLevel * xl)
{
    drm_via_cmdbuffer_t b;
    int ret;

    if (xl->use_agp) {
	b.buf = (char *)xl->agp_buffer;
	b.size = xl->agp_pos * sizeof(CARD32);
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
	} else {
	    xl->agp_pos = 0;
	}
	xl->curWaitFlags &= LL_MODE_VIDEO;
    } else {
	unsigned mode = xl->curWaitFlags;

	b.buf = (char *)xl->agp_buffer;
	b.size = xl->agp_pos * sizeof(CARD32);
	if (xl->performLocking)
	    hwlLock(xl, 0);
	if ((mode != LL_MODE_VIDEO) && (mode != 0))
	    syncDMA(xl, 0);
	if ((mode & LL_MODE_2D) || (mode & LL_MODE_3D))
	    syncAccel(xl, mode, 0);
	if (mode & LL_MODE_VIDEO)
	    syncVideo(xl, 0);
	if (mode & (LL_MODE_DECODER_SLICE | LL_MODE_DECODER_IDLE))
	    syncMpeg(xl, mode, 0);
	ret = drmCommandWrite(xl->fd, DRM_VIA_PCICMD, &b, sizeof(b));
	if (xl->performLocking)
	    hwlUnlock(xl, 0);
	if (ret) {
	    xl->errors |= LL_PCI_COMMAND_ERR;
	}
	xl->agp_pos = 0;
	xl->curWaitFlags = 0;
    }
}

unsigned
flushXvMCLowLevel(void *xlp)
{
    unsigned errors;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    if (xl->pci_pos)
	pciFlush(xl);
    if (xl->agp_pos)
	agpFlush(xl);
    errors = xl->errors;
    xl->errors = 0;
    return errors;
}

void
flushPCIXvMCLowLevel(void *xlp)
{
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    if (xl->pci_pos)
	pciFlush(xl);
    if (!xl->use_agp && xl->agp_pos)
	agpFlush(xl);
}

__inline static void
pciCommand(XvMCLowLevel * xl, unsigned offset, unsigned value, unsigned flags)
{
    if (xl->pci_pos > (LL_PCI_CMDBUF_SIZE - 2))
	pciFlush(xl);
    if (flags)
	xl->curWaitFlags |= flags;
    xl->pci_buffer[xl->pci_pos++] = (offset >> 2) | 0xF0000000;
    xl->pci_buffer[xl->pci_pos++] = value;
}

void
viaMpegSetSurfaceStride(void *xlp, ViaXvMCContext * ctx)
{
    CARD32 y_stride = ctx->yStride;
    CARD32 uv_stride = y_stride >> 1;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    BEGIN_RING_AGP(xl, 2);
    OUT_RING_QW_AGP(xl, H1_ADDR(0xc50),
	(y_stride >> 3) | ((uv_stride >> 3) << 16));
    WAITFLAGS(xl, LL_MODE_DECODER_IDLE);
}

void
viaVideoSetSWFLipLocked(void *xlp, unsigned yOffs, unsigned uOffs,
    unsigned vOffs, unsigned yStride, unsigned uvStride)
{
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    pciCommand(xl, HQV_SRC_STARTADDR_Y, yOffs, LL_MODE_VIDEO);
    pciCommand(xl, HQV_SRC_STARTADDR_U, uOffs, 0);
    pciCommand(xl, HQV_SRC_STARTADDR_V, vOffs, 0);
}

void
viaVideoSWFlipLocked(void *xlp, unsigned flags, int progressiveSequence)
{
    CARD32 andWd, orWd;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    andWd = 0;
    orWd = 0;

    if ((flags & XVMC_FRAME_PICTURE) == XVMC_BOTTOM_FIELD) {
	andWd = 0xFFFFFFFFU;
	orWd = HQV_FIELD_UV |
	    HQV_DEINTERLACE |
	    HQV_FIELD_2_FRAME |
	    HQV_FRAME_2_FIELD |
	    HQV_SW_FLIP | HQV_FLIP_ODD | HQV_FLIP_STATUS | HQV_SUBPIC_FLIP;
    } else if ((flags & XVMC_FRAME_PICTURE) == XVMC_TOP_FIELD) {
	andWd = ~HQV_FLIP_ODD;
	orWd = HQV_FIELD_UV |
	    HQV_DEINTERLACE |
	    HQV_FIELD_2_FRAME |
	    HQV_FRAME_2_FIELD |
	    HQV_SW_FLIP | HQV_FLIP_STATUS | HQV_SUBPIC_FLIP;
    } else if ((flags & XVMC_FRAME_PICTURE) == XVMC_FRAME_PICTURE) {
	andWd = ~(HQV_DEINTERLACE |
	    HQV_FRAME_2_FIELD | HQV_FIELD_2_FRAME | HQV_FIELD_UV);
	orWd = HQV_SW_FLIP | HQV_FLIP_STATUS | HQV_SUBPIC_FLIP;
    }
    if (progressiveSequence) {
	andWd &= ~HQV_FIELD_UV;
	orWd &= ~HQV_FIELD_UV;
    }

    pciCommand(xl, HQV_CONTROL, (VIDIN(xl,
		HQV_CONTROL) & andWd) | orWd, 0);
}

void
viaMpegSetFB(void *xlp, unsigned i,
    unsigned yOffs, unsigned uOffs, unsigned vOffs)
{
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    i *= 12;
    BEGIN_RING_AGP(xl, 6);
    OUT_RING_QW_AGP(xl, H1_ADDR(0xc20 + i), yOffs >> 3);
    OUT_RING_QW_AGP(xl, H1_ADDR(0xc24 + i), uOffs >> 3);
    OUT_RING_QW_AGP(xl, H1_ADDR(0xc28 + i), vOffs >> 3);
    WAITFLAGS(xl, LL_MODE_DECODER_IDLE);
}

void
viaMpegBeginPicture(void *xlp, ViaXvMCContext * ctx,
    unsigned width, unsigned height, const XvMCMpegControl * control)
{

    unsigned j, mb_width, mb_height;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    mb_width = (width + 15) >> 4;

    mb_height =
	((control->mpeg_coding == XVMC_MPEG_2) &&
	(control->flags & XVMC_PROGRESSIVE_SEQUENCE)) ?
	2 * ((height + 31) >> 5) : (((height + 15) >> 4));

    BEGIN_RING_AGP(xl, 144);
    WAITFLAGS(xl, LL_MODE_DECODER_IDLE);

    OUT_RING_QW_AGP(xl, H1_ADDR(0xc00),
	((control->picture_structure & XVMC_FRAME_PICTURE) << 2) |
	((control->picture_coding_type & 3) << 4) |
	((control->flags & XVMC_ALTERNATE_SCAN) ? (1 << 6) : 0));

    if (!(ctx->intraLoaded)) {
	OUT_RING_QW_AGP(xl, H1_ADDR(0xc5c), 0);
	for (j = 0; j < 64; j += 4) {
	    OUT_RING_QW_AGP(xl, H1_ADDR(0xc60),
		ctx->intra_quantiser_matrix[j] |
		(ctx->intra_quantiser_matrix[j + 1] << 8) |
		(ctx->intra_quantiser_matrix[j + 2] << 16) |
		(ctx->intra_quantiser_matrix[j + 3] << 24));
	}
	ctx->intraLoaded = 1;
    }

    if (!(ctx->nonIntraLoaded)) {
	OUT_RING_QW_AGP(xl, H1_ADDR(0xc5c), 1);
	for (j = 0; j < 64; j += 4) {
	    OUT_RING_QW_AGP(xl, H1_ADDR(0xc60),
		ctx->non_intra_quantiser_matrix[j] |
		(ctx->non_intra_quantiser_matrix[j + 1] << 8) |
		(ctx->non_intra_quantiser_matrix[j + 2] << 16) |
		(ctx->non_intra_quantiser_matrix[j + 3] << 24));
	}
	ctx->nonIntraLoaded = 1;
    }

    if (!(ctx->chromaIntraLoaded)) {
	OUT_RING_QW_AGP(xl, H1_ADDR(0xc5c), 2);
	for (j = 0; j < 64; j += 4) {
	    OUT_RING_QW_AGP(xl, H1_ADDR(0xc60),
		ctx->chroma_intra_quantiser_matrix[j] |
		(ctx->chroma_intra_quantiser_matrix[j + 1] << 8) |
		(ctx->chroma_intra_quantiser_matrix[j + 2] << 16) |
		(ctx->chroma_intra_quantiser_matrix[j + 3] << 24));
	}
	ctx->chromaIntraLoaded = 1;
    }

    if (!(ctx->chromaNonIntraLoaded)) {
	OUT_RING_QW_AGP(xl, H1_ADDR(0xc5c), 3);
	for (j = 0; j < 64; j += 4) {
	    OUT_RING_QW_AGP(xl, H1_ADDR(0xc60),
		ctx->chroma_non_intra_quantiser_matrix[j] |
		(ctx->chroma_non_intra_quantiser_matrix[j + 1] << 8) |
		(ctx->chroma_non_intra_quantiser_matrix[j + 2] << 16) |
		(ctx->chroma_non_intra_quantiser_matrix[j + 3] << 24));
	}
	ctx->chromaNonIntraLoaded = 1;
    }

    OUT_RING_QW_AGP(xl, H1_ADDR(0xc90),
	((mb_width * mb_height) & 0x3fff) |
	((control->flags & XVMC_PRED_DCT_FRAME) ? (1 << 14) : 0) |
	((control->flags & XVMC_TOP_FIELD_FIRST) ? (1 << 15) : 0) |
	((control->mpeg_coding == XVMC_MPEG_2) ? (1 << 16) : 0) |
	((mb_width & 0xff) << 18));

    OUT_RING_QW_AGP(xl, H1_ADDR(0xc94),
	((control->flags & XVMC_CONCEALMENT_MOTION_VECTORS) ? 1 : 0) |
	((control->flags & XVMC_Q_SCALE_TYPE) ? 2 : 0) |
	((control->intra_dc_precision & 3) << 2) |
	(((1 + 0x100000 / mb_width) & 0xfffff) << 4) |
	((control->flags & XVMC_INTRA_VLC_FORMAT) ? (1 << 24) : 0));

    OUT_RING_QW_AGP(xl, H1_ADDR(0xc98),
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

    BEGIN_RING_AGP(xl, 100);
    WAITFLAGS(xl, LL_MODE_DECODER_IDLE);

    for (i = 0; i < 14; i++)
	OUT_RING_QW_AGP(xl, H1_ADDR(0xc08), 0);

    OUT_RING_QW_AGP(xl, H1_ADDR(0xc98), 0x400000);

    for (i = 0; i < 6; i++) {
	OUT_RING_QW_AGP(xl, H1_ADDR(0xc0c), 0x43 | 0x20);
	for (j = 0xc10; j < 0xc20; j += 4)
	    OUT_RING_QW_AGP(xl, H1_ADDR(j), 0);
    }

    OUT_RING_QW_AGP(xl, H1_ADDR(0xc0c), 0xc3 | 0x20);
    for (j = 0xc10; j < 0xc20; j += 4)
	OUT_RING_QW_AGP(xl, H1_ADDR(j), 0);

}

void
viaMpegWriteSlice(void *xlp, CARD8 * slice, int nBytes, CARD32 sCode)
{
    int i, n, r;
    CARD32 *buf;
    int count;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

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

    BEGIN_RING_AGP(xl, 4);
    WAITFLAGS(xl, LL_MODE_DECODER_IDLE);

    OUT_RING_QW_AGP(xl, H1_ADDR(0xc9c), nBytes);

    if (sCode)
	OUT_RING_QW_AGP(xl, H1_ADDR(0xca0), sCode);

    i = 0;
    count = 0;

    do {
	count += (LL_AGP_CMDBUF_SIZE - 20) >> 1;
	count = (count > n) ? n : count;
	BEGIN_RING_AGP(xl, (count - i) << 1);

	for (; i < count; i++) {
	    OUT_RING_QW_AGP(xl, H1_ADDR(0xca0), *buf++);
	}
    } while (i < n);

    BEGIN_RING_AGP(xl, 6);

    if (r) {
	OUT_RING_QW_AGP(xl, H1_ADDR(0xca0), *buf & ((1 << (r << 3)) - 1));
    }
    OUT_RING_QW_AGP(xl, H1_ADDR(0xca0), 0);
    OUT_RING_QW_AGP(xl, H1_ADDR(0xca0), 0);

}

void
viaVideoSubPictureOffLocked(void *xlp)
{

    CARD32 stride;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    stride = VIDIN(xl, SUBP_CONTROL_STRIDE);

    pciCommand(xl, SUBP_CONTROL_STRIDE, stride & ~SUBP_HQV_ENABLE,
	LL_MODE_VIDEO);
}

void
viaVideoSubPictureLocked(void *xlp, ViaXvMCSubPicture * pViaSubPic)
{

    unsigned i;
    CARD32 cWord;
    XvMCLowLevel *xl = (XvMCLowLevel *) xlp;

    for (i = 0; i < VIA_SUBPIC_PALETTE_SIZE; ++i) {
	pciCommand(xl, RAM_TABLE_CONTROL, pViaSubPic->palette[i],
	    LL_MODE_VIDEO);
    }

    pciCommand(xl, SUBP_STARTADDR, pViaSubPic->offset, 0);
    cWord = (pViaSubPic->stride & SUBP_STRIDE_MASK) | SUBP_HQV_ENABLE;
    cWord |= (pViaSubPic->ia44) ? SUBP_IA44 : SUBP_AI44;
    pciCommand(xl, SUBP_CONTROL_STRIDE, cWord, 0);
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

    if (!w || !h)
	return;

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

    BEGIN_RING_AGP(xl, 20);
    WAITFLAGS(xl, LL_MODE_2D);

    OUT_RING_QW_AGP(xl, H1_ADDR(VIA_REG_GEMODE), dwGEMode);
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
	OUT_RING_QW_AGP(xl, H1_ADDR(VIA_REG_SRCCOLORKEY), color);
	OUT_RING_QW_AGP(xl, H1_ADDR(VIA_REG_KEYCONTROL), 0x4000);
	cmd |= VIA_GEC_BLT | (VIA_BLIT_COPY << 24);
	break;
    case VIABLIT_FILL:
	OUT_RING_QW_AGP(xl, H1_ADDR(VIA_REG_FGCOLOR), color);
	cmd |= VIA_GEC_BLT | VIA_GEC_FIXCOLOR_PAT | (VIA_BLIT_FILL << 24);
	break;
    default:
	OUT_RING_QW_AGP(xl, H1_ADDR(VIA_REG_KEYCONTROL), 0x0);
	cmd |= VIA_GEC_BLT | (VIA_BLIT_COPY << 24);
    }

    OUT_RING_QW_AGP(xl, H1_ADDR(VIA_REG_SRCBASE), (srcBase & ~31) >> 3);
    OUT_RING_QW_AGP(xl, H1_ADDR(VIA_REG_DSTBASE), (dstBase & ~31) >> 3);
    OUT_RING_QW_AGP(xl, H1_ADDR(VIA_REG_PITCH), VIA_PITCH_ENABLE |
	(srcPitch >> 3) | (((dstPitch) >> 3) << 16));
    OUT_RING_QW_AGP(xl, H1_ADDR(VIA_REG_SRCPOS), ((srcY << 16) | srcX));
    OUT_RING_QW_AGP(xl, H1_ADDR(VIA_REG_DSTPOS), ((dstY << 16) | dstX));
    OUT_RING_QW_AGP(xl, H1_ADDR(VIA_REG_DIMENSION),
	(((h - 1) << 16) | (w - 1)));
    OUT_RING_QW_AGP(xl, H1_ADDR(VIA_REG_GECMD), cmd);
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
	if ((mode != LL_MODE_VIDEO))
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

extern void *
initXvMCLowLevel(int fd, drm_context_t * ctx,
    drmLockPtr hwLock, drmAddress mmioAddress,
    drmAddress fbAddress, unsigned fbStride, unsigned fbDepth,
    unsigned width, unsigned height, int useAgp, unsigned chipId)
{
    int ret;
    XvMCLowLevel *xl;

    if (chipId == PCI_CHIP_VT3259 || chipId == PCI_CHIP_VT3364) {
	fprintf(stderr, "You are using an XvMC driver for the wrong chip.\n");
	fprintf(stderr, "Chipid is 0x%04x.\n", chipId);
	return NULL;
    }

    xl = (XvMCLowLevel *) malloc(sizeof(XvMCLowLevel));

    if (!xl)
	return NULL;

    xl->agp_pos = 0;
    xl->pci_pos = 0;
    xl->use_agp = useAgp;
    xl->fd = fd;
    xl->drmcontext = ctx;
    xl->hwLock = hwLock;
    xl->mmioAddress = mmioAddress;
    xl->fbAddress = fbAddress;
    xl->curWaitFlags = 0;
    xl->performLocking = 1;
    xl->errors = 0;
    xl->agpSync = 0;
    ret = viaDMAInitTimeStamp(xl);
    if (ret) {
	free(xl);
	return NULL;
    }
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

    viaDMACleanupTimeStamp(xl);
    free(xl);
}
