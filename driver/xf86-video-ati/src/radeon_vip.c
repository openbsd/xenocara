#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_macros.h"
#include "radeon_probe.h"
#include <X11/extensions/Xv.h>
#include "radeon_video.h"

#include "xf86.h"
#include "atipciids.h"

#include "generic_bus.h"
#include "theatre_reg.h"

#define VIP_NAME      "RADEON VIP BUS"
#define VIP_TYPE      "ATI VIP BUS"

/* Status defines */
#define VIP_BUSY  0
#define VIP_IDLE  1
#define VIP_RESET 2

static Bool RADEONVIP_ioctl(GENERIC_BUS_Ptr b, long ioctl, long arg1, char *arg2)
{
    long count;
    switch(ioctl){
        case GB_IOCTL_GET_NAME:
                  count=strlen(VIP_NAME)+1;
                  if(count>arg1)return FALSE;
                  memcpy(arg2,VIP_NAME,count);
                  return TRUE;
                  
        case GB_IOCTL_GET_TYPE:
                  count=strlen(VIP_TYPE)+1;
                  if(count>arg1)return FALSE;
                  memcpy(arg2,VIP_TYPE,count);
                  return TRUE;
                  
        default: 
                  return FALSE;
    }
}

static uint32_t RADEONVIP_idle(GENERIC_BUS_Ptr b)
{
   ScrnInfoPtr pScrn = b->pScrn;
   RADEONInfoPtr info = RADEONPTR(pScrn);
   unsigned char *RADEONMMIO = info->MMIO;

   uint32_t timeout;
   
   RADEONWaitForIdleMMIO(pScrn);
   timeout = INREG(RADEON_VIPH_TIMEOUT_STAT);
   if(timeout & RADEON_VIPH_TIMEOUT_STAT__VIPH_REG_STAT) /* lockup ?? */
   {
       RADEONWaitForFifo(pScrn, 2);
       OUTREG(RADEON_VIPH_TIMEOUT_STAT, (timeout & 0xffffff00) | RADEON_VIPH_TIMEOUT_STAT__VIPH_REG_AK);
       RADEONWaitForIdleMMIO(pScrn);
       return (INREG(RADEON_VIPH_CONTROL) & 0x2000) ? VIP_BUSY : VIP_RESET;
   }
   RADEONWaitForIdleMMIO(pScrn);
   return (INREG(RADEON_VIPH_CONTROL) & 0x2000) ? VIP_BUSY : VIP_IDLE ;
}

static uint32_t RADEONVIP_fifo_idle(GENERIC_BUS_Ptr b, uint8_t channel)
{
   ScrnInfoPtr pScrn = b->pScrn;
   RADEONInfoPtr info = RADEONPTR(pScrn);
   unsigned char *RADEONMMIO = info->MMIO;

   uint32_t timeout;
   
   RADEONWaitForIdleMMIO(pScrn);
   timeout = INREG(VIPH_TIMEOUT_STAT);
   if((timeout & 0x0000000f) & channel) /* lockup ?? */
   {
       xf86DrvMsg(b->pScrn->scrnIndex, X_INFO, "RADEON_fifo_idle\n");
       RADEONWaitForFifo(pScrn, 2);
       OUTREG(VIPH_TIMEOUT_STAT, (timeout & 0xfffffff0) | channel);
       RADEONWaitForIdleMMIO(pScrn);
       return (INREG(VIPH_CONTROL) & 0x2000) ? VIP_BUSY : VIP_RESET;
   }
   RADEONWaitForIdleMMIO(pScrn);
   return (INREG(VIPH_CONTROL) & 0x2000) ? VIP_BUSY : VIP_IDLE ;
}

/* address format:
     ((device & 0x3)<<14)   | (fifo << 12) | (addr)
*/

#define VIP_WAIT_FOR_IDLE() {			\
    int i2ctries = 0;				\
    while (i2ctries < 10) {			\
      status = RADEONVIP_idle(b);		\
      if (status==VIP_BUSY)			\
      {						\
	usleep(1000);				\
	i2ctries++;				\
      } else break;				\
    }						\
  } 

static Bool RADEONVIP_read(GENERIC_BUS_Ptr b, uint32_t address, uint32_t count, uint8_t *buffer)
{
   ScrnInfoPtr pScrn = b->pScrn;
   RADEONInfoPtr info = RADEONPTR(pScrn);
   unsigned char *RADEONMMIO = info->MMIO;
   uint32_t status,tmp;

   if((count!=1) && (count!=2) && (count!=4))
   {
   xf86DrvMsg(pScrn->scrnIndex,X_ERROR,"Attempt to access VIP bus with non-stadard transaction length\n");
   return FALSE;
   }
   
   RADEONWaitForFifo(pScrn, 2);
   OUTREG(RADEON_VIPH_REG_ADDR, address | 0x2000);
   write_mem_barrier();
   VIP_WAIT_FOR_IDLE();
   if(VIP_IDLE != status) return FALSE;
   
/*
         disable RADEON_VIPH_REGR_DIS to enable VIP cycle.
         The LSB of RADEON_VIPH_TIMEOUT_STAT are set to 0
         because 1 would have acknowledged various VIP
         interrupts unexpectedly 
*/      
   RADEONWaitForIdleMMIO(pScrn);
   OUTREG(RADEON_VIPH_TIMEOUT_STAT, INREG(RADEON_VIPH_TIMEOUT_STAT) & (0xffffff00 & ~RADEON_VIPH_TIMEOUT_STAT__VIPH_REGR_DIS) );
   write_mem_barrier();
/*
         the value returned here is garbage.  The read merely initiates
         a register cycle
*/
    RADEONWaitForIdleMMIO(pScrn);
    INREG(RADEON_VIPH_REG_DATA);
    
    VIP_WAIT_FOR_IDLE();
    if(VIP_IDLE != status) return FALSE;
/*
        set RADEON_VIPH_REGR_DIS so that the read won't take too long.
*/
    RADEONWaitForIdleMMIO(pScrn);
    tmp=INREG(RADEON_VIPH_TIMEOUT_STAT);
    OUTREG(RADEON_VIPH_TIMEOUT_STAT, (tmp & 0xffffff00) | RADEON_VIPH_TIMEOUT_STAT__VIPH_REGR_DIS);         
    write_mem_barrier();
    RADEONWaitForIdleMMIO(pScrn);
    switch(count){
        case 1:
             *buffer=(uint8_t)(INREG(RADEON_VIPH_REG_DATA) & 0xff);
             break;
        case 2:
             *(uint16_t *)buffer=(uint16_t) (INREG(RADEON_VIPH_REG_DATA) & 0xffff);
             break;
        case 4:
             *(uint32_t *)buffer=(uint32_t) ( INREG(RADEON_VIPH_REG_DATA) & 0xffffffff);
             break;
        }
     VIP_WAIT_FOR_IDLE();
     if(VIP_IDLE != status) return FALSE;
 /*     
 so that reading RADEON_VIPH_REG_DATA would not trigger unnecessary vip cycles.
*/
     OUTREG(RADEON_VIPH_TIMEOUT_STAT, (INREG(RADEON_VIPH_TIMEOUT_STAT) & 0xffffff00) | RADEON_VIPH_TIMEOUT_STAT__VIPH_REGR_DIS);
     write_mem_barrier();
     return TRUE;
}

static Bool RADEONVIP_fifo_read(GENERIC_BUS_Ptr b, uint32_t address, uint32_t count, uint8_t *buffer)
{
   ScrnInfoPtr pScrn = b->pScrn;
   RADEONInfoPtr info = RADEONPTR(pScrn);
   unsigned char *RADEONMMIO = info->MMIO;
   uint32_t status,tmp;

   if(count!=1)
   {
   xf86DrvMsg(pScrn->scrnIndex,X_ERROR,"Attempt to access VIP bus with non-stadard transaction length\n");
   return FALSE;
   }
   
   RADEONWaitForFifo(pScrn, 2);
   OUTREG(VIPH_REG_ADDR, address | 0x3000);
   write_mem_barrier();
   while(VIP_BUSY == (status = RADEONVIP_fifo_idle(b, 0xff)));
   if(VIP_IDLE != status) return FALSE;

/*
         disable VIPH_REGR_DIS to enable VIP cycle.
         The LSB of VIPH_TIMEOUT_STAT are set to 0
         because 1 would have acknowledged various VIP
         interrupts unexpectedly 
*/      
	
   RADEONWaitForIdleMMIO(pScrn);
   OUTREG(VIPH_TIMEOUT_STAT, INREG(VIPH_TIMEOUT_STAT) & (0xffffff00 & ~VIPH_TIMEOUT_STAT__VIPH_REGR_DIS) );
   write_mem_barrier();

/*
         the value returned here is garbage.  The read merely initiates
         a register cycle
*/
    RADEONWaitForIdleMMIO(pScrn);
    INREG(VIPH_REG_DATA);
    
    while(VIP_BUSY == (status = RADEONVIP_fifo_idle(b, 0xff)));
    if(VIP_IDLE != status) return FALSE;

/*
        set VIPH_REGR_DIS so that the read won't take too long.
*/
    RADEONWaitForIdleMMIO(pScrn);
    tmp=INREG(VIPH_TIMEOUT_STAT);
    OUTREG(VIPH_TIMEOUT_STAT, (tmp & 0xffffff00) | VIPH_TIMEOUT_STAT__VIPH_REGR_DIS);         
    write_mem_barrier();

    RADEONWaitForIdleMMIO(pScrn);
    switch(count){
        case 1:
             *buffer=(uint8_t)(INREG(VIPH_REG_DATA) & 0xff);
             break;
        case 2:
             *(uint16_t *)buffer=(uint16_t) (INREG(VIPH_REG_DATA) & 0xffff);
             break;
        case 4:
             *(uint32_t *)buffer=(uint32_t) ( INREG(VIPH_REG_DATA) & 0xffffffff);
             break;
        }
     while(VIP_BUSY == (status = RADEONVIP_fifo_idle(b, 0xff)));
     if(VIP_IDLE != status) return FALSE;

 /*     
 so that reading VIPH_REG_DATA would not trigger unnecessary vip cycles.
*/
     OUTREG(VIPH_TIMEOUT_STAT, (INREG(VIPH_TIMEOUT_STAT) & 0xffffff00) | VIPH_TIMEOUT_STAT__VIPH_REGR_DIS);
     write_mem_barrier();
     return TRUE;

   
}


static Bool RADEONVIP_write(GENERIC_BUS_Ptr b, uint32_t address, uint32_t count, uint8_t *buffer)
{
    ScrnInfoPtr pScrn = b->pScrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    
    uint32_t status;


    if((count!=4))
    {
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Attempt to access VIP bus with non-stadard transaction length\n");
    return FALSE;
    }
    
    RADEONWaitForFifo(pScrn, 2);
    OUTREG(RADEON_VIPH_REG_ADDR, address & (~0x2000));
    while(VIP_BUSY == (status = RADEONVIP_idle(b)));
    
    if(VIP_IDLE != status) return FALSE;
    
    RADEONWaitForFifo(pScrn, 2);
    switch(count){
        case 4:
             OUTREG(RADEON_VIPH_REG_DATA, *(uint32_t *)buffer);
             break;
        }
    write_mem_barrier();
    while(VIP_BUSY == (status = RADEONVIP_idle(b)));
    if(VIP_IDLE != status) return FALSE;
    return TRUE;
}

static Bool RADEONVIP_fifo_write(GENERIC_BUS_Ptr b, uint32_t address, uint32_t count, uint8_t *buffer)
{
    ScrnInfoPtr pScrn = b->pScrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    
    uint32_t status;
	uint32_t i;

    RADEONWaitForFifo(pScrn, 2);
    OUTREG(VIPH_REG_ADDR, (address & (~0x2000)) | 0x1000);
    while(VIP_BUSY == (status = RADEONVIP_fifo_idle(b, 0x0f)));

    
    if(VIP_IDLE != status){
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "cannot write %x to VIPH_REG_ADDR\n", (unsigned int)address);
		return FALSE;
	}
    
	RADEONWaitForFifo(pScrn, 2);
	for (i = 0; i < count; i+=4)
	{
		OUTREG(VIPH_REG_DATA, *(uint32_t*)(buffer + i));
    	write_mem_barrier();
		while(VIP_BUSY == (status = RADEONVIP_fifo_idle(b, 0x0f)));
    	if(VIP_IDLE != status)
		{
    		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "cannot write to VIPH_REG_DATA\n");
			return FALSE;
	 	}
	}
				
    return TRUE;
}

void RADEONVIP_reset(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;


    RADEONWaitForIdleMMIO(pScrn);
    switch(info->ChipFamily){
		case CHIP_FAMILY_RV250:
		case CHIP_FAMILY_RV350:
		case CHIP_FAMILY_R350:
		case CHIP_FAMILY_R300:
	    OUTREG(RADEON_VIPH_CONTROL, 0x003F0009); /* slowest, timeout in 16 phases */
	    OUTREG(RADEON_VIPH_TIMEOUT_STAT, (INREG(RADEON_VIPH_TIMEOUT_STAT) & 0xFFFFFF00) | RADEON_VIPH_TIMEOUT_STAT__VIPH_REGR_DIS);
	    OUTREG(RADEON_VIPH_DV_LAT, 0x444400FF); /* set timeslice */
	    OUTREG(RADEON_VIPH_BM_CHUNK, 0x0);
	    OUTREG(RADEON_TEST_DEBUG_CNTL, INREG(RADEON_TEST_DEBUG_CNTL) & (~RADEON_TEST_DEBUG_CNTL__TEST_DEBUG_OUT_EN));
	    break;
		case CHIP_FAMILY_RV380:
	    OUTREG(RADEON_VIPH_CONTROL, 0x003F000D); /* slowest, timeout in 16 phases */
	    OUTREG(RADEON_VIPH_TIMEOUT_STAT, (INREG(RADEON_VIPH_TIMEOUT_STAT) & 0xFFFFFF00) | RADEON_VIPH_TIMEOUT_STAT__VIPH_REGR_DIS);
	    OUTREG(RADEON_VIPH_DV_LAT, 0x444400FF); /* set timeslice */
	    OUTREG(RADEON_VIPH_BM_CHUNK, 0x0);
	    OUTREG(RADEON_TEST_DEBUG_CNTL, INREG(RADEON_TEST_DEBUG_CNTL) & (~RADEON_TEST_DEBUG_CNTL__TEST_DEBUG_OUT_EN));
	    break;
	default:
	    OUTREG(RADEON_VIPH_CONTROL, 0x003F0004); /* slowest, timeout in 16 phases */
	    OUTREG(RADEON_VIPH_TIMEOUT_STAT, (INREG(RADEON_VIPH_TIMEOUT_STAT) & 0xFFFFFF00) | RADEON_VIPH_TIMEOUT_STAT__VIPH_REGR_DIS);
	    OUTREG(RADEON_VIPH_DV_LAT, 0x444400FF); /* set timeslice */
	    OUTREG(RADEON_VIPH_BM_CHUNK, 0x151);
	    OUTREG(RADEON_TEST_DEBUG_CNTL, INREG(RADEON_TEST_DEBUG_CNTL) & (~RADEON_TEST_DEBUG_CNTL__TEST_DEBUG_OUT_EN));
	} 
}

void RADEONVIP_init(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    pPriv->VIP=calloc(1,sizeof(GENERIC_BUS_Rec));
    pPriv->VIP->pScrn=pScrn;
    pPriv->VIP->DriverPrivate.ptr=pPriv;
    pPriv->VIP->ioctl=RADEONVIP_ioctl;
    pPriv->VIP->read=RADEONVIP_read;
    pPriv->VIP->write=RADEONVIP_write;
    pPriv->VIP->fifo_read=RADEONVIP_fifo_read;
    pPriv->VIP->fifo_write=RADEONVIP_fifo_write;
   
    RADEONVIP_reset(pScrn, pPriv);
}
