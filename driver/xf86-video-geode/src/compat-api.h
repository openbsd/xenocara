/*
 * Copyright 2012 Red Hat, Inc.
 * Copyright 2012 Canonical Ltd.
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
 * Authors:
 *   Dave Airlie <airlied@redhat.com>
 *   Maarten Lankhorst <maarten.lankhorst@canonical.com>
 */

/* this file provides API compat between server post 1.13 and pre it,
   it should be reused inside as many drivers as possible */
#ifndef COMPAT_API_H
#define COMPAT_API_H

#ifndef GLYPH_HAS_GLYPH_PICTURE_ACCESSOR
#define GetGlyphPicture(g, s) GlyphPicture((g))[(s)->myNum]
#define SetGlyphPicture(g, s, p) GlyphPicture((g))[(s)->myNum] = p
#endif

#ifndef XF86_HAS_SCRN_CONV
#define xf86ScreenToScrn(s) xf86Screens[(s)->myNum]
#define xf86ScrnToScreen(s) screenInfo.screens[(s)->scrnIndex]
#endif

#ifndef XF86_SCRN_INTERFACE

#define DDC_CALL(pScrni) (pScrni->scrnIndex)

#define SCRN_ARG_TYPE int
#define SCRN_INFO_PTR(arg1) ScrnInfoPtr pScrni = xf86Screens[(arg1)]

#define SCREEN_ARG_TYPE int
#define SCREEN_PTR(arg1) ScreenPtr pScrn = screenInfo.screens[(arg1)]

#define SCREEN_INIT_ARGS_DECL int index, ScreenPtr pScrn, int argc, char **argv

#define BLOCKHANDLER_ARGS_DECL int arg, pointer blockData, pointer pTimeout, pointer pReadmask
#define BLOCKHANDLER_ARGS arg, blockData, pTimeout, pReadmask

#define CLOSE_SCREEN_ARGS_DECL int scrnIndex, ScreenPtr pScrn
#define CLOSE_SCREEN_ARGS scrnIndex, pScrn

#define ADJUST_FRAME_ARGS_DECL int arg, int x, int y, int flags
#define ADJUST_FRAME_ARGS(x, y) pScrni->scrnIndex, (x), (y), 0

#define SWITCH_MODE_ARGS_DECL int arg, DisplayModePtr pMode, int flags

#define FREE_SCREEN_ARGS_DECL int arg, int flags
#define FREE_SCREEN_ARGS(x) (x)->scrnIndex, 0

#define VT_FUNC_ARGS_DECL int arg, int flags
#define VT_FUNC_ARGS(flags) pScrni->scrnIndex, (flags)

#define XF86_ENABLEDISABLEFB_ARG(pScrni, x) ((pScrni)->scrnIndex), (x)

#define POINTER_MOVED_ARGS_DECL int arg, int x, int y
#define POINTER_MOVED_ARGS(x, y) pScrni->scrnIndex, (x), (y)

#define VALID_MODE_ARGS_DECL int arg, DisplayModePtr pMode, Bool Verbose, int flags

#else                           /*XF86_SCRN_INTERFACE) */

#define DDC_CALL(pScrni) (pScrni)

#define SCRN_ARG_TYPE ScrnInfoPtr
#define SCRN_INFO_PTR(arg1) ScrnInfoPtr pScrni = (arg1)

#define SCREEN_ARG_TYPE ScreenPtr
#define SCREEN_PTR(arg1) ScreenPtr pScrn = (arg1)

#define SCREEN_INIT_ARGS_DECL ScreenPtr pScrn, int argc, char **argv

#if ABI_VIDEODRV_VERSION >= SET_ABI_VERSION(23, 0)
#define BLOCKHANDLER_ARGS_DECL ScreenPtr arg, pointer pTimeout
#define BLOCKHANDLER_ARGS arg, pTimeout
#else
#define BLOCKHANDLER_ARGS_DECL ScreenPtr arg, pointer pTimeout, pointer pReadmask
#define BLOCKHANDLER_ARGS arg, pTimeout, pReadmask
#endif

#define CLOSE_SCREEN_ARGS_DECL ScreenPtr pScrn
#define CLOSE_SCREEN_ARGS pScrn

#define ADJUST_FRAME_ARGS_DECL ScrnInfoPtr arg, int x, int y
#define ADJUST_FRAME_ARGS(x, y) pScrni, (x), (y)

#define SWITCH_MODE_ARGS_DECL ScrnInfoPtr arg, DisplayModePtr pMode

#define FREE_SCREEN_ARGS_DECL ScrnInfoPtr arg
#define FREE_SCREEN_ARGS(x) (x)

#define VT_FUNC_ARGS_DECL ScrnInfoPtr arg
#define VT_FUNC_ARGS(flags) pScrni

#define XF86_ENABLEDISABLEFB_ARG(pScrni, x) (pScrni), (x)

#define POINTER_MOVED_ARGS_DECL ScrnInfoPtr arg, int x, int y
#define POINTER_MOVED_ARGS(x, y) pScrni, (x), (y)

#define VALID_MODE_ARGS_DECL ScrnInfoPtr arg, DisplayModePtr pMode, Bool Verbose, int flags

#endif

#endif
