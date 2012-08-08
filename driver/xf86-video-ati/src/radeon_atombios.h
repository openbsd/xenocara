/*
 * Copyright 2007  Egbert Eich   <eich@novell.com>
 * Copyright 2007  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2007  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef RHD_ATOMBIOS_H_
# define RHD_ATOMBIOS_H_

//#include "radeon.h"

# ifdef ATOM_BIOS

typedef enum _AtomBiosRequestID {
    ATOMBIOS_INIT,
    ATOMBIOS_TEARDOWN,
# ifdef ATOM_BIOS_PARSER
    ATOMBIOS_EXEC,
#endif
    ATOMBIOS_ALLOCATE_FB_SCRATCH,
    ATOMBIOS_GET_CONNECTORS,
    ATOMBIOS_GET_PANEL_MODE,
    ATOMBIOS_GET_PANEL_EDID,
    GET_DEFAULT_ENGINE_CLOCK,
    GET_DEFAULT_MEMORY_CLOCK,
    GET_MAX_PIXEL_CLOCK_PLL_OUTPUT,
    GET_MIN_PIXEL_CLOCK_PLL_OUTPUT,
    GET_MAX_PIXEL_CLOCK_PLL_INPUT,
    GET_MIN_PIXEL_CLOCK_PLL_INPUT,
    GET_MAX_PIXEL_CLK,
    GET_REF_CLOCK,
    GET_FW_FB_START,
    GET_FW_FB_SIZE,
    ATOM_TMDS_FREQUENCY,
    ATOM_TMDS_PLL_CHARGE_PUMP,
    ATOM_TMDS_PLL_DUTY_CYCLE,
    ATOM_TMDS_PLL_VCO_GAIN,
    ATOM_TMDS_PLL_VOLTAGE_SWING,
    ATOM_LVDS_SUPPORTED_REFRESH_RATE,
    ATOM_LVDS_OFF_DELAY,
    ATOM_LVDS_SEQ_DIG_ONTO_DE,
    ATOM_LVDS_SEQ_DE_TO_BL,
    ATOM_LVDS_DITHER,
    ATOM_LVDS_DUALLINK,
    ATOM_LVDS_24BIT,
    ATOM_LVDS_GREYLVL,
    ATOM_LVDS_FPDI,
    ATOM_GPIO_QUERIES,
    ATOM_GPIO_I2C_CLK_MASK,
    ATOM_DAC1_BG_ADJ,
    ATOM_DAC1_DAC_ADJ,
    ATOM_DAC1_FORCE,
    ATOM_DAC2_CRTC2_BG_ADJ,
    ATOM_DAC2_CRTC2_DAC_ADJ,
    ATOM_DAC2_CRTC2_FORCE,
    ATOM_DAC2_CRTC2_MUX_REG_IND,
    ATOM_DAC2_CRTC2_MUX_REG_INFO,
    ATOMBIOS_GET_CV_MODES,
    FUNC_END
} AtomBiosRequestID;

typedef enum _AtomBiosResult {
    ATOM_SUCCESS,
    ATOM_FAILED,
    ATOM_NOT_IMPLEMENTED
} AtomBiosResult;

typedef struct AtomExec {
    int index;
    pointer pspace;
    pointer *dataSpace;
} AtomExecRec, *AtomExecPtr;

typedef struct AtomFb {
    unsigned int start;
    unsigned int size;
} AtomFbRec, *AtomFbPtr;

typedef union AtomBiosArg
{
    uint32_t val;
    struct rhdConnectorInfo	*connectorInfo;
    unsigned char*		EDIDBlock;
    atomBiosHandlePtr		atomhandle;
    DisplayModePtr		modes;
    AtomExecRec			exec;
    AtomFbRec			fb;
    ScrnInfoPtr                 pScrn;
} AtomBiosArgRec, *AtomBiosArgPtr;

extern AtomBiosResult
RHDAtomBiosFunc(ScrnInfoPtr pScrn, atomBiosHandlePtr handle,
		AtomBiosRequestID id, AtomBiosArgPtr data);

extern Bool
RADEONGetATOMConnectorInfoFromBIOSObject (ScrnInfoPtr pScrn);
extern Bool
RADEONGetATOMConnectorInfoFromBIOSConnectorTable (ScrnInfoPtr pScrn);

extern int
atombios_clk_gating_setup(ScrnInfoPtr pScrn, Bool enable);

extern int
atombios_static_pwrmgt_setup(ScrnInfoPtr pScrn, Bool enable);

extern int
atombios_set_engine_clock(ScrnInfoPtr pScrn, uint32_t engclock);

extern int
atombios_set_memory_clock(ScrnInfoPtr pScrn, uint32_t memclock);

extern Bool
RADEONGetATOMTVInfo(xf86OutputPtr output);

extern int
atombios_external_tmds_setup(xf86OutputPtr output, int action);

extern void
atombios_get_command_table_version(atomBiosHandlePtr atomBIOS, int index, int *major, int *minor);

extern xf86MonPtr
radeon_atom_get_edid(xf86OutputPtr output);

Bool
rhdAtomASICInit(atomBiosHandlePtr handle);

# include "xf86int10.h"
# ifdef ATOM_BIOS_PARSER
#  define INT8 INT8
#  define INT16 INT16
#  define INT32 INT32
#  include "CD_Common_Types.h"
# else
#  ifndef ULONG
typedef unsigned int ULONG;
#   define ULONG ULONG
#  endif
#  ifndef UCHAR
typedef unsigned char UCHAR;
#   define UCHAR UCHAR
#  endif
#  ifndef USHORT
typedef unsigned short USHORT;
#   define USHORT USHORT
#  endif
# endif

# include "atombios.h"
# include "ObjectID.h"


/*
 * This works around a bug in atombios.h where
 * ATOM_MAX_SUPPORTED_DEVICE_INFO is specified incorrectly.
 */

#define ATOM_MAX_SUPPORTED_DEVICE_INFO_HD (ATOM_DEVICE_RESERVEDF_INDEX+1)
typedef struct _ATOM_SUPPORTED_DEVICES_INFO_HD
{
    ATOM_COMMON_TABLE_HEADER      sHeader;
    USHORT                        usDeviceSupport;
    ATOM_CONNECTOR_INFO_I2C       asConnInfo[ATOM_MAX_SUPPORTED_DEVICE_INFO_HD];
    ATOM_CONNECTOR_INC_SRC_BITMAP asIntSrcInfo[ATOM_MAX_SUPPORTED_DEVICE_INFO_HD];
} ATOM_SUPPORTED_DEVICES_INFO_HD;

typedef struct _atomDataTables
{
    unsigned char                       *UtilityPipeLine;
    ATOM_MULTIMEDIA_CAPABILITY_INFO     *MultimediaCapabilityInfo;
    ATOM_MULTIMEDIA_CONFIG_INFO         *MultimediaConfigInfo;
    ATOM_STANDARD_VESA_TIMING           *StandardVESA_Timing;
    union {
        void                            *base;
        ATOM_FIRMWARE_INFO              *FirmwareInfo;
        ATOM_FIRMWARE_INFO_V1_2         *FirmwareInfo_V_1_2;
        ATOM_FIRMWARE_INFO_V1_3         *FirmwareInfo_V_1_3;
        ATOM_FIRMWARE_INFO_V1_4         *FirmwareInfo_V_1_4;
        ATOM_FIRMWARE_INFO_V2_1         *FirmwareInfo_V_2_1;
    } FirmwareInfo;
    ATOM_DAC_INFO                       *DAC_Info;
    union {
        void                            *base;
        ATOM_LVDS_INFO                  *LVDS_Info;
        ATOM_LVDS_INFO_V12              *LVDS_Info_v12;
    } LVDS_Info;
    ATOM_TMDS_INFO                      *TMDS_Info;
    union {
	void                            *base;
	ATOM_ANALOG_TV_INFO             *AnalogTV_Info;
	ATOM_ANALOG_TV_INFO_V1_2        *AnalogTV_Info_v1_2;
    } AnalogTV_Info;
    union {
        void                            *base;
        ATOM_SUPPORTED_DEVICES_INFO     *SupportedDevicesInfo;
        ATOM_SUPPORTED_DEVICES_INFO_2   *SupportedDevicesInfo_2;
        ATOM_SUPPORTED_DEVICES_INFO_2d1 *SupportedDevicesInfo_2d1;
        ATOM_SUPPORTED_DEVICES_INFO_HD  *SupportedDevicesInfo_HD;
    } SupportedDevicesInfo;
    ATOM_GPIO_I2C_INFO                  *GPIO_I2C_Info;
    ATOM_VRAM_USAGE_BY_FIRMWARE         *VRAM_UsageByFirmware;
    ATOM_GPIO_PIN_LUT                   *GPIO_Pin_LUT;
    ATOM_VESA_TO_INTENAL_MODE_LUT       *VESA_ToInternalModeLUT;
    union {
        void                            *base;
        ATOM_COMPONENT_VIDEO_INFO       *ComponentVideoInfo;
        ATOM_COMPONENT_VIDEO_INFO_V21   *ComponentVideoInfo_v21;
    } ComponentVideoInfo;
/**/unsigned char                       *PowerPlayInfo;
    COMPASSIONATE_DATA                  *CompassionateData;
    ATOM_DISPLAY_DEVICE_PRIORITY_INFO   *SaveRestoreInfo;
/**/unsigned char                       *PPLL_SS_Info;
    ATOM_OEM_INFO                       *OemInfo;
    ATOM_XTMDS_INFO                     *XTMDS_Info;
    ATOM_ASIC_MVDD_INFO                 *MclkSS_Info;
    ATOM_OBJECT_HEADER                  *Object_Header;
    INDIRECT_IO_ACCESS                  *IndirectIOAccess;
    ATOM_MC_INIT_PARAM_TABLE            *MC_InitParameter;
/**/unsigned char                       *ASIC_VDDC_Info;
    ATOM_ASIC_INTERNAL_SS_INFO          *ASIC_InternalSS_Info;
/**/unsigned char                       *TV_VideoMode;
    union {
        void                            *base;
        ATOM_VRAM_INFO_V2               *VRAM_Info_v2;
        ATOM_VRAM_INFO_V3               *VRAM_Info_v3;
    } VRAM_Info;
    ATOM_MEMORY_TRAINING_INFO           *MemoryTrainingInfo;
    union {
        void                            *base;
        ATOM_INTEGRATED_SYSTEM_INFO     *IntegratedSystemInfo;
        ATOM_INTEGRATED_SYSTEM_INFO_V2  *IntegratedSystemInfo_v2;
    } IntegratedSystemInfo;
    ATOM_ASIC_PROFILING_INFO            *ASIC_ProfilingInfo;
    ATOM_VOLTAGE_OBJECT_INFO            *VoltageObjectInfo;
    ATOM_POWER_SOURCE_INFO              *PowerSourceInfo;
} atomDataTables, *atomDataTablesPtr;

typedef struct _atomBiosHandle {
    ScrnInfoPtr pScrn;
    unsigned char *BIOSBase;
    atomDataTablesPtr atomDataPtr;
    unsigned int cmd_offset;
    pointer *scratchBase;
    uint32_t fbBase;
#if XSERVER_LIBPCIACCESS
    struct pci_device *device;
#else
    PCITAG PciTag;
#endif
    unsigned int BIOSImageSize;
} atomBiosHandleRec;

# endif

extern Bool
RADEONATOMGetTVTimings(ScrnInfoPtr pScrn, int index, DisplayModePtr mode);

extern void
RADEONATOMGetIGPInfo(ScrnInfoPtr pScrn);

extern Bool
RADEONGetATOMClockInfo(ScrnInfoPtr pScrn);

extern uint32_t
radeon_get_device_index(uint32_t device_support);
extern radeon_encoder_ptr
radeon_get_encoder(xf86OutputPtr output);
extern Bool
radeon_add_encoder(ScrnInfoPtr pScrn, uint32_t encoder_id, uint32_t device_support);
extern uint32_t
radeon_get_encoder_id_from_supported_device(ScrnInfoPtr pScrn, uint32_t supported_device, int dac);

void atombios_set_output_crtc_source(xf86OutputPtr output);
#endif /*  RHD_ATOMBIOS_H_ */
