#ifndef __RADEON_VIDEO_H__
#define __RADEON_VIDEO_H__

#include "xf86i2c.h"
#include "fi1236.h"
#include "msp3430.h"
#include "tda9885.h"
#include "uda1380.h"
#include "i2c_def.h"

#include "generic_bus.h"
#include "theatre.h"

/* Xvideo port struct */
typedef struct {
   CARD32	 transform_index;
   CARD32	 gamma; /* gamma value x 1000 */
   int           brightness;
   int           saturation;
   int           hue;
   int           contrast;
   int           red_intensity;
   int           green_intensity;
   int           blue_intensity;
   int		 ecp_div;

	/* overlay composition mode */
   int		 alpha_mode; /* 0 = key mode, 1 = global mode */
   int		 ov_alpha;
   int		 gr_alpha;

     /* i2c bus and devices */
   I2CBusPtr     i2c;
   CARD32        radeon_i2c_timing;
   CARD32        radeon_M;
   CARD32        radeon_N;
   CARD32        i2c_status;
   CARD32        i2c_cntl;
   
   FI1236Ptr     fi1236;
   CARD8         tuner_type;
   MSP3430Ptr    msp3430;
   TDA9885Ptr    tda9885;
	UDA1380Ptr	  uda1380;

   /* VIP bus and devices */
   GENERIC_BUS_Ptr  VIP;
   TheatrePtr       theatre;   

   Bool          video_stream_active;
   int           encoding;
   CARD32        frequency;
   int           volume;
   Bool          mute;
   int           sap_channel;
   int           v;
   CARD32        adjustment; /* general purpose variable */
   
#define METHOD_BOB      0
#define METHOD_SINGLE   1
#define METHOD_WEAVE    2
#define METHOD_ADAPTIVE 3

   int           overlay_deinterlacing_method;
   int		 overlay_scaler_buffer_width;
   
   int           capture_vbi_data;

   int           dec_brightness;
   int           dec_saturation;
   int           dec_hue;
   int           dec_contrast;

   Bool          doubleBuffer;
   unsigned char currentBuffer;
   RegionRec     clip;
   CARD32        colorKey;
   CARD32        videoStatus;
   Time          offTime;
   Time          freeTime;
   Bool          autopaint_colorkey;
   Bool		 crt2; /* 0=CRT1, 1=CRT2 */

#ifdef USE_EXA
   int              size;
   ExaOffscreenArea *off_screen;
#endif

   void         *video_memory;
   int           video_offset;

   Atom          device_id, location_id, instance_id;
} RADEONPortPrivRec, *RADEONPortPrivPtr;


void RADEONInitI2C(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv);
void RADEONResetI2C(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv);

void RADEONVIP_init(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv);
void RADEONVIP_reset(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv);


#endif
