/***************************************************************************
 
Copyright 2000 Intel Corporation.  All Rights Reserved. 

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the 
"Software"), to deal in the Software without restriction, including 
without limitation the rights to use, copy, modify, merge, publish, 
distribute, sub license, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so, subject to 
the following conditions: 

The above copyright notice and this permission notice (including the 
next paragraph) shall be included in all copies or substantial portions 
of the Software. 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. 
IN NO EVENT SHALL INTEL, AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

#include "xf86.h"
#include "xf86_OSproc.h"

typedef struct {
   CARD32 YBuf0offset;
   CARD32 UBuf0offset;
   CARD32 VBuf0offset;

   CARD32 YBuf1offset;
   CARD32 UBuf1offset;
   CARD32 VBuf1offset;

   unsigned char currentBuf;

   int brightness;
   int contrast;
   int pipe;
   int doubleBuffer;

   RegionRec clip;
   CARD32 colorKey;

   CARD32 gamma0;
   CARD32 gamma1;
   CARD32 gamma2;
   CARD32 gamma3;
   CARD32 gamma4;
   CARD32 gamma5;

   CARD32 videoStatus;
   Time offTime;
   Time freeTime;
   FBLinearPtr linear;

   Bool overlayOK;
   int oneLineMode;
   int scaleRatio;
   Bool textured;
} I830PortPrivRec, *I830PortPrivPtr;

#define GET_PORT_PRIVATE(pScrn) \
   (I830PortPrivPtr)((I830PTR(pScrn))->adaptor->pPortPrivates[0].ptr)

void I915DisplayVideoTextured(ScrnInfoPtr pScrn, I830PortPrivPtr pPriv,
			      int id, RegionPtr dstRegion, short width,
			      short height, int video_pitch,
			      int x1, int y1, int x2, int y2,
			      short src_w, short src_h,
			      short drw_w, short drw_h,
			      DrawablePtr pDraw);
