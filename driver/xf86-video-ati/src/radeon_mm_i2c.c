#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>

#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_macros.h"
#include "radeon_probe.h"
#include <X11/extensions/Xv.h>
#include "radeon_video.h"
#include "atipciids.h"

#include "xf86.h"

/* i2c stuff */
#include "xf86i2c.h"
#include "fi1236.h"
#include "msp3430.h"
#include "tda9885.h"
#include "uda1380.h"
#include "i2c_def.h"


static void RADEON_TDA9885_Init(RADEONPortPrivPtr pPriv);

/* Wait for 10ms at the most for the I2C_GO register to drop. */
#define I2C_WAIT_FOR_GO() { \
	int i2ctries = 0; \
	RADEONWaitForIdleMMIO(pScrn); \
	write_mem_barrier(); \
	while (i2ctries < 10) { \
		reg = INREG8(RADEON_I2C_CNTL_0+1); \
		if (!(reg & (RADEON_I2C_GO >> 8))) \
			break; \
		if (reg & (RADEON_I2C_ABORT >> 8)) \
			break; \
		usleep(1000); \
		i2ctries++; \
	} \
}

/* Wait, and dump the status in the 'status' register.  If we time out or
 * receive an abort signal, halt/restart the I2C bus and leave _ABORT in the
 * status register. */
#define I2C_WAIT_WITH_STATUS() { \
	I2C_WAIT_FOR_GO() \
	if (reg & ((RADEON_I2C_ABORT >> 8) | (RADEON_I2C_GO >> 8))) { \
		RADEON_I2C_Halt(pScrn); \
		status = RADEON_I2C_ABORT; \
	} \
	else \
		status = RADEON_I2C_WaitForAck(pScrn, pPriv); \
}

/****************************************************************************
 *  I2C_WaitForAck (void)                                                   *
 *                                                                          *
 *  Function: polls the I2C status bits, waiting for an acknowledge or      *
 *            an error condition.                                           *
 *    Inputs: NONE                                                          *
 *   Outputs: I2C_DONE - the I2C transfer was completed                     *
 *            I2C_NACK - an NACK was received from the slave                *
 *            I2C_HALT - a timeout condition has occured                    *
 ****************************************************************************/
static uint8_t RADEON_I2C_WaitForAck (ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    uint8_t retval = 0;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    long counter = 0;

    usleep(1000);
    while(1)
    {
        RADEONWaitForIdleMMIO(pScrn); 
        retval = INREG8(RADEON_I2C_CNTL_0);
        if (retval & RADEON_I2C_HALT)
        {
            return (RADEON_I2C_HALT);
        }
        if (retval & RADEON_I2C_NACK)
        {
            return (RADEON_I2C_NACK);
        }
        if(retval & RADEON_I2C_DONE)
        {
            return RADEON_I2C_DONE;
        }       
        counter++;
	/* 50ms ought to be long enough. */
        if(counter > 50)
        {
             xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Timeout condition on Radeon i2c bus\n");
             return RADEON_I2C_HALT;
        }
	usleep(1000);
    }
}

static void RADEON_I2C_Halt (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint8_t    reg;

    /* reset status flags */
    RADEONWaitForIdleMMIO(pScrn);
    reg = INREG8 (RADEON_I2C_CNTL_0 + 0) & ~(RADEON_I2C_DONE|RADEON_I2C_NACK|RADEON_I2C_HALT);
    OUTREG8 (RADEON_I2C_CNTL_0 + 0, reg);

    /* issue ABORT call */
    RADEONWaitForIdleMMIO(pScrn);
    reg = INREG8 (RADEON_I2C_CNTL_0 + 1) & 0xE7;
    OUTREG8 (RADEON_I2C_CNTL_0 + 1, (reg |((RADEON_I2C_GO|RADEON_I2C_ABORT) >> 8)));

    /* wait for GO bit to go low */
    I2C_WAIT_FOR_GO();
}


static Bool RADEONI2CWriteRead(I2CDevPtr d, I2CByte *WriteBuffer, int nWrite,
                            I2CByte *ReadBuffer, int nRead)
{
    int loop, status;
    uint32_t i2c_cntl_0, i2c_cntl_1;
    uint8_t reg;
    RADEONPortPrivPtr pPriv = (RADEONPortPrivPtr)(d->pI2CBus->DriverPrivate.ptr);
    ScrnInfoPtr pScrn = xf86Screens[d->pI2CBus->scrnIndex];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    status=RADEON_I2C_DONE;

    RADEONWaitForIdleMMIO(pScrn);
    if(nWrite>0){
/*       RADEONWaitForFifo(pScrn, 4+nWrite); */

       /* Clear the status bits of the I2C Controller */
       OUTREG(RADEON_I2C_CNTL_0, RADEON_I2C_DONE | RADEON_I2C_NACK | RADEON_I2C_HALT | RADEON_I2C_SOFT_RST);

       /* Write the address into the buffer first */
       OUTREG(RADEON_I2C_DATA, (uint32_t) (d->SlaveAddr) & ~(1));

       /* Write Value into the buffer */
       for (loop = 0; loop < nWrite; loop++)
       {
          OUTREG8(RADEON_I2C_DATA, WriteBuffer[loop]);
       }

       i2c_cntl_1 = (pPriv->radeon_i2c_timing << 24) | RADEON_I2C_EN | RADEON_I2C_SEL |
                        nWrite | 0x100;
       OUTREG(RADEON_I2C_CNTL_1, i2c_cntl_1);
    
       i2c_cntl_0 = (pPriv->radeon_N << 24) | (pPriv->radeon_M << 16) | 
                        RADEON_I2C_GO | RADEON_I2C_START | ((nRead >0)?0:RADEON_I2C_STOP) | RADEON_I2C_DRIVE_EN;
       OUTREG(RADEON_I2C_CNTL_0, i2c_cntl_0);
    
       I2C_WAIT_WITH_STATUS();

       if(status!=RADEON_I2C_DONE){
          RADEON_I2C_Halt(pScrn);
          return FALSE;
          }
    }
    
    
    if(nRead > 0) {
       RADEONWaitForFifo(pScrn, 4+nRead);
    
       OUTREG(RADEON_I2C_CNTL_0, RADEON_I2C_DONE | RADEON_I2C_NACK | RADEON_I2C_HALT | RADEON_I2C_SOFT_RST); 

       /* Write the address into the buffer first */
       OUTREG(RADEON_I2C_DATA, (uint32_t) (d->SlaveAddr) | (1));

       i2c_cntl_1 = (pPriv->radeon_i2c_timing << 24) | RADEON_I2C_EN | RADEON_I2C_SEL | 
                        nRead | 0x100;
       OUTREG(RADEON_I2C_CNTL_1, i2c_cntl_1);
    
       i2c_cntl_0 = (pPriv->radeon_N << 24) | (pPriv->radeon_M << 16) | 
                        RADEON_I2C_GO | RADEON_I2C_START | RADEON_I2C_STOP | RADEON_I2C_DRIVE_EN | RADEON_I2C_RECEIVE;
       OUTREG(RADEON_I2C_CNTL_0, i2c_cntl_0);
    
       I2C_WAIT_WITH_STATUS();
  
       /* Write Value into the buffer */
       for (loop = 0; loop < nRead; loop++)
       {
          RADEONWaitForFifo(pScrn, 1);
          if((status == RADEON_I2C_HALT) || (status == RADEON_I2C_NACK))
          {
          ReadBuffer[loop]=0xff;
          } else {
          RADEONWaitForIdleMMIO(pScrn);
          ReadBuffer[loop]=INREG8(RADEON_I2C_DATA) & 0xff;
          }
       }
    }
    
    if(status!=RADEON_I2C_DONE){
       RADEON_I2C_Halt(pScrn);
       return FALSE;
       }
    return TRUE;
}

static Bool R200_I2CWriteRead(I2CDevPtr d, I2CByte *WriteBuffer, int nWrite,
                            I2CByte *ReadBuffer, int nRead)
{
    int loop, status;
    uint32_t i2c_cntl_0, i2c_cntl_1;
    uint8_t reg;
    RADEONPortPrivPtr pPriv = (RADEONPortPrivPtr)(d->pI2CBus->DriverPrivate.ptr);
    ScrnInfoPtr pScrn = xf86Screens[d->pI2CBus->scrnIndex];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    status=RADEON_I2C_DONE;

    RADEONWaitForIdleMMIO(pScrn);
    if(nWrite>0){
/*       RADEONWaitForFifo(pScrn, 4+nWrite); */

       /* Clear the status bits of the I2C Controller */
       OUTREG(RADEON_I2C_CNTL_0, RADEON_I2C_DONE | RADEON_I2C_NACK | RADEON_I2C_HALT | RADEON_I2C_SOFT_RST);

       /* Write the address into the buffer first */
       OUTREG(RADEON_I2C_DATA, (uint32_t) (d->SlaveAddr) & ~(1));

       /* Write Value into the buffer */
       for (loop = 0; loop < nWrite; loop++)
       {
          OUTREG8(RADEON_I2C_DATA, WriteBuffer[loop]);
       }

       i2c_cntl_1 = (pPriv->radeon_i2c_timing << 24) | RADEON_I2C_EN | RADEON_I2C_SEL |
                        nWrite | 0x010;
       OUTREG(RADEON_I2C_CNTL_1, i2c_cntl_1);
    
       i2c_cntl_0 = (pPriv->radeon_N << 24) | (pPriv->radeon_M << 16) | 
                        RADEON_I2C_GO | RADEON_I2C_START | ((nRead >0)?0:RADEON_I2C_STOP) | RADEON_I2C_DRIVE_EN;
       OUTREG(RADEON_I2C_CNTL_0, i2c_cntl_0);
    
       I2C_WAIT_WITH_STATUS();

       if(status!=RADEON_I2C_DONE){
          RADEON_I2C_Halt(pScrn);
          return FALSE;
          }
    }
    
    
    if(nRead > 0) {
       RADEONWaitForFifo(pScrn, 4+nRead);
    
       OUTREG(RADEON_I2C_CNTL_0, RADEON_I2C_DONE | RADEON_I2C_NACK | RADEON_I2C_HALT | RADEON_I2C_SOFT_RST); 

       /* Write the address into the buffer first */
       OUTREG(RADEON_I2C_DATA, (uint32_t) (d->SlaveAddr) | (1));

       i2c_cntl_1 = (pPriv->radeon_i2c_timing << 24) | RADEON_I2C_EN | RADEON_I2C_SEL | 
                        nRead | 0x010;
       OUTREG(RADEON_I2C_CNTL_1, i2c_cntl_1);
    
       i2c_cntl_0 = (pPriv->radeon_N << 24) | (pPriv->radeon_M << 16) | 
                        RADEON_I2C_GO | RADEON_I2C_START | RADEON_I2C_STOP | RADEON_I2C_DRIVE_EN | RADEON_I2C_RECEIVE;
       OUTREG(RADEON_I2C_CNTL_0, i2c_cntl_0);
    
       I2C_WAIT_WITH_STATUS();
  
       RADEONWaitForIdleMMIO(pScrn);
       /* Write Value into the buffer */
       for (loop = 0; loop < nRead; loop++)
       {
          if((status == RADEON_I2C_HALT) || (status == RADEON_I2C_NACK))
          {
          ReadBuffer[loop]=0xff;
          } else {
          ReadBuffer[loop]=INREG8(RADEON_I2C_DATA) & 0xff;
          }
       }
    }
    
    if(status!=RADEON_I2C_DONE){
       RADEON_I2C_Halt(pScrn);
       return FALSE;
       }
    return TRUE;
}

#if 0
static Bool RADEONProbeAddress(I2CBusPtr b, I2CSlaveAddr addr)
{
     I2CByte a;
     I2CDevRec d;
     
     d.DevName = "Probing";
     d.SlaveAddr = addr;
     d.pI2CBus = b;
     d.NextDev = NULL;
     
     return I2C_WriteRead(&d, NULL, 0, &a, 1);
}
#endif

#define I2C_CLOCK_FREQ     (60000.0)


const struct 
{
   char *name; 
   int type;
} RADEON_tuners[32] =
    {
        /* name ,index to tuner_parms table */
        {"NO TUNER"            , -1},
        {"Philips FI1236 (or compatible)"               , TUNER_TYPE_FI1236},
        {"Philips FI1236 (or compatible)"               , TUNER_TYPE_FI1236},
        {"Philips FI1216 (or compatible)"               , TUNER_TYPE_FI1216},
        {"Philips FI1246 (or compatible)"               , TUNER_TYPE_FI1246},
        {"Philips FI1216MF (or compatible)"             , TUNER_TYPE_FI1216},
        {"Philips FI1236 (or compatible)"               , TUNER_TYPE_FI1236},
        {"Philips FI1256 (or compatible)"               , TUNER_TYPE_FI1256},
        {"Philips FI1236 (or compatible)"               , TUNER_TYPE_FI1236},
        {"Philips FI1216 (or compatible)"               , TUNER_TYPE_FI1216},
        {"Philips FI1246 (or compatible)"               , TUNER_TYPE_FI1246},
        {"Philips FI1216MF (or compatible)"             , TUNER_TYPE_FI1216},
        {"Philips FI1236 (or compatible)"               , TUNER_TYPE_FI1236},
        {"TEMIC-FN5AL"          , TUNER_TYPE_TEMIC_FN5AL},
        {"FQ1216ME/P"           , TUNER_TYPE_FI1216},
        {"FI1236W"              , TUNER_TYPE_FI1236W},
	{"Philips FI1216ME (or compatible)"             , TUNER_TYPE_FM1216ME},
        /*{"Alps TSCxx"           , -1},*/
	{"Philips FM1236/F"     , TUNER_TYPE_FI1236W},
	{"Philips FI1216ME (or compatible)"             , TUNER_TYPE_FM1216ME},
        {"UNKNOWN-19"           , -1},
        {"UNKNOWN-20"           , -1},
        {"UNKNOWN-21"           , -1},
        {"UNKNOWN-22"           , -1},
        {"UNKNOWN-23"           , -1},
        {"UNKNOWN-24"           , -1},
        {"UNKNOWN-25"           , -1},
        {"UNKNOWN-26"           , -1},
        {"UNKNOWN-27"           , -1},
        {"UNKNOWN-28"           , -1},
        {"Microtuner MT2032"            , TUNER_TYPE_MT2032},
        {"Microtuner MT2032"            , TUNER_TYPE_MT2032},
        {"UNKNOWN-31"           , -1}
    };


void RADEONResetI2C(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    RADEONWaitForFifo(pScrn, 2);
    OUTREG8(RADEON_I2C_CNTL_1+2, ((RADEON_I2C_SEL | RADEON_I2C_EN)>>16));
    OUTREG8(RADEON_I2C_CNTL_0+0, (RADEON_I2C_DONE | RADEON_I2C_NACK | RADEON_I2C_HALT | RADEON_I2C_SOFT_RST | RADEON_I2C_DRIVE_EN | RADEON_I2C_DRIVE_SEL));
}

void RADEONInitI2C(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    double nm;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONPLLPtr  pll = &(info->pll);

    pPriv->i2c = NULL;
    pPriv->fi1236 = NULL;
    pPriv->msp3430 = NULL;
    pPriv->tda9885 = NULL;
	 pPriv->uda1380 = NULL;
    #if 0 /* put back on when saa7114 support is present */
    pPriv->saa7114 = NULL;
    #endif
    
    /* Blacklist chipsets that lockup - these are usually older mobility chips */

    switch(info->Chipset){
    	case PCI_CHIP_RADEON_LY:
	case PCI_CHIP_RADEON_LZ:
	     xf86DrvMsg(pScrn->scrnIndex,X_INFO,"Detected Radeon Mobility M6, disabling multimedia i2c\n");
	     return;
	case PCI_CHIP_RADEON_LW:
	     xf86DrvMsg(pScrn->scrnIndex,X_INFO,"Detected Radeon Mobility M7, disabling multimedia i2c\n");
 	     return;
	/*case PCI_CHIP_RV250_If:
	     xf86DrvMsg(pScrn->scrnIndex,X_INFO,"Detected Radeon 9000 - skipping multimedia i2c initialization code.\n");
	     return;*/
	case PCI_CHIP_RV370_5460:
	     xf86DrvMsg(pScrn->scrnIndex,X_INFO,"Detected Radeon Mobility X300, disabling multimedia i2c\n");
	     return;
	}
    
    /* no multimedia capabilities detected and no information was provided to substitute for it */
    if(!info->MM_TABLE_valid  && 
       !(info->tunerType>=0))
    {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "No video input capabilities detected and no information is provided - disabling multimedia i2c\n");
       return;
    }

    
    if(pPriv->i2c!=NULL) return;  /* for some reason we are asked to init it again.. Stop ! */
    
    if(!xf86LoadSubModule(pScrn,"i2c")) 
    {
        xf86DrvMsg(pScrn->scrnIndex,X_ERROR,"Unable to initialize i2c bus\n");
        pPriv->i2c = NULL;
        return;
    } 
    pPriv->i2c=CreateI2CBusRec();
    pPriv->i2c->scrnIndex=pScrn->scrnIndex;
    pPriv->i2c->BusName="Radeon multimedia bus";
    pPriv->i2c->DriverPrivate.ptr=(pointer)pPriv;
    switch(info->ChipFamily){
    	case CHIP_FAMILY_RV350:
    	case CHIP_FAMILY_R350:
    	case CHIP_FAMILY_R300:
	case CHIP_FAMILY_RV250:
    	case CHIP_FAMILY_R200:
		case CHIP_FAMILY_RV200:
            	pPriv->i2c->I2CWriteRead=R200_I2CWriteRead;
            	xf86DrvMsg(pScrn->scrnIndex,X_INFO,"Using R200 i2c bus access method\n");
		break;
	default:
            	pPriv->i2c->I2CWriteRead=RADEONI2CWriteRead;
            	xf86DrvMsg(pScrn->scrnIndex,X_INFO,"Using Radeon bus access method\n");
        }
    if(!I2CBusInit(pPriv->i2c))
    {
        xf86DrvMsg(pScrn->scrnIndex,X_ERROR,"Failed to register i2c bus\n");
    }

#if 1
    switch(info->ChipFamily){
	case CHIP_FAMILY_RV200:
            nm=(pll->reference_freq * 40000.0)/(1.0*I2C_CLOCK_FREQ);
	    break;
    	case CHIP_FAMILY_R300:
    	case CHIP_FAMILY_R200:
    	    if(info->MM_TABLE_valid && (RADEON_tuners[info->MM_TABLE.tuner_type & 0x1f].type==TUNER_TYPE_MT2032)){
                nm=(pll->reference_freq * 40000.0)/(4.0*I2C_CLOCK_FREQ);
	        break;
                } 
	default:
            nm=(pll->reference_freq * 10000.0)/(4.0*I2C_CLOCK_FREQ);
        }
#else
    nm=(pll->xclk * 40000.0)/(1.0*I2C_CLOCK_FREQ);         
#endif
    for(pPriv->radeon_N=1; pPriv->radeon_N<255; pPriv->radeon_N++)
          if((pPriv->radeon_N * (pPriv->radeon_N-1)) > nm)break;
    pPriv->radeon_M=pPriv->radeon_N-1;
    pPriv->radeon_i2c_timing=2*pPriv->radeon_N;


#if 0
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ref=%d M=0x%02x N=0x%02x timing=0x%02x\n", pll->reference_freq, pPriv->radeon_M, pPriv->radeon_N, pPriv->radeon_i2c_timing);
    pPriv->radeon_M=0x32;
    pPriv->radeon_N=0x33;
    pPriv->radeon_i2c_timing=2*pPriv->radeon_N;
#endif
    RADEONResetI2C(pScrn, pPriv);
    
#if 0 /* I don't know whether standalone boards are supported with Radeons */
      /* looks like none of them have AMC connectors anyway */
    if(!info->MM_TABLE_valid)RADEON_read_eeprom(pPriv);
#endif    
    
    if(!xf86LoadSubModule(pScrn,"fi1236"))
    {
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to initialize fi1236 driver\n");
    }
    else
    {
    if(pPriv->fi1236 == NULL)
    {
        pPriv->fi1236 = xf86_Detect_FI1236(pPriv->i2c, FI1236_ADDR_1);
    }
    if(pPriv->fi1236 == NULL)
    {
        pPriv->fi1236 = xf86_Detect_FI1236(pPriv->i2c, FI1236_ADDR_2);
    }
    }
    if(pPriv->fi1236 != NULL)
    {
         xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Detected %s device at 0x%02x\n", 
               RADEON_tuners[info->MM_TABLE.tuner_type & 0x1f].name,
               FI1236_ADDR(pPriv->fi1236));
         if(info->MM_TABLE_valid)xf86_FI1236_set_tuner_type(pPriv->fi1236, RADEON_tuners[info->MM_TABLE.tuner_type & 0x1f].type);
                else {
                   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "MM_TABLE not found (standalone board ?), forcing tuner type to NTSC\n");
                    xf86_FI1236_set_tuner_type(pPriv->fi1236, TUNER_TYPE_FI1236);
                }
    }
    
    if(info->MM_TABLE_valid && (RADEON_tuners[info->MM_TABLE.tuner_type & 0x1f].type==TUNER_TYPE_MT2032)){
    if(!xf86LoadSubModule(pScrn,"tda9885"))
    {
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to initialize tda9885 driver\n");
    }
    else    
    {
    if(pPriv->tda9885 == NULL)
    {
        pPriv->tda9885 = xf86_Detect_tda9885(pPriv->i2c, TDA9885_ADDR_1);
    }
    if(pPriv->tda9885 == NULL)
    {
        pPriv->tda9885 = xf86_Detect_tda9885(pPriv->i2c, TDA9885_ADDR_2);
    }
    if(pPriv->tda9885 != NULL)
    {
        RADEON_TDA9885_Init(pPriv);
    }
    }
    }

	if(info->MM_TABLE_valid && ((RADEON_tuners[info->MM_TABLE.tuner_type & 0x1f].type==TUNER_TYPE_FM1216ME)
							|| (RADEON_tuners[info->MM_TABLE.tuner_type & 0x1f].type==TUNER_TYPE_FI1236W)))
	{
		if(!xf86LoadSubModule(pScrn,"tda9885"))
		{
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to initialize tda9885 driver\n");
		}
		else
		{
			if(pPriv->tda9885 == NULL)
			{
				pPriv->tda9885 = xf86_Detect_tda9885(pPriv->i2c, TDA9885_ADDR_1);
			}
			if(pPriv->tda9885 == NULL)
			{
				pPriv->tda9885 = xf86_Detect_tda9885(pPriv->i2c, TDA9885_ADDR_2);
			}
			if(pPriv->tda9885 != NULL)
			{
				RADEON_TDA9885_Init(pPriv);
				pPriv->fi1236->afc_source = (void*)pPriv->tda9885;
			}
		}
	}
	
	if(!xf86LoadSubModule(pScrn,"uda1380"))
	{
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to initialize uda1380 driver\n");
	}
	else    
	{
		if(pPriv->uda1380 == NULL)
		{
			pPriv->uda1380 = xf86_Detect_uda1380(pPriv->i2c, UDA1380_ADDR_1);
		}
		if(pPriv->uda1380 == NULL)
		{
			pPriv->uda1380 = xf86_Detect_uda1380(pPriv->i2c, UDA1380_ADDR_2);
		}
		if(pPriv->uda1380 != NULL)
		{
			xf86_uda1380_init(pPriv->uda1380);
		}
	}

    
    if(!xf86LoadSubModule(pScrn,"msp3430"))
    {
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to initialize msp3430 driver\n");
    } 
    else 
    {
    if(pPriv->msp3430 == NULL)
    {
       pPriv->msp3430 = xf86_DetectMSP3430(pPriv->i2c, MSP3430_ADDR_1);
    }
    if(pPriv->msp3430 == NULL)
    {
       pPriv->msp3430 = xf86_DetectMSP3430(pPriv->i2c, MSP3430_ADDR_2);
    }
#if 0 /* this would confuse bt829 with msp3430 */
    if(pPriv->msp3430 == NULL)
    {
       pPriv->msp3430 = xf86_DetectMSP3430(pPriv->i2c, MSP3430_ADDR_3);
    }
#endif
    }
    if(pPriv->msp3430 != NULL)
    {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Detected MSP3430 at 0x%02x\n", 
                 MSP3430_ADDR(pPriv->msp3430));
       pPriv->msp3430->standard = MSP3430_NTSC;
       pPriv->msp3430->connector = MSP3430_CONNECTOR_1;
       xf86_ResetMSP3430(pPriv->msp3430);
       xf86_InitMSP3430(pPriv->msp3430);
       xf86_MSP3430SetVolume(pPriv->msp3430, pPriv->mute ? MSP3430_FAST_MUTE : MSP3430_VOLUME(pPriv->volume));
    }
    
#if 0 /* put this back when saa7114 driver is ready */
    if(!xf86LoadSubModule(pScrn,"saa7114"))
    {
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to initialize saa7114 driver\n");
    } 
    else 
    {
    if(pPriv->saa7114 == NULL)
    {
       pPriv->saa7114 = xf86_DetectSAA7114(pPriv->i2c, SAA7114_ADDR_1);
    }
    if(pPriv->saa7114 == NULL)
    {
       pPriv->saa7114 = xf86_DetectSAA7114(pPriv->i2c, SAA7114_ADDR_2);
    }
    }
    if(pPriv->saa7114 != NULL)
    {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Detected SAA7114 at 0x%02x\n", 
                 pPriv->saa7114->d.SlaveAddr);
       xf86_InitSAA7114(pPriv->saa7114);
    }
#endif

}

static void RADEON_TDA9885_Init(RADEONPortPrivPtr pPriv)
{
TDA9885Ptr t=pPriv->tda9885;
t->sound_trap=0;
t->auto_mute_fm=1; /* ? */
t->carrier_mode=0; /* ??? */
t->modulation=2; /* negative FM */
t->forced_mute_audio=0;
t->port1=1;
t->port2=1;
t->top_adjustment=0x10;
t->deemphasis=1; 
t->audio_gain=0;
t->minimum_gain=0;
t->gating=0; 
t->vif_agc=1; /* set to 1 ? - depends on design */
t->gating=0; 
}
