/* $XFree86$ */
/* $XdotOrg$ */
/*
 * SiS driver option evaluation
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
 * Authors:  	Thomas Winischhofer <thomas@winischhofer.net>
 *              ?
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sis.h"

#include "xf86str.h"
#include "xf86Cursor.h"

typedef enum {
    OPTION_SW_CURSOR,
    OPTION_HW_CURSOR,
    OPTION_ACCEL,
    OPTION_ACCELMETHOD,
    OPTION_TURBOQUEUE,
    OPTION_FAST_VRAM,
    OPTION_HOSTBUS,
    OPTION_RENDER,
    OPTION_FORCE_CRT1TYPE,
    OPTION_FORCE_CRT2TYPE,
    OPTION_YPBPRAR,
    OPTION_SHADOW_FB,
    OPTION_DRI,
    OPTION_AGP_SIZE,
    OPTION_ROTATE,
    OPTION_REFLECT,
    OPTION_XVIDEO,
    OPTION_VESA,
    OPTION_MAXXFBMEM,
    OPTION_FORCECRT1,
    OPTION_XVONCRT2,
    OPTION_PDC,
    OPTION_PDCA,
    OPTION_EMI,
    OPTION_TVSTANDARD,
    OPTION_USEROMDATA,
    OPTION_INTERNALMODES,
    OPTION_USEOEM,
    OPTION_YV12,
    OPTION_CHTVOVERSCAN,
    OPTION_CHTVSOVERSCAN,
    OPTION_CHTVLUMABANDWIDTHCVBS,
    OPTION_CHTVLUMABANDWIDTHSVIDEO,
    OPTION_CHTVLUMAFLICKERFILTER,
    OPTION_CHTVCHROMABANDWIDTH,
    OPTION_CHTVCHROMAFLICKERFILTER,
    OPTION_CHTVCVBSCOLOR,
    OPTION_CHTVTEXTENHANCE,
    OPTION_CHTVCONTRAST,
    OPTION_SISTVEDGEENHANCE,
    OPTION_SISTVANTIFLICKER,
    OPTION_SISTVSATURATION,
    OPTION_SISTVCHROMAFILTER,
    OPTION_SISTVLUMAFILTER,
    OPTION_SISTVCOLCALIBFINE,
    OPTION_SISTVCOLCALIBCOARSE,
    OPTION_TVXPOSOFFSET,
    OPTION_TVYPOSOFFSET,
    OPTION_TVXSCALE,
    OPTION_TVYSCALE,
    OPTION_SIS6326ANTIFLICKER,
    OPTION_SIS6326ENABLEYFILTER,
    OPTION_SIS6326YFILTERSTRONG,
    OPTION_SIS6326FORCETVPPLUG,
    OPTION_SIS6326FSCADJUST,
    OPTION_CHTVTYPE,
    OPTION_USERGBCURSOR,
    OPTION_USERGBCURSORBLEND,
    OPTION_USERGBCURSORBLENDTH,
    OPTION_RESTOREBYSET,
    OPTION_DDCFORCRT2,
    OPTION_FORCECRT2REDETECTION,
    OPTION_SENSEYPBPR,
    OPTION_CRT1GAMMA,
    OPTION_CRT2GAMMA,
    OPTION_XVGAMMA,
    OPTION_XVDEFCONTRAST,
    OPTION_XVDEFBRIGHTNESS,
    OPTION_XVDEFHUE,
    OPTION_XVDEFSATURATION,
    OPTION_XVDEFDISABLEGFX,
    OPTION_XVDEFDISABLEGFXLR,
    OPTION_XVMEMCPY,
    OPTION_XVBENCHCPY,
#ifndef SISCHECKOSSSE
    OPTION_XVSSECOPY,
#endif
    OPTION_XVUSECHROMAKEY,
    OPTION_XVCHROMAMIN,
    OPTION_XVCHROMAMAX,
    OPTION_XVDISABLECOLORKEY,
    OPTION_XVINSIDECHROMAKEY,
    OPTION_XVYUVCHROMAKEY,
    OPTION_XVDEFAULTADAPTOR,
    OPTION_SCALELCD,
    OPTION_CENTERLCD,
    OPTION_SPECIALTIMING,
    OPTION_LVDSHL,
    OPTION_PRGB,
    OPTION_ENABLEHOTKEY,
    OPTION_MERGEDFB,
    OPTION_MERGEDFBAUTO,
    OPTION_CRT2HSYNC,
    OPTION_CRT2VREFRESH,
    OPTION_CRT2POS,
    OPTION_METAMODES,
    OPTION_SISXINERAMA,
    OPTION_CRT2ISSCRN0,
    OPTION_MERGEDDPI,
    OPTION_MERGEDFBNONRECT,
    OPTION_MERGEDFBMOUSER,
    OPTION_ENABLESISCTRL,
    OPTION_STOREDBRI,
    OPTION_STOREDBRI2,
    OPTION_NEWSTOREDBRI,
    OPTION_NEWSTOREDBRI2,
    OPTION_NEWSTOREDCON,
    OPTION_NEWSTOREDCON2,
    OPTION_CRT1SATGAIN,
    OPTION_OVERRULERANGES,
    OPTION_FORCE1ASPECT,
    OPTION_FORCE2ASPECT,
    OPTION_TVBLUE,
#ifdef SIS_CP
    SIS_CP_OPT_OPTIONS
#endif
    OPTION_PSEUDO
} SISOpts;

static const OptionInfoRec SISOptions[] = {
    { OPTION_ACCEL,			"Accel",			OPTV_BOOLEAN,	{0}, FALSE },
#if defined(SIS_USE_XAA) && defined(SIS_USE_EXA)
    { OPTION_ACCELMETHOD,		"AccelMethod",			OPTV_STRING,	{0}, FALSE },
#endif
    { OPTION_TURBOQUEUE,		"TurboQueue",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_FAST_VRAM,			"FastVram",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_HOSTBUS,			"HostBus",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_RENDER,			"RenderAcceleration",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_FORCE_CRT1TYPE,		"ForceCRT1Type",		OPTV_STRING,	{0}, FALSE },
    { OPTION_FORCE_CRT2TYPE,		"ForceCRT2Type",		OPTV_STRING,	{0}, FALSE },
    { OPTION_SHADOW_FB,			"ShadowFB",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_DRI,			"DRI",				OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_AGP_SIZE,			"AGPSize",			OPTV_INTEGER,	{0}, FALSE },
    { OPTION_AGP_SIZE,			"GARTSize",			OPTV_INTEGER,	{0}, FALSE },
    { OPTION_VESA,			"Vesa",				OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_MAXXFBMEM,			"MaxXFBMem",			OPTV_INTEGER,	{0}, FALSE },
    { OPTION_ENABLESISCTRL,		"EnableSiSCtrl",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_SW_CURSOR,			"SWCursor",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_HW_CURSOR,			"HWCursor",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_USERGBCURSOR, 		"UseColorHWCursor",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_ROTATE,			"Rotate",			OPTV_STRING,	{0}, FALSE },
    { OPTION_REFLECT,			"Reflect",			OPTV_STRING,	{0}, FALSE },
    { OPTION_XVIDEO,			"Xvideo",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_INTERNALMODES,		"InternalModes",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_OVERRULERANGES,		"OverruleFrequencyRanges",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_RESTOREBYSET,		"RestoreBySetMode", 		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_FORCECRT1,			"ForceCRT1",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_XVONCRT2,			"XvOnCRT2",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_PDC,			"PanelDelayCompensation",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_PDC,			"PDC", 				OPTV_INTEGER,	{0}, FALSE },
    { OPTION_PDC,			"PanelDelayCompensation2",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_PDC,			"PDC2", 			OPTV_INTEGER,	{0}, FALSE },
    { OPTION_PDCA,			"PanelDelayCompensation1",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_PDCA,			"PDC1",				OPTV_INTEGER,	{0}, FALSE },
    { OPTION_EMI,			"EMI", 				OPTV_INTEGER,	{0}, FALSE },
    { OPTION_LVDSHL,			"LVDSHL", 			OPTV_INTEGER,	{0}, FALSE },
    { OPTION_PRGB,			"ForcePanelRGB",		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_SPECIALTIMING,		"SpecialTiming",		OPTV_STRING,	{0}, FALSE },
    { OPTION_TVSTANDARD,		"TVStandard",			OPTV_STRING,	{0}, FALSE },
    { OPTION_USEROMDATA,		"UseROMData",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_USEOEM, 			"UseOEMData",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_YV12, 			"YV12",				OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CHTVTYPE,			"CHTVType",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CHTVOVERSCAN,		"CHTVOverscan",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CHTVSOVERSCAN,		"CHTVSuperOverscan",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CHTVLUMABANDWIDTHCVBS,	"CHTVLumaBandwidthCVBS",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_CHTVLUMABANDWIDTHSVIDEO,	"CHTVLumaBandwidthSVIDEO",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_CHTVLUMAFLICKERFILTER,	"CHTVLumaFlickerFilter",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_CHTVCHROMABANDWIDTH,	"CHTVChromaBandwidth",		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_CHTVCHROMAFLICKERFILTER,	"CHTVChromaFlickerFilter",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_CHTVCVBSCOLOR,		"CHTVCVBSColor",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CHTVTEXTENHANCE,		"CHTVTextEnhance",		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_CHTVCONTRAST,		"CHTVContrast",			OPTV_INTEGER,	{0}, FALSE },
    { OPTION_SISTVEDGEENHANCE,		"SISTVEdgeEnhance",		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_SISTVANTIFLICKER,		"SISTVAntiFlicker",		OPTV_STRING,	{0}, FALSE },
    { OPTION_SISTVSATURATION,		"SISTVSaturation",		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_SISTVCHROMAFILTER,		"SISTVCFilter",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_SISTVLUMAFILTER,		"SISTVYFilter",	  		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_SISTVCOLCALIBFINE,		"SISTVColorCalibFine",		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_SISTVCOLCALIBCOARSE,	"SISTVColorCalibCoarse",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_TVXSCALE,			"SISTVXScale", 	  		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_TVYSCALE,			"SISTVYScale", 	  		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_TVXPOSOFFSET,		"TVXPosOffset", 		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_TVYPOSOFFSET,		"TVYPosOffset", 		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_SIS6326ANTIFLICKER,	"SIS6326TVAntiFlicker",		OPTV_STRING,	{0}, FALSE },
    { OPTION_SIS6326ENABLEYFILTER,	"SIS6326TVEnableYFilter",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_SIS6326YFILTERSTRONG,	"SIS6326TVYFilterStrong",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_SIS6326FORCETVPPLUG,	"SIS6326TVForcePlug",		OPTV_STRING,	{0}, FALSE },
    { OPTION_SIS6326FSCADJUST,		"SIS6326FSCAdjust",		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_YPBPRAR,			"YPbPrAspectRatio",		OPTV_STRING,	{0}, FALSE },
    { OPTION_TVBLUE,			"TVBlueWorkAround",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_USERGBCURSORBLEND,		"ColorHWCursorBlending",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_USERGBCURSORBLENDTH,	"ColorHWCursorBlendThreshold",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_DDCFORCRT2,		"CRT2Detection", 		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_FORCECRT2REDETECTION,	"ForceCRT2ReDetection",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_SENSEYPBPR,		"SenseYPbPr",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CRT1GAMMA,			"CRT1Gamma",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CRT2GAMMA,			"CRT2Gamma",			OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_STOREDBRI,			"GammaBrightness",		OPTV_STRING,	{0}, FALSE },
    { OPTION_STOREDBRI2,		"GammaBrightnessCRT2",		OPTV_STRING,	{0}, FALSE },
    { OPTION_STOREDBRI2,		"CRT2GammaBrightness",		OPTV_STRING,	{0}, FALSE },
    { OPTION_NEWSTOREDBRI,		"Brightness",			OPTV_STRING,	{0}, FALSE },
    { OPTION_NEWSTOREDBRI,		"NewGammaBrightness",		OPTV_STRING,	{0}, FALSE },
    { OPTION_NEWSTOREDBRI2,		"CRT2Brightness",		OPTV_STRING,	{0}, FALSE },
    { OPTION_NEWSTOREDBRI2,		"CRT2NewGammaBrightness",	OPTV_STRING,	{0}, FALSE },
    { OPTION_NEWSTOREDCON,		"Contrast",			OPTV_STRING,	{0}, FALSE },
    { OPTION_NEWSTOREDCON,		"NewGammaContrast",		OPTV_STRING,	{0}, FALSE },
    { OPTION_NEWSTOREDCON2,		"CRT2Contrast",			OPTV_STRING,	{0}, FALSE },
    { OPTION_NEWSTOREDCON2,		"CRT2NewGammaContrast",		OPTV_STRING,	{0}, FALSE },
    { OPTION_CRT1SATGAIN,		"CRT1Saturation", 		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_XVGAMMA,			"XvGamma", 	  		OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_XVDEFCONTRAST,		"XvDefaultContrast", 		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_XVDEFBRIGHTNESS,		"XvDefaultBrightness",		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_XVDEFHUE,			"XvDefaultHue",			OPTV_INTEGER,	{0}, FALSE },
    { OPTION_XVDEFSATURATION,		"XvDefaultSaturation",		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_XVDEFDISABLEGFX,		"XvDefaultDisableGfx",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_XVDEFDISABLEGFXLR,		"XvDefaultDisableGfxLR",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_XVCHROMAMIN,		"XvChromaMin",	  		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_XVCHROMAMAX,		"XvChromaMax", 	  		OPTV_INTEGER,	{0}, FALSE },
    { OPTION_XVUSECHROMAKEY,		"XvUseChromaKey",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_XVINSIDECHROMAKEY,		"XvInsideChromaKey",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_XVYUVCHROMAKEY,		"XvYUVChromaKey",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_XVDISABLECOLORKEY,		"XvDisableColorKey",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_XVMEMCPY,			"XvUseMemcpy",  		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_XVBENCHCPY,		"BenchmarkMemcpy",		OPTV_BOOLEAN,	{0}, FALSE },
#ifndef SISCHECKOSSSE
    { OPTION_XVSSECOPY, 		"UseSSE",  	  		OPTV_BOOLEAN,	{0}, FALSE },
#endif
    { OPTION_XVDEFAULTADAPTOR,		"XvDefaultAdaptor",		OPTV_STRING,	{0}, FALSE },
    { OPTION_SCALELCD,			"ScaleLCD",	   		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CENTERLCD,			"CenterLCD",	   		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_ENABLEHOTKEY,		"EnableHotkey",	   		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_FORCE1ASPECT,		"ForceCRT1VGAAspect",		OPTV_STRING,	{0}, FALSE },
    { OPTION_FORCE2ASPECT,		"ForceCRT2VGAAspect",		OPTV_STRING,	{0}, FALSE },
#ifdef SISMERGED
    { OPTION_MERGEDFB,			"MergedFB",			OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_MERGEDFB,			"TwinView",			OPTV_ANYSTR,	{0}, FALSE },	/* alias */
    { OPTION_MERGEDFBAUTO,		"MergedFBAuto",			OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CRT2HSYNC,			"CRT2HSync",			OPTV_STRING,	{0}, FALSE },
    { OPTION_CRT2HSYNC,			"SecondMonitorHorizSync",	OPTV_STRING,	{0}, FALSE },   /* alias */
    { OPTION_CRT2VREFRESH,		"CRT2VRefresh",			OPTV_STRING,	{0}, FALSE },
    { OPTION_CRT2VREFRESH,		"SecondMonitorVertRefresh", 	OPTV_STRING,	{0}, FALSE },   /* alias */
    { OPTION_CRT2POS,			"CRT2Position",			OPTV_STRING,	{0}, FALSE },
    { OPTION_CRT2POS,			"TwinViewOrientation",		OPTV_STRING,	{0}, FALSE },   /* alias */
    { OPTION_METAMODES,			"MetaModes",  			OPTV_STRING,	{0}, FALSE },
    { OPTION_MERGEDDPI,			"MergedDPI", 			OPTV_STRING,	{0}, FALSE },
#ifdef SISXINERAMA
    { OPTION_SISXINERAMA,		"MergedXinerama",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_SISXINERAMA,		"TwinviewXineramaInfo",		OPTV_BOOLEAN,	{0}, FALSE },   /* alias */
    { OPTION_CRT2ISSCRN0,		"MergedXineramaCRT2IsScreen0",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_MERGEDFBNONRECT,		"MergedNonRectangular",		OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_MERGEDFBMOUSER,		"MergedMouseRestriction",	OPTV_BOOLEAN,	{0}, FALSE },
#endif
#endif
#ifdef SIS_CP
    SIS_CP_OPTION_DETAIL
#endif
    { -1,				NULL,				OPTV_NONE,	{0}, FALSE }
};

static int
SiS_FIFT(const OptionInfoRec *options, int token)
{
    /* Find index from token */
    int i = 0;
    while(options[i].token >= 0) {
       if(options[i].token == token) return i;
       i++;
    }
    return 0; /* Should not happen */
}

static void
SiS_PrintBadOpt(ScrnInfoPtr pScrn, char *strptr, int token)
{
    SISPtr pSiS = SISPTR(pScrn);

    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	"\"%s\" is is not a valid parameter for option \"%s\"\n",
	strptr, pSiS->Options[SiS_FIFT(pSiS->Options, token)].name);
}

static void
SiS_PrintIlRange(ScrnInfoPtr pScrn, int token, int min, int max, UChar showhex)
{
    SISPtr pSiS = SISPTR(pScrn);
    static const char *ilparmd = "Invalid parameter for \"%s\". Valid range is %d - %d\n";
    static const char *ilparmh = "Invalid parameter for \"%s\". Valid range is 0x%x - 0x%x\n";

    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	showhex ? ilparmh : ilparmd,
	pSiS->Options[SiS_FIFT(pSiS->Options, token)].name, min, max);
}

#ifdef SISDUALHEAD
static void
SiS_PrintOverruleDHM(ScrnInfoPtr pScrn, int token1, int token2)
{
    SISPtr pSiS = SISPTR(pScrn);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	"\"%s\" overrules \"%s\" in CRT2 (Master) device section\n",
	pSiS->Options[SiS_FIFT(pSiS->Options, token1)].name,
	pSiS->Options[SiS_FIFT(pSiS->Options, token2)].name);
}
#endif

static Bool
SiS_StrIsBoolOn(char *strptr)
{
    if( (*strptr == '\0')             ||
	(!xf86NameCmp(strptr,"on"))   ||
	(!xf86NameCmp(strptr,"true")) ||
	(!xf86NameCmp(strptr,"yes"))  ||
	(!xf86NameCmp(strptr,"1")) ) return TRUE;
    return FALSE;
}

static Bool
SiS_StrIsBoolOff(char *strptr)
{
    if( (!xf86NameCmp(strptr,"off"))   ||
	(!xf86NameCmp(strptr,"false")) ||
	(!xf86NameCmp(strptr,"no"))    ||
	(!xf86NameCmp(strptr,"0")) ) return TRUE;
    return FALSE;
}

static Bool
SiS_EvalOneOrThreeFloats(ScrnInfoPtr pScrn, int token, const char *myerror,
                         char *strptr, int *v1, int *v2, int *v3)
{
    SISPtr pSiS = SISPTR(pScrn);
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
                  pSiS->Options[SiS_FIFT(pSiS->Options, token)].name);
    }
    return (valid);
}

static Bool
SiS_EvalOneOrThreeFloats2(ScrnInfoPtr pScrn, int token, const char *myerror,
                         char *strptr, float *v1, float *v2, float *v3)
{
    SISPtr pSiS = SISPTR(pScrn);
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
                  pSiS->Options[SiS_FIFT(pSiS->Options, token)].name);
    }
    return (valid);
}

void
SiSOptions(ScrnInfoPtr pScrn)
{
    SISPtr      pSiS = SISPTR(pScrn);
    MessageType from;
    char        *strptr;
    int         ival;
    static const char *baddhm     = "Option \"%s\" ignored in Dual Head mode\n";
    static const char *validparm  = "Valid parameters are";
    static const char *disabledstr= "disabled";
    static const char *enabledstr = "enabled";
    static const char *gammaopt   = "%s expects either a boolean, or 1 or 3 real numbers (0.1 - 10.0)\n";
    static const char *briopt     = "%s expects 1 or 3 real numbers (0.1 - 10.0)\n";
    static const char *newbriopt     = "%s expects 1 or 3 real numbers (-1.0 - 1.0)\n";
    Bool        val, IsDHM = FALSE;
    Bool	IsSecondHead = FALSE;

    /* Collect all of the relevant option flags (fill in pScrn->options) */
    xf86CollectOptions(pScrn, NULL);

    /* Process the options */
    if(!(pSiS->Options = xalloc(sizeof(SISOptions)))) return;

    memcpy(pSiS->Options, SISOptions, sizeof(SISOptions));

    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, pSiS->Options);

    /* Set defaults */

    pSiS->newFastVram = -1;
    pSiS->HostBus = TRUE;
    pSiS->TurboQueue = TRUE;
#ifdef SISVRAMQ
    /* TODO: Option (315 series VRAM command queue) */
    /* But beware: sisfb does not know about this!!! */
    pSiS->cmdQueueSize = 512*1024;
    if(pSiS->ChipType == XGI_20) {
       /* Hardware maximum on Z7: 128k */
       pSiS->cmdQueueSize = 128*1024;
    }
#endif
    pSiS->doRender = TRUE;
    pSiS->HWCursor = TRUE;
    pSiS->Rotate = 0;
    pSiS->Reflect = 0;
    pSiS->NoAccel = FALSE;
#if (defined(SIS_USE_EXA) && defined(SIS_USE_XAA)) || !defined(SIS_USE_EXA)
    pSiS->useEXA = FALSE;
#else
    pSiS->useEXA = TRUE;
#endif
    pSiS->ShadowFB = FALSE;
    pSiS->loadDRI = FALSE;
#ifdef XF86DRI
    pSiS->agpWantedPages = AGP_PAGES;
#endif
    pSiS->VESA = -1;
    pSiS->NoXvideo = FALSE;
    pSiS->maxxfbmem = 0;
    pSiS->forceCRT1 = -1;
    pSiS->DSTN = FALSE;
    pSiS->FSTN = FALSE;
    pSiS->XvOnCRT2 = FALSE;
    pSiS->NoYV12 = -1;
    pSiS->PDC = -1;
    pSiS->PDCA = -1;
    pSiS->EMI = -1;
    pSiS->PRGB = -1;
    pSiS->OptTVStand = -1;
    pSiS->OptROMUsage = -1;
    pSiS->noInternalModes = FALSE;
    pSiS->OptUseOEM = -1;
    pSiS->OptTVOver = -1;
    pSiS->OptTVSOver = -1;
    pSiS->chtvlumabandwidthcvbs = -1;
    pSiS->chtvlumabandwidthsvideo = -1;
    pSiS->chtvlumaflickerfilter = -1;
    pSiS->chtvchromabandwidth = -1;
    pSiS->chtvchromaflickerfilter = -1;
    pSiS->chtvcvbscolor = -1;
    pSiS->chtvtextenhance = -1;
    pSiS->chtvcontrast = -1;
    pSiS->sistvedgeenhance = -1;
    pSiS->sistvantiflicker = -1;
    pSiS->sistvsaturation = -1;
    pSiS->sistvcfilter = -1;
    pSiS->sistvyfilter = 1; /* 0 = off, 1 = default, 2-8 = filter no */
    pSiS->sistvcolcalibc = 0;
    pSiS->sistvcolcalibf = 0;
    pSiS->sis6326enableyfilter = -1;
    pSiS->sis6326yfilterstrong = -1;
    pSiS->sis6326tvplug = -1;
    pSiS->sis6326fscadjust = 0;
    pSiS->tvxpos = 0;
    pSiS->tvypos = 0;
    pSiS->tvxscale = 0;
    pSiS->tvyscale = 0;
    pSiS->siscrt1satgain = 0;
    pSiS->crt1satgaingiven = FALSE;
    pSiS->NonDefaultPAL = pSiS->NonDefaultNTSC = -1;
    pSiS->chtvtype = -1;
    pSiS->restorebyset = TRUE;
    pSiS->nocrt2ddcdetection = FALSE;
    pSiS->forcecrt2redetection = TRUE;
    pSiS->SenseYPbPr = TRUE;
    pSiS->ForceCRT1Type = CRT1_VGA;
    pSiS->CRT1TypeForced = FALSE;
    pSiS->ForceCRT2Type = CRT2_DEFAULT;
    pSiS->ForceYPbPrAR = TV_YPBPR169;
    pSiS->ForceTVType = -1;
    pSiS->CRT1gamma = TRUE;
    pSiS->CRT1gammaGiven = FALSE;
    pSiS->CRT2gamma = TRUE;
    pSiS->XvGamma = FALSE;
    pSiS->XvGammaGiven = FALSE;
    pSiS->enablesisctrl = FALSE;
    if((pSiS->VGAEngine == SIS_300_VGA) || (pSiS->VGAEngine == SIS_315_VGA)) {
       pSiS->XvDefBri = 10;
       pSiS->XvDefCon = 2;
    } else {
       pSiS->XvDefBri = 0;
       pSiS->XvDefCon = 4;
    }
    pSiS->XvDefHue = 0;
    pSiS->XvDefSat = 0;
    pSiS->XvDefDisableGfx = FALSE;
    pSiS->XvDefDisableGfxLR = FALSE;
    pSiS->XvDefAdaptorBlit = FALSE;
    pSiS->UsePanelScaler = -1;
    pSiS->CenterLCD = -1;
    pSiS->XvUseMemcpy = TRUE;
    pSiS->XvUseChromaKey = FALSE;
    pSiS->XvDisableColorKey = FALSE;
    pSiS->XvInsideChromaKey = FALSE;
    pSiS->XvYUVChromaKey = FALSE;
    pSiS->XvChromaMin = 0x000101fe;
    pSiS->XvChromaMax = 0x000101ff;
    pSiS->XvGammaRed = pSiS->XvGammaGreen = pSiS->XvGammaBlue =
          pSiS->XvGammaRedDef = pSiS->XvGammaGreenDef = pSiS->XvGammaBlueDef = 1000;
    pSiS->GammaBriR = pSiS->GammaBriG = pSiS->GammaBriB = 1000;
    pSiS->CRT2SepGamma = FALSE;
    pSiS->GammaR2 = pSiS->GammaG2 = pSiS->GammaB2 = 1.0;
    pSiS->GammaBriR2 = pSiS->GammaBriG2 = pSiS->GammaBriB2 = 1000;
    pSiS->NewGammaBriR = pSiS->NewGammaBriG = pSiS->NewGammaBriB = 0.0;
    pSiS->NewGammaConR = pSiS->NewGammaConG = pSiS->NewGammaConB = 0.0;
    pSiS->NewGammaBriR2 = pSiS->NewGammaBriG2 = pSiS->NewGammaBriB2 = 0.0;
    pSiS->NewGammaConR2 = pSiS->NewGammaConG2 = pSiS->NewGammaConB2 = 0.0;
    pSiS->HideHWCursor = FALSE;
    pSiS->HWCursorIsVisible = FALSE;
    pSiS->OverruleRanges = TRUE;
    pSiS->BenchMemCpy = TRUE;
#ifndef SISCHECKOSSSE
    pSiS->XvSSEMemcpy = FALSE;
#endif
#ifdef SISMERGED
    pSiS->MergedFB = pSiS->MergedFBAuto = FALSE;
    pSiS->CRT2Position = sisRightOf;
    pSiS->CRT2HSync = NULL;
    pSiS->CRT2VRefresh = NULL;
    pSiS->MetaModes = NULL;
    pSiS->MergedFBXDPI = pSiS->MergedFBYDPI = 0;
    pSiS->CRT1XOffs = pSiS->CRT1YOffs = pSiS->CRT2XOffs = pSiS->CRT2YOffs = 0;
    pSiS->NonRect = pSiS->HaveNonRect = pSiS->HaveOffsRegions = FALSE;
    pSiS->MBXNR1XMAX = pSiS->MBXNR1YMAX = pSiS->MBXNR2XMAX = pSiS->MBXNR2YMAX = 65536;
    pSiS->MouseRestrictions = TRUE;
#ifdef SISXINERAMA
    pSiS->UseSiSXinerama = TRUE;
    pSiS->CRT2IsScrn0 = FALSE;
#endif
#endif
#ifdef SIS_CP
    SIS_CP_OPT_DEFAULT
#endif

    /* Chipset dependent defaults */

    if(pSiS->Chipset == PCI_CHIP_SIS530) {
       /* TQ still broken on 530/620? */
       pSiS->TurboQueue = FALSE;
    }

    if(pSiS->Chipset == PCI_CHIP_SIS6326) {
       pSiS->newFastVram = 1;
    }

    if(pSiS->ChipType == SIS_315H ||
       pSiS->ChipType == SIS_315) {
       /* Cursor engine seriously broken */
       pSiS->HWCursor = FALSE;
    }

    if((pSiS->Chipset == PCI_CHIP_SIS550) ||
       (pSiS->Chipset == PCI_CHIP_XGIXG20)) {
       /* Alpha blending not supported */
       pSiS->doRender = FALSE;
    }

    if(pSiS->Chipset == PCI_CHIP_XGIXG20) {
       /* No video overlay, no video blitter */
       pSiS->NoXvideo = TRUE;
    }

    /* DRI only supported on 300 series,
     * so don't load DRI by default on
     * others.
     */
    if(pSiS->VGAEngine == SIS_300_VGA) {
       pSiS->loadDRI = TRUE;
    }

#if XF86_VERSION_CURRENT < XF86_VERSION_NUMERIC(4,2,99,0,0)
    pSiS->OptUseColorCursor = 0;
#else
    if(pSiS->VGAEngine == SIS_300_VGA) {
       pSiS->OptUseColorCursor = 0;
       pSiS->OptUseColorCursorBlend = 1;
       pSiS->OptColorCursorBlendThreshold = 0x37000000;
    } else if(pSiS->VGAEngine == SIS_315_VGA) {
       if(pSiS->Chipset == PCI_CHIP_XGIXG20) {
          /* No color HW cursor on Z7 */
          pSiS->OptUseColorCursor = 0;
       } else {
          pSiS->OptUseColorCursor = 1;
       }
    }
#endif

    if(pSiS->VGAEngine == SIS_300_VGA) {
       pSiS->AllowHotkey = 0;
    } else if(pSiS->VGAEngine == SIS_315_VGA) {
       pSiS->AllowHotkey = 1;
    }

    /* Collect the options */

    /* FastVRAM (5597/5598, 6326 and 530/620 only)
     */
    if((pSiS->VGAEngine == SIS_OLD_VGA) || (pSiS->VGAEngine == SIS_530_VGA)) {
       from = X_DEFAULT;
       if(xf86GetOptValBool(pSiS->Options, OPTION_FAST_VRAM, &pSiS->newFastVram)) {
          from = X_CONFIG;
       }
       xf86DrvMsg(pScrn->scrnIndex, from, "Fast VRAM timing %s\n",
		   (pSiS->newFastVram == -1) ?
			 ((pSiS->oldChipset == OC_SIS620) ? "enabled (for read only)" :
							    "enabled (for write only)") :
			 (pSiS->newFastVram ? "enabled (for read and write)" : disabledstr));
    }

    /* HostBus (5597/5598 only)
     */
    if(pSiS->Chipset == PCI_CHIP_SIS5597) {
       from = X_DEFAULT;
       if(xf86GetOptValBool(pSiS->Options, OPTION_HOSTBUS, &pSiS->HostBus)) {
          from = X_CONFIG;
       }
       xf86DrvMsg(pScrn->scrnIndex, from, "SiS5597/5598 VGA-to-CPU host bus %s\n",
                   pSiS->HostBus ? enabledstr : disabledstr);
    }

    /* MaxXFBMem
     * This options limits the amount of video memory X uses for screen
     * and off-screen buffers. This option should be used if using DRI
     * is intended. The kernel framebuffer driver required for DRM will
     * start its memory heap at 12MB if it detects more than 16MB, at 8MB if
     * between 8 and 16MB are available, otherwise at 4MB. So, if the amount
     * of memory X uses, a clash between the framebuffer's memory heap
     * and X is avoided. The amount is to be specified in KB.
     */
    if(xf86GetOptValInteger(pSiS->Options, OPTION_MAXXFBMEM, (int *)&pSiS->maxxfbmem)) {
       if(pSiS->maxxfbmem >= 2048) {
	  xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "MaxXFBMem: Framebuffer memory shall be limited to %d KB\n",
		    pSiS->maxxfbmem);
	  pSiS->maxxfbmem *= 1024;
       } else {
	  pSiS->maxxfbmem = 0;
	  xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "Invalid MaxXFBMem setting; minimum is 2048\n");
       }
    }

    /* Accel
     * Turns on/off 2D acceleration
     */
    if(!xf86ReturnOptValBool(pSiS->Options, OPTION_ACCEL, TRUE)) {
       pSiS->NoAccel = TRUE;
#if XF86_VERSION_CURRENT < XF86_VERSION_NUMERIC(4,2,99,0,0)
       pSiS->NoXvideo = TRUE;
       xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "2D Acceleration and Xv disabled\n");
#else
       xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "2D Acceleration disabled\n");
#endif
    }

#if defined(SIS_USE_XAA) && defined(SIS_USE_EXA)
    if(!pSiS->NoAccel) {
       from = X_DEFAULT;
       if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_ACCELMETHOD))) {
	  if(!xf86NameCmp(strptr,"XAA")) {
	     from = X_CONFIG;
	     pSiS->useEXA = FALSE;
	  } else if(!xf86NameCmp(strptr,"EXA")) {
	     from = X_CONFIG;
	     pSiS->useEXA = TRUE;
	  }
       }
       xf86DrvMsg(pScrn->scrnIndex, from, "Using %s acceleration architecture\n",
		pSiS->useEXA ? "EXA" : "XAA");
    }
#endif

    /* RenderAcceleration
     * En/Disables RENDER acceleration (315/330/340 series only, not 550, not XGI Z7)
     */
#ifdef SIS_USE_XAA
    if((pSiS->VGAEngine == SIS_315_VGA)   &&
       (pSiS->Chipset != PCI_CHIP_SIS550) &&
       (pSiS->Chipset != PCI_CHIP_XGIXG20) &&
       (!pSiS->NoAccel)) {
       if(xf86GetOptValBool(pSiS->Options, OPTION_RENDER, &pSiS->doRender)) {
	  if(!pSiS->doRender) {
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "RENDER Acceleration disabled\n");
	  }
       }
    }
#else
    pSiS->doRender = FALSE;
#endif

    /* SWCursor, HWCursor
     * Chooses whether to use the hardware or software cursor
     */
    from = X_DEFAULT;
    if(xf86GetOptValBool(pSiS->Options, OPTION_HW_CURSOR, &pSiS->HWCursor)) {
       from = X_CONFIG;
    }
    if(xf86ReturnOptValBool(pSiS->Options, OPTION_SW_CURSOR, FALSE)) {
       from = X_CONFIG;
       pSiS->HWCursor = FALSE;
       pSiS->OptUseColorCursor = 0;
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Using %s cursor\n",
                                pSiS->HWCursor ? "HW" : "SW");

    /*
     * UseColorHWCursor
     * ColorHWCursorBlending
     * ColorHWCursorBlendThreshold
     *
     * Enable/disable color hardware cursors;
     * enable/disable color hw cursor emulation for 300 series
     * select emultation transparency threshold for 300 series
     *
     */
#if XF86_VERSION_CURRENT >= XF86_VERSION_NUMERIC(4,2,99,0,0)
#ifdef ARGB_CURSOR
#ifdef SIS_ARGB_CURSOR
    if((pSiS->HWCursor) &&
       ((pSiS->VGAEngine == SIS_300_VGA) || (pSiS->VGAEngine == SIS_315_VGA)) &&
       (pSiS->Chipset != PCI_CHIP_XGIXG20)) {

       from = X_DEFAULT;
       if(xf86GetOptValBool(pSiS->Options, OPTION_USERGBCURSOR, &pSiS->OptUseColorCursor)) {
	  from = X_CONFIG;
       }
       xf86DrvMsg(pScrn->scrnIndex, from, "Color HW cursor is %s\n",
	            pSiS->OptUseColorCursor ? enabledstr : disabledstr);

       if(pSiS->VGAEngine == SIS_300_VGA) {
	  from = X_DEFAULT;
	  if(xf86GetOptValBool(pSiS->Options, OPTION_USERGBCURSORBLEND, &pSiS->OptUseColorCursorBlend)) {
	     from = X_CONFIG;
	  }
	  if(pSiS->OptUseColorCursor) {
	     xf86DrvMsg(pScrn->scrnIndex, from,
		"HW cursor color blending emulation is %s\n",
		(pSiS->OptUseColorCursorBlend) ? enabledstr : disabledstr);
	  }
	  from = X_DEFAULT;
	  if(xf86GetOptValInteger(pSiS->Options, OPTION_USERGBCURSORBLENDTH, &ival)) {
	     if((ival >= 0) && (ival <= 255)) {
		from = X_CONFIG;
		pSiS->OptColorCursorBlendThreshold = (ival << 24);
	     } else {
		ival = pSiS->OptColorCursorBlendThreshold >> 24;
		SiS_PrintIlRange(pScrn, OPTION_USERGBCURSORBLENDTH, 0, 255, 0);
	     }
	  } else {
	     ival = pSiS->OptColorCursorBlendThreshold >> 24;
          }
	  if(pSiS->OptUseColorCursor) {
	     if(pSiS->OptUseColorCursorBlend) {
		xf86DrvMsg(pScrn->scrnIndex, from,
		   "HW cursor color blending emulation threshold is %d\n", ival);
	     }
	  }
       }
    }
#endif
#endif
#endif

    /* OverruleFrequencyRanges
     * Enable/disable overruling bogus frequency ranges for TV and LCD(A)
     */
    if((pSiS->VGAEngine == SIS_300_VGA) || (pSiS->VGAEngine == SIS_315_VGA)) {
       if(xf86GetOptValBool(pSiS->Options, OPTION_OVERRULERANGES, &val)) {
	  if(!val) {
	     pSiS->OverruleRanges = FALSE;
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Overruling frequency ranges disabled\n");
	  }
       }
    }

#ifdef SISDUALHEAD
    if(pSiS->DualHeadMode) {
       IsDHM = TRUE;
       if(pSiS->SecondHead) IsSecondHead = TRUE;
    }
#endif

    /* MergedFB
     * Enable/disable and configure merged framebuffer mode
     */
#ifdef SISMERGED
#ifdef SISDUALHEAD
    if(pSiS->DualHeadMode) {
       if(xf86IsOptionSet(pSiS->Options, OPTION_MERGEDFB)) {
	  xf86DrvMsg(pScrn->scrnIndex, X_WARNING, baddhm,
	     pSiS->Options[SiS_FIFT(pSiS->Options, OPTION_MERGEDFB)].name);
       }
       if(xf86IsOptionSet(pSiS->Options, OPTION_MERGEDFBAUTO)) {
	  xf86DrvMsg(pScrn->scrnIndex, X_WARNING, baddhm,
	     pSiS->Options[SiS_FIFT(pSiS->Options, OPTION_MERGEDFBAUTO)].name);
       }
    } else
#endif
    if((pSiS->VGAEngine == SIS_300_VGA) || (pSiS->VGAEngine == SIS_315_VGA)) {
       if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_MERGEDFB))) {
	  if(SiS_StrIsBoolOn(strptr)) {
	     pSiS->MergedFB = TRUE;
	     pSiS->MergedFBAuto = FALSE;
	  } else if(!xf86NameCmp(strptr, "AUTO")) {
	     pSiS->MergedFB = TRUE;
	     pSiS->MergedFBAuto = TRUE;
	  }
       }
       if(xf86GetOptValBool(pSiS->Options, OPTION_MERGEDFBAUTO, &val)) {
	  if(!pSiS->MergedFB) {
	     if(val) pSiS->MergedFB = pSiS->MergedFBAuto = TRUE;
	  } else {
	     xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		"Option \"MergedFB\" overrules option \"MergedFBAuto\"\n");
	  }
       }

       if(pSiS->MergedFB) {
	  if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_CRT2POS))) {
	     int result;
	     Bool valid = FALSE;
	     char *tempstr = xalloc(strlen(strptr) + 1);
	     result = sscanf(strptr, "%s %d", tempstr, &ival);
	     if(result >= 1) {
		if(!xf86NameCmp(tempstr,"LeftOf")) {
		   pSiS->CRT2Position = sisLeftOf;
		   valid = TRUE;
		   if(result == 2) {
		      if(ival < 0) pSiS->CRT1YOffs = -ival;
		      else pSiS->CRT2YOffs = ival;
		   }
#ifdef SISXINERAMA
		   pSiS->CRT2IsScrn0 = TRUE;
#endif
		} else if(!xf86NameCmp(tempstr,"RightOf")) {
		   pSiS->CRT2Position = sisRightOf;
		   valid = TRUE;
		   if(result == 2) {
		      if(ival < 0) pSiS->CRT1YOffs = -ival;
		      else pSiS->CRT2YOffs = ival;
		   }
#ifdef SISXINERAMA
		   pSiS->CRT2IsScrn0 = FALSE;
#endif
		} else if(!xf86NameCmp(tempstr,"Above")) {
		   pSiS->CRT2Position = sisAbove;
		   valid = TRUE;
		   if(result == 2) {
		      if(ival < 0) pSiS->CRT1XOffs = -ival;
		      else pSiS->CRT2XOffs = ival;
		   }
#ifdef SISXINERAMA
		   pSiS->CRT2IsScrn0 = FALSE;
#endif
		} else if(!xf86NameCmp(tempstr,"Below")) {
		   pSiS->CRT2Position = sisBelow;
		   valid = TRUE;
		   if(result == 2) {
		      if(ival < 0) pSiS->CRT1XOffs = -ival;
		      else pSiS->CRT2XOffs = ival;
		   }
#ifdef SISXINERAMA
		   pSiS->CRT2IsScrn0 = TRUE;
#endif
		} else if(!xf86NameCmp(tempstr,"Clone")) {
		   pSiS->CRT2Position = sisClone;
		   if(result == 1) valid = TRUE;
#ifdef SISXINERAMA
		   pSiS->CRT2IsScrn0 = TRUE;
#endif
		}
	     }
	     if(!valid) {
		SiS_PrintBadOpt(pScrn, strptr, OPTION_CRT2POS);
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		    "%s \"RightOf\", \"LeftOf\", \"Above\", \"Below\", or \"Clone\"\n", validparm);
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		    "Except for \"Clone\", the parameter may be followed by an integer.\n");
	     }
	     xfree(tempstr);
	  }
	  if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_METAMODES))) {
	     pSiS->MetaModes = xalloc(strlen(strptr) + 1);
	     if(pSiS->MetaModes) memcpy(pSiS->MetaModes, strptr, strlen(strptr) + 1);
	  }
	  if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_CRT2HSYNC))) {
	     pSiS->CRT2HSync = xalloc(strlen(strptr) + 1);
	     if(pSiS->CRT2HSync) memcpy(pSiS->CRT2HSync, strptr, strlen(strptr) + 1);
	  }
	  if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_CRT2VREFRESH))) {
	     pSiS->CRT2VRefresh = xalloc(strlen(strptr) + 1);
	     if(pSiS->CRT2VRefresh) memcpy(pSiS->CRT2VRefresh, strptr, strlen(strptr) + 1);
	  }
	  if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_MERGEDDPI))) {
	     int val1 = 0, val2 = 0;
	     sscanf(strptr, "%d %d", &val1, &val2);
	     if(val1 && val2) {
		pSiS->MergedFBXDPI = val1;
		pSiS->MergedFBYDPI = val2;
	     } else {
		SiS_PrintBadOpt(pScrn, strptr, OPTION_MERGEDDPI);
	     }
	  }
#ifdef SISXINERAMA
	  if(pSiS->MergedFB) {
	     if(xf86GetOptValBool(pSiS->Options, OPTION_SISXINERAMA, &val)) {
		if(!val) pSiS->UseSiSXinerama = FALSE;
	     }
	     if(pSiS->UseSiSXinerama) {
		if(xf86GetOptValBool(pSiS->Options, OPTION_CRT2ISSCRN0, &val)) {
		   pSiS->CRT2IsScrn0 = val ? TRUE : FALSE;
		}
		if(xf86GetOptValBool(pSiS->Options, OPTION_MERGEDFBNONRECT, &val)) {
		   pSiS->NonRect = val ? TRUE : FALSE;
		}
		if(xf86GetOptValBool(pSiS->Options, OPTION_MERGEDFBMOUSER, &val)) {
		   pSiS->MouseRestrictions = val ? TRUE : FALSE;
		}
	     }
	  }
#endif
       }
    }
#endif

    /* Some options can only be specified in the Master Head's Device
     * section. Here we give the user a hint in the log.
     */
#ifdef SISDUALHEAD
    if((pSiS->DualHeadMode) && (pSiS->SecondHead)) {
       static const char *mystring = "Option \"%s\" only accepted in CRT2 (Master) Device section\n";
       int i;
       const short forbiddenopts[] = {
		OPTION_TURBOQUEUE, OPTION_RESTOREBYSET, OPTION_ENABLEHOTKEY,
		OPTION_ENABLESISCTRL, OPTION_USEROMDATA, OPTION_USEOEM,
		OPTION_FORCECRT1, OPTION_DDCFORCRT2, OPTION_FORCECRT2REDETECTION,
		OPTION_SENSEYPBPR, OPTION_FORCE_CRT1TYPE, OPTION_FORCE_CRT2TYPE,
		OPTION_YPBPRAR, OPTION_SCALELCD, OPTION_CENTERLCD, OPTION_PDC,
		OPTION_PDCA, OPTION_EMI, OPTION_SPECIALTIMING, OPTION_LVDSHL,
		OPTION_TVSTANDARD, OPTION_CHTVTYPE, OPTION_CHTVOVERSCAN,
		OPTION_CHTVSOVERSCAN, OPTION_CHTVLUMABANDWIDTHCVBS,
		OPTION_CHTVLUMABANDWIDTHSVIDEO, OPTION_CHTVLUMAFLICKERFILTER,
		OPTION_CHTVCHROMABANDWIDTH, OPTION_CHTVCHROMAFLICKERFILTER,
		OPTION_CHTVCVBSCOLOR, OPTION_CHTVTEXTENHANCE, OPTION_CHTVCONTRAST,
		OPTION_SISTVEDGEENHANCE, OPTION_SISTVANTIFLICKER, OPTION_SISTVSATURATION,
		OPTION_SISTVCHROMAFILTER, OPTION_SISTVLUMAFILTER, OPTION_SISTVCOLCALIBCOARSE,
		OPTION_SISTVCOLCALIBFINE, OPTION_TVXPOSOFFSET, OPTION_TVYPOSOFFSET,
		OPTION_TVXSCALE, OPTION_TVYSCALE, OPTION_TVBLUE, OPTION_CRT2GAMMA, OPTION_XVONCRT2,
		OPTION_XVDEFAULTADAPTOR, OPTION_XVMEMCPY, OPTION_XVBENCHCPY, OPTION_FORCE2ASPECT,
#if defined(SIS_USE_XAA) && defined(SIS_USE_EXA)
		OPTION_ACCELMETHOD,
#endif
#ifndef SISCHECKOSSSE
		OPTION_XVSSECOPY,
#endif
#ifdef SIS_CP
		SIS_CP_OPT_DH_WARN
#endif
		-1
       };

       i = 0;
       while(forbiddenopts[i] >= 0) {
	  if(xf86IsOptionSet(pSiS->Options, (int)forbiddenopts[i])) {
	     xf86DrvMsg(pScrn->scrnIndex, X_WARNING, mystring,
			pSiS->Options[SiS_FIFT(pSiS->Options, (int)forbiddenopts[i])].name);
	  }
	  i++;
       }

    } else
#endif
    {
       if(pSiS->VGAEngine == SIS_315_VGA) {

#ifdef SISVRAMQ
          xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using VRAM command queue, size %dk\n",
		pSiS->cmdQueueSize / 1024);
#else
	  xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using MMIO command queue, size 512k\n");
#endif

       } else {

	  /* TurboQueue */
	  from = X_DEFAULT;
	  if(xf86GetOptValBool(pSiS->Options, OPTION_TURBOQUEUE, &pSiS->TurboQueue)) {
	     from = X_CONFIG;
	  }
	  xf86DrvMsg(pScrn->scrnIndex, from, "TurboQueue %s\n",
		     pSiS->TurboQueue ? enabledstr : disabledstr);
       }

       if((pSiS->VGAEngine == SIS_300_VGA) || (pSiS->VGAEngine == SIS_315_VGA)) {

	  /* RestoreBySetMode (300/315/330 series only)
	   * Set this to force the driver to set the old mode instead of restoring
	   * the register contents. This can be used to overcome problems with
	   * LCD panels and video bridges.
	   */
	  if(xf86GetOptValBool(pSiS->Options, OPTION_RESTOREBYSET, &val)) {
	     pSiS->restorebyset = val ? TRUE : FALSE;
	  }

	  /* EnableHotkey (300/315/330 series and later only)
	   * Enables or disables the BIOS hotkey switch for
	   * switching the output device on laptops.
	   * This key causes a total machine hang on many 300 series
	   * machines, it is therefore by default disabled on such.
	   * In dual head mode, using the hotkey is lethal, so we
	   * forbid it then in any case.
	   * However, although the driver disables the hotkey as
	   * BIOS developers intented to do that, some buggy BIOSes
	   * still cause the machine to freeze. Hence the warning.
	   */
	  ival = 0;
	  from = X_DEFAULT;
#ifdef SISDUALHEAD
	  if(pSiS->DualHeadMode) {
	     pSiS->AllowHotkey = 0;
	     ival = 1;
	  } else
#endif
	  if(xf86GetOptValBool(pSiS->Options, OPTION_ENABLEHOTKEY, &val)) {
	     pSiS->AllowHotkey = val ? 1 : 0;
	     from = X_CONFIG;
	  }
	  xf86DrvMsg(pScrn->scrnIndex, from, "Hotkey display switching is %s%s\n",
		pSiS->AllowHotkey ? enabledstr : disabledstr,
		ival ? " in dual head mode" : "");
	  if(pSiS->Chipset == PCI_CHIP_SIS630 ||
	     pSiS->Chipset == PCI_CHIP_SIS650 ||
	     pSiS->Chipset == PCI_CHIP_SIS660) {
	     xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		 "WARNING: Using the Hotkey might freeze your machine, regardless\n");
	     xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		 "         whether enabled or disabled. This is no driver bug.\n");
	  }

	  /* UseROMData (300/315/330 series and later only)
	   * This option is enabling/disabling usage of some machine
	   * specific data from the BIOS ROM. This option can - and
	   * should - be used in case the driver makes problems
	   * because SiS changed the location of this data.
	   */
	  if(xf86GetOptValBool(pSiS->Options, OPTION_USEROMDATA, &val)) {
	     pSiS->OptROMUsage = val ? 1 : 0;
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		 "Video ROM data usage shall be %s\n",
		  val ? enabledstr : disabledstr);
	  }

	  /* UseOEMData (300/315/330 series and later only)
	   * The driver contains quite a lot data for OEM LCD panels
	   * and TV connector specifics which override the defaults.
	   * If this data is incorrect, the TV may lose color and
	   * the LCD panel might show some strange effects. Use this
	   * option to disable the usage of this data.
	   */
	  if(xf86GetOptValBool(pSiS->Options, OPTION_USEOEM, &val)) {
	     pSiS->OptUseOEM = val ? 1 : 0;
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		 "Internal CRT2 OEM data usage shall be %s\n",
		 val ? enabledstr : disabledstr);
	  }

	  /* CRT2DDCDetection (315/330 series and later only)
	   * If set to true, this disables CRT2 detection using DDC. This is
	   * to avoid problems with not entirely DDC compiant LCD panels or
	   * VGA monitors connected to the secondary VGA plug. Since LCD and
	   * VGA share the same DDC channel, it might in some cases be impossible
	   * to determine if the device is a CRT monitor or a flat panel.
	   */
	  if(xf86GetOptValBool(pSiS->Options, OPTION_DDCFORCRT2, &val)) {
	     pSiS->nocrt2ddcdetection = val ? FALSE : TRUE;
	  }

	  /* ForceCRT2ReDetection (315/330 series and later only)
	   * If set to true, it forces re-detection of the LCD panel and
	   * a secondary VGA connection even if the BIOS already had found
	   * about it. This is meant for custom panels (ie such with
	   * non-standard resolutions) which the BIOS will "detect" according
	   * to the established timings, resulting in only a very vague idea
	   * about the panels real resolution. As for secondary VGA, this
	   * enables us to include a Plasma panel's proprietary modes.
	   */
	  if(xf86GetOptValBool(pSiS->Options, OPTION_FORCECRT2REDETECTION, &val)) {
	     if(val) {
		pSiS->forcecrt2redetection = TRUE;
		pSiS->nocrt2ddcdetection = FALSE;
	     } else
	        pSiS->forcecrt2redetection = FALSE;
	  }

	  /* SenseYPbPr (315/330 series and later only)
	   * If set to true, the driver will sense for YPbPr TV. This is
	   * inconvenient for folks connecting SVideo and CVBS at the same
	   * time, because this condition will be detected as YPbPr (since
	   * the TV output pins are shared). "False" will not sense for
	   * YPbPr and detect SVideo or CVBS only.
	   */
	  if(xf86GetOptValBool(pSiS->Options, OPTION_SENSEYPBPR, &val)) {
	     if(val) pSiS->SenseYPbPr = TRUE;
	     else    pSiS->SenseYPbPr = FALSE;
	  }

	  /* ForceCRT1Type (315/330 series and later only)
	   * Used for forcing the driver to initialize CRT1 as
	   * VGA (analog) or LCDA (for simultanious LCD and TV
	   * display) - on M650/651 and 661 or later with 301C/30xLV only!
	   */
	  if(pSiS->VGAEngine == SIS_315_VGA) {
	     if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_FORCE_CRT1TYPE))) {
		if((!xf86NameCmp(strptr,"VGA")) || (!xf86NameCmp(strptr,"CRT"))) {
		   pSiS->ForceCRT1Type = CRT1_VGA;
		   pSiS->CRT1TypeForced = TRUE;
		} else if( (!xf86NameCmp(strptr,"LCD"))   ||
			   (!xf86NameCmp(strptr,"LCDA"))  ||
			   (!xf86NameCmp(strptr,"DVI-D")) ||
			   (!xf86NameCmp(strptr,"DVID"))  ||
			   (!xf86NameCmp(strptr,"DVI"))   ||
			   (!xf86NameCmp(strptr,"LCD-A")) ) {
		   pSiS->ForceCRT1Type = CRT1_LCDA;
		   pSiS->CRT1TypeForced = TRUE;
		} else if((!xf86NameCmp(strptr,"NONE")) || (!xf86NameCmp(strptr,"OFF"))) {
		   pSiS->ForceCRT1Type = CRT1_VGA;
		   pSiS->forceCRT1 = 0;
		   pSiS->CRT1TypeForced = TRUE;
		} else {
		   SiS_PrintBadOpt(pScrn, strptr, OPTION_FORCE_CRT1TYPE);
		   xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "%s \"VGA\" (alias \"CRT\"), \"LCD\" (alias \"DVI-D\") or NONE\n", validparm);
		}
	     }
	  }

	  /* ForceCRT1 (300/315/330 series and later only)
	   * This option can be used to force CRT1 (VGA) to be switched on/off. Its
	   * intention is mainly for old monitors that can't be detected
	   * automatically. This is only useful on machines with a video bridge.
	   * In normal cases, this option won't be necessary.
	   */
	  if(pSiS->ForceCRT1Type == CRT1_VGA) {
	     if(xf86GetOptValBool(pSiS->Options, OPTION_FORCECRT1, &val)) {
		pSiS->forceCRT1 = val ? 1 : 0;
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
			"CRT1 shall be forced %s\n",
			val ? "ON" : "OFF");
	     }
	  }

	  /* ForceCRT2Type (300/315/330 series and later only)
	   * Used for forcing the driver to use a given CRT2 device type.
	   * (SVIDEO, COMPOSITE and SCART for overriding detection)
	   */
	  strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_FORCE_CRT2TYPE);
	  if(strptr != NULL) {
	     if(!xf86NameCmp(strptr,"TV"))
		pSiS->ForceCRT2Type = CRT2_TV;
	     else if( (!xf86NameCmp(strptr,"SVIDEO")) ||
		      (!xf86NameCmp(strptr,"SVHS")) ) {
		pSiS->ForceCRT2Type = CRT2_TV;
		pSiS->ForceTVType = TV_SVIDEO;
             } else if( (!xf86NameCmp(strptr,"COMPOSITE")) ||
			(!xf86NameCmp(strptr,"CVBS")) ) {
		pSiS->ForceCRT2Type = CRT2_TV;
		pSiS->ForceTVType = TV_AVIDEO;
	     } else if( (!xf86NameCmp(strptr,"COMPOSITE SVIDEO")) || /* Ugly, but shorter than a parsing function */
			(!xf86NameCmp(strptr,"COMPOSITE+SVIDEO")) ||
			(!xf86NameCmp(strptr,"SVIDEO+COMPOSITE")) ||
			(!xf86NameCmp(strptr,"SVIDEO COMPOSITE")) ) {
		pSiS->ForceCRT2Type = CRT2_TV;
		pSiS->ForceTVType = (TV_SVIDEO | TV_AVIDEO);
	     } else if(!xf86NameCmp(strptr,"SCART")) {
		pSiS->ForceCRT2Type = CRT2_TV;
		pSiS->ForceTVType = TV_SCART;
	     } else if((!xf86NameCmp(strptr,"LCD")) || (!xf86NameCmp(strptr,"DVI-D"))) {
		if(pSiS->ForceCRT1Type == CRT1_VGA) {
		   pSiS->ForceCRT2Type = CRT2_LCD;
		} else {
		   pSiS->ForceCRT2Type = 0;
		   xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		      "Can't set both CRT1 and CRT2 type to LCD; CRT2 disabled\n");
		}
	     } else if((!xf86NameCmp(strptr,"VGA")) ||
		       (!xf86NameCmp(strptr,"DVI-A")) ||
		       (!xf86NameCmp(strptr,"CRT"))) {
		if(pSiS->ForceCRT1Type == CRT1_VGA) {
		   pSiS->ForceCRT2Type = CRT2_VGA;
		} else {
		   pSiS->ForceCRT2Type = 0;
		   xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		      "CRT2 can only be TV or off while CRT1 is LCD; CRT2 disabled\n");
		}
	     } else if(!xf86NameCmp(strptr,"NONE"))
		pSiS->ForceCRT2Type = 0;
	     else if((!xf86NameCmp(strptr,"DSTN")) && (pSiS->Chipset == PCI_CHIP_SIS550)) {
		if(pSiS->ForceCRT1Type == CRT1_VGA) {
		   pSiS->ForceCRT2Type = CRT2_LCD;
		   pSiS->DSTN = TRUE;
		}
	     } else if((!xf86NameCmp(strptr,"FSTN")) && (pSiS->Chipset == PCI_CHIP_SIS550)) {
		if(pSiS->ForceCRT1Type == CRT1_VGA) {
		   pSiS->ForceCRT2Type = CRT2_LCD;
		   pSiS->FSTN = TRUE;
		}
#ifdef ENABLE_YPBPR
	     } else if(!xf86NameCmp(strptr,"HIVISION")) {
		pSiS->ForceCRT2Type = CRT2_TV;
		pSiS->ForceTVType = TV_HIVISION;
	     } else if((!xf86NameCmp(strptr,"YPBPR1080I")) && (pSiS->VGAEngine == SIS_315_VGA)) {
		pSiS->ForceCRT2Type = CRT2_TV;
		pSiS->ForceTVType = TV_YPBPR;
		pSiS->ForceYPbPrType = TV_YPBPR1080I;
	     } else if(((!xf86NameCmp(strptr,"YPBPR525I")) || (!xf86NameCmp(strptr,"YPBPR480I"))) &&
		       (pSiS->VGAEngine == SIS_315_VGA)) {
		pSiS->ForceCRT2Type = CRT2_TV;
		pSiS->ForceTVType = TV_YPBPR;
		pSiS->ForceYPbPrType = TV_YPBPR525I;
	     } else if(((!xf86NameCmp(strptr,"YPBPR525P")) || (!xf86NameCmp(strptr,"YPBPR480P"))) &&
		       (pSiS->VGAEngine == SIS_315_VGA)) {
		pSiS->ForceCRT2Type = CRT2_TV;
		pSiS->ForceTVType = TV_YPBPR;
		pSiS->ForceYPbPrType = TV_YPBPR525P;
	     } else if(((!xf86NameCmp(strptr,"YPBPR625I")) || (!xf86NameCmp(strptr,"YPBPR576I"))) &&
		       (pSiS->VGAEngine == SIS_315_VGA)) {
		pSiS->ForceCRT2Type = CRT2_TV;
		pSiS->ForceTVType = TV_YPBPR;
		pSiS->ForceYPbPrType = TV_YPBPR625I;
	     } else if(((!xf86NameCmp(strptr,"YPBPR625P")) || (!xf86NameCmp(strptr,"YPBPR576P"))) &&
		       (pSiS->VGAEngine == SIS_315_VGA)) {
		pSiS->ForceCRT2Type = CRT2_TV;
		pSiS->ForceTVType = TV_YPBPR;
		pSiS->ForceYPbPrType = TV_YPBPR625P;
	     } else if(((!xf86NameCmp(strptr,"YPBPR750P")) || (!xf86NameCmp(strptr,"YPBPR720P"))) &&
	               (pSiS->VGAEngine == SIS_315_VGA)) {
		pSiS->ForceCRT2Type = CRT2_TV;
		pSiS->ForceTVType = TV_YPBPR;
		pSiS->ForceYPbPrType = TV_YPBPR750P;
#endif
	     } else {
		SiS_PrintBadOpt(pScrn, strptr, OPTION_FORCE_CRT2TYPE);
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		    "%s \"LCD\" (=\"DVI-D\"), \"TV\", \"SVIDEO\", \"COMPOSITE\",\n"
		    "\t\"SVIDEO+COMPOSITE\", \"SCART\", \"VGA\" (=\"DVI-A\") or \"NONE\"; on the SiS550\n"
		    "\talso \"DSTN\" and \"FSTN\""
#ifdef ENABLE_YPBPR
		    				"; on SiS 301/301B bridges also \"HIVISION\", and on\n"
		    "\tSiS315/330/340 series with 301C/30xLV bridge also \"YPBPR480I\", \"YPBPR480P\",\n"
		    "\t\"YPBPR576I\", \"YPBPR576P\", \"YPBPR720P\" and \"YPBPR1080I\""
#endif
		    "\n", validparm);
	     }

	     if(pSiS->ForceCRT2Type != CRT2_DEFAULT)
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		    "CRT2 type shall be %s\n", strptr);
	  }

	  if(pSiS->ForceTVType == TV_YPBPR) {
	     strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_YPBPRAR);
	     if(strptr != NULL) {
		if(!xf86NameCmp(strptr,"4:3LB"))
		   pSiS->ForceYPbPrAR = TV_YPBPR43LB;
		else if(!xf86NameCmp(strptr,"4:3"))
		   pSiS->ForceYPbPrAR = TV_YPBPR43;
		else if(!xf86NameCmp(strptr,"16:9"))
		   pSiS->ForceYPbPrAR = TV_YPBPR169;
		else {
		   SiS_PrintBadOpt(pScrn, strptr, OPTION_YPBPRAR);
		   xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"%s \"4:3LB\", \"4:3\" and \"16:9\"\n", validparm);
		}
	     }
	  }

	  strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_SPECIALTIMING);
	  if(strptr != NULL) {
	     int i = 0;
	     Bool found = FALSE;
	     if(!xf86NameCmp(strptr,"NONE")) {
		pSiS->SiS_Pr->SiS_CustomT = CUT_FORCENONE;
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
			"Special timing disabled\n");
	     } else {
	        while(SiS_customttable[i].chipID != 0) {
		   if(!xf86NameCmp(strptr,SiS_customttable[i].optionName)) {
		      pSiS->SiS_Pr->SiS_CustomT = SiS_customttable[i].SpecialID;
		      found = TRUE;
		      xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
			  "Special timing for %s %s forced\n",
			  SiS_customttable[i].vendorName, SiS_customttable[i].cardName);
		      break;
		   }
		   i++;
		}
		if(!found) {
		   SiS_PrintBadOpt(pScrn, strptr, OPTION_SPECIALTIMING);
		   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "%s:\n", validparm);
		   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "\t\"NONE\" (to disable special timings)\n");
		   i = 0;
		   while(SiS_customttable[i].chipID != 0) {
		      xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
				"\t\"%s\" (for %s %s)\n",
				SiS_customttable[i].optionName,
				SiS_customttable[i].vendorName,
				SiS_customttable[i].cardName);
		      i++;
		   }
		}
	     }
	  }

	  /* EnableSiSCtrl */
	  /* Allow sisctrl tool to change driver settings */
	  from = X_DEFAULT;
	  if(xf86GetOptValBool(pSiS->Options, OPTION_ENABLESISCTRL, &val)) {
	     if(val) pSiS->enablesisctrl = TRUE;
	     from = X_CONFIG;
          }
	  xf86DrvMsg(pScrn->scrnIndex, from, "SiSCtrl utility interface is %s\n",
		pSiS->enablesisctrl ? enabledstr : disabledstr);

	  if((from == X_DEFAULT) && (!pSiS->enablesisctrl)) {
	     xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"For information on SiSCtrl, see\n\t\thttp://www.winischhofer.at/linuxsispart1.shtml#sisctrl\n");
	  }


	  /* ForceCRT1Aspect, ForceCRT2Aspect */
	  /* Make driver believe that a connected CRT/VGA device is 4:3 ("normal")
	   * or 16:9 ("wide"). Note: This affects only for real VGA (analog)
	   * output devices, not TV or DVI/LCD.
	   */
	  if(pSiS->VGAEngine == SIS_315_VGA) {
	     strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_FORCE2ASPECT);
	     if(strptr != NULL) {
		if(!xf86NameCmp(strptr,"WIDE")) {
		   pSiS->SiS_Pr->SiS_UseWideCRT2 = TRUE;
		} else if(!xf86NameCmp(strptr,"NORMAL")) {
		   pSiS->SiS_Pr->SiS_UseWideCRT2 = FALSE;
		} else {
		   SiS_PrintBadOpt(pScrn, strptr, OPTION_FORCE2ASPECT);
		   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "%s:\n", validparm);
		   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "\t\"NORMAL\" or \"WIDE\"\n");
		}
		/* Print status later */
	     }
	  }

	 /* ScaleLCD (300/315/330 series and later only)
	  * Can be used to force the bridge/panel link to [do|not do] the
	  * scaling of modes lower than the panel's native resolution.
	  * Setting this to TRUE will force the bridge/panel link
	  * to scale; FALSE will rely on the panel's capabilities.
	  * Not supported on all machines.
	  */
	  if(xf86GetOptValBool(pSiS->Options, OPTION_SCALELCD, &val)) {
	     pSiS->UsePanelScaler = val ? 0 : 1;
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "LCD scaling is %s\n",
	         pSiS->UsePanelScaler ? disabledstr : enabledstr);
	  }

	 /* CenterLCD (300/315/330/later + SiS video bridge only)
	  * If LCD shall not be scaled, this selects whether 1:1 data
	  * will be sent to the output, or the image shall be centered
	  * on the LCD. For LVDS panels, screen will always be centered,
	  * since these have no built-in scaler. For TMDS, this is
	  * selectable. Non-centered means that the driver will pass
	  * 1:1 data to the output and that the panel will have to
	  * scale by itself (if supported by the panel).
	  */
	  if(xf86GetOptValBool(pSiS->Options, OPTION_CENTERLCD, &val)) {
	     pSiS->CenterLCD = val ? 1 : 0;
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Non-scaled LCD output will %sbe centered\n",
	         pSiS->CenterLCD ? "not " : "");
	  }

	 /* PanelDelayCompensation (300/315/330 series and later only)
	  * This might be required if the LCD panel shows "small waves"
	  * or wrong colors.
	  * The parameter is an integer, (on 300 series usually either
	  * 4, 32 or 24; on 315 series + LV bridge usually 3 or 51)
	  * Why this option? Simply because SiS did poor BIOS design.
	  * The PDC value depends on the very LCD panel used in a
	  * particular machine. For most panels, the driver is able
	  * to detect the correct value. However, some panels require
	  * a different setting. For 300 series, the value given must
	  * be within the mask 0x3c. For 661 and later, if must be
	  * within the range of 0 to 31.
	  */
	  ival = -1;
	  xf86GetOptValInteger(pSiS->Options, OPTION_PDC, &ival);
	  if(ival != -1) {
	     pSiS->PDC = ival;
	     if((pSiS->VGAEngine == SIS_300_VGA) && (pSiS->PDC & ~0x3c)) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		    "Invalid PanelDelayCompensation parameter\n");
		pSiS->PDC = -1;
	     } else {
		if(pSiS->VGAEngine == SIS_315_VGA) pSiS->PDC &= 0x1f;
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		    "Panel delay compensation shall be %d (for LCD=CRT2)\n",
		     pSiS->PDC);
	     }
          }

	 /* PanelDelayCompensation1 (315/330 series and later only)
	  * Same as above, but for LCD-via-CRT1 ("LCDA")
	  */
	  if(pSiS->VGAEngine == SIS_315_VGA) {
	     ival = -1;
	     xf86GetOptValInteger(pSiS->Options, OPTION_PDCA, &ival);
	     if(ival != -1) {
		pSiS->PDCA = ival;
		if(pSiS->PDCA > 0x1f) {
		   SiS_PrintIlRange(pScrn, OPTION_PDCA, 0, 31, 0);
		   pSiS->PDCA = -1;
		} else {
		   xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
			"Panel delay compensation shall be %d (for LCD=CRT1)\n",
			pSiS->PDCA);
		}
	     }
	  }

	 /* LVDSHL (300/315/330/later series + 30xLV bridge only)
	  * This might be required if the LCD panel is too dark.
	  * The parameter is an integer from 0 to 3.
	  */
	  if(xf86GetOptValInteger(pSiS->Options, OPTION_LVDSHL, &pSiS->SiS_Pr->LVDSHL)) {
	     if((pSiS->SiS_Pr->LVDSHL < 0) || (pSiS->SiS_Pr->LVDSHL > 3)) {
		SiS_PrintIlRange(pScrn, OPTION_LVDSHL, 0, 3, 0);
		pSiS->SiS_Pr->LVDSHL = -1;
	     } else {
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		     "LVDSHL will be %d\n",
		     pSiS->SiS_Pr->LVDSHL);
	     }
          }

	 /* EMI (315/330/later series + 302LV/302ELV bridge only)
	  * This might be required if the LCD panel loses sync on
	  * mode switches. So far, this problem should not show up
	  * due to the auto-detection (from reading the values set
	  * by the BIOS; however, the BIOS values are wrong sometimes
	  * such as in the case of some Compal machines with a
	  * 1400x1050, or some Inventec(Compaq) machines with a
	  * 1280x1024 panel.
	  * The parameter is an integer from 0 to 0x60ffffff.
	  */
	  if(xf86GetOptValInteger(pSiS->Options, OPTION_EMI, &pSiS->EMI)) {
	     if((pSiS->EMI < 0) || (pSiS->EMI > 0x60ffffff)) {
		SiS_PrintIlRange(pScrn, OPTION_LVDSHL, 0, 0x60ffffff, 1);
		pSiS->EMI = -1;
	     } else {
		pSiS->EMI &= 0x60ffffff;
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		     "EMI will be 0x%04x\n", pSiS->EMI);
	     }
	  }

	 /* TVBlueWorkAround (315/later series only)
	  * TRUE and FALSE are two ways to work around a "blue shade" on
	  * TV output. This work-around is disabled by not setting the
	  * option. 315 series + 301B-DH only.
	  */
	  if(pSiS->VGAEngine == SIS_315_VGA) {
	     if(xf86GetOptValBool(pSiS->Options, OPTION_TVBLUE, &val)) {
	     	pSiS->SiS_Pr->SiS_TVBlue = val ? 1 : 0;
	     }
	  }

	 /* ForcePanelRGB (300/315/330 series and later only)
	  * Can be used to force the bridge/panel link to assume a
	  * specified LCD color capability of 18 or 24 bit in cases
	  * where the BIOS carries incorrect information (such as in
	  * the case of the MSI m250).
	  */
	  if(xf86GetOptValInteger(pSiS->Options, OPTION_PRGB, &pSiS->PRGB)) {
	     if((pSiS->PRGB != 18 && pSiS->PRGB != 24)) {
		pSiS->PRGB = -1;
		SiS_PrintBadOpt(pScrn, strptr, OPTION_PRGB);
	     } else {
		   xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
			"LCD panel color depth is %d\n",
			pSiS->PRGB);
	     }
	  }

       }


       /* TVStandard (300/315/330/later series and 6326 w/ TV only)
	* This option is for overriding the autodetection of
	* the BIOS/Jumper option for PAL / NTSC
	*/
       if((pSiS->VGAEngine == SIS_300_VGA) ||
	  (pSiS->VGAEngine == SIS_315_VGA) ||
	  ((pSiS->Chipset == PCI_CHIP_SIS6326) && (pSiS->SiS6326Flags & SIS6326_HASTV))) {
	  strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_TVSTANDARD);
	  if(strptr != NULL) {
	     if(!xf86NameCmp(strptr,"PAL"))
		pSiS->OptTVStand = 1;
	     else if((!xf86NameCmp(strptr,"PALM")) ||
		     (!xf86NameCmp(strptr,"PAL-M"))) {
		pSiS->OptTVStand = 1;
		pSiS->NonDefaultPAL = 1;
	     } else if((!xf86NameCmp(strptr,"PALN")) ||
		       (!xf86NameCmp(strptr,"PAL-N"))) {
		pSiS->OptTVStand = 1;
		pSiS->NonDefaultPAL = 0;
	     } else if((!xf86NameCmp(strptr,"NTSCJ")) ||
		       (!xf86NameCmp(strptr,"NTSC-J"))) {
		pSiS->OptTVStand = 0;
		pSiS->NonDefaultNTSC = 1;
	     } else if(!xf86NameCmp(strptr,"NTSC"))
		pSiS->OptTVStand = 0;
	     else {
		SiS_PrintBadOpt(pScrn, strptr, OPTION_TVSTANDARD);
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		    "%s \"PAL\", \"PALM\", \"PALN\", \"NTSC\", \"NTSCJ\"\n", validparm);
	     }

	     if(pSiS->OptTVStand != -1) {
		static const char *tvstdstr = "TV standard shall be %s\n";
		if(pSiS->Chipset == PCI_CHIP_SIS6326) {
		   pSiS->NonDefaultPAL = -1;
		   pSiS->NonDefaultNTSC = -1;
		   xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, tvstdstr,
			pSiS->OptTVStand ? "PAL" : "NTSC");
		} else {
		   xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, tvstdstr,
		       (pSiS->OptTVStand ?
			   ( (pSiS->NonDefaultPAL == -1) ? "PAL" :
			      ((pSiS->NonDefaultPAL) ? "PALM" : "PALN") ) :
				(pSiS->NonDefaultNTSC == -1) ? "NTSC" : "NTSCJ"));
		}
	     }
	  }
       }

       /* CHTVType  (315/330/later series + Chrontel only)
	* Used for telling the driver if the TV output shall
	* be 525i YPbPr or SCART.
	*/
       if(pSiS->VGAEngine == SIS_315_VGA) {
	  strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_CHTVTYPE);
	  if(strptr != NULL) {
	     if(!xf86NameCmp(strptr,"SCART"))
		pSiS->chtvtype = 1;
	     else if(!xf86NameCmp(strptr,"YPBPR525I"))
		pSiS->chtvtype = 0;
	     else {
		SiS_PrintBadOpt(pScrn, strptr, OPTION_CHTVTYPE);
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		  "%s \"SCART\" or \"YPBPR525I\"\n", validparm);
	     }
	     if(pSiS->chtvtype != -1)
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		  "Chrontel: TV type shall be %s\n", strptr);
	  }
       }

       /* CHTVOverscan (300/315/330 series and later only)
	* CHTVSuperOverscan (300/315/330 series and later only)
	* These options are for overriding the BIOS option for
	* TV Overscan. Some BIOSes don't even have such an option.
	* SuperOverscan is only supported with PAL.
	* Both options are only effective on machines with a
	* CHRONTEL TV encoder. SuperOverscan is only available
	* on the 700x.
	*/
       if((pSiS->VGAEngine == SIS_300_VGA) || (pSiS->VGAEngine == SIS_315_VGA)) {
	  if(xf86GetOptValBool(pSiS->Options, OPTION_CHTVOVERSCAN, &val)) {
	     pSiS->OptTVOver = val ? 1 : 0;
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
	         "Chrontel: TV overscan shall be %s\n",
	         val ? enabledstr : disabledstr);
	  }
	  if(xf86GetOptValBool(pSiS->Options, OPTION_CHTVSOVERSCAN, &pSiS->OptTVSOver)) {
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
	         "Chrontel: TV super overscan shall be %s\n",
	         pSiS->OptTVSOver ? enabledstr : disabledstr);
	  }
       }

       /* Various parameters for TV output via SiS bridge, Chrontel or SiS6326
        */
       if((pSiS->VGAEngine == SIS_300_VGA) || (pSiS->VGAEngine == SIS_315_VGA)) {
	  int tmp = 0;
	  xf86GetOptValInteger(pSiS->Options, OPTION_CHTVLUMABANDWIDTHCVBS,
				&pSiS->chtvlumabandwidthcvbs);
	  xf86GetOptValInteger(pSiS->Options, OPTION_CHTVLUMABANDWIDTHSVIDEO,
				&pSiS->chtvlumabandwidthsvideo);
	  xf86GetOptValInteger(pSiS->Options, OPTION_CHTVLUMAFLICKERFILTER,
				&pSiS->chtvlumaflickerfilter);
	  xf86GetOptValInteger(pSiS->Options, OPTION_CHTVCHROMABANDWIDTH,
				&pSiS->chtvchromabandwidth);
	  xf86GetOptValInteger(pSiS->Options, OPTION_CHTVCHROMAFLICKERFILTER,
				&pSiS->chtvchromaflickerfilter);
	  xf86GetOptValBool(pSiS->Options, OPTION_CHTVCVBSCOLOR,
				&pSiS->chtvcvbscolor);
	  xf86GetOptValInteger(pSiS->Options, OPTION_CHTVTEXTENHANCE,
				&pSiS->chtvtextenhance);
	  xf86GetOptValInteger(pSiS->Options, OPTION_CHTVCONTRAST,
				&pSiS->chtvcontrast);
	  xf86GetOptValInteger(pSiS->Options, OPTION_SISTVEDGEENHANCE,
				&pSiS->sistvedgeenhance);
	  xf86GetOptValInteger(pSiS->Options, OPTION_SISTVSATURATION,
				&pSiS->sistvsaturation);
	  xf86GetOptValInteger(pSiS->Options, OPTION_SISTVLUMAFILTER,
				&pSiS->sistvyfilter);
	  if((pSiS->sistvyfilter < 0) || (pSiS->sistvyfilter > 8)) {
	     xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		"Illegal Y Filter number; valid is 0 (off), 1 (default), 2-8 (filter number 1-7)\n");
	     pSiS->sistvyfilter = 1;
	  }
	  xf86GetOptValBool(pSiS->Options, OPTION_SISTVCHROMAFILTER,
				&pSiS->sistvcfilter);
	  xf86GetOptValInteger(pSiS->Options, OPTION_SISTVCOLCALIBCOARSE,
				&pSiS->sistvcolcalibc);
	  xf86GetOptValInteger(pSiS->Options, OPTION_SISTVCOLCALIBFINE,
				&pSiS->sistvcolcalibf);
	  if((pSiS->sistvcolcalibf > 127) || (pSiS->sistvcolcalibf < -128) ||
	     (pSiS->sistvcolcalibc > 120) || (pSiS->sistvcolcalibc < -120)) {
	     pSiS->sistvcolcalibf = pSiS->sistvcolcalibc = 0;
	     xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	     	"Illegal Color Calibration. Range is -128 to 127 (fine), -120 to 120 (coarse)\n");
	  }
	  xf86GetOptValInteger(pSiS->Options, OPTION_TVXPOSOFFSET,
				&pSiS->tvxpos);
	  xf86GetOptValInteger(pSiS->Options, OPTION_TVYPOSOFFSET,
				&pSiS->tvypos);
	  if(pSiS->tvxpos > 32)  { pSiS->tvxpos = 32;  tmp = 1; }
	  if(pSiS->tvxpos < -32) { pSiS->tvxpos = -32; tmp = 1; }
	  if(pSiS->tvypos > 32)  { pSiS->tvypos = 32;  tmp = 1; }
	  if(pSiS->tvypos < -32) { pSiS->tvypos = -32;  tmp = 1; }
	  if(tmp) xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		      "Illegal TV x or y offset. Range is from -32 to 32\n");
	  tmp = 0;
	  xf86GetOptValInteger(pSiS->Options, OPTION_TVXSCALE,
				&pSiS->tvxscale);
	  xf86GetOptValInteger(pSiS->Options, OPTION_TVYSCALE,
				&pSiS->tvyscale);
	  if(pSiS->tvxscale > 16)  { pSiS->tvxscale = 16;  tmp = 1; }
	  if(pSiS->tvxscale < -16) { pSiS->tvxscale = -16; tmp = 1; }
	  if(pSiS->tvyscale > 3)  { pSiS->tvyscale = 3;  tmp = 1; }
	  if(pSiS->tvyscale < -4) { pSiS->tvyscale = -4; tmp = 1; }
	  if(tmp) xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		      "Illegal TV x or y scaling parameter. Range is from -16 to 16 (X), -4 to 3 (Y)\n");
       }

       if((pSiS->Chipset == PCI_CHIP_SIS6326) && (pSiS->SiS6326Flags & SIS6326_HASTV)) {
	  int tmp = 0;
	  strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_SIS6326FORCETVPPLUG);
	  if(strptr) {
	     if(!xf86NameCmp(strptr,"COMPOSITE"))
		pSiS->sis6326tvplug = 1;
	     else if(!xf86NameCmp(strptr,"SVIDEO"))
		pSiS->sis6326tvplug = 0;
	     else {
		SiS_PrintBadOpt(pScrn, strptr, OPTION_SIS6326FORCETVPPLUG);
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "%s \"COMPOSITE\" or \"SVIDEO\"\n", validparm);
	     }
	  }
	  xf86GetOptValBool(pSiS->Options, OPTION_SIS6326ENABLEYFILTER,
				&pSiS->sis6326enableyfilter);
	  xf86GetOptValBool(pSiS->Options, OPTION_SIS6326YFILTERSTRONG,
				&pSiS->sis6326yfilterstrong);
	  xf86GetOptValInteger(pSiS->Options, OPTION_TVXPOSOFFSET,
				&pSiS->tvxpos);
	  xf86GetOptValInteger(pSiS->Options, OPTION_TVYPOSOFFSET,
				&pSiS->tvypos);
	  if(pSiS->tvxpos > 16)  { pSiS->tvxpos = 16;  tmp = 1; }
	  if(pSiS->tvxpos < -16) { pSiS->tvxpos = -16; tmp = 1; }
	  if(pSiS->tvypos > 16)  { pSiS->tvypos = 16;  tmp = 1; }
	  if(pSiS->tvypos < -16) { pSiS->tvypos = -16;  tmp = 1; }
	  if(tmp) xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		      "Illegal TV x or y offset. Range is from -16 to 16\n");
          xf86GetOptValInteger(pSiS->Options, OPTION_SIS6326FSCADJUST,
				&pSiS->sis6326fscadjust);
	  if(pSiS->sis6326fscadjust) {
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		"Adjusting the default FSC by %d\n",
		pSiS->sis6326fscadjust);
	  }
       }

       if((pSiS->VGAEngine == SIS_300_VGA) || (pSiS->VGAEngine == SIS_315_VGA) ||
	  ((pSiS->Chipset == PCI_CHIP_SIS6326) && (pSiS->SiS6326Flags & SIS6326_HASTV))) {
	  Bool Is6326 = FALSE;
	  strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_SISTVANTIFLICKER);
	  if(!strptr) {
	     strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_SIS6326ANTIFLICKER);
	     Is6326 = TRUE;
	  }
	  if(strptr) {
	     if(!xf86NameCmp(strptr,"OFF"))
		pSiS->sistvantiflicker = 0;
	     else if(!xf86NameCmp(strptr,"LOW"))
		pSiS->sistvantiflicker = 1;
	     else if(!xf86NameCmp(strptr,"MED"))
		pSiS->sistvantiflicker = 2;
	     else if(!xf86NameCmp(strptr,"HIGH"))
		pSiS->sistvantiflicker = 3;
	     else if(!xf86NameCmp(strptr,"ADAPTIVE"))
		pSiS->sistvantiflicker = 4;
	     else {
		pSiS->sistvantiflicker = -1;
		SiS_PrintBadOpt(pScrn, strptr, Is6326 ? OPTION_SIS6326FORCETVPPLUG : OPTION_SISTVANTIFLICKER);
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		    "%s \"OFF\", \"LOW\", \"MED\", \"HIGH\" or \"ADAPTIVE\"\n", validparm);
	     }
	  }
       }

       /* CRT2Gamma - enable/disable/set gamma correction for CRT2
	* Since 2004/11/26, this option is a boolean and string option
	* simulaniously. "TRUE" (or other valid bool values) have the
	* same effect as before: The driver uses the (global) Gamma
	* for both CRT1 and CRT2. Otherwise, this option takes one or
	* three floats between 0.1 and 10.0 which define a separate
	* gamma correction for CRT2. (SiS video bridges only.)
	*/
       if((pSiS->VGAEngine == SIS_300_VGA) || (pSiS->VGAEngine == SIS_315_VGA)) {
	  if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_CRT2GAMMA))) {
	     if(SiS_StrIsBoolOn(strptr)) {
		pSiS->CRT2gamma = TRUE;
		pSiS->CRT2SepGamma = FALSE;
	     } else if(SiS_StrIsBoolOff(strptr)) {
		pSiS->CRT2gamma = pSiS->CRT2SepGamma = FALSE;
	     } else {
		if(SiS_EvalOneOrThreeFloats(pScrn, OPTION_CRT2GAMMA, gammaopt, strptr,
				&pSiS->GammaR2i, &pSiS->GammaG2i, &pSiS->GammaB2i)) {
		   pSiS->GammaR2 = (float)pSiS->GammaR2i / 1000.0;
		   pSiS->GammaG2 = (float)pSiS->GammaG2i / 1000.0;
		   pSiS->GammaB2 = (float)pSiS->GammaB2i / 1000.0;
		   pSiS->CRT2gamma = TRUE;
		   if(!IsDHM) pSiS->CRT2SepGamma = TRUE;
		   else {
#ifdef SISDUALHEAD
		      pSiS->CRT2SepGamma = FALSE;
		      xf86DrvMsg(pScrn->scrnIndex, X_INFO,
				"CRT2Gamma values overrule default and Monitor Gamma\n");
		      if(pScrn->monitor) {
			 pScrn->monitor->gamma.red = pSiS->GammaR2;
			 pScrn->monitor->gamma.green = pSiS->GammaG2;
			 pScrn->monitor->gamma.blue = pSiS->GammaB2;
		      }
#endif
		   }
		}
	     }
	  }
       }

       /* Default adaptor: Overlay (default) or blitter */
       if(pSiS->VGAEngine == SIS_315_VGA) {
	  if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_XVDEFAULTADAPTOR))) {
	     if(!xf86NameCmp(strptr, "OVERLAY")) {
		pSiS->XvDefAdaptorBlit = FALSE;
	     } else if(!xf86NameCmp(strptr, "BLITTER")) {
		pSiS->XvDefAdaptorBlit = TRUE;
	     } else {
		SiS_PrintBadOpt(pScrn, strptr, OPTION_XVDEFAULTADAPTOR);
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"%s \"OVERLAY\" or \"BLITTER\"\n", validparm);
	     }
	  }
       }

       if(xf86GetOptValBool(pSiS->Options, OPTION_XVMEMCPY, &val)) {
	  pSiS->XvUseMemcpy = val ? TRUE : FALSE;
	  xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Xv will %suse memcpy()\n",
		val ? "" : "not ");
       }

#if defined(__i386__) || defined(__AMD64__) || defined(__amd64__) || defined(__x86_64__)
       if(xf86GetOptValBool(pSiS->Options, OPTION_XVBENCHCPY, &val)) {
	  pSiS->BenchMemCpy = val ? TRUE : FALSE;
	  xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		"Will %sbenchmark methods for system RAM to video RAM transfers\n",
		val ? "" : "not ");
       }

#ifndef SISCHECKOSSSE
       if(xf86GetOptValBool(pSiS->Options, OPTION_XVSSECOPY, &val)) {
	  pSiS->XvSSEMemcpy = val ? TRUE : FALSE;
	  xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		"Will %s use SSE CPU instructions\n",
		val ? "eventually" : "not");
       }
#endif
#endif /* __i386__ || __AMD64__ || __amd64__ */

#ifdef SIS_CP
       SIS_CP_OPT_DOOPT
#endif

    }  /* DualHead */

    /* CRT1Gamma - enable/disable gamma correction for CRT1
     */
    if(xf86GetOptValBool(pSiS->Options, OPTION_CRT1GAMMA, &val)) {
       pSiS->CRT1gamma = val;
       pSiS->CRT1gammaGiven = TRUE;
    }

    /* ForceCRT1Aspect, ForceCRT2Aspect */
    /* Make driver believe that a connected CRT/VGA device is 4:3 ("normal")
     * or 16:9 ("wide"). Note: This affects only for real VGA (analog)
     * output devices, not TV or DVI/LCD.
     */
    if(pSiS->VGAEngine == SIS_315_VGA) {
       strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_FORCE1ASPECT);
       if(strptr != NULL) {
	  if(!xf86NameCmp(strptr,"WIDE")) {
	     pSiS->SiS_Pr->SiS_UseWide = 1;
	  } else if(!xf86NameCmp(strptr,"NORMAL")) {
	     pSiS->SiS_Pr->SiS_UseWide = 0;
	  } else {
	     SiS_PrintBadOpt(pScrn, strptr, OPTION_FORCE1ASPECT);
	     xf86DrvMsg(pScrn->scrnIndex, X_INFO, "%s:\n", validparm);
	     xf86DrvMsg(pScrn->scrnIndex, X_INFO, "\t\"NORMAL\" or \"WIDE\"\n");
          }
	  if(pSiS->SiS_Pr->SiS_UseWide != -1) {
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		"CRT1 (VGA) aspect ratio will be assumed %s\n",
		pSiS->SiS_Pr->SiS_UseWide ? "wide" : "normal");
	  }
       }
       if(pSiS->SiS_Pr->SiS_UseWideCRT2 != -1) {
	  xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		"CRT2 (VGA) aspect ratio will be assumed %s\n",
		pSiS->SiS_Pr->SiS_UseWideCRT2 ? "wide" : "normal");
       }
    }

    /* VESA - DEPRECATED
     * This option is for forcing the driver to use
     * the VESA BIOS extension for mode switching.
     */
    if(xf86GetOptValBool(pSiS->Options, OPTION_VESA, &val)) {
       if(IsDHM) {
	  xf86DrvMsg(pScrn->scrnIndex, X_WARNING, baddhm,
	     pSiS->Options[SiS_FIFT(pSiS->Options, OPTION_VESA)].name);
       } else {
	  pSiS->VESA = val ? 1 : 0;
	  xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		"VESA: VESA usage shall be %s\n",
		val ? enabledstr : disabledstr);
          xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		"*** Option \"VESA\" is deprecated. *** \n");
	  if(pSiS->VESA) pSiS->ForceCRT1Type = CRT1_VGA;
       }
    }

    if((pSiS->VGAEngine == SIS_300_VGA) || (pSiS->VGAEngine == SIS_315_VGA)) {
       /* InternalModes (300/315/330 series only)
	* The default behavior is to replace X's default modes with
	* a mode list generated out of the known and supported modes. Use
	* this option to disable this. NOT RECOMMENDED.
	*/
       if(xf86GetOptValBool(pSiS->Options, OPTION_INTERNALMODES, &val)) {
	  pSiS->noInternalModes = val ? FALSE : TRUE;
	  if(pSiS->noInternalModes) {
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Usage of built-in modes is %s\n", disabledstr);
	  }
       }

    }

    /* ShadowFB */
    from = X_DEFAULT;
    if(xf86GetOptValBool(pSiS->Options, OPTION_SHADOW_FB, &pSiS->ShadowFB)) {
#ifdef SISMERGED
       if(pSiS->MergedFB) {
	  pSiS->ShadowFB = FALSE;
	  xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	      "Shadow Framebuffer not supported in MergedFB mode\n");
       } else
#endif
	  from = X_CONFIG;
    }
    if(pSiS->ShadowFB) {
	pSiS->NoAccel = TRUE;
#if XF86_VERSION_CURRENT < XF86_VERSION_NUMERIC(4,2,99,0,0)
	pSiS->NoXvideo = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, from,
	   "Using \"Shadow Framebuffer\" - 2D acceleration and Xv disabled\n");
#else
	xf86DrvMsg(pScrn->scrnIndex, from,
	   "Using \"Shadow Framebuffer\" - 2D acceleration disabled\n");
#endif
    }

    /* Rotate */
    if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_ROTATE))) {
#ifdef SISMERGED
       if(pSiS->MergedFB) {
	  xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	      "Screen rotation not supported in MergedFB mode\n");
       } else
#endif
       if(IsDHM) {
	  xf86DrvMsg(pScrn->scrnIndex, X_WARNING, baddhm,
	     pSiS->Options[SiS_FIFT(pSiS->Options, OPTION_ROTATE)].name);
       } else if(!xf86NameCmp(strptr, "CW")) {
	  pSiS->Rotate = 1;
       } else if(!xf86NameCmp(strptr, "CCW")) {
	  pSiS->Rotate = -1;
       } else {
	  SiS_PrintBadOpt(pScrn, strptr, OPTION_ROTATE);
	  xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	      "%s \"CW\" or \"CCW\"\n", validparm);
       }

       if(pSiS->Rotate) {
	  pSiS->ShadowFB = TRUE;
	  pSiS->NoAccel  = TRUE;
	  pSiS->HWCursor = FALSE;
	  pSiS->NoXvideo = TRUE;
	  xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
	      "Rotating screen %sclockwise (2D acceleration and Xv disabled)\n",
	      (pSiS->Rotate == -1) ? "counter " : "");
       }
    }

    /* Reflect */
    if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_REFLECT))) {
#ifdef SISMERGED
       if(pSiS->MergedFB) {
	  xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	      "Screen reflection not supported in MergedFB mode\n");
       } else
#endif
       if(IsDHM) {
	  xf86DrvMsg(pScrn->scrnIndex, X_WARNING, baddhm,
	     pSiS->Options[SiS_FIFT(pSiS->Options, OPTION_REFLECT)].name);
       } else if(pSiS->Rotate) {
	  xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	  	"Screen rotation and reflection are mutually exclusive\n");
       } else if(!xf86NameCmp(strptr, "X")) {
	  pSiS->Reflect = 1;
       } else if(!xf86NameCmp(strptr, "Y")) {
	  pSiS->Reflect = 2;
       } else if((!xf86NameCmp(strptr, "XY")) ||
		 (!xf86NameCmp(strptr, "YX"))) {
	  pSiS->Reflect = 3;
       } else {
	  SiS_PrintBadOpt(pScrn, strptr, OPTION_REFLECT);
	  xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	      "%s \"X\", \"Y\" or \"XY\"\n", validparm);
       }

       if(pSiS->Reflect) {
	  pSiS->ShadowFB = TRUE;
	  pSiS->NoAccel  = TRUE;
	  pSiS->HWCursor = FALSE;
	  pSiS->NoXvideo = TRUE;
	  xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
	      "Reflecting screen (2D acceleration and Xv disabled)\n");
       }
    }

#ifdef XF86DRI
    /* DRI */
    from = X_DEFAULT;
    if(xf86GetOptValBool(pSiS->Options, OPTION_DRI, &pSiS->loadDRI)) {
       from = X_CONFIG;
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "DRI %s\n",
		pSiS->loadDRI ? enabledstr : disabledstr);

    /* AGPSize = GARTSize */
    if(xf86GetOptValInteger(pSiS->Options, OPTION_AGP_SIZE, &ival)) {
       if((ival >= 8) && (ival <= 512)) {
	  pSiS->agpWantedPages = (ival * 1024 * 1024) / AGP_PAGE_SIZE;
       } else {
	  SiS_PrintIlRange(pScrn, OPTION_AGP_SIZE, 8, 512, 0);
       }
    }
#endif

    /* XVideo
     * Set enables/disables Xv hardware video acceleration
     */
#if XF86_VERSION_CURRENT < XF86_VERSION_NUMERIC(4,2,99,0,0)
    if((!pSiS->NoAccel) && (!pSiS->NoXvideo)) {
#else
    if(!pSiS->NoXvideo) {
#endif
       if(!xf86ReturnOptValBool(pSiS->Options, OPTION_XVIDEO, TRUE)) {
	  pSiS->NoXvideo = TRUE;
	  xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "XVideo extension (Xv) disabled\n");
       }

       if(!pSiS->NoXvideo) {

	  if((pSiS->VGAEngine == SIS_300_VGA) || (pSiS->VGAEngine == SIS_315_VGA)) {
	     /* XvOnCRT2
	      * On chipsets with only one overlay (315, 650, 740, 330), the user can
	      * choose to display the overlay on CRT1 or CRT2. By setting this
	      * option to TRUE, the overlay will be displayed on CRT2. The
	      * default is: CRT1 if only CRT1 available, CRT2 if only CRT2
	      * available, and CRT1 if both is available and detected.
	      * Since implementation of the XV_SWITCHCRT Xv property this only
	      * selects the default CRT.
	      */
	     if(xf86GetOptValBool(pSiS->Options, OPTION_XVONCRT2, &val)) {
	        pSiS->XvOnCRT2 = val ? TRUE : FALSE;
	     }
	  }

	  if((pSiS->VGAEngine == SIS_OLD_VGA) || (pSiS->VGAEngine == SIS_530_VGA)) {
	     /* NoYV12 (for 5597/5598, 6326 and 530/620 only)
	      * YV12 has problems with videos larger than 384x288. So
	      * allow the user to disable YV12 support to force the
	      * application to use YUV2 instead.
	      */
	     if(xf86GetOptValBool(pSiS->Options, OPTION_YV12, &val)) {
		pSiS->NoYV12 = val ? 0 : 1;
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
			"Xv YV12/I420 support is %s\n",
			pSiS->NoYV12 ? disabledstr : enabledstr);
	     }
	  }

	  /* Some Xv properties' defaults can be set by options */
	  if(xf86GetOptValInteger(pSiS->Options, OPTION_XVDEFCONTRAST, &ival)) {
	     if((ival >= 0) && (ival <= 7)) pSiS->XvDefCon = ival;
	     else SiS_PrintIlRange(pScrn, OPTION_XVDEFCONTRAST, 0, 7, 0);
	  }
	  if(xf86GetOptValInteger(pSiS->Options, OPTION_XVDEFBRIGHTNESS, &ival)) {
	     if((ival >= -128) && (ival <= 127)) pSiS->XvDefBri = ival;
	     else SiS_PrintIlRange(pScrn, OPTION_XVDEFBRIGHTNESS, -128, 127, 0);
	  }
	  if(pSiS->VGAEngine == SIS_315_VGA) {
	     if(xf86GetOptValInteger(pSiS->Options, OPTION_XVDEFHUE, &ival)) {
		if((ival >= -8) && (ival <= 7)) pSiS->XvDefHue = ival;
		else SiS_PrintIlRange(pScrn, OPTION_XVDEFHUE, -8, 7, 0);
	     }
	     if(xf86GetOptValInteger(pSiS->Options, OPTION_XVDEFSATURATION, &ival)) {
		if((ival >= -7) && (ival <= 7)) pSiS->XvDefSat = ival;
		else SiS_PrintIlRange(pScrn, OPTION_XVDEFSATURATION, -7, 7, 0);
	     }
	  }
	  if(xf86GetOptValBool(pSiS->Options, OPTION_XVDEFDISABLEGFX, &val)) {
	     if(val) pSiS->XvDefDisableGfx = TRUE;
	     xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		"Graphics display will be %s during Xv usage\n",
		val ? disabledstr : enabledstr);
	  }
	  if((pSiS->VGAEngine == SIS_300_VGA) || (pSiS->VGAEngine == SIS_315_VGA)) {
	     if(xf86GetOptValBool(pSiS->Options, OPTION_XVDEFDISABLEGFXLR, &val)) {
		if(val) pSiS->XvDefDisableGfxLR = TRUE;
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "Graphics display left/right of overlay will be %s during Xv usage\n",
		   val ? disabledstr : enabledstr);
	     }
	     if(xf86GetOptValBool(pSiS->Options, OPTION_XVDISABLECOLORKEY, &val)) {
		if(val) pSiS->XvDisableColorKey = TRUE;
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "Xv Color key is %s\n",
		   val ? disabledstr : enabledstr);
	     }
	     if(xf86GetOptValBool(pSiS->Options, OPTION_XVUSECHROMAKEY, &val)) {
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "Xv Chroma-keying is %s\n",
		   val ? enabledstr : disabledstr);
		if(val) pSiS->XvUseChromaKey = TRUE;
             }
	     if(xf86GetOptValBool(pSiS->Options, OPTION_XVINSIDECHROMAKEY, &val)) {
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "Xv: Video is transparent if %s chroma key range\n",
		   val ? "inside" : "outside");
		if(val) pSiS->XvInsideChromaKey = TRUE;
             }
	     if(pSiS->VGAEngine == SIS_300_VGA) {
		if(xf86GetOptValBool(pSiS->Options, OPTION_XVYUVCHROMAKEY, &val)) {
		   xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		      "Xv: Chroma key is in %s format\n",
		      val ? "YUV" : "RGB");
		   if(val) pSiS->XvYUVChromaKey = TRUE;
		}
             } else {
                if(xf86GetOptValBool(pSiS->Options, OPTION_XVYUVCHROMAKEY, &val)) {
		   xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 4,
		      "Xv: Chroma key is of same format as video source\n");
		}
	     }
	     if(xf86GetOptValInteger(pSiS->Options, OPTION_XVCHROMAMIN, &ival)) {
		if((ival >= 0) && (ival <= 0xffffff)) pSiS->XvChromaMin = ival;
		else SiS_PrintIlRange(pScrn, OPTION_XVCHROMAMIN, 0, 0xffffff, 1);
	     }
	     if(xf86GetOptValInteger(pSiS->Options, OPTION_XVCHROMAMAX, &ival)) {
		if((ival >= 0) && (ival <= 0xffffff)) pSiS->XvChromaMax = ival;
		else SiS_PrintIlRange(pScrn, OPTION_XVCHROMAMAX, 0, 0xffffff, 1);
             }
	  }

	  if(pSiS->VGAEngine == SIS_315_VGA) {
	     /* XvGamma - enable/disable gamma correction for Xv
	      * Supported for CRT1 only
	      */
	     if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_XVGAMMA))) {
		if(SiS_StrIsBoolOn(strptr)) {
		   pSiS->XvGamma = pSiS->XvGammaGiven = TRUE;
		} else if(SiS_StrIsBoolOff(strptr)) {
		   pSiS->XvGamma = FALSE;
		   pSiS->XvGammaGiven = TRUE;
		} else {
		   if(SiS_EvalOneOrThreeFloats(pScrn, OPTION_XVGAMMA, gammaopt, strptr,
				&pSiS->XvGammaRed, &pSiS->XvGammaGreen, &pSiS->XvGammaBlue)) {
		      pSiS->XvGamma = pSiS->XvGammaGiven = TRUE;
		      pSiS->XvGammaRedDef = pSiS->XvGammaRed;
		      pSiS->XvGammaGreenDef = pSiS->XvGammaGreen;
		      pSiS->XvGammaBlue = pSiS->XvGammaBlue;
		   }
		}
	     }
	  }
       }
    }

    if((pSiS->VGAEngine == SIS_300_VGA) || (pSiS->VGAEngine == SIS_315_VGA)) {
       Bool GotNewBri = FALSE, GotOldBri = FALSE, GotCon = FALSE;
       if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_NEWSTOREDCON))) {
	  SiS_EvalOneOrThreeFloats2(pScrn, OPTION_NEWSTOREDCON, newbriopt, strptr,
		&pSiS->NewGammaConR, &pSiS->NewGammaConG, &pSiS->NewGammaConB);
	  GotCon = TRUE;
       }
       if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_NEWSTOREDBRI))) {
	  SiS_EvalOneOrThreeFloats2(pScrn, OPTION_NEWSTOREDBRI, newbriopt, strptr,
		&pSiS->NewGammaBriR, &pSiS->NewGammaBriG, &pSiS->NewGammaBriB);
	  GotNewBri = TRUE;
       }
       if(!GotCon && !GotNewBri) {
          if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_STOREDBRI))) {
	     SiS_EvalOneOrThreeFloats(pScrn, OPTION_STOREDBRI, briopt, strptr,
		   &pSiS->GammaBriR, &pSiS->GammaBriG, &pSiS->GammaBriB);
	     GotOldBri = TRUE;
	     pSiS->SiS_SD3_Flags |= SiS_SD3_OLDGAMMAINUSE;
	  }
       }

       if((!IsDHM) || (IsDHM && !IsSecondHead)) {
          Bool GotCon2 = FALSE, GotNewBri2 = FALSE;
          if(!GotOldBri) {
             if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_NEWSTOREDCON2))) {
	        SiS_EvalOneOrThreeFloats2(pScrn, OPTION_NEWSTOREDCON2, newbriopt, strptr,
		      &pSiS->NewGammaConR2, &pSiS->NewGammaConG2, &pSiS->NewGammaConB2);
	        GotCon2 = TRUE;
             }
             if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_NEWSTOREDBRI2))) {
	        if(SiS_EvalOneOrThreeFloats2(pScrn, OPTION_NEWSTOREDBRI2, newbriopt, strptr,
				&pSiS->NewGammaBriR2, &pSiS->NewGammaBriG2, &pSiS->NewGammaBriB2)) {
		   if(IsDHM) {
#ifdef SISDUALHEAD
		      if(GotNewBri) SiS_PrintOverruleDHM(pScrn, OPTION_NEWSTOREDBRI2, OPTION_NEWSTOREDBRI);
		      pSiS->NewGammaBriR = pSiS->NewGammaBriR2;
		      pSiS->NewGammaBriG = pSiS->NewGammaBriG2;
		      pSiS->NewGammaBriB = pSiS->NewGammaBriB2;
#endif
	           } else pSiS->CRT2SepGamma = TRUE;
	        }
	        GotNewBri2 = TRUE;
	     }
          }
          if(!GotCon2 && !GotNewBri2 && !GotNewBri && !GotCon) {
	     if((strptr = (char *)xf86GetOptValString(pSiS->Options, OPTION_STOREDBRI2))) {
	        pSiS->SiS_SD3_Flags |= SiS_SD3_OLDGAMMAINUSE;
	        if(SiS_EvalOneOrThreeFloats(pScrn, OPTION_STOREDBRI2, briopt, strptr,
		    &pSiS->GammaBriR2, &pSiS->GammaBriG2, &pSiS->GammaBriB2)) {
	           if(IsDHM) {
#ifdef SISDUALHEAD
		      if(GotOldBri) SiS_PrintOverruleDHM(pScrn, OPTION_STOREDBRI2, OPTION_STOREDBRI);
		      pSiS->GammaBriR = pSiS->GammaBriR2;
		      pSiS->GammaBriG = pSiS->GammaBriG2;
		      pSiS->GammaBriB = pSiS->GammaBriB2;
#endif
	           } else pSiS->CRT2SepGamma = TRUE;
	        }
	     }
	  }
       }
    }

    if(pSiS->SiS_SD3_Flags & SiS_SD3_CRT1SATGAIN) {
       if(xf86GetOptValInteger(pSiS->Options, OPTION_CRT1SATGAIN, &ival)) {
          if((ival >= 0) && (ival <= 7)) {
             pSiS->siscrt1satgain = ival;
             pSiS->crt1satgaingiven = TRUE;
          } else SiS_PrintIlRange(pScrn, OPTION_CRT1SATGAIN, 0, 7, 0);
       }
    }

}

const OptionInfoRec *
SISAvailableOptions(int chipid, int busid)
{
    return SISOptions;
}
