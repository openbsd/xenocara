/* $XFree86$ */ /* -*- mode: c; c-basic-offset: 3 -*- */
/*
 * Copyright 2000 Gareth Hughes
 * All Rights Reserved.
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
 * GARETH HUGHES BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Authors:
 *   Gareth Hughes <gareth@valinux.com>
 *   Leif Delgass <ldelgass@retinalburn.net>
 */

#ifndef __ATIDRI_H__
#define __ATIDRI_H__ 1

/* DRI driver defaults */
#define ATI_DEFAULT_AGP_SIZE     8 /* MB (must be a power of 2 and > 4MB) */
#define ATI_DEFAULT_AGP_MODE     1
#define ATI_DEFAULT_BUFFER_SIZE  2 /* MB (must be page aligned) */

#define ATI_AGP_MAX_MODE	 2

/* Imported from the radeon suspend code writen by cpbotha@ieee.org
 * to enable suspend/resume support for the mach64 card.
 */
extern void ATIDRIResume(ScreenPtr pScreen);
extern Bool ATIDRIScreenInit(ScreenPtr);
extern Bool ATIDRIFinishScreenInit(ScreenPtr);
extern void ATIDRICloseScreen(ScreenPtr);

#endif /* __ATIDRI_H__ */
