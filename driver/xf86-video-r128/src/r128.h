/*
 * Copyright 1999, 2000 ATI Technologies Inc., Markham, Ontario,
 *                      Precision Insight, Inc., Cedar Park, Texas, and
 *                      VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, PRECISION INSIGHT, VA LINUX
 * SYSTEMS AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Authors:
 *   Rickard E. Faith <faith@valinux.com>
 *   Kevin E. Martin <martin@valinux.com>
 *
 */

#ifndef _R128_H_
#define _R128_H_

#include <unistd.h>
#include "xf86str.h"

				/* PCI support */
#include "xf86Pci.h"

				/* EXA support */
#ifdef USE_EXA
#include "exa.h"
#endif

				/* XAA and Cursor Support */
#ifdef HAVE_XAA_H
#include "xaa.h"
#endif
#include "xf86fbman.h"
#include "xf86Cursor.h"

				/* DDC support */
#include "xf86DDC.h"

				/* Xv support */
#include "xf86xv.h"

				/* DRI support */
#ifndef XF86DRI
#undef R128DRI
#endif

#if R128DRI
#define _XF86DRI_SERVER_
#include "r128_dripriv.h"
#include "dri.h"
#endif

#include "fb.h"
#include "xf86Crtc.h"

#include "compat-api.h"
#include "atipcirename.h"

#include "r128_probe.h"

#if HAVE_BYTESWAP_H
#include <byteswap.h>
#elif defined(USE_SYS_ENDIAN_H)
#include <sys/endian.h>
#else
#define bswap_16(value)  \
        ((((value) & 0xff) << 8) | ((value) >> 8))

#define bswap_32(value) \
        (((uint32_t)bswap_16((uint16_t)((value) & 0xffff)) << 16) | \
        (uint32_t)bswap_16((uint16_t)((value) >> 16)))

#define bswap_64(value) \
        (((uint64_t)bswap_32((uint32_t)((value) & 0xffffffff)) \
            << 32) | \
        (uint64_t)bswap_32((uint32_t)((value) >> 32)))
#endif

#if X_BYTE_ORDER == X_BIG_ENDIAN
#define le32_to_cpu(x) bswap_32(x)
#define le16_to_cpu(x) bswap_16(x)
#define cpu_to_le32(x) bswap_32(x)
#define cpu_to_le16(x) bswap_16(x)
#else
#define le32_to_cpu(x) (x)
#define le16_to_cpu(x) (x)
#define cpu_to_le32(x) (x)
#define cpu_to_le16(x) (x)
#endif

#define R128_DEBUG          0   /* Turn off debugging output               */
#define R128_IDLE_RETRY    32   /* Fall out of idle loops after this count */
#define R128_TIMEOUT  2000000   /* Fall out of wait loops after this count */
#define R128_MMIOSIZE  0x4000

#define R128_VBIOS_SIZE 0x00010000

#if R128_DEBUG
#include "r128_version.h"

#endif

#if R128_DEBUG
#define DEBUG(x) x

#else
#define DEBUG(x)

#endif


/* Other macros */
#define R128_ARRAY_SIZE(x)  (sizeof(x)/sizeof(x[0]))
#define R128_ALIGN(x,bytes) (((x) + ((bytes) - 1)) & ~((bytes) - 1))
#define R128PTR(pScrn) ((R128InfoPtr)(pScrn)->driverPrivate)

#define R128_BIOS8(v)  ((info->VBIOS[(v)]))
#define R128_BIOS16(v) ((info->VBIOS[(v)])           | \
			(info->VBIOS[(v) + 1] << 8))
#define R128_BIOS32(v) ((info->VBIOS[(v)])           | \
			(info->VBIOS[(v) + 1] << 8)  | \
			(info->VBIOS[(v) + 2] << 16) | \
			(info->VBIOS[(v) + 3] << 24))

typedef struct {        /* All values in XCLKS    */
    int  ML;            /* Memory Read Latency    */
    int  MB;            /* Memory Burst Length    */
    int  Trcd;          /* RAS to CAS delay       */
    int  Trp;           /* RAS percentage         */
    int  Twr;           /* Write Recovery         */
    int  CL;            /* CAS Latency            */
    int  Tr2w;          /* Read to Write Delay    */
    int  Rloop;         /* Loop Latency           */
    int  Rloop_fudge;   /* Add to ML to get Rloop */
    const char *name;
} R128RAMRec, *R128RAMPtr;

typedef struct {
				/* Common registers */
    uint32_t   ovr_clr;
    uint32_t   ovr_wid_left_right;
    uint32_t   ovr_wid_top_bottom;
    uint32_t   ov0_scale_cntl;
    uint32_t   mpp_tb_config;
    uint32_t   mpp_gp_config;
    uint32_t   subpic_cntl;
    uint32_t   viph_control;
    uint32_t   i2c_cntl_1;
    uint32_t   gen_int_cntl;
    uint32_t   cap0_trig_cntl;
    uint32_t   cap1_trig_cntl;
    uint32_t   bus_cntl;
    uint32_t   config_cntl;

				/* Other registers to save for VT switches */
    uint32_t   dp_datatype;
    uint32_t   gen_reset_cntl;
    uint32_t   clock_cntl_index;
    uint32_t   amcgpio_en_reg;
    uint32_t   amcgpio_mask;

				/* CRTC registers */
    uint32_t   crtc_gen_cntl;
    uint32_t   crtc_ext_cntl;
    uint32_t   dac_cntl;
    uint32_t   crtc_h_total_disp;
    uint32_t   crtc_h_sync_strt_wid;
    uint32_t   crtc_v_total_disp;
    uint32_t   crtc_v_sync_strt_wid;
    uint32_t   crtc_offset;
    uint32_t   crtc_offset_cntl;
    uint32_t   crtc_pitch;

				/* CRTC2 registers */
    uint32_t   crtc2_gen_cntl;
    uint32_t   crtc2_h_total_disp;
    uint32_t   crtc2_h_sync_strt_wid;
    uint32_t   crtc2_v_total_disp;
    uint32_t   crtc2_v_sync_strt_wid;
    uint32_t   crtc2_offset;
    uint32_t   crtc2_offset_cntl;
    uint32_t   crtc2_pitch;

				/* Flat panel registers */
    uint32_t   fp_crtc_h_total_disp;
    uint32_t   fp_crtc_v_total_disp;
    uint32_t   fp_gen_cntl;
    uint32_t   fp_h_sync_strt_wid;
    uint32_t   fp_horz_stretch;
    uint32_t   fp_panel_cntl;
    uint32_t   fp_v_sync_strt_wid;
    uint32_t   fp_vert_stretch;
    uint32_t   lvds_gen_cntl;
    uint32_t   tmds_crc;
    uint32_t   tmds_transmitter_cntl;

				/* Computed values for PLL */
    uint32_t   dot_clock_freq;
    uint32_t   pll_output_freq;
    int        feedback_div;
    int        post_div;

				/* PLL registers */
    uint32_t   ppll_ref_div;
    uint32_t   ppll_div_3;
    uint32_t   ppll_div_0;
    uint32_t   htotal_cntl;

				/* Computed values for PLL2 */
    uint32_t   dot_clock_freq_2;
    uint32_t   pll_output_freq_2;
    int        feedback_div_2;
    int        post_div_2;

				/* PLL2 registers */
    uint32_t   p2pll_ref_div;
    uint32_t   p2pll_div_0;
    uint32_t   htotal_cntl2;

				/* DDA register */
    uint32_t   dda_config;
    uint32_t   dda_on_off;

				/* DDA2 register */
    uint32_t   dda2_config;
    uint32_t   dda2_on_off;

				/* Pallet */
    Bool       palette_valid;
    uint32_t   palette[256];
    uint32_t   palette2[256];
} R128SaveRec, *R128SavePtr;

typedef struct {
    uint16_t      reference_freq;
    uint16_t      reference_div;
    unsigned      min_pll_freq;
    unsigned      max_pll_freq;
    uint16_t      xclk;
} R128PLLRec, *R128PLLPtr;

typedef struct {
    int                bitsPerPixel;
    int                depth;
    int                displayWidth;
    int                pixel_code;
    int                pixel_bytes;
    DisplayModePtr     mode;
} R128FBLayout;

#ifdef USE_EXA
struct r128_2d_state {
    Bool in_use;
    Bool composite_setup;
    uint32_t dst_pitch_offset;
    uint32_t src_pitch_offset;
    uint32_t dp_gui_master_cntl;
    uint32_t dp_cntl;
    uint32_t dp_write_mask;
    uint32_t dp_brush_frgd_clr;
    uint32_t dp_brush_bkgd_clr;
    uint32_t dp_src_frgd_clr;
    uint32_t dp_src_bkgd_clr;
    uint32_t default_sc_bottom_right;
#if defined(R128DRI) && defined(RENDER)
    Bool has_mask;
    int x_offset;
    int y_offset;
    int widths[2];
    int heights[2];
    Bool is_transform[2];
    PictTransform *transform[2];
    PixmapPtr src_pix;
    PixmapPtr msk_pix;
#endif
};
#endif

typedef struct {
    EntityInfoPtr     pEnt;
    pciVideoPtr       PciInfo;
#ifndef XSERVER_LIBPCIACCESS
    PCITAG            PciTag;
#endif
    int               Chipset;

    Bool              FBDev;

    unsigned long     LinearAddr;   /* Frame buffer physical address         */
    unsigned long     MMIOAddr;     /* MMIO region physical address          */
    unsigned long     BIOSAddr;     /* BIOS physical address                 */

    void              *MMIO;        /* Map of MMIO region                    */
    void              *FB;          /* Map of frame buffer                   */
    uint8_t           *VBIOS;       /* Video BIOS for mode validation on FPs */
    int               FPBIOSstart;  /* Start of the flat panel info          */

    uint32_t          MemCntl;
    uint32_t          BusCntl;
    unsigned long     FbMapSize;    /* Size of frame buffer, in bytes        */
    Bool              HasPanelRegs; /* Current chip can connect to a FP      */

    R128PLLRec        pll;
    R128RAMPtr        ram;

    R128SaveRec       SavedReg;     /* Original (text) mode                  */
    R128SaveRec       ModeReg;      /* Current mode                          */
    Bool              (*CloseScreen)(CLOSE_SCREEN_ARGS_DECL);
    void              (*BlockHandler)(BLOCKHANDLER_ARGS_DECL);

    Bool              PaletteSavedOnVT; /* Palette saved on last VT switch   */

#ifdef HAVE_XAA_H
    XAAInfoRecPtr     accel;
#endif

    Bool              noAccel;
    Bool              accelOn;
    Bool	      useEXA;
    Bool	      RenderAccel;
#ifdef USE_EXA
    ExaDriverPtr      ExaDriver;
    XF86ModReqInfo    exaReq;
    struct r128_2d_state state_2d;
#endif

    int               fifo_slots;   /* Free slots in the FIFO (64 max)       */
    int               pix24bpp;     /* Depth of pixmap for 24bpp framebuffer */
    Bool              dac6bits;     /* Use 6 bit DAC?                        */
    Bool              swCursor;

				/* Computed values for Rage 128 */
    int               pitch;
    int               datatype;
    uint32_t          dp_gui_master_cntl;

				/* Saved values for ScreenToScreenCopy */
    int               xdir;
    int               ydir;

				/* ScanlineScreenToScreenColorExpand support */
    unsigned char     *scratch_buffer[1];
    unsigned char     *scratch_save;
    int               scanline_x;
    int               scanline_y;
    int               scanline_w;
    int               scanline_h;
#ifdef R128DRI
    int               scanline_hpass;
    int               scanline_x1clip;
    int               scanline_x2clip;
    int               scanline_rop;
    int               scanline_fg;
    int               scanline_bg;
#endif /* R128DRI */
    int               scanline_words;
    int               scanline_direct;
    int               scanline_bpp; /* Only used for ImageWrite */

    R128FBLayout      CurrentLayout;
#ifdef R128DRI
    Bool              directRenderingEnabled;
    DRIInfoPtr        pDRIInfo;
    int               drmFD;
    drm_context_t     drmCtx;

    drm_handle_t      fbHandle;

    drmSize           registerSize;
    drm_handle_t      registerHandle;

    Bool              IsPCI;            /* Current card is a PCI card */
    drmSize           pciSize;
    drm_handle_t      pciMemHandle;
    drmAddress        PCI;              /* Map */

    Bool              allowPageFlip;    /* Enable 3d page flipping */
    Bool              have3DWindows;    /* Are there any 3d clients? */
    int               drmMinor;

    drmSize           agpSize;
    drm_handle_t      agpMemHandle;     /* Handle from drmAgpAlloc */
    unsigned long     agpOffset;
    drmAddress        AGP;              /* Map */
    int               agpMode;

    Bool              CCEInUse;         /* CCE is currently active */
    int               CCEMode;          /* CCE mode that server/clients use */
    int               CCEFifoSize;      /* Size of the CCE command FIFO */
    Bool              CCESecure;        /* CCE security enabled */
    int               CCEusecTimeout;   /* CCE timeout in usecs */

				/* CCE ring buffer data */
    unsigned long     ringStart;        /* Offset into AGP space */
    drm_handle_t      ringHandle;       /* Handle from drmAddMap */
    drmSize           ringMapSize;      /* Size of map */
    int               ringSize;         /* Size of ring (in MB) */
    drmAddress        ring;             /* Map */
    int               ringSizeLog2QW;

    unsigned long     ringReadOffset;   /* Offset into AGP space */
    drm_handle_t      ringReadPtrHandle;/* Handle from drmAddMap */
    drmSize           ringReadMapSize;  /* Size of map */
    drmAddress        ringReadPtr;      /* Map */

				/* CCE vertex/indirect buffer data */
    unsigned long     bufStart;        /* Offset into AGP space */
    drm_handle_t      bufHandle;       /* Handle from drmAddMap */
    drmSize           bufMapSize;      /* Size of map */
    int               bufSize;         /* Size of buffers (in MB) */
    drmAddress        buf;             /* Map */
    int               bufNumBufs;      /* Number of buffers */
    drmBufMapPtr      buffers;         /* Buffer map */

				/* CCE AGP Texture data */
    unsigned long     agpTexStart;      /* Offset into AGP space */
    drm_handle_t      agpTexHandle;     /* Handle from drmAddMap */
    drmSize           agpTexMapSize;    /* Size of map */
    int               agpTexSize;       /* Size of AGP tex space (in MB) */
    drmAddress        agpTex;           /* Map */
    int               log2AGPTexGran;

				/* CCE 2D acceleration */
    drmBufPtr         indirectBuffer;
    int               indirectStart;

				/* DRI screen private data */
    int               fbX;
    int               fbY;
    int               backX;
    int               backY;
    int               depthX;
    int               depthY;

    int               frontOffset;
    int               frontPitch;
    int               backOffset;
    int               backPitch;
    int               depthOffset;
    int               depthPitch;
    int               spanOffset;
    int               textureOffset;
    int               textureSize;
    int               log2TexGran;

				/* Saved scissor values */
    uint32_t          sc_left;
    uint32_t          sc_right;
    uint32_t          sc_top;
    uint32_t          sc_bottom;

    uint32_t          re_top_left;
    uint32_t          re_width_height;

    uint32_t          aux_sc_cntl;

    int               irq;
    uint32_t          gen_int_cntl;

    Bool              DMAForXv;
#endif

    XF86VideoAdaptorPtr adaptor;
    void              (*VideoTimerCallback)(ScrnInfoPtr, Time);
    int               videoKey;
    Bool              showCache;
    OptionInfoPtr     Options;

    Bool              isDFP;
    Bool              isPro2;
    Bool              SwitchingMode;
    Bool              DDC;

    Bool              VGAAccess;
} R128InfoRec, *R128InfoPtr;

#define R128WaitForFifo(pScrn, entries)                                      \
do {                                                                         \
    if (info->fifo_slots < entries) R128WaitForFifoFunction(pScrn, entries); \
    info->fifo_slots -= entries;                                             \
} while (0)

/* Compute n/d with rounding. */
static inline int R128Div(int n, int d)
{
    return (n + (d / 2)) / d;
}

extern R128EntPtr R128EntPriv(ScrnInfoPtr pScrn);
extern void        R128WaitForFifoFunction(ScrnInfoPtr pScrn, int entries);
extern void        R128WaitForIdle(ScrnInfoPtr pScrn);
extern void        R128EngineReset(ScrnInfoPtr pScrn);
extern void        R128EngineFlush(ScrnInfoPtr pScrn);

extern unsigned    R128INPLL(ScrnInfoPtr pScrn, int addr);
extern void        R128WaitForVerticalSync(ScrnInfoPtr pScrn);

extern Bool R128XAAAccelInit(ScreenPtr pScreen);
extern void        R128EngineInit(ScrnInfoPtr pScrn);
extern Bool        R128CursorInit(ScreenPtr pScreen);

extern int         R128MinBits(int val);
extern xf86OutputPtr R128FirstOutput(xf86CrtcPtr crtc);

extern void        R128InitVideo(ScreenPtr pScreen);

extern void        R128InitCommonRegisters(R128SavePtr save, R128InfoPtr info);
extern void        R128InitRMXRegisters(R128SavePtr orig, R128SavePtr save, xf86OutputPtr output, DisplayModePtr mode);
extern void        R128InitFPRegisters(R128SavePtr orig, R128SavePtr save, xf86OutputPtr output);
extern void        R128InitLVDSRegisters(R128SavePtr orig, R128SavePtr save, xf86OutputPtr output);
extern void        R128RestoreCommonRegisters(ScrnInfoPtr pScrn, R128SavePtr restore);
extern void        R128RestoreDACRegisters(ScrnInfoPtr pScrn, R128SavePtr restore);
extern void        R128RestoreRMXRegisters(ScrnInfoPtr pScrn, R128SavePtr restore);
extern void        R128RestoreFPRegisters(ScrnInfoPtr pScrn, R128SavePtr restore);
extern void        R128RestoreLVDSRegisters(ScrnInfoPtr pScrn, R128SavePtr restore);
extern void        R128RestoreCrtcRegisters(ScrnInfoPtr pScrn, R128SavePtr restore);
extern void        R128RestorePLLRegisters(ScrnInfoPtr pScrn, R128SavePtr restore);
extern void        R128RestoreDDARegisters(ScrnInfoPtr pScrn, R128SavePtr restore);
extern void        R128RestoreCrtc2Registers(ScrnInfoPtr pScrn, R128SavePtr restore);
extern void        R128RestorePLL2Registers(ScrnInfoPtr pScrn, R128SavePtr restore);
extern void        R128RestoreDDA2Registers(ScrnInfoPtr pScrn, R128SavePtr restore);

extern void        r128_crtc_set_cursor_colors(xf86CrtcPtr crtc, int bg, int fg);
extern void        r128_crtc_set_cursor_position(xf86CrtcPtr crtc, int x, int y);
extern void        r128_crtc_show_cursor(xf86CrtcPtr crtc);
extern void        r128_crtc_hide_cursor(xf86CrtcPtr crtc);
extern void        r128_crtc_load_cursor_image(xf86CrtcPtr crtc, unsigned char *src);

extern uint32_t    R128AllocateMemory(ScrnInfoPtr pScrn, void **mem_struct, int size, int align, Bool need_accel);
extern Bool        R128SetupConnectors(ScrnInfoPtr pScrn);
extern Bool        R128AllocateControllers(ScrnInfoPtr pScrn);
extern void        R128GetPanelInfoFromBIOS(xf86OutputPtr output);
extern void        R128Blank(ScrnInfoPtr pScrn);
extern void        R128Unblank(ScrnInfoPtr pScrn);
extern void        R128DPMSSetOn(xf86OutputPtr output);
extern void        R128DPMSSetOff(xf86OutputPtr output);
extern ModeStatus     R128DoValidMode(xf86OutputPtr output, DisplayModePtr mode, int flags);
extern DisplayModePtr R128ProbeOutputModes(xf86OutputPtr output);

#ifdef R128DRI
extern Bool        R128DRIScreenInit(ScreenPtr pScreen);
extern void        R128DRICloseScreen(ScreenPtr pScreen);
extern Bool        R128DRIFinishScreenInit(ScreenPtr pScreen);

#define R128CCE_START(pScrn, info)					\
do {									\
    int _ret = drmCommandNone(info->drmFD, DRM_R128_CCE_START);		\
    if (_ret) {								\
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,				\
		   "%s: CCE start %d\n", __FUNCTION__, _ret);		\
    }									\
} while (0)

#define R128CCE_STOP(pScrn, info)					\
do {									\
    int _ret = R128CCEStop(pScrn);					\
    if (_ret) {								\
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,				\
		   "%s: CCE stop %d\n", __FUNCTION__, _ret);		\
    }									\
} while (0)

#define R128CCE_RESET(pScrn, info)					\
do {									\
    if (info->directRenderingEnabled					\
	&& R128CCE_USE_RING_BUFFER(info->CCEMode)) {			\
	int _ret = drmCommandNone(info->drmFD, DRM_R128_CCE_RESET);	\
	if (_ret) {							\
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,			\
		       "%s: CCE reset %d\n", __FUNCTION__, _ret);	\
	}								\
    }									\
} while (0)

extern drmBufPtr   R128CCEGetBuffer(ScrnInfoPtr pScrn);
#endif

extern void        R128CCEFlushIndirect(ScrnInfoPtr pScrn, int discard);
extern void        R128CCEReleaseIndirect(ScrnInfoPtr pScrn);
extern void        R128CCEWaitForIdle(ScrnInfoPtr pScrn);
extern int         R128CCEStop(ScrnInfoPtr pScrn);
extern void	   R128CopySwap(uint8_t *dst, uint8_t *src, unsigned int size, int swap);

#ifdef USE_EXA
extern Bool	   R128EXAInit(ScreenPtr pScreen, int total);
extern Bool	   R128GetDatatypeBpp(int bpp, uint32_t *type);
extern Bool	   R128GetPixmapOffsetPitch(PixmapPtr pPix, uint32_t *pitch_offset);
extern void	   R128DoPrepareCopy(ScrnInfoPtr pScrn, uint32_t src_pitch_offset,
				    uint32_t dst_pitch_offset, uint32_t datatype, int alu, Pixel planemask);
extern void R128Done(PixmapPtr pPixmap);

#ifdef R128DRI
extern void EmitCCE2DState(ScrnInfoPtr pScrn);
#endif

#ifdef RENDER
extern Bool R128CCECheckComposite(int op,
                                    PicturePtr pSrcPicture,
                                    PicturePtr pMaskPicture,
                                    PicturePtr pDstPicture);
extern Bool R128CCEPrepareComposite(int op,                                    PicturePtr pSrcPicture,
                                    PicturePtr pMaskPicture,
                                    PicturePtr pDstPicture,
                                    PixmapPtr pSrc,
                                    PixmapPtr pMask,
                                    PixmapPtr pDst);
extern void R128CCEComposite(PixmapPtr pDst,
                                int srcX, int srcY,
                                int maskX, int maskY,
                                int dstX, int dstY,
                                int w, int h);
#define R128CCEDoneComposite R128Done
#endif
#endif


#define CCE_PACKET0( reg, n )						\
	(R128_CCE_PACKET0 | ((n) << 16) | ((reg) >> 2))
#define CCE_PACKET1( reg0, reg1 )					\
	(R128_CCE_PACKET1 | (((reg1) >> 2) << 11) | ((reg0) >> 2))
#define CCE_PACKET2()							\
	(R128_CCE_PACKET2)
#define CCE_PACKET3( pkt, n )						\
	(R128_CCE_PACKET3 | (pkt) | ((n) << 16))


#define R128_VERBOSE	0

#define RING_LOCALS	uint32_t *__head; int __count;

#define R128CCE_REFRESH(pScrn, info)					\
do {									\
   if ( R128_VERBOSE ) {						\
      xf86DrvMsg( pScrn->scrnIndex, X_INFO, "REFRESH( %d ) in %s\n",	\
		  !info->CCEInUse , __FUNCTION__ );			\
   }									\
   if ( !info->CCEInUse ) {						\
      R128CCEWaitForIdle(pScrn);					\
      BEGIN_RING( 6 );							\
      OUT_RING_REG( R128_RE_TOP_LEFT,     info->re_top_left );		\
      OUT_RING_REG( R128_RE_WIDTH_HEIGHT, info->re_width_height );	\
      OUT_RING_REG( R128_AUX_SC_CNTL,     info->aux_sc_cntl );		\
      ADVANCE_RING();							\
      info->CCEInUse = TRUE;						\
   }									\
} while (0)

#define BEGIN_RING( n ) do {						\
   if ( R128_VERBOSE ) {						\
      xf86DrvMsg( pScrn->scrnIndex, X_INFO,				\
		  "BEGIN_RING( %d ) in %s\n", n, __FUNCTION__ );	\
   }									\
   if ( !info->indirectBuffer ) {					\
      info->indirectBuffer = R128CCEGetBuffer( pScrn );			\
      info->indirectStart = 0;						\
   } else if ( (info->indirectBuffer->used + 4*(n)) >			\
                info->indirectBuffer->total ) {				\
      R128CCEFlushIndirect( pScrn, 1 );					\
   }									\
   __head = (pointer)((char *)info->indirectBuffer->address +		\
		       info->indirectBuffer->used);			\
   __count = 0;								\
} while (0)

#define ADVANCE_RING() do {						\
   if ( R128_VERBOSE ) {						\
      xf86DrvMsg( pScrn->scrnIndex, X_INFO,				\
		  "ADVANCE_RING() used: %d+%d=%d/%d\n",			\
		  info->indirectBuffer->used - info->indirectStart,	\
		  __count * (int)sizeof(uint32_t),			\
		  info->indirectBuffer->used - info->indirectStart +	\
		  __count * (int)sizeof(uint32_t),			\
		  info->indirectBuffer->total - info->indirectStart );	\
   }									\
   info->indirectBuffer->used += __count * (int)sizeof(uint32_t);		\
} while (0)

#define OUT_RING( x ) do {						\
   if ( R128_VERBOSE ) {						\
      xf86DrvMsg( pScrn->scrnIndex, X_INFO,				\
		  "   OUT_RING( 0x%08x )\n", (unsigned int)(x) );	\
   }									\
   MMIO_OUT32(&__head[__count++], 0, (x));				\
} while (0)

#define OUT_RING_REG( reg, val )					\
do {									\
   OUT_RING( CCE_PACKET0( reg, 0 ) );					\
   OUT_RING( val );							\
} while (0)

#define FLUSH_RING()							\
do {									\
   if ( R128_VERBOSE )							\
      xf86DrvMsg( pScrn->scrnIndex, X_INFO,				\
		  "FLUSH_RING in %s\n", __FUNCTION__ );			\
   if ( info->indirectBuffer ) {					\
      R128CCEFlushIndirect( pScrn, 0 );					\
   }									\
} while (0)

#endif
