/*
 * Copyright 1999, 2000 ATI Technologies Inc., Markham, Ontario,
 *                      Precision Insight, Inc., Cedar Park, Texas, and
 *                      VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, PRECISION INSIGHT, VA LINUX
 * SYSTEMS AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Authors:
 *   Rickard E. Faith <faith@valinux.com>
 *   Kevin E. Martin <martin@valinux.com>
 *
 */

#ifndef _R128_ROP_H_
#define _R128_ROP_H_

#include "r128_reg.h"

static struct {
    int rop;
    int pattern;
} R128_ROP[] = {
    { R128_ROP3_ZERO, R128_ROP3_ZERO }, /* GXclear        */
    { R128_ROP3_DSa,  R128_ROP3_DPa  }, /* Gxand          */
    { R128_ROP3_SDna, R128_ROP3_PDna }, /* GXandReverse   */
    { R128_ROP3_S,    R128_ROP3_P    }, /* GXcopy         */
    { R128_ROP3_DSna, R128_ROP3_DPna }, /* GXandInverted  */
    { R128_ROP3_D,    R128_ROP3_D    }, /* GXnoop         */
    { R128_ROP3_DSx,  R128_ROP3_DPx  }, /* GXxor          */
    { R128_ROP3_DSo,  R128_ROP3_DPo  }, /* GXor           */
    { R128_ROP3_DSon, R128_ROP3_DPon }, /* GXnor          */
    { R128_ROP3_DSxn, R128_ROP3_PDxn }, /* GXequiv        */
    { R128_ROP3_Dn,   R128_ROP3_Dn   }, /* GXinvert       */
    { R128_ROP3_SDno, R128_ROP3_PDno }, /* GXorReverse    */
    { R128_ROP3_Sn,   R128_ROP3_Pn   }, /* GXcopyInverted */
    { R128_ROP3_DSno, R128_ROP3_DPno }, /* GXorInverted   */
    { R128_ROP3_DSan, R128_ROP3_DPan }, /* GXnand         */
    { R128_ROP3_ONE,  R128_ROP3_ONE  }  /* GXset          */
};
#endif

