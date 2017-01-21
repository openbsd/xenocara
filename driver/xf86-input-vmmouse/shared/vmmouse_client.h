/*
 * Copyright 2002-2006 by VMware, Inc.
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */

/*
 * vmmouse_client.h --
 *
 *     VMware Virtual Mouse Client
 *
 * This module provides functions to enable/disable, operate and
 * process packets via the VMMouse absolute positioning module
 * hosted in the VMX.
 *
 */

#ifndef _VMMOUSE_CLIENT_H_
#define _VMMOUSE_CLIENT_H_

#include "xorg-server.h"
#include "xf86_OSproc.h"

/*
 * VMMouse Input packet data structure
 */
typedef struct _VMMOUSE_INPUT_DATA {
   unsigned short Flags;
   unsigned short Buttons;
   int X;
   int Y;
   int Z;
} VMMOUSE_INPUT_DATA, *PVMMOUSE_INPUT_DATA;

/*
 * Public Functions
 */
Bool VMMouseClient_Enable(void);
void VMMouseClient_Disable(void);
unsigned int VMMouseClient_GetInput(PVMMOUSE_INPUT_DATA pvmmouseInput);
void VMMouseClient_RequestRelative(void);
void VMMouseClient_RequestAbsolute(void);

#ifdef VMX86_DEVEL
#define VMwareLog(args) ErrorF args
#else
#define VMwareLog(args)
#endif

#include "vmmouse_defs.h"

#endif /* _VMMOUSE_CLIENT_H_ */

