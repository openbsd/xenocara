#ifndef __THEATRE_H__
#define __THEATRE_H__

#define MODE_UNINITIALIZED		1
#define MODE_INITIALIZATION_IN_PROGRESS 2
#define MODE_INITIALIZED_FOR_TV_IN	3

typedef struct {
         GENERIC_BUS_Ptr VIP;
	 
	 int      theatre_num;
	 uint32_t theatre_id;
	 int      mode;
	 char*    microc_path;
	 char*    microc_type;
	 
	 uint16_t video_decoder_type;
	 uint32_t wStandard;
	 uint32_t wConnector;
	 int      iHue;
	 int      iSaturation;
	 uint32_t wSaturation_U;
	 uint32_t wSaturation_V;
	 int      iBrightness;
	 int      dbBrightnessRatio;
	 uint32_t wSharpness;
	 int      iContrast;
	 int      dbContrast;
	 uint32_t wInterlaced;
	 uint32_t wTunerConnector;
	 uint32_t wComp0Connector;
	 uint32_t wSVideo0Connector;
	 uint32_t dwHorzScalingRatio;
	 uint32_t dwVertScalingRatio;
	 
	 } TheatreRec, * TheatrePtr;


/* DO NOT FORGET to setup constants before calling InitTheatre */
#define xf86_InitTheatre		InitTheatre
_X_EXPORT void InitTheatre(TheatrePtr t);
#define xf86_RT_SetTint			RT_SetTint
_X_EXPORT void RT_SetTint (TheatrePtr t, int hue);
#define xf86_RT_SetSaturation		RT_SetSaturation
_X_EXPORT void RT_SetSaturation (TheatrePtr t, int Saturation);
#define xf86_RT_SetBrightness		RT_SetBrightness
_X_EXPORT void RT_SetBrightness (TheatrePtr t, int Brightness);
#define xf86_RT_SetSharpness		RT_SetSharpness
_X_EXPORT void RT_SetSharpness (TheatrePtr t, uint16_t wSharpness);
#define xf86_RT_SetContrast		RT_SetContrast
_X_EXPORT void RT_SetContrast (TheatrePtr t, int Contrast);
#define xf86_RT_SetInterlace		RT_SetInterlace
_X_EXPORT void RT_SetInterlace (TheatrePtr t, uint8_t bInterlace);
#define xf86_RT_SetStandard		RT_SetStandard
_X_EXPORT void RT_SetStandard (TheatrePtr t, uint16_t wStandard);
#define xf86_RT_SetOutputVideoSize	RT_SetOutputVideoSize
_X_EXPORT void RT_SetOutputVideoSize (TheatrePtr t, uint16_t wHorzSize, uint16_t wVertSize, uint8_t fCC_On, uint8_t fVBICap_On);
#define xf86_RT_SetConnector		RT_SetConnector
_X_EXPORT void RT_SetConnector (TheatrePtr t, uint16_t wConnector, int tunerFlag);
#define xf86_ResetTheatreRegsForNoTVout	ResetTheatreRegsForNoTVout
_X_EXPORT void ResetTheatreRegsForNoTVout(TheatrePtr t);
#define xf86_ResetTheatreRegsForTVout	ResetTheatreRegsForTVout
_X_EXPORT void ResetTheatreRegsForTVout(TheatrePtr t);
#define xf86_DumpRageTheatreRegs	DumpRageTheatreRegs
_X_EXPORT void DumpRageTheatreRegs(TheatrePtr t);
#define xf86_DumpRageTheatreRegsByName	DumpRageTheatreRegsByName
_X_EXPORT void DumpRageTheatreRegsByName(TheatrePtr t);
#define xf86_ShutdownTheatre		ShutdownTheatre
_X_EXPORT void ShutdownTheatre(TheatrePtr t);

#endif
