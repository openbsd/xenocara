/* $XFree86$ */
/* $XdotOrg: driver/xf86-video-sisusb/src/sisusb_utility.c,v 1.8 2005/09/16 13:52:16 twini Exp $ */
/*
 * SiSUSB driver utility interface & routines
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

#include "sisusb.h"
#define NEED_REPLIES
#define NEED_EVENTS
#include <X11/X.h>
#include "dixstruct.h"
#define _XF86MISC_SERVER_
#include <X11/extensions/xf86misc.h>

#include "sisusb_videostr.h"

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
#define SDC_CMD_CHECKMODEFORCRT2 	0x98980006
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
/* more to come, adapt MAXCOMMAND! */
#define SDC_MAXCOMMAND			SDC_CMD_SETGETNEWGAMMACRT2

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

#ifdef X_XF86MiscPassMessage
int		SISUSBHandleMessage(int scrnIndex, const char *msgtype,
					const char *msgval, char **retmsg);
#endif
void		SiSUSBCtrlExtInit(ScrnInfoPtr pScrn);
void		SiSUSBCtrlExtUnregister(SISUSBPtr pSiSUSB, int index);

#ifdef SIS_GLOBAL_ENABLEXV
#ifdef XV_SD_DEPRECATED
int		SISUSBSetPortUtilAttribute(ScrnInfoPtr pScrn, Atom attribute,
					INT32 value, SISUSBPortPrivPtr pPriv);
int		SISUSBGetPortUtilAttribute(ScrnInfoPtr pScrn,  Atom attribute,
					INT32 *value, SISUSBPortPrivPtr pPriv);
#endif
#ifdef SIS_ENABLEXV
extern void	SiSUSBUpdateXvGamma(SISUSBPtr pSiSUSB, SISUSBPortPrivPtr pPriv);
#endif
extern void	SISUSBSetPortDefaults(ScrnInfoPtr pScrn, SISUSBPortPrivPtr pPriv);
#endif /* SIS_GLOBAL_ENABLEXV */

/***********************************
 *     MessageHandler interface    *
 *   (unused now; use extension)   *
 ***********************************/

#ifdef X_XF86MiscPassMessage
int
SISUSBHandleMessage(int scrnIndex, const char *msgtype, const char *msgval, char **retmsg)
{
    return BadMatch;
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
   SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
   SISUSBPortPrivPtr pPriv = NULL;
   int i;
   ULong j;

   if(sdcbuf->sdc_id != SDC_ID) return BadMatch;

   if(pSiSUSB->adaptor) {
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
      sdcbuf->sdc_result[0]  = SDC_BUS_TYPE_USB;
      sdcbuf->sdc_result[1]  = pSiSUSB->USBBus;  /* not set! */
      sdcbuf->sdc_result[2]  = pSiSUSB->USBDev;  /* not set! */
      sdcbuf->sdc_result[3]  = 0;
      sdcbuf->sdc_result[4]  = 0;
      sdcbuf->sdc_result[5]  = pSiSUSB->ChipFlags;
      sdcbuf->sdc_result[6]  = pSiSUSB->ChipType;
      sdcbuf->sdc_result[7]  = pSiSUSB->ChipRev;
      sdcbuf->sdc_result[8]  = SISUSBDRIVERVERSIONYEAR;
      sdcbuf->sdc_result[9]  = SISUSBDRIVERVERSIONMONTH;
      sdcbuf->sdc_result[10] = SISUSBDRIVERVERSIONDAY;
      sdcbuf->sdc_result[11] = SISUSBDRIVERREVISION;
      sdcbuf->sdc_result[12] = pScrn->videoRam;
      sdcbuf->sdc_result[13] = 0;		/* No UMA */
      sdcbuf->sdc_result[14] = pScrn->videoRam;	/* All LFB */
      sdcbuf->sdc_buffer[0]  = 0;
      break;

   case SDC_CMD_GETALLFLAGS:
      sdcbuf->sdc_result[0] = SIS_VBFlagsVersion;
      sdcbuf->sdc_result[1] = pSiSUSB->VBFlags;
      sdcbuf->sdc_result[2] = pSiSUSB->SiS_SD_Flags;
      sdcbuf->sdc_result[3] = pSiSUSB->SiS_SD2_Flags;
      sdcbuf->sdc_result[4] = 0; /* No CRT2 devices */
      sdcbuf->sdc_result[5] = pSiSUSB->VBFlags2;
      sdcbuf->sdc_result[6] = pSiSUSB->SiS_SD3_Flags;
      sdcbuf->sdc_result[7] = pSiSUSB->SiS_SD4_Flags;
      break;

   case SDC_CMD_GETVBFLAGSVERSION:
      sdcbuf->sdc_result[0] = SIS_VBFlagsVersion;
      break;

   case SDC_CMD_GETVBFLAGS:
      sdcbuf->sdc_result[0] = pSiSUSB->VBFlags;
      sdcbuf->sdc_result[1] = pSiSUSB->VBFlags2;
      break;

   case SDC_CMD_CHECKMODEFORCRT2:
      sdcbuf->sdc_result[0] = 0;
      break;

   case SDC_CMD_SETVBFLAGS:
   case SDC_CMD_SETCRT1STATUS:
   case SDC_CMD_REDETECTCRT2DEVICES:
      if(!pSiSUSB->xv_sisdirectunlocked) {
	 sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      }
      break;

   case SDC_CMD_GETDETECTEDDEVICES:
      sdcbuf->sdc_result[0] = 0;
      break;

   case SDC_CMD_GETCRT1STATUS:
      sdcbuf->sdc_result[0] = 1;
      break;

   case SDC_CMD_GETSDFLAGS:
      sdcbuf->sdc_result[0] = pSiSUSB->SiS_SD_Flags;
      break;

   case SDC_CMD_GETSD2FLAGS:
      sdcbuf->sdc_result[0] = pSiSUSB->SiS_SD2_Flags;
      break;

   case SDC_CMD_GETLOCKSTATUS:
      sdcbuf->sdc_result[0] = pSiSUSB->xv_sisdirectunlocked;
      break;

   case SDC_CMD_SETLOCKSTATUS:
      if(pSiSUSB->enablesisctrl) {
	 if(sdcbuf->sdc_parm[0] == SIS_DIRECTKEY) {
	    pSiSUSB->xv_sisdirectunlocked++;
	 } else if(pSiSUSB->xv_sisdirectunlocked) {
	    pSiSUSB->xv_sisdirectunlocked--;
	 }
      } else {
	 pSiSUSB->xv_sisdirectunlocked = 0;
	 sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      }
      break;

   case SDC_CMD_GETTVANTIFLICKER:
   case SDC_CMD_GETTVSATURATION:
   case SDC_CMD_GETTVEDGEENHANCE:
   case SDC_CMD_GETTVCFILTER:
   case SDC_CMD_GETTVYFILTER:
   case SDC_CMD_GETTVCHCONTRAST:
   case SDC_CMD_GETTVCHTEXTENHANCE:
   case SDC_CMD_GETTVCHCHROMAFLICKERFILTER:
   case SDC_CMD_GETTVCHLUMAFLICKERFILTER:
   case SDC_CMD_GETTVCHCVBSCOLOR:
   case SDC_CMD_GETCHTVOVERSCAN:
      sdcbuf->sdc_result[0] = 0;
      break;

   case SDC_CMD_SETTVANTIFLICKER:
   case SDC_CMD_SETTVSATURATION:
   case SDC_CMD_SETTVEDGEENHANCE:
   case SDC_CMD_SETTVCFILTER:
   case SDC_CMD_SETTVYFILTER:
   case SDC_CMD_SETTVCHCONTRAST:
   case SDC_CMD_SETTVCHTEXTENHANCE:
   case SDC_CMD_SETTVCHCHROMAFLICKERFILTER:
   case SDC_CMD_SETTVCHLUMAFLICKERFILTER:
   case SDC_CMD_SETTVCHCVBSCOLOR:
   case SDC_CMD_SETCHTVOVERSCAN:
   case SDC_CMD_SETTVCOLORCALIB:
      if(!pSiSUSB->xv_sisdirectunlocked) {
	 sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      }
      break;

   case SDC_CMD_GETTVCOLORCALIB:
      sdcbuf->sdc_result[0] = 32768;
      sdcbuf->sdc_result[1] = 32768;
      break;

   case SDC_CMD_GETGAMMASTATUS:
      {
         int i = 0;
	 if(pSiSUSB->CRT1gamma) i |= 0x01;
	 if(pSiSUSB->XvGamma)   i |= 0x04;
         sdcbuf->sdc_result[0] = i;
      }
      break;

   case SDC_CMD_SETGAMMASTATUS:
      if(pSiSUSB->xv_sisdirectunlocked) {
         int value = sdcbuf->sdc_parm[0];
#ifdef SIS_ENABLEXV
	 Bool backup = pSiSUSB->XvGamma;
#endif
	 pSiSUSB->CRT1gamma = (value & 0x01) ? TRUE : FALSE;
	 pSiSUSB->XvGamma = (value & 0x04) ? TRUE : FALSE;
#ifdef SIS_ENABLEXV
	 if(backup != pSiSUSB->XvGamma) {
	    if(pPriv) SiSUSBUpdateXvGamma(pSiSUSB, pPriv);
	 }
#endif
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETTVXSCALE:
   case SDC_CMD_GETTVYSCALE:
      sdcbuf->sdc_result[0] = 32768;
      break;

   case SDC_CMD_SETTVXSCALE:
   case SDC_CMD_SETTVYSCALE:
      if(!pSiSUSB->xv_sisdirectunlocked) {
	 sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      }
      break;

   case SDC_CMD_GETSCREENSIZE:
      sdcbuf->sdc_result[0] = (pScrn->virtualX << 16) | pScrn->virtualY;
      break;

   case SDC_CMD_GETGAMMABRIGHTNESS:  /* xv_BRx, xv_PBx */
   case SDC_CMD_GETGAMMABRIGHTNESS2: /* xv_BRx2, xv_PBx2 */
      sdcbuf->sdc_result[0] = pSiSUSB->GammaBriR;
      sdcbuf->sdc_result[1] = pSiSUSB->GammaBriG;
      sdcbuf->sdc_result[2] = pSiSUSB->GammaBriB;
      break;

   case SDC_CMD_GETNEWGAMMABRICON:  /* no xv pendant */
   case SDC_CMD_GETNEWGAMMABRICON2: /* no xv pendant */
      sdcbuf->sdc_result[0] = (CARD32)(((int)(pSiSUSB->NewGammaBriR * 1000.0)) + 1000);
      sdcbuf->sdc_result[1] = (CARD32)(((int)(pSiSUSB->NewGammaBriG * 1000.0)) + 1000);
      sdcbuf->sdc_result[2] = (CARD32)(((int)(pSiSUSB->NewGammaBriB * 1000.0)) + 1000);
      sdcbuf->sdc_result[3] = (CARD32)(((int)(pSiSUSB->NewGammaConR * 1000.0)) + 1000);
      sdcbuf->sdc_result[4] = (CARD32)(((int)(pSiSUSB->NewGammaConG * 1000.0)) + 1000);
      sdcbuf->sdc_result[5] = (CARD32)(((int)(pSiSUSB->NewGammaConB * 1000.0)) + 1000);
      break;

   case SDC_CMD_SETGAMMABRIGHTNESS:  /* xv_BRx, xv_PBx */
   case SDC_CMD_SETGAMMABRIGHTNESS2: /* xv_BRx2, xv_PBx2 */
      if(sdcbuf->sdc_parm[0] < 100 || sdcbuf->sdc_parm[0] > 10000 ||
	 sdcbuf->sdc_parm[1] < 100 || sdcbuf->sdc_parm[1] > 10000 ||
	 sdcbuf->sdc_parm[2] < 100 || sdcbuf->sdc_parm[2] > 10000) {
	 sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else if(pSiSUSB->xv_sisdirectunlocked) {
	 pSiSUSB->GammaBriR = sdcbuf->sdc_parm[0];
	 pSiSUSB->GammaBriG = sdcbuf->sdc_parm[1];
	 pSiSUSB->GammaBriB = sdcbuf->sdc_parm[2];
	 pSiSUSB->NewGammaBriR = pSiSUSB->NewGammaBriG = pSiSUSB->NewGammaBriB = 0.0;
	 pSiSUSB->NewGammaConR = pSiSUSB->NewGammaConG = pSiSUSB->NewGammaConB = 0.0;
	 pSiSUSB->SiS_SD3_Flags |= SiS_SD3_OLDGAMMAINUSE;
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_SETNEWGAMMABRICON:  /* no xv pendant */
   case SDC_CMD_SETNEWGAMMABRICON2: /* no xv pendant */
      if(sdcbuf->sdc_parm[0] > 2000 || sdcbuf->sdc_parm[1] > 2000 ||
	 sdcbuf->sdc_parm[2] > 2000 || sdcbuf->sdc_parm[3] > 2000 ||
	 sdcbuf->sdc_parm[4] > 2000 || sdcbuf->sdc_parm[5] > 2000) {
	 sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else if(pSiSUSB->xv_sisdirectunlocked) {
	 pSiSUSB->NewGammaBriR = ((float)((int)sdcbuf->sdc_parm[0] - 1000)) / 1000.0;
	 pSiSUSB->NewGammaBriG = ((float)((int)sdcbuf->sdc_parm[1] - 1000)) / 1000.0;
	 pSiSUSB->NewGammaBriB = ((float)((int)sdcbuf->sdc_parm[2] - 1000)) / 1000.0;
	 pSiSUSB->NewGammaConR = ((float)((int)sdcbuf->sdc_parm[3] - 1000)) / 1000.0;
	 pSiSUSB->NewGammaConG = ((float)((int)sdcbuf->sdc_parm[4] - 1000)) / 1000.0;
	 pSiSUSB->NewGammaConB = ((float)((int)sdcbuf->sdc_parm[5] - 1000)) / 1000.0;
	 pSiSUSB->GammaBriR = pSiSUSB->GammaBriG = pSiSUSB->GammaBriB = 1000;
	 pSiSUSB->SiS_SD3_Flags &= ~SiS_SD3_OLDGAMMAINUSE;
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_SETGETGAMMACRT2:
   case SDC_CMD_SETGETNEWGAMMACRT2:
      if(!pSiSUSB->xv_sisdirectunlocked) {
         sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      }
      break;

   case SDC_CMD_GETGETGAMMACRT2:
      sdcbuf->sdc_result[0] =
         sdcbuf->sdc_result[1] =
         sdcbuf->sdc_result[2] =
         sdcbuf->sdc_result[3] =
         sdcbuf->sdc_result[4] =
         sdcbuf->sdc_result[5] = 1000;
      break;

   case SDC_CMD_GETGETNEWGAMMACRT2:
      sdcbuf->sdc_result[0] =
         sdcbuf->sdc_result[1] =
         sdcbuf->sdc_result[2] =
         sdcbuf->sdc_result[3] =
         sdcbuf->sdc_result[4] =
         sdcbuf->sdc_result[5] =
         sdcbuf->sdc_result[6] =
         sdcbuf->sdc_result[7] =
         sdcbuf->sdc_result[8] = 1000;
      break;

   case SDC_CMD_GETHWCURSORSTATUS:
      sdcbuf->sdc_result[0] = pSiSUSB->HideHWCursor ? 1 : 0;
      break;

   case SDC_CMD_SETHWCURSORSTATUS:
      if(pSiSUSB->xv_sisdirectunlocked) {
	 Bool VisibleBackup = pSiSUSB->HWCursorIsVisible;
	 pSiSUSB->HideHWCursor = sdcbuf->sdc_parm[0] ? TRUE : FALSE;
	 if(pSiSUSB->CursorInfoPtr) {
	    if(VisibleBackup) {
	       if(sdcbuf->sdc_parm[0]) {
	 	  (pSiSUSB->CursorInfoPtr->HideCursor)(pScrn);
	       } else {
		  (pSiSUSB->CursorInfoPtr->ShowCursor)(pScrn);
	       }
	    }
	    pSiSUSB->HWCursorIsVisible = VisibleBackup;
	 }
      } else sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      break;

   case SDC_CMD_GETPANELMODE:
      sdcbuf->sdc_result[0] = 0;
      break;

   case SDC_CMD_SETPANELMODE:
      if(!pSiSUSB->xv_sisdirectunlocked) {
         sdcbuf->sdc_result_header = SDC_RESULT_NOPERM;
      }
      break;

   case SDC_CMD_GETMERGEDMODEDETAILS:
      sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETDEVICENAME:		/* In DualHead mode, this returns CRT1 data */
      sdcbuf->sdc_result[0] = 0;
      sisutil_prepare_string(sdcbuf, pSiSUSB->devsectname);
      break;

   case SDC_CMD_GETMONITORNAME:		/* In DualHead mode, this returns CRT1 data */
      sdcbuf->sdc_result[0] = 0;
      if(pScrn->monitor) {
         sisutil_prepare_string(sdcbuf, pScrn->monitor->id);
      }
      break;

   case SDC_CMD_GETDEVICENAME2:		/* In DualHead mode, this returns CRT2 data */
      sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETMONITORNAME2:	/* In DualHead mode, this returns CRT2 data */
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
      if(pPriv) {
         int val = sdcbuf->sdc_parm[0] - 32768;
         if(val >= -8 && val <= 7) pPriv->hue = val;
	 else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETXVHUE:
      if(pPriv) {
         sdcbuf->sdc_result[0] = pPriv->hue + 32768;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVSATURATION:
      if(pPriv) {
         int val = sdcbuf->sdc_parm[0] - 32768;
         if(val >= -7 && val <= 7) pPriv->saturation = val;
	 else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETXVSATURATION:
      if(pPriv) {
         sdcbuf->sdc_result[0] = pPriv->saturation + 32768;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVGAMMA:
      if(pPriv) {
         if(sdcbuf->sdc_parm[0] < 100 || sdcbuf->sdc_parm[0] > 10000 ||
	    sdcbuf->sdc_parm[1] < 100 || sdcbuf->sdc_parm[1] > 10000 ||
	    sdcbuf->sdc_parm[2] < 100 || sdcbuf->sdc_parm[2] > 10000) {
	    sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
	 } else {
	    pSiSUSB->XvGammaRed = sdcbuf->sdc_parm[0];
	    pSiSUSB->XvGammaGreen = sdcbuf->sdc_parm[1];
	    pSiSUSB->XvGammaBlue = sdcbuf->sdc_parm[2];
#ifdef SIS_ENABLEXV
	    SiSUSBUpdateXvGamma(pSiSUSB, pPriv);
#endif
	 }
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_GETXVGAMMA:
      if(pPriv) {
         sdcbuf->sdc_result[0] = pSiSUSB->XvGammaRed;
	 sdcbuf->sdc_result[1] = pSiSUSB->XvGammaGreen;
	 sdcbuf->sdc_result[2] = pSiSUSB->XvGammaBlue;
      } else sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_SETXVCOLORKEY:
      if(pPriv) {
         pPriv->colorKey = pSiSUSB->colorKey = sdcbuf->sdc_parm[0];
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
#ifdef SIS_GLOBAL_ENABLEXV
         SISUSBSetPortDefaults(pScrn, pPriv);
#endif
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
      break;

   case SDC_CMD_GETXVSWITCHCRT:
   case SDC_CMD_GETMONGAMMACRT1:
   case SDC_CMD_GETMONGAMMACRT2:
      sdcbuf->sdc_result[0] = 0;
      break;

   case SDC_CMD_SETTVXPOS:
   case SDC_CMD_SETTVYPOS:
      break;

   case SDC_CMD_GETTVXPOS:
   case SDC_CMD_GETTVYPOS:
      sdcbuf->sdc_result[0] = 32768;
      break;

   case SDC_CMD_SETXVDEINT:
   case SDC_CMD_GETXVDEINT:
      sdcbuf->sdc_result_header = SDC_RESULT_INVAL;
      break;

   case SDC_CMD_LOGQUIET:
      pSiSUSB->SCLogQuiet = sdcbuf->sdc_parm[0] ? TRUE : FALSE;
      break;

   default:
      sdcbuf->sdc_result_header = SDC_RESULT_UNDEFCMD;
   }

   return Success;
}

/* Proc */

static int
SiSUSBProcSiSCtrlQueryVersion(ClientPtr client)
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
        swaps(&rep.sequenceNumber, n);
        swapl(&rep.length, n);
        swaps(&rep.majorVersion, n);
        swaps(&rep.minorVersion, n);
    }
    WriteToClient(client, sizeof(xSiSCtrlQueryVersionReply), (char *)&rep);
    return (client->noClientException);
}

static int
SiSUSBProcSiSCtrlCommand(ClientPtr client)
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
    if((ret = (myctrl->HandleSiSDirectCommand[rep.screen])(&rep)) != Success) {
       return ret;
    }

    rep.type = X_Reply;
    rep.length = (sizeof(xSiSCtrlCommandReply) - sizeof(xGenericReply)) >> 2;
    rep.sequenceNumber = client->sequence;

    if(client->swapped) {
       swaps(&rep.sequenceNumber, n);
       swapl(&rep.length, n);
       swapl(&rep.screen, n);
       swapl(&rep.sdc_id, n);
       swapl(&rep.sdc_command, n);
       swapl(&rep.sdc_result_header, n);
       for(i = 0; i < SDC_NUM_PARM_RESULT; i++) {
	  swapl(&rep.sdc_parm[i], n);
	  swapl(&rep.sdc_result[i], n);
       }
    }
    WriteToClient(client, sizeof(xSiSCtrlCommandReply), (char *)&rep);
    return client->noClientException;
}

static int
SiSUSBProcSiSCtrlDispatch(ClientPtr client)
{
    REQUEST(xReq);
    switch(stuff->data) {
	case X_SiSCtrlQueryVersion:
	     return SiSUSBProcSiSCtrlQueryVersion(client);
	case X_SiSCtrlCommand:
	     return SiSUSBProcSiSCtrlCommand(client);
    }
    return BadRequest;
}

/* SProc */

static int
SiSUSBSProcSiSCtrlQueryVersion(ClientPtr client)
{
    REQUEST(xSiSCtrlQueryVersionReq);
    register int n;
    swaps(&stuff->length, n);
    REQUEST_SIZE_MATCH(xSiSCtrlQueryVersionReq);
    return SiSUSBProcSiSCtrlQueryVersion(client);
}

static int
SiSUSBSProcSiSCtrlCommand(ClientPtr client)
{
    REQUEST(xSiSCtrlCommandReq);
    register int n;
    int i;
    swaps(&stuff->length, n);
    swapl(&stuff->screen, n);
    swapl(&stuff->sdc_id, n);
    swapl(&stuff->sdc_command, n);
    swapl(&stuff->sdc_result_header, n);
    for(i = 0; i < SDC_NUM_PARM_RESULT; i++) {
       swapl(&stuff->sdc_parm[i], n);
       swapl(&stuff->sdc_result[i], n);
    }
    REQUEST_SIZE_MATCH(xSiSCtrlCommandReq);
    return SiSUSBProcSiSCtrlCommand(client);
}

static int
SiSUSBSProcSiSCtrlDispatch(ClientPtr client)
{
    REQUEST(xReq);
    switch(stuff->data) {
       case X_SiSCtrlQueryVersion:
	  return SiSUSBSProcSiSCtrlQueryVersion(client);
       case X_SiSCtrlCommand:
	  return SiSUSBSProcSiSCtrlCommand(client);
    }
    return BadRequest;
}

static void
SiSUSBCtrlResetProc(ExtensionEntry* extEntry)
{
    /* Called by CloseDownExtensions() */
    if(extEntry->extPrivate) {
       xfree(extEntry->extPrivate);
       extEntry->extPrivate = NULL;
    }
}

void
SiSUSBCtrlExtInit(ScrnInfoPtr pScrn)
{
   SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
   ExtensionEntry  *myext;
   xSiSCtrlScreenTable *myctrl;
   unsigned int version, revision;

   pSiSUSB->SCLogQuiet = FALSE;

   if(!(myext = CheckExtension(SISCTRL_PROTOCOL_NAME))) {

      if(!(myctrl = xcalloc(sizeof(xSiSCtrlScreenTable), 1)))
         return;

      if(!(myext = AddExtension(SISCTRL_PROTOCOL_NAME, 0, 0,
				SiSUSBProcSiSCtrlDispatch,
				SiSUSBSProcSiSCtrlDispatch,
				SiSUSBCtrlResetProc,
				StandardMinorOpcode))) {
         xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	 		"Failed to add SISCTRL extension\n");
	 xfree(myctrl);
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

      pSiSUSB->SiSCtrlExtEntry = myext;

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
SiSUSBCtrlExtUnregister(SISUSBPtr pSiSUSB, int index)
{
   ExtensionEntry  *myext;
   xSiSCtrlScreenTable *myctrl;

   if(!pSiSUSB->SiSCtrlExtEntry) return;

   /* Since CloseDownExtensions() is called before
    * our CloseScreen(), we must not use the saved
    * ptr here, but instead check for the extension.
    */

   if((myext = CheckExtension(SISCTRL_PROTOCOL_NAME))) {
      if((myctrl = (xSiSCtrlScreenTable *)pSiSUSB->SiSCtrlExtEntry->extPrivate)) {
         myctrl->HandleSiSDirectCommand[index] = NULL;
      }
   }
}

/***********************************
 *     Xv attribute interface      *
 ***********************************/

#ifdef XV_SD_DEPRECATED

int
SISUSBSetPortUtilAttribute(ScrnInfoPtr pScrn, Atom attribute,
			INT32 value, SISUSBPortPrivPtr pPriv)
{
  SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

  if(attribute == pSiSUSB->xv_USD) {
     if(pSiSUSB->enablesisctrl) {
	 if(value == SIS_DIRECTKEY) {
	   pSiSUSB->xv_sisdirectunlocked++;
	} else if(pSiSUSB->xv_sisdirectunlocked) {
	   pSiSUSB->xv_sisdirectunlocked--;
	}
     } else {
	pSiSUSB->xv_sisdirectunlocked = 0;
     }
  } else if(attribute == pSiSUSB->xv_SVF) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_CT1) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_RDT) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_TAF) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_TSA) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_TEE) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_CFI) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_YFI) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_COC) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_COF) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_TCO) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_TTE) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_TCF) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_TLF) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_TCC) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_OVR) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_CMD) {
     if(pSiSUSB->xv_sisdirectunlocked) {
	pSiSUSB->xv_sd_result = (value & 0xffffff00);
     }
  } else if(attribute == pSiSUSB->xv_SGA) {
     if(pSiSUSB->xv_sisdirectunlocked) {
#ifdef SIS_ENABLEXV
        Bool backup = pSiSUSB->XvGamma;
#endif
        pSiSUSB->CRT1gamma = (value & 0x01) ? TRUE : FALSE;
	pSiSUSB->XvGamma = (value & 0x04) ? TRUE : FALSE;
#ifdef SIS_ENABLEXV
        if(backup != pSiSUSB->XvGamma) {
	   SiSUSBUpdateXvGamma(pSiSUSB, pPriv);
	}
#endif
     }
  } else if(attribute == pSiSUSB->xv_TXS) {
     if((value < -16) || (value > 16)) return BadValue;
  } else if(attribute == pSiSUSB->xv_TYS) {
     if((value < -4) || (value > 3)) return BadValue;
  } else if(attribute == pSiSUSB->xv_BRR) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiSUSB->xv_sisdirectunlocked) {
	pSiSUSB->GammaBriR = value;
     }
  } else if(attribute == pSiSUSB->xv_BRG) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiSUSB->xv_sisdirectunlocked) {
	pSiSUSB->GammaBriG = value;
     }
  } else if(attribute == pSiSUSB->xv_BRB) {
     if((value < 100) || (value > 10000)) return BadValue;
     if(pSiSUSB->xv_sisdirectunlocked) {
	pSiSUSB->GammaBriB = value;
     }
  } else if(attribute == pSiSUSB->xv_PBR) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_PBG) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_PBB) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_BRR2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_BRG2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_BRB2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_PBR2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_PBG2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_PBB2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_GARC2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_GAGC2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_GABC2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_BRRC2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_BRGC2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_BRBC2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_PBRC2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_PBGC2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_PBBC2) {
     /* Nop */
  } else if(attribute == pSiSUSB->xv_SHC) {
     if(pSiSUSB->xv_sisdirectunlocked) {
	Bool VisibleBackup = pSiSUSB->HWCursorIsVisible;
	pSiSUSB->HideHWCursor = value ? TRUE : FALSE;
	if(pSiSUSB->CursorInfoPtr) {
	   if(VisibleBackup) {
	      if(value) {
		 (pSiSUSB->CursorInfoPtr->HideCursor)(pScrn);
	      } else {
		 (pSiSUSB->CursorInfoPtr->ShowCursor)(pScrn);
	      }
	   }
	   pSiSUSB->HWCursorIsVisible = VisibleBackup;
	}
     }
  } else if(attribute == pSiSUSB->xv_PMD) {
     /* Nop */
#ifdef TWDEBUG
  } else if(attribute == pSiSUSB->xv_STR) {
     ULong port;
     switch((value & 0xff000000) >> 24) {
     case 0x00: port = SISSR;    break;
     case 0x01: port = SISPART1; break;
     case 0x05: port = SISCR;    break;
     case 0x06: port = SISVID;   break;
     default:   return BadValue;
     }
     outSISIDXREG(pSiSUSB,port,((value & 0x00ff0000) >> 16), ((value & 0x0000ff00) >> 8));
#endif
  } else {
     return BadMatch;
  }

  return Success;
}

int
SISUSBGetPortUtilAttribute(ScrnInfoPtr pScrn,  Atom attribute,
			INT32 *value, SISUSBPortPrivPtr pPriv)
{
  SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

  if(attribute == pSiSUSB->xv_QVF) {
     *value = pSiSUSB->VBFlags;
  } else if(attribute == pSiSUSB->xv_GDV) {
     *value = SISUSBDRIVERIVERSION;
  } else if(attribute == pSiSUSB->xv_GHI) {
     *value = (pSiSUSB->ChipFlags & 0xffff) | (pSiSUSB->ChipType << 16) | (pSiSUSB->ChipRev << 24);
  } else if(attribute == pSiSUSB->xv_GBI) {
     *value = ((pSiSUSB->USBBus & 0xff) << 8) | (pSiSUSB->USBDev & 0xff);
  } else if(attribute == pSiSUSB->xv_QVV) {
     *value = SIS_VBFlagsVersion;
  } else if(attribute == pSiSUSB->xv_QDD) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_CT1) {
     *value = 1;
  } else if(attribute == pSiSUSB->xv_GSF) {
     *value = pSiSUSB->SiS_SD_Flags;
  } else if(attribute == pSiSUSB->xv_GSF2) {
     *value = pSiSUSB->SiS_SD2_Flags;
  } else if(attribute == pSiSUSB->xv_USD) {
     *value = pSiSUSB->xv_sisdirectunlocked;
  } else if(attribute == pSiSUSB->xv_TAF) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_TSA) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_TEE) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_CFI) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_YFI) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_COC) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_COF) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_TCO) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_TTE) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_TCF) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_TLF) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_TCC) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_CMDR) {
     *value = pSiSUSB->xv_sd_result;
  } else if(attribute == pSiSUSB->xv_OVR) {
     /* Changing of CRT2 settings not supported in DHM! */
     *value = 0;
  } else if(attribute == pSiSUSB->xv_SGA) {
     *value = 0;
     if(pSiSUSB->CRT1gamma) *value |= 0x01;
     if(pSiSUSB->XvGamma)   *value |= 0x04;
  } else if(attribute == pSiSUSB->xv_TXS) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_TYS) {
     *value = 0;
  } else if(attribute == pSiSUSB->xv_GSS) {
     *value = (pScrn->virtualX << 16) | pScrn->virtualY;
  } else if(attribute == pSiSUSB->xv_BRR) {
     *value = pSiSUSB->GammaBriR;
  } else if(attribute == pSiSUSB->xv_BRG) {
     *value = pSiSUSB->GammaBriG;
  } else if(attribute == pSiSUSB->xv_BRB) {
     *value = pSiSUSB->GammaBriB;
  } else if(attribute == pSiSUSB->xv_PBR) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_PBG) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_PBB) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_BRR2) {
     *value = pSiSUSB->GammaBriR;
  } else if(attribute == pSiSUSB->xv_BRG2) {
     *value = pSiSUSB->GammaBriG;
  } else if(attribute == pSiSUSB->xv_BRB2) {
     *value = pSiSUSB->GammaBriB;
  } else if(attribute == pSiSUSB->xv_PBR2) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_PBG2) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_PBB2) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_GARC2) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_GAGC2) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_GABC2) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_BRRC2) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_BRGC2) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_BRBC2) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_PBRC2) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_PBGC2) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_PBBC2) {
     *value = 1000;
  } else if(attribute == pSiSUSB->xv_SHC) {
     *value = pSiSUSB->HideHWCursor ? 1 : 0;
  } else if(attribute == pSiSUSB->xv_PMD) {
     *value = 0;
  } else {
     return BadMatch;
  }

  return Success;
}

#endif /* XV_SD_DEPRECATED */


