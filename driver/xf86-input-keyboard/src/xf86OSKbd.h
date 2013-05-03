/*
 * Copyright (c) 2002-2003 by The XFree86 Project, Inc.
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
 *
 * Author: Ivan Pascal.
 */

#include "xf86Xinput.h"

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 18
#define LogMessageVerbSigSafe xf86MsgVerb
#endif

Bool ATScancode(InputInfoPtr pInfo, int *scanCode);

/* Public interface to OS-specific keyboard support. */

typedef	int	(*KbdInitProc)(InputInfoPtr pInfo, int what);
typedef	int	(*KbdOnProc)(InputInfoPtr pInfo, int what);
typedef	int	(*KbdOffProc)(InputInfoPtr pInfo, int what);
typedef	void	(*BellProc)(InputInfoPtr pInfo,
                            int loudness, int pitch, int duration);
typedef	void	(*SetLedsProc)(InputInfoPtr pInfo, int leds);
typedef	int	(*GetLedsProc)(InputInfoPtr pInfo);
typedef	void	(*KbdGetMappingProc)(InputInfoPtr pInfo,
                                     KeySymsPtr pKeySyms, CARD8* pModMap);
typedef	int	(*RemapScanCodeProc)(InputInfoPtr pInfo, int *scanCode);
typedef	Bool	(*OpenKeyboardProc)(InputInfoPtr pInfo);
typedef	void	(*PostEventProc)(InputInfoPtr pInfo,
                                 unsigned int key, Bool down);
typedef struct {
    int                 begin;
    int                 end;
    unsigned char       *map;
} TransMapRec, *TransMapPtr;

typedef struct {
    KbdInitProc		KbdInit;
    KbdOnProc		KbdOn;
    KbdOffProc		KbdOff;
    BellProc		Bell;
    SetLedsProc		SetLeds;
    GetLedsProc		GetLeds;
    KbdGetMappingProc	KbdGetMapping;
    RemapScanCodeProc	RemapScanCode;

    OpenKeyboardProc	OpenKeyboard;
    PostEventProc	PostEvent;

    unsigned long	leds;
    unsigned long	xledsMask;
    unsigned long	keyLeds;
    int			scanPrefix;
    Bool		CustomKeycodes;
    Bool		isConsole;
    TransMapPtr         scancodeMap;
    TransMapPtr         specialMap;

    /* os specific */
    pointer		private;
    int			consType;
    int			wsKbdType;

} KbdDevRec, *KbdDevPtr;

typedef enum {
    PROT_STD,
    PROT_WSCONS,
    PROT_UNKNOWN_KBD
} KbdProtocolId;

typedef struct {
    const char		*name;
    KbdProtocolId	id;
} KbdProtocolRec;

Bool xf86OSKbdPreInit(InputInfoPtr pInfo);
