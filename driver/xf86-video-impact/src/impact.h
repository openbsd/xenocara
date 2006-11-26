/* 
 * impact.h 2005/07/12 23:24:15, Copyright (c) 2005 peter fuerst
 *
 * Based on:
 * # newport.h,v 1.4 2000/11/29 20:58:10 agx Exp #
 * # xc/programs/Xserver/hw/xfree86/drivers/newport/newport.h,v
 *   1.8 2001/12/19 21:31:21 dawes Exp #
 */

#ifndef __IMPACT_H__
#define __IMPACT_H__

/*
 * All drivers should include these:
 */
#include "xf86.h"
#include "xf86_OSproc.h"
#include "compiler.h"
#include "xf86Resources.h"

#include "xf86cmap.h"

/* xaa & hardware cursor */
#include "xaa.h"
#include "xf86Cursor.h"

/* register definitions of the Impact card */
#include "impact_regs.h"

#if 0
# define DEBUG 1
#endif

#ifdef DEBUG
# define TRACE_ENTER(str)	ErrorF("impact: " str " %d\n",pScrn->scrnIndex)
# define TRACE_EXIT(str)	ErrorF("impact: " str " done\n")
# define TRACE(str)	ErrorF("impact trace: " str "\n")
# define TRACEV(str...)	ErrorF(str)
#else
# define TRACE_ENTER(str)
# define TRACE_EXIT(str)
# define TRACE(str)
# define TRACEV(str...)
#endif

#define IMPACT_FIXED_W_SCRN 1280
#define IMPACT_FIXED_H_SCRN 1024

typedef struct {
	unsigned busID;
	unsigned isSR;
	int devFD; 
	int bitplanes; 
	
	/* revision numbers of the various pieces of silicon */
	unsigned int board_rev, cmap_rev, rex_rev, xmap_rev;

	ImpactRegsPtr pImpactRegs;	/* Pointer to HQ registers */
	unsigned long base_addr0;
	unsigned long base_offset;

	/* ShadowFB stuff: */
	pointer ShadowPtr;
	unsigned long int ShadowPitch;
	unsigned int Bpp;		/* Bytes per pixel */
	/* for 8bpp ... */
	unsigned pseudo_palette[256];

	/* wrapped funtions: */
	CloseScreenProcPtr CloseScreen;

	/* impact register backups: */
	struct {
		mgireg32_t main_mode;
		mgireg8_t  pp1select;
	} txt_xmap;
	unsigned short txt_vc3[0x20];

	/* impact register handling for FullHouse/SpeedRacer: */
	void (*WaitCfifoEmpty)(ImpactRegsPtr);
	unsigned (*WaitCfifo)(ImpactRegsPtr);
	unsigned (*WaitDMAOver)(ImpactRegsPtr);
	unsigned (*WaitDMAReady)(ImpactRegsPtr);
	unsigned short (*Vc3Get)(ImpactRegsPtr, CARD8);
	void (*Vc3Set)(ImpactRegsPtr, CARD8, unsigned short);
	CARD32 (*XmapGetModeRegister)(ImpactRegsPtr, CARD8);
	void (*XmapSetModeRegister)(ImpactRegsPtr, CARD8, CARD32);
	void (*RefreshArea8)(ScrnInfoPtr, int num, BoxPtr pbox);
	void (*RefreshArea32)(ScrnInfoPtr, int num, BoxPtr pbox);
	void (*FlushBoxCache)(int, char*, BoxPtr, unsigned);

	OptionInfoPtr Options;
} ImpactRec, *ImpactPtr;

#define IMPACTPTR(p) ((ImpactPtr)((p)->driverPrivate))
#define IMPACTREGSPTR(p) ((IMPACTPTR(p))->pImpactRegs)

/* Impact_regs.c */
void ImpactI2WaitCfifoEmpty(ImpactRegsPtr);
void ImpactSRWaitCfifoEmpty(ImpactRegsPtr);
void ImpactI2WaitDMAOver(ImpactRegsPtr);
void ImpactSRWaitDMAOver(ImpactRegsPtr);
void ImpactI2WaitDMAReady(ImpactRegsPtr);
void ImpactSRWaitDMAReady(ImpactRegsPtr);
unsigned ImpactI2WaitCfifo(ImpactRegsPtr,int);
unsigned ImpactSRWaitCfifo(ImpactRegsPtr,int);
unsigned short ImpactI2Vc3Get(ImpactRegsPtr, CARD8 vc2Ireg);
unsigned short ImpactSRVc3Get(ImpactRegsPtr, CARD8 vc2Ireg);
void ImpactI2Vc3Set(ImpactRegsPtr, CARD8 vc2Ireg, unsigned short val);
void ImpactSRVc3Set(ImpactRegsPtr, CARD8 vc2Ireg, unsigned short val);
void ImpactI2XmapSetModeRegister(ImpactRegsPtr, CARD8 address, CARD32 mode);
void ImpactSRXmapSetModeRegister(ImpactRegsPtr, CARD8 address, CARD32 mode);
CARD32 ImpactI2XmapGetModeRegister(ImpactRegsPtr, CARD8 address);
CARD32 ImpactSRXmapGetModeRegister(ImpactRegsPtr, CARD8 address);
void ImpactVc3DisableCursor(ScrnInfoPtr);
void ImpactVc3BlankScreen(ScrnInfoPtr, int);
void ImpactXmapSet24bpp(ScrnInfoPtr);
void ImpactBackupVc3(ScrnInfoPtr);
void ImpactRestoreVc3(ScrnInfoPtr);
void ImpactBackupRex(ScrnInfoPtr);
void ImpactRestoreRex(ScrnInfoPtr);
void ImpactBackupXmap(ScrnInfoPtr);
void ImpactRestoreXmap(ScrnInfoPtr);

/* newort_cmap.c */
static __inline__ unsigned
ImpactGetPalReg(ImpactPtr pImpact, int i)
{
	return pImpact->pseudo_palette[i & 0xff];
}
void ImpactLoadPalette(ScrnInfoPtr, int numColors, int *indices,
		LOCO* colors, VisualPtr);
void ImpactRestorePalette(ScrnInfoPtr);
void ImpactBackupPalette(ScrnInfoPtr);

/* impact_shadow.c */
int ImpactDepth24Flags(void);
void ImpactI2RefreshArea8(ScrnInfoPtr, int num, BoxPtr pbox);
void ImpactSRRefreshArea8(ScrnInfoPtr, int num, BoxPtr pbox);
void ImpactI2RefreshArea32(ScrnInfoPtr, int num, BoxPtr pbox);
void ImpactSRRefreshArea32(ScrnInfoPtr, int num, BoxPtr pbox);

#endif /* __IMPACT_H__ */
