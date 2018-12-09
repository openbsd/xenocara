/*
 * Copyright 2017-2018 Kevin Brace. All Rights Reserved.
 * Copyright 2007-2015 OpenChrome Project
 *                     [https://www.freedesktop.org/wiki/Openchrome]
 * Copyright 1998-2008 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2008 S3 Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * via_fp.h
 *
 * Header file for via_fp.c.
 */

#ifndef _VIA_FP_H_
#define _VIA_FP_H_ 1


/*
 * These FP DPA parameters were copied from VIA Technologies
 * xf86-video-via v83-44398 source code.
 */


/*
 * P4M890 chipset FP DPA parameters default setting.
 */
static VIADPARec viaDPAP4M890ClockDefault[] = {
    /*      DVP0 Adjustment, DVP0 Clock Drive, DVP0 Data Drive,
     *      DVP1 Adjustment, DVP1 Clock Drive, DVP1 Data Drive,
     *  FPDP Low Adjustment,              FPDP High Adjustment */
    {                  0x07,             0x00,            0x00,
                       0x03,             0x00,            0x00,
                       0x04,                              0x08}
};

/*
 * P4M890 chipset FP DPA parameters for dot clock at or above 50 MHz
 * but below 70 MHz.
 */
static VIADPARec viaDPAP4M890Clock50M70M[] = {
    /*      DVP0 Adjustment, DVP0 Clock Drive, DVP0 Data Drive,
     *      DVP1 Adjustment, DVP1 Clock Drive, DVP1 Data Drive,
     *  FPDP Low Adjustment,              FPDP High Adjustment */
    {                  0x06,             0x00,            0x00,
                       0x03,             0x00,            0x00,
                       0x04,                              0x08}
};

/*
 * P4M890 chipset FP DPA parameters for dot clock at or above 70 MHz
 * but below 100 MHz.
 */
static VIADPARec viaDPAP4M890Clock70M100M[] = {
    /*      DVP0 Adjustment, DVP0 Clock Drive, DVP0 Data Drive,
     *      DVP1 Adjustment, DVP1 Clock Drive, DVP1 Data Drive,
     *  FPDP Low Adjustment,              FPDP High Adjustment */
    {                  0x03,             0x00,            0x00,
                       0x03,             0x00,            0x00,
                       0x04,                              0x08}
};

/*
 * P4M890 chipset FP DPA parameters for dot clock at or above 100 MHz
 * but below 150 MHz.
 */
static VIADPARec viaDPAP4M890Clock100M150M[] = {
    /*      DVP0 Adjustment, DVP0 Clock Drive, DVP0 Data Drive,
     *      DVP1 Adjustment, DVP1 Clock Drive, DVP1 Data Drive,
     *  FPDP Low Adjustment,              FPDP High Adjustment */
    {                  0x03,             0x00,            0x00,
                       0x03,             0x00,            0x00,
                       0x04,                              0x01}
};

/*
 * P4M890 chipset FP DPA parameters for dot clock at or above 150 MHz.
 */
static VIADPARec viaDPAP4M890Clock150M[] = {
    /*      DVP0 Adjustment, DVP0 Clock Drive, DVP0 Data Drive,
     *      DVP1 Adjustment, DVP1 Clock Drive, DVP1 Data Drive,
     *  FPDP Low Adjustment,              FPDP High Adjustment */
    {                  0x01,             0x02,            0x02,
                       0x03,             0x00,            0x00,
                       0x04,                              0x0D}
};

/*
 * P4M890 Chipset FP DPA (Digital Panel Adjustment?) Table
 */
static VIADPAInfoTableRec viaDPAFPP4M890[] = {
    {         VIA_DPA_CLK_RANGE_30M,      viaDPAP4M890ClockDefault},
    {     VIA_DPA_CLK_RANGE_30M_50M,      viaDPAP4M890ClockDefault},
    {     VIA_DPA_CLK_RANGE_50M_70M,       viaDPAP4M890Clock50M70M},
    {    VIA_DPA_CLK_RANGE_70M_100M,      viaDPAP4M890Clock70M100M},
    {   VIA_DPA_CLK_RANGE_100M_150M,     viaDPAP4M890Clock100M150M},
    {        VIA_DPA_CLK_RANGE_150M,         viaDPAP4M890Clock150M}
};


/*
 * K8M890 chipset FP DPA parameters default setting.
 */
static VIADPARec viaDPAK8M890ClockDefault[] = {
    /*      DVP0 Adjustment, DVP0 Clock Drive, DVP0 Data Drive,
     *      DVP1 Adjustment, DVP1 Clock Drive, DVP1 Data Drive,
     *  FPDP Low Adjustment,              FPDP High Adjustment */
    {                  0x04,             0x00,            0x00,
                       0x03,             0x00,            0x00,
                       0x04,                              0x04}
};

/*
 * K8M890 chipset FP DPA parameters for dot clock at or above 50 MHz
 * but below 70 MHz.
 */
static VIADPARec viaDPAK8M890Clock50M70M[] = {
    /*      DVP0 Adjustment, DVP0 Clock Drive, DVP0 Data Drive,
     *      DVP1 Adjustment, DVP1 Clock Drive, DVP1 Data Drive,
     *  FPDP Low Adjustment,              FPDP High Adjustment */
    {                  0x06,             0x00,            0x00,
                       0x03,             0x00,            0x00,
                       0x04,                              0x02}
};

/*
 * K8M890 chipset FP DPA parameters for dot clock at or above 70 MHz
 * but below 100 MHz.
 */
static VIADPARec viaDPAK8M890Clock70M100M[] = {
    /*      DVP0 Adjustment, DVP0 Clock Drive, DVP0 Data Drive,
     *      DVP1 Adjustment, DVP1 Clock Drive, DVP1 Data Drive,
     *  FPDP Low Adjustment,              FPDP High Adjustment */
    {                  0x02,             0x00,            0x00,
                       0x03,             0x00,            0x00,
                       0x02,                              0x02}
};

/*
 * K8M890 chipset FP DPA parameters for dot clock at or above 100 MHz
 * but below 150 MHz.
 */
static VIADPARec viaDPAK8M890Clock100M150M[] = {
    /*      DVP0 Adjustment, DVP0 Clock Drive, DVP0 Data Drive,
     *      DVP1 Adjustment, DVP1 Clock Drive, DVP1 Data Drive,
     *  FPDP Low Adjustment,              FPDP High Adjustment */
    {                  0x02,             0x00,            0x00,
                       0x03,             0x00,            0x00,
                       0x02,                              0x02}
};

/*
 * K8M890 chipset FP DPA parameters for dot clock at or above 150 MHz.
 */
static VIADPARec viaDPAK8M890Clock150M[] = {
    /*      DVP0 Adjustment, DVP0 Clock Drive, DVP0 Data Drive,
     *      DVP1 Adjustment, DVP1 Clock Drive, DVP1 Data Drive,
     *  FPDP Low Adjustment,              FPDP High Adjustment */
    {                  0x03,             0x00,            0x00,
                       0x03,             0x00,            0x00,
                       0x02,                              0x02}
};

/*
 * K8M890 Chipset FP DPA (Digital Panel Adjustment?) Table
 */
static VIADPAInfoTableRec viaDPAFPK8M890[] = {
    {         VIA_DPA_CLK_RANGE_30M,      viaDPAK8M890ClockDefault},
    {     VIA_DPA_CLK_RANGE_30M_50M,      viaDPAK8M890ClockDefault},
    {     VIA_DPA_CLK_RANGE_50M_70M,       viaDPAK8M890Clock50M70M},
    {    VIA_DPA_CLK_RANGE_70M_100M,      viaDPAK8M890Clock70M100M},
    {   VIA_DPA_CLK_RANGE_100M_150M,     viaDPAK8M890Clock100M150M},
    {        VIA_DPA_CLK_RANGE_150M,         viaDPAK8M890Clock150M}
};


/*
 * P4M900 chipset FP DPA parameters default setting.
 */
static VIADPARec viaDPAP4M900ClockDefault[] = {
    /*      DVP0 Adjustment, DVP0 Clock Drive, DVP0 Data Drive,
     *      DVP1 Adjustment, DVP1 Clock Drive, DVP1 Data Drive,
     *  FPDP Low Adjustment,              FPDP High Adjustment */
    {                  0x07,             0x00,            0x00,
                       0x03,             0x00,            0x00,
                       0x08,                              0x00}
};

/*
 * P4M900 chipset FP DPA parameters for dot clock at or above 100 MHz
 * but below 150 MHz.
 */
static VIADPARec viaDPAP4M900Clock100M150M[] = {
    /*      DVP0 Adjustment, DVP0 Clock Drive, DVP0 Data Drive,
     *      DVP1 Adjustment, DVP1 Clock Drive, DVP1 Data Drive,
     *  FPDP Low Adjustment,              FPDP High Adjustment */
    {                  0x03,             0x00,            0x01,
                       0x03,             0x00,            0x00,
                       0x08,                              0x00}
};

/*
 * P4M900 chipset FP DPA parameters for dot clock at or above 150 MHz.
 */
static VIADPARec viaDPAP4M900Clock150M[] = {
    /*      DVP0 Adjustment, DVP0 Clock Drive, DVP0 Data Drive,
     *      DVP1 Adjustment, DVP1 Clock Drive, DVP1 Data Drive,
     *  FPDP Low Adjustment,              FPDP High Adjustment */
    {                  0x01,             0x02,            0x01,
                       0x03,             0x00,            0x00,
                       0x08,                              0x00}
};

/*
 * P4M900 Chipset FP DPA (Digital Panel Adjustment?) Table
 */
static VIADPAInfoTableRec viaDPAFPP4M900[] = {
    {         VIA_DPA_CLK_RANGE_30M,      viaDPAP4M900ClockDefault},
    {     VIA_DPA_CLK_RANGE_30M_50M,      viaDPAP4M900ClockDefault},
    {     VIA_DPA_CLK_RANGE_50M_70M,      viaDPAP4M900ClockDefault},
    {    VIA_DPA_CLK_RANGE_70M_100M,      viaDPAP4M900ClockDefault},
    {   VIA_DPA_CLK_RANGE_100M_150M,     viaDPAP4M900Clock100M150M},
    {        VIA_DPA_CLK_RANGE_150M,         viaDPAP4M900Clock150M}
};


static VIA_DPA_INDEX_TABLE viaDPAIndexTable[] = {
//  {VIA_CX700,     NULL,   NULL},
    {VIA_P4M890,    NULL,   viaDPAFPP4M890},
    {VIA_K8M890,    NULL,   viaDPAFPK8M890},
    {VIA_P4M900,    NULL,   viaDPAFPP4M900},
//  {VIA_VX800,     NULL,   NULL}
};

#endif /* _VIA_FP_H_ */
