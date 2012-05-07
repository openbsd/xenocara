/*
 * Copyright 2011 VMWare, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 */
#ifndef _VMWARE_BOOTSTRAP_H_
#define _VMWARE_BOOTSTRAP_H_

#include <xf86.h>

#define VMWARE_INCHTOMM 25.4

typedef enum {
    OPTION_HW_CURSOR,
    OPTION_XINERAMA,
    OPTION_STATIC_XINERAMA,
    OPTION_GUI_LAYOUT,
    OPTION_DEFAULT_MODE,
    OPTION_RENDER_ACCEL,
    OPTION_DRI,
    OPTION_DIRECT_PRESENTS,
    OPTION_HW_PRESENTS,
    OPTION_RENDERCHECK
} VMWAREOpts;

OptionInfoPtr VMWARECopyOptions(void);

void
vmwlegacy_hookup(ScrnInfoPtr pScrn);

#ifdef BUILD_VMWGFX
void
vmwgfx_hookup(ScrnInfoPtr pScrn);
#endif /* defined(BUILD_VMWGFX) */

#ifdef XFree86LOADER
void
VMWARERefSymLists(void);
#endif	/* XFree86LOADER */

/*#define DEBUG_LOGGING*/
#ifdef DEBUG_LOGGING
# define VmwareLog(args) ErrorF args
# define TRACEPOINT VmwareLog(("%s : %s\n", __FUNCTION__, __FILE__));
#else
# define VmwareLog(args)
# define TRACEPOINT
#endif

#endif
