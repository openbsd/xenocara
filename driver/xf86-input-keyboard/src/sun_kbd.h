/*
 * Copyright (c) 2007, Oracle and/or its affiliates. All rights reserved.
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
 */

#ifndef _XORG_SUN_KBD_H_
#define _XORG_SUN_KBD_H_

typedef struct {
    int 		ktype;		/* Keyboard type from KIOCTYPE */
    Bool		kbdActive;	/* Have we set kbd modes for X? */
    int 		otranslation;	/* Original translation mode */
    int 		odirect;	/* Original "direct" mode setting */
    int			oleds;		/* Original LED state */
    const char *	strmod;		/* Streams module pushed on kbd device */
    OsTimerPtr		remove_timer;	/* Callback for removal on ENODEV */
} sunKbdPrivRec, *sunKbdPrivPtr;

/* sun_kbdMap.c */
extern void KbdGetMapping 	(InputInfoPtr pInfo, KeySymsPtr pKeySyms,
				 CARD8 *pModMap);
#endif
