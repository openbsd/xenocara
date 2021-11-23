/*
 * Copyright 1998-2003 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2003 S3 Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * I N C L U D E S
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86Resources.h"
#endif

#include "compiler.h"
#include "xf86Pci.h"
#include "regionstr.h"
#include "via_driver.h"

#include "xf86xv.h"
#include <X11/extensions/Xv.h>
#include "dixstruct.h"
#include "via_xvpriv.h"
#include "fourcc.h"

#include "via_eng_regs.h"

/*
 * D E F I N E
 */
#define OFF_DELAY       200           /* milliseconds */
#define FREE_DELAY      60000
#define PARAMSIZE       1024
#define SLICESIZE       65536
#define OFF_TIMER       0x01
#define FREE_TIMER      0x02
#define TIMER_MASK      (OFF_TIMER | FREE_TIMER)
#define VIA_MAX_XVIMAGE_X 1920
#define VIA_MAX_XVIMAGE_Y 1200

#define LOW_BAND 0x0CB0
#define MID_BAND 0x1f10

#define  XV_IMAGE          0
#define MAKE_ATOM(a) MakeAtom(a, sizeof(a) - 1, TRUE)
#ifndef XvExtension
void
viaInitVideo(ScreenPtr pScreen)
{
}

void
viaExitVideo(ScrnInfoPtr pScrn)
{
}
void
viaSaveVideo(ScrnInfoPtr pScrn)
{
}
void
viaRestoreVideo(ScrnInfoPtr pScrn)
{
}
void
VIAVidAdjustFrame(ScrnInfoPtr pScrn, int x, int y)
{
}
#else

static vidCopyFunc viaFastVidCpy = NULL;

/*
 *  F U N C T I O N   D E C L A R A T I O N
 */
static unsigned viaSetupAdaptors(ScreenPtr pScreen,
    XF86VideoAdaptorPtr ** adaptors);
static void viaStopVideo(ScrnInfoPtr, pointer, Bool);
static void viaQueryBestSize(ScrnInfoPtr, Bool,
    short, short, short, short, unsigned int *, unsigned int *, pointer);
static int viaQueryImageAttributes(ScrnInfoPtr,
    int, unsigned short *, unsigned short *, int *, int *);
static int viaGetPortAttribute(ScrnInfoPtr, Atom, INT32 *, pointer);
static int viaSetPortAttribute(ScrnInfoPtr, Atom, INT32, pointer);
static int viaPutImage(ScrnInfoPtr, short, short, short, short, short, short,
    short, short, int, unsigned char *, short, short, Bool,
    RegionPtr, pointer, DrawablePtr);
static void nv12Blit(unsigned char *nv12Chroma,
    const unsigned char *uBuffer,
    const unsigned char *vBuffer,
    unsigned width, unsigned srcPitch, unsigned dstPitch, unsigned lines);

static Atom xvBrightness, xvContrast, xvColorKey, xvHue, xvSaturation,
    xvAutoPaint;

/*
 *  S T R U C T S
 */
/* client libraries expect an encoding */
static XF86VideoEncodingRec DummyEncoding[1] = {
    {XV_IMAGE, "XV_IMAGE", VIA_MAX_XVIMAGE_X, VIA_MAX_XVIMAGE_Y, {1, 1}},
};

#define NUM_FORMATS_G 9

static XF86VideoFormatRec FormatsG[NUM_FORMATS_G] = {
    {8, TrueColor},               /* Dithered */
    {8, PseudoColor},               /* Using .. */
    {8, StaticColor},
    {8, GrayScale},
    {8, StaticGray},               /* .. TexelLUT */
    {16, TrueColor},
    {24, TrueColor},
    {16, DirectColor},
    {24, DirectColor}
};

#define NUM_ATTRIBUTES_G 6

static char attributeXvColorkey[] = { "XV_COLORKEY" };
static char attributeXvBrightness[] = { "XV_BRIGHTNESS" };
static char attributeXvContrast[] = { "XV_CONTRAST" };
static char attributeXvSaturation[] = { "XV_SATURATION" };
static char attributeXvHue[] = { "XV_HUE" };
static char attributeXvAutopaintColorkey[] =
                                        { "XV_AUTOPAINT_COLORKEY" };

static XF86AttributeRec AttributesG[NUM_ATTRIBUTES_G] = {
    {XvSettable | XvGettable,      0,  (1 << 24) - 1,          attributeXvColorkey},
    {XvSettable | XvGettable,      0,          10000,          attributeXvBrightness},
    {XvSettable | XvGettable,      0,          20000,          attributeXvContrast},
    {XvSettable | XvGettable,      0,          20000,          attributeXvSaturation},
    {XvSettable | XvGettable,   -180,            180,                 attributeXvHue},
    {XvSettable | XvGettable,      0,              1,   attributeXvAutopaintColorkey}
};

#define NUM_IMAGES_G 7

static XF86ImageRec ImagesG[NUM_IMAGES_G] = {
    XVIMAGE_YUY2,
    XVIMAGE_YV12,
    XVIMAGE_I420,
    {
        /*
         * Below, a dummy picture type that is used in XvPutImage only to do
         * an overlay update. Introduced for the XvMC client lib.
         * Defined to have a zero data size.
         */

        FOURCC_XVMC,
        XvYUV,
        LSBFirst,
        {   'V', 'I', 'A', 0x00,
            0x00, 0x00, 0x00, 0x10, 0x80, 0x00, 0x00, 0xAA, 0x00,
            0x38, 0x9B, 0x71},
        12,
        XvPlanar,
        1,
        0, 0, 0, 0,
        8, 8, 8,
        1, 2, 2,
        1, 2, 2,
        {   'Y', 'V', 'U',
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        XvTopToBottom},
    { /* RGB 555 */
        FOURCC_RV15,
        XvRGB,
        LSBFirst,
        {   'R', 'V', '1', '5',
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00},
        16,
        XvPacked,
        1,
        15, 0x7C00, 0x03E0, 0x001F,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        {   'R', 'V', 'B', 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0},
        XvTopToBottom},
    { /* RGB 565 */
        FOURCC_RV16,
        XvRGB,
        LSBFirst,
        {   'R', 'V', '1', '6',
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00},
        16,
        XvPacked,
        1,
        16, 0xF800, 0x07E0, 0x001F,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        {   'R', 'V', 'B', 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0},
        XvTopToBottom},
    { /* RGB 888 */
        FOURCC_RV32,
        XvRGB,
        LSBFirst,
        {   'R', 'V', '3', '2',
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00},
        32,
        XvPacked,
        1,
        24, 0xff0000, 0x00ff00, 0x0000ff,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        {   'R', 'V', 'B', 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0},
        XvTopToBottom}

};

static const char *XvAdaptorName[XV_ADAPT_NUM] = {
    "XV_SWOV"
};

static XF86VideoAdaptorPtr viaAdaptPtr[XV_ADAPT_NUM];
static XF86VideoAdaptorPtr *allAdaptors;
static unsigned numAdaptPort[XV_ADAPT_NUM] = { 1 };

/*
 *  F U N C T I O N
 */

static xf86CrtcPtr
window_belongs_to_crtc(ScrnInfoPtr pScrn, int x, int y, int w, int h)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int largest = 0, area = 0, i;
    BoxRec crtc_area, overlap;
    xf86CrtcPtr best = NULL;

    for (i = 0; i < xf86_config->num_crtc; i++) {
        xf86CrtcPtr crtc = xf86_config->crtc[i];

        if (crtc->enabled) {
            crtc_area.x1 = crtc->x;
            crtc_area.x2 = crtc->x + xf86ModeWidth(&crtc->mode, crtc->rotation);
            crtc_area.y1 = crtc->y;
            crtc_area.y2 = crtc->y + xf86ModeHeight(&crtc->mode, crtc->rotation);
            overlap.x1 = crtc_area.x1 > x ? crtc_area.x1 : x;
            overlap.x2 = crtc_area.x2 < x + w ? crtc_area.x2 : x + w;
            overlap.y1 = crtc_area.y1 > y ? crtc_area.y1 : y;
            overlap.y2 = crtc_area.y2 < y ? crtc_area.y2 : y + h;

            if (overlap.x1 >= overlap.x2 || overlap.y1 >= overlap.y2)
                overlap.x1 = overlap.x2 = overlap.y1 = overlap.y2 = 0;

            area = (overlap.x2 - overlap.x1) * (overlap.y2 - overlap.y1);
            if (area > largest) {
                area = largest;
                best = crtc;
            }
        }
    }
    return best;
}

/*
 *   Decide if the mode support video overlay. This depends on the bandwidth
 *   of the mode and the type of RAM available.
 */

static Bool
DecideOverlaySupport(xf86CrtcPtr crtc)
{
    DisplayModePtr mode = &crtc->desiredMode;
    ScrnInfoPtr pScrn = crtc->scrn;
    VIAPtr pVia = VIAPTR(pScrn);

#ifdef HAVE_DEBUG
    if (pVia->disableXvBWCheck)
        return TRUE;
#endif

    /* Small trick here. We keep the height in 16's of lines and width in 32's
     * to avoid numeric overflow */

    if (pVia->ChipId != PCI_CHIP_VT3205 &&
        pVia->ChipId != PCI_CHIP_VT3204 &&
        pVia->ChipId != PCI_CHIP_VT3259 &&
        pVia->ChipId != PCI_CHIP_VT3314 &&
        pVia->ChipId != PCI_CHIP_VT3327 &&
        pVia->ChipId != PCI_CHIP_VT3336 &&
        pVia->ChipId != PCI_CHIP_VT3409 &&
        pVia->ChipId != PCI_CHIP_VT3410 &&
        pVia->ChipId != PCI_CHIP_VT3364 &&
        pVia->ChipId != PCI_CHIP_VT3324 &&
        pVia->ChipId != PCI_CHIP_VT3353) {
        CARD32 bandwidth = (mode->HDisplay >> 4) * (mode->VDisplay >> 5) *
                            pScrn->bitsPerPixel * mode->VRefresh;

        switch (pVia->MemClk) {
        case VIA_MEM_SDR100:           /* No overlay without DDR */
        case VIA_MEM_SDR133:
            return FALSE;
        case VIA_MEM_DDR200:
            /* Basic limit for DDR200 is about this */
            if (bandwidth > 1800000)
                return FALSE;
            /* But we have constraints at higher than 800x600 */
            if (mode->HDisplay > 800) {
                if (pScrn->bitsPerPixel != 8)
                    return FALSE;
                if (mode->VDisplay > 768)
                    return FALSE;
                if (mode->VRefresh > 60)
                    return FALSE;
            }
            return TRUE;
        case 0:               /*      FIXME: Why does my CLE266 report 0? */
        case VIA_MEM_DDR266:
            if (bandwidth > 7901250)
                return FALSE;
            return TRUE;
        }
        return FALSE;

    } else {
        unsigned width, height, refresh, dClock;
        float mClock, memEfficiency, needBandWidth, totalBandWidth;

        switch (pVia->MemClk) {
            case VIA_MEM_SDR100:
                mClock = 50;           /*HW base on 128 bit */
                memEfficiency = (float)SINGLE_3205_100;
                break;
            case VIA_MEM_SDR133:
                mClock = 66.5;
                memEfficiency = (float)SINGLE_3205_100;
                break;
            case VIA_MEM_DDR200:
                mClock = 100;
                memEfficiency = (float)SINGLE_3205_100;
                break;
            case VIA_MEM_DDR266:
                mClock = 133;
                memEfficiency = (float)SINGLE_3205_133;
                break;
            case VIA_MEM_DDR333:
                mClock = 166;
                memEfficiency = (float)SINGLE_3205_133;
                break;
            case VIA_MEM_DDR400:
                mClock = 200;
                memEfficiency = (float)SINGLE_3205_133;
                break;
            case VIA_MEM_DDR533:
                mClock = 266;
                memEfficiency = (float)SINGLE_3205_133;
                break;
            case VIA_MEM_DDR667:
                mClock = 333;
                memEfficiency = (float)SINGLE_3205_133;
                break;
            case VIA_MEM_DDR800:
                mClock = 400;
                memEfficiency = (float)SINGLE_3205_133;
                break;
            case VIA_MEM_DDR1066:
                mClock = 533;
                memEfficiency = (float)SINGLE_3205_133;
                break;
            default:
                ErrorF("Unknown DRAM Type!\n");
                mClock = 166;
                memEfficiency = (float)SINGLE_3205_133;
                break;
        }

        width = mode->HDisplay;
        height = mode->VDisplay;
        refresh = mode->VRefresh;

        if (refresh==0) {
            refresh=60;
            ErrorF("Unable to fetch vertical refresh value, needed for bandwidth calculation.\n");
        }

        /*
         * Approximative, VERY conservative formula in some cases.
         * This formula and the one below are derived analyzing the
         * tables present in VIA's own drivers. They may reject the over-
         * lay in some cases where VIA's driver don't.
         */
        dClock = (width * height * refresh) / 680000;
        if (dClock) {
            needBandWidth =
                (float)(((pScrn->bitsPerPixel >> 3) + VIDEO_BPP) * dClock);
            totalBandWidth = (float)(mClock * 16. * memEfficiency);

            DBG_DD(ErrorF(" via_xv.c : cBitsPerPel= %d : \n",
                pScrn->bitsPerPixel));
            DBG_DD(ErrorF(" via_xv.c : Video_Bpp= %d : \n", VIDEO_BPP));
            DBG_DD(ErrorF(" via_xv.c : refresh = %d : \n", refresh));
            DBG_DD(ErrorF(" via_xv.c : dClock= %d : \n", dClock));
            DBG_DD(ErrorF(" via_xv.c : mClk= %f : \n", mClock));
            DBG_DD(ErrorF(" via_xv.c : memEfficiency= %f : \n",
                memEfficiency));
            if (needBandWidth < totalBandWidth)
                return TRUE;
            ErrorF(" via_xv.c : needBandwidth= %f : \n",
                needBandWidth);
            ErrorF(" via_xv.c : totalBandwidth= %f : \n",
                totalBandWidth);
        }
        return FALSE;
    }
    return FALSE;
}

static const char *viaXvErrMsg[xve_numerr] = { "No Error.",
    "Bandwidth is insufficient. Check bios memory settings.",
    "PCI DMA blit failed. You probably encountered a bug.",
    "Not enough resources to complete the request. Probably out of memory.",
    "General Error. I wish I could be more specific.",
    "Wrong adaptor used. Try another port number."
};

static void
viaXvError(ScrnInfoPtr pScrn, viaPortPrivPtr pPriv, XvError error)
{
    if (error == xve_none) {
        pPriv->xvErr = xve_none;
        return;
    }
    if (error == pPriv->xvErr) {
        return;
    }
    pPriv->xvErr = error;
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "[Xv] Port %d: %s\n",
            pPriv->xv_portnum, viaXvErrMsg[error]);
}

static void
viaResetVideo(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vmmtr viaVidEng = (vmmtr) (pVia->MapBase + 0x200);

    DBG_DD(ErrorF(" via_xv.c : viaResetVideo: \n"));

    viaVidEng->video1_ctl = 0;
    viaVidEng->video3_ctl = 0;
    viaVidEng->compose = V1_COMMAND_FIRE;
    viaVidEng->compose = V3_COMMAND_FIRE;
    viaVidEng->color_key = 0x821;
    viaVidEng->snd_color_key = 0x821;
}

void
viaSaveVideo(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vmmtr viaVidEng = (vmmtr) (pVia->MapBase + 0x200);

    DBG_DD(ErrorF(" via_xv.c : viaSaveVideo : \n"));
    /* Save video registers */
    memcpy(pVia->VideoRegs, (void*)viaVidEng, sizeof(video_via_regs));

    pVia->dwV1 = ((vmmtr) viaVidEng)->video1_ctl;
    pVia->dwV3 = ((vmmtr) viaVidEng)->video3_ctl;
    viaVidEng->video1_ctl = 0;
    viaVidEng->video3_ctl = 0;
    viaVidEng->compose = V1_COMMAND_FIRE;
    viaVidEng->compose = V3_COMMAND_FIRE;
}

void
viaRestoreVideo(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vmmtr viaVidEng = (vmmtr) (pVia->MapBase + 0x200);
    video_via_regs  *localVidEng = pVia->VideoRegs;

    DBG_DD(ErrorF(" via_xv.c : viaRestoreVideo : \n"));

    /* Restore video registers */
    /* flush restored video engines' setting to MapBase */
    viaVidEng->alphawin_hvstart = localVidEng->alphawin_hvstart;
    viaVidEng->alphawin_size   = localVidEng->alphawin_size;
    viaVidEng->alphawin_ctl    = localVidEng->alphawin_ctl;
    viaVidEng->alphafb_stride  = localVidEng->alphafb_stride;
    viaVidEng->color_key       = localVidEng->color_key;
    viaVidEng->alphafb_addr    = localVidEng->alphafb_addr;
    viaVidEng->chroma_low      = localVidEng->chroma_low;
    viaVidEng->chroma_up       = localVidEng->chroma_up;
    viaVidEng->interruptflag   = localVidEng->interruptflag;

    if (pVia->ChipId != PCI_CHIP_VT3314)
    {
        /*VT3314 only has V3*/
        viaVidEng->video1_ctl      = localVidEng->video1_ctl;
        viaVidEng->video1_fetch    = localVidEng->video1_fetch;
        viaVidEng->video1y_addr1   = localVidEng->video1y_addr1;
        viaVidEng->video1_stride   = localVidEng->video1_stride;
        viaVidEng->video1_hvstart  = localVidEng->video1_hvstart;
        viaVidEng->video1_size     = localVidEng->video1_size;
        viaVidEng->video1y_addr2   = localVidEng->video1y_addr2;
        viaVidEng->video1_zoom     = localVidEng->video1_zoom;
        viaVidEng->video1_mictl    = localVidEng->video1_mictl;
        viaVidEng->video1y_addr0   = localVidEng->video1y_addr0;
        viaVidEng->video1_fifo     = localVidEng->video1_fifo;
        viaVidEng->video1y_addr3   = localVidEng->video1y_addr3;
        viaVidEng->v1_source_w_h   = localVidEng->v1_source_w_h;
        viaVidEng->video1_CSC1     = localVidEng->video1_CSC1;
        viaVidEng->video1_CSC2     = localVidEng->video1_CSC2;

        /* Fix cursor garbage after suspend for VX855 and VX900 (#405) */
        /* 0x2E4 T Signature Data Result 1 */
        viaVidEng->video1u_addr1         = localVidEng->video1u_addr1;
        /* 0x2E8 HI for Primary Display FIFO Control Signal */
        viaVidEng->video1u_addr2         = localVidEng->video1u_addr2;
        /* 0x2EC HI for Primary Display FIFO Transparent color */
        viaVidEng->video1u_addr3         = localVidEng->video1u_addr3;
        /* 0x2F0 HI for Primary Display Control Signal */
        viaVidEng->video1v_addr0         = localVidEng->video1v_addr0;
        /* 0x2F4 HI for Primary Display Frame Buffer Starting Address */
        viaVidEng->video1v_addr1         = localVidEng->video1v_addr1;
        /* 0x2F8 HI for Primary Display Horizontal and Vertical Start */
        viaVidEng->video1v_addr2         = localVidEng->video1v_addr2;
        /* 0x2FC HI for Primary Display Center Offset */
        viaVidEng->video1v_addr3         = localVidEng->video1v_addr3;
    }
    viaVidEng->snd_color_key   = localVidEng->snd_color_key;
    viaVidEng->v3alpha_prefifo = localVidEng->v3alpha_prefifo;
    viaVidEng->v3alpha_fifo    = localVidEng->v3alpha_fifo;
    viaVidEng->video3_CSC2     = localVidEng->video3_CSC2;
    viaVidEng->video3_CSC2     = localVidEng->video3_CSC2;
    viaVidEng->v3_source_width = localVidEng->v3_source_width;
    viaVidEng->video3_ctl      = localVidEng->video3_ctl;
    viaVidEng->video3_addr0    = localVidEng->video3_addr0;
    viaVidEng->video3_addr1    = localVidEng->video3_addr1;
    viaVidEng->video3_stride   = localVidEng->video3_stride;
    viaVidEng->video3_hvstart  = localVidEng->video3_hvstart;
    viaVidEng->video3_size     = localVidEng->video3_size;
    viaVidEng->v3alpha_fetch   = localVidEng->v3alpha_fetch;
    viaVidEng->video3_zoom     = localVidEng->video3_zoom;
    viaVidEng->video3_mictl    = localVidEng->video3_mictl;
    viaVidEng->video3_CSC1     = localVidEng->video3_CSC1;
    viaVidEng->video3_CSC2     = localVidEng->video3_CSC2;
    viaVidEng->compose         = localVidEng->compose;
    viaVidEng->video3_ctl      = pVia->dwV3;

    if (pVia->ChipId != PCI_CHIP_VT3314) {
        viaVidEng->video1_ctl = pVia->dwV1;
        viaVidEng->compose = V1_COMMAND_FIRE;
    }
    viaVidEng->compose = V3_COMMAND_FIRE;
}

void
viaExitVideo(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vmmtr viaVidEng = (vmmtr) (pVia->MapBase + 0x200);
    XF86VideoAdaptorPtr curAdapt;
    int i, j, numPorts;

    DBG_DD(ErrorF(" via_xv.c : viaExitVideo : \n"));

#ifdef OPENCHROMEDRI
    ViaCleanupXVMC(pScrn, viaAdaptPtr, XV_ADAPT_NUM);
#endif

    viaVidEng->video1_ctl = 0;
    viaVidEng->video3_ctl = 0;
    viaVidEng->compose = V1_COMMAND_FIRE;
    viaVidEng->compose = V3_COMMAND_FIRE;

    /*
     * Free all adaptor info allocated in viaInitVideo.
     */

    for (i = 0; i < XV_ADAPT_NUM; ++i) {
        curAdapt = viaAdaptPtr[i];
        if (curAdapt) {
            if (curAdapt->pPortPrivates) {
                if (curAdapt->pPortPrivates->ptr) {
                    numPorts = numAdaptPort[i];
                    for (j = 0; j < numPorts; ++j) {
                        viaStopVideo(pScrn,
                            (viaPortPrivPtr) curAdapt->pPortPrivates->ptr + j,
                            TRUE);
                    }
                    free(curAdapt->pPortPrivates->ptr);
                }
                free(curAdapt->pPortPrivates);
            }
            free(curAdapt);
        }
    }
    if (allAdaptors)
        free(allAdaptors);
}

void
viaInitVideo(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    XF86VideoAdaptorPtr *adaptors, *newAdaptors;
    VIAPtr pVia = VIAPTR(pScrn);
    int num_adaptors, num_new;

    DBG_DD(ErrorF(" via_xv.c : viaInitVideo, Screen[%d]\n", pScrn->scrnIndex));

    allAdaptors = NULL;
    newAdaptors = NULL;
    num_new = 0;

    pVia->useDmaBlit = FALSE;
#ifdef OPENCHROMEDRI
    pVia->useDmaBlit = (pVia->directRenderingType == DRI_1) &&
    ((pVia->Chipset == VIA_CLE266) ||
        (pVia->Chipset == VIA_KM400) ||
        (pVia->Chipset == VIA_K8M800) ||
        (pVia->Chipset == VIA_PM800) ||
        (pVia->Chipset == VIA_P4M800PRO) ||
        (pVia->Chipset == VIA_K8M890) ||
        (pVia->Chipset == VIA_P4M900) ||
        (pVia->Chipset == VIA_CX700) ||
        (pVia->Chipset == VIA_VX800) ||
        (pVia->Chipset == VIA_VX855) ||
        (pVia->Chipset == VIA_VX900) ||
        (pVia->Chipset == VIA_P4M890));
    if ((pVia->drmVerMajor < 2) ||
        ((pVia->drmVerMajor == 2) && (pVia->drmVerMinor < 9)))
        pVia->useDmaBlit = FALSE;
#endif
    pVia->useDmaBlit = pVia->useDmaBlit && pVia->dmaXV;

    if (pVia->useDmaBlit)
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
            "[Xv] Using PCI DMA for Xv image transfer.\n");

    if (!viaFastVidCpy)
        viaFastVidCpy = viaVidCopyInit("video", pScreen);

    if ((pVia->Chipset == VIA_CLE266) || (pVia->Chipset == VIA_KM400) ||
        (pVia->Chipset == VIA_K8M800) || (pVia->Chipset == VIA_PM800) ||
        (pVia->Chipset == VIA_P4M800PRO) || (pVia->Chipset == VIA_K8M890) ||
        (pVia->Chipset == VIA_P4M900) || (pVia->Chipset == VIA_CX700) ||
        (pVia->Chipset == VIA_P4M890) || (pVia->Chipset == VIA_VX800) ||
        (pVia->Chipset == VIA_VX855 || (pVia->Chipset == VIA_VX900))) {
        num_new = viaSetupAdaptors(pScreen, &newAdaptors);
        num_adaptors = xf86XVListGenericAdaptors(pScrn, &adaptors);
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                "[Xv] Unsupported Chipset. X video functionality disabled.\n");
        num_adaptors = 0;
        memset(viaAdaptPtr, 0, sizeof(viaAdaptPtr));
    }

    DBG_DD(ErrorF(" via_xv.c : num_adaptors : %d\n", num_adaptors));
    if (newAdaptors) {
        allAdaptors = malloc((num_adaptors + num_new) *
                sizeof(XF86VideoAdaptorPtr *));
        if (allAdaptors) {
            if (num_adaptors)
                memcpy(allAdaptors, adaptors,
                    num_adaptors * sizeof(XF86VideoAdaptorPtr));
            memcpy(allAdaptors + num_adaptors, newAdaptors,
                    num_new * sizeof(XF86VideoAdaptorPtr));
            num_adaptors += num_new;
        }
    }

    if (num_adaptors) {
        xf86XVScreenInit(pScreen, allAdaptors, num_adaptors);
#ifdef OPENCHROMEDRI
        ViaInitXVMC(pScreen);
#endif
        viaSetColorSpace(pVia, 0, 0, 0, 0, TRUE);
        pVia->swov.panning_x = 0;
        pVia->swov.panning_y = 0;
        pVia->swov.oldPanningX = 0;
        pVia->swov.oldPanningY = 0;
    }
}

static unsigned
viaSetupAdaptors(ScreenPtr pScreen, XF86VideoAdaptorPtr ** adaptors)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    int i, j, usedPorts, numPorts;
    viaPortPrivPtr pPriv;
    DevUnion *pdevUnion;

    DBG_DD(ErrorF(" via_xv.c : viaSetupAdaptors (viaSetupImageVideo): \n"));

    xvBrightness = MAKE_ATOM("XV_BRIGHTNESS");
    xvContrast = MAKE_ATOM("XV_CONTRAST");
    xvColorKey = MAKE_ATOM("XV_COLORKEY");
    xvHue = MAKE_ATOM("XV_HUE");
    xvSaturation = MAKE_ATOM("XV_SATURATION");
    xvAutoPaint = MAKE_ATOM("XV_AUTOPAINT_COLORKEY");

    *adaptors = NULL;
    usedPorts = 0;

    for (i = 0; i < XV_ADAPT_NUM; i++) {
        if (!(viaAdaptPtr[i] = xf86XVAllocateVideoAdaptorRec(pScrn)))
            return 0;
        numPorts = numAdaptPort[i];

        pPriv =
            (viaPortPrivPtr) xnfcalloc(numPorts, sizeof(viaPortPrivRec));
        pdevUnion = (DevUnion *) xnfcalloc(numPorts, sizeof(DevUnion));

        if (i == XV_ADAPT_SWOV) { /* Overlay engine */
            viaAdaptPtr[i]->type = XvInputMask | XvWindowMask | XvImageMask |
            XvVideoMask | XvStillMask;
            viaAdaptPtr[i]->flags =
            VIDEO_OVERLAID_IMAGES | VIDEO_CLIP_TO_VIEWPORT;
        } else {
            viaAdaptPtr[i]->type = XvInputMask | XvWindowMask | XvVideoMask;
            viaAdaptPtr[i]->flags =
            VIDEO_OVERLAID_IMAGES | VIDEO_CLIP_TO_VIEWPORT;
        }
        viaAdaptPtr[i]->name = XvAdaptorName[i];
        viaAdaptPtr[i]->nEncodings = 1;
        viaAdaptPtr[i]->pEncodings = DummyEncoding;
        viaAdaptPtr[i]->nFormats = sizeof(FormatsG) / sizeof(FormatsG[0]);
        viaAdaptPtr[i]->pFormats = FormatsG;

        /* The adapter can handle 1 port simultaneously */
        viaAdaptPtr[i]->nPorts = numPorts;
        viaAdaptPtr[i]->pPortPrivates = pdevUnion;
        viaAdaptPtr[i]->pPortPrivates->ptr = (pointer) pPriv;
        viaAdaptPtr[i]->nAttributes = NUM_ATTRIBUTES_G;
        viaAdaptPtr[i]->pAttributes = AttributesG;

        viaAdaptPtr[i]->nImages = NUM_IMAGES_G;
        viaAdaptPtr[i]->pImages = ImagesG;
        viaAdaptPtr[i]->PutVideo = NULL;
        viaAdaptPtr[i]->StopVideo = viaStopVideo;
        viaAdaptPtr[i]->QueryBestSize = viaQueryBestSize;
        viaAdaptPtr[i]->GetPortAttribute = viaGetPortAttribute;
        viaAdaptPtr[i]->SetPortAttribute = viaSetPortAttribute;
        viaAdaptPtr[i]->PutImage = viaPutImage;
        viaAdaptPtr[i]->ReputImage = NULL;
        viaAdaptPtr[i]->QueryImageAttributes = viaQueryImageAttributes;
        for (j = 0; j < numPorts; ++j) {
            pPriv[j].dmaBounceBuffer = NULL;
            pPriv[j].dmaBounceStride = 0;
            pPriv[j].dmaBounceLines = 0;
            pPriv[j].colorKey = 0x0821;
            pPriv[j].autoPaint = TRUE;
            pPriv[j].brightness = 5000.;
            pPriv[j].saturation = 10000;
            pPriv[j].contrast = 10000;
            pPriv[j].hue = 0;
            pPriv[j].FourCC = 0;
            pPriv[j].xv_portnum = j + usedPorts;
            pPriv[j].xvErr = xve_none;

#ifdef X_USE_REGION_NULL
            REGION_NULL(pScreen, &pPriv[j].clip);
#else
            REGION_INIT(pScreen, &pPriv[j].clip, NullBox, 1);
#endif
        }
        usedPorts += j;

#ifdef OPENCHROMEDRI
        viaXvMCInitXv(pScrn, viaAdaptPtr[i]);
#endif

    } /* End of for */
    viaResetVideo(pScrn);
    *adaptors = viaAdaptPtr;
    return XV_ADAPT_NUM;
}

static void
viaStopVideo(ScrnInfoPtr pScrn, pointer data, Bool exit)
{
    VIAPtr pVia = VIAPTR(pScrn);
    viaPortPrivPtr pPriv = (viaPortPrivPtr) data;

    DBG_DD(ErrorF(" via_xv.c : viaStopVideo: exit=%d\n", exit));

    REGION_EMPTY(pScrn->pScreen, &pPriv->clip);
    ViaOverlayHide(pScrn);
    if (exit) {
        ViaSwovSurfaceDestroy(pScrn, pPriv);
        if (pPriv->dmaBounceBuffer)
            free(pPriv->dmaBounceBuffer);
        pPriv->dmaBounceBuffer = 0;
        pPriv->dmaBounceStride = 0;
        pPriv->dmaBounceLines = 0;
        pVia->dwFrameNum = 0;
        pPriv->old_drw_x = 0;
        pPriv->old_drw_y = 0;
        pPriv->old_drw_w = 0;
        pPriv->old_drw_h = 0;
    }
}

static int
viaSetPortAttribute(ScrnInfoPtr pScrn,
        Atom attribute, INT32 value, pointer data)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vmmtr viaVidEng = (vmmtr) (pVia->MapBase + 0x200);
    viaPortPrivPtr pPriv = (viaPortPrivPtr) data;
    int attr, avalue;

    DBG_DD(ErrorF(" via_xv.c : viaSetPortAttribute : \n"));

    /* Color Key */
    if (attribute == xvColorKey) {
        DBG_DD(ErrorF("  V4L Disable  xvColorKey = %08lx\n", value));

        pPriv->colorKey = value;
        /* All assume color depth is 16 */
        value &= 0x00FFFFFF;
        viaVidEng->color_key = value;
        viaVidEng->snd_color_key = value;
        REGION_EMPTY(pScrn->pScreen, &pPriv->clip);
        DBG_DD(ErrorF("  V4L Disable done  xvColorKey = %08lx\n", value));

    } else if (attribute == xvAutoPaint) {
        pPriv->autoPaint = value;
        DBG_DD(ErrorF("       xvAutoPaint = %08lx\n", value));
        /* Color Control */
    } else if (attribute == xvBrightness ||
            attribute == xvContrast ||
            attribute == xvSaturation || attribute == xvHue) {
        if (attribute == xvBrightness) {
            DBG_DD(ErrorF("     xvBrightness = %08ld\n", value));
            pPriv->brightness = value;
        }
        if (attribute == xvContrast) {
            DBG_DD(ErrorF("     xvContrast = %08ld\n", value));
            pPriv->contrast = value;
        }
        if (attribute == xvSaturation) {
            DBG_DD(ErrorF("     xvSaturation = %08ld\n", value));
            pPriv->saturation = value;
        }
        if (attribute == xvHue) {
            DBG_DD(ErrorF("     xvHue = %08ld\n", value));
            pPriv->hue = value;
        }
        viaSetColorSpace(pVia, pPriv->hue, pPriv->saturation,
                pPriv->brightness, pPriv->contrast, FALSE);
    } else {
        DBG_DD(ErrorF
                (" via_xv.c : viaSetPortAttribute : is not supported the attribute"));
        return BadMatch;
    }

    /* attr,avalue hardware processing goes here */
    (void)attr;
    (void)avalue;

    return Success;
}

static int
viaGetPortAttribute(ScrnInfoPtr pScrn,
        Atom attribute, INT32 * value, pointer data)
{
    viaPortPrivPtr pPriv = (viaPortPrivPtr) data;

    DBG_DD(ErrorF(" via_xv.c : viaGetPortAttribute : port %d %ld\n",
                    pPriv->xv_portnum, attribute));

    *value = 0;
    if (attribute == xvColorKey) {
        *value = (INT32) pPriv->colorKey;
        DBG_DD(ErrorF(" via_xv.c :    ColorKey 0x%lx\n", pPriv->colorKey));
    } else if (attribute == xvAutoPaint) {
        *value = (INT32) pPriv->autoPaint;
        DBG_DD(ErrorF("    AutoPaint = %08ld\n", *value));
        /* Color Control */
    } else if (attribute == xvBrightness ||
            attribute == xvContrast ||
            attribute == xvSaturation || attribute == xvHue) {
        if (attribute == xvBrightness) {
            *value = pPriv->brightness;
            DBG_DD(ErrorF("    xvBrightness = %08ld\n", *value));
        }
        if (attribute == xvContrast) {
            *value = pPriv->contrast;
            DBG_DD(ErrorF("    xvContrast = %08ld\n", *value));
        }
        if (attribute == xvSaturation) {
            *value = pPriv->saturation;
            DBG_DD(ErrorF("    xvSaturation = %08ld\n", *value));
        }
        if (attribute == xvHue) {
            *value = pPriv->hue;
            DBG_DD(ErrorF("    xvHue = %08ld\n", *value));
        }

    } else {
        DBG_DD(ErrorF(" via_xv.c : viaGetPortAttribute : is not supported the attribute\n"));
        /*return BadMatch */;
    }
    return Success;
}

static void
viaQueryBestSize(ScrnInfoPtr pScrn,
    Bool motion,
    short vid_w, short vid_h,
    short drw_w, short drw_h,
    unsigned int *p_w, unsigned int *p_h, pointer data)
{
    DBG_DD(ErrorF(" via_xv.c : viaQueryBestSize :\n"));
    *p_w = drw_w;
    *p_h = drw_h;

    if (*p_w > 2048)
        *p_w = 2048;
}

/*
 *  To do SW Flip
 */
static void
Flip(VIAPtr pVia, viaPortPrivPtr pPriv, int fourcc,
        unsigned long DisplayBufferIndex)
{
    unsigned long proReg = 0;
    unsigned count = 50000;

    if (pVia->ChipId == PCI_CHIP_VT3259
        && !(pVia->swov.gdwVideoFlagSW & VIDEO_1_INUSE))
        proReg = PRO_HQV1_OFFSET;

    switch (fourcc) {
        case FOURCC_UYVY:
        case FOURCC_YUY2:
        case FOURCC_RV15:
        case FOURCC_RV16:
        case FOURCC_RV32:
            while ((VIAGETREG(HQV_CONTROL + proReg) & HQV_SW_FLIP)
                    && --count);
            VIASETREG(HQV_SRC_STARTADDR_Y + proReg,
                pVia->swov.SWDevice.dwSWPhysicalAddr[DisplayBufferIndex]);
            VIASETREG(HQV_CONTROL + proReg, (VIAGETREG(HQV_CONTROL + proReg) & ~HQV_FLIP_ODD) | HQV_SW_FLIP | HQV_FLIP_STATUS);
            break;
        case FOURCC_YV12:
        case FOURCC_I420:
        default:
            while ((VIAGETREG(HQV_CONTROL + proReg) & HQV_SW_FLIP)
                    && --count);
            VIASETREG(HQV_SRC_STARTADDR_Y + proReg,
                pVia->swov.SWDevice.dwSWPhysicalAddr[DisplayBufferIndex]);
            if (pVia->VideoEngine == VIDEO_ENGINE_CME) {
                VIASETREG(HQV_SRC_STARTADDR_U + proReg,
                pVia->swov.SWDevice.dwSWCrPhysicalAddr[DisplayBufferIndex]);
            } else {
                VIASETREG(HQV_SRC_STARTADDR_U,
                    pVia->swov.SWDevice.dwSWCbPhysicalAddr[DisplayBufferIndex]);
                VIASETREG(HQV_SRC_STARTADDR_V,
                    pVia->swov.SWDevice.dwSWCrPhysicalAddr[DisplayBufferIndex]);
            }
            VIASETREG(HQV_CONTROL + proReg, (VIAGETREG(HQV_CONTROL + proReg) & ~HQV_FLIP_ODD) | HQV_SW_FLIP | HQV_FLIP_STATUS);
	    break;
    }
}

/*
 * Slow and dirty. NV12 blit.
 */
static void
nv12cp(unsigned char *dst, const unsigned char *src, int dstPitch,
        int w, int h, int i420)
{
    unsigned long srcUOffset, srcVOffset;

    /*
     * Blit luma component as a fake YUY2 assembler blit.
     */
    if (i420) {
        srcVOffset  = w * h + (w >> 1) * (h >> 1);
        srcUOffset = w * h;
    } else {
        srcUOffset  = w * h + (w >> 1) * (h >> 1);
        srcVOffset = w * h;
    }

    (*viaFastVidCpy) (dst, src, dstPitch, w >> 1, h, TRUE);
    nv12Blit(dst + dstPitch * h, src + srcUOffset,
            src + srcVOffset, w >> 1, w >>1, dstPitch, h >> 1);
}

#ifdef OPENCHROMEDRI

static int
viaDmaBlitImage(VIAPtr pVia,
    viaPortPrivPtr pPort,
    unsigned char *src,
    CARD32 dst, unsigned width, unsigned height, unsigned lumaStride, int id)
{
    Bool bounceBuffer;
    drm_via_dmablit_t blit;
    drm_via_blitsync_t *chromaSync = &blit.sync;
    unsigned char *base;
    unsigned char *bounceBase;
    unsigned bounceStride;
    unsigned bounceLines;
    unsigned size;
    int err = 0;
    Bool nv12Conversion;

    bounceBuffer = ((unsigned long)src & 15);
    nv12Conversion = (pVia->VideoEngine == VIDEO_ENGINE_CME &&
                     (id == FOURCC_YV12 || id == FOURCC_I420));

    switch (id) {
        case FOURCC_YUY2:
        case FOURCC_RV15:
        case FOURCC_RV16:
            bounceStride = ALIGN_TO(2 * width, 16);
            bounceLines = height;
            break;
        case FOURCC_RV32:
            bounceStride = ALIGN_TO(4 * width, 16);
            bounceLines = height;
            break;

        case FOURCC_YV12:
        case FOURCC_I420:
        default:
            bounceStride = ALIGN_TO(width, 16);
            bounceLines = height;
            break;
    }

    if (bounceBuffer || nv12Conversion) {
        if (!pPort->dmaBounceBuffer ||
            pPort->dmaBounceStride != bounceStride ||
            pPort->dmaBounceLines != bounceLines) {
            if (pPort->dmaBounceBuffer) {
                free(pPort->dmaBounceBuffer);
                pPort->dmaBounceBuffer = 0;
            }
            size = bounceStride * bounceLines + 16;
            if (id == FOURCC_YV12 || id == FOURCC_I420)
                size += ALIGN_TO(bounceStride >> 1, 16) * bounceLines;
            pPort->dmaBounceBuffer = (unsigned char *)malloc(size);
            pPort->dmaBounceLines = bounceLines;
            pPort->dmaBounceStride = bounceStride;
        }
    }

    bounceBase =
    (unsigned char *)ALIGN_TO((unsigned long)(pPort->dmaBounceBuffer),
        16);
    base = (bounceBuffer) ? bounceBase : src;

    if (bounceBuffer) {
        (*viaFastVidCpy) (base, src, bounceStride, bounceStride >> 1, height,
        1);
    }

    blit.num_lines = height;
    blit.line_length = bounceStride;
    blit.fb_addr = dst;
    blit.fb_stride = lumaStride;
    blit.mem_addr = base;
    blit.mem_stride = bounceStride;
    blit.to_fb = 1;
#ifdef XV_DEBUG
    ErrorF
    ("Addr: 0x%lx, Offset 0x%lx\n Fb_stride: %u, Mem_stride: %u\n width: %u num_lines: %u\n",
    (unsigned long)blit.mem_addr, (unsigned long)blit.fb_addr,
    (unsigned)blit.fb_stride, (unsigned)blit.mem_stride,
    (unsigned)blit.line_length, (unsigned)blit.num_lines);
#endif
    while (-EAGAIN == (err =
        drmCommandWriteRead(pVia->drmmode.fd, DRM_VIA_DMA_BLIT, &blit,
        sizeof(blit)))) ;
    if (err < 0)
        return -1;

    if (id == FOURCC_YV12 || id == FOURCC_I420) {
        unsigned tmp = ALIGN_TO(width >> 1, 16);

        if (nv12Conversion) {
            nv12Blit(bounceBase + bounceStride * height,
                src + bounceStride * height + tmp * (height >> 1),
                src + bounceStride * height, width >> 1, tmp,
                bounceStride, height >> 1);
        } else if (bounceBuffer) {
            (*viaFastVidCpy) (base + bounceStride * height,
                    src + bounceStride * height, tmp, tmp >> 1, height, 1);
        }

        if (nv12Conversion) {
            blit.num_lines = height >> 1;
            blit.line_length = bounceStride;
            blit.mem_addr = bounceBase + bounceStride * height;
            blit.fb_stride = lumaStride;
            blit.mem_stride = bounceStride;
        } else {
            blit.num_lines = height;
            blit.line_length = tmp;
            blit.mem_addr = base + bounceStride * height;
            blit.fb_stride = lumaStride >> 1;
            blit.mem_stride = tmp;
        }

        blit.fb_addr = dst + lumaStride * height;
        blit.to_fb = 1;

        while (-EAGAIN == (err =
            drmCommandWriteRead(pVia->drmmode.fd, DRM_VIA_DMA_BLIT, &blit,
                sizeof(blit))));
        if (err < 0)
            return -1;
    }

    while (-EAGAIN == (err = drmCommandWrite(pVia->drmmode.fd, DRM_VIA_BLIT_SYNC,
        chromaSync, sizeof(*chromaSync)))) ;
    if (err < 0)
        return -1;

    return Success;
}

#endif


/*
 * The source rectangle of the video is defined by (src_x, src_y, src_w, src_h).
 * The dest rectangle of the video is defined by (drw_x, drw_y, drw_w, drw_h).
 * id is a fourcc code for the format of the video.
 * buf is the pointer to the source data in system memory.
 * width and height are the w/h of the source data.
 * If "sync" is TRUE, then we must be finished with *buf at the point of return
 * (which we always are).
 * clipBoxes is the clipping region in screen space.
 * data is a pointer to our port private.
 * pDraw is a Drawable, which might not be the screen in the case of
 * compositing.  It's a new argument to the function in the 1.1 server.
 */

static int
viaPutImage(ScrnInfoPtr pScrn,
        short src_x, short src_y,
        short drw_x, short drw_y,
        short src_w, short src_h,
        short drw_w, short drw_h,
        int id, unsigned char *buf,
        short width, short height, Bool sync, RegionPtr clipBoxes,
        pointer data, DrawablePtr pDraw)
{
    VIAPtr pVia = VIAPTR(pScrn);
    viaPortPrivPtr pPriv = (viaPortPrivPtr) data;
    xf86CrtcPtr crtc = NULL;
    unsigned long retCode;

# ifdef XV_DEBUG
    ErrorF(" via_xv.c : viaPutImage : called,  Screen[%d]\n", pScrn->scrnIndex);
    ErrorF(" via_xv.c : FourCC=0x%x width=%d height=%d sync=%d\n", id,
            width, height, sync);
    ErrorF
    (" via_xv.c : src_x=%d src_y=%d src_w=%d src_h=%d colorkey=0x%lx\n",
            src_x, src_y, src_w, src_h, pPriv->colorKey);
    ErrorF(" via_xv.c : drw_x=%d drw_y=%d drw_w=%d drw_h=%d\n", drw_x,
            drw_y, drw_w, drw_h);
# endif

    /* Find out which CRTC the surface will belong to */
    crtc = window_belongs_to_crtc(pScrn, drw_x, drw_y, drw_w, drw_h);
    if (!crtc) {
        DBG_DD(ErrorF(" via_xv.c : No usable CRTC\n"));
        viaXvError(pScrn, pPriv, xve_adaptor);
        return BadAlloc;
    }

    switch (pPriv->xv_adaptor) {
        case XV_ADAPT_SWOV:
        {
            DDUPDATEOVERLAY UpdateOverlay_Video;
            LPDDUPDATEOVERLAY lpUpdateOverlay = &UpdateOverlay_Video;

            int dstPitch;
            unsigned long dwUseExtendedFIFO = 0;

            DBG_DD(ErrorF(" via_xv.c :              : S/W Overlay! \n"));
            /*  Allocate video memory(CreateSurface),
             *  add codes to judge if need to re-create surface
             */
            if ((pPriv->old_src_w != src_w) || (pPriv->old_src_h != src_h)) {
                ViaSwovSurfaceDestroy(pScrn, pPriv);
            }

            if (Success != (retCode =
                ViaSwovSurfaceCreate(pScrn, pPriv, id, width, height))) {
                DBG_DD(ErrorF
                        ("             : Fail to Create SW Video Surface\n"));
                viaXvError(pScrn, pPriv, xve_mem);
                return retCode;
            }

            /*  Copy image data from system memory to video memory
             *  TODO: use DRM's DMA feature to accelerate data copy
             */
            if (id != FOURCC_XVMC) {
                dstPitch = pVia->swov.SWDevice.dwPitch;

                if (pVia->useDmaBlit) {
#ifdef OPENCHROMEDRI
                    if (viaDmaBlitImage(pVia, pPriv, buf,
                        (CARD32) pVia->swov.SWDevice.dwSWPhysicalAddr[pVia->dwFrameNum & 1],
                        width, height, dstPitch, id)) {
                            viaXvError(pScrn, pPriv, xve_dmablit);
                        return BadAccess;
                    }
#endif
                } else {
                    switch (id) {
                        case FOURCC_I420:
                            if (pVia->VideoEngine == VIDEO_ENGINE_CME) {
                                nv12cp(pVia->swov.SWDevice.
                                    lpSWOverlaySurface[pVia->dwFrameNum & 1],
                                    buf, dstPitch, width, height, 1);
                            } else {
                                (*viaFastVidCpy)(pVia->swov.SWDevice.
                                    lpSWOverlaySurface[pVia->dwFrameNum & 1],
                                    buf, dstPitch, width, height, 0);
                            }
                            break;
                        case FOURCC_YV12:
                            if (pVia->VideoEngine == VIDEO_ENGINE_CME) {
                                nv12cp(pVia->swov.SWDevice.
                                    lpSWOverlaySurface[pVia->dwFrameNum & 1],
                                    buf, dstPitch, width, height, 0);
                            } else {
                                (*viaFastVidCpy)(pVia->swov.SWDevice.
                                    lpSWOverlaySurface[pVia->dwFrameNum & 1],
                                    buf, dstPitch, width, height, 0);
                            }
                            break;
                        case FOURCC_RV32:
                            (*viaFastVidCpy) (pVia->swov.SWDevice.
                                lpSWOverlaySurface[pVia->dwFrameNum & 1],
                                buf, dstPitch, width << 1, height, 1);
                            break;
                        case FOURCC_UYVY:
                        case FOURCC_YUY2:
                        case FOURCC_RV15:
                        case FOURCC_RV16:
                        default:
                            (*viaFastVidCpy) (pVia->swov.SWDevice.
                                lpSWOverlaySurface[pVia->dwFrameNum & 1],
                                buf, dstPitch, width, height, 1);
                            break;
                    }
                }
            }

            /* If there is bandwidth issue, block the H/W overlay */
            if (!(DecideOverlaySupport(crtc))) {
                DBG_DD(ErrorF
                        (" via_xv.c : Xv Overlay rejected due to insufficient "
                                "memory bandwidth.\n"));
                viaXvError(pScrn, pPriv, xve_bandwidth);
                return BadAlloc;
            }

            /*
             *  fill video overlay parameter
             */
            lpUpdateOverlay->SrcLeft = src_x;
            lpUpdateOverlay->SrcTop = src_y;
            lpUpdateOverlay->SrcRight = src_x + src_w;
            lpUpdateOverlay->SrcBottom = src_y + src_h;

            lpUpdateOverlay->DstLeft = drw_x;
            lpUpdateOverlay->DstTop = drw_y;
            lpUpdateOverlay->DstRight = drw_x + drw_w;
            lpUpdateOverlay->DstBottom = drw_y + drw_h;

            lpUpdateOverlay->dwFlags = DDOVER_KEYDEST;

            if (pScrn->bitsPerPixel == 8) {
                lpUpdateOverlay->dwColorSpaceLowValue = pPriv->colorKey & 0xff;
            } else {
                lpUpdateOverlay->dwColorSpaceLowValue = pPriv->colorKey;
            }
            /* If use extend FIFO mode */
            if (pScrn->currentMode->HDisplay > 1024) {
                dwUseExtendedFIFO = 1;
            }

            if (FOURCC_XVMC != id) {

                /*
                 * XvMC flipping is done in the client lib.
                 */

                DBG_DD(ErrorF("             : Flip\n"));
                Flip(pVia, pPriv, id, pVia->dwFrameNum & 1);
            }

            pVia->dwFrameNum++;

            /* If the dest rec. & extendFIFO doesn't change, don't do UpdateOverlay
             * unless the surface clipping has changed */
            if ((pPriv->old_drw_x == drw_x) && (pPriv->old_drw_y == drw_y)
                    && (pPriv->old_drw_w == drw_w) && (pPriv->old_drw_h == drw_h)
                    && (pPriv->old_src_x == src_x) && (pPriv->old_src_y == src_y)
                    && (pPriv->old_src_w == src_w) && (pPriv->old_src_h == src_h)
                    && (pVia->old_dwUseExtendedFIFO == dwUseExtendedFIFO)
                    && (pVia->VideoStatus & VIDEO_SWOV_ON) &&
                    REGION_EQUAL(pScrn->pScreen, &pPriv->clip, clipBoxes)) {
                DBG_DD(ErrorF(" via_xv.c : don't do UpdateOverlay! \n"));
                viaXvError(pScrn, pPriv, xve_none);
                return Success;
            }

            pPriv->old_src_x = src_x;
            pPriv->old_src_y = src_y;
            pPriv->old_src_w = src_w;
            pPriv->old_src_h = src_h;

            pPriv->old_drw_x = drw_x;
            pPriv->old_drw_y = drw_y;
            pPriv->old_drw_w = drw_w;
            pPriv->old_drw_h = drw_h;
            pVia->old_dwUseExtendedFIFO = dwUseExtendedFIFO;
            pVia->VideoStatus |= VIDEO_SWOV_ON;

            /*  BitBlt: Draw the colorkey rectangle */
            if (!REGION_EQUAL(pScrn->pScreen, &pPriv->clip, clipBoxes)) {
                REGION_COPY(pScrn->pScreen, &pPriv->clip, clipBoxes);
                if (pPriv->autoPaint) {
                    if (pDraw->type == DRAWABLE_WINDOW) {
                        xf86XVFillKeyHelperDrawable(pDraw, pPriv->colorKey, clipBoxes);
                        DamageDamageRegion(pDraw, clipBoxes);
                    } else {
                        xf86XVFillKeyHelper(pScrn->pScreen, pPriv->colorKey, clipBoxes);
                    }
                }
            } else {
                DBG_DD(ErrorF(" via_xv.c : // No need to draw Colorkey!! \n"));
            }
            /*
             *  Update video overlay
             */
            if (!VIAVidUpdateOverlay(crtc, lpUpdateOverlay)) {
                DBG_DD(ErrorF
                        (" via_xv.c : call v4l updateoverlay fail. \n"));
            } else {
                DBG_DD(ErrorF(" via_xv.c : PutImage done OK\n"));
                viaXvError(pScrn, pPriv, xve_none);
                return Success;
            }
            break;
        }
        default:
            DBG_DD(ErrorF(" via_xv.c : XVPort not supported\n"));
            viaXvError(pScrn, pPriv, xve_adaptor);
            break;
    }
    DBG_DD(ErrorF(" via_xv.c : PutImage done OK\n"));
    viaXvError(pScrn, pPriv, xve_none);
    return Success;
}

static int
viaQueryImageAttributes(ScrnInfoPtr pScrn,
        int id, unsigned short *w, unsigned short *h, int *pitches,
        int *offsets)
{
    int size, tmp;
    VIAPtr pVia = VIAPTR(pScrn);

    DBG_DD(ErrorF(" via_xv.c : viaQueryImageAttributes : FourCC=0x%x, ",
            id));
    DBG_DD(ErrorF(" via_xv.c : Screen[%d],  w=%d, h=%d\n", pScrn->scrnIndex, *w, *h));

    if ((!w) || (!h))
        return 0;

    if (*w > VIA_MAX_XVIMAGE_X)
        *w = VIA_MAX_XVIMAGE_X;
    if (*h > VIA_MAX_XVIMAGE_Y)
        *h = VIA_MAX_XVIMAGE_Y;

    *w = (*w + 1) & ~1;
    if (offsets)
        offsets[0] = 0;

    switch (id) {
        case FOURCC_I420:
        case FOURCC_YV12: /*Planar format : YV12 -4:2:0 */
            *h = (*h + 1) & ~1;
            size = *w;
            if (pVia->useDmaBlit)
                size = (size + 15) & ~15;
            if (pitches)
                pitches[0] = size;
            size *= *h;
            if (offsets)
                offsets[1] = size;
            tmp = (*w >> 1);
            if (pVia->useDmaBlit)
                tmp = (tmp + 15) & ~15;
            if (pitches)
                pitches[1] = pitches[2] = tmp;
            tmp *= (*h >> 1);
            size += tmp;
            if (offsets)
                offsets[2] = size;
            size += tmp;
            break;
        case FOURCC_XVMC:
            *h = (*h + 1) & ~1;
#ifdef OPENCHROMEDRI
            size = viaXvMCPutImageSize(pScrn);
#else
            size = 0;
#endif
            if (pitches)
                pitches[0] = size;
            break;
        case FOURCC_AI44:
        case FOURCC_IA44:
            size = *w * *h;
            if (pitches)
                pitches[0] = *w;
            if (offsets)
                offsets[0] = 0;
            break;
        case FOURCC_RV32:
            size = *w << 2;
            if (pVia->useDmaBlit)
                size = (size + 15) & ~15;
            if (pitches)
                pitches[0] = size;
            size *= *h;
            break;
        case FOURCC_UYVY: /*Packed format : UYVY -4:2:2 */
        case FOURCC_YUY2: /*Packed format : YUY2 -4:2:2 */
        case FOURCC_RV15:
        case FOURCC_RV16:
        default:
            size = *w << 1;
            if (pVia->useDmaBlit)
                size = (size + 15) & ~15;
            if (pitches)
                pitches[0] = size;
            size *= *h;
        break;
    }

    if (pitches)
        DBG_DD(ErrorF(" pitches[0]=%d, pitches[1]=%d, pitches[2]=%d, ",
                    pitches[0], pitches[1], pitches[2]));
    if (offsets)
        DBG_DD(ErrorF(" offsets[0]=%d, offsets[1]=%d, offsets[2]=%d, ",
                    offsets[0], offsets[1], offsets[2]));

    DBG_DD(ErrorF(" width=%d, height=%d \n", *w, *h));
    return size;
}

/*
 *
 */
void
VIAVidAdjustFrame(ScrnInfoPtr pScrn, int x, int y)
{
    VIAPtr pVia = VIAPTR(pScrn);

    pVia->swov.panning_x = x;
    pVia->swov.panning_y = y;
}

/*
 * Blit the chroma field from one buffer to another while at the same time converting from
 * YV12 to NV12.
 */

static void
nv12Blit(unsigned char *nv12Chroma,
        const unsigned char *uBuffer,
        const unsigned char *vBuffer,
        unsigned width, unsigned srcPitch, unsigned dstPitch, unsigned lines)
{
    int x;
    int dstAdd;
    int srcAdd;

    dstAdd = dstPitch - (width << 1);
    srcAdd = srcPitch - width;

    while (lines--) {
        x = width;
        while (x > 3) {
            register CARD32
            dst32,
            src32 = *((CARD32 *) vBuffer),
            src32_2 = *((CARD32 *) uBuffer);
            dst32 =
                (src32_2 & 0xff) | ((src32 & 0xff) << 8) |
                ((src32_2 & 0x0000ff00) << 8) | ((src32 & 0x0000ff00) << 16);
            *((CARD32 *) nv12Chroma) = dst32;
            nv12Chroma += 4;
            dst32 =
                ((src32_2 & 0x00ff0000) >> 16) | ((src32 & 0x00ff0000) >> 8) |
                ((src32_2 & 0xff000000) >> 8) | (src32 & 0xff000000);
            *((CARD32 *) nv12Chroma) = dst32;
            nv12Chroma += 4;
            x -= 4;
            vBuffer += 4;
            uBuffer += 4;
        }
        while (x--) {
            *nv12Chroma++ = *uBuffer++;
            *nv12Chroma++ = *vBuffer++;
        }
        nv12Chroma += dstAdd;
        vBuffer += srcAdd;
        uBuffer += srcAdd;
    }
}

#endif /* !XvExtension */
