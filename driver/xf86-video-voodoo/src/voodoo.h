#include "voodoo_pcirename.h"
#include <string.h>

#include "compat-api.h"

typedef struct {
  CARD32		m;
  CARD32		n;
  CARD32		p;
} PLLClock;

typedef struct {
  CARD8 *             ShadowPtr;	/* Shadow buffer */
  CARD32              ShadowPitch;
  CloseScreenProcPtr  CloseScreen;	/* Wrapped Close */
#ifdef HAVE_XAA_H
  XAAInfoRecPtr	      AccelInfoRec;	/* Cached Accel rec for close */
#endif
  Bool                Blanked;
  Bool                PassThrough;     /* Set to restore pass through on exit */
  EntityInfoPtr       pEnt;
  OptionInfoPtr       Options;
  
  Bool		      Voodoo2;		/* Set if Voodoo2 */
  pciVideoPtr	      PciInfo;		/* PCI data */
#ifndef XSERVER_LIBPCIACCESS
  PCITAG	      PciTag;
#endif
  CARD32	      PhysBase;
  
  CARD32	      Width;		/* Current width */
  CARD32	      Height;   	/* Current height */
  CARD32	      FullHeight;	/* Height including pixmap cache */
  CARD32	      Tiles;		/* 32 tile count */
  
  int		      BlitDirX;		/* Cache blitter direction */
  int		      BlitDirY;		/* Cache blitter direction */
  
  CARD32	      lfbMode;		/* Cached lfbMode value */

  CARD32	      alpha;		/* Cached alpha reg for sw blit */
  CARD32	      alphaPitch;	/* Software render blit state */
  int		      alphaType;
  CARD8		      *alphaPtr;
  CARD32	      alphaC;
  CARD32	      alphaW;
  CARD32              alphaH;
  
  CARD32	      texPitch; 
  int		      texType;
  CARD8		      *texPtr;
  CARD32	      texW;
  CARD32              texH;
  
  int		      ShadowFB;		/* Using shadow FB */
  int		      Accel;		/* Using acceleration */
  
  CARD8	*	      MMIO;		/* MMIO base */
  CARD8 * 	      FBBase;   	/* Virtual FB base */
  CARD32	      Pitch;		/* Pitch of FB */

  DGAModePtr	      pDGAMode;		/* DGA mode */
  int		      nDGAMode;
  
  int		      DAC;
#define DAC_ID_ATT	1
#define DAC_ID_TI	2
#define DAC_ID_ICS	3
#define DAC_UNKNOWN     0
  CARD32	      MaxClock;

  PLLClock	      vClock;
  PLLClock	      gClock;
  
  unsigned char	      LineBuffer[1028];	/* Draw buffer */
  unsigned char       *LinePtr;		/* To keep XAA amused */  
} VoodooRec, *VoodooPtr;

#define TRUE 1
#define FALSE 0

/* Card-specific driver information */

#define VoodooPTR(p) ((VoodooPtr)((p)->driverPrivate))

#define VOODOO_VERSION 4000
#define VOODOO_NAME "Voodoo"
#define VOODOO_DRIVER_NAME "voodoo"
#define VOODOO_MAJOR_VERSION 1
#define VOODOO_MINOR_VERSION 1
#define VOODOO_PATCHLEVEL 0

#define PCI_CHIP_VOODOO1	0x0001
#define PCI_CHIP_VOODOO2	0x0002

/*
 *	Hardware functions
 */
 
extern void VoodooClear(VoodooPtr pVoo);
extern void VoodooCopy16(VoodooPtr pVoo, CARD32 x1, CARD32 y1, CARD32 w, CARD32 h, CARD32 spitch, unsigned char *src);
extern void VoodooCopy24(VoodooPtr pVoo, CARD32 x1, CARD32 y1, CARD32 w, CARD32 h, CARD32 spitch, unsigned char *src);
extern int VoodooHardwareInit(VoodooPtr pVoo);
extern int VoodooMode(ScrnInfoPtr pScrn, DisplayModePtr mode);
extern void VoodooBlank(VoodooPtr pVoo);
extern int VoodooMemorySize(VoodooPtr pVoo);
extern void Voodoo2XAAInit(ScreenPtr pScreen);
extern void VoodooSync(ScrnInfoPtr pScrn);
extern void VoodooReadBank(ScreenPtr pScreen, int bank);
extern void VoodooWriteBank(ScreenPtr pScreen, int bank);
extern void VoodooRestorePassThrough(VoodooPtr pVoo);

/*
 *	DGA
 */

extern Bool VoodooDGAInit(ScrnInfoPtr pScrn, ScreenPtr pScreen);

