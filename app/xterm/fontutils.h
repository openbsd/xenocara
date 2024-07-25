/* $XTermId: fontutils.h,v 1.147 2024/07/11 08:16:39 tom Exp $ */

/*
 * Copyright 1998-2022,2024 by Thomas E. Dickey
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
extern Bool xtermOpenFont (XtermWidget /* xw */, const char */* name */, XTermFonts * /* result */, XTermFonts * /* current */, Bool /* force */);
extern XFontStruct * xtermLoadQueryFont(XtermWidget /* xw */, const char * /*name */);
extern XTermFonts * getDoubleFont (TScreen * /* screen */, int /* which */);
extern XTermFonts * getItalicFont (TScreen * /* screen */, int /* which */);
extern XTermFonts * getNormalFont (TScreen * /* screen */, int /* which */);
extern const VTFontNames * defaultVTFontNames(XtermWidget /* xw */);
extern const VTFontNames * xtermFontName (const char */* normal */);
extern const char * whichFontEnum (VTFontEnum /* value */);
extern const char * whichFontList (XtermWidget /* xw */, VTFontList * /* value */);
extern const char * whichFontList2(XtermWidget /* xw */, char ** /* value */);
extern int lookupRelativeFontSize (XtermWidget /* xw */, int /* old */, int /* relative */);
extern int xtermGetFont (const char * /* param */);
extern int xtermLoadFont (XtermWidget /* xw */, const VTFontNames */* fonts */, Bool /* doresize */, int /* fontnum */);
extern void HandleSetFont PROTO_XT_ACTIONS_ARGS;
extern Bool SetVTFont (XtermWidget /* xw */, int /* i */, Bool /* doresize */, const VTFontNames */* fonts */);
extern void allocFontList (XtermWidget /* xw */, const char * /* name */, XtermFontNames * /* target */, VTFontEnum /* which */, const char * /* source */, Bool /* ttf */);
extern void copyFontList (char *** /* targetp */, char ** /* source */);
extern void initFontLists (XtermWidget /* xw */);
extern void freeFontList (char *** /* targetp */);
extern void freeFontLists (VTFontList * /* lists */);
extern void xtermCloseFont (XtermWidget /* xw */, XTermFonts * /* fnt */);
extern void xtermCloseFonts (XtermWidget /* xw */, XTermFonts * /* fnts[fMAX] */);
extern void xtermComputeFontInfo (XtermWidget /* xw */, VTwin */* win */, XFontStruct */* font */, int /* sbwidth */);
extern void xtermCopyFontInfo (XTermFonts * /* target */, XTermFonts * /* source */);
extern void xtermDerivedFont (const char * /* name */);
extern void xtermFreeFontInfo (XTermFonts * /* target */);
extern void xtermSetCursorBox (TScreen * /* screen */);
extern void xtermUpdateFontGCs (XtermWidget /* xw */, MyGetFont /* myfunc */);
extern void xtermUpdateFontInfo (XtermWidget /* xw */, Bool /* doresize */);

#define getIconicFont(screen) (&((screen)->fnt_icon))

/* use these when "which" is constant, or known in-limits */
#define GetNormalFont(screen, which) (&((screen)->fnts[which]))
#define GetDoubleFont(screen, which) (&((screen)->double_fonts[which]))
#if OPT_WIDE_ATTRS
#define GetItalicFont(screen, which) (&((screen)->ifnts[which]))
#else
#define GetItalicFont(screen, which) 0
#endif

#define FirstItemOf(vector) ((vector) ? (vector)[0] : 0)
#define CurrentXftFont(xw)  ((xw)->work.fonts.xft.list_n[0])
#define DefaultFontN(xw)    ((xw)->work.fonts.x11.list_n[0])
#define DefaultFontB(xw)    ((xw)->work.fonts.x11.list_b[0])
#define DefaultFontW(xw)    ((xw)->work.fonts.x11.list_w[0])
#define DefaultFontWB(xw)   ((xw)->work.fonts.x11.list_wb[0])

#if OPT_DEC_CHRSET
extern char *xtermSpecialFont (XTermDraw * /* params */);
#endif

#define FontLacksMetrics(font) \
	((font)->fs != 0 \
	 && ((font)->fs->per_char == 0))

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
	 (((ch) < MaxUChar) && ((font)->known_missing[(Char)(ch)] > 0))
#else
#define CheckedKnownMissing(font, ch) \
	 ((font)->known_missing[(Char)(ch)] > 0)
#endif

#define IsXtermMissingChar(screen, ch, font) \
	 (CheckedKnownMissing(font, ch) \
	  ? ((font)->known_missing[(Char)(ch)] > 1) \
	  : ((FontIsIncomplete(font) && xtermMissingChar(ch, font)) \
	   || ForceBoxChars(screen, ch)))
#else
#define IsXtermMissingChar(screen, ch, font) False
#endif

extern void xtermDrawBoxChar (XTermDraw * /* params */, unsigned /* ch */, GC /* gc */, int /* x */, int /* y */, int /* cols */, Bool /* xftords */);

#if OPT_BOX_CHARS || OPT_REPORT_FONTS
#define XTermFontsRef(fontList, which) \
	(((which) != fNorm && \
	  ((fontList)[(which)].fs == NULL || \
	   (fontList)[(which)].fs->per_char == NULL) && \
	  ((fontList)[fNorm].fs != NULL && \
	  (fontList)[fNorm].fs->per_char != NULL)) \
	 ? &((fontList)[fNorm]) \
	 : &((fontList)[(which)]))
extern Bool xtermMissingChar (unsigned /* ch */, XTermFonts */* font */);
#endif

#if OPT_LOAD_VTFONTS
extern void HandleLoadVTFonts PROTO_XT_ACTIONS_ARGS;
#endif

#if OPT_LOAD_VTFONTS || OPT_WIDE_CHARS
extern Bool xtermLoadWideFonts (XtermWidget /* w */, Bool /* nullOk */);
extern void xtermSaveVTFonts (XtermWidget /* xw */);
#endif

/* checks for internal charset codes */
#define xtermIsDecTechnical(ch)	((ch) >= XTERM_PUA && (ch) <= XTERM_PUA + 6)
#define xtermIsDecGraphic(ch)	((ch) > 0 && (ch) < 32)
#define xtermIsInternalCs(ch)	(xtermIsDecGraphic(ch) || xtermIsDecTechnical(ch))

#if OPT_RENDERFONT
extern Boolean maybeXftCache(XtermWidget /* xw */, XftFont * /* font */);
extern Bool xtermXftMissing (XtermWidget /* xw */, XTermXftFonts * /* fontData */, int /* fontNum */, XftFont * /* font */, unsigned /* wc */);
extern XTermXftFonts *getMyXftFont (XtermWidget /* xw */, int /* which */, int /* fontnum */);
extern const char * whichXftFonts(XtermWidget /* xw */, XTermXftFonts * /* data */);
extern int findXftGlyph (XtermWidget /* xw */, XTermXftFonts * /* fontData */, unsigned /* wc */);
extern XftFont *getXftFont (XtermWidget /* xw */, VTFontEnum /* which */, int /* fontnum */);
extern void closeCachedXft (TScreen * /* screen */, XftFont * /* font */);
extern void xtermCloseXft (TScreen * /* screen */, XTermXftFonts * /* pub */);
#if OPT_DEC_CHRSET
extern void getDoubleXftFont(XTermDraw * /* params */, XTermXftFonts * /* fontData */, unsigned /* chrset */, unsigned /* attr_flags */);
#endif
#endif

#if OPT_SHIFT_FONTS
extern String getFaceName (XtermWidget /* xw */, Bool /* wideName */);
extern void HandleLargerFont PROTO_XT_ACTIONS_ARGS;
extern void HandleSmallerFont PROTO_XT_ACTIONS_ARGS;
extern void setFaceName (XtermWidget /* xw */, const char * /*value */);
#endif

#if OPT_WIDE_ATTRS
extern unsigned xtermUpdateItalics (XtermWidget /* xw */, unsigned /* new_attrs */, unsigned /* old_attrs */);
extern void xtermLoadItalics (XtermWidget /* xw */);
#endif

#if OPT_WIDE_CHARS
extern unsigned ucs2dec (TScreen * /* screen */, unsigned /* ch */);
extern unsigned dec2ucs (TScreen * /* screen */, unsigned /* ch */);
#endif

/* *INDENT-ON* */

#endif /* included_fontutils_h */
