/*
 * Acceleration for the Creator and Creator3D framebuffer.
 *
 * Copyright (C) 1998,1999,2000 Jakub Jelinek (jakub@redhat.com)
 * Copyright (C) 1998 Michal Rehacek (majkl@iname.com)
 * Copyright (C) 1999,2000 David S. Miller (davem@redhat.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * JAKUB JELINEK, MICHAL REHACEK, OR DAVID MILLER BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include	"scrnintstr.h"
#include	"pixmapstr.h"
#include	"regionstr.h"
#include	"mistruct.h"
#include	"miline.h"
#include	"fb.h"

#include	"ffb.h"
#include	"ffb_fifo.h"
#include	"ffb_rcache.h"
#include	"ffb_loops.h"
#include	"ffb_regs.h"

#ifdef HAVE_XAA_H
/* VISmoveImage.s */
extern void VISmoveImageRL(unsigned char *src, unsigned char *dst, long w, long h, long skind, long dkind);
extern void VISmoveImageLR(unsigned char *src, unsigned char *dst, long w, long h, long skind, long dkind);

/* Indexed by ffb resolution enum. */
struct fastfill_parms ffb_fastfill_parms[] = {
	/* fsmall, psmall,  ffh,  ffw,  pfh,  pfw */
	{  0x00c0, 0x1400, 0x04, 0x08, 0x10, 0x50 },	/* Standard: 1280 x 1024 */
	{  0x0140, 0x2800, 0x04, 0x10, 0x10, 0xa0 },	/* High:     1920 x 1360 */
	{  0x0080, 0x0a00, 0x02, 0x08, 0x08, 0x50 },	/* Stereo:   960  x 580  */
/*XXX*/	{  0x00c0, 0x0a00, 0x04, 0x08, 0x08, 0x50 },	/* Portrait: 1280 x 2048 XXX */
};

void
CreatorVtChange (ScreenPtr pScreen, int enter)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	FFBPtr pFfb = GET_FFB_FROM_SCRN (pScrn);
	ffb_fbcPtr ffb = pFfb->regs;

	pFfb->rp_active = 1;
	FFBWait(pFfb, ffb);     
	pFfb->fifo_cache = -1;
	pFfb->fbc_cache = (FFB_FBC_WB_A | FFB_FBC_WM_COMBINED |
			   FFB_FBC_RB_A | FFB_FBC_SB_BOTH| FFB_FBC_XE_OFF |
			   FFB_FBC_ZE_OFF | FFB_FBC_YE_OFF | FFB_FBC_RGBE_MASK);
	pFfb->ppc_cache = (FFB_PPC_FW_DISABLE |
			   FFB_PPC_VCE_DISABLE | FFB_PPC_APE_DISABLE | FFB_PPC_CS_CONST |
			   FFB_PPC_XS_CONST | FFB_PPC_YS_CONST | FFB_PPC_ZS_CONST|
			   FFB_PPC_DCE_DISABLE | FFB_PPC_ABE_DISABLE | FFB_PPC_TBE_OPAQUE);

	pFfb->pmask_cache = ~0;
	pFfb->rop_cache = FFB_ROP_EDIT_BIT;
	pFfb->drawop_cache = FFB_DRAWOP_RECTANGLE;
	pFfb->fg_cache = pFfb->bg_cache = 0;
	pFfb->fontw_cache = 32;
	pFfb->fontinc_cache = (1 << 16) | 0;
	pFfb->laststipple = NULL;
	FFBFifo(pFfb, 9);
	ffb->fbc = pFfb->fbc_cache;
	ffb->ppc = pFfb->ppc_cache;
	ffb->pmask = pFfb->pmask_cache;
	ffb->rop = pFfb->rop_cache;
	ffb->drawop = pFfb->drawop_cache;
	ffb->fg = pFfb->fg_cache;
	ffb->bg = pFfb->bg_cache;
	ffb->fontw = pFfb->fontw_cache;
	ffb->fontinc = pFfb->fontinc_cache;
	pFfb->rp_active = 1;
	FFBWait(pFfb, ffb);

	/* Fixup the FBC/PPC caches to deal with actually using
	 * a WID for every ROP.
	 */
	pFfb->fbc_cache = (FFB_FBC_WB_A | FFB_FBC_WM_COMBINED |
			   FFB_FBC_RB_A | FFB_FBC_SB_BOTH | FFB_FBC_XE_ON |
			   FFB_FBC_ZE_OFF | FFB_FBC_YE_OFF | FFB_FBC_RGBE_ON);
	pFfb->ppc_cache &= ~FFB_PPC_XS_MASK;
	pFfb->ppc_cache |= FFB_PPC_XS_WID;
	pFfb->wid_cache = (enter ? pFfb->xaa_wid : 0xff);
	FFBFifo(pFfb, 11);
	ffb->fbc = pFfb->fbc_cache;
	ffb->ppc = FFB_PPC_XS_WID;
	ffb->wid = pFfb->wid_cache;
	ffb->xpmask = 0xff;
	ffb->xclip = FFB_XCLIP_TEST_ALWAYS;
	ffb->cmp = 0x80808080;
	ffb->matchab = 0x80808080;
	ffb->magnab = 0x80808080;
	ffb->blendc = (FFB_BLENDC_FORCE_ONE |
		       FFB_BLENDC_DF_ONE_M_A |
		       FFB_BLENDC_SF_A);
	ffb->blendc1 = 0;
	ffb->blendc2 = 0;
	pFfb->rp_active = 1;
	FFBWait(pFfb, ffb);

	if (enter) {
		pFfb->drawop_cache = FFB_DRAWOP_RECTANGLE;
		
		FFBFifo(pFfb, 5);
		ffb->drawop = pFfb->drawop_cache;
		FFB_WRITE64(&ffb->by, 0, 0);
		FFB_WRITE64_2(&ffb->bh, pFfb->psdp->height, pFfb->psdp->width);
		pFfb->rp_active = 1;
		FFBWait(pFfb, ffb);

		SET_SYNC_FLAG(pFfb->pXAAInfo);
	}
}

#ifdef DEBUG_FFB
FILE *FDEBUG_FD = NULL;
#endif

#define FFB_ATTR_SFB_VAR_XAA(__fpriv, __pmask, __alu) \
do {   unsigned int __ppc = FFB_PPC_ABE_DISABLE | FFB_PPC_APE_DISABLE | FFB_PPC_CS_VAR | FFB_PPC_XS_WID; \
       unsigned int __ppc_mask = FFB_PPC_ABE_MASK | FFB_PPC_APE_MASK | FFB_PPC_CS_MASK | FFB_PPC_XS_MASK; \
       unsigned int __rop = (FFB_ROP_EDIT_BIT | (__alu))|(FFB_ROP_NEW<<8); \
       unsigned int __fbc = (__fpriv)->xaa_fbc; \
       unsigned int __wid = (__fpriv)->xaa_wid; \
       if (((__fpriv)->ppc_cache & __ppc_mask) != __ppc || \
           (__fpriv)->fbc_cache != __fbc || \
           (__fpriv)->wid_cache != __wid || \
           (__fpriv)->rop_cache != __rop || \
           (__fpriv)->pmask_cache != (__pmask)) \
               __FFB_Attr_SFB_VAR(__fpriv, __ppc, __ppc_mask, __fbc, \
                                  __wid, __rop, (__pmask)); \
} while(0)

#define FFB_ATTR_VSCROLL_XAA(__fpriv, __pmask) \
do {   unsigned int __rop = (FFB_ROP_OLD | (FFB_ROP_OLD << 8)); \
       unsigned int __fbc = (__fpriv)->xaa_fbc; \
       if ((__fpriv)->fbc_cache != __fbc || \
           (__fpriv)->rop_cache != __rop || \
           (__fpriv)->pmask_cache != (__pmask) || \
           (__fpriv)->drawop_cache != FFB_DRAWOP_VSCROLL) { \
               ffb_fbcPtr __ffb = (__fpriv)->regs; \
               (__fpriv)->fbc_cache = __fbc; \
               (__fpriv)->rop_cache = __rop; \
               (__fpriv)->pmask_cache = (__pmask); \
               (__fpriv)->drawop_cache = FFB_DRAWOP_VSCROLL; \
               (__fpriv)->rp_active = 1; \
               FFBFifo(__fpriv, 4); \
               (__ffb)->fbc = __fbc; \
               (__ffb)->rop = __rop; \
               (__ffb)->pmask = (__pmask); \
               (__ffb)->drawop = FFB_DRAWOP_VSCROLL; \
       } \
} while(0)

static CARD32 FFBAlphaTextureFormats[2] = { PICT_a8, 0 };
static CARD32 FFBTextureFormats[2] = { PICT_a8b8g8r8, 0 };

static void FFB_SetupTextureAttrs(FFBPtr pFfb)
{
       ffb_fbcPtr ffb = pFfb->regs;
       unsigned int ppc = FFB_PPC_APE_DISABLE | FFB_PPC_CS_VAR | FFB_PPC_XS_VAR;
       unsigned int ppc_mask = FFB_PPC_APE_MASK | FFB_PPC_CS_MASK | FFB_PPC_XS_MASK;
       unsigned int rop = FFB_ROP_NEW | (FFB_ROP_NEW << 8);
       unsigned int fbc = pFfb->xaa_fbc;
       unsigned int wid = pFfb->xaa_wid;

       ppc |= FFB_PPC_ABE_ENABLE;
       ppc_mask |= FFB_PPC_ABE_MASK;

       if ((pFfb->ppc_cache & ppc_mask) != ppc ||
           pFfb->fbc_cache != fbc ||
           pFfb->wid_cache != wid ||
           pFfb->rop_cache != rop ||
           pFfb->pmask_cache != 0xffffffff)
               __FFB_Attr_SFB_VAR(pFfb, ppc, ppc_mask, fbc,
                                  wid, rop, 0xffffffff);
       FFBWait(pFfb, ffb);
}

static Bool FFB_SetupForCPUToScreenAlphaTexture(ScrnInfoPtr pScrn, int op,
                                               CARD16 red, CARD16 green, CARD16 blue,
                                               CARD16 alpha, int alphaType,
                                               CARD8 *alphaPtr, int alphaPitch,
                                               int width, int height, int flags)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);

       FFBLOG(("FFB_SetupForCPUToScreenAlphaTexture: "
               "argb[%04x:%04x:%04x:%04x] alpha[T(%x):P(%d)] "
               "wh[%d:%d] flgs[%x]\n",
               alpha, red, green, blue,
               alphaType, alphaPitch,
               width, height, flags));

       FFB_SetupTextureAttrs(pFfb);

       pFfb->xaa_tex = (unsigned char *) alphaPtr;
       pFfb->xaa_tex_pitch = alphaPitch;
       pFfb->xaa_tex_width = width;
       pFfb->xaa_tex_height = height;
       pFfb->xaa_tex_color = (/*((alpha >> 8) << 24) |*/
                              ((blue >> 8) << 16) |
                              ((green >> 8) << 8) |
                              ((red >> 8) << 0));
       return TRUE;
}

static void FFB_SubsequentCPUToScreenAlphaTexture(ScrnInfoPtr pScrn,
                                                 int dstx, int dsty,
                                                 int srcx, int srcy,
                                                 int width, int height)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       unsigned char *dst_base, *alpha_base, *sfb32;
       unsigned int pixel_base;
       int psz_shift = 2;

       FFBLOG(("FFB_SubsequentCPUToScreenAlphaTexture: "
               "dst[%d:%d] src[%d:%d] wh[%d:%d]\n",
               dstx, dsty, srcx, srcy, width, height));

       sfb32 = (unsigned char *) pFfb->sfb32;
       dst_base = sfb32 + (dsty * (2048 << psz_shift)) + (dstx << psz_shift);
       alpha_base = pFfb->xaa_tex;
       alpha_base += srcx;
       if (srcy)
               alpha_base += (srcy * pFfb->xaa_tex_pitch);
       pixel_base = pFfb->xaa_tex_color;
       while (height--) {
               unsigned int *dst = (unsigned int *) dst_base;
               unsigned char *alpha = alpha_base;
               int w = width;

               while (w--) {
                       (*dst) = (((unsigned int)*alpha << 24) | pixel_base);
                       dst++;
                       alpha++;
	       }
	       dst_base += (2048 << psz_shift);
	       alpha_base += pFfb->xaa_tex_pitch;
       }
}


static Bool FFB_SetupForCPUToScreenTexture(ScrnInfoPtr pScrn, int op,
                                          int texType,
                                          CARD8 *texPtr, int texPitch,
                                          int width, int height, int flags)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);

       FFBLOG(("FFB_SetupForCPUToScreenTexture: "
               "TEX[T(%x):P(%d)] "
               "wh[%d:%d] flgs[%x]\n",
               texType, texPitch,
               width, height, flags));

       FFB_SetupTextureAttrs(pFfb);

       pFfb->xaa_tex = (unsigned char *) texPtr;
       pFfb->xaa_tex_pitch = texPitch;
       pFfb->xaa_tex_width = width;
       pFfb->xaa_tex_height = height;

       return TRUE;
}

static void FFB_SubsequentCPUToScreenTexture(ScrnInfoPtr pScrn,
                                            int dstx, int dsty,
                                            int srcx, int srcy,
                                            int width, int height)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       unsigned char *dst_base, *sfb32;
       unsigned int *tex_base;
       int psz_shift = 2;

       FFBLOG(("FFB_SubsequentCPUToScreenAlphaTexture: "
               "dst[%d:%d] src[%d:%d] wh[%d:%d]\n",
               dstx, dsty, srcx, srcy, width, height));

       sfb32 = (unsigned char *) pFfb->sfb32;
       dst_base = sfb32 + (dsty * (2048 << psz_shift)) + (dstx << psz_shift);
       tex_base = (unsigned int *) pFfb->xaa_tex;
       tex_base += srcx;
       if (srcy)
               tex_base += (srcy * pFfb->xaa_tex_pitch);
       while (height--) {
               unsigned int *dst = (unsigned int *) dst_base;
               unsigned int *tex = tex_base;
               int w = width;
               while (w--) {
                       (*dst) = *tex;

                       dst++;
                       tex++;
               }
               dst_base += (2048 << psz_shift);
               tex_base += pFfb->xaa_tex_pitch;
       }
}

static void FFB_WritePixmap(ScrnInfoPtr pScrn,
                           int x, int y, int w, int h,
                           unsigned char *src,
                           int srcwidth,
                           int rop,
                           unsigned int planemask,
                           int trans, int bpp, int depth)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       unsigned char *dst, *sfb32;
       int psz_shift = 2;
       ffb_fbcPtr ffb = pFfb->regs;

       FFBLOG(("FFB_WritePixmap: "
               "x[%d] y[%d] w[%d] h[%d] srcw[%d] rop[%d] pmask[%x] "
               "trans[%d] bpp[%d] depth[%d]\n",
               x, y, w, h, srcwidth, rop, planemask,
               trans, bpp, depth));

       FFB_ATTR_SFB_VAR_XAA(pFfb, planemask, rop);
       FFBWait(pFfb, ffb);

       sfb32 = (unsigned char *) pFfb->sfb32;
       dst = sfb32 + (y * (2048 << psz_shift)) + (x << psz_shift);
       VISmoveImageLR(src, dst, w << psz_shift, h,
                      srcwidth, (2048 << psz_shift));
}

static void FFB_SetupForMono8x8PatternFill(ScrnInfoPtr pScrn,
                                          int pat_word1, int pat_word2,
                                          int fg, int bg, int rop,
                                          unsigned int planemask)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       ffb_fbcPtr ffb = pFfb->regs;
       unsigned int ppc, ppc_mask, fbc;
       int i;

       FFBLOG(("FFB_SetupForMono8x8PatternFill: "
               "pat[%08x:%08x] fg[%x] bg[%x] rop[%d] pmask[%x]\n",
               pat_word1, pat_word2,
               fg, bg, rop, planemask));

       ppc = FFB_PPC_ABE_DISABLE | FFB_PPC_APE_ENABLE | FFB_PPC_CS_CONST;
       if (bg < 0)
               ppc |= FFB_PPC_TBE_TRANSPARENT;
       else
               ppc |= FFB_PPC_TBE_OPAQUE;
       ppc_mask = FFB_PPC_ABE_MASK | FFB_PPC_APE_MASK |
         FFB_PPC_TBE_MASK | FFB_PPC_CS_MASK;
       fbc = pFfb->xaa_fbc;
       rop = (rop | FFB_ROP_EDIT_BIT) | (FFB_ROP_NEW << 8);

       FFB_ATTR_RAW(pFfb, ppc, ppc_mask, planemask, rop,
                    FFB_DRAWOP_RECTANGLE, fg, fbc, pFfb->xaa_wid);
       if (bg >= 0)
               FFB_WRITE_BG(pFfb, ffb, bg);

       FFBFifo(pFfb, 32);
       for (i = 0; i < 32; i += 2) {
               CARD32 val1, val2;
               int shift = (24 - ((i % 4) * 8));

               if ((i % 8) < 4) {
                       val1 = (pat_word1 >> shift) & 0xff;
                       val2 = (pat_word1 >> (shift + 8)) & 0xff;
               } else {
                       val1 = (pat_word2 >> shift) & 0xff;
                       val2 = (pat_word2 >> (shift + 8)) & 0xff;
               }
               val1 |= (val1 << 8) | (val1 << 16) | (val1 << 24);
               val2 |= (val2 << 8) | (val2 << 16) | (val2 << 24);
               FFB_WRITE64(&ffb->pattern[i], val1, val2);
       }
       pFfb->rp_active = 1;
}

static void FFB_SubsequentMono8x8PatternFillRect(ScrnInfoPtr pScrn,
                                                int pat_word1, int pat_word2,
                                                int x, int y, int w, int h)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       ffb_fbcPtr ffb = pFfb->regs;

       FFBLOG(("FFB_SubsequentMono8x8PatternFillRect: "
               "x[%d] y[%d] w[%d] h[%d]\n", x, y, w, h));

       FFBFifo(pFfb, 4);
       FFB_WRITE64(&ffb->by, y, x);
       FFB_WRITE64_2(&ffb->bh, h, w);
}

static void FFB_SetupForScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
                                                          int fg, int bg,
                                                          int rop,
                                                          unsigned int planemask)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       ffb_fbcPtr ffb = pFfb->regs;
       unsigned int ppc, ppc_mask, fbc;

       FFBLOG(("FFB_SetupForScanlineCPUToScreenColorExpandFill: "
               "fg[%x] bg[%x] rop[%d] pmask[%x]\n",
               fg, bg, rop, planemask));

       ppc = FFB_PPC_ABE_DISABLE | FFB_PPC_APE_DISABLE | FFB_PPC_CS_CONST;
       if (bg < 0)
               ppc |= FFB_PPC_TBE_TRANSPARENT;
       else
               ppc |= FFB_PPC_TBE_OPAQUE;
       ppc_mask = FFB_PPC_ABE_MASK | FFB_PPC_APE_MASK |
         FFB_PPC_TBE_MASK | FFB_PPC_CS_MASK;
       fbc = pFfb->xaa_fbc;
       rop = (rop | FFB_ROP_EDIT_BIT) | (FFB_ROP_NEW << 8);

       if ((pFfb->ppc_cache & ppc_mask) != ppc ||
           pFfb->fg_cache != fg ||
           pFfb->fbc_cache != fbc ||
           pFfb->rop_cache != rop ||
           pFfb->pmask_cache != planemask ||
           pFfb->fontinc_cache != ((0<<16) | 32) ||
           (bg >= 0 && pFfb->bg_cache != bg)) {
               pFfb->ppc_cache &= ~ppc_mask;
               pFfb->ppc_cache |= ppc;
               pFfb->fg_cache = fg;
               pFfb->fbc_cache = fbc;
               pFfb->rop_cache = rop;
               pFfb->pmask_cache = planemask;
               pFfb->fontinc_cache = ((0<<16) | 32);
               if (bg >= 0)
                       pFfb->bg_cache = bg;
               FFBFifo(pFfb, (bg >= 0 ? 7 : 6));
               ffb->ppc = ppc;
               ffb->fg = fg;
               ffb->fbc = fbc;
               ffb->rop = rop;
               ffb->pmask = planemask;
               ffb->fontinc = ((0 << 16) | 32);
               if(bg >= 0)
                       ffb->bg = bg;
       }
       pFfb->rp_active = 1;
}

static void FFB_SubsequentScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
                                                            int x, int y, int w, int h,
                                                            int skipleft)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       FFBLOG(("FFB_SubsequentCPUToScreenColorExpandFill: "
               "x[%d] y[%d] w[%d] h[%d] skipleft[%d]\n",
               x, y, w, h, skipleft));

       pFfb->xaa_scanline_x = x;
       pFfb->xaa_scanline_y = y;
       pFfb->xaa_scanline_w = w;
}

static void FFB_SubsequentColorExpandScanline(ScrnInfoPtr pScrn, int bufno)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       ffb_fbcPtr ffb = pFfb->regs;
       CARD32 *bits = (CARD32 *) pFfb->xaa_scanline_buffers[bufno];
       int w;

       FFBFifo(pFfb, 1);
       ffb->fontxy = ((pFfb->xaa_scanline_y << 16) | pFfb->xaa_scanline_x);

       w = pFfb->xaa_scanline_w;
       if (w >= 32) {
               FFB_WRITE_FONTW(pFfb, ffb, 32);
               FFBFifo(pFfb, (w / 32));
               do {
                       ffb->font = *bits++;
                       w -= 32;
               } while (w >= 32);
       }
       if (w > 0) {
               FFB_WRITE_FONTW(pFfb, ffb, w);
               FFBFifo(pFfb, 1);
               ffb->font = *bits++;
       }

       pFfb->xaa_scanline_y++;
}

static void FFB_SetupForDashedLine(ScrnInfoPtr pScrn,
                                  int fg, int bg, int rop,
                                  unsigned int planemask,
                                  int length, unsigned char *pattern)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       CARD32 *pat_ptr = (CARD32 *)pattern;
       unsigned int ppc, ppc_mask, fbc;

       FFBLOG(("FFB_SetupForDashedLine: "
               "fg[%x] bg[%x] rop[%d] pmask[%x] patlen[%d] pat[%x]\n",
               fg, bg, rop, planemask, length, *pat_ptr));

       pFfb->xaa_planemask = planemask;
       pFfb->xaa_rop = rop;
       pFfb->xaa_linepat = 
               (*pat_ptr << FFB_LPAT_PATTERN_SHIFT) |
               (1 << FFB_LPAT_SCALEVAL_SHIFT) |
               ((length & 0xf) << FFB_LPAT_PATLEN_SHIFT);

       fbc = pFfb->xaa_fbc;
       ppc = FFB_PPC_ABE_DISABLE | FFB_PPC_APE_DISABLE | FFB_PPC_CS_CONST | FFB_PPC_XS_WID;
       ppc_mask = FFB_PPC_ABE_MASK | FFB_PPC_APE_MASK | FFB_PPC_CS_MASK | FFB_PPC_XS_MASK;

       FFB_ATTR_RAW(pFfb, ppc, ppc_mask, planemask,
                    (FFB_ROP_EDIT_BIT | rop) | (FFB_ROP_NEW << 8),
                    FFB_DRAWOP_BRLINEOPEN, fg, fbc, pFfb->xaa_wid);
       pFfb->rp_active = 1;
}

static void FFB_SubsequentDashedTwoPointLine( ScrnInfoPtr pScrn,
                                             int x1, int y1,
                                             int x2, int y2,
                                             int flags, int phase)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       ffb_fbcPtr ffb = pFfb->regs;
       unsigned int linepat = pFfb->xaa_linepat;
       unsigned int drawop;

       FFBLOG(("FFB_SubsequentDashedTwoPointLine: "
               "x1[%d] y1[%d] x2[%d] y2[%d] flgs[%x] phase[%d]\n",
               x1, y2, x2, y2, flags, phase));

       linepat |= (phase & 0xf) << FFB_LPAT_PATPTR_SHIFT;

       drawop = (flags & OMIT_LAST) ?
         FFB_DRAWOP_BRLINEOPEN : FFB_DRAWOP_BRLINECAP;
       FFB_WRITE_DRAWOP(pFfb, ffb, drawop);

       if (pFfb->has_brline_bug) {
               FFBFifo(pFfb, 6);
               ffb->ppc = 0;
       } else
               FFBFifo(pFfb, 5);
       ffb->lpat = linepat;
       FFB_WRITE64(&ffb->by, y1, x1);
       FFB_WRITE64_2(&ffb->bh, y2, x2);
}

static void FFB_SetupForSolidLine(ScrnInfoPtr pScrn, 
                                 int color, int rop, unsigned int planemask)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       ffb_fbcPtr ffb = pFfb->regs;
       unsigned int ppc, ppc_mask, fbc;
       FFBLOG(("FFB_SetupForSolidLine: "
               "color[%d] rop[%d] pmask[%x]\n",
               color, rop, planemask));

       pFfb->xaa_planemask = planemask;
       pFfb->xaa_rop = rop;

       fbc = pFfb->xaa_fbc;
       ppc = FFB_PPC_ABE_DISABLE | FFB_PPC_APE_DISABLE | FFB_PPC_CS_CONST | FFB_PPC_XS_WID;
       ppc_mask = FFB_PPC_ABE_MASK | FFB_PPC_APE_MASK | FFB_PPC_CS_MASK | FFB_PPC_XS_MASK;

       FFB_ATTR_RAW(pFfb, ppc, ppc_mask, planemask,
                    (FFB_ROP_EDIT_BIT | rop) | (FFB_ROP_NEW << 8),
                    FFB_DRAWOP_BRLINEOPEN, color, fbc, pFfb->xaa_wid);
       FFBFifo(pFfb, 1);
       ffb->lpat = 0;
       pFfb->rp_active = 1;
}

static void FFB_SubsequentSolidTwoPointLine(ScrnInfoPtr pScrn,
                                           int x1, int y1,
                                           int x2, int y2,
                                           int flags)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       ffb_fbcPtr ffb = pFfb->regs;
       int drawop;

       FFBLOG(("FFB_SubsequentSolidTwoPointLine: "
               "x1[%d] y1[%d] x2[%d] y2[%d] flags[%d]\n",
               x1, y1, x2, y2, flags));

       drawop = (flags & OMIT_LAST) ?
         FFB_DRAWOP_BRLINEOPEN : FFB_DRAWOP_BRLINECAP;
       FFB_WRITE_DRAWOP(pFfb, ffb, drawop);

       if (pFfb->has_brline_bug) {
               FFBFifo(pFfb, 5);
               ffb->ppc = 0;
       } else
               FFBFifo(pFfb, 4);
       FFB_WRITE64(&ffb->by, y1, x1);
       FFB_WRITE64_2(&ffb->bh, y2, x2);
}

void FFB_SetupForSolidFill(ScrnInfoPtr pScrn,
                          int color, int rop, unsigned int planemask)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       unsigned int ppc, ppc_mask, fbc;

       FFBLOG(("FFB_SetupForSolidFill: "
               "color[%d] rop[%d] pmask[%u]\n",
               color, rop, planemask));

       pFfb->xaa_planemask = planemask;
       pFfb->xaa_rop = rop;

       fbc = pFfb->xaa_fbc;
       if (pFfb->ffb_res == ffb_res_high)
               fbc |= FFB_FBC_WB_B;
       ppc = FFB_PPC_ABE_DISABLE | FFB_PPC_APE_DISABLE | FFB_PPC_CS_CONST | FFB_PPC_XS_WID;
       ppc_mask = FFB_PPC_ABE_MASK | FFB_PPC_APE_MASK | FFB_PPC_CS_MASK | FFB_PPC_XS_MASK;

       FFB_ATTR_RAW(pFfb, ppc, ppc_mask, planemask,
                    (FFB_ROP_EDIT_BIT | rop) | (FFB_ROP_NEW << 8),
                    FFB_DRAWOP_RECTANGLE, color, fbc, pFfb->xaa_wid);
       pFfb->rp_active = 1;
}

void FFB_SubsequentSolidFillRect(ScrnInfoPtr pScrn,
                                int x, int y,
                                int w, int h)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       ffb_fbcPtr ffb = pFfb->regs;

       FFBLOG(("FFB_SubsequentSolidFillRect: "
               "x[%d] y[%d] w[%d] h[%d]\n", x, y, w, h));

       FFBFifo(pFfb, 4);
       FFB_WRITE64(&ffb->by, y, x);
       FFB_WRITE64_2(&ffb->bh, h, w);
}

static void FFB_ScreenToScreenBitBlt(ScrnInfoPtr pScrn,
                                    int nbox,
                                    DDXPointPtr pptSrc,
                                    BoxPtr pbox,
                                    int xdir, int ydir,
                                    int rop, unsigned int planemask)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       ffb_fbcPtr ffb = pFfb->regs;
       int use_vscroll;

       FFBLOG(("FFB_ScreenToScreenBitBlt: "
               "nbox[%d] xdir[%d] ydir[%d] rop[%d] pmask[%x]\n",
               nbox, xdir, ydir, rop, planemask));

       use_vscroll = 0;
       if (!pFfb->disable_vscroll &&
           rop == GXcopy) {
               int i;

               for (i = 0; i < nbox; i++)
                       if (pptSrc[i].x != pbox[i].x1 ||
                           pptSrc[i].y == pbox[i].y1)
                               break;
               if (i == nbox) {
                       /* If/When double buffer extension is re-enabled
                        * check buffers here.
                        */
                       use_vscroll = 1;
               }
       }
       if (use_vscroll) {
               FFB_ATTR_VSCROLL_XAA(pFfb, planemask);
               while (nbox--) {
                       FFBFifo(pFfb, 7);
                       ffb->drawop = FFB_DRAWOP_VSCROLL;
                       FFB_WRITE64(&ffb->by, pptSrc->y, pptSrc->x);
                       FFB_WRITE64_2(&ffb->dy, pbox->y1, pbox->x1);
                       FFB_WRITE64_3(&ffb->bh, (pbox->y2 - pbox->y1),
                                     (pbox->x2 - pbox->x1));

                       pbox++;
                       pptSrc++;
               }
               pFfb->rp_active = 1;
               SET_SYNC_FLAG(pFfb->pXAAInfo);
       } else {
               unsigned char *sfb32 = (unsigned char *) pFfb->sfb32;
               int psz_shift = 2;

               FFB_ATTR_SFB_VAR_XAA(pFfb, planemask, rop);
               if (pFfb->use_blkread_prefetch) {
                       unsigned int bit;

                       if (xdir < 0)
                               bit = FFB_MER_EDRA;
                       else
                               bit = FFB_MER_EIRA;
                       FFBFifo(pFfb, 1);
                       ffb->mer = bit;
                       pFfb->rp_active = 1;
               }
               FFBWait(pFfb, ffb);

               while (nbox--) {
                       unsigned char *src, *dst;
                       int x1, y1, x2, y2;
                       int width, height;
                       int sdkind;

                       x1 = pptSrc->x;
                       y1 = pptSrc->y;
                       x2 = pbox->x1;
                       y2 = pbox->y1;
                       width = (pbox->x2 - pbox->x1);
                       height = (pbox->y2 - pbox->y1);

                       src = sfb32 + (y1 * (2048 << psz_shift))
                               + (x1 << psz_shift);
                       dst = sfb32 + (y2 * (2048 << psz_shift))
                               + (x2 << psz_shift);
                       sdkind = (2048 << psz_shift);

                       if (ydir < 0) {
                               src += ((height - 1) * (2048 << psz_shift));
                               dst += ((height - 1) * (2048 << psz_shift));
                               sdkind = -sdkind;
                       }
                       width <<= psz_shift;
                       if (xdir < 0)
                               VISmoveImageRL(src, dst, width, height,
                                              sdkind, sdkind);
                       else
                               VISmoveImageLR(src, dst, width, height,
                                              sdkind, sdkind);
                       pbox++;
                       pptSrc++;
	       }
               if (pFfb->use_blkread_prefetch) {
                       FFBFifo(pFfb, 1);
                       ffb->mer = FFB_MER_DRA;
                       pFfb->rp_active = 1;
                       FFBWait(pFfb, ffb);
               }
       }
}

void FFB_SetupForScreenToScreenCopy(ScrnInfoPtr pScrn,
                                   int xdir, int ydir, int rop,
                                   unsigned int planemask,
                                   int trans_color)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       ffb_fbcPtr ffb = pFfb->regs;
       FFBLOG(("FFB_SetupForScreenToScreenCopy: "
               "xdir[%d] ydir[%d] rop[%d] pmask[%x] tcolor[%d]\n",
               xdir, ydir, rop, planemask, trans_color));

       pFfb->xaa_planemask = planemask;
       pFfb->xaa_xdir = xdir;
       pFfb->xaa_ydir = ydir;
       pFfb->xaa_rop = rop;
       FFB_ATTR_SFB_VAR_XAA(pFfb, planemask, rop);
       FFBWait(pFfb, ffb);
}
		  
void FFB_SubsequentScreenToScreenCopy(ScrnInfoPtr pScrn,
                                     int x1, int y1,
                                     int x2, int y2,
                                     int width, int height)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       unsigned char *src, *dst, *sfb32;
       int psz_shift = 2;
       int sdkind;

       FFBLOG(("FFB_SubsequentScreenToScreenCopy: "
               "x1[%d] y1[%d] x2[%d] y2[%u] w[%d] h[%d]\n",
               x1, y1, x2, y2, width, height));

       sfb32 = (unsigned char *) pFfb->sfb32;
       src = sfb32 + (y1 * (2048 << psz_shift)) + (x1 << psz_shift);
       dst = sfb32 + (y2 * (2048 << psz_shift)) + (x2 << psz_shift);
       sdkind = (2048 << psz_shift);

       if (pFfb->xaa_ydir < 0) {
               src += ((height - 1) * (2048 << psz_shift));
               dst += ((height - 1) * (2048 << psz_shift));
               sdkind = -sdkind;
       }

       width <<= psz_shift;
       if (pFfb->xaa_xdir < 0)
               VISmoveImageRL(src, dst, width, height, sdkind, sdkind);
       else
               VISmoveImageLR(src, dst, width, height, sdkind, sdkind);
}

static void FFB_Sync(ScrnInfoPtr pScrn)
{
       FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
       ffb_fbcPtr ffb = pFfb->regs;

       FFB_ATTR_SFB_VAR_XAA(pFfb, 0xffffffff, GXcopy);
       FFBWait(pFfb, ffb);
}

/* Multiplies and divides suck... */
static void CreatorAlignTabInit(FFBPtr pFfb)
{
	struct fastfill_parms *ffp = &FFB_FFPARMS(pFfb);
	short *tab = pFfb->Pf_AlignTab;
	int i;

	for(i = 0; i < 0x800; i++) {
		int alignval;

		alignval = (i / ffp->pagefill_width) * ffp->pagefill_width;
		*tab++ = alignval;
	}
}

#endif

Bool FFBAccelInit(ScreenPtr pScreen, FFBPtr pFfb)
{
#ifdef HAVE_XAA_H
	XAAInfoRecPtr infoRec;
	ffb_fbcPtr ffb = pFfb->regs;

	pFfb->xaa_fbc = (FFB_FBC_WB_A | FFB_FBC_WM_COMBINED | FFB_FBC_RB_A |
			 FFB_FBC_WE_FORCEON |
			 FFB_FBC_SB_BOTH |
			 FFB_FBC_ZE_OFF | FFB_FBC_YE_OFF |
			 FFB_FBC_RGBE_MASK |
			 FFB_FBC_XE_ON);
	pFfb->xaa_wid = FFBWidAlloc(pFfb, TrueColor, 0, TRUE);
	if (pFfb->xaa_wid == (unsigned int) -1)
		return FALSE;

	pFfb->pXAAInfo = infoRec = XAACreateInfoRec();
	if (!infoRec) {
		FFBWidFree(pFfb, pFfb->xaa_wid);
		return FALSE;
	}

	pFfb->xaa_scanline_buffers[0] = malloc(2048 * 4);
	if (!pFfb->xaa_scanline_buffers[0]) {
		XAADestroyInfoRec(infoRec);
		return FALSE;
	}

	pFfb->xaa_scanline_buffers[1] = malloc(2048 * 4);
	if (!pFfb->xaa_scanline_buffers[1]) {
		free(pFfb->xaa_scanline_buffers[0]);
		XAADestroyInfoRec(infoRec);
		return FALSE;
	}

	infoRec->Sync = FFB_Sync;

	/* Use VIS and VSCROLL for screen to screen copies.  */
	infoRec->ScreenToScreenCopyFlags = NO_TRANSPARENCY;
	infoRec->SetupForScreenToScreenCopy =
		FFB_SetupForScreenToScreenCopy;
	infoRec->SubsequentScreenToScreenCopy =
		FFB_SubsequentScreenToScreenCopy;

	/* In order to optimize VSCROLL and prefetching properly we
	 * have to use our own mid-layer routine.
	 */
	infoRec->ScreenToScreenBitBltFlags = NO_TRANSPARENCY;
	infoRec->ScreenToScreenBitBlt =
		FFB_ScreenToScreenBitBlt;

	infoRec->SolidFillFlags = 0;
	infoRec->SetupForSolidFill =
		FFB_SetupForSolidFill;
	infoRec->SubsequentSolidFillRect =
		FFB_SubsequentSolidFillRect;

	infoRec->SolidLineFlags = 0;
	infoRec->SetupForSolidLine =
		FFB_SetupForSolidLine;
	infoRec->SubsequentSolidTwoPointLine =
		FFB_SubsequentSolidTwoPointLine;
	miSetZeroLineBias(pScreen, OCTANT3 | OCTANT4 | OCTANT6 | OCTANT1);

	infoRec->DashedLineFlags = (TRANSPARENCY_ONLY |
				    LINE_PATTERN_LSBFIRST_LSBJUSTIFIED);
	infoRec->DashPatternMaxLength = 16;
	infoRec->SetupForDashedLine =
		FFB_SetupForDashedLine;
	infoRec->SubsequentDashedTwoPointLine =
		FFB_SubsequentDashedTwoPointLine;

	/* We cannot use the non-scanline color expansion mechanism on FFB
	 * for two reasons:
	 *
	 * 1) A render pass can only render 32-pixels wide on FFB, XAA expects
	 *    that arbitrary widths are possible per render pass.
	 *
	 * 2) The FFB accelerator FIFO is only 100 or so words deep, and
	 *    XAA gives no way to limit the number of words it writes into
	 *    the ColorExpandBase register per rendering pass.
	 */
	infoRec->ScanlineColorExpandBuffers = pFfb->xaa_scanline_buffers;
	infoRec->NumScanlineColorExpandBuffers = 2;
	infoRec->ScanlineCPUToScreenColorExpandFillFlags =
		CPU_TRANSFER_PAD_DWORD |
		SCANLINE_PAD_DWORD |
		CPU_TRANSFER_BASE_FIXED |
		BIT_ORDER_IN_BYTE_LSBFIRST;
	infoRec->SetupForScanlineCPUToScreenColorExpandFill =
		FFB_SetupForScanlineCPUToScreenColorExpandFill;
	infoRec->SubsequentScanlineCPUToScreenColorExpandFill =
		FFB_SubsequentScanlineCPUToScreenColorExpandFill;
	infoRec->SubsequentColorExpandScanline =
		FFB_SubsequentColorExpandScanline;

	infoRec->Mono8x8PatternFillFlags =
		HARDWARE_PATTERN_PROGRAMMED_BITS |
		HARDWARE_PATTERN_SCREEN_ORIGIN |
		BIT_ORDER_IN_BYTE_LSBFIRST;
	infoRec->SetupForMono8x8PatternFill =
		FFB_SetupForMono8x8PatternFill;
	infoRec->SubsequentMono8x8PatternFillRect =
		FFB_SubsequentMono8x8PatternFillRect;

	/* Use VIS for pixmap writes.  */
	infoRec->WritePixmap = FFB_WritePixmap;

	/* RENDER optimizations.  */
	infoRec->CPUToScreenAlphaTextureFlags =
		XAA_RENDER_NO_TILE |
		XAA_RENDER_NO_SRC_ALPHA;
	infoRec->CPUToScreenAlphaTextureFormats = FFBAlphaTextureFormats;
	infoRec->SetupForCPUToScreenAlphaTexture =
		FFB_SetupForCPUToScreenAlphaTexture;
	infoRec->SubsequentCPUToScreenAlphaTexture =
		FFB_SubsequentCPUToScreenAlphaTexture;

	infoRec->CPUToScreenTextureFlags =
		XAA_RENDER_NO_TILE |
		XAA_RENDER_NO_SRC_ALPHA;
	infoRec->CPUToScreenTextureFormats = FFBTextureFormats;
	infoRec->SetupForCPUToScreenTexture =
		FFB_SetupForCPUToScreenTexture;
	infoRec->SubsequentCPUToScreenTexture =
		FFB_SubsequentCPUToScreenTexture;

	pFfb->fifo_cache = 0;

	FFB_DEBUG_init();
	FDEBUG((FDEBUG_FD,
		"FFB: cfg0(%08x) cfg1(%08x) cfg2(%08x) cfg3(%08x) ppcfg(%08x)\n",
		ffb->fbcfg0, ffb->fbcfg1, ffb->fbcfg2, ffb->fbcfg3, ffb->ppcfg));

	/* Determine the current screen resolution type.  This is
	 * needed to figure out the fastfill/pagefill parameters.
	 */
	switch(ffb->fbcfg0 & FFB_FBCFG0_RES_MASK) {
	default:
	case FFB_FBCFG0_RES_STD:
		pFfb->ffb_res = ffb_res_standard;
		break;
	case FFB_FBCFG0_RES_HIGH:
		pFfb->ffb_res = ffb_res_high;
		break;
	case FFB_FBCFG0_RES_STEREO:
		pFfb->ffb_res = ffb_res_stereo;
		break;
	case FFB_FBCFG0_RES_PRTRAIT:
		pFfb->ffb_res = ffb_res_portrait;
		break;
	};
	CreatorAlignTabInit(pFfb);

	/* Next, determine the hwbug workarounds and feature enables
	 * we should be using on this board.
	 */
	pFfb->disable_pagefill = 0;
	pFfb->disable_vscroll = 0;
	pFfb->has_brline_bug = 0;
	pFfb->use_blkread_prefetch = 0;
	if (pFfb->ffb_type == ffb1_prototype ||
	    pFfb->ffb_type == ffb1_standard ||
	    pFfb->ffb_type == ffb1_speedsort) {
		pFfb->has_brline_bug = 1;
		if (pFfb->ffb_res == ffb_res_high)
			pFfb->disable_vscroll = 1;
		if (pFfb->ffb_res == ffb_res_high ||
		    pFfb->ffb_res == ffb_res_stereo)
			pFfb->disable_pagefill = 1;

	} else {
		/* FFB2 has blkread prefetch.  AFB supposedly does too
		 * but the chip locks up on me when I try to use it. -DaveM
		 */
#define AFB_PREFETCH_IS_BUGGY	1
		if (!AFB_PREFETCH_IS_BUGGY ||
		    (pFfb->ffb_type != afb_m3 &&
		     pFfb->ffb_type != afb_m6)) {
			pFfb->use_blkread_prefetch = 1;
		}
		/* XXX I still cannot get page/block fast fills
		 * XXX to work reliably on any of my AFB boards. -DaveM
		 */
#define AFB_FASTFILL_IS_BUGGY	1
		if (AFB_FASTFILL_IS_BUGGY &&
		    (pFfb->ffb_type == afb_m3 ||
		     pFfb->ffb_type == afb_m6))
			pFfb->disable_pagefill = 1;
	}
	pFfb->disable_fastfill_ap = 0;
	if (pFfb->ffb_res == ffb_res_stereo ||
	    pFfb->ffb_res == ffb_res_high)
		pFfb->disable_fastfill_ap = 1;

	pFfb->ppc_cache = (FFB_PPC_FW_DISABLE |
			   FFB_PPC_VCE_DISABLE | FFB_PPC_APE_DISABLE | FFB_PPC_CS_CONST |
			   FFB_PPC_XS_WID | FFB_PPC_YS_CONST | FFB_PPC_ZS_CONST |
			   FFB_PPC_DCE_DISABLE | FFB_PPC_ABE_DISABLE | FFB_PPC_TBE_OPAQUE);
	pFfb->wid_cache = pFfb->xaa_wid;
	pFfb->pmask_cache = ~0;
	pFfb->rop_cache = (FFB_ROP_NEW | (FFB_ROP_NEW << 8));
	pFfb->drawop_cache = FFB_DRAWOP_RECTANGLE;
	pFfb->fg_cache = pFfb->bg_cache = 0;
	pFfb->fontw_cache = 32;
	pFfb->fontinc_cache = (1 << 16) | 0;
	pFfb->fbc_cache = (FFB_FBC_WB_A | FFB_FBC_WM_COMBINED | FFB_FBC_RB_A |
			   FFB_FBC_WE_FORCEON |
			   FFB_FBC_SB_BOTH |
			   FFB_FBC_ZE_OFF | FFB_FBC_YE_OFF |
			   FFB_FBC_RGBE_OFF |
			   FFB_FBC_XE_ON);
	pFfb->laststipple = NULL;

	/* We will now clear the screen: we'll draw a rectangle covering all the
	 * viewscreen, using a 'blackness' ROP.
	 */
	FFBFifo(pFfb, 22);
	ffb->fbc = pFfb->fbc_cache;
	ffb->ppc = pFfb->ppc_cache;
	ffb->wid = pFfb->wid_cache;
	ffb->xpmask = 0xff;
	ffb->pmask = pFfb->pmask_cache;
	ffb->rop = pFfb->rop_cache;
	ffb->drawop = pFfb->drawop_cache;
	ffb->fg = pFfb->fg_cache;
	ffb->bg = pFfb->bg_cache;
	ffb->fontw = pFfb->fontw_cache;
	ffb->fontinc = pFfb->fontinc_cache;
	ffb->xclip = FFB_XCLIP_TEST_ALWAYS;
	ffb->cmp = 0x80808080;
	ffb->matchab = 0x80808080;
	ffb->magnab = 0x80808080;
	ffb->blendc = (FFB_BLENDC_FORCE_ONE |
		       FFB_BLENDC_DF_ONE_M_A |
		       FFB_BLENDC_SF_A);
	ffb->blendc1 = 0;
	ffb->blendc2 = 0;
	FFB_WRITE64(&ffb->by, 0, 0);
	FFB_WRITE64_2(&ffb->bh, pFfb->psdp->height, pFfb->psdp->width);
	pFfb->rp_active = 1;
	FFBWait(pFfb, ffb);
	
	FFB_ATTR_SFB_VAR_XAA(pFfb, 0xffffffff, GXcopy);
	FFBWait(pFfb, ffb);

	if (!XAAInit(pScreen, infoRec)) {
		XAADestroyInfoRec(infoRec);
		free(pFfb->xaa_scanline_buffers[0]);
		free(pFfb->xaa_scanline_buffers[1]);
		pFfb->pXAAInfo = NULL;
		FFBWidFree(pFfb, pFfb->xaa_wid);
		return FALSE;
	}

	/* Success */
	return TRUE;
#else
	return FALSE;
#endif
}
