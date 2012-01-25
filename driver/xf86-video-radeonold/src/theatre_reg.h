#ifndef __THEATRE_REGS_H__
#define __THEATRE_REGS_H__


#define VIPH_CH0_DATA                              0x0c00
#define VIPH_CH1_DATA                              0x0c04
#define VIPH_CH2_DATA                              0x0c08
#define VIPH_CH3_DATA                              0x0c0c
#define VIPH_CH0_ADDR                              0x0c10
#define VIPH_CH1_ADDR                              0x0c14
#define VIPH_CH2_ADDR                              0x0c18
#define VIPH_CH3_ADDR                              0x0c1c
#define VIPH_CH0_SBCNT                             0x0c20
#define VIPH_CH1_SBCNT                             0x0c24
#define VIPH_CH2_SBCNT                             0x0c28
#define VIPH_CH3_SBCNT                             0x0c2c
#define VIPH_CH0_ABCNT                             0x0c30
#define VIPH_CH1_ABCNT                             0x0c34
#define VIPH_CH2_ABCNT                             0x0c38
#define VIPH_CH3_ABCNT                             0x0c3c
#define VIPH_CONTROL                               0x0c40
#define VIPH_DV_LAT                                0x0c44
#define VIPH_BM_CHUNK                              0x0c48
#define VIPH_DV_INT                                0x0c4c
#define VIPH_TIMEOUT_STAT                          0x0c50

#define VIPH_REG_DATA                              0x0084
#define VIPH_REG_ADDR                              0x0080

/* Address Space Rage Theatre Registers (VIP Access) */
#define VIP_VIP_VENDOR_DEVICE_ID                   0x0000
#define VIP_VIP_SUB_VENDOR_DEVICE_ID               0x0004
#define VIP_VIP_COMMAND_STATUS                     0x0008
#define VIP_VIP_REVISION_ID                        0x000c
#define VIP_HW_DEBUG                               0x0010
#define VIP_SW_SCRATCH                             0x0014
#define VIP_I2C_CNTL_0                             0x0020
#define VIP_I2C_CNTL_1                             0x0024
#define VIP_I2C_DATA                               0x0028
#define VIP_INT_CNTL                               0x002c
/* RT200 */
#define VIP_INT_CNTL__FB_INT0                      0x02000000
#define VIP_INT_CNTL__FB_INT0_CLR                  0x02000000
#define VIP_GPIO_INOUT                             0x0030
#define VIP_GPIO_CNTL                              0x0034
#define VIP_CLKOUT_GPIO_CNTL                       0x0038
#define VIP_RIPINTF_PORT_CNTL                      0x003c

/* RT200 */
#define VIP_GPIO_INOUT                             0x0030
#define VIP_GPIO_CNTL                              0x0034
#define VIP_HOSTINTF_PORT_CNTL                     0x003c
#define VIP_HOSTINTF_PORT_CNTL__HAD_HCTL_SDA_SN    0x00000008
#define VIP_HOSTINTF_PORT_CNTL__HAD_HCTL_SDA_SP    0x00000080
#define VIP_HOSTINTF_PORT_CNTL__HAD_HCTL_SDA_SR    0x00000100
#define VIP_HOSTINTF_PORT_CNTL__SUB_SYS_ID_EN      0x00010000
#define VIP_HOSTINTF_PORT_CNTL__FIFO_RW_MODE       0x00300000
#define VIP_HOSTINTF_PORT_CNTL__FIFOA_ENDIAN_SWAP  0x00c00000
#define VIP_HOSTINTF_PORT_CNTL__FIFOB_ENDIAN_SWAP  0x03000000
#define VIP_HOSTINTF_PORT_CNTL__FIFOC_ENDIAN_SWAP  0x0c000000
#define VIP_HOSTINTF_PORT_CNTL__FIFOD_ENDIAN_SWAP  0x30000000
#define VIP_HOSTINTF_PORT_CNTL__FIFOE_ENDIAN_SWAP  0xc0000000

/* RT200 */
#define VIP_DSP_PLL_CNTL                           0x0bc

/* RT200 */
#define VIP_TC_SOURCE                              0x300
#define VIP_TC_DESTINATION                         0x304
#define VIP_TC_COMMAND                             0x308

/* RT200 */
#define VIP_TC_STATUS                              0x030c
#define VIP_TC_STATUS__TC_CHAN_BUSY                0x00007fff
#define VIP_TC_STATUS__TC_WRITE_PENDING            0x00008000
#define VIP_TC_STATUS__TC_FIFO_4_EMPTY             0x00040000
#define VIP_TC_STATUS__TC_FIFO_6_EMPTY             0x00080000
#define VIP_TC_STATUS__TC_FIFO_8_EMPTY             0x00100000
#define VIP_TC_STATUS__TC_FIFO_10_EMPTY            0x00200000
#define VIP_TC_STATUS__TC_FIFO_4_FULL              0x04000000
#define VIP_TC_STATUS__TC_FIFO_6_FULL              0x08080000
#define VIP_TC_STATUS__TC_FIFO_8_FULL              0x10080000
#define VIP_TC_STATUS__TC_FIFO_10_FULL             0x20080000
#define VIP_TC_STATUS__DSP_ILLEGAL_OP              0x80080000

/* RT200 */
#define VIP_TC_DOWNLOAD                            0x0310
#define VIP_TC_DOWNLOAD__TC_DONE_MASK              0x00003fff
#define VIP_TC_DOWNLOAD__TC_RESET_MODE             0x00060000

/* RT200 */
#define VIP_FB_INT                                 0x0314
#define VIP_FB_INT__INT_7                          0x00000080
#define VIP_FB_SCRATCH0                            0x0318 
#define VIP_FB_SCRATCH1                            0x031c 

#define VIP_ADC_CNTL                             0x0400
#define VIP_ADC_DEBUG                            0x0404
#define VIP_STANDARD_SELECT                      0x0408
#define VIP_THERMO2BIN_STATUS                    0x040c
#define VIP_COMB_CNTL0                           0x0440
#define VIP_COMB_CNTL1                           0x0444
#define VIP_COMB_CNTL2                           0x0448
#define VIP_COMB_LINE_LENGTH                     0x044c
#define VIP_NOISE_CNTL0                          0x0450
#define VIP_HS_PLINE                             0x0480
#define VIP_HS_DTOINC                            0x0484
#define VIP_HS_PLLGAIN                           0x0488
#define VIP_HS_MINMAXWIDTH                       0x048c
#define VIP_HS_GENLOCKDELAY                      0x0490
#define VIP_HS_WINDOW_LIMIT                      0x0494
#define VIP_HS_WINDOW_OC_SPEED                   0x0498
#define VIP_HS_PULSE_WIDTH                       0x049c
#define VIP_HS_PLL_ERROR                         0x04a0
#define VIP_HS_PLL_FS_PATH                       0x04a4
#define VIP_SG_BLACK_GATE                        0x04c0
#define VIP_SG_SYNCTIP_GATE                      0x04c4
#define VIP_SG_UVGATE_GATE                       0x04c8
#define VIP_LP_AGC_CLAMP_CNTL0                   0x0500
#define VIP_LP_AGC_CLAMP_CNTL1                   0x0504
#define VIP_LP_BRIGHTNESS                        0x0508
#define VIP_LP_CONTRAST                          0x050c
#define VIP_LP_SLICE_LIMIT                       0x0510
#define VIP_LP_WPA_CNTL0                         0x0514
#define VIP_LP_WPA_CNTL1                         0x0518
#define VIP_LP_BLACK_LEVEL                       0x051c
#define VIP_LP_SLICE_LEVEL                       0x0520
#define VIP_LP_SYNCTIP_LEVEL                     0x0524
#define VIP_LP_VERT_LOCKOUT                      0x0528
#define VIP_VS_DETECTOR_CNTL                     0x0540
#define VIP_VS_BLANKING_CNTL                     0x0544
#define VIP_VS_FIELD_ID_CNTL                     0x0548
#define VIP_VS_COUNTER_CNTL                      0x054c
#define VIP_VS_FRAME_TOTAL                       0x0550
#define VIP_VS_LINE_COUNT                        0x0554
#define VIP_CP_PLL_CNTL0                         0x0580
#define VIP_CP_PLL_CNTL1                         0x0584
#define VIP_CP_HUE_CNTL                          0x0588
#define VIP_CP_BURST_GAIN                        0x058c
#define VIP_CP_AGC_CNTL                          0x0590
#define VIP_CP_ACTIVE_GAIN                       0x0594
#define VIP_CP_PLL_STATUS0                       0x0598
#define VIP_CP_PLL_STATUS1                       0x059c
#define VIP_CP_PLL_STATUS2                       0x05a0
#define VIP_CP_PLL_STATUS3                       0x05a4
#define VIP_CP_PLL_STATUS4                       0x05a8
#define VIP_CP_PLL_STATUS5                       0x05ac
#define VIP_CP_PLL_STATUS6                       0x05b0
#define VIP_CP_PLL_STATUS7                       0x05b4
#define VIP_CP_DEBUG_FORCE                       0x05b8
#define VIP_CP_VERT_LOCKOUT                      0x05bc
#define VIP_H_ACTIVE_WINDOW                      0x05c0
#define VIP_V_ACTIVE_WINDOW                      0x05c4
#define VIP_H_VBI_WINDOW                         0x05c8
#define VIP_V_VBI_WINDOW                         0x05cc
#define VIP_VBI_CONTROL                          0x05d0
#define VIP_DECODER_DEBUG_CNTL                   0x05d4
#define VIP_SINGLE_STEP_DATA                     0x05d8
#define VIP_MASTER_CNTL                          0x0040
#define VIP_RGB_CNTL                             0x0048
#define VIP_CLKOUT_CNTL                          0x004c
#define VIP_SYNC_CNTL                            0x0050
#define VIP_I2C_CNTL                             0x0054
#define VIP_HTOTAL                               0x0080
#define VIP_HDISP                                0x0084
#define VIP_HSIZE                                0x0088
#define VIP_HSTART                               0x008c
#define VIP_HCOUNT                               0x0090
#define VIP_VTOTAL                               0x0094
#define VIP_VDISP                                0x0098
#define VIP_VCOUNT                               0x009c
#define VIP_VFTOTAL                              0x00a0
#define VIP_DFCOUNT                              0x00a4
#define VIP_DFRESTART                            0x00a8
#define VIP_DHRESTART                            0x00ac
#define VIP_DVRESTART                            0x00b0
#define VIP_SYNC_SIZE                            0x00b4
#define VIP_TV_PLL_FINE_CNTL                     0x00b8
#define VIP_CRT_PLL_FINE_CNTL                    0x00bc
#define VIP_TV_PLL_CNTL                          0x00c0
#define VIP_CRT_PLL_CNTL                         0x00c4
#define VIP_PLL_CNTL0                            0x00c8
#define VIP_PLL_TEST_CNTL                        0x00cc
#define VIP_CLOCK_SEL_CNTL                       0x00d0
#define VIP_VIN_PLL_CNTL                         0x00d4
#define VIP_VIN_PLL_FINE_CNTL                    0x00d8
#define VIP_AUD_PLL_CNTL                         0x00e0
#define VIP_AUD_PLL_FINE_CNTL                    0x00e4
#define VIP_AUD_CLK_DIVIDERS                     0x00e8
#define VIP_AUD_DTO_INCREMENTS                   0x00ec
#define VIP_L54_PLL_CNTL                         0x00f0
#define VIP_L54_PLL_FINE_CNTL                    0x00f4
#define VIP_L54_DTO_INCREMENTS                   0x00f8
#define VIP_PLL_CNTL1                            0x00fc
#define VIP_FRAME_LOCK_CNTL                      0x0100
#define VIP_SYNC_LOCK_CNTL                       0x0104
#define VIP_TVO_SYNC_PAT_ACCUM                   0x0108
#define VIP_TVO_SYNC_THRESHOLD                   0x010c
#define VIP_TVO_SYNC_PAT_EXPECT                  0x0110
#define VIP_DELAY_ONE_MAP_A                      0x0114
#define VIP_DELAY_ONE_MAP_B                      0x0118
#define VIP_DELAY_ZERO_MAP_A                     0x011c
#define VIP_DELAY_ZERO_MAP_B                     0x0120
#define VIP_TVO_DATA_DELAY_A                     0x0140
#define VIP_TVO_DATA_DELAY_B                     0x0144
#define VIP_HOST_READ_DATA                       0x0180
#define VIP_HOST_WRITE_DATA                      0x0184
#define VIP_HOST_RD_WT_CNTL                      0x0188
#define VIP_VSCALER_CNTL1                        0x01c0
#define VIP_TIMING_CNTL                          0x01c4
#define VIP_VSCALER_CNTL2                        0x01c8
#define VIP_Y_FALL_CNTL                          0x01cc
#define VIP_Y_RISE_CNTL                          0x01d0
#define VIP_Y_SAW_TOOTH_CNTL                     0x01d4
#define VIP_UPSAMP_AND_GAIN_CNTL                 0x01e0
#define VIP_GAIN_LIMIT_SETTINGS                  0x01e4
#define VIP_LINEAR_GAIN_SETTINGS                 0x01e8
#define VIP_MODULATOR_CNTL1                      0x0200
#define VIP_MODULATOR_CNTL2                      0x0204
#define VIP_MV_MODE_CNTL                         0x0208
#define VIP_MV_STRIPE_CNTL                       0x020c
#define VIP_MV_LEVEL_CNTL1                       0x0210
#define VIP_MV_LEVEL_CNTL2                       0x0214
#define VIP_PRE_DAC_MUX_CNTL                     0x0240
#define VIP_TV_DAC_CNTL                          0x0280
#define VIP_CRC_CNTL                             0x02c0
#define VIP_VIDEO_PORT_SIG                       0x02c4
#define VIP_VBI_CC_CNTL                          0x02c8
#define VIP_VBI_EDS_CNTL                         0x02cc
#define VIP_VBI_20BIT_CNTL                       0x02d0
#define VIP_VBI_DTO_CNTL                         0x02d4
#define VIP_VBI_LEVEL_CNTL                       0x02d8
#define VIP_UV_ADR                               0x0300
#define VIP_MV_STATUS                            0x0330
#define VIP_UPSAMP_COEFF0_0                      0x0340
#define VIP_UPSAMP_COEFF0_1                      0x0344
#define VIP_UPSAMP_COEFF0_2                      0x0348
#define VIP_UPSAMP_COEFF1_0                      0x034c
#define VIP_UPSAMP_COEFF1_1                      0x0350
#define VIP_UPSAMP_COEFF1_2                      0x0354
#define VIP_UPSAMP_COEFF2_0                      0x0358
#define VIP_UPSAMP_COEFF2_1                      0x035c
#define VIP_UPSAMP_COEFF2_2                      0x0360
#define VIP_UPSAMP_COEFF3_0                      0x0364
#define VIP_UPSAMP_COEFF3_1                      0x0368
#define VIP_UPSAMP_COEFF3_2                      0x036c
#define VIP_UPSAMP_COEFF4_0                      0x0370
#define VIP_UPSAMP_COEFF4_1                      0x0374
#define VIP_UPSAMP_COEFF4_2                      0x0378
#define VIP_TV_DTO_INCREMENTS                    0x0390
#define VIP_CRT_DTO_INCREMENTS                   0x0394
#define VIP_VSYNC_DIFF_CNTL                      0x03a0
#define VIP_VSYNC_DIFF_LIMITS                    0x03a4
#define VIP_VSYNC_DIFF_RD_DATA                   0x03a8
#define VIP_SCALER_IN_WINDOW                     0x0618
#define VIP_SCALER_OUT_WINDOW                    0x061c
#define VIP_H_SCALER_CONTROL                     0x0600
#define VIP_V_SCALER_CONTROL                     0x0604
#define VIP_V_DEINTERLACE_CONTROL                0x0608
#define VIP_VBI_SCALER_CONTROL                   0x060c
#define VIP_DVS_PORT_CTRL                        0x0610
#define VIP_DVS_PORT_READBACK                    0x0614
#define VIP_FIFOA_CONFIG                         0x0800
#define VIP_FIFOB_CONFIG                         0x0804
#define VIP_FIFOC_CONFIG                         0x0808
#define VIP_SPDIF_PORT_CNTL                      0x080c
#define VIP_SPDIF_CHANNEL_STAT                   0x0810
#define VIP_SPDIF_AC3_PREAMBLE                   0x0814
#define VIP_I2S_TRANSMIT_CNTL                    0x0818
#define VIP_I2S_RECEIVE_CNTL                     0x081c
#define VIP_SPDIF_TX_CNT_REG                     0x0820
#define VIP_IIS_TX_CNT_REG                       0x0824

/* Status defines */
#define VIP_BUSY 0
#define VIP_IDLE 1
#define VIP_RESET 2

#define VIPH_TIMEOUT_STAT__VIPH_FIFO0_STAT 0x00000001
#define VIPH_TIMEOUT_STAT__VIPH_FIFO0_AK 0x00000001
#define VIPH_TIMEOUT_STAT__VIPH_FIFO1_STAT 0x00000002
#define VIPH_TIMEOUT_STAT__VIPH_FIFO1_AK 0x00000002
#define VIPH_TIMEOUT_STAT__VIPH_FIFO2_STAT 0x00000004
#define VIPH_TIMEOUT_STAT__VIPH_FIFO2_AK 0x00000004
#define VIPH_TIMEOUT_STAT__VIPH_FIFO3_STAT 0x00000008
#define VIPH_TIMEOUT_STAT__VIPH_FIFO3_AK 0x00000008

#define VIPH_TIMEOUT_STAT__VIPH_REG_STAT 0x00000010
#define VIPH_TIMEOUT_STAT__VIPH_REG_AK 0x00000010
#define VIPH_TIMEOUT_STAT__VIPH_REGR_DIS 0x01000000
#define TEST_DEBUG_CNTL__TEST_DEBUG_OUT_EN 0x00000001

#define RT100_ATI_ID 0x4D541002
#define RT200_ATI_ID 0x4d4a1002

/* Register/Field values: */
#define     RT_COMP0              0x0
#define     RT_COMP1              0x1
#define     RT_COMP2              0x2
#define     RT_YF_COMP3           0x3
#define     RT_YR_COMP3           0x4
#define     RT_YCF_COMP4          0x5
#define     RT_YCR_COMP4          0x6

/* Video standard defines */
#define     RT_NTSC           0x0
#define     RT_PAL            0x1
#define     RT_SECAM          0x2
#define     extNONE           0x0000
#define     extNTSC           0x0100
#define     extRsvd           0x0200
#define     extPAL            0x0300
#define     extPAL_M          0x0400
#define     extPAL_N          0x0500
#define     extSECAM          0x0600
#define     extPAL_NCOMB      0x0700
#define     extNTSC_J         0x0800
#define     extNTSC_443       0x0900
#define     extPAL_BGHI       0x0A00
#define     extPAL_60         0x0B00
 /* these are used in MSP3430 */
#define     extPAL_DK1	      0x0C00
#define     extPAL_AUTO       0x0D00
 /* these are used in RT200. Some are defined above */
#define		extPAL_B			0x0E00
#define 	extPAL_D			0x0F00
#define		extPAL_G			0x1000
#define		extPAL_H			0x1100
#define		extPAL_I			0x1200
#define		extSECAM_B			0x1300
#define		extSECAM_D			0x1400
#define		extSECAM_G			0x1500
#define		extSECAM_H			0x1600
#define		extSECAM_K			0x1700
#define		extSECAM_K1			0x1800
#define		extSECAM_L			0x1900
#define		extSECAM_L1			0x1A00

#define     RT_FREF_2700      6
#define     RT_FREF_2950      5

#define     RT_COMPOSITE      0x0
#define     RT_SVIDEO         0x1

#define     RT_NORM_SHARPNESS 0x03
#define     RT_HIGH_SHARPNESS 0x0F

#define     RT_HUE_PAL_DEF    0x00

#define     RT_DECINTERLACED      0x1
#define     RT_DECNONINTERLACED   0x0

#define     NTSC_LINES          525
#define     PAL_SECAM_LINES     625

#define     RT_ASYNC_ENABLE   0x0
#define     RT_ASYNC_DISABLE  0x1
#define     RT_ASYNC_RESET    0x1

#define     RT_VINRST_ACTIVE  0x0
#define     RT_VINRST_RESET   0x1
#define     RT_L54RST_RESET   0x1

#define     RT_REF_CLK        0x0
#define     RT_PLL_VIN_CLK    0x1

#define     RT_VIN_ASYNC_RST  0x20
#define     RT_DVS_ASYNC_RST  0x80

#define     RT_ADC_ENABLE     0x0
#define     RT_ADC_DISABLE    0x1

#define     RT_DVSDIR_IN      0x0
#define     RT_DVSDIR_OUT     0x1

#define     RT_DVSCLK_HIGH    0x0
#define     RT_DVSCLK_LOW     0x1

#define     RT_DVSCLK_SEL_8FS     0x0
#define     RT_DVSCLK_SEL_27MHZ   0x1

#define     RT_DVS_CONTSTREAM     0x1
#define     RT_DVS_NONCONTSTREAM  0x0

#define     RT_DVSDAT_HIGH    0x0
#define     RT_DVSDAT_LOW     0x1

#define     RT_ADC_CNTL_DEFAULT               0x03252338

/* COMB_CNTL0 FILTER SETTINGS FOR DIFFERENT STANDARDS: */
#define     RT_NTSCM_COMB_CNTL0_COMPOSITE     0x09438090  /* was 0x09438090 */
#define     RT_NTSCM_COMB_CNTL0_SVIDEO        0x48540000

#define     RT_PAL_COMB_CNTL0_COMPOSITE       0x09438090
#define     RT_PAL_COMB_CNTL0_SVIDEO          0x40348090

#define     RT_SECAM_COMB_CNTL0_COMPOSITE     0xD0108090 /* instead of orig 0xD0088090 - eric*/
#define     RT_SECAM_COMB_CNTL0_SVIDEO        0x50148090

#define     RT_PALN_COMB_CNTL0_COMPOSITE      0x09438090
#define     RT_PALN_COMB_CNTL0_SVIDEO         0x40348090

#define     RT_PALM_COMB_CNTL0_COMPOSITE      0x09438090
#define     RT_PALM_COMB_CNTL0_SVIDEO         0x40348090
/* End of filter settings. */

/* COMB_CNTL1 FILTER SETTINGS FOR DIFFERENT STANDARDS: */
#define     RT_NTSCM_COMB_CNTL1_COMPOSITE     0x00000010
#define     RT_NTSCM_COMB_CNTL1_SVIDEO        0x00000081

#define     RT_PAL_COMB_CNTL1_COMPOSITE       0x00000010
#define     RT_PAL_COMB_CNTL1_SVIDEO          0x000000A1

#define     RT_SECAM_COMB_CNTL1_COMPOSITE     0x00000091
#define     RT_SECAM_COMB_CNTL1_SVIDEO        0x00000081

#define     RT_PALN_COMB_CNTL1_COMPOSITE      0x00000010
#define     RT_PALN_COMB_CNTL1_SVIDEO         0x000000A1

#define     RT_PALM_COMB_CNTL1_COMPOSITE      0x00000010
#define     RT_PALM_COMB_CNTL1_SVIDEO         0x000000A1
/* End of filter settings. */

/* COMB_CNTL2 FILTER SETTINGS FOR DIFFERENT STANDARDS: */
#define     RT_NTSCM_COMB_CNTL2_COMPOSITE     0x16161010
#define     RT_NTSCM_COMB_CNTL2_SVIDEO        0xFFFFFFFF

#define     RT_PAL_COMB_CNTL2_COMPOSITE       0x06080102 /* instead of 0x16161010 - Ivo */
#define     RT_PAL_COMB_CNTL2_SVIDEO          0x06080102

#define     RT_SECAM_COMB_CNTL2_COMPOSITE     0xffffffff /* instead of 0x06080102 - eric */
#define     RT_SECAM_COMB_CNTL2_SVIDEO        0x06080102

#define     RT_PALN_COMB_CNTL2_COMPOSITE      0x06080102
#define     RT_PALN_COMB_CNTL2_SVIDEO         0x06080102

#define     RT_PALM_COMB_CNTL2_COMPOSITE      0x06080102
#define     RT_PALM_COMB_CNTL2_SVIDEO         0x06080102
/* End of filter settings. */

/* COMB_LINE_LENGTH FILTER SETTINGS FOR DIFFERENT STANDARDS: */
#define     RT_NTSCM_COMB_LENGTH_COMPOSITE    0x0718038A
#define     RT_NTSCM_COMB_LENGTH_SVIDEO       0x0718038A

#define     RT_PAL_COMB_LENGTH_COMPOSITE      0x08DA046B
#define     RT_PAL_COMB_LENGTH_SVIDEO         0x08DA046B

#define     RT_SECAM_COMB_LENGTH_COMPOSITE    0x08DA046A
#define     RT_SECAM_COMB_LENGTH_SVIDEO       0x08DA046A

#define     RT_PALN_COMB_LENGTH_COMPOSITE     0x07260391
#define     RT_PALN_COMB_LENGTH_SVIDEO        0x07260391

#define     RT_PALM_COMB_LENGTH_COMPOSITE     0x07160389
#define     RT_PALM_COMB_LENGTH_SVIDEO        0x07160389
/* End of filter settings. */

/* LP_AGC_CLAMP_CNTL0 */
#define     RT_NTSCM_SYNCTIP_REF0              0x00000037
#define     RT_NTSCM_SYNCTIP_REF1              0x00000029
#define     RT_NTSCM_CLAMP_REF                 0x0000003B
#define     RT_NTSCM_PEAKWHITE                 0x000000FF
#define     RT_NTSCM_VBI_PEAKWHITE             0x000000D2  /* was 0xc2 - docs say d2 */

#define     RT_NTSCM_WPA_THRESHOLD             0x00000406
#define     RT_NTSCM_WPA_TRIGGER_LO            0x000000B3

#define     RT_NTSCM_WPA_TRIGGER_HIGH          0x0000021B

#define     RT_NTSCM_LP_LOCKOUT_START          0x00000206
#define     RT_NTSCM_LP_LOCKOUT_END            0x00000021
#define     RT_NTSCM_CH_DTO_INC                0x00400000
#define     RT_NTSCM_CH_PLL_SGAIN              0x00000001
#define     RT_NTSCM_CH_PLL_FGAIN              0x00000002

#define     RT_NTSCM_CR_BURST_GAIN             0x0000007A
#define     RT_NTSCM_CB_BURST_GAIN             0x000000AC

#define     RT_NTSCM_CH_HEIGHT                 0x000000CD
#define     RT_NTSCM_CH_KILL_LEVEL             0x000000C0
#define     RT_NTSCM_CH_AGC_ERROR_LIM          0x00000002
#define     RT_NTSCM_CH_AGC_FILTER_EN          0x00000000
#define     RT_NTSCM_CH_AGC_LOOP_SPEED         0x00000000

#define     RT_NTSCM_CRDR_ACTIVE_GAIN          0x0000007A
#define     RT_NTSCM_CBDB_ACTIVE_GAIN          0x000000AC

#define     RT_NTSCM_VERT_LOCKOUT_START        0x00000207
#define     RT_NTSCM_VERT_LOCKOUT_END          0x0000000E

#define     RT_NTSCJ_SYNCTIP_REF0              0x00000004
#define     RT_NTSCJ_SYNCTIP_REF1              0x00000012
#define     RT_NTSCJ_CLAMP_REF                 0x0000003B
#define     RT_NTSCJ_PEAKWHITE                 0x000000CB
#define     RT_NTSCJ_VBI_PEAKWHITE             0x000000C2
#define     RT_NTSCJ_WPA_THRESHOLD             0x000004B0
#define     RT_NTSCJ_WPA_TRIGGER_LO            0x000000B4
#define     RT_NTSCJ_WPA_TRIGGER_HIGH          0x0000021C
#define     RT_NTSCJ_LP_LOCKOUT_START          0x00000206
#define     RT_NTSCJ_LP_LOCKOUT_END            0x00000021

#define     RT_NTSCJ_CR_BURST_GAIN             0x00000071
#define     RT_NTSCJ_CB_BURST_GAIN             0x0000009F
#define     RT_NTSCJ_CH_HEIGHT                 0x000000CD
#define     RT_NTSCJ_CH_KILL_LEVEL             0x000000C0
#define     RT_NTSCJ_CH_AGC_ERROR_LIM          0x00000002
#define     RT_NTSCJ_CH_AGC_FILTER_EN          0x00000000
#define     RT_NTSCJ_CH_AGC_LOOP_SPEED         0x00000000

#define     RT_NTSCJ_CRDR_ACTIVE_GAIN          0x00000071
#define     RT_NTSCJ_CBDB_ACTIVE_GAIN          0x0000009F
#define     RT_NTSCJ_VERT_LOCKOUT_START        0x00000207
#define     RT_NTSCJ_VERT_LOCKOUT_END          0x0000000E

#define     RT_PAL_SYNCTIP_REF0                0x37  /* instead of 0x00000004 - Ivo */
#define     RT_PAL_SYNCTIP_REF1                0x26  /* instead of 0x0000000F - Ivo */
#define     RT_PAL_CLAMP_REF                   0x0000003B
#define     RT_PAL_PEAKWHITE                   0xFF /* instead of 0x000000C1 -  Ivo */
#define     RT_PAL_VBI_PEAKWHITE               0xC6 /* instead of 0x000000C7 - Ivo */
#define     RT_PAL_WPA_THRESHOLD               0x59C /* instead of 0x000006A4 - Ivo */

#define     RT_PAL_WPA_TRIGGER_LO              0x00000096
#define     RT_PAL_WPA_TRIGGER_HIGH            0x000001C2
#define     RT_PAL_LP_LOCKOUT_START            0x00000263
#define     RT_PAL_LP_LOCKOUT_END              0x0000002C

#define     RT_PAL_CH_DTO_INC                  0x00400000
#define     RT_PAL_CH_PLL_SGAIN                1   /* instead of 0x00000002 - Ivo */
#define     RT_PAL_CH_PLL_FGAIN                2   /* instead of 0x00000001 - Ivo */
#define     RT_PAL_CR_BURST_GAIN               0x0000007A
#define     RT_PAL_CB_BURST_GAIN               0x000000AB
#define     RT_PAL_CH_HEIGHT                   0x0000009C
#define     RT_PAL_CH_KILL_LEVEL               4   /* instead of 0x00000090 - Ivo */
#define     RT_PAL_CH_AGC_ERROR_LIM            1   /* instead of 0x00000002 - Ivo */
#define     RT_PAL_CH_AGC_FILTER_EN            1   /* instead of 0x00000000 - Ivo */
#define     RT_PAL_CH_AGC_LOOP_SPEED           0x00000000

#define     RT_PAL_CRDR_ACTIVE_GAIN            0x9E /* instead of 0x0000007A - Ivo */
#define     RT_PAL_CBDB_ACTIVE_GAIN            0xDF /* instead of 0x000000AB - Ivo */
#define     RT_PAL_VERT_LOCKOUT_START          0x00000269
#define     RT_PAL_VERT_LOCKOUT_END            0x00000012

#define     RT_SECAM_SYNCTIP_REF0              0x37 /* instead of 0x00000004 - Ivo */
#define     RT_SECAM_SYNCTIP_REF1              0x26 /* instead of 0x0000000F - Ivo */
#define     RT_SECAM_CLAMP_REF                 0x0000003B
#define     RT_SECAM_PEAKWHITE                 0xFF /* instead of 0x000000C1 - Ivo */
#define     RT_SECAM_VBI_PEAKWHITE             0xC6 /* instead of 0x000000C7 - Ivo */
#define     RT_SECAM_WPA_THRESHOLD             0x57A /* instead of 0x6A4,  instead of 0x0000059C is Ivo's value , -eric*/

#define     RT_SECAM_WPA_TRIGGER_LO            0x96 /* instead of 0x0000026B - eric */
#define     RT_SECAM_WPA_TRIGGER_HIGH          0x000001C2
#define     RT_SECAM_LP_LOCKOUT_START          0x263 /* instead of 0x0000026B - eric */
#define     RT_SECAM_LP_LOCKOUT_END            0x2b /* instead of 0x0000002C -eric */

#define     RT_SECAM_CH_DTO_INC                0x003E7A28
#define     RT_SECAM_CH_PLL_SGAIN              0x4 /* instead of 0x00000006 - Volodya */
#define     RT_SECAM_CH_PLL_FGAIN              0x7 /* instead of 0x00000006 -Volodya */

#define     RT_SECAM_CR_BURST_GAIN             0x1FF /* instead of 0x00000200 -Volodya */
#define     RT_SECAM_CB_BURST_GAIN             0x1FF /* instead of 0x00000200 -Volodya */
#define     RT_SECAM_CH_HEIGHT                 0x00000066
#define     RT_SECAM_CH_KILL_LEVEL             0x00000060
#define     RT_SECAM_CH_AGC_ERROR_LIM          0x00000003
#define     RT_SECAM_CH_AGC_FILTER_EN          0x00000000
#define     RT_SECAM_CH_AGC_LOOP_SPEED         0x00000000

#define     RT_SECAM_CRDR_ACTIVE_GAIN          0x11B /* instead of 0x00000200 - eric */
#define     RT_SECAM_CBDB_ACTIVE_GAIN          0x15A /* instead of 0x00000200 - eric */
#define     RT_SECAM_VERT_LOCKOUT_START        0x00000269
#define     RT_SECAM_VERT_LOCKOUT_END          0x00000012

#define     RT_PAL_VS_FIELD_BLANK_END          0x2A /* instead of 0x0000002C - Ivo*/
#define     RT_NTSCM_VS_FIELD_BLANK_END        0x0000000a

#define     RT_NTSCM_FIELD_IDLOCATION          0x00000105
#define     RT_PAL_FIELD_IDLOCATION            0x00000137

#define     RT_NTSCM_H_ACTIVE_START            0x00000070
#define     RT_NTSCM_H_ACTIVE_END              0x00000363

#define     RT_PAL_H_ACTIVE_START              0x0000009A
#define     RT_PAL_H_ACTIVE_END                0x00000439

#define     RT_NTSCM_V_ACTIVE_START            ((22-4)*2+1)
#define     RT_NTSCM_V_ACTIVE_END              ((22+240-4)*2+1)

#define     RT_PAL_V_ACTIVE_START              0x2E /* instead of 0x00000023  (Same as SECAM) - Ivo */
#define     RT_PAL_V_ACTIVE_END                0x269 /* instead of 0x00000262 - Ivo */

/* VBI */
#define     RT_NTSCM_H_VBI_WIND_START          0x32    /* instead of 0x00000049 - V.D. */
#define     RT_NTSCM_H_VBI_WIND_END            0x367   /* instead of 0x00000366 - V.D. */

#define     RT_PAL_H_VBI_WIND_START            0x00000084
#define     RT_PAL_H_VBI_WIND_END              0x0000041F

#define     RT_NTSCM_V_VBI_WIND_START          fld_V_VBI_WIND_START_def
#define     RT_NTSCM_V_VBI_WIND_END            fld_V_VBI_WIND_END_def

#define     RT_PAL_V_VBI_WIND_START            0x8 /* instead of 0x0000000B - Ivo */
#define     RT_PAL_V_VBI_WIND_END              0x2D /* instead of 0x00000022 - Ivo */

#define     RT_VBI_CAPTURE_EN                  0x00000001  /* Enable */
#define     RT_VBI_CAPTURE_DIS                 0x00000000  /* Disable */
#define     RT_RAW_CAPTURE                     0x00000002  /* Use raw Video Capture. */

#define     RT_NTSCM_VSYNC_INT_TRIGGER         0x2AA
#define     RT_PALSEM_VSYNC_INT_TRIGGER        0x353

#define     RT_NTSCM_VSYNC_INT_HOLD            0x17
#define     RT_PALSEM_VSYNC_INT_HOLD           0x1C

#define     RT_NTSCM_VS_FIELD_BLANK_START      0x206
#define     RT_PALSEM_VS_FIELD_BLANK_START     0x26D /* instead of 0x26C - Ivo */

#define     RT_FIELD_FLIP_EN                   0x4
#define     RT_V_FIELD_FLIP_INVERTED           0x2000

#define     RT_NTSCM_H_IN_START                0x70
#define     RT_PAL_H_IN_START                  154 /* instead of 144 - Ivo */
#define     RT_SECAM_H_IN_START                0x91 /* instead of 0x9A,  Ivo value is 154,  instead of 144 - Volodya, - eric */
#define     RT_NTSC_H_ACTIVE_SIZE              744
#define     RT_PAL_H_ACTIVE_SIZE               928 /* instead of 927 - Ivo */
#define     RT_SECAM_H_ACTIVE_SIZE             932 /* instead of 928, instead of 927 - Ivo, - eric */
#define     RT_NTSCM_V_IN_START                (0x23)
#define     RT_PAL_V_IN_START                  44 /* instead of (45-6) - Ivo */
#define     RT_SECAM_V_IN_START                0x2C /* instead of (45-6) - Volodya */
#define     RT_NTSCM_V_ACTIVE_SIZE             480
#define     RT_PAL_V_ACTIVE_SIZE               572 /* instead of 575 - Ivo */
#define     RT_SECAM_V_ACTIVE_SIZE             570 /* instead of 572, instead of 575 - Ivo, - eric */

#define     RT_NTSCM_WIN_CLOSE_LIMIT           0x4D
#define     RT_NTSCJ_WIN_CLOSE_LIMIT           0x4D
#define     RT_NTSC443_WIN_CLOSE_LIMIT         0x5F
#define     RT_PALM_WIN_CLOSE_LIMIT            0x4D
#define     RT_PALN_WIN_CLOSE_LIMIT            0x5F
#define     RT_SECAM_WIN_CLOSE_LIMIT           0xC7 /* instead of 0x5F - eric */

#define     RT_NTSCM_VS_FIELD_BLANK_START      0x206

#define     RT_NTSCM_HS_PLL_SGAIN              0x5
#define     RT_NTSCM_HS_PLL_FGAIN              0x7

#define     RT_NTSCM_H_OUT_WIND_WIDTH          0x2F4
#define     RT_NTSCM_V_OUT_WIND_HEIGHT         0xF0

#define     TV          0x1
#define     LINEIN      0x2
#define     MUTE        0x3

#define  DEC_COMPOSITE              0
#define  DEC_SVIDEO                 1
#define  DEC_TUNER                  2

#define  DEC_NTSC                   0
#define  DEC_PAL                    1
#define  DEC_SECAM                  2
#define  DEC_NTSC_J                 8

#define  DEC_SMOOTH                 0
#define  DEC_SHARP                  1

/* RT Register Field Defaults: */
#define     fld_tmpReg1_def             (uint32_t) 0x00000000
#define     fld_tmpReg2_def             (uint32_t) 0x00000001
#define     fld_tmpReg3_def             (uint32_t) 0x00000002

#define     fld_LP_CONTRAST_def         (uint32_t) 0x0000006e
#define     fld_LP_BRIGHTNESS_def       (uint32_t) 0x00003ff0
#define     fld_CP_HUE_CNTL_def         (uint32_t) 0x00000000
#define     fld_LUMA_FILTER_def         (uint32_t) 0x00000001
#define     fld_H_SCALE_RATIO_def       (uint32_t) 0x00010000
#define     fld_H_SHARPNESS_def         (uint32_t) 0x00000000

#define     fld_V_SCALE_RATIO_def       (uint32_t) 0x00000800
#define     fld_V_DEINTERLACE_ON_def    (uint32_t) 0x00000001
#define     fld_V_BYPSS_def             (uint32_t) 0x00000000
#define     fld_V_DITHER_ON_def         (uint32_t) 0x00000001
#define     fld_EVENF_OFFSET_def        (uint32_t) 0x00000000
#define     fld_ODDF_OFFSET_def         (uint32_t) 0x00000000

#define     fld_INTERLACE_DETECTED_def  (uint32_t) 0x00000000

#define     fld_VS_LINE_COUNT_def       (uint32_t) 0x00000000
#define     fld_VS_DETECTED_LINES_def   (uint32_t) 0x00000000
#define     fld_VS_ITU656_VB_def        (uint32_t) 0x00000000

#define     fld_VBI_CC_DATA_def         (uint32_t) 0x00000000
#define     fld_VBI_CC_WT_def           (uint32_t) 0x00000000
#define     fld_VBI_CC_WT_ACK_def       (uint32_t) 0x00000000
#define     fld_VBI_CC_HOLD_def         (uint32_t) 0x00000000
#define     fld_VBI_DECODE_EN_def       (uint32_t) 0x00000000

#define     fld_VBI_CC_DTO_P_def        (uint32_t) 0x00001802
#define     fld_VBI_20BIT_DTO_P_def     (uint32_t) 0x0000155c

#define     fld_VBI_CC_LEVEL_def        (uint32_t) 0x0000003f
#define     fld_VBI_20BIT_LEVEL_def     (uint32_t) 0x00000059
#define     fld_VBI_CLK_RUNIN_GAIN_def  (uint32_t) 0x0000010f

#define     fld_H_VBI_WIND_START_def    (uint32_t) 0x00000041
#define     fld_H_VBI_WIND_END_def      (uint32_t) 0x00000366

#define     fld_V_VBI_WIND_START_def    (uint32_t) 0x0B  /* instead of 0x0D - V.D. */
#define     fld_V_VBI_WIND_END_def      (uint32_t) 0x24

#define     fld_VBI_20BIT_DATA0_def     (uint32_t) 0x00000000
#define     fld_VBI_20BIT_DATA1_def     (uint32_t) 0x00000000
#define     fld_VBI_20BIT_WT_def        (uint32_t) 0x00000000
#define     fld_VBI_20BIT_WT_ACK_def    (uint32_t) 0x00000000
#define     fld_VBI_20BIT_HOLD_def      (uint32_t) 0x00000000

#define     fld_VBI_CAPTURE_ENABLE_def  (uint32_t) 0x00000000

#define     fld_VBI_EDS_DATA_def        (uint32_t) 0x00000000
#define     fld_VBI_EDS_WT_def          (uint32_t) 0x00000000
#define     fld_VBI_EDS_WT_ACK_def      (uint32_t) 0x00000000
#define     fld_VBI_EDS_HOLD_def        (uint32_t) 0x00000000

#define     fld_VBI_SCALING_RATIO_def   (uint32_t) 0x00010000
#define     fld_VBI_ALIGNER_ENABLE_def  (uint32_t) 0x00000000

#define     fld_H_ACTIVE_START_def      (uint32_t) 0x00000070
#define     fld_H_ACTIVE_END_def        (uint32_t) 0x000002f0

#define     fld_V_ACTIVE_START_def      (uint32_t) ((22-4)*2+1)
#define     fld_V_ACTIVE_END_def        (uint32_t) ((22+240-4)*2+2)

#define     fld_CH_HEIGHT_def           (uint32_t) 0x000000CD
#define     fld_CH_KILL_LEVEL_def       (uint32_t) 0x000000C0
#define     fld_CH_AGC_ERROR_LIM_def    (uint32_t) 0x00000002
#define     fld_CH_AGC_FILTER_EN_def    (uint32_t) 0x00000000
#define     fld_CH_AGC_LOOP_SPEED_def   (uint32_t) 0x00000000

#define     fld_HUE_ADJ_def             (uint32_t) 0x00000000

#define     fld_STANDARD_SEL_def        (uint32_t) 0x00000000
#define     fld_STANDARD_YC_def         (uint32_t) 0x00000000

#define     fld_ADC_PDWN_def            (uint32_t) 0x00000001
#define     fld_INPUT_SELECT_def        (uint32_t) 0x00000000

#define     fld_ADC_PREFLO_def          (uint32_t) 0x00000003
#define     fld_H_SYNC_PULSE_WIDTH_def  (uint32_t) 0x00000000
#define     fld_HS_GENLOCKED_def        (uint32_t) 0x00000000
#define     fld_HS_SYNC_IN_WIN_def      (uint32_t) 0x00000000

#define     fld_VIN_ASYNC_RST_def       (uint32_t) 0x00000001
#define     fld_DVS_ASYNC_RST_def       (uint32_t) 0x00000001

/* Vendor IDs: */
#define     fld_VIP_VENDOR_ID_def       (uint32_t) 0x00001002
#define     fld_VIP_DEVICE_ID_def       (uint32_t) 0x00004d54
#define     fld_VIP_REVISION_ID_def     (uint32_t) 0x00000001

/* AGC Delay Register */
#define     fld_BLACK_INT_START_def     (uint32_t) 0x00000031
#define     fld_BLACK_INT_LENGTH_def    (uint32_t) 0x0000000f

#define     fld_UV_INT_START_def        (uint32_t) 0x0000003b
#define     fld_U_INT_LENGTH_def        (uint32_t) 0x0000000f
#define     fld_V_INT_LENGTH_def        (uint32_t) 0x0000000f
#define     fld_CRDR_ACTIVE_GAIN_def    (uint32_t) 0x0000007a
#define     fld_CBDB_ACTIVE_GAIN_def    (uint32_t) 0x000000ac

#define     fld_DVS_DIRECTION_def       (uint32_t) 0x00000000
#define     fld_DVS_VBI_UINT8_SWAP_def  (uint32_t) 0x00000000
#define     fld_DVS_CLK_SELECT_def      (uint32_t) 0x00000000
#define     fld_CONTINUOUS_STREAM_def   (uint32_t) 0x00000000
#define     fld_DVSOUT_CLK_DRV_def      (uint32_t) 0x00000001
#define     fld_DVSOUT_DATA_DRV_def     (uint32_t) 0x00000001

#define     fld_COMB_CNTL0_def          (uint32_t) 0x09438090
#define     fld_COMB_CNTL1_def          (uint32_t) 0x00000010

#define     fld_COMB_CNTL2_def          (uint32_t) 0x16161010
#define     fld_COMB_LENGTH_def         (uint32_t) 0x0718038A

#define     fld_SYNCTIP_REF0_def        (uint32_t) 0x00000037
#define     fld_SYNCTIP_REF1_def        (uint32_t) 0x00000029
#define     fld_CLAMP_REF_def           (uint32_t) 0x0000003B
#define     fld_AGC_PEAKWHITE_def       (uint32_t) 0x000000FF
#define     fld_VBI_PEAKWHITE_def       (uint32_t) 0x000000D2

#define     fld_WPA_THRESHOLD_def       (uint32_t) 0x000003B0

#define     fld_WPA_TRIGGER_LO_def      (uint32_t) 0x000000B4
#define     fld_WPA_TRIGGER_HIGH_def    (uint32_t) 0x0000021C

#define     fld_LOCKOUT_START_def       (uint32_t) 0x00000206
#define     fld_LOCKOUT_END_def         (uint32_t) 0x00000021

#define     fld_CH_DTO_INC_def          (uint32_t) 0x00400000
#define     fld_PLL_SGAIN_def           (uint32_t) 0x00000001
#define     fld_PLL_FGAIN_def           (uint32_t) 0x00000002

#define     fld_CR_BURST_GAIN_def       (uint32_t) 0x0000007a
#define     fld_CB_BURST_GAIN_def       (uint32_t) 0x000000ac

#define     fld_VERT_LOCKOUT_START_def  (uint32_t) 0x00000207
#define     fld_VERT_LOCKOUT_END_def    (uint32_t) 0x0000000E

#define     fld_H_IN_WIND_START_def     (uint32_t) 0x00000070
#define     fld_V_IN_WIND_START_def     (uint32_t) 0x00000027

#define     fld_H_OUT_WIND_WIDTH_def    (uint32_t) 0x000002f4

#define     fld_V_OUT_WIND_WIDTH_def    (uint32_t) 0x000000f0

#define     fld_HS_LINE_TOTAL_def       (uint32_t) 0x0000038E

#define     fld_MIN_PULSE_WIDTH_def     (uint32_t) 0x0000002F
#define     fld_MAX_PULSE_WIDTH_def     (uint32_t) 0x00000046

#define     fld_WIN_CLOSE_LIMIT_def     (uint32_t) 0x0000004D
#define     fld_WIN_OPEN_LIMIT_def      (uint32_t) 0x000001B7

#define     fld_VSYNC_INT_TRIGGER_def   (uint32_t) 0x000002AA

#define     fld_VSYNC_INT_HOLD_def      (uint32_t) 0x0000001D

#define     fld_VIN_M0_def              (uint32_t) 0x00000039
#define     fld_VIN_N0_def              (uint32_t) 0x0000014c
#define     fld_MNFLIP_EN_def           (uint32_t) 0x00000000
#define     fld_VIN_P_def               (uint32_t) 0x00000006
#define     fld_REG_CLK_SEL_def         (uint32_t) 0x00000000

#define     fld_VIN_M1_def              (uint32_t) 0x00000000
#define     fld_VIN_N1_def              (uint32_t) 0x00000000
#define     fld_VIN_DRIVER_SEL_def      (uint32_t) 0x00000000
#define     fld_VIN_MNFLIP_REQ_def      (uint32_t) 0x00000000
#define     fld_VIN_MNFLIP_DONE_def     (uint32_t) 0x00000000
#define     fld_TV_LOCK_TO_VIN_def      (uint32_t) 0x00000000
#define     fld_TV_P_FOR_WINCLK_def     (uint32_t) 0x00000004

#define     fld_VINRST_def              (uint32_t) 0x00000001
#define     fld_VIN_CLK_SEL_def         (uint32_t) 0x00000000

#define     fld_VS_FIELD_BLANK_START_def    (uint32_t) 0x00000206

#define     fld_VS_FIELD_BLANK_END_def  (uint32_t) 0x0000000A

/*#define     fld_VS_FIELD_IDLOCATION_def (uint32_t) 0x00000105 */
#define     fld_VS_FIELD_IDLOCATION_def (uint32_t) 0x00000001
#define     fld_VS_FRAME_TOTAL_def      (uint32_t) 0x00000217

#define     fld_SYNC_TIP_START_def      (uint32_t) 0x00000372
#define     fld_SYNC_TIP_LENGTH_def     (uint32_t) 0x0000000F

#define     fld_GAIN_FORCE_DATA_def     (uint32_t) 0x00000000
#define     fld_GAIN_FORCE_EN_def       (uint32_t) 0x00000000
#define     fld_I_CLAMP_SEL_def         (uint32_t) 0x00000003
#define     fld_I_AGC_SEL_def           (uint32_t) 0x00000001
#define     fld_EXT_CLAMP_CAP_def       (uint32_t) 0x00000001
#define     fld_EXT_AGC_CAP_def         (uint32_t) 0x00000001
#define     fld_DECI_DITHER_EN_def      (uint32_t) 0x00000001
#define     fld_ADC_PREFHI_def          (uint32_t) 0x00000000
#define     fld_ADC_CH_GAIN_SEL_def     (uint32_t) 0x00000001

#define     fld_HS_PLL_SGAIN_def        (uint32_t) 0x00000003

#define     fld_NREn_def                (uint32_t) 0x00000000
#define     fld_NRGainCntl_def          (uint32_t) 0x00000000
#define     fld_NRBWTresh_def           (uint32_t) 0x00000000
#define     fld_NRGCTresh_def           (uint32_t) 0x00000000
#define     fld_NRCoefDespeclMode_def   (uint32_t) 0x00000000

#define     fld_GPIO_5_OE_def           (uint32_t) 0x00000000
#define     fld_GPIO_6_OE_def           (uint32_t) 0x00000000

#define     fld_GPIO_5_OUT_def          (uint32_t) 0x00000000
#define     fld_GPIO_6_OUT_def          (uint32_t) 0x00000000

/* End of field default values. */

#endif
