/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/savage/savage_vbe.c,v 1.11 2002/05/14 20:19:52 alanh Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "savage_driver.h"
#include "savage_vbe.h"

#define iabs(a)	((int)(a)>0?(a):(-(a)))

#if X_BYTE_ORDER == X_LITTLE_ENDIAN
#define B_O16(x)  (x) 
#define B_O32(x)  (x)
#else
#define B_O16(x)  ((((x) & 0xff) << 8) | (((x) & 0xff) >> 8))
#define B_O32(x)  ((((x) & 0xff) << 24) | (((x) & 0xff00) << 8) \
                  | (((x) & 0xff0000) >> 8) | (((x) & 0xff000000) >> 24))
#endif
#define L_ADD(x)  (B_O32(x) & 0xffff) + ((B_O32(x) >> 12) & 0xffff00)

Bool vbeModeInit( vbeInfoPtr, int );
static int SavageGetDevice( SavagePtr psav );
/*static int SavageGetTVType( SavagePtr psav );*/
void SavageSetVESAModeCrtc1( SavagePtr psav, int n, int Refresh );
void SavageSetVESAModeCrtc2( SavagePtr psav, int n, int Refresh );

static void
SavageClearVM86Regs( xf86Int10InfoPtr pInt )
{
    pInt->ax = 0;
    pInt->bx = 0;
    pInt->cx = 0;
    pInt->dx = 0;
    pInt->si = 0;
    pInt->di = 0;
    pInt->es = 0xc000;
    pInt->num = 0x10;
}

void
SavageSetTextMode( SavagePtr psav )
{
    /* Restore display device if changed. */
    if( psav->iDevInfo != psav->iDevInfoPrim ) {
	SavageClearVM86Regs( psav->pVbe->pInt10 );
	psav->pVbe->pInt10->ax = 0x4f14;
	psav->pVbe->pInt10->bx = 0x0003;
	psav->pVbe->pInt10->cx = psav->iDevInfoPrim;
	xf86ExecX86int10( psav->pVbe->pInt10 );
    }

    SavageClearVM86Regs( psav->pVbe->pInt10 );

    psav->pVbe->pInt10->ax = 0x83;

    xf86ExecX86int10( psav->pVbe->pInt10 );
}

void
SavageSetVESAModeCrtc1(SavagePtr psav, int n, int refresh)
{
    unsigned char byte;

    xf86Msg(X_INFO,"SavageSetVESAModeCrtc1:mode=0x%x,refresh=%dHZ\n",n,refresh);

    SavageClearVM86Regs(psav->pVbe->pInt10);
    
    /* set active displays. */
    psav->pVbe->pInt10->ax = S3_EXTBIOS_INFO;
    psav->pVbe->pInt10->bx = S3_SET_ACTIVE_DISP;
    if (psav->TvOn)
    	psav->pVbe->pInt10->cx = 0x87; /* lcd, tv, crt, duoview */
    else
    	psav->pVbe->pInt10->cx = 0x83; /* lcd, crt, duoview */
    xf86ExecX86int10(psav->pVbe->pInt10);
    
    SavageClearVM86Regs(psav->pVbe->pInt10);
    
    /* Establish the refresh rate for this mode. */
    psav->pVbe->pInt10->ax = S3_EXTBIOS_INFO;
    psav->pVbe->pInt10->bx = S3_SET_REFRESH;
    psav->pVbe->pInt10->cx = n & 0x1ff;
    psav->pVbe->pInt10->di = refresh & 0xffff;
    xf86ExecX86int10(psav->pVbe->pInt10);

    /* SR01:turn off screen */
    OUTREG8 (SEQ_ADDRESS_REG,0x01);
    byte = INREG8(SEQ_DATA_REG) | 0x20;
    OUTREG8(SEQ_DATA_REG,byte);
    
    psav->pVbe->pInt10->ax = BIOS_SET_VBE_MODE;
    psav->pVbe->pInt10->bx = n;
    xf86ExecX86int10(psav->pVbe->pInt10);
    
}

void
SavageSetVESAModeCrtc2( SavagePtr psav, int n, int refresh )
{

    xf86Msg(X_INFO,"SavageSetVESAModeCrtc2:mode=0x%x,refresh=%dHZ\n",n,refresh);

    SavageClearVM86Regs(psav->pVbe->pInt10);

    UnLockExtRegs();
    
    psav->pVbe->pInt10->ax = S3_EXTBIOS_INFO;
    psav->pVbe->pInt10->bx = S3_ALT_SET_ACTIVE_DISP;
    if (psav->TvOn)
    	psav->pVbe->pInt10->cx = 0x87; /* lcd, tv, crt, duoview */
    else
    	psav->pVbe->pInt10->cx = 0x83; /* lcd, crt, duoview */
    psav->pVbe->pInt10->dx = n & 0x1ff;
    psav->pVbe->pInt10->di = refresh & 0xffff;
    xf86ExecX86int10(psav->pVbe->pInt10);

}

void
SavageSetVESAMode( SavagePtr psav, int n, int Refresh )
{
    int iDevInfo;
    static int iCount = 0;

    if (psav->IsSecondary) {
        SavageSetVESAModeCrtc2(psav, n, Refresh);
	return;
    }
    if (psav->IsPrimary) {
        SavageSetVESAModeCrtc1(psav, n, Refresh);
	return;
    }

    /* Get current display device status. */

    iDevInfo = SavageGetDevice(psav);
    psav->iDevInfo = iDevInfo;
    if( !iCount++ )
	psav->iDevInfoPrim = psav->iDevInfo;
    if( psav->CrtOnly )
	psav->iDevInfo = CRT_ACTIVE;
    if( psav->TvOn )
	psav->iDevInfo = TV_ACTIVE;

    /* Establish the refresh rate for this mode. */

    SavageClearVM86Regs( psav->pVbe->pInt10 );
    psav->pVbe->pInt10->ax = 0x4f14;	/* S3 extensions */
    psav->pVbe->pInt10->bx = 0x0001;	/* Set default refresh rate */
    psav->pVbe->pInt10->cx = n & 0x3fff;
    psav->pVbe->pInt10->di = Refresh & 0xffff;

    xf86ExecX86int10( psav->pVbe->pInt10 );

    /* Set TV type if TV is on. */
    if( psav->TvOn ) {
	SavageClearVM86Regs( psav->pVbe->pInt10 );
	psav->pVbe->pInt10->ax = 0x4f14;	/* S3 extensions */
	psav->pVbe->pInt10->bx = 0x0007;	/* TV extensions */
	psav->pVbe->pInt10->cx = psav->PAL ? 0x08 : 0x04;
	psav->pVbe->pInt10->dx = 0x0c;
	xf86ExecX86int10( psav->pVbe->pInt10 );
    }

    /* Manipulate output device set. */
    if( psav->iDevInfo != iDevInfo ) {
	SavageClearVM86Regs( psav->pVbe->pInt10 );
	psav->pVbe->pInt10->ax = 0x4f14;	/* S3 extensions */
	psav->pVbe->pInt10->bx = 0x0003;	/* set active devices */
	psav->pVbe->pInt10->cx = psav->iDevInfo;
	xf86ExecX86int10( psav->pVbe->pInt10 );

	/* Re-fetch actual device set. */
	psav->iDevInfo = SavageGetDevice( psav );
	iDevInfo = psav->iDevInfo;
	psav->CrtOnly = (iDevInfo == 1);
	psav->TvOn = !!(iDevInfo & 4);
    }

    /* Now, make this mode current. */

    if( xf86LoaderCheckSymbol( "VBESetVBEMode" ) )
    {
	if( !VBESetVBEMode( psav->pVbe, n, NULL ) )
	{
	    ErrorF("Set video mode failed\n");
	}
    }
#ifdef XFree86LOADER
    else
    {
	if( !vbeModeInit( psav->pVbe, n ) )
	{
	    ErrorF("Set video mode failed\n");
	}
    }
#endif
}

void
SavageSetPanelEnabled( SavagePtr psav, Bool active )
{
    int iDevInfo;
    if( !psav->PanelX )
	return; /* no panel */
    iDevInfo = SavageGetDevice( psav );
    if( active )
	iDevInfo |= LCD_ACTIVE;
    else
	iDevInfo &= ~LCD_ACTIVE;
    SavageClearVM86Regs( psav->pVbe->pInt10 );
    psav->pVbe->pInt10->ax = 0x4f14;	/* S3 extensions */
    psav->pVbe->pInt10->bx = 0x0003;	/* set active devices */
    psav->pVbe->pInt10->cx = iDevInfo;
    xf86ExecX86int10( psav->pVbe->pInt10 );
}

/* Function to get supported device list. */

static int SavageGetDevice( SavagePtr psav )
{
    SavageClearVM86Regs( psav->pVbe->pInt10 );
    psav->pVbe->pInt10->ax = 0x4f14;	/* S3 extensions */
    psav->pVbe->pInt10->bx = 0x0103;	/* get active devices */

    xf86ExecX86int10( psav->pVbe->pInt10 );

    return ((psav->pVbe->pInt10->cx) & 0xf);
}


void
SavageFreeBIOSModeTable( SavagePtr psav, SavageModeTablePtr* ppTable )
{
    int i;
    SavageModeEntryPtr pMode = (*ppTable)->Modes;

    for( i = (*ppTable)->NumModes; i--; )
    {
	if( pMode->RefreshRate )
	{
	    xfree( pMode->RefreshRate );
	    pMode->RefreshRate = NULL;
	}
	pMode++;
    }

    xfree( *ppTable );
}


SavageModeTablePtr
SavageGetBIOSModeTable( SavagePtr psav, int iDepth )
{
    int nModes = SavageGetBIOSModes( psav, iDepth, NULL );
    SavageModeTablePtr pTable;

    pTable = (SavageModeTablePtr) 
	xcalloc( 1, sizeof(SavageModeTableRec) + 
		    (nModes-1) * sizeof(SavageModeEntry) );
    if( pTable ) {
	pTable->NumModes = nModes;
	SavageGetBIOSModes( psav, iDepth, pTable->Modes );
    }

    return pTable;
}


unsigned short
SavageGetBIOSModes( 
    SavagePtr psav,
    int iDepth,
    SavageModeEntryPtr s3vModeTable )
{
    unsigned short iModeCount = 0;
    unsigned short int *mode_list;
    pointer vbeLinear = NULL;
    VbeInfoBlock *vbe;
    int vbeReal;
    struct vbe_mode_info_block * vmib;

    if( !psav->pVbe )
	return 0;

    vbeLinear = xf86Int10AllocPages( psav->pVbe->pInt10, 1, &vbeReal );
    if( !vbeLinear )
    {
	ErrorF( "Cannot allocate scratch page in real mode memory." );
	return 0;
    }
    vmib = (struct vbe_mode_info_block *) vbeLinear;
    
    if (!(vbe = VBEGetVBEInfo(psav->pVbe)))
	return 0;

    for (mode_list = vbe->VideoModePtr; *mode_list != 0xffff; mode_list++) {

	/*
	 * This is a HACK to work around what I believe is a BUG in the
	 * Toshiba Satellite BIOSes in 08/2000 and 09/2000.  The BIOS
	 * table for 1024x600 says it has six refresh rates, when in fact
	 * it only has 3.  When I ask for rate #4, the BIOS goes into an 
	 * infinite loop until the user interrupts it, usually by pressing
	 * Ctrl-Alt-F1.  For now, we'll just punt everything with a VESA
	 * number greater than or equal to 0200.
	 *
	 * This also prevents some strange and unusual results seen with
	 * the later ProSavage/PM133 BIOSes directly from S3/VIA.
	 */
	if( *mode_list >= 0x0200 )
	    continue;

	SavageClearVM86Regs( psav->pVbe->pInt10 );

	psav->pVbe->pInt10->ax = 0x4f01;
	psav->pVbe->pInt10->cx = *mode_list;
	psav->pVbe->pInt10->es = SEG_ADDR(vbeReal);
	psav->pVbe->pInt10->di = SEG_OFF(vbeReal);
	psav->pVbe->pInt10->num = 0x10;

	xf86ExecX86int10( psav->pVbe->pInt10 );

	if( 
	   (vmib->bits_per_pixel == iDepth) &&
	   (
	      (vmib->memory_model == VBE_MODEL_256) ||
	      (vmib->memory_model == VBE_MODEL_PACKED) ||
	      (vmib->memory_model == VBE_MODEL_RGB)
	   )
	)
	{
	    /* This mode is a match. */

	    iModeCount++;

	    /* If we're supposed to fetch information, do it now. */

	    if( s3vModeTable )
	    {
	        int iRefresh = 0;

		s3vModeTable->Width = vmib->x_resolution;
		s3vModeTable->Height = vmib->y_resolution;
		s3vModeTable->VesaMode = *mode_list;
		
		/* Query the refresh rates at this mode. */

		psav->pVbe->pInt10->cx = *mode_list;
		psav->pVbe->pInt10->dx = 0;

		do
		{
		    if( (iRefresh % 8) == 0 )
		    {
			if( s3vModeTable->RefreshRate )
			{
			    s3vModeTable->RefreshRate = (unsigned char *)
				xrealloc( 
				    s3vModeTable->RefreshRate,
				    (iRefresh+8) * sizeof(unsigned char)
				);
			}
			else
			{
			    s3vModeTable->RefreshRate = (unsigned char *)
				xcalloc( 
				    sizeof(unsigned char),
				    (iRefresh+8)
				);
			}
		    }

		    psav->pVbe->pInt10->ax = 0x4f14;	/* S3 extended functions */
		    psav->pVbe->pInt10->bx = 0x0201;	/* query refresh rates */
		    psav->pVbe->pInt10->num = 0x10;
		    xf86ExecX86int10( psav->pVbe->pInt10 );

		    s3vModeTable->RefreshRate[iRefresh++] = psav->pVbe->pInt10->di;
		}
		while( psav->pVbe->pInt10->dx );

		s3vModeTable->RefreshCount = iRefresh;

	    	s3vModeTable++;
	    }
	}
    }

    VBEFreeVBEInfo(vbe);

    xf86Int10FreePages( psav->pVbe->pInt10, vbeLinear, 1 );

    return iModeCount;
}

ModeStatus SavageMatchBiosMode(ScrnInfoPtr pScrn,int width,int height,int refresh,
                              unsigned int *vesaMode,unsigned int *newRefresh)
{
    SavageModeEntryPtr pmt;
    Bool found = FALSE;
    SavagePtr psav = SAVPTR(pScrn);    
    int i,j;
    unsigned int chosenVesaMode = 0;
    unsigned int chosenRefresh = 0;
    
    /* Scan through our BIOS list to locate the closest valid mode. */
    
    /*
     * If we ever break 4GHz clocks on video boards, we'll need to
     * change this.
     * refresh = (mode->Clock * 1000) / (mode->HTotal * mode->VTotal);
     * now we use VRefresh directly,instead of by calculating from dot clock
     */

    for( i = 0, pmt = psav->ModeTable->Modes; 
	i < psav->ModeTable->NumModes;
	i++, pmt++ )
    {
	if( (pmt->Width == width) && 
	    (pmt->Height == height) )
	{
	    int jDelta = 99;
	    int jBest = 0;

	    /* We have an acceptable mode.  Find a refresh rate. */
	    chosenVesaMode = pmt->VesaMode;
            if (vesaMode)
                *vesaMode = chosenVesaMode;
	    for( j = 0; j < pmt->RefreshCount; j++ )
	    {
		if( pmt->RefreshRate[j] == refresh )
		{
		    /* Exact match. */
		    jBest = j;
		    break;
		}
		else if( iabs(pmt->RefreshRate[j] - refresh) < jDelta )
		{
		    jDelta = iabs(pmt->RefreshRate[j] - refresh);
		    jBest = j;
		}
	    }
	    chosenRefresh = pmt->RefreshRate[jBest];
            if (newRefresh)
                *newRefresh = chosenRefresh;
            found = TRUE;
	    break;
	}
    }

    if( found ) {
	/* Success: we found a match in the BIOS. */
	xf86DrvMsg(pScrn->scrnIndex, X_PROBED, 
		  "Chose mode %x at %dHz.\n", chosenVesaMode, chosenRefresh );
        return MODE_OK;
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_PROBED, 
		  "No suitable BIOS mode found for %dx%d %dHz.\n",
		  width, height, refresh);
        return MODE_NOMODE;
    }
}
