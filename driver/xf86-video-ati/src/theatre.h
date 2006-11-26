#ifndef __THEATRE_H__
#define __THEATRE_H__

#define MODE_UNINITIALIZED		1
#define MODE_INITIALIZATION_IN_PROGRESS 2
#define MODE_INITIALIZED_FOR_TV_IN	3

typedef struct {
         GENERIC_BUS_Ptr VIP;
	 
	 int theatre_num;
	 CARD32 theatre_id;
	 int  mode;
	 char* microc_path;
	 char* microc_type;
	 
	 CARD16 video_decoder_type;
	 CARD32 wStandard;
	 CARD32 wConnector;
	 int    iHue;
	 int    iSaturation;
	 CARD32 wSaturation_U;
	 CARD32 wSaturation_V;
	 int    iBrightness;
	 int    dbBrightnessRatio;
	 CARD32 wSharpness;
	 int    iContrast;
	 int    dbContrast;
	 CARD32 wInterlaced;
	 CARD32 wTunerConnector;
	 CARD32 wComp0Connector;
	 CARD32 wSVideo0Connector;
	 CARD32 dwHorzScalingRatio;
	 CARD32 dwVertScalingRatio;
	 
	 } TheatreRec, * TheatrePtr;

/* DO NOT FORGET to setup constants before calling InitTheatre */
void InitTheatre(TheatrePtr t);

void RT_SetTint (TheatrePtr t, int hue);
void RT_SetSaturation (TheatrePtr t, int Saturation);
void RT_SetBrightness (TheatrePtr t, int Brightness);
void RT_SetSharpness (TheatrePtr t, CARD16 wSharpness);
void RT_SetContrast (TheatrePtr t, int Contrast);
void RT_SetInterlace (TheatrePtr t, CARD8 bInterlace);
void RT_SetStandard (TheatrePtr t, CARD16 wStandard);
void RT_SetCombFilter (TheatrePtr t, CARD16 wStandard, CARD16 wConnector);
void RT_SetOutputVideoSize (TheatrePtr t, CARD16 wHorzSize, CARD16 wVertSize, CARD8 fCC_On, CARD8 fVBICap_On);
void CalculateCrCbGain (TheatrePtr t, double *CrGain, double *CbGain, CARD16 wStandard);
void RT_SetConnector (TheatrePtr t, CARD16 wConnector, int tunerFlag);

void RageTheatreDebugGain(TheatrePtr t, Bool on, CARD32 gain);
void ShutdownTheatre(TheatrePtr t);
void DumpRageTheatreRegs(TheatrePtr t);
void ResetTheatreRegsForTVout(TheatrePtr t);
void ResetTheatreRegsForNoTVout(TheatrePtr t);


#define TheatreSymbolsList  \
		"InitTheatre" \
		"RT_SetTint", \
		"RT_SetSaturation", \
		"RT_SetBrightness", \
		"RT_SetSharpness", \
		"RT_SetContrast", \
		"RT_SetInterlace", \
		"RT_SetStandard", \
		"RT_SetCombFilter", \
		"RT_SetOutputVideoSize", \
		"RT_SetConnector", \
		"ResetTheatreRegsForNoTVout", \
		"ResetTheatreRegsForTVout", \
		"DumpRageTheatreRegs", \
		"ShutdownTheatre"

#ifdef XFree86LOADER

#define xf86_InitTheatre           ((void (*)(TheatrePtr t))LoaderSymbol("InitTheatre"))

#define xf86_RT_SetTint            ((void (*)(TheatrePtr, int))LoaderSymbol("RT_SetTint"))
#define xf86_RT_SetSaturation      ((void (*)(TheatrePtr, int))LoaderSymbol("RT_SetSaturation"))
#define xf86_RT_SetBrightness      ((void (*)(TheatrePtr, int))LoaderSymbol("RT_SetBrightness"))
#define xf86_RT_SetSharpness       ((void (*)(TheatrePtr, CARD16))LoaderSymbol("RT_SetSharpness"))
#define xf86_RT_SetContrast        ((void (*)(TheatrePtr, int))LoaderSymbol("RT_SetContrast"))
#define xf86_RT_SetInterlace       ((void (*)(TheatrePtr, CARD8))LoaderSymbol("RT_SetInterlace"))
#define xf86_RT_SetStandard        ((void (*)(TheatrePtr, CARD16))LoaderSymbol("RT_SetStandard"))
#define xf86_RT_SetOutputVideoSize ((void (*)(TheatrePtr, CARD16, CARD16, CARD8, CARD8))LoaderSymbol("RT_SetOutputVideoSize"))
#define xf86_RT_SetConnector       ((void (*)(TheatrePtr, CARD16, int))LoaderSymbol("RT_SetConnector"))

#define xf86_RageTheatreDebugGain       ((void (*)(TheatrePtr, Bool, CARD32))LoaderSymbol("RageTheatreDebugGain"))
#define xf86_ShutdownTheatre       ((void (*)(TheatrePtr))LoaderSymbol("ShutdownTheatre"))
#define xf86_DumpRageTheatreRegs       ((void (*)(TheatrePtr))LoaderSymbol("DumpRageTheatreRegs"))
#define xf86_ResetTheatreRegsForTVout       ((void (*)(TheatrePtr))LoaderSymbol("ResetTheatreRegsForTVout"))
#define xf86_ResetTheatreRegsForNoTVout       ((void (*)(TheatrePtr))LoaderSymbol("ResetTheatreRegsForNoTVout"))
#define xf86_RT_GetSignalStatus       ((void (*)(TheatrePtr))LoaderSymbol("xf86_RT_GetSignalStatus"))
#else

#define xf86_InitTheatre               InitTheatre

#define xf86_RT_SetTint                RT_SetTint
#define xf86_RT_SetSaturation          RT_SetSaturation
#define xf86_RT_SetBrightness          RT_SetBrightness
#define xf86_RT_SetSharpness           RT_SetSharpness
#define xf86_RT_SetContrast            RT_SetContrast
#define xf86_RT_SetInterlace           RT_SetInterlace
#define xf86_RT_SetStandard            RT_SetStandard
#define xf86_RT_SetOutputVideoSize     RT_SetOutputVideoSize
#define xf86_RT_SetConnector           RT_SetConnector

#define xf86_RageTheatreDebugGain      RageTheatreDebugGain
#define xf86_ShutdownTheatre           ShutdownTheatre
#define xf86_DumpRageTheatreRegs       DumpRageTheatreRegs 
#define xf86_ResetTheatreRegsForNoTVout ResetTheatreRegsForNoTVout
#endif		

#endif
