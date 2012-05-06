/*
 * SiS driver utility interface & routines
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
 * Author: Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sis.h"
#include <X11/X.h>
#include "dixstruct.h"

#include "sis_videostr.h"

#ifdef TWDEBUG
#define SIS_NEED_orSISIDXREG
#include "sis_regs.h"
#endif

/* Definitions for SISCTRL extension */

#define SISCTRL_PROTOCOL_NAME	"SISCTRL"

#define SISCTRL_MAJOR_VERSION		0	/* current version numbers */
#define SISCTRL_MINOR_VERSION		1

#define SISCTRL_MAX_SCREENS		32	/* number of screens the ext can handle */

#define X_SiSCtrlQueryVersion		0
#define X_SiSCtrlCommand		1

#define SDC_ID				0x53495321

#define SDC_VERSION			1

#define SDC_NUM_PARM_RESULT		20

/* sdc_command */
#define SDC_CMD_GETVERSION 		0x98980001
#define SDC_CMD_GETHWINFO		0x98980002
#define SDC_CMD_GETALLFLAGS		0x98980003
#define SDC_CMD_GETVBFLAGSVERSION	0x98980004
#define SDC_CMD_GETVBFLAGS		0x98980005
#define SDC_CMD_CHECKMODEFORCRT2	0x98980006
#define SDC_CMD_SETVBFLAGS		0x98980007
#define SDC_CMD_GETDETECTEDDEVICES	0x98980008
#define SDC_CMD_REDETECTCRT2DEVICES	0x98980009
#define SDC_CMD_GETCRT1STATUS		0x9898000a
#define SDC_CMD_SETCRT1STATUS		0x9898000b
#define SDC_CMD_GETSDFLAGS		0x9898000c
#define SDC_CMD_GETSD2FLAGS		0x9898000d
#define SDC_CMD_GETLOCKSTATUS		0x9898000e
#define SDC_CMD_SETLOCKSTATUS		0x9898000f
#define SDC_CMD_GETTVANTIFLICKER	0x98980010
#define SDC_CMD_SETTVANTIFLICKER	0x98980011
#define SDC_CMD_GETTVSATURATION		0x98980012
#define SDC_CMD_SETTVSATURATION		0x98980013
#define SDC_CMD_GETTVEDGEENHANCE	0x98980014
#define SDC_CMD_SETTVEDGEENHANCE	0x98980015
#define SDC_CMD_GETTVCFILTER		0x98980016
#define SDC_CMD_SETTVCFILTER		0x98980017
#define SDC_CMD_GETTVYFILTER		0x98980018
#define SDC_CMD_SETTVYFILTER		0x98980019
#define SDC_CMD_GETTVCOLORCALIB		0x9898001a
#define SDC_CMD_SETTVCOLORCALIB		0x9898001b
#define SDC_CMD_GETTVCHCONTRAST		0x9898001c
#define SDC_CMD_SETTVCHCONTRAST		0x9898001d
#define SDC_CMD_GETTVCHTEXTENHANCE	0x9898001e
#define SDC_CMD_SETTVCHTEXTENHANCE	0x9898001f
#define SDC_CMD_GETTVCHCHROMAFLICKERFILTER 0x98980020
#define SDC_CMD_SETTVCHCHROMAFLICKERFILTER 0x98980021
#define SDC_CMD_GETTVCHLUMAFLICKERFILTER 0x98980022
#define SDC_CMD_SETTVCHLUMAFLICKERFILTER 0x98980023
#define SDC_CMD_GETTVCHCVBSCOLOR	0x98980024
#define SDC_CMD_SETTVCHCVBSCOLOR	0x98980025
#define SDC_CMD_GETCHTVOVERSCAN		0x98980026
#define SDC_CMD_SETCHTVOVERSCAN		0x98980027
#define SDC_CMD_GETGAMMASTATUS		0x98980028
#define SDC_CMD_SETGAMMASTATUS		0x98980029
#define SDC_CMD_GETTVXSCALE		0x9898002a
#define SDC_CMD_SETTVXSCALE		0x9898002b
#define SDC_CMD_GETTVYSCALE		0x9898002c
#define SDC_CMD_SETTVYSCALE		0x9898002d
#define SDC_CMD_GETSCREENSIZE		0x9898002e
#define SDC_CMD_GETGAMMABRIGHTNESS	0x9898002f
#define SDC_CMD_SETGAMMABRIGHTNESS	0x98980030
#define SDC_CMD_GETGAMMABRIGHTNESS2	0x98980031
#define SDC_CMD_SETGAMMABRIGHTNESS2	0x98980032
#define SDC_CMD_GETGETGAMMACRT2		0x98980033
#define SDC_CMD_SETGETGAMMACRT2		0x98980034
#define SDC_CMD_GETHWCURSORSTATUS	0x98980035
#define SDC_CMD_SETHWCURSORSTATUS	0x98980036
#define SDC_CMD_GETPANELMODE		0x98980037
#define SDC_CMD_SETPANELMODE		0x98980038
#define SDC_CMD_GETMERGEDMODEDETAILS	0x98980039
#define SDC_CMD_GETDEVICENAME		0x9898003a
#define SDC_CMD_GETMONITORNAME		0x9898003b
#define SDC_CMD_GETDEVICENAME2		0x9898003c
#define SDC_CMD_GETMONITORNAME2		0x9898003d
#define SDC_CMD_SETXVBRIGHTNESS		0x9898003e
#define SDC_CMD_GETXVBRIGHTNESS		0x9898003f
#define SDC_CMD_SETXVCONTRAST		0x98980040
#define SDC_CMD_GETXVCONTRAST		0x98980041
#define SDC_CMD_SETXVHUE		0x98980042
#define SDC_CMD_GETXVHUE		0x98980043
#define SDC_CMD_SETXVSATURATION		0x98980044
#define SDC_CMD_GETXVSATURATION		0x98980045
#define SDC_CMD_SETXVGAMMA		0x98980046
#define SDC_CMD_GETXVGAMMA		0x98980047
#define SDC_CMD_SETXVCOLORKEY		0x98980048
#define SDC_CMD_GETXVCOLORKEY		0x98980049
#define SDC_CMD_SETXVAUTOPAINTCOLORKEY	0x9898004a
#define SDC_CMD_GETXVAUTOPAINTCOLORKEY	0x9898004b
#define SDC_CMD_SETXVDEFAULTS		0x9898004c
#define SDC_CMD_SETXVDISABLEGFX		0x9898004d
#define SDC_CMD_GETXVDISABLEGFX		0x9898004e
#define SDC_CMD_SETXVDISABLEGFXLR	0x9898004f
#define SDC_CMD_GETXVDISABLEGFXLR	0x98980050
#define SDC_CMD_SETXVSWITCHCRT		0x98980051
#define SDC_CMD_GETXVSWITCHCRT		0x98980052
#define SDC_CMD_GETTVXPOS		0x98980053
#define SDC_CMD_SETTVXPOS		0x98980054
#define SDC_CMD_GETTVYPOS		0x98980055
#define SDC_CMD_SETTVYPOS		0x98980056
#define SDC_CMD_SETXVDEINT		0x98980057
#define SDC_CMD_GETXVDEINT		0x98980058
#define SDC_CMD_GETMONGAMMACRT1		0x98980059
#define SDC_CMD_GETMONGAMMACRT2		0x9898005a
#define SDC_CMD_LOGQUIET		0x9898005b
#define SDC_CMD_GETNEWGAMMABRICON	0x9898005c
#define SDC_CMD_SETNEWGAMMABRICON	0x9898005d
#define SDC_CMD_GETNEWGAMMABRICON2	0x9898005e
#define SDC_CMD_SETNEWGAMMABRICON2	0x9898005f
#define SDC_CMD_GETGETNEWGAMMACRT2	0x98980060
#define SDC_CMD_SETGETNEWGAMMACRT2	0x98980061
#define SDC_CMD_NEWSETVBFLAGS		0x98980062
#define SDC_CMD_GETCRT1SATGAIN		0x98980063
#define SDC_CMD_SETCRT1SATGAIN		0x98980064
#define SDC_CMD_GETCRT2SATGAIN		0x98980065
#define SDC_CMD_SETCRT2SATGAIN		0x98980066
/* more to come, adapt MAXCOMMAND! */
#define SDC_MAXCOMMAND			SDC_CMD_SETCRT2SATGAIN

/* in result_header */
#define SDC_RESULT_OK  			0x66670000
#define SDC_RESULT_UNDEFCMD		0x66670001
#define SDC_RESULT_NOPERM		0x66670002
#define SDC_RESULT_INVAL		0x66670003
#define SDC_RESULT_MESSAGEERROR		0x66670004  /* Client side only */
#define SDC_RESULT_NOEXTENSION		0x66670005  /* Client side only */

/* For SDC_CMD_GETHWINFO */
#define SDC_BUS_TYPE_PCI		0
#define SDC_BUS_TYPE_AGP		1
#define SDC_BUS_TYPE_PCIE		2
#define SDC_BUS_TYPE_USB		3

/* For SDC_CMD_GETMERGEDMODEDETAILS */
#define SDC_MMODE_POS_ERROR		0
#define SDC_MMODE_POS_LEFTOF		1
#define SDC_MMODE_POS_RIGHTOF		2
#define SDC_MMODE_POS_ABOVE		3
#define SDC_MMODE_POS_BELOW		4
#define SDC_MMODE_POS_CLONE		5

typedef struct _SiSCtrlQueryVersion {
    CARD8	reqType;		/* always SiSCtrlReqCode */
    CARD8	SiSCtrlReqType;		/* always X_SiSCtrlQueryVersion */
    CARD16	length B16;
} xSiSCtrlQueryVersionReq;
#define sz_xSiSCtrlQueryVersionReq	4

typedef struct {
    BYTE	type;			/* X_Reply */
    BOOL	pad1;
    CARD16	sequenceNumber B16;
    CARD32	length B32;
    CARD16	majorVersion B16;	/* major version of SISCTRL */
    CARD16	minorVersion B16;	/* minor version of SISCTRL */
    CARD32	pad2 B32;
    CARD32	pad3 B32;
    CARD32	pad4 B32;
    CARD32	pad5 B32;
    CARD32	pad6 B32;
} xSiSCtrlQueryVersionReply;
#define sz_xSiSCtrlQueryVersionReply	32

typedef struct {
    CARD8	reqType;		/* always SiSCtrlReqCode */
    CARD8	SiSCtrlReqType;		/* always SiSCtrl_SiSCtrlCommand */
    CARD16	length B16;
    CARD32	pad1 B32;
    CARD32	screen;
    CARD32 	sdc_id;
    CARD32 	sdc_chksum;
    CARD32	sdc_command;
    CARD32	sdc_parm[SDC_NUM_PARM_RESULT];
    CARD32	sdc_result_header;
    CARD32	sdc_result[SDC_NUM_PARM_RESULT];
    char	sdc_buffer[32];
} xSiSCtrlCommandReq;
#define sz_xSiSCtrlCommandReq		(28 + (SDC_NUM_PARM_RESULT * 4 * 2) + 32)

typedef struct {
    BYTE	type;			/* X_Reply */
    BOOL	pad1;
    CARD16	sequenceNumber B16;
    CARD32	length B32;
    CARD32	screen;
    CARD32 	sdc_id;
    CARD32 	sdc_chksum;
    CARD32	sdc_command;
    CARD32	sdc_parm[SDC_NUM_PARM_RESULT];
    CARD32	sdc_result_header;
    CARD32	sdc_result[SDC_NUM_PARM_RESULT];
    char	sdc_buffer[32];
} xSiSCtrlCommandReply;
#define sz_xSiSCtrlCommandReply		(28 + (SDC_NUM_PARM_RESULT * 4 * 2) + 32)

typedef struct {
    unsigned int maxscreens;		/* Max number of entries = SISCTRL_MAX_SCREENS*/
    unsigned int version_major;		/* Extension major version */
    unsigned int version_minor; 	/* Extension minor version */
    int		 (*HandleSiSDirectCommand[SISCTRL_MAX_SCREENS])(xSiSCtrlCommandReply *);
} xSiSCtrlScreenTable;

void		SiSCtrlExtInit(ScrnInfoPtr pScrn);
void		SiSCtrlExtUnregister(SISPtr pSiS, int index);

#ifdef XV_SD_DEPRECATED
int		SISSetPortUtilAttribute(ScrnInfoPtr pScrn, Atom attribute,
					INT32 value, SISPortPrivPtr pPriv);
int		SISGetPortUtilAttribute(ScrnInfoPtr pScrn,  Atom attribute,
					INT32 *value, SISPortPrivPtr pPriv);
#endif

extern Bool 	SISRedetectCRT2Type(ScrnInfoPtr pScrn);
extern UShort	SiS_CheckModeCRT1(ScrnInfoPtr pScrn, DisplayModePtr mode,
				 unsigned int VBFlags, Bool hcm);
extern UShort	SiS_CheckModeCRT2(ScrnInfoPtr pScrn, DisplayModePtr mode,
				 unsigned int VBFlags, Bool hcm);
extern void	SISAdjustFrame(int scrnIndex, int x, int y, int flags);
extern float	SiSCalcVRate(DisplayModePtr mode);
extern void	SiS_UpdateGammaCRT2(ScrnInfoPtr pScrn);
#ifdef SISGAMMARAMP
extern void	SISCalculateGammaRamp(ScreenPtr pScreen, ScrnInfoPtr pScrn);
#endif

extern void	SISSetPortDefaults(ScrnInfoPtr pScrn, SISPortPrivPtr pPriv);
extern void	SISUpdateVideoParms(SISPtr pSiS, SISPortPrivPtr pPriv);
extern void	SiSUpdateXvGamma(SISPtr pSiS, SISPortPrivPtr pPriv);

/***********************************
 *        Low-level routines       *
 ***********************************/

static Bool
SISSwitchCRT1Status(ScrnInfoPtr pScrn, int onoff, Bool quiet)
{
    SISPtr pSiS = SISPTR(pScrn);
    DisplayModePtr mode = pScrn->currentMode;
    ULong vbflags = pSiS->VBFlags;
    ULong vbflags3 = pSiS->VBFlags3;
    int crt1off;

    /* onoff: 0=OFF, 1=ON(VGA), 2=ON(LCDA) */

    /* Switching to LCDA will disable CRT2 if previously LCD or VGA,
     * unless on a dual-vb setup
     */

    /* For usability reasons, the user should not simply "lose" one
     * of his output devices in MergedFB mode. Therefore, a switch
     * which might lead to this situation will not be performed in
     * MergedFB mode. (For example: If CRT2 is either LCD or VGA,
     * don't let the user switch to LCD-via-CRT1 mode, because he
     * would lose one output device since LCD-via-CRT1 is only
     * supported together with TV, not any other CRT2 type.)
     * In Non-MergedFB mode, losing one output device is not
     * considered that harmful.
     * Update: We let the user switch off a device if currently
     * a "clone" mode is active. Same for switching CRT1 to LCD
     * while CRT2 is LCD or VGA.
     */

    /* Do NOT use this to switch from CRT1_LCDA to CRT2_LCD */

    /* Only on 300 and 315/330/340 series */
    if(pSiS->VGAEngine != SIS_300_VGA &&
       pSiS->VGAEngine != SIS_315_VGA) return FALSE;

    /* Off only if at least one CRT2 device is active */
    if((!onoff) && (!(vbflags & CRT2_ENABLE))) return FALSE;

#ifdef SISDUALHEAD
    if(pSiS->DualHeadMode) return FALSE;
#endif

    /* Can't switch to LCDA if not supported (duh!) */
    if(!(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTLCDA)) {
       if(onoff == 2) {
          if(!quiet) {
             xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	        "LCD-via-CRT1 not supported by hardware or no panel detected\n");
	  }
          return FALSE;
       }
    }

#ifdef SISMERGED
    if(pSiS->MergedFB) {
       if(((SiSMergedDisplayModePtr)mode->Private)->CRT2Position != sisClone) {
          if(!onoff) {
             if(!quiet) {
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	           "CRT1 can't be switched off in MergedFB mode unless a clone mode is active\n");
	     }
             return FALSE;
          } else if(onoff == 2) {
             if(!(pSiS->SiS_SD3_Flags & SiS_SD3_SUPPORTDUALDVI)) {
                if(vbflags & (CRT2_LCD|CRT2_VGA)) {
	           if(!quiet) {
	              xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	   	         "CRT1 type can only be VGA while CRT2 is LCD or VGA\n");
	           }
                   return FALSE;
                }
	     }
          }
       }
       if(mode->Private) {
	  mode = ((SiSMergedDisplayModePtr)mode->Private)->CRT1;
       }
    }
#endif

    vbflags &= ~(DISPTYPE_CRT1 | SINGLE_MODE | MIRROR_MODE | CRT1_LCDA);
    vbflags3 &= ~(VB3_CRT1_TV | VB3_CRT1_LCD | VB3_CRT1_VGA);
    crt1off = 1;
    if(onoff > 0) {
       vbflags |= DISPTYPE_CRT1;
       crt1off = 0;
       if(onoff == 2) {
	  vbflags |= CRT1_LCDA;
	  vbflags3 |= VB3_CRT1_LCD;
	  if(!(pSiS->SiS_SD3_Flags & SiS_SD3_SUPPORTDUALDVI)) {
	     vbflags &= ~(CRT2_LCD|CRT2_VGA);
	  }
       } else {
          vbflags3 |= VB3_CRT1_VGA;
       }
       /* Remember: Dualhead not supported */
       if(vbflags & CRT2_ENABLE) vbflags |= MIRROR_MODE;
       else vbflags |= SINGLE_MODE;
    } else {
       vbflags |= SINGLE_MODE;
    }

    if(vbflags & CRT1_LCDA) {
       if(SiS_CheckModeCRT1(pScrn, mode, vbflags, pSiS->HaveCustomModes) < 0x14) {
          if(!quiet) {
             xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"Current mode not suitable for LCD-via-CRT1\n");
	  }
          return FALSE;
       }
    }

    pSiS->CRT1off = crt1off;
    pSiS->VBFlags = pSiS->VBFlags_backup = vbflags;
    pSiS->VBFlags3 = pSiS->VBFlags_backup3 = vbflags3;

    /* Sync the accelerators */
    (*pSiS->SyncAccel)(pScrn);

    pSiS->skipswitchcheck = TRUE;
    if(!(pScrn->SwitchMode(pScrn->scrnIndex, pScrn->currentMode, 0))) {
       pSiS->skipswitchcheck = FALSE;
       return FALSE;
    }
    pSiS->skipswitchcheck = FALSE;
    SISAdjustFrame(pScrn->scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);
    return TRUE;
}

static Bool
SISRedetectCRT2Devices(ScrnInfoPtr pScrn)
{
    SISPtr pSiS = SISPTR(pScrn);

    if((pSiS->VGAEngine != SIS_300_VGA) && (pSiS->VGAEngine != SIS_315_VGA)) {
       return FALSE;
    }

#ifdef SISDUALHEAD
    if(pSiS->DualHeadMode) return FALSE;
#endif

    /* Sync the accelerators */
    (*pSiS->SyncAccel)(pScrn);

    if(SISRedetectCRT2Type(pScrn)) {
       /* If this returns TRUE, we need to reset the display mode */
       /* Sync the accelerators */
       (*pSiS->SyncAccel)(pScrn);
       pSiS->skipswitchcheck = TRUE;
       if(!(pScrn->SwitchMode(pScrn->scrnIndex, pScrn->currentMode, 0))) {
          pSiS->skipswitchcheck = FALSE;
          return FALSE;
       }
       pSiS->skipswitchcheck = FALSE;
       SISAdjustFrame(pScrn->scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);
    }
    return TRUE;
}

static Bool
SISSwitchCRT2Type(ScrnInfoPtr pScrn, ULong newvbflags, Bool quiet)
{
    SISPtr pSiS = SISPTR(pScrn);
    Bool hcm = pSiS->HaveCustomModes;
    DisplayModePtr mode = pScrn->currentMode;

    /* Do NOT use this to switch from CRT2_LCD to CRT1_LCDA */

    /* Switching CRT2 to LCD or VGA will switch CRT1 to VGA if
     * previously LCD-via-CRT1
     */

    /* For usability reasons, the user should not simply "lose" one
     * of his output devices in MergedFB mode. Therefore, a switch
     * which might lead to this situation will not be performed in
     * MergedFB mode. (For example: If CRT1 is LCD-via-CRT1, don't
     * let the user switch CRT2 to LCD or VGA mode, because he
     * would lose one output device since LCD-via-CRT1 is only
     * supported together with TV, not any other CRT2 type.)
     * In Non-MergedFB mode, losing one output device is not
     * considered that harmful.
     * Update: We allow this if currently a "clone" display mode
     * is active.
     */

    /* Only on 300 and 315/330/340 series */
    if(pSiS->VGAEngine != SIS_300_VGA &&
       pSiS->VGAEngine != SIS_315_VGA) return FALSE;

    /* Only if there is a video bridge */
    if(!(pSiS->VBFlags2 & VB2_VIDEOBRIDGE)) return FALSE;

#ifdef SISDUALHEAD
    if(pSiS->DualHeadMode) return FALSE;
#endif

#define SiS_NewVBMask (CRT2_ENABLE|CRT1_LCDA|TV_PAL|TV_NTSC|TV_PALM|TV_PALN|TV_NTSCJ| \
		       TV_AVIDEO|TV_SVIDEO|TV_SCART|TV_HIVISION|TV_YPBPR|TV_YPBPRALL|\
		       TV_YPBPRAR)

    newvbflags &= SiS_NewVBMask;
    newvbflags |= pSiS->VBFlags & ~SiS_NewVBMask;

    if(!(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTLCDA)) {
       newvbflags &= ~CRT1_LCDA;
    }
    if(!(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTHIVISION)) {
       newvbflags &= ~TV_HIVISION;
    }
    if(!(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTYPBPR)) {
       newvbflags &= ~TV_YPBPR;
    }

#ifdef SISMERGED
    if(pSiS->MergedFB) {
       if((mode->Private) &&
          ((SiSMergedDisplayModePtr)mode->Private)->CRT2Position != sisClone) {
          if(!(newvbflags & CRT2_ENABLE)) {
             if(!quiet) {
	        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	           "CRT2 can't be switched off in MergedFB mode unless a clone mode is active\n");
	     }
	     return FALSE;
          }
          if(!(pSiS->SiS_SD3_Flags & SiS_SD3_SUPPORTDUALDVI)) {
             if((newvbflags & (CRT2_LCD|CRT2_VGA)) && (newvbflags & CRT1_LCDA)) {
                if(!quiet) {
	           xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	              "CRT2 type can only be TV while in LCD-via-CRT1 mode\n");
	        }
	        return FALSE;
	     }
          }
       }
       hcm = pSiS->HaveCustomModes2;
       if(mode->Private) {
	  mode = ((SiSMergedDisplayModePtr)mode->Private)->CRT2;
       }
    }
#endif

    if((!(newvbflags & CRT2_ENABLE)) && (!newvbflags & DISPTYPE_CRT1)) {
       if(!quiet) {
          xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
             "CRT2 can't be switched off while CRT1 is off\n");
       }
       return FALSE;
    }

    /* CRT2_LCD and CRT2_VGA overrule LCDA (in non-MergedFB mode) */
    if(!(pSiS->SiS_SD3_Flags & SiS_SD3_SUPPORTDUALDVI)) {
       if(newvbflags & (CRT2_LCD|CRT2_VGA)) {
          newvbflags &= ~CRT1_LCDA;
       }
    }

    /* Check if the current mode is suitable for desired output device (if any) */
    if(SiS_CheckModeCRT2(pScrn, mode, newvbflags, hcm) < 0x14) {
       if(!quiet) {
          xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	     "Current mode not suitable for desired CRT2 output device\n");
       }
       return FALSE;
    }

    /* Remember: Dualhead not supported */
    newvbflags &= ~(SINGLE_MODE | MIRROR_MODE);
    if((newvbflags & DISPTYPE_CRT1) && (newvbflags & CRT2_ENABLE)) {
       newvbflags |= MIRROR_MODE;
    } else {
       newvbflags |= SINGLE_MODE;
    }

    /* Sync the accelerators */
    (*pSiS->SyncAccel)(pScrn);

    pSiS->VBFlags = pSiS->VBFlags_backup = newvbflags;

    pSiS->skipswitchcheck = TRUE;
    if(!(pScrn->SwitchMode(pScrn->scrnIndex, pScrn->currentMode, 0))) {
       pSiS->skipswitchcheck = FALSE;
       return FALSE;
    }
    pSiS->skipswitchcheck = FALSE;
    SISAdjustFrame(pScrn->scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);
    return TRUE;
}

static Bool
SISSwitchOutputType(ScrnInfoPtr pScrn, ULong newvbflags, ULong newvbflags3,
		ULong newvbflags4, Bool quiet)
{
    /* For future use */

    return FALSE;
}

static ULong
SiSCondToVBFlags(UShort cond, ULong GivenVBFlags)
{
    ULong vbflags = GivenVBFlags;

    /* No special treatment for NTSC-J here; conditions equal NTSC */
    if(cond) {
       vbflags &= ~(CRT2_ENABLE | CRT1_LCDA | TV_STANDARD | TV_INTERFACE);
       if((cond & SiS_CF2_TYPEMASK) == SiS_CF2_LCD) {
	  vbflags |= CRT2_LCD;
       } else if((cond & SiS_CF2_TYPEMASK) == SiS_CF2_TV) {
	  vbflags |= (CRT2_TV | TV_SVIDEO);
	  if(cond & SiS_CF2_TVPAL)  	  vbflags |= TV_PAL;
	  else if(cond & SiS_CF2_TVPALM)  vbflags |= (TV_PAL | TV_PALM);
	  else if(cond & SiS_CF2_TVPALN)  vbflags |= (TV_PAL | TV_PALN);
	  else if(cond & SiS_CF2_TVNTSC)  vbflags |= TV_NTSC;
       } else if((cond & SiS_CF2_TYPEMASK) == SiS_CF2_TVSPECIAL) {
          vbflags |= CRT2_TV;
	  if((cond & SiS_CF2_TVSPECMASK) == SiS_CF2_TVHIVISION)
		vbflags |= TV_HIVISION;
	  else if((cond & SiS_CF2_TVSPECMASK) == SiS_CF2_TVYPBPR525I)
		vbflags |= (TV_YPBPR | TV_YPBPR525I);
	  else if((cond & SiS_CF2_TVSPECMASK) == SiS_CF2_TVYPBPR525P)
		vbflags |= (TV_YPBPR | TV_YPBPR525P);
	  else if((cond & SiS_CF2_TVSPECMASK) == SiS_CF2_TVYPBPR625I)
		vbflags |= (TV_YPBPR | TV_YPBPR625I);
	  else if((cond & SiS_CF2_TVSPECMASK) == SiS_CF2_TVYPBPR625P)
		vbflags |= (TV_YPBPR | TV_YPBPR625P);
	  else if((cond & SiS_CF2_TVSPECMASK) == SiS_CF2_TVYPBPR750P)
		vbflags |= (TV_YPBPR | TV_YPBPR750P);
	  else if((cond & SiS_CF2_TVSPECMASK) == SiS_CF2_TVYPBPR1080I)
		vbflags |= (TV_YPBPR | TV_YPBPR1080I);
       } else if((cond & SiS_CF2_TYPEMASK) == SiS_CF2_VGA2) {
	  vbflags |= CRT2_VGA;
       } else if((cond & SiS_CF2_TYPEMASK) == SiS_CF2_CRT1LCDA) {
	  vbflags |= CRT1_LCDA;
       }
    }
    return(vbflags);
}

static int
SISCheckModeForCRT2Type(ScrnInfoPtr pScrn, DisplayModePtr mode, ULong vbflags, UShort cond, Bool quiet)
{
    SISPtr pSiS = SISPTR(pScrn);
    DisplayModePtr mastermode;
    Bool hcm = pSiS->HaveCustomModes;
    int result = 0;

    mastermode = mode;

#ifdef SISDUALHEAD
    if((!pSiS->DualHeadMode) || (!pSiS->SecondHead)) {
#endif

       if(vbflags & CRT2_ENABLE) {

#ifdef SISMERGED
          if(pSiS->MergedFB) {
             hcm = pSiS->HaveCustomModes2;
             if(mode->Private) {
	        mode = ((SiSMergedDisplayModePtr)mode->Private)->CRT2;
             }
          }
#endif

          /* For RandR */
          if((mode->HDisplay > pScrn->virtualX) || (mode->VDisplay > pScrn->virtualY)) {
             if(!quiet) {
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "Desired mode too large for current screen size\n");
             }
             result |= 0x02;
          }

          /* Check if the desired mode is suitable for current CRT2 output device */
          if(SiS_CheckModeCRT2(pScrn, mode, vbflags, hcm) < 0x14) {
             if((!cond) && (!quiet)) {
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "Desired mode not suitable for current CRT2 output device\n");
             }
             result |= 0x01;
          }

       }

#ifdef SISDUALHEAD
    }
#endif

    mode = mastermode;

#ifdef SISDUALHEAD
    if((!pSiS->DualHeadMode) || (pSiS->SecondHead)) {
#endif

       if(vbflags & CRT1_LCDA) {

#ifdef SISMERGED
          if(pSiS->MergedFB) {
             hcm = pSiS->HaveCustomModes;
             if(mode->Private) {
	        mode = ((SiSMergedDisplayModePtr)mode->Private)->CRT1;
	     }
          }
#endif

	  /* For RandR */
	  if((mode->HDisplay > pScrn->virtualX) || (mode->VDisplay > pScrn->virtualY)) {
	     if(!quiet) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			"Desired mode too large for current screen size\n");
	     }
	     result |= 0x02;
	  }

	  /* Check if the desired mode is suitable for current CRT1 output device */
	  if(SiS_CheckModeCRT1(pScrn, mode, vbflags, hcm) < 0x14) {
	     if((!cond) && (!quiet)) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		      "Desired mode not suitable for current CRT1 output device\n");
	     }
	     result |= 0x01;
	  }

       }

#ifdef SISDUALHEAD
    }
#endif

    return result;
}

#ifdef XV_SD_DEPRECATED
static int
SISCheckModeIndexForCRT2Type(ScrnInfoPtr pScrn, UShort cond, UShort index, Bool quiet)
{
    SISPtr pSiS = SISPTR(pScrn);
    DisplayModePtr mode = pScrn->modes;
    ULong vbflags;
    int i;

    /* Not only CRT2, but also LCDA */

    /* returns 0 if mode ok,
     *         0x01 if mode not ok for CRT2 device,
     *         0x02 if mode too large for current root window
     *         or combinations thereof
     */

    vbflags = SiSCondToVBFlags(cond, pSiS->VBFlags);

    /* Find mode of given index */
    if(index) {
       for(i = 0; i < index; i++) {
          if(!mode) return 0x03;
          mode = mode->next;
       }
    }

    return(SISCheckModeForCRT2Type(pScrn, mode, vbflags, cond, quiet));
}
#endif

static DisplayModePtr
sisFindModeFromTiming(ScrnInfoPtr pScrn, UShort hdisplay,
			      UShort vdisplay, UShort htotal, UShort vtotal,
			      UShort hsyncstart, UShort hsyncend, UShort vsyncstart,
			      UShort vsyncend, int clock)
{
   DisplayModePtr mode = pScrn->modes, pmode = pScrn->modes;
   Bool found = FALSE;

   do {
       if( (mode->HDisplay == hdisplay) &&
           (mode->VDisplay == vdisplay) &&
	   (mode->HTotal == htotal) &&
	   (mode->VTotal == vtotal) &&
	   (mode->HSyncStart == hsyncstart) &&
	   (mode->VSyncStart == vsyncstart) &&
	   (mode->HSyncEnd == hsyncend) &&
	   (mode->VSyncEnd == vsyncend) &&
	   (mode->Clock == clock) ) {
	  found = TRUE;
	  break;
       }
       mode = mode->next;
    } while((mode) && (mode != pmode));

    if(found) return mode;
    else      return NULL;
}

static int
SISCheckModeTimingForCRT2Type(ScrnInfoPtr pScrn, UShort cond, UShort hdisplay,
			      UShort vdisplay, UShort htotal, UShort vtotal,
			      UShort hsyncstart, UShort hsyncend, UShort vsyncstart,
			      UShort vsyncend, int clock, Bool quiet)
{
    SISPtr pSiS = SISPTR(pScrn);
    DisplayModePtr mode;
    ULong vbflags;

    /* Not only CRT2, but also LCDA */

    /* returns 0 if mode ok,
     *         0x01 if mode not ok for CRT2 device,
     *         0x02 if mode too large for current root window
     *         or combinations thereof
     */

    vbflags = SiSCondToVBFlags(cond, pSiS->VBFlags);

    /* Find mode with given timing */
    mode = sisFindModeFromTiming(pScrn, hdisplay, vdisplay, htotal, vtotal,
				hsyncstart, hsyncend, vsyncstart, vsyncend,
				clock);
    if(!mode) return 0x03;

    return(SISCheckModeForCRT2Type(pScrn, mode, vbflags, cond, quiet));
}

#ifdef SISMERGED
static void
SISGetMergedModeDetails(ScrnInfoPtr pScrn,
	   int hd, int vd, int ht, int vt, int hss, int hse, int vss, int vse, int clk,
	   unsigned int *pos, unsigned int *crt1x, unsigned int *crt1y,
	   unsigned int *crt1clk, unsigned int *crt2x, unsigned int *crt2y,
	   unsigned int *crt2clk)
{
    SISPtr pSiS = SISPTR(pScrn);
    DisplayModePtr mode, tmode;

    if(!pSiS->MergedFB) {
       *pos = SDC_MMODE_POS_ERROR;
       return;
    }

    /* Find mode with given timing */
    mode = sisFindModeFromTiming(pScrn, hd, vd, ht, vt, hss, hse, vss, vse, clk);
    if(!mode) {
       *pos = SDC_MMODE_POS_ERROR;
       return;
    }

    switch(((SiSMergedDisplayModePtr)mode->Private)->CRT2Position) {
       case sisLeftOf:  *pos = SDC_MMODE_POS_LEFTOF;  break;
       case sisRightOf: *pos = SDC_MMODE_POS_RIGHTOF; break;
       case sisAbove:   *pos = SDC_MMODE_POS_ABOVE;   break;
       case sisBelow:   *pos = SDC_MMODE_POS_BELOW;   break;
       default:         *pos = SDC_MMODE_POS_CLONE;
    }

    tmode = ((SiSMergedDisplayModePtr)mode->Private)->CRT1;
    *crt1x = tmode->HDisplay;
    *crt1y = tmode->VDisplay;
    *crt1clk = (unsigned int)SiSCalcVRate(tmode);

    tmode = ((SiSMergedDisplayModePtr)mode->Private)->CRT2;
    *crt2x = tmode->HDisplay;
    *crt2y = tmode->VDisplay;
    *crt2clk = (unsigned int)SiSCalcVRate(tmode);
}
#endif

/***********************************
 *   SiSCtrl extension interface   *
 ***********************************/

static void
sisutil_prepare_string(xSiSCtrlCommandReply *sdcbuf, char *mystring)
{
   int slen = 0;
   sdcbuf->sdc_buffer[0] = 0;
   if(mystring) {
      slen = strlen(mystring);
      if(slen > 31) slen = 31;
      strncpy(&sdcbuf->sdc_buffer[0], mystring, slen);
      sdcbuf->sdc_buffer[slen] = 0;
   }
   sdcbuf->sdc_result[0] = slen;
}

static int
SiSHandleSiSDirectCommand(xSiSCtrlCommandReply *sdcbuf)
{
   ScrnInfoPtr pScrn = xf86Screens[sdcbuf->screen];
   SISPtr pSiS = SISPTR(pScrn);
#ifdef SISDUALHEAD
   SISEntPtr pSiSEnt = pSiS->entityPrivate;
#endif
   SISPortPrivPtr pPriv = NULL;
   int i;
   ULong j;

   if(sdcbuf->sdc_id != SDC_ID) return BadMatch;

   if(pSiS->adaptor) {
      pPriv = GET_PORT_PRIVATE(pScrn);
   }

   j = sdcbuf->sdc_command;
   for(i = 0; i < SDC_NUM_PARM_RESULT; i++) {
      j += sdcbuf->sdc_parm[i];
   }

   if(j != sdcbuf->sdc_chksum) return BadMatch;

   sdcbuf->sdc_result_header = SDC_RESULT_OK;

   switch(sdcbuf->sdc_command) {

   case SDC_CMD_GETVERSION:
      sdcbuf->sdc_result[0] = SDC_VERSION;
      sdcbuf->sdc_result[1] = SDC_MAXCOMMAND;
      break;

   case SDC_CMD_GETHWINFO:
      {
	 UShort romptr = 0;
	 char *biosvers = NULL;
	 int slen = 0;

	 if(pSiS->BIOS) {
	    romptr = pSiS->BIOS[0x16] | (pSiS->BIOS[0x17] << 8);
	    if(romptr < 65530) {
	       biosvers = (char *)&pSiS->BIOS[romptr];
	       slen = strlen(biosvers);
	       if(slen > 10) slen = 10;
	    }
	 }
	 sdcbuf->sdc_result[0]  = pSiS->IsAGPCard ? SDC_BUS_TYPE_AGP : SDC_BUS_TYPE_PCI;
	 if(pSiS->IsPCIExpress) sdcbuf->sdc_result[0] = SDC_BUS_TYPE_PCIE;
	 sdcbuf->sdc_result[1]  = pSiS->PciBus;
	 sdcbuf->sdc_result[2]  = pSiS->PciDevice;
	 sdcbuf->sdc_result[3]  = pSiS->PciFunc;
	 sdcbuf->sdc_result[4]  = pSiS->ROM661New ? 1 : (pSiS->HaveXGIBIOS ? 2 : 0),
	 sdcbuf->sdc_result[5]  = pSiS->ChipFlags;
	 sdcbuf->sdc_result[6]  = pSiS->ChipType;
	 sdcbuf->sdc_result[7]  = pSiS->ChipRev;
	 sdcbuf->sdc_result[8]  = SISDRIVERVERSIONYEAR;
	 sdcbuf->sdc_result[9]  = SISDRIVERVERSIONMONTH;
	 sdcbuf->sdc_result[10] = SISDRIVERVERSIONDAY;
	 sdcbuf->sdc_result[11] = SISDRIVERREVISION;
	 sdcbuf->sdc_result[12] = pScrn->videoRam;
	 sdcbuf->sdc_result[13] = pSiS->UMAsize;
	 sdcbuf->sdc_result[14] = pSiS->LFBsize;
	 if(slen) strncpy(&sdcbuf->sdc_buffer[0], biosvers, slen);
	 sdcbuf->sdc_buffer[slen] = 0;
      }
      break;

   case SDC_CMD_GETALLFLAGS:
      sdcbuf->sdc_result[0] = SIS_VBFlagsVersion;
      sdcbuf->sdc_result[1] = pSiS->VBFlags;
      sdcbuf->sdc_result[2] = pSiS->SiS_SD_Flags;
      sdcbuf->sdc_result[3] = pSiS->SiS_SD2_Flags;
      sdcbuf->sdc_result[4] = pSiS->detectedCRT2Devices;
      sdcbuf->sdc_result[5] = pSiS->VBFlags2;
      sdcbuf->sdc_result[6] = pSiS->SiS_SD3_Flags;
      sdcbuf->sdc_result[7] = pSiS->SiS_SD4_Flags;
      sdcbuf->sdc_result[8] = pSiS->VBFlags3;
      sdcbuf->sdc_result[9] = pSiS->VBFlags4;
      break;

   case SDC_CMD_GETVBFLAGSVERSION:
      sdcbuf->sdc_result[0] = SIS_VBFlagsVersion;
      break;

   case SDC_CMD_GETVBFLAGS:
      sdcbuf->sdc_result[0] = pSiS->VBFlags;
      sdcbuf->sdc_result[1] = pSiS->VBFlags2;
      sdcbuf->sdc_result[2] = pSiS->VBFlags3;
      sdcbuf->sdc_result[3] = pSiS->VBFlags4;
      break;

   case SDC_CMD_CHECKMODEFORCRT2:
      {
	 int clk, hd, hss, hse, ht, vd, vss, vse, vt, result;
	 ULong flags;

	 flags = sdcbuf->sdc_parm[0];
	 clk   = sdcbuf->sdc_parm[1];
	 hd    = sdcbuf->sdc_parm[2];
	 hss   = sdcbuf->sdc_parm[3];
	 hse   = sdcbuf->sdc_parm[4];
	 ht    = sdcbuf->sdc_parm[5];
	 vd    = sdcbuf->sdc_parm[6];
	 vss   = sdcbuf->sdc_parm[7];
	 vse   = sdcbuf->sdc_parm[8];
	 vt    = sdcbuf->sdc_parm[9];

	 result = SISCheckModeTimingForCRT2Type(pScrn, (UShort)(flags & 0xff),
			hd, vd, ht, vt, hss, hse, vss, vse, clk, pSiS->SCLogQuiet) & 0xff;
	 sdcbuf->sdc_result[0] = result;
      }
      break;

   case SDC_CMD_SETVBFLAGS:
#ifdef SISDUALHEAD
      if(!pSiS->DualHeadMode) {
#endif
	 if(pSiS->xv_sisdirectunlocked) {
	    SISSwitchCRT2Type(pScrn, (ULong)sdcbuf->sdc_parm[0], pSiS->SCLogQuiet);
	    if(pPriv) SISUpdateVideoParms(pSiS, pPriv);
	 } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
#ifdef SISDUALHEAD
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
#endif
      break;

   case SDC_CMD_NEWSETVBFLAGS:
#ifdef SISDUALHEAD
      if(!pSiS->DualHeadMode) {
#endif
	 if(pSiS->xv_sisdirectunlocked) {
	    SISSwitchOutputType(pScrn, (ULong)sdcbuf->sdc_parm[0], (ULong)sdcbuf->sdc_parm[1],
	    		(ULong)sdcbuf->sdc_parm[2], pSiS->SCLogQuiet);
	    if(pPriv) SISUpdateVideoParms(pSiS, pPriv);
	 } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
#ifdef SISDUALHEAD
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
#endif
      break;

   case SDC_CMD_GETDETECTEDDEVICES:
      sdcbuf->sdc_result[0] = pSiS->detectedCRT2Devices;
      break;

   case SDC_CMD_REDETECTCRT2DEVICES:
#ifdef SISDUALHEAD
      if(!pSiS->DualHeadMode) {
#endif
	 if(pSiS->xv_sisdirectunlocked) {
	    SISRedetectCRT2Devices(pScrn);
	 } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
#ifdef SISDUALHEAD
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
#endif
      break;

   case SDC_CMD_GETCRT1STATUS:
      sdcbuf->sdc_result[0] = pSiS->CRT1isoff ? 0 : 1;
      break;

   case SDC_CMD_SETCRT1STATUS:
#ifdef SISDUALHEAD
      if(!pSiS->DualHeadMode) {
#endif
	 if(pSiS->xv_sisdirectunlocked) {
	    SISSwitchCRT1Status(pScrn, (ULong)sdcbuf->sdc_parm[0], pSiS->SCLogQuiet);
	    if(pPriv) SISUpdateVideoParms(pSiS, pPriv);
	 } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
#ifdef SISDUALHEAD
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
#endif
      break;

   case SDC_CMD_GETSDFLAGS:
      sdcbuf->sdc_result[0] = pSiS->SiS_SD_Flags;
      break;

   case SDC_CMD_GETSD2FLAGS:
      sdcbuf->sdc_result[0] = pSiS->SiS_SD2_Flags;
      break;

   case SDC_CMD_GETLOCKSTATUS:
      sdcbuf->sdc_result[0] = pSiS->xv_sisdirectunlocked;
      break;

   case SDC_CMD_SETLOCKSTATUS:
      if(pSiS->enablesisctrl) {
	 if(sdcbuf->sdc_parm[0] == SIS_DIRECTKEY) {
	    pSiS->xv_sisdirectunlocked++;
	 } else if(pSiS->xv_sisdirectunlocked) {
	    pSiS->xv_sisdirectunlocked--;
	 }
      } else {
	 pSiS->xv_sisdirectunlocked = 0;
	 sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      }
      break;

   case SDC_CMD_GETTVANTIFLICKER:
      sdcbuf->sdc_result[0] = SiS_GetSISTVantiflicker(pScrn);
      break;

   case SDC_CMD_SETTVANTIFLICKER:
      if(pSiS->xv_sisdirectunlocked) {
	 SiS_SetSISTVantiflicker(pScrn, (int)sdcbuf->sdc_parm[0]);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETTVSATURATION:
      sdcbuf->sdc_result[0] = SiS_GetSISTVsaturation(pScrn);
      break;

   case SDC_CMD_SETTVSATURATION:
      if(pSiS->xv_sisdirectunlocked) {
	 SiS_SetSISTVsaturation(pScrn, (int)sdcbuf->sdc_parm[0]);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETTVEDGEENHANCE:
      sdcbuf->sdc_result[0] = SiS_GetSISTVedgeenhance(pScrn);;
      break;

   case SDC_CMD_SETTVEDGEENHANCE:
      if(pSiS->xv_sisdirectunlocked) {
	 SiS_SetSISTVedgeenhance(pScrn, (int)sdcbuf->sdc_parm[0]);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETTVCFILTER:
      sdcbuf->sdc_result[0] = SiS_GetSISTVcfilter(pScrn);
      break;

   case SDC_CMD_SETTVCFILTER:
      if(pSiS->xv_sisdirectunlocked) {
	 SiS_SetSISTVcfilter(pScrn, sdcbuf->sdc_parm[0] ? 1 : 0);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETTVYFILTER:
      sdcbuf->sdc_result[0] = SiS_GetSISTVyfilter(pScrn);
      break;

   case SDC_CMD_SETTVYFILTER:
      if(pSiS->xv_sisdirectunlocked) {
	 SiS_SetSISTVyfilter(pScrn, sdcbuf->sdc_parm[0]);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETTVCOLORCALIB:
      sdcbuf->sdc_result[0] = SiS_GetSISTVcolcalib(pScrn, TRUE) + 32768;
      sdcbuf->sdc_result[1] = SiS_GetSISTVcolcalib(pScrn, FALSE) + 32768;
      break;

   case SDC_CMD_SETTVCOLORCALIB:
      if(pSiS->xv_sisdirectunlocked) {
         SiS_SetSISTVcolcalib(pScrn, (int)sdcbuf->sdc_parm[0] - 32768, TRUE);
	 SiS_SetSISTVcolcalib(pScrn, (int)sdcbuf->sdc_parm[1] - 32768, FALSE);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETTVCHCONTRAST:
      sdcbuf->sdc_result[0] = SiS_GetCHTVcontrast(pScrn);
      break;

   case SDC_CMD_SETTVCHCONTRAST:
      if(pSiS->xv_sisdirectunlocked) {
	 SiS_SetCHTVcontrast(pScrn, (int)sdcbuf->sdc_parm[0]);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETTVCHTEXTENHANCE:
      sdcbuf->sdc_result[0] = SiS_GetCHTVtextenhance(pScrn);
      break;

   case SDC_CMD_SETTVCHTEXTENHANCE:
      if(pSiS->xv_sisdirectunlocked) {
	 SiS_SetCHTVtextenhance(pScrn, (int)sdcbuf->sdc_parm[0]);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETTVCHCHROMAFLICKERFILTER:
      sdcbuf->sdc_result[0] = SiS_GetCHTVchromaflickerfilter(pScrn);
      break;

   case SDC_CMD_SETTVCHCHROMAFLICKERFILTER:
      if(pSiS->xv_sisdirectunlocked) {
	 SiS_SetCHTVchromaflickerfilter(pScrn, (int)sdcbuf->sdc_parm[0]);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETTVCHLUMAFLICKERFILTER:
      sdcbuf->sdc_result[0] = SiS_GetCHTVlumaflickerfilter(pScrn);
      break;

   case SDC_CMD_SETTVCHLUMAFLICKERFILTER:
      if(pSiS->xv_sisdirectunlocked) {
	 SiS_SetCHTVlumaflickerfilter(pScrn, (int)sdcbuf->sdc_parm[0]);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETTVCHCVBSCOLOR:
      sdcbuf->sdc_result[0] = SiS_GetCHTVcvbscolor(pScrn);
      break;

   case SDC_CMD_SETTVCHCVBSCOLOR:
      if(pSiS->xv_sisdirectunlocked) {
	 SiS_SetCHTVcvbscolor(pScrn, sdcbuf->sdc_parm[0] ? 1 : 0);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETCHTVOVERSCAN:
      {  /* Changing of CRT2 settings not supported in DHM! */
	 int i = 0;
	 if(pSiS->OptTVSOver == 1)         i = 3;
	 else if(pSiS->UseCHOverScan == 1) i = 2;
	 else if(pSiS->UseCHOverScan == 0) i = 1;
	 sdcbuf->sdc_result[0] = i;
      }
      break;

   case SDC_CMD_SETCHTVOVERSCAN:
      if(pSiS->xv_sisdirectunlocked) {
	 pSiS->UseCHOverScan = -1;
	 pSiS->OptTVSOver = FALSE;
	 if(sdcbuf->sdc_parm[0] == 3) {
	    if(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTSOVER) {
	       pSiS->OptTVSOver = TRUE;
	    }
	    pSiS->UseCHOverScan = 1;
	 } else if(sdcbuf->sdc_parm[0] == 2) pSiS->UseCHOverScan = 1;
	 else if(sdcbuf->sdc_parm[0] == 1)   pSiS->UseCHOverScan = 0;
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETGAMMASTATUS:
      {
         int i = 0;
#ifdef SISDUALHEAD
	 if(pSiS->DualHeadMode) {
	    if(pSiSEnt->CRT1gamma) i |= 0x01;
	    if(pSiSEnt->CRT2gamma) i |= 0x02;
	 } else {
#endif
	    if(pSiS->CRT1gamma)    i |= 0x01;
	    if(pSiS->CRT2gamma)    i |= 0x02;
	    if(pSiS->CRT2SepGamma) i |= 0x08;
#ifdef SISDUALHEAD
	 }
#endif
	 if(pSiS->XvGamma) i |= 0x04;
         sdcbuf->sdc_result[0] = i;
      }
      break;

   case SDC_CMD_SETGAMMASTATUS:
      if(pSiS->xv_sisdirectunlocked) {
         int value = sdcbuf->sdc_parm[0];
	 Bool backup = pSiS->XvGamma;
	 Bool backup2 = pSiS->CRT2SepGamma;
	 pSiS->CRT1gamma = (value & 0x01) ? TRUE : FALSE;
	 pSiS->CRT2gamma = (value & 0x02) ? TRUE : FALSE;
#ifdef SISDUALHEAD
	 if(pSiS->DualHeadMode) {
	    pSiSEnt->CRT1gamma = pSiS->CRT1gamma;
	    pSiSEnt->CRT2gamma = pSiS->CRT2gamma;
	 }
#endif
	 if(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTSGRCRT2) {
	    pSiS->CRT2SepGamma = (value & 0x08) ? TRUE : FALSE;
	    if(pSiS->CRT2SepGamma != backup2) {
	       if(pSiS->CRT2SepGamma) {
		  SiS_UpdateGammaCRT2(pScrn);
	       }
#ifdef SISGAMMARAMP
	       else {
		  SISCalculateGammaRamp(pScrn->pScreen, pScrn);
	       }
#endif
	      /* ATTN: When disabling CRT2 sep gamma,
	       * as long as SISGAMMARAMP is not defined,
	       * application needs to reset palette using
	       * vidmodextension! (See SiS_SD_CANSETGAMMA)
	       *
	       * Note: Difference between setting gamma values
	       * for CRT1 and CRT2: For CRT1, driver only
	       * stores the values, but does nothing with them;
	       * App needs to calc gamma ramp itself for CRT1,
	       * and set it using the VidModExtention.
	       * For CRT2, the driver will IMMEDIATELY recalc
	       * the palette.
	       */
	    }
	 } else pSiS->CRT2SepGamma = FALSE;

	 pSiS->XvGamma = (value & 0x04) ? TRUE : FALSE;
	 if(pSiS->VGAEngine == SIS_315_VGA) {
	    if(backup != pSiS->XvGamma) {
	       if(pPriv) SiSUpdateXvGamma(pSiS, pPriv);
	    }
	 }
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETTVXSCALE:
      sdcbuf->sdc_result[0] = SiS_GetTVxscale(pScrn) + 32768;
      break;

   case SDC_CMD_SETTVXSCALE:
      {
         int value = sdcbuf->sdc_parm[0] - 32768;
         if((value < -16) || (value > 16)) {
	    sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
         } else if(pSiS->xv_sisdirectunlocked) {
	    SiS_SetTVxscale(pScrn, value);
         } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      }
      break;

   case SDC_CMD_GETTVYSCALE:
      sdcbuf->sdc_result[0] = SiS_GetTVyscale(pScrn) + 32768;
      break;

   case SDC_CMD_SETTVYSCALE:
      {
         int value = sdcbuf->sdc_parm[0] - 32768;
         if((value < -4) || (value > 3)) {
	    sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
         } else if(pSiS->xv_sisdirectunlocked) {
	    SiS_SetTVyscale(pScrn, value);
         } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      }
      break;

   case SDC_CMD_GETSCREENSIZE:
      sdcbuf->sdc_result[0] = (pScrn->virtualX << 16) | pScrn->virtualY;
      break;

   case SDC_CMD_GETGAMMABRIGHTNESS:  /* xv_BRx, xv_PBx */
      sdcbuf->sdc_result[0] = pSiS->GammaBriR;
      sdcbuf->sdc_result[1] = pSiS->GammaBriG;
      sdcbuf->sdc_result[2] = pSiS->GammaBriB;
      break;

   case SDC_CMD_GETNEWGAMMABRICON:  /* no xv pendant */
      sdcbuf->sdc_result[0] = (CARD32)(((int)(pSiS->NewGammaBriR * 1000.0)) + 1000);
      sdcbuf->sdc_result[1] = (CARD32)(((int)(pSiS->NewGammaBriG * 1000.0)) + 1000);
      sdcbuf->sdc_result[2] = (CARD32)(((int)(pSiS->NewGammaBriB * 1000.0)) + 1000);
      sdcbuf->sdc_result[3] = (CARD32)(((int)(pSiS->NewGammaConR * 1000.0)) + 1000);
      sdcbuf->sdc_result[4] = (CARD32)(((int)(pSiS->NewGammaConG * 1000.0)) + 1000);
      sdcbuf->sdc_result[5] = (CARD32)(((int)(pSiS->NewGammaConB * 1000.0)) + 1000);
      break;

   case SDC_CMD_SETGAMMABRIGHTNESS:  /* xv_BRx, xv_PBx */
      if(sdcbuf->sdc_parm[0] < 100 || sdcbuf->sdc_parm[0] > 10000 ||
	 sdcbuf->sdc_parm[1] < 100 || sdcbuf->sdc_parm[1] > 10000 ||
	 sdcbuf->sdc_parm[2] < 100 || sdcbuf->sdc_parm[2] > 10000) {
	 sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else if(pSiS->xv_sisdirectunlocked) {
	 pSiS->GammaBriR = sdcbuf->sdc_parm[0];
	 pSiS->GammaBriG = sdcbuf->sdc_parm[1];
	 pSiS->GammaBriB = sdcbuf->sdc_parm[2];
	 pSiS->NewGammaBriR = pSiS->NewGammaBriG = pSiS->NewGammaBriB = 0.0;
	 pSiS->NewGammaConR = pSiS->NewGammaConG = pSiS->NewGammaConB = 0.0;
	 pSiS->SiS_SD3_Flags |= SiS_SD3_OLDGAMMAINUSE;
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_SETNEWGAMMABRICON:  /* no xv pendant */
      if(sdcbuf->sdc_parm[0] > 2000 || sdcbuf->sdc_parm[1] > 2000 ||
	 sdcbuf->sdc_parm[2] > 2000 || sdcbuf->sdc_parm[3] > 2000 ||
	 sdcbuf->sdc_parm[4] > 2000 || sdcbuf->sdc_parm[5] > 2000) {
	 sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else if(pSiS->xv_sisdirectunlocked) {
	 pSiS->NewGammaBriR = ((float)((int)sdcbuf->sdc_parm[0] - 1000)) / 1000.0;
	 pSiS->NewGammaBriG = ((float)((int)sdcbuf->sdc_parm[1] - 1000)) / 1000.0;
	 pSiS->NewGammaBriB = ((float)((int)sdcbuf->sdc_parm[2] - 1000)) / 1000.0;
	 pSiS->NewGammaConR = ((float)((int)sdcbuf->sdc_parm[3] - 1000)) / 1000.0;
	 pSiS->NewGammaConG = ((float)((int)sdcbuf->sdc_parm[4] - 1000)) / 1000.0;
	 pSiS->NewGammaConB = ((float)((int)sdcbuf->sdc_parm[5] - 1000)) / 1000.0;
	 pSiS->GammaBriR = pSiS->GammaBriG = pSiS->GammaBriB = 1000;
	 pSiS->SiS_SD3_Flags &= ~SiS_SD3_OLDGAMMAINUSE;
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETGAMMABRIGHTNESS2: /* xv_BRx2, xv_PBx2 */
#ifdef SISDUALHEAD
      if(pSiS->DualHeadMode) {
         sdcbuf->sdc_result[0] = pSiSEnt->GammaBriR;
	 sdcbuf->sdc_result[1] = pSiSEnt->GammaBriG;
	 sdcbuf->sdc_result[2] = pSiSEnt->GammaBriB;
	 break;
      }
#endif
      sdcbuf->sdc_result[0] = pSiS->GammaBriR;
      sdcbuf->sdc_result[1] = pSiS->GammaBriG;
      sdcbuf->sdc_result[2] = pSiS->GammaBriB;
      break;

   case SDC_CMD_GETNEWGAMMABRICON2: /* no xv pendant */
#ifdef SISDUALHEAD
      if(pSiS->DualHeadMode) {
         sdcbuf->sdc_result[0] = (CARD32)(((int)(pSiSEnt->NewGammaBriR * 1000.0)) + 1000);
	 sdcbuf->sdc_result[1] = (CARD32)(((int)(pSiSEnt->NewGammaBriG * 1000.0)) + 1000);
	 sdcbuf->sdc_result[2] = (CARD32)(((int)(pSiSEnt->NewGammaBriB * 1000.0)) + 1000);
	 sdcbuf->sdc_result[3] = (CARD32)(((int)(pSiSEnt->NewGammaConR * 1000.0)) + 1000);
	 sdcbuf->sdc_result[4] = (CARD32)(((int)(pSiSEnt->NewGammaConG * 1000.0)) + 1000);
	 sdcbuf->sdc_result[5] = (CARD32)(((int)(pSiSEnt->NewGammaConB * 1000.0)) + 1000);
	 break;
      }
#endif
      sdcbuf->sdc_result[0] = (CARD32)(((int)(pSiS->NewGammaBriR * 1000.0)) + 1000);
      sdcbuf->sdc_result[1] = (CARD32)(((int)(pSiS->NewGammaBriG * 1000.0)) + 1000);
      sdcbuf->sdc_result[2] = (CARD32)(((int)(pSiS->NewGammaBriB * 1000.0)) + 1000);
      sdcbuf->sdc_result[3] = (CARD32)(((int)(pSiS->NewGammaConR * 1000.0)) + 1000);
      sdcbuf->sdc_result[4] = (CARD32)(((int)(pSiS->NewGammaConG * 1000.0)) + 1000);
      sdcbuf->sdc_result[5] = (CARD32)(((int)(pSiS->NewGammaConB * 1000.0)) + 1000);
      break;

   case SDC_CMD_SETGAMMABRIGHTNESS2: /* xv_BRx2, xv_PBx2 */
      if(sdcbuf->sdc_parm[0] < 100 || sdcbuf->sdc_parm[0] > 10000 ||
	 sdcbuf->sdc_parm[1] < 100 || sdcbuf->sdc_parm[1] > 10000 ||
	 sdcbuf->sdc_parm[2] < 100 || sdcbuf->sdc_parm[2] > 10000) {
	 sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else if(pSiS->xv_sisdirectunlocked) {
         pSiS->SiS_SD3_Flags |= SiS_SD3_OLDGAMMAINUSE;
#ifdef SISDUALHEAD
	 if(pSiS->DualHeadMode) {
	    pSiSEnt->GammaBriR = sdcbuf->sdc_parm[0];
	    pSiSEnt->GammaBriG = sdcbuf->sdc_parm[1];
	    pSiSEnt->GammaBriB = sdcbuf->sdc_parm[2];
	    pSiSEnt->NewGammaBriR = pSiSEnt->NewGammaBriG = pSiSEnt->NewGammaBriB = 0.0;
	    pSiSEnt->NewGammaConR = pSiSEnt->NewGammaConG = pSiSEnt->NewGammaConB = 0.0;
	 }
#endif
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_SETNEWGAMMABRICON2: /* no xv pendant */
      if(sdcbuf->sdc_parm[0] > 2000 || sdcbuf->sdc_parm[1] > 2000 ||
	 sdcbuf->sdc_parm[2] > 2000 || sdcbuf->sdc_parm[3] > 2000 ||
	 sdcbuf->sdc_parm[4] > 2000 || sdcbuf->sdc_parm[5] > 2000) {
	 sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else if(pSiS->xv_sisdirectunlocked) {
         pSiS->SiS_SD3_Flags &= ~SiS_SD3_OLDGAMMAINUSE;
#ifdef SISDUALHEAD
	 if(pSiS->DualHeadMode) {
	    pSiSEnt->NewGammaBriR = ((float)((int)sdcbuf->sdc_parm[0] - 1000)) / 1000.0;
	    pSiSEnt->NewGammaBriG = ((float)((int)sdcbuf->sdc_parm[1] - 1000)) / 1000.0;
	    pSiSEnt->NewGammaBriB = ((float)((int)sdcbuf->sdc_parm[2] - 1000)) / 1000.0;
	    pSiSEnt->NewGammaConR = ((float)((int)sdcbuf->sdc_parm[3] - 1000)) / 1000.0;
	    pSiSEnt->NewGammaConG = ((float)((int)sdcbuf->sdc_parm[4] - 1000)) / 1000.0;
	    pSiSEnt->NewGammaConB = ((float)((int)sdcbuf->sdc_parm[5] - 1000)) / 1000.0;
	    pSiSEnt->GammaBriR = pSiSEnt->GammaBriG = pSiSEnt->GammaBriB = 1000;
	 }
#endif
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETGETGAMMACRT2:
      sdcbuf->sdc_result[0] = (ULong)(pSiS->GammaR2 * 1000);
      sdcbuf->sdc_result[1] = (ULong)(pSiS->GammaG2 * 1000);
      sdcbuf->sdc_result[2] = (ULong)(pSiS->GammaB2 * 1000);
      sdcbuf->sdc_result[3] = pSiS->GammaBriR2;
      sdcbuf->sdc_result[4] = pSiS->GammaBriG2;
      sdcbuf->sdc_result[5] = pSiS->GammaBriB2;
      break;

   case SDC_CMD_GETGETNEWGAMMACRT2:
      sdcbuf->sdc_result[0] = (ULong)(pSiS->GammaR2 * 1000);
      sdcbuf->sdc_result[1] = (ULong)(pSiS->GammaG2 * 1000);
      sdcbuf->sdc_result[2] = (ULong)(pSiS->GammaB2 * 1000);
      sdcbuf->sdc_result[3] = (CARD32)(((int)(pSiS->NewGammaBriR2 * 1000.0)) + 1000);
      sdcbuf->sdc_result[4] = (CARD32)(((int)(pSiS->NewGammaBriG2 * 1000.0)) + 1000);
      sdcbuf->sdc_result[5] = (CARD32)(((int)(pSiS->NewGammaBriB2 * 1000.0)) + 1000);
      sdcbuf->sdc_result[6] = (CARD32)(((int)(pSiS->NewGammaConR2 * 1000.0)) + 1000);
      sdcbuf->sdc_result[7] = (CARD32)(((int)(pSiS->NewGammaConG2 * 1000.0)) + 1000);
      sdcbuf->sdc_result[8] = (CARD32)(((int)(pSiS->NewGammaConB2 * 1000.0)) + 1000);
      break;

   case SDC_CMD_SETGETGAMMACRT2:
      if(sdcbuf->sdc_parm[0] < 100 || sdcbuf->sdc_parm[0] > 10000 ||
	 sdcbuf->sdc_parm[1] < 100 || sdcbuf->sdc_parm[1] > 10000 ||
	 sdcbuf->sdc_parm[2] < 100 || sdcbuf->sdc_parm[2] > 10000 ||
	 sdcbuf->sdc_parm[3] < 100 || sdcbuf->sdc_parm[3] > 10000 ||
	 sdcbuf->sdc_parm[4] < 100 || sdcbuf->sdc_parm[4] > 10000 ||
	 sdcbuf->sdc_parm[5] < 100 || sdcbuf->sdc_parm[5] > 10000) {
	 sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else if(pSiS->xv_sisdirectunlocked) {
         pSiS->GammaR2 = (float)sdcbuf->sdc_parm[0] / (float)1000;
	 pSiS->GammaG2 = (float)sdcbuf->sdc_parm[1] / (float)1000;
	 pSiS->GammaB2 = (float)sdcbuf->sdc_parm[2] / (float)1000;
	 pSiS->GammaBriR2 = sdcbuf->sdc_parm[3];
	 pSiS->GammaBriG2 = sdcbuf->sdc_parm[4];
	 pSiS->GammaBriB2 = sdcbuf->sdc_parm[5];
	 pSiS->NewGammaBriR2 = pSiS->NewGammaBriG2 = pSiS->NewGammaBriB2 = 0.0;
	 pSiS->NewGammaConR2 = pSiS->NewGammaConG2 = pSiS->NewGammaConB2 = 0.0;
	 pSiS->SiS_SD3_Flags |= SiS_SD3_OLDGAMMAINUSE;
	 SiS_UpdateGammaCRT2(pScrn);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_SETGETNEWGAMMACRT2:
      if(sdcbuf->sdc_parm[0] < 100 || sdcbuf->sdc_parm[0] > 10000 ||
	 sdcbuf->sdc_parm[1] < 100 || sdcbuf->sdc_parm[1] > 10000 ||
	 sdcbuf->sdc_parm[2] < 100 || sdcbuf->sdc_parm[2] > 10000 ||
	 sdcbuf->sdc_parm[3] > 2000 || sdcbuf->sdc_parm[4] > 2000 ||
	 sdcbuf->sdc_parm[5] > 2000 || sdcbuf->sdc_parm[6] > 2000 ||
	 sdcbuf->sdc_parm[7] > 2000 || sdcbuf->sdc_parm[8] > 2000) {
	 sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else if(pSiS->xv_sisdirectunlocked) {
         pSiS->GammaR2 = (float)sdcbuf->sdc_parm[0] / (float)1000;
	 pSiS->GammaG2 = (float)sdcbuf->sdc_parm[1] / (float)1000;
	 pSiS->GammaB2 = (float)sdcbuf->sdc_parm[2] / (float)1000;
	 pSiS->NewGammaBriR2 = ((float)((int)sdcbuf->sdc_parm[3] - 1000)) / 1000.0;
	 pSiS->NewGammaBriG2 = ((float)((int)sdcbuf->sdc_parm[4] - 1000)) / 1000.0;
	 pSiS->NewGammaBriB2 = ((float)((int)sdcbuf->sdc_parm[5] - 1000)) / 1000.0;
	 pSiS->NewGammaConR2 = ((float)((int)sdcbuf->sdc_parm[6] - 1000)) / 1000.0;
	 pSiS->NewGammaConG2 = ((float)((int)sdcbuf->sdc_parm[7] - 1000)) / 1000.0;
	 pSiS->NewGammaConB2 = ((float)((int)sdcbuf->sdc_parm[8] - 1000)) / 1000.0;
	 pSiS->GammaBriR2 = pSiS->GammaBriG2 = pSiS->GammaBriB2 = 1000;
	 pSiS->SiS_SD3_Flags &= ~SiS_SD3_OLDGAMMAINUSE;
	 SiS_UpdateGammaCRT2(pScrn);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETHWCURSORSTATUS:
      sdcbuf->sdc_result[0] = pSiS->HideHWCursor ? 1 : 0;
      break;

   case SDC_CMD_SETHWCURSORSTATUS:
      if(pSiS->xv_sisdirectunlocked) {
	 Bool VisibleBackup = pSiS->HWCursorIsVisible;
	 pSiS->HideHWCursor = sdcbuf->sdc_parm[0] ? TRUE : FALSE;
	 if(pSiS->CursorInfoPtr) {
	    if(VisibleBackup) {
	       if(sdcbuf->sdc_parm[0]) {
	 	  (pSiS->CursorInfoPtr->HideCursor)(pScrn);
	       } else {
		  (pSiS->CursorInfoPtr->ShowCursor)(pScrn);
	       }
	    }
	    pSiS->HWCursorIsVisible = VisibleBackup;
	 }
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETPANELMODE:
      {
	 int i = 0;
	 if(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTSCALE) {
	    switch(pSiS->SiS_Pr->UsePanelScaler) {
	    case -1: i |= 0x01; break;
	    case 1:  i |= 0x02; break;
	    }
	    if(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTCENTER) {
	       switch(pSiS->SiS_Pr->CenterScreen) {
	       case -1: i |= 0x04; break;
	       case 1:  i |= 0x08; break;
	       }
	    }
	 }
	 sdcbuf->sdc_result[0] = i;
      }
      break;

   case SDC_CMD_SETPANELMODE:
      if(pSiS->xv_sisdirectunlocked) {
         int value = sdcbuf->sdc_parm[0];
         if(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTSCALE) {
	    if(value & 0x01)      pSiS->SiS_Pr->UsePanelScaler = -1;
	    else if(value & 0x02) pSiS->SiS_Pr->UsePanelScaler = 1;
	    else		  pSiS->SiS_Pr->UsePanelScaler = 0;
	    if(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTCENTER) {
	       if(value & 0x04)      pSiS->SiS_Pr->CenterScreen = -1;
	       else if(value & 0x08) pSiS->SiS_Pr->CenterScreen = 1;
	       else		     pSiS->SiS_Pr->CenterScreen = 0;
	    }
         }
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETMERGEDMODEDETAILS:
#ifdef SISMERGED
      if(pSiS->MergedFB) {
         int clk, hd, hss, hse, ht, vd, vss, vse, vt;
	 unsigned int pos, crt1x, crt1y, crt1clk, crt2x, crt2y, crt2clk;

	 clk   = sdcbuf->sdc_parm[0];
	 hd    = sdcbuf->sdc_parm[1];
	 hss   = sdcbuf->sdc_parm[2];
	 hse   = sdcbuf->sdc_parm[3];
	 ht    = sdcbuf->sdc_parm[4];
	 vd    = sdcbuf->sdc_parm[5];
	 vss   = sdcbuf->sdc_parm[6];
	 vse   = sdcbuf->sdc_parm[7];
	 vt    = sdcbuf->sdc_parm[8];

	 SISGetMergedModeDetails(pScrn,
		hd, vd, ht, vt, hss, hse, vss, vse, clk,
		&pos, &crt1x, &crt1y, &crt1clk, &crt2x, &crt2y, &crt2clk);

	 sdcbuf->sdc_result[0] = pos;
	 sdcbuf->sdc_result[1] = crt1x;
	 sdcbuf->sdc_result[2] = crt1y;
	 sdcbuf->sdc_result[3] = crt1clk;
	 sdcbuf->sdc_result[4] = crt2x;
	 sdcbuf->sdc_result[5] = crt2y;
	 sdcbuf->sdc_result[6] = crt2clk;

	 switch(pSiS->CRT2Position) {
	 case sisLeftOf:  sdcbuf->sdc_result[7] = SDC_MMODE_POS_LEFTOF; break;
	 case sisRightOf: sdcbuf->sdc_result[7] = SDC_MMODE_POS_RIGHTOF; break;
	 case sisAbove:   sdcbuf->sdc_result[7] = SDC_MMODE_POS_ABOVE; break;
	 case sisBelow:   sdcbuf->sdc_result[7] = SDC_MMODE_POS_BELOW; break;
	 default:         sdcbuf->sdc_result[7] = SDC_MMODE_POS_CLONE;
	 }

      } else
#endif
         sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETDEVICENAME:		/* In DualHead mode, this returns CRT1 data */
      {
         SISPtr	mypSiS = pSiS;
	 sdcbuf->sdc_result[0] = 0;
#ifdef SISDUALHEAD
	 if(pSiS->DualHeadMode) {
	    if(pSiSEnt->pScrn_2) mypSiS = SISPTR(pSiSEnt->pScrn_2);
	 }
#endif
	 sisutil_prepare_string(sdcbuf, mypSiS->devsectname);
      }
      break;

   case SDC_CMD_GETMONITORNAME:		/* In DualHead mode, this returns CRT1 data */
      {
         ScrnInfoPtr mypScrn = pScrn;
         sdcbuf->sdc_result[0] = 0;
#ifdef SISDUALHEAD
	 if(pSiS->DualHeadMode) {
	    if(pSiSEnt->pScrn_2) mypScrn = pSiSEnt->pScrn_2;
	 }
#endif
         if(mypScrn->monitor) {
            sisutil_prepare_string(sdcbuf, mypScrn->monitor->id);
         }
      }
      break;

   case SDC_CMD_GETDEVICENAME2:		/* In DualHead mode, this returns CRT2 data */
      sdcbuf->sdc_result[0] = 0;
#ifdef SISDUALHEAD
      if(pSiS->DualHeadMode) {
         if(pSiSEnt->pScrn_1) {
	    sisutil_prepare_string(sdcbuf, SISPTR(pSiSEnt->pScrn_1)->devsectname);
	 }
      } else
#endif
	 sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETMONITORNAME2:	/* In DualHead mode, this returns CRT2 data */
      sdcbuf->sdc_result[0] = 0;
#ifdef SISDUALHEAD
      if(pSiS->DualHeadMode) {
         if(pSiSEnt->pScrn_1) {
	    if(pSiSEnt->pScrn_1->monitor) {
               sisutil_prepare_string(sdcbuf, pSiSEnt->pScrn_1->monitor->id);
            }
	 }
      } else
#endif
	 sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVBRIGHTNESS:
      if(pPriv) {
         int val = sdcbuf->sdc_parm[0] - 32768;
         if(val >= -128 && val <= 127) pPriv->brightness = val;
	 else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETXVBRIGHTNESS:
      if(pPriv) {
         sdcbuf->sdc_result[0] = pPriv->brightness + 32768;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVCONTRAST:
      if(pPriv) {
         int val = sdcbuf->sdc_parm[0] - 32768;
         if(val >= 0 && val <= 7) pPriv->contrast = val;
	 else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETXVCONTRAST:
      if(pPriv) {
         sdcbuf->sdc_result[0] = pPriv->contrast + 32768;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVHUE:
      if((pPriv) && (pSiS->SiS_SD2_Flags & SiS_SD2_SUPPORTXVHUESAT)) {
         int val = sdcbuf->sdc_parm[0] - 32768;
         if(val >= -8 && val <= 7) pPriv->hue = val;
	 else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETXVHUE:
      if((pPriv) && (pSiS->SiS_SD2_Flags & SiS_SD2_SUPPORTXVHUESAT)) {
         sdcbuf->sdc_result[0] = pPriv->hue + 32768;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVSATURATION:
      if((pPriv) && (pSiS->SiS_SD2_Flags & SiS_SD2_SUPPORTXVHUESAT)) {
         int val = sdcbuf->sdc_parm[0] - 32768;
         if(val >= -7 && val <= 7) pPriv->saturation = val;
	 else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETXVSATURATION:
      if((pPriv) && (pSiS->SiS_SD2_Flags & SiS_SD2_SUPPORTXVHUESAT)) {
         sdcbuf->sdc_result[0] = pPriv->saturation + 32768;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVGAMMA:
      if((pPriv) && (pSiS->SiS_SD_Flags & SiS_SD_SUPPORTXVGAMMA1)) {
         if(sdcbuf->sdc_parm[0] < 100 || sdcbuf->sdc_parm[0] > 10000 ||
	    sdcbuf->sdc_parm[1] < 100 || sdcbuf->sdc_parm[1] > 10000 ||
	    sdcbuf->sdc_parm[2] < 100 || sdcbuf->sdc_parm[2] > 10000) {
	    sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
	 } else {
	    pSiS->XvGammaRed = sdcbuf->sdc_parm[0];
	    pSiS->XvGammaGreen = sdcbuf->sdc_parm[1];
	    pSiS->XvGammaBlue = sdcbuf->sdc_parm[2];
	    SiSUpdateXvGamma(pSiS, pPriv);
	 }
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETXVGAMMA:
      if((pPriv) && (pSiS->SiS_SD_Flags & SiS_SD_SUPPORTXVGAMMA1)) {
         sdcbuf->sdc_result[0] = pSiS->XvGammaRed;
	 sdcbuf->sdc_result[1] = pSiS->XvGammaGreen;
	 sdcbuf->sdc_result[2] = pSiS->XvGammaBlue;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVCOLORKEY:
      if(pPriv) {
         pPriv->colorKey = pSiS->colorKey = sdcbuf->sdc_parm[0];
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETXVCOLORKEY:
      if(pPriv) {
         sdcbuf->sdc_result[0] = pPriv->colorKey;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVAUTOPAINTCOLORKEY:
      if(pPriv) {
         pPriv->autopaintColorKey = sdcbuf->sdc_parm[0] ? 1 : 0;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETXVAUTOPAINTCOLORKEY:
      if(pPriv) {
         sdcbuf->sdc_result[0] = pPriv->autopaintColorKey ? 1 : 0;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVDEFAULTS:
      if(pPriv) {
         SISSetPortDefaults(pScrn, pPriv);
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVDISABLEGFX:
      if(pPriv) {
         pPriv->disablegfx = sdcbuf->sdc_parm[0] ? 1 : 0;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETXVDISABLEGFX:
      if(pPriv) {
         sdcbuf->sdc_result[0] = pPriv->disablegfx ? 1 : 0;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVDISABLEGFXLR:
      if(pPriv) {
         pPriv->disablegfxlr = sdcbuf->sdc_parm[0] ? 1 : 0;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETXVDISABLEGFXLR:
      if(pPriv) {
         sdcbuf->sdc_result[0] = pPriv->disablegfxlr ? 1 : 0;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVSWITCHCRT:
      if((pPriv) && (pSiS->VGAEngine == SIS_315_VGA)) {
         if(pPriv->AllowSwitchCRT) {
	    pPriv->crtnum = sdcbuf->sdc_parm[0] ? 1 : 0;
#ifdef SISDUALHEAD
            if(pPriv->dualHeadMode) pSiSEnt->curxvcrtnum = pPriv->crtnum;
#endif
	 }
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETXVSWITCHCRT:
      if((pPriv) && (pSiS->VGAEngine == SIS_315_VGA)) {
#ifdef SISDUALHEAD
         if(pPriv->dualHeadMode)
            sdcbuf->sdc_result[0] = pSiSEnt->curxvcrtnum;
         else
#endif
            sdcbuf->sdc_result[0] = pPriv->crtnum;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;


   case SDC_CMD_SETTVXPOS:
      {
	 int value = sdcbuf->sdc_parm[0] - 32768;
	 if((value < -32) || (value > 32)) {
	    sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
	 } else if(pSiS->xv_sisdirectunlocked) {
	    pPriv->tvxpos = value;
	    SiS_SetTVxposoffset(pScrn, pPriv->tvxpos);
	    pPriv->updatetvxpos = FALSE;
         } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      }
      break;

   case SDC_CMD_GETTVXPOS:
      sdcbuf->sdc_result[0] = SiS_GetTVxposoffset(pScrn) + 32768;
      break;

   case SDC_CMD_SETTVYPOS:
      {
	 int value = sdcbuf->sdc_parm[0] - 32768;
	 if((value < -32) || (value > 32)) {
	    sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
	 } else if(pSiS->xv_sisdirectunlocked) {
	    pPriv->tvypos = value;
	    SiS_SetTVyposoffset(pScrn, pPriv->tvypos);
	    pPriv->updatetvypos = FALSE;
	 } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      }
      break;

   case SDC_CMD_GETTVYPOS:
      sdcbuf->sdc_result[0] = SiS_GetTVyposoffset(pScrn) + 32768;
      break;

   case SDC_CMD_SETXVDEINT:
   case SDC_CMD_GETXVDEINT:
      sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETMONGAMMACRT1:
      sdcbuf->sdc_result[0] = pSiS->CRT1MonGamma;
      break;

   case SDC_CMD_GETMONGAMMACRT2:
      sdcbuf->sdc_result[0] = pSiS->CRT2MonGamma;
      break;

   case SDC_CMD_LOGQUIET:
      pSiS->SCLogQuiet = sdcbuf->sdc_parm[0] ? TRUE : FALSE;
      break;

   case SDC_CMD_GETCRT1SATGAIN:
      sdcbuf->sdc_result[0] = SiS_GetSISCRT1SaturationGain(pScrn);
      break;

   case SDC_CMD_SETCRT1SATGAIN:
      if(pSiS->xv_sisdirectunlocked) {
	 SiS_SetSISCRT1SaturationGain(pScrn, (int)sdcbuf->sdc_parm[0]);
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   default:
      sdcbuf->sdc_result_header = SDC_RESULT_UNDEFCMD;
   }

   return Success;
}

/* Proc */

static int
SiSProcSiSCtrlQueryVersion(ClientPtr client)
{
    xSiSCtrlQueryVersionReply	  rep;
    register int		  n;

    REQUEST_SIZE_MATCH(xSiSCtrlQueryVersionReq);
    rep.type = X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    rep.majorVersion = SISCTRL_MAJOR_VERSION;
    rep.minorVersion = SISCTRL_MINOR_VERSION;
    if(client->swapped) {
        _swaps(&rep.sequenceNumber, n);
        _swapl(&rep.length, n);
        _swaps(&rep.majorVersion, n);
        _swaps(&rep.minorVersion, n);
    }
    WriteToClient(client, sizeof(xSiSCtrlQueryVersionReply), (char *)&rep);
    return (client->noClientException);
}

static int
SiSProcSiSCtrlCommand(ClientPtr client)
{
    REQUEST(xSiSCtrlCommandReq);
    xSiSCtrlCommandReply rep;
    ExtensionEntry 	 *myext;
    xSiSCtrlScreenTable  *myctrl;
    register int	 n;
    int 		 i, ret;

    REQUEST_SIZE_MATCH(xSiSCtrlCommandReq);

    memcpy(&rep, stuff, sizeof(xSiSCtrlCommandReply));

    /* Get pointer to ExtensionEntry */
    if(!(myext = CheckExtension(SISCTRL_PROTOCOL_NAME))) return BadMatch;

    /* Get pointer to our private */
    if(!(myctrl = (xSiSCtrlScreenTable *)myext->extPrivate)) return BadMatch;

    /* Check if screen index is within out limits */
    if(rep.screen > myctrl->maxscreens) return BadMatch;

    /* Check if this screen has added itself */
    if(!(myctrl->HandleSiSDirectCommand[rep.screen])) return BadMatch;

    /* Finally, execute the command */
    if((ret = (myctrl->HandleSiSDirectCommand[rep.screen])(&rep)) != Success)
       return ret;

    rep.type = X_Reply;
    rep.length = (sizeof(xSiSCtrlCommandReply) - sizeof(xGenericReply)) >> 2;
    rep.sequenceNumber = client->sequence;

    if(client->swapped) {
       _swaps(&rep.sequenceNumber, n);
       _swapl(&rep.length, n);
       _swapl(&rep.screen, n);
       _swapl(&rep.sdc_id, n);
       _swapl(&rep.sdc_command, n);
       _swapl(&rep.sdc_result_header, n);
       for(i = 0; i < SDC_NUM_PARM_RESULT; i++) {
	  _swapl(&rep.sdc_parm[i], n);
	  _swapl(&rep.sdc_result[i], n);
       }
    }
    WriteToClient(client, sizeof(xSiSCtrlCommandReply), (char *)&rep);
    return client->noClientException;
}

static int
SiSProcSiSCtrlDispatch(ClientPtr client)
{
    REQUEST(xReq);
    switch(stuff->data) {
	case X_SiSCtrlQueryVersion:
	     return SiSProcSiSCtrlQueryVersion(client);
	case X_SiSCtrlCommand:
	     return SiSProcSiSCtrlCommand(client);
    }
    return BadRequest;
}

/* SProc */

static int
SiSSProcSiSCtrlQueryVersion(ClientPtr client)
{
    REQUEST(xSiSCtrlQueryVersionReq);
    register int n;
    _swaps(&stuff->length, n);
    REQUEST_SIZE_MATCH(xSiSCtrlQueryVersionReq);
    return SiSProcSiSCtrlQueryVersion(client);
}

static int
SiSSProcSiSCtrlCommand(ClientPtr client)
{
    REQUEST(xSiSCtrlCommandReq);
    register int n;
    int i;
    _swaps(&stuff->length, n);
    _swapl(&stuff->screen, n);
    _swapl(&stuff->sdc_id, n);
    _swapl(&stuff->sdc_command, n);
    _swapl(&stuff->sdc_result_header, n);
    for(i = 0; i < SDC_NUM_PARM_RESULT; i++) {
       _swapl(&stuff->sdc_parm[i], n);
       _swapl(&stuff->sdc_result[i], n);
    }
    REQUEST_SIZE_MATCH(xSiSCtrlCommandReq);
    return SiSProcSiSCtrlCommand(client);
}

static int
SiSSProcSiSCtrlDispatch(ClientPtr client)
{
    REQUEST(xReq);
    switch(stuff->data) {
       case X_SiSCtrlQueryVersion:
	  return SiSSProcSiSCtrlQueryVersion(client);
       case X_SiSCtrlCommand:
	  return SiSSProcSiSCtrlCommand(client);
    }
    return BadRequest;
}

static void
SiSCtrlResetProc(ExtensionEntry* extEntry)
{
    /* Called by CloseDownExtensions() */
    /* (CloseDownExtensions() is called BEFORE CloseScreen,
     * which is why we must not use pSiS->SiSCtrlExtEntry
     * in SiSCtrlExtUnregister())
     */
    if(extEntry->extPrivate) {
       free(extEntry->extPrivate);
       extEntry->extPrivate = NULL;
    }
}

void
SiSCtrlExtInit(ScrnInfoPtr pScrn)
{
   SISPtr pSiS = SISPTR(pScrn);
   ExtensionEntry  *myext;
   xSiSCtrlScreenTable *myctrl;
   unsigned int version, revision;

   if((pSiS->VGAEngine != SIS_300_VGA) &&
      (pSiS->VGAEngine != SIS_315_VGA))
      return;

   pSiS->SCLogQuiet = FALSE;

   if(!(myext = CheckExtension(SISCTRL_PROTOCOL_NAME))) {

      if(!(myctrl = calloc(sizeof(xSiSCtrlScreenTable), 1)))
         return;

      if(!(myext = AddExtension(SISCTRL_PROTOCOL_NAME, 0, 0,
				SiSProcSiSCtrlDispatch,
				SiSSProcSiSCtrlDispatch,
				SiSCtrlResetProc,
				StandardMinorOpcode))) {
         xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	 		"Failed to add SISCTRL extension\n");
	 free(myctrl);
	 return;
      }

      myext->extPrivate = (pointer)myctrl;

      myctrl->maxscreens = SISCTRL_MAX_SCREENS;
      myctrl->version_major = version = SISCTRL_MAJOR_VERSION;
      myctrl->version_minor = revision = SISCTRL_MINOR_VERSION;

      xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"Initialized SISCTRL extension version %d.%d\n",
		version, revision);

   } else {

      if(!(myctrl = (xSiSCtrlScreenTable *)myext->extPrivate)) {
         xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"Internal error: Found SISCTRL extension with NULL-private!\n");
	 return;
      }

      version = myctrl->version_major;
      revision = myctrl->version_minor;
   }

   if(pScrn->scrnIndex < myctrl->maxscreens) {

      myctrl->HandleSiSDirectCommand[pScrn->scrnIndex] = SiSHandleSiSDirectCommand;

      pSiS->SiSCtrlExtEntry = myext;

      xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"Registered screen %d with SISCTRL extension version %d.%d\n",
		pScrn->scrnIndex, version, revision);
   } else {

      xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"Screen number (%d) too high for SISCTRL extension %d.%d\n",
		pScrn->scrnIndex, version, revision);

   }

}

void
SiSCtrlExtUnregister(SISPtr pSiS, int index)
{
   ExtensionEntry  *myext;
   xSiSCtrlScreenTable *myctrl;

   if(!pSiS->SiSCtrlExtEntry) return;

   /* Since CloseDownExtensions() is called before
    * our CloseScreen(), we must not use the saved
    * ptr here, but instead check for the extension.
    */

   if((myext = CheckExtension(SISCTRL_PROTOCOL_NAME))) {
      if((myctrl = (xSiSCtrlScreenTable *)myext->extPrivate)) {
         myctrl->HandleSiSDirectCommand[index] = NULL;
      }
   }
}

/***********************************
 *     Xv attribute interface      *
 ***********************************/

#ifdef XV_SD_DEPRECATED

int
SISSetPortUtilAttribute(ScrnInfoPtr pScrn, Atom attribute,
			INT32 value, SISPortPrivPtr pPriv)
{
  SISPtr pSiS = SISPTR(pScrn);
#ifdef SISDUALHEAD
  SISEntPtr pSiSEnt = pSiS->entityPrivate;;
#endif

  if(attribute == pSiS->xv_USD) {
     if(pSiS->enablesisctrl) {
	 if(value == SIS_DIRECTKEY) {
	   pSiS->xv_sisdirectunlocked++;
	} else if(pSiS->xv_sisdirectunlocked) {
	   pSiS->xv_sisdirectunlocked--;
	}
     } else {
	pSiS->xv_sisdirectunlocked = 0;
     }
  } else if(attribute == pSiS->xv_SVF) {
#ifdef SISDUALHEAD
     if(!pPriv->dualHeadMode)
#endif
	if(pSiS->xv_sisdirectunlocked) {
	   SISSwitchCRT2Type(pScrn, (ULong)value, FALSE);
	   SISUpdateVideoParms(pSiS, pPriv);
	}
  } else if(attribute == pSiS->xv_CT1) {
#ifdef SISDUALHEAD
     if(!pPriv->dualHeadMode)
#endif
	if(pSiS->xv_sisdirectunlocked) {
	   SISSwitchCRT1Status(pScrn, (ULong)value, FALSE);
	   SISUpdateVideoParms(pSiS, pPriv);
        }
  } else if(attribute == pSiS->xv_RDT) {
#ifdef SISDUALHEAD
     if(!pPriv->dualHeadMode)
#endif
	if(pSiS->xv_sisdirectunlocked) {
	   SISRedetectCRT2Devices(pScrn);
	}
  } else if(attribute == pSiS->xv_TAF) {
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetSISTVantiflicker(pScrn, (int)value);
     }
  } else if(attribute == pSiS->xv_TSA) {
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetSISTVsaturation(pScrn, (int)value);
     }
  } else if(attribute == pSiS->xv_TEE) {
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetSISTVedgeenhance(pScrn, (int)value);
     }
  } else if(attribute == pSiS->xv_CFI) {
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetSISTVcfilter(pScrn, value ? 1 : 0);
     }
  } else if(attribute == pSiS->xv_YFI) {
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetSISTVyfilter(pScrn, value);
     }
  } else if(attribute == pSiS->xv_COC) {
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetSISTVcolcalib(pScrn, (int)value, TRUE);
     }
  } else if(attribute == pSiS->xv_COF) {
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetSISTVcolcalib(pScrn, (int)value, FALSE);
     }
  } else if(attribute == pSiS->xv_TCO) {
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetCHTVcontrast(pScrn, (int)value);
     }
  } else if(attribute == pSiS->xv_TTE) {
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetCHTVtextenhance(pScrn, (int)value);
     }
  } else if(attribute == pSiS->xv_TCF) {
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetCHTVchromaflickerfilter(pScrn, (int)value);
     }
  } else if(attribute == pSiS->xv_TLF) {
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetCHTVlumaflickerfilter(pScrn, (int)value);
     }
  } else if(attribute == pSiS->xv_TCC) {
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetCHTVcvbscolor(pScrn, value ? 1 : 0);
     }
  } else if(attribute == pSiS->xv_OVR) {
     if(pSiS->xv_sisdirectunlocked) {
	pSiS->UseCHOverScan = -1;
	pSiS->OptTVSOver = FALSE;
	if(value == 3) {
	   if(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTSOVER) {
	      pSiS->OptTVSOver = TRUE;
	   }
	   pSiS->UseCHOverScan = 1;
	} else if(value == 2) pSiS->UseCHOverScan = 1;
	else if(value == 1)   pSiS->UseCHOverScan = 0;
     }
  } else if(attribute == pSiS->xv_CMD) {
     if(pSiS->xv_sisdirectunlocked) {
	int result = 0;
	pSiS->xv_sd_result = (value & 0xffffff00);
	result = SISCheckModeIndexForCRT2Type(pScrn, (UShort)(value & 0xff),
					      (UShort)((value >> 8) & 0xff),
					      FALSE);
	pSiS->xv_sd_result |= (result & 0xff);
     }
  } else if(attribute == pSiS->xv_SGA) {
     if(pSiS->xv_sisdirectunlocked) {
	Bool backup = pSiS->XvGamma;
	Bool backup2 = pSiS->CRT2SepGamma;
	pSiS->CRT1gamma = (value & 0x01) ? TRUE : FALSE;
	pSiS->CRT2gamma = (value & 0x02) ? TRUE : FALSE;
#ifdef SISDUALHEAD
	if(pPriv->dualHeadMode) {
	   pSiSEnt->CRT1gamma = pSiS->CRT1gamma;
	   pSiSEnt->CRT2gamma = pSiS->CRT2gamma;
	}
#endif
	if(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTSGRCRT2) {
	   pSiS->CRT2SepGamma = (value & 0x08) ? TRUE : FALSE;
	   if(pSiS->CRT2SepGamma != backup2) {
	      if(pSiS->CRT2SepGamma) {
		 SiS_UpdateGammaCRT2(pScrn);
	      }
#ifdef SISGAMMARAMP
	      else {
		 SISCalculateGammaRamp(pScrn->pScreen, pScrn);
	      }
#endif
	      /* ATTN: When disabling CRT2 sep gamma,
	       * as long as SISGAMMARAMP is not defined,
	       * application needs to reset palette using
	       * vidmodextension! (See SiS_SD_CANSETGAMMA)
	       *
	       * Note: Difference between setting gamma values
	       * for CRT1 and CRT2: For CRT1, driver only
	       * stores the values, but does nothing with them.
	       * For CRT2, the driver will IMMEDIATELY recalc
	       * the palette. App needs to calc gamma ramp
	       * itself for CRT1, and set it using the
	       * VidModExtention.
	       */
	   }
	} else pSiS->CRT2SepGamma = FALSE;
	pSiS->XvGamma = (value & 0x04) ? TRUE : FALSE;
	if(pSiS->VGAEngine == SIS_315_VGA) {
	   if(backup != pSiS->XvGamma) {
	      SiSUpdateXvGamma(pSiS, pPriv);
	   }
	}
     }
  } else if(attribute == pSiS->xv_TXS) {
     if((value < -16) || (value > 16)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetTVxscale(pScrn, value);
     }
  } else if(attribute == pSiS->xv_TYS) {
     if((value < -4) || (value > 3)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
	SiS_SetTVyscale(pScrn, value);
     }
  } else if(attribute == pSiS->xv_BRR) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
	pSiS->GammaBriR = value;
     }
  } else if(attribute == pSiS->xv_BRG) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
	pSiS->GammaBriG = value;
     }
  } else if(attribute == pSiS->xv_BRB) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
	pSiS->GammaBriB = value;
     }
  } else if(attribute == pSiS->xv_PBR) {
     /* Nop */
  } else if(attribute == pSiS->xv_PBG) {
     /* Nop */
  } else if(attribute == pSiS->xv_PBB) {
     /* Nop */
  } else if(attribute == pSiS->xv_BRR2) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
#ifdef SISDUALHEAD
	if(pPriv->dualHeadMode) pSiSEnt->GammaBriR = value;
#endif
     }
  } else if(attribute == pSiS->xv_BRG2) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
#ifdef SISDUALHEAD
	if(pPriv->dualHeadMode) pSiSEnt->GammaBriG = value;
#endif
     }
  } else if(attribute == pSiS->xv_BRB2) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
#ifdef SISDUALHEAD
	if(pPriv->dualHeadMode) pSiSEnt->GammaBriB = value;
#endif
     }
  } else if(attribute == pSiS->xv_PBR2) {
     /* Nop */
  } else if(attribute == pSiS->xv_PBG2) {
     /* Nop */
  } else if(attribute == pSiS->xv_PBB2) {
     /* Nop */
  } else if(attribute == pSiS->xv_GARC2) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
	pSiS->GammaR2 = (float)value / (float)1000;
	SiS_UpdateGammaCRT2(pScrn);
     }
  } else if(attribute == pSiS->xv_GAGC2) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
	pSiS->GammaG2 = (float)value / (float)1000;
	SiS_UpdateGammaCRT2(pScrn);
     }
  } else if(attribute == pSiS->xv_GABC2) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
	pSiS->GammaB2 = (float)value / (float)1000;
	SiS_UpdateGammaCRT2(pScrn);
     }
  } else if(attribute == pSiS->xv_BRRC2) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
	pSiS->GammaBriR2 = value;
	SiS_UpdateGammaCRT2(pScrn);
     }
  } else if(attribute == pSiS->xv_BRGC2) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
	pSiS->GammaBriG2 = value;
	SiS_UpdateGammaCRT2(pScrn);
     }
  } else if(attribute == pSiS->xv_BRBC2) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiS->xv_sisdirectunlocked) {
	pSiS->GammaBriB2 = value;
	SiS_UpdateGammaCRT2(pScrn);
     }
  } else if(attribute == pSiS->xv_PBRC2) {
     /* Nop */
  } else if(attribute == pSiS->xv_PBGC2) {
     /* Nop */
  } else if(attribute == pSiS->xv_PBBC2) {
     /* Nop */
  } else if(attribute == pSiS->xv_SHC) {
     if(pSiS->xv_sisdirectunlocked) {
	Bool VisibleBackup = pSiS->HWCursorIsVisible;
	pSiS->HideHWCursor = value ? TRUE : FALSE;
	if(pSiS->CursorInfoPtr) {
	   if(VisibleBackup) {
	      if(value) {
		 (pSiS->CursorInfoPtr->HideCursor)(pScrn);
	      } else {
		 (pSiS->CursorInfoPtr->ShowCursor)(pScrn);
	      }
	   }
	   pSiS->HWCursorIsVisible = VisibleBackup;
	}
     }
  } else if(attribute == pSiS->xv_PMD) {
     if(pSiS->xv_sisdirectunlocked) {
        if(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTSCALE) {
	   if(value & 0x01)      pSiS->SiS_Pr->UsePanelScaler = -1;
	   else if(value & 0x02) pSiS->SiS_Pr->UsePanelScaler = 1;
	   else			 pSiS->SiS_Pr->UsePanelScaler = 0;
	   if(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTCENTER) {
	      if(value & 0x04)      pSiS->SiS_Pr->CenterScreen = -1;
	      else if(value & 0x08) pSiS->SiS_Pr->CenterScreen = 1;
	      else		    pSiS->SiS_Pr->CenterScreen = 0;
	   }
        }
     }
#ifdef TWDEBUG
  } else if(attribute == pSiS->xv_STR) {
     ULong port;
     CARD8 reg;
     switch((value & 0xff000000) >> 24) {
     case 0x00: port = SISSR;    break;
     case 0x01: port = SISPART1; break;
     case 0x02: port = SISPART2; break;
     case 0x03: port = SISPART3; break;
     case 0x04: port = SISPART4; break;
     case 0x05: port = SISCR;    break;
     case 0x06: port = SISVID;   break;
     default:   return BadValue;
     }
     outSISIDXREG(port,((value & 0x00ff0000) >> 16), ((value & 0x0000ff00) >> 8));
     inSISIDXREG(port, ((value & 0x00ff0000) >> 16), reg);
     xf86DrvMsg(0, 0, "SetREG %x -> %x -> %x\n", ((value & 0x00ff0000) >> 16), ((value & 0x0000ff00) >> 8), reg);
#endif
  } else {
     return BadMatch;
  }

  return Success;
}

int
SISGetPortUtilAttribute(ScrnInfoPtr pScrn,  Atom attribute,
			INT32 *value, SISPortPrivPtr pPriv)
{
  SISPtr pSiS = SISPTR(pScrn);
#ifdef SISDUALHEAD
  SISEntPtr pSiSEnt = pSiS->entityPrivate;;
#endif

  if(attribute == pSiS->xv_QVF) {
     *value = pSiS->VBFlags;
  } else if(attribute == pSiS->xv_GDV) {
     *value = SISDRIVERIVERSION;
  } else if(attribute == pSiS->xv_GHI) {
     *value = (pSiS->ChipFlags & 0xffff) | (pSiS->ChipType << 16) | (pSiS->ChipRev << 24);
  } else if(attribute == pSiS->xv_GBI) {
     *value = (pSiS->PciBus << 16) | (pSiS->PciDevice << 8) | pSiS->PciFunc;
  } else if(attribute == pSiS->xv_QVV) {
     *value = SIS_VBFlagsVersion;
  } else if(attribute == pSiS->xv_QDD) {
     *value = pSiS->detectedCRT2Devices;
  } else if(attribute == pSiS->xv_CT1) {
     *value = pSiS->CRT1isoff ? 0 : 1;
  } else if(attribute == pSiS->xv_GSF) {
     *value = pSiS->SiS_SD_Flags;
  } else if(attribute == pSiS->xv_GSF2) {
     *value = pSiS->SiS_SD2_Flags;
  } else if(attribute == pSiS->xv_USD) {
     *value = pSiS->xv_sisdirectunlocked;
  } else if(attribute == pSiS->xv_TAF) {
     *value = SiS_GetSISTVantiflicker(pScrn);
  } else if(attribute == pSiS->xv_TSA) {
     *value = SiS_GetSISTVsaturation(pScrn);
  } else if(attribute == pSiS->xv_TEE) {
     *value = SiS_GetSISTVedgeenhance(pScrn);
  } else if(attribute == pSiS->xv_CFI) {
     *value = SiS_GetSISTVcfilter(pScrn);
  } else if(attribute == pSiS->xv_YFI) {
     *value = SiS_GetSISTVyfilter(pScrn);
  } else if(attribute == pSiS->xv_COC) {
     *value = SiS_GetSISTVcolcalib(pScrn, TRUE);
  } else if(attribute == pSiS->xv_COF) {
     *value = SiS_GetSISTVcolcalib(pScrn, FALSE);
  } else if(attribute == pSiS->xv_TCO) {
     *value = SiS_GetCHTVcontrast(pScrn);
  } else if(attribute == pSiS->xv_TTE) {
     *value = SiS_GetCHTVtextenhance(pScrn);
  } else if(attribute == pSiS->xv_TCF) {
     *value = SiS_GetCHTVchromaflickerfilter(pScrn);
  } else if(attribute == pSiS->xv_TLF) {
     *value = SiS_GetCHTVlumaflickerfilter(pScrn);
  } else if(attribute == pSiS->xv_TCC) {
     *value = SiS_GetCHTVcvbscolor(pScrn);
  } else if(attribute == pSiS->xv_CMDR) {
     *value = pSiS->xv_sd_result;
  } else if(attribute == pSiS->xv_OVR) {
     /* Changing of CRT2 settings not supported in DHM! */
     *value = 0;
     if(pSiS->OptTVSOver == 1)         *value = 3;
     else if(pSiS->UseCHOverScan == 1) *value = 2;
     else if(pSiS->UseCHOverScan == 0) *value = 1;
  } else if(attribute == pSiS->xv_SGA) {
     *value = 0;
#ifdef SISDUALHEAD
     if(pPriv->dualHeadMode) {
        if(pSiSEnt->CRT1gamma) *value |= 0x01;
	if(pSiSEnt->CRT2gamma) *value |= 0x02;
     } else {
#endif
	if(pSiS->CRT1gamma)    *value |= 0x01;
	if(pSiS->CRT2gamma)    *value |= 0x02;
	if(pSiS->CRT2SepGamma) *value |= 0x08;
#ifdef SISDUALHEAD
     }
#endif
     if(pSiS->XvGamma) *value |= 0x04;
  } else if(attribute == pSiS->xv_TXS) {
     *value = SiS_GetTVxscale(pScrn);
  } else if(attribute == pSiS->xv_TYS) {
     *value = SiS_GetTVyscale(pScrn);
  } else if(attribute == pSiS->xv_GSS) {
     *value = (pScrn->virtualX << 16) | pScrn->virtualY;
  } else if(attribute == pSiS->xv_BRR) {
     *value = pSiS->GammaBriR;
  } else if(attribute == pSiS->xv_BRG) {
     *value = pSiS->GammaBriG;
  } else if(attribute == pSiS->xv_BRB) {
     *value = pSiS->GammaBriB;
  } else if(attribute == pSiS->xv_PBR) {
     *value = 1000;
  } else if(attribute == pSiS->xv_PBG) {
     *value = 1000;
  } else if(attribute == pSiS->xv_PBB) {
     *value = 1000;
  } else if(attribute == pSiS->xv_BRR2) {
#ifdef SISDUALHEAD
     if(pPriv->dualHeadMode) *value = pSiSEnt->GammaBriR;
     else
#endif
          *value = pSiS->GammaBriR;
  } else if(attribute == pSiS->xv_BRG2) {
#ifdef SISDUALHEAD
     if(pPriv->dualHeadMode) *value = pSiSEnt->GammaBriG;
     else
#endif
          *value = pSiS->GammaBriG;
  } else if(attribute == pSiS->xv_BRB2) {
#ifdef SISDUALHEAD
     if(pPriv->dualHeadMode) *value = pSiSEnt->GammaBriB;
     else
#endif
          *value = pSiS->GammaBriB;
  } else if(attribute == pSiS->xv_PBR2) {
     *value = 1000;
  } else if(attribute == pSiS->xv_PBG2) {
     *value = 1000;
  } else if(attribute == pSiS->xv_PBB2) {
     *value = 1000;
  } else if(attribute == pSiS->xv_GARC2) {
     *value = (int)(pSiS->GammaR2 * 1000);
  } else if(attribute == pSiS->xv_GAGC2) {
     *value = (int)(pSiS->GammaG2 * 1000);
  } else if(attribute == pSiS->xv_GABC2) {
     *value = (int)(pSiS->GammaB2 * 1000);
  } else if(attribute == pSiS->xv_BRRC2) {
     *value = pSiS->GammaBriR2;
  } else if(attribute == pSiS->xv_BRGC2) {
     *value = pSiS->GammaBriG2;
  } else if(attribute == pSiS->xv_BRBC2) {
     *value = pSiS->GammaBriB2;
  } else if(attribute == pSiS->xv_PBRC2) {
     *value = 1000;
  } else if(attribute == pSiS->xv_PBGC2) {
     *value = 1000;
  } else if(attribute == pSiS->xv_PBBC2) {
     *value = 1000;
  } else if(attribute == pSiS->xv_SHC) {
     *value = pSiS->HideHWCursor ? 1 : 0;
  } else if(attribute == pSiS->xv_PMD) {
     *value = 0;
     if(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTSCALE) {
        switch(pSiS->SiS_Pr->UsePanelScaler) {
           case -1: *value |= 0x01; break;
           case 1:  *value |= 0x02; break;
        }
	if(pSiS->SiS_SD_Flags & SiS_SD_SUPPORTCENTER) {
           switch(pSiS->SiS_Pr->CenterScreen) {
              case -1: *value |= 0x04; break;
              case 1:  *value |= 0x08; break;
           }
	}
     }
  } else {
     return BadMatch;
  }

  return Success;
}

#endif /* XV_SD_DEPRECATED */

