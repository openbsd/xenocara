/*
 * Copyright 2007-2008 Gabriel Mansi.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * GTF and CVT timing calculator
 *
 * Based on
 * GTF spreadsheet developed by Andy Morrish
 * http://www.vesa.org/Public/GTF/GTF_V1R1.xls
 * and
 * CVT spreadsheet developed by Graham Loveridge
 * http://www.vesa.org/Public/CVT/CVTd6r1.xls
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "via_timing.h"

__inline__ static float froundf(Bool gtf, float f) {
    return gtf ? rintf(f) : floorf(f) ;
}

#define MODE_NAME_LENGTH 20
static int timingGenerateMode(DisplayModePtr mode, Bool gtf, int width, int height, float refresh, Bool interlaced, Bool reducedBlanking) {

    if (!mode) return TIMING_ERR_BAD_ALLOC ;

    int ret = 0 ;
    float H_PIXELS = width ;
    float V_LINES = height ;
    Bool MARGINS_RQD = FALSE ;
    Bool INT_RQD = interlaced ;
    float IP_FREQ_RQD = refresh ;
    Bool RED_BLANK_RQD = reducedBlanking && !gtf ;

    if (!gtf) {
        if (refresh != 50.0f
                && refresh != 60.0f
                && refresh != 75.0f
                && refresh != 85.0f )
            ret |= TIMING_CVT_WARN_REFRESH_RATE ;

        if (reducedBlanking && refresh != 60.0f)
            ret |= TIMING_CVT_WARN_REFRESH_RATE_NOT_RB ;

    }
    /* 1) These are the default values that define the MARGIN size:  */
    /* Top/ bottom MARGIN size as % of height  (%)  {DEFAULT = 1.8} */
    float TOP_BOTTOM_MARGIN = 1.8f ;

    /* 2) This default value defines the horizontal timing boundaries: */
    /* Character cell horizontal granularity (pixels)  {DEFAULT = 8} */
    float CELL_GRAN = 8.0f ;

    /* 3) These default values define analog system sync pulse width limitations: */
    /* 4:3 -> 4, 16:9 -> 5, 16: -> 6, 5:4 -> 7, 15:9 -> 7,
     * Reserved -> 8, Reserved -> 9, Custom -> 10
     */
    float V_SYNC_RQD = 3.0f ;
    float vSync ;
    if (gtf) {
        vSync = V_SYNC_RQD ;
    } else {
        float aspectRatio = (float)width / height ;
        if ( aspectRatio == 4.0f / 3.0f ) vSync = 4 ;
        else if ( aspectRatio == 16.0f / 9.0f ) vSync = 5 ;
        else if ( aspectRatio == 16.0f / 10.0f ) vSync = 6 ;
        else if ( aspectRatio == 5.0f / 4.0f || aspectRatio == 15.0f / 9.0f ) vSync = 7 ;
        else {
            vSync = 10 ;
            ret |= TIMING_CVT_WARN_ASPECT_RATIO ;
        }
    }

    /* Nominal H sync width (% of line period)  {DEFAULT = 8} */
    float NOMINAL_H_SYNC_WIDTH = 8.0f ;

    /* 4) These default values define analog scan system vertical blanking time limitations: */
    /* Minimum time of vertical sync+back porch interval (us) */
    float MIN_V_SYNC_BP_INTERVAL =  550.0f ;
    /* Minimum number of vertical back porch lines {DEFAULT = 6} */
    float MIN_V_BPORCH = 6 ;
    /* Minimum vertical porch (no of lines) {DEFAULT = 3} */
    float MIN_V_PORCH = gtf ? 1.0f : 3.0f ;
    /* 5) Definition of Horizontal blanking time limitation: */
    /* Generalized blanking limitation formula used of the form:
     * <H BLANKING TIME (%)> =C - ( M / Fh)
     * Where:
     * M (gradient) (%/kHz)  {DEFAULT = 600}
     * C (offset) (%)  {DEFAULT = 40}
     * K (blanking time scaling factor)  {DEFAULT = 128}
     * J (scaling factor weighting)  {DEFAULT = 20}
     */
    float GTF_M_VAR = 600 ;
    float GTF_C_VAR = 40 ;
    float GTF_K_VAR = 128 ;
    float GTF_J_VAR = 20 ;

    /* M' = K / 256 * M */
    float mPrime = GTF_K_VAR / 256 * GTF_M_VAR ;
    /* C' = ( ( C - J ) * K / 256 ) + J */
    float cPrime = ( ( GTF_C_VAR - GTF_J_VAR ) * GTF_K_VAR / 256 ) + GTF_J_VAR ;
    /* Fixed number of clocks for horizontal blanking  {DEFAULT = 160} */
    float K130 = 160 ;
    /* Fixed number of clocks for horizontal sync  {DEFAULT = 32} */
    float K131 = 32 ;
    /* Minimum vertical blanking interval time (us)  {DEFAULT = 460} */
    float K133 = 460 ;
    /* Fixed number of lines for vertical front porch  {DEFAULT = 3} */
    float RB_V_FPORCH = 3 ;

    /* Minimum number of vertical back porch lines  {DEFAULT = 6} */
    /* float RB_MIN_V_BPORCH = 6 ; */

    float CLOCK_STEP = 0.25f ;

    /* CONSTANTS */

    float cellGranRnd = floorf(CELL_GRAN) ;
    float marginPer = TOP_BOTTOM_MARGIN;
    float minVPorchRnd = floorf(MIN_V_PORCH) ;

    /* STANDARD CRT TIMING SCRATCH PAD: */
    float minVSyncBP = MIN_V_SYNC_BP_INTERVAL ;
    float hSyncPer = NOMINAL_H_SYNC_WIDTH ;

    /* REDUCED BLANKING SCRATCH PAD: */
    float rbMinVBlank = K133 ;
    float rbHSync = K131 ;
    float rbHBlank = K130 ;

    /* COMMON TIMING PARAMETERS: */
    /* 1 REQUIRED FIELD RATE */
    float vFieldRateRqd = INT_RQD ? IP_FREQ_RQD * 2.0f : IP_FREQ_RQD ;
    /* 2 HORIZONTAL PIXELS */
    float hPixelsRnd = froundf( gtf, ( H_PIXELS/cellGranRnd ) * cellGranRnd );
    /* 3 DETERMINE LEFT & RIGHT BORDERS */
    float leftMargin = MARGINS_RQD ? floorf( hPixelsRnd * marginPer / 100.0f / cellGranRnd ) * cellGranRnd : 0 ;
    float rightMargin = leftMargin ;
    /* 4 FIND TOTAL ACTIVE PIXELS */
    float totalActivePixels = hPixelsRnd + leftMargin + rightMargin ;
    /* 5 FIND NUMBER OF LINES PER FIELD */
    float vLinesRnd = INT_RQD ? froundf( gtf, V_LINES / 2.0f ) : froundf( gtf, V_LINES ) ;
    /* 6 FIND TOP & BOTTOM MARGINS */
    float topMargin = MARGINS_RQD ? froundf( gtf, marginPer/100.0f*vLinesRnd ) : 0 ;
    float botMargin = topMargin ;

    float interlace = INT_RQD ? 0.5f : 0 ;


    /* 8 ESTIMATE HORIZ. PERIOD (us): */
    float U23 = ( ( 1.0f / vFieldRateRqd ) - minVSyncBP / 1000000.0f )
                    / ( vLinesRnd + ( 2.0f * topMargin ) + minVPorchRnd + interlace ) * 1000000.0f ;

    /* 8.1 Reduced blanking */
    float Y23 = ( ( 1000000.0f / vFieldRateRqd ) - rbMinVBlank ) / ( vLinesRnd + topMargin + botMargin ) ;

    /* RESULTS Estimated Horizontal Frequency (kHz): */
    float hPeriodEst = RED_BLANK_RQD ? Y23 : U23 ;

    /* 9 FIND NUMBER OF LINES IN (SYNC + BACK PORCH): */
    /* Estimated V_SYNC_BP */
    float U26 = froundf( gtf,  minVSyncBP / hPeriodEst ) ;
    if ( !gtf ) U26 += 1.0f ;

    /* float U27 = MIN_VSYNC_BP/H_PERIOD_EST ; */

    /* 9.1 RB */
    float vbiLines = floorf( rbMinVBlank/hPeriodEst ) + 1.0f ;
    /* float Y27 = RB_MIN_V_BLANK/H_PERIOD_EST ; */

    float vSyncBP ;
    if (gtf)
        vSyncBP = rintf( MIN_V_SYNC_BP_INTERVAL / hPeriodEst ) ;
    else
        vSyncBP = U26 < ( vSync + MIN_V_BPORCH ) ? vSync + MIN_V_BPORCH : U26 ;

    /* RESULTS Ver Sync */
    float vSyncRnd = (int) vSync ;

    /* 10 FIND NUMBER OF LINES IN BACK PORCH (Lines): */
    /* float U31 = V_SYNC_BP-V_SYNC_RND ; */
    /* 10.1 RB */
    float rbMinVbi = RB_V_FPORCH + vSyncRnd + MIN_V_BPORCH ;
    float actVbiLines = vbiLines < rbMinVbi ? rbMinVbi : vbiLines ;

    /*11 FIND TOTAL NUMBER OF LINES IN VERTICAL FIELD: */
    float U34 = vLinesRnd + topMargin + botMargin + vSyncBP + interlace + minVPorchRnd ;
    /* 11.1 RB FIND TOTAL NUMBER OF LINES IN VERTICAL FIELD: */
    float Y34 = actVbiLines + vLinesRnd + topMargin + botMargin + interlace ;

    /* RESULTS */
    float totalVLines = RED_BLANK_RQD ? Y34 : U34 ;

    /* 12 FIND IDEAL BLANKING DUTY CYCLE FROM FORMULA (%): */
    float idealDutyCicle = cPrime - ( mPrime * hPeriodEst / 1000.0f ) ;

    float hPeriod ;
    float vFieldRateEst ;
    if (gtf) {
        vFieldRateEst = 1.0f / hPeriodEst / totalVLines *  1000000.0f ;
        hPeriod = hPeriodEst / ( vFieldRateRqd / vFieldRateEst ) ;
    } else
        hPeriod = idealDutyCicle ;

    /* 12.1 RB FIND TOTAL NUMBER OF PIXELS IN A LINE (Pixels): */
    float Y37 = rbHBlank + totalActivePixels ;

    /* 13 FIND BLANKING TIME TO NEAREST CHAR CELL (Pixels): */

    float vFieldRate ;
    if (gtf) {
        vFieldRate = rintf( totalActivePixels * idealDutyCicle / ( 100.0f - idealDutyCicle ) / ( 2.0f * cellGranRnd ) ) * ( 2.0f * cellGranRnd ) ;
    } else {
        vFieldRate = hPeriod < 20.0f ?
                floorf( totalActivePixels * 20.0f / ( 100.0f - 20.0f ) / ( 2.0f * cellGranRnd ) ) * (2.0f * cellGranRnd ) :
                floorf( totalActivePixels * idealDutyCicle / ( 100.0f - idealDutyCicle ) / ( 2.0f * cellGranRnd ) ) * ( 2.0f * cellGranRnd ) ;
    }

    /* RESULTS Horizontal Blanking (Pixels): */
    float hBlank = RED_BLANK_RQD ? rbHBlank : vFieldRate ;

    /* 14 FIND TOTAL NUMBER OF PIXELS IN A LINE (Pixels): */
    float vFrameRate = totalActivePixels + hBlank ;

    float totalPixels  = RED_BLANK_RQD ? Y37 : vFrameRate ;

    /* 15 FIND PIXEL CLOCK FREQUENCY (MHz): */
    float pixelFreq ;
    if (gtf)
        pixelFreq = totalPixels / hPeriodEst ;
    else
        pixelFreq = CLOCK_STEP * floorf( ( totalPixels / hPeriodEst ) / CLOCK_STEP ) ;

    /* float U47 = TOTAL_PIXELS/H_PERIOD_EST ; */

    /* 13 RB FIND PIXEL CLOCK FREQUENCY (MHz): Y40*/
    float Y40 = CLOCK_STEP * floorf( ( vFieldRateRqd * totalVLines * totalPixels / 1000000.0f ) / CLOCK_STEP ) ;
    /* float Y41 = V_FIELD_RATE_RQD*TOTAL_V_LINES*TOTAL_PIXELS/1000000.0f ; */

    /* RESULTS Actual Pixel Clock (MHz): */
    float actPixelFreq = RED_BLANK_RQD ? Y40 : pixelFreq ;

    /* 16 FIND ACTUAL HORIZONTAL FREQUENCY (kHz): */
    //float U50 = 1000.0f*ACT_PIXEL_FREQ/TOTAL_PIXELS ;

    /* 14 RB FIND ACTUAL HORIZONTAL FREQUENCY (kHz): */
    //float Y44 = 1000.0f*ACT_PIXEL_FREQ/TOTAL_PIXELS ;

    /* RESULTS Actual Horizontal Frequency (kHz): */
    //float ACT_H_FREQ = RED_BLANK_RQD ? Y44 : U50 ;

    float actHFreq ;
    if (gtf)
        actHFreq = 1000.0f / hPeriod ;
    else
        actHFreq = 1000.0f * actPixelFreq / totalPixels ;

    /* 17 FIND ACTUAL FIELD RATE (Hz): */
    // float U53 = 1000.0f*ACT_H_FREQ/TOTAL_V_LINES ;

    /* 15 RB FIND ACTUAL FIELD RATE (Hz): */
    // float Y47 = 1000.0f*ACT_H_FREQ/TOTAL_V_LINES ;

    /* RESULTS Actual Vertical Frequency (Hz): */
    // float ACT_FIELD_RATE = RED_BLANK_RQD ? Y47 : U53 ;
    float actFieldRate = 1000.0f * actHFreq / totalVLines ;

    /* 16 RB FIND ACTUAL VERTICAL  FRAME FREQUENCY (Hz): */
    //float Y50 = INT_RQD ? ACT_FIELD_RATE/2.0f : ACT_FIELD_RATE ;

    /* 18 FIND ACTUAL VERTICAL  FRAME FREQUENCY (Hz): */
    //float U56 = INT_RQD ? ACT_FIELD_RATE/2.0f : ACT_FIELD_RATE ;

    /* RESULTS Actual Vertical Frequency (Hz): */
    //float ACT_FRAME_RATE = RED_BLANK_RQD ? Y50 : U56 ;
    float actFrameRat = INT_RQD ? actFieldRate / 2.0f : actFieldRate ;

    /* RESULTS Hor Back porch*/
    float hBackPorch = hBlank / 2 ;

    /* RESULTS Ver Blank */
//    float vBlank = RED_BLANK_RQD ? actVbiLines : vSyncBP + minVPorchRnd ;

    /* RESULTS Ver Front Porch*/
    float vFrontPorch = RED_BLANK_RQD ? RB_V_FPORCH : minVPorchRnd ;

    /* RESULTS Ver back porch */
//    float vBackPorch = vBlank - vFrontPorch - vSyncRnd ;

    /* RESULTS Hor Sync */
    float hSyncRnd = RED_BLANK_RQD ? rbHSync : froundf( gtf, ( hSyncPer / 100.0f * totalPixels / cellGranRnd ) ) * cellGranRnd ;

    /* RESULTS Hor Front Porch: */
    float hFrontPorch ;
    if (gtf)
        hFrontPorch = ( hBlank / 2.0f ) - hSyncRnd ;
    else
        hFrontPorch = hBlank - hBackPorch - hSyncRnd ;

#if DEBUG
    fprintf( stderr, "hFrontPorch:\t\t%f\n", hFrontPorch ) ;
    fprintf( stderr, "totalActivePixels:\t\t%f\n", totalActivePixels ) ;
    fprintf( stderr, "vFieldRateRqd:\t\t\t%f\n", vFieldRateRqd ) ;
    fprintf( stderr, "minVSyncBP:\t\t\t%f\n", minVSyncBP ) ;
    fprintf( stderr, "vLinesRnd:\t\t\t%f\n", vLinesRnd ) ;
    fprintf( stderr, "minVPorchRnd:\t\t\t%f\n", minVPorchRnd ) ;
    fprintf( stderr, "interlace:\t\t\t%f\n", interlace ) ;
    fprintf( stderr, "vSyncBP:\t\t\t%f\n", vSyncBP ) ;
    fprintf( stderr, "hSyncPer:\t\t\t%f\n", hSyncPer ) ;
    fprintf( stderr, "totalPixels:\t\t\t%f\n", totalPixels ) ;
    fprintf( stderr, "cellGranRnd:\t\t\t%f\n", cellGranRnd ) ;
    fprintf( stderr, "hPeriod:\t\t\t%f\n", hPeriod ) ;
    fprintf( stderr, "vFieldRate:\t\t\t%f\n", vFieldRate ) ;
    fprintf( stderr, "hPeriodEst:\t\t\t%f\n", hPeriodEst ) ;
    fprintf( stderr, "totalVLines:\t\t\t%f\n", totalVLines ) ;
    fprintf( stderr, "vFieldRateEst:\t\t\t%f\n", vFieldRateEst ) ;
    fprintf( stderr, "vFieldRateRqd:\t\t\t%f\n", vFieldRateRqd ) ;
    fprintf( stderr, "idealDutyCicle:\t\t\t%f\n", idealDutyCicle ) ;
    fprintf( stderr, "actHFreq:\t\t\t%f\n", actHFreq ) ;
    fprintf( stderr, "hblank:\t\t\t\t%f\n", hBlank ) ;
    fprintf( stderr, "actPixelFreq:\t\t\t%f\n", actPixelFreq ) ;
#endif

    if (mode) {
        if (mode->name == NULL) {
            mode->name = malloc(MODE_NAME_LENGTH);
            if (mode->name) {
                memset(mode->name, 0, MODE_NAME_LENGTH) ;
                char c = 0 ;
                if (RED_BLANK_RQD) c = 'r' ;
                if (INT_RQD) c = 'i' ;
                sprintf(mode->name, "%dx%d@%d%c", width, height, (int)refresh, c ) ;
            }
        }

        mode->Clock = actPixelFreq * 1000.0f ;
        mode->VRefresh = actFrameRat ;

        mode->HDisplay = width ;
        mode->HSyncStart = width + hFrontPorch ;
        mode->HSyncEnd = width + ( hFrontPorch + hSyncRnd ) ;
        mode->HTotal = totalPixels ;

        mode->VDisplay = height ;
        mode->VSyncStart = height + vFrontPorch ;
        mode->VSyncEnd = height + (vFrontPorch + vSyncRnd) ;
        mode->VTotal = INT_RQD ? totalVLines * 2 : totalVLines ;
        mode->Flags = 0 ;

        if (RED_BLANK_RQD) {
            mode->Flags |= V_PHSYNC ;
            mode->Flags |= V_NVSYNC ;
        } else {
            mode->Flags |= V_NHSYNC ;
            mode->Flags |= V_PVSYNC ;
        }

        if (!(mode->Flags & V_PHSYNC)) mode->Flags |= V_NHSYNC ;
        if (!(mode->Flags & V_NHSYNC)) mode->Flags |= V_PHSYNC ;
        if (!(mode->Flags & V_PVSYNC)) mode->Flags |= V_NVSYNC ;
        if (!(mode->Flags & V_NVSYNC)) mode->Flags |= V_PVSYNC ;

        if (INT_RQD) mode->Flags |= V_INTERLACE ;
    } else {
        ret |= TIMING_ERR_BAD_ALLOC ;
    }

    return ret ;
}

int viaTimingCvt(DisplayModePtr mode, int width, int height, float refresh, Bool interlaced, Bool reducedBlanking) {
    return timingGenerateMode( mode, FALSE, width, height, refresh, interlaced, reducedBlanking ) ;
}

int viaTimingGtf(DisplayModePtr mode, int width, int height, float refresh, Bool interlaced) {
    return timingGenerateMode( mode, TRUE, width, height, refresh, interlaced, FALSE ) ;
}
