/* $XTermId: fontutils.h,v 1.89 2013/12/09 12:18:01 tom Exp $ */

/*
 * Copyright 1998-2011,2013 by Thomas E. Dickey
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

#ifndef included_fontutils_h
#define included_fontutils_h 1

#include <xterm.h>
/* *INDENT-OFF* */

extern Bool xtermLoadDefaultFonts (XtermWidget /* xw */);
extern Bool xtermOpenFont (XtermWidget /* xw */, const char */* name */, XTermFonts * /* result */, fontWarningTypes /* warn */, Bool /* force */);
extern XTermFonts * xtermCloseFont (XtermWidget /* xw */, XTermFonts * /* fnt */);
extern const VTFontNames * xtermFontName (const char */* normal */);
extern int lookupRelativeFontSize (XtermWidget /* xw */, int /* old */, int /* relative */);
extern int xtermGetFont(const char * /* param */);
extern int xtermLoadFont (XtermWidget /* xw */, const VTFontNames */* fonts */, Bool /* doresize */, int /* fontnum */);
extern void HandleSetFont PROTO_XT_ACTIONS_ARGS;
extern void SetVTFont (XtermWidget /* xw */, int /* i */, Bool /* doresize */, const VTFontNames */* fonts */);
extern void xtermCloseFonts (XtermWidget /* xw */, XTermFonts * /* fnts[fMAX] */);
extern void xtermComputeFontInfo (XtermWidget /* xw */, VTwin */* win */, XFontStruct */* font */, int /* sbwidth */);
extern void xtermCopyFontInfo (XTermFonts * /* target */, XTermFonts * /* source */);
extern void xtermFreeFontInfo (XTermFonts * /* target */);
extern void xtermSaveFontInfo (TScreen * /* screen */, XFontStruct */* font */);
extern void xtermSetCursorBox (TScreen * /* screen */);
extern void xtermUpdateFontInfo (XtermWidget /* xw */, Bool /* doresize */);

#if OPT_DEC_CHRSET
extern char *xtermSpecialFont (TScreen */* screen */, unsigned /* atts */, unsigned /* chrset */);
#endif

#define FontIsIncomplete(font) \
	((font)->fs != 0 \
	 && (font)->fs->per_char != 0 \
	 && !(font)->fs->all_chars_exist)

#if OPT_BOX_CHARS

#define ForceBoxChars(screen,ch) \
	(xtermIsDecGraphic(ch) \
	 && (screen)->force_box_chars)

	 /*
	  * Keep track of (some) characters to make the check for missing
	  * characters faster.  If the character is known to be missing,
	  * the cache value is '2'.  If we have checked the character, the
	  * cached value is '1'.
	  */
#if OPT_WIDE_CHARS
#define CheckedKnownMissing(font, ch) \
	 (((ch) < KNOWN_MISSING) && ((font)->known_missing[(Char)(ch)] > 0))
#else
#define CheckedKnownMissing(font, ch) \
	 ((font)->known_missing[(Char)(ch)] > 0)
#endif

#define IsXtermMissingChar(screen, ch, font) \
	 (CheckedKnownMissing(font, ch) \
	  ? ((font)->known_missing[(Char)(ch)] > 1) \
	  : ((FontIsIncomplete(font) && xtermMissingChar(ch, font)) \
	   || ForceBoxChars(screen, ch)))

extern void xtermDrawBoxChar (XtermWidget /* xw */, unsigned /* ch */, unsigned /* flags */, GC /* gc */, int /* x */, int /* y */, int /* cols */);
#else
#define IsXtermMissingChar(screen, ch, font) False
#endif

#if OPT_BOX_CHARS || OPT_REPORT_FONTS 
extern Bool xtermMissingChar (unsigned /* ch */, XTermFonts */* font */);
#endif

#if OPT_LOAD_VTFONTS
extern void HandleLoadVTFonts PROTO_XT_ACTIONS_ARGS;
#endif

#if OPT_LOAD_VTFONTS || OPT_WIDE_CHARS
extern Bool xtermLoadWideFonts (XtermWidget /* w */, Bool /* nullOk */);
extern void xtermSaveVTFonts(XtermWidget /* xw */);
#endif

#define xtermIsDecGraphic(ch)	((ch) > 0 && (ch) < 32)

#if OPT_RENDERFONT
extern Bool xtermXftMissing (XtermWidget /* xw */, XftFont * /* font */, unsigned /* wc */);
extern void xtermCloseXft(TScreen * /* screen */, XTermXftFonts * /* pub */);
#endif

#if OPT_SHIFT_FONTS
extern String getFaceName(XtermWidget /* xw */, Bool /* wideName */);
extern void HandleLargerFont PROTO_XT_ACTIONS_ARGS;
extern void HandleSmallerFont PROTO_XT_ACTIONS_ARGS;
extern void setFaceName(XtermWidget /* xw */, const char * /*value */);
#endif

#if OPT_WIDE_CHARS
extern unsigned ucs2dec (unsigned);
extern unsigned dec2ucs (unsigned);
#endif

/* *INDENT-ON* */

#endif /* included_fontutils_h */
