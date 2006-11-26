/* $XFree86$ */
/* $XdotOrg$ */
/*
 * X.org/XFree86 specific supplements to init.c/init301.c
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria
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
 * Author: 	Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifndef _INITEXTX_H_
#define _INITEXTX_H_

#include "osdef.h"
#include "initdef.h"

#include "sis.h"
#include "sis_regs.h"

#ifdef SIS315H
static const struct {
	unsigned char  Ext_ModeID;     /* ModeID in new ROM */
	unsigned char  Ext_MyModeID;   /* corresponding ModeID in my tables (0 = identical) */
	unsigned short Ext_VESAID;     /* corresponding VESA ID in new ROM */
} SiS_EModeIDTable661[] = {
	{ 0x6a, 0x00, 0x0102 },
	{ 0x1d, 0x20, 0x0000 },
	{ 0x1e, 0x21, 0x0000 },
	{ 0x1f, 0x22, 0x0000 },
	{ 0x20, 0x29, 0x0000 },
	{ 0x21, 0x2a, 0x0000 },
	{ 0x22, 0x2b, 0x0000 },
	{ 0x23, 0x00, 0x011c },
	{ 0x24, 0x00, 0x011d },
	{ 0x25, 0x00, 0x011e },
	{ 0x26, 0x00, 0x011f },
	{ 0x27, 0x00, 0x0120 },
	{ 0x28, 0x00, 0x0121 },
	{ 0x2a, 0x14, 0x013d },
	{ 0x2b, 0x15, 0x013e },
	{ 0x2c, 0x16, 0x013f },
	{ 0x2e, 0x00, 0x0101 },
	{ 0x2f, 0x00, 0x0100 },
	{ 0x30, 0x00, 0x0103 },
	{ 0x37, 0x00, 0x0104 },
	{ 0x38, 0x00, 0x0105 },
	{ 0x3a, 0x00, 0x0107 },
	{ 0x3c, 0x00, 0x0125 },
	{ 0x3d, 0x00, 0x0126 },
	{ 0x40, 0x00, 0x010d },
	{ 0x41, 0x00, 0x010e },
	{ 0x43, 0x00, 0x0110 },
	{ 0x44, 0x00, 0x0111 },
	{ 0x46, 0x00, 0x0113 },
	{ 0x47, 0x00, 0x0114 },
	{ 0x49, 0x00, 0x0116 },
	{ 0x4a, 0x00, 0x0117 },
	{ 0x4c, 0x00, 0x0119 },
	{ 0x4d, 0x00, 0x011a },
	{ 0x50, 0x00, 0x0127 },
	{ 0x51, 0x00, 0x0128 },
	{ 0x52, 0x00, 0x0129 },
	{ 0x53, 0x1a, 0x0141 },
	{ 0x54, 0x1b, 0x0142 },
	{ 0x55, 0x1c, 0x0143 },
	{ 0x56, 0x00, 0x012a },
	{ 0x57, 0x00, 0x012b },
	{ 0x58, 0x00, 0x012c },
	{ 0x59, 0x00, 0x012d },
	{ 0x5a, 0x17, 0x012e },
	{ 0x5b, 0x18, 0x012f },
	{ 0x5c, 0x19, 0x0130 },
	{ 0x5d, 0x00, 0x0131 },
	{ 0x62, 0x00, 0x0112 },
	{ 0x63, 0x00, 0x0115 },
	{ 0x64, 0x00, 0x0118 },
	{ 0x65, 0x00, 0x011b },
	{ 0x66, 0x00, 0x0132 },
	{ 0x75, 0x00, 0x013a },
	{ 0x78, 0x00, 0x013b },
	{ 0x79, 0x00, 0x013c },
	{ 0x7b, 0x7c, 0x0136 },
	{ 0x7c, 0x7d, 0x0137 },
	{ 0x7d, 0x7e, 0x0138 },
	{ 0xff, 0xff, 0xffff }
};
#endif

#define SIS_PL_HSYNCP 0x01
#define SIS_PL_HSYNCN 0x02
#define SIS_PL_VSYNCP 0x04
#define SIS_PL_VSYNCN 0x08
#define SIS_PL_DVI    0x80

struct SiS_PlasmaModes
{
   const char     *name;
   unsigned int   clock;
   unsigned short HDisplay, HTotal, HFrontPorch, HSyncWidth;
   unsigned short VDisplay, VTotal, VFrontPorch, VSyncWidth;
   unsigned char  SyncFlags;
};

struct SiS_PlasmaTables
{
   unsigned short vendor;
   unsigned char  productnum;
   unsigned short product[5];
   const char     *DDCnames[5];
   const char     *plasmaname;
   unsigned short maxx,maxy;
   unsigned short prefx, prefy;
   unsigned char  modenum;
   unsigned char  plasmamodes[20];  /* | 0x80 = DVI-capable, | 0x40 = analog */
};

static const struct SiS_PlasmaModes SiS_PlasmaMode[] = {
   {  "640x400",		/* 00: IBM 400@70 */
      25175,
       640,  800, 17,  64,
       400,  449, 13,   2,
      SIS_PL_HSYNCN | SIS_PL_VSYNCN },
   {  "640x480",		/* 01: VESA 480@72 */
      31500,
       640,  832, 24,  40,
       480,  520,  9,   3,
      SIS_PL_HSYNCN | SIS_PL_VSYNCN },
   {  "800x600",		/* 02: VESA 600@72 */
      50000,
       800, 1040, 56, 120,
       600,  666, 37,   6,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "864x480",		/* 03: Cereb wide 1 */
      42526,
       864, 1134, 22,  86,
       480,  500,  1,   3,
      SIS_PL_HSYNCP | SIS_PL_VSYNCN },
   {  "848x480",		/* 04: VESA wide (NEC1) */
      33750,
       848, 1088, 16, 112,
       480,  517,  6,   8,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "1024x576",		/* 05: VESA wide (NEC2) */
      47250,
      1024, 1320, 16, 144,
       576,  596,  2,   4,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "1280x720",		/* 06: VESA wide (NEC3) */
      76500,
      1280, 1696, 48, 176,
       720,  750,  4,   8,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "1360x765",		/* 07: VESA wide (NEC4) */
      85500,
      1360, 1792, 64, 176,
       765,  795,  4,   8,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "1024x600",		/* 08: CEREB wide 2 */
      51200,
      1024, 1352, 51, 164,
       600,  628,  1,   4,
      SIS_PL_HSYNCN | SIS_PL_VSYNCP },
   {  "1024x768",		/* 09: VESA 768@75 */
      78750,
      1024, 1312,  16, 96,
       768,  800,   1,  3,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "1152x864",		/* 10: VESA 1152x864@75 */
      108000,
      1152, 1600, 64, 128,
       864,  900,  1,   3,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "1280x1024",		/* 11: VESA 1024@60 */
      108000,
      1280, 1688, 48, 112,
      1024, 1066,  1,   3,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "1280x768",		/* 12: W_XGA */
      81000,
      1280, 1688, 48, 112,
       768,  802,  3,   6,
      SIS_PL_HSYNCP | SIS_PL_VSYNCN },
   {  "1280x768",		/* 13: I/O Data W_XGA@56Hz */
      76064,
      1280, 1688, 48, 112,
       768,  802,  2,   3,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "1376x768",		/* 14: I/O Wide XGA */
      87340,
      1376, 1808, 32, 128,
       768,  806,  3,   6,
      SIS_PL_HSYNCN | SIS_PL_VSYNCP },
   {  "1280x960",		/* 15: VESA 960@60 */
      108000,
      1280, 1800, 96, 112,
       960, 1000,  1,   3,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "1400x1050",		/* 16: VESA 1050@60Hz */
      108000,
      1400, 1688, 48, 112,
      1050, 1066,  1,   3,
      SIS_PL_HSYNCN | SIS_PL_VSYNCN },
   {  "1360x768",		/* 17: VESA wide (NEC4/2) */
      85500,
      1360, 1792, 64, 112,
       765,  795,  3,   6,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "800x600",		/* 18: VESA 600@56 */
      36000,
       800, 1024, 24,   2,
       600,  625,  1,   2,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "1072x600",		/* 19: Panasonic 1072x600 (sync?) */
      54100,
       1072, 1424, 48, 176,
        600,  628, 16,   1,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "848x480",		/* 20: Panasonic 848x480 (sync?) */
      33070,			/* is 852x480, but we can't use 852 */
        848, 1068, 20,  40,	/* differs from DDC data, better centered */
        480,  516,  3,   5,	/* won't work assumingly, because data is % 8 */
      SIS_PL_HSYNCN | SIS_PL_VSYNCN },
   {  "1280x720",		/* 21: WIDE720(60) (aka "750p") (Panasonic) */
      74300,
      1280, 1650,110,  40,
       720,  750,  5,   5,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "1280x768",		/* 22: 1280x768@56.5 (Panasonic) */
      76200,			/* (According to manual not supported for HDMI; but works) */
      1280, 1680, 16,  24,
       768,  802,  2,   5,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "1280x720@50",		/* 23: WIDE720(50) (aka "750p") (Panasonic) */
      74300,			/* Panasonic states 45.0kHz. Not possible. This one works (with some overscan) */
      1280, 1980,400,  80,
       720,  750,  1,   2,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "720x480",		/* 24: 720x480 (aka "525p" and "480p") (Panasonic) */
      27000,
       720,  856, 40,  32,
       480,  525,  1,   3,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "720x576",		/* 25: 720x576 (aka "625p"and "576p") (Panasonic) */
      27500,
       720,  864, 16,  64,
       576,  625,  5,   6,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
   {  "1280x720@50",		/* 26: WIDE720(50) (aka "750p") (Generic) */
      74300,
      1280, 1980,400,  80,
       720,  750,  5,   5,
      SIS_PL_HSYNCP | SIS_PL_VSYNCP },
};

/*
27.00  720 755 791 858  480 480 484 525
27.50  720 732 795 864  576 581 587 625
*/

static const struct SiS_PlasmaTables SiS_PlasmaTable[] = {
#if 0  /* Product IDs missing */
   { 0x38a3, 4,
     { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "", "", "", "", "" },
     "NEC PlasmaSync 42VP4/42VP4D/42VP4G/42VP4DG",
     0, 0,
     0, 0,
     11,   /* All DVI, except 0, 7, 13 */
     { 0|0x40, 1|0xc0, 2|0xc0, 4|0xc0, 7|0x40, 9|0xc0,10|0xc0,11|0xc0,13|0x40,14|0xc0,
      17|0xc0, 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
#endif
#if 0  /* Product IDs missing */
   { 0x38a3, 3,
     { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "", "", "", "", "" },
     "NEC PlasmaSync 42PD1/50PD1/50PD2",
     0, 0,
     0, 0,
     5,   /* DVI entirely unknown */
     { 0|0x40, 1|0xc0, 2|0xc0, 4|0xc0, 9|0xc0, 0     , 0     , 0     , 0     , 0     ,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
   { 0x38a3, 1,
     { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "", "", "", "", "" },
     "NEC PlasmaSync 42PD3",
     0, 0,
     0, 0,
     10,   /* DVI entirely unknown */
     { 0|0x40, 1|0xc0, 2|0xc0, 3|0xc0, 4|0xc0, 5|0xc0, 6|0xc0, 7|0x40, 8|0xc0, 9|0xc0,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
   { 0x38a3, 2,
     { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "", "", "", "", "" },
     "NEC PlasmaSync 42VM3/61XM1",
     0, 0,
     0, 0,
     11,  /* DVI entirely unknown */
     { 0|0x40, 1|0xc0, 2|0xc0, 3|0xc0, 4|0xc0, 5|0xc0, 6|0xc0, 8|0xc0, 9|0xc0,11|0xc0,
      17|0xc0, 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
   { 0x38a3, 2,
     { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "", "", "", "", "" },
     "NEC PlasmaSync 42MP1/42MP2",
     0, 0,
     0, 0,
     6,   /* DVI entirely unknown */
     { 0|0x40, 1|0xc0, 2|0xc0, 4|0xc0, 9|0xc0,11|0xc0, 0     , 0     , 0     , 0     ,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
   { 0x38a3, 1,
     { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "", "", "", "", "" },
     "NEC PlasmaSync 50MP1",
     0, 0,
     0, 0,
     10,   /* DVI entirely unknown */
     { 0|0x40, 1|0xc0, 2|0xc0, 4|0xc0, 7|0x40, 9|0xc0,10|0xc0,11|0xc0,13|0x40,14|0xc0,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
#endif
   { 0x38a3, 4,
     { 0xa482, 0xa483, 0x0000, 0x0000, 0x0000 },
     { "PX-42VM", "", "", "", "" },
     "NEC PlasmaSync 42MP3/42MP4/50MP2/61MP1",
     0, 0,
     0, 0,
     11,   /* All DVI except 0, 7, 13, 17 */
     { 0|0x40, 1|0xc0, 2|0xc0, 4|0xc0, 7|0x40, 9|0xc0,10|0xc0,11|0xc0,13|0x40,14|0xc0,
      17|0x40, 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
#if 0  /* Product IDs missing */
   { 0x38a3, 1,
     { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "", "", "", "", "" },
     "NEC PlasmaSync 3300W",
     0, 0,
     0, 0,
     3,
     { 0|0x40, 1|0xc0,18|0xc0, 0     , 0     , 0     , 0     , 0     , 0     , 0     ,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
   { 0x38a3, 1,
     { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "", "", "", "", "" },
     "NEC PlasmaSync 4200W",
     4,   /* DVI entirely unknown */
     { 0|0x40, 1|0xc0, 2|0xc0, 4|0xc0, 0     , 0     , 0     , 0     , 0     , 0     ,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
   { 0x38a3, 1,
     { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "", "", "", "", "" },
     "NEC PlasmaSync 4210W",
     0, 0,
     0, 0,
     6,   /* DVI entirely unknown */
     { 0|0x40, 1|0xc0, 2|0xc0, 4|0xc0, 9|0xc0,11|0xc0, 0     , 0     , 0     , 0     ,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
   { 0x38a3, 1,
     { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "", "", "", "", "" },
     "NEC PlasmaSync 5000W",
     0, 0,
     0, 0,
     7,   /* DVI entirely unknown */
     { 0|0x40, 1|0xc0, 2|0xc0, 4|0xc0, 7|0x40, 9|0xc0,11|0xc0, 0     , 0     , 0     ,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
#endif
   { 0x412f, 2,
     { 0x000c, 0x000b, 0x0000, 0x0000, 0x0000 },
     { "", "", "", "", "" },
     "Pioneer 503CMX/PDA-5002",
     0, 0,
     0, 0,
     6,   /* DVI unknown */
     { 1|0xc0, 2|0xc0, 9|0xc0,11|0xc0,12|0xc0,15|0xc0, 0     , 0     , 0     , 0     ,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
   { 0x34a9, 1,
     { 0xa00e, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "", "", "", "", "" },
     "Panasonic TH-42",
     0, 0,
     0, 0,
     5,   /* No DVI output */
     { 1|0x40, 2|0x40, 4|0x40, 9|0x40,15|0x40, 0     , 0     , 0     , 0     , 0     ,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
   { 0x34a9, 1,
     { 0xa005, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "TH-42PW*4", "", "", "", "" },
     "Panasonic TH-42PW5",
     0, 0,
     0, 0,
     1,   /* No special modes otherwise; no DVI. */
     {20|0x40,19|0x40, 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     ,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
   { 0x4c2e, 1,
     { 0x9b05, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "PLV-Z2", "", "", "", "" },
     "Sanyo PLV-Z2 (non HDCP-mode)", 	/* HDCP mode would be id 9b06, but not needed */
     1280, 768,				/* as it then advertises correct size */
     1280, 720,
     1,   /* 1280x720, no special modes otherwise */
     {21|0xc0, 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     ,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
   { 0x34a9, 1,
     { 0xd034, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "AE500U (DVI-D)", "", "", "", "" },
     "Panasonic AE500U",
     1280, 768,
     1280, 720,
     1,   /* 1280x720, no special modes otherwise */
     {21|0xc0, 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     ,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
   { 0x34a9, 1,
     { 0xd043, 0x0000, 0x0000, 0x0000, 0x0000 },
     { "AE700U (HDMI)", "", "", "", "" },
     "Panasonic AE700U",
     1360, 768,
     1280, 720,
     6,   /* 1280x720/60, 1280x720/50, 1280x768@56(digital/analog), 720x480, 720x576 */
     {21|0xc0,23|0xc0,22|0x80,13|0x40,24|0x80,25|0x80, 0     , 0     , 0     , 0     ,
       0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0     , 0       }
   },
   { 0x0000 }
};

unsigned short	SiS_CheckBuildCustomMode(ScrnInfoPtr pScrn, DisplayModePtr mode, unsigned int VBFlags);
DisplayModePtr	SiSBuildBuiltInModeList(ScrnInfoPtr pScrn, BOOLEAN, BOOLEAN, BOOLEAN, BOOLEAN);
int		SiSTranslateToVESA(ScrnInfoPtr pScrn, int modenumber);
int		SiSTranslateToOldMode(int modenumber);
BOOLEAN		SiS_GetPanelID(struct SiS_Private *SiS_Pr);

unsigned short	SiS_SenseLCDDDC(struct SiS_Private *SiS_Pr, SISPtr pSiS);
unsigned short	SiS_SenseVGA2DDC(struct SiS_Private *SiS_Pr, SISPtr pSiS);

void		SiS_CalcXTapScaler(struct SiS_Private *SiS_Pr, int srcsize, int destsize, int taps, Bool ishoriz);
void		SiS_SetGroup2_C_ELV(struct SiS_Private *SiS_Pr, unsigned short ModeNo,
			unsigned short ModeIdIndex, unsigned short RefreshRateTableIndex);

extern unsigned char	SiS_GetReg(SISIOADDRESS port, unsigned short index);
extern void		SiS_SetReg(SISIOADDRESS port, unsigned short index, unsigned short data);
extern void		SiS_SetRegOR(SISIOADDRESS, unsigned short, unsigned short);
extern void		SiS_SetRegAND(SISIOADDRESS, unsigned short, unsigned short);
extern void		SiS_SetRegANDOR(SISIOADDRESS Port, unsigned short Index, unsigned short DataAND,
				unsigned short DataOR);
extern int		SiS_compute_vclk(int Clock, int *out_n, int *out_dn, int *out_div,
				int *out_sbit, int *out_scale);
extern void		SiSCalcClock(ScrnInfoPtr pScrn, int clock, int max_VLD, unsigned int *vclk);
extern void		SiS_CalcCRRegisters(struct SiS_Private *SiS_Pr, int depth);
extern BOOLEAN		SiSInitPtr(struct SiS_Private *SiS_Pr);
extern unsigned short	SiS_GetRefCRT1CRTC(struct SiS_Private *SiS_Pr, unsigned short Index, int UseWide);
extern unsigned short	SiS_GetRefCRTVCLK(struct SiS_Private *SiS_Pr, unsigned short Index, int UseWide);
extern void		SiS_Generic_ConvertCRData(struct SiS_Private *SiS_Pr, unsigned char *crdata,
				int xres, int yres, DisplayModePtr current);

extern unsigned short	SiS_InitDDCRegs(struct SiS_Private *SiS_Pr, unsigned int VBFlags, int VGAEngine,
                		unsigned short adaptnum, unsigned short DDCdatatype, BOOLEAN checkcr32,
				unsigned int VBFlags2);
extern unsigned short	SiS_ProbeDDC(struct SiS_Private *SiS_Pr);
extern unsigned short	SiS_ReadDDC(struct SiS_Private *SiS_Pr, unsigned short DDCdatatype,
				unsigned char *buffer);

#endif



