/* 
 * impact_shadow.c 2005/07/12 23:24:15, Copyright (c) 2005 peter fuerst
 *
 * Based on:
 * - linux/drivers/video/impact.c, 2005 pf.
 * - linux/drivers/video/impactsr.c, (c) 2004 by Stanislaw Skowronek.
 * - newport_shadow.c
 *   # newport_shadow.c,v 1.3 2000/11/29 20:58:10 agx Exp #
 *   # xc/programs/Xserver/hw/xfree86/drivers/newport/newport_shadow.c,v
 *     1.2 2001/11/23 19:50:45 dawes Exp #
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <asm/cachectl.h>
#include <sys/cachectl.h>
#include <sys/ioctl.h>
#include "impact.h"

/*
 * 4th parameter to be passed to xf86SetDepthBpp()
 */
int ImpactDepth24Flags(void)
{
	return Support32bppFb;
}


/*
 * Common sanity check and adjustment for all ImpactRefreshArea...
 */
static __inline__ int
ImpactAdjRefreshBox(const BoxPtr obox, BoxPtr nbox)
{
	/*
	 * NB: Box defines [x1,x2[ x [x2,y2[, not [x1,x2] x [x2,y2] !
	 */
	*nbox = *obox;
	if (nbox->x1 < 0) nbox->x1 = 0;
	if (nbox->y1 < 0) nbox->y1 = 0;
	if (IMPACT_FIXED_W_SCRN < nbox->x2) nbox->x2 = IMPACT_FIXED_W_SCRN;
	if (IMPACT_FIXED_H_SCRN < nbox->y2) nbox->y2 = IMPACT_FIXED_H_SCRN;
	/*
	 * We may need to do some adjustment, since
	 * 1) the actual number of pixels *must* match the settings
	 *    in "xystarti", "xyendi", "xfrsize", "xfrcounters",
	 * 2) we draw pairs of pixels.
	 * Otherwise... (try it :))
	 */
	nbox->x1 &= ~1;
	nbox->x2 = ++nbox->x2 & ~1;
	return (nbox->x2 <= nbox->x1 || nbox->y2 <= nbox->y1);
}


/*
 * Common per-box-prologue for PIO-version of ImpactRefreshArea*
 */
static __inline__ int
ImpactPreRefreshBoxPIO(mgicfifo_t *cfifo, const BoxPtr obox, BoxPtr nbox)
{
	int h, w, i;

	if (ImpactAdjRefreshBox(obox, nbox))
		return 0;

	w = nbox->x2 - nbox->x1;
	h = nbox->y2 - nbox->y1;

	/* setup PIO to RE */
	impact_cmd_pp1fillmode(cfifo, 0x6300, IMPACT_LO_COPY);
	impact_cmd_blockxystarti(cfifo, nbox->x1, nbox->y1);
	impact_cmd_blockxyendi(cfifo, nbox->x2-1, nbox->y2-1);
	impact_cmd_fillmode(cfifo, 0x00c00000);
	impact_cmd_xfrmode(cfifo, 0x00080);
	impact_cmd_xfrsize(cfifo, w, h);
	impact_cmd_xfrcounters(cfifo, w, h);
	impact_cmd_gline_xstartf(cfifo, 1);
	impact_cmd_ir_alias(cfifo, 0x18);
	/* another workaround.. 33 writes to alpha... hmm... */
	for (i = 0; i < 33; i++)
		impact_cmd_alpha(cfifo, 0);
	impact_cmd_xfrcontrol(cfifo, 2);
	return 1;
}

/*
 * Common per-box-epilogue for PIO-version of ImpactRefreshArea*
 */
static __inline__ void
ImpactPostRefreshBoxPIO(mgicfifo_t *cfifo)
{
	impact_cmd_gline_xstartf(cfifo, 0);
	impact_cmd_re_togglecntx(cfifo, 0);
	impact_cmd_xfrcounters(cfifo, 0, 0);
}


/*
 * 8-bpp blits are done as PIO draw operation; the pixels are unpacked
 * into 32-bpp values from the current palette in software.
 */
static void
ImpactRefreshArea8(ImpactPtr pImpact, mgicfifo_t *cfifo, int num, BoxPtr pbox)
{
	unsigned pal[256];
	int i;

	for (i = 0; i < sizeof(pal)/sizeof(*pal); i++)
		pal[i] = ImpactGetPalReg(pImpact, i);

	for (; num--; pbox++) {
		BoxRec sanebox;
		unsigned char *base;
		int dy;
		TRACEV("ImpactRefreshArea8: %d [%d..%d[x[%d..%d[\n",
				num, pbox->x1,pbox->x2, pbox->y1,pbox->y2 );

		(*pImpact->WaitCfifoEmpty)(pImpact->pImpactRegs);
		if (ImpactPreRefreshBoxPIO(cfifo,pbox,&sanebox) <= 0)
			continue;

		base = (unsigned char*)pImpact->ShadowPtr 
			+ (dy = sanebox.y1) * pImpact->ShadowPitch + sanebox.x1;

		for ( ; dy < sanebox.y2; dy++) {
			unsigned char *src = base;
			int dx = sanebox.x1;
			while (dx < sanebox.x2) {
				/* pairs of pixels are sent in two writes to the RE */
				impact_cmd_char_h(cfifo, pal[*src++]);
				impact_cmd_char_l(cfifo, pal[*src++]);
				dx += 2;
			}
			base += pImpact->ShadowPitch;
		}
		ImpactPostRefreshBoxPIO(cfifo);
	}
	TRACE_EXIT("ImpactRefreshArea8");
}

void
ImpactI2RefreshArea8(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
	if (num > 0)
	{	ImpactPtr pImpact = IMPACTPTR(pScrn);
		ImpactRegsPtr pImpactRegs = pImpact->pImpactRegs;

		ImpactRefreshArea8(pImpact, &pImpactRegs->i2.cfifo, num, pbox);
	}
}

void
ImpactSRRefreshArea8(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
	if (num > 0)
	{	ImpactPtr pImpact = IMPACTPTR(pScrn);
		ImpactRegsPtr pImpactRegs = pImpact->pImpactRegs;

		ImpactRefreshArea8(pImpact, &pImpactRegs->sr.cfifo, num, pbox);
	}
}


/*
 * 32-bit blits are done as DMA operations, which is FAST on SGI machines
 */

static void
ImpactCflushBoxPixlines(int fd, char *base, BoxPtr box, unsigned bpitch)
{
	int w = (box->x2 - box->x1) << 2;
	int h = box->y2 - box->y1;
	int i;

	base += bpitch*box->y1 + 4*box->x1;

	/* One syscall for each pixel-line. */
	for (i = 0; i < h; i++, base += bpitch)
		cacheflush(base, w, DCACHE);
}

static void
ImpactCflushBoxEnclosure(int fd, char *base, BoxPtr box, unsigned bpitch)
{
	int w = (box->x2 - box->x1) << 2;
	int h = box->y2 - box->y1;

	base += bpitch*box->y1 + 4*box->x1;

	/* Whole enclosing mem, although may be mostly unconcerned. */
	cacheflush(base, (h-1)*bpitch + w, DCACHE);
}


static __inline__ int
ImpactRefreshBoxDMA(int isSR, mgicfifo_t *cfifo, BoxPtr box, unsigned bpitch)
{
	int w = box->x2 - box->x1;
	int h = box->y2 - box->y1;

	/* setup DMA to RE */
	impact_cmd_pp1fillmode(cfifo, isSR ? 0x6300:0x6200, IMPACT_LO_COPY);
	impact_cmd_colormasklsbsa(cfifo, 0xFFFFFFFF);
	impact_cmd_colormasklsbsb(cfifo, 0xFFFFFFFF);
	impact_cmd_colormaskmsbs(cfifo, 0xFFFF);
	impact_cmd_xfrmasklo(cfifo, 0xFFFFFFFF);
	impact_cmd_xfrmaskhi(cfifo, 0xFFFFFFFF);
	impact_cmd_packedcolor(cfifo, 0);
	impact_cmd_red(cfifo, 0);
	impact_cmd_drbpointers(cfifo, 0xc8240);
	impact_cmd_blockxystarti(cfifo, box->x1, box->y1);
	impact_cmd_blockxyendi(cfifo, box->x2-1, box->y2-1);
	impact_cmd_fillmode(cfifo, 0x01400000);
	impact_cmd_xfrmode(cfifo, 0x00080);
	impact_cmd_hq_pixelformat(cfifo, 0x600);
	impact_cmd_hq_scanwidth(cfifo, w<<2);
	impact_cmd_hq_dmatype(cfifo, 0x0c);
	impact_cmd_pixcmd(cfifo, 3);
	impact_cmd_xfrsize(cfifo, w, h);
	impact_cmd_xfrcounters(cfifo, w, h);
	impact_cmd_gline_xstartf(cfifo, 1);
	impact_cmd_ir_alias(cfifo, 0x18);
	if (isSR) {
		int i;
		for (i = 0; i < 33; i++)
			impact_cmd_alpha(cfifo, 0);
	}
	impact_cmd_xfrcontrol(cfifo, 1);
	impact_cmd_hq_pg_list0(cfifo, 0x80000000);
	impact_cmd_hq_pg_width(cfifo, bpitch);
	impact_cmd_hq_pg_offset(cfifo, 0);
	impact_cmd_hq_pg_startaddr(cfifo, bpitch*box->y1+(box->x1<<2));
	impact_cmd_hq_pg_linecnt(cfifo, h);
	impact_cmd_hq_pg_widtha(cfifo, w<<2);
	impact_cmd_hq_dmactrl_a(cfifo,3);
	impact_cmd_hq_dmactrl_b(cfifo);

	return 1;
}

static __inline__ void
ImpactPostRefreshBoxDMA(mgicfifo_t *cfifo)
{
	impact_cmd_gline_xstartf(cfifo, 0);
	impact_cmd_re_togglecntx(cfifo, 0);
	impact_cmd_xfrcounters(cfifo, 0, 0);
	impact_cmd_pixcmd(cfifo, 0);
	impact_cmd_hq_pixelformat(cfifo, 0xe00);
}


static void
ImpactRefreshArea32DMA(ImpactPtr pImpact, int num, BoxPtr pbox)
{
	ImpactRegsPtr pImpactRegs = pImpact->pImpactRegs;
	mgicfifo_t *cfifo =
		pImpact->isSR ? &pImpactRegs->sr.cfifo:&pImpactRegs->i2.cfifo;
	unsigned bpitch = pImpact->ShadowPitch;

	for (; num-- > 0; pbox++) {
		BoxRec sanebox;
		TRACEV("ImpactRefreshArea32: %d [%d..%d]x[%d..%d]\n",
			num, pbox->x1,pbox->x2, pbox->y1,pbox->y2);

		if (ImpactAdjRefreshBox(pbox, &sanebox))
			continue;

		if (pImpact->FlushBoxCache)
			(*pImpact->FlushBoxCache)(pImpact->devFD, (char*)pImpact->ShadowPtr,
						&sanebox, bpitch);

		(*pImpact->WaitDMAOver)(pImpactRegs);
		ImpactRefreshBoxDMA(pImpact->isSR, cfifo, &sanebox, bpitch);
		(*pImpact->WaitDMAOver)(pImpactRegs);
		ImpactPostRefreshBoxDMA(cfifo);
	}
	TRACE_EXIT("ImpactRefreshArea32");
}


/*
 * 32-bpp blits, done as PIO draw operation.  On Indigo2 faster than DMA
 * for very small boxes or boxes, no wider than about a dozen pixels.
 */
static void
ImpactRefreshArea32PIO(ImpactPtr pImpact, int num, BoxPtr pbox)
{
	ImpactRegsPtr pImpactRegs = pImpact->pImpactRegs;
	mgicfifo_t *cfifo =
		pImpact->isSR ? &pImpactRegs->sr.cfifo:&pImpactRegs->i2.cfifo;
	unsigned bpitch = pImpact->ShadowPitch;

	for (; num--; pbox++) {
		BoxRec sanebox;
		unsigned char* base;
		int dy;
		TRACEV("ImpactRefreshArea32 PIO: %d [%d..%d[x[%d..%d[\n",
				num, pbox->x1,pbox->x2, pbox->y1,pbox->y2 );

		(*pImpact->WaitCfifoEmpty)(pImpactRegs);
		if (ImpactPreRefreshBoxPIO(cfifo,pbox,&sanebox) <= 0)
			continue;

		base = (unsigned char*)pImpact->ShadowPtr
			+ (dy = sanebox.y1) * bpitch + sanebox.x1 * 4;

		for ( ; dy < sanebox.y2; dy++) {
			unsigned *src = (unsigned*)base;
			int dx = sanebox.x1;
			while (dx < sanebox.x2) {
				/* pairs of pixels are sent in two writes to the RE */
				impact_cmd_char_h(cfifo, *src++);
				impact_cmd_char_l(cfifo, *src++);
				dx += 2;
 			}
			base += bpitch;
 		}
		ImpactPostRefreshBoxPIO(cfifo);
	}
	/* TRACE_EXIT("ImpactRefreshArea32"); */
}


void
ImpactI2RefreshArea32(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
	ImpactPtr pImpact = IMPACTPTR(pScrn);

	for (; num--; pbox++) {
		int w = pbox->x2 - pbox->x1;
		int h = pbox->y2 - pbox->y1;

		if (w < 16 || w < 24 && h < 16 || w < 40 && h < 4)
			ImpactRefreshArea32PIO(pImpact, 1, pbox);
		else {
			if (h < 580)
				pImpact->FlushBoxCache = ImpactCflushBoxPixlines;
			else
				pImpact->FlushBoxCache = ImpactCflushBoxEnclosure;
			ImpactRefreshArea32DMA(pImpact, 1, pbox);
		}
	}
}

void
ImpactSRRefreshArea32(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
	ImpactRefreshArea32DMA(IMPACTPTR(pScrn), num, pbox);
}

/* eof */
