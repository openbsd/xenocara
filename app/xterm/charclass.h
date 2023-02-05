/* $XTermId: charclass.h,v 1.9 2023/01/04 09:28:05 tom Exp $ */

/* $XFree86: xc/programs/xterm/charclass.h,v 1.3 2006/02/13 01:14:58 dickey Exp $ */

/*
 * Copyright 2006-2020,2023 by Thomas E. Dickey
 *
 *                         All Rights Reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization.
 */

#ifndef CHARCLASS_H
#define CHARCLASS_H

typedef enum {
    IDENT = -1,
    OTHER = 0,
    CNTRL = 1,
    ALNUM = 48,
    BLANK = 32,
    U_CJK = 0x4e00,
    U_SUP = 0x2070,
    U_SUB = 0x2080,
    U_HIR = 0x3040,
    U_KAT = 0x30a0,
    U_HAN = 0xac00
} Classes;

extern void init_classtab(void);
/* initialise the table. needs calling before either of the 
   others. */

extern int SetCharacterClassRange(int low, int high, int value);
extern int CharacterClass(int c);

#if OPT_REPORT_CCLASS
extern void report_wide_char_class(void);
#endif

#ifdef NO_LEAKS
extern void noleaks_CharacterClass(void);
#endif

#endif
