#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via.h"
#include "via_driver.h"
#include "via_vgahw.h"
#include "via_id.h"

/*
 * Enables the second display channel.
 */
void
ViaSecondDisplayChannelEnable(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "ViaSecondDisplayChannelEnable\n"));
    ViaCrtcMask(hwp, 0x6A, 0x00, 1 << 6);
    ViaCrtcMask(hwp, 0x6A, 1 << 7, 1 << 7);
    ViaCrtcMask(hwp, 0x6A, 1 << 6, 1 << 6);
}

/*
 * Disables the second display channel.
 */
void
ViaSecondDisplayChannelDisable(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "ViaSecondDisplayChannelDisable\n"));

    ViaCrtcMask(hwp, 0x6A, 0x00, 1 << 6);
    ViaCrtcMask(hwp, 0x6A, 0x00, 1 << 7);
    ViaCrtcMask(hwp, 0x6A, 1 << 6, 1 << 6);
}

/*
 * Initial settings for displays.
 */
void
ViaDisplayInit(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaDisplayPreInit\n"));

    ViaSecondDisplayChannelDisable(pScrn);
    ViaCrtcMask(hwp, 0x6A, 0x00, 0x3D);

    hwp->writeCrtc(hwp, 0x6B, 0x00);
    hwp->writeCrtc(hwp, 0x6C, 0x00);
    hwp->writeCrtc(hwp, 0x79, 0x00);

    /* (IGA1 Timing Plus 2, added in VT3259 A3 or later) */
    if (pVia->Chipset != VIA_CLE266 && pVia->Chipset != VIA_KM400)
        ViaCrtcMask(hwp, 0x47, 0x00, 0xC8);
}

/*
 * Enables simultaneous mode.
 */
void
ViaDisplayEnableSimultaneous(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "ViaDisplayEnableSimultaneous\n"));
    ViaCrtcMask(hwp, 0x6B, 0x08, 0x08);
}

/*
 * Disables simultaneous mode.
 */
void
ViaDisplayDisableSimultaneous(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "ViaDisplayDisableSimultaneous\n"));
    ViaCrtcMask(hwp, 0x6B, 0x00, 0x08);
}

/*
 * Enables CRT using DPMS registers.
 */
void
ViaDisplayEnableCRT(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaDisplayEnableCRT\n"));
    ViaCrtcMask(hwp, 0x36, 0x00, 0x30);
}

/*
 * Disables CRT using DPMS registers.
 */
void
ViaDisplayDisableCRT(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaDisplayDisableCRT\n"));
    ViaCrtcMask(hwp, 0x36, 0x30, 0x30);
}

void
ViaDisplayEnableDVO(ScrnInfoPtr pScrn, int port)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaDisplayEnableDVO, port: %d\n", port));
    switch (port) {
        case VIA_DI_PORT_DVP0:
            ViaSeqMask(hwp, 0x1E, 0xC0, 0xC0);
            break;
        case VIA_DI_PORT_DVP1:
            ViaSeqMask(hwp, 0x1E, 0x30, 0x30);
            break;
    }
}

void
ViaDisplayDisableDVO(ScrnInfoPtr pScrn, int port)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaDisplayDisableDVO, port: %d\n", port));
    switch (port) {
        case VIA_DI_PORT_DVP0:
            ViaSeqMask(hwp, 0x1E, 0x00, 0xC0);
            break;
        case VIA_DI_PORT_DVP1:
            ViaSeqMask(hwp, 0x1E, 0x00, 0x30);
            break;
    }
}

/*
 * Sets the primary or secondary display stream on CRT.
 */
void
ViaDisplaySetStreamOnCRT(ScrnInfoPtr pScrn, Bool primary)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaDisplaySetStreamOnCRT\n"));

    if (primary)
        ViaSeqMask(hwp, 0x16, 0x00, 0x40);
    else
        ViaSeqMask(hwp, 0x16, 0x40, 0x40);
}

/*
 * Sets the primary or secondary display stream on internal TMDS.
 */
void
ViaDisplaySetStreamOnDFP(ScrnInfoPtr pScrn, Bool primary)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaDisplaySetStreamOnDFP\n"));

    if (primary)
        ViaCrtcMask(hwp, 0x99, 0x00, 0x10);
    else
        ViaCrtcMask(hwp, 0x99, 0x10, 0x10);
}

void
ViaDisplaySetStreamOnDVO(ScrnInfoPtr pScrn, int port, Bool primary)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    int regNum;
    
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaDisplaySetStreamOnDVO, port: %d\n", port));

    switch (port) {
        case VIA_DI_PORT_DVP0:
            regNum = 0x96;
            break;
        case VIA_DI_PORT_DVP1:
            regNum = 0x9B;
            break;
        case VIA_DI_PORT_DFPLOW:
            regNum = 0x97;
            break;
        case VIA_DI_PORT_DFPHIGH:
            regNum = 0x99;
            break;
    }

    if (primary)
        ViaCrtcMask(hwp, regNum, 0x00, 0x10);
    else
        ViaCrtcMask(hwp, regNum, 0x10, 0x10);
}

