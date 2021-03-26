/*
 * Copyright 1999, 2000 ATI Technologies Inc., Markham, Ontario,
 *                      Precision Insight, Inc., Cedar Park, Texas, and
 *                      VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, PRECISION INSIGHT, VA LINUX
 * SYSTEMS AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Authors:
 *   Rickard E. Faith <faith@valinux.com>
 *   Kevin E. Martin <martin@valinux.com>
 *
 * References:
 *
 *   RAGE 128 VR/ RAGE 128 GL Register Reference Manual (Technical
 *   Reference Manual P/N RRG-G04100-C Rev. 0.04), ATI Technologies: April
 *   1999.
 *
 *   RAGE 128 Software Development Manual (Technical Reference Manual P/N
 *   SDK-G04000 Rev. 0.01), ATI Technologies: June 1999.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

				/* Driver data structures */
#include "r128.h"
#include "r128_reg.h"

				/* X and server generic header files */
#include "xf86.h"

				/* Because for EXA we need to use a different allocator */
#ifdef USE_EXA
#include "exa.h"
#endif

#define CURSOR_WIDTH    64
#define CURSOR_HEIGHT   64

void r128_crtc_show_cursor(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;
    R128InfoPtr info = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    int crtc_id = r128_crtc->crtc_id;

    switch (crtc_id) {
    case 0:
        OUTREGP(R128_CRTC_GEN_CNTL, R128_CRTC_CUR_EN, ~R128_CRTC_CUR_EN);
        break;
    case 1:
        OUTREGP(R128_CRTC2_GEN_CNTL, R128_CRTC2_CUR_EN, ~R128_CRTC2_CUR_EN);
        break;
    default:
        return;
    }
}

void r128_crtc_hide_cursor(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;
    R128InfoPtr info = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    int crtc_id = r128_crtc->crtc_id;

    switch (crtc_id) {
    case 0:
        OUTREGP(R128_CRTC_GEN_CNTL, 0, ~R128_CRTC_CUR_EN);
        break;
    case 1:
        OUTREGP(R128_CRTC2_GEN_CNTL, 0, ~R128_CRTC2_CUR_EN);
        break;
    default:
        return;
    }
}

void r128_crtc_set_cursor_colors(xf86CrtcPtr crtc, int bg, int fg)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;
    R128InfoPtr info = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    int crtc_id = r128_crtc->crtc_id;

    switch (crtc_id) {
    case 0:
        OUTREG(R128_CUR_CLR0, bg);
        OUTREG(R128_CUR_CLR1, fg);
        break;
    case 1:
        OUTREG(R128_CUR2_CLR0, bg);
        OUTREG(R128_CUR2_CLR1, fg);
        break;
    default:
        return;
    }
}

void r128_crtc_set_cursor_position (xf86CrtcPtr crtc, int x, int y)
{
    ScrnInfoPtr           pScrn         = crtc->scrn;
    R128InfoPtr           info          = R128PTR(pScrn);
    R128CrtcPrivatePtr    r128_crtc     = crtc->driver_private;
    unsigned char         *R128MMIO     = info->MMIO;
    int                   crtc_id       = r128_crtc->crtc_id;

    int xorigin = 0, yorigin = 0;
    DisplayModePtr mode = &crtc->mode;

    if (x < 0) xorigin = -x + 1;
    if (y < 0) yorigin = -y + 1;
    if (xorigin >= CURSOR_WIDTH)  xorigin = CURSOR_WIDTH - 1;
    if (yorigin >= CURSOR_HEIGHT) yorigin = CURSOR_HEIGHT - 1;

    if (mode->Flags & V_INTERLACE)
        y /= 2;
    else if (mode->Flags & V_DBLSCAN)
        y *= 2;

    if(crtc_id == 0) {
        OUTREG(R128_CUR_HORZ_VERT_OFF, (R128_CUR_LOCK | (xorigin << 16) | yorigin));
        OUTREG(R128_CUR_HORZ_VERT_POSN, (R128_CUR_LOCK | ((xorigin ? 0 : x) << 16) | (yorigin ? 0 : y)));
        OUTREG(R128_CUR_OFFSET, r128_crtc->cursor_offset + pScrn->fbOffset + yorigin * 16);
    } else if (crtc_id == 1) {
        OUTREG(R128_CUR2_HORZ_VERT_OFF, (R128_CUR2_LOCK | (xorigin << 16) | yorigin));
        OUTREG(R128_CUR2_HORZ_VERT_POSN, (R128_CUR2_LOCK | ((xorigin ? 0 : x) << 16) | (yorigin ? 0 : y)));
        OUTREG(R128_CUR2_OFFSET, r128_crtc->cursor_offset + pScrn->fbOffset + yorigin * 16);
    }
}

void r128_crtc_load_cursor_image(xf86CrtcPtr crtc, unsigned char *src)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;
    int crtc_id = r128_crtc->crtc_id;

    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    uint32_t      save1     = 0;
    uint32_t      save2     = 0;

    if (crtc_id == 0) {
	save1 = INREG(R128_CRTC_GEN_CNTL);
	OUTREG(R128_CRTC_GEN_CNTL, save1 & (uint32_t)~R128_CRTC_CUR_EN);
    } else if (crtc_id == 1) {
	save2 = INREG(R128_CRTC2_GEN_CNTL);
	OUTREG(R128_CRTC2_GEN_CNTL, save2 & (uint32_t)~R128_CRTC2_CUR_EN);
    }

#if X_BYTE_ORDER == X_BIG_ENDIAN
    if (info->CurrentLayout.pixel_bytes == 4 || info->CurrentLayout.pixel_bytes == 3)
        R128CopySwap(info->FB + r128_crtc->cursor_offset + pScrn->fbOffset, src,
                     CURSOR_WIDTH * CURSOR_HEIGHT / 4, APER_0_BIG_ENDIAN_32BPP_SWAP);
    else if (info->CurrentLayout.pixel_bytes == 2)
        R128CopySwap(info->FB + r128_crtc->cursor_offset + pScrn->fbOffset, src,
                     CURSOR_WIDTH * CURSOR_HEIGHT / 4, APER_0_BIG_ENDIAN_16BPP_SWAP);
    else
#endif
    memcpy(info->FB + r128_crtc->cursor_offset + pScrn->fbOffset, src, CURSOR_WIDTH * CURSOR_HEIGHT / 4);

    if (crtc_id == 0)
	OUTREG(R128_CRTC_GEN_CNTL, save1);
    else
        OUTREG(R128_CRTC2_GEN_CNTL, save2);
}

/* Initialize hardware cursor support. */
Bool R128CursorInit(ScreenPtr pScreen)
{
    ScrnInfoPtr           pScrn   = xf86ScreenToScrn(pScreen);
    R128InfoPtr           info    = R128PTR(pScrn);
    FBAreaPtr             fbarea  = NULL;
#ifdef USE_EXA
    ExaOffscreenArea*	  osArea  = NULL;
#else
    void*		  osArea  = NULL;
#endif
    xf86CrtcConfigPtr     xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    uint32_t              cursor_offset = 0;
    int                   cpp = info->CurrentLayout.pixel_bytes;
    int                   width;
    int                   width_bytes;
    int                   height;
    int                   size;
    int                   size_bytes;
    int                   c;

    size                      = CURSOR_WIDTH * CURSOR_HEIGHT / 4;
    size_bytes                = size * 2;
    width                     = pScrn->displayWidth;
    width_bytes               = width * (pScrn->bitsPerPixel / 8);
    height                    = ((size_bytes * xf86_config->num_crtc) + width_bytes - 1) / width_bytes;

    if(!info->useEXA) {
	fbarea = xf86AllocateOffscreenArea(pScreen, width, height,
					   16, NULL, NULL, NULL);

	if (fbarea)
	    cursor_offset = R128_ALIGN((fbarea->box.x1 + width * fbarea->box.y1) * cpp, 16);
    }
#ifdef USE_EXA
    else {
	osArea = exaOffscreenAlloc(pScreen, width * height, 16,
				   TRUE, NULL, NULL);

	if (osArea)
	    cursor_offset = osArea->offset;
    }
#endif

    if ((!info->useEXA && !fbarea) || (info->useEXA && !osArea)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Hardware cursor disabled"
		   " due to insufficient offscreen memory\n");
        return FALSE;
    } else {
        for (c = 0; c < xf86_config->num_crtc; c++) {
            xf86CrtcPtr crtc = xf86_config->crtc[c];
	    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;

            r128_crtc->cursor_offset = cursor_offset + (c * size);
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Will use %d kb for hardware cursor %d at offset 0x%08x\n",
		       (size_bytes * xf86_config->num_crtc) / 1024, c,
		       (unsigned int)r128_crtc->cursor_offset);
        }
    }

    return xf86_cursors_init(pScreen, CURSOR_WIDTH, CURSOR_HEIGHT,
			     (HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
			      HARDWARE_CURSOR_AND_SOURCE_WITH_MASK |
			      HARDWARE_CURSOR_SHOW_TRANSPARENT |
			      HARDWARE_CURSOR_UPDATE_UNHIDDEN |
#if X_BYTE_ORDER == X_LITTLE_ENDIAN
			      HARDWARE_CURSOR_BIT_ORDER_MSBFIRST |
#endif
			      HARDWARE_CURSOR_INVERT_MASK |
			      HARDWARE_CURSOR_SWAP_SOURCE_AND_MASK |
			      HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_64));
}
