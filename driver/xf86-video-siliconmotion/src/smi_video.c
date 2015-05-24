/*
Copyright (C) 1994-1999 The XFree86 Project, Inc.  All Rights Reserved.
Copyright (C) 2000,2008 Silicon Motion, Inc.  All Rights Reserved.
Copyright (C) 2001 Corvin Zahn.  All Rights Reserved.
Copyright (C) 2008 Mandriva Linux.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FIT-
NESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the names of the XFree86 Project and
Silicon Motion shall not be used in advertising or otherwise to promote the
sale, use or other dealings in this Software without prior written
authorization from the XFree86 Project and silicon Motion.
*/


/*
  Corvin Zahn <zahn@zac.de>	Date:   2.11.2001
    - SAA7111 support
    - supports attributes: XV_ENCODING, XV_BRIGHTNESS, XV_CONTRAST,
      XV_SATURATION, XV_HUE, XV_COLORKEY, XV_INTERLACED
      XV_CAPTURE_BRIGHTNESS can be used to set brightness in the capture device
    - bug fixes
    - tries not to use acceleration functions
    - interlaced video for double vertical resolution
	XV_INTERLACED = 0: only one field of an interlaced video signal is
			   displayed:
			-> half vertical resolution, but no comb like artifacts
			   from moving vertical edges
	XV_INTERLACED = 1: both fields of an interlaced video signal are
			   displayed:
			-> full vertical resolution, but comb like artifacts from
			   moving vertical edges
	The default value can be set with the driver option Interlaced
*/


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "smi.h"
#include "smi_video.h"

#include "xf86Crtc.h"

#undef MIN
#undef ABS
#undef CLAMP
#undef ENTRIES

#define MIN(a, b) (((a) < (b)) ? (a) : (b)) 
#define ABS(n) (((n) < 0) ? -(n) : (n))
#define CLAMP(v, min, max) (((v) < (min)) ? (min) : MIN(v, max))

#define ENTRIES(array) (sizeof(array) / sizeof((array)[0]))
#define nElems(x)		(sizeof(x) / sizeof(x[0]))

#define MAKE_ATOM(a)	MakeAtom(a, sizeof(a) - 1, TRUE)

#if SMI_USE_VIDEO
#include "dixstruct.h"


static int SMI_AddEncoding(XF86VideoEncodingPtr enc, int i,
			   int norm, int input, int channel);
static void SMI_BuildEncodings(SMI_PortPtr p);

static XF86VideoAdaptorPtr SMI_SetupVideo(ScreenPtr pScreen);
static void SMI_ResetVideo(ScrnInfoPtr pScrn);

#if SMI_USE_CAPTURE
static int SMI_PutVideo(ScrnInfoPtr pScrn,
		short vid_x, short vid_y, short drw_x, short drw_y,
		short vid_w, short vid_h, short drw_w, short drw_h,
		RegionPtr clipBoxes, pointer data, DrawablePtr);
#endif
static void SMI_StopVideo(ScrnInfoPtr pScrn, pointer data, Bool shutdown);
static int SMI_SetPortAttribute(ScrnInfoPtr pScrn, Atom attribute,
		INT32 value, pointer data);
static int SMI_GetPortAttribute(ScrnInfoPtr pScrn, Atom attribute,
		INT32 *value, pointer data);
static void SMI_QueryBestSize(ScrnInfoPtr pScrn, Bool motion,
		short vid_w, short vid_h, short drw_w, short drw_h,
		unsigned int *p_w, unsigned int *p_h, pointer data);
static int SMI_PutImage(ScrnInfoPtr pScrn,
		short src_x, short src_y, short drw_x, short drw_y,
		short src_w, short src_h, short drw_w, short drw_h,
		int id, unsigned char *buf, short width, short height, Bool sync,
		RegionPtr clipBoxes, pointer data, DrawablePtr);
static int SMI_QueryImageAttributes(ScrnInfoPtr pScrn,
		int id, unsigned short *width, unsigned short *height,
		int *picthes, int *offsets);

static void SMI_DisplayVideo(ScrnInfoPtr pScrn, int id, int offset,
		short width, short height, int pitch, int x1, int y1, int x2, int y2,
		BoxPtr dstBox, short vid_w, short vid_h, short drw_w, short drw_h);
static void SMI_DisplayVideo0501_CSC(ScrnInfoPtr pScrn, int id, int offset,
				     short width, short height, int pitch,
				     int x1, int y1, int x2, int y2,
				     BoxPtr dstBox, short vid_w, short vid_h,
				     short drw_w, short drw_h,
				     RegionPtr clipboxes);
static void SMI_DisplayVideo0501(ScrnInfoPtr pScrn, int id, int offset,
				 short width, short height, int pitch,
				 int x1, int y1, int x2, int y2,
				 BoxPtr dstBox, short vid_w, short vid_h,
				 short drw_w, short drw_h);
static void SMI_DisplayVideo0730(ScrnInfoPtr pScrn, int id, int offset,
		short width, short height, int pitch, int x1, int y1, int x2, int y2,
		BoxPtr dstBox, short vid_w, short vid_h, short drw_w, short drw_h);
static void SMI_BlockHandler(BLOCKHANDLER_ARGS_DECL);
/*static int SMI_SendI2C(ScrnInfoPtr pScrn, CARD8 device, char *devName,
        SMI_I2CDataPtr i2cData);*/

static void SMI_InitOffscreenImages(ScreenPtr pScreen);
static void SMI_VideoSave(ScreenPtr pScreen, ExaOffscreenArea *area);

static void CopyYV12ToVideoMem(unsigned char *src1, unsigned char *src2,
			       unsigned char *src3, unsigned char *dst,
			       int src1Pitch, int src23Pitch, int dstPitch,
			       int height, int width);
static int SMI_AllocSurface(ScrnInfoPtr pScrn,
		int id, unsigned short width, unsigned short height,
		XF86SurfacePtr surface);
static int SMI_FreeSurface(XF86SurfacePtr surface);
static int SMI_DisplaySurface(XF86SurfacePtr surface,
		short vid_x, short vid_y, short drw_x, short drw_y,
		short vid_w, short vid_h, short drw_w, short drw_h,
		RegionPtr clipBoxes);
static int SMI_StopSurface(XF86SurfacePtr surface);
static int SMI_GetSurfaceAttribute(ScrnInfoPtr pScrn, Atom attr, INT32 *value);
static int SMI_SetSurfaceAttribute(ScrnInfoPtr pScrn, Atom attr, INT32 value);

static int SetAttr(ScrnInfoPtr pScrn, int i, int value);
static int SetAttrSAA7110(ScrnInfoPtr pScrn, int i, int value);
static int SetAttrSAA7111(ScrnInfoPtr pScrn, int i, int value);
static void SetKeyReg(SMIPtr pSmi, int reg, int value);

/**
 * Atoms
 */

static Atom xvColorKey;
static Atom xvEncoding;
static Atom xvBrightness,xvCapBrightness, xvContrast, xvSaturation, xvHue;
static Atom xvInterlaced;


/******************************************************************************\
**																			  **
**                           C A P A B I L I T I E S                          **
**																			  **
\******************************************************************************/


/**************************************************************************/
/* input channels */

#define N_COMPOSITE_CHANNELS 4
#define N_SVIDEO_CHANNELS 2

#define N_VIDEO_INPUTS 2
typedef enum _VideoInput { VID_COMPOSITE, VID_SVIDEO } VideoInput;


/**************************************************************************/
/* video input formats */

typedef struct _VideoInputDataRec {
    char* name;
} VideoInputDataRec;

static VideoInputDataRec VideoInputs[] = {
    { "composite" },
    { "svideo" }
};


/**************************************************************************/
/* video norms */

#define N_VIDEO_NORMS 3
typedef enum _VideoNorm { PAL, NTSC, SECAM } VideoNorm;

typedef struct _VideoNormDataRec {
    char* name;
    unsigned long Wt;
    unsigned long Wa;
    unsigned long Ht;
    unsigned long Ha;
    unsigned long HStart;
    unsigned long VStart;
    XvRationalRec rate;
} VideoNormDataRec;


static VideoNormDataRec VideoNorms[] =
{
    /* PAL-BDGHI */
    {"pal", 864, 704, 625, 576, 16, 16, { 1, 50 }},
    /* NTSC */
    {"ntsc", 858, 704, 525, 480, 21, 8, { 1001, 60000 }},
    /* SECAM (not tested) */
    {"secam", 864, 7040, 625, 576, 31, 16, { 1, 50 }},
};


/**************************************************************************/
/* number of (generated) XV_ENCODING vaulues */
#define N_ENCODINGS ((N_VIDEO_NORMS) * (N_COMPOSITE_CHANNELS + N_SVIDEO_CHANNELS))


/**************************************************************************/

static XF86VideoFormatRec SMI_VideoFormats[] =
{
    { 15, TrueColor },	/* depth, class				*/
    { 16, TrueColor },	/* depth, class				*/
    { 24, TrueColor },	/* depth, class				*/
};


/**************************************************************************/

/**
 * Attributes
 */

#define XV_ENCODING_NAME        "XV_ENCODING"
#define XV_BRIGHTNESS_NAME      "XV_BRIGHTNESS"
#define XV_CAPTURE_BRIGHTNESS_NAME      "XV_CAPTURE_BRIGHTNESS"
#define XV_CONTRAST_NAME        "XV_CONTRAST"
#define XV_SATURATION_NAME      "XV_SATURATION"
#define XV_HUE_NAME             "XV_HUE"
#define XV_COLORKEY_NAME        "XV_COLORKEY"
#define XV_INTERLACED_NAME      "XV_INTERLACED"


/* fixed order! */
static XF86AttributeRec SMI_VideoAttributesSAA711x[N_ATTRS] = {
    {XvSettable | XvGettable,        0, N_ENCODINGS-1, XV_ENCODING_NAME},
    {XvSettable | XvGettable,        0,           255, XV_BRIGHTNESS_NAME},
    {XvSettable | XvGettable,        0,           255, XV_CAPTURE_BRIGHTNESS_NAME},
    {XvSettable | XvGettable,        0,           127, XV_CONTRAST_NAME},
    {XvSettable | XvGettable,        0,           127, XV_SATURATION_NAME},
    {XvSettable | XvGettable,     -128,           127, XV_HUE_NAME},
    {XvSettable | XvGettable, 0x000000,      0xFFFFFF, XV_COLORKEY_NAME},
    {XvSettable | XvGettable,        0,             1, XV_INTERLACED_NAME},
};

static XF86AttributeRec SMI_VideoAttributes[2] = {
    {XvSettable | XvGettable,        0,           255, XV_BRIGHTNESS_NAME},
    {XvSettable | XvGettable, 0x000000,      0xFFFFFF, XV_COLORKEY_NAME},
};


/**************************************************************************/
static XF86ImageRec SMI_VideoImages[] =
{
    XVIMAGE_YUY2,
    XVIMAGE_YV12,
    XVIMAGE_I420,
    {
	FOURCC_RV15,			/* id				*/
	XvRGB,				/* type				*/
	LSBFirst,			/* byte_order			*/
	{ 'R', 'V' ,'1', '5',
	  0x00, '5',  0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00 },	/* guid				*/
	16,				/* bits_per_pixel		*/
	XvPacked,			/* format			*/
	1,				/* num_planes			*/
	15,				/* depth			*/
	0x001F, 0x03E0, 0x7C00,		/* red_mask, green, blue	*/
	0, 0, 0,			/* y_sample_bits, u, v		*/
	0, 0, 0,			/* horz_y_period, u, v		*/
	0, 0, 0,			/* vert_y_period, u, v		*/
	{ 'R', 'V', 'B' },		/* component_order		*/
	XvTopToBottom			/* scaline_order		*/
    },
    {
	FOURCC_RV16,			/* id				*/
	XvRGB,				/* type				*/
	LSBFirst,			/* byte_order			*/
	{ 'R', 'V' ,'1', '6',
	  0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00 },	/* guid				*/
	16,				/* bits_per_pixel		*/
	XvPacked,			/* format			*/
	1,				/* num_planes			*/
	16,				/* depth			*/
	0x001F, 0x07E0, 0xF800,		/* red_mask, green, blue	*/
	0, 0, 0,			/* y_sample_bits, u, v		*/
	0, 0, 0,			/* horz_y_period, u, v		*/
	0, 0, 0,			/* vert_y_period, u, v		*/
	{ 'R', 'V', 'B' },		/* component_order		*/
	XvTopToBottom			/* scaline_order		*/
    },
    {
	FOURCC_RV24,			/* id				*/
	XvRGB,				/* type				*/
	LSBFirst,			/* byte_order			*/
	{ 'R', 'V' ,'2', '4',
	  0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00 },	/* guid				*/
	24,				/* bits_per_pixel		*/
	XvPacked,			/* format			*/
	1,				/* num_planes			*/
	24,				/* depth			*/
	0x0000FF, 0x00FF00, 0xFF0000,	/* red_mask, green, blue	*/
	0, 0, 0,			/* y_sample_bits, u, v		*/
	0, 0, 0,			/* horz_y_period, u, v		*/
	0, 0, 0,			/* vert_y_period, u, v		*/
	{ 'R', 'V', 'B' },		/* component_order			*/
	XvTopToBottom			/* scaline_order			*/
    },
    {
	FOURCC_RV32,			/* id				*/
	XvRGB,				/* type				*/
	LSBFirst,			/* byte_order			*/
	{ 'R', 'V' ,'3', '2',
	  0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00 },	/* guid				*/
	32,				/* bits_per_pixel		*/
	XvPacked,			/* format			*/
	1,				/* num_planes			*/
	24,				/* depth			*/
	0x0000FF, 0x00FF00, 0xFF0000,	/* red_mask, green, blue	*/
	0, 0, 0,			/* y_sample_bits, u, v		*/
	0, 0, 0,			/* horz_y_period, u, v		*/
	0, 0, 0,			/* vert_y_period, u, v		*/
	{ 'R', 'V', 'B' },		/* component_order			*/
	XvTopToBottom			/* scaline_order			*/
    },
};


/**************************************************************************/
static XF86ImageRec SMI501_VideoImages[] = {
    XVIMAGE_YUY2,
    XVIMAGE_YV12,
    XVIMAGE_I420,
    {
     FOURCC_RV16,		/* id                                           */
     XvRGB,			/* type                                         */
     LSBFirst,			/* byte_order                           */
     {'R', 'V', '1', '6',
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00},	/* guid                                         */
     16,			/* bits_per_pixel                       */
     XvPacked,			/* format                                       */
     1,				/* num_planes                           */
     16,			/* depth                                        */
     0x001F, 0x07E0, 0xF800,	/* red_mask, green, blue        */
     0, 0, 0,			/* y_sample_bits, u, v          */
     0, 0, 0,			/* horz_y_period, u, v          */
     0, 0, 0,			/* vert_y_period, u, v          */
     {'R', 'V', 'B'},		/* component_order                      */
     XvTopToBottom		/* scaline_order                        */
     },
    {
     FOURCC_RV32,		/* id                                           */
     XvRGB,			/* type                                         */
     LSBFirst,			/* byte_order                           */
     {'R', 'V', '3', '2',
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00},	/* guid                                         */
     32,			/* bits_per_pixel                       */
     XvPacked,			/* format                                       */
     1,				/* num_planes                           */
     24,			/* depth                                        */
     0x0000FF, 0x00FF00, 0xFF0000,	/* red_mask, green, blue        */
     0, 0, 0,			/* y_sample_bits, u, v          */
     0, 0, 0,			/* horz_y_period, u, v          */
     0, 0, 0,			/* vert_y_period, u, v          */
     {'R', 'V', 'B'},		/* component_order                      */
     XvTopToBottom		/* scaline_order                        */
     },
};

/**************************************************************************/

/**
 * SAA7111 video decoder register values
 */


/** SAA7111 control sequences for selecting one out of four
    composite input channels */
static I2CByte SAA7111CompositeChannelSelect[N_COMPOSITE_CHANNELS][4] = {
    { 0x02, 0xC0, 0x09, 0x4A}, /* CVBS AI11 */
    { 0x02, 0xC1, 0x09, 0x4A}, /* CVBS AI12 */
    { 0x02, 0xC2, 0x09, 0x4A}, /* CVBS AI21 */
    { 0x02, 0xC3, 0x09, 0x4A}, /* CVBS AI22 */
};


/** SAA7111 control sequences for selecting one out of two
    s-video input channels */
static I2CByte SAA7111SVideoChannelSelect[N_SVIDEO_CHANNELS][4] = {
    { 0x02, 0xC6, 0x09, 0xCA}, /* Y/C AI11/AI21 */
    { 0x02, 0xC7, 0x09, 0xCA}, /* Y/C AI12/AI22 */
};


/** SAA7111 control sequences for selecting one out of three
    video norms */
static I2CByte SAA7111VideoStd[3][8] = {
    {0x06, 108, 0x07, 108, 0x08, 0x09, 0x0E, 0x01}, /* PAL */
    {0x06, 107, 0x07, 107, 0x08, 0x49, 0x0E, 0x01}, /* NTSC */
    {0x06, 108, 0x07, 108, 0x08, 0x01, 0x0E, 0x51}  /* SECAM */
};


#if 0
static I2CByte SAA7110InitData[] =
{
	/* Configuration */
    0x00, 0x4C, 0x01, 0x3C, 0x02, 0x00, 0x03, 0xEF,
    0x04, 0xBD, 0x05, 0xE2, 0x06, 0x00, 0x07, 0x00,
    0x08, 0xF8, 0x09, 0xF8, 0x0A, 0x60, 0x0B, 0x60,
    0x0C, 0x00, 0x0D, 0x80, 0x0E, 0x18, 0x0F, 0xD9,
    0x10, 0x00, 0x11, 0x2B, 0x12, 0x40, 0x13, 0x40,
    0x14, 0x42, 0x15, 0x1A, 0x16, 0xFF, 0x17, 0xDA,
    0x18, 0xE6, 0x19, 0x90, 0x20, 0xD9, 0x21, 0x16,
    0x22, 0x40, 0x23, 0x40, 0x24, 0x80, 0x25, 0x40,
    0x26, 0x80, 0x27, 0x4F, 0x28, 0xFE, 0x29, 0x01,
    0x2A, 0xCF, 0x2B, 0x0F, 0x2C, 0x03, 0x2D, 0x01,
    0x2E, 0x83, 0x2F, 0x03, 0x30, 0x40, 0x31, 0x35,
    0x32, 0x02, 0x33, 0x8C, 0x34, 0x03,

	/* NTSC */
    0x11, 0x2B, 0x0F, 0xD9,

	/* RCA input connector */
    0x06, 0x00, 0x0E, 0x18, 0x20, 0xD9, 0x21, 0x16,
    0x22, 0x40, 0x2C, 0x03,

};
#endif

static I2CByte SAA7111InitData[] =
{
    0x11, 0x1D, /* 0D D0=1: automatic colour killer off
		   D1=0: DMSD data to YUV output
		   D2=1: output enable H/V sync on
		   D3=1: output enable YUV data on */
    0x02, 0xC0, /* Mode 0 */
    0x03, 0x23, /* automatic gain */
    0x04, 0x00, /*  */
    0x05, 0x00, /*  */
    0x06, 108,  /* hor sync begin */
    0x07, 108,  /* hor sync stop */
    0x08, 0x88, /* sync control:
		   D1-0=00: VNOI = normal mode
		   D2=0: PLL closed
		   D3=1: VTR mode
		   D7=1: automatic field detection */
    0x09, 0x41, /* 4A luminance control */
    0x0A, 0x80, /* brightness = 128 (CCIR level) */
    0x0B, 0x40, /* contrast = 1.0 */
    0x0C, 0x40, /* crominance = 1.0 (CCIR level) */
    0x0D, 0x00, /* hue = 0 */
    0x0E, 0x01, /* chroma bandwidth = nominal
		   fast colour time constant = nominal
		   chrom comp filter on
		   colour standard PAL BGHI, NTSC M */
    0x10, 0x48, /* luminance delay compensation = 0
		   VRLN = 1
		   fine pos of hs = 0
		   output format = YUV 422 */
    0x12, 0x00, /* 20 D5=1: VPO in tristate */
    0x13, 0x00,
    0x15, 0x00,
    0x16, 0x00,
    0x17, 0x00,

};


/**************************************************************************/

/**
 * generates XF86VideoEncoding[i] with video norm norm, video input format
 * input and video input channel channel
 */
static int
SMI_AddEncoding(XF86VideoEncodingPtr enc, int i,
		int norm, int input, int channel)
{
    char* norm_string;
    char* input_string;
    char channel_string[20];

    ENTER();

    norm_string = VideoNorms[norm].name;
    input_string = VideoInputs[input].name;
    sprintf(channel_string, "%d", channel);
    enc[i].id     = i;
    enc[i].name   = malloc(strlen(norm_string) +
			   strlen(input_string) + 
			   strlen(channel_string)+3);
    if (NULL == enc[i].name)
	LEAVE(-1);

    enc[i].width  = VideoNorms[norm].Wa;
    enc[i].height = VideoNorms[norm].Ha;
    enc[i].rate   = VideoNorms[norm].rate;
    sprintf(enc[i].name,"%s-%s-%s", norm_string, input_string, channel_string);

    LEAVE(0);
}


/**
 * builds XF86VideoEncodings with all legal combinations of video norm,
 * video input format and video input channel
 */
static void
SMI_BuildEncodings(SMI_PortPtr p)
{
    int ch, n;

    ENTER();

    /* allocate memory for encoding array */
    p->enc = malloc(sizeof(XF86VideoEncodingRec) * N_ENCODINGS);
    if (NULL == p->enc)
	goto fail;
    memset(p->enc,0,sizeof(XF86VideoEncodingRec) * N_ENCODINGS);
    /* allocate memory for video norm array */
    p->norm = malloc(sizeof(int) * N_ENCODINGS);
    if (NULL == p->norm)
	goto fail;
    memset(p->norm,0,sizeof(int) * N_ENCODINGS);
    /* allocate memory for video input format array */
    p->input = malloc(sizeof(int) * N_ENCODINGS);
    if (NULL == p->input)
	goto fail;
    memset(p->input,0,sizeof(int) * N_ENCODINGS);
    /* allocate memory for video channel number array */
    p->channel = malloc(sizeof(int) * N_ENCODINGS);
    if (NULL == p->channel)
	goto fail;
    memset(p->channel,0,sizeof(int) * N_ENCODINGS);

    /* fill arrays */
    p->nenc = 0;
    for (ch = 0; ch < N_COMPOSITE_CHANNELS; ch++) {
	for (n = 0; n < N_VIDEO_NORMS; n++) {
	    SMI_AddEncoding(p->enc, p->nenc, n, VID_COMPOSITE, ch);
	    p->norm[p->nenc]  = n;
	    p->input[p->nenc] = VID_COMPOSITE;
	    p->channel[p->nenc] = ch;
	    p->nenc++;
	}
    }
    for (ch = 0; ch < N_SVIDEO_CHANNELS; ch++) {
	for (n = 0; n < N_VIDEO_NORMS; n++) {
	    SMI_AddEncoding(p->enc, p->nenc, n, VID_SVIDEO, ch);
	    p->norm[p->nenc]  = n;
	    p->input[p->nenc] = VID_SVIDEO;
	    p->channel[p->nenc] = ch;
	    p->nenc++;
	}
    }
    LEAVE();
    
 fail:
    free(p->input);
    p->input = NULL;
    free(p->norm);
    p->norm = NULL;
    free(p->channel);
    p->channel = NULL;
    free(p->enc);
    p->enc = NULL;
    p->nenc = 0;
    LEAVE();
}


/******************************************************************************\
**                                                                            **
**                  X V E X T E N S I O N   I N T E R F A C E                 **
**                                                                            **
\******************************************************************************/

void
SMI_InitVideo(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    XF86VideoAdaptorPtr *ptrAdaptors, *newAdaptors = NULL;
    XF86VideoAdaptorPtr newAdaptor = NULL;
    int numAdaptors;

    ENTER();

    numAdaptors = xf86XVListGenericAdaptors(pScrn, &ptrAdaptors);

    DEBUG("numAdaptors=%d\n", numAdaptors);

    newAdaptor = SMI_SetupVideo(pScreen);
    DEBUG("newAdaptor=%p\n", newAdaptor);
    SMI_InitOffscreenImages(pScreen);

    if (newAdaptor != NULL) {
        if (numAdaptors == 0) {
            numAdaptors = 1;
            ptrAdaptors = &newAdaptor;
        } else {
            newAdaptors = malloc((numAdaptors + 1) *
                    sizeof(XF86VideoAdaptorPtr*));
            if (newAdaptors != NULL) {
                memcpy(newAdaptors, ptrAdaptors,
                        numAdaptors * sizeof(XF86VideoAdaptorPtr));
                newAdaptors[numAdaptors++] = newAdaptor;
                ptrAdaptors = newAdaptors;
            }
        }
    }

    if (numAdaptors != 0) {
        DEBUG("ScreenInit %i\n",numAdaptors);
        xf86XVScreenInit(pScreen, ptrAdaptors, numAdaptors);
    }

    free(newAdaptors);

    LEAVE();
}


/*************************************************************************/

/*
 *  Video codec controls
 */

#if 0
/**
 * scales value value of attribute i to range min, max
 */
static int
Scale(int i, int value, int min, int max)
{
    return min + (value - SMI_VideoAttributes[i].min_value) * (max - min) /
	(SMI_VideoAttributes[i].max_value - SMI_VideoAttributes[i].min_value);
}
#endif
/**
 * sets video decoder attributes channel, encoding, brightness, contrast, saturation, hue
 */
static int
SetAttr(ScrnInfoPtr pScrn, int i, int value)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    SMI_PortPtr pPort = (SMI_PortPtr) pSmi->ptrAdaptor->pPortPrivates[0].ptr;

    if (i < XV_ENCODING || i > XV_HUE)
	return BadMatch;
    
    /* clamps value to attribute range */
    value = CLAMP(value, SMI_VideoAttributes[i].min_value,
		  SMI_VideoAttributes[i].max_value);

    if (i == XV_BRIGHTNESS) {
	int my_value = (value <= 128? value + 128 : value - 128);
	SetKeyReg(pSmi, 0x5C, 0xEDEDED | (my_value << 24));
    } else if (pPort->I2CDev.SlaveAddr == SAA7110) {
	return SetAttrSAA7110(pScrn, i, value);
    } else if (pPort->I2CDev.SlaveAddr == SAA7111) {
	return SetAttrSAA7111(pScrn, i, value);
    }
#if 0
    else {
	return XvBadAlloc;
    }
#endif

    return Success;
}


/**
 * sets SAA7110 video decoder attributes channel, encoding, brightness, contrast, saturation, hue
 */
static int
SetAttrSAA7110(ScrnInfoPtr pScrn, int i, int value)
{
    /* not supported */
    return XvBadAlloc;
}


/**
 * sets SAA7111 video decoder attributes channel, encoding,
 * brightness, contrast, saturation, hue
 */
static int
SetAttrSAA7111(ScrnInfoPtr pScrn, int i, int value)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    SMI_PortPtr pPort = (SMI_PortPtr) pSmi->ptrAdaptor->pPortPrivates[0].ptr;

    if (i == XV_ENCODING) {
	int norm;
	int input;
	int channel;
	norm = pPort->norm[value];
	input = pPort->input[value];
	channel = pPort->channel[value];

	DEBUG("SetAttribute XV_ENCODING: %d. norm=%d input=%d channel=%d\n",
	      value, norm, input, channel);

	/* set video norm */
	if (!xf86I2CWriteVec(&(pPort->I2CDev), SAA7111VideoStd[norm],
			     ENTRIES(SAA7111VideoStd[norm]) / 2)) {
	    return XvBadAlloc;
	}
	/* set video input format and channel */
	if (input == VID_COMPOSITE) {
	    if (!xf86I2CWriteVec(&(pPort->I2CDev),
				 SAA7111CompositeChannelSelect[channel],
				 ENTRIES(SAA7111CompositeChannelSelect[channel]) / 2)) {
		return XvBadAlloc;
	    }
	} else {
	    if (!xf86I2CWriteVec(&(pPort->I2CDev),
				 SAA7111SVideoChannelSelect[channel],
				 ENTRIES(SAA7111SVideoChannelSelect[channel]) / 2)) {
		return XvBadAlloc;
	    }
	}
    } else if (i >= XV_CAPTURE_BRIGHTNESS && i <= XV_HUE) {
	int slave_adr = 0;

	switch (i) {

	case XV_CAPTURE_BRIGHTNESS:
	    DEBUG("SetAttribute XV_BRIGHTNESS: %d\n", value);
	    slave_adr = 0x0a;
	    break;
		
	case XV_CONTRAST:
	    DEBUG("SetAttribute XV_CONTRAST: %d\n", value);
	    slave_adr = 0x0b;
	    break;

	case XV_SATURATION:
	    DEBUG("SetAttribute XV_SATURATION: %d\n", value);
	    slave_adr = 0x0c;
	    break;

	case XV_HUE:
	    DEBUG("SetAttribute XV_HUE: %d\n", value);
	    slave_adr = 0x0d;
	    break;

	default:
	    return XvBadAlloc;
	}
	if (!xf86I2CWriteByte(&(pPort->I2CDev), slave_adr, (value & 0xff)))
	    return XvBadAlloc;
    } else {
	return BadMatch;
    }

    /* debug: show registers */
    {
	I2CByte i2c_bytes[32];
	int j;
	xf86I2CReadBytes(&(pPort->I2CDev), 0, i2c_bytes, 32);
	DEBUG("SAA7111 Registers\n");
	for (j=0; j<32; j++) {
	    DEBUG("%02X=%02X ", j, i2c_bytes[j]);
	    if ((j&7) == 7) DEBUG("\n");
	}
    }

    return Success;
}


/******************************************************************************\
**									      **
**	V I D E O   M A N A G E M E N T					      **
**									      **
\******************************************************************************/

static XF86VideoAdaptorPtr
SMI_SetupVideo(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    SMIPtr pSmi = SMIPTR(pScrn);
    SMI_PortPtr smiPortPtr;
    XF86VideoAdaptorPtr ptrAdaptor;

    ENTER();

    ptrAdaptor = calloc(1, sizeof(XF86VideoAdaptorRec) +
		 sizeof(DevUnion) + sizeof(SMI_PortRec));
    if (ptrAdaptor == NULL)
	LEAVE(NULL);

    ptrAdaptor->type = XvInputMask
#if SMI_USE_CAPTURE
		     | XvOutputMask
		     | XvVideoMask
#endif
		     | XvImageMask
		     | XvWindowMask
		     ;

    ptrAdaptor->flags = VIDEO_OVERLAID_IMAGES;
    if (IS_MSOC(pSmi)) {
	ptrAdaptor->name = "Silicon Motion MSOC Series Video Engine";
    }
    else
	ptrAdaptor->name = "Silicon Motion Lynx Series Video Engine";

    ptrAdaptor->nPorts = 1;
    ptrAdaptor->pPortPrivates = (DevUnion*) &ptrAdaptor[1];
    ptrAdaptor->pPortPrivates[0].ptr = (pointer) &ptrAdaptor->pPortPrivates[1];

    smiPortPtr = (SMI_PortPtr) ptrAdaptor->pPortPrivates[0].ptr;

    SMI_BuildEncodings(smiPortPtr);
    ptrAdaptor->nEncodings = smiPortPtr->nenc;
    ptrAdaptor->pEncodings = smiPortPtr->enc;
#if 0
    /* aaa whats this? */
	for (i = 0; i < nElems(SMI_VideoEncodings); i++)
	{
		SMI_VideoEncodings[i].width = pSmi->lcdWidth;
		SMI_VideoEncodings[i].height = pSmi->lcdHeight;
	}
#endif

    ptrAdaptor->nFormats = nElems(SMI_VideoFormats);
    ptrAdaptor->pFormats = SMI_VideoFormats;

    ptrAdaptor->nAttributes = nElems(SMI_VideoAttributes);
    ptrAdaptor->pAttributes = SMI_VideoAttributes;

    if (IS_MSOC(pSmi)) {
	ptrAdaptor->nImages = nElems(SMI501_VideoImages);
	ptrAdaptor->pImages = SMI501_VideoImages;
    }
    else {
	ptrAdaptor->nImages = nElems(SMI_VideoImages);
	ptrAdaptor->pImages = SMI_VideoImages;
    }

#if SMI_USE_CAPTURE
    if (pSmi->Chipset == SMI_COUGAR3DR || IS_MSOC(pSmi))
	ptrAdaptor->PutVideo = NULL;
    else
	ptrAdaptor->PutVideo = SMI_PutVideo;
    ptrAdaptor->PutStill = NULL;
    ptrAdaptor->GetVideo = NULL;
    ptrAdaptor->GetStill = NULL;
#else
    ptrAdaptor->PutVideo = NULL;
    ptrAdaptor->PutStill = NULL;
    ptrAdaptor->GetVideo = NULL;
    ptrAdaptor->GetStill = NULL;
#endif
    ptrAdaptor->StopVideo = SMI_StopVideo;
    ptrAdaptor->SetPortAttribute = SMI_SetPortAttribute;
    ptrAdaptor->GetPortAttribute = SMI_GetPortAttribute;
    ptrAdaptor->QueryBestSize = SMI_QueryBestSize;
    ptrAdaptor->PutImage = SMI_PutImage;
    ptrAdaptor->QueryImageAttributes = SMI_QueryImageAttributes;

    smiPortPtr->Attribute[XV_COLORKEY] = pSmi->videoKey;
    smiPortPtr->Attribute[XV_INTERLACED] = pSmi->interlaced;
    smiPortPtr->videoStatus = 0;

#if 0
    /* aaa does not work ? */
    if (xf86I2CProbeAddress(pSmi->I2C, SAA7111))
        LEAVE(NULL);
    DEBUG("SAA7111 detected\n");
#endif

    smiPortPtr->I2CDev.DevName = "SAA 7111A";
    smiPortPtr->I2CDev.SlaveAddr = SAA7111;
    smiPortPtr->I2CDev.pI2CBus = pSmi->I2C;


    if (!IS_MSOC(pSmi) && xf86I2CDevInit(&(smiPortPtr->I2CDev))) {
	
	if (xf86I2CWriteVec(&(smiPortPtr->I2CDev), SAA7111InitData, ENTRIES(SAA7111InitData) / 2)) {
	    xvEncoding   = MAKE_ATOM(XV_ENCODING_NAME);
	    xvHue        = MAKE_ATOM(XV_HUE_NAME);
	    xvSaturation = MAKE_ATOM(XV_SATURATION_NAME);
	    xvContrast   = MAKE_ATOM(XV_CONTRAST_NAME);
	    
	    xvInterlaced = MAKE_ATOM(XV_INTERLACED_NAME);
	    DEBUG("SAA7111 intialized\n");
    
	} else { 
	    xf86DestroyI2CDevRec(&(smiPortPtr->I2CDev),FALSE);
	    smiPortPtr->I2CDev.SlaveAddr = 0;
	}
    } else
	smiPortPtr->I2CDev.SlaveAddr = 0;
	
#if defined(REGION_NULL)
    REGION_NULL(pScreen, &smiPortPtr->clip);
#else
    REGION_INIT(pScreen, &smiPortPtr->clip, NullBox, 0);
#endif
    
    pSmi->ptrAdaptor = ptrAdaptor;
    pSmi->BlockHandler = pScreen->BlockHandler;
    pScreen->BlockHandler = SMI_BlockHandler;
    
    xvColorKey   = MAKE_ATOM(XV_COLORKEY_NAME);
    xvBrightness = MAKE_ATOM(XV_BRIGHTNESS_NAME);
    xvCapBrightness = MAKE_ATOM(XV_CAPTURE_BRIGHTNESS_NAME);
    
    SMI_ResetVideo(pScrn);

    LEAVE(ptrAdaptor);
}


static void
SMI_ResetVideo(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    SMI_PortPtr pPort = (SMI_PortPtr) pSmi->ptrAdaptor->pPortPrivates[0].ptr;
    int r, g, b;

    ENTER();

    SetAttr(pScrn, XV_ENCODING, 0);     /* Encoding = pal-composite-0 */
    SetAttr(pScrn, XV_BRIGHTNESS, 128); /* Brightness = 128 (CCIR level) */
    SetAttr(pScrn, XV_CAPTURE_BRIGHTNESS, 128); /* Brightness = 128 (CCIR level) */
    SetAttr(pScrn, XV_CONTRAST, 71);    /* Contrast = 71 (CCIR level) */
    SetAttr(pScrn, XV_SATURATION, 64);  /* Color saturation = 64 (CCIR level) */
    SetAttr(pScrn, XV_HUE, 0);          /* Hue = 0 */

    switch (pScrn->depth) {
    case 8:
	SetKeyReg(pSmi, FPR04, pPort->Attribute[XV_COLORKEY] & 0x00FF);
	SetKeyReg(pSmi, FPR08, 0);
	break;
    case 15:
    case 16:
	SetKeyReg(pSmi, FPR04, pPort->Attribute[XV_COLORKEY] & 0xFFFF);
	SetKeyReg(pSmi, FPR08, 0);
	break;
    default:
        r = (pPort->Attribute[XV_COLORKEY] & pScrn->mask.red) >> pScrn->offset.red;
        g = (pPort->Attribute[XV_COLORKEY] & pScrn->mask.green) >> pScrn->offset.green;
        b = (pPort->Attribute[XV_COLORKEY] & pScrn->mask.blue) >> pScrn->offset.blue;
	SetKeyReg(pSmi, FPR04, ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
	SetKeyReg(pSmi, FPR08, 0);
	break;
    }

    SetKeyReg(pSmi, FPR5C, 0xEDEDED | (pPort->Attribute[XV_BRIGHTNESS] << 24));

    LEAVE();
}


#if SMI_USE_CAPTURE
static int
SMI_PutVideo(
	ScrnInfoPtr	pScrn,
	short		vid_x,
	short		vid_y,
	short		drw_x,
	short		drw_y,
	short		vid_w,
	short		vid_h,
	short		drw_w,
	short		drw_h,
	RegionPtr	clipBoxes,
	pointer		data,
	DrawablePtr	pDraw
)
{
    SMI_PortPtr pPort = (SMI_PortPtr) data;
    SMIPtr pSmi = SMIPTR(pScrn);
    CARD32 vid_pitch, vid_address;
    CARD32 vpr00, cpr00;
    int xscale, yscale;
    BoxRec dstBox;
    INT32 x1, y1, x2, y2;
    int norm;
    int size, width, height, fbPitch;
    int top, left;
    xf86CrtcConfigPtr crtcConf = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86CrtcPtr crtc;

    ENTER();

    DEBUG("Interlaced Video %d\n", pPort->Attribute[XV_INTERLACED]);

    if (!pPort->Attribute[XV_INTERLACED]) {
	/* no interlace: lines will be doubled */
	vid_h /= 2;
    }

    /* field start aaa*/
    norm = pPort->norm[pPort->Attribute[XV_ENCODING]];
    vid_x += VideoNorms[norm].HStart;
    vid_y += VideoNorms[norm].VStart;
    /* only even values allowed (UV-phase) */
    vid_x &= ~1;

    DEBUG("vid_x=%d vid_y=%d drw_x=%d drw_y=%d  "
	  "vid_w=%d vid_h=%d drw_w=%d drw_h=%d\n",
	  vid_x, vid_y, drw_x, drw_y, vid_w, vid_h, drw_w, drw_h);

    x1 = vid_x;
    y1 = vid_y;
    x2 = vid_x + vid_w;
    y2 = vid_y + vid_h;

    width = vid_w;
    height = vid_h;

    dstBox.x1 = drw_x;
    dstBox.y1 = drw_y;
    dstBox.x2 = drw_x + drw_w;
    dstBox.y2 = drw_y + drw_h;

    if(!xf86_crtc_clip_video_helper(pScrn, &crtc, crtcConf->crtc[0], &dstBox, &x1, &x2, &y1, &y2, clipBoxes, width, height))
	LEAVE(Success);

    if (crtc != crtcConf->crtc[0])
	LEAVE(Success);

    /* Transform dstBox to the CRTC coordinates */
    dstBox.x1 -= crtc->x;
    dstBox.y1 -= crtc->y;
    dstBox.x2 -= crtc->x;
    dstBox.y2 -= crtc->y;

    DEBUG("Clip: x1=%d y1=%d x2=%d y2=%d\n",  x1 >> 16, y1 >> 16, x2 >> 16, y2 >> 16);

    vid_pitch = (vid_w * 2 + 7) & ~7;

    vpr00 = READ_VPR(pSmi, 0x00) & ~0x0FF000FF;
    cpr00 = READ_CPR(pSmi, 0x00) & ~0x000FFF00;

    /* vpr00:
       Bit 2..0   = 6: Video Window I Format                    = YUV4:2:2
       Bit 3      = 1: Video Window I Enable                    = enabled
       Bit 4      = 0: Video Window I YUV Averaging             = disabled
       Bit 5      = 0: Video Window I Hor. Replication          = disabled
       Bit 6      = 0: Video Window I data doubling             = disabled
       Bit 14..8  = 0: Video Window II                          = disabled
       Bit 18..16 = 0: Graphics Data Format                     = 8-bit index
       Bit 19     = 0: Top Video Window Select                  = window I
       Bit 20     = 1: Color Key for Window I                   = enabled
       Bit 21     = 0: Vertical Interpolation                   = s. below
       Bit 22     = 0: Flicker Reduction for TV Modes           = disabled
       Bit 23     = 0: Fixed Vertical Interpolation             = disabled
       Bit 24     = 1: Select Video Window I Source Addr        = 
       Bit 25     = 0: Enable V0FIFO to fetch 8-Bit color data  = disabled
       Bit 26     = 0:
       Bit 27     = 1: Color Key for Window II                  = disabled
       Bit 31..28 = reserved
    */
    if (pPort->Attribute[XV_INTERLACED]) {
	/*
	  Bit 21     = 0: Vertical Interpolation                   = disabled
	  Bit 24     = 0: Select Video Window I Source Addr        = 0
	*/
	vpr00 |= 0x0010000E;
    } else {
	/*
	  Bit 21     = 1: Vertical Interpolation                   = enabled
	  Bit 24     = 1: Select Video Window I Source Addr        = 1
	  1= Video window I source addr = capture port buffer ?
	*/
	vpr00 |= 0x0130000E;
    }

    /* cpr00:
       Bit 0      = 1: Video Capture Enable                     = enabled
       Bit 8      = 0: Capture Control                          = continous
       Bit 9      = 0: Double Buffer Enable                     = s. below
       Bit 10     = 0: Interlace Data Capture                   = s. below
       Bit 13..11 = 0: Frame Skip Enable                        = s. below
       Bit 15..14 = 0: Video Capture Input Format               = YUV4:2:2
       Bit 17..16 = 0: Enable Hor. Reduction                    = s. below
       Bit 19..18 = 0: Enable Vert. Reduction                   = s. below
       Bit 21..20 = 0: Enable Hor. Filtering                    = s. below
       Bit 22     = 0: HREF Polarity                            = high active
       Bit 23     = 0: VREF Polarity                            = high active
       Bit 24     = 1: Field Detection Method VSYNC edge        = rising
    */
    if (pPort->Attribute[XV_INTERLACED]) {
	/*
	  Bit 9      = 1: Double Buffer Enable                  = enabled
	  Bit 10     = 1: Interlace Data Capture                = enabled
	  Bit 13..11 = 0: Frame Skip Enable                     = no skip
	*/
	cpr00 |= 0x01000601;
    } else {
	/*
	  Bit 9      = 0: Double Buffer Enable                  = disabled
	  Bit 10     = 0: Interlace Data Capture                = disabled
	  Bit 13..11 = 010: Frame Skip Enable                   = skip every other frame
	*/
	cpr00 |= 0x01001001;
    }

    if (pSmi->ByteSwap)
	cpr00 |= 0x00004000;

    fbPitch = (pScrn->displayWidth * pSmi->Bpp + 15) & ~15;

    if (vid_w <= drw_w) {
	xscale = (256 * vid_w / drw_w) & 0xFF;
    } else if (vid_w / 2 <= drw_w) {
	xscale = (128 * vid_w / drw_w) & 0xFF;
	width /= 2;
	vid_pitch /= 2;
	cpr00 |= 0x00010000;
    } else if (vid_w / 4 <= drw_w) {
	xscale = (64 * vid_w / drw_w) & 0xFF;
	width /= 4;
	vid_pitch /= 4;
	cpr00 |= 0x00020000;
    } else {
	xscale = 0;
	width /= 4;
	vid_pitch /= 4;
	cpr00 |= 0x00020000;
    }

    if (vid_h <= drw_h) {
	yscale = (256 * vid_h / drw_h) & 0xFF;
    } else if (vid_h / 2 <= drw_h) {
	yscale = (128 * vid_h / drw_h) & 0xFF;
	height /= 2;
	cpr00 |= 0x00040000;
    } else if (vid_h / 4 <= drw_h) {
	yscale = (64 * vid_h / drw_h) & 0xFF;
	height /= 4;
	cpr00 |= 0x00080000;
    } else {
	yscale = 0;
	height /= 4;
	cpr00 |= 0x00080000;
    }

    do {
	size = vid_pitch * height;
	DEBUG("SMI_AllocateMemory: vid_pitch=%d height=%d size=%d\n",
	      vid_pitch, height, size);
	pPort->video_offset = SMI_AllocateMemory(pScrn, &pPort->video_memory, size);
        if (pPort->video_offset == 0) {
	    if ((cpr00 & 0x000C0000) == 0) {
		/* height -> 1/2 height */
		yscale = (128 * vid_h / drw_h) & 0xFF;
		height = vid_h / 2;
		cpr00 |= 0x00040000;
	    } else if (cpr00 & 0x00040000) {
		/* 1/2 height -> 1/4 height */
		yscale = (64 * vid_h / drw_h) & 0xFF;
		height = vid_h / 4;
		cpr00 ^= 0x000C0000;
	    } else {
		/* 1/4 height */
		if ((cpr00 & 0x00030000) == 0) {
		    /* width -> 1/2 width */
		    xscale = (128 * vid_w / drw_w) & 0xFF;
		    width = vid_w / 2;
		    cpr00 |= 0x00010000;
		} else if (cpr00 & 0x00010000) {
		    /* 1/2 width -> 1/4 width */
		    xscale = (64 * vid_w / drw_w) & 0xFF;
		    width = vid_w / 4;
		    cpr00 ^= 0x00030000;
		} else {
		    DEBUG("allocate error\n");
		    LEAVE(BadAlloc);
		}
	    }
	}
    } while (pPort->video_offset == 0);

    DEBUG("xscale==%d yscale=%d width=%d height=%d\n",
	  xscale, yscale, width, height);

    /* aaa whats this                     ----------------------v ?
    vid_address = (pPort->area->box.y1 * fbPitch) + ((y1 >> 16) * vid_pitch);*/
    vid_address = pPort->video_offset;

    DEBUG("test RegionsEqual\n");
    if (!REGION_EQUAL(pScrn->pScreen, &pPort->clip, clipBoxes))
    {
	DEBUG((VERBLEV, "RegionCopy\n"));
        REGION_COPY(pScrn->pScreen, &pPort->clip, clipBoxes);
	DEBUG("FillKey\n");
	xf86XVFillKeyHelper(pScrn->pScreen, pPort->Attribute[XV_COLORKEY], clipBoxes);

    }

    left = x1 >> 16;
    top = y1 >> 16;
    width = (x2 - x1) >> 16;
    height = (y2 - y1) >> 16;

    if (!IS_MSOC(pSmi))
	VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA,
		      0x21,
		      VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA,
				   0x21) & ~0x04);
    WRITE_VPR(pSmi, 0x54, READ_VPR(pSmi, 0x54) | 0x00200000);
    /* Video Window I Left and Top Boundaries */
    WRITE_VPR(pSmi, 0x14, dstBox.x1 + (dstBox.y1 << 16));
    /* Video Window I Right and Bottom Boundaries */
    WRITE_VPR(pSmi, 0x18, dstBox.x2 + (dstBox.y2 << 16));
    /* Video Window I Source Width and Offset */
    WRITE_VPR(pSmi, 0x20, (vid_pitch / 8) + ((vid_pitch / 8) << 16));
    /* Video Window I Stretch Factor */
    WRITE_VPR(pSmi, 0x24, (xscale << 8) + yscale);

    if (pPort->Attribute[XV_INTERLACED]) {
	/* Video Window II Left and Top Boundaries */
	WRITE_VPR(pSmi, 0x28, dstBox.x1 + (dstBox.y1 << 16));
	/* Video Window II Right and Bottom Boundaries */
	WRITE_VPR(pSmi, 0x2C, dstBox.x2 + (dstBox.y2 << 16));
	/* Video Window II Source Width and Offset */
	WRITE_VPR(pSmi, 0x34, (vid_pitch / 8) + ((vid_pitch / 8) << 16));
	/* Video Window II Stretch Factor */
	WRITE_VPR(pSmi, 0x38, (xscale << 8) + yscale);

	/* Video Window I Source Start Address */
	WRITE_VPR(pSmi, 0x1C, vid_address / 8);
	/* Video Window II Source Start Address */
	WRITE_VPR(pSmi, 0x30, vid_address / 8);

	/* Video Window I Source Start Address */
	WRITE_VPR(pSmi, 0x48, vid_address / 8);
	/* Video Window II Source Start Address */
	WRITE_VPR(pSmi, 0x4C, vid_address / 8 + vid_pitch / 8);
	
	/* Video Source Clipping Control */
	WRITE_CPR(pSmi, 0x04, left + ((top/2) << 16));
	/* Video Source Capture Size Control */
	WRITE_CPR(pSmi, 0x08, width + ((height/2) << 16));
	/* Capture Port Buffer I Source Start Address */
	WRITE_CPR(pSmi, 0x0C, vid_address / 8);
	/* Capture Port Buffer II Source Start Address */
	WRITE_CPR(pSmi, 0x10, vid_address / 8 + vid_pitch / 8);
	/* Capture Port Source Offset Address */
	WRITE_CPR(pSmi, 0x14, 2*(vid_pitch / 8) + ((2*(vid_pitch / 8)) << 16));
    } else {
	/* Video Source Clipping Control */
	WRITE_CPR(pSmi, 0x04, left + (top << 16));
	/* Video Source Capture Size Control */
	WRITE_CPR(pSmi, 0x08, width + (height << 16));
	/* Capture Port Buffer I Source Start Address */
	WRITE_CPR(pSmi, 0x0C, vid_address / 8);
	/* Capture Port Buffer II Source Start Address */
	WRITE_CPR(pSmi, 0x10, vid_address / 8);
	/* Capture Port Source Offset Address */
	WRITE_CPR(pSmi, 0x14, (vid_pitch / 8) + ((vid_pitch / 8) << 16));
    }

    WRITE_CPR(pSmi, 0x00, cpr00);
    WRITE_VPR(pSmi, 0x00, vpr00);

    pPort->videoStatus = CLIENT_VIDEO_ON;
    DEBUG("SMI_PutVideo success\n");

    LEAVE(Success);
}
#endif


static void
SMI_StopVideo(
	ScrnInfoPtr	pScrn,
	pointer		data,
	Bool		shutdown
)
{
    SMI_PortPtr pPort = (SMI_PortPtr) data;
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    REGION_EMPTY(pScrn->pScreen, &pPort->clip);

    if (shutdown) {
	if (pPort->videoStatus & CLIENT_VIDEO_ON) {
	    if (pSmi->Chipset == SMI_COUGAR3DR)
		WRITE_FPR(pSmi, FPR00, READ_FPR(pSmi, 0x00) & ~(FPR00_VWIENABLE));
	    else if (IS_MSOC(pSmi))
		WRITE_DCR(pSmi, 0x0040, READ_DCR(pSmi, 0x0040) & ~0x00000004);
	    else
		WRITE_VPR(pSmi, 0x00, READ_VPR(pSmi, 0x00) & ~0x01000008);
#if SMI_USE_CAPTURE
	    if (!IS_MSOC(pSmi) && pSmi->Chipset != SMI_COUGAR3DR) {
		WRITE_CPR(pSmi, 0x00, READ_CPR(pSmi, 0x00) & ~0x00000001);
		WRITE_VPR(pSmi, 0x54, READ_VPR(pSmi, 0x54) & ~0x00F00000);
	    }
#endif
	}
        if (pPort->video_memory != NULL) {
            SMI_FreeMemory(pScrn, pPort->video_memory);
            pPort->video_memory = NULL;
	}
        pPort->videoStatus = 0;
        /* pPort->i2cDevice = 0;aaa*/
    } else {
        if (pPort->videoStatus & CLIENT_VIDEO_ON) {
            pPort->videoStatus |= OFF_TIMER;
            pPort->offTime = currentTime.milliseconds + OFF_DELAY;
	}
    }

    LEAVE();
}

static int
SMI_SetPortAttribute(
	ScrnInfoPtr	pScrn,
	Atom		attribute,
	INT32		value,
	pointer		data
)
{
    int res;
    SMI_PortPtr pPort = (SMI_PortPtr) data;
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    if (attribute == xvColorKey) {
	int r, g, b;

        pPort->Attribute[XV_COLORKEY] = value;
	switch (pScrn->depth) {
	case 8:
	    SetKeyReg(pSmi, FPR04, value & 0x00FF);
	    break;
	case 15:
	case 16:
	    SetKeyReg(pSmi, FPR04, value & 0xFFFF);
	    break;
	default:
	    r = (value & pScrn->mask.red) >> pScrn->offset.red;
	    g = (value & pScrn->mask.green) >> pScrn->offset.green;
	    b = (value & pScrn->mask.blue) >> pScrn->offset.blue;
	    SetKeyReg(pSmi, FPR04, ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
	    break;
	}
	res = Success;
    } else if (attribute == xvInterlaced) {
        pPort->Attribute[XV_INTERLACED] = (value != 0);
	res = Success;
    } else if (attribute == xvEncoding) {
        res = SetAttr(pScrn, XV_ENCODING, value);
    } else if (attribute == xvBrightness) {
        res = SetAttr(pScrn, XV_BRIGHTNESS, value);
    } else if (attribute == xvCapBrightness) {
        res = SetAttr(pScrn, XV_CAPTURE_BRIGHTNESS, value);
    } else if (attribute == xvContrast) {
        res = SetAttr(pScrn, XV_CONTRAST, value);
    } else if (attribute == xvSaturation) {
        res = SetAttr(pScrn, XV_SATURATION, value);
    } else if (attribute == xvHue) {
        res = SetAttr(pScrn, XV_HUE, value);
    } else {
        res = BadMatch;
    }

    LEAVE(res);
}


static int
SMI_GetPortAttribute(
	ScrnInfoPtr	pScrn,
	Atom		attribute,
	INT32		*value,
	pointer		data
)
{
    SMI_PortPtr pPort = (SMI_PortPtr) data;

    ENTER();
    if (attribute == xvEncoding)
        *value = pPort->Attribute[XV_ENCODING];
    else if (attribute == xvBrightness)
        *value = pPort->Attribute[XV_BRIGHTNESS];
    else if (attribute == xvCapBrightness)
        *value = pPort->Attribute[XV_CAPTURE_BRIGHTNESS];
    else if (attribute == xvContrast)
        *value = pPort->Attribute[XV_CONTRAST];
    else if (attribute == xvSaturation)
        *value = pPort->Attribute[XV_SATURATION];
    else if (attribute == xvHue)
        *value = pPort->Attribute[XV_HUE];
    else if (attribute == xvColorKey)
        *value = pPort->Attribute[XV_COLORKEY];
    else
	LEAVE(BadMatch);

    LEAVE(Success);
}


static void
SMI_QueryBestSize(
	ScrnInfoPtr		pScrn,
	Bool			motion,
	short			vid_w,
	short			vid_h,
	short			drw_w,
	short			drw_h,
	unsigned int	*p_w,
	unsigned int	*p_h,
	pointer			data
)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    *p_w = min(drw_w, pSmi->lcdWidth);
    *p_h = min(drw_h, pSmi->lcdHeight);

    LEAVE();
}


static int
SMI_PutImage(
	ScrnInfoPtr		pScrn,
	short			src_x,
	short			src_y,
	short			drw_x,
	short			drw_y,
	short			src_w,
	short			src_h,
	short			drw_w,
	short			drw_h,
	int			id,
	unsigned char		*buf,
	short			width,
	short			height,
	Bool			sync,
	RegionPtr		clipBoxes,
	pointer			data,
	DrawablePtr		pDraw
)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    SMI_PortPtr pPort = (SMI_PortPtr) pSmi->ptrAdaptor->pPortPrivates[0].ptr;
    INT32 x1, y1, x2, y2;
    int bpp = 0;
    int srcPitch, srcPitch2 = 0, dstPitch, size;
    BoxRec dstBox;
    CARD32 offset, offset2 = 0, offset3 = 0, tmp;
    int left, top, nPixels, nLines;
    unsigned char *dstStart;
    xf86CrtcConfigPtr crtcConf = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86CrtcPtr crtc;

    ENTER();

    x1 = src_x;
    y1 = src_y;
    x2 = src_x + src_w;
    y2 = src_y + src_h;

    dstBox.x1 = drw_x;
    dstBox.y1 = drw_y;
    dstBox.x2 = drw_x + drw_w;
    dstBox.y2 = drw_y + drw_h;

    if (pSmi->CSCVideo) {
	if (!xf86XVClipVideoHelper(&dstBox, &x1, &x2, &y1, &y2, clipBoxes,
				   width, height))
	    LEAVE(Success);
    }
    else {
	if (!xf86_crtc_clip_video_helper(pScrn, &crtc, crtcConf->crtc[0], &dstBox,
					 &x1, &x2, &y1, &y2, clipBoxes,
					 width, height))
	    LEAVE(Success);

	if (!crtc)
	    LEAVE(Success);

	/* Transform dstBox to the CRTC coordinates */
	dstBox.x1 -= crtc->x;
	dstBox.y1 -= crtc->y;
	dstBox.x2 -= crtc->x;
	dstBox.y2 -= crtc->y;
    }

    switch (id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	srcPitch  = (width + 3) & ~3;
	offset2   = srcPitch * height;
	srcPitch2 = ((width >> 1) + 3) & ~3;
	offset3   = offset2 + (srcPitch2 * (height >> 1));
	if (pSmi->CSCVideo)
	    dstPitch  = (((width >> 1) + 15) & ~15) << 1;
	else
	    dstPitch  = ((width << 1) + 15) & ~15;
	break;
    case FOURCC_RV24:
	bpp = 3;
	srcPitch = width * bpp;
	dstPitch = (srcPitch + 15) & ~15;
	break;
    case FOURCC_RV32:
	bpp = 4;
	srcPitch = width * bpp;
	dstPitch = (srcPitch + 15) & ~15;
	break;
    case FOURCC_YUY2:
    case FOURCC_RV15:
    case FOURCC_RV16:
    default:
	bpp = 2;
	srcPitch = width * bpp;
	dstPitch = (srcPitch + 15) & ~15;
	break;
    }

    size = dstPitch * height;
    pPort->video_offset = SMI_AllocateMemory(pScrn, &pPort->video_memory, size);
    if (pPort->video_memory == NULL)
	LEAVE(BadAlloc);

    top = y1 >> 16;
    left = (x1 >> 16) & ~1;
    nPixels = ((((x2 + 0xFFFF) >> 16) + 1) & ~1) - left;
    left *= bpp;

    offset = pPort->video_offset + (top * dstPitch);
    dstStart = pSmi->FBBase + offset + left;

    switch(id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	top &= ~1;
	tmp = ((top >> 1) * srcPitch2) + (left >> 2);
	offset2 += tmp;
	offset3 += tmp;
	if (pSmi->CSCVideo)
	    CopyYV12ToVideoMem(buf,
			       buf + offset2, buf + offset3,
			       dstStart, srcPitch, srcPitch2, dstPitch,
			       height, width);
	else {
	    if (id == FOURCC_I420) {
		tmp = offset2;
		offset2 = offset3;
		offset3 = tmp;
	    }
	    nLines = ((((y2 + 0xffff) >> 16) + 1) & ~1) - top;
	    xf86XVCopyYUV12ToPacked(buf + (top * srcPitch) + (left >> 1), 
				    buf + offset2, buf + offset3, dstStart,
				    srcPitch, srcPitch2, dstPitch, nLines,
				    nPixels);
	}
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	buf += (top * srcPitch) + left;
	nLines = ((y2 + 0xffff) >> 16) - top;
	xf86XVCopyPacked(buf, dstStart, srcPitch, dstPitch, nLines, nPixels);
        break;
    }

    if (IS_MSOC(pSmi) ||
	!REGION_EQUAL(pScrn->pScreen, &pPort->clip, clipBoxes)) {
	REGION_COPY(pScrn->pScreen, &pPort->clip, clipBoxes);
	if (!pSmi->CSCVideo)
	    xf86XVFillKeyHelper(pScrn->pScreen, pPort->Attribute[XV_COLORKEY],
				clipBoxes);
    }

    if (pSmi->Chipset == SMI_COUGAR3DR)
	SMI_DisplayVideo0730(pScrn, id, offset, width, height, dstPitch, x1, y1, x2, y2,
			     &dstBox, src_w, src_h, drw_w, drw_h);
    else if (IS_MSOC(pSmi)) {
	if (pSmi->CSCVideo)
	    SMI_DisplayVideo0501_CSC(pScrn, id, offset, width, height, dstPitch,
				     x1, y1, x2, y2, &dstBox,
				     src_w, src_h, drw_w, drw_h, clipBoxes);
	else
	    SMI_DisplayVideo0501(pScrn, id, offset, width, height, dstPitch,
				 x1, y1, x2, y2, &dstBox, src_w, src_h,
				 drw_w, drw_h);
    }
    else{
	if(crtc == crtcConf->crtc[0])
	    SMI_DisplayVideo(pScrn, id, offset, width, height, dstPitch, x1, y1, x2, y2,
			     &dstBox, src_w, src_h, drw_w, drw_h);
    }
    pPort->videoStatus = CLIENT_VIDEO_ON;

    LEAVE(Success);
	
}


static int
SMI_QueryImageAttributes(
	ScrnInfoPtr	pScrn,
	int		id,
	unsigned short	*width,
	unsigned short	*height,
	int		*pitches,
	int		*offsets
)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    int size, tmp;

    ENTER();

    if (*width > pSmi->lcdWidth) {
	*width = pSmi->lcdWidth;
    }
    if (*height > pSmi->lcdHeight) {
	*height = pSmi->lcdHeight;
    }

    *width = (*width + 1) & ~1;
    if (offsets != NULL) {
	offsets[0] = 0;
    }

    switch (id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	*height = (*height + 1) & ~1;
	size = (*width + 3) & ~3;
	if (pitches != NULL) {
	    pitches[0] = size;
	}
	size *= *height;
	if (offsets != NULL) {
	    offsets[1] = size;
	}
	tmp = ((*width >> 1) + 3) & ~3;
	if (pitches != NULL) {
	    pitches[1] = pitches[2] = tmp;
	}
	tmp *= (*height >> 1);
	size += tmp;
	if (offsets != NULL) {
	    offsets[2] = size;
	}
	size += tmp;
	break;
    case FOURCC_YUY2:
    case FOURCC_RV15:
    case FOURCC_RV16:
    default:
	size = *width * 2;
	if (pitches != NULL) {
	    pitches[0] = size;
	}
	size *= *height;
	break;
    case FOURCC_RV24:
	size = *width * 3;
	if (pitches != NULL) {
	    pitches[0] = size;
	}
	size *= *height;
	break;
    case FOURCC_RV32:
	size = *width * 4;
	if (pitches != NULL) {
	    pitches[0] = size;
	}
	size *= *height;
	break;
    }

    LEAVE(size);
}


/******************************************************************************\
**									      **
**	S U P P O R T   F U N C T I O N S				      **
**									      **
\******************************************************************************/

static void
SMI_DisplayVideo(
	ScrnInfoPtr	pScrn,
	int		id,
	int		offset,
	short		width,
	short		height,
	int		pitch,
	int		x1,
	int		y1,
	int		x2,
	int		y2,
	BoxPtr		dstBox,
	short		vid_w,
	short		vid_h,
	short		drw_w,
	short		drw_h
)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    CARD32 vpr00;
    uint32_t hstretch, vstretch;

    ENTER();

    vpr00 = READ_VPR(pSmi, 0x00) & ~0x0CB800FF;

    switch (id) {
    case FOURCC_YV12:
    case FOURCC_I420:
    case FOURCC_YUY2:
	vpr00 |= 0x6;
	break;
    case FOURCC_RV15:
	vpr00 |= 0x1;
	break;
    case FOURCC_RV16:
	vpr00 |= 0x2;
	break;
    case FOURCC_RV24:
	vpr00 |= 0x4;
	break;
    case FOURCC_RV32:
	vpr00 |= 0x3;
	break;
    }

    if (drw_w > vid_w) {
	hstretch = ((uint32_t)(vid_w - 1) << 16) / (drw_w - 1);
    } else {
	hstretch = 0;
    }

    if (drw_h > vid_h) {
	vstretch = ((uint32_t)(vid_h - 1) << 16) / (drw_h - 1);
	vpr00 |= 1 << 21;
    } else {
	vstretch = 0;
    }

    WRITE_VPR(pSmi, 0x00, vpr00 | (1 << 3) | (1 << 20));
    WRITE_VPR(pSmi, 0x14, (dstBox->x1) | (dstBox->y1 << 16));
    WRITE_VPR(pSmi, 0x18, (dstBox->x2) | (dstBox->y2 << 16));
    WRITE_VPR(pSmi, 0x1C, offset >> 3);
    WRITE_VPR(pSmi, 0x20, (pitch >> 3) | ((pitch >> 3) << 16));
    WRITE_VPR(pSmi, 0x24, (hstretch & 0xff00) | ((vstretch & 0xff00) >> 8));
    if (pSmi->Chipset == SMI_LYNXEMplus) {	/* This one can store additional precision */
	WRITE_VPR(pSmi, 0x68, ((hstretch & 0xff) << 8) | (vstretch & 0xff));
    }

    LEAVE();
}

static void
SMI_DisplayVideo0501_CSC(ScrnInfoPtr pScrn, int id, int offset,
			 short width, short height, int pitch,
			 int x1, int y1, int x2, int y2, BoxPtr dstBox,
			 short vid_w, short vid_h, short drw_w, short drw_h,
			 RegionPtr clipboxes)
{
    int32_t	ScaleXn, ScaleXd, ScaleYn, ScaleYd;
    int32_t	SrcTn, SrcTd, SrcLn, SrcLd;
    int32_t	SrcRn, SrcBn;
    int32_t	SrcDimX, SrcDimY;
    int32_t	SrcYBase, SrcUBase, SrcVBase, SrcYPitch, SrcUVPitch;
    int32_t	DestPitch;
    SMIPtr	pSmi = SMIPTR(pScrn);
    BoxPtr	pbox = REGION_RECTS(clipboxes);
    int		i, nbox = REGION_NUM_RECTS(clipboxes);
    int32_t	rect_x, rect_y, rect_w, rect_h, csc;
    float	Hscale, Vscale;

    ENTER();

    SrcYBase = offset;
    SrcYPitch = pitch;

    DestPitch = (pScrn->displayWidth * pSmi->Bpp + 15) & ~15;

    Hscale = (vid_w - 1) / (float)(drw_w - 1);
    ScaleXn = Hscale;
    ScaleXd = ((vid_w - 1) << 13) / (drw_w - 1) - (ScaleXn << 13);

    Vscale = (vid_h - 1) / (float)(drw_h - 1);
    ScaleYn = Vscale;
    ScaleYd = ((vid_h - 1) << 13) / (drw_h - 1) - (ScaleYn << 13);

    /* CSC constants */
    WRITE_DPR(pSmi, 0xcc, 0);
    /* Use start of framebuffer as base offset */
    WRITE_DPR(pSmi, 0xf8, 0);

    csc = (1 << 31) | (1 << 25);
    if (pSmi->Bpp > 2)
	csc |= 1 << 26;

    switch (id) {
	case FOURCC_YV12:
	    SrcUVPitch = SrcYPitch / 2;
	    SrcVBase = SrcYBase + SrcYPitch * height;
	    SrcUBase = SrcVBase + SrcUVPitch * height / 2;
	    csc |= 2 << 28;
	    break;

	case FOURCC_I420:
	    SrcUVPitch = SrcYPitch / 2;
	    SrcUBase = SrcYBase + SrcYPitch * height;
	    SrcVBase = SrcUBase + SrcUVPitch * height / 2;
	    csc |= 2 << 28;
	    break;

	case FOURCC_YUY2:
	case FOURCC_RV16:
	case FOURCC_RV32:
	    SrcUBase = SrcVBase = SrcYBase;
	    SrcUVPitch = SrcYPitch;
	    break;

	default:
	    LEAVE();
    }

    WRITE_DPR(pSmi, 0xE4, ((SrcYPitch >> 4) << 16) | (SrcUVPitch >> 4));
    WRITE_DPR(pSmi, 0xC8, SrcYBase);
    WRITE_DPR(pSmi, 0xD8, SrcUBase);
    WRITE_DPR(pSmi, 0xDC, SrcVBase);
    WRITE_DPR(pSmi, 0xF4, (((ScaleXn << 13) | ScaleXd) << 16) |
	      (ScaleYn << 13 | ScaleYd));

    for (i = 0; i < nbox; i++, pbox++) {
	rect_x = pbox->x1;
	rect_y = pbox->y1;
	rect_w = pbox->x2 - pbox->x1;
	rect_h = pbox->y2 - pbox->y1;

	SrcLn = (rect_x - dstBox->x1) * Hscale;
	SrcLd = ((rect_x - dstBox->x1) << 13) * Hscale - (SrcLn << 13);
	SrcRn = (rect_x + rect_w - dstBox->x1) * Hscale;

	SrcTn = (rect_y - dstBox->y1) * Vscale;
	SrcTd = ((rect_y - dstBox->y1) << 13) * Vscale - (SrcTn << 13);
	SrcBn = (rect_y + rect_h - dstBox->y1) * Vscale;

	SrcDimX = SrcRn - SrcLn + 2;
	SrcDimY = SrcBn - SrcTn + 2;

	WRITE_DPR(pSmi, 0xD0, (SrcLn << 16) | SrcLd);
	WRITE_DPR(pSmi, 0xD4, (SrcTn << 16) | SrcTd);
	WRITE_DPR(pSmi, 0xE0, (SrcDimX << 16) | SrcDimY);
	WRITE_DPR(pSmi, 0xE8, (rect_x << 16) | rect_y);
	WRITE_DPR(pSmi, 0xEC, (rect_w << 16) | rect_h);
	WRITE_DPR(pSmi, 0xF0, ((DestPitch >> 4) << 16) | rect_h);

	while (READ_DPR(pSmi, 0xfc) & (1 << 31))
	    ;
	WRITE_DPR(pSmi, 0xfc, csc);
	/* CSC stop */
	while (READ_DPR(pSmi, 0xfc) & (1 << 31))
	    ;
    }

    LEAVE();
}

static void
SMI_DisplayVideo0501(ScrnInfoPtr pScrn,
		     int id,
		     int offset,
		     short width,
		     short height,
		     int pitch,
		     int x1,
		     int y1,
		     int x2,
		     int y2,
		     BoxPtr dstBox,
		     short vid_w, short vid_h, short drw_w, short drw_h)
{
    SMIPtr	pSmi = SMIPTR (pScrn);
    CARD32	dcr40;
    int		hstretch, vstretch;

    ENTER();

    dcr40 = READ_DCR(pSmi, 0x0040) & ~0x00003FFF;

    switch (id) {
	case FOURCC_YV12:
	case FOURCC_I420:
	case FOURCC_YUY2:
	    dcr40 |= 0x3;
	    break;

	case FOURCC_RV16:
	    dcr40 |= 0x1;
	    break;

	case FOURCC_RV32:
	    dcr40 |= 0x2;
	    break;
    }

    if (drw_w > vid_w) {	/*  Horizontal Stretch */
	hstretch = 4096 * vid_w / drw_w;
	dcr40 |= 1 << 8;
    }
    else {			/*  Horizontal Shrink */
	if (drw_w < (vid_w >> 1))
	    drw_w = vid_w >> 1;
	hstretch = (4096 * drw_w / vid_w) | 0x8000;
    }

    if (drw_h > vid_h) {	/* Vertical Stretch */
	vstretch = 4096 * vid_h / drw_h;
	dcr40 |= 1 << 9;
    }
    else {			/* Vertical Shrink */
	if (drw_h < (vid_h >> 1))
	    drw_h = vid_h >> 1;
	vstretch = (4096 * drw_h / vid_h) | 0x8000;
    }
    
    /* Set Color Key Enable bit */

    WRITE_DCR(pSmi, 0x0000, READ_DCR(pSmi, 0x0000) | (1 << 9));
    WRITE_DCR(pSmi, 0x0050, dstBox->x1 | (dstBox->y1 << 16));
    WRITE_DCR(pSmi, 0x0054, dstBox->x2 | (dstBox->y2 << 16));
    WRITE_DCR(pSmi, 0x0044, offset);

    WRITE_DCR(pSmi, 0x0048, pitch | (pitch << 16));
    WRITE_DCR(pSmi, 0x004C, offset + (pitch * height));
    WRITE_DCR(pSmi, 0x0058, (vstretch << 16) | hstretch);
    WRITE_DCR(pSmi, 0x005C, 0x00000000);
    WRITE_DCR(pSmi, 0x0060, 0x00EDEDED);

    WRITE_DCR(pSmi, 0x0040, dcr40 | (1 << 2));

    LEAVE();
}

static void
SMI_DisplayVideo0730(
	ScrnInfoPtr	pScrn,
	int		id,
	int		offset,
	short		width,
	short		height,
	int		pitch,
	int		x1,
	int		y1,
	int		x2,
	int		y2,
	BoxPtr		dstBox,
	short		vid_w,
	short		vid_h,
	short		drw_w,
	short		drw_h
)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    CARD32 fpr00;
    int hstretch, vstretch;

    ENTER();

    fpr00 = READ_FPR(pSmi, 0x00) & ~(FPR00_MASKBITS);

    switch (id) {
    case FOURCC_YV12:
    case FOURCC_I420:
    case FOURCC_YUY2:
	fpr00 |= FPR00_FMT_YUV422;
	break;
    case FOURCC_RV15:
	fpr00 |= FPR00_FMT_15P;
	break;
    case FOURCC_RV16:
	fpr00 |= FPR00_FMT_16P;
	break;
    case FOURCC_RV24:
	fpr00 |= FPR00_FMT_24P;
	break;
    case FOURCC_RV32:
	fpr00 |= FPR00_FMT_32P;
	break;
    }

    /* the formulas for calculating the stretch values do not match the
       documentation, but they're the same as the ddraw driver and they work */
    if (drw_w > vid_w) {
	hstretch = (8192 * vid_w / drw_w);
    } else {
	hstretch = 0;
    }

    if (drw_h > vid_h) {
	vstretch = (8192 * vid_h / drw_h);
    } else {
	vstretch = 0;
    }

    WRITE_FPR(pSmi, FPR00, fpr00 | FPR00_VWIENABLE | FPR00_VWIKEYENABLE);
    WRITE_FPR(pSmi, FPR14, (dstBox->x1) | (dstBox->y1 << 16));
    WRITE_FPR(pSmi, FPR18, (dstBox->x2) | (dstBox->y2 << 16));
    WRITE_FPR(pSmi, FPR1C, offset >> 3);
    WRITE_FPR(pSmi, FPR20, (pitch >> 3) | ((pitch >> 3) << 16));
    WRITE_FPR(pSmi, FPR24, (hstretch & 0xFF00) | ((vstretch & 0xFF00)>>8)); 
    WRITE_FPR(pSmi, FPR68, ((hstretch & 0x00FF)<<8) | (vstretch & 0x00FF)); 

    LEAVE();
}

static void
SMI_BlockHandler(BLOCKHANDLER_ARGS_DECL)
{
    SCREEN_PTR(arg);
    ScrnInfoPtr	pScrn	= xf86ScreenToScrn(pScreen);
    SMIPtr	pSmi    = SMIPTR(pScrn);
    SMI_PortPtr pPort = (SMI_PortPtr) pSmi->ptrAdaptor->pPortPrivates[0].ptr;

    pScreen->BlockHandler = pSmi->BlockHandler;
    (*pScreen->BlockHandler)(BLOCKHANDLER_ARGS);
    pScreen->BlockHandler = SMI_BlockHandler;

    if (pPort->videoStatus & TIMER_MASK) {
	UpdateCurrentTime();
        if (pPort->videoStatus & OFF_TIMER) {
            if (pPort->offTime < currentTime.milliseconds) {
		if (pSmi->Chipset == SMI_COUGAR3DR) {
		    WRITE_FPR(pSmi, FPR00, READ_FPR(pSmi, 0x00) & ~(FPR00_VWIENABLE));
		}
		else if (IS_MSOC(pSmi))
		    WRITE_DCR(pSmi, 0x0040, READ_DCR(pSmi, 0x0040) & ~0x00000004);
		else
		    WRITE_VPR(pSmi, 0x00, READ_VPR(pSmi, 0x00) & ~0x00000008);
                pPort->videoStatus = FREE_TIMER;
                pPort->freeTime = currentTime.milliseconds + FREE_DELAY;
	    }
	} else {
            if (pPort->freeTime < currentTime.milliseconds) {
		SMI_FreeMemory(pScrn, pPort->video_memory);
                pPort->video_memory = NULL;
	    }
            pPort->videoStatus = 0;
	}
    }
}

#if 0
static int
SMI_SendI2C(
	ScrnInfoPtr		pScrn,
	CARD8			device,
	char			*devName,
	SMI_I2CDataPtr	i2cData
)
{
    SMIPtr	pSmi = SMIPTR(pScrn);
    I2CDevPtr	dev;
    int		status = Success;

    ENTER();

    if (pSmi->I2C == NULL)
	LEAVE(BadAlloc);

    dev = xf86CreateI2CDevRec();
    if (dev == NULL)
	LEAVE(BadAlloc);

    dev->DevName = devName;
    dev->SlaveAddr = device;
    dev->pI2CBus = pSmi->I2C;

    if (!xf86I2CDevInit(dev))
	status = BadAlloc;
    else {
	while (i2cData->address != 0xFF || i2cData->data != 0xFF) {	/* PDR#676 */
	    if (!xf86I2CWriteByte(dev, i2cData->address, i2cData->data)) {
		status = BadAlloc;
		break;
	    }
	    i2cData++;
	}
    }

    xf86DestroyI2CDevRec(dev, TRUE);

    LEAVE(status);
}
#endif

/******************************************************************************\
**									      **
**	 O F F S C R E E N   M E M O R Y   M A N A G E R		      **
**									      **
\******************************************************************************/

static void
SMI_InitOffscreenImages(
	ScreenPtr	pScreen
)
{
    XF86OffscreenImagePtr offscreenImages;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    SMIPtr pSmi = SMIPTR(pScrn);
    SMI_PortPtr pPort = (SMI_PortPtr) pSmi->ptrAdaptor->pPortPrivates[0].ptr;

    ENTER();

    offscreenImages = malloc(sizeof(XF86OffscreenImageRec));
    if (offscreenImages == NULL) {
	LEAVE();
    }

    offscreenImages->image = SMI_VideoImages;
    offscreenImages->flags = VIDEO_OVERLAID_IMAGES;
    if (IS_MSOC(pSmi))
	offscreenImages->flags |= VIDEO_CLIP_TO_VIEWPORT;
    offscreenImages->alloc_surface = SMI_AllocSurface;
    offscreenImages->free_surface = SMI_FreeSurface;
    offscreenImages->display = SMI_DisplaySurface;
    offscreenImages->stop = SMI_StopSurface;
    offscreenImages->getAttribute = SMI_GetSurfaceAttribute;
    offscreenImages->setAttribute = SMI_SetSurfaceAttribute;
    offscreenImages->max_width = pSmi->lcdWidth;
    offscreenImages->max_height = pSmi->lcdHeight;
    if (!pPort->I2CDev.SlaveAddr) {
	offscreenImages->num_attributes = nElems(SMI_VideoAttributes);
	offscreenImages->attributes = SMI_VideoAttributes; 
    } else {
	offscreenImages->num_attributes = nElems(SMI_VideoAttributesSAA711x);
	offscreenImages->attributes = SMI_VideoAttributesSAA711x; 
    }
    xf86XVRegisterOffscreenImages(pScreen, offscreenImages, 1);

    LEAVE();
}

static void
SMI_VideoSave(ScreenPtr pScreen, ExaOffscreenArea *area)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    SMIPtr pSmi = SMIPTR(pScrn);
    SMI_PortPtr pPort = pSmi->ptrAdaptor->pPortPrivates[0].ptr;
	
    ENTER();

    if (pPort->video_memory == area)
	pPort->video_memory = NULL;

    LEAVE();
}

CARD32
SMI_AllocateMemory(ScrnInfoPtr pScrn, void **mem_struct, int size)
{
    ScreenPtr	pScreen = xf86ScrnToScreen(pScrn);
    SMIPtr	pSmi = SMIPTR(pScrn);
    int		offset = 0;

    ENTER();

    if (pSmi->useEXA) {
	ExaOffscreenArea *area = *mem_struct;

	if (area != NULL) {
	    if (area->size >= size)
		LEAVE(area->offset);

	    exaOffscreenFree(pScrn->pScreen, area);
	}

	area = exaOffscreenAlloc(pScrn->pScreen, size, 64, TRUE,
				 SMI_VideoSave, NULL);

	*mem_struct = area;
	if (area != NULL)
	    offset = area->offset;
    }
    else {
	FBLinearPtr	linear = *mem_struct;

	/*  XAA allocates in units of pixels at the screen bpp,
	 *  so adjust size appropriately.
	 */
	size = (size + pSmi->Bpp - 1) / pSmi->Bpp;

	if (linear) {
	    if (linear->size >= size)
		LEAVE(linear->offset * pSmi->Bpp);

	    if (xf86ResizeOffscreenLinear(linear, size))
		LEAVE(linear->offset * pSmi->Bpp);

	    xf86FreeOffscreenLinear(linear);
	}
	else {
	    int max_size;

	    xf86QueryLargestOffscreenLinear(pScreen, &max_size, 16,
					    PRIORITY_EXTREME);
	    if (max_size < size)
		LEAVE(0);

	    xf86PurgeUnlockedOffscreenAreas(pScreen);
	}

	linear = xf86AllocateOffscreenLinear(pScreen, size, 16,
					     NULL, NULL, NULL);
	if ((*mem_struct = linear) != NULL)
	    offset = linear->offset * pSmi->Bpp;

	DEBUG("offset = %p\n", offset);
    }

    LEAVE(offset);
}

void
SMI_FreeMemory(
	ScrnInfoPtr pScrn,
	void *mem_struct
)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    if (pSmi->useEXA) {
	ExaOffscreenArea *area = mem_struct;
		
	if (area != NULL) 
	    exaOffscreenFree(pScrn->pScreen, area);
    } else {
	FBLinearPtr linear = mem_struct;
		
	if (linear != NULL) 
	    xf86FreeOffscreenLinear(linear);
    }

    LEAVE();
}

static void
CopyYV12ToVideoMem(unsigned char *src1, unsigned char *src2,
		   unsigned char *src3, unsigned char *dst,
		   int src1Pitch, int src23Pitch, int dstPitch,
		   int height, int width)
{
    int		j = height;

    ENTER();

    /* copy 1 data */
    while (j -- > 0) {
	memcpy(dst, src1, width);
	src1 += src1Pitch;
	dst += dstPitch;
    }
    /* copy 2 data */
    j = height / 2;
    while (j -- > 0) {
	memcpy(dst, src2, width / 2);
	src2 += src23Pitch;
	dst += dstPitch / 2;
    }
    /* copy 3 data */
    j = height / 2;
    while (j -- > 0) {
	memcpy(dst, src3, width / 2);
	src3 += src23Pitch;
	dst += dstPitch / 2;
    }

    LEAVE();
}

static int
SMI_AllocSurface(
	ScrnInfoPtr	pScrn,
	int		id,
	unsigned short	width,
	unsigned short	height,
	XF86SurfacePtr	surface
)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    int pitch, bpp, offset, size;
    void *surface_memory = NULL;
    SMI_OffscreenPtr ptrOffscreen;

    ENTER();

    if (width > pSmi->lcdWidth || height > pSmi->lcdHeight)
	LEAVE(BadAlloc);

    switch (id) {
    case FOURCC_YV12:
    case FOURCC_I420:
    case FOURCC_YUY2:
    case FOURCC_RV15:
    case FOURCC_RV16:
	bpp = 2;
	break;
    case FOURCC_RV24:
	bpp = 3;
	break;
    case FOURCC_RV32:
	bpp = 4;
	break;
    default:
	LEAVE(BadAlloc);
    }

    width = (width + 1) & ~1;
    pitch = (width * bpp + 15) & ~15;
    size  = pitch * height;

    offset = SMI_AllocateMemory(pScrn, &surface_memory, size);
    if (offset == 0)
	LEAVE(BadAlloc);

    surface->pitches = malloc(sizeof(int));
    if (surface->pitches == NULL) {
	SMI_FreeMemory(pScrn, surface_memory);
	LEAVE(BadAlloc);
    }
    surface->offsets = malloc(sizeof(int));
    if (surface->offsets == NULL) {
	free(surface->pitches);
	SMI_FreeMemory(pScrn, surface_memory);
	LEAVE(BadAlloc);
    }

    ptrOffscreen = malloc(sizeof(SMI_OffscreenRec));
    if (ptrOffscreen == NULL) {
	free(surface->offsets);
	free(surface->pitches);
	SMI_FreeMemory(pScrn, surface_memory);
	LEAVE(BadAlloc);
    }

    surface->pScrn = pScrn;
    surface->id = id;
    surface->width = width;
    surface->height = height;
    surface->pitches[0] = pitch;
    surface->offsets[0] = offset;
    surface->devPrivate.ptr = (pointer) ptrOffscreen;

    ptrOffscreen->surface_memory = surface_memory;
    ptrOffscreen->isOn = FALSE;

    LEAVE(Success);
}

static int
SMI_FreeSurface(
	XF86SurfacePtr	surface
)
{
    ScrnInfoPtr pScrn = surface->pScrn;
    SMI_OffscreenPtr ptrOffscreen = (SMI_OffscreenPtr) surface->devPrivate.ptr;

    ENTER();

    if (ptrOffscreen->isOn) {
	SMI_StopSurface(surface);
    }

    SMI_FreeMemory(pScrn, ptrOffscreen->surface_memory);
    free(surface->pitches);
    free(surface->offsets);
    free(surface->devPrivate.ptr);

    LEAVE(Success);
}

static int
SMI_DisplaySurface(
	XF86SurfacePtr	surface,
	short		vid_x,
	short		vid_y,
	short		drw_x,
	short		drw_y,
	short		vid_w,
	short		vid_h,
	short		drw_w,
	short		drw_h,
	RegionPtr	clipBoxes
)
{
    SMI_OffscreenPtr ptrOffscreen = (SMI_OffscreenPtr) surface->devPrivate.ptr;
    SMIPtr pSmi = SMIPTR(surface->pScrn);
    SMI_PortPtr pPort = pSmi->ptrAdaptor->pPortPrivates[0].ptr;
    INT32 x1, y1, x2, y2;
    BoxRec dstBox;
    xf86CrtcConfigPtr crtcConf = XF86_CRTC_CONFIG_PTR(surface->pScrn);
    xf86CrtcPtr crtc;

    ENTER();

    x1 = vid_x;
    x2 = vid_x + vid_w;
    y1 = vid_y;
    y2 = vid_y + vid_h;

    dstBox.x1 = drw_x;
    dstBox.x2 = drw_x + drw_w;
    dstBox.y1 = drw_y;
    dstBox.y2 = drw_y + drw_h;

    if(!xf86_crtc_clip_video_helper(surface->pScrn, &crtc, crtcConf->crtc[0], &dstBox,
				    &x1, &x2, &y1, &y2, clipBoxes, surface->width, surface->height))
	LEAVE(Success);

    if (!crtc)
	LEAVE(Success);

    /* Transform dstBox to the CRTC coordinates */
    dstBox.x1 -= crtc->x;
    dstBox.y1 -= crtc->y;
    dstBox.x2 -= crtc->x;
    dstBox.y2 -= crtc->y;

    xf86XVFillKeyHelper(surface->pScrn->pScreen,
			pPort->Attribute[XV_COLORKEY], clipBoxes);
    SMI_ResetVideo(surface->pScrn);

    if (pSmi->Chipset == SMI_COUGAR3DR)
	SMI_DisplayVideo0730(surface->pScrn, surface->id, surface->offsets[0],
			     surface->width, surface->height, surface->pitches[0], x1, y1, x2,
			     y2, &dstBox, vid_w, vid_h, drw_w, drw_h);
    else if (IS_MSOC(pSmi))
	SMI_DisplayVideo0501(surface->pScrn, surface->id,
			     surface->offsets[0], surface->width,
			     surface->height, surface->pitches[0], x1, y1,
			     x2, y2, &dstBox, vid_w, vid_h, drw_w, drw_h);
    else{
	if(crtc == crtcConf->crtc[0])
	    SMI_DisplayVideo(surface->pScrn, surface->id, surface->offsets[0],
			     surface->width, surface->height, surface->pitches[0], x1, y1, x2,
			     y2, &dstBox, vid_w, vid_h, drw_w, drw_h);
    }

    ptrOffscreen->isOn = TRUE;
    if (pPort->videoStatus & CLIENT_VIDEO_ON) {
        REGION_EMPTY(surface->pScrn->pScreen, &pPort->clip);
	UpdateCurrentTime();
        pPort->videoStatus = FREE_TIMER;
        pPort->freeTime = currentTime.milliseconds + FREE_DELAY;
    }

    LEAVE(Success);
}

static int
SMI_StopSurface(
	XF86SurfacePtr	surface
)
{
    SMI_OffscreenPtr ptrOffscreen = (SMI_OffscreenPtr) surface->devPrivate.ptr;

    ENTER();

    if (ptrOffscreen->isOn) {
	SMIPtr pSmi = SMIPTR(surface->pScrn);
	if (pSmi->Chipset == SMI_COUGAR3DR) {
	    WRITE_FPR(pSmi, FPR00, READ_FPR(pSmi, 0x00) & ~(FPR00_VWIENABLE));
	} else {
	    WRITE_VPR(pSmi, 0x00, READ_VPR(pSmi, 0x00) & ~0x00000008);
	}

	ptrOffscreen->isOn = FALSE;
    }

    LEAVE(Success);
}

static int
SMI_GetSurfaceAttribute(
	ScrnInfoPtr	pScrn,
	Atom		attr,
	INT32		*value
)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    return SMI_GetPortAttribute(pScrn, attr, value,
			(pointer) pSmi->ptrAdaptor->pPortPrivates[0].ptr);
}

static int
SMI_SetSurfaceAttribute(
	ScrnInfoPtr	pScrn,
	Atom		attr,
	INT32		value
)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    return SMI_SetPortAttribute(pScrn, attr, value,
			(pointer) pSmi->ptrAdaptor->pPortPrivates[0].ptr);
}

static void
SetKeyReg(SMIPtr pSmi, int reg, int value)
{
    if (pSmi->Chipset == SMI_COUGAR3DR)
	WRITE_FPR(pSmi, reg, value);
    else if (IS_MSOC(pSmi)) {
	/* We don't change the color mask, and we don't do brightness.  IF
	 * they write to the colorkey register, we'll write the value to the
	 * 501 colorkey register */
	if (FPR04 == reg)		   /* Only act on colorkey value writes */
	    WRITE_DCR(pSmi, 0x0008, value);/* ColorKey register is DCR08 */
    }
    else
	WRITE_VPR(pSmi, reg, value);
}

#else /* SMI_USE_VIDEO */
void SMI_InitVideo(ScreenPtr pScreen) {}
#endif
