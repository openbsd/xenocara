/*
 * Portions derived from  linux/drivers/video/sstfb.c 
 *	-- voodoo graphics frame buffer
 *     Copyright (c) 2000-2002 Ghozlane Toumi <gtoumi@laposte.net>
 *
 * Relicensed from GPL to the X license by consent of the author
 *
 * Other code Alan Cox (c) Copyright 2004 Red Hat Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of Red Hat, Alan Cox and Ghozlane Toumi
 * not be used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.  Th authors make no 
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL RICHARD HECKER BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 * 
 * THIS SOFTWARE IS NOT DESIGNED FOR USE IN SAFETY CRITICAL SYSTEMS OF
 * ANY KIND OR FORM.
 *
 *	Freely reviewably music for this hacking supplied by
 *		http://www.hoerstreich.de
 *		http://www.machinaesupremacy.com
 *		http://www.magnatune.com
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "fb.h"
#include "mibank.h"
#include "micmap.h"
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Version.h"
#include "xf86PciInfo.h"
#include "xf86Pci.h"
#include "xf86cmap.h"
#include "shadowfb.h"
#include "vgaHW.h"
#include "xf86DDC.h"
#include "xf86RAC.h"
#include "xf86Resources.h"
#include "xaa.h"
#include "compiler.h"

#include "voodoo.h"

#include <X11/extensions/xf86dgastr.h>

#include "opaque.h"
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#include "mipict.h"
#include "dixstruct.h"

static int debug = 0;

/*
 *	Big endian might need to byteswap these ?
 */
 
/*
 *	Write to chuck and bruce
 */
 
static CARD32 mmio32_r(VoodooPtr pVoo, int reg)
{
	volatile CARD32 *ptr = (CARD32 *)(pVoo->MMIO + reg);
	CARD32 val = *ptr;
	return val;
}

static void mmio32_w(VoodooPtr pVoo, int reg, CARD32 val)
{
	volatile CARD32 *ptr = (CARD32 *)(pVoo->MMIO + reg);
	*ptr = val;
}

/*
 *	Write to chuck only
 */
 
static void mmio32_w_chuck(VoodooPtr pVoo, int reg, CARD32 val)
{
	volatile CARD32 *ptr = (CARD32 *)(pVoo->MMIO + (reg |(1<<10)));
	*ptr = val;
}

/*
 *	Size the video RAM. Texture ram sizing is different and seperate
 *	but we don't use the texture ram for 2D anyway.
 */
 
int VoodooMemorySize(VoodooPtr pVoo)
{
	volatile CARD32 *fp = (volatile CARD32 *)pVoo->FBBase;
	fp[0] = 0xA5A5A5A5;
	fp[1<<18] = 0xA5A5A5A5;
	fp[1<<19] = 0xA5A5A5A5;
	
	fp[0] = 0x5A5A5A5A;
	
	if(fp[1<<19] == 0xA5A5A5A5)
		return 4;	/* 4Mb Card */
	if(fp[1<<18] == 0xA5A5A5A5)
		return 2;	/* 2Mb Card */
	return 1;
}

/*
 *	FIXME: If we are also doing 3D or DRI paths do we need
 *	to fix this path to issue a NOP command as per the chip errata? 
 */
 
static void wait_idle(VoodooPtr pVoo)
{
	int i = 0;
	
	while(i < 5)
	{
		if((mmio32_r(pVoo,0) & 0x80) == 0)
			i++;
	}
}
	
/*
 *	PCI control registers
 */

/*
 *	PCI enables. Used for setup, mode switch etc
 */ 

static void pci_enable(VoodooPtr pVoo, int wr, int dac, int fifo)
{
	CARD32 x;
	PCI_READ_LONG(pVoo->PciInfo, &x, 0x40);
	x &= ~7;
	x |= wr;
	x |= fifo<<1;
	x |= dac<<2;
	PCI_WRITE_LONG(pVoo->PciInfo, x, 0x40);
}

/*
 *	VClock control
 */
 
static void vclock_enable(VoodooPtr pVoo, int enable)
{
	if(enable)
		PCI_WRITE_LONG(pVoo->PciInfo, 0, 0xc0);
	else
		PCI_WRITE_LONG(pVoo->PciInfo, 0, 0xe0);
}

/*
 *	DPMS
 */
 
void VoodooBlank(VoodooPtr pVoo)
{
	vclock_enable(pVoo, 0);
	pci_enable(pVoo, 1, 0, 0);
	
	mmio32_w(pVoo, 0x214, mmio32_r(pVoo, 0x214) | 0x100);
	wait_idle(pVoo);
	
	mmio32_w(pVoo, 0x210, mmio32_r(pVoo, 0x210) | 6);
	wait_idle(pVoo);
	
	mmio32_w(pVoo, 0x218, mmio32_r(pVoo, 0x218) & ~(1<<22));
	wait_idle(pVoo);
}

/*
 *	Read and write the DAC registers
 */

static void dac_out(VoodooPtr pVoo, CARD32 reg, CARD32 val)
{
	mmio32_w(pVoo, 0x22C, (reg << 8) | val);
	wait_idle(pVoo);
}

static CARD32 dac_in(VoodooPtr pVoo, CARD32 reg)
{
	CARD32 r;
	mmio32_w(pVoo, 0x22C, (1<<11)|(reg<< 8));
	wait_idle(pVoo);
	r = mmio32_r(pVoo, 0x218) & 0xFF;
	return r;
}

/*
 *	Indexed (or should that be "demented") mode on the TI
 *	and AT&T DAC's
 */
 
static void dac_out_idx(VoodooPtr pVoo, CARD32 reg, CARD32 val)
{
	dac_out(pVoo, 0, reg);
	dac_out(pVoo, 2, val);
}

static CARD32 dac_in_idx(VoodooPtr pVoo, CARD32 reg)
{
	dac_out(pVoo, 0, reg);
	return dac_in(pVoo, 2);
}

/*
 *	We have generic ics5432 code we	perhaps ought to use instead. 
 *	This is based on the sstfb and Glide logic.
 */
 
#define ICS_PLL_CLK0_1_INI	0x55	/* Frequencies for detecting ICS */
#define ICS_PLL_CLK0_7_INI	0x71
#define ICS_PLL_CLK1_B_INI	0x79

static int probe_ics5432(VoodooPtr pVoo)
{
	int i;
	CARD32 m01, m07, m11;
	
	for(i = 0; i < 5; i++)
	{
		dac_out(pVoo, 7, 1);
		m01 = dac_in(pVoo,5);
		dac_in(pVoo,5);
		dac_out(pVoo,7, 7);
		m07 = dac_in(pVoo, 5);
		dac_in(pVoo,5);
		dac_out(pVoo,7, 11);
		m11 = dac_in(pVoo, 5);
		dac_in(pVoo, 5);
		
		if(m01 == ICS_PLL_CLK0_1_INI &&
		   m07 == ICS_PLL_CLK0_7_INI &&
		   m11 == ICS_PLL_CLK1_B_INI)
		   return 1;
	}
	return 0;
}

/*
 *	Activate backdoor on DACs
 */
 
static void dacdoor(VoodooPtr pVoo)
{
	dac_out(pVoo,0, 0);
	dac_in(pVoo,2);
	dac_in(pVoo,2);
	dac_in(pVoo,2);
	dac_in(pVoo,2);
}

/*
 *	Set a PLL on the DAC. Either vClock (0) or gClock (1)
 */

static void voodoo_set_pll(VoodooPtr pVoo, int pllnum)
{
	CARD32 cr0;
	if(pVoo->DAC == DAC_ID_ATT || pVoo->DAC == DAC_ID_TI)
	{
		CARD32 cc;
		dacdoor(pVoo);
		cr0 = dac_in(pVoo, 2);
		
		dacdoor(pVoo);
		dac_out(pVoo, 2, (cr0 & 0xF0) | 0xB);	/* Switch to index mode */
		
		/* Sleep a bit */
		usleep(300);
		
		cc = dac_in_idx(pVoo, 6);
		if(pllnum == 0)
		{
			dac_out_idx(pVoo, 0x48, pVoo->vClock.m);
			dac_out_idx(pVoo, 0x49, (pVoo->vClock.p << 6) | pVoo->vClock.n);
			dac_out_idx(pVoo, 6, (cc & 0x0F) | 0xA0 );
		}
		else
		{	
			dac_out_idx(pVoo, 0x6C, pVoo->gClock.m);
			dac_out_idx(pVoo, 0x6D, (pVoo->gClock.p << 6) | pVoo->vClock.n);
			dac_out_idx(pVoo, 6, (cc & 0x0F) | 0x0B);
		}
		return;
	}
	/* ICS5432 */
	dac_out(pVoo, 7, 14);
	cr0 = dac_in(pVoo, 5);
	if(pllnum == 0)
	{
		dac_out(pVoo, 4, 0);
		dac_out(pVoo, 5, pVoo->vClock.m);
		dac_out(pVoo, 5, pVoo->vClock.p << 5 | pVoo->vClock.n);
		dac_out(pVoo, 4, 14);
		dac_out(pVoo, 5, (cr0 & 0xD8) | (1<<5));
	}
	else
	{
		dac_out(pVoo, 4, 10);
		dac_out(pVoo, 5, pVoo->gClock.m);
		dac_out(pVoo, 5, pVoo->gClock.p << 5 | pVoo->gClock.n);
		dac_out(pVoo, 4, 14);
		dac_out(pVoo, 5, (cr0 & 0xEF));
	}
}

/*
 *	Set the depth in the DAC. This must match the frame
 *	buffer format. Right now we could hard code 16, in fact
 * 	it may be correct to always do so.. ?
 */
 
static void voodoo_set_depth(VoodooPtr pVoo, int depth)
{
	CARD32 cr0;
	
	if(pVoo->DAC == DAC_ID_ATT || pVoo->DAC == DAC_ID_TI)
	{
		dacdoor(pVoo);
		cr0 = dac_in(pVoo, 2);
		
		dacdoor(pVoo);
		
		if(depth == 16)
			dac_out(pVoo, 2, (cr0 & 0x0F) | 0x50);
		else if(depth ==24 || depth == 32)
			dac_out(pVoo, 2, (cr0 & 0x0F) | 0x70);
	}
	else if(pVoo->DAC == DAC_ID_ICS)
	{
		if(depth == 16)
			dac_out(pVoo, 6, 0x50);
		else
			dac_out(pVoo, 6, 0x70);
	}
}
		
static int voodoo_find_dac(VoodooPtr pVoo)
{
	CARD32 vendor_id, device_id;
	
#define DAC_VENDOR_ATT		0x84
#define DAC_DEVICE_ATT20C409	0x09
#define DAC_VENDOR_TI		0x97
#define DAC_DEVICE_TITVP3409	0x09

	dacdoor(pVoo);	
	dac_in(pVoo, 2);
	vendor_id = dac_in(pVoo, 2);
	device_id = dac_in(pVoo, 2);
	
	/* AT&T 20C409 and clones */
	if(vendor_id == DAC_VENDOR_ATT && DAC_DEVICE_ATT20C409)
		return DAC_ID_ATT;
		
	if(vendor_id == DAC_VENDOR_TI && DAC_DEVICE_TITVP3409)
		return DAC_ID_TI;
		
	/* ICS5432 doesn't implement the back door. Glide does some
	   quick tests to see if it is an ICS5432 just in case. */
	   
	if(probe_ics5432(pVoo))
		return DAC_ID_ICS;
	
	/* Shouldn't be any boards that get this far */
	ErrorF("Voodoo card with unknown DAC. Not supported.\n");
	return DAC_UNKNOWN;
}

/*
 *	Compute the PLL clock values. This is directly based on the
 *	technique used by sstfb. 
 */
 
/* compute the m,n,p  , returns the real freq
 * (ics datasheet :  N <-> N1 , P <-> N2)
 *
 * Fout= Fref * (M+2)/( 2^P * (N+2))
 *  we try to get close to the asked freq
 *  with P as high, and M as low as possible
 *  range:
 *	ti/att : 0 <= M <= 255; 0 <= P <= 3; 0<= N <= 63
 *	ics    : 1 <= M <= 127; 0 <= P <= 3; 1<= N <= 31
 *
 *  We will use the lowest limitation, should be precise enough
 */

static int iabs(int a)
{
	if(a >= 0)
		return a;
	return -a;
}

static int sst_calc_pll(const int freq, PLLClock *t)
{
	int m, m2, n, p, best_err, fout;
	int best_n=-1;
	int best_m=-1;

	best_err = freq;
	p=3;
	/* f * 2^P = vco should be less than VCOmax ~ 250 MHz for ics */
	while (((1 << p) * freq > 260000) && (p >= 0))
		p--;
	if (p == -1)
		return 0;
	for (n = 1; n < 32; n++) {
		/* Calc 2 * m so we can round it later */
		m2 = (2 * freq * (1 << p) * (n + 2) ) / 14318 - 4 ;

		m = (m2 % 2) ? m2/2+1 : m2/2 ;
		if (m >= 128)
			break;
		fout = (14318 * (m + 2)) / ((1 << p) * (n + 2));
		if (iabs(fout - freq) < best_err && m > 0) {	
			best_n = n;
			best_m = m;
			best_err = iabs(fout - freq);
			/* We get the lowest m , allowing 0.5% error in freq */
			if (200*best_err < freq)
				break;
		}
	}
	if (best_n == -1)  /* Unlikely, but who knows ? */
		return 0;
	t->p=p;
	t->n=best_n;
	t->m=best_m;
	return (14318 * (t->m + 2)) / ((1 << t->p) * (t->n + 2));
}


/*
 *		Here endeth the DAC code
 */
 

/*
 *		Higher level mode management
 */
 
static void voodoo_set_mode(VoodooPtr pVoo, DisplayModePtr mode)
{
	CARD32 hbporch = mode->CrtcHTotal - mode->CrtcHSyncEnd;
	CARD32 vbporch = mode->CrtcVTotal - mode->CrtcVSyncEnd;
	CARD32 hsyncstart = mode->CrtcHSyncEnd - mode->CrtcHSyncStart;
	CARD32 hsyncend = mode->CrtcHTotal - hsyncstart;
	CARD32 vsyncstart = mode->CrtcVSyncEnd - mode->CrtcVSyncStart;
	CARD32 vsyncend = mode->CrtcVTotal - vsyncstart;
	CARD32 width = mode->CrtcHDisplay;
	CARD32 height = mode->CrtcVDisplay;
	
	/* Even numbers of lines for interlace */
	if((mode->Flags & V_INTERLACE) && (vbporch & 1))
		vbporch += 1;
	/* Doublescan does what you expect */
	if(mode->Flags & V_DBLSCAN)
	{
		/* Not sure if we need to do all of these */
		vbporch <<=  1;
		hbporch <<= 1;
		hsyncend <<= 1;
		hsyncstart <<= 1;
		vsyncend <<= 1;
		vsyncstart <<= 1;
		width <<= 1;
		height <<= 1;
	}

	/* Write back porch values */	
	mmio32_w(pVoo, 0x208, (vbporch << 16) | (hbporch - 2));
	/* Write displayed area */
	mmio32_w(pVoo, 0x20C, (height << 16) | (width - 1));
	mmio32_w(pVoo, 0x220, (hsyncend - 1 ) << 16 | (hsyncstart - 1));
	mmio32_w(pVoo, 0x224, (vsyncend << 16) | vsyncstart);
}

/*
 *	Set up a Voodoo video mode. Note that we do need to have
 *	save/restore functions here too because the user might be 
 *	using sstfb... Eventually..
 */

int VoodooMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
	CARD32 f1, f2, f3;
	VoodooPtr pVoo = VoodooPTR(pScrn);
	int tiles;

	/* FIXME: Compute PLL and check */
	
	sst_calc_pll(mode->SynthClock, &pVoo->vClock);
		
	mmio32_w(pVoo, 0x120, 0);	/* NOP */
	wait_idle(pVoo);
	pci_enable(pVoo, 1, 0, 0);
	mmio32_w(pVoo, 0x214, mmio32_r(pVoo,0x214)|(1<<8));
	mmio32_w(pVoo, 0x210, mmio32_r(pVoo,0x210)|6);
	mmio32_w(pVoo, 0x218, mmio32_r(pVoo,0x218) & ~(1<<22));
	wait_idle(pVoo);

	voodoo_set_mode(pVoo, mode);
	
	f2 = mmio32_r(pVoo,0x218);
	f3 = mmio32_r(pVoo,0x21c);

	pci_enable(pVoo, 1, 1, 0);	
	
	voodoo_set_depth(pVoo, 16);	/* Hardware output is always 16bpp */
	voodoo_set_pll(pVoo, 0);
	
	pci_enable(pVoo, 1, 0, 0);
	
	mmio32_w(pVoo, 0x218, f2);
	mmio32_w(pVoo, 0x21C, f3);
	
	f1 = mmio32_r(pVoo,0x214);
	
	f1 &= 0x8080010F;		/* Mask off video bits */
	f1 |= 0x21E000;			/* Enable blanking, data, vsync, dclock 2x sel */
	
	/* Number of 64 pixel tiles */
	tiles = (mode->CrtcHDisplay + 63) / 64;
	
	if(pVoo->Voodoo2)
		f1|= ((tiles & 0x10) ? 1<<24 : 0) | (tiles & 0x0f) << 4;
	else
		f1|= tiles << 4;
		
	pVoo->Tiles = tiles * 2;
	pVoo->Width = mode->CrtcHDisplay;
	pVoo->Height = mode->CrtcVDisplay;
	if(!pVoo->Accel)
		pVoo->FullHeight = mode->CrtcVDisplay;
		 
	mmio32_w(pVoo, 0x214, f1);
	
	/* Voodoo 2 support */
	if(pVoo->Voodoo2)
	{
		CARD32 f5 = mmio32_r(pVoo, 0x244);
		mmio32_w(pVoo, 0x248, 0);
		
		f5 &= ~0x05BF0000;
		
		f5 &= ~(1<<22);	/* For now */

		if(mode->Flags & V_INTERLACE)
			f5 |= 1<<26;
		/* FIXME: is this H, V or both doublescan ?? */
		if(mode->Flags & V_DBLSCAN)
			f5 |= (1<<21) | (1<<20);
		if(mode->Flags & V_PHSYNC)
			f5 |= (1<<23);
		if(mode->Flags & V_PVSYNC)
			f5 |= (1<<24);
		mmio32_w(pVoo, 0x244, f5);
	}
	wait_idle(pVoo);
	mmio32_w(pVoo, 0x214, mmio32_r(pVoo,0x214) & ~0x100);
	mmio32_w(pVoo, 0x210, 1 |  (mmio32_r(pVoo,0x210) & ~6));
	mmio32_w(pVoo, 0x218, mmio32_r(pVoo,0x218) | (1<<22));
	
	pci_enable(pVoo, 0, 0, 1);

	mmio32_w(pVoo, 0x114, 0x100);
	pVoo->lfbMode = 0x100;	/* Cached LFB mode, front, front, on */

	/*
	 *	Set a clipping rectangle. We really deeply emphatically
	 *	don't want to write off screen otherwise.
	 */
	mmio32_w(pVoo, 0x118, mode->CrtcHDisplay);	
	mmio32_w(pVoo, 0x11C, mode->CrtcVDisplay);
	mmio32_w(pVoo, 0x110, /*mmio32_r(pVoo, 0x110) | */0x201);

	/*
	 *	Set up the 2D engine drawing logic
	 */
	
	if(pVoo->Voodoo2)
	{
		mmio32_w_chuck(pVoo, 0x2C0, 0);	/* Zero source */
		mmio32_w_chuck(pVoo, 0x2C4, 0);	/* Zero destination */
		/* Use screen sized tiles for src/dst */
		mmio32_w_chuck(pVoo, 0x2C8, (pVoo->Tiles << 16) | pVoo->Tiles);
		mmio32_w_chuck(pVoo, 0x2D4, pVoo->Width);
		mmio32_w_chuck(pVoo, 0x2D8, pVoo->FullHeight);
	}
	return 0;
}

/*
 *	Probe and initialize a Voodoo 1 or Voodoo 2 that we found.
 *	We don't set up any video mode at this point.
 */
 
int VoodooHardwareInit(VoodooPtr pVoo)
{
	vclock_enable(pVoo, 0);
	pci_enable(pVoo, 1, 0, 0);
	
	mmio32_w(pVoo, 0x214, mmio32_r(pVoo, 0x214) | 0x100);
	wait_idle(pVoo);

 	mmio32_w(pVoo, 0x210, mmio32_r(pVoo, 0x210) | 7); 
	wait_idle(pVoo);
	
	mmio32_w(pVoo, 0x218, mmio32_r(pVoo, 0x218) & ~(1<<22));
	wait_idle(pVoo);
	
	/* At this point we are basically shut down */
	
	pci_enable(pVoo, 1, 1, 0);
	
	pVoo->DAC = voodoo_find_dac(pVoo);
	
	/* Graphics clock depends on the board */
	pVoo->MaxClock = 50000;
	if(pVoo->Voodoo2)
		pVoo->MaxClock = 75000;
	
	sst_calc_pll(pVoo->MaxClock, &pVoo->gClock);
	voodoo_set_pll(pVoo, 1);

	pci_enable(pVoo, 1, 0, 1);	
	
	mmio32_w(pVoo, 0x210, 0);		/* 1 for VGA pass through */
	wait_idle(pVoo);
	mmio32_w(pVoo, 0x214, 0x1A8 | (1<<21) );
	wait_idle(pVoo);
	mmio32_w(pVoo, 0x218, 0x186000E0);	/* RAM setup with fast ras, oe, fifo, refresh on and a 16mS refresh */
	wait_idle(pVoo);
	mmio32_w(pVoo, 0x21C, 0x40);		/* Need to review with DRI */
	wait_idle(pVoo);
	mmio32_w(pVoo, 0x200, 2);
	wait_idle(pVoo);
	
	if(pVoo->Voodoo2)
	{
		mmio32_w(pVoo, 0x248, 0);
		wait_idle(pVoo);
	}

	pci_enable(pVoo, 0, 0, 1);
	vclock_enable(pVoo, 1);
	
	return 0;
}	

/*
 *	Copiers for Voodoo1
 *
 *	Voodoo1 has no CPU to screen blit, and also lacks SGRAM fill
 */
 
void VoodooCopy16(VoodooPtr pVoo, CARD32 x1, CARD32 y1, CARD32 w, CARD32 h, CARD32 spitch, unsigned char *src)
{
	/* DWord copy pointer */
	CARD32 *out = (CARD32 *)(pVoo->FBBase + y1 * pVoo->Pitch + x1 *2);
	CARD32 *in = (CARD32 *)src;
	/* DWords to skip */
	CARD32 skipo = (pVoo->Pitch - w*2)>>2;
	CARD32 skipi = (pVoo->ShadowPitch - w*2)>>2;
	int ct;
	
	/* LFB will do all our work for us */

	mmio32_w(pVoo, 0x10C, 0);
	mmio32_w(pVoo, 0x110, (1<<9)|1);
	mmio32_w(pVoo, 0x114, (1<<8));
	while(h > 0)
	{
		for(ct = 0; ct < w; ct+=2)
			*out++=*in++;
		in += skipi;
		out += skipo;
		h--;
	}
}

/*
 *	Copiers for Voodoo 1 and Voodoo 2 24bit
 *
 *	Voodoo1 has no CPU to screen blit, and also lacks SGRAM fill
 *	Voodoo2 has no read side 24bit, nor 24bit accelerator
 */
 
void VoodooCopy24(VoodooPtr pVoo, CARD32 x1, CARD32 y1, CARD32 w, CARD32 h, CARD32 spitch, unsigned char *src)
{
	/* DWord copy pointer */
	CARD32 *out = (CARD32 *)(pVoo->FBBase + y1 * pVoo->Pitch + x1 * 4);
	CARD32 *in = (CARD32 *)src;
	/* DWords to skip */
	CARD32 skipo = (pVoo->Pitch - w*4)>>2;
	CARD32 skipi = (pVoo->ShadowPitch - w*4)>>2;
	int ct;
	
	/* LFB will do all our work for us */
	/* FIXME: Should we allow dither options ? */
	
	mmio32_w(pVoo, 0x10C, 0);
	mmio32_w(pVoo, 0x110, (1<<9)|1);
	mmio32_w(pVoo, 0x114, (1<<8)|4);
	while(h > 0)
	{
		for(ct = 0; ct < w; ct++)
			*out++=*in++;
		in += skipi;
		out += skipo;
		h--;
	}
}

/*
 *	Use hardware clear. For Voodoo2 we want to use the SGRAM fill
 *	really. fbzMode bits 9/10 write to depth buffer, bit 14/15 to select
 *	the right buffer, bit 9 off for no dither, depth in zacolor, bits in
 *	colour1.
 *
 *	load cliplr/lyhy and then fire.
 */
 
void VoodooClear(VoodooPtr pVoo)
{
	memset(pVoo->FBBase,0, 0x400000);
#if 0
	/*
	 *	We can't do this as the 3D engine setup is not
	 *	done by this driver..
	 */
	mmio32_w(pVoo, 0x130, 0);		/* No Alpha ! */
	
	mmio32_w(pVoo, 0x118, pVoo->Width);
	mmio32_w(pVoo, 0x11C, pVoo->Height << 16);
	/* On entry we know Clip is set correctly so we wil clear the lot */
	mmio32_w(pVoo, 0x148, 0xC0C0C0);	/* RGB888 black */
	mmio32_w(pVoo, 0x130, 0xFFFF);	/* I think ?? */
	/* We want to write to screen and depth, front buffer, and no dither */
	mmio32_w(pVoo, 0x110, (mmio32_r(pVoo, 0x110) | 0x601) & 0xFFE00EE1);
	/* Fire */
	mmio32_w(pVoo, 0x124, 1);
	wait_idle(pVoo);
	/* In case X decides to read the LFB before clear finishes */
#endif	
}

/*
 *	Voodoo banking. The voodoo isn't exactly banked in the conventional
 *	sense but claiming to be banked allows us to use the back buffer
 *	as the pixcache, providing we are careful with our acceleration
 *	ops
 *
 *	There are two banks. Bank 0 is the front buffer, bank 1 is the
 *	back buffer. We don't use the aux buffer. Additionally the
 *	back bufer in 1024x768 with 2Mbyte cards is only a partial buffer.
 *	(No SLI yet)
 *
 *	Not yet used (TODO: figure out the offsets for the backbuffer layout)
 *	For now we work the screen as one deep display instead
 */
 
void VoodooReadBank(ScreenPtr pScreen, int bank)
{
	ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
	VoodooPtr pVoo = VoodooPTR(pScrn);
	if(bank)
	{
		mmio32_w(pVoo, 0x2C0, pVoo->Height);
		pVoo->lfbMode |= (1<<6);
	}
	else
	{
		mmio32_w(pVoo, 0x2C0, 0);
		pVoo->lfbMode &= ~(1<<6);
	}
	mmio32_w(pVoo, 0x114, pVoo->lfbMode);
}

void VoodooWriteBank(ScreenPtr pScreen, int bank)
{
	ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
	VoodooPtr pVoo = VoodooPTR(pScrn);
	if(bank)
	{
		mmio32_w(pVoo, 0x2C4, pVoo->Height);
		pVoo->lfbMode |= (1<<4);
	}
	else
	{
		mmio32_w(pVoo, 0x2C4, 0);
		pVoo->lfbMode &= ~(1<<4);
	}
	mmio32_w(pVoo, 0x114, pVoo->lfbMode);
}

void VoodooReadWriteBank(ScreenPtr pScreen, int bank)
{
	ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
	VoodooPtr pVoo = VoodooPTR(pScrn);
	if(bank)
	{
		mmio32_w(pVoo, 0x2C0, pVoo->Height);
		mmio32_w(pVoo, 0x2C4, pVoo->Height);
		pVoo->lfbMode |= (1<<4) | (1<<6);
	}
	else
	{
		mmio32_w(pVoo, 0x2C0, 0);
		mmio32_w(pVoo, 0x2C4, 0);
		pVoo->lfbMode &= ~((1<<4) | (1<<6));
	}
	mmio32_w(pVoo, 0x114, pVoo->lfbMode);
}

/*
 *	We normally want to load all four rop variants at once so
 *	the table is the 16bits for the lot equal.
 */

static CARD16 ropxlate[16] = {
	0x0000, 	/* GXclear */
	0x8888,		/* GXand */
	0x4444,		/* GXandReverse */
	0xCCCC,		/* GXcopy */
	0x2222,		/* GXandInverted */
	0xAAAA,		/* GXnop */
	0x6666,		/* GXxor */
	0xEEEE,		/* GXor */
	0x1111,		/* GXnor */
	0x9999,		/* GXequiv */
	0x5555,		/* GXinvert */
	0xDDDD,		/* GXorReverse */
	0x3333,		/* GXcopyInverted */
	0xBBBB,		/* GXorInverted */
	0x7777,		/* GXnand */
	0xFFFF		/* GXset */
};

/*
 *	Transparent mask rops
 */

static CARD16 tropxlate[16] = {
	0xAA00, 	/* GXclear */
	0xAA88,		/* GXand */
	0xAA44,		/* GXandReverse */
	0xAACC,		/* GXcopy */
	0xAA22,		/* GXandInverted */
	0xAAAA,		/* GXnop */
	0xAA66,		/* GXxor */
	0xAAEE,		/* GXor */
	0xAA11,		/* GXnor */
	0xAA99,		/* GXequiv */
	0xAA55,		/* GXinvert */
	0xAADD,		/* GXorReverse */
	0xAA33,		/* GXcopyInverted */
	0xAABB,		/* GXorInverted */
	0xAA77,		/* GXnand */
	0xAAFF		/* GXset */
};

	
void VoodooSync(ScrnInfoPtr pScrn)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	/* FIXME: Check if cliprect dirty if so rewrite */
	wait_idle(pVoo);
	mmio32_w(pVoo, 0x10C, 0);	/* Maybe flag this */
}

static void Voodoo2Setup2D(VoodooPtr pVoo)
{
	wait_idle(pVoo);
}

static void Voodoo2SetupForScreenToScreenCopy(ScrnInfoPtr pScrn,
			int xdir, int ydir, int rop,
			unsigned int planemask,
			int trans_color)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	Voodoo2Setup2D(pVoo);
	pVoo->BlitDirX = xdir;
	pVoo->BlitDirY = ydir;
	
	if(trans_color == -1)
	{
		mmio32_w_chuck(pVoo, 0x2EC, ropxlate[rop]);	/* Set the rop */
		mmio32_w_chuck(pVoo, 0x2F8, 0 | (1<<14) | (1<<15) | (1<<16));	/* 16bpp no color compare */
	}
	else
	{
		mmio32_w_chuck(pVoo, 0x2EC, tropxlate[rop]);	 /* Transparent src rop */
		mmio32_w_chuck(pVoo, 0x2CC, (trans_color << 16) | trans_color);	/* Match transparent colour */
		mmio32_w_chuck(pVoo, 0x2F8, 0 | (1<<10) | (1<<14) | (1<<15) | (1<<16));	/* 16bpp color compare */
	}
}

static void Voodoo2SubsequentScreenToScreenCopy(ScrnInfoPtr pScrn,
			int x1, int y1,
			int x2, int y2,
			int width, int height)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	wait_idle(pVoo);
	/* Adjust co-ordinates for backward blits */
	height --;	/* Adjust for fenceposting in the hardware */
	width --;
	if(pVoo->BlitDirY < 0)
	{
		y1 += height;
		y2 += height;
		height = -height;
	}
	if(pVoo->BlitDirY < 0)
	{
		x1 += width;
		x2 += width;
		width = -width;
	}
	mmio32_w_chuck(pVoo, 0x2E0, (y1 << 16) | x1);  /* Src x/y */
	mmio32_w_chuck(pVoo, 0x2E4, (y2 << 16) | x2);  /* Dst x/y */
	/* Set size and fire */
	height &= 0xFFF;
	width  &= 0xFFF;
	mmio32_w_chuck(pVoo, 0x2E8, (height << 16) | width | (1<<31));
}

static void Voodoo2SetupForSolidFill(ScrnInfoPtr pScrn, int color,
			int rop, unsigned int planemask)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	Voodoo2Setup2D(pVoo);
	mmio32_w_chuck(pVoo, 0x2EC, ropxlate[rop]); 	/* rop */
	mmio32_w_chuck(pVoo, 0x2F0, color);		/* fg color */
	mmio32_w_chuck(pVoo, 0x2F8, 2 | (1<<14) | (1<<15) | (0/*1*/<<16)); /* Solid fill 16bpp front */
}

static void Voodoo2SubsequentSolidFillRect(ScrnInfoPtr pScrn, int x, int y, 
			int w, int h)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	wait_idle(pVoo);
	mmio32_w_chuck(pVoo, 0x2E4, (y<<16) | x);	/* Dst x,y */
	/* Set size and fire */
	mmio32_w_chuck(pVoo, 0x2E8, ((h-1) << 16) | (w-1) | (1<<31));
}


/*
 *	Colour expand fills are standard hardware goodies
 */
 
static void Voodoo2SetupForScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
			int fg, int bg,
			int rop,
			unsigned int planemask)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	Voodoo2Setup2D(pVoo);
	mmio32_w_chuck(pVoo, 0x2EC, ropxlate[rop]);	/* Pattern op */
	mmio32_w_chuck(pVoo, 0x2F0, fg | (bg << 16));	/* colors */
	if(bg != -1)	/* Set transparent if needed */
		mmio32_w_chuck(pVoo, 0x2F8, 1 | (1<<14) | (1<<15) | (1<<16));
	else
		mmio32_w_chuck(pVoo, 0x2F8, 1 | (1<<14) | (1<<15) | (1<<16) | (1<<17));
}

static void Voodoo2SubsequentScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
			int x, int y, int w, int h,
			int skipleft)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	wait_idle(pVoo);
	mmio32_w_chuck(pVoo, 0x2E4,  x | (y<<16));			/* destination */
	mmio32_w_chuck(pVoo, 0x2E8, (w-1) | ((h-1)<<16) | (1<<31)); 	/* fire */
	pVoo->texW = w;
}

static void Voodoo2SubsequentColorExpandScanline(ScrnInfoPtr pScrn, int bufno)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	CARD32 *data = (CARD32 *)pVoo->LineBuffer;
	int w = pVoo->texW;
	int i;
	
	wait_idle(pVoo);
	for(i = 0; i < w; i += 32)	/* Each dword is 32 pixels mask */
		mmio32_w(pVoo, 0x2FC, *data++);
}

static void Voodoo2SetupForMono8x8PatternFill(ScrnInfoPtr pScrn, int patx, int paty,
	int fg, int bg, int rop, unsigned int planemask)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	Voodoo2Setup2D(pVoo);
	mmio32_w_chuck(pVoo, 0x2EC, ropxlate[rop]);
	mmio32_w_chuck(pVoo, 0x2F0, fg | (bg << 16));
	if(bg != -1)
		mmio32_w_chuck(pVoo, 0x2F8, 1 | (1<<14) | (1<<15) | (1<<16));
	else
		mmio32_w_chuck(pVoo, 0x2F8, 1 | (1<<14) | (1<<15) | (1<<16) | (1<<17));
}

/*
 *	We don't have pattern fill hardware but for any operation that
 *	references dst it is going to be faster to use the hardware
 *	and simply upload the pattern a lot as we avoid reading
 *	video memory. GXcopy ought to be the same either way.
 */
 
static __inline__ CARD32 spread(CARD32 v)
{
	return v * 16843009;
}

static void Voodoo2SubsequentMono8x8PatternFillRect(ScrnInfoPtr pScrn, int patx, int paty,
	int x, int y, int w, int h)
{
	int ln = 0;
	CARD32 l[8];
	VoodooPtr pVoo = VoodooPTR(pScrn);

	wait_idle(pVoo);
	
	if( w <3)
		return;
	
	mmio32_w_chuck(pVoo, 0x2E4,  x | (y<<16));
	mmio32_w_chuck(pVoo, 0x2E8, (w - 1) | ((h - 1)<<16) | (1<<31));
	
	/* Turn the pattern into 32x8 for the expansion engine */
	l[0] = spread((patx >> 24) & 0xFF);
	l[1] = spread((patx >> 16) & 0xFF);
	l[2] = spread((patx >> 8)  & 0xFF);
	l[3] = spread(patx & 0xFF);
	
	l[4] = spread((paty >> 24) & 0xFF);
	l[5] = spread((paty >> 16) & 0xFF);
	l[6] = spread((paty >> 8)  & 0xFF);
	l[7] = spread(paty & 0xFF);
	
	while(h > 0)
	{
		int i;
		for(i = 0; i < w; i += 32) /* DWORD pad */
			mmio32_w_chuck(pVoo, 0x2FC, l[ln]);
		wait_idle(pVoo);
		ln = (ln + 1) & 7;
		h--;
	}
}

/*
 *	The XAA layer uses video memory as the basis for colour pattern
 *	fill, so we can't usefully perform it. 
 */
 
static void Voodoo2SetupForSolidLine(ScrnInfoPtr pScrn, int color, int rop,
	unsigned int planemask)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	Voodoo2Setup2D(pVoo);
	mmio32_w_chuck(pVoo, 0x2CC, color);
	mmio32_w_chuck(pVoo, 0x2EC, ropxlate[rop]);
	mmio32_w_chuck(pVoo, 0x2F8, 2 | (1<<14) | (1<<15) | (1<<16)); 	/* Solid fill 16 bpp front */
}	

static void Voodoo2SubsequentSolidHorVertLine(ScrnInfoPtr pScrn, int x, int y, int len, int dir)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	wait_idle(pVoo);
	mmio32_w_chuck(pVoo, 0x2E4, (y<<16) | x);	/* Dst x,y */
	if(dir == DEGREES_0)
		mmio32_w_chuck(pVoo, 0x2E8,  (len - 1) | (1<<31));
	else
		mmio32_w_chuck(pVoo, 0x2E8, ((len - 1) << 16) | (1<<31));
}
	
static void Voodoo2SetupForScanlineImageWrite(ScrnInfoPtr pScrn, int rop,
			unsigned int planemask, int trans_color,
			int bpp, int depth)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	Voodoo2Setup2D(pVoo);
	if(trans_color != -1)
	{
		mmio32_w_chuck(pVoo, 0x2CC, (trans_color << 16) | trans_color);
		mmio32_w_chuck(pVoo, 0x2EC, tropxlate[rop]);
		mmio32_w_chuck(pVoo, 0x2F8, 1 | (2<<3) | (1<<10) | (1<<14) | (1<<15) | (1<<16));
	}
	else
	{
		mmio32_w_chuck(pVoo, 0x2EC, ropxlate[rop]);
		mmio32_w_chuck(pVoo, 0x2F8, 1 | (2<<3) | (1<<14) | (1<<15) | (1<<16));
	}
	if(debug)
		ErrorF("Setup for image write rop %d col %d bpp %d depth %d\n",
			rop, trans_color, bpp, depth);
}

static void Voodoo2SubsequentImageWriteRect(ScrnInfoPtr pScrn, 
			int x, int y,
			int w, int h,
			int skipleft)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	wait_idle(pVoo);
	mmio32_w_chuck(pVoo, 0x2E4, x | (y<<16));
	mmio32_w_chuck(pVoo, 0x2E8, (w - 1) | ((h - 1)<<16) | (1<<31));
	if(debug)
		ErrorF("Image Write (%d,%d) [%d,%d]\n", x,y,w,h);
	pVoo->texW = w;
}

static void Voodoo2SubsequentImageWriteScanline(ScrnInfoPtr pScrn, int bufno)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	CARD32 *data = (CARD32 *)pVoo->LineBuffer;
	int w = pVoo->texW;
	int i;
	
	wait_idle(pVoo);
	for(i = 0; i < w; i += 2)
		mmio32_w(pVoo, 0x2FC, *data++);
}

static void Voodoo2SetClippingRectangle(ScrnInfoPtr pScrn,
			int left, int top, int right, int bottom)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	if(debug)
		ErrorF("Clip to (%d,%d)-(%d,%d)\n", left,top,right,bottom);
	mmio32_w_chuck(pVoo, 0x2D4, (left << 16) | right);
	mmio32_w_chuck(pVoo, 0x2D8, (top << 16 ) | bottom);
}

static void Voodoo2DisableClipping(ScrnInfoPtr pScrn)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	/* FIXME: pVoo->FullHeight for the cache ! */
	if(debug)
		ErrorF("Clip to (0,0)-(%d,%d)\n", (int)pVoo->Width, (int)pVoo->Height);
	mmio32_w_chuck(pVoo, 0x2D4, pVoo->Width);
	mmio32_w_chuck(pVoo, 0x2D8, pVoo->FullHeight);
}

/*
 *	TODO: Implement 2D line acceleration using the 3D engines
 */
 
#ifdef RENDER

/*
 *	Render acceleration. All Voodoo chips support cpu driver alpha
 *	composite to the frame buffer. This is presumably meant for software
 *	fallbacks on rendering 3D but happens to be very useful to avoid
 *	some render operations reading from the frame buffer as much
 *
 *	Possibly we could the 3D engine for this once we get it working.
 *	We can't however use the 2D engine much as it lacks Alpha
 */
 

Bool VoodooSetupForCPUToScreenAlphaTexture(ScrnInfoPtr pScrn, int op, CARD16 red, 
	CARD16 green, CARD16 blue, CARD16 alpha, int alphaType, CARD8 *alphaPtr,
	int alphaPitch,	int width, int height, int flags)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);

	pVoo->alphaType = alphaType;
	pVoo->alphaPitch = alphaPitch;
	pVoo->alphaPtr = alphaPtr;
	pVoo->alphaW = width;
	pVoo->alphaH = height;
	pVoo->alphaC = (red & 0xFF00)  << 8 | (green & 0xFF00) | blue >> 8;
	
	if(op != PictOpOver && op != PictOpSrc)
		return FALSE;

	if(debug)
		ErrorF("Supported CPU To Screen Alpha Texture (%d) -> %d,%d\n", op, width, height);		
	wait_idle(pVoo);
	if(op == PictOpSrc)
		pVoo->alpha = 0;
	else	/* dst = src * srcalpha + (1-a) * dst */
		pVoo->alpha = (1<<4) | (1<<8) | (5<<12);

	return TRUE;	
}	

void VoodooSubsequentCPUToScreenAlphaTexture(ScrnInfoPtr pScrn, int dstx, int dsty, int srcx, int srcy, int width, int height)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	/* 32bit LFB write mode */
	CARD32 *fb = (CARD32 *)(pVoo->FBBase + 4096 * dsty + 4 * dstx);
	CARD8 *db = pVoo->alphaPtr + pVoo->alphaW * srcy + srcx;
	int x, y;
	CARD32 *fdb;
	CARD8 *cdb;
	CARD32 colour = pVoo->alphaC;
	int dw, dh;
	int w, h;

	mmio32_w(pVoo, 0x10C, pVoo->alpha);
	mmio32_w(pVoo, 0x110, 1 | (1<<9));
	mmio32_w(pVoo, 0x114, (1<<8) | 5);	/* ARGB888 */

	dh = srcy;
	w = pVoo->alphaW;
	h = pVoo->alphaH;
	
	for(y = 0; y < height; y++)
	{
		cdb = db;
		fdb = fb;

		dw = srcx;
		for(x = 0; x < width; x++)
		{
			*fdb++ = (*cdb++<< 24) | colour;
			if(++dw == w)
			{
				dw = 0;
				cdb -= pVoo->alphaW;
			}
		}
		db += pVoo->alphaW;
		fb += 1024;
		if(++dh == h)
		{
			db = pVoo->alphaPtr + srcx;
			dh = 0;
		}
	}	
	mmio32_w(pVoo, 0x114, pVoo->lfbMode);
	mmio32_w(pVoo, 0x10C, 0);
}

Bool VoodooSetupForCPUToScreenTexture(ScrnInfoPtr pScrn, int op, int texType,
	CARD8 *texPtr, int texPitch, int width, int height, int flags)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	
	if(op != PictOpOver && op != PictOpSrc)
		return FALSE;	/* For now */

	if(debug)
		ErrorF("Supported CPU TO Screen Texture (%d) -> %d,%d\n", op, width, height);		
	pVoo->texType = texType;
	pVoo->texPitch = texPitch;
	pVoo->texPtr = texPtr;
	pVoo->texW = width;
	pVoo->texH = height;

	wait_idle(pVoo);
	if(op == PictOpSrc || texType == PICT_x8r8g8b8)
		pVoo->alpha = 0;
	else
		pVoo->alpha = (1<<4) | (1<<8) | (5<<12);
		
	return TRUE;
}

void VoodooSubsequentCPUToScreenTexture(ScrnInfoPtr pScrn, int dstx, int dsty, int srcx, int srcy, int width, int height)
{
	VoodooPtr pVoo = VoodooPTR(pScrn);
	/* 32bit LFB write mode */
	CARD32 *fb = (CARD32 *)(pVoo->FBBase + 4096 * dsty + 4 * dstx);
	CARD32 *db = ((CARD32 *)(pVoo->texPtr)) + pVoo->texW * srcy + srcx;
	int x, y;
	CARD32 *cdb, *fdb;
	int dw, dh;
	int w, h;

	mmio32_w(pVoo, 0x10C, pVoo->alpha);
	mmio32_w(pVoo, 0x110, 1 | (1<<9));
	
	if(pVoo->texType == PICT_a8r8g8b8)
		mmio32_w(pVoo, 0x114, (1<<8) | 5);	/* ARGB888 */
	else if(pVoo->texType == PICT_x8r8g8b8)
		mmio32_w(pVoo, 0x114, (1<<8) | 4);	/* xRGB888 */
	else ErrorF("BOGOFORMAT\n");
	
	dh = srcy;
	w = pVoo->texW;
	h = pVoo->texH;
	
	if(debug)
		ErrorF("CPUToScreenTexture (%d,%d)->(%d,%d)[%d,%d]\n",
			srcx,srcy,dstx,dsty,width,height);
	/*
	 *	Tiled software render letting hardware do the read merge
	 *	that we don't want the CPU to do.
	 */
	 
	for(y = 0; y < height; y++)
	{
		cdb = db;
		fdb = fb;
		dw = srcx;
		for(x = 0; x < width; x++)
		{
			*fdb++ = *cdb++;
			
			if(++dw == w)
			{
				dw = 0;
				cdb -= pVoo->texW;
			}
		}
		db += pVoo->texW;
		fb += 1024;
		dh ++;
		if(dh == h)
		{
			db = ((CARD32 *)pVoo->texPtr) + srcx;
			dh = 0;
		}
	}	
	mmio32_w(pVoo, 0x114, pVoo->lfbMode);
	mmio32_w(pVoo, 0x10C, 0);
}

CARD32 VoodooAlphaTextureFormats[2] = {PICT_a8, 0};
CARD32 VoodooTextureFormats[3] = {PICT_a8r8g8b8, PICT_x8r8g8b8, 0};

#endif

void Voodoo2XAAInit(ScreenPtr pScreen)
{
	ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
	VoodooPtr pVoo = VoodooPTR(pScrn);
	XAAInfoRecPtr pAccel = XAACreateInfoRec();
	BoxRec cacheArea;
	
	pAccel->Flags = OFFSCREEN_PIXMAPS|LINEAR_FRAMEBUFFER;
	pAccel->Sync = VoodooSync;

	pAccel->ScreenToScreenCopyFlags =  NO_PLANEMASK;
	pAccel->SetupForScreenToScreenCopy = Voodoo2SetupForScreenToScreenCopy;
	pAccel->SubsequentScreenToScreenCopy = Voodoo2SubsequentScreenToScreenCopy;

	pAccel->SolidFillFlags = NO_PLANEMASK;
	pAccel->SetupForSolidFill = Voodoo2SetupForSolidFill;
	pAccel->SubsequentSolidFillRect = Voodoo2SubsequentSolidFillRect;

	pAccel->ScanlineCPUToScreenColorExpandFillFlags = 
		BIT_ORDER_IN_BYTE_MSBFIRST | NO_PLANEMASK | 
			SCANLINE_PAD_DWORD | CPU_TRANSFER_BASE_FIXED;
	pAccel->SetupForScanlineCPUToScreenColorExpandFill = 
				Voodoo2SetupForScanlineCPUToScreenColorExpandFill;
	pAccel->SubsequentScanlineCPUToScreenColorExpandFill =
				Voodoo2SubsequentScanlineCPUToScreenColorExpandFill;
	pAccel->SubsequentColorExpandScanline = 
				Voodoo2SubsequentColorExpandScanline;

	pAccel->NumScanlineColorExpandBuffers = 1;
	pVoo->LinePtr = pVoo->LineBuffer;
	pAccel->ScanlineColorExpandBuffers = &pVoo->LinePtr;

	pAccel->SetupForSolidLine = Voodoo2SetupForSolidLine;
	pAccel->SubsequentSolidHorVertLine = Voodoo2SubsequentSolidHorVertLine;
	pAccel->SolidLineFlags = NO_PLANEMASK;

	pAccel->Mono8x8PatternFillFlags = HARDWARE_PATTERN_PROGRAMMED_BITS;
	pAccel->SetupForMono8x8PatternFill = Voodoo2SetupForMono8x8PatternFill;
	pAccel->SubsequentMono8x8PatternFillRect = Voodoo2SubsequentMono8x8PatternFillRect;

	pAccel->ScanlineImageWriteFlags = NO_PLANEMASK;
	pAccel->SetupForScanlineImageWrite = Voodoo2SetupForScanlineImageWrite;
	pAccel->SubsequentImageWriteRect = Voodoo2SubsequentImageWriteRect;
	pAccel->SubsequentImageWriteScanline = Voodoo2SubsequentImageWriteScanline;

	pAccel->ClippingFlags =
		HARDWARE_CLIP_SCREEN_TO_SCREEN_COLOR_EXPAND |
		HARDWARE_CLIP_SCREEN_TO_SCREEN_COPY |
		HARDWARE_CLIP_MONO_8x8_FILL |
		HARDWARE_CLIP_SOLID_FILL;
		
	pAccel->SetClippingRectangle = Voodoo2SetClippingRectangle;
	pAccel->DisableClipping = Voodoo2DisableClipping;
	
#ifdef RENDER
	pAccel->CPUToScreenAlphaTextureFlags = 0;
	pAccel->SetupForCPUToScreenAlphaTexture = VoodooSetupForCPUToScreenAlphaTexture;
	pAccel->SubsequentCPUToScreenAlphaTexture = VoodooSubsequentCPUToScreenAlphaTexture;
	
	pAccel->CPUToScreenTextureFlags = 0;
	pAccel->SetupForCPUToScreenTexture = VoodooSetupForCPUToScreenTexture;
	pAccel->SubsequentCPUToScreenTexture = VoodooSubsequentCPUToScreenTexture;
	
	pAccel->CPUToScreenTextureFormats = VoodooTextureFormats;
	pAccel->CPUToScreenAlphaTextureFormats = VoodooAlphaTextureFormats;
#endif

	cacheArea.x1 = 0;
	cacheArea.x2 = pScrn->displayWidth;
	cacheArea.y1 = pVoo->Height;
	cacheArea.y2 = (pScrn->videoRam * 1024) / (pVoo->Tiles * 64);
	if(cacheArea.y2 > 2047)
		cacheArea.y2 = 2047;
		
	if(cacheArea.y2 > cacheArea.y1)
	{
		xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT, "Using %d lines of pixmap cache.\n", cacheArea.y2-cacheArea.y1);
		pAccel->Flags |= PIXMAP_CACHE;
		pVoo->FullHeight = cacheArea.y2;
		xf86InitFBManager(pScreen, &cacheArea);
	}
	if( XAAInit(pScreen, pAccel) == FALSE)
		ErrorF("Unable to set up acceleration.\n");
		
	Voodoo2DisableClipping(pScrn);
}
