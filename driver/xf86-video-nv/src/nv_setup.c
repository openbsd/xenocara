/*
 * Copyright (c) 2003 NVIDIA, Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "nv_include.h"

/*
 * Override VGA I/O routines.
 */
static void NVWriteCrtc(vgaHWPtr pVga, CARD8 index, CARD8 value)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    VGA_WR08(pNv->PCIO, pVga->IOBase + VGA_CRTC_INDEX_OFFSET, index);
    VGA_WR08(pNv->PCIO, pVga->IOBase + VGA_CRTC_DATA_OFFSET,  value);
}
static CARD8 NVReadCrtc(vgaHWPtr pVga, CARD8 index)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    VGA_WR08(pNv->PCIO, pVga->IOBase + VGA_CRTC_INDEX_OFFSET, index);
    return (VGA_RD08(pNv->PCIO, pVga->IOBase + VGA_CRTC_DATA_OFFSET));
}
static void NVWriteGr(vgaHWPtr pVga, CARD8 index, CARD8 value)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    VGA_WR08(pNv->PVIO, VGA_GRAPH_INDEX, index);
    VGA_WR08(pNv->PVIO, VGA_GRAPH_DATA,  value);
}
static CARD8 NVReadGr(vgaHWPtr pVga, CARD8 index)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    VGA_WR08(pNv->PVIO, VGA_GRAPH_INDEX, index);
    return (VGA_RD08(pNv->PVIO, VGA_GRAPH_DATA));
}
static void NVWriteSeq(vgaHWPtr pVga, CARD8 index, CARD8 value)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    VGA_WR08(pNv->PVIO, VGA_SEQ_INDEX, index);
    VGA_WR08(pNv->PVIO, VGA_SEQ_DATA,  value);
}
static CARD8 NVReadSeq(vgaHWPtr pVga, CARD8 index)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    VGA_WR08(pNv->PVIO, VGA_SEQ_INDEX, index);
    return (VGA_RD08(pNv->PVIO, VGA_SEQ_DATA));
}
static void NVWriteAttr(vgaHWPtr pVga, CARD8 index, CARD8 value)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    volatile CARD8 tmp;

    tmp = VGA_RD08(pNv->PCIO, pVga->IOBase + VGA_IN_STAT_1_OFFSET);
    if (pVga->paletteEnabled)
        index &= ~0x20;
    else
        index |= 0x20;
    VGA_WR08(pNv->PCIO, VGA_ATTR_INDEX,  index);
    VGA_WR08(pNv->PCIO, VGA_ATTR_DATA_W, value);
}
static CARD8 NVReadAttr(vgaHWPtr pVga, CARD8 index)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    volatile CARD8 tmp;

    tmp = VGA_RD08(pNv->PCIO, pVga->IOBase + VGA_IN_STAT_1_OFFSET);
    if (pVga->paletteEnabled)
        index &= ~0x20;
    else
        index |= 0x20;
    VGA_WR08(pNv->PCIO, VGA_ATTR_INDEX, index);
    return (VGA_RD08(pNv->PCIO, VGA_ATTR_DATA_R));
}
static void NVWriteMiscOut(vgaHWPtr pVga, CARD8 value)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    VGA_WR08(pNv->PVIO, VGA_MISC_OUT_W, value);
}
static CARD8 NVReadMiscOut(vgaHWPtr pVga)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    return (VGA_RD08(pNv->PVIO, VGA_MISC_OUT_R));
}
static void NVEnablePalette(vgaHWPtr pVga)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    volatile CARD8 tmp;

    tmp = VGA_RD08(pNv->PCIO, pVga->IOBase + VGA_IN_STAT_1_OFFSET);
    VGA_WR08(pNv->PCIO, VGA_ATTR_INDEX, 0x00);
    pVga->paletteEnabled = TRUE;
}
static void NVDisablePalette(vgaHWPtr pVga)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    volatile CARD8 tmp;

    tmp = VGA_RD08(pNv->PCIO, pVga->IOBase + VGA_IN_STAT_1_OFFSET);
    VGA_WR08(pNv->PCIO, VGA_ATTR_INDEX, 0x20);
    pVga->paletteEnabled = FALSE;
}
static void NVWriteDacMask(vgaHWPtr pVga, CARD8 value)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    VGA_WR08(pNv->PDIO, VGA_DAC_MASK, value);
}
static CARD8 NVReadDacMask(vgaHWPtr pVga)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    return (VGA_RD08(pNv->PDIO, VGA_DAC_MASK));
}
static void NVWriteDacReadAddr(vgaHWPtr pVga, CARD8 value)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    VGA_WR08(pNv->PDIO, VGA_DAC_READ_ADDR, value);
}
static void NVWriteDacWriteAddr(vgaHWPtr pVga, CARD8 value)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    VGA_WR08(pNv->PDIO, VGA_DAC_WRITE_ADDR, value);
}
static void NVWriteDacData(vgaHWPtr pVga, CARD8 value)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    VGA_WR08(pNv->PDIO, VGA_DAC_DATA, value);
}
static CARD8 NVReadDacData(vgaHWPtr pVga)
{
    NVPtr pNv = (NVPtr)pVga->MMIOBase;
    return (VGA_RD08(pNv->PDIO, VGA_DAC_DATA));
}

static Bool 
NVIsConnected (ScrnInfoPtr pScrn, int output)
{
    NVPtr pNv = NVPTR(pScrn);
    volatile U032 *PRAMDAC = pNv->PRAMDAC0;
    CARD32 reg52C, reg608, dac0_reg608 = 0;
    Bool present;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "Probing for analog device on output %s...\n", 
                output ? "B" : "A");

    if(output) {
        dac0_reg608 = PRAMDAC[0x0608/4];
        PRAMDAC += 0x800;
    }

    reg52C = PRAMDAC[0x052C/4];
    reg608 = PRAMDAC[0x0608/4];

    PRAMDAC[0x0608/4] = reg608 & ~0x00010000;

    PRAMDAC[0x052C/4] = reg52C & 0x0000FEEE;
    usleep(1000);
    PRAMDAC[0x052C/4] |= 1;

    pNv->PRAMDAC0[0x0610/4] = 0x94050140;
    pNv->PRAMDAC0[0x0608/4] |= 0x00001000;

    usleep(1000);

    present = (PRAMDAC[0x0608/4] & (1 << 28)) ? TRUE : FALSE;

    if(present)
       xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "  ...found one\n");
    else
       xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "  ...can't find one\n");

    if(output)
        pNv->PRAMDAC0[0x0608/4] = dac0_reg608;

    PRAMDAC[0x052C/4] = reg52C;
    PRAMDAC[0x0608/4] = reg608;

    return present;
}

static void
NVSelectHeadRegisters(ScrnInfoPtr pScrn, int head)
{
    NVPtr pNv = NVPTR(pScrn);

    if(head) {
       pNv->PCIO = pNv->PCIO0 + 0x2000;
       pNv->PCRTC = pNv->PCRTC0 + 0x800;
       pNv->PRAMDAC = pNv->PRAMDAC0 + 0x800;
       pNv->PDIO = pNv->PDIO0 + 0x2000;
    } else {
       pNv->PCIO = pNv->PCIO0;
       pNv->PCRTC = pNv->PCRTC0;
       pNv->PRAMDAC = pNv->PRAMDAC0;
       pNv->PDIO = pNv->PDIO0;
    }
}

static xf86MonPtr 
NVProbeDDC (ScrnInfoPtr pScrn, int bus)
{
    NVPtr pNv = NVPTR(pScrn);
    xf86MonPtr MonInfo = NULL;

    if(!pNv->I2C) return NULL;

    pNv->DDCBase = bus ? 0x36 : 0x3e;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
               "Probing for EDID on I2C bus %s...\n", bus ? "B" : "A");

#ifdef EDID_COMPLETE_RAWDATA
    MonInfo = xf86DoEEDID(XF86_SCRN_ARG(pScrn), pNv->I2C, TRUE);
#else
    MonInfo = xf86DoEDID_DDC2(XF86_SCRN_ARG(pScrn), pNv->I2C);
#endif
    if (MonInfo) {
       xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                  "DDC detected a %s:\n", MonInfo->features.input_type ?
                  "DFP" : "CRT");
       xf86PrintEDID( MonInfo );
    } else {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
                  "  ... none found\n");
    }

    return MonInfo;
}

static void nv4GetConfig (NVPtr pNv)
{
    if (pNv->PFB[0x0000/4] & 0x00000100) {
        pNv->RamAmountKBytes = ((pNv->PFB[0x0000/4] >> 12) & 0x0F) * 1024 * 2
                              + 1024 * 2;
    } else {
        switch (pNv->PFB[0x0000/4] & 0x00000003) {
        case 0:
            pNv->RamAmountKBytes = 1024 * 32;
            break;
        case 1:
            pNv->RamAmountKBytes = 1024 * 4;
            break;
        case 2:
            pNv->RamAmountKBytes = 1024 * 8;
            break;
        case 3:
        default:
            pNv->RamAmountKBytes = 1024 * 16;
            break;
        }
    }
    pNv->CrystalFreqKHz = (pNv->PEXTDEV[0x0000/4] & 0x00000040) ? 14318 : 13500;
    pNv->CURSOR         = &(pNv->PRAMIN[0x1E00]);
    pNv->MinVClockFreqKHz = 12000;
    pNv->MaxVClockFreqKHz = 350000;
}

static void nv10GetConfig (NVPtr pNv)
{
    CARD32 implementation = pNv->Chipset & 0x0ff0;

#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* turn on big endian register access */
    if(!(pNv->PMC[0x0004/4] & 0x01000001)) {
       pNv->PMC[0x0004/4] = 0x01000001;
       mem_barrier();
    }
#endif

#if XSERVER_LIBPCIACCESS
    {
    /* [AGP]: I don't know if this is correct */
    struct pci_device *dev = pci_device_find_by_slot(0, 0, 0, 1);

    if(implementation == 0x01a0) {
        uint32_t amt;
        pci_device_cfg_read_u32(dev, &amt, 0x7C);
        pNv->RamAmountKBytes = (((amt >> 6) & 31) + 1) * 1024;
    } else if(implementation == 0x01f0) {
        uint32_t amt;
        pci_device_cfg_read_u32(dev, &amt, 0x84);
        pNv->RamAmountKBytes = (((amt >> 4) & 127) + 1) * 1024;
    } else {
        pNv->RamAmountKBytes = (pNv->PFB[0x020C/4] & 0xFFF00000) >> 10;
    }
    }
#else
    if(implementation == 0x01a0) {
        int amt = pciReadLong(pciTag(0, 0, 1), 0x7C);
        pNv->RamAmountKBytes = (((amt >> 6) & 31) + 1) * 1024;
    } else if(implementation == 0x01f0) {
        int amt = pciReadLong(pciTag(0, 0, 1), 0x84);
        pNv->RamAmountKBytes = (((amt >> 4) & 127) + 1) * 1024;
    } else {
        pNv->RamAmountKBytes = (pNv->PFB[0x020C/4] & 0xFFF00000) >> 10;
    }
#endif

    if(pNv->RamAmountKBytes > 256*1024)
        pNv->RamAmountKBytes = 256*1024;

    pNv->CrystalFreqKHz = (pNv->PEXTDEV[0x0000/4] & (1 << 6)) ? 14318 : 13500;
    
    if(pNv->twoHeads && (implementation != 0x0110))
    {
       if(pNv->PEXTDEV[0x0000/4] & (1 << 22))
           pNv->CrystalFreqKHz = 27000;
    }

    pNv->CURSOR           = NULL;  /* can't set this here */
    pNv->MinVClockFreqKHz = 12000;
    pNv->MaxVClockFreqKHz = pNv->twoStagePLL ? 400000 : 350000;
}


void
NVCommonSetup(ScrnInfoPtr pScrn)
{
    NVPtr pNv = NVPTR(pScrn);
    vgaHWPtr pVga = VGAHWPTR(pScrn);
    CARD16 implementation = pNv->Chipset & 0x0ff0;
    xf86MonPtr monitorA, monitorB;
    Bool mobile = FALSE;
    Bool tvA = FALSE;
    Bool tvB = FALSE;
    int FlatPanel = -1;   /* really means the CRTC is slaved */
    Bool Television = FALSE;
    void *tmp;
#if XSERVER_LIBPCIACCESS
    int err;
#endif

    /*
     * Override VGA I/O routines.
     */
    pVga->writeCrtc         = NVWriteCrtc;
    pVga->readCrtc          = NVReadCrtc;
    pVga->writeGr           = NVWriteGr;
    pVga->readGr            = NVReadGr;
    pVga->writeAttr         = NVWriteAttr;
    pVga->readAttr          = NVReadAttr;
    pVga->writeSeq          = NVWriteSeq;
    pVga->readSeq           = NVReadSeq;
    pVga->writeMiscOut      = NVWriteMiscOut;
    pVga->readMiscOut       = NVReadMiscOut;
    pVga->enablePalette     = NVEnablePalette;
    pVga->disablePalette    = NVDisablePalette;
    pVga->writeDacMask      = NVWriteDacMask;
    pVga->readDacMask       = NVReadDacMask;
    pVga->writeDacWriteAddr = NVWriteDacWriteAddr;
    pVga->writeDacReadAddr  = NVWriteDacReadAddr;
    pVga->writeDacData      = NVWriteDacData;
    pVga->readDacData       = NVReadDacData;
    /*
     * Note: There are different pointers to the CRTC/AR and GR/SEQ registers.
     * Bastardize the intended uses of these to make it work.
     */
    pVga->MMIOBase   = (CARD8 *)pNv;
    pVga->MMIOOffset = 0;

#if XSERVER_LIBPCIACCESS
    err = pci_device_map_range(pNv->PciInfo, pNv->IOAddress, 0x01000000,
			       PCI_DEV_MAP_FLAG_WRITABLE, &tmp);
    if (err != 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "pci_device_map_range failed: %s\n", strerror(err));
    }
#else
    tmp = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO | VIDMEM_READSIDEEFFECT,
                        pNv->PciTag, pNv->IOAddress, 0x01000000);
#endif
    pNv->REGS = tmp;

    pNv->PRAMIN   = pNv->REGS + (0x00710000/4);
    pNv->PCRTC0   = pNv->REGS + (0x00600000/4);
    pNv->PRAMDAC0 = pNv->REGS + (0x00680000/4);
    pNv->PFB      = pNv->REGS + (0x00100000/4);
    pNv->PFIFO    = pNv->REGS + (0x00002000/4);
    pNv->PGRAPH   = pNv->REGS + (0x00400000/4);
    pNv->PEXTDEV  = pNv->REGS + (0x00101000/4);
    pNv->PTIMER   = pNv->REGS + (0x00009000/4);
    pNv->PMC      = pNv->REGS + (0x00000000/4);
    pNv->FIFO     = pNv->REGS + (0x00800000/4);

    /* 8 bit registers */
    pNv->PCIO0    = (U008*)pNv->REGS + 0x00601000;
    pNv->PDIO0    = (U008*)pNv->REGS + 0x00681000;
    pNv->PVIO     = (U008*)pNv->REGS + 0x000C0000;

    pNv->twoHeads =  (pNv->Architecture >= NV_ARCH_10) &&
                     (implementation != 0x0100) &&
                     (implementation != 0x0150) &&
                     (implementation != 0x01A0) &&
                     (implementation != 0x0200);

    pNv->fpScaler = (pNv->FpScale && pNv->twoHeads && (implementation!=0x0110));

    pNv->twoStagePLL = (implementation == 0x0310) ||
                       (implementation == 0x0340) ||
                       (pNv->Architecture >= NV_ARCH_40);

    pNv->WaitVSyncPossible = (pNv->Architecture >= NV_ARCH_10) &&
                             (implementation != 0x0100);

    pNv->BlendingPossible = ((pNv->Chipset & 0xffff) != 0x0020);

    /* look for known laptop chips */
    switch(pNv->Chipset & 0xffff) {
    case 0x0112:
    case 0x0174:
    case 0x0175:
    case 0x0176:
    case 0x0177:
    case 0x0179:
    case 0x017C:
    case 0x017D:
    case 0x0186:
    case 0x0187:
    case 0x018D:
    case 0x0228:
    case 0x0286:
    case 0x028C:
    case 0x0316:
    case 0x0317:
    case 0x031A:
    case 0x031B:
    case 0x031C:
    case 0x031D:
    case 0x031E:
    case 0x031F:
    case 0x0324:
    case 0x0325:
    case 0x0328:
    case 0x0329:
    case 0x032C:
    case 0x032D:
    case 0x0347:
    case 0x0348:
    case 0x0349:
    case 0x034B:
    case 0x034C:
    case 0x0160:
    case 0x0166:
    case 0x0169:
    case 0x016B:
    case 0x016C:
    case 0x016D:
    case 0x00C8:
    case 0x00CC:
    case 0x0144:
    case 0x0146:
    case 0x0148:
    case 0x0098:
    case 0x0099:
        mobile = TRUE;
        break;
    default:
        break;
    }

    if(pNv->Architecture == NV_ARCH_04)
        nv4GetConfig(pNv);
    else
        nv10GetConfig(pNv);

    NVSelectHeadRegisters(pScrn, 0);

    NVLockUnlock(pNv, 0);

    NVI2CInit(pScrn);

    pNv->Television = FALSE;

    vgaHWGetIOBase(pVga);

    if(!pNv->twoHeads) {
       pNv->CRTCnumber = 0;
       if((monitorA = NVProbeDDC(pScrn, 0))) {
           FlatPanel = monitorA->features.input_type ? 1 : 0;

           /* NV4 doesn't support FlatPanels */
           if((pNv->Chipset & 0x0fff) <= 0x0020)
              FlatPanel = 0;
       } else {
           VGA_WR08(pNv->PCIO, 0x03D4, 0x28);
           if(VGA_RD08(pNv->PCIO, 0x03D5) & 0x80) {
              VGA_WR08(pNv->PCIO, 0x03D4, 0x33);
              if(!(VGA_RD08(pNv->PCIO, 0x03D5) & 0x01)) 
                 Television = TRUE;
              FlatPanel = 1;
           } else {
              FlatPanel = 0;
           }
           xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                         "HW is currently programmed for %s\n",
                          FlatPanel ? (Television ? "TV" : "DFP") : "CRT");
       } 

       if(pNv->FlatPanel == -1) {
           pNv->FlatPanel = FlatPanel;
           pNv->Television = Television;
       } else {
           xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                      "Forcing display type to %s as specified\n", 
                       pNv->FlatPanel ? "DFP" : "CRT");
       }
    } else {
       CARD8 outputAfromCRTC, outputBfromCRTC;
       int CRTCnumber = -1;
       CARD8 slaved_on_A, slaved_on_B;
       Bool analog_on_A, analog_on_B;
       CARD32 oldhead;
       CARD8 cr44;
      
       if(implementation != 0x0110) {
           if(pNv->PRAMDAC0[0x0000052C/4] & 0x100)
               outputAfromCRTC = 1;
           else            
               outputAfromCRTC = 0;
           if(pNv->PRAMDAC0[0x0000252C/4] & 0x100)
               outputBfromCRTC = 1;
           else
               outputBfromCRTC = 0;
          analog_on_A = NVIsConnected(pScrn, 0);
          analog_on_B = NVIsConnected(pScrn, 1);
       } else {
          outputAfromCRTC = 0;
          outputBfromCRTC = 1;
          analog_on_A = FALSE;
          analog_on_B = FALSE;
       }

       VGA_WR08(pNv->PCIO, 0x03D4, 0x44);
       cr44 = VGA_RD08(pNv->PCIO, 0x03D5);

       VGA_WR08(pNv->PCIO, 0x03D5, 3);
       NVSelectHeadRegisters(pScrn, 1);
       NVLockUnlock(pNv, 0);

       VGA_WR08(pNv->PCIO, 0x03D4, 0x28);
       slaved_on_B = VGA_RD08(pNv->PCIO, 0x03D5) & 0x80;
       if(slaved_on_B) {
           VGA_WR08(pNv->PCIO, 0x03D4, 0x33);
           tvB = !(VGA_RD08(pNv->PCIO, 0x03D5) & 0x01);
       }

       VGA_WR08(pNv->PCIO, 0x03D4, 0x44);
       VGA_WR08(pNv->PCIO, 0x03D5, 0);
       NVSelectHeadRegisters(pScrn, 0);
       NVLockUnlock(pNv, 0);

       VGA_WR08(pNv->PCIO, 0x03D4, 0x28);
       slaved_on_A = VGA_RD08(pNv->PCIO, 0x03D5) & 0x80; 
       if(slaved_on_A) {
           VGA_WR08(pNv->PCIO, 0x03D4, 0x33);
           tvA = !(VGA_RD08(pNv->PCIO, 0x03D5) & 0x01);
       }

       oldhead = pNv->PCRTC0[0x00000860/4];
       pNv->PCRTC0[0x00000860/4] = oldhead | 0x00000010;

       monitorA = NVProbeDDC(pScrn, 0);
       monitorB = NVProbeDDC(pScrn, 1);

       if(slaved_on_A && !tvA) {
          CRTCnumber = 0;
          FlatPanel = 1;
          xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                    "CRTC 0 is currently programmed for DFP\n");
       } else 
       if(slaved_on_B && !tvB) {
          CRTCnumber = 1;
          FlatPanel = 1;
          xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                    "CRTC 1 is currently programmed for DFP\n");
       } else
       if(analog_on_A) {
          CRTCnumber = outputAfromCRTC;
          FlatPanel = 0;
          xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                    "CRTC %i appears to have a CRT attached\n", CRTCnumber);
       } else
       if(analog_on_B) {
           CRTCnumber = outputBfromCRTC;
           FlatPanel = 0;
           xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                    "CRTC %i appears to have a CRT attached\n", CRTCnumber);
       } else
       if(slaved_on_A) {
          CRTCnumber = 0;
          FlatPanel = 1;
          Television = 1;
          xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                    "CRTC 0 is currently programmed for TV\n");
       } else
       if(slaved_on_B) {
          CRTCnumber = 1;
          FlatPanel = 1;
          Television = 1;
          xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                    "CRTC 1 is currently programmed for TV\n");
       } else
       if(monitorA) {
           FlatPanel = monitorA->features.input_type ? 1 : 0;
       } else 
       if(monitorB) {
           FlatPanel = monitorB->features.input_type ? 1 : 0;
       }

       if(pNv->FlatPanel == -1) {
          if(FlatPanel != -1) {
             pNv->FlatPanel = FlatPanel;
             pNv->Television = Television;
          } else {
             xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Unable to detect display type...\n");
             if(mobile) {
                 xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT,
                            "...On a laptop, assuming DFP\n");
                 pNv->FlatPanel = 1;
             } else {
                 xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT,
                            "...Using default of CRT\n");
                 pNv->FlatPanel = 0;
             }
          }
       } else {
           xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                      "Forcing display type to %s as specified\n", 
                       pNv->FlatPanel ? "DFP" : "CRT");
       }

       if(pNv->CRTCnumber == -1) {
          if(CRTCnumber != -1) pNv->CRTCnumber = CRTCnumber;
          else {
             xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Unable to detect which CRTCNumber...\n");
             if(pNv->FlatPanel) pNv->CRTCnumber = 1;
             else pNv->CRTCnumber = 0;
             xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT,
                        "...Defaulting to CRTCNumber %i\n", pNv->CRTCnumber);
          }
       } else {
           xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                      "Forcing CRTCNumber %i as specified\n", pNv->CRTCnumber);
       }
     
       if(monitorA) {
           if((monitorA->features.input_type && pNv->FlatPanel) ||
              (!monitorA->features.input_type && !pNv->FlatPanel))
           {
               if(monitorB) { 
                  free(monitorB);
                  monitorB = NULL;
               }
           } else {
              free(monitorA);
              monitorA = NULL;
           }
       }

       if(monitorB) {
           if((monitorB->features.input_type && !pNv->FlatPanel) ||
              (!monitorB->features.input_type && pNv->FlatPanel)) 
           {
              free(monitorB);
           } else {
              monitorA = monitorB;
           }
           monitorB = NULL;
       }

       if(implementation == 0x0110)
           cr44 = pNv->CRTCnumber * 0x3;

       pNv->PCRTC0[0x00000860/4] = oldhead;

       VGA_WR08(pNv->PCIO, 0x03D4, 0x44);
       VGA_WR08(pNv->PCIO, 0x03D5, cr44);
       NVSelectHeadRegisters(pScrn, pNv->CRTCnumber);
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
              "Using %s on CRTC %i\n",
              pNv->FlatPanel ? (pNv->Television ? "TV" : "DFP") : "CRT", 
              pNv->CRTCnumber);

    if(pNv->FlatPanel && !pNv->Television) {
       pNv->fpWidth = pNv->PRAMDAC[0x0820/4] + 1;
       pNv->fpHeight = pNv->PRAMDAC[0x0800/4] + 1;
       pNv->fpVTotal = pNv->PRAMDAC[0x804/4] + 1;
       pNv->fpSyncs = pNv->PRAMDAC[0x0848/4] & 0x30000033;
       xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Panel size is %i x %i\n",
                  pNv->fpWidth, pNv->fpHeight);
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "NOTE: This driver cannot "
                  "reconfigure the BIOS-programmed size.\n");
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "These dimensions will be used as "
                  "the panel size for mode validation.\n");
    }

    if(monitorA)
      xf86SetDDCproperties(pScrn, monitorA);

    if(!pNv->FlatPanel || (pScrn->depth != 24) || !pNv->twoHeads)
        pNv->FPDither = FALSE;

    pNv->LVDS = FALSE;
    if(pNv->FlatPanel && pNv->twoHeads) {
        pNv->PRAMDAC0[0x08B0/4] = 0x00010004;
        if(pNv->PRAMDAC0[0x08B4/4] & 1)
           pNv->LVDS = TRUE;
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Panel is %s\n", 
                   pNv->LVDS ? "LVDS" : "TMDS");
    }
}
