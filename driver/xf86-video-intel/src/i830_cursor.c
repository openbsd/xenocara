/* -*- c-basic-offset: 4 -*- */
/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
Copyright © 2002 David Dawes
All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/
/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/i810/i830_cursor.c,v 1.6 2002/12/18 15:49:01 dawes Exp $ */

/*
 * Reformatted with GNU indent (2.2.8), using the following options:
 *
 *    -bad -bap -c41 -cd0 -ncdb -ci6 -cli0 -cp0 -ncs -d0 -di3 -i3 -ip3 -l78
 *    -lp -npcs -psl -sob -ss -br -ce -sc -hnl
 *
 * This provides a good match with the original i810 code and preferred
 * XFree86 formatting conventions.
 *
 * When editing this driver, please follow the existing formatting, and edit
 * with <TAB> characters expanded at 8-column intervals.
 */

/*
 * Authors:
 *   Keith Whitwell <keith@tungstengraphics.com>
 *   David Dawes <dawes@xfree86.org>
 *
 * Updated for Dual Head capabilities:
 *   Alan Hourihane <alanh@tungstengraphics.com>
 *
 * Add ARGB HW cursor support:
 *   Alan Hourihane <alanh@tungstengraphics.com>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "xf86.h"
#include "xf86_OSproc.h"
#include "compiler.h"

#include "xf86fbman.h"

#include "i830.h"

static void
I830SetPipeCursorBase (xf86CrtcPtr crtc)
{
    ScrnInfoPtr		pScrn = crtc->scrn;
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    int			pipe = intel_crtc->pipe;
    I830Ptr		pI830 = I830PTR(pScrn);
    int			cursor_base;

    cursor_base = (pipe == 0) ? CURSOR_A_BASE : CURSOR_B_BASE;
    
    if (intel_crtc->cursor_is_argb)
       OUTREG(cursor_base, intel_crtc->cursor_argb_addr);
    else
       OUTREG(cursor_base, intel_crtc->cursor_addr);
}

void
I830InitHWCursor(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    I830Ptr		pI830 = I830PTR(pScrn);
    uint32_t		temp;
    int			i;

    DPRINTF(PFX, "I830InitHWCursor\n");

    if (!IS_I9XX(pI830))
       OUTREG(CURSOR_SIZE, (I810_CURSOR_Y << 12) | I810_CURSOR_X);

    /* Initialise the HW cursor registers, leaving the cursor hidden. */
    for (i = 0; i < xf86_config->num_crtc; i++)
    {
	int   cursor_control = i == 0 ? CURSOR_A_CONTROL : CURSOR_B_CONTROL;
	
	temp = INREG(cursor_control);
	if (IS_MOBILE(pI830) || IS_I9XX(pI830)) 
	{
	    temp &= ~(CURSOR_MODE | MCURSOR_GAMMA_ENABLE |
		      MCURSOR_MEM_TYPE_LOCAL |
		      MCURSOR_PIPE_SELECT);
	    temp |= (i << 28);
	    temp |= CURSOR_MODE_DISABLE;
	}
	else
	{
	    temp &= ~(CURSOR_ENABLE|CURSOR_GAMMA_ENABLE);
	}
	
	/* Need to set control, then address. */
	OUTREG(cursor_control, temp);
	I830SetPipeCursorBase(xf86_config->crtc[i]);
    }
}

Bool
I830CursorInit(ScreenPtr pScreen)
{
    return xf86_cursors_init (pScreen, I810_CURSOR_X, I810_CURSOR_Y,
			      (HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
			       HARDWARE_CURSOR_BIT_ORDER_MSBFIRST |
			       HARDWARE_CURSOR_INVERT_MASK |
			       HARDWARE_CURSOR_SWAP_SOURCE_AND_MASK |
			       HARDWARE_CURSOR_AND_SOURCE_WITH_MASK |
			       HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_64 |
			       HARDWARE_CURSOR_ARGB));
}

void
i830_crtc_load_cursor_image (xf86CrtcPtr crtc, unsigned char *src)
{
    I830Ptr		pI830 = I830PTR(crtc->scrn);
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    uint8_t		*pcurs;

    pcurs = pI830->FbBase + intel_crtc->cursor_offset;

    intel_crtc->cursor_is_argb = FALSE;
    memcpy (pcurs, src, I810_CURSOR_X * I810_CURSOR_Y / 4);
}

#ifdef ARGB_CURSOR
void
i830_crtc_load_cursor_argb (xf86CrtcPtr crtc, CARD32 *image)
{
    I830Ptr		pI830 = I830PTR(crtc->scrn);
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    uint32_t		*pcurs;

    pcurs = (uint32_t *) (pI830->FbBase + intel_crtc->cursor_argb_offset);

    intel_crtc->cursor_is_argb = TRUE;
    memcpy (pcurs, image, I810_CURSOR_Y * I810_CURSOR_X * 4);
}
#endif

void
i830_crtc_set_cursor_position (xf86CrtcPtr crtc, int x, int y)
{
    ScrnInfoPtr		scrn = crtc->scrn;
    I830Ptr		pI830 = I830PTR(scrn);
    I830CrtcPrivatePtr	intel_crtc = I830CrtcPrivate(crtc);
    uint32_t		temp;

    temp = 0;
    if (x < 0) {
	temp |= (CURSOR_POS_SIGN << CURSOR_X_SHIFT);
	x = -x;
    }
    if (y < 0) {
	temp |= (CURSOR_POS_SIGN << CURSOR_Y_SHIFT);
	y = -y;
    }
    temp |= ((x & CURSOR_POS_MASK) << CURSOR_X_SHIFT);
    temp |= ((y & CURSOR_POS_MASK) << CURSOR_Y_SHIFT);

    switch (intel_crtc->pipe) {
    case 0:
	OUTREG(CURSOR_A_POSITION, temp);
	break;
    case 1:
	OUTREG(CURSOR_B_POSITION, temp);
	break;
    }
    
    if (crtc->cursor_shown)
	I830SetPipeCursorBase (crtc);
}

void
i830_crtc_show_cursor (xf86CrtcPtr crtc)
{
    ScrnInfoPtr		scrn = crtc->scrn;
    I830Ptr		pI830 = I830PTR(scrn);
    I830CrtcPrivatePtr	intel_crtc = I830CrtcPrivate(crtc);
    int			pipe = intel_crtc->pipe;
    uint32_t		temp;
    int			cursor_control = (pipe == 0 ? CURSOR_A_CONTROL :
					  CURSOR_B_CONTROL);
    
    temp = INREG(cursor_control);
    
    if (IS_MOBILE(pI830) || IS_I9XX(pI830)) 
    {
	temp &= ~(CURSOR_MODE | MCURSOR_PIPE_SELECT);
	if (intel_crtc->cursor_is_argb)
	    temp |= CURSOR_MODE_64_ARGB_AX | MCURSOR_GAMMA_ENABLE;
	else
	    temp |= CURSOR_MODE_64_4C_AX;

	temp |= (pipe << 28); /* Connect to correct pipe */
    }
    else 
    {
	temp &= ~(CURSOR_FORMAT_MASK);
	temp |= CURSOR_ENABLE;
	if (intel_crtc->cursor_is_argb)
	    temp |= CURSOR_FORMAT_ARGB | CURSOR_GAMMA_ENABLE;
	else
	    temp |= CURSOR_FORMAT_3C;
    }
    
    /* Need to set mode, then address. */
    OUTREG(cursor_control, temp);
    I830SetPipeCursorBase (crtc);
}

void
i830_crtc_hide_cursor (xf86CrtcPtr crtc)
{
    ScrnInfoPtr		scrn = crtc->scrn;
    I830Ptr		pI830 = I830PTR(scrn);
    I830CrtcPrivatePtr	intel_crtc = I830CrtcPrivate(crtc);
    int			pipe = intel_crtc->pipe;
    uint32_t		temp;
    int			cursor_control = (pipe == 0 ? CURSOR_A_CONTROL :
					  CURSOR_B_CONTROL);
    
    temp = INREG(cursor_control);
    
    if (IS_MOBILE(pI830) || IS_I9XX(pI830)) 
    {
	temp &= ~(CURSOR_MODE|MCURSOR_GAMMA_ENABLE);
	temp |= CURSOR_MODE_DISABLE;
    }
    else
	temp &= ~(CURSOR_ENABLE|CURSOR_GAMMA_ENABLE);

    /* Need to set mode, then address. */
    OUTREG(cursor_control, temp);
    I830SetPipeCursorBase (crtc);
}

void
i830_crtc_set_cursor_colors (xf86CrtcPtr crtc, int bg, int fg)
{
    ScrnInfoPtr		scrn = crtc->scrn;
    I830Ptr		pI830 = I830PTR(scrn);
    I830CrtcPrivatePtr	intel_crtc = I830CrtcPrivate(crtc);
    int			pipe = intel_crtc->pipe;
    int			pal0 = pipe == 0 ? CURSOR_A_PALETTE0 : CURSOR_B_PALETTE0;

    OUTREG(pal0 +  0, bg & 0x00ffffff);
    OUTREG(pal0 +  4, fg & 0x00ffffff);
    OUTREG(pal0 +  8, fg & 0x00ffffff);
    OUTREG(pal0 + 12, bg & 0x00ffffff);
}

void
i830_update_cursor_offsets (ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    if (pI830->cursor_mem) {
	unsigned long cursor_offset_base = pI830->cursor_mem->offset;
	unsigned long cursor_addr_base, offset = 0;

	/* Single memory buffer for cursors */
	if (pI830->CursorNeedsPhysical) {
	    /* On any hardware that requires physical addresses for cursors,
	     * the PTEs don't support memory above 4GB, so we can safely
	     * ignore the top 32 bits of cursor_mem->bus_addr.
	     */
	    cursor_addr_base = (unsigned long)pI830->cursor_mem->bus_addr;
	} else
	    cursor_addr_base = pI830->cursor_mem->offset;

	for (i = 0; i < xf86_config->num_crtc; i++) {
	    xf86CrtcPtr crtc = xf86_config->crtc[i];
	    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;

	    intel_crtc->cursor_argb_addr = cursor_addr_base + offset;
	    intel_crtc->cursor_argb_offset = cursor_offset_base + offset;
	    offset += HWCURSOR_SIZE_ARGB;

	    intel_crtc->cursor_addr = cursor_addr_base + offset;
	    intel_crtc->cursor_offset = cursor_offset_base + offset;
	    offset += HWCURSOR_SIZE;
	}
    } else {
	/* Separate allocations per cursor */
	for (i = 0; i < xf86_config->num_crtc; i++) {
	    xf86CrtcPtr crtc = xf86_config->crtc[i];
	    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;

	    if (pI830->CursorNeedsPhysical) {
		intel_crtc->cursor_addr =
		    pI830->cursor_mem_classic[i]->bus_addr;
		intel_crtc->cursor_argb_addr =
		    pI830->cursor_mem_argb[i]->bus_addr;
	    } else {
		intel_crtc->cursor_addr =
		    pI830->cursor_mem_classic[i]->offset;
		intel_crtc->cursor_argb_addr =
		    pI830->cursor_mem_argb[i]->offset;
	    }
	    intel_crtc->cursor_offset = pI830->cursor_mem_classic[i]->offset;
	    intel_crtc->cursor_argb_offset = pI830->cursor_mem_argb[i]->offset;
	}
    }
}
