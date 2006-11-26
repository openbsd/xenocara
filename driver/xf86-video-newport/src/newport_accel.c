/*
 * Accelerated Driver for the SGI Indy's Newport graphics card
 * 
 * (c) 2004 Dominik Behr <dominikbehr@yahoo.com>
 * this code is released under xfree 4.3.0 and xorg 6.8.0 license
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "newport.h"
#include <limits.h>

#ifdef NEWPORT_ACCEL

#include "mi.h"
#include "mizerarc.h"

#define BARF(a) xf86DrvMsg(0, X_INFO, (a))
#define BARF1(a,b) xf86DrvMsg(0, X_INFO, (a), (b))
#define BARF2(a,b,c) xf86DrvMsg(0, X_INFO, (a), (b), (c))
#define BARF3(a,b,c,d) xf86DrvMsg(0, X_INFO, (a), (b), (c), (d))
#define BARF4(a,b,c,d,e) xf86DrvMsg(0, X_INFO, (a), (b), (c), (d), (e))
#define BARF5(a,b,c,d,e,f) xf86DrvMsg(0, X_INFO, (a), (b), (c), (d), (e), (f))
#define BARF6(a,b,c,d,e,f,g) xf86DrvMsg(0, X_INFO, (a), (b), (c), (d), (e), (f), (g))

/* XAA Functions */

#define NEWPORT_PREROTATE
/*
    there is a bug in XAA which causes it to reference NULL pointer to pattern cache when
    using HARDWARE_PROGRAMMED_PATTERN only (try x11perf -strap1)
    thus we have to also set HARDWARE_PROGRAMMED_ORIGIN and prerotate the pattern 
    in the setup function
*/

#define NEWPORT_GFIFO_ENTRIES 30
/* 
 I dont know how many entries are in the gfx FIFO, judging by 6 bits in the GFIFO 
 level status register it can be at most 63. it must be at least 32 because 
 otherwise they would use less bits for status
 
 for now 16 seems to be safe value
*/
#define NEWPORT_DELAY 128

/*******************************************************************************

*******************************************************************************/
static void
NewportWaitIdle(NewportPtr pNewport, unsigned int uEntries)
{
    NewportRegsPtr pNewportRegs;
    pNewportRegs = pNewport->pNewportRegs;
    /* wait for the GFIFO to drain */
    while ((pNewportRegs->cset.stat & NPORT_STAT_GLMSK)) 
    {
	int i;
	volatile int x;	
	for (x = 0, i = 0; i < NEWPORT_DELAY; i++)
	{
	    x += i;
	}
    } 
    /* and then wait for the graphic to be idle */
    while (pNewportRegs->cset.stat & NPORT_STAT_GBUSY)
    {
	int i;
	volatile int x;
	for (x = 0, i = 0; i < NEWPORT_DELAY; i++)
	{
	    x += i;
	}
    }
    pNewport->fifoleft = NEWPORT_GFIFO_ENTRIES-uEntries;
}


#if 0
/*******************************************************************************

*******************************************************************************/
static void
NewportWaitGFIFO(NewportPtr pNewport, unsigned int uEntries)
{
    unsigned int uWaitLevel;
    
/*    NewportWaitIdle(pNewport, NEWPORT_GFIFO_ENTRIES);
    return;*/
    
    if (uEntries >= NEWPORT_GFIFO_ENTRIES)
    {
	uWaitLevel = 0;
    }
    else
    {
	uWaitLevel = NEWPORT_GFIFO_ENTRIES-uEntries;
    }
    /* HACK */
    /*uWaitLevel = 0;*/
    while (((pNewport->pNewportRegs->cset.stat & NPORT_STAT_GLMSK) >> 7) > uWaitLevel)
    {
	int i;
	volatile int x;
	for (x = 0, i = 0; i < NEWPORT_DELAY; i++)
	{
	    x += i;
	}
    }
}
#endif
#if 1
/*******************************************************************************

*******************************************************************************/
static void
NewportWaitGFIFO(NewportPtr pNewport, unsigned int uEntries)
{
    if (uEntries > NEWPORT_GFIFO_ENTRIES)
    {
	uEntries = NEWPORT_GFIFO_ENTRIES;
    }
    
    if (uEntries <= pNewport->fifoleft)
    {
	pNewport->fifoleft -= uEntries;
	return;
    }
    
    while (1)
    {
	unsigned int fifolevel;
	int i;
	volatile int x;
	
	fifolevel = (pNewport->pNewportRegs->cset.stat & NPORT_STAT_GLMSK) >> 7;
	if (fifolevel < NEWPORT_GFIFO_ENTRIES)
	{
	    pNewport->fifoleft = NEWPORT_GFIFO_ENTRIES - fifolevel;
	}
	else
	{
	    pNewport->fifoleft = 0;
	}
	if (uEntries <= pNewport->fifoleft)
	{
	    pNewport->fifoleft -= uEntries;
	    return;
        }
	
	for (x = 0, i = 0; i < NEWPORT_DELAY; i++)
	{
	    x += i;
	}
    }
}
#endif


/*******************************************************************************

*******************************************************************************/
static void
NewportXAASync(ScrnInfoPtr pScrn)
{
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    
    NewportWaitIdle(pNewport, 0);
}


/*******************************************************************************

*******************************************************************************/
static unsigned int
Rop2LogicOp(int rop)
{
    return (unsigned int)rop << 28;
}

/*******************************************************************************

*******************************************************************************/
static void
NewportUpdateClipping(NewportPtr pNewport)
{
    unsigned int smask0x, smask0y;
    
    if (pNewport->skipleft > pNewport->clipsx)
    {
        smask0x = ((pNewport->skipleft & 0xFFFF) << 16) | (pNewport->clipex & 0xFFFF);
    }
    else
    {
	smask0x = ((pNewport->clipsx & 0xFFFF) << 16) | (pNewport->clipex & 0xFFFF);
    }

    if (smask0x != pNewport->shadow_smask0x)
    {
	NewportWaitGFIFO(pNewport, 1);
	pNewport->shadow_smask0x = smask0x;
	pNewport->pNewportRegs->set.smask0x = smask0x;
    }
    
    smask0y = ((pNewport->clipsy & 0xFFFF) << 16) | (pNewport->clipey & 0xFFFF);	
    if (smask0y != pNewport->shadow_smask0y)
    {
	NewportWaitGFIFO(pNewport, 1);
	pNewport->shadow_smask0y = smask0y;
	pNewport->pNewportRegs->set.smask0y = smask0y;
    }
}

/*******************************************************************************

*******************************************************************************/
static unsigned int
NewportColor2HOSTRW(unsigned int color)
{
 /*
  default XAA color is 0,R,G,B
 */
#if 0 
 return  ((color & 0x0000FF) << 16)
       | ((color & 0xFF0000) >> 16)
       | ((color & 0x00FF00))
       ;
#endif
/* but we changed masks to match the native format */
 return color;       
}

/*******************************************************************************

*******************************************************************************/
static unsigned int
NewportColor2Planes24(unsigned int color)
{
    unsigned int res;
    unsigned int i;
    unsigned int mr, mg, mb;
    unsigned int sr, sg, sb;
    
 /*
  XAA color is 0,R,G,B
 */
 
    res = 0;
#if 0    
    mr = 0x800000;
    mg = 0x008000;
    mb = 0x000080;
#endif    
    mr = 0x000080;
    mg = 0x008000;
    mb = 0x800000;
    sr = 2;
    sg = 1;
    sb = 4;
    
    for (i = 0; i < 8; i++)
    {
	res |= (color & mr)?sr:0;
	res |= (color & mg)?sg:0;
	res |= (color & mb)?sb:0;

	sr <<= 3;    
	sg <<= 3;
	sb <<= 3;
	mr >>= 1;
	mg >>= 1;
	mb >>= 1;
    }
    
    return  res;
}

/*******************************************************************************

*******************************************************************************/
static unsigned int
NewportColor2Planes8(unsigned int color)
{
    return color;
}

/*******************************************************************************

*******************************************************************************/
static void
NewportUpdateCOLORI(NewportPtr pNewport, unsigned long colori)
{
    if (colori != pNewport->shadow_colori)
    {
	NewportWaitGFIFO(pNewport, 1);
	pNewport->shadow_colori = colori;
	pNewport->pNewportRegs->set.colori = colori;
    }
}


/*******************************************************************************

*******************************************************************************/
static void
NewportUpdateDRAWMODE0(NewportPtr pNewport, unsigned long drawmode0)
{
    if (drawmode0 != pNewport->shadow_drawmode0)
    {
	NewportWaitGFIFO(pNewport, 1);
	pNewport->shadow_drawmode0 = drawmode0;
	pNewport->pNewportRegs->set.drawmode0 = drawmode0;
    }
}


/*******************************************************************************

*******************************************************************************/
static void
NewportUpdateDRAWMODE1(NewportPtr pNewport, unsigned long drawmode1)
{
    if (drawmode1 != pNewport->shadow_drawmode1)
    {
	NewportWaitIdle(pNewport, 1);
	pNewport->shadow_drawmode1 = drawmode1;
	pNewport->pNewportRegs->set.drawmode1 = drawmode1;
    }
}

/*******************************************************************************

*******************************************************************************/
static void
NewportUpdateCOLORVRAM(NewportPtr pNewport, unsigned long colorvram)
{
    if (colorvram != pNewport->shadow_colorvram)
    {
	NewportWaitIdle(pNewport, 1);
	pNewport->shadow_colorvram = colorvram;
	pNewport->pNewportRegs->set.colorvram = colorvram;
    }
}

/*******************************************************************************

*******************************************************************************/
static void
NewportUpdateCOLORBACK(NewportPtr pNewport, unsigned long colorback)
{
    if (colorback != pNewport->shadow_colorback)
    {
	NewportWaitIdle(pNewport, 1);
	pNewport->shadow_colorback = colorback;
	pNewport->pNewportRegs->set.colorback = colorback;
    }
}

/*******************************************************************************

*******************************************************************************/
static void
NewportUpdateWRMASK(NewportPtr pNewport, unsigned long wrmask)
{
    if (wrmask != pNewport->shadow_wrmask)
    {
	NewportWaitIdle(pNewport, 1);
	pNewport->shadow_wrmask = wrmask;
	pNewport->pNewportRegs->set.wrmask = wrmask;
    }
}

/*******************************************************************************

*******************************************************************************/
static void
NewportUpdateXYMOVE(NewportPtr pNewport, unsigned long xymove)
{
    if (xymove != pNewport->shadow_xymove)
    {
	NewportWaitIdle(pNewport, 1);
	pNewport->shadow_xymove = xymove;
	pNewport->pNewportRegs->set.xymove = xymove;
    }
}


/*******************************************************************************

*******************************************************************************/
static void
NewportXAASetupForScreenToScreenCopy(ScrnInfoPtr pScrn,
                                     int xdir,
				     int ydir,
				     int rop,
				     unsigned int planemask,
				     int trans_color)
{
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);
    
    NewportUpdateDRAWMODE1(pNewport, pNewport->setup_drawmode1 | Rop2LogicOp(rop));
    NewportUpdateWRMASK(pNewport, pNewport->Color2Planes(planemask));
    pNewport->skipleft = 0;
    NewportUpdateClipping(pNewport);
    NewportUpdateDRAWMODE0(pNewport,
                           0
                           | NPORT_DMODE0_S2S
                           | NPORT_DMODE0_BLOCK
		           | NPORT_DMODE0_DOSETUP
		           | NPORT_DMODE0_STOPX
			   | NPORT_DMODE0_STOPY
			  );
    
    
}

/*******************************************************************************

*******************************************************************************/
static void
NewportXAASubsequentScreenToScreenCopy(ScrnInfoPtr pScrn,
                                       int x1, int y1,
				       int x2, int y2,
				       int width, int height)
{
    int dx, dy;
    unsigned int sx, sy, ex, ey;
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);
    
    dx = x2 - x1;
    dy = y2 - y1;
    if (!dx && !dy) 
	return;
    
    if (width)
	x2 = x1 + width - 1;
    else
	x2 = x1;
	
    if (height)
	y2 = y1 + height - 1;
    else
	y2 = y1;
    
    if (dx < 0)
    {
	sx = x1 & 0xFFFF;
	ex = x2 & 0xFFFF;
    }
    else
    {
	sx = x2 & 0xFFFF;
	ex = x1 & 0xFFFF;
    }
    
    if (dy < 0)
    {
	sy = y1 & 0xFFFF;
	ey = y2 & 0xFFFF;
    }
    else
    {
	sy = y2 & 0xFFFF;
	ey = y1 & 0xFFFF;
    }
    
    dx &= 0xFFFF;
    dy &= 0xFFFF;

    NewportUpdateXYMOVE(pNewport, (dx << 16) | dy); /* this is bad because it stalls the pipeline but nothing can be done about it */    
    NewportWaitGFIFO(pNewport, 2);
    pNewportRegs->set.xystarti = (sx << 16) | sy;
    pNewportRegs->go.xyendi = (ex << 16) | ey;        
}				       

/*******************************************************************************

*******************************************************************************/
static void
NewportXAASetupForSolidFill(ScrnInfoPtr pScrn,
                            int Color,
			    int rop,
			    unsigned int planemask)
{
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);

    if (rop == GXcopy
        || rop == GXclear
	|| rop == GXset)
    {
	/* if possible try to set up a fast clear which is 4x faster */
	NewportUpdateDRAWMODE1(pNewport, pNewport->setup_drawmode1 | NPORT_DMODE1_FCLR | Rop2LogicOp(GXcopy));
	if (rop == GXclear)
	    NewportUpdateCOLORVRAM(pNewport, 0);
	else
	if (rop == GXset)
	    NewportUpdateCOLORVRAM(pNewport, 0xFFFFFF);
	else
            NewportUpdateCOLORVRAM(pNewport, pNewport->Color2Planes((unsigned int)Color));
    }
    else
    {
	NewportUpdateDRAWMODE1(pNewport, pNewport->setup_drawmode1 | Rop2LogicOp(rop));
	NewportUpdateCOLORI(pNewport, NewportColor2HOSTRW(Color));
    }
    
    NewportUpdateWRMASK(pNewport, pNewport->Color2Planes(planemask));
    pNewport->skipleft = 0;
    NewportUpdateClipping(pNewport);
    NewportUpdateDRAWMODE0(pNewport, 
                           0
                           | NPORT_DMODE0_DRAW
                           | NPORT_DMODE0_BLOCK
			   | NPORT_DMODE0_DOSETUP
			   | NPORT_DMODE0_STOPX
			   | NPORT_DMODE0_STOPY
			  );
}

/*******************************************************************************

*******************************************************************************/
static void
NewportXAASubsequentSolidFillRect(ScrnInfoPtr pScrn,
                                  int x,
				  int y,
				  int w,
				  int h)
{
    int ex, ey;
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);
    
    if (w == 0) w = 1;
    if (h == 0) h = 1;
    ex = x + w - 1;
    ey = y + h - 1;
    
    NewportWaitGFIFO(pNewport, 2);
    pNewportRegs->set.xystarti = ((x & 0xFFFF) << 16) | (y & 0xFFFF);
    pNewportRegs->go.xyendi = ((ex & 0xFFFF) << 16) | (ey & 0xFFFF);
}

/*******************************************************************************

*******************************************************************************/
static void
NewportXAASetupForSolidLine(ScrnInfoPtr pScrn,
                            int Color,
			    int rop,
			    unsigned int planemask)
{
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);

    NewportUpdateDRAWMODE1(pNewport, pNewport->setup_drawmode1 | Rop2LogicOp(rop));
    NewportUpdateWRMASK(pNewport, pNewport->Color2Planes(planemask));
    NewportUpdateCOLORI(pNewport, NewportColor2HOSTRW(Color));

    pNewport->skipleft = 0;
    NewportUpdateClipping(pNewport);
    pNewport->setup_drawmode0 = (0
                                | NPORT_DMODE0_DRAW
                                | NPORT_DMODE0_ILINE
				| NPORT_DMODE0_DOSETUP
				| NPORT_DMODE0_STOPX
				| NPORT_DMODE0_STOPY
				);
}

/*******************************************************************************

*******************************************************************************/
static void
NewportXAASubsequentSolidTwoPointLine(ScrnInfoPtr pScrn,
                                      int x1,
				      int y1,
				      int x2,
				      int y2,
				      int flags)
{
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);
    
    NewportUpdateDRAWMODE0(pNewport, 
                           pNewport->setup_drawmode0
			   | ((flags & OMIT_LAST) ? NPORT_DMODE0_SKLST : 0)
			  );    
    NewportWaitGFIFO(pNewport, 2);
    pNewportRegs->set.xystarti = ((x1 & 0xFFFF) << 16) | (y1 & 0xFFFF);
    pNewportRegs->go.xyendi = ((x2 & 0xFFFF) << 16) | (y2 & 0xFFFF);
}


/*******************************************************************************

*******************************************************************************/
static void
NewportXAASetupForDashedLine(ScrnInfoPtr pScrn,
                             int fg,
			     int bg,
			     int rop,
			     unsigned int planemask,
			     int length,
			     unsigned char *pattern)
{
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    int i;
    
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);
    
    pNewport->dashline_patlen = length;
    for (i = 0; i < (length+7)>>3; i++)
	pNewport->dashline_pat[i] = pattern[i];

    NewportUpdateDRAWMODE1(pNewport, pNewport->setup_drawmode1 | Rop2LogicOp(rop));
    NewportUpdateWRMASK(pNewport, pNewport->Color2Planes(planemask));
    if (bg != -1)
	NewportUpdateCOLORBACK(pNewport, NewportColor2HOSTRW(bg));
    NewportUpdateCOLORI(pNewport, NewportColor2HOSTRW(fg));
    pNewport->skipleft = 0;
    NewportUpdateClipping(pNewport);    
    pNewport->setup_drawmode0 = (0
                                | NPORT_DMODE0_DRAW
                                | NPORT_DMODE0_ILINE
				| NPORT_DMODE0_DOSETUP
				| NPORT_DMODE0_STOPX
				| NPORT_DMODE0_STOPY
				| NPORT_DMODE0_L32
				| NPORT_DMODE0_ZPENAB
				| ((bg == -1) ? 0 : NPORT_DMODE0_ZOPQ)
				);
}

/*******************************************************************************

*******************************************************************************/
static void
NewportXAASubsequentDashedTwoPointLine(ScrnInfoPtr pScrn,
                                       int x1,
			 	       int y1,
			 	       int x2,
			 	       int y2,
			 	       int flags,
				       int phase)
{
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    int dx, dy;
    unsigned int linelen;
    unsigned int dwords;
    
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);

    dx = x2 - x1; if (dx < 0) dx = -dx; dx++;
    dy = y2 - y1; if (dy < 0) dy = -dy; dy++;
    if (dx > dy)
	linelen = dx;
    else
	linelen = dy;
    dwords = (linelen + 31) >> 5;

    NewportUpdateDRAWMODE0(pNewport, 
                           pNewport->setup_drawmode0
			   | ((flags & OMIT_LAST) ? NPORT_DMODE0_SKLST : 0)
			  );
    
    NewportWaitGFIFO(pNewport, 3);
    pNewportRegs->set.xystarti = ((x1 & 0xFFFF) << 16) | (y1 & 0xFFFF);
    pNewportRegs->set.xyendi = ((x2 & 0xFFFF) << 16) | (y2 & 0xFFFF);
    pNewportRegs->set._setup = 1;
    
    phase %= pNewport->dashline_patlen;
    while (dwords--) {
	unsigned int bit;
	unsigned int pat;
	unsigned int mask;
	pat = 0;
	mask = 0x80000000;
	for (bit = 0; bit < 32; bit++)
	{
	    if (pNewport->dashline_pat[phase >> 3] & (0x80 >> (phase & 7)))
		pat |= mask;	    
	    phase = (phase + 1) % pNewport->dashline_patlen;
	    mask >>= 1;
	}	
	NewportWaitGFIFO(pNewport, 1);
	pNewportRegs->go.zpattern = pat;
    }

}

/*******************************************************************************

*******************************************************************************/
static void
NewportXAASetupForCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
                                             int fg,
					     int bg,
					     int rop,
					     unsigned int planemask)
{
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);
    
    NewportUpdateDRAWMODE1(pNewport, pNewport->setup_drawmode1 | Rop2LogicOp(rop));
    NewportUpdateWRMASK(pNewport, pNewport->Color2Planes(planemask));    
    if (bg != -1)
	NewportUpdateCOLORBACK(pNewport, NewportColor2HOSTRW(bg));
    NewportUpdateCOLORI(pNewport, NewportColor2HOSTRW(fg));
    NewportUpdateDRAWMODE0(pNewport,
                           0
                           | NPORT_DMODE0_DRAW
                           | NPORT_DMODE0_BLOCK
			   | NPORT_DMODE0_DOSETUP
			   | NPORT_DMODE0_STOPX
			   | NPORT_DMODE0_ZPENAB
			   | ((bg == -1) ? 0 : NPORT_DMODE0_ZOPQ)
			   | NPORT_DMODE0_L32
			  );
}

/*******************************************************************************

*******************************************************************************/
static void
NewportXAASubsequentCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
					       int x,
					       int y,
					       int w,
					       int h,
					       int skipleft)
{
    int ex, ey;
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);
    
    if (w == 0) w = 1;
    if (h == 0) h = 1;
    ex = x + w - 1;
    ey = y + h - 1;
	
    if (skipleft)
    {
	pNewport->skipleft = x + skipleft;
    }
    else
    {
	pNewport->skipleft = 0;
    }
    NewportUpdateClipping(pNewport);
    
    NewportWaitGFIFO(pNewport, 3);
    pNewportRegs->set.xystarti = ((x & 0xFFFF) << 16) | (y & 0xFFFF);
    pNewportRegs->set.xyendi = ((ex & 0xFFFF) << 16) | (ey & 0xFFFF);
    pNewportRegs->set._setup = 1;

    /* after return XAA will start blasting the pattern to go.zpattern register */    
    /* we have to wait here for idle because if there is something in the pipeline that will take
     long time to complete and if following cpu transfers will overflow the FIFO causing GIO bus 
     stall it will end up in bus error */
    NewportWaitIdle(pNewport, NEWPORT_GFIFO_ENTRIES);
    
}

/*******************************************************************************

*******************************************************************************/
static void
NewportXAASetupForImageWrite(ScrnInfoPtr pScrn,
                             int rop,
			     unsigned int planemask,
			     int trans_color,
			     int bpp,
			     int depth)
{
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);
 
    NewportUpdateDRAWMODE1(pNewport, pNewport->setup_drawmode1 | Rop2LogicOp(rop));
    NewportUpdateWRMASK(pNewport, pNewport->Color2Planes(planemask));
    NewportUpdateDRAWMODE0(pNewport,
                           0
                           | NPORT_DMODE0_DRAW
                           | NPORT_DMODE0_BLOCK
			   | NPORT_DMODE0_CHOST
			   | NPORT_DMODE0_DOSETUP
			  );

}
				       
/*******************************************************************************

*******************************************************************************/
static void
NewportXAASubsequentImageWriteRect(ScrnInfoPtr pScrn,
                                   int x,
				   int y,
				   int w,
				   int h,
				   int skipleft)
{
    int ex, ey;
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);
    
    if (w == 0) w = 1;
    if (h == 0) h = 1;
    
    ex = x + w - 1;
    ey = y + h - 1;

    if (skipleft)
    {
	pNewport->skipleft = x + skipleft;
    }
    else
    {
	pNewport->skipleft = 0;
    }
    NewportUpdateClipping(pNewport);
    
    NewportWaitGFIFO(pNewport, 3);
    pNewportRegs->set.xystarti = ((x & 0xFFFF) << 16) | (y & 0xFFFF);
    pNewportRegs->set.xyendi = ((ex & 0xFFFF) << 16) | (ey & 0xFFFF);
    pNewportRegs->set._setup = 1;

    /* after return XAA will start blasting the image to go.hostrw0 register */    
    /* we have to wait here for idle because if there is something in the pipeline that will take
     long time to complete and if following cpu transfers will overflow the FIFO causing GIO bus 
     stall it will end up in bus error */
    NewportWaitIdle(pNewport, NEWPORT_GFIFO_ENTRIES);
}

static unsigned int 
repbyte(unsigned int b)
{
    b &= 0xFF;
    return b | (b << 8) | (b << 16) | (b << 24);
}

static void 
prerotatebyte(unsigned int b, unsigned int *p)
{
    int i;
    
    b &= 0xFF;
    
    for (i = 0; i < 8; i++)
    {
	p[i] = repbyte(b);
	if (b & 1)
	    b = (b >> 1) | 0x80;
	else
	    b = b >> 1;
    }
}

/*******************************************************************************

*******************************************************************************/
static void
NewportXAASetupForMono8x8PatternFill(ScrnInfoPtr pScrn,
                                     int patx,
				     int paty,
				     int fg,
				     int bg,
				     int rop,
				     unsigned int planemask)
{
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);
    
#ifdef NEWPORT_PREROTATE    
    /* prerotate the pattern */
    prerotatebyte((unsigned int)patx >> 24, pNewport->pat8x8[0]);
    prerotatebyte((unsigned int)patx >> 16, pNewport->pat8x8[1]);
    prerotatebyte((unsigned int)patx >> 8, pNewport->pat8x8[2]);
    prerotatebyte((unsigned int)patx, pNewport->pat8x8[3]);
    prerotatebyte((unsigned int)paty >> 24, pNewport->pat8x8[4]);
    prerotatebyte((unsigned int)paty >> 16, pNewport->pat8x8[5]);
    prerotatebyte((unsigned int)paty >> 8, pNewport->pat8x8[6]);
    prerotatebyte((unsigned int)paty, pNewport->pat8x8[7]);
#endif    
    NewportUpdateDRAWMODE1(pNewport, pNewport->setup_drawmode1 | Rop2LogicOp(rop));
    NewportUpdateWRMASK(pNewport, pNewport->Color2Planes(planemask));    
    if (bg != -1)
	NewportUpdateCOLORBACK(pNewport, NewportColor2HOSTRW(bg));
    NewportUpdateCOLORI(pNewport, NewportColor2HOSTRW(fg));
    pNewport->skipleft = 0;
    NewportUpdateClipping(pNewport);
    NewportUpdateDRAWMODE0(pNewport,
                           0
                           | NPORT_DMODE0_DRAW
                           | NPORT_DMODE0_BLOCK
		           | NPORT_DMODE0_DOSETUP
		           | NPORT_DMODE0_STOPX
			   | NPORT_DMODE0_ZPENAB
			   | ((bg == -1) ? 0 : NPORT_DMODE0_ZOPQ)
			   | NPORT_DMODE0_L32
			  );
}



/*******************************************************************************

*******************************************************************************/
static void
NewportXAASubsequentMono8x8PatternFillRect(ScrnInfoPtr pScrn,
                                           int patx,
					   int paty,
					   int x,
					   int y,
					   int w,
					   int h)
{
    int ex, ey;
    unsigned int d;    
#ifndef NEWPORT_PREROTATE
    unsigned int p;
    unsigned int epat[8];
#endif    
    
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);

    if (w == 0) w = 1;
    if (h == 0) h = 1;
    ex = x + w - 1;
    ey = y + h - 1;
    
    NewportWaitGFIFO(pNewport, 3);
    pNewportRegs->set.xystarti = ((x & 0xFFFF) << 16) | (y & 0xFFFF);
    pNewportRegs->set.xyendi = ((ex & 0xFFFF) << 16) | (ey & 0xFFFF);
    pNewportRegs->set._setup = 1;
    
#ifdef NEWPORT_PREROTATE
    patx &= 7;
    paty &= 7;
    while (h--)
    {
	for (d = (w + 31) >> 5; d; d--)
	{
	    NewportWaitGFIFO(pNewport, 1);
	    pNewportRegs->go.zpattern = pNewport->pat8x8[paty][patx];
	}
	paty = (paty + 1) & 7;
    }
#else
    epat[0] = repbyte(patx >> 24);
    epat[1] = repbyte(patx >> 16);
    epat[2] = repbyte(patx >> 8);
    epat[3] = repbyte(patx);
    epat[4] = repbyte(paty >> 24);
    epat[5] = repbyte(paty >> 16);
    epat[6] = repbyte(paty >> 8);
    epat[7] = repbyte(paty);
    
    p = 0;
    while (h--)
    {
	for (d = (w + 31) >> 5; d; d--)
	{
	    NewportWaitGFIFO(pNewport, 1);
	    pNewportRegs->go.zpattern = epat[p];
	}
	p = (p + 1) ^ 7;
    }
#endif    
}					   

/*******************************************************************************

*******************************************************************************/
static void 
NewportXAAReadPixmap(ScrnInfoPtr pScrn, 
                     int x, 
		     int y, 
		     int w, 
		     int h,
		     unsigned char *dst, 
		     int dstwidth, 
		     int bpp, 
		     int depth)
{
    int ex, ey;
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);
    
    if (w == 0) w = 1;
    if (h == 0) h = 1;
    ex = x + w - 1;
    ey = y + h - 1;
    
    NewportWaitIdle(pNewport, 0);
    NewportUpdateDRAWMODE1(pNewport, pNewport->setup_drawmode1 | Rop2LogicOp(GXcopy) | NPORT_DMODE1_PFENAB);
    NewportUpdateWRMASK(pNewport, pNewport->Color2Planes(0xFFFFFFFF));
    NewportUpdateDRAWMODE0(pNewport,
                           0
                           | NPORT_DMODE0_RD
                           | NPORT_DMODE0_BLOCK
			   | NPORT_DMODE0_CHOST
			   | NPORT_DMODE0_DOSETUP
			  );

    NewportWaitGFIFO(pNewport, 3);
    pNewportRegs->set.xystarti = ((x & 0xFFFF) << 16) | (y & 0xFFFF);
    pNewportRegs->set.xyendi = ((ex & 0xFFFF) << 16) | (ey & 0xFFFF);
    /* with prefetch enabled go has to be issued before we start reading */
    pNewportRegs->go._setup = 1;


    if (pScrn->bitsPerPixel == 8)
    {
	unsigned int d;
	while (h--)
	{
	    unsigned char *p;
	    p = (unsigned char *)dst;
    	    for (x = 0; x < w; x += 4)
	    {
		if (!h && x+4 >= w)
		{ /* the last dword does not need go */
		    d = pNewportRegs->set.hostrw0;
		}
		else
		{
		    d = pNewportRegs->go.hostrw0;
		}
		
		*p++ = (d & 0xFF000000) >> 24;
		if (x+1 < w)
		{
		    *p++ = (d & 0x00FF0000) >> 16;
		}
		if (x+2 < w)
		{
		    *p++ = (d & 0x0000FF00) >> 8;
		}
		if (x+3 < w)
		{
		    *p++ = d & 0x000000FF;
		}
	    }
	    dst += dstwidth;
	}
    }
    else
    {
	while (h--)
	{
	    unsigned int *p;
	    p = (unsigned int *)dst;
    	    for (x = 0; x < w; x++)
	    {
		if (!h && x+1 == w)
		{ /* the last dword does not need go */
		    *p++ = pNewportRegs->set.hostrw0;
		}
		else
		{
		    *p++ = pNewportRegs->go.hostrw0;
		}
	    }
	    dst += dstwidth;
	}
    }
}

/*******************************************************************************

*******************************************************************************/
static void
NewportXAASetClippingRectangle(ScrnInfoPtr pScrn,
                               int left,
			       int top,
			       int right,
			       int bottom)
{
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);

    if (left < 0) left = 0;
    if (right > pScrn->virtualX-1) right = pScrn->virtualX-1;
    
    if (top < 0) top = 0;
    if (bottom > pScrn->virtualY-1) bottom = pScrn->virtualY-1;
    
    pNewport->clipsx = left;
    pNewport->clipex = right;
    pNewport->clipsy = top;
    pNewport->clipey = bottom;

    NewportUpdateClipping(pNewport);
}

/*******************************************************************************

*******************************************************************************/
static void
NewportXAADisableClipping(ScrnInfoPtr pScrn)
{
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);

    pNewport->clipsx = 0;
    pNewport->clipex = pScrn->virtualX-1;
    pNewport->clipsy = 0;
    pNewport->clipey = pScrn->virtualY-1;
    NewportUpdateClipping(pNewport);
}

/*******************************************************************************

*******************************************************************************/
static void
NewportPolyPoint(DrawablePtr pDraw,
                 GCPtr pGC,
                 int mode,
                 int npt,
                 xPoint *ppt)
{
    ScrnInfoPtr pScrn;
    int numRects = REGION_NUM_RECTS(pGC->pCompositeClip);
    int rect;
    XAAInfoRecPtr infoRec;
    BoxPtr pbox;
    int x, y;
    int rop;

    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    
    infoRec = GET_XAAINFORECPTR_FROM_GC(pGC);
    pScrn = infoRec->pScrn;
    
    if (!numRects) 
	return;

    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);

    x = pDraw->x;
    y = pDraw->y;
    
    rop = pGC->alu;
    
    NewportUpdateWRMASK(pNewport, pNewport->Color2Planes(pGC->planemask));
    if (rop == GXcopy
        || rop == GXclear
	|| rop == GXset)
    {
	/* if possible try to set up a fast clear which is 4x faster */
	NewportUpdateDRAWMODE1(pNewport, pNewport->setup_drawmode1 | NPORT_DMODE1_FCLR | Rop2LogicOp(GXcopy));
	if (rop == GXclear)
	    NewportUpdateCOLORVRAM(pNewport, 0);
	else
	if (rop == GXset)
	    NewportUpdateCOLORVRAM(pNewport, 0xFFFFFF);
	else
            NewportUpdateCOLORVRAM(pNewport, pNewport->Color2Planes((unsigned int)(pGC->fgPixel)));
    }
    else
    {
	NewportUpdateDRAWMODE1(pNewport, pNewport->setup_drawmode1 | Rop2LogicOp(rop));
	NewportUpdateCOLORI(pNewport, NewportColor2HOSTRW(pGC->fgPixel));
    }
    
    pNewport->skipleft = 0;
    NewportUpdateClipping(pNewport);
    NewportUpdateDRAWMODE0(pNewport,
                           0
                           | NPORT_DMODE0_DRAW
			   | NPORT_DMODE0_BLOCK
		           | NPORT_DMODE0_DOSETUP
			  );
    
    pbox = REGION_RECTS(pGC->pCompositeClip);
    
    while (npt--) {
	if (mode == CoordModePrevious)
	{
	    x += ppt->x;
	    y += ppt->y;
	}
	else
	{
	    x = pDraw->x + ppt->x;
	    y = pDraw->y + ppt->y;
	}
	for (rect = 0; rect < numRects; rect++)
	    if (x >= pbox[rect].x1 && x < pbox[rect].x2
	        && y >= pbox[rect].y1 && y < pbox[rect].y2)
	       { /* draw point */
	    	    unsigned int xy;
		    xy = ((x & 0xFFFF) << 16) | (y & 0xFFFF);
	    	    NewportWaitGFIFO(pNewport, 2);
		    pNewportRegs->set.xystarti = xy;
		    pNewportRegs->go.xyendi = xy;
		    break;
	       }
	ppt++;
    }
}

/*******************************************************************************

*******************************************************************************/
static void
NewportValidatePolyPoint(GCPtr pGC,
                         unsigned long changes,
                         DrawablePtr pDraw)
{

    if (pDraw->type == DRAWABLE_WINDOW)   
    {
	pGC->ops->PolyPoint = NewportPolyPoint;
    }
    else
    {
	pGC->ops->PolyPoint = XAAGetFallbackOps()->PolyPoint;
    }
}

#if 1
/*******************************************************************************

*******************************************************************************/
static void
NewportPolyArc(DrawablePtr pDraw,
               GCPtr pGC,
               int narcs,
               xArc *parcs)
{
    xArc *arc;
    BoxRec box;
    int i, x2, y2;
    RegionPtr cclip;

    cclip = pGC->pCompositeClip;

    if(!REGION_NUM_RECTS(cclip))
	return;

    for (arc = parcs, i = narcs; --i >= 0; arc++) {
	if (miCanZeroArc(arc)) {
	    box.x1 = arc->x + pDraw->x;
	    box.y1 = arc->y + pDraw->y;
 	    x2 = box.x1 + (int)arc->width + 1;
 	    box.x2 = x2;
 	    y2 = box.y1 + (int)arc->height + 1;
 	    box.y2 = y2;
 	    if ( (x2 <= SHRT_MAX) && (y2 <= SHRT_MAX) &&
 		    (RECT_IN_REGION(pDraw->pScreen, cclip, &box) == rgnIN) )
		miZeroPolyArc(pDraw, pGC, 1, arc);
	}
	else
	    miPolyArc(pDraw, pGC, 1, arc);
    }
}
#endif

/*******************************************************************************

*******************************************************************************/
static void
NewportValidatePolyArc(GCPtr pGC,
                       unsigned long changes,
                       DrawablePtr pDraw)
{
    if (pDraw->type == DRAWABLE_WINDOW)   
    {
	pGC->ops->PolyPoint = NewportPolyPoint;
	/*pGC->ops->PolyArc = miPolyArc;*/
	pGC->ops->PolyArc = NewportPolyArc;
	
    }
    else
    {
	pGC->ops->PolyPoint = XAAGetFallbackOps()->PolyPoint;
	pGC->ops->PolyArc = XAAGetFallbackOps()->PolyArc;
    }
}


#ifdef RENDER
/*******************************************************************************

*******************************************************************************/
static Bool 
NewportXAASetupForCPUToScreenAlphaTexture(
	ScrnInfoPtr	pScrn,
	int		op,
	CARD16		red,
	CARD16		green,
	CARD16		blue,
	CARD16		alpha,
	CARD32          maskFormat,
	CARD32          dstFormat,
	CARD8		*alphaPtr,
	int		alphaPitch,
	int		width,
	int		height,
	int		flags)
{
    unsigned int col, a;
    unsigned char *pSrc;
    unsigned int *pDst;
    unsigned int w;
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);

    if (width * height * sizeof(unsigned int) > pNewport->uTextureSize)
    {
	free(pNewport->pTexture);
	pNewport->pTexture = xnfalloc(pNewport->uTextureSize = width * height * sizeof(unsigned int));
    }
    col = (((unsigned int)red & 0xFF00) << 8)
        | ((unsigned int)green & 0xFF00)
	| ((unsigned int)blue >> 8);
	
    pNewport->uTextureWidth = width;
    pNewport->uTextureHeight = height;
    pNewport->uTextureFlags = flags;
    
    /* copy texture */
    pDst = pNewport->pTexture;
    while (height--) {
	pSrc = (unsigned char *)alphaPtr;
	
	for (w = width; w; w--)
	{
	    /* premultiply alpha */
	    a = *pSrc++;
	    a *= alpha;
	    a /= 0xFFFF;
	    a <<= 24;
	    *pDst++ = col | a;
	}
	alphaPtr += alphaPitch;
    }
#define SFACTOR (4 << 19)
#define DFACTOR (5 << 22)
    NewportUpdateDRAWMODE1(pNewport, pNewport->setup_drawmode1 | Rop2LogicOp(GXcopy) | NPORT_DMODE1_BENAB | SFACTOR | DFACTOR );
#undef SFACTOR
#undef DFACTOR    
    NewportUpdateWRMASK(pNewport, pNewport->Color2Planes(0xFFFFFFFF));

    pNewport->skipleft = 0;
    NewportUpdateClipping(pNewport);
    NewportUpdateDRAWMODE0(pNewport,
                           0
                           | NPORT_DMODE0_DRAW
                           | NPORT_DMODE0_BLOCK
			   | NPORT_DMODE0_CHOST
			   | NPORT_DMODE0_AHOST
			   | NPORT_DMODE0_DOSETUP
			  );
    return TRUE;
}   

/*******************************************************************************

*******************************************************************************/
static Bool 
NewportXAASetupForCPUToScreenTexture(
	ScrnInfoPtr	pScrn,
	int		op,
	CARD32          srcFormat,
	CARD32          dstFormat,
	CARD8		*texPtr,
	int		texPitch,
	int		width,
	int		height,
	int		flags)
{
    unsigned int *pSrc, *pDst;
    unsigned int w;
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);
    
    if (width * height * sizeof(unsigned int) > pNewport->uTextureSize)
    {
	free(pNewport->pTexture);
	pNewport->pTexture = xnfalloc(pNewport->uTextureSize = width * height * sizeof(unsigned int));
    }
	
    pNewport->uTextureWidth = width;
    pNewport->uTextureHeight = height;
    pNewport->uTextureFlags = flags;
    
    /* copy texture */
    pDst = pNewport->pTexture;
    
    if (srcFormat == PICT_a8r8g8b8)
    {
	while (height--) {
	    pSrc = (unsigned int *)texPtr;
	
	    for (w = width; w; w--)
	    {
		unsigned int v;
		v = *pSrc++;
		*pDst++ = (v & 0xFF00FF00) | ((v & 0x00FF0000) >> 16) | ((v & 0x000000FF) << 16);
	    }
	    texPtr += texPitch;
	}
    }
    else
    if (srcFormat == PICT_a8b8g8r8)
    {
	while (height--) {
	    pSrc = (unsigned int *)texPtr;
	
	    for (w = width; w; w--)
		*pDst++ = *pSrc++;
	    texPtr += texPitch;
	}
    }
    else
    {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unknown texture format\n");
    }
    
#define SFACTOR (4 << 19)
#define DFACTOR (5 << 22)
    NewportUpdateDRAWMODE1(pNewport, pNewport->setup_drawmode1 | Rop2LogicOp(GXcopy) | NPORT_DMODE1_BENAB | SFACTOR | DFACTOR );
#undef SFACTOR
#undef DFACTOR    
    NewportUpdateWRMASK(pNewport, pNewport->Color2Planes(0xFFFFFFFF));

    pNewport->skipleft = 0;
    NewportUpdateClipping(pNewport);
    NewportUpdateDRAWMODE0(pNewport,
                           0
                           | NPORT_DMODE0_DRAW
                           | NPORT_DMODE0_BLOCK
			   | NPORT_DMODE0_CHOST
			   | NPORT_DMODE0_AHOST
			   | NPORT_DMODE0_DOSETUP
			  );

    return TRUE;
}	

/*******************************************************************************

*******************************************************************************/
static void
NewportRenderTexture1to1(NewportPtr pNewport, int srcx, int srcy, int w, int h)
{
    unsigned int *p;
    unsigned int add, d;
    NewportRegsPtr pNewportRegs;
    pNewportRegs = pNewport->pNewportRegs;
    
    p = pNewport->pTexture + srcx + (srcy * pNewport->uTextureWidth);
    add = pNewport->uTextureWidth - w + srcx;
    while (h--) 
    {
	for (d = w; d; d--)
	{
	    /*NewportWaitGFIFO(pNewport, 1);*/
	    /* hopefully we cannot write faster than XL24 can blend */
	    pNewportRegs->go.hostrw0 = *p++;
	}
	p += add;
    }
}

/*******************************************************************************

*******************************************************************************/
static void
NewportRenderTextureScale(NewportPtr pNewport, int srcx, int srcy, int w, int h)
{
    int d;
    int dx, dy;
    int curx, cury;
    unsigned int *pLine;
    int l, p;
    NewportRegsPtr pNewportRegs;
    pNewportRegs = pNewport->pNewportRegs;
    
    dx = ((pNewport->uTextureWidth - srcx) << 16) / w;
    dy = ((pNewport->uTextureHeight - srcy) << 16) / h;
    
    cury = srcy << 16;    
    while (h--)
    {	
	l = (cury + 0x7FFF) >> 16;
	if (l >= pNewport->uTextureHeight)
	    l = pNewport->uTextureHeight-1;
	pLine = pNewport->pTexture + l * pNewport->uTextureWidth;

	curx = srcx << 16;
	for (d = w; d; d--)
	{
	    p = (curx + 0x7FFF) >> 16;
	    if (p >= pNewport->uTextureWidth)
		p = pNewport->uTextureWidth-1;
	    pNewportRegs->go.hostrw0 = pLine[p];		
	    curx += dx;
	}
        cury += dy;
    }
}

/*******************************************************************************

*******************************************************************************/
static void
NewportRenderTextureRepeat(NewportPtr pNewport, int srcx, int srcy, int w, int h)
{
    int d;
    unsigned int *pLine;
    NewportRegsPtr pNewportRegs;
    pNewportRegs = pNewport->pNewportRegs;
    
    srcx %= pNewport->uTextureWidth;
    srcy %= pNewport->uTextureHeight;
    
    while (h--)
    {	
	pLine = pNewport->pTexture + pNewport->uTextureWidth * srcy;
	for (d = w; d; d--)
	{
	    pNewportRegs->go.hostrw0 = pLine[srcx];
	    srcx++;
	    if (srcx >= pNewport->uTextureWidth)
		srcx = 0;
	}
        srcy++;
	if (srcy >= pNewport->uTextureHeight)
	    srcy = 0;
    }

}


/*******************************************************************************

*******************************************************************************/
static void 
NewportXAASubsequentCPUToScreenTexture(
	ScrnInfoPtr	pScrn,
	int		x,
	int		y,
	int		srcx,
	int		srcy,
	int		w,
	int		h)
{
    int ex, ey;
    NewportRegsPtr pNewportRegs;
    NewportPtr pNewport;
    pNewport = NEWPORTPTR(pScrn);
    pNewportRegs = NEWPORTREGSPTR(pScrn);
    
    if (w == 0) w = 1;
    if (h == 0) h = 1;
    ex = x + w - 1;
    ey = y + h - 1;
    
    NewportWaitGFIFO(pNewport, 3);
    pNewportRegs->set.xystarti = ((x & 0xFFFF) << 16) | (y & 0xFFFF);
    pNewportRegs->set.xyendi = ((ex & 0xFFFF) << 16) | (ey & 0xFFFF);
    pNewportRegs->set._setup = 1;

    NewportWaitIdle(pNewport, NEWPORT_GFIFO_ENTRIES);
    
    if (srcx + w == pNewport->uTextureWidth
        && srcy + h == pNewport->uTextureHeight)
	NewportRenderTexture1to1(pNewport, srcx, srcy, w, h);
    else
    if (pNewport->uTextureFlags & XAA_RENDER_REPEAT)
	NewportRenderTextureRepeat(pNewport,  srcx, srcy, w, h);
    else
	NewportRenderTextureScale(pNewport, srcx, srcy, w, h);

    NewportWaitIdle(pNewport, NEWPORT_GFIFO_ENTRIES);        

}	

CARD32 NewportAlphaTextureFormats[2] = {PICT_a8, 0};
CARD32 NewportTextureFormats[3] = {PICT_a8r8g8b8, PICT_a8b8g8r8, 0};
/*CARD32 NewportTextureFormats[2] = {PICT_a8r8g8b8, 0};*/
CARD32 NewportDstFormats[7] = {PICT_a8r8g8b8, PICT_a8b8g8r8, PICT_x8r8g8b8, PICT_x8b8g8r8, PICT_r8g8b8, PICT_b8g8r8, 0};

#endif

/*******************************************************************************

*******************************************************************************/
Bool
NewportXAAScreenInit(ScreenPtr pScreen)
{
	ScrnInfoPtr pScrn;
	NewportPtr pNewport;
	NewportRegsPtr pNewportRegs;
	XAAInfoRecPtr pXAAInfoRec;

	/* First get a pointer to our private info */
	pScrn = xf86Screens[pScreen->myNum];
	pNewport = NEWPORTPTR(pScrn);
	pNewportRegs = NEWPORTREGSPTR(pScrn);

	pXAAInfoRec =
	pNewport->pXAAInfoRec = XAACreateInfoRec();
	/* initialize accelerated functions */
	pXAAInfoRec->Sync = NewportXAASync;
	
	pXAAInfoRec->Flags = 0
	                     /*| LINEAR_FRAMEBUFFER*/
	                     ;
	/* screen to screen copy */
	pXAAInfoRec->ScreenToScreenCopyFlags = 0
					     | NO_TRANSPARENCY
					     ;
	pXAAInfoRec->SetupForScreenToScreenCopy = NewportXAASetupForScreenToScreenCopy;
	pXAAInfoRec->SubsequentScreenToScreenCopy = NewportXAASubsequentScreenToScreenCopy;

	/* solid fills */
	pXAAInfoRec->SolidFillFlags = 0
				      ;
	pXAAInfoRec->SetupForSolidFill = NewportXAASetupForSolidFill;
	pXAAInfoRec->SubsequentSolidFillRect = NewportXAASubsequentSolidFillRect;
	
	/* solid lines */
	pXAAInfoRec->SolidLineFlags = 0
	                              ;
	pXAAInfoRec->SetupForSolidLine = NewportXAASetupForSolidLine;
	pXAAInfoRec->SubsequentSolidTwoPointLine = NewportXAASubsequentSolidTwoPointLine;
	
	/* dashed lines */
	pXAAInfoRec->DashedLineFlags = 0
	                             | LINE_PATTERN_MSBFIRST_MSBJUSTIFIED
	                             ;
	pXAAInfoRec->DashPatternMaxLength = 2048;
	pXAAInfoRec->SetupForDashedLine = NewportXAASetupForDashedLine;
	pXAAInfoRec->SubsequentDashedTwoPointLine = NewportXAASubsequentDashedTwoPointLine;

	/* cpu to screen expand color fill */
	pXAAInfoRec->CPUToScreenColorExpandFillFlags = 0
						     | BIT_ORDER_IN_BYTE_LSBFIRST
						     | SCANLINE_PAD_DWORD
						     | CPU_TRANSFER_PAD_DWORD
						     | CPU_TRANSFER_BASE_FIXED
				                     | LEFT_EDGE_CLIPPING
				                     | LEFT_EDGE_CLIPPING_NEGATIVE_X
						     /*| SYNC_AFTER_COLOR_EXPAND*/
						     ;
						     
	pXAAInfoRec->SetupForCPUToScreenColorExpandFill	= NewportXAASetupForCPUToScreenColorExpandFill;
	pXAAInfoRec->SubsequentCPUToScreenColorExpandFill = NewportXAASubsequentCPUToScreenColorExpandFill;
	pXAAInfoRec->ColorExpandRange = 4;
	pXAAInfoRec->ColorExpandBase = (unsigned char *)&(pNewportRegs->go.zpattern);


	/* mono 8x8 pattern fill */
	pXAAInfoRec->Mono8x8PatternFillFlags = 0
	                                       | BIT_ORDER_IN_BYTE_LSBFIRST
					       | HARDWARE_PATTERN_PROGRAMMED_BITS
#ifdef NEWPORT_PREROTATE
                                               | HARDWARE_PATTERN_PROGRAMMED_ORIGIN
#endif					       
                                               ;
						     
	pXAAInfoRec->SetupForMono8x8PatternFill	= NewportXAASetupForMono8x8PatternFill;
	pXAAInfoRec->SubsequentMono8x8PatternFillRect = NewportXAASubsequentMono8x8PatternFillRect;

	/* Image write */
	pXAAInfoRec->ImageWriteFlags = 0
				     | NO_TRANSPARENCY
				     | CPU_TRANSFER_BASE_FIXED
				     | CPU_TRANSFER_PAD_DWORD
				     | SCANLINE_PAD_DWORD
				     | LEFT_EDGE_CLIPPING
				     | LEFT_EDGE_CLIPPING_NEGATIVE_X
				     /*| SYNC_AFTER_IMAGE_WRITE*/
				     ;
	pXAAInfoRec->SetupForImageWrite = NewportXAASetupForImageWrite;
	pXAAInfoRec->SubsequentImageWriteRect = NewportXAASubsequentImageWriteRect;
	pXAAInfoRec->ImageWriteRange = 4;
	pXAAInfoRec->ImageWriteBase =  (unsigned char *)&(pNewportRegs->go.hostrw0);

	/* read pixmap */
	pXAAInfoRec->ReadPixmapFlags = 0
	                             | CPU_TRANSFER_PAD_DWORD
				     ;
	pXAAInfoRec->ReadPixmap = NewportXAAReadPixmap;

	/* clipping */
	pXAAInfoRec->ClippingFlags = 0
	                             /*| HARDWARE_CLIP_SCREEN_TO_SCREEN_COLOR_EXPAND*/
				     | HARDWARE_CLIP_SCREEN_TO_SCREEN_COPY
				     | HARDWARE_CLIP_MONO_8x8_FILL
				     /*| HARDWARE_CLIP_COLOR_8x8_FILL*/
				     | HARDWARE_CLIP_SOLID_FILL
				     | HARDWARE_CLIP_DASHED_LINE
				     | HARDWARE_CLIP_SOLID_LINE
	                             ;
	pXAAInfoRec->SetClippingRectangle = NewportXAASetClippingRectangle;
	pXAAInfoRec->DisableClipping = NewportXAADisableClipping;

	/* make it draw points using our function */	
	pXAAInfoRec->ValidatePolyPoint = NewportValidatePolyPoint;
	pXAAInfoRec->PolyPointMask = GCFunction | GCPlaneMask;
	/*pXAAInfoRec->PolyPointMask = 0xFFFFFFFF;*/

	pXAAInfoRec->ValidatePolyArc = NewportValidatePolyArc;
	pXAAInfoRec->PolyArcMask = GCFunction | GCLineWidth;
#ifdef RENDER
	if (pScrn->bitsPerPixel > 8) 
	{
	    pXAAInfoRec->CPUToScreenTextureFlags = 0
		                                   ;
	    pXAAInfoRec->CPUToScreenTextureFormats = NewportTextureFormats;
	    pXAAInfoRec->CPUToScreenTextureDstFormats = NewportDstFormats;
	    pXAAInfoRec->SetupForCPUToScreenTexture2 = NewportXAASetupForCPUToScreenTexture;
	    pXAAInfoRec->SubsequentCPUToScreenTexture = NewportXAASubsequentCPUToScreenTexture;

	    pXAAInfoRec->CPUToScreenAlphaTextureFlags = 0
	                                                ;
	    pXAAInfoRec->CPUToScreenAlphaTextureFormats = NewportAlphaTextureFormats;
	    pXAAInfoRec->CPUToScreenAlphaTextureDstFormats = NewportDstFormats;
	    pXAAInfoRec->SetupForCPUToScreenAlphaTexture2 = NewportXAASetupForCPUToScreenAlphaTexture;
	    pXAAInfoRec->SubsequentCPUToScreenAlphaTexture = NewportXAASubsequentCPUToScreenTexture; /* this is the same for both */
	    pNewport->pTexture = (unsigned int *)xnfalloc(pNewport->uTextureSize = 16*16*sizeof(unsigned int));
	}
#endif	
	
	pNewport->Color2Planes = NewportColor2Planes24;
	if (pScrn->bitsPerPixel == 8)
	{
	    pNewport->Color2Planes = NewportColor2Planes8;
	}

	if (!XAAInit(pScreen, pXAAInfoRec))
	{
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "XAAInit Failed!\n"); 
	    return FALSE;
	}

	pNewport->fifoleft = 0;
	/* init bunch of registers */
	
	pNewport->shadow_drawmode0 = pNewportRegs->set.drawmode0;
	pNewport->shadow_colori = pNewportRegs->set.colori;
	pNewport->shadow_smask0x = pNewportRegs->set.smask0x;
	pNewport->shadow_smask0y = pNewportRegs->set.smask0y;
	
	pNewport->shadow_drawmode1 = pNewport->drawmode1;
	pNewportRegs->set.drawmode1 = pNewport->drawmode1;
	pNewport->setup_drawmode1 = pNewport->drawmode1 & ~NPORT_DMODE1_LOMASK;
	
	pNewport->shadow_xymove = 0;
	pNewportRegs->set.xymove = 0;
	
	pNewport->shadow_wrmask = 0xFFFFFF;
	pNewportRegs->set.wrmask = 0xFFFFFF;
	
	pNewport->shadow_colorvram = 0;
	pNewportRegs->set.colorvram = 0;
	
	pNewport->shadow_colorback = 0;
	pNewportRegs->set.colorback = 0;
	
	pNewport->clipsx = 0;
	pNewport->clipex = pScrn->virtualX-1;
	pNewport->clipsy = 0;
	pNewport->clipey = pScrn->virtualY-1;
	pNewport->skipleft = 0;
	
	BARF1("CLIPMODE %08X\n", pNewportRegs->cset.clipmode);
	BARF1("XYWIN %08X\n", pNewportRegs->cset.xywin);
	BARF1("CONFIG %08X\n", pNewportRegs->cset.config);
	BARF1("SMASK0X %08X\n", pNewportRegs->set.smask0x);
	BARF1("SMASK0Y %08X\n", pNewportRegs->set.smask0y);
	
/*
  set GIO bus timeout to highest possible value 4.32us 
  this will allow for longer bus stalls without bus error
*/	
	{
	    unsigned int conf;
	    conf = pNewportRegs->cset.config;
	    conf &= ~NPORT_CFG_TOMSK;
	    conf |= NPORT_CFG_TOMSK;
	    
	    conf &= ~NPORT_CFG_GDMSK;
	    conf |= NPORT_CFG_GDMSK;
	    
	    pNewportRegs->cset.config = conf;
	}
	
	BARF1("CONFIG %08X\n", pNewportRegs->cset.config);
	
	pNewport->shadow_clipmode = pNewportRegs->cset.clipmode;
	pNewportRegs->cset.clipmode |= 1; /* enable clipping mask 0 */
	NewportUpdateClipping(pNewport);
	
	return TRUE;
}

#endif 
/* NEWPORT_ACCEL */
