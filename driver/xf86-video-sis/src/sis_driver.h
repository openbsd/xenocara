/*
 * Global data and definitions
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
 * Author:     	Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

/* VESA */
/*     The following is included because there are BIOSes out there that
 *     report incomplete mode lists. These are 630 BIOS versions <2.01.2x
 *     -) VBE 3.0 on SiS300 and 315 series do not support 24 fpp modes
 *     -) Only SiS315 series support 1920x1440x32
 */

static const UShort VESAModeIndices[] = {
   /*   x    y     8      16    (24)    32   */
       320, 200, 0x138, 0x10e, 0x000, 0x000,
       320, 240, 0x132, 0x135, 0x000, 0x000,
       400, 300, 0x133, 0x136, 0x000, 0x000,
       512, 384, 0x134, 0x137, 0x000, 0x000,
       640, 400, 0x100, 0x139, 0x000, 0x000,
       640, 480, 0x101, 0x111, 0x000, 0x13a,
       800, 600, 0x103, 0x114, 0x000, 0x13b,
      1024, 768, 0x105, 0x117, 0x000, 0x13c,
      1280,1024, 0x107, 0x11a, 0x000, 0x13d,
      1600,1200, 0x130, 0x131, 0x000, 0x13e,
      1920,1440, 0x13f, 0x140, 0x000, 0x141,
      9999,9999, 0,     0,     0,     0
};

/* For calculating refresh rate index (CR33) */
static const struct _sis_vrate {
    CARD16 idx;
    CARD16 xres;
    CARD16 yres;
    CARD16 refresh;
    Bool SiS730valid32bpp;
} sisx_vrate[] = {
	{1,  320,  200,  60,  TRUE}, {1,  320,  200,  70,  TRUE},
	{1,  320,  240,  60,  TRUE},
	{1,  400,  300,  60,  TRUE},
        {1,  512,  384,  60,  TRUE},
	{1,  640,  400,  60,  TRUE}, {1,  640,  400,  72,  TRUE},
	{1,  640,  480,  60,  TRUE}, {2,  640,  480,  72,  TRUE}, {3,  640,  480,  75,  TRUE},
	{4,  640,  480,  85,  TRUE}, {5,  640,  480, 100,  TRUE}, {6,  640,  480, 120,  TRUE},
	{7,  640,  480, 160, FALSE}, {8,  640,  480, 200, FALSE},
	{1,  720,  480,  60,  TRUE},
	{1,  720,  576,  60,  TRUE},
	{1,  768,  576,  60,  TRUE},
	{1,  800,  480,  60,  TRUE}, {2,  800,  480,  75,  TRUE}, {3,  800,  480,  85,  TRUE},
	{1,  800,  600,  56,  TRUE}, {2,  800,  600,  60,  TRUE}, {3,  800,  600,  72,  TRUE},
	{4,  800,  600,  75,  TRUE}, {5,  800,  600,  85,  TRUE}, {6,  800,  600, 105,  TRUE},
	{7,  800,  600, 120, FALSE}, {8,  800,  600, 160, FALSE},
	{1,  848,  480,  39,  TRUE}, {2,  848,  480,  60,  TRUE},
	{1,  856,  480,  39,  TRUE}, {2,  856,  480,  60,  TRUE},
	{1,  960,  540,  60,  TRUE},
	{1,  960,  600,  60,  TRUE},
	{1, 1024,  576,  60,  TRUE}, {2, 1024,  576,  75,  TRUE}, {3, 1024,  576,  85,  TRUE},
	{1, 1024,  600,  60,  TRUE},
	{1, 1024,  768,  43,  TRUE}, {2, 1024,  768,  60,  TRUE}, {3, 1024,  768,  70, FALSE},
	{4, 1024,  768,  75, FALSE}, {5, 1024,  768,  85,  TRUE}, {6, 1024,  768, 100,  TRUE},
	{7, 1024,  768, 120,  TRUE},
	{1, 1152,  768,  60,  TRUE},
	{1, 1152,  864,  60,  TRUE}, {2, 1152,  864,  75,  TRUE}, {3, 1152,  864,  84, FALSE},
	{1, 1280,  720,  60,  TRUE}, {2, 1280,  720,  75, FALSE}, {3, 1280,  720,  85,  TRUE},
	{1, 1280,  768,  60,  TRUE}, {2, 1280,  768,  75,  TRUE}, {3, 1280,  768,  85,  TRUE},
	{1, 1280,  800,  60,  TRUE}, {2, 1280,  800,  75,  TRUE}, {3, 1280,  800,  85,  TRUE},
	{1, 1280,  854,  60,  TRUE}, {2, 1280,  854,  75,  TRUE}, {3, 1280,  854,  85,  TRUE},
	{1, 1280,  960,  60,  TRUE}, {2, 1280,  960,  85,  TRUE},
	{1, 1280, 1024,  43, FALSE}, {2, 1280, 1024,  60,  TRUE}, {3, 1280, 1024,  75, FALSE},
	{4, 1280, 1024,  85,  TRUE},
	{1, 1360,  768,  60,  TRUE},
	{1, 1400, 1050,  60,  TRUE}, {2, 1400, 1050,  75,  TRUE},
	{1, 1600, 1200,  60,  TRUE}, {2, 1600, 1200,  65,  TRUE}, {3, 1600, 1200,  70,  TRUE},
	{4, 1600, 1200,  75,  TRUE}, {5, 1600, 1200,  85,  TRUE}, {6, 1600, 1200, 100,  TRUE},
	{7, 1600, 1200, 120,  TRUE},
	{1, 1680, 1050,  60,  TRUE},
	{1, 1920, 1080,  30,  TRUE},
	{1, 1920, 1440,  60,  TRUE}, {2, 1920, 1440,  65,  TRUE}, {3, 1920, 1440,  70,  TRUE},
	{4, 1920, 1440,  75,  TRUE}, {5, 1920, 1440,  85,  TRUE}, {6, 1920, 1440, 100,  TRUE},
	{1, 2048, 1536,  60,  TRUE}, {2, 2048, 1536,  65,  TRUE}, {3, 2048, 1536,  70,  TRUE},
	{4, 2048, 1536,  75,  TRUE}, {5, 2048, 1536,  85,  TRUE},
	{0,    0,    0,   0, FALSE}
};

/*     Some 300-series laptops have a badly designed BIOS and make it
 *     impossible to detect the correct panel delay compensation. This
 *     table used to detect such machines by their PCI subsystem IDs;
 *     however, I don't know how reliable this method is. (With Asus
 *     machines, it is to general, ASUS uses the same ID for different
 *     boxes)
 */
static const pdctable mypdctable[] = {
        { 0x1071, 0x7522, 32, "Mitac", "7521T" },
	{ 0,      0,       0, ""     , ""      }
};

/*     These machines require setting/clearing a GPIO bit for enabling/
 *     disabling communication with the Chrontel TV encoder
 */
static const chswtable mychswtable[] = {
        { 0x1631, 0x1002, "Mitachi", "0x1002" },
	{ 0x1071, 0x7521, "Mitac"  , "7521P"  },
	{ 0,      0,      ""       , ""       }
};

/*     These machines require special timing/handling
 */
const customttable SiS_customttable[] = {
        { SIS_630, "2.00.07", "09/27/2002-13:38:25",
	  0x3240A8,
	  { 0x220, 0x227, 0x228, 0x229, 0x0ee },
	  {  0x01,  0xe3,  0x9a,  0x6a,  0xef },
	  0x1039, 0x6300,
	  "Barco", "iQ R200L/300/400", CUT_BARCO1366, "BARCO_1366"
	},
	{ SIS_630, "2.00.07", "09/27/2002-13:38:25",
	  0x323FBD,
	  { 0x220, 0x227, 0x228, 0x229, 0x0ee },
	  {  0x00,  0x5a,  0x64,  0x41,  0xef },
	  0x1039, 0x6300,
	  "Barco", "iQ G200L/300/400/500", CUT_BARCO1024, "BARCO_1024"
	},
	{ SIS_650, "", "",
	  0,
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0x0e11, 0x083c,
	  "Inventec (Compaq)", "3017cl/3045US", CUT_COMPAQ12802, "COMPAQ_1280"
	},
	{ SIS_650, "", "",
	  0,	/* Special 1024x768 / dual link */
	  { 0x00c, 0, 0, 0, 0 },
	  { 'e'  , 0, 0, 0, 0 },
	  0x1558, 0x0287,
	  "Clevo", "L285/L287 (Version 1)", CUT_CLEVO1024, "CLEVO_L28X_1"
	},
	{ SIS_650, "", "",
	  0,	/* Special 1024x768 / single link */
	  { 0x00c, 0, 0, 0, 0 },
	  { 'y'  , 0, 0, 0, 0 },
	  0x1558, 0x0287,
	  "Clevo", "L285/L287 (Version 2)", CUT_CLEVO10242, "CLEVO_L28X_2"
	},
	{ SIS_650, "", "",
	  0,	/* Special 1400x1050 */
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0x1558, 0x0400,  /* possibly 401 and 402 as well; not panelsize specific? */
	  "Clevo", "D400S/D410S/D400H/D410H", CUT_CLEVO1400, "CLEVO_D4X0"
	},
	{ SIS_650, "", "",
	  0,	/* Shift LCD in LCD-via-CRT1 mode */
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0x1558, 0x2263,
	  "Clevo", "D22ES/D27ES", CUT_UNIWILL1024, "CLEVO_D2X0ES"
	},
	{ SIS_650, "", "",
	  0,	/* Shift LCD in LCD-via-CRT1 mode */
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0x1734, 0x101f,
	  "Uniwill", "N243S9", CUT_UNIWILL1024, "UNIWILL_N243S9"
	},
	{ SIS_650, "", "",
	  0,	/* Shift LCD in LCD-via-CRT1 mode */
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0x1584, 0x5103,
	  "Uniwill", "N35BS1", CUT_UNIWILL10242, "UNIWILL_N35BS1"
	},
	{ SIS_650, "1.09.2c", "",  /* Other versions, too? */
	  0,	/* Shift LCD in LCD-via-CRT1 mode */
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0x1019, 0x0f05,
	  "ECS", "A928", CUT_UNIWILL1024, "ECS_A928"
	},
	{ SIS_740, "1.11.27a", "",
	  0,
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0x1043, 0x1612,
	  "Asus", "L3000D/L3500D", CUT_ASUSL3000D, "ASUS_L3X00"
	},
	{ SIS_650, "1.10.9k", "",
	  0,	/* For EMI */
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0x1025, 0x0028,
	  "Acer", "Aspire 1700", CUT_ACER1280, "ACER_ASPIRE1700"
	},
	{ SIS_650, "1.10.7w", "",
	  0,	/* For EMI */
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0x14c0, 0x0012,
	  "Compal", "??? (V1)", CUT_COMPAL1400_1, "COMPAL_1400_1"
	},
	{ SIS_650, "1.10.7x", "", /* New BIOS on its way (from BG.) */
	  0,	/* For EMI */
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0x14c0, 0x0012,
	  "Compal", "??? (V2)", CUT_COMPAL1400_2, "COMPAL_1400_2"
	},
	{ SIS_650, "1.10.8o", "",
	  0,	/* For EMI (unknown) */
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0x1043, 0x1612,
	  "Asus", "A2H (V1)", CUT_ASUSA2H_1, "ASUS_A2H_1"
	},
	{ SIS_650, "1.10.8q", "",
	  0,	/* For EMI */
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0x1043, 0x1612,
	  "Asus", "A2H (V2)", CUT_ASUSA2H_2, "ASUS_A2H_2"
	},
#if 0
	{ SIS_550, "1.02.0z", "",
	  0x317f37,	/* 320x240 LCD panel */
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0, 0,
	  "AAEON", "AOP-8060", CUT_AOP8060, "AAEON_AOP_8060"
	},
#endif
	{ 4321, "", "",			/* never autodetected */
	  0,
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0, 0,
	  "Generic", "LVDS/Parallel 848x480", CUT_PANEL848, "PANEL848x480"
	},
	{ 4322, "", "",			/* never autodetected */
	  0,
	  { 0, 0, 0, 0, 0 },
	  { 0, 0, 0, 0, 0 },
	  0, 0,
	  "Generic", "LVDS/Parallel 856x480", CUT_PANEL856, "PANEL856x480"
	},
	{ 0, "", "",
	  0,
	  { 0, 0, 0, 0 },
	  { 0, 0, 0, 0 },
	  0, 0,
	  "", "", CUT_NONE, ""
	}
};

/*     Our TV modes for the 6326. The data in these structures
 *     is mainly correct, but since we use our private CR and
 *     clock values anyway, small errors do no matter.
 */
static DisplayModeRec SiS6326PAL800x600Mode = {
	NULL, NULL,     /* prev, next */
	"PAL800x600",   /* identifier of this mode */
	MODE_OK,        /* mode status */
	M_T_BUILTIN,    /* mode type */
	36000,		/* Clock frequency */
	800,		/* HDisplay */
	848,		/* HSyncStart */
	912,		/* HSyncEnd */
	1008,		/* HTotal */
	0,		/* HSkew */
	600,		/* VDisplay */
	600,		/* VSyncStart */
	602,		/* VSyncEnd */
	625,		/* VTotal */
	0,		/* VScan */
	V_PHSYNC | V_PVSYNC,	/* Flags */
	-1,		/* ClockIndex */
	36000,		/* SynthClock */
	800,		/* CRTC HDisplay */
	808,            /* CRTC HBlankStart */
	848,            /* CRTC HSyncStart */
	912,            /* CRTC HSyncEnd */
	1008,           /* CRTC HBlankEnd */
	1008,           /* CRTC HTotal */
	0,              /* CRTC HSkew */
	600,		/* CRTC VDisplay */
	600,		/* CRTC VBlankStart */
	600,		/* CRTC VSyncStart */
	602,		/* CRTC VSyncEnd */
	625,		/* CRTC VBlankEnd */
	625,		/* CRTC VTotal */
	FALSE,		/* CrtcHAdjusted */
	FALSE,		/* CrtcVAdjusted */
	0,		/* PrivSize */
	NULL,		/* Private */
	0.0,		/* HSync */
	0.0		/* VRefresh */
};

/*     Due to the scaling method this mode uses, the vertical data here
 *     does not match the CR data. But this does not matter, we use our
 *     private CR data anyway.
 */
static DisplayModeRec SiS6326PAL800x600UMode = {
	NULL,           /* prev */
	&SiS6326PAL800x600Mode, /* next */
	"PAL800x600U",  /* identifier of this mode */
	MODE_OK,        /* mode status */
	M_T_BUILTIN,    /* mode type */
	37120,		/* Clock frequency */
	800,		/* HDisplay */
	872,		/* HSyncStart */
	984,		/* HSyncEnd */
	1088,		/* HTotal */
	0,		/* HSkew */
	600,		/* VDisplay (548 due to scaling) */
	600,		/* VSyncStart (584) */
	602,		/* VSyncEnd (586) */
	625,		/* VTotal */
	0,		/* VScan */
	V_PHSYNC | V_PVSYNC,	/* Flags */
	-1,		/* ClockIndex */
	37120,		/* SynthClock */
	800,		/* CRTC HDisplay */
	808,            /* CRTC HBlankStart */
	872,            /* CRTC HSyncStart */
	984,            /* CRTC HSyncEnd */
	1024,           /* CRTC HBlankEnd */
	1088,           /* CRTC HTotal */
	0,              /* CRTC HSkew */
	600,		/* CRTC VDisplay (548 due to scaling) */
	600,		/* CRTC VBlankStart (600) */
	600,		/* CRTC VSyncStart (584) */
	602,		/* CRTC VSyncEnd (586) */
	625,		/* CRTC VBlankEnd */
	625,		/* CRTC VTotal */
	FALSE,		/* CrtcHAdjusted */
	FALSE,		/* CrtcVAdjusted */
	0,		/* PrivSize */
	NULL,		/* Private */
	0.0,		/* HSync */
	0.0		/* VRefresh */
};

static DisplayModeRec SiS6326PAL720x540Mode = {
	NULL,      	/* prev */
	&SiS6326PAL800x600UMode, /* next */
	"PAL720x540",   /* identifier of this mode */
	MODE_OK,        /* mode status */
	M_T_BUILTIN,    /* mode type */
	36000,		/* Clock frequency */
	720,		/* HDisplay */
	816,		/* HSyncStart */
	920,		/* HSyncEnd */
	1008,		/* HTotal */
	0,		/* HSkew */
	540,		/* VDisplay */
	578,		/* VSyncStart */
	580,		/* VSyncEnd */
	625,		/* VTotal */
	0,		/* VScan */
	V_PHSYNC | V_PVSYNC,	/* Flags */
	-1,		/* ClockIndex */
	36000,		/* SynthClock */
	720,		/* CRTC HDisplay */
	736,            /* CRTC HBlankStart */
	816,            /* CRTC HSyncStart */
	920,            /* CRTC HSyncEnd */
	1008,           /* CRTC HBlankEnd */
	1008,           /* CRTC HTotal */
	0,              /* CRTC HSkew */
	540,		/* CRTC VDisplay */
	577,		/* CRTC VBlankStart */
	578,		/* CRTC VSyncStart */
	580,		/* CRTC VSyncEnd */
	625,		/* CRTC VBlankEnd */
	625,		/* CRTC VTotal */
	FALSE,		/* CrtcHAdjusted */
	FALSE,		/* CrtcVAdjusted */
	0,		/* PrivSize */
	NULL,		/* Private */
	0.0,		/* HSync */
	0.0		/* VRefresh */
};

static DisplayModeRec SiS6326PAL640x480Mode = {
	NULL,      	/* prev */
	&SiS6326PAL720x540Mode, /* next */
	"PAL640x480",   /* identifier of this mode */
	MODE_OK,        /* mode status */
	M_T_BUILTIN,    /* mode type */
	36000,		/* Clock frequency */
	640,		/* HDisplay */
	768,		/* HSyncStart */
	920,		/* HSyncEnd */
	1008,		/* HTotal */
	0,		/* HSkew */
	480,		/* VDisplay */
	532,		/* VSyncStart */
	534,		/* VSyncEnd */
	625,		/* VTotal */
	0,		/* VScan */
	V_NHSYNC | V_NVSYNC,	/* Flags */
	-1,		/* ClockIndex */
	36000,		/* SynthClock */
	640,		/* CRTC HDisplay */
	648,            /* CRTC HBlankStart */
	768,            /* CRTC HSyncStart */
	920,            /* CRTC HSyncEnd */
	944,            /* CRTC HBlankEnd */
	1008,           /* CRTC HTotal */
	0,              /* CRTC HSkew */
	480,		/* CRTC VDisplay */
	481,		/* CRTC VBlankStart */
	532,		/* CRTC VSyncStart */
	534,		/* CRTC VSyncEnd */
	561,		/* CRTC VBlankEnd */
	625,		/* CRTC VTotal */
	FALSE,		/* CrtcHAdjusted */
	FALSE,		/* CrtcVAdjusted */
	0,		/* PrivSize */
	NULL,		/* Private */
	0.0,		/* HSync */
	0.0		/* VRefresh */
};

static DisplayModeRec SiS6326NTSC640x480Mode = {
	NULL, NULL,	/* prev, next */
	"NTSC640x480",  /* identifier of this mode */
	MODE_OK,        /* mode status */
	M_T_BUILTIN,    /* mode type */
	27000,		/* Clock frequency */
	640,		/* HDisplay */
	664,		/* HSyncStart */
	760,		/* HSyncEnd */
	800,		/* HTotal */
	0,		/* HSkew */
	480,		/* VDisplay */
	489,		/* VSyncStart */
	491,		/* VSyncEnd */
	525,		/* VTotal */
	0,		/* VScan */
	V_NHSYNC | V_NVSYNC,	/* Flags */
	-1,		/* ClockIndex */
	27000,		/* SynthClock */
	640,		/* CRTC HDisplay */
	648,            /* CRTC HBlankStart */
	664,            /* CRTC HSyncStart */
	760,            /* CRTC HSyncEnd */
	792,            /* CRTC HBlankEnd */
	800,            /* CRTC HTotal */
	0,              /* CRTC HSkew */
	480,		/* CRTC VDisplay */
	488,		/* CRTC VBlankStart */
	489,		/* CRTC VSyncStart */
	491,		/* CRTC VSyncEnd */
	517,		/* CRTC VBlankEnd */
	525,		/* CRTC VTotal */
	FALSE,		/* CrtcHAdjusted */
	FALSE,		/* CrtcVAdjusted */
	0,		/* PrivSize */
	NULL,		/* Private */
	0.0,		/* HSync */
	0.0		/* VRefresh */
};

/*     Due to the scaling method this mode uses, the vertical data here
 *     does not match the CR data. But this does not matter, we use our
 *     private CR data anyway.
 */
static DisplayModeRec SiS6326NTSC640x480UMode = {
	NULL, 		/* prev */
	&SiS6326NTSC640x480Mode, /* next */
	"NTSC640x480U", /* identifier of this mode */
	MODE_OK,        /* mode status */
	M_T_BUILTIN,    /* mode type */
	32215,		/* Clock frequency */
	640,		/* HDisplay */
	696,		/* HSyncStart */
	840,		/* HSyncEnd */
	856,		/* HTotal */
	0,		/* HSkew */
	480,		/* VDisplay (439 due to scaling) */
	489,		/* VSyncStart (473) */
	491,		/* VSyncEnd (475) */
	525,		/* VTotal */
	0,		/* VScan */
	V_NHSYNC | V_NVSYNC,	/* Flags */
	-1,		/* ClockIndex */
	32215,		/* SynthClock */
	640,		/* CRTC HDisplay */
	656,            /* CRTC HBlankStart */
	696,            /* CRTC HSyncStart */
	840,            /* CRTC HSyncEnd */
	856,            /* CRTC HBlankEnd */
	856,            /* CRTC HTotal */
	0,              /* CRTC HSkew */
	480,		/* CRTC VDisplay */
	488,		/* CRTC VBlankStart */
	489,		/* CRTC VSyncStart */
	491,		/* CRTC VSyncEnd */
	517,		/* CRTC VBlankEnd */
	525,		/* CRTC VTotal */
	FALSE,		/* CrtcHAdjusted */
	FALSE,		/* CrtcVAdjusted */
	0,		/* PrivSize */
	NULL,		/* Private */
	0.0,		/* HSync */
	0.0		/* VRefresh */
};


static DisplayModeRec SiS6326NTSC640x400Mode = {
	NULL, 	     	/* prev */
	&SiS6326NTSC640x480UMode, /* next */
	"NTSC640x400",  /* identifier of this mode */
	MODE_OK,        /* mode status */
	M_T_BUILTIN,    /* mode type */
	27000,		/* Clock frequency */
	640,		/* HDisplay */
	664,		/* HSyncStart */
	760,		/* HSyncEnd */
	800,		/* HTotal */
	0,		/* HSkew */
	400,		/* VDisplay */
	459,		/* VSyncStart */
	461,		/* VSyncEnd */
	525,		/* VTotal */
	0,		/* VScan */
	V_NHSYNC | V_NVSYNC,	/* Flags */
	-1,		/* ClockIndex */
	27000,		/* SynthClock */
	640,		/* CRTC HDisplay */
	648,            /* CRTC HBlankStart */
	664,            /* CRTC HSyncStart */
	760,            /* CRTC HSyncEnd */
	792,            /* CRTC HBlankEnd */
	800,            /* CRTC HTotal */
	0,              /* CRTC HSkew */
	400,		/* CRTC VDisplay */
	407,		/* CRTC VBlankStart */
	459,		/* CRTC VSyncStart */
	461,		/* CRTC VSyncEnd */
	490,		/* CRTC VBlankEnd */
	525,		/* CRTC VTotal */
	FALSE,		/* CrtcHAdjusted */
	FALSE,		/* CrtcVAdjusted */
	0,		/* PrivSize */
	NULL,		/* Private */
	0.0,		/* HSync */
	0.0		/* VRefresh */
};

/*     Built-in hi-res modes for the 6326.
 *     For some reason, our default mode lines and the
 *     clock calculation functions in sis_dac.c do no
 *     good job on higher clocks. It seems, the hardware
 *     needs some tricks so make mode with higher clock
 *     rates than ca. 120MHz work. I didn't bother trying
 *     to find out what exactly is going wrong, so I
 *     implemented two special modes instead for 1280x1024
 *     and 1600x1200. These two are automatically added
 *     to the list if they are supported with the current
 *     depth.
 *     The data in the strucures below is a proximation,
 *     in sis_vga.c the register contents are fetched from
 *     fixed tables anyway.
 */
static DisplayModeRec SiS6326SIS1280x1024_75Mode = {
	NULL, 	       	/* prev */
	NULL,           /* next */
	"SIS1280x1024-75",  /* identifier of this mode */
	MODE_OK,        /* mode status */
	M_T_BUILTIN,    /* mode type */
	135000,		/* Clock frequency */
	1280,		/* HDisplay */
	1296,		/* HSyncStart */
	1440,		/* HSyncEnd */
	1688,		/* HTotal */
	0,		/* HSkew */
	1024,		/* VDisplay */
	1025,		/* VSyncStart */
	1028,		/* VSyncEnd */
	1066,		/* VTotal */
	0,		/* VScan */
	V_PHSYNC | V_PVSYNC,	/* Flags */
	-1,		/* ClockIndex */
	135000,		/* SynthClock */
	1280,		/* CRTC HDisplay */
	1280,           /* CRTC HBlankStart */
	1296,           /* CRTC HSyncStart */
	1440,           /* CRTC HSyncEnd */
	1680,           /* CRTC HBlankEnd */
	1688,           /* CRTC HTotal */
	0,              /* CRTC HSkew */
	1024,		/* CRTC VDisplay */
	1024,		/* CRTC VBlankStart */
	1025,		/* CRTC VSyncStart */
	1028,		/* CRTC VSyncEnd */
	1065,		/* CRTC VBlankEnd */
	1066,		/* CRTC VTotal */
	FALSE,		/* CrtcHAdjusted */
	FALSE,		/* CrtcVAdjusted */
	0,		/* PrivSize */
	NULL,		/* Private */
	0.0,		/* HSync */
	0.0		/* VRefresh */
};

static DisplayModeRec SiS6326SIS1600x1200_60Mode = {
	NULL, 	       	/* prev */
	NULL,           /* next */
	"SIS1600x1200-60",  /* identifier of this mode */
	MODE_OK,        /* mode status */
	M_T_BUILTIN,    /* mode type */
	162000,		/* Clock frequency */
	1600,		/* HDisplay */
	1664,		/* HSyncStart */
	1856,		/* HSyncEnd */
	2160,		/* HTotal */
	0,		/* HSkew */
	1200,		/* VDisplay */
	1201,		/* VSyncStart */
	1204,		/* VSyncEnd */
	1250,		/* VTotal */
	0,		/* VScan */
	V_PHSYNC | V_PVSYNC,	/* Flags */
	-1,		/* ClockIndex */
	162000,		/* SynthClock */
	1600,		/* CRTC HDisplay */
	1600,           /* CRTC HBlankStart */
	1664,           /* CRTC HSyncStart */
	1856,           /* CRTC HSyncEnd */
	2152,            /* CRTC HBlankEnd */
	2160,            /* CRTC HTotal */
	0,              /* CRTC HSkew */
	1200,		/* CRTC VDisplay */
	1200,		/* CRTC VBlankStart */
	1201,		/* CRTC VSyncStart */
	1204,		/* CRTC VSyncEnd */
	1249,		/* CRTC VBlankEnd */
	1250,		/* CRTC VTotal */
	FALSE,		/* CrtcHAdjusted */
	FALSE,		/* CrtcVAdjusted */
	0,		/* PrivSize */
	NULL,		/* Private */
	0.0,		/* HSync */
	0.0		/* VRefresh */
};

/*     TV scaling data for SiS video bridges
 */
typedef struct _SiSTVVScale {
        UShort ScaleVDE;
	int sindex;
	UShort RealVDE;
	UShort reg[4];
} MySiSTVVScale, *MySiSTVVScalePtr;

static const MySiSTVVScale SiSTVVScale[] = {
	{ 470, 3, 480,			/* NTSC 640x480 */
	  { 893, 508, 0x004c, 0x008f }
	},
	{ 460, 2, 480,
	  { 874, 513, 0x004c, 0x008f }
	},
	{ 450, 1, 480,
	  { 855, 518, 0x004c, 0x008f }
	},
	{ 440, 0, 480,	/* default */
	  { 836, 523, 0x004c, 0x008f }
	},
	{ 430, -1, 480,
	  { 860, 528, 0x0050, 0x008f }
	},
	{ 420, -2, 480,
	  { 840, 533, 0x0050, 0x008f }
	},
	{ 410, -3, 480,
	  { 820, 538, 0x0050, 0x008f }
	},
	{ 470, 3, 480,			/* NTSC 720x480 */
	  { 893, 509, 0x004c, 0x008f }
	},
	{ 460, 2, 480,
	  { 874, 514, 0x004c, 0x008f }
	},
	{ 450, 1, 480,
	  { 855, 519, 0x004c, 0x008f }
	},
	{ 440, 0, 480,	/* default */
	  { 836, 524, 0x004c, 0x008f }
	},
	{ 430, -1, 480,
	  { 860, 529, 0x0050, 0x008f }
	},
	{ 420, -2, 480,
	  { 840, 534, 0x0050, 0x008f }
	},
	{ 410, -3, 480,
	  { 820, 539, 0x0050, 0x008f }
	},
	{ 470, 3, 600,			/* NTSC 800x600 */
	  { 1081, 628, 0x0073, 0x008f }
	},
	{ 460, 2, 600,
	  { 1058, 633, 0x0073, 0x008f }
	},
	{ 450, 1, 600,
	  { 1044, 638, 0x0074, 0x008f }
	},
	{ 440, 0, 600,	/* default */
	  { 1056, 643, 0x0078, 0x008f }
	},
	{ 430, -1, 600,
	  { 1032, 648, 0x0078, 0x008f }
	},
	{ 420, -2, 600,
	  { 1008, 653, 0x0078, 0x008f }
	},
	{ 410, -3, 600,
	  { 1066, 658, 0x0082, 0x008f }
	},
	{ 560, 3, 480,			/* PAL 640x480 */
	  { 882, 513, 0x0007, 0x0010 }
	},
	{ 550, 2, 480,
	  { 900, 518, 0x0005, 0x000b }
	},
	{ 540, 1, 480,
	  { 864, 523, 0x0004, 0x0009 }
	},
	{ 530, 0, 480,	/* default */
	  { 848, 528, 0x0004, 0x0009 }
	},
	{ 520, -1, 480,
	  { 832, 533, 0x0004, 0x0009 }
	},
	{ 510, -2, 480,
	  { 918, 538, 0x0001, 0x0002 }
	},
	{ 500, -3, 480,
	  { 900, 543, 0x0001, 0x0002 }
	},
	{ 560, 2, 576,			/* PAL 720x576 */
	  { 960, 610, 0x0004, 0x0007 }
	},
	{ 550, 1, 576,
	  { 990, 614, 0x0003, 0x0005 }
	},
	{ 540, 0, 576,	/* default */
	  { 1080, 620, 0x0002, 0x0003 }
	},
	{ 530, -1, 576,
	  { 1060, 625, 0x8002, 0x0003 }
	},
	{ 520, -2, 576,
	  { 1040, 630, 0x0002, 0x0003 }
	},
	{ 510, -3, 576,
	  { 1020, 635, 0x0002, 0x0003 }
	},
	{ 500, -4, 576,
	  { 1000, 640, 0x0002, 0x0003 }
	},
	{ 560, 3, 600,			/* PAL 800x600 */
	  { 1152, 633, 0x0005, 0x0007 }
	},
	{ 550, 2, 600,
	  { 1100, 638, 0x0019, 0x0024 }
	},
	{ 540, 1, 600,
	  { 1080, 643, 0x0019, 0x0024 }
	},
	{ 530, 0, 600,	/* default */
	  { 1060, 648, 0x0019, 0x0024 }
	},
	{ 520, -1, 600,
	  { 1040, 653, 0x0019, 0x0024 }
	},
	{ 510, -2, 600,
	  { 1020, 658, 0x0019, 0x0024 }
	},
	{ 500, -3, 600,
	  { 1080, 663, 0x0003, 0x0004 }
	},
	{ 720, 3, 480,			/* 750p 640x480 (42) */
          { 1238, 500, 0x0001, 0x0002 }
        },
        { 693, 2, 480,
          { 1191, 519, 0x0001, 0x0002 }
        },
        { 667, 1, 480,
          { 1146, 540, 0x0001, 0x0002 }
        },
        { 640, 0, 480,
          { 1100, 563, 0x0001, 0x0002 }
        },
        { 613, -1, 480,
          { 1054, 587, 0x0001, 0x0002 }
        },
        { 587, -2, 480,
          { 1009, 613, 0x0001, 0x0002 }
        },
        { 560, -3, 480,
          { 963, 643, 0x0001, 0x0002 }
        },
	{ 720, 3, 480,			/* 750p 720x480 (49) */
          { 1238, 500, 0x0001, 0x0002 }
        },
        { 693, 2, 480,
          { 1191, 519, 0x0001, 0x0002 }
        },
        { 667, 1, 480,
          { 1146, 540, 0x0001, 0x0002 }
        },
        { 640, 0, 480,
          { 1100, 563, 0x0001, 0x0002 }
        },
        { 613, -1, 480,
          { 1054, 587, 0x0001, 0x0002 }
        },
        { 587, -2, 480,
          { 1009, 613, 0x0001, 0x0002 }
        },
        { 560, -3, 480,
          { 963, 643, 0x0001, 0x0002 }
        },
	{ 720, 3, 576,			/* 750p 720/768x576 (56) */
          { 1238, 600, 0x0003, 0x0005 }
        },
        { 693, 2, 576,
          { 1191, 623, 0x0003, 0x0005 }
        },
        { 667, 1, 576,
          { 1146, 648, 0x0003, 0x0005 }
        },
        { 640, 0, 576,
          { 1100, 675, 0x0003, 0x0005 }
        },
        { 613, -1, 576,
          { 1054, 705, 0x0003, 0x0005 }
        },
        { 587, -2, 576,
          { 1009, 736, 0x0003, 0x0005 }
        },
        { 560, -3, 576,
          { 963, 771, 0x0003, 0x0005 }
        },
	{ 720, 3, 480,			/* 750p 800x480 (63) */
          { 1238, 500, 0x0001, 0x0002 }
        },
        { 693, 2, 480,
          { 1191, 519, 0x0001, 0x0002 }
        },
        { 667, 1, 480,
          { 1146, 540, 0x0001, 0x0002 }
        },
        { 640, 0, 480,
          { 1100, 563, 0x0001, 0x0002 }
        },
        { 613, -1, 480,
          { 1054, 587, 0x0001, 0x0002 }
        },
        { 587, -2, 480,
          { 1009, 613, 0x0001, 0x0002 }
        },
        { 560, -3, 480,
          { 963, 643, 0x0001, 0x0002 }
        },
	{ 720, 3, 600,			/* 750p 800x600 (70) */
          { 1320, 625, 0x0002, 0x0003 }
        },
        { 700, 2, 600,
          { 1283, 643, 0x0002, 0x0003 }
        },
        { 680, 1, 600,
          { 1247, 662, 0x0002, 0x0003 }
        },
        { 660, 0, 600,
          { 1210, 682, 0x0002, 0x0003 }
        },
        { 640, -1, 600,
          { 1173, 703, 0x0002, 0x0003 }
        },
        { 620, -2, 600,
          { 1137, 726, 0x0002, 0x0003 }
        },
        { 600, -3, 600,
          { 1100, 750, 0x0002, 0x0003 }
        },
	{ 720, 3, 576,			/* 750p 1024x576 (77) */
          { 1238, 600, 0x0003, 0x0005 }
        },
        { 693, 2, 576,
          { 1191, 623, 0x0003, 0x0005 }
        },
        { 667, 1, 576,
          { 1146, 648, 0x0003, 0x0005 }
        },
        { 640, 0, 576,
          { 1100, 675, 0x0003, 0x0005 }
        },
	{ 630, -1, 576,
          { 1083, 686, 0x0003, 0x0005 }
	},
	{ 620, -2, 576,
          { 1066, 697, 0x0003, 0x0005 }
        },
	{ 616, -3, 576,
          { 1059, 701, 0x0003, 0x0005 }
        },
	{ 720, 3, 768,			/* 750p 1024x768 (84) */
          { 1547, 800, 0x0001, 0x0001 }
        },
        { 693, 2, 768,
          { 1489, 831, 0x0001, 0x0001 }
        },
        { 667, 1, 768,
          { 1433, 864, 0x0001, 0x0001 }
        },
        { 640, 0, 768,
          { 1375, 900, 0x0001, 0x0001 }
        },
        { 613, -1, 768,
          { 1317, 940, 0x0001, 0x0001 }
        },
        { 587, -2, 768,
          { 1261, 981, 0x0001, 0x0001 }
        },
        { 560, -3, 768,
          { 1203, 1029, 0x0001, 0x0001 }
        },
#ifdef OLD1280720P
	{ 720, 3, 720,			/* 750p 1280x720-old (91) */
          { 1584, 750, 0x0018, 0x0019 }
        },
        { 707, 2, 720,
          { 1555, 764, 0x0018, 0x0019 }
        },
        { 693, 1, 720,
          { 1525, 779, 0x0018, 0x0019 }
        },
        { 680, 0, 720,
          { 1496, 794, 0x0018, 0x0019 }
        },
        { 667, -1, 720,
          { 1467, 810, 0x0018, 0x0019 }
        },
        { 653, -2, 720,
          { 1437, 827, 0x0018, 0x0019 }
        },
        { 640, -3, 720,
          { 1408, 844, 0x0018, 0x0019 }
        },
#endif
#ifndef OLD1280720P
	{ 720, 3, 720,			/* 750p 1280x720-new (91) */
	  { 1650, 750, 0x0001, 0x0001 }
	},
	{ 720, 2, 720,
	  { 1650, 750, 0x0001, 0x0001 }
	},
	{ 720, 1, 720,
	  { 1650, 750, 0x0001, 0x0001 }
	},
	{ 720, 0, 720,
	  { 1650, 750, 0x0001, 0x0001 }
	},
	{ 704, -1, 720,
          { 1613, 767, 0x0001, 0x0001 }
        },
        { 688, -2, 720,
          { 1577, 785, 0x0001, 0x0001 }
        },
        { 672, -3, 720,
          { 1540, 804, 0x0001, 0x0001 }
        },
#endif
	{ 1080, 3, 480,			/* 1080i 640x480 (98) */
          { 945, 500, 0x8001, 0x0005 }
        },
        { 1040, 2, 480,
          { 910, 519, 0x8001, 0x0005 }
        },
        { 1000, 1, 480,
          { 875, 540, 0x8001, 0x0005 }
        },
        { 960, 0, 480,
          { 840, 563, 0x8001, 0x0005 }
        },
        { 920, -1, 480,
          { 805, 587, 0x8001, 0x0005 }
        },
        { 880, -2, 480,
          { 770, 614, 0x8001, 0x0005 }
        },
        { 840, -3, 480,
          { 735, 643, 0x8001, 0x0005 }
        },
	{ 1080, 3, 480,			/* 1080i 800x480 (105) */
          { 1181, 500, 0x8001, 0x0004 }
        },
        { 1040, 2, 480,
          { 1138, 519, 0x8001, 0x0004 }
        },
        { 1000, 1, 480,
          { 1094, 540, 0x8001, 0x0004 }
        },
        { 960, 0, 480,
          { 1050, 563, 0x8001, 0x0004 }
        },
        { 920, -1, 480,
          { 1006, 587, 0x8001, 0x0004 }
        },
        { 880, -2, 480,
          { 963, 614, 0x8001, 0x0004 }
        },
        { 840, -3, 480,
          { 919, 643, 0x8001, 0x0004 }
        },
	{ 1080, 3, 600,			/* 1080i 800x600 (112) */
          { 1181, 625, 0x8005, 0x0010 }
        },
        { 1040, 2, 600,
          { 1138, 649, 0x8005, 0x0010 }
        },
        { 1000, 1, 600,
          { 1094, 675, 0x8005, 0x0010 }
        },
        { 960, 0, 600,
          { 1050, 703, 0x8005, 0x0010 }
        },
        { 920, -1, 600,
          { 1006, 734, 0x8005, 0x0010 }
        },
        { 880, -2, 600,
          { 963, 767, 0x8005, 0x0010 }
        },
        { 840, -3, 600,
          { 919, 804, 0x8005, 0x0010 }
        },
	{ 1080, 3, 576,			/* 1080i 1024x576 (119) */
          { 1575, 600, 0x0002, 0x0005 }
        },
        { 1040, 2, 576,
          { 1517, 623, 0x0002, 0x0005 }
        },
        { 1000, 1, 576,
          { 1458, 648, 0x0002, 0x0005 }
        },
        { 960, 0, 576,
          { 1400, 675, 0x0002, 0x0005 }
        },
        { 920, -1, 576,
          { 1342, 704, 0x0002, 0x0005 }
        },
        { 880, -2, 576,
          { 1283, 736, 0x0002, 0x0005 }
        },
        { 840, -3, 576,
          { 1225, 771, 0x0002, 0x0005 }
        },
	{ 1080, 3, 768,			/* 1080i 1024x768 (126) */
          { 1418, 800, 0x000c, 0x0019 }
        },
        { 1040, 2, 768,
          { 1365, 831, 0x000c, 0x0019 }
        },
        { 1000, 1, 768,
          { 1313, 864, 0x000c, 0x0019 }
        },
        { 960, 0, 768,
          { 1260, 900, 0x000c, 0x0019 }
        },
        { 920, -1, 768,
          { 1208, 939, 0x000c, 0x0019 }
        },
        { 880, -2, 768,
          { 1155, 982, 0x000c, 0x0019 }
        },
        { 840, -3, 768,
          { 1103, 1029, 0x000c, 0x0019 }
        },
	{ 1080, 3, 720,			/* 1080i 1280x720 (133) */
          { 1969, 750, 0x0005, 0x0008 }
        },
        { 1040, 2, 720,
          { 1896, 779, 0x0005, 0x0008 }
        },
        { 1000, 1, 720,
          { 1823, 810, 0x0005, 0x0008 }
        },
        { 960, 0, 720,
          { 1750, 844, 0x0005, 0x0008 }
        },
        { 920, -1, 720,
          { 1677, 880, 0x0005, 0x0008 }
        },
        { 880, -2, 720,
          { 1604, 920, 0x0005, 0x0008 }
        },
        { 840, -3, 720,
          { 1531, 964, 0x0005, 0x0008 }
        },
	{ 1080, 3, 1024,		/* 1080i 1280x1024 (140) */
          { 1772, 1067, 0x0004, 0x0005 }
        },
        { 1040, 2, 1024,
          { 1706, 1108, 0x0004, 0x0005 }
        },
        { 1000, 1, 1024,
          { 1641, 1152, 0x0004, 0x0005 }
        },
        { 960, 0, 1024,
          { 1575, 1200, 0x0004, 0x0005 }
        },
        { 920, -1, 1024,
          { 1509, 1252, 0x0004, 0x0005 }
        },
        { 880, -2, 1024,
          { 1444, 1309, 0x0004, 0x0005 }
        },
        { 840, -3, 1024,
          { 1378, 1371, 0x0004, 0x0005 }
        },
	{ 470, 3, 576,			/* NTSC 720x576 (147) */
          { 1175, 602, 0x8078, 0x008f }
        },
        { 460, 2, 576,
          { 1150, 614, 0x8078, 0x008f }
        },
        { 450, 1, 576,
          { 1125, 628, 0x8078, 0x008f }
        },
        { 440, 0, 576,
          { 1100, 643, 0x8078, 0x008f }
        },
        { 430, -1, 576,
          { 1075, 658, 0x8078, 0x008f }
        },
        { 420, -2, 576,
          { 1050, 673, 0x8078, 0x008f }
        },
        { 410, -3, 576,
          { 1025, 680, 0x8078, 0x008f }
        },
	{ 550, 3, 768,			/* PAL 1024x768 (154) */
          { 1238, 776, 0x0001, 0x0001 }
        },
        { 540, 2, 768,
          { 1215, 790, 0x0001, 0x0001 }
        },
        { 530, 1, 768,
          { 1193, 805, 0x0001, 0x0001 }
        },
        { 520, 0, 768,
          { 1170, 821, 0x0001, 0x0001 }
        },
        { 510, -1, 768,
          { 1148, 837, 0x0001, 0x0001 }
        },
        { 500, -2, 768,
          { 1125, 853, 0x0001, 0x0001 }
        },
        { 490, -3, 768,
          { 1103, 871, 0x0001, 0x0001 }
        },
	{ 470, 3, 768,			/* NTSC 1024 i (161) */
          { 1175, 759, 0x8001, 0x0001 }
        },
        { 460, 2, 768,
          { 1150, 775, 0x8001, 0x0001 }
        },
        { 450, 1, 768,
          { 1125, 792, 0x8001, 0x0001 }
        },
        { 440, 0, 768,
          { 1100, 811, 0x8001, 0x0001 }
        },
        { 430, -1, 768,
          { 1075, 829, 0x8001, 0x0001 }
        },
        { 430, -2, 768,
          { 1075, 829, 0x8001, 0x0001 }
        },
        { 430, -3, 768,
          { 1075, 829, 0x8001, 0x0001 }
        },
	{ 470, 3, 768,			/* NTSC 1024 p (168) */
          { 1175, 792, 0x0001, 0x0001 }
        },
        { 460, 2, 768,
          { 1150, 809, 0x0001, 0x0001 }
        },
        { 450, 1, 768,
          { 1125, 827, 0x0001, 0x0001 }
        },
        { 440, 0, 768,
          { 1100, 846, 0x0001, 0x0001 }
        },
        { 430, -1, 768,
          { 1075, 865, 0x0001, 0x0001 }
        },
        { 430, -2, 768,
          { 1075, 865, 0x0001, 0x0001 }
        },
        { 430, -3, 768,
          { 1075, 865, 0x0001, 0x0001 }
        },
	{ 470, 3, 480,			/* NTSC 800x480 (175) */
	  { 893, 509, 0x004c, 0x008f }
	},
	{ 460, 2, 480,
	  { 874, 514, 0x004c, 0x008f }
	},
	{ 450, 1, 480,
	  { 855, 519, 0x004c, 0x008f }
	},
	{ 440, 0, 480,	/* default */
	  { 836, 524, 0x004c, 0x008f }
	},
	{ 430, -1, 480,
	  { 860, 529, 0x0050, 0x008f }
	},
	{ 420, -2, 480,
	  { 840, 534, 0x0050, 0x008f }
	},
	{ 420, -3, 480,
	  { 840, 534, 0x0050, 0x008f }
	},
	{ 470, 3, 576,			/* NTSC 1024x576 (182) */
          { 1175, 602, 0x8078, 0x008f }
        },
        { 460, 2, 576,
          { 1150, 614, 0x8078, 0x008f }
        },
        { 450, 1, 576,
          { 1125, 628, 0x8078, 0x008f }
        },
        { 440, 0, 576,
          { 1100, 643, 0x8078, 0x008f }
        },
        { 430, -1, 576,
          { 1075, 658, 0x8078, 0x008f }
        },
        { 430, -2, 576,
          { 1075, 658, 0x8078, 0x008f }
        },
        { 430, -3, 576,
          { 1075, 658, 0x8078, 0x008f }
        },
	{ 564, 3, 576,			/* PAL 1024x576 (189) */
          { 1128, 592, 0x0002, 0x0003 }
        },
        { 556, 2, 576,
          { 1112, 601, 0x0002, 0x0003 }
        },
        { 548, 1, 576,
          { 1096, 610, 0x0002, 0x0003 }
        },
        { 540, 0, 576,
          { 1080, 619, 0x0002, 0x0003 }
        },
        { 532, -1, 576,
          { 1064, 628, 0x0002, 0x0003 }
        },
        { 532, -2, 576,
          { 1064, 628, 0x0002, 0x0003 }
        },
        { 532, -3, 576,
          { 1064, 628, 0x0002, 0x0003 }
        },
	{ 1080, 3, 540,			/* 1080i 960x540 (196) */
          { 1050, 600, 0x0001, 0x0004 }
        },
        { 1080, 2, 540,
          { 1050, 600, 0x0001, 0x0004 }
        },
        { 1080, 1, 540,
          { 1050, 600, 0x0001, 0x0004 }
        },
        { 1080, 0, 540,
          { 1050, 600, 0x0001, 0x0004 }
        },
        { 1040, -1, 540,
          { 1011, 623, 0x0001, 0x0004 }
        },
        { 1000, -2, 540,
          { 1944, 648, 0x0001, 0x0002 }
        },
        { 960, -3, 540,
          { 1866, 675, 0x0001, 0x0002 }
        },
	{ 1080, 3, 600,			/* 1080i 960x600 (203) */
          { 1418, 670, 0x0003, 0x0008 }
        },
        { 1040, 2, 600,
          { 1365, 700, 0x0003, 0x0008 }
        },
        { 1000, 1, 600,
          { 1313, 816, 0x0003, 0x0008 }
        },
        { 960, 0, 600,
          { 1260, 851, 0x0003, 0x0008 }
        },
        { 920, -1, 600,
          { 1208, 887, 0x0003, 0x0008 }
        },
        { 880, -2, 600,
          { 1155, 928, 0x0003, 0x0008 }
        },
        { 840, -3, 600,
          { 1103, 972, 0x0003, 0x0008 }
        }
};

static unsigned const char SiSScalingP1Regs[] = {
	0x08,0x09,0x0b,0x0c,0x0d,0x0e,0x10,0x11,0x12
};
static unsigned const char SiSScalingP4Regs[] = {
	0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b
};

/*     TV filters for SiS video bridges
 */
static const struct _SiSTVFilter301 {
	UChar filter[7][4];
} SiSTVFilter301[] = {
	{{ {0x00,0xE0,0x10,0x60},   /* NTSCFilter - 320 */
	   {0x00,0xEE,0x10,0x44},
	   {0x00,0xF4,0x10,0x38},
	   {0xF8,0xF4,0x18,0x38},
	   {0xFC,0xFB,0x14,0x2A},
	   {0x00,0x00,0x10,0x20},
	   {0x00,0x04,0x10,0x18} }},
	{{ {0xF5,0xEE,0x1B,0x44},   /* NTSCFilter - 640 */
	   {0xF8,0xF4,0x18,0x38},
	   {0xEB,0x04,0x25,0x18},
	   {0xF1,0x05,0x1F,0x16},
	   {0xF6,0x06,0x1A,0x14},
	   {0xFA,0x06,0x16,0x14},
	   {0x00,0x04,0x10,0x18} }},
	{{ {0xEB,0x04,0x25,0x18},   /* NTSCFilter - 720 */
	   {0xE7,0x0E,0x29,0x04},
	   {0xEE,0x0C,0x22,0x08},
	   {0xF6,0x0B,0x1A,0x0A},
	   {0xF9,0x0A,0x17,0x0C},
	   {0xFC,0x0A,0x14,0x0C},
	   {0x00,0x08,0x10,0x10} }},
	{{ {0xEC,0x02,0x24,0x1C},   /* NTSCFilter - 800/400 */
	   {0xF2,0x04,0x1E,0x18},
	   {0xEB,0x15,0x25,0xF6},
	   {0xF4,0x10,0x1C,0x00},
	   {0xF8,0x0F,0x18,0x02},
	   {0x00,0x04,0x10,0x18},
	   {0x01,0x06,0x0F,0x14} }},
	{{ {0x00,0xE0,0x10,0x60},   /* PALFilter - 320 */
	   {0x00,0xEE,0x10,0x44},
	   {0x00,0xF4,0x10,0x38},
	   {0xF8,0xF4,0x18,0x38},
	   {0xFC,0xFB,0x14,0x2A},
	   {0x00,0x00,0x10,0x20},
	   {0x00,0x04,0x10,0x18} }},
	{{ {0xF5,0xEE,0x1B,0x44},   /* PALFilter - 640 */
	   {0xF8,0xF4,0x18,0x38},
	   {0xF1,0xF7,0x1F,0x32},
	   {0xF5,0xFB,0x1B,0x2A},
	   {0xF9,0xFF,0x17,0x22},
	   {0xFB,0x01,0x15,0x1E},
	   {0x00,0x04,0x10,0x18} }},
	{{ {0xF5,0xEE,0x1B,0x2A},   /* PALFilter - 720 */
	   {0xEE,0xFE,0x22,0x24},
	   {0xF3,0x00,0x1D,0x20},
	   {0xF9,0x03,0x17,0x1A},
	   {0xFB,0x02,0x14,0x1E},
	   {0xFB,0x04,0x15,0x18},
	   {0x00,0x06,0x10,0x14} }},
	{{ {0xF5,0xEE,0x1B,0x44},   /* PALFilter - 800/400 */
	   {0xF8,0xF4,0x18,0x38},
	   {0xFC,0xFB,0x14,0x2A},
	   {0xEB,0x05,0x25,0x16},
	   {0xF1,0x05,0x1F,0x16},
	   {0xFA,0x07,0x16,0x12},
	   {0x00,0x07,0x10,0x12} }}
};

static const struct _SiSTVFilter301B {
	UChar filter[7][7];
} SiSTVFilter301B[] = {
	{{ {0x01,0x02,0xfb,0xf8,0x06,0x27,0x3a},   /* NTSC - 640 */
	   {0x01,0x02,0xfe,0xf7,0x03,0x27,0x3c},
	   {0x01,0x01,0x00,0xf6,0x00,0x28,0x40},
	   {0xff,0x03,0x02,0xf6,0xfc,0x27,0x46},
	   {0xff,0x01,0x04,0xf8,0xfa,0x27,0x46},
	   {0xff,0x01,0x05,0xf9,0xf7,0x26,0x4a},
	   {0xff,0xff,0x05,0xfc,0xf4,0x24,0x52} }},
	{{ {0x01,0x00,0xfb,0xfb,0x0b,0x25,0x32},   /* NTSC - 720 (?) */
	   {0x01,0x01,0xfb,0xf9,0x09,0x26,0x36},
	   {0x01,0x02,0xfc,0xf8,0x06,0x27,0x38},
	   {0x01,0x02,0xfe,0xf7,0x03,0x27,0x3c},
	   {0x01,0x03,0xff,0xf6,0x00,0x27,0x40},
	   {0xff,0x03,0x02,0xf6,0xfe,0x27,0x42},
	   {0xff,0x02,0x03,0xf7,0xfb,0x27,0x46} }},
	{{ {0x01,0xfe,0xfb,0xfe,0x0e,0x23,0x2e},   /* NTSC - 800 */
	   {0x01,0xff,0xfb,0xfc,0x0c,0x25,0x30},
	   {0x01,0x00,0xfb,0xfa,0x0a,0x26,0x34},
	   {0x01,0x01,0xfc,0xf8,0x08,0x26,0x38},
	   {0x01,0x02,0xfd,0xf7,0x06,0x27,0x38},
	   {0x01,0x02,0xfe,0xf7,0x03,0x27,0x3c},
	   {0xff,0x03,0x00,0xf6,0x00,0x27,0x42} }},
	{{ {0xff,0xfd,0xfe,0x05,0x11,0x1e,0x24},   /* NTSC - 1024 */
	   {0xff,0xfd,0xfd,0x04,0x11,0x1f,0x26},
	   {0xff,0xfd,0xfc,0x02,0x10,0x22,0x28},
	   {0xff,0xff,0xfc,0x00,0x0f,0x22,0x28},
	   {0x01,0xfe,0xfb,0xff,0x0e,0x23,0x2c},
	   {0x01,0xff,0xfb,0xfd,0x0d,0x24,0x2e},
	   {0x01,0xff,0xfb,0xfb,0x0c,0x25,0x32} }},
	{{ {0x01,0x02,0xfb,0xf8,0x06,0x27,0x3a},   /* PAL - 640 */
	   {0x01,0x02,0xfe,0xf7,0x03,0x27,0x3c},
	   {0x01,0x01,0x00,0xf6,0x00,0x28,0x40},
	   {0xff,0x03,0x02,0xf6,0xfc,0x27,0x46},
	   {0xff,0x01,0x04,0xf8,0xfa,0x27,0x46},
	   {0xff,0x01,0x05,0xf9,0xf7,0x26,0x4a},
	   {0xff,0xff,0x05,0xfc,0xf4,0x24,0x52} }},
	{{ {0x01,0x00,0xfb,0xfb,0x0b,0x25,0x32},   /* PAL - 720/768 */
	   {0x01,0x01,0xfb,0xf9,0x09,0x26,0x36},
	   {0x01,0x02,0xfc,0xf8,0x06,0x27,0x38},
	   {0x01,0x02,0xfe,0xf7,0x03,0x27,0x3c},
	   {0x01,0x03,0xff,0xf6,0x00,0x27,0x40},
	   {0xff,0x03,0x02,0xf6,0xfe,0x27,0x42},
	   {0xff,0x02,0x03,0xf7,0xfb,0x27,0x46} }},
	{{ {0x01,0xfe,0xfb,0xfe,0x0e,0x23,0x2e},   /* PAL - 800 */
	   {0x01,0xff,0xfb,0xfc,0x0c,0x25,0x30},
	   {0x01,0x00,0xfb,0xfa,0x0a,0x26,0x34},
	   {0x01,0x01,0xfc,0xf8,0x08,0x26,0x38},
	   {0x01,0x02,0xfd,0xf7,0x06,0x27,0x38},
	   {0x01,0x02,0xfe,0xf7,0x03,0x27,0x3c},
	   {0xff,0x03,0x00,0xf6,0x00,0x27,0x42} }},
	{{ {0xff,0xfd,0xfe,0x05,0x11,0x1e,0x24},   /* PAL - 1024 */
	   {0xff,0xfd,0xfd,0x04,0x11,0x1f,0x26},
	   {0xff,0xfd,0xfc,0x02,0x10,0x22,0x28},
	   {0xff,0xff,0xfc,0x00,0x0f,0x22,0x28},
	   {0x01,0xfe,0xfb,0xff,0x0e,0x23,0x2c},
	   {0x01,0xff,0xfb,0xfd,0x0d,0x24,0x2e},
	   {0x01,0xff,0xfb,0xfb,0x0c,0x25,0x32} }},
	{{ {0x54,0x69,0x6c,0x6c,0x20,0x53,0x6f},   /* PAL-M - 1024 */
	   {0x66,0x69,0x61,0x20,0x42,0x65,0x72},
	   {0x6e,0x74,0x73,0x73,0x6f,0x6e,0x20},
	   {0x2d,0x20,0x42,0x72,0x6f,0x75,0x67},
	   {0x68,0x74,0x20,0x74,0x6f,0x20,0x79},
	   {0x6f,0x75,0x20,0x62,0x79,0x20,0x6e},
	   {0x6f,0x74,0x20,0x61,0x20,0x6d,0x65,} }},
	{{ {0x72,0x65,0x20,0x57,0x69,0x7a,0x61},   /* PAL-N - 1024 */
	   {0x72,0x64,0x20,0x62,0x75,0x74,0x20},
	   {0x74,0x68,0x65,0x20,0x57,0x69,0x7a},
	   {0x61,0x72,0x64,0x20,0x45,0x78,0x74},
	   {0x72,0x61,0x6f,0x72,0x64,0x69,0x6e},
	   {0x61,0x69,0x72,0x65,0x21,0x20,0x48},
	   {0x69,0x20,0x44,0x61,0x6c,0x65,0x21} }}
};

/* For communication with the SiS Linux framebuffer driver (sisfb) */

/* ioctl for identifying and giving some info (esp. memory heap start) */
#define SISFB_GET_INFO_SIZE	0x8004f300
#define SISFB_GET_INFO		0x8000f301  /* Must be patched with result from ..._SIZE at D[29:16] */
/* deprecated ioctl number (for older versions of sisfb) */
#define SISFB_GET_INFO_OLD	0x80046ef8

/* ioctls for tv parameters (position) */
#define SISFB_SET_TVPOSOFFSET	0x4004f304

/* lock sisfb from register access */
#define SISFB_SET_LOCK		0x4004f306

/* Magic value for USB device */
#ifndef SISFB_USB_MAGIC
#define SISFB_USB_MAGIC		0x55aa2011
#endif

/* Structure argument for SISFB_GET_INFO ioctl  */
typedef struct _SISFB_INFO sisfb_info, *psisfb_info;

struct _SISFB_INFO {
	CARD32 	sisfb_id;		/* for identifying sisfb */
#ifndef SISFB_ID
#define SISFB_ID	  0x53495346	/* Identify myself with 'SISF' */
#endif
 	CARD32 	chip_id;		/* PCI ID of detected chip */
	CARD32	memory;			/* video memory in KB which sisfb manages */
	CARD32	heapstart;		/* heap start (= sisfb "mem" argument) in KB */
	CARD8 	fbvidmode;		/* current sisfb mode */

	CARD8 	sisfb_version;
	CARD8	sisfb_revision;
	CARD8 	sisfb_patchlevel;

	CARD8 	sisfb_caps;		/* sisfb's capabilities */

	CARD32 	sisfb_tqlen;		/* turbo queue length (in KB) */

	CARD32 	sisfb_pcibus;		/* The card's PCI bus ID. For USB, bus = SISFB_USB_MAGIC */
	CARD32 	sisfb_pcislot;		/* alias usbbus */
	CARD32 	sisfb_pcifunc;		/* alias usbdev */

	CARD8 	sisfb_lcdpdc;

	CARD8	sisfb_lcda;

	CARD32	sisfb_vbflags;
	CARD32	sisfb_currentvbflags;

	CARD32 	sisfb_scalelcd;
	CARD32 	sisfb_specialtiming;

	CARD8 	sisfb_haveemi;
	CARD8 	sisfb_emi30,sisfb_emi31,sisfb_emi32,sisfb_emi33;
	CARD8 	sisfb_haveemilcd;

	CARD8 	sisfb_lcdpdca;

	CARD16  sisfb_tvxpos, sisfb_tvypos;	/* Warning: Values + 32 ! */

	CARD32	sisfb_heapsize;			/* heap size (in KB) */
	CARD32	sisfb_videooffset;		/* Offset of viewport in video memory (in bytes) */

	CARD32	sisfb_curfstn;			/* currently running FSTN/DSTN mode */
	CARD32	sisfb_curdstn;

	CARD16	sisfb_pci_vendor;		/* PCI vendor (SiS or XGI) */

	CARD32	sisfb_vbflags2;

	CARD8	sisfb_can_post;			/* sisfb can POST this card */
	CARD8	sisfb_card_posted;		/* card is POSTED */
	CARD8	sisfb_was_boot_device;		/* This card was the boot video device (ie is primary) */

	CARD8 reserved[183];			/* for future use */
};

/* Mandatory functions */
static void SISIdentify(int flags);
static Bool SISProbe(DriverPtr drv, int flags);
static Bool SISPreInit(ScrnInfoPtr pScrn, int flags);
static Bool SISScreenInit(int Index, ScreenPtr pScreen, int argc, char **argv);
static Bool SISEnterVT(int scrnIndex, int flags);
static void SISLeaveVT(int scrnIndex, int flags);
static Bool SISCloseScreen(int scrnIndex, ScreenPtr pScreen);
static Bool SISSaveScreen(ScreenPtr pScreen, int mode);
static Bool SISSwitchMode(int scrnIndex, DisplayModePtr mode, int flags);
void	    SISAdjustFrame(int scrnIndex, int x, int y, int flags);

/* Optional functions */
#ifdef SISDUALHEAD
static Bool 	  SISSaveScreenDH(ScreenPtr pScreen, int mode);
#endif
static void       SISFreeScreen(int scrnIndex, int flags);
static ModeStatus SISValidMode(int scrnIndex, DisplayModePtr mode,
				Bool verbose, int flags);

/* Internally used functions */
static Bool    SISMapMem(ScrnInfoPtr pScrn);
static Bool    SISUnmapMem(ScrnInfoPtr pScrn);
#ifdef SIS_NEED_MAP_IOP
static Bool    SISMapIOPMem(ScrnInfoPtr pScrn);
static Bool    SISUnmapIOPMem(ScrnInfoPtr pScrn);
#endif
static void    SISSave(ScrnInfoPtr pScrn);
static void    SISRestore(ScrnInfoPtr pScrn);
static Bool    SISModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
static void    SISModifyModeInfo(DisplayModePtr mode);
static void    SiSPreSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode, int viewmode);
static void    SiSPostSetMode(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static void    SiS6326PostSetMode(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static Bool    SiSSetVESAMode(ScrnInfoPtr pScrn, DisplayModePtr pMode);
static void    SISVESARestore(ScrnInfoPtr pScrn);
static void    SiSBuildVesaModeList(ScrnInfoPtr pScrn, vbeInfoPtr pVbe, VbeInfoBlock *vbe);
static UShort  SiSCalcVESAModeIndex(ScrnInfoPtr pScrn, DisplayModePtr mode);
static void    SISVESASaveRestore(ScrnInfoPtr pScrn, vbeSaveRestoreFunction function);
static void    SISBridgeRestore(ScrnInfoPtr pScrn);
static void    SiSEnableTurboQueue(ScrnInfoPtr pScrn);
static void    SiSRestoreQueueMode(SISPtr pSiS, SISRegPtr sisReg);
UChar  	       SISSearchCRT1Rate(ScrnInfoPtr pScrn, DisplayModePtr mode);
static void    SISWaitVBRetrace(ScrnInfoPtr pScrn);
void           SISWaitRetraceCRT1(ScrnInfoPtr pScrn);
void           SISWaitRetraceCRT2(ScrnInfoPtr pScrn);
UShort         SiS_CheckModeCRT1(ScrnInfoPtr pScrn, DisplayModePtr mode,
				 unsigned int VBFlags, Bool hcm);
UShort         SiS_CheckModeCRT2(ScrnInfoPtr pScrn, DisplayModePtr mode,
				 unsigned int VBFlags, Bool hcm);

#ifdef SISMERGED
static Bool    InRegion(int x, int y, region r);
static void    SISMergedPointerMoved(int scrnIndex, int x, int y);
#endif
Bool           SiSBridgeIsInSlaveMode(ScrnInfoPtr pScrn);
UShort	       SiS_GetModeNumber(ScrnInfoPtr pScrn, DisplayModePtr mode, unsigned int VBFlags);
UChar  	       SiS_GetSetBIOSScratch(ScrnInfoPtr pScrn, UShort offset, UChar value);
#ifdef DEBUG
static void    SiSDumpModeInfo(ScrnInfoPtr pScrn, DisplayModePtr mode);
#endif
Bool           SISDetermineLCDACap(ScrnInfoPtr pScrn);
void           SISSaveDetectedDevices(ScrnInfoPtr pScrn);
#ifdef SISGAMMARAMP
void	       SISCalculateGammaRamp(ScreenPtr pScreen, ScrnInfoPtr pScrn);
#endif

/* Our very own vgaHW functions (sis_vga.c) */
extern void 	SiSVGASave(ScrnInfoPtr pScrn, SISRegPtr save, int flags);
extern void 	SiSVGARestore(ScrnInfoPtr pScrn, SISRegPtr restore, int flags);
extern void 	SiSVGASaveFonts(ScrnInfoPtr pScrn);
extern void 	SiSVGARestoreFonts(ScrnInfoPtr pScrn);
extern void 	SISVGALock(SISPtr pSiS);
extern void 	SiSVGAUnlock(SISPtr pSiS);
extern void 	SiSVGAProtect(ScrnInfoPtr pScrn, Bool on);
extern Bool 	SiSVGAMapMem(ScrnInfoPtr pScrn);
extern void 	SiSVGAUnmapMem(ScrnInfoPtr pScrn);
extern Bool 	SiSVGASaveScreen(ScreenPtr pScreen, int mode);

/* shadow */
extern void 	SISPointerMoved(int index, int x, int y);
extern void 	SISPointerMovedReflect(int index, int x, int y);
extern void 	SISRefreshArea(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
extern void 	SISRefreshAreaReflect(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
extern void 	SISRefreshArea8(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
extern void 	SISRefreshArea16(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
extern void 	SISRefreshArea24(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
extern void 	SISRefreshArea32(ScrnInfoPtr pScrn, int num, BoxPtr pbox);

/* vb */
extern void 	SISCRT1PreInit(ScrnInfoPtr pScrn);
extern void 	SISLCDPreInit(ScrnInfoPtr pScrn, Bool quiet);
extern void 	SISTVPreInit(ScrnInfoPtr pScrn, Bool quiet);
extern void 	SISCRT2PreInit(ScrnInfoPtr pScrn, Bool quiet);
extern void 	SISSense30x(ScrnInfoPtr pScrn, Bool quiet);
extern void 	SISSenseChrontel(ScrnInfoPtr pScrn, Bool quiet);
extern void     SiSSetupPseudoPanel(ScrnInfoPtr pScrn);

/* utility */
extern void	SiSCtrlExtInit(ScrnInfoPtr pScrn);
extern void	SiSCtrlExtUnregister(SISPtr pSiS, int index);

/* init.c, init301.c ----- (use their data types!) */
extern unsigned short	SiS_GetModeID(int VGAEngine, unsigned int VBFlags, int HDisplay, int VDisplay,
				int Depth, BOOLEAN FSTN, int LCDwith, int LCDheight);
extern unsigned short	SiS_GetModeID_LCD(int VGAEngine, unsigned int VBFlags, int HDisplay,
				int VDisplay, int Depth, BOOLEAN FSTN, unsigned short CustomT,
				int LCDwith, int LCDheight, unsigned int VBFlags2);
extern unsigned short	SiS_GetModeID_TV(int VGAEngine, unsigned int VBFlags, int HDisplay,
				int VDisplay, int Depth, unsigned int VBFlags2);
extern unsigned short	SiS_GetModeID_VGA2(int VGAEngine, unsigned int VBFlags, int HDisplay,
				int VDisplay, int Depth, unsigned int VBFlags2);
extern int		SiSTranslateToVESA(ScrnInfoPtr pScrn, int modenumber);
extern int		SiSTranslateToOldMode(int modenumber);
extern BOOLEAN		SiSDetermineROMLayout661(struct SiS_Private *SiS_Pr);
extern BOOLEAN		SiSBIOSSetMode(struct SiS_Private *SiS_Pr,
                               ScrnInfoPtr pScrn, DisplayModePtr mode, BOOLEAN IsCustom);
extern BOOLEAN		SiSSetMode(struct SiS_Private *SiS_Pr,
                           ScrnInfoPtr pScrn, unsigned short ModeNo, BOOLEAN dosetpitch);
extern BOOLEAN		SiSBIOSSetModeCRT1(struct SiS_Private *SiS_Pr,
				   ScrnInfoPtr pScrn, DisplayModePtr mode, BOOLEAN IsCustom);
extern BOOLEAN		SiSBIOSSetModeCRT2(struct SiS_Private *SiS_Pr,
				   ScrnInfoPtr pScrn, DisplayModePtr mode, BOOLEAN IsCustom);
extern DisplayModePtr	SiSBuildBuiltInModeList(ScrnInfoPtr pScrn, BOOLEAN includelcdmodes,
					      BOOLEAN isfordvi, BOOLEAN fakecrt2modes, BOOLEAN IsForCRT2);
extern void		SiS_Chrontel701xBLOn(struct SiS_Private *SiS_Pr);
extern void		SiS_Chrontel701xBLOff(struct SiS_Private *SiS_Pr);
extern void		SiS_SiS30xBLOn(struct SiS_Private *SiS_Pr);
extern void		SiS_SiS30xBLOff(struct SiS_Private *SiS_Pr);
extern void		SiS_CalcXTapScaler(struct SiS_Private *SiS_Pr, int srcsize, int destsize, int taps, Bool ishoriz);
/* End of init.c, init301.c ----- */






