/*
Copyright (C) 1994-1999 The XFree86 Project, Inc.  All Rights Reserved.
Copyright (C) 2000 Silicon Motion, Inc.  All Rights Reserved.
Copyright (C) 2008 Mandriva Linux.  All Rights Reserved.
Copyright (C) 2008 Francisco Jerez.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FIT-
NESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the names of The XFree86 Project and
Silicon Motion shall not be used in advertising or otherwise to promote the
sale, use or other dealings in this Software without prior written
authorization from The XFree86 Project or Silicon Motion.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "smi.h"
#include "smi_crtc.h"
#include "smi_501.h"

/* Want to see register dumps for now */
#undef VERBLEV
#define VERBLEV		1

/*
 * Prototypes
 */
static void SMI501_CrtcHideCursor(xf86CrtcPtr crtc);

/*
 * Implementation
 */
static void
SMI501_CrtcVideoInit_lcd(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    MSOCRegPtr mode = pSmi->mode;
    int		pitch, width;

    ENTER();

    if (!pSmi->HwCursor)
	SMI501_CrtcHideCursor(crtc);

    mode->panel_display_ctl.value = READ_SCR(pSmi, PANEL_DISPLAY_CTL);
    mode->panel_fb_width.value = READ_SCR(pSmi, PANEL_FB_WIDTH);

    mode->panel_display_ctl.f.format =
	pScrn->bitsPerPixel == 8 ? 0 :
	pScrn->bitsPerPixel == 16 ? 1 : 2;

    pitch = (((crtc->rotatedData? crtc->mode.HDisplay : pScrn->displayWidth) *
	      pSmi->Bpp) + 15) & ~15;
    width = ((crtc->mode.HDisplay * pSmi->Bpp) + 15) & ~ 15;

    /* >> 4 because of the "unused bits" that should be set to 0 */
    mode->panel_fb_width.f.offset = pitch >> 4;
    mode->panel_fb_width.f.width = width >> 4;

    mode->panel_display_ctl.f.gamma = pSmi->Bpp > 1;

    WRITE_SCR(pSmi, PANEL_DISPLAY_CTL, mode->panel_display_ctl.value);
    WRITE_SCR(pSmi, PANEL_FB_WIDTH, mode->panel_fb_width.value);

    LEAVE();
}

static void
SMI501_CrtcVideoInit_crt(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    MSOCRegPtr mode = pSmi->mode;
    int		pitch, width;

    ENTER();

    if (!pSmi->HwCursor)
	SMI501_CrtcHideCursor(crtc);

    mode->crt_display_ctl.value = READ_SCR(pSmi, CRT_DISPLAY_CTL);
    mode->crt_fb_width.value = READ_SCR(pSmi, CRT_FB_WIDTH);

    mode->crt_display_ctl.f.format =
	pScrn->bitsPerPixel == 8 ? 0 :
	pScrn->bitsPerPixel == 16 ? 1 : 2;

    pitch = (((crtc->rotatedData? crtc->mode.HDisplay : pScrn->displayWidth) *
	      pSmi->Bpp) + 15) & ~15;
    width = ((crtc->mode.HDisplay * pSmi->Bpp) + 15) & ~ 15;

    /* >> 4 because of the "unused bits" that should be set to 0 */
    mode->crt_fb_width.f.offset = pitch >> 4;
    mode->crt_fb_width.f.width = width >> 4;

    mode->crt_display_ctl.f.gamma = pSmi->Bpp > 1;

    WRITE_SCR(pSmi, CRT_DISPLAY_CTL, mode->crt_display_ctl.value);
    WRITE_SCR(pSmi, CRT_FB_WIDTH, mode->crt_fb_width.value);

    LEAVE();
}

static void
SMI501_CrtcAdjustFrame(xf86CrtcPtr crtc, int x, int y)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    xf86CrtcConfigPtr crtcConf = XF86_CRTC_CONFIG_PTR(pScrn);
    MSOCRegPtr mode = pSmi->mode;
    CARD32 Base;

    ENTER();

    if(crtc->rotatedData)
	Base = (char*)crtc->rotatedData - (char*)pSmi->FBBase;
    else
	Base = pSmi->FBOffset + (x + y * pScrn->displayWidth) * pSmi->Bpp;

    Base = (Base + 15) & ~15;

    if (crtc == crtcConf->crtc[0]) {
	mode->panel_fb_address.f.address = Base >> 4;
	mode->panel_fb_address.f.pending = 1;
	WRITE_SCR(pSmi, PANEL_FB_ADDRESS, mode->panel_fb_address.value);
    }
    else {
	mode->crt_display_ctl.f.pixel = ((x * pSmi->Bpp) & 15) / pSmi->Bpp;
	WRITE_SCR(pSmi, CRT_DISPLAY_CTL, mode->crt_display_ctl.value);
	mode->crt_fb_address.f.address = Base >> 4;
	mode->crt_fb_address.f.mselect = 0;
	mode->crt_fb_address.f.pending = 1;
	WRITE_SCR(pSmi, CRT_FB_ADDRESS, mode->crt_fb_address.value);
    }

    LEAVE();
}

static void
SMI501_CrtcModeSet_lcd(xf86CrtcPtr crtc,
		       DisplayModePtr xf86mode,
		       DisplayModePtr adjusted_mode,
		       int x, int y)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    MSOCRegPtr mode = pSmi->mode;
    double	p2_diff, pll_diff;
    int32_t	x2_select, x2_divider, x2_shift, x2_1xclck;

    ENTER();

    /* Initialize the display controller */

    SMI501_CrtcVideoInit_lcd(crtc);

    /* P2CLK have dividers 1, 3 and 5 */
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		   "Clock request %5.2f (max_divider %d)\n",
		   (double)xf86mode->Clock, 5);
    p2_diff = SMI501_FindClock(xf86mode->Clock, 5,
			       (uint32_t)mode->device_id.f.revision >= 0xc0,
			       &x2_1xclck, &x2_select, &x2_divider,
			       &x2_shift);
    mode->clock.f.p2_select = x2_select;
    mode->clock.f.p2_divider = x2_divider;
    mode->clock.f.p2_shift = x2_shift;
    mode->clock.f.p2_1xclck = x2_1xclck;

    /* Check if it is a SMI 502 */
    /* FIXME May need to add a Boolean option here, (or use extra
     * xorg.conf options?) to force it to not use 502 mode set. */
    if ((uint32_t)mode->device_id.f.revision >= 0xc0) {
	int32_t	m, n, xclck;

	pll_diff = SMI501_FindPLLClock(xf86mode->Clock, &m, &n, &xclck);
	if (pll_diff < p2_diff) {

	    /* Zero the pre 502 bitfield */
	    mode->clock.f.p2_select  = 0;
	    mode->clock.f.p2_divider = 0;
	    mode->clock.f.p2_shift   = 0;
	    mode->clock.f.p2_1xclck  = 0;

	    mode->clock.f.pll_select = 1;
	    mode->pll_ctl.f.m = m;
	    mode->pll_ctl.f.n = n;

	    /* 0: Crystal input
	     * 1: Test clock input */
	    mode->pll_ctl.f.select = 0;

	    /* 0: pll output divided by 1
	     * 1: pll output divided by 2 */
	    mode->pll_ctl.f.divider = xclck != 1;
	    mode->pll_ctl.f.power = 1;
	}
	else
	    mode->clock.f.pll_select = 0;
    }
    else
	mode->clock.f.pll_select = 0;

    mode->panel_display_ctl.f.enable = 1;
    mode->panel_display_ctl.f.timing = 1;

    mode->panel_wwidth.f.x = 0;
    mode->panel_wwidth.f.width = xf86mode->HDisplay;

    mode->panel_wheight.f.y = 0;
    mode->panel_wheight.f.height = xf86mode->VDisplay;

#ifdef USE_PANEL_CENTER
    mode->panel_plane_tl.f.left = (pSmi->lcdWidth - xf86mode->HDisplay) >> 1;
    mode->panel_plane_tl.f.top = (pSmi->lcdHeight - xf86mode->VDisplay) >> 1;

    mode->panel_plane_br.f.right = mode->panel_plane_tl.f.left +
	xf86mode->HDisplay - 1;
    mode->panel_plane_br.f.bottom = mode->panel_plane_tl.f.top +
	xf86mode->VDisplay - 1;
#else
    mode->panel_plane_tl.f.left = 0;
    mode->panel_plane_tl.f.top = 0;

    mode->panel_plane_br.f.right = xf86mode->HDisplay - 1;
    mode->panel_plane_br.f.bottom = xf86mode->VDisplay - 1;
#endif

    /* 0 means pulse high */
    mode->panel_display_ctl.f.hsync = !(xf86mode->Flags & V_PHSYNC);
    mode->panel_display_ctl.f.vsync = !(xf86mode->Flags & V_PVSYNC);

    mode->panel_htotal.f.total = xf86mode->HTotal - 1;
    mode->panel_htotal.f.end = xf86mode->HDisplay - 1;

    mode->panel_hsync.f.start = xf86mode->HSyncStart - 1;
    mode->panel_hsync.f.width = xf86mode->HSyncEnd -
	xf86mode->HSyncStart;

    mode->panel_vtotal.f.total = xf86mode->VTotal - 1;
    mode->panel_vtotal.f.end = xf86mode->VDisplay - 1;

    mode->panel_vsync.f.start = xf86mode->VSyncStart;
    mode->panel_vsync.f.height = xf86mode->VSyncEnd -
	xf86mode->VSyncStart;


    SMI501_WriteMode_lcd(pScrn,mode);
    SMI501_CrtcAdjustFrame(crtc, x, y);

    LEAVE();
}

static void
SMI501_CrtcModeSet_crt(xf86CrtcPtr crtc,
		       DisplayModePtr xf86mode,
		       DisplayModePtr adjusted_mode,
		       int x, int y)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    MSOCRegPtr mode = pSmi->mode;
    int32_t	x2_select, x2_divider, x2_shift, x2_1xclck;

    ENTER();

    /* Initialize the display controller */

    SMI501_CrtcVideoInit_crt(crtc);

    /* V2CLK have dividers 1 and 3 */
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		   "Clock request %5.2f (max_divider %d)\n",
		   (double)xf86mode->Clock, 3);
    (void)SMI501_FindClock(xf86mode->Clock, 3,
			   (uint32_t)mode->device_id.f.revision >= 0xc0,
			   &x2_1xclck, &x2_select, &x2_divider, &x2_shift);
    mode->clock.f.v2_select = x2_select;
    mode->clock.f.v2_divider = x2_divider;
    mode->clock.f.v2_shift = x2_shift;
    mode->clock.f.v2_1xclck = x2_1xclck;

    /* 0: select panel - 1: select crt */
    mode->crt_display_ctl.f.select = 1;
    mode->crt_display_ctl.f.enable = 1;
    mode->crt_display_ctl.f.timing = 1;
    /* 0: show pixels - 1: blank */
    mode->crt_display_ctl.f.blank = 0;

    mode->crt_fb_address.f.mextern = 0;	/* local memory */

    /* 0 means pulse high */
    mode->crt_display_ctl.f.hsync = !(xf86mode->Flags & V_PHSYNC);
    mode->crt_display_ctl.f.vsync = !(xf86mode->Flags & V_PVSYNC);

    mode->crt_htotal.f.total = xf86mode->HTotal - 1;
    mode->crt_htotal.f.end = xf86mode->HDisplay - 1;

    mode->crt_hsync.f.start = xf86mode->HSyncStart - 1;
    mode->crt_hsync.f.width = xf86mode->HSyncEnd -
	xf86mode->HSyncStart;

    mode->crt_vtotal.f.total = xf86mode->VTotal - 1;
    mode->crt_vtotal.f.end = xf86mode->VDisplay - 1;

    mode->crt_vsync.f.start = xf86mode->VSyncStart;
    mode->crt_vsync.f.height = xf86mode->VSyncEnd -
	xf86mode->VSyncStart;

    SMI501_WriteMode_crt(pScrn,mode);
    SMI501_CrtcAdjustFrame(crtc, x, y);

    LEAVE();
}

static void
SMI501_CrtcLoadLUT(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    xf86CrtcConfigPtr crtcConf = XF86_CRTC_CONFIG_PTR(pScrn);
    SMICrtcPrivatePtr crtcPriv = SMICRTC(crtc);
    int i,port;

    ENTER();

    port = crtc == crtcConf->crtc[0] ? PANEL_PALETTE : CRT_PALETTE;
    for (i = 0; i < 256; i++)
	WRITE_SCR(pSmi, port + (i  <<  2),
		  (crtcPriv->lut_r[i] >> 8 << 16) |
		  (crtcPriv->lut_g[i] >> 8 << 8) |
		  (crtcPriv->lut_b[i] >> 8) );

    LEAVE();
}

static void
SMI501_CrtcSetCursorColors(xf86CrtcPtr crtc, int bg, int fg)
{
    ScrnInfoPtr		pScrn = crtc->scrn;
    SMIPtr		pSmi = SMIPTR(pScrn);
    xf86CrtcConfigPtr	crtcConf = XF86_CRTC_CONFIG_PTR(pScrn);
    int32_t		port, value;

    ENTER();

    /* for the SMI501 HWCursor, there are 4 possible colors, one of which
     * is transparent:	M,S:  0,0 = Transparent
     *						      0,1 = color 1
     *						      1,0 = color 2
     *						      1,1 = color 3
     *	To simplify implementation, we use color2 == bg and
     *					   color3 == fg
     *	Color 1 is don't care, so we set it to color 2's value
     */

    /* Pack the true color components into 16 bit RGB -- 5:6:5 */
    value = ((bg & 0xF80000) >> 8 |
	     (bg & 0x00FC00) >> 5 |
	     (bg & 0x0000F8) >> 3);

    value |= ((bg & 0xF80000) <<  8 |
	      (bg & 0x00FC00) << 11 |
	      (bg & 0x0000F8) << 13);
    port = crtc == crtcConf->crtc[0] ? 0x00f8 : 0x0238;
    WRITE_DCR(pSmi, port, value);

    value = ((fg & 0xF80000) >> 8 |
	     (fg & 0x00FC00) >> 5 |
	     (fg & 0x0000F8) >> 3);
    port = crtc == crtcConf->crtc[0] ? 0x00fc : 0x023c;
    WRITE_DCR(pSmi, port, value);

    LEAVE();
}

static void
SMI501_CrtcSetCursorPosition(xf86CrtcPtr crtc, int x, int y)
{
    ScrnInfoPtr		pScrn = crtc->scrn;
    SMIPtr		pSmi = SMIPTR(pScrn);
    xf86CrtcConfigPtr	crtcConf;
#if SMI_CURSOR_ALPHA_PLANE
    SMICrtcPrivatePtr	smi_crtc = SMICRTC(crtc);
    MSOCRegPtr		mode;
#endif
    int32_t		port, offset;

    ENTER();

#if SMI_CURSOR_ALPHA_PLANE
    if (smi_crtc->argb_cursor) {
	mode = pSmi->mode;

	/* uncomment next line if you want to see it rendering the cursor */
	/* x = y = 0; */

	mode->alpha_plane_tl.f.left = x;
	mode->alpha_plane_tl.f.top = y;

	mode->alpha_plane_br.f.right = x + SMI501_CURSOR_SIZE - 1;
	mode->alpha_plane_br.f.bottom = y + SMI501_CURSOR_SIZE - 1;

	WRITE_SCR(pSmi, ALPHA_PLANE_TL, mode->alpha_plane_tl.value);
	WRITE_SCR(pSmi, ALPHA_PLANE_BR, mode->alpha_plane_br.value);
    }
    else
#endif
    {
	crtcConf = XF86_CRTC_CONFIG_PTR(pScrn);

	if (x >= 0)
	    offset = x & SMI501_MASK_MAXBITS;
	else
	    offset = (-x & SMI501_MASK_MAXBITS) | SMI501_MASK_BOUNDARY;

	if (y >= 0)
	    offset |= (y & SMI501_MASK_MAXBITS) << 16;
	else
	    offset |= ((-y & SMI501_MASK_MAXBITS) | SMI501_MASK_BOUNDARY) << 16;

	port = crtc == crtcConf->crtc[0] ? 0x00f4 : 0x0234;
	WRITE_DCR(pSmi, port, offset);
    }

    LEAVE();
}

static void
SMI501_CrtcShowCursor(xf86CrtcPtr crtc)
{
    ScrnInfoPtr		pScrn = crtc->scrn;
    SMIPtr		pSmi = SMIPTR(pScrn);
    xf86CrtcConfigPtr	crtcConf;
#if SMI_CURSOR_ALPHA_PLANE
    SMICrtcPrivatePtr	smi_crtc = SMICRTC(crtc);
    MSOCRegPtr		mode;
#endif
    int32_t		port, value;

    ENTER();

#if SMI_CURSOR_ALPHA_PLANE
    if (smi_crtc->argb_cursor) {
	mode = pSmi->mode;

	mode->alpha_display_ctl.f.enable = 1;
	WRITE_SCR(pSmi, ALPHA_DISPLAY_CTL, mode->alpha_display_ctl.value);
    }
    else
#endif
    {
	crtcConf = XF86_CRTC_CONFIG_PTR(pScrn);

	port = crtc == crtcConf->crtc[0] ? 0x00f0 : 0x0230;
	value = READ_DCR(pSmi, port);
	value |= SMI501_MASK_HWCENABLE;
	WRITE_DCR(pSmi, port, value);
    }

    LEAVE();
}

static void
SMI501_CrtcHideCursor(xf86CrtcPtr crtc)
{
    ScrnInfoPtr		pScrn = crtc->scrn;
    SMIPtr		pSmi = SMIPTR(pScrn);
    xf86CrtcConfigPtr	crtcConf;
#if SMI_CURSOR_ALPHA_PLANE
    SMICrtcPrivatePtr	smi_crtc = SMICRTC(crtc);
    MSOCRegPtr		mode;
#endif
    int32_t		port, value;

    ENTER();

#if SMI_CURSOR_ALPHA_PLANE
    if (smi_crtc->argb_cursor) {
	mode = pSmi->mode;

	mode->alpha_display_ctl.f.enable = 0;
	WRITE_SCR(pSmi, ALPHA_DISPLAY_CTL, mode->alpha_display_ctl.value);
    }
    else
#endif
    {
	crtcConf = XF86_CRTC_CONFIG_PTR(pScrn);

	port = crtc == crtcConf->crtc[0] ? 0x00f0 : 0x0230;
	value = READ_DCR(pSmi, port);
	value &= ~SMI501_MASK_HWCENABLE;
	WRITE_DCR(pSmi, port, value);
    }

    LEAVE();
}

static void
SMI501_CrtcLoadCursorImage(xf86CrtcPtr crtc, CARD8 *image)
{
    ScrnInfoPtr		pScrn = crtc->scrn;
    SMIPtr		pSmi = SMIPTR(pScrn);
#if SMI_CURSOR_ALPHA_PLANE
    SMICrtcPrivatePtr	smi_crtc = SMICRTC(crtc);
#endif
    xf86CrtcConfigPtr	crtcConf = XF86_CRTC_CONFIG_PTR(pScrn);
    int32_t		port, value;

    ENTER();

    port = crtc == crtcConf->crtc[0] ? 0x00f0 : 0x0230;
    value = pSmi->FBCursorOffset + (port == 0x00f0 ? 0 : SMI501_CURSOR_SIZE);
    WRITE_DCR(pSmi, port, value);
    memcpy(pSmi->FBBase + value, image,
	   /* FIXME 1024, but then, should not be using 64x64 cursors */
	   (SMI501_MAX_CURSOR >> 2) * SMI501_MAX_CURSOR);
#if SMI_CURSOR_ALPHA_PLANE
    smi_crtc->argb_cursor = FALSE;
#endif

    LEAVE();
}

#if SMI_CURSOR_ALPHA_PLANE
static void
SMI501_CrtcLoadCursorArgb(xf86CrtcPtr crtc, CARD32 *image)
{
    ScrnInfoPtr		 pScrn = crtc->scrn;
    SMIPtr		 pSmi = SMIPTR(pScrn);
    SMICrtcPrivatePtr	 smi_crtc = SMICRTC(crtc);
    MSOCRegPtr		 mode = pSmi->mode;
    int16_t		*framebuffer;
    int32_t		 x, y, bits;
    int32_t		 format;

    ENTER();

#define ALPHA_RGB_565		1
#define ALPHA_ARGB_4444		3

    /* select alpha format */
    mode->alpha_display_ctl.f.format = ALPHA_ARGB_4444;

    /* 0: use per pixel alpha value  1: use alpha value specified in alpha */
    if (mode->alpha_display_ctl.f.format == ALPHA_RGB_565) {
	mode->alpha_display_ctl.f.select = 1;
	/* 0 to 15, with 0 being transparent and 15 opaque */
	mode->alpha_display_ctl.f.alpha = 7;
    }
    else {
	/* use per pixel alpha */
	mode->alpha_display_ctl.f.select = 0;
    }

    /* alpha layer buffer */
    mode->alpha_fb_address.value = 0;
    mode->alpha_fb_address.f.address = pSmi->FBCursorOffset >> 4;

    /* more clearly: width = (SMI501_MAX_CURSOR << 1) >> 4
     * as the structure is matching the register spec, where it says
     * the first 4 bits are hardwired to zero */
    mode->alpha_fb_width.f.offset = SMI501_MAX_CURSOR >> 3;
    mode->alpha_fb_width.f.width = SMI501_MAX_CURSOR >> 3;

    mode->alpha_chroma_key.f.value = 0;
    mode->alpha_chroma_key.f.mask = 0;
    /* enable chroma key */
    mode->alpha_display_ctl.f.chromakey = 1;

    framebuffer = (int16_t *)(pSmi->FBBase + pSmi->FBCursorOffset);
    if (mode->alpha_display_ctl.f.format == ALPHA_RGB_565) {
	/* convert image to rgb 5:6:5 */
	for (y = 0; y < SMI501_MAX_CURSOR; y++) {
	    for (x = 0; x < SMI501_MAX_CURSOR; x++) {
		bits = image[y * SMI501_MAX_CURSOR + x];
		framebuffer[y * SMI501_MAX_CURSOR + x] =
		(((bits & 0xf80000) >> 8) |
		 ((bits & 0x00fc00) >> 5) |
		 ((bits & 0x0000f8) >> 3));
	    }
	}
    }
    else {
	/* convert image to argb 4:4:4:4 */
	for (y = 0; y < SMI501_MAX_CURSOR; y++) {
	    for (x = 0; x < SMI501_MAX_CURSOR; x++) {
		bits = image[y * SMI501_MAX_CURSOR + x];
		framebuffer[y * SMI501_MAX_CURSOR + x] =
		(((bits & 0xf0000000) >> 16) |
		 ((bits & 0x00f00000) >> 12) |
		 ((bits & 0x0000f000) >>  8) |
		 ((bits & 0x000000f0) >>  4));
	    }
	}
    }
    SMI501_WriteMode_alpha(pScrn, mode);
    smi_crtc->argb_cursor = TRUE;
 
     LEAVE();
 }
#endif

Bool
SMI501_CrtcPreInit(ScrnInfoPtr pScrn)
{
    SMIPtr	pSmi = SMIPTR(pScrn);
    xf86CrtcPtr crtc;
    xf86CrtcFuncsPtr crtcFuncs;
    SMICrtcPrivatePtr crtcPriv;

    ENTER();

    /* CRTC0 is LCD */
    SMI_CrtcFuncsInit_base(&crtcFuncs, &crtcPriv);
    crtcFuncs->mode_set		= SMI501_CrtcModeSet_lcd;
    crtcPriv->adjust_frame	= SMI501_CrtcAdjustFrame;
    crtcPriv->video_init	= SMI501_CrtcVideoInit_lcd;
    crtcPriv->load_lut		= SMI501_CrtcLoadLUT;

    if (pSmi->HwCursor) {
	crtcFuncs->set_cursor_colors = SMI501_CrtcSetCursorColors;
	crtcFuncs->set_cursor_position = SMI501_CrtcSetCursorPosition;
	crtcFuncs->show_cursor = SMI501_CrtcShowCursor;
	crtcFuncs->hide_cursor = SMI501_CrtcHideCursor;
	crtcFuncs->load_cursor_image = SMI501_CrtcLoadCursorImage;
#if SMI_CURSOR_ALPHA_PLANE
	if (!pSmi->Dualhead)
	    crtcFuncs->load_cursor_argb = SMI501_CrtcLoadCursorArgb;
#endif
    }

    if (! (crtc = xf86CrtcCreate(pScrn, crtcFuncs)))
	LEAVE(FALSE);
    crtc->driver_private = crtcPriv;

    /* CRTC1 is CRT */
    if (pSmi->Dualhead) {
	SMI_CrtcFuncsInit_base(&crtcFuncs, &crtcPriv);
	crtcFuncs->mode_set	= SMI501_CrtcModeSet_crt;
	crtcPriv->adjust_frame	= SMI501_CrtcAdjustFrame;
	crtcPriv->video_init	= SMI501_CrtcVideoInit_crt;
	crtcPriv->load_lut	= SMI501_CrtcLoadLUT;

	if (pSmi->HwCursor) {
	    crtcFuncs->set_cursor_colors = SMI501_CrtcSetCursorColors;
	    crtcFuncs->set_cursor_position = SMI501_CrtcSetCursorPosition;
	    crtcFuncs->show_cursor = SMI501_CrtcShowCursor;
	    crtcFuncs->hide_cursor = SMI501_CrtcHideCursor;
	    crtcFuncs->load_cursor_image = SMI501_CrtcLoadCursorImage;
	}

	if (! (crtc = xf86CrtcCreate(pScrn, crtcFuncs)))
	    LEAVE(FALSE);
	crtc->driver_private = crtcPriv;
    }

    LEAVE(TRUE);
}

