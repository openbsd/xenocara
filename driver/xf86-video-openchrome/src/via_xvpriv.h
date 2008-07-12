/*
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
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _VIA_XVPRIV_H_
#define _VIA_XVPRIV_H_ 1

#include "xf86xv.h"

enum
{ XV_ADAPT_SWOV = 0,
    XV_ADAPT_NUM
};

typedef enum
{
    xve_none = 0,
    xve_bandwidth,
    xve_dmablit,
    xve_mem,
    xve_general,
    xve_adaptor,
    xve_numerr
} XvError;

#define VIA_MAX_XV_PORTS 1

typedef struct
{
    unsigned char xv_adaptor;
    unsigned char xv_portnum;
    int adaptor;
    int brightness;
    int saturation;
    int contrast;
    int hue;
    RegionRec clip;
    CARD32 colorKey;
    Bool autoPaint;

    CARD32 FourCC;		       /* from old SurfaceDesc -- passed down from viaPutImageG */

    /* store old video source & dst data */
    short old_src_x;
    short old_src_y;
    short old_src_w;
    short old_src_h;

    short old_drw_x;
    short old_drw_y;
    short old_drw_w;
    short old_drw_h;

    void *xvmc_priv;

    /*
     * For PCI DMA image transfer to frame-buffer memory.
     */

    unsigned char *dmaBounceBuffer;
    unsigned dmaBounceStride;
    unsigned dmaBounceLines;
    XvError xvErr;

} viaPortPrivRec, *viaPortPrivPtr;

extern viaPortPrivPtr viaPortPriv[];
extern unsigned viaNumXvPorts;

#endif /* _VIA_XVPRIV_H_ */
