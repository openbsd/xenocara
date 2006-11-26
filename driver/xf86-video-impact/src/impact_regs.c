/*
 * impact_regs.c 2005/07/15 23:36:16, Copyright (c) 2005 peter fuerst
 *
 * Based on:
 * - linux/drivers/video/impact.c, 2005 pf.
 * - linux/drivers/video/impactsr.c, (c) 2004 by Stanislaw Skowronek.
 * - newport_regs.c
 *   # newport_regs.c,v 1.3 2000/11/29 20:58:10 agx Exp #
 *   # xc/programs/Xserver/hw/xfree86/drivers/newport/newport_regs.c,v
 *     1.6 2001/12/21 15:37:23 tsi Exp #
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "impact.h"

/* FIFO status */
#define MSK_I2_CFIFO_CNT    0x7f
#define MSK_SR_CFIFO_CNT    0xff
#define IMPACTI2_CFIFO_MAX	64
#define IMPACTSR_CFIFO_MAX	128
#define IMPACT_BFIFO_MAX	16

/* Sometimes we just have to wait until we can do anything */

/*
 * ImpactWaitCfifo
 */
unsigned
ImpactI2WaitCfifo(ImpactRegsPtr pImpactRegs, int nslots)
{
	unsigned x;
	while (((x = pImpactRegs->i2.fifostatus) & MSK_I2_CFIFO_CNT)
		> IMPACTI2_CFIFO_MAX-nslots);
	return x;
}

unsigned
ImpactSRWaitCfifo(ImpactRegsPtr pImpactRegs, int nslots)
{
	unsigned x;
	while (((x = pImpactRegs->sr.fifostatus) & MSK_SR_CFIFO_CNT)
		> IMPACTSR_CFIFO_MAX-nslots);
	return x;
}

/*
 * ImpactWaitCfifoEmpty
 */
void
ImpactI2WaitCfifoEmpty(ImpactRegsPtr pImpactRegs)
{
	while (pImpactRegs->i2.fifostatus & MSK_I2_CFIFO_CNT);
}

void
ImpactSRWaitCfifoEmpty(ImpactRegsPtr pImpactRegs)
{
	while (pImpactRegs->sr.fifostatus & MSK_SR_CFIFO_CNT);
}

/*
 * ImpactWaitDMAOver
 */
void
ImpactI2WaitDMAOver(ImpactRegsPtr pImpactRegs)
{
	while (pImpactRegs->i2.dmabusy & 0x1F);
	while (!(pImpactRegs->i2.status & 1));
	while (!(pImpactRegs->i2.status & 2));
	while (!(pImpactRegs->i2.rss.status & 0x100));
}

void
ImpactSRWaitDMAOver(ImpactRegsPtr pImpactRegs)
{
	while (pImpactRegs->sr.dmabusy & 0x1F);
	while (!(pImpactRegs->sr.status & 1));
	while (!(pImpactRegs->sr.status & 2));
	while (!(pImpactRegs->sr.rss.status & 0x100));
}

/*
 * ImpactWaitDMAReady
 */
void
ImpactI2WaitDMAReady(ImpactRegsPtr pImpactRegs)
{
	ImpactFifoCmd32(&pImpactRegs->i2.cfifo, 0x000e0100, 0);
	while (pImpactRegs->i2.dmabusy & 0x1EFF);
	while (!(pImpactRegs->i2.status & 2));
}

void
ImpactSRWaitDMAReady(ImpactRegsPtr pImpactRegs)
{
	ImpactFifoCmd32(&pImpactRegs->sr.cfifo, 0x000e0100, 0);
	while (pImpactRegs->sr.dmabusy & 0x1EFF);
	while (!(pImpactRegs->sr.status & 2));
}

/*
 * ImpactWaitBfifo
 */
static __inline__ unsigned
ImpactWaitBfifo(mgireg32_t *giostatus, int nslots)
{
	unsigned x;
	while (((x = *giostatus) & 0x1f) > IMPACT_BFIFO_MAX-nslots);
	return x;
}

unsigned
ImpactI2WaitBfifo(ImpactRegsPtr pImpactRegs, int nslots)
{
	return ImpactWaitBfifo(&pImpactRegs->i2.giostatus, nslots);
}

unsigned
ImpactSRWaitBfifo(ImpactRegsPtr pImpactRegs, int nslots)
{
	return ImpactWaitBfifo(&pImpactRegs->sr.giostatus, nslots);
}

/*
 * ImpactWaitBfifoEmpty
 */
static __inline__ void
ImpactWaitBfifoEmpty(mgireg32_t *giostatus)
{
	while (*giostatus & 0x1f);
}

void
ImpactI2WaitBfifoEmpty(ImpactRegsPtr pImpactRegs)
{
	ImpactWaitBfifoEmpty(&pImpactRegs->i2.giostatus);
}

void
ImpactSRWaitBfifoEmpty(ImpactRegsPtr pImpactRegs)
{
	ImpactWaitBfifoEmpty(&pImpactRegs->sr.giostatus);
}

/*
 * ImpactVc3[GS]et
 */
static __inline__ void
ImpactVc3Set(Impact_vc3regs_t* vc3, CARD8 idx, unsigned short val)
{
	vc3->indexdata = (idx << 24) | (val << 8);
}

void 
ImpactI2Vc3Set(ImpactRegsPtr pImpactRegs, CARD8 idx, unsigned short val)
{
	ImpactVc3Set(&pImpactRegs->i2.vc3, idx, val);
}

void 
ImpactSRVc3Set(ImpactRegsPtr pImpactRegs, CARD8 idx, unsigned short val)
{
	ImpactVc3Set(&pImpactRegs->sr.vc3, idx, val);
}

static __inline__ unsigned short 
ImpactVc3Get(Impact_vc3regs_t* vc3, CARD8 idx)
{
	vc3->index = idx;
	return vc3->data;
}

unsigned short 
ImpactI2Vc3Get(ImpactRegsPtr pImpactRegs, CARD8 idx)
{
	return ImpactVc3Get(&pImpactRegs->i2.vc3, idx);
}

unsigned short 
ImpactSRVc3Get(ImpactRegsPtr pImpactRegs, CARD8 idx)
{
	return ImpactVc3Get(&pImpactRegs->sr.vc3, idx);
}


/*
 * ImpactXmap[GS]etModeRegister
 */
static __inline__ void
ImpactXmapSetModeRegister(Impact_xmapregs_t *xmap, CARD8 address, CARD32 mode)
{
	xmap->index = address << 2;
	xmap->main_mode = mode;
}

void
ImpactI2XmapSetModeRegister(ImpactRegsPtr pImpactRegs, CARD8 address, CARD32 mode)
{
	ImpactXmapSetModeRegister(&pImpactRegs->i2.xmap, address, mode);
}

void
ImpactSRXmapSetModeRegister(ImpactRegsPtr pImpactRegs, CARD8 address, CARD32 mode)
{
	ImpactXmapSetModeRegister(&pImpactRegs->sr.xmap, address, mode);
}

static __inline__ CARD32
ImpactXmapGetModeRegister(Impact_xmapregs_t *xmap, CARD8 address)
{
	xmap->index = address << 2;
	return xmap->main_mode;
}

CARD32
ImpactI2XmapGetModeRegister(ImpactRegsPtr pImpactRegs, CARD8 address)
{
	return ImpactXmapGetModeRegister(&pImpactRegs->i2.xmap, address);
}

CARD32
ImpactSRXmapGetModeRegister(ImpactRegsPtr pImpactRegs, CARD8 address)
{
	return ImpactXmapGetModeRegister(&pImpactRegs->sr.xmap, address);
}

/*
 * ImpactBackup.../ImpactRestore...
 */
void 
ImpactBackupVc3( ScrnInfoPtr pScrn )
{
	ImpactPtr pImpact = IMPACTPTR(pScrn);
	ImpactRegsPtr pImpactRegs = pImpact->pImpactRegs;

	CARD8 i = sizeof(pImpact->txt_vc3)/sizeof(*pImpact->txt_vc3);
	while (i--)
		pImpact->txt_vc3[i] = (*pImpact->Vc3Get)( pImpactRegs, i );
}

void
ImpactRestoreVc3( ScrnInfoPtr pScrn )
{
	ImpactPtr pImpact = IMPACTPTR(pScrn);
	ImpactRegsPtr pImpactRegs = pImpact->pImpactRegs;

	CARD8 i = sizeof(pImpact->txt_vc3)/sizeof(*pImpact->txt_vc3);
	while (i--)
		(*pImpact->Vc3Set)( pImpactRegs, i, pImpact->txt_vc3[i] );
}

void
ImpactVc3DisableCursor( ScrnInfoPtr pScrn )
{
	ImpactPtr pImpact = IMPACTPTR(pScrn);
	ImpactRegsPtr pImpactRegs = pImpact->pImpactRegs;

	unsigned short val = (*pImpact->Vc3Get)( pImpactRegs, VC3_IREG_CURSOR );
	(*pImpact->Vc3Set)(pImpactRegs, VC3_IREG_CURSOR, val & ~VC3_CTRL_ECURS);
}

void 
ImpactVc3BlankScreen( ScrnInfoPtr pScrn, int blank )
{/*                                                 
	ImpactPtr pImpact = IMPACTPTR(pScrn);
	ImpactRegsPtr pImpactRegs = pImpact->pImpactRegs;

	unsigned short treg = (*pImpact->Vc3Get)( pImpactRegs, VC3_IREG_CONTROL );
	if (blank)
		treg &= ~VC3_CTRL_EDISP;
	else
		treg |= VC3_CTRL_EDISP;
	(*pImpact->Vc3Set)( pImpactRegs, VC3_IREG_CONTROL, treg );
*/}

void 
ImpactBackupRex( ScrnInfoPtr pScrn )
{
}

void
ImpactRestoreRex( ScrnInfoPtr pScrn )
{
}

void ImpactBackupXmap( ScrnInfoPtr pScrn )
{
	ImpactPtr pImpact = IMPACTPTR(pScrn);
	ImpactRegsPtr pImpactRegs = pImpact->pImpactRegs;
	Impact_xmapregs_t *xmap =
		(&ImpactSRXmapGetModeRegister != pImpact->XmapGetModeRegister)
			? &pImpactRegs->i2.xmap
			: &pImpactRegs->sr.xmap;

	pImpact->txt_xmap.pp1select = xmap->pp1select;
	pImpact->txt_xmap.main_mode =
			(*pImpact->XmapGetModeRegister)( pImpactRegs, 0 );
}

void ImpactRestoreXmap( ScrnInfoPtr pScrn )
{
	ImpactPtr pImpact = IMPACTPTR(pScrn);
	ImpactRegsPtr pImpactRegs = pImpact->pImpactRegs;
	Impact_xmapregs_t *xmap =
		(&ImpactSRXmapGetModeRegister != pImpact->XmapGetModeRegister)
			? &pImpactRegs->i2.xmap
			: &pImpactRegs->sr.xmap;

	xmap->pp1select = pImpact->txt_xmap.pp1select;
	(*pImpact->XmapSetModeRegister)(
			pImpactRegs, 0, pImpact->txt_xmap.main_mode );
}

/* set XMAP into 24-bpp mode */

void
ImpactXmapSet24bpp( ScrnInfoPtr pScrn )
{
	ImpactPtr pImpact = IMPACTPTR(pScrn);
	ImpactRegsPtr pImpactRegs = pImpact->pImpactRegs;
	Impact_xmapregs_t *xmap =
		(&ImpactSRXmapGetModeRegister != pImpact->XmapGetModeRegister)
			? &pImpactRegs->i2.xmap
			: &pImpactRegs->sr.xmap;
	xmap->pp1select = 1;
	(*pImpact->XmapSetModeRegister)( pImpactRegs, 0, 0x07a4 );
}

/* eof */
