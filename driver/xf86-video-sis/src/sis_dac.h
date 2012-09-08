/*
 * DAC helper functions (Save/Restore, MemClk, etc)
 * Definitions and prototypes
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1) Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2) Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3) The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

int  SiS_compute_vclk(int Clock, int *out_n, int *out_dn, int *out_div,
	     		int *out_sbit, int *out_scale);
void SISDACPreInit(ScrnInfoPtr pScrn);
void SISLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indicies,
		        LOCO *colors, VisualPtr pVisual);
void SiSCalcClock(ScrnInfoPtr pScrn, int clock, int max_VLD,
                        unsigned int *vclk);
void SiSIODump(ScrnInfoPtr pScrn);
int  SiSMemBandWidth(ScrnInfoPtr pScrn, Bool IsForCRT2);
int  SiSMclk(SISPtr pSiS);
void SiSRestoreBridge(ScrnInfoPtr pScrn, SISRegPtr sisReg);
void SiS_UpdateGammaCRT2(ScrnInfoPtr pScrn);

extern void	SiS6326SetTVReg(ScrnInfoPtr pScrn, CARD8 index, CARD8 data);
extern UChar	SiS6326GetTVReg(ScrnInfoPtr pScrn, CARD8 index);
extern void	SiS6326SetXXReg(ScrnInfoPtr pScrn, CARD8 index, CARD8 data);
extern UChar	SiS6326GetXXReg(ScrnInfoPtr pScrn, CARD8 index);

extern float	SiSCalcVRate(DisplayModePtr mode);

extern void	SISCalculateGammaRampCRT2(ScrnInfoPtr pScrn);

/* Functions from init.c and init301.c (use their datatypes!) */
extern void		SiS_UnLockCRT2(struct SiS_Private *SiS_Pr);
extern void		SiS_LockCRT2(struct SiS_Private *SiS_Pr);
extern void		SiS_DisableBridge(struct SiS_Private *SiS_Pr);
extern void		SiS_EnableBridge(struct SiS_Private *SiS_Pr);
extern unsigned short	SiS_GetCH700x(struct SiS_Private *SiS_Pr, unsigned short reg);
extern void		SiS_SetCH700x(struct SiS_Private *SiS_Pr, unsigned short reg, unsigned char val);
extern unsigned short	SiS_GetCH701x(struct SiS_Private *SiS_Pr, unsigned short reg);
extern void		SiS_SetCH701x(struct SiS_Private *SiS_Pr, unsigned short reg, unsigned char val);
extern unsigned short	SiS_GetCH70xx(struct SiS_Private *SiS_Pr, unsigned short reg);
extern void		SiS_SetCH70xx(struct SiS_Private *SiS_Pr, unsigned short reg, unsigned char val);
extern void		SiS_SetCH70xxANDOR(struct SiS_Private *SiS_Pr, unsigned short reg, unsigned char orval, unsigned char andval);
extern void		SiS_DDC2Delay(struct SiS_Private *SiS_Pr, unsigned int delaytime);
extern unsigned short	SiS_ReadDDC1Bit(struct SiS_Private *SiS_Pr);
extern unsigned short	SiS_HandleDDC(struct SiS_Private *SiS_Pr, unsigned int VBFlags, int VGAEngine,
                              unsigned short adaptnum, unsigned short DDCdatatype, unsigned char *buffer,
			      unsigned int VBFlags2);
extern void		SiS_SetChrontelGPIO(struct SiS_Private *SiS_Pr, unsigned short myvbinfo);
extern void		SiS_DisplayOn(struct SiS_Private *SiS_Pr);
extern unsigned char	SiS_GetSetModeID(ScrnInfoPtr pScrn, unsigned char id);
extern void		SiS_SetEnableDstn(struct SiS_Private *SiS_Pr, int enable);
extern void		SiS_SetEnableFstn(struct SiS_Private *SiS_Pr, int enable);
extern void		SiSRegInit(struct SiS_Private *SiS_Pr, SISIOADDRESS BaseAddr);
extern void		SiSSetLVDSetc(struct SiS_Private *SiS_Pr);
extern void		SiS_GetVBType(struct SiS_Private *SiS_Pr);
/* End of init.c/init301.c imports */
