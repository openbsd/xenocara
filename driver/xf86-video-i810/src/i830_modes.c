#define DEBUG_VERB 2
/*
 * Copyright © 2002 David Dawes
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of the author(s) shall
 * not be used in advertising or otherwise to promote the sale, use or other
 * dealings in this Software without prior written authorization from
 * the author(s).
 *
 * Authors: David Dawes <dawes@xfree86.org>
 *
 * $XFree86: xc/programs/Xserver/hw/xfree86/os-support/vbe/vbeModes.c,v 1.6 2002/11/02 01:38:25 dawes Exp $
 */
/*
 * Modified by Alan Hourihane <alanh@tungstengraphics.com>
 * to support extended BIOS modes for the Intel chipsets
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_ansic.h"
#include "vbe.h"
#include "vbeModes.h"
#include "i830.h"

#include <math.h>

#define rint(x) floor(x)

#define MARGIN_PERCENT    1.8   /* % of active vertical image                */
#define CELL_GRAN         8.0   /* assumed character cell granularity        */
#define MIN_PORCH         1     /* minimum front porch                       */
#define V_SYNC_RQD        3     /* width of vsync in lines                   */
#define H_SYNC_PERCENT    8.0   /* width of hsync as % of total line         */
#define MIN_VSYNC_PLUS_BP 550.0 /* min time of vsync + back porch (microsec) */
#define M                 600.0 /* blanking formula gradient                 */
#define C                 40.0  /* blanking formula offset                   */
#define K                 128.0 /* blanking formula scaling factor           */
#define J                 20.0  /* blanking formula scaling factor           */

/* C' and M' are part of the Blanking Duty Cycle computation */

#define C_PRIME           (((C - J) * K/256.0) + J)
#define M_PRIME           (K/256.0 * M)

extern const int i830refreshes[];

static DisplayModePtr
I830GetGTF (int h_pixels, int v_lines, float freq,
                    int interlaced, int margins)
{
    float h_pixels_rnd;
    float v_lines_rnd;
    float v_field_rate_rqd;
    float top_margin;
    float bottom_margin;
    float interlace;
    float h_period_est;
    float vsync_plus_bp;
    float v_back_porch;
    float total_v_lines;
    float v_field_rate_est;
    float h_period;
    float v_field_rate;
    float v_frame_rate;
    float left_margin;
    float right_margin;
    float total_active_pixels;
    float ideal_duty_cycle;
    float h_blank;
    float total_pixels;
    float pixel_freq;
    float h_freq;

    float h_sync;
    float h_front_porch;
    float v_odd_front_porch_lines;
    char modename[20];
    DisplayModePtr m;

    m = xnfcalloc(sizeof(DisplayModeRec), 1);
    
    
    /*  1. In order to give correct results, the number of horizontal
     *  pixels requested is first processed to ensure that it is divisible
     *  by the character size, by rounding it to the nearest character
     *  cell boundary:
     *
     *  [H PIXELS RND] = ((ROUND([H PIXELS]/[CELL GRAN RND],0))*[CELLGRAN RND])
     */
    
    h_pixels_rnd = rint((float) h_pixels / CELL_GRAN) * CELL_GRAN;
    
    
    /*  2. If interlace is requested, the number of vertical lines assumed
     *  by the calculation must be halved, as the computation calculates
     *  the number of vertical lines per field. In either case, the
     *  number of lines is rounded to the nearest integer.
     *   
     *  [V LINES RND] = IF([INT RQD?]="y", ROUND([V LINES]/2,0),
     *                                     ROUND([V LINES],0))
     */

    v_lines_rnd = interlaced ?
            rint((float) v_lines) / 2.0 :
            rint((float) v_lines);
    
    /*  3. Find the frame rate required:
     *
     *  [V FIELD RATE RQD] = IF([INT RQD?]="y", [I/P FREQ RQD]*2,
     *                                          [I/P FREQ RQD])
     */

    v_field_rate_rqd = interlaced ? (freq * 2.0) : (freq);

    /*  4. Find number of lines in Top margin:
     *
     *  [TOP MARGIN (LINES)] = IF([MARGINS RQD?]="Y",
     *          ROUND(([MARGIN%]/100*[V LINES RND]),0),
     *          0)
     */

    top_margin = margins ? rint(MARGIN_PERCENT / 100.0 * v_lines_rnd) : (0.0);

    /*  5. Find number of lines in Bottom margin:
     *
     *  [BOT MARGIN (LINES)] = IF([MARGINS RQD?]="Y",
     *          ROUND(([MARGIN%]/100*[V LINES RND]),0),
     *          0)
     */

    bottom_margin = margins ? rint(MARGIN_PERCENT/100.0 * v_lines_rnd) : (0.0);

    /*  6. If interlace is required, then set variable [INTERLACE]=0.5:
     *   
     *  [INTERLACE]=(IF([INT RQD?]="y",0.5,0))
     */

    interlace = interlaced ? 0.5 : 0.0;

    /*  7. Estimate the Horizontal period
     *
     *  [H PERIOD EST] = ((1/[V FIELD RATE RQD]) - [MIN VSYNC+BP]/1000000) /
     *                    ([V LINES RND] + (2*[TOP MARGIN (LINES)]) +
     *                     [MIN PORCH RND]+[INTERLACE]) * 1000000
     */

    h_period_est = (((1.0/v_field_rate_rqd) - (MIN_VSYNC_PLUS_BP/1000000.0))
                    / (v_lines_rnd + (2*top_margin) + MIN_PORCH + interlace)
                    * 1000000.0);

    /*  8. Find the number of lines in V sync + back porch:
     *
     *  [V SYNC+BP] = ROUND(([MIN VSYNC+BP]/[H PERIOD EST]),0)
     */

    vsync_plus_bp = rint(MIN_VSYNC_PLUS_BP/h_period_est);

    /*  9. Find the number of lines in V back porch alone:
     *
     *  [V BACK PORCH] = [V SYNC+BP] - [V SYNC RND]
     *
     *  XXX is "[V SYNC RND]" a typo? should be [V SYNC RQD]?
     */
    
    v_back_porch = vsync_plus_bp - V_SYNC_RQD;
    
    /*  10. Find the total number of lines in Vertical field period:
     *
     *  [TOTAL V LINES] = [V LINES RND] + [TOP MARGIN (LINES)] +
     *                    [BOT MARGIN (LINES)] + [V SYNC+BP] + [INTERLACE] +
     *                    [MIN PORCH RND]
     */

    total_v_lines = v_lines_rnd + top_margin + bottom_margin + vsync_plus_bp +
        interlace + MIN_PORCH;
    
    /*  11. Estimate the Vertical field frequency:
     *
     *  [V FIELD RATE EST] = 1 / [H PERIOD EST] / [TOTAL V LINES] * 1000000
     */

    v_field_rate_est = 1.0 / h_period_est / total_v_lines * 1000000.0;
    
    /*  12. Find the actual horizontal period:
     *
     *  [H PERIOD] = [H PERIOD EST] / ([V FIELD RATE RQD] / [V FIELD RATE EST])
     */

    h_period = h_period_est / (v_field_rate_rqd / v_field_rate_est);
    
    /*  13. Find the actual Vertical field frequency:
     *
     *  [V FIELD RATE] = 1 / [H PERIOD] / [TOTAL V LINES] * 1000000
     */

    v_field_rate = 1.0 / h_period / total_v_lines * 1000000.0;

    /*  14. Find the Vertical frame frequency:
     *
     *  [V FRAME RATE] = (IF([INT RQD?]="y", [V FIELD RATE]/2, [V FIELD RATE]))
     */

    v_frame_rate = interlaced ? v_field_rate / 2.0 : v_field_rate;

    /*  15. Find number of pixels in left margin:
     *
     *  [LEFT MARGIN (PIXELS)] = (IF( [MARGINS RQD?]="Y",
     *          (ROUND( ([H PIXELS RND] * [MARGIN%] / 100 /
     *                   [CELL GRAN RND]),0)) * [CELL GRAN RND],
     *          0))
     */

    left_margin = margins ?
        rint(h_pixels_rnd * MARGIN_PERCENT / 100.0 / CELL_GRAN) * CELL_GRAN :
        0.0;
    
    /*  16. Find number of pixels in right margin:
     *
     *  [RIGHT MARGIN (PIXELS)] = (IF( [MARGINS RQD?]="Y",
     *          (ROUND( ([H PIXELS RND] * [MARGIN%] / 100 /
     *                   [CELL GRAN RND]),0)) * [CELL GRAN RND],
     *          0))
     */
    
    right_margin = margins ?
        rint(h_pixels_rnd * MARGIN_PERCENT / 100.0 / CELL_GRAN) * CELL_GRAN :
        0.0;
    
    /*  17. Find total number of active pixels in image and left and right
     *  margins:
     *
     *  [TOTAL ACTIVE PIXELS] = [H PIXELS RND] + [LEFT MARGIN (PIXELS)] +
     *                          [RIGHT MARGIN (PIXELS)]
     */

    total_active_pixels = h_pixels_rnd + left_margin + right_margin;
    
    /*  18. Find the ideal blanking duty cycle from the blanking duty cycle
     *  equation:
     *
     *  [IDEAL DUTY CYCLE] = [C'] - ([M']*[H PERIOD]/1000)
     */

    ideal_duty_cycle = C_PRIME - (M_PRIME * h_period / 1000.0);
    
    /*  19. Find the number of pixels in the blanking time to the nearest
     *  double character cell:
     *
     *  [H BLANK (PIXELS)] = (ROUND(([TOTAL ACTIVE PIXELS] *
     *                               [IDEAL DUTY CYCLE] /
     *                               (100-[IDEAL DUTY CYCLE]) /
     *                               (2*[CELL GRAN RND])), 0))
     *                       * (2*[CELL GRAN RND])
     */

    h_blank = rint(total_active_pixels *
                   ideal_duty_cycle /
                   (100.0 - ideal_duty_cycle) /
                   (2.0 * CELL_GRAN)) * (2.0 * CELL_GRAN);
    
    /*  20. Find total number of pixels:
     *
     *  [TOTAL PIXELS] = [TOTAL ACTIVE PIXELS] + [H BLANK (PIXELS)]
     */

    total_pixels = total_active_pixels + h_blank;
    
    /*  21. Find pixel clock frequency:
     *
     *  [PIXEL FREQ] = [TOTAL PIXELS] / [H PERIOD]
     */
    
    pixel_freq = total_pixels / h_period;
    
    /*  22. Find horizontal frequency:
     *
     *  [H FREQ] = 1000 / [H PERIOD]
     */

    h_freq = 1000.0 / h_period;
    

    /* Stage 1 computations are now complete; I should really pass
       the results to another function and do the Stage 2
       computations, but I only need a few more values so I'll just
       append the computations here for now */

    

    /*  17. Find the number of pixels in the horizontal sync period:
     *
     *  [H SYNC (PIXELS)] =(ROUND(([H SYNC%] / 100 * [TOTAL PIXELS] /
     *                             [CELL GRAN RND]),0))*[CELL GRAN RND]
     */

    h_sync = rint(H_SYNC_PERCENT/100.0 * total_pixels / CELL_GRAN) * CELL_GRAN;

    /*  18. Find the number of pixels in the horizontal front porch period:
     *
     *  [H FRONT PORCH (PIXELS)] = ([H BLANK (PIXELS)]/2)-[H SYNC (PIXELS)]
     */

    h_front_porch = (h_blank / 2.0) - h_sync;

    /*  36. Find the number of lines in the odd front porch period:
     *
     *  [V ODD FRONT PORCH(LINES)]=([MIN PORCH RND]+[INTERLACE])
     */
    
    v_odd_front_porch_lines = MIN_PORCH + interlace;
    
    /* finally, pack the results in the DisplayMode struct */
    
    m->HDisplay  = (int) (h_pixels_rnd);
    m->HSyncStart = (int) (h_pixels_rnd + h_front_porch);
    m->HSyncEnd = (int) (h_pixels_rnd + h_front_porch + h_sync);
    m->HTotal = (int) (total_pixels);

    m->VDisplay  = (int) (v_lines_rnd);
    m->VSyncStart = (int) (v_lines_rnd + v_odd_front_porch_lines);
    m->VSyncEnd = (int) (int) (v_lines_rnd + v_odd_front_porch_lines + V_SYNC_RQD);
    m->VTotal = (int) (total_v_lines);

    m->Clock   = (int)(pixel_freq * 1000);
    m->SynthClock   = m->Clock;
    m->HSync = h_freq;
    m->VRefresh = freq;

    snprintf(modename, sizeof(modename), "%dx%d", m->HDisplay,m->VDisplay);
    m->name = xnfstrdup(modename);

    return (m);
}

static DisplayModePtr
CheckMode(ScrnInfoPtr pScrn, vbeInfoPtr pVbe, VbeInfoBlock *vbe, int id,
	  int flags)
{
    CARD16 major, minor;
    VbeModeInfoBlock *mode;
    DisplayModePtr p = NULL, pMode = NULL;
    VbeModeInfoData *data;
    Bool modeOK = FALSE;
    ModeStatus status = MODE_OK;

    major = (unsigned)(vbe->VESAVersion >> 8);
    minor = vbe->VESAVersion & 0xff;

    if ((mode = VBEGetModeInfo(pVbe, id)) == NULL)
	return NULL;

    /* Does the mode match the depth/bpp? */
    /* Some BIOS's set BitsPerPixel to 15 instead of 16 for 15/16 */
    if (VBE_MODE_USABLE(mode, flags) &&
	((pScrn->bitsPerPixel == 1 && !VBE_MODE_COLOR(mode)) ||
	 (mode->BitsPerPixel > 8 &&
	  (mode->RedMaskSize + mode->GreenMaskSize +
	   mode->BlueMaskSize) == pScrn->depth &&
	  mode->BitsPerPixel == pScrn->bitsPerPixel) ||
	 (mode->BitsPerPixel == 15 && pScrn->depth == 15) ||
	 (mode->BitsPerPixel <= 8 &&
	  mode->BitsPerPixel == pScrn->bitsPerPixel))) {
	modeOK = TRUE;
	xf86ErrorFVerb(DEBUG_VERB, "*");
    }

    if (mode->XResolution && mode->YResolution &&
	!I830CheckModeSupport(pScrn, mode->XResolution, mode->YResolution, id)) 
	modeOK = FALSE;


    /*
     * Check if there's a valid monitor mode that this one can be matched
     * up with from the 'specified' modes list.
     */
    if (modeOK) {
	for (p = pScrn->monitor->Modes; p != NULL; p = p->next) {
	    if ((p->type != 0) ||
		(p->HDisplay != mode->XResolution) ||
		(p->VDisplay != mode->YResolution) ||
		(p->Flags & (V_INTERLACE | V_DBLSCAN | V_CLKDIV2)))
		continue;
	    status = xf86CheckModeForMonitor(p, pScrn->monitor);
	    if (status == MODE_OK) {
		modeOK = TRUE;
		break;
	    }
	}
	if (p) {
    		pMode = xnfcalloc(sizeof(DisplayModeRec), 1);
		memcpy((char*)pMode,(char*)p,sizeof(DisplayModeRec));
    		pMode->name = xnfstrdup(p->name);
	}
    } 

    /*
     * Now, check if there's a valid monitor mode that this one can be matched
     * up with from the default modes list. i.e. VESA modes in xf86DefModes.c
     */
    if (modeOK && !pMode) {
	int refresh = 0, calcrefresh = 0;
	DisplayModePtr newMode = NULL;

	for (p = pScrn->monitor->Modes; p != NULL; p = p->next) {
	    calcrefresh = (int)(((double)(p->Clock * 1000) /
                       (double)(p->HTotal * p->VTotal)) * 100);
	    if ((p->type != M_T_DEFAULT) ||
		(p->HDisplay != mode->XResolution) ||
		(p->VDisplay != mode->YResolution) ||
		(p->Flags & (V_INTERLACE | V_DBLSCAN | V_CLKDIV2)))
		continue;
	    status = xf86CheckModeForMonitor(p, pScrn->monitor);
	    if (status == MODE_OK) {
	    	if (calcrefresh > refresh) {
			refresh = calcrefresh;
			newMode = p;
		}
		modeOK = TRUE;
	    }
	}
	if (newMode) {
    		pMode = xnfcalloc(sizeof(DisplayModeRec), 1);
		memcpy((char*)pMode,(char*)newMode,sizeof(DisplayModeRec));
    		pMode->name = xnfstrdup(newMode->name);
	}
    } 

    /*
     * Check if there's a valid monitor mode that this one can be matched
     * up with.  The actual matching is done later.
     */
    if (modeOK && !pMode) {
	float vrefresh = 0.0f;
	int i;

	for (i=0;i<pScrn->monitor->nVrefresh;i++) {

  	    for (vrefresh = pScrn->monitor->vrefresh[i].hi; 
		 vrefresh >= pScrn->monitor->vrefresh[i].lo; vrefresh -= 1.0f) {

	        if (vrefresh != (float)0.0f) {
                    float best_vrefresh;
                    int int_vrefresh;
 
                    /* Find the best refresh for the Intel chipsets */
                    int_vrefresh = I830GetBestRefresh(pScrn, (int)vrefresh);
		    best_vrefresh = (float)i830refreshes[int_vrefresh];

                    /* Now, grab the best mode from the available refresh */
		    pMode = I830GetGTF(mode->XResolution, mode->YResolution, 
							best_vrefresh, 0, 0);

    	            pMode->type = M_T_BUILTIN;

	            status = xf86CheckModeForMonitor(pMode, pScrn->monitor);
	            if (status == MODE_OK) {
			if (major >= 3) {
			    if (pMode->Clock * 1000 <= mode->MaxPixelClock)
				modeOK = TRUE;
			    else
				modeOK = FALSE;
			} else
			    modeOK = TRUE;
	            } else
	    	        modeOK = FALSE;
  	            pMode->status = status;
	        } else { 
	            modeOK = FALSE;
	        }
	        if (modeOK) break;
            }
	    if (modeOK) break;
        }
    }

    xf86ErrorFVerb(DEBUG_VERB,
	    "Mode: %x (%dx%d)\n", id, mode->XResolution, mode->YResolution);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	ModeAttributes: 0x%x\n", mode->ModeAttributes);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	WinAAttributes: 0x%x\n", mode->WinAAttributes);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	WinBAttributes: 0x%x\n", mode->WinBAttributes);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	WinGranularity: %d\n", mode->WinGranularity);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	WinSize: %d\n", mode->WinSize);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	WinASegment: 0x%x\n", mode->WinASegment);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	WinBSegment: 0x%x\n", mode->WinBSegment);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	WinFuncPtr: 0x%lx\n", mode->WinFuncPtr);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	BytesPerScanline: %d\n", mode->BytesPerScanline);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	XResolution: %d\n", mode->XResolution);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	YResolution: %d\n", mode->YResolution);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	XCharSize: %d\n", mode->XCharSize);
    xf86ErrorFVerb(DEBUG_VERB,
           "	YCharSize: %d\n", mode->YCharSize);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	NumberOfPlanes: %d\n", mode->NumberOfPlanes);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	BitsPerPixel: %d\n", mode->BitsPerPixel);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	NumberOfBanks: %d\n", mode->NumberOfBanks);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	MemoryModel: %d\n", mode->MemoryModel);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	BankSize: %d\n", mode->BankSize);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	NumberOfImages: %d\n", mode->NumberOfImages);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	RedMaskSize: %d\n", mode->RedMaskSize);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	RedFieldPosition: %d\n", mode->RedFieldPosition);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	GreenMaskSize: %d\n", mode->GreenMaskSize);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	GreenFieldPosition: %d\n", mode->GreenFieldPosition);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	BlueMaskSize: %d\n", mode->BlueMaskSize);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	BlueFieldPosition: %d\n", mode->BlueFieldPosition);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	RsvdMaskSize: %d\n", mode->RsvdMaskSize);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	RsvdFieldPosition: %d\n", mode->RsvdFieldPosition);
    xf86ErrorFVerb(DEBUG_VERB,
	    "	DirectColorModeInfo: %d\n", mode->DirectColorModeInfo);
    if (major >= 2) {
	xf86ErrorFVerb(DEBUG_VERB,
		"	PhysBasePtr: 0x%lx\n", mode->PhysBasePtr);
	if (major >= 3) {
	    xf86ErrorFVerb(DEBUG_VERB,
		    "	LinBytesPerScanLine: %d\n", mode->LinBytesPerScanLine);
	    xf86ErrorFVerb(DEBUG_VERB,
		    "	BnkNumberOfImagePages: %d\n", mode->BnkNumberOfImagePages);
	    xf86ErrorFVerb(DEBUG_VERB,
		    "	LinNumberOfImagePages: %d\n", mode->LinNumberOfImagePages);
	    xf86ErrorFVerb(DEBUG_VERB,
		    "	LinRedMaskSize: %d\n", mode->LinRedMaskSize);
	    xf86ErrorFVerb(DEBUG_VERB,
		    "	LinRedFieldPosition: %d\n", mode->LinRedFieldPosition);
	    xf86ErrorFVerb(DEBUG_VERB,
		    "	LinGreenMaskSize: %d\n", mode->LinGreenMaskSize);
	    xf86ErrorFVerb(DEBUG_VERB,
		    "	LinGreenFieldPosition: %d\n", mode->LinGreenFieldPosition);
	    xf86ErrorFVerb(DEBUG_VERB,
		    "	LinBlueMaskSize: %d\n", mode->LinBlueMaskSize);
	    xf86ErrorFVerb(DEBUG_VERB,
		    "	LinBlueFieldPosition: %d\n", mode->LinBlueFieldPosition);
	    xf86ErrorFVerb(DEBUG_VERB,
		    "	LinRsvdMaskSize: %d\n", mode->LinRsvdMaskSize);
	    xf86ErrorFVerb(DEBUG_VERB,
		    "	LinRsvdFieldPosition: %d\n", mode->LinRsvdFieldPosition);
	    xf86ErrorFVerb(DEBUG_VERB,
		    "	MaxPixelClock: %ld\n", mode->MaxPixelClock);
	}
    }

    if (!modeOK) {
	VBEFreeModeInfo(mode);
	if (pMode)
	    xfree(pMode);
	return NULL;
    }

    pMode->status = MODE_OK;
    pMode->type = M_T_BUILTIN;

    /* for adjust frame */
    pMode->HDisplay = mode->XResolution;
    pMode->VDisplay = mode->YResolution;

    data = xnfcalloc(sizeof(VbeModeInfoData), 1);
    data->mode = id;
    data->data = mode;
    pMode->PrivSize = sizeof(VbeModeInfoData);
    pMode->Private = (INT32*)data;
    pMode->next = NULL;
    return pMode;
}

/*
 * Check the available BIOS modes, and extract those that match the
 * requirements into the modePool.  Note: modePool is a NULL-terminated
 * list.
 */

DisplayModePtr
I830GetModePool(ScrnInfoPtr pScrn, vbeInfoPtr pVbe, VbeInfoBlock *vbe)
{
   DisplayModePtr pMode, p = NULL, modePool = NULL;
   int i = 0;

   for (i = 0; i < 0x7F; i++) {
      if ((pMode = CheckMode(pScrn, pVbe, vbe, i, V_MODETYPE_VGA)) != NULL) {
         ModeStatus status = MODE_OK;

		/* Check the mode against a specified virtual size (if any) */
		if (pScrn->display->virtualX > 0 &&
		    pMode->HDisplay > pScrn->display->virtualX) {
		    status = MODE_VIRTUAL_X;
		}
		if (pScrn->display->virtualY > 0 &&
		    pMode->VDisplay > pScrn->display->virtualY) {
		    status = MODE_VIRTUAL_Y;
		}
		if (status != MODE_OK) {
		     xf86DrvMsg(pScrn->scrnIndex, X_INFO,
				"Not using mode \"%dx%d\" (%s)\n",
				pMode->HDisplay, pMode->VDisplay,
				xf86ModeStatusToString(status));
		} else {
		    if (p == NULL) {
			modePool = pMode;
		    } else {
			p->next = pMode;
		    }
		    pMode->prev = NULL;
		    p = pMode;
		}
	    }
	}
    return modePool;
}

/*
 * Go through the monitor modes and selecting the best set of
 * parameters for each BIOS mode.  Note: This is only supported in
 * VBE version 3.0 or later.
 */
void
I830SetModeParameters(ScrnInfoPtr pScrn, vbeInfoPtr pVbe)
{
    DisplayModePtr pMode;
    VbeModeInfoData *data;

    pMode = pScrn->modes;
    do {
	int clock;

	data = (VbeModeInfoData*)pMode->Private;
	data->block = xcalloc(sizeof(VbeCRTCInfoBlock), 1);
	data->block->HorizontalTotal = pMode->HTotal;
	data->block->HorizontalSyncStart = pMode->HSyncStart;
	data->block->HorizontalSyncEnd = pMode->HSyncEnd;
	data->block->VerticalTotal = pMode->VTotal;
	data->block->VerticalSyncStart = pMode->VSyncStart;
	data->block->VerticalSyncEnd = pMode->VSyncEnd;
	data->block->Flags = ((pMode->Flags & V_NHSYNC) ? CRTC_NHSYNC : 0) |
				 ((pMode->Flags & V_NVSYNC) ? CRTC_NVSYNC : 0);
	data->block->PixelClock = pMode->Clock * 1000;
	/* XXX May not have this. */
	clock = VBEGetPixelClock(pVbe, data->mode, data->block->PixelClock);
	if (clock)
	    data->block->PixelClock = clock;
#ifdef DEBUG
	ErrorF("Setting clock %.2fMHz, closest is %.2fMHz\n",
		(double)data->block->PixelClock / 1000000.0, 
		(double)clock / 1000000.0);
#endif
	data->mode |= (1 << 11);
	if (pMode->VRefresh != 0) {
	    data->block->RefreshRate = pMode->VRefresh * 100;
	} else {
	    data->block->RefreshRate = (int)(((double)(data->block->PixelClock)/
                       (double)(pMode->HTotal * pMode->VTotal)) * 100);
	}
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Attempting to use %2.2fHz refresh for mode \"%s\" (%x)\n",
		       (float)(((double)(data->block->PixelClock) / (double)(pMode->HTotal * pMode->VTotal))), pMode->name, data->mode);
#ifdef DEBUG
	ErrorF("Video Modeline: ID: 0x%x Name: %s %i %i %i %i - "
	       "  %i %i %i %i %.2f MHz Refresh: %.2f Hz\n",
	       data->mode, pMode->name, pMode->HDisplay, pMode->HSyncStart,
	       pMode->HSyncEnd, pMode->HTotal, pMode->VDisplay,
	       pMode->VSyncStart,pMode->VSyncEnd,pMode->VTotal,
	       (double)data->block->PixelClock/1000000.0,
	       (double)data->block->RefreshRate/100);
#endif
	pMode = pMode->next;
    } while (pMode != pScrn->modes);
}

void
I830PrintModes(ScrnInfoPtr scrp)
{
    DisplayModePtr p;
    float hsync, refresh = 0;
    char *desc, *desc2, *prefix, *uprefix;

    if (scrp == NULL)
	return;

    xf86DrvMsg(scrp->scrnIndex, scrp->virtualFrom, "Virtual size is %dx%d "
	       "(pitch %d)\n", scrp->virtualX, scrp->virtualY,
	       scrp->displayWidth);
    
    p = scrp->modes;
    if (p == NULL)
	return;

    do {
	desc = desc2 = "";
	if (p->HSync > 0.0)
	    hsync = p->HSync;
	else if (p->HTotal > 0)
	    hsync = (float)p->Clock / (float)p->HTotal;
	else
	    hsync = 0.0;
	if (p->VTotal > 0)
	    refresh = hsync * 1000.0 / p->VTotal;
	if (p->Flags & V_INTERLACE) {
	    refresh *= 2.0;
	    desc = " (I)";
	}
	if (p->Flags & V_DBLSCAN) {
	    refresh /= 2.0;
	    desc = " (D)";
	}
	if (p->VScan > 1) {
	    refresh /= p->VScan;
	    desc2 = " (VScan)";
	}
	if (p->VRefresh > 0.0)
	    refresh = p->VRefresh;
	if (p->type & M_T_BUILTIN)
	    prefix = "Built-in mode";
	else if (p->type & M_T_DEFAULT)
	    prefix = "Default mode";
	else
	    prefix = "Mode";
	if (p->type & M_T_USERDEF)
	    uprefix = "*";
	else
	    uprefix = " ";
	if (p->name)
	    xf86DrvMsg(scrp->scrnIndex, X_CONFIG,
			   "%s%s \"%s\"\n", uprefix, prefix, p->name);
	else
	    xf86DrvMsg(scrp->scrnIndex, X_PROBED,
			   "%s%s %dx%d (unnamed)\n",
			   uprefix, prefix, p->HDisplay, p->VDisplay);
	p = p->next;
    } while (p != NULL && p != scrp->modes);
}
