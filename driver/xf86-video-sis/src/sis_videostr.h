/*
 * Xv driver for SiS 300, 315 and 330 series
 *
 * Overlay port private structure
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1) Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2) Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3) The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author:    Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifndef _SIS_VIDEOSTR_H_
#define _SIS_VIDEOSTR_H_

#define GET_PORT_PRIVATE(pScrn) \
   (SISPortPrivPtr)((SISPTR(pScrn))->adaptor->pPortPrivates[0].ptr)

typedef struct {
    void *	 handle;
    CARD32	 bufAddr[2];

    UChar	 currentBuf;

    short	 drw_x, drw_y, drw_w, drw_h;
    short	 src_x, src_y, src_w, src_h;
    int		 id;
    short	 srcPitch, height;

    char         brightness;
    UChar        contrast;
    char 	 hue;
    short        saturation;

    RegionRec    clip;
    CARD32       colorKey;
    Bool 	 autopaintColorKey;

    Bool 	 disablegfx;
    Bool	 disablegfxlr;

    Bool         usechromakey;
    Bool	 insidechromakey, yuvchromakey;
    CARD32	 chromamin, chromamax;
#ifdef SISDEINT
    int		 deinterlacemethod;
#endif

    CARD32       videoStatus;
    Bool	 overlayStatus;
    Time         offTime;
    Time         freeTime;

    CARD32       displayMode;
    Bool	 bridgeIsSlave;

    Bool         hasTwoOverlays;   /* Chipset has two overlays */
    Bool         dualHeadMode;     /* We're running in DHM */

    Bool  	 NoOverlay;
    Bool	 PrevOverlay;

    Bool	 AllowSwitchCRT;
    int 	 crtnum;	   /* 0=CRT1, 1=CRT2 */

    Bool         needToScale;      /* Need to scale video */

    Bool	 havetapscaler;	   /* 4tap scaler (340) */

    Bool    	 mustresettap;

    int          shiftValue;       /* 315/330 series need word addr/pitch, 300 series double word */

    short  	 linebufMergeLimit;
    CARD8        linebufmask;

    short        oldx1, oldx2, oldy1, oldy2;
#ifdef SISMERGED
    short        oldx1_2, oldx2_2, oldy1_2, oldy2_2;
    Bool    	 mustresettap2;
#endif
    int          mustwait;

    Bool         grabbedByV4L;	   /* V4L stuff */
    int          pitch;
    int          offset;

    int 	 modeflags;	   /* Flags field of current display mode */

    int 	 tvxpos, tvypos;
    Bool 	 updatetvxpos, updatetvypos;

    Bool	 is661741760;
    Bool	 is760;
    Bool	 is340;
    Bool	 is761;
    Bool	 isXGI;

} SISPortPrivRec, *SISPortPrivPtr;

#endif

