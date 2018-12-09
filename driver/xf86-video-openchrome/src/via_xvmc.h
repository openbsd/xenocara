/*****************************************************************************
 * VIA Unichrome XvMC extension X server driver.
 *
 * Copyright (c) 2004 The Unichrome project. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHOR(S) OR COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _VIA_XVMC_H
#define _VIA_XVMC_H 1

#include "via_drmclient.h"
#include "via_drm.h"

/*
 * This file contains the common definitions between the XvMC lib and the
 * X server side. It is referenced also by the XvMC lib. Make sure any
 * structure change is reflected also in a change in minor version number!!
 */

#define VIAXVMC_MAJOR 0
#define VIAXVMC_MINOR 10
#define VIAXVMC_PL    0
#define VIA_NUM_XVMC_ATTRIBUTES 6
#define VIA_XVMC_VALID 0x80000000

/*
 * Commands that client submits through XvPutImage:
 */

#define VIA_XVMC_COMMAND_FDISPLAY 0
#define VIA_XVMC_COMMAND_DISPLAY 1
#define VIA_XVMC_COMMAND_UNDISPLAY 2
#define VIA_XVMC_COMMAND_ATTRIBUTES 3

typedef drm_via_sarea_t ViaXvMCSAreaPriv;

typedef struct
{
    INT32 attribute;
    INT32 value;
} ViaAttrPair;

typedef struct
{
    unsigned numAttr;
    ViaAttrPair attributes[VIA_NUM_XVMC_ATTRIBUTES];
} ViaXvMCAttrHolder;

/*
 * Passed from client to X server during overlay updates.
 */

typedef struct
{
    unsigned command;
    unsigned ctxNo;
    unsigned srfNo;
    unsigned subPicNo;
    ViaXvMCAttrHolder attrib;
    unsigned pad;
} ViaXvMCCommandBuffer;

/*
 * Passed from X server to client at context creation.
 */

typedef struct
{
    unsigned int ctxNo;
    unsigned int major, minor, pl;
    unsigned xvmc_port;
    unsigned int fbOffset;
    unsigned int fbSize;
    unsigned int mmioOffset;
    unsigned int mmioSize;
    unsigned int sAreaSize;
    unsigned int sAreaPrivOffset;
    ViaXvMCAttrHolder initAttrs;
    int useAGP;
    unsigned int chipId;
    unsigned int screen;
    unsigned int depth;
    unsigned int stride;
    unsigned int pad;
} ViaXvMCCreateContextRec;

#endif
