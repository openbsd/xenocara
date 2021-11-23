/*
 * Copyright 2005 Terry Lewis. All Rights Reserved.
 * Copyright 2005 Philip Langdale. All Rights Reserved. (CH7011 additions)
 * Copyright 1998-2003 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2003 S3 Graphics, Inc. All Rights Reserved.
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
 * VIA, S3 GRAPHICS, AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _VIA_CH7xxx_H_
#define _VIA_CH7xxx_H_ 1

/*+#define     VIA_BIOS_MAX_NUM_TV_REG         0x80
+#define     VIA_BIOS_MAX_NUM_TV_CRTC        32
+#define     VIA_BIOS_NUM_TV_SPECIAL_REG     8
+#define     VIA_BIOS_MAX_NUM_TV_PATCH       8
+#define     VIA_BIOS_NUM_TV_OTHER           16
*/

#define     VIA_BIOS_TABLE_NUM_TV_REG   0x23 /* 0x00 - 0x22 */

#define     CH_7011_MAX_NUM_REG         0x4C /* 0x00 - 0x4B */
#define     CH_7019_MAX_NUM_REG         0x80 /* 0x00 - 0x7F */

#define     VIA_BIOS_MAX_NUM_TV_CRTC        32
#define     VIA_BIOS_NUM_TV_SPECIAL_REG     8
#define     VIA_BIOS_MAX_NUM_TV_PATCH       8
#define     VIA_BIOS_NUM_TV_OTHER           16

struct CH7xxxModePrivate {
    char   id[12]; /* "CH7xxx" */
    CARD8  Standard;
};

static struct CH7xxxModePrivate CH7xxxModePrivateNTSC = {
    { 'C', 'H', '7', 'x', 'x', 'x', 0, 0, 0, 0, 0, 0 },
    TVTYPE_NTSC,
};

static struct CH7xxxModePrivate CH7xxxModePrivatePAL = {
    { 'C', 'H', '7', 'x', 'x', 'x', 0, 0, 0, 0, 0, 0 },
    TVTYPE_PAL,
};


#define MODEPREFIX(name) NULL, NULL, name, 0,M_T_DEFAULT
#define MODESUFFIXNTSC       0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,FALSE,FALSE,\
	sizeof(struct CH7xxxModePrivate),(void *)&CH7xxxModePrivateNTSC,0,0.0,0.0
#define MODESUFFIXPAL        0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,FALSE,FALSE,\
	sizeof(struct CH7xxxModePrivate),(void *)&CH7xxxModePrivatePAL,0,0.0,0.0

/* dotclock is just for modeline validation */
static DisplayModeRec CH7011Modes[]={
    { MODEPREFIX("640x480"),      23520,  640,  656,  744,  784, 0,  480,  487,  491,  600, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("640x480"),      30000,  640,  680,  808, 1000, 0,  480,  520,  523,  600, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXPAL  },
    { MODEPREFIX("800x600"),      39900,  800,  840,  976, 1064, 0,  600,  604,  620,  750, 0, V_PHSYNC | V_PVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("800x600"),      34500,  800,  816,  880,  920, 0,  600,  604,  620,  750, 0, V_PHSYNC | V_PVSYNC, MODESUFFIXPAL  },
    { MODEPREFIX("1024x768"),     54810, 1024, 1032, 1088, 1160, 0,  768,  780,  792,  945, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("1024x768"),     57000, 1024, 1040, 1112, 1200, 0,  768,  829,  840,  950, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXPAL  },

    { MODEPREFIX("640x480Over"),    20160,  640,  648,  704,  720, 0,  480,  487,  491,  560, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("640x480Over"),    21000,  640,  664,  792,  840, 0,  480,  485,  491,  500, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXPAL  },
    { MODEPREFIX("800x600Over"),    35910,  800,  840,  984, 1080, 0,  600,  601,  604,  665, 0, V_PHSYNC | V_PVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("800x600Over"),    32500,  800,  832,  928, 1000, 0,  600,  600,  604,  650, 0, V_PHSYNC | V_PVSYNC, MODESUFFIXPAL  },
    { MODEPREFIX("1024x768Over"),   50400, 1024, 1040, 1112, 1200, 0,  768,  772,  776,  840, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("1024x768Over"),   49500, 1024, 1032, 1112, 1200, 0,  768,  771,  776,  825, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXPAL  },

    { MODEPREFIX("720x480"),      25200,  720,  728,  776,  840, 0,  480,  511,  515,  600, 0, V_NHSYNC | V_PVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("720x576"),      28500,  720,  728,  744,  760, 0,  576,  635,  643,  750, 0, V_NHSYNC | V_PVSYNC, MODESUFFIXPAL  },
    { MODEPREFIX("720x480Noscale"), 27972,  720,  736,  768,  888, 0,  480,  480,  483,  525, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("720x576Noscale"), 28000,  720,  728,  864,  896, 0,  576,  576,  579,  625, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXPAL  },
};

static DisplayModeRec CH7019Modes[]={
    { MODEPREFIX("640x480"),      23520,  640,  656,  744,  784, 0,  480,  487,  491,  600, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("640x480"),      30000,  640,  680,  808, 1000, 0,  480,  520,  523,  600, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXPAL  },
    { MODEPREFIX("800x600"),      39900,  800,  840,  976, 1064, 0,  600,  604,  620,  750, 0, V_PHSYNC | V_PVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("800x600"),      34500,  800,  816,  880,  920, 0,  600,  604,  620,  750, 0, V_PHSYNC | V_PVSYNC, MODESUFFIXPAL  },
    { MODEPREFIX("1024x768"),     54810, 1024, 1032, 1088, 1160, 0,  768,  780,  792,  945, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("1024x768"),     57000, 1024, 1040, 1112, 1200, 0,  768,  829,  840,  950, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXPAL  },
    { MODEPREFIX("640x480Over"),    20160,  640,  648,  704,  720, 0,  480,  487,  491,  560, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("640x480Over"),    21000,  640,  664,  792,  840, 0,  480,  485,  491,  500, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXPAL  },
    { MODEPREFIX("800x600Over"),    35910,  800,  840,  984, 1080, 0,  600,  601,  604,  665, 0, V_PHSYNC | V_PVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("800x600Over"),    32500,  800,  832,  928, 1000, 0,  600,  600,  604,  650, 0, V_PHSYNC | V_PVSYNC, MODESUFFIXPAL  },
    { MODEPREFIX("1024x768Over"),   50400, 1024, 1040, 1112, 1200, 0,  768,  772,  776,  840, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXNTSC },
    { MODEPREFIX("1024x768Over"),   49500, 1024, 1032, 1112, 1200, 0,  768,  771,  776,  825, 0, V_NHSYNC | V_NVSYNC, MODESUFFIXPAL  },
};


typedef struct _VIATVMASKTABLE {
    CARD8   TV[VIA_BIOS_TABLE_NUM_TV_REG];
    CARD8   CRTC1[VIA_BIOS_MAX_NUM_TV_CRTC];
    CARD8   CRTC2[VIA_BIOS_MAX_NUM_TV_CRTC];
    CARD8   misc1;
    CARD8   misc2;
    int     numTV;
    int     numCRTC1;
    int     numCRTC2;
} VIABIOSTVMASKTableRec, *VIABIOSTVMASKTablePtr;

struct CH7xxxTableRec {
    const char* name;
    CARD16  Width;
    CARD16  Height;
    int     Standard;

    CARD8   TV[VIA_BIOS_TABLE_NUM_TV_REG];       /*35*/
    CARD8   CRTC1[VIA_BIOS_MAX_NUM_TV_CRTC];
    CARD8   Misc1[VIA_BIOS_NUM_TV_SPECIAL_REG];
    CARD8   Misc2[VIA_BIOS_NUM_TV_SPECIAL_REG];
/*merge these three*/
    CARD8   CRTC2_8BPP[VIA_BIOS_MAX_NUM_TV_CRTC];
    CARD8   CRTC2_16BPP[VIA_BIOS_MAX_NUM_TV_CRTC];
    CARD8   CRTC2_32BPP[VIA_BIOS_MAX_NUM_TV_CRTC];
    CARD16  Patch2[VIA_BIOS_MAX_NUM_TV_PATCH];
    CARD16  DotCrawlNTSC[VIA_BIOS_NUM_TV_OTHER];
};

static struct CH7xxxTableRec
CH7011Table[] = {
    { "640x480", 640,  480, TVTYPE_NTSC,
      { 0X6A, /* 0x00		Mode 17 */
        0X3F, /* 0x01 FF	Default 0x27 (was 7F) */
        0X7E, /* 0x02 VBW	Default 0xBE (was 0x7E) */
        0X8B, /* 0x03 TE	Decent Text 0x8B (was 8D) */
        0X28, /* 0x04 SAV	Default 0x50 (was 0x21) */
        0X2C, /* 0x05 HP	Default 0x50 (was 0x2E) */
        0X05, /* 0x06 VP	Default 0x00 (was 0x04) */
        0X83, /* 0x07 BL	Default 0x83 */
        0X03, /* 0x08 CE	Default 0x03 */
        0X80, /* 0x09 TPC	Default 0x80 */
        0X3F, /* 0x0A PLLM	Default 0x3F */
        0X7E, /* 0x0B PLLN	Default 0x7E */
        0X20, /* 0x0C FSCI	Default 0x20 */
        0X80, /* 0x0D FSCI	Default 0x80 */
        0X00, /* 0x0E FSCI	Default 0x08 (was 00) */
        0X00, /* 0x0F FSCI	Default 0xEB (was 00) */
        0,    /* 0x10 CIVC */
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* We don't touch these */
        0X48, /* 0x1C */
        0X40, /* 0x1D */
        0XD2, /* 0x1E */
        0X80, /* 0x1F */
        0X40, /* 0x20 */
        0,    /* 0x21 */
        0,    /* 0x22 */ },
      { 0X5D, 0X4F, 0X4F, 0X81, 0X52, 0X9E, 0X56, 0XBA, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X8, 0, 0XDF, 0, 0, 0XDF, 0X57, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0X20, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0XF, 0X7F, 0X7F, 0XF, 0X9A, 0X23, 0X8F, 0XEF, 0X57, 0XDF, 0XDF, 0X57, 0X11, 0XA, 0X8, 0X50, 0, 0, 0, 0, 0, 0X28, 0X50, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XF, 0X7F, 0X7F, 0XF, 0X9A, 0X23, 0X8F, 0XEF, 0X57, 0XDF, 0XDF, 0X57, 0X11, 0XA, 0X8, 0X50, 0, 0, 0, 0, 0, 0X50, 0XA0, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XF, 0X7F, 0X7F, 0XF, 0X9A, 0X23, 0X8F, 0XEF, 0X57, 0XDF, 0XDF, 0X57, 0X11, 0XA, 0X8, 0X50, 0, 0, 0, 0, 0, 0XA0, 0X40, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X2284, 0, 0, 0, 0, 0, 0, 0 },
      { 0X2, 0X811, 0X9217, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "640x480", 640,  480, TVTYPE_PAL,
      { 0X61, /* 0x00         PAL Mode 14 non-OS 640x480 1:1 */
        0X27, /* 0x01 FF      Default 0x27 (was 7F) */
        0XBE, /* 0x02 VBW     Default 0xBE (was 0x7E) */
        0X8B, /* 0x03 TE      Decent Text 0x8B (was 8D) */
        0X28, /* 0x04 SAV     Default 0x50 (was 0x21) */
        0X2C, /* 0x05 HP      Default 0x50 (was 0x2E) */
        0X05, /* 0x06 VP      Default 0x00 (was 0x04) */
        0X83, /* 0x07 BL      Default 0x83 */
        0X01, /* 0x08 CE      Default 0x03 */
        0X81, /* 0x09 TPC     Default 0x80 */
        0X04, /* 0x0A PLLM    Default 0x3F */
        0X09, /* 0x0B PLLN    Default 0x7E */
        0X26, /* 0x0C FSCI    Default 0x20 */
        0X6F, /* 0x0D FSCI    Default 0x80 */
        0X1F, /* 0x0E FSCI    Default 0x08 */
        0XD0, /* 0x0F FSCI    Default 0xEB */
        0,    /* 0x10 CIVC */
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* We don't touch these */
        0X48, /* 0x1C */
        0X40, /* 0x1D */
        0XD2, /* 0x1E */
        0X80, /* 0x1F */
        0X40, /* 0x20 */
        0,    /* 0x21 */
        0,    /* 0x22 */ },
      { 0X64, 0X4F, 0X4F, 0X88, 0X53, 0X83, 0X6F, 0XBA, 0, 0X40, 0, 0, 0, 0, 0, 0, 0X11, 0, 0XDF, 0, 0, 0XDF, 0X70, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0X20, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X47, 0X7F, 0X7F, 0X47, 0X9A, 0X23, 0X95, 0X1E, 0X70, 0XDF, 0XDF, 0X70, 0X51, 0XA, 0X11, 0X5D, 0, 0, 0, 0, 0, 0X28, 0X50, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X47, 0X7F, 0X7F, 0X47, 0X9A, 0X23, 0X95, 0X1E, 0X70, 0XDF, 0XDF, 0X70, 0X51, 0XA, 0X11, 0X5D, 0, 0, 0, 0, 0, 0X50, 0XA0, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X47, 0X7F, 0X7F, 0X47, 0X9A, 0X23, 0X95, 0X1E, 0X70, 0XDF, 0XDF, 0X70, 0X51, 0XA, 0X11, 0X5D, 0, 0, 0, 0, 0, 0XA0, 0X40, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X3284, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "800x600", 800,  600, TVTYPE_NTSC,
      { 0XCF, /* 0x00		Mode 29 */
        0X27, /* 0x01 FF	Default 0x27  (was 7F) */
        0XBE, /* 0x02 VBW	Default 0xBE (was 0x76) */
        0X8B, /* 0x03 TE	Decent Text 0x8B (was 8F) */
        0X59, /* 0x04 SAV*/
        0X3C, /* 0x05 HP */
        0X15, /* 0x06 VP */
        0X66, /* 0x07 BL	Default 0x83 */
        0X3,  /* 0x08 CE	Default 0x03 */
        0X88,
        0X59,
        0X2E,
        0X19,
        0X8B,
        0X3A,
        0X63,
        0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0X48,
        0X40,
        0XD2,
        0X80,
        0X40,
        0,
        0, },
        { 0X80, 0X63, 0X63, 0X84, 0X69, 0X1A, 0XEC, 0XF0, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X5C, 0, 0X57, 0, 0, 0X57, 0XED, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
        { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
        { 0X27, 0X1F, 0X1F, 0X27, 0XE3, 0X34, 0X48, 0XD6, 0XED, 0X57, 0X57, 0XED, 0X52, 0X12, 0X5C, 0X5D, 0, 0, 0, 0, 0, 0X32, 0X64, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
        { 0X27, 0X1F, 0X1F, 0X27, 0XE3, 0X34, 0X48, 0XD6, 0XED, 0X57, 0X57, 0XED, 0X52, 0X12, 0X5C, 0X5D, 0, 0, 0, 0, 0, 0X64, 0XC8, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
        { 0X27, 0X1F, 0X1F, 0X27, 0XE3, 0X34, 0X48, 0XD6, 0XED, 0X57, 0X57, 0XED, 0X52, 0X12, 0X5C, 0X5D, 0, 0, 0, 0, 0, 0XC8, 0X90, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
        { 0X5A84, 0, 0, 0, 0, 0, 0, 0 },
        { 0X2, 0X811, 0X5117, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "800x600", 800,  600, TVTYPE_PAL,
      { 0XC3, 0X7F, 0XE0, 0X8F, 0X39, 0X3F, 0X38, 0X70, 0X3, 0X81, 0X21, 0X56, 0X1F, 0X87, 0X28, 0X18,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0, },
      { 0X73, 0X63, 0X63, 0X97, 0X67, 0X91, 0XEC, 0XF0, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X7E, 0, 0X57, 0, 0, 0X57, 0XED, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0X20, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0XBF, 0X1F, 0X1F, 0XBF, 0XDB, 0X33, 0X38, 0X8E, 0XED, 0X57, 0X57, 0XED, 0X52, 0X12, 0X74, 0X4D, 0, 0, 0, 0, 0, 0X32, 0X64, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XBF, 0X1F, 0X1F, 0XBF, 0XDB, 0X33, 0X38, 0X8E, 0XED, 0X57, 0X57, 0XED, 0X52, 0X12, 0X74, 0X4D, 0, 0, 0, 0, 0, 0X64, 0XC8, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XBF, 0X1F, 0X1F, 0XBF, 0XDB, 0X33, 0X38, 0X8E, 0XED, 0X57, 0X57, 0XED, 0X52, 0X12, 0X74, 0X4D, 0, 0, 0, 0, 0, 0XC8, 0X90, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X3A84, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

/*check these two modes*/
    { "1024x768", 1024, 768, TVTYPE_NTSC,
      { 0XEE,
        0X3F, /* 0x01 FF	Default 0x27 (was 7F) */
        0X7E,
        0X87,
        0X49,
        0X32,
        0X9,
        0X83,
        0X3,
        0X88,
        0X47,
        0X4D,
        0X1B,
        0XE4,
        0X89,
        0X51,
        0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0X48,
        0X40,
        0XD2,
        0X80,
        0X40,
        0,
        0, },
      { 0X8C, 0X7F, 0X7F, 0X90, 0X81, 0X8, 0XAF, 0XF5, 0, 0X60, 0, 0, 0, 0, 0, 0, 0XC, 0, 0XFF, 0, 0, 0XFF, 0XB0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0X40, 0X80, 0XE, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X87, 0XFF, 0XFF, 0X87, 0X23, 0X34, 0X9, 0X3F, 0XB0, 0XFF, 0XFF, 0XB0, 0X9A, 0X13, 0XC, 0X7A, 0, 0, 0, 0, 0, 0X40, 0X80, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X87, 0XFF, 0XFF, 0X87, 0X23, 0X34, 0X9, 0X3F, 0XB0, 0XFF, 0XFF, 0XB0, 0X9A, 0X13, 0XC, 0X7A, 0, 0, 0, 0, 0, 0X80, 0, 0X41, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X87, 0XFF, 0XFF, 0X87, 0X23, 0X34, 0X9, 0X3F, 0XB0, 0XFF, 0XFF, 0XB0, 0X9A, 0X13, 0XC, 0X7A, 0, 0, 0, 0, 0, 0, 0, 0X86, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X4A84, 0, 0, 0, 0, 0, 0, 0 },
      { 0X2, 0X811, 0X6717, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "1024x768", 1024, 768, TVTYPE_PAL,
      { 0XE5, 0X7F, 0XE0, 0X8F, 0XC1, 0X3E, 0X4A, 0X70, 0, 0X81, 0X7, 0X2A, 0X20, 0X6D, 0XC2, 0XD7,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0, },
      { 0XAA, 0X7F, 0X7F, 0X8E, 0X83, 0X97, 0XE6, 0XF5, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X11, 0, 0XFF, 0, 0, 0XFF, 0XE7, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0X40, 0X80, 0XE, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XBE, 0XE7, 0XFF, 0XFF, 0XE7, 0X9A, 0X13, 0X7, 0X7B, 0, 0, 0, 0, 0, 0X40, 0X80, 0, 0, 0, 0X80, 0X20, 0X8E, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XBE, 0XE7, 0XFF, 0XFF, 0XE7, 0X9A, 0X13, 0X7, 0X7B, 0, 0, 0, 0, 0, 0X80, 0, 0X41, 0, 0, 0X80, 0X20, 0X8E, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XBE, 0XE7, 0XFF, 0XFF, 0XE7, 0X9A, 0X13, 0X7, 0X7B, 0, 0, 0, 0, 0, 0, 0, 0X86, 0, 0, 0X80, 0X20, 0X8E, 0, 0, 0 },
      { 0XC284, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "640x480Over", 640,  480, TVTYPE_NTSC,
      { 0X69, /* 0x00 DM	Mode 16 640x480 1/1 */
        0X3F, /* 0x01 FF	Default 0x27 (was 7F) */
        0X7E, /* 0x02 VBW	Default 0xBE (was 7E) */
        0X03, /* 0x03 TE	Decent text 0x83 (was 8D) */
        0X18, /* 0x04 SAV	Default 0x50 (was 10) */
        0X19, /* 0x05 HP	Default 0x50 */
        0XFB, /* 0x06 VP	Default 0x00 */
        0X83, /* 0x07 BL	Default 0x83 (NTSC-J 66) */
        0X03, /* 0x08 CE	Default 0x03 */
        0X80, /* 0x09 TPC	Default 0x80 */
        0X3F, /* 0x0A PLLM	Default 0x3F */
        0X6E, /* 0x0B PLLN	Default 0x7E */
        0X25, /* 0x0C FSCI	Default 0x25 */
        0X24, /* 0x0D FSCI	Default 0x24 */
        0X92, /* 0x0E FSCI	Default 0x9C (was 92) */
        0X49, /* 0x0F FSCI	Default 0x7A (was 49) */
        0X00, /* 0x10 CIVC	Default 0x01 */
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* We don't touch these */
        0X48, /* 0x1C CM	Default 0x00 */
        0X40, /* 0x1D IC	Default 0x88 */
        0XD2, /* 0x1E GPIO	Default 0xC0 */
        0X80, /* 0x1F IDF	Default	0x00 */
        0X40, /* 0x20 CD */
        0X00, /* 0x21 DC */
        0X00, /* 0x22 BCO	Default 0x00 */ },
/* why is this #ifed, what's the difference? */
#if 0
        { 0X55, 0X4F, 0X4F, 0X99, 0X51, 0X18, 0X2E, 0X3E, 0, 0X40, 0, 0, 0, 0, 0, 0, 0XE7, 0, 0XDF, 0, 0, 0XDF, 0X2F, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0X20, 0X40, 0, 0, 0X87, 0X1C, 0, 0 },
        { 0, 0, 0, 0X87, 0X1C, 0, 0, 0 },
        { 0XCF, 0X7F, 0X7F, 0XCF, 0X92, 0X22, 0X87, 0XBC, 0X2F, 0XDF, 0XDF, 0X2F, 0X11, 0XA, 0XFF, 0X24, 0, 0, 0, 0, 0, 0X28, 0X50, 0, 0, 0, 0X80, 0, 0X80, 0, 0, 0 },
        { 0XCF, 0X7F, 0X7F, 0XCF, 0X92, 0X22, 0X87, 0XBC, 0X2F, 0XDF, 0XDF, 0X2F, 0X11, 0XA, 0XFF, 0X24, 0, 0, 0, 0, 0, 0X50, 0XA0, 0X40, 0, 0, 0X80, 0, 0X80, 0, 0, 0 },
        { 0XCF, 0X7F, 0X7F, 0XCF, 0X92, 0X22, 0X87, 0XBC, 0X2F, 0XDF, 0XDF, 0X2F, 0X11, 0XA, 0XFF, 0X24, 0, 0, 0, 0, 0, 0XA0, 0X40, 0X81, 0, 0, 0X80, 0, 0X80, 0, 0, 0 },
        { 0X7107, 0, 0, 0, 0, 0, 0, 0 },
        { 0X3, 0X811, 0XF416, 0X9F17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
#else
        { 0X5D, 0X4F, 0X4F, 0X81, 0X52, 0X9E, 0XB, 0X3E, 0, 0X60, 0, 0, 0, 0, 0, 0, 0XEE, 0, 0XDF, 0, 0, 0XDF, 0XC, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0X20, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
        { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
        { 0XF, 0X7F, 0X7F, 0XF, 0X9A, 0X23, 0X8F, 0XFF, 0XC, 0XDF, 0XDF, 0XC, 0X11, 0XA, 0XEE, 0X31, 0, 0, 0, 0, 0, 0X28, 0X50, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
        { 0XF, 0X7F, 0X7F, 0XF, 0X9A, 0X23, 0X8F, 0XFF, 0XC, 0XDF, 0XDF, 0XC, 0X11, 0XA, 0XEE, 0X31, 0, 0, 0, 0, 0, 0X50, 0XA0, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
        { 0XF, 0X7F, 0X7F, 0XF, 0X9A, 0X23, 0X8F, 0XFF, 0XC, 0XDF, 0XDF, 0XC, 0X11, 0XA, 0XEE, 0X31, 0, 0, 0, 0, 0, 0XA0, 0X40, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
        { 0X1184, 0, 0, 0, 0, 0, 0, 0 },
        { 0X2, 0X811, 0XAD17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
#endif
    },

    { "640x480Over", 640,  480, TVTYPE_PAL,
      { 0X60, /* 0x00 DM      Mode 13 PAL 640x480 OS 5/4 */
        0X27, /* 0x01 FF      Default 0x27 (was 7F) */
        0XBE, /* 0x02 VBW     Default 0xBE (was 7E) */
        0X83, /* 0x03 TE      Decent text 0x8B (was 8D) */
        0X10, /* 0x04 SAV     Default 0x50 */
        0X19, /* 0x05 HP      Default 0x50 */
        0XFB, /* 0x06 VP      Default 0x00 */
        0X83, /* 0x07 BL      Default 0x83 */
        0X01, /* 0x08 CE      Default 0x03 */
        0X81, /* 0x09 TPC     Default 0x80 */
        0X0D, /* 0x0A PLLM    Default 0x3F */
        0X0B, /* 0x0B PLLN    Default 0x7E */
        0X30, /* 0x0C FSCI    Default 0x25 */
        0X0A, /* 0x0D FSCI    Default 0x24 */
        0XE7, /* 0x0E FSCI    Default 0x9C */
        0XC4, /* 0x0F FSCI    Default 0x7A */
        0X00, /* 0x10 CIVC    Default 0x01 */
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* We don't touch these */
        0X48, /* 0x1C CM      Default 0x00 */
        0X40, /* 0x1D IC      Default 0x88 */
        0XD2, /* 0x1E GPIO    Default 0xC0 */
        0X80, /* 0x1F IDF     Default 0x00 */
        0X40, /* 0x20 CD */
        0X00, /* 0x21 DC */
        0X00, /* 0x22 BCO     Default 0x00 */ },
      { 0X64, 0X4F, 0X4F, 0X88, 0X53, 0X83, 0XF2, 0X1F, 0, 0X40, 0, 0, 0, 0, 0, 0, 0XE5, 0, 0XDF, 0, 0, 0XDF, 0XF3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0X20, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X47, 0X7F, 0X7F, 0X47, 0X9A, 0X23, 0X95, 0XFF, 0XF3, 0XDF, 0XDF, 0XF3, 0X9, 0X9, 0XE5, 0X40, 0, 0, 0, 0, 0, 0X28, 0X50, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X47, 0X7F, 0X7F, 0X47, 0X9A, 0X23, 0X95, 0XFF, 0XF3, 0XDF, 0XDF, 0XF3, 0X9, 0X9, 0XE5, 0X40, 0, 0, 0, 0, 0, 0X50, 0XA0, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X47, 0X7F, 0X7F, 0X47, 0X9A, 0X23, 0X95, 0XFF, 0XF3, 0XDF, 0XDF, 0XF3, 0X9, 0X9, 0XE5, 0X40, 0, 0, 0, 0, 0, 0XA0, 0X40, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X3184, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "800x600Over", 800,  600, TVTYPE_NTSC,
        { 0XCE, /* 0x00		Mode 28 */
          0X27, /* 0x01		Default 0x27 (was 7F) */
          0XBE, /* 0x02		Default 0xBE (was 76) */
          0X8F, /* 0x03 */
          0X51, /* 0x04 */
          0X2E, /* 0x05 */
          0X10, /* 0x06 */
          0X83, /* 0x07 */
          0X3,  /* 0x08 */
          0X81, /* 0x09 */
          0X13, /* 0x0A */
          0X3E, /* 0x0B */
          0X1C, /* 0x0C */
          0,    /* 0x0D */
          0,    /* 0x0E */
          0,    /* 0x0F */
          0,    /* 0x10 */
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0X48,
          0X40,
          0XD2,
          0X80,
          0X40, 0, 0, },
        { 0X7D, 0X63, 0X63, 0X81, 0X69, 0X18, 0XBA, 0XF0, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X5A, 0, 0X57, 0, 0, 0X57, 0XBB, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
        { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
        { 0XF, 0X1F, 0X1F, 0XF, 0XE3, 0X34, 0X44, 0XC6, 0XBB, 0X57, 0X57, 0XBB, 0X52, 0X12, 0X3F, 0X59, 0, 0, 0, 0, 0, 0X32, 0X64, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
        { 0XF, 0X1F, 0X1F, 0XF, 0XE3, 0X34, 0X44, 0XC6, 0XBB, 0X57, 0X57, 0XBB, 0X52, 0X12, 0X3F, 0X59, 0, 0, 0, 0, 0, 0X64, 0XC8, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
        { 0XF, 0X1F, 0X1F, 0XF, 0XE3, 0X34, 0X44, 0XC6, 0XBB, 0X57, 0X57, 0XBB, 0X52, 0X12, 0X3F, 0X59, 0, 0, 0, 0, 0, 0XC8, 0X90, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
        { 0X5284, 0, 0, 0, 0, 0, 0, 0 },
        { 0X2, 0X811, 0XD017, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "800x600Over", 800,  600, TVTYPE_PAL,
        { 0XC1, 0X7F, 0XE0, 0X8F, 0X20, 0X1D, 0X36, 0X70, 0X3, 0X94, 0X39, 0X87, 0X26, 0X79, 0X8C, 0XC,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
          0X40, 0, 0, },
      { 0X71, 0X63, 0X63, 0X95, 0X67, 0X90, 0X6F, 0XF0, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X57, 0, 0X57, 0, 0, 0X57, 0X70, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0X20, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0XAF, 0X1F, 0X1F, 0XAF, 0XDB, 0X33, 0X35, 0X8E, 0X70, 0X57, 0X57, 0X70, 0X52, 0X12, 0X57, 0X5A, 0, 0, 0, 0, 0, 0X32, 0X64, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XAF, 0X1F, 0X1F, 0XAF, 0XDB, 0X33, 0X35, 0X8E, 0X70, 0X57, 0X57, 0X70, 0X52, 0X12, 0X57, 0X5A, 0, 0, 0, 0, 0, 0X64, 0XC8, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XAF, 0X1F, 0X1F, 0XAF, 0XDB, 0X33, 0X35, 0X8E, 0X70, 0X57, 0X57, 0X70, 0X52, 0X12, 0X57, 0X5A, 0, 0, 0, 0, 0, 0XC8, 0X90, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X2184, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "1024x768Over", 1024,  768, TVTYPE_NTSC,
      { 0XED,
        0X3F, /* 0x01 FF	Default 0x27 (was 7F) */
        0X7E,
        0X87,
        0X49,
        0X20,
        0,
        0X83,
        0X3,
        0X90,
        0X89,
        0X35,
        0X1F,
        0X61,
        0X1A,
        0X7C,
        0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0X48,
        0X40,
        0XD2,
        0X80,
        0X40,
        0,
        0, },
      { 0X8C, 0X7F, 0X7F, 0X90, 0X81, 0X8, 0X46, 0XF5, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X4, 0, 0XFF, 0, 0, 0XFF, 0X47, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X87, 0XFF, 0XFF, 0X87, 0X23, 0X34, 0X9, 0X38, 0X47, 0XFF, 0XFF, 0X47, 0X9A, 0X13, 0X4, 0X6F, 0, 0, 0, 0, 0, 0X40, 0X80, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X87, 0XFF, 0XFF, 0X87, 0X23, 0X34, 0X9, 0X38, 0X47, 0XFF, 0XFF, 0X47, 0X9A, 0X13, 0X4, 0X6F, 0, 0, 0, 0, 0, 0X80, 0, 0X41, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X87, 0XFF, 0XFF, 0X87, 0X23, 0X34, 0X9, 0X38, 0X47, 0XFF, 0XFF, 0X47, 0X9A, 0X13, 0X4, 0X6F, 0, 0, 0, 0, 0, 0, 0, 0X86, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X5084, 0, 0, 0, 0, 0, 0, 0 },
      { 0X2, 0X811, 0X4517, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "1024x768Over", 1024,  768, TVTYPE_PAL,
      { 0XE4, 0X7F, 0XA0, 0X8F, 0XB1, 0X28, 0X37, 0X70, 0, 0X81, 0X10, 0X4C, 0X25, 0XF, 0XBA, 0X1B,
        0X1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0, },
      { 0XAA, 0X7F, 0X7F, 0X8E, 0X84, 0X97, 0X69, 0XF5, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X7, 0, 0XFF, 0, 0, 0XFF, 0X6A, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XB7, 0X6A, 0XFF, 0XFF, 0X6A, 0X9A, 0X13, 0X7, 0X77, 0, 0, 0, 0, 0, 0X40, 0X80, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XB7, 0X6A, 0XFF, 0XFF, 0X6A, 0X9A, 0X13, 0X7, 0X77, 0, 0, 0, 0, 0, 0X80, 0, 0X41, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XB7, 0X6A, 0XFF, 0XFF, 0X6A, 0X9A, 0X13, 0X7, 0X77, 0, 0, 0, 0, 0, 0, 0, 0X86, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XB184, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "720x480", 720,  480, TVTYPE_NTSC,
        { 0X89, /* 0x00 DM	Mode 19 720x480 1/1 */
          0X3F, /* 0x01 FF	Default 0x27 (was 7F) */
          0X7E, /* 0x02 VBW	Default 0xBE (was 7E) */
          0X03, /* 0x03 TE	Decent text 0x83 (was 8D) */
          0X18, /* 0x04 SAV	Default 0x50 (was 10) */
          0X19, /* 0x05 HP	Default 0x50 */
          0XFB, /* 0x06 VP	Default 0x00 */
          0X83, /* 0x07 BL	Default 0x83 (NTSC-J 66) */
          0X03, /* 0x08 CE	Default 0x03 */
          0X80, /* 0x09 TPC	Default 0x80 */
          0X3F, /* 0x0A PLLM	Default 0x3F */
          0X7C, /* 0x0B PLLN	Default 0x7C */
          0X21, /* 0x0C FSCI	Default 0x25 */
          0X04, /* 0x0D FSCI	Default 0x04 */
          0X10, /* 0x0E FSCI	Default 0x10 */
          0X41, /* 0x0F FSCI	Default 0x41 */
          0X00, /* 0x10 CIVC	Default 0x01 */
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* We don't touch these */
          0X48, /* 0x1C CM	Default 0x00 */
          0X40, /* 0x1D IC	Default 0x88 */
          0XD2, /* 0x1E GPIO	Default 0xC0 */
          0X80, /* 0x1F IDF	Default	0x00 */
          0X40, /* 0x20 CD */
          0X00, /* 0x21 DC */
          0X00, /* 0x22 BCO	Default 0x00 */ },
        { 0X64, 0X59, 0X59, 0X88, 0X5B, 0X81, 0X56, 0X3E, 0, 0X40, 0, 0, 0, 0, 0, 0, 0XFF, 0, 0XDF, 0, 0, 0XDF, 0X57, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0X20, 0X40, 0, 0X4, 0X87, 0X1C, 0, 0 },
        { 0, 0, 0, 0X87, 0X1C, 0, 0, 0 },
        { 0X47, 0XCF, 0XCF, 0X47, 0X9A, 0X23, 0XD9, 0XA, 0X57, 0XDF, 0XDF, 0X57, 0X51, 0XA, 0XFF, 0X3B, 0, 0, 0, 0, 0, 0X2D, 0X5A, 0, 0, 0, 0X80, 0, 0X80, 0, 0, 0 },
        { 0X47, 0XCF, 0XCF, 0X47, 0X9A, 0X23, 0XD9, 0XA, 0X57, 0XDF, 0XDF, 0X57, 0X51, 0XA, 0XFF, 0X3B, 0, 0, 0, 0, 0, 0X5A, 0XB4, 0X40, 0, 0, 0X80, 0, 0X80, 0, 0, 0 },
        { 0X47, 0XCF, 0XCF, 0X47, 0X9A, 0X23, 0XD9, 0XA, 0X57, 0XDF, 0XDF, 0X57, 0X51, 0XA, 0XFF, 0X3B, 0, 0, 0, 0, 0, 0XB4, 0X68, 0X81, 0, 0, 0X80, 0, 0X80, 0, 0, 0 },
        { 0X6E07, 0, 0, 0, 0, 0, 0, 0 },
        { 0X3, 0X811, 0XC316, 0X4C17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

/* don't we want 720x576 for pal? */
    { "720x480", 720,  480, TVTYPE_PAL,
        { 0XE4, 0X7F, 0XA0, 0X8F, 0XB1, 0X28, 0X37, 0X70, 0, 0X81, 0X10, 0X4C, 0X25, 0XF, 0XBA, 0X1B,
          0X1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
          0X40, 0, 0, },
      { 0XAA, 0X7F, 0X7F, 0X8E, 0X84, 0X97, 0X69, 0XF5, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X7, 0, 0XFF, 0, 0, 0XFF, 0X6A, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XB7, 0X6A, 0XFF, 0XFF, 0X6A, 0X9A, 0X13, 0X7, 0X77, 0, 0, 0, 0, 0, 0X40, 0X80, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XB7, 0X6A, 0XFF, 0XFF, 0X6A, 0X9A, 0X13, 0X7, 0X77, 0, 0, 0, 0, 0, 0X80, 0, 0X41, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XB7, 0X6A, 0XFF, 0XFF, 0X6A, 0X9A, 0X13, 0X7, 0X77, 0, 0, 0, 0, 0, 0, 0, 0X86, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XB184, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },
};

static struct CH7xxxTableRec
CH7019Table[] = {
    { "640x480", 640,  480, TVTYPE_NTSC,
      { 0X6A, 0X7F, 0X7E, 0X8D, 0X21, 0X2E, 0X4, 0X83, 0X3, 0X80, 0X3F, 0X7E, 0X20, 0X80,   0,   0,
        0,    0,    0,    0,    0,    0,   0,    0,   0,    0,    0,    0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0 },
      { 0X5D, 0X4F, 0X4F, 0X81, 0X52, 0X9E, 0X56, 0XBA, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X8, 0, 0XDF, 0, 0, 0XDF, 0X57, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0X20, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0XF, 0X7F, 0X7F, 0XF, 0X9A, 0X23, 0X8F, 0XEF, 0X57, 0XDF, 0XDF, 0X57, 0X11, 0XA, 0X8, 0X50, 0, 0, 0, 0, 0, 0X28, 0X50, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XF, 0X7F, 0X7F, 0XF, 0X9A, 0X23, 0X8F, 0XEF, 0X57, 0XDF, 0XDF, 0X57, 0X11, 0XA, 0X8, 0X50, 0, 0, 0, 0, 0, 0X50, 0XA0, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XF, 0X7F, 0X7F, 0XF, 0X9A, 0X23, 0X8F, 0XEF, 0X57, 0XDF, 0XDF, 0X57, 0X11, 0XA, 0X8, 0X50, 0, 0, 0, 0, 0, 0XA0, 0X40, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X2284, 0, 0, 0, 0, 0, 0, 0 },
      { 0X2, 0X811, 0X9217, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "640x480", 640,  480, TVTYPE_PAL,
      { 0X61, 0X7F, 0XE0, 0X8F, 0X31, 0X35, 0X33, 0X6E, 0X3, 0X81, 0X4, 0X9, 0X26, 0X6F, 0X1F, 0XD0,
        0,    0,    0,    0,    0,    0,    0,    0,   0,    0  , 0,   0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0 },
      { 0X64, 0X4F, 0X4F, 0X88, 0X53, 0X83, 0X6F, 0XBA, 0, 0X40, 0, 0, 0, 0, 0, 0, 0X11, 0, 0XDF, 0, 0, 0XDF, 0X70, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0X20, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X47, 0X7F, 0X7F, 0X47, 0X9A, 0X23, 0X95, 0X1E, 0X70, 0XDF, 0XDF, 0X70, 0X51, 0XA, 0X11, 0X5D, 0, 0, 0, 0, 0, 0X28, 0X50, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X47, 0X7F, 0X7F, 0X47, 0X9A, 0X23, 0X95, 0X1E, 0X70, 0XDF, 0XDF, 0X70, 0X51, 0XA, 0X11, 0X5D, 0, 0, 0, 0, 0, 0X50, 0XA0, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X47, 0X7F, 0X7F, 0X47, 0X9A, 0X23, 0X95, 0X1E, 0X70, 0XDF, 0XDF, 0X70, 0X51, 0XA, 0X11, 0X5D, 0, 0, 0, 0, 0, 0XA0, 0X40, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X3284, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "800x600", 800,  600, TVTYPE_NTSC,
      { 0XCF, 0X7F, 0X76, 0X8F, 0X59, 0X3C, 0X15, 0X83, 0X3, 0X88, 0X59, 0X2E, 0X19, 0X8B, 0X3A, 0X63,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0},
      { 0X80, 0X63, 0X63, 0X84, 0X69, 0X1A, 0XEC, 0XF0, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X5C, 0, 0X57, 0, 0, 0X57, 0XED, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X27, 0X1F, 0X1F, 0X27, 0XE3, 0X34, 0X48, 0XD6, 0XED, 0X57, 0X57, 0XED, 0X52, 0X12, 0X5C, 0X5D, 0, 0, 0, 0, 0, 0X32, 0X64, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X27, 0X1F, 0X1F, 0X27, 0XE3, 0X34, 0X48, 0XD6, 0XED, 0X57, 0X57, 0XED, 0X52, 0X12, 0X5C, 0X5D, 0, 0, 0, 0, 0, 0X64, 0XC8, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X27, 0X1F, 0X1F, 0X27, 0XE3, 0X34, 0X48, 0XD6, 0XED, 0X57, 0X57, 0XED, 0X52, 0X12, 0X5C, 0X5D, 0, 0, 0, 0, 0, 0XC8, 0X90, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X5A84, 0, 0, 0, 0, 0, 0, 0 },
      { 0X2, 0X811, 0X5117, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "800x600", 800,  600, TVTYPE_PAL,
      { 0XC3, 0X7F, 0XE0, 0X8F, 0X39, 0X3F, 0X38, 0X70, 0X3, 0X81, 0X21, 0X56, 0X1F, 0X87, 0X28, 0X18,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0},
      { 0X73, 0X63, 0X63, 0X97, 0X67, 0X91, 0XEC, 0XF0, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X7E, 0, 0X57, 0, 0, 0X57, 0XED, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0X20, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0XBF, 0X1F, 0X1F, 0XBF, 0XDB, 0X33, 0X38, 0X8E, 0XED, 0X57, 0X57, 0XED, 0X52, 0X12, 0X74, 0X4D, 0, 0, 0, 0, 0, 0X32, 0X64, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XBF, 0X1F, 0X1F, 0XBF, 0XDB, 0X33, 0X38, 0X8E, 0XED, 0X57, 0X57, 0XED, 0X52, 0X12, 0X74, 0X4D, 0, 0, 0, 0, 0, 0X64, 0XC8, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XBF, 0X1F, 0X1F, 0XBF, 0XDB, 0X33, 0X38, 0X8E, 0XED, 0X57, 0X57, 0XED, 0X52, 0X12, 0X74, 0X4D, 0, 0, 0, 0, 0, 0XC8, 0X90, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X3A84, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "1024x768", 1024,  768, TVTYPE_NTSC,
      { 0XEE, 0X7F, 0X7E, 0X87, 0X49, 0X32, 0X9, 0X83, 0X3, 0X88, 0X47, 0X4D, 0X1B, 0XE4, 0X89, 0X51,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0},
      { 0X8C, 0X7F, 0X7F, 0X90, 0X81, 0X8, 0XAF, 0XF5, 0, 0X60, 0, 0, 0, 0, 0, 0, 0XC, 0, 0XFF, 0, 0, 0XFF, 0XB0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0X40, 0X80, 0XE, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X87, 0XFF, 0XFF, 0X87, 0X23, 0X34, 0X9, 0X3F, 0XB0, 0XFF, 0XFF, 0XB0, 0X9A, 0X13, 0XC, 0X7A, 0, 0, 0, 0, 0, 0X40, 0X80, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X87, 0XFF, 0XFF, 0X87, 0X23, 0X34, 0X9, 0X3F, 0XB0, 0XFF, 0XFF, 0XB0, 0X9A, 0X13, 0XC, 0X7A, 0, 0, 0, 0, 0, 0X80, 0, 0X41, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X87, 0XFF, 0XFF, 0X87, 0X23, 0X34, 0X9, 0X3F, 0XB0, 0XFF, 0XFF, 0XB0, 0X9A, 0X13, 0XC, 0X7A, 0, 0, 0, 0, 0, 0, 0, 0X86, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X4A84, 0, 0, 0, 0, 0, 0, 0 },
      { 0X2, 0X811, 0X6717, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },

    { "1024x768", 1024,  768, TVTYPE_PAL,
      { 0XE5, 0X7F, 0XE0, 0X8F, 0XC1, 0X3E, 0X4A, 0X70, 0, 0X81, 0X7, 0X2A, 0X20, 0X6D, 0XC2, 0XD7,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0},
      { 0XAA, 0X7F, 0X7F, 0X8E, 0X83, 0X97, 0XE6, 0XF5, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X11, 0, 0XFF, 0, 0, 0XFF, 0XE7, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0X40, 0X80, 0XE, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XBE, 0XE7, 0XFF, 0XFF, 0XE7, 0X9A, 0X13, 0X7, 0X7B, 0, 0, 0, 0, 0, 0X40, 0X80, 0, 0, 0, 0X80, 0X20, 0X8E, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XBE, 0XE7, 0XFF, 0XFF, 0XE7, 0X9A, 0X13, 0X7, 0X7B, 0, 0, 0, 0, 0, 0X80, 0, 0X41, 0, 0, 0X80, 0X20, 0X8E, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XBE, 0XE7, 0XFF, 0XFF, 0XE7, 0X9A, 0X13, 0X7, 0X7B, 0, 0, 0, 0, 0, 0, 0, 0X86, 0, 0, 0X80, 0X20, 0X8E, 0, 0, 0 },
      { 0XC284, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "640x480Over", 640,  480, TVTYPE_NTSC,
      { 0X69, 0X7F, 0X7E, 0X8D, 0X10, 0X19, 0, 0X83, 0X3, 0X80, 0X3F, 0X6E, 0X25, 0X24, 0X92, 0X49,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0},
      { 0X5D, 0X4F, 0X4F, 0X81, 0X52, 0X9E, 0XB, 0X3E, 0, 0X60, 0, 0, 0, 0, 0, 0, 0XEE, 0, 0XDF, 0, 0, 0XDF, 0XC, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0X20, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0XF, 0X7F, 0X7F, 0XF, 0X9A, 0X23, 0X8F, 0XFF, 0XC, 0XDF, 0XDF, 0XC, 0X11, 0XA, 0XEE, 0X31, 0, 0, 0, 0, 0, 0X28, 0X50, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XF, 0X7F, 0X7F, 0XF, 0X9A, 0X23, 0X8F, 0XFF, 0XC, 0XDF, 0XDF, 0XC, 0X11, 0XA, 0XEE, 0X31, 0, 0, 0, 0, 0, 0X50, 0XA0, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XF, 0X7F, 0X7F, 0XF, 0X9A, 0X23, 0X8F, 0XFF, 0XC, 0XDF, 0XDF, 0XC, 0X11, 0XA, 0XEE, 0X31, 0, 0, 0, 0, 0, 0XA0, 0X40, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X1184, 0, 0, 0, 0, 0, 0, 0 },
      { 0X2, 0X811, 0XAD17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },

    { "640x480Over", 640,  480, TVTYPE_PAL,
      { 0X60, 0X7F, 0XE0, 0X8F, 0X31, 0X1B, 0X2D, 0X6E, 0X3, 0X81, 0XD, 0X14, 0X30, 0XA, 0XE7, 0XC4,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0},
      { 0X64, 0X4F, 0X4F, 0X88, 0X53, 0X83, 0XF2, 0X1F, 0, 0X40, 0, 0, 0, 0, 0, 0, 0XE5, 0, 0XDF, 0, 0, 0XDF, 0XF3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0X20, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X47, 0X7F, 0X7F, 0X47, 0X9A, 0X23, 0X95, 0XFF, 0XF3, 0XDF, 0XDF, 0XF3, 0X9, 0X9, 0XE5, 0X40, 0, 0, 0, 0, 0, 0X28, 0X50, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X47, 0X7F, 0X7F, 0X47, 0X9A, 0X23, 0X95, 0XFF, 0XF3, 0XDF, 0XDF, 0XF3, 0X9, 0X9, 0XE5, 0X40, 0, 0, 0, 0, 0, 0X50, 0XA0, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X47, 0X7F, 0X7F, 0X47, 0X9A, 0X23, 0X95, 0XFF, 0XF3, 0XDF, 0XDF, 0XF3, 0X9, 0X9, 0XE5, 0X40, 0, 0, 0, 0, 0, 0XA0, 0X40, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X3184, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "800x600Over", 800,  600, TVTYPE_NTSC,
      { 0XCE, 0X7F, 0X76, 0X8F, 0X51, 0X2E, 0X10, 0X83, 0X3, 0X81, 0X13, 0X3E, 0X1C,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0},
      { 0X7D, 0X63, 0X63, 0X81, 0X69, 0X18, 0XBA, 0XF0, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X5A, 0, 0X57, 0, 0, 0X57, 0XBB, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0XF, 0X1F, 0X1F, 0XF, 0XE3, 0X34, 0X44, 0XC6, 0XBB, 0X57, 0X57, 0XBB, 0X52, 0X12, 0X3F, 0X59, 0, 0, 0, 0, 0, 0X32, 0X64, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XF, 0X1F, 0X1F, 0XF, 0XE3, 0X34, 0X44, 0XC6, 0XBB, 0X57, 0X57, 0XBB, 0X52, 0X12, 0X3F, 0X59, 0, 0, 0, 0, 0, 0X64, 0XC8, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XF, 0X1F, 0X1F, 0XF, 0XE3, 0X34, 0X44, 0XC6, 0XBB, 0X57, 0X57, 0XBB, 0X52, 0X12, 0X3F, 0X59, 0, 0, 0, 0, 0, 0XC8, 0X90, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X5284, 0, 0, 0, 0, 0, 0, 0 },
      { 0X2, 0X811, 0XD017, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },

    { "800x600Over", 800,  600, TVTYPE_PAL,
      { 0XC1, 0X7F, 0XE0, 0X8F, 0X20, 0X1D, 0X36, 0X70, 0X3, 0X94, 0X39, 0X87, 0X26, 0X79, 0X8C, 0XC,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0},
      { 0X71, 0X63, 0X63, 0X95, 0X67, 0X90, 0X6F, 0XF0, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X57, 0, 0X57, 0, 0, 0X57, 0X70, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0X20, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0XAF, 0X1F, 0X1F, 0XAF, 0XDB, 0X33, 0X35, 0X8E, 0X70, 0X57, 0X57, 0X70, 0X52, 0X12, 0X57, 0X5A, 0, 0, 0, 0, 0, 0X32, 0X64, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XAF, 0X1F, 0X1F, 0XAF, 0XDB, 0X33, 0X35, 0X8E, 0X70, 0X57, 0X57, 0X70, 0X52, 0X12, 0X57, 0X5A, 0, 0, 0, 0, 0, 0X64, 0XC8, 0X40, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XAF, 0X1F, 0X1F, 0XAF, 0XDB, 0X33, 0X35, 0X8E, 0X70, 0X57, 0X57, 0X70, 0X52, 0X12, 0X57, 0X5A, 0, 0, 0, 0, 0, 0XC8, 0X90, 0X81, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X2184, 0, 0, 0, 0, 0, 0, 0 },
    },

    { "1024x768Over", 1024,  768, TVTYPE_NTSC,
      { 0XED, 0X7F, 0X7E, 0X87, 0X49, 0X20, 0, 0X83, 0X3, 0X90, 0X89, 0X35, 0X1F, 0X61, 0X1A, 0X7C,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0},
      { 0X8C, 0X7F, 0X7F, 0X90, 0X81, 0X8, 0X46, 0XF5, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X4, 0, 0XFF, 0, 0, 0XFF, 0X47, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X87, 0XFF, 0XFF, 0X87, 0X23, 0X34, 0X9, 0X38, 0X47, 0XFF, 0XFF, 0X47, 0X9A, 0X13, 0X4, 0X6F, 0, 0, 0, 0, 0, 0X40, 0X80, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X87, 0XFF, 0XFF, 0X87, 0X23, 0X34, 0X9, 0X38, 0X47, 0XFF, 0XFF, 0X47, 0X9A, 0X13, 0X4, 0X6F, 0, 0, 0, 0, 0, 0X80, 0, 0X41, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X87, 0XFF, 0XFF, 0X87, 0X23, 0X34, 0X9, 0X38, 0X47, 0XFF, 0XFF, 0X47, 0X9A, 0X13, 0X4, 0X6F, 0, 0, 0, 0, 0, 0, 0, 0X86, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X5084, 0, 0, 0, 0, 0, 0, 0 },
      { 0X2, 0X811, 0X4517, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },

    { "1024x768Over", 1024,  768, TVTYPE_PAL,
      { 0XE4, 0X7F, 0XA0, 0X8F, 0XB1, 0X28, 0X37, 0X70, 0, 0X81, 0X10, 0X4C, 0X25, 0XF, 0XBA, 0X1B, 0X1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0X48, 0X40, 0XD2, 0X80,
        0X40, 0, 0},
      { 0XAA, 0X7F, 0X7F, 0X8E, 0X84, 0X97, 0X69, 0XF5, 0, 0X60, 0, 0, 0, 0, 0, 0, 0X7, 0, 0XFF, 0, 0, 0XFF, 0X6A, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0X40, 0X80, 0, 0X47, 0X1C, 0, 0 },
      { 0, 0, 0, 0X47, 0X1C, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XB7, 0X6A, 0XFF, 0XFF, 0X6A, 0X9A, 0X13, 0X7, 0X77, 0, 0, 0, 0, 0, 0X40, 0X80, 0, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XB7, 0X6A, 0XFF, 0XFF, 0X6A, 0X9A, 0X13, 0X7, 0X77, 0, 0, 0, 0, 0, 0X80, 0, 0X41, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0X77, 0XFF, 0XFF, 0X77, 0X2B, 0X35, 0X1B, 0XB7, 0X6A, 0XFF, 0XFF, 0X6A, 0X9A, 0X13, 0X7, 0X77, 0, 0, 0, 0, 0, 0, 0, 0X86, 0, 0, 0X80, 0X20, 0X90, 0, 0, 0 },
      { 0XB184, 0, 0, 0, 0, 0, 0, 0 },
    }
};

static const VIABIOSTVMASKTableRec ch7011MaskTable = {
    { 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
      0XFF, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0XFF, 0XFF, 0XFF, 0XFF,
      0XFF, 0XFF, 0XFF },
    { 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,    0, 0XFF,    0,    0,    0,    0,    0,    0,
      0XFF,    0, 0XFF,    0,    0, 0XFF, 0XFF,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
    { 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
      0,    0,    0,    0,    0, 0XFF, 0XFF, 0XFF,    0,    0, 0XFF, 0XFF, 0XFF,    0,    0,    0 },
    0X3F, 0X38,24,13,22
};

static const VIABIOSTVMASKTableRec ch7019MaskTable = {
    { 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
      0XFF,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 0XFF, 0XFF, 0XFF, 0XFF,
      0XFF, 0XFF, 0XFF },
    { 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,    0, 0XFF,    0,    0,    0,    0,    0,    0,
      0XFF,    0, 0XFF,    0,    0, 0XFF, 0XFF,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
    { 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
         0,    0,    0,    0,    0, 0XFF, 0XFF, 0XFF,    0,    0, 0XFF, 0XFF, 0XFF,    0,    0,    0 },
    0X3F, 0X38,24,13,22
};

#endif
