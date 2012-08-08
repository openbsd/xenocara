#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include "xf86.h"
#include "generic_bus.h"
#include "theatre.h"
#include "theatre_reg.h"

#undef read
#undef write
#undef ioctl

static Bool theatre_read(TheatrePtr t,uint32_t reg, uint32_t *data)
{
   if(t->theatre_num<0)return FALSE;
   return t->VIP->read(t->VIP, ((t->theatre_num & 0x3)<<14) | reg,4, (uint8_t *) data);
}

static Bool theatre_write(TheatrePtr t,uint32_t reg, uint32_t data)
{
   if(t->theatre_num<0)return FALSE;
   return t->VIP->write(t->VIP,((t->theatre_num & 0x03)<<14) | reg,4, (uint8_t *) &data);
}

#define RT_regr(reg,data)	theatre_read(t,(reg),(data))
#define RT_regw(reg,data)	theatre_write(t,(reg),(data))
#define VIP_TYPE      "ATI VIP BUS"

static void CalculateCrCbGain (TheatrePtr t, double *CrGain, double *CbGain, uint16_t wStandard);
static void RT_SetCombFilter (TheatrePtr t, uint16_t wStandard, uint16_t wConnector);

#if 0
TheatrePtr DetectTheatre(GENERIC_BUS_Ptr b)
{
   TheatrePtr t;  
   uint32_t i;
   uint32_t val;
   char s[20];
   
   b->ioctl(b,GB_IOCTL_GET_TYPE,20,s);
   if(strcmp(VIP_TYPE, s)){
   xf86DrvMsg(b->scrnIndex, X_ERROR, "DetectTheatre must be called with bus of type \"%s\", not \"%s\"\n",
          VIP_TYPE, s);
   return NULL;
   }
   
   t = calloc(1,sizeof(TheatreRec));
   t->VIP = b;
   t->theatre_num = -1;
   t->mode=MODE_UNINITIALIZED;
   
   b->read(b, VIP_VIP_VENDOR_DEVICE_ID, 4, (uint8_t *)&val);
   for(i=0;i<4;i++)
   {
	if(b->read(b, ((i & 0x03)<<14) | VIP_VIP_VENDOR_DEVICE_ID, 4, (uint8_t *)&val))
        {
	  if(val)xf86DrvMsg(b->scrnIndex, X_INFO, "Device %d on VIP bus ids as 0x%08x\n",i,val);
	  if(t->theatre_num>=0)continue; /* already found one instance */
	  switch(val){
	  	case RT100_ATI_ID:
	           t->theatre_num=i;
		   t->theatre_id=RT100_ATI_ID;
		   break;
		case RT200_ATI_ID:
	           t->theatre_num=i;
		   t->theatre_id=RT200_ATI_ID;
		   break;
                }
	} else {
	  xf86DrvMsg(b->scrnIndex, X_INFO, "No response from device %d on VIP bus\n",i);	
	}
   }
   if(t->theatre_num>=0)xf86DrvMsg(b->scrnIndex, X_INFO, "Detected Rage Theatre as device %d on VIP bus with id 0x%08x\n",t->theatre_num,t->theatre_id);

   if(t->theatre_id==RT200_ATI_ID){
   	xf86DrvMsg(b->scrnIndex, X_INFO, "Rage Theatre 200 is not supported yet\n");
	t->theatre_num=-1;
	}

   if(t->theatre_num < 0)
   {
   free(t);
   return NULL;
   }

   RT_regr(VIP_VIP_REVISION_ID, &val);
   xf86DrvMsg(b->scrnIndex, X_INFO, "Detected Rage Theatre revision %8.8X\n", val);

#if 0
DumpRageTheatreRegsByName(t);
#endif
   return t;
}
#endif

enum
{
fld_tmpReg1=0,
fld_tmpReg2,
fld_tmpReg3,
fld_LP_CONTRAST,
fld_LP_BRIGHTNESS,
fld_CP_HUE_CNTL,
fld_LUMA_FILTER,
fld_H_SCALE_RATIO,
fld_H_SHARPNESS,
fld_V_SCALE_RATIO,
fld_V_DEINTERLACE_ON,
fld_V_BYPSS,
fld_V_DITHER_ON,
fld_EVENF_OFFSET,
fld_ODDF_OFFSET,
fld_INTERLACE_DETECTED,
fld_VS_LINE_COUNT,
fld_VS_DETECTED_LINES,
fld_VS_ITU656_VB,
fld_VBI_CC_DATA,
fld_VBI_CC_WT,
fld_VBI_CC_WT_ACK,
fld_VBI_CC_HOLD,
fld_VBI_DECODE_EN,
fld_VBI_CC_DTO_P,
fld_VBI_20BIT_DTO_P,
fld_VBI_CC_LEVEL,
fld_VBI_20BIT_LEVEL,
fld_VBI_CLK_RUNIN_GAIN,
fld_H_VBI_WIND_START,
fld_H_VBI_WIND_END,
fld_V_VBI_WIND_START,
fld_V_VBI_WIND_END,
fld_VBI_20BIT_DATA0,
fld_VBI_20BIT_DATA1,
fld_VBI_20BIT_WT,
fld_VBI_20BIT_WT_ACK,
fld_VBI_20BIT_HOLD,
fld_VBI_CAPTURE_ENABLE,
fld_VBI_EDS_DATA,
fld_VBI_EDS_WT,
fld_VBI_EDS_WT_ACK,
fld_VBI_EDS_HOLD,
fld_VBI_SCALING_RATIO,
fld_VBI_ALIGNER_ENABLE,
fld_H_ACTIVE_START,
fld_H_ACTIVE_END,
fld_V_ACTIVE_START,
fld_V_ACTIVE_END,
fld_CH_HEIGHT,
fld_CH_KILL_LEVEL,
fld_CH_AGC_ERROR_LIM,
fld_CH_AGC_FILTER_EN,
fld_CH_AGC_LOOP_SPEED,
fld_HUE_ADJ,
fld_STANDARD_SEL,
fld_STANDARD_YC,
fld_ADC_PDWN,
fld_INPUT_SELECT,
fld_ADC_PREFLO,
fld_H_SYNC_PULSE_WIDTH,
fld_HS_GENLOCKED,
fld_HS_SYNC_IN_WIN,
fld_VIN_ASYNC_RST,
fld_DVS_ASYNC_RST,
fld_VIP_VENDOR_ID,
fld_VIP_DEVICE_ID,
fld_VIP_REVISION_ID,
fld_BLACK_INT_START,
fld_BLACK_INT_LENGTH,
fld_UV_INT_START,
fld_U_INT_LENGTH,
fld_V_INT_LENGTH,
fld_CRDR_ACTIVE_GAIN,
fld_CBDB_ACTIVE_GAIN,
fld_DVS_DIRECTION,
fld_DVS_VBI_UINT8_SWAP,
fld_DVS_CLK_SELECT,
fld_CONTINUOUS_STREAM,
fld_DVSOUT_CLK_DRV,
fld_DVSOUT_DATA_DRV,
fld_COMB_CNTL0,
fld_COMB_CNTL1,
fld_COMB_CNTL2,
fld_COMB_LENGTH,
fld_SYNCTIP_REF0,
fld_SYNCTIP_REF1,
fld_CLAMP_REF,
fld_AGC_PEAKWHITE,
fld_VBI_PEAKWHITE,
fld_WPA_THRESHOLD,
fld_WPA_TRIGGER_LO,
fld_WPA_TRIGGER_HIGH,
fld_LOCKOUT_START,
fld_LOCKOUT_END,
fld_CH_DTO_INC,
fld_PLL_SGAIN,
fld_PLL_FGAIN,
fld_CR_BURST_GAIN,
fld_CB_BURST_GAIN,
fld_VERT_LOCKOUT_START,
fld_VERT_LOCKOUT_END,
fld_H_IN_WIND_START,
fld_V_IN_WIND_START,
fld_H_OUT_WIND_WIDTH,
fld_V_OUT_WIND_WIDTH,
fld_HS_LINE_TOTAL,
fld_MIN_PULSE_WIDTH,
fld_MAX_PULSE_WIDTH,
fld_WIN_CLOSE_LIMIT,
fld_WIN_OPEN_LIMIT,
fld_VSYNC_INT_TRIGGER,
fld_VSYNC_INT_HOLD,
fld_VIN_M0,
fld_VIN_N0,
fld_MNFLIP_EN,
fld_VIN_P,
fld_REG_CLK_SEL,
fld_VIN_M1,
fld_VIN_N1,
fld_VIN_DRIVER_SEL,
fld_VIN_MNFLIP_REQ,
fld_VIN_MNFLIP_DONE,
fld_TV_LOCK_TO_VIN,
fld_TV_P_FOR_WINCLK,
fld_VINRST,
fld_VIN_CLK_SEL,
fld_VS_FIELD_BLANK_START,
fld_VS_FIELD_BLANK_END,
fld_VS_FIELD_IDLOCATION,
fld_VS_FRAME_TOTAL,
fld_SYNC_TIP_START,
fld_SYNC_TIP_LENGTH,
fld_GAIN_FORCE_DATA,
fld_GAIN_FORCE_EN,
fld_I_CLAMP_SEL,
fld_I_AGC_SEL,
fld_EXT_CLAMP_CAP,
fld_EXT_AGC_CAP,
fld_DECI_DITHER_EN,
fld_ADC_PREFHI,
fld_ADC_CH_GAIN_SEL,
fld_HS_PLL_SGAIN,
fld_NREn,
fld_NRGainCntl,
fld_NRBWTresh,
fld_NRGCTresh,
fld_NRCoefDespeclMode,
fld_GPIO_5_OE,
fld_GPIO_6_OE,
fld_GPIO_5_OUT,
fld_GPIO_6_OUT,

regRT_MAX_REGS
} a;


typedef struct {
	uint8_t size;
	uint32_t fld_id;
	uint32_t dwRegAddrLSBs;
	uint32_t dwFldOffsetLSBs;
	uint32_t dwMaskLSBs;
	uint32_t addr2;
	uint32_t offs2;
	uint32_t mask2;
	uint32_t dwCurrValue;
	uint32_t rw;
} RTREGMAP;

#define READONLY 1
#define WRITEONLY 2
#define READWRITE 3

/* Rage Theatre's Register Mappings, including the default values: */
RTREGMAP RT_RegMap[regRT_MAX_REGS]={
/*
{size, fidname, AddrOfst, Ofst, Mask, Addr, Ofst, Mask, Cur, R/W
*/
{32 , fld_tmpReg1       ,0x151                          , 0, 0x0, 0, 0,0, 0,READWRITE },
{1  , fld_tmpReg2       ,VIP_VIP_SUB_VENDOR_DEVICE_ID   , 3, 0xFFFFFFFF, 0, 0,0, 0,READWRITE },
{1  , fld_tmpReg3       ,VIP_VIP_COMMAND_STATUS         , 3, 0xFFFFFFFF, 0, 0,0, 0,READWRITE },
{8  , fld_LP_CONTRAST   ,VIP_LP_CONTRAST            ,  0, 0xFFFFFF00, 0, 0,0, fld_LP_CONTRAST_def       ,READWRITE  },
{14 , fld_LP_BRIGHTNESS ,VIP_LP_BRIGHTNESS          ,  0, 0xFFFFC000, 0, 0,0, fld_LP_BRIGHTNESS_def     ,READWRITE  },
{8  , fld_CP_HUE_CNTL   ,VIP_CP_HUE_CNTL            ,  0, 0xFFFFFF00, 0, 0,0, fld_CP_HUE_CNTL_def       ,READWRITE  },
{1  , fld_LUMA_FILTER   ,VIP_LP_BRIGHTNESS          , 15, 0xFFFF7FFF, 0, 0,0, fld_LUMA_FILTER_def       ,READWRITE  },
{21 , fld_H_SCALE_RATIO ,VIP_H_SCALER_CONTROL       ,  0, 0xFFE00000, 0, 0,0, fld_H_SCALE_RATIO_def     ,READWRITE  },
{4  , fld_H_SHARPNESS   ,VIP_H_SCALER_CONTROL       , 25, 0xE1FFFFFF, 0, 0,0, fld_H_SHARPNESS_def       ,READWRITE  },
{12 , fld_V_SCALE_RATIO ,VIP_V_SCALER_CONTROL       ,  0, 0xFFFFF000, 0, 0,0, fld_V_SCALE_RATIO_def     ,READWRITE  },
{1  , fld_V_DEINTERLACE_ON,VIP_V_SCALER_CONTROL     , 12, 0xFFFFEFFF, 0, 0,0, fld_V_DEINTERLACE_ON_def  ,READWRITE  },
{1  , fld_V_BYPSS       ,VIP_V_SCALER_CONTROL       , 14, 0xFFFFBFFF, 0, 0,0, fld_V_BYPSS_def           ,READWRITE  },
{1  , fld_V_DITHER_ON   ,VIP_V_SCALER_CONTROL       , 15, 0xFFFF7FFF, 0, 0,0, fld_V_DITHER_ON_def       ,READWRITE  },
{11 , fld_EVENF_OFFSET  ,VIP_V_DEINTERLACE_CONTROL  ,  0, 0xFFFFF800, 0, 0,0, fld_EVENF_OFFSET_def      ,READWRITE  },
{11 , fld_ODDF_OFFSET   ,VIP_V_DEINTERLACE_CONTROL  , 11, 0xFFC007FF, 0, 0,0, fld_ODDF_OFFSET_def       ,READWRITE  },
{1  , fld_INTERLACE_DETECTED    ,VIP_VS_LINE_COUNT  , 15, 0xFFFF7FFF, 0, 0,0, fld_INTERLACE_DETECTED_def,READONLY   },
{10 , fld_VS_LINE_COUNT     ,VIP_VS_LINE_COUNT      ,  0, 0xFFFFFC00, 0, 0,0, fld_VS_LINE_COUNT_def     ,READONLY   },
{10 , fld_VS_DETECTED_LINES ,VIP_VS_LINE_COUNT      , 16, 0xFC00FFFF, 0, 0,0, fld_VS_DETECTED_LINES_def ,READONLY   },
{1  , fld_VS_ITU656_VB  ,VIP_VS_LINE_COUNT          , 13, 0xFFFFDFFF, 0, 0,0, fld_VS_ITU656_VB_def  ,READONLY   },
{16 , fld_VBI_CC_DATA   ,VIP_VBI_CC_CNTL            ,  0, 0xFFFF0000, 0, 0,0, fld_VBI_CC_DATA_def       ,READWRITE  },
{1  , fld_VBI_CC_WT     ,VIP_VBI_CC_CNTL            , 24, 0xFEFFFFFF, 0, 0,0, fld_VBI_CC_WT_def         ,READWRITE  },
{1  , fld_VBI_CC_WT_ACK ,VIP_VBI_CC_CNTL            , 25, 0xFDFFFFFF, 0, 0,0, fld_VBI_CC_WT_ACK_def     ,READONLY   },
{1  , fld_VBI_CC_HOLD   ,VIP_VBI_CC_CNTL            , 26, 0xFBFFFFFF, 0, 0,0, fld_VBI_CC_HOLD_def       ,READWRITE  },
{1  , fld_VBI_DECODE_EN ,VIP_VBI_CC_CNTL            , 31, 0x7FFFFFFF, 0, 0,0, fld_VBI_DECODE_EN_def     ,READWRITE  },
{16 , fld_VBI_CC_DTO_P  ,VIP_VBI_DTO_CNTL           ,  0, 0xFFFF0000, 0, 0,0, fld_VBI_CC_DTO_P_def      ,READWRITE  },
{16 ,fld_VBI_20BIT_DTO_P,VIP_VBI_DTO_CNTL           , 16, 0x0000FFFF, 0, 0,0, fld_VBI_20BIT_DTO_P_def   ,READWRITE  },
{7  ,fld_VBI_CC_LEVEL   ,VIP_VBI_LEVEL_CNTL         ,  0, 0xFFFFFF80, 0, 0,0, fld_VBI_CC_LEVEL_def      ,READWRITE  },
{7  ,fld_VBI_20BIT_LEVEL,VIP_VBI_LEVEL_CNTL         ,  8, 0xFFFF80FF, 0, 0,0, fld_VBI_20BIT_LEVEL_def   ,READWRITE  },
{9  ,fld_VBI_CLK_RUNIN_GAIN,VIP_VBI_LEVEL_CNTL      , 16, 0xFE00FFFF, 0, 0,0, fld_VBI_CLK_RUNIN_GAIN_def,READWRITE  },
{11 ,fld_H_VBI_WIND_START,VIP_H_VBI_WINDOW          ,  0, 0xFFFFF800, 0, 0,0, fld_H_VBI_WIND_START_def  ,READWRITE  },
{11 ,fld_H_VBI_WIND_END,VIP_H_VBI_WINDOW            , 16, 0xF800FFFF, 0, 0,0, fld_H_VBI_WIND_END_def    ,READWRITE  },
{10 ,fld_V_VBI_WIND_START,VIP_V_VBI_WINDOW          ,  0, 0xFFFFFC00, 0, 0,0, fld_V_VBI_WIND_START_def  ,READWRITE  },
{10 ,fld_V_VBI_WIND_END,VIP_V_VBI_WINDOW            , 16, 0xFC00FFFF, 0, 0,0, fld_V_VBI_WIND_END_def    ,READWRITE  }, /* CHK */
{16 ,fld_VBI_20BIT_DATA0,VIP_VBI_20BIT_CNTL         ,  0, 0xFFFF0000, 0, 0,0, fld_VBI_20BIT_DATA0_def   ,READWRITE  },
{4  ,fld_VBI_20BIT_DATA1,VIP_VBI_20BIT_CNTL         , 16, 0xFFF0FFFF, 0, 0,0, fld_VBI_20BIT_DATA1_def   ,READWRITE  },
{1  ,fld_VBI_20BIT_WT   ,VIP_VBI_20BIT_CNTL         , 24, 0xFEFFFFFF, 0, 0,0, fld_VBI_20BIT_WT_def      ,READWRITE  },
{1  ,fld_VBI_20BIT_WT_ACK   ,VIP_VBI_20BIT_CNTL     , 25, 0xFDFFFFFF, 0, 0,0, fld_VBI_20BIT_WT_ACK_def  ,READONLY   },
{1  ,fld_VBI_20BIT_HOLD ,VIP_VBI_20BIT_CNTL         , 26, 0xFBFFFFFF, 0, 0,0, fld_VBI_20BIT_HOLD_def    ,READWRITE  },
{2  ,fld_VBI_CAPTURE_ENABLE ,VIP_VBI_CONTROL        ,  0, 0xFFFFFFFC, 0, 0,0, fld_VBI_CAPTURE_ENABLE_def,READWRITE  },
{16 ,fld_VBI_EDS_DATA   ,VIP_VBI_EDS_CNTL           ,  0, 0xFFFF0000, 0, 0,0, fld_VBI_EDS_DATA_def      ,READWRITE  },
{1  ,fld_VBI_EDS_WT     ,VIP_VBI_EDS_CNTL           , 24, 0xFEFFFFFF, 0, 0,0, fld_VBI_EDS_WT_def        ,READWRITE  },
{1  ,fld_VBI_EDS_WT_ACK ,VIP_VBI_EDS_CNTL           , 25, 0xFDFFFFFF, 0, 0,0, fld_VBI_EDS_WT_ACK_def    ,READONLY   },
{1  ,fld_VBI_EDS_HOLD   ,VIP_VBI_EDS_CNTL           , 26, 0xFBFFFFFF, 0, 0,0, fld_VBI_EDS_HOLD_def      ,READWRITE  },
{17 ,fld_VBI_SCALING_RATIO  ,VIP_VBI_SCALER_CONTROL ,  0, 0xFFFE0000, 0, 0,0, fld_VBI_SCALING_RATIO_def ,READWRITE  },
{1  ,fld_VBI_ALIGNER_ENABLE ,VIP_VBI_SCALER_CONTROL , 17, 0xFFFDFFFF, 0, 0,0, fld_VBI_ALIGNER_ENABLE_def,READWRITE  },
{11 ,fld_H_ACTIVE_START ,VIP_H_ACTIVE_WINDOW        ,  0, 0xFFFFF800, 0, 0,0, fld_H_ACTIVE_START_def    ,READWRITE  },
{11 ,fld_H_ACTIVE_END   ,VIP_H_ACTIVE_WINDOW        , 16, 0xF800FFFF, 0, 0,0, fld_H_ACTIVE_END_def      ,READWRITE  },
{10 ,fld_V_ACTIVE_START ,VIP_V_ACTIVE_WINDOW        ,  0, 0xFFFFFC00, 0, 0,0, fld_V_ACTIVE_START_def    ,READWRITE  },
{10 ,fld_V_ACTIVE_END   ,VIP_V_ACTIVE_WINDOW        , 16, 0xFC00FFFF, 0, 0,0, fld_V_ACTIVE_END_def      ,READWRITE  },
{8  ,fld_CH_HEIGHT          ,VIP_CP_AGC_CNTL        ,  0, 0xFFFFFF00, 0, 0,0, fld_CH_HEIGHT_def         ,READWRITE  },
{8  ,fld_CH_KILL_LEVEL      ,VIP_CP_AGC_CNTL        ,  8, 0xFFFF00FF, 0, 0,0, fld_CH_KILL_LEVEL_def     ,READWRITE  },
{2  ,fld_CH_AGC_ERROR_LIM   ,VIP_CP_AGC_CNTL        , 16, 0xFFFCFFFF, 0, 0,0, fld_CH_AGC_ERROR_LIM_def  ,READWRITE  },
{1  ,fld_CH_AGC_FILTER_EN   ,VIP_CP_AGC_CNTL        , 18, 0xFFFBFFFF, 0, 0,0, fld_CH_AGC_FILTER_EN_def  ,READWRITE  },
{1  ,fld_CH_AGC_LOOP_SPEED  ,VIP_CP_AGC_CNTL        , 19, 0xFFF7FFFF, 0, 0,0, fld_CH_AGC_LOOP_SPEED_def ,READWRITE  },
{8  ,fld_HUE_ADJ            ,VIP_CP_HUE_CNTL        ,  0, 0xFFFFFF00, 0, 0,0, fld_HUE_ADJ_def           ,READWRITE  },
{2  ,fld_STANDARD_SEL       ,VIP_STANDARD_SELECT    ,  0, 0xFFFFFFFC, 0, 0,0, fld_STANDARD_SEL_def      ,READWRITE  },
{1  ,fld_STANDARD_YC        ,VIP_STANDARD_SELECT    ,  2, 0xFFFFFFFB, 0, 0,0, fld_STANDARD_YC_def       ,READWRITE  },
{1  ,fld_ADC_PDWN           ,VIP_ADC_CNTL           ,  7, 0xFFFFFF7F, 0, 0,0, fld_ADC_PDWN_def          ,READWRITE  },
{3  ,fld_INPUT_SELECT       ,VIP_ADC_CNTL           ,  0, 0xFFFFFFF8, 0, 0,0, fld_INPUT_SELECT_def      ,READWRITE  },
{2  ,fld_ADC_PREFLO         ,VIP_ADC_CNTL           , 24, 0xFCFFFFFF, 0, 0,0, fld_ADC_PREFLO_def        ,READWRITE  },
{8  ,fld_H_SYNC_PULSE_WIDTH ,VIP_HS_PULSE_WIDTH     ,  0, 0xFFFFFF00, 0, 0,0, fld_H_SYNC_PULSE_WIDTH_def,READONLY   },
{1  ,fld_HS_GENLOCKED       ,VIP_HS_PULSE_WIDTH     ,  8, 0xFFFFFEFF, 0, 0,0, fld_HS_GENLOCKED_def      ,READONLY   },
{1  ,fld_HS_SYNC_IN_WIN     ,VIP_HS_PULSE_WIDTH     ,  9, 0xFFFFFDFF, 0, 0,0, fld_HS_SYNC_IN_WIN_def    ,READONLY   },
{1  ,fld_VIN_ASYNC_RST      ,VIP_MASTER_CNTL        ,  5, 0xFFFFFFDF, 0, 0,0, fld_VIN_ASYNC_RST_def     ,READWRITE  },
{1  ,fld_DVS_ASYNC_RST      ,VIP_MASTER_CNTL        ,  7, 0xFFFFFF7F, 0, 0,0, fld_DVS_ASYNC_RST_def     ,READWRITE  },
{16 ,fld_VIP_VENDOR_ID      ,VIP_VIP_VENDOR_DEVICE_ID, 0, 0xFFFF0000, 0, 0,0, fld_VIP_VENDOR_ID_def     ,READONLY   },
{16 ,fld_VIP_DEVICE_ID      ,VIP_VIP_VENDOR_DEVICE_ID,16, 0x0000FFFF, 0, 0,0, fld_VIP_DEVICE_ID_def     ,READONLY   },
{16 ,fld_VIP_REVISION_ID    ,VIP_VIP_REVISION_ID    ,  0, 0xFFFF0000, 0, 0,0, fld_VIP_REVISION_ID_def   ,READONLY   },
{8  ,fld_BLACK_INT_START    ,VIP_SG_BLACK_GATE      ,  0, 0xFFFFFF00, 0, 0,0, fld_BLACK_INT_START_def   ,READWRITE  },
{4  ,fld_BLACK_INT_LENGTH   ,VIP_SG_BLACK_GATE      ,  8, 0xFFFFF0FF, 0, 0,0, fld_BLACK_INT_LENGTH_def  ,READWRITE  },
{8  ,fld_UV_INT_START       ,VIP_SG_UVGATE_GATE     ,  0, 0xFFFFFF00, 0, 0,0, fld_UV_INT_START_def      ,READWRITE  },
{4  ,fld_U_INT_LENGTH       ,VIP_SG_UVGATE_GATE     ,  8, 0xFFFFF0FF, 0, 0,0, fld_U_INT_LENGTH_def      ,READWRITE  },
{4  ,fld_V_INT_LENGTH       ,VIP_SG_UVGATE_GATE     , 12, 0xFFFF0FFF, 0, 0,0, fld_V_INT_LENGTH_def      ,READWRITE  },
{10 ,fld_CRDR_ACTIVE_GAIN   ,VIP_CP_ACTIVE_GAIN     ,  0, 0xFFFFFC00, 0, 0,0, fld_CRDR_ACTIVE_GAIN_def  ,READWRITE  },
{10 ,fld_CBDB_ACTIVE_GAIN   ,VIP_CP_ACTIVE_GAIN     , 16, 0xFC00FFFF, 0, 0,0, fld_CBDB_ACTIVE_GAIN_def  ,READWRITE  },
{1  ,fld_DVS_DIRECTION      ,VIP_DVS_PORT_CTRL      ,  0, 0xFFFFFFFE, 0, 0,0, fld_DVS_DIRECTION_def     ,READWRITE  },
{1  ,fld_DVS_VBI_UINT8_SWAP  ,VIP_DVS_PORT_CTRL      ,  1, 0xFFFFFFFD, 0, 0,0, fld_DVS_VBI_UINT8_SWAP_def ,READWRITE  },
{1  ,fld_DVS_CLK_SELECT     ,VIP_DVS_PORT_CTRL      ,  2, 0xFFFFFFFB, 0, 0,0, fld_DVS_CLK_SELECT_def    ,READWRITE  },
{1  ,fld_CONTINUOUS_STREAM  ,VIP_DVS_PORT_CTRL      ,  3, 0xFFFFFFF7, 0, 0,0, fld_CONTINUOUS_STREAM_def ,READWRITE  },
{1  ,fld_DVSOUT_CLK_DRV     ,VIP_DVS_PORT_CTRL      ,  4, 0xFFFFFFEF, 0, 0,0, fld_DVSOUT_CLK_DRV_def    ,READWRITE  },
{1  ,fld_DVSOUT_DATA_DRV    ,VIP_DVS_PORT_CTRL      ,  5, 0xFFFFFFDF, 0, 0,0, fld_DVSOUT_DATA_DRV_def   ,READWRITE  },
{32 ,fld_COMB_CNTL0         ,VIP_COMB_CNTL0         ,  0, 0x00000000, 0, 0,0, fld_COMB_CNTL0_def        ,READWRITE  },
{32 ,fld_COMB_CNTL1         ,VIP_COMB_CNTL1         ,  0, 0x00000000, 0, 0,0, fld_COMB_CNTL1_def        ,READWRITE  },
{32 ,fld_COMB_CNTL2         ,VIP_COMB_CNTL2         ,  0, 0x00000000, 0, 0,0, fld_COMB_CNTL2_def        ,READWRITE  },
{32 ,fld_COMB_LENGTH        ,VIP_COMB_LINE_LENGTH   ,  0, 0x00000000, 0, 0,0, fld_COMB_LENGTH_def       ,READWRITE  },
{8  ,fld_SYNCTIP_REF0       ,VIP_LP_AGC_CLAMP_CNTL0 ,  0, 0xFFFFFF00, 0, 0,0, fld_SYNCTIP_REF0_def      ,READWRITE  },
{8  ,fld_SYNCTIP_REF1       ,VIP_LP_AGC_CLAMP_CNTL0 ,  8, 0xFFFF00FF, 0, 0,0, fld_SYNCTIP_REF1_def      ,READWRITE  },
{8  ,fld_CLAMP_REF          ,VIP_LP_AGC_CLAMP_CNTL0 , 16, 0xFF00FFFF, 0, 0,0, fld_CLAMP_REF_def          ,READWRITE  },
{8  ,fld_AGC_PEAKWHITE      ,VIP_LP_AGC_CLAMP_CNTL0 , 24, 0x00FFFFFF, 0, 0,0, fld_AGC_PEAKWHITE_def     ,READWRITE  },
{8  ,fld_VBI_PEAKWHITE      ,VIP_LP_AGC_CLAMP_CNTL1 ,  0, 0xFFFFFF00, 0, 0,0, fld_VBI_PEAKWHITE_def     ,READWRITE  },
{11 ,fld_WPA_THRESHOLD      ,VIP_LP_WPA_CNTL0       ,  0, 0xFFFFF800, 0, 0,0, fld_WPA_THRESHOLD_def     ,READWRITE  },
{10 ,fld_WPA_TRIGGER_LO     ,VIP_LP_WPA_CNTL1       ,  0, 0xFFFFFC00, 0, 0,0, fld_WPA_TRIGGER_LO_def    ,READWRITE  },
{10 ,fld_WPA_TRIGGER_HIGH   ,VIP_LP_WPA_CNTL1       , 16, 0xFC00FFFF, 0, 0,0, fld_WPA_TRIGGER_HIGH_def  ,READWRITE  },
{10 ,fld_LOCKOUT_START      ,VIP_LP_VERT_LOCKOUT    ,  0, 0xFFFFFC00, 0, 0,0, fld_LOCKOUT_START_def     ,READWRITE  },
{10 ,fld_LOCKOUT_END        ,VIP_LP_VERT_LOCKOUT    , 16, 0xFC00FFFF, 0, 0,0, fld_LOCKOUT_END_def       ,READWRITE  },
{24 ,fld_CH_DTO_INC         ,VIP_CP_PLL_CNTL0       ,  0, 0xFF000000, 0, 0,0, fld_CH_DTO_INC_def        ,READWRITE  },
{4  ,fld_PLL_SGAIN          ,VIP_CP_PLL_CNTL0       , 24, 0xF0FFFFFF, 0, 0,0, fld_PLL_SGAIN_def         ,READWRITE  },
{4  ,fld_PLL_FGAIN          ,VIP_CP_PLL_CNTL0       , 28, 0x0FFFFFFF, 0, 0,0, fld_PLL_FGAIN_def         ,READWRITE  },
{9  ,fld_CR_BURST_GAIN      ,VIP_CP_BURST_GAIN      ,  0, 0xFFFFFE00, 0, 0,0, fld_CR_BURST_GAIN_def     ,READWRITE  },
{9  ,fld_CB_BURST_GAIN      ,VIP_CP_BURST_GAIN      , 16, 0xFE00FFFF, 0, 0,0, fld_CB_BURST_GAIN_def     ,READWRITE  },
{10 ,fld_VERT_LOCKOUT_START ,VIP_CP_VERT_LOCKOUT    ,  0, 0xFFFFFC00, 0, 0,0, fld_VERT_LOCKOUT_START_def,READWRITE  },
{10 ,fld_VERT_LOCKOUT_END   ,VIP_CP_VERT_LOCKOUT    , 16, 0xFC00FFFF, 0, 0,0, fld_VERT_LOCKOUT_END_def  ,READWRITE  },
{11 ,fld_H_IN_WIND_START    ,VIP_SCALER_IN_WINDOW   ,  0, 0xFFFFF800, 0, 0,0, fld_H_IN_WIND_START_def   ,READWRITE  },
{10 ,fld_V_IN_WIND_START    ,VIP_SCALER_IN_WINDOW   , 16, 0xFC00FFFF, 0, 0,0, fld_V_IN_WIND_START_def   ,READWRITE  },
{10 ,fld_H_OUT_WIND_WIDTH   ,VIP_SCALER_OUT_WINDOW ,  0, 0xFFFFFC00, 0, 0,0, fld_H_OUT_WIND_WIDTH_def   ,READWRITE  },
{9  ,fld_V_OUT_WIND_WIDTH   ,VIP_SCALER_OUT_WINDOW , 16, 0xFE00FFFF, 0, 0,0, fld_V_OUT_WIND_WIDTH_def   ,READWRITE  },
{11 ,fld_HS_LINE_TOTAL      ,VIP_HS_PLINE          ,  0, 0xFFFFF800, 0, 0,0, fld_HS_LINE_TOTAL_def      ,READWRITE  },
{8  ,fld_MIN_PULSE_WIDTH    ,VIP_HS_MINMAXWIDTH    ,  0, 0xFFFFFF00, 0, 0,0, fld_MIN_PULSE_WIDTH_def    ,READWRITE  },
{8  ,fld_MAX_PULSE_WIDTH    ,VIP_HS_MINMAXWIDTH    ,  8, 0xFFFF00FF, 0, 0,0, fld_MAX_PULSE_WIDTH_def    ,READWRITE  },
{11 ,fld_WIN_CLOSE_LIMIT    ,VIP_HS_WINDOW_LIMIT   ,  0, 0xFFFFF800, 0, 0,0, fld_WIN_CLOSE_LIMIT_def    ,READWRITE  },
{11 ,fld_WIN_OPEN_LIMIT     ,VIP_HS_WINDOW_LIMIT   , 16, 0xF800FFFF, 0, 0,0, fld_WIN_OPEN_LIMIT_def     ,READWRITE  },
{11 ,fld_VSYNC_INT_TRIGGER  ,VIP_VS_DETECTOR_CNTL   ,  0, 0xFFFFF800, 0, 0,0, fld_VSYNC_INT_TRIGGER_def ,READWRITE  },
{11 ,fld_VSYNC_INT_HOLD     ,VIP_VS_DETECTOR_CNTL   , 16, 0xF800FFFF, 0, 0,0, fld_VSYNC_INT_HOLD_def        ,READWRITE  },
{11 ,fld_VIN_M0             ,VIP_VIN_PLL_CNTL      ,  0, 0xFFFFF800, 0, 0,0, fld_VIN_M0_def             ,READWRITE  },
{11 ,fld_VIN_N0             ,VIP_VIN_PLL_CNTL      , 11, 0xFFC007FF, 0, 0,0, fld_VIN_N0_def             ,READWRITE  },
{1  ,fld_MNFLIP_EN          ,VIP_VIN_PLL_CNTL      , 22, 0xFFBFFFFF, 0, 0,0, fld_MNFLIP_EN_def          ,READWRITE  },
{4  ,fld_VIN_P              ,VIP_VIN_PLL_CNTL      , 24, 0xF0FFFFFF, 0, 0,0, fld_VIN_P_def              ,READWRITE  },
{2  ,fld_REG_CLK_SEL        ,VIP_VIN_PLL_CNTL      , 30, 0x3FFFFFFF, 0, 0,0, fld_REG_CLK_SEL_def        ,READWRITE  },
{11 ,fld_VIN_M1             ,VIP_VIN_PLL_FINE_CNTL  ,  0, 0xFFFFF800, 0, 0,0, fld_VIN_M1_def            ,READWRITE  },
{11 ,fld_VIN_N1             ,VIP_VIN_PLL_FINE_CNTL  , 11, 0xFFC007FF, 0, 0,0, fld_VIN_N1_def            ,READWRITE  },
{1  ,fld_VIN_DRIVER_SEL     ,VIP_VIN_PLL_FINE_CNTL  , 22, 0xFFBFFFFF, 0, 0,0, fld_VIN_DRIVER_SEL_def    ,READWRITE  },
{1  ,fld_VIN_MNFLIP_REQ     ,VIP_VIN_PLL_FINE_CNTL  , 23, 0xFF7FFFFF, 0, 0,0, fld_VIN_MNFLIP_REQ_def    ,READWRITE  },
{1  ,fld_VIN_MNFLIP_DONE    ,VIP_VIN_PLL_FINE_CNTL  , 24, 0xFEFFFFFF, 0, 0,0, fld_VIN_MNFLIP_DONE_def   ,READONLY   },
{1  ,fld_TV_LOCK_TO_VIN     ,VIP_VIN_PLL_FINE_CNTL  , 27, 0xF7FFFFFF, 0, 0,0, fld_TV_LOCK_TO_VIN_def    ,READWRITE  },
{4  ,fld_TV_P_FOR_WINCLK    ,VIP_VIN_PLL_FINE_CNTL  , 28, 0x0FFFFFFF, 0, 0,0, fld_TV_P_FOR_WINCLK_def   ,READWRITE  },
{1  ,fld_VINRST             ,VIP_PLL_CNTL1          ,  1, 0xFFFFFFFD, 0, 0,0, fld_VINRST_def            ,READWRITE  },
{1  ,fld_VIN_CLK_SEL        ,VIP_CLOCK_SEL_CNTL     ,  7, 0xFFFFFF7F, 0, 0,0, fld_VIN_CLK_SEL_def       ,READWRITE  },
{10 ,fld_VS_FIELD_BLANK_START,VIP_VS_BLANKING_CNTL  ,  0, 0xFFFFFC00, 0, 0,0, fld_VS_FIELD_BLANK_START_def  ,READWRITE  },
{10 ,fld_VS_FIELD_BLANK_END,VIP_VS_BLANKING_CNTL    , 16, 0xFC00FFFF, 0, 0,0, fld_VS_FIELD_BLANK_END_def    ,READWRITE  },
{9  ,fld_VS_FIELD_IDLOCATION,VIP_VS_FIELD_ID_CNTL   ,  0, 0xFFFFFE00, 0, 0,0, fld_VS_FIELD_IDLOCATION_def   ,READWRITE  },
{10 ,fld_VS_FRAME_TOTAL     ,VIP_VS_FRAME_TOTAL     ,  0, 0xFFFFFC00, 0, 0,0, fld_VS_FRAME_TOTAL_def    ,READWRITE  },
{11 ,fld_SYNC_TIP_START     ,VIP_SG_SYNCTIP_GATE    ,  0, 0xFFFFF800, 0, 0,0, fld_SYNC_TIP_START_def    ,READWRITE  },
{4  ,fld_SYNC_TIP_LENGTH    ,VIP_SG_SYNCTIP_GATE    , 12, 0xFFFF0FFF, 0, 0,0, fld_SYNC_TIP_LENGTH_def   ,READWRITE  },
{12 ,fld_GAIN_FORCE_DATA    ,VIP_CP_DEBUG_FORCE     ,  0, 0xFFFFF000, 0, 0,0, fld_GAIN_FORCE_DATA_def   ,READWRITE  },
{1  ,fld_GAIN_FORCE_EN      ,VIP_CP_DEBUG_FORCE     , 12, 0xFFFFEFFF, 0, 0,0, fld_GAIN_FORCE_EN_def ,READWRITE  },
{2  ,fld_I_CLAMP_SEL        ,VIP_ADC_CNTL           ,  3, 0xFFFFFFE7, 0, 0,0, fld_I_CLAMP_SEL_def   ,READWRITE  },
{2  ,fld_I_AGC_SEL          ,VIP_ADC_CNTL           ,  5, 0xFFFFFF9F, 0, 0,0, fld_I_AGC_SEL_def     ,READWRITE  },
{1  ,fld_EXT_CLAMP_CAP      ,VIP_ADC_CNTL           ,  8, 0xFFFFFEFF, 0, 0,0, fld_EXT_CLAMP_CAP_def ,READWRITE  },
{1  ,fld_EXT_AGC_CAP        ,VIP_ADC_CNTL           ,  9, 0xFFFFFDFF, 0, 0,0, fld_EXT_AGC_CAP_def       ,READWRITE  },
{1  ,fld_DECI_DITHER_EN     ,VIP_ADC_CNTL           , 12, 0xFFFFEFFF, 0, 0,0, fld_DECI_DITHER_EN_def ,READWRITE },
{2  ,fld_ADC_PREFHI         ,VIP_ADC_CNTL           , 22, 0xFF3FFFFF, 0, 0,0, fld_ADC_PREFHI_def        ,READWRITE  },
{2  ,fld_ADC_CH_GAIN_SEL    ,VIP_ADC_CNTL           , 16, 0xFFFCFFFF, 0, 0,0, fld_ADC_CH_GAIN_SEL_def   ,READWRITE  },
{4  ,fld_HS_PLL_SGAIN       ,VIP_HS_PLLGAIN         ,  0, 0xFFFFFFF0, 0, 0,0, fld_HS_PLL_SGAIN_def      ,READWRITE  },
{1  ,fld_NREn               ,VIP_NOISE_CNTL0        ,  0, 0xFFFFFFFE, 0, 0,0, fld_NREn_def      ,READWRITE  },
{3  ,fld_NRGainCntl         ,VIP_NOISE_CNTL0        ,  1, 0xFFFFFFF1, 0, 0,0, fld_NRGainCntl_def        ,READWRITE  },
{6  ,fld_NRBWTresh          ,VIP_NOISE_CNTL0        ,  4, 0xFFFFFC0F, 0, 0,0, fld_NRBWTresh_def     ,READWRITE  },
{5  ,fld_NRGCTresh          ,VIP_NOISE_CNTL0       ,  10, 0xFFFF83FF, 0, 0,0, fld_NRGCTresh_def     ,READWRITE  },
{1  ,fld_NRCoefDespeclMode  ,VIP_NOISE_CNTL0       ,  15, 0xFFFF7FFF, 0, 0,0, fld_NRCoefDespeclMode_def     ,READWRITE  },
{1  ,fld_GPIO_5_OE      ,VIP_GPIO_CNTL      ,  5, 0xFFFFFFDF, 0, 0,0, fld_GPIO_5_OE_def     ,READWRITE  },
{1  ,fld_GPIO_6_OE      ,VIP_GPIO_CNTL      ,  6, 0xFFFFFFBF, 0, 0,0, fld_GPIO_6_OE_def     ,READWRITE  },
{1  ,fld_GPIO_5_OUT     ,VIP_GPIO_INOUT    ,   5, 0xFFFFFFDF, 0, 0,0, fld_GPIO_5_OUT_def        ,READWRITE  },
{1  ,fld_GPIO_6_OUT     ,VIP_GPIO_INOUT    ,   6, 0xFFFFFFBF, 0, 0,0, fld_GPIO_6_OUT_def        ,READWRITE  },
};

/* Rage Theatre's register fields default values: */
uint32_t RT_RegDef[regRT_MAX_REGS]=
{
fld_tmpReg1_def,
fld_tmpReg2_def,
fld_tmpReg3_def,
fld_LP_CONTRAST_def,
fld_LP_BRIGHTNESS_def,
fld_CP_HUE_CNTL_def,
fld_LUMA_FILTER_def,
fld_H_SCALE_RATIO_def,
fld_H_SHARPNESS_def,
fld_V_SCALE_RATIO_def,
fld_V_DEINTERLACE_ON_def,
fld_V_BYPSS_def,
fld_V_DITHER_ON_def,
fld_EVENF_OFFSET_def,
fld_ODDF_OFFSET_def,
fld_INTERLACE_DETECTED_def,
fld_VS_LINE_COUNT_def,
fld_VS_DETECTED_LINES_def,
fld_VS_ITU656_VB_def,
fld_VBI_CC_DATA_def,
fld_VBI_CC_WT_def,
fld_VBI_CC_WT_ACK_def,
fld_VBI_CC_HOLD_def,
fld_VBI_DECODE_EN_def,
fld_VBI_CC_DTO_P_def,
fld_VBI_20BIT_DTO_P_def,
fld_VBI_CC_LEVEL_def,
fld_VBI_20BIT_LEVEL_def,
fld_VBI_CLK_RUNIN_GAIN_def,
fld_H_VBI_WIND_START_def,
fld_H_VBI_WIND_END_def,
fld_V_VBI_WIND_START_def,
fld_V_VBI_WIND_END_def,
fld_VBI_20BIT_DATA0_def,
fld_VBI_20BIT_DATA1_def,
fld_VBI_20BIT_WT_def,
fld_VBI_20BIT_WT_ACK_def,
fld_VBI_20BIT_HOLD_def,
fld_VBI_CAPTURE_ENABLE_def,
fld_VBI_EDS_DATA_def,
fld_VBI_EDS_WT_def,
fld_VBI_EDS_WT_ACK_def,
fld_VBI_EDS_HOLD_def,
fld_VBI_SCALING_RATIO_def,
fld_VBI_ALIGNER_ENABLE_def,
fld_H_ACTIVE_START_def,
fld_H_ACTIVE_END_def,
fld_V_ACTIVE_START_def,
fld_V_ACTIVE_END_def,
fld_CH_HEIGHT_def,
fld_CH_KILL_LEVEL_def,
fld_CH_AGC_ERROR_LIM_def,
fld_CH_AGC_FILTER_EN_def,
fld_CH_AGC_LOOP_SPEED_def,
fld_HUE_ADJ_def,
fld_STANDARD_SEL_def,
fld_STANDARD_YC_def,
fld_ADC_PDWN_def,
fld_INPUT_SELECT_def,
fld_ADC_PREFLO_def,
fld_H_SYNC_PULSE_WIDTH_def,
fld_HS_GENLOCKED_def,
fld_HS_SYNC_IN_WIN_def,
fld_VIN_ASYNC_RST_def,
fld_DVS_ASYNC_RST_def,
fld_VIP_VENDOR_ID_def,
fld_VIP_DEVICE_ID_def,
fld_VIP_REVISION_ID_def,
fld_BLACK_INT_START_def,
fld_BLACK_INT_LENGTH_def,
fld_UV_INT_START_def,
fld_U_INT_LENGTH_def,
fld_V_INT_LENGTH_def,
fld_CRDR_ACTIVE_GAIN_def,
fld_CBDB_ACTIVE_GAIN_def,
fld_DVS_DIRECTION_def,
fld_DVS_VBI_UINT8_SWAP_def,
fld_DVS_CLK_SELECT_def,
fld_CONTINUOUS_STREAM_def,
fld_DVSOUT_CLK_DRV_def,
fld_DVSOUT_DATA_DRV_def,
fld_COMB_CNTL0_def,
fld_COMB_CNTL1_def,
fld_COMB_CNTL2_def,
fld_COMB_LENGTH_def,
fld_SYNCTIP_REF0_def,
fld_SYNCTIP_REF1_def,
fld_CLAMP_REF_def,
fld_AGC_PEAKWHITE_def,
fld_VBI_PEAKWHITE_def,
fld_WPA_THRESHOLD_def,
fld_WPA_TRIGGER_LO_def,
fld_WPA_TRIGGER_HIGH_def,
fld_LOCKOUT_START_def,
fld_LOCKOUT_END_def,
fld_CH_DTO_INC_def,
fld_PLL_SGAIN_def,
fld_PLL_FGAIN_def,
fld_CR_BURST_GAIN_def,
fld_CB_BURST_GAIN_def,
fld_VERT_LOCKOUT_START_def,
fld_VERT_LOCKOUT_END_def,
fld_H_IN_WIND_START_def,
fld_V_IN_WIND_START_def,
fld_H_OUT_WIND_WIDTH_def,
fld_V_OUT_WIND_WIDTH_def,
fld_HS_LINE_TOTAL_def,
fld_MIN_PULSE_WIDTH_def,
fld_MAX_PULSE_WIDTH_def,
fld_WIN_CLOSE_LIMIT_def,
fld_WIN_OPEN_LIMIT_def,
fld_VSYNC_INT_TRIGGER_def,
fld_VSYNC_INT_HOLD_def,
fld_VIN_M0_def,
fld_VIN_N0_def,
fld_MNFLIP_EN_def,
fld_VIN_P_def,
fld_REG_CLK_SEL_def,
fld_VIN_M1_def,
fld_VIN_N1_def,
fld_VIN_DRIVER_SEL_def,
fld_VIN_MNFLIP_REQ_def,
fld_VIN_MNFLIP_DONE_def,
fld_TV_LOCK_TO_VIN_def,
fld_TV_P_FOR_WINCLK_def,
fld_VINRST_def,
fld_VIN_CLK_SEL_def,
fld_VS_FIELD_BLANK_START_def,
fld_VS_FIELD_BLANK_END_def,
fld_VS_FIELD_IDLOCATION_def,
fld_VS_FRAME_TOTAL_def,
fld_SYNC_TIP_START_def,
fld_SYNC_TIP_LENGTH_def,
fld_GAIN_FORCE_DATA_def,
fld_GAIN_FORCE_EN_def,
fld_I_CLAMP_SEL_def,
fld_I_AGC_SEL_def,
fld_EXT_CLAMP_CAP_def,
fld_EXT_AGC_CAP_def,
fld_DECI_DITHER_EN_def,
fld_ADC_PREFHI_def,
fld_ADC_CH_GAIN_SEL_def,
fld_HS_PLL_SGAIN_def,
fld_NREn_def,
fld_NRGainCntl_def,
fld_NRBWTresh_def,
fld_NRGCTresh_def,
fld_NRCoefDespeclMode_def,
fld_GPIO_5_OE_def,
fld_GPIO_6_OE_def,
fld_GPIO_5_OUT_def,
fld_GPIO_6_OUT_def,
};

/****************************************************************************
 * WriteRT_fld (uint32_t dwReg, uint32_t dwData)                                  *
 *  Function: Writes a register field within Rage Theatre                   *
 *    Inputs: uint32_t dwReg = register field to be written                    *
 *            uint32_t dwData = data that will be written to the reg field     *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
static void WriteRT_fld1 (TheatrePtr t, uint32_t dwReg, uint32_t dwData)
{
    uint32_t dwResult=0;
    uint32_t dwValue=0;

    if (RT_regr (RT_RegMap[dwReg].dwRegAddrLSBs, &dwResult) == TRUE)
    {
        dwValue = (dwResult & RT_RegMap[dwReg].dwMaskLSBs) |
                  (dwData << RT_RegMap[dwReg].dwFldOffsetLSBs);

        if (RT_regw (RT_RegMap[dwReg].dwRegAddrLSBs, dwValue) == TRUE)
        {
            /* update the memory mapped registers */
            RT_RegMap[dwReg].dwCurrValue = dwData;
        }

    }

    return;

} /* WriteRT_fld ()... */

/****************************************************************************
 * ReadRT_fld (uint32_t dwReg)                                                 *
 *  Function: Reads a register field within Rage Theatre                    *
 *    Inputs: uint32_t dwReg = register field to be read                       *
 *   Outputs: uint32_t - value read from register field                        *
 ****************************************************************************/
static uint32_t ReadRT_fld1 (TheatrePtr t,uint32_t dwReg)
{
    uint32_t dwResult=0;

    if (RT_regr (RT_RegMap[dwReg].dwRegAddrLSBs, &dwResult) == TRUE)
    {
        RT_RegMap[dwReg].dwCurrValue = ((dwResult & ~RT_RegMap[dwReg].dwMaskLSBs) >>
                                                            RT_RegMap[dwReg].dwFldOffsetLSBs);
        return (RT_RegMap[dwReg].dwCurrValue);
    }
    else
    {
        return (0xFFFFFFFF);
    }

} /* ReadRT_fld ()... */

#define WriteRT_fld(a,b)   WriteRT_fld1(t, (a), (b))
#define ReadRT_fld(a)	   ReadRT_fld1(t,(a))

/****************************************************************************
 * RT_SetVINClock (uint16_t wStandard)                                          *
 *  Function: to set the VIN clock for the selected standard                *
 *    Inputs: uint16_t wStandard - input standard (NTSC, PAL, SECAM)            *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
static void RT_SetVINClock(TheatrePtr t, uint16_t wStandard)
{
    uint32_t dwM0=0, dwN0=0, dwP=0;
    uint8_t ref_freq;

    /* Determine the reference frequency first.  This can be obtained
       from the MMTABLE.video_decoder_type field (bits 4:7)
       The Rage Theatre currently only supports reference frequencies of
       27 or 29.49 MHz. */
       /*
    R128ReadBIOS(0x48,
		     (uint8_t *)&bios_header, sizeof(bios_header));
     R128ReadBIOS(bios_header + 0x30,
		     (uint8_t *)&pll_info_block, sizeof(pll_info_block));

     R128ReadBIOS(pll_info_block+0x07, &video_decoder_type, sizeof(video_decoder_type)); 
      */ 
     ref_freq = (t->video_decoder_type & 0xF0) >> 4; 
       
       
    switch (wStandard & 0x00FF)
    {
        case (DEC_NTSC): /* NTSC GROUP - 480 lines */
            switch (wStandard & 0xFF00)
            {
                case  (extNONE):
                case  (extNTSC):
                case  (extNTSC_J):
                    if (ref_freq == RT_FREF_2950)
                    {
                        dwM0 =  0x39;
                        dwN0 =  0x14C;
                        dwP  =  0x6;
                    }
                    else
                    {
                        dwM0 =  0x0B;
                        dwN0 =  0x46;
                        dwP  =  0x6;
                    }
                    break;

                case  (extNTSC_443):
                    if (ref_freq == RT_FREF_2950)
                    {
                        dwM0 =  0x23;
                        dwN0 =  0x88;
                        dwP  =  0x7;
                    }
                    else
                    {
                        dwM0 =  0x2C;
                        dwN0 =  0x121;
                        dwP  =  0x5;
                    }
                    break;

                case (extPAL_M):
                    if (ref_freq == RT_FREF_2950)
                    {
                        dwM0 =  0x2C;
                        dwN0 =  0x12B;
                        dwP  =  0x7;
                    }
                    else
                    {
                        dwM0 =  0x0B;
                        dwN0 =  0x46;
                        dwP  =  0x6;
                    }
                    break;

                default:
                    return;
            }
            break;
        case (DEC_PAL):
            switch (wStandard & 0xFF00)
            {
	    	case (extPAL):
                case (extPAL_N):
                case (extPAL_BGHI):
                case (extPAL_60):
                    if (ref_freq == RT_FREF_2950)
                    {
                        dwM0 = 0x0E;
                        dwN0 = 0x65;
                        dwP  = 0x6;
                    }
                    else
                    {
                        dwM0 = 0x2C;
                        dwN0 = 0x0121;
                        dwP  = 0x5;
                    }
                    break;

                case (extPAL_NCOMB):
                    if (ref_freq == RT_FREF_2950)
                    {
                        dwM0 = 0x23;
                        dwN0 = 0x88;
                        dwP  = 0x7;
                    }
                    else
                    {
                        dwM0 = 0x37;
                        dwN0 = 0x1D3;
                        dwP  = 0x8;
                    }
                    break;

                default:
                    return;
            }
            break;

        case (DEC_SECAM):
            if (ref_freq == RT_FREF_2950)
            {
                dwM0 =  0xE;
                dwN0 =  0x65;
                dwP  =  0x6;
            }
            else
            {
                dwM0 =  0x2C;
                dwN0 =  0x121;
                dwP  =  0x5;
            }
            break;
    }

    /* VIN_PLL_CNTL */
    WriteRT_fld (fld_VIN_M0, dwM0);
    WriteRT_fld (fld_VIN_N0, dwN0);
    WriteRT_fld (fld_VIN_P, dwP);

    return;
} /* RT_SetVINClock ()... */

/****************************************************************************
 * RT_SetTint (int hue)                                                     *
 *  Function: sets the tint (hue) for the Rage Theatre video in             *
 *    Inputs: int hue - the hue value to be set.                            *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetTint (TheatrePtr t, int hue)
{
    uint32_t nhue = 0;

    t->iHue=hue;
    /* Scale hue value from -1000<->1000 to -180<->180 */
    hue = (double)(hue+1000) * 0.18 - 180;

    /* Validate Hue level */
    if (hue < -180)
    {
        hue = -180;
    }
    else if (hue > 180)
    {
        hue = 180;
    }

    /* save the "validated" hue, but scale it back up to -1000<->1000 */
    t->iHue = (double)hue/0.18;

    switch (t->wStandard & 0x00FF)
    {
        case (DEC_NTSC): /* original ATI code had _empty_ section for PAL/SECAM... which did not work,
	                    obviously */
        case (DEC_PAL):
        case (DEC_SECAM):
                            if (hue >= 0)
                            {
                                nhue = (uint32_t) (256 * hue)/360;
                            }
                            else
                            {
                                nhue = (uint32_t) (256 * (hue + 360))/360;
                            }
                            break;

        default:            break;
    }

    WriteRT_fld(fld_CP_HUE_CNTL, nhue);

    return;

} /* RT_SetTint ()... */


/****************************************************************************
 * RT_SetSaturation (int Saturation)                                        *
 *  Function: sets the saturation level for the Rage Theatre video in       *
 *    Inputs: int Saturation - the saturation value to be set.              *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetSaturation (TheatrePtr t, int Saturation)
{
    uint16_t   wSaturation_V, wSaturation_U;
    double dbSaturation = 0, dbCrGain = 0, dbCbGain = 0;

    /* VALIDATE SATURATION LEVEL */
    if (Saturation < -1000L)
    {
        Saturation = -1000;
    }
    else if (Saturation > 1000L)
    {
        Saturation = 1000;
    }

    t->iSaturation = Saturation;

    if (Saturation > 0)
    {
        /* Scale saturation up, to use full allowable register width */
        Saturation = (double)(Saturation) * 4.9;
    }

    dbSaturation = (double) (Saturation+1000.0) / 1000.0;

    CalculateCrCbGain (t, &dbCrGain, &dbCbGain, t->wStandard);

    wSaturation_U = (uint16_t) ((dbCrGain * dbSaturation * 128.0) + 0.5);
    wSaturation_V = (uint16_t) ((dbCbGain * dbSaturation * 128.0) + 0.5);

    /* SET SATURATION LEVEL */
    WriteRT_fld (fld_CRDR_ACTIVE_GAIN, wSaturation_U);
    WriteRT_fld (fld_CBDB_ACTIVE_GAIN, wSaturation_V);

    t->wSaturation_U = wSaturation_U;
    t->wSaturation_V = wSaturation_V;

    return;

} /* RT_SetSaturation ()...*/

/****************************************************************************
 * RT_SetBrightness (int Brightness)                                        *
 *  Function: sets the brightness level for the Rage Theatre video in       *
 *    Inputs: int Brightness - the brightness value to be set.              *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetBrightness (TheatrePtr t, int Brightness)
{
    double dbSynctipRef0=0, dbContrast=1;

    double dbYgain=0;
    double dbBrightness=0;
    double dbSetup=0;
    uint16_t   wBrightness=0;

    /* VALIDATE BRIGHTNESS LEVEL */
    if (Brightness < -1000)
    {
        Brightness = -1000;
    }
    else if (Brightness > 1000)
    {
        Brightness = 1000;
    }

    /* Save value */
    t->iBrightness = Brightness;

    t->dbBrightnessRatio =  (double) (Brightness+1000.0) / 10.0;

    dbBrightness = (double) (Brightness)/10.0;

    dbSynctipRef0 = ReadRT_fld (fld_SYNCTIP_REF0);

    if(t->dbContrast == 0)
    {
        t->dbContrast = 1.0; /*NTSC default; */
    }

    dbContrast = (double) t->dbContrast;

    /* Use the following formula to determine the brightness level */
    switch (t->wStandard & 0x00FF)
    {
        case (DEC_NTSC):
            if ((t->wStandard & 0xFF00) == extNTSC_J)
            {
                dbYgain = 219.0 / ( 100.0 * (double)(dbSynctipRef0) /40.0);
            }
            else
            {
                dbSetup = 7.5 * (double)(dbSynctipRef0) / 40.0;
                dbYgain = 219.0 / (92.5 * (double)(dbSynctipRef0) / 40.0);
            }
            break;
        case (DEC_PAL):
        case (DEC_SECAM):
            dbYgain = 219.0 / ( 100.0 * (double)(dbSynctipRef0) /43.0);
            break;
        default:
            break;
    }

    wBrightness = (uint16_t) (16.0 * ((dbBrightness-dbSetup) + (16.0 / (dbContrast * dbYgain))));

    WriteRT_fld (fld_LP_BRIGHTNESS, wBrightness);

    /*RT_SetSaturation (t->iSaturation); */

    return;

} /* RT_SetBrightness ()... */


/****************************************************************************
 * RT_SetSharpness (uint16_t wSharpness)                                        *
 *  Function: sets the sharpness level for the Rage Theatre video in        *
 *    Inputs: uint16_t wSharpness - the sharpness value to be set.              *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetSharpness (TheatrePtr t, uint16_t wSharpness)
{
    switch (wSharpness)
    {
        case DEC_SMOOTH :
            WriteRT_fld (fld_H_SHARPNESS, RT_NORM_SHARPNESS);
            t->wSharpness = RT_NORM_SHARPNESS;
            break;
        case DEC_SHARP  :
            WriteRT_fld (fld_H_SHARPNESS, RT_HIGH_SHARPNESS);
            t->wSharpness = RT_HIGH_SHARPNESS;
            break;
        default:
            break;
    }
    return;

} /* RT_SetSharpness ()... */


/****************************************************************************
 * RT_SetContrast (int Contrast)                                            *
 *  Function: sets the contrast level for the Rage Theatre video in         *
 *    Inputs: int Contrast - the contrast value to be set.                  *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetContrast (TheatrePtr t, int Contrast)
{
    double dbSynctipRef0=0, dbContrast=0;
    double dbYgain=0;
    uint8_t   bTempContrast=0;

    /* VALIDATE CONTRAST LEVEL */
    if (Contrast < -1000)
    {
        Contrast = -1000;
    }
    else if (Contrast > 1000)
    {
        Contrast = 1000;
    }

    /* Save contrast value */
    t->iContrast = Contrast;

    dbSynctipRef0 = ReadRT_fld (fld_SYNCTIP_REF0);
    dbContrast = (double) (Contrast+1000.0) / 1000.0;

    switch (t->wStandard & 0x00FF)
    {
        case (DEC_NTSC):
            if ((t->wStandard & 0xFF00) == (extNTSC_J))
            {
                dbYgain = 219.0 / ( 100.0 * (double)(dbSynctipRef0) /40.0);
            }
            else
            {
                dbYgain = 219.0 / ( 92.5 * (double)(dbSynctipRef0) /40.0);
            }
            break;
        case (DEC_PAL):
        case (DEC_SECAM):
            dbYgain = 219.0 / ( 100.0 * (double)(dbSynctipRef0) /43.0);
            break;
        default:
            break;
    }

    bTempContrast = (uint8_t) ((dbContrast * dbYgain * 64) + 0.5);

    WriteRT_fld (fld_LP_CONTRAST, (uint32_t)bTempContrast);

    /* Save value for future modification */
    t->dbContrast = dbContrast;

    return;

} /* RT_SetContrast ()... */

/****************************************************************************
 * RT_SetInterlace (uint8_t bInterlace)                                        *
 *  Function: to set the interlacing pattern for the Rage Theatre video in  *
 *    Inputs: uint8_t bInterlace                                               *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetInterlace (TheatrePtr t, uint8_t bInterlace)
{

    switch(bInterlace)
     {
        case (TRUE):    /*DEC_INTERLACE */
                        WriteRT_fld (fld_V_DEINTERLACE_ON, 0x1);
                        t->wInterlaced = (uint16_t) RT_DECINTERLACED;
                        break;
       case (FALSE):    /*DEC_NONINTERLACE */
                        WriteRT_fld (fld_V_DEINTERLACE_ON, RT_DECNONINTERLACED);
                        t->wInterlaced = (uint16_t) RT_DECNONINTERLACED;
                        break;
       default:
                        break;
     }

    return;

} /* RT_SetInterlace ()... */

/****************************************************************************
 * GetStandardConstants (double *LPeriod, double *FPeriod,                  *
 *                          double *Fsamp, uint16_t wStandard)                  *
 *  Function: return timing values for a given standard                     *
 *    Inputs: double *LPeriod -
 *            double *FPeriod -
 *            double *Fsamp - sampling frequency used for a given standard  *
 *            uint16_t wStandard - input standard (NTSC, PAL, SECAM)            *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
static void GetStandardConstants (double *LPeriod, double *FPeriod,
                           double *Fsamp, uint16_t wStandard)
{
    *LPeriod = 0.0;
    *FPeriod = 0.0;
    *Fsamp   = 0.0;

    switch (wStandard & 0x00FF)
    {
        case (DEC_NTSC): /*NTSC GROUP - 480 lines*/
            switch (wStandard & 0xFF00)
            {
                case  (extNONE):
                case  (extNTSC):
                case  (extNTSC_J):
                    *LPeriod = (double) 63.5555;
                    *FPeriod = (double) 16.6833;
                    *Fsamp = (double) 28.63636;
                    break;
                case  (extPAL_M):
                    *LPeriod = (double) 63.492;
                    *FPeriod = (double) 16.667;
                    *Fsamp = (double) 28.63689192;
                    break;
                default:
                    return;
            }
            break;
        case (DEC_PAL):
            if(  (wStandard & 0xFF00) == extPAL_N )
            {
                *LPeriod = (double) 64.0;
                *FPeriod = (double) 20.0;
                *Fsamp = (double) 28.65645;
            }
            else
            {
                *LPeriod = (double) 64.0;
                *FPeriod = (double) 20.0;
                *Fsamp = (double) 35.46895;
            }
            break;
        case (DEC_SECAM):
            *LPeriod = (double) 64.0;
            *FPeriod = (double) 20.0;
            *Fsamp = (double) 35.46895;
            break;
    }
    return;

} /* GetStandardConstants ()...*/


/****************************************************************************
 * RT_SetStandard (uint16_t wStandard)                                          *
 *  Function: to set the input standard for the Rage Theatre video in       *
 *    Inputs: uint16_t wStandard - input standard (NTSC, PAL, SECAM)            *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetStandard (TheatrePtr t, uint16_t wStandard)
{
    double dbFsamp=0, dbLPeriod=0, dbFPeriod=0;
    uint16_t   wFrameTotal = 0;
    double dbSPPeriod = 4.70;

    xf86DrvMsg(t->VIP->pScrn->scrnIndex,X_INFO,"Rage Theatre setting standard 0x%04x\n",
    		wStandard);
    t->wStandard = wStandard;
    	
    /* Get the constants for the given standard. */    
    GetStandardConstants (&dbLPeriod, &dbFPeriod, &dbFsamp, wStandard);

    wFrameTotal = (uint16_t) (((2.0 * dbFPeriod) * 1000 / dbLPeriod) + 0.5);

    /* Procedures before setting the standards: */
    WriteRT_fld (fld_VIN_CLK_SEL, RT_REF_CLK);
    WriteRT_fld (fld_VINRST, RT_VINRST_RESET);

    RT_SetVINClock (t, wStandard);

    WriteRT_fld (fld_VINRST, RT_VINRST_ACTIVE);
    WriteRT_fld (fld_VIN_CLK_SEL, RT_PLL_VIN_CLK);

    /* Program the new standards: */
    switch (wStandard & 0x00FF)
    {
        case (DEC_NTSC): /*NTSC GROUP - 480 lines */
            WriteRT_fld (fld_STANDARD_SEL,     RT_NTSC);
            WriteRT_fld (fld_SYNCTIP_REF0,     RT_NTSCM_SYNCTIP_REF0);
            WriteRT_fld (fld_SYNCTIP_REF1,     RT_NTSCM_SYNCTIP_REF1);
            WriteRT_fld (fld_CLAMP_REF,         RT_NTSCM_CLAMP_REF);
            WriteRT_fld (fld_AGC_PEAKWHITE,    RT_NTSCM_PEAKWHITE);
            WriteRT_fld (fld_VBI_PEAKWHITE,    RT_NTSCM_VBI_PEAKWHITE);
            WriteRT_fld (fld_WPA_THRESHOLD,    RT_NTSCM_WPA_THRESHOLD);
            WriteRT_fld (fld_WPA_TRIGGER_LO,   RT_NTSCM_WPA_TRIGGER_LO);
            WriteRT_fld (fld_WPA_TRIGGER_HIGH, RT_NTSCM_WPA_TRIGGER_HIGH);
            WriteRT_fld (fld_LOCKOUT_START,    RT_NTSCM_LP_LOCKOUT_START);
            WriteRT_fld (fld_LOCKOUT_END,      RT_NTSCM_LP_LOCKOUT_END);
            WriteRT_fld (fld_CH_DTO_INC,       RT_NTSCM_CH_DTO_INC);
            WriteRT_fld (fld_PLL_SGAIN,        RT_NTSCM_CH_PLL_SGAIN);
            WriteRT_fld (fld_PLL_FGAIN,        RT_NTSCM_CH_PLL_FGAIN);

            WriteRT_fld (fld_CH_HEIGHT,        RT_NTSCM_CH_HEIGHT);
            WriteRT_fld (fld_CH_KILL_LEVEL,    RT_NTSCM_CH_KILL_LEVEL);

            WriteRT_fld (fld_CH_AGC_ERROR_LIM, RT_NTSCM_CH_AGC_ERROR_LIM);
            WriteRT_fld (fld_CH_AGC_FILTER_EN, RT_NTSCM_CH_AGC_FILTER_EN);
            WriteRT_fld (fld_CH_AGC_LOOP_SPEED,RT_NTSCM_CH_AGC_LOOP_SPEED);

            WriteRT_fld (fld_VS_FIELD_BLANK_START,  RT_NTSCM_VS_FIELD_BLANK_START);
            WriteRT_fld (fld_VS_FIELD_BLANK_END,   RT_NTSCM_VS_FIELD_BLANK_END);

            WriteRT_fld (fld_H_ACTIVE_START,   RT_NTSCM_H_ACTIVE_START);
            WriteRT_fld (fld_H_ACTIVE_END,   RT_NTSCM_H_ACTIVE_END);

            WriteRT_fld (fld_V_ACTIVE_START,   RT_NTSCM_V_ACTIVE_START);
            WriteRT_fld (fld_V_ACTIVE_END,   RT_NTSCM_V_ACTIVE_END);

            WriteRT_fld (fld_H_VBI_WIND_START,   RT_NTSCM_H_VBI_WIND_START);
            WriteRT_fld (fld_H_VBI_WIND_END,   RT_NTSCM_H_VBI_WIND_END);

            WriteRT_fld (fld_V_VBI_WIND_START,   RT_NTSCM_V_VBI_WIND_START);
            WriteRT_fld (fld_V_VBI_WIND_END,   RT_NTSCM_V_VBI_WIND_END);

            WriteRT_fld (fld_UV_INT_START,   (uint8_t)((0.10 * dbLPeriod * dbFsamp / 2.0) + 0.5 - 32));

            WriteRT_fld (fld_VSYNC_INT_TRIGGER , (uint16_t) RT_NTSCM_VSYNC_INT_TRIGGER);
            WriteRT_fld (fld_VSYNC_INT_HOLD, (uint16_t) RT_NTSCM_VSYNC_INT_HOLD);

            switch (wStandard & 0xFF00)
            {
                case (extPAL_M):
                case (extNONE):
                case (extNTSC):
                    WriteRT_fld (fld_CR_BURST_GAIN,        RT_NTSCM_CR_BURST_GAIN);
                    WriteRT_fld (fld_CB_BURST_GAIN,        RT_NTSCM_CB_BURST_GAIN);

                    WriteRT_fld (fld_CRDR_ACTIVE_GAIN,     RT_NTSCM_CRDR_ACTIVE_GAIN);
                    WriteRT_fld (fld_CBDB_ACTIVE_GAIN,     RT_NTSCM_CBDB_ACTIVE_GAIN);

                    WriteRT_fld (fld_VERT_LOCKOUT_START,   RT_NTSCM_VERT_LOCKOUT_START);
                    WriteRT_fld (fld_VERT_LOCKOUT_END,     RT_NTSCM_VERT_LOCKOUT_END);

                    break;
                case (extNTSC_J):
                    WriteRT_fld (fld_CR_BURST_GAIN,        RT_NTSCJ_CR_BURST_GAIN);
                    WriteRT_fld (fld_CB_BURST_GAIN,        RT_NTSCJ_CB_BURST_GAIN);

                    WriteRT_fld (fld_CRDR_ACTIVE_GAIN,     RT_NTSCJ_CRDR_ACTIVE_GAIN);
                    WriteRT_fld (fld_CBDB_ACTIVE_GAIN,     RT_NTSCJ_CBDB_ACTIVE_GAIN);

                    WriteRT_fld (fld_CH_HEIGHT,            RT_NTSCJ_CH_HEIGHT);
                    WriteRT_fld (fld_CH_KILL_LEVEL,        RT_NTSCJ_CH_KILL_LEVEL);

                    WriteRT_fld (fld_CH_AGC_ERROR_LIM,     RT_NTSCJ_CH_AGC_ERROR_LIM);
                    WriteRT_fld (fld_CH_AGC_FILTER_EN,     RT_NTSCJ_CH_AGC_FILTER_EN);
                    WriteRT_fld (fld_CH_AGC_LOOP_SPEED,    RT_NTSCJ_CH_AGC_LOOP_SPEED);

                    WriteRT_fld (fld_VERT_LOCKOUT_START,   RT_NTSCJ_VERT_LOCKOUT_START);
                    WriteRT_fld (fld_VERT_LOCKOUT_END,     RT_NTSCJ_VERT_LOCKOUT_END);

                    break;
                default:
                    break;
            }
            break;
        case (DEC_PAL):  /*PAL GROUP  - 525 lines */
            WriteRT_fld (fld_STANDARD_SEL,     RT_PAL);
            WriteRT_fld (fld_SYNCTIP_REF0,     RT_PAL_SYNCTIP_REF0);
            WriteRT_fld (fld_SYNCTIP_REF1,     RT_PAL_SYNCTIP_REF1);
	    
            WriteRT_fld (fld_CLAMP_REF,         RT_PAL_CLAMP_REF);
            WriteRT_fld (fld_AGC_PEAKWHITE,    RT_PAL_PEAKWHITE);
            WriteRT_fld (fld_VBI_PEAKWHITE,    RT_PAL_VBI_PEAKWHITE);

            WriteRT_fld (fld_WPA_THRESHOLD,    RT_PAL_WPA_THRESHOLD);
            WriteRT_fld (fld_WPA_TRIGGER_LO,   RT_PAL_WPA_TRIGGER_LO);
            WriteRT_fld (fld_WPA_TRIGGER_HIGH, RT_PAL_WPA_TRIGGER_HIGH);

            WriteRT_fld (fld_LOCKOUT_START,RT_PAL_LP_LOCKOUT_START);
            WriteRT_fld (fld_LOCKOUT_END,  RT_PAL_LP_LOCKOUT_END);
            WriteRT_fld (fld_CH_DTO_INC,       RT_PAL_CH_DTO_INC);
            WriteRT_fld (fld_PLL_SGAIN,        RT_PAL_CH_PLL_SGAIN);
            WriteRT_fld (fld_PLL_FGAIN,        RT_PAL_CH_PLL_FGAIN);

            WriteRT_fld (fld_CR_BURST_GAIN,    RT_PAL_CR_BURST_GAIN);
            WriteRT_fld (fld_CB_BURST_GAIN,    RT_PAL_CB_BURST_GAIN);

            WriteRT_fld (fld_CRDR_ACTIVE_GAIN, RT_PAL_CRDR_ACTIVE_GAIN);
            WriteRT_fld (fld_CBDB_ACTIVE_GAIN, RT_PAL_CBDB_ACTIVE_GAIN);

            WriteRT_fld (fld_CH_HEIGHT,        RT_PAL_CH_HEIGHT);
            WriteRT_fld (fld_CH_KILL_LEVEL,    RT_PAL_CH_KILL_LEVEL);

            WriteRT_fld (fld_CH_AGC_ERROR_LIM, RT_PAL_CH_AGC_ERROR_LIM);
            WriteRT_fld (fld_CH_AGC_FILTER_EN, RT_PAL_CH_AGC_FILTER_EN);
            WriteRT_fld (fld_CH_AGC_LOOP_SPEED,RT_PAL_CH_AGC_LOOP_SPEED);

            WriteRT_fld (fld_VERT_LOCKOUT_START,   RT_PAL_VERT_LOCKOUT_START);
            WriteRT_fld (fld_VERT_LOCKOUT_END, RT_PAL_VERT_LOCKOUT_END);
            WriteRT_fld (fld_VS_FIELD_BLANK_START,  (uint16_t)RT_PALSEM_VS_FIELD_BLANK_START);

            WriteRT_fld (fld_VS_FIELD_BLANK_END,   RT_PAL_VS_FIELD_BLANK_END);

            WriteRT_fld (fld_H_ACTIVE_START,   RT_PAL_H_ACTIVE_START);
            WriteRT_fld (fld_H_ACTIVE_END,   RT_PAL_H_ACTIVE_END);

            WriteRT_fld (fld_V_ACTIVE_START,   RT_PAL_V_ACTIVE_START);
            WriteRT_fld (fld_V_ACTIVE_END,   RT_PAL_V_ACTIVE_END);

            WriteRT_fld (fld_H_VBI_WIND_START,   RT_PAL_H_VBI_WIND_START);
            WriteRT_fld (fld_H_VBI_WIND_END,   RT_PAL_H_VBI_WIND_END);

            WriteRT_fld (fld_V_VBI_WIND_START,   RT_PAL_V_VBI_WIND_START);
            WriteRT_fld (fld_V_VBI_WIND_END,   RT_PAL_V_VBI_WIND_END);

	    /* Magic 0.10 is correct - according to Ivo. Also see SECAM code below */
/*            WriteRT_fld (fld_UV_INT_START,   (uint8_t)( (0.12 * dbLPeriod * dbFsamp / 2.0) + 0.5 - 32 )); */
            WriteRT_fld (fld_UV_INT_START,   (uint8_t)( (0.10 * dbLPeriod * dbFsamp / 2.0) + 0.5 - 32 ));

            WriteRT_fld (fld_VSYNC_INT_TRIGGER , (uint16_t) RT_PALSEM_VSYNC_INT_TRIGGER);
            WriteRT_fld (fld_VSYNC_INT_HOLD, (uint16_t) RT_PALSEM_VSYNC_INT_HOLD);

            break;
        case (DEC_SECAM):  /*PAL GROUP*/
            WriteRT_fld (fld_STANDARD_SEL,     RT_SECAM);
            WriteRT_fld (fld_SYNCTIP_REF0,     RT_SECAM_SYNCTIP_REF0);
            WriteRT_fld (fld_SYNCTIP_REF1,     RT_SECAM_SYNCTIP_REF1);
            WriteRT_fld (fld_CLAMP_REF,         RT_SECAM_CLAMP_REF);
            WriteRT_fld (fld_AGC_PEAKWHITE,    RT_SECAM_PEAKWHITE);
            WriteRT_fld (fld_VBI_PEAKWHITE,    RT_SECAM_VBI_PEAKWHITE);

            WriteRT_fld (fld_WPA_THRESHOLD,    RT_SECAM_WPA_THRESHOLD);

            WriteRT_fld (fld_WPA_TRIGGER_LO,   RT_SECAM_WPA_TRIGGER_LO);
            WriteRT_fld (fld_WPA_TRIGGER_HIGH, RT_SECAM_WPA_TRIGGER_HIGH);

            WriteRT_fld (fld_LOCKOUT_START,RT_SECAM_LP_LOCKOUT_START);
            WriteRT_fld (fld_LOCKOUT_END,  RT_SECAM_LP_LOCKOUT_END);

            WriteRT_fld (fld_CH_DTO_INC,       RT_SECAM_CH_DTO_INC);
            WriteRT_fld (fld_PLL_SGAIN,        RT_SECAM_CH_PLL_SGAIN);
            WriteRT_fld (fld_PLL_FGAIN,        RT_SECAM_CH_PLL_FGAIN);

            WriteRT_fld (fld_CR_BURST_GAIN,    RT_SECAM_CR_BURST_GAIN);
            WriteRT_fld (fld_CB_BURST_GAIN,    RT_SECAM_CB_BURST_GAIN);

            WriteRT_fld (fld_CRDR_ACTIVE_GAIN, RT_SECAM_CRDR_ACTIVE_GAIN);
            WriteRT_fld (fld_CBDB_ACTIVE_GAIN, RT_SECAM_CBDB_ACTIVE_GAIN);

            WriteRT_fld (fld_CH_HEIGHT,        RT_SECAM_CH_HEIGHT);
            WriteRT_fld (fld_CH_KILL_LEVEL,    RT_SECAM_CH_KILL_LEVEL);

            WriteRT_fld (fld_CH_AGC_ERROR_LIM, RT_SECAM_CH_AGC_ERROR_LIM);
            WriteRT_fld (fld_CH_AGC_FILTER_EN, RT_SECAM_CH_AGC_FILTER_EN);
            WriteRT_fld (fld_CH_AGC_LOOP_SPEED,RT_SECAM_CH_AGC_LOOP_SPEED);

            WriteRT_fld (fld_VERT_LOCKOUT_START,   RT_SECAM_VERT_LOCKOUT_START);  /*Might not need */
            WriteRT_fld (fld_VERT_LOCKOUT_END, RT_SECAM_VERT_LOCKOUT_END);  /* Might not need */

            WriteRT_fld (fld_VS_FIELD_BLANK_START,  (uint16_t)RT_PALSEM_VS_FIELD_BLANK_START);
            WriteRT_fld (fld_VS_FIELD_BLANK_END,   RT_PAL_VS_FIELD_BLANK_END);

            WriteRT_fld (fld_H_ACTIVE_START,   RT_PAL_H_ACTIVE_START);
            WriteRT_fld (fld_H_ACTIVE_END,   RT_PAL_H_ACTIVE_END);

            WriteRT_fld (fld_V_ACTIVE_START,   RT_PAL_V_ACTIVE_START);
            WriteRT_fld (fld_V_ACTIVE_END,   RT_PAL_V_ACTIVE_END);

            WriteRT_fld (fld_H_VBI_WIND_START,   RT_PAL_H_VBI_WIND_START);
            WriteRT_fld (fld_H_VBI_WIND_END,   RT_PAL_H_VBI_WIND_END);

            WriteRT_fld (fld_V_VBI_WIND_START,   RT_PAL_V_VBI_WIND_START);
            WriteRT_fld (fld_V_VBI_WIND_END,   RT_PAL_V_VBI_WIND_END);

            WriteRT_fld (fld_VSYNC_INT_TRIGGER , (uint16_t) RT_PALSEM_VSYNC_INT_TRIGGER);
            WriteRT_fld (fld_VSYNC_INT_HOLD, (uint16_t) RT_PALSEM_VSYNC_INT_HOLD);

/*            WriteRT_fld (fld_UV_INT_START,   (uint8_t)( (0.12 * dbLPeriod * dbFsamp / 2.0) + 0.5 - 32 )); */
            WriteRT_fld (fld_UV_INT_START,   (uint8_t)( (0.10 * dbLPeriod * dbFsamp / 2.0) + 0.5 - 32 ));

            break;
        default:
            break;
    }

    if (t->wConnector == DEC_SVIDEO)
    {

        RT_SetCombFilter (t, wStandard, RT_SVIDEO);
    }
    else
    {
        /* Set up extra (connector and std) registers. */
        RT_SetCombFilter (t, wStandard, RT_COMPOSITE);
    }

    /* Set the following values according to the formulas */
    WriteRT_fld (fld_HS_LINE_TOTAL, (uint16_t)((dbLPeriod * dbFsamp / 2.0) +0.5));
    /* According to Ivo PAL/SECAM needs different treatment */
    switch(wStandard & 0x00FF)
    {
        case DEC_PAL:
	case DEC_SECAM:
			WriteRT_fld (fld_MIN_PULSE_WIDTH, (uint8_t)(0.5 * dbSPPeriod * dbFsamp/2.0));
			WriteRT_fld (fld_MAX_PULSE_WIDTH, (uint8_t)(1.5 * dbSPPeriod * dbFsamp/2.0));
    			WriteRT_fld (fld_WIN_OPEN_LIMIT, (uint16_t)(((dbLPeriod * dbFsamp / 4.0) + 0.5) - 16));
  		    	WriteRT_fld (fld_WIN_CLOSE_LIMIT, (uint16_t)(2.39 * dbSPPeriod * dbFsamp / 2.0));
		/*    	WriteRT_fld (fld_VS_FIELD_IDLOCATION,   (uint16_t)RT_PAL_FIELD_IDLOCATION); */
		/*      According to docs the following value will work right, though the resulting stream deviates
		        slightly from CCIR..., in particular the value that was before will do nuts to VCRs in
			pause/rewind state. */
		    	WriteRT_fld (fld_VS_FIELD_IDLOCATION,   (uint16_t)0x01);
		    	WriteRT_fld (fld_HS_PLL_SGAIN, 2);
			break;
  	case DEC_NTSC:
			WriteRT_fld (fld_MIN_PULSE_WIDTH, (uint8_t)(0.75 * dbSPPeriod * dbFsamp/2.0));
    			WriteRT_fld (fld_MAX_PULSE_WIDTH, (uint8_t)(1.25 * dbSPPeriod * dbFsamp/2.0));
    			WriteRT_fld (fld_WIN_OPEN_LIMIT, (uint16_t)(((dbLPeriod * dbFsamp / 4.0) + 0.5) - 16));
    			WriteRT_fld (fld_WIN_CLOSE_LIMIT, (uint16_t)(1.15 * dbSPPeriod * dbFsamp / 2.0));
    		/*	WriteRT_fld (fld_VS_FIELD_IDLOCATION,   (uint16_t)fld_VS_FIELD_IDLOCATION_def);*/
		/*      I think the default value was the same as the one here.. does not hurt to hardcode it */
			WriteRT_fld (fld_VS_FIELD_IDLOCATION,   (uint16_t)0x01);

     }

    WriteRT_fld (fld_VS_FRAME_TOTAL,   (uint16_t)(wFrameTotal) + 10);
    WriteRT_fld (fld_BLACK_INT_START,   (uint8_t)((0.09 * dbLPeriod * dbFsamp / 2.0) - 32 ));
    WriteRT_fld (fld_SYNC_TIP_START,   (uint16_t)((dbLPeriod * dbFsamp / 2.0 + 0.5) - 28 ));

    return;

} /* RT_SetStandard ()... */



/****************************************************************************
 * RT_SetCombFilter (uint16_t wStandard, uint16_t wConnector)                       *
 *  Function: sets the input comb filter based on the standard and          *
 *            connector being used (composite vs. svideo)                   *
 *    Inputs: uint16_t wStandard - input standard (NTSC, PAL, SECAM)            *
 *            uint16_t wConnector - COMPOSITE, SVIDEO                           *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
static void RT_SetCombFilter (TheatrePtr t, uint16_t wStandard, uint16_t wConnector)
{
    uint32_t dwComb_Cntl0=0;
    uint32_t dwComb_Cntl1=0;
    uint32_t dwComb_Cntl2=0;
    uint32_t dwComb_Line_Length=0;

    switch (wConnector)
    {
        case RT_COMPOSITE:
                switch (wStandard & 0x00FF)
                {
                    case (DEC_NTSC):
                        switch (wStandard & 0xFF00)
                        {
                            case  (extNONE):
                            case  (extNTSC):
                            case  (extNTSC_J):
                                dwComb_Cntl0= RT_NTSCM_COMB_CNTL0_COMPOSITE;
                                dwComb_Cntl1= RT_NTSCM_COMB_CNTL1_COMPOSITE;
                                dwComb_Cntl2= RT_NTSCM_COMB_CNTL2_COMPOSITE;
                                dwComb_Line_Length= RT_NTSCM_COMB_LENGTH_COMPOSITE;
                                break;
                            case  (extPAL_M):
                                dwComb_Cntl0= RT_PALM_COMB_CNTL0_COMPOSITE;
                                dwComb_Cntl1= RT_PALM_COMB_CNTL1_COMPOSITE;
                                dwComb_Cntl2= RT_PALM_COMB_CNTL2_COMPOSITE;
                                dwComb_Line_Length= RT_PALM_COMB_LENGTH_COMPOSITE;
                                break;
                            default:
                                return;
                        }
                        break;
                    case (DEC_PAL):
                        switch (wStandard & 0xFF00)
                        {
                            case  (extNONE):
                            case  (extPAL):
                                dwComb_Cntl0=   RT_PAL_COMB_CNTL0_COMPOSITE;
                                dwComb_Cntl1=   RT_PAL_COMB_CNTL1_COMPOSITE;
                                dwComb_Cntl2=   RT_PAL_COMB_CNTL2_COMPOSITE;
                                dwComb_Line_Length=  RT_PAL_COMB_LENGTH_COMPOSITE;
                                break;
                            case  (extPAL_N):
                                dwComb_Cntl0=   RT_PALN_COMB_CNTL0_COMPOSITE;
                                dwComb_Cntl1=   RT_PALN_COMB_CNTL1_COMPOSITE;
                                dwComb_Cntl2=   RT_PALN_COMB_CNTL2_COMPOSITE;
                                dwComb_Line_Length=  RT_PALN_COMB_LENGTH_COMPOSITE;
                                break;
                            default:
                                return;
                        }
                        break;
                    case (DEC_SECAM):
                        dwComb_Cntl0=   RT_SECAM_COMB_CNTL0_COMPOSITE;
                        dwComb_Cntl1=   RT_SECAM_COMB_CNTL1_COMPOSITE;
                        dwComb_Cntl2=   RT_SECAM_COMB_CNTL2_COMPOSITE;
                        dwComb_Line_Length=  RT_SECAM_COMB_LENGTH_COMPOSITE;
                        break;
                    default:
                        return;
                }
            break;
        case RT_SVIDEO:
                switch (wStandard & 0x00FF)
                {
                    case (DEC_NTSC):
                        switch (wStandard & 0xFF00)
                        {
                            case  (extNONE):
                            case  (extNTSC):
                                dwComb_Cntl0= RT_NTSCM_COMB_CNTL0_SVIDEO;
                                dwComb_Cntl1= RT_NTSCM_COMB_CNTL1_SVIDEO;
                                dwComb_Cntl2= RT_NTSCM_COMB_CNTL2_SVIDEO;
                                dwComb_Line_Length= RT_NTSCM_COMB_LENGTH_SVIDEO;
                                break;
                            case  (extPAL_M):
                                dwComb_Cntl0= RT_PALM_COMB_CNTL0_SVIDEO;
                                dwComb_Cntl1= RT_PALM_COMB_CNTL1_SVIDEO;
                                dwComb_Cntl2= RT_PALM_COMB_CNTL2_SVIDEO;
                                dwComb_Line_Length= RT_PALM_COMB_LENGTH_SVIDEO;
                                break;
                            default:
                                return;
                        }
                        break;
                    case (DEC_PAL):
                        switch (wStandard & 0xFF00)
                        {
                            case  (extNONE):
                            case  (extPAL):
                                dwComb_Cntl0=   RT_PAL_COMB_CNTL0_SVIDEO;
                                dwComb_Cntl1=   RT_PAL_COMB_CNTL1_SVIDEO;
                                dwComb_Cntl2=   RT_PAL_COMB_CNTL2_SVIDEO;
                                dwComb_Line_Length=  RT_PAL_COMB_LENGTH_SVIDEO;
                                break;
                            case  (extPAL_N):
                                dwComb_Cntl0=   RT_PALN_COMB_CNTL0_SVIDEO;
                                dwComb_Cntl1=   RT_PALN_COMB_CNTL1_SVIDEO;
                                dwComb_Cntl2=   RT_PALN_COMB_CNTL2_SVIDEO;
                                dwComb_Line_Length=  RT_PALN_COMB_LENGTH_SVIDEO;
                                break;
                            default:
                                return;
                        }
                        break;
                    case (DEC_SECAM):
                        dwComb_Cntl0=   RT_SECAM_COMB_CNTL0_SVIDEO;
                        dwComb_Cntl1=   RT_SECAM_COMB_CNTL1_SVIDEO;
                        dwComb_Cntl2=   RT_SECAM_COMB_CNTL2_SVIDEO;
                        dwComb_Line_Length=  RT_SECAM_COMB_LENGTH_SVIDEO;
                        break;
                    default:
                        return;
                }
            break;
        default:
            return;
    }

    WriteRT_fld (fld_COMB_CNTL0, dwComb_Cntl0);
    WriteRT_fld (fld_COMB_CNTL1, dwComb_Cntl1);
    WriteRT_fld (fld_COMB_CNTL2, dwComb_Cntl2);
    WriteRT_fld (fld_COMB_LENGTH, dwComb_Line_Length);

    return;

} /* RT_SetCombFilter ()... */


/****************************************************************************
 * RT_SetOutputVideoSize (uint16_t wHorzSize, uint16_t wVertSize,                   *
 *                          uint8_t fCC_On, uint8_t fVBICap_On)                   *
 *  Function: sets the output video size for the Rage Theatre video in      *
 *    Inputs: uint16_t wHorzSize - width of output in pixels                    *
 *            uint16_t wVertSize - height of output in pixels (lines)           *
 *            uint8_t fCC_On - enable CC output                                *
 *            uint8_t fVBI_Cap_On - enable VBI capture                         *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
_X_EXPORT void RT_SetOutputVideoSize (TheatrePtr t, uint16_t wHorzSize, uint16_t wVertSize, uint8_t fCC_On, uint8_t fVBICap_On)
{
    uint32_t  dwHwinStart=0;
    uint32_t  dwHScaleRatio=0;
    uint32_t  dwHActiveLength=0;
    uint32_t  dwVwinStart=0;
    uint32_t  dwVScaleRatio=0;
    uint32_t  dwVActiveLength=0;
    uint32_t  dwTempRatio=0;
    uint32_t  dwEvenFieldOffset=0;
    uint32_t  dwOddFieldOffset=0;
    uint32_t  dwXin=0;
    uint32_t  dwYin=0;

    if (fVBICap_On)
    {
        WriteRT_fld (fld_VBI_CAPTURE_ENABLE, 1);
	WriteRT_fld (fld_VBI_SCALING_RATIO, fld_VBI_SCALING_RATIO_def);
        switch (t->wStandard & 0x00FF)
        {
            case (DEC_NTSC):
                WriteRT_fld (fld_H_VBI_WIND_START,  RT_NTSCM_H_VBI_WIND_START);
                WriteRT_fld (fld_H_VBI_WIND_END, RT_NTSCM_H_VBI_WIND_END);
                WriteRT_fld (fld_V_VBI_WIND_START, RT_NTSCM_V_VBI_WIND_START);
                WriteRT_fld (fld_V_VBI_WIND_END, RT_NTSCM_V_VBI_WIND_END);
                break;
            case (DEC_PAL):
                WriteRT_fld (fld_H_VBI_WIND_START, RT_PAL_H_VBI_WIND_START);
                WriteRT_fld (fld_H_VBI_WIND_END, RT_PAL_H_VBI_WIND_END);
                WriteRT_fld (fld_V_VBI_WIND_START, RT_PAL_V_VBI_WIND_START);
                WriteRT_fld (fld_V_VBI_WIND_END, RT_PAL_V_VBI_WIND_END);
                break;
            case (DEC_SECAM):
                WriteRT_fld (fld_H_VBI_WIND_START, RT_PAL_H_VBI_WIND_START);
                WriteRT_fld (fld_H_VBI_WIND_END, RT_PAL_H_VBI_WIND_END);
                WriteRT_fld (fld_V_VBI_WIND_START, RT_PAL_V_VBI_WIND_START);
                WriteRT_fld (fld_V_VBI_WIND_END, RT_PAL_V_VBI_WIND_END);
                break;
            default:
                break;
        }
    }
    else
    {
        WriteRT_fld (fld_VBI_CAPTURE_ENABLE, 0);
    }

    if (t->wInterlaced != RT_DECINTERLACED)
    {
        wVertSize *= 2;
    }

    /*1. Calculate Horizontal Scaling ratio:*/
    switch (t->wStandard & 0x00FF)
    {
        case (DEC_NTSC):
            dwHwinStart = RT_NTSCM_H_IN_START;
            dwXin = (ReadRT_fld (fld_H_ACTIVE_END) - ReadRT_fld (fld_H_ACTIVE_START)); /*tempscaler*/
            dwXin = RT_NTSC_H_ACTIVE_SIZE;
            dwHScaleRatio = (uint32_t) ((long) dwXin * 65536L / wHorzSize);
            dwHScaleRatio = dwHScaleRatio & 0x001FFFFF; /*21 bit number;*/
            dwHActiveLength = wHorzSize;
            break;
        case (DEC_PAL):
            dwHwinStart = RT_PAL_H_IN_START;
            dwXin = RT_PAL_H_ACTIVE_SIZE;
            dwHScaleRatio = (uint32_t) ((long) dwXin * 65536L / wHorzSize);
            dwHScaleRatio = dwHScaleRatio & 0x001FFFFF; /*21 bit number;*/
            dwHActiveLength = wHorzSize;
            break;
        case (DEC_SECAM):
            dwHwinStart = RT_SECAM_H_IN_START;
            dwXin = RT_SECAM_H_ACTIVE_SIZE;
            dwHScaleRatio = (uint32_t) ((long) dwXin * 65536L / wHorzSize);
            dwHScaleRatio = dwHScaleRatio & 0x001FFFFF; /*21 bit number;*/
            dwHActiveLength = wHorzSize;
            break;
        default:
            break;
    }

    /*2. Calculate Vertical Scaling ratio:*/
    switch (t->wStandard & 0x00FF)
    {
        case (DEC_NTSC):
            dwVwinStart = RT_NTSCM_V_IN_START;
            /* dwYin = (ReadRT_fld (fld_V_ACTIVE_END) - ReadRT_fld (fld_V_ACTIVE_START)); */ /*tempscaler*/
	    dwYin = RT_NTSCM_V_ACTIVE_SIZE;
            dwTempRatio = (uint32_t)((long) wVertSize / dwYin);
            dwVScaleRatio = (uint32_t)((long)wVertSize * 2048L / dwYin);
            dwVScaleRatio = dwVScaleRatio & 0x00000FFF;
            dwVActiveLength = wVertSize/2;
            break;
        case (DEC_PAL):
            dwVwinStart = RT_PAL_V_IN_START;
            dwYin = RT_PAL_V_ACTIVE_SIZE;
            dwTempRatio = (uint32_t)(wVertSize/dwYin);
            dwVScaleRatio = (uint32_t)((long)wVertSize * 2048L / dwYin);
            dwVScaleRatio = dwVScaleRatio & 0x00000FFF;
            dwVActiveLength = wVertSize/2;
            break;
        case (DEC_SECAM):
            dwVwinStart = RT_SECAM_V_IN_START;
            dwYin = RT_SECAM_V_ACTIVE_SIZE;
            dwTempRatio = (uint32_t) (wVertSize / dwYin);
            dwVScaleRatio = (uint32_t) ((long) wVertSize  * 2048L / dwYin);
            dwVScaleRatio = dwVScaleRatio & 0x00000FFF;
            dwVActiveLength = wVertSize/2;
            break;
        default:
            break;
    }

    /*4. Set up offset based on if interlaced or not:*/
    if (t->wInterlaced == RT_DECINTERLACED)
    {
        dwEvenFieldOffset = (uint32_t) ((1.0 - ((double) wVertSize / (double) dwYin)) * 512.0);
        dwOddFieldOffset  =  dwEvenFieldOffset;
        WriteRT_fld (fld_V_DEINTERLACE_ON, 0x1);
    }
    else
    {
        dwEvenFieldOffset = (uint32_t)(dwTempRatio * 512.0);
        dwOddFieldOffset  = (uint32_t)(2048 - dwEvenFieldOffset);
        WriteRT_fld (fld_V_DEINTERLACE_ON, 0x0);
    }

    /* Set the registers:*/
    WriteRT_fld (fld_H_IN_WIND_START,  dwHwinStart);
    WriteRT_fld (fld_H_SCALE_RATIO,    dwHScaleRatio);
    WriteRT_fld (fld_H_OUT_WIND_WIDTH, dwHActiveLength);

    WriteRT_fld (fld_V_IN_WIND_START,  dwVwinStart);
    WriteRT_fld (fld_V_SCALE_RATIO,    dwVScaleRatio);
    WriteRT_fld (fld_V_OUT_WIND_WIDTH, dwVActiveLength);

    WriteRT_fld (fld_EVENF_OFFSET,     dwEvenFieldOffset);
    WriteRT_fld (fld_ODDF_OFFSET,      dwOddFieldOffset);

    t->dwHorzScalingRatio = dwHScaleRatio;
    t->dwVertScalingRatio = dwVScaleRatio;

    return;

} /* RT_SetOutputVideoSize ()...*/



/****************************************************************************
 * CalculateCrCbGain (double *CrGain, double *CbGain, uint16_t wStandard)       *
 *  Function:                                                               *
 *    Inputs: double *CrGain -
 *            double *CbGain -
 *            uint16_t wStandard - input standard (NTSC, PAL, SECAM)            *
 *   Outputs: NONE                                                          *
 ****************************************************************************/
static void CalculateCrCbGain (TheatrePtr t, double *CrGain, double *CbGain, uint16_t wStandard)
{
    #define UVFLTGAIN   1.5
    #define FRMAX       280000.0
    #define FBMAX       230000.0

    double dbSynctipRef0=0, dbFsamp=0, dbLPeriod=0, dbFPeriod=0;

    dbSynctipRef0 = ReadRT_fld (fld_SYNCTIP_REF0);

    GetStandardConstants (&dbLPeriod, &dbFPeriod, &dbFsamp, wStandard);

    *CrGain=0.0;
    *CbGain=0.0;

    switch (wStandard & 0x00FF)
    {
        case (DEC_NTSC): /*NTSC GROUP - 480 lines*/
            switch (wStandard & 0xFF00)
            {
                case  (extNONE):
                case  (extNTSC):
                case  (extPAL_M):
                    *CrGain = (double)(40.0 / (dbSynctipRef0)) * (100.0/92.5) * (1.0/0.877) * ((112.0/70.1)/UVFLTGAIN);
                    *CbGain = (double)(40.0 / (dbSynctipRef0)) * (100.0/92.5) * (1.0/0.492) * ((112.0/88.6)/UVFLTGAIN);
                    break;
                case  (extNTSC_J):
                    *CrGain = (double)(40.0 / (dbSynctipRef0)) * (100.0/100.0) * (1.0/0.877) * ((112.0/70.1)/UVFLTGAIN);
                    *CbGain = (double)(40.0 / (dbSynctipRef0)) * (100.0/100.0) * (1.0/0.492) * ((112.0/88.6)/UVFLTGAIN);
                    break;
                default:
                    return;
            }
            break;
        case (DEC_PAL):
            *CrGain = (double)(43.0 / (dbSynctipRef0)) * (100.0/92.5) * (1.0/0.877) * ((112.0/70.1)/UVFLTGAIN);
            *CbGain = (double)(43.0 / (dbSynctipRef0)) * (100.0/92.5) * (1.0/0.492) * ((112.0/88.6)/UVFLTGAIN);
            break;
        case (DEC_SECAM):
            *CrGain = (double) 32.0 * 32768.0 / FRMAX / (33554432.0 / dbFsamp) * (1.597 / 1.902) / UVFLTGAIN;
            *CbGain = (double) 32.0 * 32768.0 / FBMAX / (33554432.0 / dbFsamp) * (1.267 / 1.505) / UVFLTGAIN;
            break;
    }

    return;

} /* CalculateCrCbGain ()...*/


/****************************************************************************
 * RT_SetConnector (uint16_t wStandard, int tunerFlag)                          *
 *  Function:
 *    Inputs: uint16_t wStandard - input standard (NTSC, PAL, SECAM)            *
 *            int tunerFlag
 *   Outputs: NONE                                                          *
 ****************************************************************************/
void RT_SetConnector (TheatrePtr t, uint16_t wConnector, int tunerFlag)
{
    uint32_t dwTempContrast=0;
    int i;
    long counter;

    t->wConnector = wConnector;

    /* Get the contrast value - make sure we are viewing a visible line*/
    counter=0;
    #if 0
    while (!((ReadRT_fld (fld_VS_LINE_COUNT)> 1) && (ReadRT_fld (fld_VS_LINE_COUNT)<20)) && (counter < 100000)){
    #endif
    while ((ReadRT_fld (fld_VS_LINE_COUNT)<20) && (counter < 10000)){
    	counter++;
	}
    dwTempContrast = ReadRT_fld (fld_LP_CONTRAST);
    if(counter>=10000)xf86DrvMsg(t->VIP->pScrn->scrnIndex, X_INFO,
				 "Rage Theatre: timeout waiting for line count (%u)\n",
				 (unsigned)ReadRT_fld (fld_VS_LINE_COUNT));


    WriteRT_fld (fld_LP_CONTRAST, 0x0);

    switch (wConnector)
    {
        case (DEC_TUNER):   /* Tuner*/
            WriteRT_fld (fld_INPUT_SELECT, t->wTunerConnector );
            WriteRT_fld (fld_STANDARD_YC, RT_COMPOSITE);
            RT_SetCombFilter (t, t->wStandard, RT_COMPOSITE);
            break;
        case (DEC_COMPOSITE):   /* Comp*/
            WriteRT_fld (fld_INPUT_SELECT, t->wComp0Connector);
            WriteRT_fld (fld_STANDARD_YC, RT_COMPOSITE);
            RT_SetCombFilter (t, t->wStandard, RT_COMPOSITE);
            break;
        case (DEC_SVIDEO):  /* Svideo*/
            WriteRT_fld (fld_INPUT_SELECT, t->wSVideo0Connector);
            WriteRT_fld (fld_STANDARD_YC, RT_SVIDEO);
            RT_SetCombFilter (t, t->wStandard, RT_SVIDEO);
            break;
        default:
            WriteRT_fld (fld_INPUT_SELECT, t->wComp0Connector);
            WriteRT_fld (fld_STANDARD_YC, RT_COMPOSITE);
            RT_SetCombFilter (t, t->wStandard, RT_COMPOSITE);
            break;
    }

    t->wConnector = wConnector;

    WriteRT_fld (fld_COMB_CNTL1, ReadRT_fld (fld_COMB_CNTL1) ^ 0x100);
    WriteRT_fld (fld_COMB_CNTL1, ReadRT_fld (fld_COMB_CNTL1) ^ 0x100);

    /* wait at most 1 sec here 
      VIP bus has a bandwidth of 27MB and it is 8bit.
      A single Rage Theatre read should take at least 6 bytes (2 for address one way and 4 for data the other way)
      However there are also latencies associated with such reads, plus latencies for PCI accesses.
      
      I guess we should not be doing more than 100000 per second.. At some point 
      I should really write a program to time this.
      */
    i = 100000;
    
    while ((i>=0) && (! ReadRT_fld (fld_HS_GENLOCKED)))
    {
      i--;
    }
    if(i<0) xf86DrvMsg(t->VIP->pScrn->scrnIndex, X_INFO, "Rage Theatre: waiting for fld_HS_GENLOCKED failed\n");
    /* now we are waiting for a non-visible line.. and there is absolutely no point to wait too long */
    counter = 0;
    while (!((ReadRT_fld (fld_VS_LINE_COUNT)> 1) && (ReadRT_fld (fld_VS_LINE_COUNT)<20)) && (counter < 10000)){
    	counter++;
	}
    WriteRT_fld (fld_LP_CONTRAST, dwTempContrast);
    if(counter>=10000)xf86DrvMsg(t->VIP->pScrn->scrnIndex, X_INFO,
				 "Rage Theatre: timeout waiting for line count (%u)\n",
				 (unsigned)ReadRT_fld (fld_VS_LINE_COUNT));



    return;

} /* RT_SetConnector ()...*/


_X_EXPORT void InitTheatre(TheatrePtr t)
{
    uint32_t data;

    
    /* 0 reset Rage Theatre */
    ShutdownTheatre(t);
    usleep(100000);
       
    t->mode=MODE_INITIALIZATION_IN_PROGRESS;
    /* 1.
     Set the VIN_PLL to NTSC value */
    RT_SetVINClock(t, RT_NTSC);

    /* Take VINRST and L54RST out of reset */
    RT_regr (VIP_PLL_CNTL1, &data);
    RT_regw (VIP_PLL_CNTL1, data & ~((RT_VINRST_RESET << 1) | (RT_L54RST_RESET << 3)));
    RT_regr (VIP_PLL_CNTL1, &data);

    /* Set VIN_CLK_SEL to PLL_VIN_CLK */
    RT_regr (VIP_CLOCK_SEL_CNTL, &data);
    RT_regw (VIP_CLOCK_SEL_CNTL, data | (RT_PLL_VIN_CLK << 7));
    RT_regr (VIP_CLOCK_SEL_CNTL, &data);

    /* 2.
     Set HW_DEBUG to 0xF000 before setting the standards registers */
    RT_regw (VIP_HW_DEBUG, 0x0000F000);
    
    /* wait for things to settle */
    usleep(100000);
    
    RT_SetStandard(t, t->wStandard);

    /* 3.
      Set DVS port to OUTPUT */
    RT_regr (VIP_DVS_PORT_CTRL, &data);
    RT_regw (VIP_DVS_PORT_CTRL, data | RT_DVSDIR_OUT);
    RT_regr (VIP_DVS_PORT_CTRL, &data);

    /* 4.
      Set default values for ADC_CNTL */
    RT_regw (VIP_ADC_CNTL, RT_ADC_CNTL_DEFAULT);

    /* 5.
      Clear the VIN_ASYNC_RST bit */
    RT_regr (VIP_MASTER_CNTL, &data);
    RT_regw (VIP_MASTER_CNTL, data & ~0x20);
    RT_regr (VIP_MASTER_CNTL, &data);

    /* Clear the DVS_ASYNC_RST bit */
    RT_regr (VIP_MASTER_CNTL, &data);
    RT_regw (VIP_MASTER_CNTL, data & ~(RT_DVS_ASYNC_RST));
    RT_regr (VIP_MASTER_CNTL, &data);

    /* Set the GENLOCK delay */
    RT_regw (VIP_HS_GENLOCKDELAY, 0x10);

    RT_regr (fld_DVS_DIRECTION, &data);
    RT_regw (fld_DVS_DIRECTION, data & RT_DVSDIR_OUT);
/*	WriteRT_fld (fld_DVS_DIRECTION, RT_DVSDIR_IN); */

    t->mode=MODE_INITIALIZED_FOR_TV_IN;
}


_X_EXPORT void ShutdownTheatre(TheatrePtr t)
{
    WriteRT_fld (fld_VIN_ASYNC_RST, RT_ASYNC_DISABLE);
    WriteRT_fld (fld_VINRST       , RT_VINRST_RESET);
    WriteRT_fld (fld_ADC_PDWN     , RT_ADC_DISABLE);
    WriteRT_fld (fld_DVS_DIRECTION, RT_DVSDIR_IN);
    t->mode=MODE_UNINITIALIZED;
}

_X_EXPORT void DumpRageTheatreRegs(TheatrePtr t)
{
    int i;
    uint32_t data;
    
    for(i=0;i<0x900;i+=4)
    {
       RT_regr(i, &data);
       xf86DrvMsg(t->VIP->pScrn->scrnIndex, X_INFO,
		  "register 0x%04x is equal to 0x%08x\n", i, (unsigned)data);
    }   

}

void DumpRageTheatreRegsByName(TheatrePtr t)
{
    int i;
    uint32_t data;
    struct { char *name; long addr; } rt_reg_list[]={
    { "ADC_CNTL                ", 0x0400 },
    { "ADC_DEBUG               ", 0x0404 },
    { "AUD_CLK_DIVIDERS        ", 0x00e8 },
    { "AUD_DTO_INCREMENTS      ", 0x00ec },
    { "AUD_PLL_CNTL            ", 0x00e0 },
    { "AUD_PLL_FINE_CNTL       ", 0x00e4 },
    { "CLKOUT_CNTL             ", 0x004c },
    { "CLKOUT_GPIO_CNTL        ", 0x0038 },
    { "CLOCK_SEL_CNTL          ", 0x00d0 },
    { "COMB_CNTL0              ", 0x0440 },
    { "COMB_CNTL1              ", 0x0444 },
    { "COMB_CNTL2              ", 0x0448 },
    { "COMB_LINE_LENGTH        ", 0x044c },
    { "CP_ACTIVE_GAIN          ", 0x0594 },
    { "CP_AGC_CNTL             ", 0x0590 },
    { "CP_BURST_GAIN           ", 0x058c },
    { "CP_DEBUG_FORCE          ", 0x05b8 },
    { "CP_HUE_CNTL             ", 0x0588 },
    { "CP_PLL_CNTL0            ", 0x0580 },
    { "CP_PLL_CNTL1            ", 0x0584 },
    { "CP_PLL_STATUS0          ", 0x0598 },
    { "CP_PLL_STATUS1          ", 0x059c },
    { "CP_PLL_STATUS2          ", 0x05a0 },
    { "CP_PLL_STATUS3          ", 0x05a4 },
    { "CP_PLL_STATUS4          ", 0x05a8 },
    { "CP_PLL_STATUS5          ", 0x05ac },
    { "CP_PLL_STATUS6          ", 0x05b0 },
    { "CP_PLL_STATUS7          ", 0x05b4 },
    { "CP_VERT_LOCKOUT         ", 0x05bc },
    { "CRC_CNTL                ", 0x02c0 },
    { "CRT_DTO_INCREMENTS      ", 0x0394 },
    { "CRT_PLL_CNTL            ", 0x00c4 },
    { "CRT_PLL_FINE_CNTL       ", 0x00bc },
    { "DECODER_DEBUG_CNTL      ", 0x05d4 },
    { "DELAY_ONE_MAP_A         ", 0x0114 },
    { "DELAY_ONE_MAP_B         ", 0x0118 },
    { "DELAY_ZERO_MAP_A        ", 0x011c },
    { "DELAY_ZERO_MAP_B        ", 0x0120 },
    { "DFCOUNT                 ", 0x00a4 },
    { "DFRESTART               ", 0x00a8 },
    { "DHRESTART               ", 0x00ac },
    { "DVRESTART               ", 0x00b0 },
    { "DVS_PORT_CTRL           ", 0x0610 },
    { "DVS_PORT_READBACK       ", 0x0614 },
    { "FIFOA_CONFIG            ", 0x0800 },
    { "FIFOB_CONFIG            ", 0x0804 },
    { "FIFOC_CONFIG            ", 0x0808 },
    { "FRAME_LOCK_CNTL         ", 0x0100 },
    { "GAIN_LIMIT_SETTINGS     ", 0x01e4 },
    { "GPIO_CNTL               ", 0x0034 },
    { "GPIO_INOUT              ", 0x0030 },
    { "HCOUNT                  ", 0x0090 },
    { "HDISP                   ", 0x0084 },
    { "HOST_RD_WT_CNTL         ", 0x0188 },
    { "HOST_READ_DATA          ", 0x0180 },
    { "HOST_WRITE_DATA         ", 0x0184 },
    { "HSIZE                   ", 0x0088 },
    { "HSTART                  ", 0x008c },
    { "HS_DTOINC               ", 0x0484 },
    { "HS_GENLOCKDELAY         ", 0x0490 },
    { "HS_MINMAXWIDTH          ", 0x048c },
    { "HS_PLINE                ", 0x0480 },
    { "HS_PLLGAIN              ", 0x0488 },
    { "HS_PLL_ERROR            ", 0x04a0 },
    { "HS_PLL_FS_PATH          ", 0x04a4 },
    { "HS_PULSE_WIDTH          ", 0x049c },
    { "HS_WINDOW_LIMIT         ", 0x0494 },
    { "HS_WINDOW_OC_SPEED      ", 0x0498 },
    { "HTOTAL                  ", 0x0080 },
    { "HW_DEBUG                ", 0x0010 },
    { "H_ACTIVE_WINDOW         ", 0x05c0 },
    { "H_SCALER_CONTROL        ", 0x0600 },
    { "H_VBI_WINDOW            ", 0x05c8 },
    { "I2C_CNTL                ", 0x0054 },
    { "I2C_CNTL_0              ", 0x0020 },
    { "I2C_CNTL_1              ", 0x0024 },
    { "I2C_DATA                ", 0x0028 },
    { "I2S_RECEIVE_CNTL        ", 0x081c },
    { "I2S_TRANSMIT_CNTL       ", 0x0818 },
    { "IIS_TX_CNT_REG          ", 0x0824 },
    { "INT_CNTL                ", 0x002c },
    { "L54_DTO_INCREMENTS      ", 0x00f8 },
    { "L54_PLL_CNTL            ", 0x00f0 },
    { "L54_PLL_FINE_CNTL       ", 0x00f4 },
    { "LINEAR_GAIN_SETTINGS    ", 0x01e8 },
    { "LP_AGC_CLAMP_CNTL0      ", 0x0500 },
    { "LP_AGC_CLAMP_CNTL1      ", 0x0504 },
    { "LP_BLACK_LEVEL          ", 0x051c },
    { "LP_BRIGHTNESS           ", 0x0508 },
    { "LP_CONTRAST             ", 0x050c },
    { "LP_SLICE_LEVEL          ", 0x0520 },
    { "LP_SLICE_LIMIT          ", 0x0510 },
    { "LP_SYNCTIP_LEVEL        ", 0x0524 },
    { "LP_VERT_LOCKOUT         ", 0x0528 },
    { "LP_WPA_CNTL0            ", 0x0514 },
    { "LP_WPA_CNTL1            ", 0x0518 },
    { "MASTER_CNTL             ", 0x0040 },
    { "MODULATOR_CNTL1         ", 0x0200 },
    { "MODULATOR_CNTL2         ", 0x0204 },
    { "MV_LEVEL_CNTL1          ", 0x0210 },
    { "MV_LEVEL_CNTL2          ", 0x0214 },
    { "MV_MODE_CNTL            ", 0x0208 },
    { "MV_STATUS               ", 0x0330 },
    { "MV_STRIPE_CNTL          ", 0x020c },
    { "NOISE_CNTL0             ", 0x0450 },
    { "PLL_CNTL0               ", 0x00c8 },
    { "PLL_CNTL1               ", 0x00fc },
    { "PLL_TEST_CNTL           ", 0x00cc },
    { "PRE_DAC_MUX_CNTL        ", 0x0240 },
    { "RGB_CNTL                ", 0x0048 },
    { "RIPINTF_PORT_CNTL       ", 0x003c },
    { "SCALER_IN_WINDOW        ", 0x0618 },
    { "SCALER_OUT_WINDOW       ", 0x061c },
    { "SG_BLACK_GATE           ", 0x04c0 },
    { "SG_SYNCTIP_GATE         ", 0x04c4 },
    { "SG_UVGATE_GATE          ", 0x04c8 },
    { "SINGLE_STEP_DATA        ", 0x05d8 },
    { "SPDIF_AC3_PREAMBLE      ", 0x0814 },
    { "SPDIF_CHANNEL_STAT      ", 0x0810 },
    { "SPDIF_PORT_CNTL         ", 0x080c },
    { "SPDIF_TX_CNT_REG        ", 0x0820 },
    { "STANDARD_SELECT         ", 0x0408 },
    { "SW_SCRATCH              ", 0x0014 },
    { "SYNC_CNTL               ", 0x0050 },
    { "SYNC_LOCK_CNTL          ", 0x0104 },
    { "SYNC_SIZE               ", 0x00b4 },
    { "THERMO2BIN_STATUS       ", 0x040c },
    { "TIMING_CNTL             ", 0x01c4 },
    { "TVO_DATA_DELAY_A        ", 0x0140 },
    { "TVO_DATA_DELAY_B        ", 0x0144 },
    { "TVO_SYNC_PAT_ACCUM      ", 0x0108 },
    { "TVO_SYNC_PAT_EXPECT     ", 0x0110 },
    { "TVO_SYNC_THRESHOLD      ", 0x010c },
    { "TV_DAC_CNTL             ", 0x0280 },
    { "TV_DTO_INCREMENTS       ", 0x0390 },
    { "TV_PLL_CNTL             ", 0x00c0 },
    { "TV_PLL_FINE_CNTL        ", 0x00b8 },
    { "UPSAMP_AND_GAIN_CNTL    ", 0x01e0 },
    { "UPSAMP_COEFF0_0         ", 0x0340 },
    { "UPSAMP_COEFF0_1         ", 0x0344 },
    { "UPSAMP_COEFF0_2         ", 0x0348 },
    { "UPSAMP_COEFF1_0         ", 0x034c },
    { "UPSAMP_COEFF1_1         ", 0x0350 },
    { "UPSAMP_COEFF1_2         ", 0x0354 },
    { "UPSAMP_COEFF2_0         ", 0x0358 },
    { "UPSAMP_COEFF2_1         ", 0x035c },
    { "UPSAMP_COEFF2_2         ", 0x0360 },
    { "UPSAMP_COEFF3_0         ", 0x0364 },
    { "UPSAMP_COEFF3_1         ", 0x0368 },
    { "UPSAMP_COEFF3_2         ", 0x036c },
    { "UPSAMP_COEFF4_0         ", 0x0370 },
    { "UPSAMP_COEFF4_1         ", 0x0374 },
    { "UPSAMP_COEFF4_2         ", 0x0378 },
    { "UV_ADR                  ", 0x0300 },
    { "VBI_20BIT_CNTL          ", 0x02d0 },
    { "VBI_CC_CNTL             ", 0x02c8 },
    { "VBI_CONTROL             ", 0x05d0 },
    { "VBI_DTO_CNTL            ", 0x02d4 },
    { "VBI_EDS_CNTL            ", 0x02cc },
    { "VBI_LEVEL_CNTL          ", 0x02d8 },
    { "VBI_SCALER_CONTROL      ", 0x060c },
    { "VCOUNT                  ", 0x009c },
    { "VDISP                   ", 0x0098 },
    { "VFTOTAL                 ", 0x00a0 },
    { "VIDEO_PORT_SIG          ", 0x02c4 },
    { "VIN_PLL_CNTL            ", 0x00d4 },
    { "VIN_PLL_FINE_CNTL       ", 0x00d8 },
    { "VIP_COMMAND_STATUS      ", 0x0008 },
    { "VIP_REVISION_ID         ", 0x000c },
    { "VIP_SUB_VENDOR_DEVICE_ID", 0x0004 },
    { "VIP_VENDOR_DEVICE_ID    ", 0x0000 },
    { "VSCALER_CNTL1           ", 0x01c0 },
    { "VSCALER_CNTL2           ", 0x01c8 },
    { "VSYNC_DIFF_CNTL         ", 0x03a0 },
    { "VSYNC_DIFF_LIMITS       ", 0x03a4 },
    { "VSYNC_DIFF_RD_DATA      ", 0x03a8 },
    { "VS_BLANKING_CNTL        ", 0x0544 },
    { "VS_COUNTER_CNTL         ", 0x054c },
    { "VS_DETECTOR_CNTL        ", 0x0540 },
    { "VS_FIELD_ID_CNTL        ", 0x0548 },
    { "VS_FRAME_TOTAL          ", 0x0550 },
    { "VS_LINE_COUNT           ", 0x0554 },
    { "VTOTAL                  ", 0x0094 },
    { "V_ACTIVE_WINDOW         ", 0x05c4 },
    { "V_DEINTERLACE_CONTROL   ", 0x0608 },
    { "V_SCALER_CONTROL        ", 0x0604 },
    { "V_VBI_WINDOW            ", 0x05cc },
    { "Y_FALL_CNTL             ", 0x01cc },
    { "Y_RISE_CNTL             ", 0x01d0 },
    { "Y_SAW_TOOTH_CNTL        ", 0x01d4 },
    {NULL, 0}
    };

    for(i=0; rt_reg_list[i].name!=NULL;i++){
        RT_regr(rt_reg_list[i].addr, &data);
        xf86DrvMsg(t->VIP->pScrn->scrnIndex, X_INFO,
		   "register (0x%04lx) %s is equal to 0x%08x\n",
		   rt_reg_list[i].addr, rt_reg_list[i].name, (unsigned)data);
    	}

}

_X_EXPORT void ResetTheatreRegsForNoTVout(TheatrePtr t)
{
     RT_regw(VIP_CLKOUT_CNTL, 0x0); 
     RT_regw(VIP_HCOUNT, 0x0); 
     RT_regw(VIP_VCOUNT, 0x0); 
     RT_regw(VIP_DFCOUNT, 0x0); 
     #if 0
     RT_regw(VIP_CLOCK_SEL_CNTL, 0x2b7);  /* versus 0x237 <-> 0x2b7 */
     RT_regw(VIP_VIN_PLL_CNTL, 0x60a6039);
     #endif
     RT_regw(VIP_FRAME_LOCK_CNTL, 0x0);
}


_X_EXPORT void ResetTheatreRegsForTVout(TheatrePtr t)
{
/*    RT_regw(VIP_HW_DEBUG, 0x200);   */
/*     RT_regw(VIP_INT_CNTL, 0x0); 
     RT_regw(VIP_GPIO_INOUT, 0x10090000); 
     RT_regw(VIP_GPIO_INOUT, 0x340b0000);  */
/*     RT_regw(VIP_MASTER_CNTL, 0x6e8);  */
     RT_regw(VIP_CLKOUT_CNTL, 0x29); 
#if 1
     RT_regw(VIP_HCOUNT, 0x1d1); 
     RT_regw(VIP_VCOUNT, 0x1e3); 
#else
     RT_regw(VIP_HCOUNT, 0x322); 
     RT_regw(VIP_VCOUNT, 0x151);
#endif
     RT_regw(VIP_DFCOUNT, 0x01); 
/*     RT_regw(VIP_CLOCK_SEL_CNTL, 0xb7);   versus 0x237 <-> 0x2b7 */
     RT_regw(VIP_CLOCK_SEL_CNTL, 0x2b7);  /* versus 0x237 <-> 0x2b7 */
     RT_regw(VIP_VIN_PLL_CNTL, 0x60a6039);
/*     RT_regw(VIP_PLL_CNTL1, 0xacacac74); */
     RT_regw(VIP_FRAME_LOCK_CNTL, 0x0f);
/*     RT_regw(VIP_ADC_CNTL, 0x02a420a8); 
     RT_regw(VIP_COMB_CNTL_0, 0x0d438083); 
     RT_regw(VIP_COMB_CNTL_2, 0x06080102); 
     RT_regw(VIP_HS_MINMAXWIDTH, 0x462f); 
     ...
     */
/*
     RT_regw(VIP_HS_PULSE_WIDTH, 0x359);
     RT_regw(VIP_HS_PLL_ERROR, 0xab6);
     RT_regw(VIP_HS_PLL_FS_PATH, 0x7fff08f8);
     RT_regw(VIP_VS_LINE_COUNT, 0x49b5e005);
	*/
}
