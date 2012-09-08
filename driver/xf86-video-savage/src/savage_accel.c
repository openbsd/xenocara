
/*
 *
 * Copyright 1995-1997 The XFree86 Project, Inc.
 *
 */

/*
 * The accel file for the Savage driver.  
 * 
 * Created 20/03/97 by Sebastien Marineau for 3.3.6
 * Modified 17-Nov-2000 by Tim Roberts for 4.0.1
 * Modified Feb-2004 by Alex Deucher - integrating DRI support
 * Revision: 
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "savage_driver.h"
#include "savage_regs.h"
#include "savage_bci.h"
#include "savage_streams.h"

#ifdef SAVAGEDRI
#define _XF86DRI_SERVER_
#include "savage_dri.h"
#endif

extern int gSavageEntityIndex;

/* Forward declaration of functions used in the driver */

unsigned long writedw( unsigned long addr, unsigned long value );
unsigned long readdw( unsigned long addr );
unsigned long readfb( unsigned long addr );
unsigned long writefb( unsigned long addr, unsigned long value );
void writescan( unsigned long scan, unsigned long color );

static int GetTileAperturePitch(unsigned long dwWidth, unsigned long dwBpp);
void SavageSetGBD_M7(ScrnInfoPtr pScrn);
void SavageSetGBD_3D(ScrnInfoPtr pScrn);
void SavageSetGBD_Twister(ScrnInfoPtr pScrn);
void SavageSetGBD_PM(ScrnInfoPtr pScrn);
void SavageSetGBD_2000(ScrnInfoPtr pScrn);

/*
 * This is used to cache the last known value for routines we want to
 * call from the debugger.
 */

ScrnInfoPtr gpScrn = 0;

/*
 *  returns the aperture pitch for tiled mode.
 *  if MM850C_15 = 0 (use NB linear tile mode) the pitch is screen stride aligned to 128bytes
 *  if MM850C_15 = 1 (use MS-1 128bit non-linear tile mode),we should do it as follows
 *  we now only support the later, and don't use Y range flag,see tile surface register
*/
static int GetTileAperturePitch(unsigned long dwWidth, unsigned long dwBpp)
{
    switch (dwBpp) {
        case 4:
        case 8:
            return(0x2000);
            break;
        case 16:
            return(0x1000);
            break;
        case 32:
            return(0x2000);
            break;
        default:
            return(0x2000);
    }
}

static int GetTileAperturePitch2000(unsigned long dwWidth, unsigned long dwBpp, int lDelta)
{
    switch (dwBpp) {
        case 4:
        case 8:
            return(0x2000);
            break;
        case 16:
	    if (lDelta > 0x800)
                return(0x1000);
	    else
	        return(0x800);
            break;
        case 32:
	    if (lDelta > 0x1000)
                return(0x2000);
	    else
	        return(0x1000);
            break;
        default:
            return(0x2000);
    }
}

void
SavageInitialize2DEngine(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    SavagePtr psav = SAVPTR(pScrn);
    unsigned int vgaCRIndex = hwp->IOBase + 4;
    unsigned int vgaCRReg = hwp->IOBase + 5;

    gpScrn = pScrn;

    VGAOUT16(vgaCRIndex, 0x0140);
    VGAOUT8(vgaCRIndex, 0x31);
    VGAOUT8(vgaCRReg, 0x0c);

    /* Setup plane masks */
    OUTREG(0x8128, ~0); /* enable all write planes */
    OUTREG(0x812C, ~0); /* enable all read planes */
    OUTREG16(0x8134, 0x27);
    OUTREG16(0x8136, 0x07);
    
    switch( psav->Chipset ) {

    case S3_SAVAGE3D:
    case S3_SAVAGE_MX:
	/* Disable BCI */
	OUTREG(0x48C18, INREG(0x48C18) & 0x3FF0);
	/* Setup BCI command overflow buffer */
	OUTREG(0x48C14, (psav->cobOffset >> 11) | (psav->cobIndex << 29)); /* tim */
    	/*OUTREG(S3_OVERFLOW_BUFFER, psav->cobOffset >> 11 | 0xE0000000);*/ /* S3 */
	/* Program shadow status update. */
	{
	    unsigned long thresholds = ((psav->bciThresholdLo & 0xffff) << 16) |
		(psav->bciThresholdHi & 0xffff);
	    OUTREG(0x48C10, thresholds);
	    /* used to be 0x78207220 */
	}
	if( psav->ShadowStatus )
	{
	    OUTREG(0x48C0C, psav->ShadowPhysical | 1 );
	    /* Enable BCI and command overflow buffer */
	    OUTREG(0x48C18, INREG(0x48C18) | 0x0E);
	}
	else
	{
	    OUTREG(0x48C0C, 0);
	    /* Enable BCI and command overflow buffer */
	    OUTREG(0x48C18, INREG(0x48C18) | 0x0C);
	}
	break;

    case S3_SAVAGE4:
    case S3_TWISTER:
    case S3_PROSAVAGE:
    case S3_PROSAVAGEDDR:
    case S3_SUPERSAVAGE:
	/* Disable BCI */
	OUTREG(0x48C18, INREG(0x48C18) & 0x3FF0);
	if (!psav->disableCOB) {
	    /* Setup BCI command overflow buffer */
	    OUTREG(0x48C14, (psav->cobOffset >> 11) | (psav->cobIndex << 29));
	}
	/* Program shadow status update */ /* AGD: what should this be? */
	{
	    unsigned long thresholds = ((psav->bciThresholdLo & 0x1fffe0) << 11) |
		((psav->bciThresholdHi & 0x1fffe0) >> 5);
	    OUTREG(0x48C10, thresholds);
	}
	/*OUTREG(0x48C10, 0x00700040);*/ /* tim */
        /*OUTREG(0x48C10, 0x0e440f04L);*/ /* S3 */
	if( psav->ShadowStatus )
	{
	    OUTREG(0x48C0C, psav->ShadowPhysical | 1 );
	    if (psav->disableCOB) {
	    	/* Enable BCI without the COB */
		OUTREG(0x48C18, INREG(0x48C18) | 0x0a);
	    } else {
		OUTREG32(0x48C18, INREG32(0x48C18) | 0x0E);
	    }
	}
	else
	{
	    OUTREG(0x48C0C, 0);
	    if (psav->disableCOB) {
	    	/* Enable BCI without the COB */
	    	OUTREG(0x48C18, INREG(0x48C18) | 0x08);
	    } else {
		OUTREG32(0x48C18, INREG32(0x48C18) | 0x0C);
	    }
	}
	break;

    case S3_SAVAGE2000:
	/* Disable BCI */
	OUTREG(0x48C18, 0);
	/* Setup BCI command overflow buffer */
	OUTREG(0x48C18, (psav->cobOffset >> 7) | (psav->cobIndex));
	if( psav->ShadowStatus )
	{
	    /* Set shadow update threshholds. */
	    /*OUTREG(0x48C10, 0x6090 );
	      OUTREG(0x48C14, 0x70A8 );*/
	    OUTREG(0x48C10, psav->bciThresholdLo >> 2);
	    OUTREG(0x48C14, psav->bciThresholdHi >> 2);
	    /* Enable shadow status update */
	    OUTREG(0x48A30, psav->ShadowPhysical );
	    /* Enable BCI, command overflow buffer and shadow status. */
	    OUTREG(0x48C18, INREG(0x48C18) | 0x00380000 );
	}
	else
	{
	    /* Disable shadow status update */
	    OUTREG(0x48A30, 0);
	    /* Enable BCI and command overflow buffer */
	    OUTREG(0x48C18, INREG(0x48C18) | 0x00280000 );
	}
	break;
    }

    /* Use and set global bitmap descriptor. */

    /* For reasons I do not fully understand yet, on the Savage4, the */
    /* write to the GBD register, MM816C, does not "take" at this time. */
    /* Only the low-order byte is acknowledged, resulting in an incorrect */
    /* stride.  Writing the register later, after the mode switch, works */
    /* correctly.  This needs to get resolved. */

    SavageSetGBD(pScrn);
} 

void
SavageSetGBD(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);
    
    UnProtectCRTC();
    UnLockExtRegs();
    VerticalRetraceWait();

    psav->lDelta = pScrn->virtualX * (pScrn->bitsPerPixel >> 3);

    /*
     * we can use Option "DisableTile" "TRUE" to disable tile mode
     * if don't disable tile,we only support tile mode under 16/32bpp
     */
    if ((!psav->bDisableTile) && ((pScrn->bitsPerPixel == 16) || (pScrn->bitsPerPixel == 32))) {
        /* tileing in 16/32 BPP */
        psav->bTiled = TRUE;        
        psav->lDelta = ((psav->lDelta + 127) >> 7) << 7;
            
        if (S3_SAVAGE3D_SERIES(psav->Chipset))
            psav->ulAperturePitch = 0x2000;
	else if (psav->Chipset == S3_SAVAGE2000)
	    psav->ulAperturePitch = GetTileAperturePitch2000(pScrn->virtualX,
							     pScrn->bitsPerPixel,
							     psav->lDelta);
        else            
            psav->ulAperturePitch = GetTileAperturePitch(pScrn->virtualX,pScrn->bitsPerPixel);
            
        /* Use the aperture for linear screen */
        psav->FBStart = psav->ApertureMap;
    } else {
        psav->bTiled = FALSE;
        /* 32: Alignment for nontiled mode */
        psav->lDelta = ((psav->lDelta + 31) >> 5) << 5;
        psav->ulAperturePitch = psav->lDelta;            
    }
        
    psav->Bpp = pScrn->bitsPerPixel >> 3;
    psav->cxMemory = psav->lDelta / (psav->Bpp);
    psav->cyMemory = psav->endfb / psav->lDelta - 1;
    /* ??????????? */
    if (psav->cyMemory > 2048)
        psav->cyMemory = 2048;
        
    /*
     * If tiling, adjust down psav->cyMemory to the last multiple
     * of a tileheight, so that we don't try to use partial tiles.
     */
    if (psav->bTiled)  {
        psav->cyMemory -= (psav->cyMemory % 16);
    }
    
    /*
     *  Initialization per GX-3.
     * 
     *  1. MM48C18 - Disable BCI.
     *  2. MM48C0C - Enable updating shadow status
     *              and initialize shadow memory address.
     *  2b. MM48C18 - bit 1 = 1, Enable Command Buffer status updates
     *              (S3_OVERFLOW_BUFFER_PTR)
     *  3. MM48C10 - Initialize command buffer threshold
     *              (S3_BUFFER_THRESHOLD)
     *  4. MM48C14 - Setup command buffer offset and size
     *              (S3_OVERFLOW_BUFFER)
     *  5. MM816C  - Enable BCI.
     *  6. MM48C40 - Setup tiled surface 0 register.
     *  7. CR31 - bit 0 = 0, Disable address offset bits(CR6A_6-0).
     *  8. CR50 - bit 7,6,0 = 111, Use Global Bitmap Descriptor.
     *  9. CR88 - bit 4 = 0, Block write on (linear mode) IFF we know we
     *                       have the right kind of SGRAM memory,
     *                       bit 4 = 1, Block write off (always off if tiling)
     *  10.CR69 - Bit 7 = 1, MM81C0 and 81C4 are used to control
     *                       primary stream.
     *  11.MM8128, MM812c - Setup read/write mask registers
     *  12.MM816C, MM8168 - Set up Global Bitmap Descriptor 1 and 2.
     */
    switch (psav->Chipset) {
        case S3_SAVAGE3D:
	    SavageSetGBD_3D(pScrn);
	    break;
        case S3_SAVAGE_MX:
            SavageSetGBD_M7(pScrn);            
            break;
        case S3_SAVAGE4:
        case S3_TWISTER:
        case S3_PROSAVAGE:            
        case S3_PROSAVAGEDDR:
            SavageSetGBD_Twister(pScrn);
            break;
        case S3_SUPERSAVAGE:
            SavageSetGBD_PM(pScrn);
            break;
        case S3_SAVAGE2000:
	    SavageSetGBD_2000(pScrn);
	    break;
    }
}

void SavageSetGBD_Twister(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);
    unsigned long       ulTmp;
    unsigned char byte;
    int bci_enable, tile16, tile32;

    if (psav->Chipset == S3_SAVAGE4) {
	bci_enable = BCI_ENABLE;
	tile16 = TILE_FORMAT_16BPP;
	tile32 = TILE_FORMAT_32BPP;
    } else {
	bci_enable = BCI_ENABLE_TWISTER;
	tile16 = TILE_DESTINATION;
	tile32 = TILE_DESTINATION;
    }
    
    /* MM81C0 and 81C4 are used to control primary stream. */
    OUTREG32(PSTREAM_FBADDR0_REG,0x00000000);
    OUTREG32(PSTREAM_FBADDR1_REG,0x00000000);
    
    /*
     *  Program Primary Stream Stride Register.
     *
     *  Tell engine if tiling on or off, set primary stream stride, and
     *  if tiling, set tiling bits/pixel and primary stream tile offset.
     *  Note that tile offset (bits 16 - 29) must be scanline width in
     *  bytes/128bytespertile * 256 Qwords/tile.  This is equivalent to
     *  lDelta * 2.  Remember that if tiling, lDelta is screenwidth in
     *  bytes padded up to an even number of tilewidths.
     */
    if (!psav->bTiled) {
        OUTREG32(PSTREAM_STRIDE_REG,
                 (((psav->lDelta * 2) << 16) & 0x3FFFE000) |
                 (psav->lDelta & 0x00001fff));
    }
    else if (pScrn->bitsPerPixel == 16) {
        /* Scanline-length-in-bytes/128-bytes-per-tile * 256 Qwords/tile */
        OUTREG32(PSTREAM_STRIDE_REG,
                 (((psav->lDelta * 2) << 16) & 0x3FFFE000)
                 | 0x80000000 | (psav->lDelta & 0x00001fff));
    }
    else if (pScrn->bitsPerPixel == 32) {
        OUTREG32(PSTREAM_STRIDE_REG,
                 (((psav->lDelta * 2) << 16) & 0x3FFFE000)
                 | 0xC0000000 | (psav->lDelta & 0x00001fff));
    }

    /*
     *  CR69, bit 7 = 1
     *  to use MM streams processor registers to control primary stream.
     */
    OUTREG8(CRT_ADDRESS_REG,0x69);
    byte = INREG8(CRT_DATA_REG) | 0x80;
    OUTREG8(CRT_DATA_REG,byte);

    OUTREG32(0x8128, 0xFFFFFFFFL);
    OUTREG32(0x812C, 0xFFFFFFFFL);

    OUTREG32(S3_BCI_GLB_BD_HIGH, bci_enable | S3_LITTLE_ENDIAN | S3_BD64);


    /* CR50, bit 7,6,0 = 111, Use GBD.*/
    OUTREG8(CRT_ADDRESS_REG,0x50);
    byte = INREG8(CRT_DATA_REG) | 0xC1;
    OUTREG8(CRT_DATA_REG, byte);

    /*
     * if MS1NB style linear tiling mode.
     * bit MM850C[15] = 0 select NB linear tile mode.
     * bit MM850C[15] = 1 select MS-1 128-bit non-linear tile mode.
     */
    ulTmp = INREG32(ADVANCED_FUNC_CTRL) | 0x8000; /* use MS-s style tile mode*/
    OUTREG32(ADVANCED_FUNC_CTRL,ulTmp);

    /*
     * Set up Tiled Surface Registers
     *  Bit 25:20 - Surface width in tiles.
     *  Bit 29 - Y Range Flag.
     *  Bit 31:30 = 00, 4 bpp.
     *            = 01, 8 bpp.
     *            = 10, 16 bpp.
     *            = 11, 32 bpp.
     */
    /*
     * Global Bitmap Descriptor Register MM816C - twister/prosavage
     *   bit 24~25: tile format
     *          00: linear 
     *          01: destination tiling format
     *          10: texture tiling format
     *          11: reserved
     *   bit 28: block write disble/enable
     *          0: disable
     *          1: enable
     */
    /*
     * Global Bitmap Descriptor Register MM816C - savage4
     *   bit 24~25: tile format
     *          00: linear 
     *          01: reserved
     *          10: 16 bpp tiles
     *          11: 32 bpp tiles
     *   bit 28: block write disable/enable
     *          0: enable
     *          1: disable
     */
    if (!psav->bTiled) {
        /*
         *  Do not enable block_write even for non-tiling modes, because
         *  the driver cannot determine if the memory type is the certain
         *  type of SGRAM for which block_write can be used.
         */
        psav->GlobalBD.bd1.HighPart.ResBWTile = TILE_FORMAT_LINEAR;/* linear */
    }
    else if (pScrn->bitsPerPixel == 16) {
	psav->GlobalBD.bd1.HighPart.ResBWTile = tile16; /* 16 bpp/destination tiling format */
        
        ulTmp = (((pScrn->virtualX + 0x3F) & 0x0000FFC0) >> 6) << 20;
        OUTREG32(TILED_SURFACE_REGISTER_0,ulTmp | TILED_SURF_BPP16);
    }
    else if (pScrn->bitsPerPixel == 32) {
        psav->GlobalBD.bd1.HighPart.ResBWTile = tile32; /* 32 bpp/destination tiling format */
        
        ulTmp = ( ((pScrn->virtualX + 0x1F) & 0x0000FFE0) >> 5) << 20;
        OUTREG32(TILED_SURFACE_REGISTER_0,ulTmp | TILED_SURF_BPP32);
    }

    psav->GlobalBD.bd1.HighPart.ResBWTile |= 0x10;/* disable block write - was 0 */
    /* HW uses width */
    psav->GlobalBD.bd1.HighPart.Stride = (unsigned short) psav->lDelta / (pScrn->bitsPerPixel >> 3);
    psav->GlobalBD.bd1.HighPart.Bpp = (unsigned char) (pScrn->bitsPerPixel);
    psav->GlobalBD.bd1.Offset = pScrn->fbOffset;
    

    /*
     * CR88, bit 4 - Block write enabled/disabled.
     *
     *      Note: Block write must be disabled when writing to tiled
     *            memory.  Even when writing to non-tiled memory, block
     *            write should only be enabled for certain types of SGRAM.
     */
    OUTREG8(CRT_ADDRESS_REG,0x88);
    byte = INREG8(CRT_DATA_REG) | DISABLE_BLOCK_WRITE_2D;
    OUTREG8(CRT_DATA_REG,byte);

    /*
     * CR31, bit 0 = 0, Disable address offset bits(CR6A_6-0).
     *       bit 0 = 1, Enable 8 Mbytes of display memory thru 64K window
     *                  at A000:0.
     */
    OUTREG8(CRT_ADDRESS_REG,MEMORY_CONFIG_REG); /* cr31 */
    byte = INREG8(CRT_DATA_REG) & (~(ENABLE_CPUA_BASE_A0000));
    OUTREG8(CRT_DATA_REG,byte); /* perhaps this should be 0x0c */

    /* turn on screen */
    OUTREG8(SEQ_ADDRESS_REG,0x01);
    byte = INREG8(SEQ_DATA_REG) & ~0x20;
    OUTREG8(SEQ_DATA_REG,byte);
    
    /* program the GBD and SBD's */
    OUTREG32(S3_GLB_BD_LOW,psav->GlobalBD.bd2.LoPart);
    OUTREG32(S3_GLB_BD_HIGH,psav->GlobalBD.bd2.HiPart | bci_enable | S3_LITTLE_ENDIAN | S3_BD64);
    OUTREG32(S3_PRI_BD_LOW,psav->GlobalBD.bd2.LoPart);
    OUTREG32(S3_PRI_BD_HIGH,psav->GlobalBD.bd2.HiPart);
    OUTREG32(S3_SEC_BD_LOW,psav->GlobalBD.bd2.LoPart);
    OUTREG32(S3_SEC_BD_HIGH,psav->GlobalBD.bd2.HiPart);
}

void SavageSetGBD_3D(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);
    unsigned long       ulTmp;
    unsigned char byte;
    int bci_enable, tile16, tile32;

    bci_enable = BCI_ENABLE;
    tile16 = TILE_FORMAT_16BPP;
    tile32 = TILE_FORMAT_32BPP;
    
    /* MM81C0 and 81C4 are used to control primary stream. */
    OUTREG32(PSTREAM_FBADDR0_REG,0x00000000);
    OUTREG32(PSTREAM_FBADDR1_REG,0x00000000);
    
    /*
     *  Program Primary Stream Stride Register.
     *
     *  Tell engine if tiling on or off, set primary stream stride, and
     *  if tiling, set tiling bits/pixel and primary stream tile offset.
     *  Note that tile offset (bits 16 - 29) must be scanline width in
     *  bytes/128bytespertile * 256 Qwords/tile.  This is equivalent to
     *  lDelta * 2.  Remember that if tiling, lDelta is screenwidth in
     *  bytes padded up to an even number of tilewidths.
     */
    if (!psav->bTiled) {
        OUTREG32(PSTREAM_STRIDE_REG,
                 (((psav->lDelta * 2) << 16) & 0x3FFFE000) |
                 (psav->lDelta & 0x00001fff));
    }
    else if (pScrn->bitsPerPixel == 16) {
        /* Scanline-length-in-bytes/128-bytes-per-tile * 256 Qwords/tile */
        OUTREG32(PSTREAM_STRIDE_REG,
                 (((psav->lDelta * 2) << 16) & 0x3FFFE000)
                 | 0x80000000 | (psav->lDelta & 0x00001fff));
    }
    else if (pScrn->bitsPerPixel == 32) {
        OUTREG32(PSTREAM_STRIDE_REG,
                 (((psav->lDelta * 2) << 16) & 0x3FFFE000)
                 | 0xC0000000 | (psav->lDelta & 0x00001fff));
    }

    /*
     *  CR69, bit 7 = 1
     *  to use MM streams processor registers to control primary stream.
     */
    OUTREG8(CRT_ADDRESS_REG,0x69);
    byte = INREG8(CRT_DATA_REG) | 0x80;
    OUTREG8(CRT_DATA_REG,byte);

    OUTREG32(0x8128, 0xFFFFFFFFL);
    OUTREG32(0x812C, 0xFFFFFFFFL);

    OUTREG32(S3_BCI_GLB_BD_HIGH, bci_enable | S3_LITTLE_ENDIAN | S3_BD64);


    /* CR50, bit 7,6,0 = 111, Use GBD.*/
    OUTREG8(CRT_ADDRESS_REG,0x50);
    byte = INREG8(CRT_DATA_REG) | 0xC1;
    OUTREG8(CRT_DATA_REG, byte);

    /*
     * if MS1NB style linear tiling mode.
     * bit MM850C[15] = 0 select NB linear tile mode.
     * bit MM850C[15] = 1 select MS-1 128-bit non-linear tile mode.
     */
    ulTmp = INREG32(ADVANCED_FUNC_CTRL) | 0x8000; /* use MS-s style tile mode*/
    OUTREG32(ADVANCED_FUNC_CTRL,ulTmp);

    /*
     * Tiled Surface 0 Registers MM48C40:
     *  bit 0~23: tile surface 0 frame buffer offset
     *  bit 24~29:tile surface 0 width
     *  bit 30~31:tile surface 0 bits/pixel
     *            00: reserved
     *            01, 8 bits
     *            10, 16 Bits.
     *            11, 32 Bits.
     */
    /*
     * Global Bitmap Descriptor Register MM816C
     *   bit 24~25: tile format
     *          00: linear 
     *          01: reserved
     *          10: 16 bpp tiles
     *          11: 32 bpp tiles
     *   bit 28: block write disable/enable
     *          0: enable
     *          1: disable
     */
    if (!psav->bTiled) {
        /*
         *  Do not enable block_write even for non-tiling modes, because
         *  the driver cannot determine if the memory type is the certain
         *  type of SGRAM for which block_write can be used.
         */
        psav->GlobalBD.bd1.HighPart.ResBWTile = TILE_FORMAT_LINEAR;/* linear */
    }
    else if (pScrn->bitsPerPixel == 16) {
	psav->GlobalBD.bd1.HighPart.ResBWTile = tile16; /* 16 bpp/destination tiling format */
        
        ulTmp = (((pScrn->virtualX + 0x3F) & 0x0000FFC0) >> 6) << 24;
        OUTREG32(TILED_SURFACE_REGISTER_0,ulTmp | TILED_SURF_BPP16);
    }
    else if (pScrn->bitsPerPixel == 32) {
        psav->GlobalBD.bd1.HighPart.ResBWTile = tile32; /* 32 bpp/destination tiling format */
        
        ulTmp = ( ((pScrn->virtualX + 0x1F) & 0x0000FFE0) >> 5) << 24;
        OUTREG32(TILED_SURFACE_REGISTER_0,ulTmp | TILED_SURF_BPP32);
    }

    psav->GlobalBD.bd1.HighPart.ResBWTile |= 0x10;/* disable block write - was 0 */
    /* HW uses width */
    psav->GlobalBD.bd1.HighPart.Stride = (unsigned short) psav->lDelta / (pScrn->bitsPerPixel >> 3);
    psav->GlobalBD.bd1.HighPart.Bpp = (unsigned char) (pScrn->bitsPerPixel);
    psav->GlobalBD.bd1.Offset = pScrn->fbOffset;
    

    /*
     * CR88, bit 4 - Block write enabled/disabled.
     *
     *      Note: Block write must be disabled when writing to tiled
     *            memory.  Even when writing to non-tiled memory, block
     *            write should only be enabled for certain types of SGRAM.
     */
    OUTREG8(CRT_ADDRESS_REG,0x88);
    byte = INREG8(CRT_DATA_REG) | DISABLE_BLOCK_WRITE_2D;
    OUTREG8(CRT_DATA_REG,byte);

    /*
     * CR31, bit 0 = 0, Disable address offset bits(CR6A_6-0).
     *       bit 0 = 1, Enable 8 Mbytes of display memory thru 64K window
     *                  at A000:0.
     */
    OUTREG8(CRT_ADDRESS_REG,MEMORY_CONFIG_REG); /* cr31 */
    byte = INREG8(CRT_DATA_REG) & (~(ENABLE_CPUA_BASE_A0000));
    OUTREG8(CRT_DATA_REG,byte); /* perhaps this should be 0x0c */

    /* turn on screen */
    OUTREG8(SEQ_ADDRESS_REG,0x01);
    byte = INREG8(SEQ_DATA_REG) & ~0x20;
    OUTREG8(SEQ_DATA_REG,byte);
    
    /* program the GBD and SBD's */
    OUTREG32(S3_GLB_BD_LOW,psav->GlobalBD.bd2.LoPart);
    OUTREG32(S3_GLB_BD_HIGH,psav->GlobalBD.bd2.HiPart | bci_enable | S3_LITTLE_ENDIAN | S3_BD64);
    OUTREG32(S3_PRI_BD_LOW,psav->GlobalBD.bd2.LoPart);
    OUTREG32(S3_PRI_BD_HIGH,psav->GlobalBD.bd2.HiPart);
    OUTREG32(S3_SEC_BD_LOW,psav->GlobalBD.bd2.LoPart);
    OUTREG32(S3_SEC_BD_HIGH,psav->GlobalBD.bd2.HiPart);
}

void SavageSetGBD_M7(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);
    unsigned long ulTmp;
    unsigned char byte;
    int bci_enable, tile16, tile32;

    bci_enable = BCI_ENABLE;
    tile16 = TILE_FORMAT_16BPP;
    tile32 = TILE_FORMAT_32BPP;


    /* following is the enable case */

    /* SR01:turn off screen */
    OUTREG8 (SEQ_ADDRESS_REG,0x01);
    byte = INREG8(SEQ_DATA_REG) | 0x20;
    OUTREG8(SEQ_DATA_REG,byte);

    /*
     * CR67_3:
     *  = 1  stream processor MMIO address and stride register
     *       are used to control the primary stream
     *  = 0  standard VGA address and stride registers
     *       are used to control the primary streams
     */
    if (psav->IsPrimary) {
    	OUTREG8(CRT_ADDRESS_REG,0x67); 
    	byte =  INREG8(CRT_DATA_REG) | 0x08;
    	OUTREG8(CRT_DATA_REG,byte);
    } else if (psav->IsSecondary) {
    	/* IGA 2 */
    	OUTREG16(SEQ_ADDRESS_REG,SELECT_IGA2_READS_WRITES);

    	OUTREG8(CRT_ADDRESS_REG,0x67); 
    	byte =  INREG8(CRT_DATA_REG) | 0x08;
    	OUTREG8(CRT_DATA_REG,byte);
             
    	OUTREG16(SEQ_ADDRESS_REG,SELECT_IGA1);
    } else {
    	OUTREG8(CRT_ADDRESS_REG,0x67); 
    	byte =  INREG8(CRT_DATA_REG) | 0x08;
    	OUTREG8(CRT_DATA_REG,byte);
    	/* IGA 2 */
    	OUTREG16(SEQ_ADDRESS_REG,SELECT_IGA2_READS_WRITES);
    	OUTREG8(CRT_ADDRESS_REG,0x67); 
    	byte =  INREG8(CRT_DATA_REG) | 0x08;
    	OUTREG8(CRT_DATA_REG,byte);             
    	OUTREG16(SEQ_ADDRESS_REG,SELECT_IGA1);
    }
    /* Set primary stream to bank 0 */
    OUTREG8(CRT_ADDRESS_REG, MEMORY_CTRL0_REG);/* CRCA */
    byte =  INREG8(CRT_DATA_REG) & ~(MEM_PS1 + MEM_PS2) ;
    OUTREG8(CRT_DATA_REG,byte);
#if 0
    /*
     * if we have 8MB of frame buffer here then we must really be a 16MB
     * card and that means that the second device is always in the upper
     * bank of memory (MHS)
     */
    if (psav->videoRambytes >= 0x800000) {
        /* 16MB Video Memory cursor is at the end in Bank 1 */
        byte |= 0x3;
        OUTREG16(CRT_ADDRESS_REG, (byte << 8) | MEMORY_CTRL0_REG);
    }
#endif

    /* MM81C0 and 81C4 are used to control primary stream. */
    if (psav->IsPrimary) {
    	OUTREG32(PRI_STREAM_FBUF_ADDR0,pScrn->fbOffset & 0x7fffff);
    	OUTREG32(PRI_STREAM_FBUF_ADDR1,pScrn->fbOffset & 0x7fffff);
    } else if (psav->IsSecondary) {
    	OUTREG32(PRI_STREAM2_FBUF_ADDR0,pScrn->fbOffset & 0x7fffff);
    	OUTREG32(PRI_STREAM2_FBUF_ADDR1,pScrn->fbOffset & 0x7fffff);
    } else {
    	OUTREG32(PRI_STREAM_FBUF_ADDR0,pScrn->fbOffset & 0x7fffff);
    	OUTREG32(PRI_STREAM_FBUF_ADDR1,pScrn->fbOffset & 0x7fffff);
    	OUTREG32(PRI_STREAM2_FBUF_ADDR0,pScrn->fbOffset & 0x7fffff);
    	OUTREG32(PRI_STREAM2_FBUF_ADDR1,pScrn->fbOffset & 0x7fffff);
    }
 
    /*
     *  Program Primary Stream Stride Register.
     *
     *  Tell engine if tiling on or off, set primary stream stride, and
     *  if tiling, set tiling bits/pixel and primary stream tile offset.
     *  Note that tile offset (bits 16 - 29) must be scanline width in
     *  bytes/128bytespertile * 256 Qwords/tile.  This is equivalent to
     *  lDelta * 2.  Remember that if tiling, lDelta is screenwidth in
     *  bytes padded up to an even number of tilewidths.
     */
    if (!psav->bTiled) {
 	if (psav->IsPrimary) {
            OUTREG32(PRI_STREAM_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000) |
                 (psav->lDelta & 0x00003fff));
	} else if (psav->IsSecondary) {
            OUTREG32(PRI_STREAM2_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000) |
                 (psav->lDelta & 0x00003fff));
	} else {
            OUTREG32(PRI_STREAM_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000) |
                 (psav->lDelta & 0x00003fff));
            OUTREG32(PRI_STREAM2_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000) |
                 (psav->lDelta & 0x00003fff));
	}

    } else if (pScrn->bitsPerPixel == 16) {
        /* Scanline-length-in-bytes/128-bytes-per-tile * 256 Qwords/tile */
	if (psav->IsPrimary) {
            OUTREG32(PRI_STREAM_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0x80000000 | (psav->lDelta & 0x00003fff));
        } else if (psav->IsSecondary) {
            OUTREG32(PRI_STREAM2_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0x80000000 | (psav->lDelta & 0x00003fff));
        } else {
            OUTREG32(PRI_STREAM_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0x80000000 | (psav->lDelta & 0x00003fff));
            OUTREG32(PRI_STREAM2_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0x80000000 | (psav->lDelta & 0x00003fff));
	}

    } else if (pScrn->bitsPerPixel == 32) {
	if (psav->IsPrimary) {
            OUTREG32(PRI_STREAM_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0xC0000000 | (psav->lDelta & 0x00003fff));
	} else if (psav->IsSecondary) {
            OUTREG32(PRI_STREAM2_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0xC0000000 | (psav->lDelta & 0x00003fff));
	} else {
            OUTREG32(PRI_STREAM_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0xC0000000 | (psav->lDelta & 0x00003fff));
            OUTREG32(PRI_STREAM2_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0xC0000000 | (psav->lDelta & 0x00003fff));
	}
    }

    OUTREG32(0x8128, 0xFFFFFFFFL);
    OUTREG32(0x812C, 0xFFFFFFFFL);

    if (!psav->IsSecondary)
    	OUTREG32(S3_BCI_GLB_BD_HIGH, bci_enable | S3_LITTLE_ENDIAN | S3_BD64);
    
    /* CR50, bit 7,6,0 = 111, Use GBD.*/
    OUTREG8(CRT_ADDRESS_REG,0x50);
    byte = INREG8(CRT_DATA_REG) | 0xC1;
    OUTREG8(CRT_DATA_REG, byte);

    /*
     * CR78, bit 3  - Block write enabled(1)/disabled(0).
     *       bit 2  - Block write cycle time(0:2 cycles,1: 1 cycle)
     *      Note: Block write must be disabled when writing to tiled
     *            memory.  Even when writing to non-tiled memory, block
     *            write should only be enabled for certain types of SGRAM.
     */
    OUTREG8(CRT_ADDRESS_REG,0x78);
    /*byte = INREG8(CRT_DATA_REG) & ~0x0C;*/
    byte = INREG8(CRT_DATA_REG) | 0xfb;
    OUTREG8(CRT_DATA_REG,byte);
    
    /*
     * Tiled Surface 0 Registers MM48C40:
     *  bit 0~23: tile surface 0 frame buffer offset
     *  bit 24~29:tile surface 0 width
     *  bit 30~31:tile surface 0 bits/pixel
     *            00: reserved
     *            01, 8 bits
     *            10, 16 Bits.
     *            11, 32 Bits.
     */
    /*
     * Global Bitmap Descriptor Register MM816C
     *   bit 24~25: tile format
     *          00: linear
     *          01: reserved
     *          10: 16 bit
     *          11: 32 bit
     *   bit 28: block write disble/enable
     *          0: enable
     *          1: disable
     */
    if (!psav->bTiled) {
        /*
         *  Do not enable block_write even for non-tiling modes, because
         *  the driver cannot determine if the memory type is the certain
         *  type of SGRAM for which block_write can be used.
         */
        psav->GlobalBD.bd1.HighPart.ResBWTile = TILE_FORMAT_LINEAR;/* linear */

    }
    else if (pScrn->bitsPerPixel == 16) {
        psav->GlobalBD.bd1.HighPart.ResBWTile = tile16;/* 16 bit */

	ulTmp =  ((psav->lDelta / 2) >> 6) << 24;
	if (psav->IsSecondary)
            OUTREG32(TILED_SURFACE_REGISTER_1,ulTmp | TILED_SURF_BPP16 | pScrn->fbOffset);
	else 
            OUTREG32(TILED_SURFACE_REGISTER_0,ulTmp | TILED_SURF_BPP16 | pScrn->fbOffset);
    }
    else if (pScrn->bitsPerPixel == 32) {
        psav->GlobalBD.bd1.HighPart.ResBWTile = tile32;/* 32 bit */
     
	ulTmp =  ((psav->lDelta / 4) >> 5) << 24;
	if (psav->IsSecondary)
            OUTREG32(TILED_SURFACE_REGISTER_1,ulTmp | TILED_SURF_BPP32 | pScrn->fbOffset);
	else 
            OUTREG32(TILED_SURFACE_REGISTER_0,ulTmp | TILED_SURF_BPP32 | pScrn->fbOffset);
    }
    
    psav->GlobalBD.bd1.HighPart.ResBWTile |= 0x10;/* disable block write */
    /* HW uses width */
    psav->GlobalBD.bd1.HighPart.Stride = (unsigned short)(psav->lDelta / (pScrn->bitsPerPixel >> 3));
    psav->GlobalBD.bd1.HighPart.Bpp = (unsigned char) (pScrn->bitsPerPixel);
    psav->GlobalBD.bd1.Offset = pScrn->fbOffset;    


    /*
     * CR31, bit 0 = 0, Disable address offset bits(CR6A_6-0).
     *       bit 0 = 1, Enable 8 Mbytes of display memory thru 64K window
     *                  at A000:0.
     */
#if 0
    OUTREG8(CRT_ADDRESS_REG,MEMORY_CONFIG_REG); /* cr31 */
    byte = INREG8(CRT_DATA_REG) & (~(ENABLE_CPUA_BASE_A0000));
    OUTREG8(CRT_DATA_REG,byte);
#endif
    OUTREG8(CRT_ADDRESS_REG,MEMORY_CONFIG_REG); /* cr31 */
    byte = (INREG8(CRT_DATA_REG) | 0x04) & 0xFE;
    OUTREG8(CRT_DATA_REG,byte);

    if (!psav->IsSecondary) {
    	/* program the GBD and SBD's */
    	OUTREG32(S3_GLB_BD_LOW,psav->GlobalBD.bd2.LoPart );
    	/* 8: bci enable */
    	OUTREG32(S3_GLB_BD_HIGH,(psav->GlobalBD.bd2.HiPart
                             | bci_enable | S3_LITTLE_ENDIAN | S3_BD64));
    	OUTREG32(S3_PRI_BD_LOW,psav->GlobalBD.bd2.LoPart);
    	OUTREG32(S3_PRI_BD_HIGH,psav->GlobalBD.bd2.HiPart);
    	OUTREG32(S3_SEC_BD_LOW,psav->GlobalBD.bd2.LoPart);
    	OUTREG32(S3_SEC_BD_HIGH,psav->GlobalBD.bd2.HiPart);
    }

    /* turn on screen */
    OUTREG8(SEQ_ADDRESS_REG,0x01);
    byte = INREG8(SEQ_DATA_REG) & ~0X20;
    OUTREG8(SEQ_DATA_REG,byte);
}
                       
void SavageSetGBD_PM(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);
    unsigned long ulTmp;
    unsigned char byte;
    int bci_enable, tile16, tile32;
    

    bci_enable = BCI_ENABLE_TWISTER;
    tile16 = TILE_DESTINATION;
    tile32 = TILE_DESTINATION;


    /* following is the enable case */
             
    /* SR01:turn off screen */
    OUTREG8 (SEQ_ADDRESS_REG,0x01);
    byte = INREG8(SEQ_DATA_REG) | 0x20;
    OUTREG8(SEQ_DATA_REG,byte);

    /*
     * CR67_3:
     *  = 1  stream processor MMIO address and stride register
     *       are used to control the primary stream
     *  = 0  standard VGA address and stride registers
     *       are used to control the primary streams
     */
    if (psav->IsPrimary) {
    	OUTREG8(CRT_ADDRESS_REG,0x67); 
    	byte =  INREG8(CRT_DATA_REG) | 0x08;
    	OUTREG8(CRT_DATA_REG,byte);
    } else if (psav->IsSecondary) {
    	/* IGA 2 */
    	OUTREG16(SEQ_ADDRESS_REG,SELECT_IGA2_READS_WRITES);

    	OUTREG8(CRT_ADDRESS_REG,0x67); 
    	byte =  INREG8(CRT_DATA_REG) | 0x08;
    	OUTREG8(CRT_DATA_REG,byte);
    
    	OUTREG16(SEQ_ADDRESS_REG,SELECT_IGA1);
    } else {
    	OUTREG8(CRT_ADDRESS_REG,0x67); 
    	byte =  INREG8(CRT_DATA_REG) | 0x08;
    	OUTREG8(CRT_DATA_REG,byte);
    	/* IGA 2 */
    	OUTREG16(SEQ_ADDRESS_REG,SELECT_IGA2_READS_WRITES);

    	OUTREG8(CRT_ADDRESS_REG,0x67); 
    	byte =  INREG8(CRT_DATA_REG) | 0x08;
    	OUTREG8(CRT_DATA_REG,byte);
    
    	OUTREG16(SEQ_ADDRESS_REG,SELECT_IGA1);
    }

    /*
     * load ps1 active registers as determined by MM81C0/81C4
     * load ps2 active registers as determined by MM81B0/81B4
     */
    OUTREG8(CRT_ADDRESS_REG,0x65); 
    byte =  INREG8(CRT_DATA_REG) | 0x03;
    OUTREG8(CRT_DATA_REG,byte);
    
    /*
     *  Program Primary Stream Stride Register.
     *
     *  Tell engine if tiling on or off, set primary stream stride, and
     *  if tiling, set tiling bits/pixel and primary stream tile offset.
     *  Note that tile offset (bits 16 - 29) must be scanline width in
     *  bytes/128bytespertile * 256 Qwords/tile.  This is equivalent to
     *  lDelta * 2.  Remember that if tiling, lDelta is screenwidth in
     *  bytes padded up to an even number of tilewidths.
     */
    if (!psav->bTiled) {
	if (psav->IsPrimary) {
            OUTREG32(PRI_STREAM_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000) |
                 (psav->lDelta & 0x00001fff));
	} else if (psav->IsSecondary) {
            OUTREG32(PRI_STREAM2_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000) |
                 (psav->lDelta & 0x00001fff));
	} else {
            OUTREG32(PRI_STREAM_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000) |
                 (psav->lDelta & 0x00001fff));
            OUTREG32(PRI_STREAM2_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000) |
                 (psav->lDelta & 0x00001fff));
	}
    } else if (pScrn->bitsPerPixel == 16) {
        /* Scanline-length-in-bytes/128-bytes-per-tile * 256 Qwords/tile */
	if (psav->IsPrimary) {
            OUTREG32(PRI_STREAM_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0x80000000 | (psav->lDelta & 0x00001fff));
	} else if (psav->IsSecondary) {
            OUTREG32(PRI_STREAM2_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0x80000000 | (psav->lDelta & 0x00001fff));
	} else {
            OUTREG32(PRI_STREAM_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0x80000000 | (psav->lDelta & 0x00001fff));
            OUTREG32(PRI_STREAM2_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0x80000000 | (psav->lDelta & 0x00001fff));
	}
        
    } else if (pScrn->bitsPerPixel == 32) {
	if (psav->IsPrimary) {
            OUTREG32(PRI_STREAM_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0xC0000000 | (psav->lDelta & 0x00001fff));
	} else if (psav->IsSecondary) {
            OUTREG32(PRI_STREAM2_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0xC0000000 | (psav->lDelta & 0x00001fff));
	} else {
            OUTREG32(PRI_STREAM_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0xC0000000 | (psav->lDelta & 0x00001fff));
            OUTREG32(PRI_STREAM2_STRIDE,
                 (((psav->lDelta * 2) << 16) & 0x3FFF0000)
                 | 0xC0000000 | (psav->lDelta & 0x00001fff));
	}
    }
    
    /* MM81C0 and 81C4 are used to control primary stream. */
    if (psav->IsPrimary) {
        OUTREG32(PRI_STREAM_FBUF_ADDR0,pScrn->fbOffset);
        OUTREG32(PRI_STREAM_FBUF_ADDR1,0x80000000);
    } else if (psav->IsSecondary) {
        OUTREG32(PRI_STREAM2_FBUF_ADDR0,(pScrn->fbOffset & 0xfffffffc) | 0x80000000);
        OUTREG32(PRI_STREAM2_FBUF_ADDR1,pScrn->fbOffset & 0xffffffc);
    } else {
        OUTREG32(PRI_STREAM_FBUF_ADDR0,pScrn->fbOffset);
        OUTREG32(PRI_STREAM_FBUF_ADDR1,0x80000000);
        OUTREG32(PRI_STREAM2_FBUF_ADDR0,(pScrn->fbOffset & 0xfffffffc) | 0x80000000);
        OUTREG32(PRI_STREAM2_FBUF_ADDR1,pScrn->fbOffset & 0xffffffc);
    }
    
    OUTREG32(0x8128, 0xFFFFFFFFL);
    OUTREG32(0x812C, 0xFFFFFFFFL);
    
    if (!psav->IsSecondary)
    	/* bit 28:block write disable */
    	OUTREG32(S3_GLB_BD_HIGH, bci_enable | S3_BD64 | 0x10000000); 
    
    /* CR50, bit 7,6,0 = 111, Use GBD.*/
    OUTREG8(CRT_ADDRESS_REG,0x50);
    byte = INREG8(CRT_DATA_REG) | 0xC1;
    OUTREG8(CRT_DATA_REG, byte);

    if (!psav->bTiled) {
        /*
         *  Do not enable block_write even for non-tiling modes, because
         *  the driver cannot determine if the memory type is the certain
         *  type of SGRAM for which block_write can be used.
         */
        psav->GlobalBD.bd1.HighPart.ResBWTile = TILE_FORMAT_LINEAR;/* linear */
        
    }
    else if (pScrn->bitsPerPixel == 16) {
        psav->GlobalBD.bd1.HighPart.ResBWTile = tile16;/* tile format destination */
        
        ulTmp =  (((pScrn->virtualX + 0x3f) & 0x0000ffc0) >> 6) << 20;
	if (psav->IsSecondary)
            OUTREG32(TILED_SURFACE_REGISTER_1,ulTmp | TILED_SURF_BPP16 | (pScrn->fbOffset>>6));
	else 
            OUTREG32(TILED_SURFACE_REGISTER_0,ulTmp | TILED_SURF_BPP16 | (pScrn->fbOffset>>6));
    }
    else if (pScrn->bitsPerPixel == 32) {
        psav->GlobalBD.bd1.HighPart.ResBWTile = tile32;/* tile format destination */
        
        ulTmp =  (((pScrn->virtualX + 0x1f) & 0x0000ffe0) >> 5) << 20;        
	if (psav->IsSecondary)
            OUTREG32(TILED_SURFACE_REGISTER_1,ulTmp | TILED_SURF_BPP32 | (pScrn->fbOffset>>6));
	else 
            OUTREG32(TILED_SURFACE_REGISTER_0,ulTmp | TILED_SURF_BPP32 | (pScrn->fbOffset>>6));
    }
    
    psav->GlobalBD.bd1.HighPart.ResBWTile |= 0x10;/* disable block write */
    /* HW uses width */
    psav->GlobalBD.bd1.HighPart.Stride = (unsigned short)(psav->lDelta / (pScrn->bitsPerPixel >> 3));
    psav->GlobalBD.bd1.HighPart.Bpp = (unsigned char) (pScrn->bitsPerPixel);
    psav->GlobalBD.bd1.Offset = pScrn->fbOffset;    

    /*
     * CR31, bit 0 = 0, Disable address offset bits(CR6A_6-0).
     *       bit 0 = 1, Enable 8 Mbytes of display memory thru 64K window
     *                  at A000:0.
     */
    OUTREG8(CRT_ADDRESS_REG,MEMORY_CONFIG_REG);
    byte = INREG8(CRT_DATA_REG) & (~(ENABLE_CPUA_BASE_A0000));
    OUTREG8(CRT_DATA_REG,byte);
    
    if (!psav->IsSecondary) {
    	/* program the GBD and SBDs */
    	OUTREG32(S3_GLB_BD_LOW,psav->GlobalBD.bd2.LoPart );
    	OUTREG32(S3_GLB_BD_HIGH,(psav->GlobalBD.bd2.HiPart 
			     | bci_enable | S3_LITTLE_ENDIAN | S3_BD64));
    	OUTREG32(S3_PRI_BD_LOW,psav->GlobalBD.bd2.LoPart);
    	OUTREG32(S3_PRI_BD_HIGH,psav->GlobalBD.bd2.HiPart);
    	OUTREG32(S3_SEC_BD_LOW,psav->GlobalBD.bd2.LoPart);
    	OUTREG32(S3_SEC_BD_HIGH,psav->GlobalBD.bd2.HiPart);
    }

    /* turn on screen */
    OUTREG8(SEQ_ADDRESS_REG,0x01);
    byte = INREG8(SEQ_DATA_REG) & ~0x20;
    OUTREG8(SEQ_DATA_REG,byte);
}

void SavageSetGBD_2000(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);
    unsigned long ulTmp, ulYRange;
    unsigned char byte;
    int bci_enable, tile16, tile32;
    
    bci_enable = BCI_ENABLE_TWISTER;
    tile16 = TILE_DESTINATION;
    tile32 = TILE_DESTINATION;

    if (pScrn->virtualX > 1024)
      ulYRange = 0x40000000;
    else
      ulYRange = 0x20000000;


    /* following is the enable case */
             
    /* SR01:turn off screen */
    OUTREG8 (SEQ_ADDRESS_REG,0x01);
    byte = INREG8(SEQ_DATA_REG) | 0x20;
    OUTREG8(SEQ_DATA_REG,byte);


    /* MM81C0 and 81B0 are used to control primary stream. */
    OUTREG32(PRI_STREAM_FBUF_ADDR0, pScrn->fbOffset);
    OUTREG32(PRI_STREAM2_FBUF_ADDR0, pScrn->fbOffset);


    /*
     *  Program Primary Stream Stride Register.
     *
     *  Tell engine if tiling on or off, set primary stream stride, and
     *  if tiling, set tiling bits/pixel and primary stream tile offset.
     *  Note that tile offset (bits 16 - 29) must be scanline width in
     *  bytes/128bytespertile * 256 Qwords/tile.  This is equivalent to
     *  lDelta * 2.  Remember that if tiling, lDelta is screenwidth in
     *  bytes padded up to an even number of tilewidths.
     */
    if (!psav->bTiled) {
        OUTREG32(PRI_STREAM_STRIDE,
		 ((psav->lDelta << 4) & 0x7ff0));
        OUTREG32(PRI_STREAM2_STRIDE,
                 ((psav->lDelta << 4) & 0x7ff0));
    } else {
        OUTREG32(PRI_STREAM_STRIDE,
		 (0x80000000 |((psav->lDelta << 4) & 0x7ff0)));
        OUTREG32(PRI_STREAM2_STRIDE,
                 (0x80000000 |((psav->lDelta << 4) & 0x7ff0)));
    }
    
    /*
     * CR67_3:
     *  = 1  stream processor MMIO address and stride register
     *       are used to control the primary stream
     *  = 0  standard VGA address and stride registers
     *       are used to control the primary streams
     */

    OUTREG8(CRT_ADDRESS_REG,0x67); 
    byte =  INREG8(CRT_DATA_REG) | 0x08;
    OUTREG8(CRT_DATA_REG,byte);

    
    OUTREG32(0x8128, 0xFFFFFFFFL);
    OUTREG32(0x812C, 0xFFFFFFFFL);
    
    /* bit 28:block write disable */
    OUTREG32(S3_GLB_BD_HIGH, bci_enable | S3_BD64 | 0x10000000); 
    
    /* CR50, bit 7,6,0 = 111, Use GBD.*/
    OUTREG8(CRT_ADDRESS_REG,0x50);
    byte = INREG8(CRT_DATA_REG) | 0xC1;
    OUTREG8(CRT_DATA_REG, byte);

    /* CR73 bit 5 = 0 block write disable */
    OUTREG8(CRT_ADDRESS_REG,0x73);
    byte = INREG8(CRT_DATA_REG) & ~0x20;
    OUTREG8(CRT_DATA_REG, byte);

    if (!psav->bTiled) {
        /*
         *  Do not enable block_write even for non-tiling modes, because
         *  the driver cannot determine if the memory type is the certain
         *  type of SGRAM for which block_write can be used.
         */
        psav->GlobalBD.bd1.HighPart.ResBWTile = TILE_FORMAT_LINEAR;/* linear */
        
    }
    else if (pScrn->bitsPerPixel == 16) {
        psav->GlobalBD.bd1.HighPart.ResBWTile = tile16;/* tile format destination */
        ulTmp =  (((pScrn->virtualX + 0x3f) & 0x0000ffc0) >> 6) << 23;
        OUTREG32(TILED_SURFACE_REGISTER_0,ulTmp | TILED_SURF_BPP16_2000 | ulYRange);
        ulTmp |= (TILED_SURF_BPP16_2000 | ulYRange);
        OUTREG32(PRI_STREAM_STRIDE, ((ulTmp >> 19) & 0x03f0) | 0x80000000);
        OUTREG32(PRI_STREAM2_STRIDE, ((ulTmp >> 19) & 0x03f0) | 0x80000000);
    }
    else if (pScrn->bitsPerPixel == 32) {
        psav->GlobalBD.bd1.HighPart.ResBWTile = tile32;/* tile format destination */        
        ulTmp =  (((pScrn->virtualX + 0x1f) & 0x0000ffe0) >> 5) << 23;        
        OUTREG32(TILED_SURFACE_REGISTER_0,ulTmp | TILED_SURF_BPP32_2000 | ulYRange);
        ulTmp |= (TILED_SURF_BPP32_2000 | ulYRange);
        OUTREG32(PRI_STREAM_STRIDE, ((ulTmp >> 19) & 0x03f0) | 0x80000000);
        OUTREG32(PRI_STREAM2_STRIDE, ((ulTmp >> 19) & 0x03f0) | 0x80000000);
    }
    
    psav->GlobalBD.bd1.HighPart.ResBWTile |= 0x10;/* disable block write */
    /* HW uses width */
    psav->GlobalBD.bd1.HighPart.Stride = (unsigned short)(psav->lDelta / (pScrn->bitsPerPixel >> 3));
    psav->GlobalBD.bd1.HighPart.Bpp = (unsigned char) (pScrn->bitsPerPixel);
    psav->GlobalBD.bd1.Offset = pScrn->fbOffset;    

    /*
     * CR31, bit 0 = 0, Disable address offset bits(CR6A_6-0).
     *       bit 0 = 1, Enable 8 Mbytes of display memory thru 64K window
     *                  at A000:0.
     */
    OUTREG8(CRT_ADDRESS_REG,MEMORY_CONFIG_REG);
    byte = INREG8(CRT_DATA_REG) & (~(ENABLE_CPUA_BASE_A0000));
    OUTREG8(CRT_DATA_REG,byte);
    
    /* program the GBD and SBDs */
    OUTREG32(S3_GLB_BD_LOW,psav->GlobalBD.bd2.LoPart );
    OUTREG32(S3_GLB_BD_HIGH,(psav->GlobalBD.bd2.HiPart 
			     | bci_enable | S3_LITTLE_ENDIAN | S3_BD64));
    OUTREG32(S3_PRI_BD_LOW,psav->GlobalBD.bd2.LoPart);
    OUTREG32(S3_PRI_BD_HIGH,psav->GlobalBD.bd2.HiPart);
    OUTREG32(S3_SEC_BD_LOW,psav->GlobalBD.bd2.LoPart);
    OUTREG32(S3_SEC_BD_HIGH,psav->GlobalBD.bd2.HiPart);

    /* turn on screen */
    OUTREG8(SEQ_ADDRESS_REG,0x01);
    byte = INREG8(SEQ_DATA_REG) & ~0x20;
    OUTREG8(SEQ_DATA_REG,byte);
}

#if 0
static
void SavageRestoreAccelState(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);

    psav->WaitIdleEmpty(psav);

    return;
}
#endif

/* Acceleration init function, sets up pointers to our accelerated functions */

Bool 
SavageInitAccel(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    SavagePtr psav = SAVPTR(pScrn);

#ifdef SAVAGEDRI
    if (psav->directRenderingEnabled) {
        SAVAGEDRIServerPrivatePtr pSAVAGEDRIServer = psav->DRIServerInfo;
        int cpp = pScrn->bitsPerPixel / 8;
        int widthBytes = psav->lDelta;
        int bufferSize = ((pScrn->virtualY * widthBytes + SAVAGE_BUFFER_ALIGN)
                          & ~SAVAGE_BUFFER_ALIGN);
        int tiledWidth, tiledwidthBytes,tiledBufferSize;

        pSAVAGEDRIServer->frontbufferSize = bufferSize;
        tiledwidthBytes = psav->lDelta;
	tiledWidth = tiledwidthBytes / cpp;
        
        if (cpp == 2) {
            tiledBufferSize = ((pScrn->virtualX+63)/64)*((pScrn->virtualY+15)/16)
                *2048;
        } else {
            tiledBufferSize = ((pScrn->virtualX+31)/32)*((pScrn->virtualY+15)/16)
                *2048;
        }
        /*set Depth buffer to 32bpp*/
        /*tiledwidthBytes_Z = ((pScrn->virtualX + 31)& ~0x0000001F)*4;
          tiledBufferSize_Z = ((pScrn->virtualX+31)/32)*((pScrn->virtualY+15)/16)
          *2048;*/

        pSAVAGEDRIServer->backbufferSize = tiledBufferSize;
        /*pSAVAGEDRIServer->depthbufferSize = tiledBufferSize_Z;*/
        pSAVAGEDRIServer->depthbufferSize = tiledBufferSize;

        xf86DrvMsg(pScrn->scrnIndex,X_INFO,
                   "virtualX:%d,virtualY:%d\n",
                   pScrn->virtualX,pScrn->virtualY);
        xf86DrvMsg( pScrn->scrnIndex, X_INFO,
                    "bpp:%d,tiledwidthBytes:%d,tiledBufferSize:%d \n",
                    pScrn->bitsPerPixel,
                    tiledwidthBytes,tiledBufferSize);

        xf86DrvMsg( pScrn->scrnIndex, X_INFO,
                    "bpp:%d,widthBytes:%d,BufferSize:%d \n",
                    pScrn->bitsPerPixel,
                    widthBytes,bufferSize);

        pSAVAGEDRIServer->frontOffset = pScrn->fbOffset; /* 0 */
        pSAVAGEDRIServer->frontPitch = widthBytes;

        /* Try for front, back, depth, and two framebuffers worth of
         * pixmap cache.  Should be enough for a fullscreen background
         * image plus some leftovers.
         */
        /*     pSAVAGEDRIServer->textureSize = psav->videoRambytes -
               tiledBufferSize -
               tiledBufferSize_Z -
               -0x602000;*/
        pSAVAGEDRIServer->textureSize = psav->videoRambytes -
            4096 - /* hw cursor*/
            psav->cobSize - /*COB*/
            bufferSize-
            tiledBufferSize -
            tiledBufferSize -
            0x200000;

        xf86DrvMsg( pScrn->scrnIndex, X_INFO,
                    "videoRambytes:0x%08x \n",
                    psav->videoRambytes);

        xf86DrvMsg( pScrn->scrnIndex, X_INFO,
                    "textureSize:0x%08x \n",
                    pSAVAGEDRIServer->textureSize);

        /* If that gives us less than half the available memory, let's
         * be greedy and grab some more.  Sorry, I care more about 3D
         * performance than playing nicely, and you'll get around a full
         * framebuffer's worth of pixmap cache anyway.
         */
#if 0
        if ( pSAVAGEDRIServer->textureSize < (int)psav->FbMapSize / 2 ) {
            pSAVAGEDRIServer->textureSize = psav->FbMapSize - 4 * bufferSize;
        }
#endif
        /* Check to see if there is more room available after the maximum
         * scanline for textures.
         */
#if 0
        if ( (int)psav->FbMapSize - maxlines * widthBytes - bufferSize * 2
             > pSAVAGEDRIServer->textureSize ) {
            pSAVAGEDRIServer->textureSize = (psav->FbMapSize -
                                             maxlines * widthBytes -
                                             bufferSize * 2);
        }
#endif
        /* Set a minimum usable local texture heap size.  This will fit
         * two 256x256x32bpp textures.
         */
        if ( pSAVAGEDRIServer->textureSize < 512 * 1024 ) {
            pSAVAGEDRIServer->textureOffset = 0;
            pSAVAGEDRIServer->textureSize = 0;
        }

        xf86DrvMsg( pScrn->scrnIndex, X_INFO,
                    "textureSize:0x%08x \n",
                    pSAVAGEDRIServer->textureSize);

        /* Reserve space for textures */
        /*       if (pSAVAGEDRIServer->textureSize)*/
        pSAVAGEDRIServer->textureOffset = (psav->videoRambytes -
                                           4096 - /* hw cursor*/
                                           psav->cobSize - /*COB*/
                                           pSAVAGEDRIServer->textureSize) & ~SAVAGE_BUFFER_ALIGN;

        xf86DrvMsg( pScrn->scrnIndex, X_INFO,
                    "textureOffset:0x%08x \n",
                    pSAVAGEDRIServer->textureOffset);

        /* Reserve space for the shared depth buffer */
        /*pSAVAGEDRIServer->depthOffset = (pSAVAGEDRIServer->textureOffset -
          tiledBufferSize_Z + SAVAGE_BUFFER_ALIGN) &  ~SAVAGE_BUFFER_ALIGN;
        */
        pSAVAGEDRIServer->depthOffset = (pSAVAGEDRIServer->textureOffset -
                                         tiledBufferSize) & ~SAVAGE_BUFFER_ALIGN;
        /*pSAVAGEDRIServer->depthPitch = tiledwidthBytes_Z;*/
        pSAVAGEDRIServer->depthPitch = tiledwidthBytes;

        xf86DrvMsg( pScrn->scrnIndex, X_INFO,
                    "depthOffset:0x%08x,depthPitch:%d\n",
                    pSAVAGEDRIServer->depthOffset,pSAVAGEDRIServer->depthPitch);

        /* Reserve space for the shared back buffer */
        pSAVAGEDRIServer->backOffset = (pSAVAGEDRIServer->depthOffset -
                                        tiledBufferSize ) & ~SAVAGE_BUFFER_ALIGN;

        pSAVAGEDRIServer->backPitch = tiledwidthBytes;

        xf86DrvMsg( pScrn->scrnIndex, X_INFO,
                    "backOffset:0x%08x,backPitch:%d\n",
                    pSAVAGEDRIServer->backOffset,pSAVAGEDRIServer->backPitch);

	/* Compute bitmap descriptors for front, back and depth buffers */
	if ((psav->Chipset == S3_TWISTER)
	    || (psav->Chipset == S3_PROSAVAGE)
	    || (psav->Chipset == S3_PROSAVAGEDDR)
	    || (psav->Chipset == S3_SUPERSAVAGE)) { 
	    pSAVAGEDRIServer->frontBitmapDesc =
		BCI_BD_BW_DISABLE | /* block write disabled */
		(1<<24) | /* destination tile format */
		(pScrn->bitsPerPixel<<16) | /* bpp */
		tiledWidth; /* stride */
	    pSAVAGEDRIServer->backBitmapDesc =
		BCI_BD_BW_DISABLE |
		(1<<24) |
		(pScrn->bitsPerPixel<<16) |
		tiledWidth;
	    pSAVAGEDRIServer->depthBitmapDesc =
		BCI_BD_BW_DISABLE |
		(1<<24) |
		(pScrn->bitsPerPixel<<16) | /* FIXME: allow zpp != cpp */
		tiledWidth;
	} else {
	    pSAVAGEDRIServer->frontBitmapDesc =
		BCI_BD_BW_DISABLE | /* block write disabled */
		(cpp==2 ? BCI_BD_TILE_16:BCI_BD_TILE_32) | /*16/32 bpp tile format */
		(pScrn->bitsPerPixel<<16) | /* bpp */
		tiledWidth; /* stride */
	    pSAVAGEDRIServer->backBitmapDesc =
		BCI_BD_BW_DISABLE |
		(cpp==2 ? BCI_BD_TILE_16:BCI_BD_TILE_32) |
		(pScrn->bitsPerPixel<<16) |
		tiledWidth;
	    pSAVAGEDRIServer->depthBitmapDesc =
		BCI_BD_BW_DISABLE |
		(cpp==2 ? BCI_BD_TILE_16:BCI_BD_TILE_32) |
		(pScrn->bitsPerPixel<<16) | /* FIXME: allow zpp != cpp */
		tiledWidth;
	}

        /*scanlines = pSAVAGEDRIServer->backOffset / widthBytes - 1;*/
        /*if ( scanlines > maxlines ) scanlines = maxlines;*/
        /* CR47983, XvMC do not work on system with frame buffer less than 32MB.
         * VBE reports frame buffer size a little less than 16MB, this makes the condition
         *   truns out FALSE.
         * Now just reduce the level to 14.5MB, things should be OK, while the hwmc frame buffer layout
         *    caculation need more understanding and should be fixed.
         */
        /*if total memory is less than 16M, there is no HWMC support */
        if((psav->videoRambytes < /*16*/(14*1024+512)*1024L) || psav->bDisableXvMC)
        {
            psav->hwmcOffset = 0;
            psav->hwmcSize = 0;
        }
        else
        {
            psav->hwmcSize = (10*1024+512)*1024;  /* HWMC needs 10MB FB */
            psav->hwmcOffset = (psav->videoRambytes - 0x2000 - psav->hwmcSize) & 
                ~SAVAGE_BUFFER_ALIGN; 
            if (psav->hwmcOffset < bufferSize) {
                /* If hwmc buffer will lay in on-screen buffer. */
                psav->hwmcSize = 0;
                psav->hwmcOffset = 0;
            }
        }

        /* CR48438: Title: "Lots of garbage appear on the background when 
         *  drag the DVD player XINE window at 1024x768 or higher mode."
         * hwmc used xserver's memory, now xserver will get less memory.
         * Both 3D and hwmc's memory usage are considered now.
         */  
#if 0
        if (pSAVAGEDRIServer->backOffset < psav->hwmcOffset )
            psav->cyMemory = pSAVAGEDRIServer->backOffset / widthBytes - 1;
        else
            psav->cyMemory = psav->hwmcOffset / widthBytes -1;
#endif

        psav->cyMemory = pSAVAGEDRIServer->backOffset / widthBytes - 1;
        if (psav->cyMemory > 0x7FFF) {
            psav->cyMemory = 0x7FFF;
        }

	psav->EXAendfb = pSAVAGEDRIServer->backOffset & ~SAVAGE_BUFFER_ALIGN;

        xf86DrvMsg( pScrn->scrnIndex, X_INFO,
                    "Reserved back buffer at offset 0x%x\n",
                    pSAVAGEDRIServer->backOffset );
        xf86DrvMsg( pScrn->scrnIndex, X_INFO,
                    "Reserved depth buffer at offset 0x%x\n",
                    pSAVAGEDRIServer->depthOffset );
        xf86DrvMsg( pScrn->scrnIndex, X_INFO,
                    "Reserved %d kb for textures at offset 0x%x\n",
                    pSAVAGEDRIServer->textureSize/1024,
                    pSAVAGEDRIServer->textureOffset );
    }
    else
#endif
    {

        /*
         * why this code? because BoxRec members are  short int
         * if cyMemory is bigger than 0x7fff,then it will overflow
         */
        if (psav->cyMemory > 0x7FFF) {
            psav->cyMemory = 0x7FFF;
        }

	if (psav->IsPrimary) {
	    psav->EXAendfb = psav->videoRambytes -
            		     4096 - /* hw cursor*/
			     0x200000;
	} else {
	    psav->EXAendfb = psav->videoRambytes -
            		     4096 - /* hw cursor*/
            		     psav->cobSize - /*COB*/
			     0x200000;
	}
            
    }

    if (psav->useEXA)
	return SavageEXAInit(pScreen);
    else
    	return SavageXAAInit(pScreen);
}

int SavageGetCopyROP(int rop) {

    int ALUCopyROP[16] =
    {
       0x00, /*ROP_0 GXclear */
       0x88, /*ROP_DSa GXand */
       0x44, /*ROP_SDna GXandReverse */
       0xCC, /*ROP_S GXcopy */
       0x22, /*ROP_DSna GXandInverted */
       0xAA, /*ROP_D GXnoop */
       0x66, /*ROP_DSx GXxor */
       0xEE, /*ROP_DSo GXor */
       0x11, /*ROP_DSon GXnor */
       0x99, /*ROP_DSxn GXequiv */
       0x55, /*ROP_Dn GXinvert*/
       0xDD, /*ROP_SDno GXorReverse */
       0x33, /*ROP_Sn GXcopyInverted */
       0xBB, /*ROP_DSno GXorInverted */
       0x77, /*ROP_DSan GXnand */
       0xFF, /*ROP_1 GXset */
    };

    return (ALUCopyROP[rop]);

}

/* Routines for debugging. */


unsigned long
writedw( unsigned long addr, unsigned long value )
{
    SavagePtr psav = SAVPTR(gpScrn);
    OUTREG( addr, value );
    return INREG( addr );
}

unsigned long
readdw( unsigned long addr )
{
    SavagePtr psav = SAVPTR(gpScrn);
    return INREG( addr );
}

unsigned long
readfb( unsigned long addr )
{
    SavagePtr psav = SAVPTR(gpScrn);
    char * videobuffer = (char *) psav->FBBase;
    return *(volatile unsigned long*)(videobuffer + (addr & ~3) );
}

unsigned long
writefb( unsigned long addr, unsigned long value )
{
    SavagePtr psav = SAVPTR(gpScrn);
    char * videobuffer = (char *) psav->FBBase;
    *(unsigned long*)(videobuffer + (addr & ~3)) = value;
    return *(volatile unsigned long*)(videobuffer + (addr & ~3) );
}

void
writescan( unsigned long scan, unsigned long color )
{
    SavagePtr psav = SAVPTR(gpScrn);
    int i;
    char * videobuffer = (char *)psav->FBBase;
    videobuffer += scan * gpScrn->displayWidth * gpScrn->bitsPerPixel >> 3;
    for( i = gpScrn->displayWidth; --i; ) {
	switch( gpScrn->bitsPerPixel ) {
	    case 8: 
		*videobuffer++ = color; 
		break;
	    case 16: 
		*(CARD16 *)videobuffer = color;
		videobuffer += 2;
		break;
	    case 32:
		*(CARD32 *)videobuffer = color;
		videobuffer += 4;
		break;
	}
    }
}
