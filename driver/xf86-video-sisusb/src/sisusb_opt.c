/* $XFree86$ */
/* $XdotOrg: driver/xf86-video-sisusb/src/sisusb_opt.c,v 1.7 2005/10/27 18:27:19 twini Exp $ */
/*
 * SiSUSB driver option evaluation
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria
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
 * Author:  	Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sisusb.h"

#include "xf86str.h"
#include "xf86Cursor.h"

typedef enum {
    OPTION_NOACCEL,
    OPTION_SW_CURSOR,
    OPTION_HW_CURSOR,
    OPTION_NOXVIDEO,
    OPTION_NOINTERNALMODES,
    OPTION_USERGBCURSOR,
    OPTION_RESTOREBYSET,
    OPTION_CRT1GAMMA,
    OPTION_XVGAMMA,
    OPTION_XVDEFCONTRAST,
    OPTION_XVDEFBRIGHTNESS,
    OPTION_XVDEFHUE,
    OPTION_XVDEFSATURATION,
    OPTION_XVDEFDISABLEGFX,
    OPTION_XVDEFDISABLEGFXLR,
    OPTION_XVUSECHROMAKEY,
    OPTION_XVCHROMAMIN,
    OPTION_XVCHROMAMAX,
    OPTION_XVDISABLECOLORKEY,
    OPTION_XVINSIDECHROMAKEY,
    OPTION_XVYUVCHROMAKEY,
    OPTION_ENABLESISCTRL,
    OPTION_STOREDBRI,
    OPTION_NEWSTOREDBRI,
    OPTION_NEWSTOREDCON,
    OPTION_DISCONNTIMEOUT,
    OPTION_PSEUDO
} SISUSBOpts;

static const OptionInfoRec SISUSBOptions[] = {
    { OPTION_DISCONNTIMEOUT,           	"DisconnectTimeout",      OPTV_INTEGER,   {0}, FALSE },
    { OPTION_ENABLESISCTRL,		"EnableSiSCtrl",   	  OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_SW_CURSOR,         	"SWCursor",               OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_HW_CURSOR,         	"HWCursor",               OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_USERGBCURSOR, 		"UseColorHWCursor",	  OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_NOINTERNALMODES,   	"NoInternalModes",        OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_RESTOREBYSET,		"RestoreBySetMode", 	  OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_CRT1GAMMA,			"CRT1Gamma", 	  	  OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_STOREDBRI,			"GammaBrightness",  	  OPTV_STRING,    {0}, FALSE },
    { OPTION_STOREDBRI,			"StoredGammaBrightness",  OPTV_STRING,    {0}, FALSE },
    { OPTION_NEWSTOREDBRI,		"Brightness",		  OPTV_STRING,	  {0}, FALSE },
    { OPTION_NEWSTOREDBRI,		"NewGammaBrightness",	  OPTV_STRING,	  {0}, FALSE },
    { OPTION_NEWSTOREDCON,		"Contrast",		  OPTV_STRING,	  {0}, FALSE },
    { OPTION_NEWSTOREDCON,		"NewGammaContrast",	  OPTV_STRING,	  {0}, FALSE },
#ifdef SIS_GLOBAL_ENABLEXV
    { OPTION_NOXVIDEO,          	"NoXvideo",               OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_XVGAMMA,			"XvGamma", 	  	  OPTV_STRING,    {0}, FALSE },
    { OPTION_XVDEFCONTRAST,		"XvDefaultContrast", 	  OPTV_INTEGER,   {0}, FALSE },
    { OPTION_XVDEFBRIGHTNESS,		"XvDefaultBrightness", 	  OPTV_INTEGER,   {0}, FALSE },
    { OPTION_XVDEFHUE,			"XvDefaultHue", 	  OPTV_INTEGER,   {0}, FALSE },
    { OPTION_XVDEFSATURATION,		"XvDefaultSaturation", 	  OPTV_INTEGER,   {0}, FALSE },
    { OPTION_XVDEFDISABLEGFX,		"XvDefaultDisableGfx", 	  OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_XVDEFDISABLEGFXLR,		"XvDefaultDisableGfxLR",  OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_XVCHROMAMIN,		"XvChromaMin", 	  	  OPTV_INTEGER,   {0}, FALSE },
    { OPTION_XVCHROMAMAX,		"XvChromaMax", 	  	  OPTV_INTEGER,   {0}, FALSE },
    { OPTION_XVUSECHROMAKEY,		"XvUseChromaKey",         OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_XVINSIDECHROMAKEY,		"XvInsideChromaKey",      OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_XVYUVCHROMAKEY,		"XvYUVChromaKey",         OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_XVDISABLECOLORKEY,		"XvDisableColorKey",      OPTV_BOOLEAN,   {0}, FALSE },
#endif
    { -1,                       	NULL,                     OPTV_NONE,      {0}, FALSE }
};

static int
SiSUSB_FIFT(const OptionInfoRec *options, int token)
{
    /* Find index from token */
    int i = 0;
    while(options[i].token >= 0) {
       if(options[i].token == token) return i;
       i++;
    }
    return 0; /* Should not happen */
}

#ifdef SIS_GLOBAL_ENABLEXV
static void
SiSUSB_PrintIlRange(ScrnInfoPtr pScrn, int token, int min, int max, UChar showhex)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    static const char *ilparmd = "Invalid parameter for \"%s\". Valid range is %d - %d\n";
    static const char *ilparmh = "Invalid parameter for \"%s\". Valid range is 0x%x - 0x%x\n";

    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
        showhex ? ilparmh : ilparmd,
	pSiSUSB->Options[SiSUSB_FIFT(pSiSUSB->Options, token)].name, min, max);
}

static Bool
SiSUSB_StrIsBoolOff(char *strptr)
{
    if( (!xf86NameCmp(strptr,"off"))   ||
        (!xf86NameCmp(strptr,"false")) ||
        (!xf86NameCmp(strptr,"no"))    ||
        (!xf86NameCmp(strptr,"0")) ) return TRUE;
    return FALSE;
}

static Bool
SiSUSB_StrIsBoolOn(char *strptr)
{
    if( (!xf86NameCmp(strptr,"on"))   ||
        (!xf86NameCmp(strptr,"true")) ||
        (!xf86NameCmp(strptr,"yes"))  ||
        (!xf86NameCmp(strptr,"1")) ) return TRUE;
    return FALSE;
}
#endif

static Bool
SiSUSB_EvalOneOrThreeFloats(ScrnInfoPtr pScrn, int token, const char *myerror,
                         char *strptr, int *v1, int *v2, int *v3)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    float val1 = 0.0, val2 = 0.0, val3 = 0.0;
    Bool valid = FALSE;
    int result = sscanf(strptr, "%f %f %f", &val1, &val2, &val3);
    if(result == 1) {
       if((val1 >= 0.1) && (val1 <= 10.0)) {
          valid = TRUE;
  	  *v1 = *v2 = *v3 = (int)(val1 * 1000);
       }
    } else if(result == 3) {
       if((val1 >= 0.1) && (val1 <= 10.0) &&
	  (val2 >= 0.1) && (val2 <= 10.0) &&
	  (val3 >= 0.1) && (val3 <= 10.0)) {
	  valid = TRUE;
	  *v1 = (int)(val1 * 1000);
	  *v2 = (int)(val2 * 1000);
	  *v3 = (int)(val3 * 1000);
       }
    }
    if(!valid) {
       xf86DrvMsg(pScrn->scrnIndex, X_WARNING, myerror,
                  pSiSUSB->Options[SiSUSB_FIFT(pSiSUSB->Options, token)].name);
    }
    return (valid);
}

static Bool
SiSUSB_EvalOneOrThreeFloats2(ScrnInfoPtr pScrn, int token, const char *myerror,
                         char *strptr, float *v1, float *v2, float *v3)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    float val1 = 0.0, val2 = 0.0, val3 = 0.0;
    Bool valid = FALSE;
    int result = sscanf(strptr, "%f %f %f", &val1, &val2, &val3);
    if(result == 1) {
       if((val1 >= -1.0) && (val1 <= 1.0)) {
	  valid = TRUE;
	  *v1 = *v2 = *v3 = val1;
       }
    } else if(result == 3) {
       if((val1 >= -1.0) && (val1 <= 1.0) &&
	  (val2 >= -1.0) && (val2 <= 1.0) &&
	  (val3 >= -1.0) && (val3 <= 1.0)) {
	  valid = TRUE;
	  *v1 = val1;
	  *v2 = val2;
	  *v3 = val3;
       }
    }
    if(!valid) {
       xf86DrvMsg(pScrn->scrnIndex, X_WARNING, myerror,
                  pSiSUSB->Options[SiSUSB_FIFT(pSiSUSB->Options, token)].name);
    }
    return (valid);
}

void
SiSUSBOptions(ScrnInfoPtr pScrn)
{
    SISUSBPtr      pSiSUSB = SISUSBPTR(pScrn);
    MessageType from;
    char        *strptr;
    int         ival;
    static const char *disabledstr= "disabled";
    static const char *enabledstr = "enabled";
#ifdef SIS_GLOBAL_ENABLEXV
    static const char *gammaopt   = "%s expects either a boolean, or 1 or 3 real numbers (0.1 - 10.0)\n";
#endif
    static const char *briopt     = "%s expects one or three real numbers (0.1 - 10.0)\n";
    static const char *newbriopt  = "%s expects one or three real numbers (-1.0 - 1.0)\n";
    Bool        val;

    /* Collect all of the relevant option flags (fill in pScrn->options) */
    xf86CollectOptions(pScrn, NULL);

    /* Process the options */
    if(!(pSiSUSB->Options = xalloc(sizeof(SISUSBOptions)))) return;

    memcpy(pSiSUSB->Options, SISUSBOptions, sizeof(SISUSBOptions));

    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, pSiSUSB->Options);

    /* Set defaults */

#ifdef SISVRAMQ
    /* TODO: Option (315 series VRAM command queue) */
    /* But beware: sisusbfb does not know about this!!! */
    pSiSUSB->cmdQueueSize = 512*1024;
#endif
    pSiSUSB->HWCursor = TRUE;

    pSiSUSB->NoAccel = TRUE;	/* ! */
    pSiSUSB->ShadowFB = TRUE;	/* ! */

    pSiSUSB->timeout = 0;

    pSiSUSB->VESA = -1;
#ifdef SIS_GLOBAL_ENABLEXV
    pSiSUSB->NoXvideo = FALSE;
#else
    pSiSUSB->NoXvideo = TRUE;
#endif
    pSiSUSB->maxxfbmem = 0;
    pSiSUSB->DSTN = FALSE;
    pSiSUSB->FSTN = FALSE;
    pSiSUSB->XvOnCRT2 = FALSE;
    pSiSUSB->noInternalModes = FALSE;
    pSiSUSB->restorebyset = TRUE;
    pSiSUSB->CRT1gamma = TRUE;
    pSiSUSB->CRT1gammaGiven = FALSE;

    pSiSUSB->XvGamma = FALSE;
    pSiSUSB->XvGammaGiven = FALSE;
    pSiSUSB->enablesisctrl = FALSE;
    pSiSUSB->XvDefBri = 10;
    pSiSUSB->XvDefCon = 2;
    pSiSUSB->XvDefHue = 0;
    pSiSUSB->XvDefSat = 0;
    pSiSUSB->XvDefDisableGfx = FALSE;
    pSiSUSB->XvDefDisableGfxLR = FALSE;
    pSiSUSB->XvUseChromaKey = FALSE;
    pSiSUSB->XvDisableColorKey = FALSE;
    pSiSUSB->XvInsideChromaKey = FALSE;
    pSiSUSB->XvYUVChromaKey = FALSE;
    pSiSUSB->XvChromaMin = 0x000101fe;
    pSiSUSB->XvChromaMax = 0x000101ff;
    pSiSUSB->XvGammaRed = pSiSUSB->XvGammaGreen = pSiSUSB->XvGammaBlue =
          pSiSUSB->XvGammaRedDef = pSiSUSB->XvGammaGreenDef = pSiSUSB->XvGammaBlueDef = 1000;
    pSiSUSB->GammaBriR = pSiSUSB->GammaBriG = pSiSUSB->GammaBriB = 1000;
    pSiSUSB->NewGammaBriR = pSiSUSB->NewGammaBriG = pSiSUSB->NewGammaBriB = 0.0;
    pSiSUSB->NewGammaConR = pSiSUSB->NewGammaConG = pSiSUSB->NewGammaConB = 0.0;

    pSiSUSB->HideHWCursor = FALSE;
    pSiSUSB->HWCursorIsVisible = FALSE;

#if XF86_VERSION_CURRENT < XF86_VERSION_NUMERIC(4,2,99,0,0)
    pSiSUSB->OptUseColorCursor = 0;
#else
    pSiSUSB->OptUseColorCursor = 1;
#endif

    /* Collect the options */

    /* DisconnectTimeout
     *
     * 0 = forever, -1 = abort server immediately
     * otherwise: re-probe for n seconds
     *
     */
    from = X_DEFAULT;
    if(xf86GetOptValInteger(pSiSUSB->Options, OPTION_DISCONNTIMEOUT, &ival)) {
       if(ival < 0) pSiSUSB->timeout = -1;
       else pSiSUSB->timeout = (ival > 32768) ? 32768 : ival;
       from = X_CONFIG;
    }
    switch(pSiSUSB->timeout) {
    case -1:
       xf86DrvMsg(pScrn->scrnIndex, from,
		"Device disconnection will abort X server\n");
       break;
    case 0:
       xf86DrvMsg(pScrn->scrnIndex, from,
		"Device will be re-probed forever after disconnection\n");
       break;
    default:
       xf86DrvMsg(pScrn->scrnIndex, from,
		"Device will be re-probed for %d seconds after disconnection\n",
		pSiSUSB->timeout);
       break;
    }

    /* SWCursor, HWCursor
     * Chooses whether to use the hardware or software cursor
     */
    from = X_DEFAULT;
    if(xf86GetOptValBool(pSiSUSB->Options, OPTION_HW_CURSOR, &pSiSUSB->HWCursor)) {
       from = X_CONFIG;
    }
    if(xf86ReturnOptValBool(pSiSUSB->Options, OPTION_SW_CURSOR, FALSE)) {
       from = X_CONFIG;
       pSiSUSB->HWCursor = FALSE;
       pSiSUSB->OptUseColorCursor = 0;
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Using %s cursor\n",
                                pSiSUSB->HWCursor ? "HW" : "SW");

    /*
     * UseColorHWCursor
     * Enable/disable color hardware cursor
     *
     */
#if XF86_VERSION_CURRENT >= XF86_VERSION_NUMERIC(4,2,99,0,0)
#ifdef ARGB_CURSOR
#ifdef SIS_ARGB_CURSOR
    if(pSiSUSB->HWCursor) {
       from = X_DEFAULT;
       if(xf86GetOptValBool(pSiSUSB->Options, OPTION_USERGBCURSOR, &pSiSUSB->OptUseColorCursor)) {
    	  from = X_CONFIG;
       }
       xf86DrvMsg(pScrn->scrnIndex, from, "Color HW cursor is %s\n",
                    pSiSUSB->OptUseColorCursor ? enabledstr : disabledstr);
    }
#endif
#endif
#endif

    /* RestoreBySetMode
     * Set this to force the driver to set the old mode instead of restoring
     * the register contents. This can be used to overcome problems with
     * LCD panels and video bridges.
     */
    if(xf86GetOptValBool(pSiSUSB->Options, OPTION_RESTOREBYSET, &val)) {
       pSiSUSB->restorebyset = val ? TRUE : FALSE;
    }

    /* EnableSiSCtrl */
    /* Allow sisctrl tool to change driver settings */
    from = X_DEFAULT;
    if(xf86GetOptValBool(pSiSUSB->Options, OPTION_ENABLESISCTRL, &val)) {
       if(val) pSiSUSB->enablesisctrl = TRUE;
       from = X_CONFIG;
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "SiSCtrl utility interface is %s\n",
  	pSiSUSB->enablesisctrl ? enabledstr : disabledstr);


    /* CRT1Gamma - enable/disable gamma correction for CRT1
     */
    if(xf86GetOptValBool(pSiSUSB->Options, OPTION_CRT1GAMMA, &val)) {
       pSiSUSB->CRT1gamma = val;
       pSiSUSB->CRT1gammaGiven = TRUE;
    }

    /* NoInternalModes (300/315/330 series only)
     * Don't use that.
     */
    if(xf86GetOptValBool(pSiSUSB->Options, OPTION_NOINTERNALMODES, &pSiSUSB->noInternalModes)) {
       if(pSiSUSB->noInternalModes) {
	  xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Usage of built-in modes is %s\n", disabledstr);
       }
    }

    /* NoXVideo
     * Set this to TRUE to disable Xv hardware video acceleration
     */
#ifdef SIS_GLOBAL_ENABLEXV
    if(!pSiSUSB->NoXvideo) {
       if(xf86ReturnOptValBool(pSiSUSB->Options, OPTION_NOXVIDEO, FALSE)) {
          pSiSUSB->NoXvideo = TRUE;
          xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "XVideo extension (Xv) disabled\n");
       }

       if(!pSiSUSB->NoXvideo) {

	  /* Some Xv properties' defaults can be set by options */
          if(xf86GetOptValInteger(pSiSUSB->Options, OPTION_XVDEFCONTRAST, &ival)) {
             if((ival >= 0) && (ival <= 7)) pSiSUSB->XvDefCon = ival;
             else SiSUSB_PrintIlRange(pScrn, OPTION_XVDEFCONTRAST, 0, 7, 0);
          }
          if(xf86GetOptValInteger(pSiSUSB->Options, OPTION_XVDEFBRIGHTNESS, &ival)) {
             if((ival >= -128) && (ival <= 127)) pSiSUSB->XvDefBri = ival;
             else SiSUSB_PrintIlRange(pScrn, OPTION_XVDEFBRIGHTNESS, -128, 127, 0);
          }

          if(xf86GetOptValInteger(pSiSUSB->Options, OPTION_XVDEFHUE, &ival)) {
             if((ival >= -8) && (ival <= 7)) pSiSUSB->XvDefHue = ival;
             else SiSUSB_PrintIlRange(pScrn, OPTION_XVDEFHUE, -8, 7, 0);
          }
          if(xf86GetOptValInteger(pSiSUSB->Options, OPTION_XVDEFSATURATION, &ival)) {
             if((ival >= -7) && (ival <= 7)) pSiSUSB->XvDefSat = ival;
             else SiSUSB_PrintIlRange(pScrn, OPTION_XVDEFSATURATION, -7, 7, 0);
          }

	  if(xf86GetOptValBool(pSiSUSB->Options, OPTION_XVDEFDISABLEGFX, &val)) {
	     if(val) pSiSUSB->XvDefDisableGfx = TRUE;
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
	        "Graphics display will be %s during Xv usage\n",
		val ? disabledstr : enabledstr);
          }

	  if(xf86GetOptValBool(pSiSUSB->Options, OPTION_XVDEFDISABLEGFXLR, &val)) {
	     if(val) pSiSUSB->XvDefDisableGfxLR = TRUE;
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "Graphics display left/right of overlay will be %s during Xv usage\n",
		   val ? disabledstr : enabledstr);
          }
	  if(xf86GetOptValBool(pSiSUSB->Options, OPTION_XVDISABLECOLORKEY, &val)) {
	     if(val) pSiSUSB->XvDisableColorKey = TRUE;
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "Xv Color key is %s\n",
		   val ? disabledstr : enabledstr);
          }
	  if(xf86GetOptValBool(pSiSUSB->Options, OPTION_XVUSECHROMAKEY, &val)) {
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "Xv Chroma-keying is %s\n",
		   val ? enabledstr : disabledstr);
	     if(val) pSiSUSB->XvUseChromaKey = TRUE;
          }
	  if(xf86GetOptValBool(pSiSUSB->Options, OPTION_XVINSIDECHROMAKEY, &val)) {
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "Xv: Video is transparent if %s chroma key range\n",
		   val ? "inside" : "outside");
	     if(val) pSiSUSB->XvInsideChromaKey = TRUE;
          }
	  if(xf86GetOptValInteger(pSiSUSB->Options, OPTION_XVCHROMAMIN, &ival)) {
             if((ival >= 0) && (ival <= 0xffffff)) pSiSUSB->XvChromaMin = ival;
             else SiSUSB_PrintIlRange(pScrn, OPTION_XVCHROMAMIN, 0, 0xffffff, 1);
          }
	  if(xf86GetOptValInteger(pSiSUSB->Options, OPTION_XVCHROMAMAX, &ival)) {
             if((ival >= 0) && (ival <= 0xffffff)) pSiSUSB->XvChromaMax = ival;
             else SiSUSB_PrintIlRange(pScrn, OPTION_XVCHROMAMAX, 0, 0xffffff, 1);
          }

	  /* XvGamma - enable/disable gamma correction for Xv
	   * Supported for CRT1 only
           */
	  if((strptr = (char *)xf86GetOptValString(pSiSUSB->Options, OPTION_XVGAMMA))) {
             if(SiSUSB_StrIsBoolOff(strptr)) {
		pSiSUSB->XvGamma = FALSE;
		pSiSUSB->XvGammaGiven = TRUE;
	     } else if(SiSUSB_StrIsBoolOn(strptr)) {
	        pSiSUSB->XvGamma = pSiSUSB->XvGammaGiven = TRUE;
             } else {
		if(SiSUSB_EvalOneOrThreeFloats(pScrn, OPTION_XVGAMMA, gammaopt, strptr,
		   		&pSiSUSB->XvGammaRed, &pSiSUSB->XvGammaGreen, &pSiSUSB->XvGammaBlue)) {
		   pSiSUSB->XvGamma = pSiSUSB->XvGammaGiven = TRUE;
		   pSiSUSB->XvGammaRedDef = pSiSUSB->XvGammaRed;
		   pSiSUSB->XvGammaGreenDef = pSiSUSB->XvGammaGreen;
		   pSiSUSB->XvGammaBlue = pSiSUSB->XvGammaBlue;
		}
	     }
          }

       }
    }
#endif

    {
       Bool GotNewBri = FALSE;
       if((strptr = (char *)xf86GetOptValString(pSiSUSB->Options, OPTION_NEWSTOREDCON))) {
	  SiSUSB_EvalOneOrThreeFloats2(pScrn, OPTION_NEWSTOREDCON, newbriopt, strptr,
		&pSiSUSB->NewGammaConR, &pSiSUSB->NewGammaConG, &pSiSUSB->NewGammaConB);
	  GotNewBri = TRUE;
       }
       if((strptr = (char *)xf86GetOptValString(pSiSUSB->Options, OPTION_NEWSTOREDBRI))) {
	  SiSUSB_EvalOneOrThreeFloats2(pScrn, OPTION_NEWSTOREDBRI, newbriopt, strptr,
		&pSiSUSB->NewGammaBriR, &pSiSUSB->NewGammaBriG, &pSiSUSB->NewGammaBriB);
	  GotNewBri = TRUE;
       }
       if(!GotNewBri) {
          if((strptr = (char *)xf86GetOptValString(pSiSUSB->Options, OPTION_STOREDBRI))) {
             SiSUSB_EvalOneOrThreeFloats(pScrn, OPTION_STOREDBRI, briopt, strptr,
		  &pSiSUSB->GammaBriR, &pSiSUSB->GammaBriG, &pSiSUSB->GammaBriB);
	     pSiSUSB->SiS_SD3_Flags |= SiS_SD3_OLDGAMMAINUSE;
	  }
       }
    }

}

const OptionInfoRec *
SISUSBAvailableOptions(int chipid, int busid)
{
    return SISUSBOptions;
}
