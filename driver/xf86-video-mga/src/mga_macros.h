#ifndef _MGA_MACROS_H_
#define _MGA_MACROS_H_

#ifdef XSERVER_LIBPCIACCESS
#define MGA_IO_ADDRESS(p) (p)->PciInfo->regions[(p)->io_bar].base_addr
#define VENDOR_ID(p)      (p)->vendor_id
#define DEVICE_ID(p)      (p)->device_id
#define SUBSYS_ID(p)      (p)->subdevice_id
#define CHIP_REVISION(p)  (p)->revision
#else
#define MGA_IO_ADDRESS(p) (p)->IOAddress
#define VENDOR_ID(p)      (p)->vendor
#define DEVICE_ID(p)      (p)->chipType
#define SUBSYS_ID(p)      (p)->subsysCard
#define CHIP_REVISION(p)  (p)->chipRev
#endif

#define RGBEQUAL(c) (!((((c) >> 8) ^ (c)) & 0xffff))

#ifdef MGADRI
#define MGA_SYNC_XTAG                 0x275f4200

#define MGABUSYWAIT() do { \
OUTREG(MGAREG_DWGSYNC, MGA_SYNC_XTAG); \
while(INREG(MGAREG_DWGSYNC) != MGA_SYNC_XTAG) ; \
}while(0);

#endif

#define MGAISBUSY() (INREG8(MGAREG_Status + 2) & 0x01)

#define WAITFIFO(cnt) \
   if(!pMga->UsePCIRetry) {\
	register int n = cnt; \
	if(n > pMga->FifoSize) n = pMga->FifoSize; \
	while(pMga->fifoCount < (n))\
	    pMga->fifoCount = INREG8(MGAREG_FIFOSTATUS);\
	pMga->fifoCount -= n;\
   }

#define XYADDRESS(x,y) \
    ((y) * pMga->CurrentLayout.displayWidth + (x) + pMga->YDstOrg)

#define MAKEDMAINDEX(index)  ((((index) >> 2) & 0x7f) | (((index) >> 6) & 0x80))

#define DMAINDICES(one,two,three,four)	\
	( MAKEDMAINDEX(one) | \
	 (MAKEDMAINDEX(two) << 8) | \
	 (MAKEDMAINDEX(three) << 16) | \
 	 (MAKEDMAINDEX(four) << 24) )

#define SET_PLANEMASK_REPLICATED(mask, rep_mask, bpp) \
    do { \
	if( (bpp != 24) \
	    && !(pMga->AccelFlags & MGA_NO_PLANEMASK) \
	    && ((mask) != pMga->PlaneMask)) { \
	   pMga->PlaneMask = (mask); \
	   OUTREG(MGAREG_PLNWT,(rep_mask)); \
	} \
    } while( 0 )

#define DISABLE_CLIP() { \
	pMga->AccelFlags &= ~CLIPPER_ON; \
	WAITFIFO(1); \
	OUTREG(MGAREG_CXBNDRY, 0xFFFF0000); }

#ifdef MGADRI
#define CHECK_DMA_QUIESCENT(pMGA, pScrn) {	\
   if (!pMGA->haveQuiescense) {			\
      pMGA->GetQuiescence( pScrn );		\
   }						\
}
#else
#define CHECK_DMA_QUIESCENT(pMGA, pScrn)
#endif

#define MGA_NOT_HAL(x) { x; }

#define MGAISGx50(x) ((x)->is_Gx50)

#define MGA_DH_NEEDS_HAL(x) (((x)->Chipset == PCI_CHIP_MGAG400) && \
			     ((x)->ChipRev < 0x80))

#endif /* _MGA_MACROS_H_ */
