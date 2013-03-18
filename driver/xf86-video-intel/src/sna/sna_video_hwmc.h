/*
 * Copyright © 2007 Intel Corporation
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
 * Authors:
 *    Zhenyu Wang <zhenyu.z.wang@sna.com>
 *
 */
#ifndef SNA_VIDEO_HWMC_H
#define SNA_VIDEO_HWMC_H

#define SNA_XVMC_LIBNAME	"IntelXvMC"
#define SNA_XVMC_MAJOR	0
#define SNA_XVMC_MINOR	1
#define SNA_XVMC_PATCHLEVEL	0

/* hw xvmc support type */
#define XVMC_I915_MPEG2_MC	0x01
#define XVMC_I965_MPEG2_MC	0x02
#define XVMC_I945_MPEG2_VLD	0x04
#define XVMC_I965_MPEG2_VLD	0x08

#ifdef _SNA_XVMC_SERVER_
#include <xf86xvmc.h>
Bool sna_video_xvmc_setup(struct sna *sna,
			  ScreenPtr screen,
			  XF86VideoAdaptorPtr target);
#endif

#endif
