
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * if NO_OPTIMIZE is set, some optimizations are disabled.
 *
 * What it basically tries to do is minimize the amounts of writes to
 * accelerator registers, since these are the ones that slow down small
 * operations a lot.
 */
/* #define NO_OPTIMIZE */

/*
 * if ET6K_TRANSPARENCY is set, ScreentoScreenCopy operations (and pattern
 * fills) will support transparency. But then the planemask support has to
 * be dropped. The default here is to support planemasks, because all Tseng
 * chips can do this. Only the ET6000 supports a transparency compare. The
 * code could be easily changed to support transparency on the ET6000 and
 * planemasks on the others, but that's only useful when transparency is
 * more important than planemasks.
 */
#undef ET6K_TRANSPARENCY

#include "tseng.h"
#include "tseng_accel.h"

#ifdef HAVE_XAA_H
#include "miline.h"

/*
 * conversion from X ROPs to Microsoft ROPs.
 */

static int W32OpTable[] =
{
    0x00,			       /* Xclear             0 */
    0x88,			       /* Xand               src AND dst */
    0x44,			       /* XandReverse        src AND NOT dst */
    0xcc,			       /* Xcopy              src */
    0x22,			       /* XandInverted       NOT src AND dst */
    0xaa,			       /* Xnoop              dst */
    0x66,			       /* Xxor               src XOR dst */
    0xee,			       /* Xor                src OR dst */
    0x11,			       /* Xnor               NOT src AND NOT dst */
    0x99,			       /* Xequiv             NOT src XOR dst */
    0x55,			       /* Xinvert            NOT dst */
    0xdd,			       /* XorReverse         src OR NOT dst */
    0x33,			       /* XcopyInverted      NOT src */
    0xbb,			       /* XorInverted        NOT src OR dst */
    0x77,			       /* Xnand              NOT src OR NOT dst */
    0xff			       /* Xset               1 */
};

static int W32OpTable_planemask[] =
{
    0x0a,			       /* Xclear             0 */
    0x8a,			       /* Xand               src AND dst */
    0x4a,			       /* XandReverse        src AND NOT dst */
    0xca,			       /* Xcopy              src */
    0x2a,			       /* XandInverted       NOT src AND dst */
    0xaa,			       /* Xnoop              dst */
    0x6a,			       /* Xxor               src XOR dst */
    0xea,			       /* Xor                src OR dst */
    0x1a,			       /* Xnor               NOT src AND NOT dst */
    0x9a,			       /* Xequiv             NOT src XOR dst */
    0x5a,			       /* Xinvert            NOT dst */
    0xda,			       /* XorReverse         src OR NOT dst */
    0x3a,			       /* XcopyInverted      NOT src */
    0xba,			       /* XorInverted        NOT src OR dst */
    0x7a,			       /* Xnand              NOT src OR NOT dst */
    0xfa			       /* Xset               1 */
};

static int W32PatternOpTable[] =
{
    0x00,			       /* Xclear             0 */
    0xa0,			       /* Xand               pat AND dst */
    0x50,			       /* XandReverse        pat AND NOT dst */
    0xf0,			       /* Xcopy              pat */
    0x0a,			       /* XandInverted       NOT pat AND dst */
    0xaa,			       /* Xnoop              dst */
    0x5a,			       /* Xxor               pat XOR dst */
    0xfa,			       /* Xor                pat OR dst */
    0x05,			       /* Xnor               NOT pat AND NOT dst */
    0xa5,			       /* Xequiv             NOT pat XOR dst */
    0x55,			       /* Xinvert            NOT dst */
    0xf5,			       /* XorReverse         pat OR NOT dst */
    0x0f,			       /* XcopyInverted      NOT pat */
    0xaf,			       /* XorInverted        NOT pat OR dst */
    0x5f,			       /* Xnand              NOT pat OR NOT dst */
    0xff			       /* Xset               1 */
};



/**********************************************************************/

static void 
tseng_terminate_acl(TsengPtr pTseng)
{
    /* only terminate when needed */
/*  if (*(volatile unsigned char *)ACL_ACCELERATOR_STATUS & 0x06) */
    {
	ACL_SUSPEND_TERMINATE(0x00);
	/* suspend any running operation */
	ACL_SUSPEND_TERMINATE(0x01);
	WAIT_ACL;
	ACL_SUSPEND_TERMINATE(0x00);
	/* ... and now terminate it */
	ACL_SUSPEND_TERMINATE(0x10);
	WAIT_ACL;
	ACL_SUSPEND_TERMINATE(0x00);
    }
}

static void 
tseng_recover_timeout(TsengPtr pTseng)
{
    if (pTseng->ChipType == ET4000) {
	ErrorF("trying to unlock......................................\n");
	MMIO_OUT32(pTseng->tsengCPU2ACLBase,0,0L); /* try unlocking the bus when CPU-to-accel gets stuck */

        /* flush the accelerator pipeline */
	ACL_SUSPEND_TERMINATE(0x00);
	ACL_SUSPEND_TERMINATE(0x02);
	ACL_SUSPEND_TERMINATE(0x00);
    }
}

void 
tseng_init_acl(ScrnInfoPtr pScrn)
{
    TsengPtr pTseng = TsengPTR(pScrn);

    PDEBUG("	tseng_init_acl\n");
    /*
     * prepare some shortcuts for faster access to memory mapped registers
     */

    pTseng->scratchMemBase = pTseng->FbBase + pTseng->AccelColorBufferOffset;
    /* 
     * we won't be using tsengCPU2ACLBase in linear memory mode anyway, since
     * using the MMU apertures restricts the amount of useable video memory
     * to only 2MB, supposing we ONLY redirect MMU aperture 2 to the CPU.
     * (see data book W32p, page 207)
     */
    pTseng->tsengCPU2ACLBase = pTseng->FbBase + 0x200000;	/* MMU aperture 2 */

#ifdef DEBUG    
    ErrorF("MMioBase = 0x%x, scratchMemBase = 0x%x\n", pTseng->MMioBase, pTseng->scratchMemBase);
#endif

    /*
     * prepare the accelerator for some real work
     */

    tseng_terminate_acl(pTseng);

    ACL_INTERRUPT_STATUS(0xe);       /* clear interrupts */
    ACL_INTERRUPT_MASK(0x04);	       /* disable interrupts, but enable deadlock exit */
    ACL_INTERRUPT_STATUS(0x0);
    ACL_ACCELERATOR_STATUS_SET(0x0);

    if (pTseng->ChipType == ET6000) {
	ACL_STEPPING_INHIBIT(0x0);   /* Undefined at power-on, let all maps (Src, Dst, Mix, Pat) step */
	ACL_6K_CONFIG(0x00);	       /* maximum performance -- what did you think? */
	ACL_POWER_CONTROL(0x01);     /* conserve power when ACL is idle */
	ACL_MIX_CONTROL(0x33);
	ACL_TRANSFER_DISABLE(0x00);  /* Undefined at power-on, enable all transfers */
    } else {			       /* W32i/W32p */
  	ACL_RELOAD_CONTROL(0x0); 
	ACL_SYNC_ENABLE(0x1);	       /* | 0x2 = 0WS ACL read. Yields up to 10% faster operation for small blits */
	ACL_ROUTING_CONTROL(0x00);
    }

    /* Enable the W32p startup bit and set use an eight-bit pixel depth */
    ACL_NQ_X_POSITION(0);
    ACL_NQ_Y_POSITION(0);
    ACL_PIXEL_DEPTH((pScrn->bitsPerPixel - 8) << 1);
    /* writing destination address will start ACL */
    ACL_OPERATION_STATE(0x10);

    ACL_DESTINATION_Y_OFFSET(pScrn->displayWidth * pTseng->Bytesperpixel - 1);
    ACL_XY_DIRECTION(0);

    MMU_CONTROL(0x74);

    if (pTseng->ChipType == ET4000) {
	/*
	 * Since the w32p revs C and D don't have any memory mapped when the
	 * accelerator registers are used it is necessary to use the MMUs to
	 * provide a semblance of linear memory. Fortunately on these chips
	 * the MMU appertures are 1 megabyte each. So as long as we are
	 * willing to only use 3 megs of video memory we can have some
	 * acceleration. If we ever get the CPU-to-screen-color-expansion
	 * stuff working then we will NOT need to sacrifice the extra 1MB
	 * provided by MBP2, because we could do dynamic switching of the APT
	 * bit in the MMU control register.
	 *
	 * On W32p rev c and d MBP2 is hardwired to 0x200000 when linear
	 * memory mode is enabled. (On rev a it is programmable).
	 *
	 * W32p rev a and b have their first 2M mapped in the normal (non-MMU)
	 * way, and MMU0 and MMU1, each 512 kb wide, can be used to access
	 * another 1MB of memory. This totals to 3MB of mem. available in
	 * linear memory when the accelerator is enabled.
	 */
	if ((pTseng->ChipRev == REV_A) || (pTseng->ChipRev == REV_B)) {
	    MMIO_OUT32(pTseng->MMioBase, 0x00<<0, 0x200000L);
	    MMIO_OUT32(pTseng->MMioBase, 0x04<<0, 0x280000L);
	} else {		       /* rev C & D */
	    MMIO_OUT32(pTseng->MMioBase, 0x00<<0, 0x0L);
	    MMIO_OUT32 (pTseng->MMioBase, 0x04<<0, 0x100000L);
	}
    }
}

/*
 * ET4/6K acceleration interface -- color expansion primitives.
 *
 * Uses Harm Hanemaayer's generic acceleration interface (XAA).
 *
 * Author: Koen Gadeyne
 *
 * Much of the acceleration code is based on the XF86_W32 server code from
 * Glenn Lai.
 *
 *
 *     Color expansion capabilities of the Tseng chip families:
 *
 *     Chip     screen-to-screen   CPU-to-screen   Supported depths
 *
 *   ET4000W32/W32i   No               Yes             8bpp only
 *   ET4000W32p       Yes              Yes             8bpp only
 *   ET6000           Yes              No              8/16/24/32 bpp
 */
#define SET_FUNCTION_COLOREXPAND \
    if (pTseng->ChipType == ET6000) \
      ACL_MIX_CONTROL(0x32); \
    else \
      ACL_ROUTING_CONTROL(0x08);

#define SET_FUNCTION_COLOREXPAND_CPU \
    ACL_ROUTING_CONTROL(0x02);


static void
TsengSubsequentScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
    int x, int y, int w, int h, int skipleft)
{
    TsengPtr pTseng = TsengPTR(pScrn);

    if (pTseng->ChipType == ET4000) {
	/* the accelerator needs DWORD padding, and "w" is in PIXELS... */
	pTseng->acl_colexp_width_dwords = (MULBPP(pTseng, w) + 31) >> 5;
	pTseng->acl_colexp_width_bytes = (MULBPP(pTseng, w) + 7) >> 3;
    }

    pTseng->acl_ColorExpandDst = FBADDR(pTseng, x, y);
    pTseng->acl_skipleft = skipleft;

    wait_acl_queue(pTseng);

#if 0
    ACL_MIX_Y_OFFSET(w - 1);

    ErrorF(" W=%d", w);
#endif
    SET_XY(pTseng, w, 1);
}

static void
TsengSubsequentColorExpandScanline(ScrnInfoPtr pScrn,
    int bufno)
{
    TsengPtr pTseng = TsengPTR(pScrn);

    wait_acl_queue(pTseng);

    ACL_MIX_ADDRESS((pTseng->AccelColorExpandBufferOffsets[bufno] << 3) + pTseng->acl_skipleft);
    START_ACL(pTseng, pTseng->acl_ColorExpandDst);

    /* move to next scanline */
    pTseng->acl_ColorExpandDst += pTseng->line_width;

    /*
     * If not using triple-buffering, we need to wait for the queued
     * register set to be transferred to the working register set here,
     * because otherwise an e.g. double-buffering mechanism could overwrite
     * the buffer that's currently being worked with with new data too soon.
     *
     * WAIT_QUEUE; // not needed with triple-buffering
     */
}



/*
 * We use this intermediate CPU-to-Screen color expansion because the one
 * provided by XAA seems to lock up the accelerator engine.
 *
 * One of the main differences between the XAA approach and this one is that
 * transfers are done per byte. I'm not sure if that is needed though.
 */
static void
TsengSubsequentColorExpandScanline_8bpp(ScrnInfoPtr pScrn, int bufno)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    pointer dest = pTseng->tsengCPU2ACLBase;
    int i,j;
    CARD8 *bufptr;

    i = pTseng->acl_colexp_width_bytes;
    bufptr = (CARD8 *) (pTseng->XAAScanlineColorExpandBuffers[bufno]);

    wait_acl_queue(pTseng);
    START_ACL (pTseng, pTseng->acl_ColorExpandDst);

/*  *((LongP) (MMioBase + 0x08)) = (CARD32) pTseng->acl_ColorExpandDst;*/
/*  MMIO_OUT32(tsengCPU2ACLBase,0, (CARD32)pTseng->acl_ColorExpandDst); */
    j = 0;
    /* Copy scanline data to accelerator MMU aperture byte by byte */
    while (i--) {		       /* FIXME: we need to take care of PCI bursting and MMU overflow here! */
	MMIO_OUT8(dest,j++, *bufptr++);
    }

    /* move to next scanline */
    pTseng->acl_ColorExpandDst += pTseng->line_width;
}

/*
 * This function does direct memory-to-CPU bit doubling for color-expansion
 * at 16bpp on W32 chips. They can only do 8bpp color expansion, so we have
 * to expand the incoming data to 2bpp first.
 */
static void
TsengSubsequentColorExpandScanline_16bpp(ScrnInfoPtr pScrn, int bufno)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    pointer dest = pTseng->tsengCPU2ACLBase;
    int i,j;
    CARD8 *bufptr;
    register CARD32 bits16;
    
    i = pTseng->acl_colexp_width_dwords * 2;
    bufptr = (CARD8 *) (pTseng->XAAScanlineColorExpandBuffers[bufno]);
    
    wait_acl_queue(pTseng);
    START_ACL(pTseng, pTseng->acl_ColorExpandDst);

    j = 0;
    while (i--) {
	bits16 = pTseng->ColExpLUT[*bufptr++];
	MMIO_OUT8(dest,j++,bits16 & 0xFF);
	MMIO_OUT8(dest,j++,(bits16 >> 8) & 0xFF);
    }

    /* move to next scanline */
    pTseng->acl_ColorExpandDst += pTseng->line_width;
}

/*
 * This function does direct memory-to-CPU bit doubling for color-expansion
 * at 24bpp on W32 chips. They can only do 8bpp color expansion, so we have
 * to expand the incoming data to 3bpp first.
 */
static void
TsengSubsequentColorExpandScanline_24bpp(ScrnInfoPtr pScrn, int bufno)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    pointer dest = pTseng->tsengCPU2ACLBase;
    int i, k, j = -1;
    CARD8 *bufptr;
    register CARD32 bits24;

    i = pTseng->acl_colexp_width_dwords * 4;
    bufptr = (CARD8 *) (pTseng->XAAScanlineColorExpandBuffers[bufno]);

    wait_acl_queue(pTseng);
    START_ACL(pTseng, pTseng->acl_ColorExpandDst);

    /* take 8 input bits, expand to 3 output bytes */
    bits24 = pTseng->ColExpLUT[*bufptr++];
    k = 0;
    while (i--) {
	if ((j++) == 2) {	       /* "i % 3" operation is much to expensive */
	    j = 0;
	    bits24 = pTseng->ColExpLUT[*bufptr++];
	}
	MMIO_OUT8(dest,k++,bits24 & 0xFF);
	bits24 >>= 8;
    }

    /* move to next scanline */
    pTseng->acl_ColorExpandDst += pTseng->line_width;
}

/*
 * This function does direct memory-to-CPU bit doubling for color-expansion
 * at 32bpp on W32 chips. They can only do 8bpp color expansion, so we have
 * to expand the incoming data to 4bpp first.
 */
static void
TsengSubsequentColorExpandScanline_32bpp(ScrnInfoPtr pScrn, int bufno)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    pointer dest = pTseng->tsengCPU2ACLBase;
    int i,j;
    CARD8 *bufptr;
    register CARD32 bits32;

    i = pTseng->acl_colexp_width_dwords;
   /* amount of blocks of 8 bits to expand to 32 bits (=1 DWORD) */
    bufptr = (CARD8 *) (pTseng->XAAScanlineColorExpandBuffers[bufno]);

    wait_acl_queue(pTseng);
    START_ACL(pTseng, pTseng->acl_ColorExpandDst);

    j = 0;
    while (i--) {
	bits32 = pTseng->ColExpLUT[*bufptr++];
	MMIO_OUT8(dest,j++,bits32 & 0xFF);
	MMIO_OUT8(dest,j++,(bits32 >> 8) & 0xFF);
	MMIO_OUT8(dest,j++,(bits32 >> 16) & 0xFF);
	MMIO_OUT8(dest,j++,(bits32 >> 24) & 0xFF);
    }

    /* move to next scanline */
    pTseng->acl_ColorExpandDst += pTseng->line_width;
}

/*
 * CPU-to-Screen color expansion.
 *   This is for ET4000 only (The ET6000 cannot do this)
 */
static void
TsengSetupForCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
    int fg, int bg, int rop, unsigned int planemask)
{
    TsengPtr pTseng = TsengPTR(pScrn);

/*  ErrorF("X"); */

    PINGPONG(pTseng);

    wait_acl_queue(pTseng);

    SET_FG_ROP(rop);
    SET_BG_ROP_TR(rop, bg);

    SET_XYDIR(0);

    SET_FG_BG_COLOR(pTseng, fg, bg);

    SET_FUNCTION_COLOREXPAND_CPU;

    /* assure correct alignment of MIX address (ACL needs same alignment here as in MMU aperture) */
    ACL_MIX_ADDRESS(0);
}

#ifdef TSENG_CPU_TO_SCREEN_COLOREXPAND
/*
 * TsengSubsequentCPUToScreenColorExpand() is potentially dangerous:
 *   Not writing enough data to the MMU aperture for CPU-to-screen color
 *   expansion will eventually cause a system deadlock!
 *
 * Note that CPUToScreenColorExpand operations _always_ require a
 * WAIT_INTERFACE before starting a new operation (this is empyrical,
 * though)
 */
static void
TsengSubsequentCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
    int x, int y, int w, int h, int skipleft)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    int destaddr = FBADDR(pTseng, x, y);

    /* ErrorF(" %dx%d|%d ",w,h,skipleft); */
    if (skipleft)
	ErrorF("Can't do: Skipleft = %d\n", skipleft);

/*  wait_acl_queue(); */
    ErrorF("=========WAIT     FIXME!\n");
    WAIT_INTERFACE;

    ACL_MIX_Y_OFFSET(w - 1);
    SET_XY(pTseng, w, h);
    START_ACL(pTseng, destaddr);
}
#endif

static void
TsengSetupForScreenToScreenColorExpandFill(ScrnInfoPtr pScrn,
    int fg, int bg, int rop, unsigned int planemask)
{
    TsengPtr pTseng = TsengPTR(pScrn);

/*  ErrorF("SSC "); */

    PINGPONG(pTseng);

    wait_acl_queue(pTseng);

    SET_FG_ROP(rop);
    SET_BG_ROP_TR(rop, bg);

    SET_FG_BG_COLOR(pTseng, fg, bg);

    SET_FUNCTION_COLOREXPAND;

    SET_XYDIR(0);
}

static void
TsengSubsequentScreenToScreenColorExpandFill(ScrnInfoPtr pScrn,
    int x, int y, int w, int h, int srcx, int srcy, int skipleft)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    int destaddr = FBADDR(pTseng, x, y);

/*    int srcaddr = FBADDR(pTseng, srcx, srcy); */

    wait_acl_queue(pTseng);

    SET_XY(pTseng, w, h);
    ACL_MIX_ADDRESS(		       /* MIX address is in BITS */
	(((srcy * pScrn->displayWidth) + srcx) * pScrn->bitsPerPixel) + skipleft);

    ACL_MIX_Y_OFFSET(pTseng->line_width << 3);

    START_ACL(pTseng, destaddr);
}

/*
 *
 */
static Bool
TsengXAAInit_Colexp(ScrnInfoPtr pScrn)
{
    int i, j, r;
    TsengPtr pTseng = TsengPTR(pScrn);
    XAAInfoRecPtr pXAAInfo = pTseng->AccelInfoRec;

    PDEBUG("	TsengXAAInit_Colexp\n");

#ifdef TODO
    if (OFLG_ISSET(OPTION_XAA_NO_COL_EXP, &vga256InfoRec.options))
	return;
#endif

    /* FIXME! disable accelerated color expansion for W32/W32i until it's fixed */
/*  if (Is_W32 || Is_W32i) return; */

    /*
     * Screen-to-screen color expansion.
     *
     * Scanline-screen-to-screen color expansion is slower than
     * CPU-to-screen color expansion.
     */

    pXAAInfo->ScreenToScreenColorExpandFillFlags =
	BIT_ORDER_IN_BYTE_LSBFIRST |
	SCANLINE_PAD_DWORD |
	LEFT_EDGE_CLIPPING |
	NO_PLANEMASK;

#if 1
    if ((pTseng->ChipType == ET6000) || (pScrn->bitsPerPixel == 8)) {
	pXAAInfo->SetupForScreenToScreenColorExpandFill =
	    TsengSetupForScreenToScreenColorExpandFill;
	pXAAInfo->SubsequentScreenToScreenColorExpandFill =
	    TsengSubsequentScreenToScreenColorExpandFill;
    }
#endif

    /*
     * Scanline CPU to screen color expansion for all W32 engines.
     *
     * real CPU-to-screen color expansion is extremely tricky, and only
     * works for 8bpp anyway.
     *
     * This also allows us to do 16, 24 and 32 bpp color expansion by first
     * doubling the bitmap pattern before color-expanding it, because W32s
     * can only do 8bpp color expansion.
     */

    pXAAInfo->ScanlineCPUToScreenColorExpandFillFlags =
	BIT_ORDER_IN_BYTE_LSBFIRST |
	SCANLINE_PAD_DWORD |
	NO_PLANEMASK;

    if (pTseng->ChipType == ET4000) {
	pTseng->XAAScanlineColorExpandBuffers[0] =
	    xnfalloc(((pScrn->virtualX + 31)/32) * 4 * pTseng->Bytesperpixel);
	if (pTseng->XAAScanlineColorExpandBuffers[0] == NULL) {
	    xf86Msg(X_ERROR, "Could not malloc color expansion scanline buffer.\n");
	    return FALSE;
	}
	pXAAInfo->NumScanlineColorExpandBuffers = 1;
	pXAAInfo->ScanlineColorExpandBuffers = pTseng->XAAScanlineColorExpandBuffers;

	pXAAInfo->SetupForScanlineCPUToScreenColorExpandFill =
	    TsengSetupForCPUToScreenColorExpandFill;

	pXAAInfo->SubsequentScanlineCPUToScreenColorExpandFill =
	    TsengSubsequentScanlineCPUToScreenColorExpandFill;

	switch (pScrn->bitsPerPixel) {
	case 8:
	    pXAAInfo->SubsequentColorExpandScanline =
		TsengSubsequentColorExpandScanline_8bpp;
	    break;
	case 15:
	case 16:
	    pXAAInfo->SubsequentColorExpandScanline =
		TsengSubsequentColorExpandScanline_16bpp;
	    break;
	case 24:
	    pXAAInfo->SubsequentColorExpandScanline =
		TsengSubsequentColorExpandScanline_24bpp;
	    break;
	case 32:
	    pXAAInfo->SubsequentColorExpandScanline =
		TsengSubsequentColorExpandScanline_32bpp;
	    break;
	}
	/* create color expansion LUT (used for >8bpp only) */
	pTseng->ColExpLUT = xnfalloc(sizeof(CARD32)*256);
	if (pTseng->ColExpLUT == NULL) {
	    xf86Msg(X_ERROR, "Could not malloc color expansion tables.\n");
	    return FALSE;
	}
	for (i = 0; i < 256; i++) {
	    r = 0;
	    for (j = 7; j >= 0; j--) {
		r <<= pTseng->Bytesperpixel;
		if ((i >> j) & 1)
		    r |= (1 << pTseng->Bytesperpixel) - 1;
	    }
	    pTseng->ColExpLUT[i] = r;
	    /* ErrorF("0x%08X, ",r ); if ((i%8)==7) ErrorF("\n"); */
	}
    } else {
	/*
	 * Triple-buffering is needed to account for double-buffering of Tseng
	 * acceleration registers.
	 */
	pXAAInfo->NumScanlineColorExpandBuffers = 3;
	pXAAInfo->ScanlineColorExpandBuffers =
	    pTseng->XAAColorExpandBuffers;
	pXAAInfo->SetupForScanlineCPUToScreenColorExpandFill =
	    TsengSetupForScreenToScreenColorExpandFill;
	pXAAInfo->SubsequentScanlineCPUToScreenColorExpandFill =
	    TsengSubsequentScanlineCPUToScreenColorExpandFill;
	pXAAInfo->SubsequentColorExpandScanline =
	    TsengSubsequentColorExpandScanline;

	/* calculate memory addresses from video memory offsets */
	for (i = 0; i < pXAAInfo->NumScanlineColorExpandBuffers; i++) {
	    pTseng->XAAColorExpandBuffers[i] =
		pTseng->FbBase + pTseng->AccelColorExpandBufferOffsets[i];
	}

	pXAAInfo->ScanlineColorExpandBuffers = pTseng->XAAColorExpandBuffers;
    }

#ifdef TSENG_CPU_TO_SCREEN_COLOREXPAND
    /*
     * CPU-to-screen color expansion doesn't seem to be reliable yet. The
     * W32 needs the correct amount of data sent to it in this mode, or it
     * hangs the machine until is does (?). Currently, the init code in this
     * file or the XAA code that uses this does something wrong, so that
     * occasionally we get accelerator timeouts, and after a few, complete
     * system hangs.
     *
     * The W32 engine requires SCANLINE_NO_PAD, but that doesn't seem to
     * work very well (accelerator hangs).
     *
     * What works is this: tell XAA that we have SCANLINE_PAD_DWORD, and then
     * add the following code in TsengSubsequentCPUToScreenColorExpand():
     *     w = (w + 31) & ~31; this code rounds the width up to the nearest
     * multiple of 32, and together with SCANLINE_PAD_DWORD, this makes
     * CPU-to-screen color expansion work. Of course, the display isn't
     * correct (4 chars are "blanked out" when only one is written, for
     * example). But this shows that the principle works. But the code
     * doesn't...
     *
     * The same thing goes for PAD_BYTE: this also works (with the same
     * problems as SCANLINE_PAD_DWORD, although less prominent)
     */

    pXAAInfo->CPUToScreenColorExpandFillFlags =
	BIT_ORDER_IN_BYTE_LSBFIRST |
	SCANLINE_PAD_DWORD |   /* no other choice */
	CPU_TRANSFER_PAD_DWORD |
	NO_PLANEMASK;

    if (Is_W32_any && (pScrn->bitsPerPixel == 8)) {
	pXAAInfo->SetupForCPUToScreenColorExpandFill =
	    TsengSetupForCPUToScreenColorExpandFill;
	pXAAInfo->SubsequentCPUToScreenColorExpandFill =
	    TsengSubsequentCPUToScreenColorExpandFill;

	/* we'll be using MMU aperture 2 */
	pXAAInfo->ColorExpandBase = (CARD8 *)pTseng->tsengCPU2ACLBase;
	/* ErrorF("tsengCPU2ACLBase = 0x%x\n", pTseng->tsengCPU2ACLBase); */
	/* aperture size is 8kb in banked mode. Larger in linear mode, but 8kb is enough */
	pXAAInfo->ColorExpandRange = 8192;
    }
#endif
    return TRUE;
}

/*
 * ET4/6K acceleration interface.
 *
 * Uses Harm Hanemaayer's generic acceleration interface (XAA).
 *
 * Author: Koen Gadeyne
 *
 * Much of the acceleration code is based on the XF86_W32 server code from
 * Glenn Lai.
 *
 */

/*
 * This is the implementation of the Sync() function.
 *
 * To avoid pipeline/cache/buffer flushing in the PCI subsystem and the VGA
 * controller, we might replace this read-intensive code with a dummy
 * accelerator operation that causes a hardware-blocking (wait-states) until
 * the running operation is done.
 */
static void
TsengSync(ScrnInfoPtr pScrn)
{
    TsengPtr pTseng = TsengPTR(pScrn);

    WAIT_ACL;
}

/*
 * This is the implementation of the SetupForSolidFill function
 * that sets up the coprocessor for a subsequent batch for solid
 * rectangle fills.
 */
static void
TsengSetupForSolidFill(ScrnInfoPtr pScrn,
    int color, int rop, unsigned int planemask)
{
    TsengPtr pTseng = TsengPTR(pScrn);

    /*
     * all registers are queued in the Tseng chips, except of course for the
     * stuff we want to store in off-screen memory. So we have to use a
     * ping-pong method for those if we want to avoid having to wait for the
     * accelerator when we want to write to these.
     */

/*    ErrorF("S"); */

    PINGPONG(pTseng);

    wait_acl_queue(pTseng);

    /*
     * planemask emulation uses a modified "standard" FG ROP (see ET6000
     * data book p 66 or W32p databook p 37: "Bit masking"). We only enable
     * the planemask emulation when the planemask is not a no-op, because
     * blitting speed would suffer.
     */

    if ((planemask & pTseng->planemask_mask) != pTseng->planemask_mask) {
	SET_FG_ROP_PLANEMASK(rop);
	SET_BG_COLOR(pTseng, planemask);
    } else {
	SET_FG_ROP(rop);
    }
    SET_FG_COLOR(pTseng, color);

    SET_FUNCTION_BLT;
}

/*
 * This is the implementation of the SubsequentForSolidFillRect function
 * that sends commands to the coprocessor to fill a solid rectangle of
 * the specified location and size, with the parameters from the SetUp
 * call.
 *
 * Splitting it up between ET4000 and ET6000 avoids lots of chipset type
 * comparisons.
 */
static void
TsengW32pSubsequentSolidFillRect(ScrnInfoPtr pScrn,
    int x, int y, int w, int h)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    int destaddr = FBADDR(pTseng, x, y);

    wait_acl_queue(pTseng);

    /* 
     * Restoring the ACL_SOURCE_ADDRESS here is needed as long as Bresenham
     * lines are enabled for >8bpp. Or until XAA allows us to render
     * horizontal lines using the same Bresenham code instead of re-routing
     * them to FillRectSolid. For XDECREASING lines, the SubsequentBresenham
     * code adjusts the ACL_SOURCE_ADDRESS to make sure XDECREASING lines
     * are drawn with the correct colors. But if a batch of subsequent
     * operations also holds a few horizontal lines, they will be routed to
     * here without calling the SetupFor... code again, and the
     * ACL_SOURCE_ADDRESS will be wrong.
     */
    ACL_SOURCE_ADDRESS(pTseng->AccelColorBufferOffset + pTseng->tsengFg);

    SET_XYDIR(0);   /* FIXME: not needed with separate setupforsolidline */

    SET_XY_4(pTseng, w, h);
    START_ACL(pTseng, destaddr);
}

static void
Tseng6KSubsequentSolidFillRect(ScrnInfoPtr pScrn,
    int x, int y, int w, int h)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    int destaddr = FBADDR(pTseng, x, y);

    wait_acl_queue(pTseng);

    /* see comment in TsengW32pSubsequentFillRectSolid */
    ACL_SOURCE_ADDRESS(pTseng->AccelColorBufferOffset + pTseng->tsengFg);

    /* if XYDIR is not reset here, drawing a hardware line in between
     * blitting, with the same ROP, color, etc will not cause a call to
     * SetupFor... (because linedrawing uses SetupForSolidFill() as its
     * Setup() function), and thus the direction register will have been
     * changed by the last LineDraw operation.
     */
    SET_XYDIR(0);

    SET_XY_6(pTseng, w, h);
    START_ACL_6(destaddr);
}

/*
 * This is the implementation of the SetupForScreenToScreenCopy function
 * that sets up the coprocessor for a subsequent batch of
 * screen-to-screen copies.
 */

static __inline__ void
Tseng_setup_screencopy(TsengPtr pTseng,
    int rop, unsigned int planemask,
    int trans_color, int blit_dir)
{
    wait_acl_queue(pTseng);

#ifdef ET6K_TRANSPARENCY
    if ((pTseng->ChipType == ET6000) && (trans_color != -1)) {
	SET_BG_COLOR(trans_color);
	SET_FUNCTION_BLT_TR;
    } else
	SET_FUNCTION_BLT;

    SET_FG_ROP(rop);
#else
    if ((planemask & pTseng->planemask_mask) != pTseng->planemask_mask) {
	SET_FG_ROP_PLANEMASK(rop);
	SET_BG_COLOR(pTseng, planemask);
    } else {
	SET_FG_ROP(rop);
    }
    SET_FUNCTION_BLT;
#endif
    SET_XYDIR(blit_dir);
}

static void
TsengSetupForScreenToScreenCopy(ScrnInfoPtr pScrn,
    int xdir, int ydir, int rop,
    unsigned int planemask, int trans_color)
{
    /*
     * xdir can be either 1 (left-to-right) or -1 (right-to-left).
     * ydir can be either 1 (top-to-bottom) or -1 (bottom-to-top).
     */

    TsengPtr pTseng = TsengPTR(pScrn);
    int blit_dir = 0;

/*    ErrorF("C%d ", trans_color); */

    pTseng->acl_blitxdir = xdir;
    pTseng->acl_blitydir = ydir;

    if (xdir == -1)
	blit_dir |= 0x1;
    if (ydir == -1)
	blit_dir |= 0x2;

    Tseng_setup_screencopy(pTseng, rop, planemask, trans_color, blit_dir);

    ACL_SOURCE_WRAP(0x77);	       /* no wrap */
    ACL_SOURCE_Y_OFFSET(pTseng->line_width - 1);
}

/*
 * This is the implementation of the SubsequentForScreenToScreenCopy
 * that sends commands to the coprocessor to perform a screen-to-screen
 * copy of the specified areas, with the parameters from the SetUp call.
 * In this sample implementation, the direction must be taken into
 * account when calculating the addresses (with coordinates, it might be
 * a little easier).
 *
 * Splitting up the SubsequentScreenToScreenCopy between ET4000 and ET6000
 * doesn't seem to improve speed for small blits (as it did with
 * SolidFillRect).
 */
static void
TsengSubsequentScreenToScreenCopy(ScrnInfoPtr pScrn,
    int x1, int y1, int x2, int y2,
    int w, int h)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    int srcaddr, destaddr;

    /*
     * Optimizing note: the pre-calc code below (i.e. until the first
     * register write) doesn't significantly affect performance. Removing it
     * all boosts small blits from 24.22 to 25.47 MB/sec. Don't waste time
     * on that. One less PCI bus write would boost us to 30.00 MB/sec, up
     * from 24.22. Waste time on _that_...
     */

    /* tseng chips want x-sizes in bytes, not pixels */
    x1 = MULBPP(pTseng, x1);
    x2 = MULBPP(pTseng, x2);

    /*
     * If the direction is "decreasing", the chip wants the addresses
     * to be at the other end, so we must be aware of that in our
     * calculations.
     */
    if (pTseng->acl_blitydir == -1) {
	srcaddr = (y1 + h - 1) * pTseng->line_width;
	destaddr = (y2 + h - 1) * pTseng->line_width;
    } else {
	srcaddr = y1 * pTseng->line_width;
	destaddr = y2 * pTseng->line_width;
    }
    if (pTseng->acl_blitxdir == -1) {
	/* Accelerator start address must point to first byte to be processed.
	 * Depending on the direction, this is the first or the last byte
	 * in the multi-byte pixel.
	 */
	int eol = MULBPP(pTseng, w);

	srcaddr += x1 + eol - 1;
	destaddr += x2 + eol - 1;
    } else {
	srcaddr += x1;
	destaddr += x2;
    }

    wait_acl_queue(pTseng);

    SET_XY(pTseng, w, h);
    ACL_SOURCE_ADDRESS(srcaddr);
    START_ACL(pTseng, destaddr);
}

#if 0
static int pat_src_addr;

static void
TsengSetupForColor8x8PatternFill(ScrnInfoPtr pScrn,
    int patx, int paty, int rop, unsigned int planemask, int trans_color)
{
    TsengPtr pTseng = TsengPTR(pScrn);

    pat_src_addr = FBADDR(pTseng, patx, paty);

    ErrorF("P");

    Tseng_setup_screencopy(pTseng, rop, planemask, trans_color, 0);

    switch (pTseng->Bytesperpixel) {
    case 1:
	ACL_SOURCE_WRAP(0x33);       /* 8x8 wrap */
	ACL_SOURCE_Y_OFFSET(8 - 1);
	break;
    case 2:
	ACL_SOURCE_WRAP(0x34);       /* 16x8 wrap */
	ACL_SOURCE_Y_OFFSET(16 - 1);
	break;
    case 3:
	ACL_SOURCE_WRAP(0x3D);       /* 24x8 wrap --- only for ET6000 !!! */
	ACL_SOURCE_Y_OFFSET(32 - 1); /* this is no error -- see databook */
	break;
    case 4:
	ACL_SOURCE_WRAP(0x35);       /* 32x8 wrap */
	ACL_SOURCE_Y_OFFSET(32 - 1);
    }
}

static void
TsengSubsequentColor8x8PatternFillRect(ScrnInfoPtr pScrn,
    int patx, int paty, int x, int y, int w, int h)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    int destaddr = FBADDR(pTseng, x, y);
    int srcaddr = pat_src_addr + MULBPP(pTseng, paty * 8 + patx);

    wait_acl_queue(pTseng);

    ACL_SOURCE_ADDRESS(srcaddr);

    SET_XY(pTseng, w, h);
    START_ACL(pTseng, destaddr);
}
#endif

#if 0
/*
 * ImageWrite is nothing more than a per-scanline screencopy.
 */

static void 
TsengSetupForScanlineImageWrite(ScrnInfoPtr pScrn,
    int rop, unsigned int planemask, int trans_color, int bpp, int depth)
{
    TsengPtr pTseng = TsengPTR(pScrn);

/*    ErrorF("IW"); */

    Tseng_setup_screencopy(pTseng, rop, planemask, trans_color, 0);

    ACL_SOURCE_WRAP(0x77);	       /* no wrap */
    ACL_SOURCE_Y_OFFSET(pTseng->line_width - 1);
}

static void 
TsengSubsequentScanlineImageWriteRect(ScrnInfoPtr pScrn,
    int x, int y, int w, int h, int skipleft)
{
    TsengPtr pTseng = TsengPTR(pScrn);

/*    ErrorF("r%d",h); */

    pTseng->acl_iw_dest = y * pTseng->line_width + MULBPP(pTseng, x);
    pTseng->acl_skipleft = MULBPP(pTseng, skipleft);

    wait_acl_queue(pTseng);
    SET_XY(pTseng, w, 1);
}

static void 
TsengSubsequentImageWriteScanline(ScrnInfoPtr pScrn,
    int bufno)
{
    TsengPtr pTseng = TsengPTR(pScrn);

/*    ErrorF("%d", bufno); */

    wait_acl_queue(pTseng);

    ACL_SOURCE_ADDRESS(pTseng->AccelImageWriteBufferOffsets[bufno] 
		       + pTseng->acl_skipleft);
    START_ACL(pTseng, pTseng->acl_iw_dest);
    pTseng->acl_iw_dest += pTseng->line_width;
}
#endif

#if 0
/*
 * W32p/ET6000 hardware linedraw code. 
 *
 * TsengSetupForSolidFill() is used as a setup function.
 *
 * Three major problems that needed to be solved here:
 *
 * 1. The "bias" value must be translated into the "line draw algorithm"
 *    parameter in the Tseng accelerators. This parameter, although not
 *    documented as such, needs to be set to the _inverse_ of the
 *    appropriate bias bit (i.e. for the appropriate octant).
 *
 * 2. In >8bpp modes, the accelerator will render BYTES in the same order as
 *    it is drawing the line. This means it will render the colors in the
 *    same order as well, reversing the byte-order in pixels that are drawn
 *    right-to-left. This causes wrong colors to be rendered.
 *
 * 3. The Tseng data book says that the ACL Y count register needs to be
 *    programmed with "dy-1". A similar thing is said about ACL X count. But
 *    this assumes (x2,y2) is NOT drawn (although that is not mentionned in
 *    the data book). X assumes the endpoint _is_ drawn. If "dy-1" is used,
 *    this sometimes results in a negative value (if dx==dy==0),
 *    causing a complete accelerator hang.
 */

static void
TsengSubsequentSolidBresenhamLine(ScrnInfoPtr pScrn,
    int x, int y, int major, int minor, int err, int len, int octant)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    int destaddr = FBADDR(pTseng, x, y);
    int xydir = pTseng->BresenhamTable[octant];
    
    /* Tseng wants the real dx/dy in major/minor. Bresenham uses 2*dx and 2*dy */
    minor >>= 1;
    major >>= 1;

    wait_acl_queue(pTseng);

    if (!(octant & YMAJOR)) {
	SET_X_YRAW(pTseng, len, 0xFFF);
    } else {
	SET_XY_RAW(pTseng,0xFFF, len - 1);
    }

    SET_DELTA(minor, major);
    ACL_ERROR_TERM(-err);  /* error term from XAA is NEGATIVE */

    /* make sure colors are rendered correctly if >8bpp */
    if (octant & XDECREASING) {
	destaddr += pTseng->Bytesperpixel - 1;
	ACL_SOURCE_ADDRESS(pTseng->AccelColorBufferOffset 
			   + pTseng->tsengFg + pTseng->neg_x_pixel_offset);
    } else
	ACL_SOURCE_ADDRESS(pTseng->AccelColorBufferOffset + pTseng->tsengFg);

    SET_XYDIR(xydir);

    START_ACL(pTseng, destaddr);
}
#endif

#ifdef TODO
/*
 * Trapezoid filling code.
 *
 * TsengSetupForSolidFill() is used as a setup function
 */

#undef DEBUG_TRAP

#ifdef TSENG_TRAPEZOIDS
static void
TsengSubsequentFillTrapezoidSolid(ytop, height, left, dxL, dyL, eL, right, dxR, dyR, eR)
    int ytop;
    int height;
    int left;
    int dxL, dyL;
    int eL;
    int right;
    int dxR, dyR;
    int eR;
{
    unsigned int tseng_bias_compensate = 0xd8;
    int destaddr, algrthm;
    int xcount = right - left + 1;     /* both edges included */
    int dir_reg = 0x60;		       /* trapezoid drawing; use error term for primary edge */
    int sec_dir_reg = 0x20;	       /* use error term for secondary edge */
    int octant = 0;

    /*    ErrorF("#"); */

    int destaddr, algrthm;
    int xcount = right - left + 1;

#ifdef USE_ERROR_TERM
    int dir_reg = 0x60;
    int sec_dir_reg = 0x20;

#else
    int dir_reg = 0x40;
    int sec_dir_reg = 0x00;

#endif
    int octant = 0;
    int bias = 0x00;		       /* FIXME !!! */

/*    ErrorF("#"); */

#ifdef DEBUG_TRAP
    ErrorF("ytop=%d, height=%d, left=%d, dxL=%d, dyL=%d, eL=%d, right=%d, dxR=%d, dyR=%d, eR=%d ",
	ytop, height, left, dxL, dyL, eL, right, dxR, dyR, eR);
#endif

    if ((dyL < 0) || (dyR < 0))
	ErrorF("Tseng Trapezoids: Wrong assumption: dyL/R < 0\n");

    destaddr = FBADDR(pTseng, left, ytop);

    /* left edge */
    if (dxL < 0) {
	dir_reg |= 1;
	octant |= XDECREASING;
	dxL = -dxL;
    }
    /* Y direction is always positive (top-to-bottom drawing) */

    wait_acl_queue(pTseng);

    /* left edge */
    /* compute axial direction and load registers */
    if (dxL >= dyL) {		       /* X is major axis */
	dir_reg |= 4;
	SET_DELTA(dyL, dxL);
	if (dir_reg & 1) {	       /* edge coherency: draw left edge */
	    destaddr += pTseng->Bytesperpixel;
	    sec_dir_reg |= 0x80;
	    xcount--;
	}
    } else {			       /* Y is major axis */
	SetYMajorOctant(octant);
	SET_DELTA(dxL, dyL);
    }
    ACL_ERROR_TERM(eL);

    /* select "linedraw algorithm" (=bias) and load direction register */
    /* ErrorF(" o=%d ", octant); */
    algrthm = ((tseng_bias_compensate >> octant) & 1) ^ 1;
    dir_reg |= algrthm << 4;
    SET_XYDIR(dir_reg);

    /* right edge */
    if (dxR < 0) {
	sec_dir_reg |= 1;
	dxR = -dxR;
    }
    /* compute axial direction and load registers */
    if (dxR >= dyR) {		       /* X is major axis */
	sec_dir_reg |= 4;
	SET_SECONDARY_DELTA(dyR, dxR);
	if (dir_reg & 1) {	       /* edge coherency: do not draw right edge */
	    sec_dir_reg |= 0x40;
	    xcount++;
	}
    } else {			       /* Y is major axis */
	SET_SECONDARY_DELTA(dxR, dyR);
    }
    ACL_SECONDARY_ERROR_TERM(eR);

    /* ErrorF("%02x", sec_dir_reg); */
    SET_SECONDARY_XYDIR(sec_dir_reg);

    SET_XY_6(pTseng, xcount, height);

#ifdef DEBUG_TRAP
    ErrorF("-> %d,%d\n", xcount, height);
#endif

    START_ACL_6(destaddr);
}
#endif

#endif

#endif

/*
 * The following function sets up the supported acceleration. Call it from
 * the FbInit() function in the SVGA driver. Do NOT initialize any hardware
 * in here. That belongs in tseng_init_acl().
 */
Bool
TsengXAAInit(ScreenPtr pScreen)
{
#ifdef HAVE_XAA_H
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    TsengPtr pTseng = TsengPTR(pScrn);
    XAAInfoRecPtr pXAAinfo;
    BoxRec AvailFBArea;

    PDEBUG("	TsengXAAInit\n");
    pTseng->AccelInfoRec = pXAAinfo = XAACreateInfoRec();
    if (!pXAAinfo)
	return FALSE;

    /*
     * Set up the main acceleration flags.
     */
    pXAAinfo->Flags = PIXMAP_CACHE;

    /*
     * The following line installs a "Sync" function, that waits for
     * all coprocessor operations to complete.
     */
    pXAAinfo->Sync = TsengSync;

    /* W32 and W32i must wait for ACL before changing registers */
    if (pTseng->ChipType == ET4000)
        pTseng->need_wait_acl = TRUE;
    else
        pTseng->need_wait_acl = FALSE;

    pTseng->line_width = pScrn->displayWidth * pTseng->Bytesperpixel;

#if 1
    /*
     * SolidFillRect.
     *
     * The W32 and W32i chips don't have a register to set the amount of
     * bytes per pixel, and hence they don't skip 1 byte in each 4-byte word
     * at 24bpp. Therefor, the FG or BG colors would have to be concatenated
     * in video memory (R-G-B-R-G-B-... instead of R-G-B-X-R-G-B-X-..., with
     * X = dont' care), plus a wrap value that is a multiple of 3 would have
     * to be set. There is no such wrap combination available.
     */
#ifdef OBSOLETE
    pXAAinfo->SolidFillFlags |= NO_PLANEMASK;
#endif

    pXAAinfo->SetupForSolidFill = TsengSetupForSolidFill;
    if (pTseng->ChipType == ET6000)
        pXAAinfo->SubsequentSolidFillRect = Tseng6KSubsequentSolidFillRect;
    else
        pXAAinfo->SubsequentSolidFillRect = TsengW32pSubsequentSolidFillRect;

#ifdef TSENG_TRAPEZOIDS
    if (pTseng->ChipType == ET6000)
	/* disabled for now: not fully compliant yet */
	pXAAinfo->SubsequentFillTrapezoidSolid = TsengSubsequentFillTrapezoidSolid;
#endif
#endif

#if 1
    /*
     * SceenToScreenCopy (BitBLT).
     * 
     * Restrictions: On ET6000, we support EITHER a planemask OR
     * TRANSPARENCY, but not both (they use the same Pattern map).
     * All other chips can't do TRANSPARENCY at all.
     */
#ifdef ET6K_TRANSPARENCY
    pXAAinfo->CopyAreaFlags = NO_PLANEMASK;
    if (pTseng->ChipType == ET4000)
	pXAAinfo->CopyAreaFlags |= NO_TRANSPARENCY;

#else
    pXAAinfo->CopyAreaFlags = NO_TRANSPARENCY;
#endif

    pXAAinfo->SetupForScreenToScreenCopy =
	TsengSetupForScreenToScreenCopy;
    pXAAinfo->SubsequentScreenToScreenCopy =
	TsengSubsequentScreenToScreenCopy;
#endif

#if 0
    /*
     * ImageWrite.
     *
     * SInce this uses off-screen scanline buffers, it is only of use when
     * complex ROPs are used. But since the current XAA pixmap cache code
     * only works when an ImageWrite is provided, the NO_GXCOPY flag is
     * temporarily disabled.
     */

    if (pTseng->AccelImageWriteBufferOffsets[0]) {
	pXAAinfo->ScanlineImageWriteFlags =
	    pXAAinfo->CopyAreaFlags | LEFT_EDGE_CLIPPING /* | NO_GXCOPY */ ;
	pXAAinfo->NumScanlineImageWriteBuffers = 2;
	pXAAinfo->SetupForScanlineImageWrite =
	    TsengSetupForScanlineImageWrite;
	pXAAinfo->SubsequentScanlineImageWriteRect =
	    TsengSubsequentScanlineImageWriteRect;
	pXAAinfo->SubsequentImageWriteScanline =
	    TsengSubsequentImageWriteScanline;

	/* calculate memory addresses from video memory offsets */
	for (i = 0; i < pXAAinfo->NumScanlineImageWriteBuffers; i++) {
	    pTseng->XAAScanlineImageWriteBuffers[i] =
		pTseng->FbBase + pTseng->AccelImageWriteBufferOffsets[i];
	}

	pXAAinfo->ScanlineImageWriteBuffers = pTseng->XAAScanlineImageWriteBuffers;
    }
#endif
    /*
     * 8x8 pattern tiling not possible on W32/i/p chips in 24bpp mode.
     * Currently, 24bpp pattern tiling doesn't work at all on those.
     *
     * FIXME: On W32 cards, pattern tiling doesn't work as expected.
     */
    pXAAinfo->Color8x8PatternFillFlags = HARDWARE_PATTERN_PROGRAMMED_ORIGIN;

    pXAAinfo->CachePixelGranularity = 8 * 8;

#ifdef ET6K_TRANSPARENCY
    pXAAinfo->PatternFlags |= HARDWARE_PATTERN_NO_PLANEMASK;
    if (pTseng->ChipType == ET6000)
	pXAAinfo->PatternFlags |= HARDWARE_PATTERN_TRANSPARENCY;
#endif

#if 0
    /* FIXME! This needs to be fixed for W32 and W32i (it "should work") */
    if (pScrn->bitsPerPixel != 24) {
	pXAAinfo->SetupForColor8x8PatternFill =
	    TsengSetupForColor8x8PatternFill;
	pXAAinfo->SubsequentColor8x8PatternFillRect =
	    TsengSubsequentColor8x8PatternFillRect;
    }
#endif

#if 0 /*1*/
    /*
     * SolidLine.
     *
     * We use Bresenham by preference, because it supports hardware clipping
     * (using the error term). TwoPointLines() is implemented, but not used,
     * because clipped lines are not accelerated (hardware clipping support
     * is lacking)...
     */

    /*
     * Fill in the hardware linedraw ACL_XY_DIRECTION table
     *
     * W32BresTable[] converts XAA interface Bresenham octants to direct
     * ACL direction register contents. This includes the correct bias
     * setting etc.
     *
     * According to miline.h (but with base 0 instead of base 1 as in
     * miline.h), the octants are numbered as follows:
     *
     *   \    |    /
     *    \ 2 | 1 /
     *     \  |  /
     *    3 \ | / 0
     *       \|/
     *   -----------
     *       /|                                 \
     *    4 / | \ 7
     *     /  |       \
     *    / 5 | 6      \
     *   /    |        \
     *
     * In ACL_XY_DIRECTION, bits 2:0 are defined as follows:
     *	0: '1' if XDECREASING
     *	1: '1' if YDECREASING
     *	2: '1' if XMAJOR (== not YMAJOR)
     *
     * Bit 4 defines the bias.  It should be set to '1' for all octants
     * NOT passed to miSetZeroLineBias(). i.e. the inverse of the X bias.
     *
     * (For MS compatible bias, the data book says to set to the same as
     * YDIR, i.e. bit 1 of the same register, = '1' if YDECREASING. MS
     * bias is towards octants 0..3 (i.e. Y decreasing), hence this
     * definition of bit 4)
     *
     */
    pTseng->BresenhamTable = xnfalloc(8);
    if (pTseng->BresenhamTable == NULL) {
        xf86Msg(X_ERROR, "Could not malloc Bresenham Table.\n");
        return FALSE;
    }
    for (i=0; i<8; i++) {
        unsigned char zerolinebias = miGetZeroLineBias(pScreen);
        pTseng->BresenhamTable[i] = 0xA0; /* command=linedraw, use error term */
        if (i & XDECREASING) pTseng->BresenhamTable[i] |= 0x01;
        if (i & YDECREASING) pTseng->BresenhamTable[i] |= 0x02;
        if (!(i & YMAJOR))   pTseng->BresenhamTable[i] |= 0x04;
        if ((1 << i) & zerolinebias) pTseng->BresenhamTable[i] |= 0x10;
        /* ErrorF("BresenhamTable[%d]=0x%x\n", i, pTseng->BresenhamTable[i]); */
    } 
    
    pXAAinfo->SolidLineFlags = 0;
    pXAAinfo->SetupForSolidLine = TsengSetupForSolidFill;
    pXAAinfo->SubsequentSolidBresenhamLine =
        TsengSubsequentSolidBresenhamLine;
    /*
     * ErrorTermBits is used to limit minor, major and error term, so it
     * must be min(errorterm_size, delta_major_size, delta_minor_size)
     * But the calculation for major and minor is done on the DOUBLED
     * values (as per the Bresenham algorithm), so they can also have 13
     * bits (inside XAA). They are divided by 2 in this driver, so they
     * are then again limited to 12 bits.
     */
    pXAAinfo->SolidBresenhamLineErrorTermBits = 13;

#endif

#if 1
    /* set up color expansion acceleration */
    if (!TsengXAAInit_Colexp(pScrn))
	return FALSE;
#endif


    /*
     * For Tseng, we set up some often-used values
     */

    switch (pTseng->Bytesperpixel) {   /* for MULBPP optimization */
    case 1:
	pTseng->powerPerPixel = 0;
	pTseng->planemask_mask = 0x000000FF;
	pTseng->neg_x_pixel_offset = 0;
	break;
    case 2:
	pTseng->powerPerPixel = 1;
	pTseng->planemask_mask = 0x0000FFFF;
	pTseng->neg_x_pixel_offset = 1;
	break;
    case 3:
	pTseng->powerPerPixel = 1;
	pTseng->planemask_mask = 0x00FFFFFF;
	pTseng->neg_x_pixel_offset = 2;		/* is this correct ??? */
	break;
    case 4:
	pTseng->powerPerPixel = 2;
	pTseng->planemask_mask = 0xFFFFFFFF;
	pTseng->neg_x_pixel_offset = 3;
	break;
    }

    /*
     * Init ping-pong registers.
     * This might be obsoleted by the BACKGROUND_OPERATIONS flag.
     */
    pTseng->tsengFg = 0;
    pTseng->tsengBg = 16;
    pTseng->tsengPat = 32;

    /* for register write optimisation */
    pTseng->tseng_old_dir = -1;
    pTseng->old_x = 0;
    pTseng->old_y = 0;

    /*
     * Finally, we set up the video memory space available to the pixmap
     * cache. In this case, all memory from the end of the virtual screen to
     * the end of video memory minus 1K (which we already reserved), can be
     * used.
     */

    AvailFBArea.x1 = 0;
    AvailFBArea.y1 = 0;
    AvailFBArea.x2 = pScrn->displayWidth;
    AvailFBArea.y2 = (pScrn->videoRam * 1024) /
	(pScrn->displayWidth * pTseng->Bytesperpixel);

    xf86InitFBManager(pScreen, &AvailFBArea);

    return (XAAInit(pScreen, pXAAinfo));
#else
    return FALSE;
#endif
}
