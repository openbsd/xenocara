/*
 * Copyright (c) 2007 NVIDIA, Corporation
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

static int
NVDACPanelTweaks(NVPtr pNv, NVRegPtr state)
{
   int tweak = 0;

   if(pNv->usePanelTweak) {
       tweak = pNv->PanelTweak;
   } else {
       /* begin flat panel hacks */
       /* This is unfortunate, but some chips need this register
          tweaked or else you get artifacts where adjacent pixels are
          swapped.  There are no hard rules for what to set here so all
          we can do is experiment and apply hacks. */

       if(((pNv->Chipset & 0xffff) == 0x0328) && (state->bpp == 32)) {
#if XSERVER_LIBPCIACCESS
          if (((pNv->PciInfo->subvendor_id & 0xffff) == 0x1179) &&
              ((pNv->PciInfo->subdevice_id & 0xffff) == 0x0020))
#else
          if (((pNv->PciInfo->subsysVendor & 0xffff) == 0x1179) &&
              ((pNv->PciInfo->subsysCard & 0xffff) == 0x0020))
#endif
          {

             /* Toshiba Tecra M2 */
             tweak = 1;
          } else {
             /* At least one NV34 laptop needs this workaround. */
             tweak = -1;
          }
       }

       if((pNv->Chipset & 0xfff0) == 0x0310) {
          tweak = 1;
       }
       /* end flat panel hacks */
   }

   return tweak;
}

Bool
NVDACInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    int i;
    int horizDisplay    = (mode->CrtcHDisplay/8)   - 1;
    int horizStart      = (mode->CrtcHSyncStart/8) - 1;
    int horizEnd        = (mode->CrtcHSyncEnd/8)   - 1;
    int horizTotal      = (mode->CrtcHTotal/8)     - 5;
    int horizBlankStart = (mode->CrtcHDisplay/8)   - 1;
    int horizBlankEnd   = (mode->CrtcHTotal/8)     - 1;
    int vertDisplay     =  mode->CrtcVDisplay      - 1;
    int vertStart       =  mode->CrtcVSyncStart    - 1;
    int vertEnd         =  mode->CrtcVSyncEnd      - 1;
    int vertTotal       =  mode->CrtcVTotal        - 2;
    int vertBlankStart  =  mode->CrtcVDisplay      - 1;
    int vertBlankEnd    =  mode->CrtcVTotal        - 1;
   
    NVPtr pNv = NVPTR(pScrn);
    NVRegPtr nvReg = &pNv->ModeReg;
    NVFBLayout *pLayout = &pNv->CurrentLayout;
    vgaRegPtr   pVga;

    /*
     * Initialize all of the generic VGA registers.  Don't bother with
     * VGA_FIX_SYNC_PULSES, given the relevant CRTC settings are overridden
     * below.  Ditto for the KGA workaround.
     */
    if (!vgaHWInit(pScrn, mode))
        return(FALSE);

    pVga = &VGAHWPTR(pScrn)->ModeReg;

    /*
     * Set all CRTC values.
     */

    if(mode->Flags & V_INTERLACE) 
        vertTotal |= 1;

    if(pNv->FlatPanel == 1) {
       vertStart = vertTotal - 3;  
       vertEnd = vertTotal - 2;
       vertBlankStart = vertStart;
       horizStart = horizTotal - 5;
       horizEnd = horizTotal - 2;   
       horizBlankEnd = horizTotal + 4;
       if(pNv->Architecture == NV_ARCH_30)
           horizTotal += 2;
    }

    pVga->CRTC[0x0]  = NV_Set8Bits(horizTotal);
    pVga->CRTC[0x1]  = NV_Set8Bits(horizDisplay);
    pVga->CRTC[0x2]  = NV_Set8Bits(horizBlankStart);
    pVga->CRTC[0x3]  = NV_SetBitField(horizBlankEnd,4:0,4:0) 
                       | NV_SetBit(7);
    pVga->CRTC[0x4]  = NV_Set8Bits(horizStart);
    pVga->CRTC[0x5]  = NV_SetBitField(horizBlankEnd,5:5,7:7)
                       | NV_SetBitField(horizEnd,4:0,4:0);
    pVga->CRTC[0x6]  = NV_SetBitField(vertTotal,7:0,7:0);
    pVga->CRTC[0x7]  = NV_SetBitField(vertTotal,8:8,0:0)
                       | NV_SetBitField(vertDisplay,8:8,1:1)
                       | NV_SetBitField(vertStart,8:8,2:2)
                       | NV_SetBitField(vertBlankStart,8:8,3:3)
                       | NV_SetBit(4)
                       | NV_SetBitField(vertTotal,9:9,5:5)
                       | NV_SetBitField(vertDisplay,9:9,6:6)
                       | NV_SetBitField(vertStart,9:9,7:7);
    pVga->CRTC[0x9]  = NV_SetBitField(vertBlankStart,9:9,5:5)
                       | NV_SetBit(6)
                       | ((mode->Flags & V_DBLSCAN) ? 0x80 : 0x00);
    pVga->CRTC[0x10] = NV_Set8Bits(vertStart);
    pVga->CRTC[0x11] = NV_SetBitField(vertEnd,3:0,3:0) | NV_SetBit(5);
    pVga->CRTC[0x12] = NV_Set8Bits(vertDisplay);
    pVga->CRTC[0x13] = ((pLayout->displayWidth/8)*(pLayout->bitsPerPixel/8));
    pVga->CRTC[0x15] = NV_Set8Bits(vertBlankStart);
    pVga->CRTC[0x16] = NV_Set8Bits(vertBlankEnd);

    pVga->Attribute[0x10] = 0x01;

    if(pNv->Television)
       pVga->Attribute[0x11] = 0x00;

    nvReg->screen = NV_SetBitField(horizBlankEnd,6:6,4:4)
                  | NV_SetBitField(vertBlankStart,10:10,3:3)
                  | NV_SetBitField(vertStart,10:10,2:2)
                  | NV_SetBitField(vertDisplay,10:10,1:1)
                  | NV_SetBitField(vertTotal,10:10,0:0);

    nvReg->horiz  = NV_SetBitField(horizTotal,8:8,0:0) 
                  | NV_SetBitField(horizDisplay,8:8,1:1)
                  | NV_SetBitField(horizBlankStart,8:8,2:2)
                  | NV_SetBitField(horizStart,8:8,3:3);

    nvReg->extra  = NV_SetBitField(vertTotal,11:11,0:0)
                    | NV_SetBitField(vertDisplay,11:11,2:2)
                    | NV_SetBitField(vertStart,11:11,4:4)
                    | NV_SetBitField(vertBlankStart,11:11,6:6);

    if(mode->Flags & V_INTERLACE) {
       horizTotal = (horizTotal >> 1) & ~1;
       nvReg->interlace = NV_Set8Bits(horizTotal);
       nvReg->horiz |= NV_SetBitField(horizTotal,8:8,4:4);
    } else {
       nvReg->interlace = 0xff;  /* interlace off */
    }


    /*
     * Initialize DAC palette.
     */
    if(pLayout->bitsPerPixel != 8 )
    {
        for (i = 0; i < 256; i++)
        {
            pVga->DAC[i*3]     = i;
            pVga->DAC[(i*3)+1] = i;
            pVga->DAC[(i*3)+2] = i;
        }
    }
    
    /*
     * Calculate the extended registers.
     */

    if(pLayout->depth < 24) 
	i = pLayout->depth;
    else i = 32;

    if(pNv->Architecture >= NV_ARCH_10)
	pNv->CURSOR = (U032 *)(pNv->FbStart + pNv->CursorStart);

    NVCalcStateExt(pNv, 
                    nvReg,
                    i,
                    pLayout->displayWidth,
                    mode->CrtcHDisplay,
                    pScrn->virtualY,
                    mode->Clock,
                    mode->Flags);

    nvReg->scale = pNv->PRAMDAC[0x00000848/4] & 0xfff000ff;
    if(pNv->FlatPanel == 1) {
       nvReg->pixel |= (1 << 7);
       if(!pNv->fpScaler || (pNv->fpWidth <= mode->HDisplay)
                         || (pNv->fpHeight <= mode->VDisplay))
       {
           nvReg->scale |= (1 << 8) ;
       }
       nvReg->crtcSync = pNv->PRAMDAC[0x0828/4];
       nvReg->crtcSync += NVDACPanelTweaks(pNv, nvReg);
       nvReg->crtcVSync = pNv->fpVTotal - 6;
    }

    nvReg->vpll = nvReg->pll;
    nvReg->vpll2 = nvReg->pll;
    nvReg->vpllB = nvReg->pllB;
    nvReg->vpll2B = nvReg->pllB;

    VGA_WR08(pNv->PCIO, 0x03D4, 0x1C);
    nvReg->fifo = VGA_RD08(pNv->PCIO, 0x03D5) & ~(1<<5);

    if(pNv->CRTCnumber) {
       nvReg->head  = pNv->PCRTC0[0x00000860/4] & ~0x00001000;
       nvReg->head2 = pNv->PCRTC0[0x00002860/4] | 0x00001000;
       nvReg->crtcOwner = 3;
       nvReg->pllsel |= 0x20000800;
       nvReg->vpll = pNv->PRAMDAC0[0x0508/4];
       if(pNv->twoStagePLL) 
          nvReg->vpllB = pNv->PRAMDAC0[0x0578/4];
    } else 
    if(pNv->twoHeads) {
       nvReg->head  =  pNv->PCRTC0[0x00000860/4] | 0x00001000;
       nvReg->head2 =  pNv->PCRTC0[0x00002860/4] & ~0x00001000;
       nvReg->crtcOwner = 0;
       nvReg->vpll2 = pNv->PRAMDAC0[0x0520/4];
       if(pNv->twoStagePLL) 
          nvReg->vpll2B = pNv->PRAMDAC0[0x057C/4];
    }

    nvReg->cursorConfig = 0x00000100;
    if(mode->Flags & V_DBLSCAN)
       nvReg->cursorConfig |= (1 << 4);
    if(pNv->alphaCursor) {
        if((pNv->Chipset & 0x0ff0) != 0x0110) 
           nvReg->cursorConfig |= 0x04011000;
        else
           nvReg->cursorConfig |= 0x14011000;
        nvReg->general |= (1 << 29);
    } else
       nvReg->cursorConfig |= 0x02000000;

    if(pNv->twoHeads) {
        if((pNv->Chipset & 0x0ff0) == 0x0110) {
           nvReg->dither = pNv->PRAMDAC[0x0528/4] & ~0x00010000;
           if(pNv->FPDither)
              nvReg->dither |= 0x00010000;
        } else {
           nvReg->dither = pNv->PRAMDAC[0x083C/4] & ~1;
           if(pNv->FPDither)
              nvReg->dither |= 1;
        } 
    }

    nvReg->timingH = 0;
    nvReg->timingV = 0;
    nvReg->displayV = mode->CrtcVDisplay;

    return (TRUE);
}

void 
NVDACRestore(ScrnInfoPtr pScrn, vgaRegPtr vgaReg, NVRegPtr nvReg,
             Bool primary)
{
    NVPtr pNv = NVPTR(pScrn);
    int restore = VGA_SR_MODE;

    if(primary) restore |= VGA_SR_CMAP | VGA_SR_FONTS;
    NVLoadStateExt(pNv, nvReg);
#if defined(__powerpc__)
    restore &= ~VGA_SR_FONTS;
#endif
    vgaHWRestore(pScrn, vgaReg, restore);
}

/*
 * NVDACSave
 *
 * This function saves the video state.
 */
void
NVDACSave(ScrnInfoPtr pScrn, vgaRegPtr vgaReg, NVRegPtr nvReg,
          Bool saveFonts)
{
    NVPtr pNv = NVPTR(pScrn);

#if defined(__powerpc__)
    saveFonts = FALSE;
#endif

    vgaHWSave(pScrn, vgaReg, VGA_SR_CMAP | VGA_SR_MODE | 
                             (saveFonts? VGA_SR_FONTS : 0));
    NVUnloadStateExt(pNv, nvReg);

    /* can't read this reliably on NV11 */
    if((pNv->Chipset & 0x0ff0) == 0x0110) 
       nvReg->crtcOwner = pNv->CRTCnumber;
}

#define DEPTH_SHIFT(val, w) ((val << (8 - w)) | (val >> ((w << 1) - 8)))
#define MAKE_INDEX(in, w) (DEPTH_SHIFT(in, w) * 3)

void
NVDACLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices, LOCO *colors,
                 VisualPtr pVisual )
{
    int i, index;
    NVPtr pNv = NVPTR(pScrn);
    vgaRegPtr   pVga;

    pVga = &VGAHWPTR(pScrn)->ModeReg;

    switch(pNv->CurrentLayout.depth) {
    case 15:
        for(i = 0; i < numColors; i++) {
            index = indices[i];
            pVga->DAC[MAKE_INDEX(index, 5) + 0] = colors[index].red;
            pVga->DAC[MAKE_INDEX(index, 5) + 1] = colors[index].green;
            pVga->DAC[MAKE_INDEX(index, 5) + 2] = colors[index].blue;
        }
        break;
    case 16:
        for(i = 0; i < numColors; i++) {
            index = indices[i];
            pVga->DAC[MAKE_INDEX(index, 6) + 1] = colors[index].green;
	    if(index < 32) {
            	pVga->DAC[MAKE_INDEX(index, 5) + 0] = colors[index].red;
            	pVga->DAC[MAKE_INDEX(index, 5) + 2] = colors[index].blue;
	    }
        }
        break;
    default:
	for(i = 0; i < numColors; i++) {
            index = indices[i];
            pVga->DAC[index*3]     = colors[index].red;
            pVga->DAC[(index*3)+1] = colors[index].green;
            pVga->DAC[(index*3)+2] = colors[index].blue;
	}
	break;
    }
    vgaHWRestore(pScrn, pVga, VGA_SR_CMAP);
}

/*
 * DDC1 support only requires DDC_SDA_MASK,
 * DDC2 support requires DDC_SDA_MASK and DDC_SCL_MASK
 */
#define DDC_SDA_READ_MASK  (1 << 3)
#define DDC_SCL_READ_MASK  (1 << 2)
#define DDC_SDA_WRITE_MASK (1 << 4)
#define DDC_SCL_WRITE_MASK (1 << 5)

static void
NV_I2CGetBits(I2CBusPtr b, int *clock, int *data)
{
#ifdef XF86_SCRN_INTERFACE
    NVPtr pNv = NVPTR(b->pScrn);
#else
    NVPtr pNv = NVPTR(xf86Screens[b->scrnIndex]);
#endif
    unsigned char val;

    /* Get the result. */
    VGA_WR08(pNv->PCIO, 0x3d4, pNv->DDCBase);
    val = VGA_RD08(pNv->PCIO, 0x3d5);

    *clock = (val & DDC_SCL_READ_MASK) != 0;
    *data  = (val & DDC_SDA_READ_MASK) != 0;
}

static void
NV_I2CPutBits(I2CBusPtr b, int clock, int data)
{
#ifdef XF86_SCRN_INTERFACE
    NVPtr pNv = NVPTR(b->pScrn);
#else
    NVPtr pNv = NVPTR(xf86Screens[b->scrnIndex]);
#endif
    unsigned char val;

    VGA_WR08(pNv->PCIO, 0x3d4, pNv->DDCBase + 1);
    val = VGA_RD08(pNv->PCIO, 0x3d5) & 0xf0;
    if (clock)
        val |= DDC_SCL_WRITE_MASK;
    else
        val &= ~DDC_SCL_WRITE_MASK;

    if (data)
        val |= DDC_SDA_WRITE_MASK;
    else
        val &= ~DDC_SDA_WRITE_MASK;

    VGA_WR08(pNv->PCIO, 0x3d4, pNv->DDCBase + 1);
    VGA_WR08(pNv->PCIO, 0x3d5, val | 0x1);
}

Bool
NVDACi2cInit(ScrnInfoPtr pScrn)
{
    NVPtr pNv = NVPTR(pScrn);
    I2CBusPtr I2CPtr;

    I2CPtr = xf86CreateI2CBusRec();
    if(!I2CPtr) return FALSE;

    pNv->I2C = I2CPtr;

    I2CPtr->BusName    = "DDC";
    I2CPtr->scrnIndex  = pScrn->scrnIndex;
#ifdef XF86_SCRN_INTERFACE
    I2CPtr->pScrn = pScrn;
#endif
    I2CPtr->I2CPutBits = NV_I2CPutBits;
    I2CPtr->I2CGetBits = NV_I2CGetBits;
    I2CPtr->AcknTimeout = 5;

    if (!xf86I2CBusInit(I2CPtr)) {
        return FALSE;
    }
    return TRUE;
}
