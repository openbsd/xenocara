/*
 * Copyright 2005 Eric Anholt
 * Copyright 2005 Benjamin Herrenschmidt
 * All Rights Reserved.
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
 * Authors:
 *    Eric Anholt <anholt@FreeBSD.org>
 *    Zack Rusin <zrusin@trolltech.com>
 *    Benjamin Herrenschmidt <benh@kernel.crashing.org>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "radeon.h"
#include "radeon_reg.h"
#ifdef XF86DRI
#include "radeon_dri.h"
#endif
#include "radeon_macros.h"
#include "radeon_probe.h"
#include "radeon_version.h"
#ifdef XF86DRI
#include "radeon_sarea.h"
#endif

#include "xf86.h"


/***********************************************************************/
#define RINFO_FROM_SCREEN(pScr) ScrnInfoPtr pScrn =  xf86Screens[pScr->myNum]; \
    RADEONInfoPtr info   = RADEONPTR(pScrn)

#define RADEON_TRACE_FALL 0
#define RADEON_TRACE_DRAW 0

#if RADEON_TRACE_FALL
#define RADEON_FALLBACK(x)     		\
do {					\
	ErrorF("%s: ", __FUNCTION__);	\
	ErrorF x;			\
	return FALSE;			\
} while (0)
#else
#define RADEON_FALLBACK(x) return FALSE
#endif

#if RADEON_TRACE_DRAW
#define TRACE do { ErrorF("TRACE: %s\n", __FUNCTION__); } while(0)
#else
#define TRACE
#endif

static struct {
    int rop;
    int pattern;
} RADEON_ROP[] = {
    { RADEON_ROP3_ZERO, RADEON_ROP3_ZERO }, /* GXclear        */
    { RADEON_ROP3_DSa,  RADEON_ROP3_DPa  }, /* Gxand          */
    { RADEON_ROP3_SDna, RADEON_ROP3_PDna }, /* GXandReverse   */
    { RADEON_ROP3_S,    RADEON_ROP3_P    }, /* GXcopy         */
    { RADEON_ROP3_DSna, RADEON_ROP3_DPna }, /* GXandInverted  */
    { RADEON_ROP3_D,    RADEON_ROP3_D    }, /* GXnoop         */
    { RADEON_ROP3_DSx,  RADEON_ROP3_DPx  }, /* GXxor          */
    { RADEON_ROP3_DSo,  RADEON_ROP3_DPo  }, /* GXor           */
    { RADEON_ROP3_DSon, RADEON_ROP3_DPon }, /* GXnor          */
    { RADEON_ROP3_DSxn, RADEON_ROP3_PDxn }, /* GXequiv        */
    { RADEON_ROP3_Dn,   RADEON_ROP3_Dn   }, /* GXinvert       */
    { RADEON_ROP3_SDno, RADEON_ROP3_PDno }, /* GXorReverse    */
    { RADEON_ROP3_Sn,   RADEON_ROP3_Pn   }, /* GXcopyInverted */
    { RADEON_ROP3_DSno, RADEON_ROP3_DPno }, /* GXorInverted   */
    { RADEON_ROP3_DSan, RADEON_ROP3_DPan }, /* GXnand         */
    { RADEON_ROP3_ONE,  RADEON_ROP3_ONE  }  /* GXset          */
};

/* Compute log base 2 of val. */
static __inline__ int
RADEONLog2(int val)
{
	int bits;

	for (bits = 0; val != 0; val >>= 1, ++bits)
		;
	return bits - 1;
}

static __inline__ CARD32 F_TO_DW(float val)
{
    union {
	float f;
	CARD32 l;
    } tmp;
    tmp.f = val;
    return tmp.l;
}

/* Assumes that depth 15 and 16 can be used as depth 16, which is okay since we
 * require src and dest datatypes to be equal.
 */
static Bool RADEONGetDatatypeBpp(int bpp, CARD32 *type)
{
	switch (bpp) {
	case 8:
		*type = ATI_DATATYPE_CI8;
		return TRUE;
	case 16:
		*type = ATI_DATATYPE_RGB565;
		return TRUE;
	case 24:
		*type = ATI_DATATYPE_CI8;
		return TRUE;
	case 32:
		*type = ATI_DATATYPE_ARGB8888;
		return TRUE;
	default:
		RADEON_FALLBACK(("Unsupported bpp: %d\n", bpp));
		return FALSE;
	}
}

static Bool RADEONPixmapIsColortiled(PixmapPtr pPix)
{
    RINFO_FROM_SCREEN(pPix->drawable.pScreen);

    /* This doesn't account for the back buffer, which we may want to wrap in
     * a pixmap at some point for the purposes of DRI buffer moves.
     */
    if (info->tilingEnabled && exaGetPixmapOffset(pPix) == 0)
	return TRUE;
    else
	return FALSE;
}

static Bool RADEONGetOffsetPitch(PixmapPtr pPix, int bpp, CARD32 *pitch_offset,
				 unsigned int offset, unsigned int pitch)
{
	RINFO_FROM_SCREEN(pPix->drawable.pScreen);

	if (pitch % info->exa->pixmapPitchAlign != 0)
		RADEON_FALLBACK(("Bad pitch 0x%08x\n", pitch));

	if (offset % info->exa->pixmapOffsetAlign != 0)
		RADEON_FALLBACK(("Bad offset 0x%08x\n", offset));

	pitch = pitch >> 6;
	*pitch_offset = (pitch << 22) | (offset >> 10);

	/* If it's the front buffer, we've got to note that it's tiled? */
	if (RADEONPixmapIsColortiled(pPix))
		*pitch_offset |= RADEON_DST_TILE_MACRO;
	return TRUE;
}

static Bool RADEONGetPixmapOffsetPitch(PixmapPtr pPix, CARD32 *pitch_offset)
{
	RINFO_FROM_SCREEN(pPix->drawable.pScreen);
	CARD32 pitch, offset;
	int bpp;

	bpp = pPix->drawable.bitsPerPixel;
	if (bpp == 24)
		bpp = 8;

	offset = exaGetPixmapOffset(pPix) + info->fbLocation;
	pitch = exaGetPixmapPitch(pPix);

	return RADEONGetOffsetPitch(pPix, bpp, pitch_offset, offset, pitch);
}

#if X_BYTE_ORDER == X_BIG_ENDIAN

static unsigned long swapper_surfaces[3];

static Bool RADEONPrepareAccess(PixmapPtr pPix, int index)
{
    RINFO_FROM_SCREEN(pPix->drawable.pScreen);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32 offset = exaGetPixmapOffset(pPix);
    int bpp, soff;
    CARD32 size, flags;

    /* Front buffer is always set with proper swappers */
    if (offset == 0)
        return TRUE;

    /* If same bpp as front buffer, just do nothing as the main
     * swappers will apply
     */
    bpp = pPix->drawable.bitsPerPixel;
    if (bpp == pScrn->bitsPerPixel)
        return TRUE;

    /* We need to setup a separate swapper, let's request a
     * surface. We need to align the size first
     */
    size = exaGetPixmapSize(pPix);
    size = (size + RADEON_BUFFER_ALIGN) & ~(RADEON_BUFFER_ALIGN);

    /* Set surface to tiling disabled with appropriate swapper */
    switch (bpp) {
    case 16:
        flags = RADEON_SURF_AP0_SWP_16BPP | RADEON_SURF_AP1_SWP_16BPP;
	break;
    case 32:
        flags = RADEON_SURF_AP0_SWP_32BPP | RADEON_SURF_AP1_SWP_32BPP;
	break;
    default:
        flags = 0;
    }
#if defined(XF86DRI)
    if (info->directRenderingEnabled && info->allowColorTiling) {
	drmRadeonSurfaceAlloc drmsurfalloc;
	int rc;

        drmsurfalloc.address = offset;
        drmsurfalloc.size = size;
	drmsurfalloc.flags = flags | 1; /* bogus pitch to please DRM */

        rc = drmCommandWrite(info->drmFD, DRM_RADEON_SURF_ALLOC,
			     &drmsurfalloc, sizeof(drmsurfalloc));
	if (rc < 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "drm: could not allocate surface for access"
		       " swapper, err: %d!\n", rc);
	    return FALSE;
	}
	swapper_surfaces[index] = offset;

	return TRUE;
    }
#endif
    soff = (index + 1) * 0x10;
    OUTREG(RADEON_SURFACE0_INFO + soff, flags);
    OUTREG(RADEON_SURFACE0_LOWER_BOUND + soff, offset);
    OUTREG(RADEON_SURFACE0_UPPER_BOUND + soff, offset + size - 1);
    swapper_surfaces[index] = offset;
    return TRUE;
}

static void RADEONFinishAccess(PixmapPtr pPix, int index)
{
    RINFO_FROM_SCREEN(pPix->drawable.pScreen);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32 offset = exaGetPixmapOffset(pPix);
    int soff;

    /* Front buffer is always set with proper swappers */
    if (offset == 0)
        return;

    if (swapper_surfaces[index] == 0)
        return;
#if defined(XF86DRI)
    if (info->directRenderingEnabled && info->allowColorTiling) {
	drmRadeonSurfaceFree drmsurffree;

	drmsurffree.address = offset;
	drmCommandWrite(info->drmFD, DRM_RADEON_SURF_FREE,
			&drmsurffree, sizeof(drmsurffree));
	swapper_surfaces[index] = 0;
	return;
    }
#endif
    soff = (index + 1) * 0x10;
    OUTREG(RADEON_SURFACE0_INFO + soff, 0);
    OUTREG(RADEON_SURFACE0_LOWER_BOUND + soff, 0);
    OUTREG(RADEON_SURFACE0_UPPER_BOUND + soff, 0);
    swapper_surfaces[index] = 0;
}

#endif /* X_BYTE_ORDER == X_BIG_ENDIAN */

#define RADEON_SWITCH_TO_2D()						\
do {									\
	CARD32 wait_until = 0;			\
	BEGIN_ACCEL(1);							\
	switch (info->engineMode) {					\
	case EXA_ENGINEMODE_UNKNOWN:					\
	    wait_until |= RADEON_WAIT_HOST_IDLECLEAN | RADEON_WAIT_2D_IDLECLEAN;	\
	case EXA_ENGINEMODE_3D:						\
	    wait_until |= RADEON_WAIT_3D_IDLECLEAN;			\
	case EXA_ENGINEMODE_2D:						\
	    break;							\
	}								\
	OUT_ACCEL_REG(RADEON_WAIT_UNTIL, wait_until);			\
	FINISH_ACCEL();							\
        info->engineMode = EXA_ENGINEMODE_2D;                           \
} while (0);

#define RADEON_SWITCH_TO_3D()						\
do {									\
	CARD32 wait_until = 0;			\
	BEGIN_ACCEL(1);							\
	switch (info->engineMode) {					\
	case EXA_ENGINEMODE_UNKNOWN:					\
	    wait_until |= RADEON_WAIT_HOST_IDLECLEAN | RADEON_WAIT_3D_IDLECLEAN;	\
	case EXA_ENGINEMODE_2D:						\
	    wait_until |= RADEON_WAIT_2D_IDLECLEAN;			\
	case EXA_ENGINEMODE_3D:						\
	    break;							\
	}								\
	OUT_ACCEL_REG(RADEON_WAIT_UNTIL, wait_until);			\
	FINISH_ACCEL();							\
        info->engineMode = EXA_ENGINEMODE_3D;                           \
} while (0);

#define ENTER_DRAW(x) TRACE
#define LEAVE_DRAW(x) TRACE
/***********************************************************************/

#define ACCEL_MMIO
#define ACCEL_PREAMBLE()	unsigned char *RADEONMMIO = info->MMIO
#define BEGIN_ACCEL(n)		RADEONWaitForFifo(pScrn, (n))
#define OUT_ACCEL_REG(reg, val)	OUTREG(reg, val)
#define OUT_ACCEL_REG_F(reg, val) OUTREG(reg, F_TO_DW(val))
#define FINISH_ACCEL()

#ifdef RENDER
#include "radeon_exa_render.c"
#endif
#include "radeon_exa_funcs.c"

#undef ACCEL_MMIO
#undef ACCEL_PREAMBLE
#undef BEGIN_ACCEL
#undef OUT_ACCEL_REG
#undef FINISH_ACCEL

#ifdef XF86DRI

#define ACCEL_CP
#define ACCEL_PREAMBLE()						\
    RING_LOCALS;							\
    RADEONCP_REFRESH(pScrn, info)
#define BEGIN_ACCEL(n)		BEGIN_RING(2*(n))
#define OUT_ACCEL_REG(reg, val)	OUT_RING_REG(reg, val)
#define FINISH_ACCEL()		ADVANCE_RING()

#define OUT_RING_F(x) OUT_RING(F_TO_DW(x))

#ifdef RENDER
#include "radeon_exa_render.c"
#endif
#include "radeon_exa_funcs.c"

#endif /* XF86DRI */

/*
 * Once screen->off_screen_base is set, this function
 * allocates the remaining memory appropriately
 */
Bool RADEONSetupMemEXA (ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    int cpp = info->CurrentLayout.pixel_bytes;
    int screen_size;
    int byteStride = pScrn->displayWidth * cpp;

    if (info->exa != NULL) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "Memory map already initialized\n");
	return FALSE;
    }
    info->exa = exaDriverAlloc();
    if (info->exa == NULL)
	return FALSE;

    /* Need to adjust screen size for 16 line tiles, and then make it align to.
     * the buffer alignment requirement.
     */
    if (info->allowColorTiling)
	screen_size = RADEON_ALIGN(pScrn->virtualY, 16) * byteStride;
    else
	screen_size = pScrn->virtualY * byteStride;

    info->exa->memoryBase = info->FB + pScrn->fbOffset;
    info->exa->memorySize = info->FbMapSize - info->FbSecureSize;
    info->exa->offScreenBase = screen_size;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Allocating from a screen of %ld kb\n",
	       info->exa->memorySize / 1024);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Will use %d kb for front buffer at offset 0x%08x\n",
	       screen_size / 1024, 0);

    /* Reserve static area for hardware cursor */
    if (!xf86ReturnOptValBool(info->Options, OPTION_SW_CURSOR, FALSE)) {
	int cursor_size = 64 * 4 * 64;

	info->cursor_offset = info->exa->offScreenBase;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Will use %d kb for hardware cursor at offset 0x%08x\n",
		   cursor_size / 1024, (unsigned int)info->cursor_offset);

	info->exa->offScreenBase += cursor_size;
    }

#if defined(XF86DRI)
    if (info->directRenderingEnabled) {
	int depthCpp = (info->depthBits - 8) / 4, l, next, depth_size;

	info->frontOffset = 0;
	info->frontPitch = pScrn->displayWidth;

	RADEONDRIAllocatePCIGARTTable(pScreen);
	
	if (info->cardType==CARD_PCIE)
	  xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		     "Will use %d kb for PCI GART at offset 0x%08x\n",
		     RADEON_PCIGART_TABLE_SIZE / 1024,
		     (int)info->pciGartOffset);

	/* Reserve a static area for the back buffer the same size as the
	 * visible screen.  XXX: This would be better initialized in ati_dri.c
	 * when GLX is set up, but the offscreen memory manager's allocations
	 * don't last through VT switches, while the kernel's understanding of
	 * offscreen locations does.
	 */
	info->backPitch = pScrn->displayWidth;
	next = RADEON_ALIGN(info->exa->offScreenBase, RADEON_BUFFER_ALIGN);
	if (!info->noBackBuffer &&
	    next + screen_size <= info->exa->memorySize)
	{
	    info->backOffset = next;
	    info->exa->offScreenBase = next + screen_size;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Will use %d kb for back buffer at offset 0x%08x\n",
		       screen_size / 1024, info->backOffset);
	}

	/* Reserve the static depth buffer, and adjust pitch and height to
	 * handle tiling.
	 */
	info->depthPitch = RADEON_ALIGN(pScrn->displayWidth, 32);
	depth_size = RADEON_ALIGN(pScrn->virtualY, 16) * info->depthPitch * depthCpp;
	next = RADEON_ALIGN(info->exa->offScreenBase, RADEON_BUFFER_ALIGN);
	if (next + depth_size <= info->exa->memorySize)
	{
	    info->depthOffset = next;
	    info->exa->offScreenBase = next + depth_size;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Will use %d kb for depth buffer at offset 0x%08x\n",
		       depth_size / 1024, info->depthOffset);
	}
	
	info->textureSize *= (info->exa->memorySize -
			      info->exa->offScreenBase) / 100;

	l = RADEONLog2(info->textureSize / RADEON_NR_TEX_REGIONS);
	if (l < RADEON_LOG_TEX_GRANULARITY)
	    l = RADEON_LOG_TEX_GRANULARITY;
	info->textureSize = (info->textureSize >> l) << l;
	if (info->textureSize >= 512 * 1024) {
	    info->textureOffset = info->exa->offScreenBase;
	    info->exa->offScreenBase += info->textureSize;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Will use %d kb for textures at offset 0x%08x\n",
		       info->textureSize / 1024, info->textureOffset);
	} else {
	    /* Minimum texture size is for 2 256x256x32bpp textures */
	    info->textureSize = 0;
	}
    }
#endif /* XF86DRI */
	
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Will use %ld kb for X Server offscreen at offset 0x%08lx\n",
	       (info->exa->memorySize - info->exa->offScreenBase) /
	       1024, info->exa->offScreenBase);

    return TRUE;
}
