/*
 * Copyright 2010 Christian König
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 * Based on vl_hwmc.c from xf86-video-nouveau
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xf86.h>
#include "radeon_video.h"
#include "compat-api.h"

#include <X11/extensions/Xv.h>
#include <X11/extensions/XvMC.h>
#include "fourcc.h"

#define FOURCC_RGB	0x0000003

static int subpicture_index_list[] =
{
	FOURCC_RGB,
	FOURCC_IA44,
	FOURCC_AI44
};

static XF86MCImageIDList subpicture_list =
{
	3,
	subpicture_index_list
};

static XF86MCSurfaceInfoRec yv12_mpeg2_surface =
{
	FOURCC_YV12,
	XVMC_CHROMA_FORMAT_420,
	0,
	2048,
	2048,
	2048,
	2048,
	XVMC_IDCT | XVMC_MOCOMP | XVMC_MPEG_2,
	XVMC_SUBPICTURE_INDEPENDENT_SCALING | XVMC_BACKEND_SUBPICTURE,
	&subpicture_list
};

static XF86MCSurfaceInfoPtr surfaces[] =
{
	(XF86MCSurfaceInfoPtr)&yv12_mpeg2_surface,
};

static XF86ImageRec rgb_subpicture =
{
	FOURCC_RGB,
	XvRGB,
	LSBFirst,
	{
		'R', 'G', 'B', 0x00,
		0x00,0x00,0x00,0x10,0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71
	},
	32,
	XvPacked,
	1,
	24, 0x00FF0000, 0x0000FF00, 0x000000FF,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	{
		'B','G','R','X',
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	},
	XvTopToBottom
};

static XF86ImageRec ia44_subpicture = XVIMAGE_IA44;
static XF86ImageRec ai44_subpicture = XVIMAGE_AI44;

static XF86ImagePtr subpictures[] =
{
	(XF86ImagePtr)&rgb_subpicture,
	(XF86ImagePtr)&ia44_subpicture,
	(XF86ImagePtr)&ai44_subpicture
};

static XF86MCAdaptorRec adaptor_template =
{
	"",
	1,
	surfaces,
	3,
	subpictures,
	(xf86XvMCCreateContextProcPtr)NULL,
	(xf86XvMCDestroyContextProcPtr)NULL,
	(xf86XvMCCreateSurfaceProcPtr)NULL,
	(xf86XvMCDestroySurfaceProcPtr)NULL,
	(xf86XvMCCreateSubpictureProcPtr)NULL,
	(xf86XvMCDestroySubpictureProcPtr)NULL
};

XF86MCAdaptorPtr
RADEONCreateAdaptorXvMC(ScreenPtr pScreen, char *xv_adaptor_name)
{
	XF86MCAdaptorPtr	adaptor;
	ScrnInfoPtr		pScrn;

	assert(pScreen);

	pScrn = xf86ScreenToScrn(pScreen);
	adaptor = xf86XvMCCreateAdaptorRec();

	if (!adaptor)
	{
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "[XvMC] Memory allocation failed.\n");
		return NULL;
	}

	*adaptor = adaptor_template;
	adaptor->name = xv_adaptor_name;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[XvMC] Associated with %s.\n", adaptor->name);

	return adaptor;
}
