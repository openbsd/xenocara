/*
 * Copyright 2003 through 2004 by Marc Aurele La France (TSI @ UQV), tsi@xfree86.org
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of Marc Aurele La France not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Marc Aurele La France makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as-is" without express or implied warranty.
 *
 * MARC AURELE LA FRANCE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO
 * EVENT SHALL MARC AURELE LA FRANCE BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
/*
 * Copyright 1999-2000 Precision Insight, Inc., Cedar Park, Texas.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
 * PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
/* 
 * DRI support by:
 *    Manuel Teira
 *    Leif Delgass <ldelgass@retinalburn.net>
 *
 * EXA support by:
 *    Jakub Stachowski <qbast@go2.pl>
 *    George Sapountzis <gsap7@yahoo.gr>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "ati.h"
#include "atichip.h"
#include "atidri.h"
#include "atimach64accel.h"
#include "atimach64io.h"
#include "atipriv.h"
#include "atiregs.h"

#ifdef XF86DRI_DEVEL
#include "mach64_dri.h"
#include "mach64_sarea.h"
#endif

#ifdef USE_EXA
extern CARD8 ATIMach64ALU[];

extern void
ATIMach64ValidateClip
(
    ATIPtr pATI,
    int sc_left,
    int sc_right,
    int sc_top,
    int sc_bottom
);

#if 0
#define MACH64_TRACE(x)				\
do {						\
    ErrorF("Mach64(%s): ", __FUNCTION__);	\
    ErrorF x;					\
} while(0)
#else
#define MACH64_TRACE(x) do { } while(0)
#endif

#if 0
#define MACH64_FALLBACK(x)			\
do {						\
    ErrorF("Fallback(%s): ", __FUNCTION__);	\
    ErrorF x;					\
    return FALSE;				\
} while (0)
#else
#define MACH64_FALLBACK(x) return FALSE
#endif

static void
Mach64WaitMarker(ScreenPtr pScreenInfo, int Marker)
{
    ATIMach64Sync(xf86ScreenToScrn(pScreenInfo));
}

static Bool
Mach64GetDatatypeBpp(PixmapPtr pPix, CARD32 *pix_width)
{
	int bpp = pPix->drawable.bitsPerPixel;

	switch (bpp) {
	case 8:
		*pix_width =
			SetBits(PIX_WIDTH_8BPP, DP_DST_PIX_WIDTH) |
			SetBits(PIX_WIDTH_8BPP, DP_SRC_PIX_WIDTH) |
			SetBits(PIX_WIDTH_1BPP, DP_HOST_PIX_WIDTH);
		break;
	case 16:
		*pix_width =
			SetBits(PIX_WIDTH_16BPP, DP_DST_PIX_WIDTH) |
			SetBits(PIX_WIDTH_16BPP, DP_SRC_PIX_WIDTH) |
			SetBits(PIX_WIDTH_1BPP, DP_HOST_PIX_WIDTH);
		break;
	case 24:
		*pix_width =
			SetBits(PIX_WIDTH_8BPP, DP_DST_PIX_WIDTH) |
			SetBits(PIX_WIDTH_8BPP, DP_SRC_PIX_WIDTH) |
			SetBits(PIX_WIDTH_1BPP, DP_HOST_PIX_WIDTH);
		break;
	case 32:
		*pix_width =
			SetBits(PIX_WIDTH_32BPP, DP_DST_PIX_WIDTH) |
			SetBits(PIX_WIDTH_32BPP, DP_SRC_PIX_WIDTH) |
			SetBits(PIX_WIDTH_1BPP, DP_HOST_PIX_WIDTH);
		break;
	default:
		MACH64_FALLBACK(("Unsupported bpp: %d\n", bpp));
	}

#if X_BYTE_ORDER == X_LITTLE_ENDIAN

        *pix_width |= DP_BYTE_PIX_ORDER;

#endif /* X_BYTE_ORDER */

	return TRUE;
}

static Bool
Mach64GetOffsetPitch(PixmapPtr pPix, int bpp, CARD32 *pitch_offset,
		     unsigned int offset, unsigned int pitch)
{
#if 0
    ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pPix->drawable.pScreen);
    ATIPtr pATI = ATIPTR(pScreenInfo);

    if (pitch % pATI->pExa->pixmapPitchAlign != 0)
        MACH64_FALLBACK(("Bad pitch 0x%08x\n", pitch));

    if (offset % pATI->pExa->pixmapOffsetAlign != 0)
        MACH64_FALLBACK(("Bad offset 0x%08x\n", offset));
#endif

    /* pixels / 8 = ((bytes * 8) / bpp) / 8 = bytes / bpp */
    pitch = pitch / bpp;

    /* bytes / 8 */
    offset = offset >> 3;

    *pitch_offset = ((pitch  << 22) | (offset <<  0));

    return TRUE;
}

static Bool
Mach64GetPixmapOffsetPitch(PixmapPtr pPix, CARD32 *pitch_offset)
{
    CARD32 pitch, offset;
    int bpp;

    bpp = pPix->drawable.bitsPerPixel;
    if (bpp == 24)
        bpp = 8;

    pitch = exaGetPixmapPitch(pPix);
    offset = exaGetPixmapOffset(pPix);

    return Mach64GetOffsetPitch(pPix, bpp, pitch_offset, offset, pitch);
}

static Bool
Mach64PrepareCopy
(
    PixmapPtr pSrcPixmap,
    PixmapPtr pDstPixmap,
    int       xdir,
    int       ydir,
    int       alu,
    Pixel     planemask
)
{
    ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pDstPixmap->drawable.pScreen);
    ATIPtr pATI = ATIPTR(pScreenInfo);
    CARD32 src_pitch_offset, dst_pitch_offset, dp_pix_width;

    ATIDRISync(pScreenInfo);

    if (!Mach64GetDatatypeBpp(pDstPixmap, &dp_pix_width))
        return FALSE;
    if (!Mach64GetPixmapOffsetPitch(pSrcPixmap, &src_pitch_offset))
        return FALSE;
    if (!Mach64GetPixmapOffsetPitch(pDstPixmap, &dst_pitch_offset))
        return FALSE;

    ATIMach64WaitForFIFO(pATI, 7);
    outf(DP_WRITE_MASK, planemask);
    outf(DP_PIX_WIDTH, dp_pix_width);
    outf(SRC_OFF_PITCH, src_pitch_offset);
    outf(DST_OFF_PITCH, dst_pitch_offset); 
    
    outf(DP_SRC, DP_MONO_SRC_ALLONES |
        SetBits(SRC_BLIT, DP_FRGD_SRC) | SetBits(SRC_BKGD, DP_BKGD_SRC));
    outf(DP_MIX, SetBits(ATIMach64ALU[alu], DP_FRGD_MIX));

    outf(CLR_CMP_CNTL, CLR_CMP_FN_FALSE);

    pATI->dst_cntl = 0;

    if (ydir > 0)
        pATI->dst_cntl |= DST_Y_DIR;
    if (xdir > 0)
        pATI->dst_cntl |= DST_X_DIR;

    if (pATI->XModifier == 1)
        outf(DST_CNTL, pATI->dst_cntl);
    else
        pATI->dst_cntl |= DST_24_ROT_EN;

    return TRUE;
}

static void
Mach64Copy
(
    PixmapPtr pDstPixmap,
    int       srcX,
    int       srcY,
    int       dstX,
    int       dstY,
    int       w,
    int       h
)
{
    ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pDstPixmap->drawable.pScreen);
    ATIPtr pATI = ATIPTR(pScreenInfo);

    srcX *= pATI->XModifier;
    dstY *= pATI->XModifier;
    w    *= pATI->XModifier;

    ATIDRISync(pScreenInfo);

    /* Disable clipping if it gets in the way */
    ATIMach64ValidateClip(pATI, dstX, dstX + w - 1, dstY, dstY + h - 1);

    if (!(pATI->dst_cntl & DST_X_DIR))
    {
        srcX += w - 1;
        dstX += w - 1;
    }

    if (!(pATI->dst_cntl & DST_Y_DIR))
    {
        srcY += h - 1;
        dstY += h - 1;
    }

    if (pATI->XModifier != 1)
        outf(DST_CNTL, pATI->dst_cntl | SetBits((dstX / 4) % 6, DST_24_ROT));

    ATIMach64WaitForFIFO(pATI, 4);
    outf(SRC_Y_X, SetWord(srcX, 1) | SetWord(srcY, 0));
    outf(SRC_WIDTH1, w);
    outf(DST_Y_X, SetWord(dstX, 1) | SetWord(dstY, 0));
    outf(DST_HEIGHT_WIDTH, SetWord(w, 1) | SetWord(h, 0));

    /*
     * On VTB's and later, the engine will randomly not wait for a copy
     * operation to commit its results to video memory before starting the next
     * one.  The probability of such occurrences increases with GUI_WB_FLUSH
     * (or GUI_WB_FLUSH_P) setting, bitsPerPixel and/or CRTC clock.  This
     * would point to some kind of video memory bandwidth problem were it noti
     * for the fact that the problem occurs less often (but still occurs) when
     * copying larger rectangles.
     */
    if ((pATI->Chip >= ATI_CHIP_264VTB) && !pATI->OptionDevel)
    {
        exaMarkSync(pScreenInfo->pScreen); /* Force sync. */
        exaWaitSync(pScreenInfo->pScreen); /* Sync and notify EXA. */
    }
}

static void Mach64DoneCopy(PixmapPtr pDstPixmap) { }

static Bool
Mach64PrepareSolid
(
    PixmapPtr pPixmap,
    int       alu, 
    Pixel     planemask, 
    Pixel     fg
)
{
    ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pPixmap->drawable.pScreen);
    ATIPtr pATI = ATIPTR(pScreenInfo);
    CARD32 dst_pitch_offset, dp_pix_width;

    ATIDRISync(pScreenInfo);

    if (!Mach64GetDatatypeBpp(pPixmap, &dp_pix_width))
        return FALSE;
    if (!Mach64GetPixmapOffsetPitch(pPixmap, &dst_pitch_offset))
        return FALSE;

    ATIMach64WaitForFIFO(pATI, 7);
    outf(DP_WRITE_MASK, planemask);
    outf(DP_PIX_WIDTH, dp_pix_width);
    outf(DST_OFF_PITCH, dst_pitch_offset); 

    outf(DP_SRC, DP_MONO_SRC_ALLONES |
        SetBits(SRC_FRGD, DP_FRGD_SRC) | SetBits(SRC_BKGD, DP_BKGD_SRC));
    outf(DP_FRGD_CLR, fg);
    outf(DP_MIX, SetBits(ATIMach64ALU[alu], DP_FRGD_MIX));

    outf(CLR_CMP_CNTL, CLR_CMP_FN_FALSE);

    if (pATI->XModifier == 1)
        outf(DST_CNTL, DST_X_DIR | DST_Y_DIR);

    return TRUE;
}

static void
Mach64Solid
(
    PixmapPtr pPixmap, 
    int       x1, 
    int       y1, 
    int       x2, 
    int       y2
)
{
    ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pPixmap->drawable.pScreen);
    ATIPtr pATI = ATIPTR(pScreenInfo);

    int x = x1;
    int y = y1;
    int w = x2-x1;
    int h = y2-y1;

    ATIDRISync(pScreenInfo);

    if (pATI->XModifier != 1)
    {
        x *= pATI->XModifier;
        w *= pATI->XModifier;

        outf(DST_CNTL, SetBits((x / 4) % 6, DST_24_ROT) |
            (DST_X_DIR | DST_Y_DIR | DST_24_ROT_EN));
    }

    /* Disable clipping if it gets in the way */
    ATIMach64ValidateClip(pATI, x, x + w - 1, y, y + h - 1);

    ATIMach64WaitForFIFO(pATI, 2);
    outf(DST_Y_X, SetWord(x, 1) | SetWord(y, 0));
    outf(DST_HEIGHT_WIDTH, SetWord(w, 1) | SetWord(h, 0));
}

static void Mach64DoneSolid(PixmapPtr pPixmap) { }

#include "atimach64render.c"

/* Compute log base 2 of val. */
static __inline__ int Mach64Log2(int val)
{
    int bits;

    for (bits = 0; val != 0; val >>= 1, ++bits)
        ;
    return bits - 1;
}

/*
 * Memory layour for EXA with DRI (no local_textures):
 * | front  | back   | depth  | textures | pixmaps, xv | c |
 *
 * 1024x768@16bpp with 8 MB:
 * | 1.5 MB | 1.5 MB | 1.5 MB | 0        | ~3.5 MB     | c |
 *
 * 1024x768@32bpp with 8 MB:
 * | 3.0 MB | 3.0 MB | 1.5 MB | 0        | ~0.5 MB     | c |
 *
 * "c" is the hw cursor which occupies 1KB
 */
static void
Mach64SetupMemEXA(ScreenPtr pScreen)
{
    ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
    ATIPtr pATI = ATIPTR(pScreenInfo);

    int cpp = (pScreenInfo->bitsPerPixel + 7) / 8;
    /* front and back buffer */
    int bufferSize = pScreenInfo->virtualY * pScreenInfo->displayWidth * cpp;
    /* always 16-bit z-buffer */
    int depthSize  = pScreenInfo->virtualY * pScreenInfo->displayWidth * 2;

    ExaDriverPtr pExa = pATI->pExa;

    pExa->memoryBase = pATI->pMemory;
    pExa->memorySize = pScreenInfo->videoRam * 1024;
    pExa->offScreenBase = bufferSize;

#ifdef XF86DRI_DEVEL
    if (pATI->directRenderingEnabled)
    {
	ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;
	Bool is_pci = pATIDRIServer->IsPCI;

	int textureSize = 0;
	int pixmapCache = 0;
	int next = 0;

	/* front buffer */
	pATIDRIServer->frontOffset = 0;
	pATIDRIServer->frontPitch = pScreenInfo->displayWidth;
	next += bufferSize;

	/* back buffer */
	pATIDRIServer->backOffset = next;
	pATIDRIServer->backPitch = pScreenInfo->displayWidth;
	next += bufferSize;

	/* depth buffer */
	pATIDRIServer->depthOffset = next;
	pATIDRIServer->depthPitch = pScreenInfo->displayWidth;
	next += depthSize;

	/* ATIScreenInit does check for the this condition. */
	if (next > pExa->memorySize)
	{
	    xf86DrvMsg(pScreen->myNum, X_WARNING,
		"DRI static buffer allocation failed, disabling DRI --"
		"need at least %d kB video memory\n", next / 1024 );
	    ATIDRICloseScreen(pScreen);
	    pATI->directRenderingEnabled = FALSE;
	}

	/* local textures */

	/* Reserve approx. half of offscreen memory for local textures */
	textureSize = (pExa->memorySize - next) / 2;

	/* In case DRI requires more offscreen memory than available,
	 * should not happen as ATIScreenInit would have not enabled DRI */
	if (textureSize < 0)
	    textureSize = 0;

	/* Try for enough pixmap cache for a full viewport */
	pixmapCache = (pExa->memorySize - next) - textureSize;
	if (pixmapCache < bufferSize)
	    textureSize = 0;

	/* Don't allocate a local texture heap for AGP unless requested */
	if ( !is_pci && !pATI->OptionLocalTextures )
	    textureSize = 0;

	if (textureSize > 0)
	{
	    int l = Mach64Log2(textureSize / MACH64_NR_TEX_REGIONS);
	    if (l < MACH64_LOG_TEX_GRANULARITY)
		l = MACH64_LOG_TEX_GRANULARITY;
	    pATIDRIServer->logTextureGranularity = l;

	    /* Round the texture size down to the nearest whole number of
	     * texture regions.
	     */
	    textureSize = (textureSize >> l) << l;
	}

	/* Set a minimum usable local texture heap size.  This will fit
	 * two 256x256 textures.  We check this after any rounding of
	 * the texture area.
	 */
	if (textureSize < 256*256 * cpp * 2)
	    textureSize = 0;

	/* Disable DRI for PCI if cannot allocate a local texture heap */
	if ( is_pci && textureSize == 0 )
	{
	    xf86DrvMsg(pScreen->myNum, X_WARNING,
		"Not enough memory for local textures, disabling DRI\n");
	    ATIDRICloseScreen(pScreen);
	    pATI->directRenderingEnabled = FALSE;
	}

	pATIDRIServer->textureOffset = next;
	pATIDRIServer->textureSize = textureSize;
	next += textureSize;

	/* pExa->offScreenBase is moved to `next' when DRI gets activated */
    }
#endif /* XF86DRI_DEVEL */

    xf86DrvMsg(pScreen->myNum, X_INFO,
        "EXA memory management initialized\n"
        "\t base     :  %10p\n"
        "\t offscreen: +%10lx\n"
        "\t size     : +%10lx\n"
        "\t cursor   :  %10p\n",
        pExa->memoryBase,
        pExa->offScreenBase,
        pExa->memorySize,
        pATI->pCursorImage);

    if (TRUE || xf86GetVerbosity() > 1)
    {
        int offscreen = pExa->memorySize - pExa->offScreenBase;
        int viewport = bufferSize;
        int dvdframe = 720*480*cpp; /* enough for single-buffered DVD */

        xf86DrvMsg(pScreen->myNum, X_INFO,
            "Will use %d kB of offscreen memory for EXA\n"
            "\t\t or %5.2f viewports (composite)\n"
            "\t\t or %5.2f dvdframes (xvideo)\n",
            offscreen / 1024,
            1.0 * offscreen / viewport,
            1.0 * offscreen / dvdframe);
    }

#ifdef XF86DRI_DEVEL
    if (pATI->directRenderingEnabled)
    {
        ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;

        xf86DrvMsg(pScreen->myNum, X_INFO,
                   "Will use back  buffer at offset 0x%x\n",
                   pATIDRIServer->backOffset);

        xf86DrvMsg(pScreen->myNum, X_INFO,
                   "Will use depth buffer at offset 0x%x\n",
                   pATIDRIServer->depthOffset);

        if (pATIDRIServer->textureSize > 0)
        {
            xf86DrvMsg(pScreen->myNum, X_INFO,
                   "Will use %d kB for local textures at offset 0x%x\n",
                   pATIDRIServer->textureSize/1024,
                   pATIDRIServer->textureOffset);
        }
    }
#endif /* XF86DRI_DEVEL */

    pExa->pixmapOffsetAlign = 64;
    pExa->pixmapPitchAlign = 64;

    pExa->flags = EXA_OFFSCREEN_PIXMAPS;

    pExa->maxX = ATIMach64MaxX;
    pExa->maxY = ATIMach64MaxY;
}

Bool ATIMach64ExaInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
    ATIPtr pATI = ATIPTR(pScreenInfo);
    ExaDriverPtr pExa;

    pExa = exaDriverAlloc();
    if (!pExa)
        return FALSE;

    pATI->pExa = pExa;

    pExa->exa_major = 2;
    pExa->exa_minor = 0;

    Mach64SetupMemEXA(pScreen);

    pExa->WaitMarker = Mach64WaitMarker;

    pExa->PrepareSolid = Mach64PrepareSolid;
    pExa->Solid = Mach64Solid;
    pExa->DoneSolid = Mach64DoneSolid;

    pExa->PrepareCopy = Mach64PrepareCopy;
    pExa->Copy = Mach64Copy;
    pExa->DoneCopy = Mach64DoneCopy;

    if (pATI->RenderAccelEnabled) {
	if (pATI->Chip >= ATI_CHIP_264GTPRO) {
	    /* 3D Rage Pro does not support NPOT textures. */
	    pExa->flags |= EXA_OFFSCREEN_ALIGN_POT;

	    pExa->CheckComposite = Mach64CheckComposite;
	    pExa->PrepareComposite = Mach64PrepareComposite;
	    pExa->Composite = Mach64Composite;
	    pExa->DoneComposite = Mach64DoneComposite;
	} else {
	    xf86DrvMsg(pScreen->myNum, X_INFO,
		       "Render acceleration is not supported for ATI chips "
		       "earlier than the ATI 3D Rage Pro.\n");
	    pATI->RenderAccelEnabled = FALSE;
	}
    }

    xf86DrvMsg(pScreen->myNum, X_INFO, "Render acceleration %s\n",
	       pATI->RenderAccelEnabled ? "enabled" : "disabled");

    if (!exaDriverInit(pScreen, pATI->pExa)) {
	free(pATI->pExa);
	pATI->pExa = NULL;
	return FALSE;
    }

    return TRUE;
}
#endif
