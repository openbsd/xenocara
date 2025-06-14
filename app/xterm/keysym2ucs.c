/* $XTermId: keysym2ucs.c,v 1.25 2025/03/30 15:49:40 tom Exp $
 * ----------------------------------------------------------------------------
 * this file is part of xterm
 *
 * Copyright 2007-2018,2025 by Thomas E. Dickey
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
 * ----------------------------------------------------------------------------
 * Note:
 * ----
 * This file has been updated and revised to provide for mapping all keysyms
 * to UCS.  The BMP private use area is used for keysyms which do not map to
 * characters.
 *
 * Original header:
 * ---------------
 * This module converts keysym values into the corresponding ISO 10646
 * (UCS, Unicode) values.
 *
 * The array keysymtab[] contains pairs of X11 keysym values for graphical
 * characters and the corresponding Unicode value. The function
 * keysym2ucs() maps a keysym onto a Unicode value using a binary search,
 * therefore keysymtab[] must remain SORTED by keysym value.
 *
 * The keysym -> UTF-8 conversion will hopefully one day be provided
 * by Xlib via XmbLookupString() and should ideally not have to be
 * done in X applications. But we are not there yet.
 *
 * We allow to represent any UCS character in the range U-00000000 to
 * U-00FFFFFF by a keysym value in the range 0x01000000 to 0x01ffffff.
 * This admittedly does not cover the entire 31-bit space of UCS, but
 * it does cover all of the characters up to U-10FFFF, which can be
 * represented by UTF-16, and more, and it is very unlikely that higher
 * UCS codes will ever be assigned by ISO. So to get Unicode character
 * U+ABCD you can directly use keysym 0x0100abcd.
 *
 * NOTE: The comments in the table below contain the actual character
 * encoded in UTF-8, so for viewing and editing best use an editor in
 * UTF-8 mode.
 *
 * Author: Markus G. Kuhn <mkuhn@acm.org>, University of Cambridge, April 2001
 *
 * Special thanks to Richard Verhoeven <river@win.tue.nl> for preparing
 * an initial draft of the mapping table.
 *
 * This software is in the public domain. Share and enjoy!
 *
 * AUTOMATICALLY GENERATED FILE, DO NOT EDIT !!! (unicode/convmap.pl)
 */

#include <keysym2ucs.h>

static struct codepair {
    unsigned keysym;
    unsigned ucs;
} keysymtab[] = {
/* *INDENT-OFF* */
  { 0x01a1, 0x0104 }, /*                      Aogonek Ą LATIN CAPITAL LETTER A WITH OGONEK */
  { 0x01a2, 0x02d8 }, /*                        breve ˘ BREVE */
  { 0x01a3, 0x0141 }, /*                      Lstroke Ł LATIN CAPITAL LETTER L WITH STROKE */
  { 0x01a5, 0x013d }, /*                       Lcaron Ľ LATIN CAPITAL LETTER L WITH CARON */
  { 0x01a6, 0x015a }, /*                       Sacute Ś LATIN CAPITAL LETTER S WITH ACUTE */
  { 0x01a9, 0x0160 }, /*                       Scaron Š LATIN CAPITAL LETTER S WITH CARON */
  { 0x01aa, 0x015e }, /*                     Scedilla Ş LATIN CAPITAL LETTER S WITH CEDILLA */
  { 0x01ab, 0x0164 }, /*                       Tcaron Ť LATIN CAPITAL LETTER T WITH CARON */
  { 0x01ac, 0x0179 }, /*                       Zacute Ź LATIN CAPITAL LETTER Z WITH ACUTE */
  { 0x01ae, 0x017d }, /*                       Zcaron Ž LATIN CAPITAL LETTER Z WITH CARON */
  { 0x01af, 0x017b }, /*                    Zabovedot Ż LATIN CAPITAL LETTER Z WITH DOT ABOVE */
  { 0x01b1, 0x0105 }, /*                      aogonek ą LATIN SMALL LETTER A WITH OGONEK */
  { 0x01b2, 0x02db }, /*                       ogonek ˛ OGONEK */
  { 0x01b3, 0x0142 }, /*                      lstroke ł LATIN SMALL LETTER L WITH STROKE */
  { 0x01b5, 0x013e }, /*                       lcaron ľ LATIN SMALL LETTER L WITH CARON */
  { 0x01b6, 0x015b }, /*                       sacute ś LATIN SMALL LETTER S WITH ACUTE */
  { 0x01b7, 0x02c7 }, /*                        caron ˇ CARON */
  { 0x01b9, 0x0161 }, /*                       scaron š LATIN SMALL LETTER S WITH CARON */
  { 0x01ba, 0x015f }, /*                     scedilla ş LATIN SMALL LETTER S WITH CEDILLA */
  { 0x01bb, 0x0165 }, /*                       tcaron ť LATIN SMALL LETTER T WITH CARON */
  { 0x01bc, 0x017a }, /*                       zacute ź LATIN SMALL LETTER Z WITH ACUTE */
  { 0x01bd, 0x02dd }, /*                  doubleacute ˝ DOUBLE ACUTE ACCENT */
  { 0x01be, 0x017e }, /*                       zcaron ž LATIN SMALL LETTER Z WITH CARON */
  { 0x01bf, 0x017c }, /*                    zabovedot ż LATIN SMALL LETTER Z WITH DOT ABOVE */
  { 0x01c0, 0x0154 }, /*                       Racute Ŕ LATIN CAPITAL LETTER R WITH ACUTE */
  { 0x01c3, 0x0102 }, /*                       Abreve Ă LATIN CAPITAL LETTER A WITH BREVE */
  { 0x01c5, 0x0139 }, /*                       Lacute Ĺ LATIN CAPITAL LETTER L WITH ACUTE */
  { 0x01c6, 0x0106 }, /*                       Cacute Ć LATIN CAPITAL LETTER C WITH ACUTE */
  { 0x01c8, 0x010c }, /*                       Ccaron Č LATIN CAPITAL LETTER C WITH CARON */
  { 0x01ca, 0x0118 }, /*                      Eogonek Ę LATIN CAPITAL LETTER E WITH OGONEK */
  { 0x01cc, 0x011a }, /*                       Ecaron Ě LATIN CAPITAL LETTER E WITH CARON */
  { 0x01cf, 0x010e }, /*                       Dcaron Ď LATIN CAPITAL LETTER D WITH CARON */
  { 0x01d0, 0x0110 }, /*                      Dstroke Đ LATIN CAPITAL LETTER D WITH STROKE */
  { 0x01d1, 0x0143 }, /*                       Nacute Ń LATIN CAPITAL LETTER N WITH ACUTE */
  { 0x01d2, 0x0147 }, /*                       Ncaron Ň LATIN CAPITAL LETTER N WITH CARON */
  { 0x01d5, 0x0150 }, /*                 Odoubleacute Ő LATIN CAPITAL LETTER O WITH DOUBLE ACUTE */
  { 0x01d8, 0x0158 }, /*                       Rcaron Ř LATIN CAPITAL LETTER R WITH CARON */
  { 0x01d9, 0x016e }, /*                        Uring Ů LATIN CAPITAL LETTER U WITH RING ABOVE */
  { 0x01db, 0x0170 }, /*                 Udoubleacute Ű LATIN CAPITAL LETTER U WITH DOUBLE ACUTE */
  { 0x01de, 0x0162 }, /*                     Tcedilla Ţ LATIN CAPITAL LETTER T WITH CEDILLA */
  { 0x01e0, 0x0155 }, /*                       racute ŕ LATIN SMALL LETTER R WITH ACUTE */
  { 0x01e3, 0x0103 }, /*                       abreve ă LATIN SMALL LETTER A WITH BREVE */
  { 0x01e5, 0x013a }, /*                       lacute ĺ LATIN SMALL LETTER L WITH ACUTE */
  { 0x01e6, 0x0107 }, /*                       cacute ć LATIN SMALL LETTER C WITH ACUTE */
  { 0x01e8, 0x010d }, /*                       ccaron č LATIN SMALL LETTER C WITH CARON */
  { 0x01ea, 0x0119 }, /*                      eogonek ę LATIN SMALL LETTER E WITH OGONEK */
  { 0x01ec, 0x011b }, /*                       ecaron ě LATIN SMALL LETTER E WITH CARON */
  { 0x01ef, 0x010f }, /*                       dcaron ď LATIN SMALL LETTER D WITH CARON */
  { 0x01f0, 0x0111 }, /*                      dstroke đ LATIN SMALL LETTER D WITH STROKE */
  { 0x01f1, 0x0144 }, /*                       nacute ń LATIN SMALL LETTER N WITH ACUTE */
  { 0x01f2, 0x0148 }, /*                       ncaron ň LATIN SMALL LETTER N WITH CARON */
  { 0x01f5, 0x0151 }, /*                 odoubleacute ő LATIN SMALL LETTER O WITH DOUBLE ACUTE */
  { 0x01f8, 0x0159 }, /*                       rcaron ř LATIN SMALL LETTER R WITH CARON */
  { 0x01f9, 0x016f }, /*                        uring ů LATIN SMALL LETTER U WITH RING ABOVE */
  { 0x01fb, 0x0171 }, /*                 udoubleacute ű LATIN SMALL LETTER U WITH DOUBLE ACUTE */
  { 0x01fe, 0x0163 }, /*                     tcedilla ţ LATIN SMALL LETTER T WITH CEDILLA */
  { 0x01ff, 0x02d9 }, /*                     abovedot ˙ DOT ABOVE */
  { 0x02a1, 0x0126 }, /*                      Hstroke Ħ LATIN CAPITAL LETTER H WITH STROKE */
  { 0x02a6, 0x0124 }, /*                  Hcircumflex Ĥ LATIN CAPITAL LETTER H WITH CIRCUMFLEX */
  { 0x02a9, 0x0130 }, /*                    Iabovedot İ LATIN CAPITAL LETTER I WITH DOT ABOVE */
  { 0x02ab, 0x011e }, /*                       Gbreve Ğ LATIN CAPITAL LETTER G WITH BREVE */
  { 0x02ac, 0x0134 }, /*                  Jcircumflex Ĵ LATIN CAPITAL LETTER J WITH CIRCUMFLEX */
  { 0x02b1, 0x0127 }, /*                      hstroke ħ LATIN SMALL LETTER H WITH STROKE */
  { 0x02b6, 0x0125 }, /*                  hcircumflex ĥ LATIN SMALL LETTER H WITH CIRCUMFLEX */
  { 0x02b9, 0x0131 }, /*                     idotless ı LATIN SMALL LETTER DOTLESS I */
  { 0x02bb, 0x011f }, /*                       gbreve ğ LATIN SMALL LETTER G WITH BREVE */
  { 0x02bc, 0x0135 }, /*                  jcircumflex ĵ LATIN SMALL LETTER J WITH CIRCUMFLEX */
  { 0x02c5, 0x010a }, /*                    Cabovedot Ċ LATIN CAPITAL LETTER C WITH DOT ABOVE */
  { 0x02c6, 0x0108 }, /*                  Ccircumflex Ĉ LATIN CAPITAL LETTER C WITH CIRCUMFLEX */
  { 0x02d5, 0x0120 }, /*                    Gabovedot Ġ LATIN CAPITAL LETTER G WITH DOT ABOVE */
  { 0x02d8, 0x011c }, /*                  Gcircumflex Ĝ LATIN CAPITAL LETTER G WITH CIRCUMFLEX */
  { 0x02dd, 0x016c }, /*                       Ubreve Ŭ LATIN CAPITAL LETTER U WITH BREVE */
  { 0x02de, 0x015c }, /*                  Scircumflex Ŝ LATIN CAPITAL LETTER S WITH CIRCUMFLEX */
  { 0x02e5, 0x010b }, /*                    cabovedot ċ LATIN SMALL LETTER C WITH DOT ABOVE */
  { 0x02e6, 0x0109 }, /*                  ccircumflex ĉ LATIN SMALL LETTER C WITH CIRCUMFLEX */
  { 0x02f5, 0x0121 }, /*                    gabovedot ġ LATIN SMALL LETTER G WITH DOT ABOVE */
  { 0x02f8, 0x011d }, /*                  gcircumflex ĝ LATIN SMALL LETTER G WITH CIRCUMFLEX */
  { 0x02fd, 0x016d }, /*                       ubreve ŭ LATIN SMALL LETTER U WITH BREVE */
  { 0x02fe, 0x015d }, /*                  scircumflex ŝ LATIN SMALL LETTER S WITH CIRCUMFLEX */
  { 0x03a2, 0x0138 }, /*                          kra ĸ LATIN SMALL LETTER KRA */
  { 0x03a3, 0x0156 }, /*                     Rcedilla Ŗ LATIN CAPITAL LETTER R WITH CEDILLA */
  { 0x03a5, 0x0128 }, /*                       Itilde Ĩ LATIN CAPITAL LETTER I WITH TILDE */
  { 0x03a6, 0x013b }, /*                     Lcedilla Ļ LATIN CAPITAL LETTER L WITH CEDILLA */
  { 0x03aa, 0x0112 }, /*                      Emacron Ē LATIN CAPITAL LETTER E WITH MACRON */
  { 0x03ab, 0x0122 }, /*                     Gcedilla Ģ LATIN CAPITAL LETTER G WITH CEDILLA */
  { 0x03ac, 0x0166 }, /*                       Tslash Ŧ LATIN CAPITAL LETTER T WITH STROKE */
  { 0x03b3, 0x0157 }, /*                     rcedilla ŗ LATIN SMALL LETTER R WITH CEDILLA */
  { 0x03b5, 0x0129 }, /*                       itilde ĩ LATIN SMALL LETTER I WITH TILDE */
  { 0x03b6, 0x013c }, /*                     lcedilla ļ LATIN SMALL LETTER L WITH CEDILLA */
  { 0x03ba, 0x0113 }, /*                      emacron ē LATIN SMALL LETTER E WITH MACRON */
  { 0x03bb, 0x0123 }, /*                     gcedilla ģ LATIN SMALL LETTER G WITH CEDILLA */
  { 0x03bc, 0x0167 }, /*                       tslash ŧ LATIN SMALL LETTER T WITH STROKE */
  { 0x03bd, 0x014a }, /*                          ENG Ŋ LATIN CAPITAL LETTER ENG */
  { 0x03bf, 0x014b }, /*                          eng ŋ LATIN SMALL LETTER ENG */
  { 0x03c0, 0x0100 }, /*                      Amacron Ā LATIN CAPITAL LETTER A WITH MACRON */
  { 0x03c7, 0x012e }, /*                      Iogonek Į LATIN CAPITAL LETTER I WITH OGONEK */
  { 0x03cc, 0x0116 }, /*                    Eabovedot Ė LATIN CAPITAL LETTER E WITH DOT ABOVE */
  { 0x03cf, 0x012a }, /*                      Imacron Ī LATIN CAPITAL LETTER I WITH MACRON */
  { 0x03d1, 0x0145 }, /*                     Ncedilla Ņ LATIN CAPITAL LETTER N WITH CEDILLA */
  { 0x03d2, 0x014c }, /*                      Omacron Ō LATIN CAPITAL LETTER O WITH MACRON */
  { 0x03d3, 0x0136 }, /*                     Kcedilla Ķ LATIN CAPITAL LETTER K WITH CEDILLA */
  { 0x03d9, 0x0172 }, /*                      Uogonek Ų LATIN CAPITAL LETTER U WITH OGONEK */
  { 0x03dd, 0x0168 }, /*                       Utilde Ũ LATIN CAPITAL LETTER U WITH TILDE */
  { 0x03de, 0x016a }, /*                      Umacron Ū LATIN CAPITAL LETTER U WITH MACRON */
  { 0x03e0, 0x0101 }, /*                      amacron ā LATIN SMALL LETTER A WITH MACRON */
  { 0x03e7, 0x012f }, /*                      iogonek į LATIN SMALL LETTER I WITH OGONEK */
  { 0x03ec, 0x0117 }, /*                    eabovedot ė LATIN SMALL LETTER E WITH DOT ABOVE */
  { 0x03ef, 0x012b }, /*                      imacron ī LATIN SMALL LETTER I WITH MACRON */
  { 0x03f1, 0x0146 }, /*                     ncedilla ņ LATIN SMALL LETTER N WITH CEDILLA */
  { 0x03f2, 0x014d }, /*                      omacron ō LATIN SMALL LETTER O WITH MACRON */
  { 0x03f3, 0x0137 }, /*                     kcedilla ķ LATIN SMALL LETTER K WITH CEDILLA */
  { 0x03f9, 0x0173 }, /*                      uogonek ų LATIN SMALL LETTER U WITH OGONEK */
  { 0x03fd, 0x0169 }, /*                       utilde ũ LATIN SMALL LETTER U WITH TILDE */
  { 0x03fe, 0x016b }, /*                      umacron ū LATIN SMALL LETTER U WITH MACRON */
  { 0x047e, 0x203e }, /*                     overline ‾ OVERLINE */
  { 0x04a1, 0x3002 }, /*                kana_fullstop 。 IDEOGRAPHIC FULL STOP */
  { 0x04a2, 0x300c }, /*          kana_openingbracket 「 LEFT CORNER BRACKET */
  { 0x04a3, 0x300d }, /*          kana_closingbracket 」 RIGHT CORNER BRACKET */
  { 0x04a4, 0x3001 }, /*                   kana_comma 、 IDEOGRAPHIC COMMA */
  { 0x04a5, 0x30fb }, /*             kana_conjunctive ・ KATAKANA MIDDLE DOT */
  { 0x04a6, 0x30f2 }, /*                      kana_WO ヲ KATAKANA LETTER WO */
  { 0x04a7, 0x30a1 }, /*                       kana_a ァ KATAKANA LETTER SMALL A */
  { 0x04a8, 0x30a3 }, /*                       kana_i ィ KATAKANA LETTER SMALL I */
  { 0x04a9, 0x30a5 }, /*                       kana_u ゥ KATAKANA LETTER SMALL U */
  { 0x04aa, 0x30a7 }, /*                       kana_e ェ KATAKANA LETTER SMALL E */
  { 0x04ab, 0x30a9 }, /*                       kana_o ォ KATAKANA LETTER SMALL O */
  { 0x04ac, 0x30e3 }, /*                      kana_ya ャ KATAKANA LETTER SMALL YA */
  { 0x04ad, 0x30e5 }, /*                      kana_yu ュ KATAKANA LETTER SMALL YU */
  { 0x04ae, 0x30e7 }, /*                      kana_yo ョ KATAKANA LETTER SMALL YO */
  { 0x04af, 0x30c3 }, /*                     kana_tsu ッ KATAKANA LETTER SMALL TU */
  { 0x04b0, 0x30fc }, /*               prolongedsound ー KATAKANA-HIRAGANA PROLONGED SOUND MARK */
  { 0x04b1, 0x30a2 }, /*                       kana_A ア KATAKANA LETTER A */
  { 0x04b2, 0x30a4 }, /*                       kana_I イ KATAKANA LETTER I */
  { 0x04b3, 0x30a6 }, /*                       kana_U ウ KATAKANA LETTER U */
  { 0x04b4, 0x30a8 }, /*                       kana_E エ KATAKANA LETTER E */
  { 0x04b5, 0x30aa }, /*                       kana_O オ KATAKANA LETTER O */
  { 0x04b6, 0x30ab }, /*                      kana_KA カ KATAKANA LETTER KA */
  { 0x04b7, 0x30ad }, /*                      kana_KI キ KATAKANA LETTER KI */
  { 0x04b8, 0x30af }, /*                      kana_KU ク KATAKANA LETTER KU */
  { 0x04b9, 0x30b1 }, /*                      kana_KE ケ KATAKANA LETTER KE */
  { 0x04ba, 0x30b3 }, /*                      kana_KO コ KATAKANA LETTER KO */
  { 0x04bb, 0x30b5 }, /*                      kana_SA サ KATAKANA LETTER SA */
  { 0x04bc, 0x30b7 }, /*                     kana_SHI シ KATAKANA LETTER SI */
  { 0x04bd, 0x30b9 }, /*                      kana_SU ス KATAKANA LETTER SU */
  { 0x04be, 0x30bb }, /*                      kana_SE セ KATAKANA LETTER SE */
  { 0x04bf, 0x30bd }, /*                      kana_SO ソ KATAKANA LETTER SO */
  { 0x04c0, 0x30bf }, /*                      kana_TA タ KATAKANA LETTER TA */
  { 0x04c1, 0x30c1 }, /*                     kana_CHI チ KATAKANA LETTER TI */
  { 0x04c2, 0x30c4 }, /*                     kana_TSU ツ KATAKANA LETTER TU */
  { 0x04c3, 0x30c6 }, /*                      kana_TE テ KATAKANA LETTER TE */
  { 0x04c4, 0x30c8 }, /*                      kana_TO ト KATAKANA LETTER TO */
  { 0x04c5, 0x30ca }, /*                      kana_NA ナ KATAKANA LETTER NA */
  { 0x04c6, 0x30cb }, /*                      kana_NI ニ KATAKANA LETTER NI */
  { 0x04c7, 0x30cc }, /*                      kana_NU ヌ KATAKANA LETTER NU */
  { 0x04c8, 0x30cd }, /*                      kana_NE ネ KATAKANA LETTER NE */
  { 0x04c9, 0x30ce }, /*                      kana_NO ノ KATAKANA LETTER NO */
  { 0x04ca, 0x30cf }, /*                      kana_HA ハ KATAKANA LETTER HA */
  { 0x04cb, 0x30d2 }, /*                      kana_HI ヒ KATAKANA LETTER HI */
  { 0x04cc, 0x30d5 }, /*                      kana_FU フ KATAKANA LETTER HU */
  { 0x04cd, 0x30d8 }, /*                      kana_HE ヘ KATAKANA LETTER HE */
  { 0x04ce, 0x30db }, /*                      kana_HO ホ KATAKANA LETTER HO */
  { 0x04cf, 0x30de }, /*                      kana_MA マ KATAKANA LETTER MA */
  { 0x04d0, 0x30df }, /*                      kana_MI ミ KATAKANA LETTER MI */
  { 0x04d1, 0x30e0 }, /*                      kana_MU ム KATAKANA LETTER MU */
  { 0x04d2, 0x30e1 }, /*                      kana_ME メ KATAKANA LETTER ME */
  { 0x04d3, 0x30e2 }, /*                      kana_MO モ KATAKANA LETTER MO */
  { 0x04d4, 0x30e4 }, /*                      kana_YA ヤ KATAKANA LETTER YA */
  { 0x04d5, 0x30e6 }, /*                      kana_YU ユ KATAKANA LETTER YU */
  { 0x04d6, 0x30e8 }, /*                      kana_YO ヨ KATAKANA LETTER YO */
  { 0x04d7, 0x30e9 }, /*                      kana_RA ラ KATAKANA LETTER RA */
  { 0x04d8, 0x30ea }, /*                      kana_RI リ KATAKANA LETTER RI */
  { 0x04d9, 0x30eb }, /*                      kana_RU ル KATAKANA LETTER RU */
  { 0x04da, 0x30ec }, /*                      kana_RE レ KATAKANA LETTER RE */
  { 0x04db, 0x30ed }, /*                      kana_RO ロ KATAKANA LETTER RO */
  { 0x04dc, 0x30ef }, /*                      kana_WA ワ KATAKANA LETTER WA */
  { 0x04dd, 0x30f3 }, /*                       kana_N ン KATAKANA LETTER N */
  { 0x04de, 0x309b }, /*                  voicedsound ゛ KATAKANA-HIRAGANA VOICED SOUND MARK */
  { 0x04df, 0x309c }, /*              semivoicedsound ゜ KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK */
  { 0x05ac, 0x060c }, /*                 Arabic_comma ، ARABIC COMMA */
  { 0x05bb, 0x061b }, /*             Arabic_semicolon ؛ ARABIC SEMICOLON */
  { 0x05bf, 0x061f }, /*         Arabic_question_mark ؟ ARABIC QUESTION MARK */
  { 0x05c1, 0x0621 }, /*                 Arabic_hamza ء ARABIC LETTER HAMZA */
  { 0x05c2, 0x0622 }, /*           Arabic_maddaonalef آ ARABIC LETTER ALEF WITH MADDA ABOVE */
  { 0x05c3, 0x0623 }, /*           Arabic_hamzaonalef أ ARABIC LETTER ALEF WITH HAMZA ABOVE */
  { 0x05c4, 0x0624 }, /*            Arabic_hamzaonwaw ؤ ARABIC LETTER WAW WITH HAMZA ABOVE */
  { 0x05c5, 0x0625 }, /*        Arabic_hamzaunderalef إ ARABIC LETTER ALEF WITH HAMZA BELOW */
  { 0x05c6, 0x0626 }, /*            Arabic_hamzaonyeh ئ ARABIC LETTER YEH WITH HAMZA ABOVE */
  { 0x05c7, 0x0627 }, /*                  Arabic_alef ا ARABIC LETTER ALEF */
  { 0x05c8, 0x0628 }, /*                   Arabic_beh ب ARABIC LETTER BEH */
  { 0x05c9, 0x0629 }, /*            Arabic_tehmarbuta ة ARABIC LETTER TEH MARBUTA */
  { 0x05ca, 0x062a }, /*                   Arabic_teh ت ARABIC LETTER TEH */
  { 0x05cb, 0x062b }, /*                  Arabic_theh ث ARABIC LETTER THEH */
  { 0x05cc, 0x062c }, /*                  Arabic_jeem ج ARABIC LETTER JEEM */
  { 0x05cd, 0x062d }, /*                   Arabic_hah ح ARABIC LETTER HAH */
  { 0x05ce, 0x062e }, /*                  Arabic_khah خ ARABIC LETTER KHAH */
  { 0x05cf, 0x062f }, /*                   Arabic_dal د ARABIC LETTER DAL */
  { 0x05d0, 0x0630 }, /*                  Arabic_thal ذ ARABIC LETTER THAL */
  { 0x05d1, 0x0631 }, /*                    Arabic_ra ر ARABIC LETTER REH */
  { 0x05d2, 0x0632 }, /*                  Arabic_zain ز ARABIC LETTER ZAIN */
  { 0x05d3, 0x0633 }, /*                  Arabic_seen س ARABIC LETTER SEEN */
  { 0x05d4, 0x0634 }, /*                 Arabic_sheen ش ARABIC LETTER SHEEN */
  { 0x05d5, 0x0635 }, /*                   Arabic_sad ص ARABIC LETTER SAD */
  { 0x05d6, 0x0636 }, /*                   Arabic_dad ض ARABIC LETTER DAD */
  { 0x05d7, 0x0637 }, /*                   Arabic_tah ط ARABIC LETTER TAH */
  { 0x05d8, 0x0638 }, /*                   Arabic_zah ظ ARABIC LETTER ZAH */
  { 0x05d9, 0x0639 }, /*                   Arabic_ain ع ARABIC LETTER AIN */
  { 0x05da, 0x063a }, /*                 Arabic_ghain غ ARABIC LETTER GHAIN */
  { 0x05e0, 0x0640 }, /*               Arabic_tatweel ـ ARABIC TATWEEL */
  { 0x05e1, 0x0641 }, /*                   Arabic_feh ف ARABIC LETTER FEH */
  { 0x05e2, 0x0642 }, /*                   Arabic_qaf ق ARABIC LETTER QAF */
  { 0x05e3, 0x0643 }, /*                   Arabic_kaf ك ARABIC LETTER KAF */
  { 0x05e4, 0x0644 }, /*                   Arabic_lam ل ARABIC LETTER LAM */
  { 0x05e5, 0x0645 }, /*                  Arabic_meem م ARABIC LETTER MEEM */
  { 0x05e6, 0x0646 }, /*                  Arabic_noon ن ARABIC LETTER NOON */
  { 0x05e7, 0x0647 }, /*                    Arabic_ha ه ARABIC LETTER HEH */
  { 0x05e8, 0x0648 }, /*                   Arabic_waw و ARABIC LETTER WAW */
  { 0x05e9, 0x0649 }, /*           Arabic_alefmaksura ى ARABIC LETTER ALEF MAKSURA */
  { 0x05ea, 0x064a }, /*                   Arabic_yeh ي ARABIC LETTER YEH */
  { 0x05eb, 0x064b }, /*              Arabic_fathatan ً ARABIC FATHATAN */
  { 0x05ec, 0x064c }, /*              Arabic_dammatan ٌ ARABIC DAMMATAN */
  { 0x05ed, 0x064d }, /*              Arabic_kasratan ٍ ARABIC KASRATAN */
  { 0x05ee, 0x064e }, /*                 Arabic_fatha َ ARABIC FATHA */
  { 0x05ef, 0x064f }, /*                 Arabic_damma ُ ARABIC DAMMA */
  { 0x05f0, 0x0650 }, /*                 Arabic_kasra ِ ARABIC KASRA */
  { 0x05f1, 0x0651 }, /*                Arabic_shadda ّ ARABIC SHADDA */
  { 0x05f2, 0x0652 }, /*                 Arabic_sukun ْ ARABIC SUKUN */
  { 0x06a1, 0x0452 }, /*                  Serbian_dje ђ CYRILLIC SMALL LETTER DJE */
  { 0x06a2, 0x0453 }, /*                Macedonia_gje ѓ CYRILLIC SMALL LETTER GJE */
  { 0x06a3, 0x0451 }, /*                  Cyrillic_io ё CYRILLIC SMALL LETTER IO */
  { 0x06a4, 0x0454 }, /*                 Ukrainian_ie є CYRILLIC SMALL LETTER UKRAINIAN IE */
  { 0x06a5, 0x0455 }, /*                Macedonia_dse ѕ CYRILLIC SMALL LETTER DZE */
  { 0x06a6, 0x0456 }, /*                  Ukrainian_i і CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I */
  { 0x06a7, 0x0457 }, /*                 Ukrainian_yi ї CYRILLIC SMALL LETTER YI */
  { 0x06a8, 0x0458 }, /*                  Cyrillic_je ј CYRILLIC SMALL LETTER JE */
  { 0x06a9, 0x0459 }, /*                 Cyrillic_lje љ CYRILLIC SMALL LETTER LJE */
  { 0x06aa, 0x045a }, /*                 Cyrillic_nje њ CYRILLIC SMALL LETTER NJE */
  { 0x06ab, 0x045b }, /*                 Serbian_tshe ћ CYRILLIC SMALL LETTER TSHE */
  { 0x06ac, 0x045c }, /*                Macedonia_kje ќ CYRILLIC SMALL LETTER KJE */
  { 0x06ad, 0x0491 }, /*    Ukrainian_ghe_with_upturn ґ CYRILLIC SMALL LETTER GHE WITH UPTURN */
  { 0x06ae, 0x045e }, /*          Byelorussian_shortu ў CYRILLIC SMALL LETTER SHORT U */
  { 0x06af, 0x045f }, /*                Cyrillic_dzhe џ CYRILLIC SMALL LETTER DZHE */
  { 0x06b0, 0x2116 }, /*                   numerosign № NUMERO SIGN */
  { 0x06b1, 0x0402 }, /*                  Serbian_DJE Ђ CYRILLIC CAPITAL LETTER DJE */
  { 0x06b2, 0x0403 }, /*                Macedonia_GJE Ѓ CYRILLIC CAPITAL LETTER GJE */
  { 0x06b3, 0x0401 }, /*                  Cyrillic_IO Ё CYRILLIC CAPITAL LETTER IO */
  { 0x06b4, 0x0404 }, /*                 Ukrainian_IE Є CYRILLIC CAPITAL LETTER UKRAINIAN IE */
  { 0x06b5, 0x0405 }, /*                Macedonia_DSE Ѕ CYRILLIC CAPITAL LETTER DZE */
  { 0x06b6, 0x0406 }, /*                  Ukrainian_I І CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I */
  { 0x06b7, 0x0407 }, /*                 Ukrainian_YI Ї CYRILLIC CAPITAL LETTER YI */
  { 0x06b8, 0x0408 }, /*                  Cyrillic_JE Ј CYRILLIC CAPITAL LETTER JE */
  { 0x06b9, 0x0409 }, /*                 Cyrillic_LJE Љ CYRILLIC CAPITAL LETTER LJE */
  { 0x06ba, 0x040a }, /*                 Cyrillic_NJE Њ CYRILLIC CAPITAL LETTER NJE */
  { 0x06bb, 0x040b }, /*                 Serbian_TSHE Ћ CYRILLIC CAPITAL LETTER TSHE */
  { 0x06bc, 0x040c }, /*                Macedonia_KJE Ќ CYRILLIC CAPITAL LETTER KJE */
  { 0x06bd, 0x0490 }, /*    Ukrainian_GHE_WITH_UPTURN Ґ CYRILLIC CAPITAL LETTER GHE WITH UPTURN */
  { 0x06be, 0x040e }, /*          Byelorussian_SHORTU Ў CYRILLIC CAPITAL LETTER SHORT U */
  { 0x06bf, 0x040f }, /*                Cyrillic_DZHE Џ CYRILLIC CAPITAL LETTER DZHE */
  { 0x06c0, 0x044e }, /*                  Cyrillic_yu ю CYRILLIC SMALL LETTER YU */
  { 0x06c1, 0x0430 }, /*                   Cyrillic_a а CYRILLIC SMALL LETTER A */
  { 0x06c2, 0x0431 }, /*                  Cyrillic_be б CYRILLIC SMALL LETTER BE */
  { 0x06c3, 0x0446 }, /*                 Cyrillic_tse ц CYRILLIC SMALL LETTER TSE */
  { 0x06c4, 0x0434 }, /*                  Cyrillic_de д CYRILLIC SMALL LETTER DE */
  { 0x06c5, 0x0435 }, /*                  Cyrillic_ie е CYRILLIC SMALL LETTER IE */
  { 0x06c6, 0x0444 }, /*                  Cyrillic_ef ф CYRILLIC SMALL LETTER EF */
  { 0x06c7, 0x0433 }, /*                 Cyrillic_ghe г CYRILLIC SMALL LETTER GHE */
  { 0x06c8, 0x0445 }, /*                  Cyrillic_ha х CYRILLIC SMALL LETTER HA */
  { 0x06c9, 0x0438 }, /*                   Cyrillic_i и CYRILLIC SMALL LETTER I */
  { 0x06ca, 0x0439 }, /*              Cyrillic_shorti й CYRILLIC SMALL LETTER SHORT I */
  { 0x06cb, 0x043a }, /*                  Cyrillic_ka к CYRILLIC SMALL LETTER KA */
  { 0x06cc, 0x043b }, /*                  Cyrillic_el л CYRILLIC SMALL LETTER EL */
  { 0x06cd, 0x043c }, /*                  Cyrillic_em м CYRILLIC SMALL LETTER EM */
  { 0x06ce, 0x043d }, /*                  Cyrillic_en н CYRILLIC SMALL LETTER EN */
  { 0x06cf, 0x043e }, /*                   Cyrillic_o о CYRILLIC SMALL LETTER O */
  { 0x06d0, 0x043f }, /*                  Cyrillic_pe п CYRILLIC SMALL LETTER PE */
  { 0x06d1, 0x044f }, /*                  Cyrillic_ya я CYRILLIC SMALL LETTER YA */
  { 0x06d2, 0x0440 }, /*                  Cyrillic_er р CYRILLIC SMALL LETTER ER */
  { 0x06d3, 0x0441 }, /*                  Cyrillic_es с CYRILLIC SMALL LETTER ES */
  { 0x06d4, 0x0442 }, /*                  Cyrillic_te т CYRILLIC SMALL LETTER TE */
  { 0x06d5, 0x0443 }, /*                   Cyrillic_u у CYRILLIC SMALL LETTER U */
  { 0x06d6, 0x0436 }, /*                 Cyrillic_zhe ж CYRILLIC SMALL LETTER ZHE */
  { 0x06d7, 0x0432 }, /*                  Cyrillic_ve в CYRILLIC SMALL LETTER VE */
  { 0x06d8, 0x044c }, /*            Cyrillic_softsign ь CYRILLIC SMALL LETTER SOFT SIGN */
  { 0x06d9, 0x044b }, /*                Cyrillic_yeru ы CYRILLIC SMALL LETTER YERU */
  { 0x06da, 0x0437 }, /*                  Cyrillic_ze з CYRILLIC SMALL LETTER ZE */
  { 0x06db, 0x0448 }, /*                 Cyrillic_sha ш CYRILLIC SMALL LETTER SHA */
  { 0x06dc, 0x044d }, /*                   Cyrillic_e э CYRILLIC SMALL LETTER E */
  { 0x06dd, 0x0449 }, /*               Cyrillic_shcha щ CYRILLIC SMALL LETTER SHCHA */
  { 0x06de, 0x0447 }, /*                 Cyrillic_che ч CYRILLIC SMALL LETTER CHE */
  { 0x06df, 0x044a }, /*            Cyrillic_hardsign ъ CYRILLIC SMALL LETTER HARD SIGN */
  { 0x06e0, 0x042e }, /*                  Cyrillic_YU Ю CYRILLIC CAPITAL LETTER YU */
  { 0x06e1, 0x0410 }, /*                   Cyrillic_A А CYRILLIC CAPITAL LETTER A */
  { 0x06e2, 0x0411 }, /*                  Cyrillic_BE Б CYRILLIC CAPITAL LETTER BE */
  { 0x06e3, 0x0426 }, /*                 Cyrillic_TSE Ц CYRILLIC CAPITAL LETTER TSE */
  { 0x06e4, 0x0414 }, /*                  Cyrillic_DE Д CYRILLIC CAPITAL LETTER DE */
  { 0x06e5, 0x0415 }, /*                  Cyrillic_IE Е CYRILLIC CAPITAL LETTER IE */
  { 0x06e6, 0x0424 }, /*                  Cyrillic_EF Ф CYRILLIC CAPITAL LETTER EF */
  { 0x06e7, 0x0413 }, /*                 Cyrillic_GHE Г CYRILLIC CAPITAL LETTER GHE */
  { 0x06e8, 0x0425 }, /*                  Cyrillic_HA Х CYRILLIC CAPITAL LETTER HA */
  { 0x06e9, 0x0418 }, /*                   Cyrillic_I И CYRILLIC CAPITAL LETTER I */
  { 0x06ea, 0x0419 }, /*              Cyrillic_SHORTI Й CYRILLIC CAPITAL LETTER SHORT I */
  { 0x06eb, 0x041a }, /*                  Cyrillic_KA К CYRILLIC CAPITAL LETTER KA */
  { 0x06ec, 0x041b }, /*                  Cyrillic_EL Л CYRILLIC CAPITAL LETTER EL */
  { 0x06ed, 0x041c }, /*                  Cyrillic_EM М CYRILLIC CAPITAL LETTER EM */
  { 0x06ee, 0x041d }, /*                  Cyrillic_EN Н CYRILLIC CAPITAL LETTER EN */
  { 0x06ef, 0x041e }, /*                   Cyrillic_O О CYRILLIC CAPITAL LETTER O */
  { 0x06f0, 0x041f }, /*                  Cyrillic_PE П CYRILLIC CAPITAL LETTER PE */
  { 0x06f1, 0x042f }, /*                  Cyrillic_YA Я CYRILLIC CAPITAL LETTER YA */
  { 0x06f2, 0x0420 }, /*                  Cyrillic_ER Р CYRILLIC CAPITAL LETTER ER */
  { 0x06f3, 0x0421 }, /*                  Cyrillic_ES С CYRILLIC CAPITAL LETTER ES */
  { 0x06f4, 0x0422 }, /*                  Cyrillic_TE Т CYRILLIC CAPITAL LETTER TE */
  { 0x06f5, 0x0423 }, /*                   Cyrillic_U У CYRILLIC CAPITAL LETTER U */
  { 0x06f6, 0x0416 }, /*                 Cyrillic_ZHE Ж CYRILLIC CAPITAL LETTER ZHE */
  { 0x06f7, 0x0412 }, /*                  Cyrillic_VE В CYRILLIC CAPITAL LETTER VE */
  { 0x06f8, 0x042c }, /*            Cyrillic_SOFTSIGN Ь CYRILLIC CAPITAL LETTER SOFT SIGN */
  { 0x06f9, 0x042b }, /*                Cyrillic_YERU Ы CYRILLIC CAPITAL LETTER YERU */
  { 0x06fa, 0x0417 }, /*                  Cyrillic_ZE З CYRILLIC CAPITAL LETTER ZE */
  { 0x06fb, 0x0428 }, /*                 Cyrillic_SHA Ш CYRILLIC CAPITAL LETTER SHA */
  { 0x06fc, 0x042d }, /*                   Cyrillic_E Э CYRILLIC CAPITAL LETTER E */
  { 0x06fd, 0x0429 }, /*               Cyrillic_SHCHA Щ CYRILLIC CAPITAL LETTER SHCHA */
  { 0x06fe, 0x0427 }, /*                 Cyrillic_CHE Ч CYRILLIC CAPITAL LETTER CHE */
  { 0x06ff, 0x042a }, /*            Cyrillic_HARDSIGN Ъ CYRILLIC CAPITAL LETTER HARD SIGN */
  { 0x07a1, 0x0386 }, /*            Greek_ALPHAaccent Ά GREEK CAPITAL LETTER ALPHA WITH TONOS */
  { 0x07a2, 0x0388 }, /*          Greek_EPSILONaccent Έ GREEK CAPITAL LETTER EPSILON WITH TONOS */
  { 0x07a3, 0x0389 }, /*              Greek_ETAaccent Ή GREEK CAPITAL LETTER ETA WITH TONOS */
  { 0x07a4, 0x038a }, /*             Greek_IOTAaccent Ί GREEK CAPITAL LETTER IOTA WITH TONOS */
  { 0x07a5, 0x03aa }, /*          Greek_IOTAdiaeresis Ϊ GREEK CAPITAL LETTER IOTA WITH DIALYTIKA */
  { 0x07a7, 0x038c }, /*          Greek_OMICRONaccent Ό GREEK CAPITAL LETTER OMICRON WITH TONOS */
  { 0x07a8, 0x038e }, /*          Greek_UPSILONaccent Ύ GREEK CAPITAL LETTER UPSILON WITH TONOS */
  { 0x07a9, 0x03ab }, /*        Greek_UPSILONdieresis Ϋ GREEK CAPITAL LETTER UPSILON WITH DIALYTIKA */
  { 0x07ab, 0x038f }, /*            Greek_OMEGAaccent Ώ GREEK CAPITAL LETTER OMEGA WITH TONOS */
  { 0x07ae, 0x0385 }, /*         Greek_accentdieresis ΅ GREEK DIALYTIKA TONOS */
  { 0x07af, 0x2015 }, /*               Greek_horizbar ― HORIZONTAL BAR */
  { 0x07b1, 0x03ac }, /*            Greek_alphaaccent ά GREEK SMALL LETTER ALPHA WITH TONOS */
  { 0x07b2, 0x03ad }, /*          Greek_epsilonaccent έ GREEK SMALL LETTER EPSILON WITH TONOS */
  { 0x07b3, 0x03ae }, /*              Greek_etaaccent ή GREEK SMALL LETTER ETA WITH TONOS */
  { 0x07b4, 0x03af }, /*             Greek_iotaaccent ί GREEK SMALL LETTER IOTA WITH TONOS */
  { 0x07b5, 0x03ca }, /*           Greek_iotadieresis ϊ GREEK SMALL LETTER IOTA WITH DIALYTIKA */
  { 0x07b6, 0x0390 }, /*     Greek_iotaaccentdieresis ΐ GREEK SMALL LETTER IOTA WITH DIALYTIKA AND TONOS */
  { 0x07b7, 0x03cc }, /*          Greek_omicronaccent ό GREEK SMALL LETTER OMICRON WITH TONOS */
  { 0x07b8, 0x03cd }, /*          Greek_upsilonaccent ύ GREEK SMALL LETTER UPSILON WITH TONOS */
  { 0x07b9, 0x03cb }, /*        Greek_upsilondieresis ϋ GREEK SMALL LETTER UPSILON WITH DIALYTIKA */
  { 0x07ba, 0x03b0 }, /*  Greek_upsilonaccentdieresis ΰ GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND TONOS */
  { 0x07bb, 0x03ce }, /*            Greek_omegaaccent ώ GREEK SMALL LETTER OMEGA WITH TONOS */
  { 0x07c1, 0x0391 }, /*                  Greek_ALPHA Α GREEK CAPITAL LETTER ALPHA */
  { 0x07c2, 0x0392 }, /*                   Greek_BETA Β GREEK CAPITAL LETTER BETA */
  { 0x07c3, 0x0393 }, /*                  Greek_GAMMA Γ GREEK CAPITAL LETTER GAMMA */
  { 0x07c4, 0x0394 }, /*                  Greek_DELTA Δ GREEK CAPITAL LETTER DELTA */
  { 0x07c5, 0x0395 }, /*                Greek_EPSILON Ε GREEK CAPITAL LETTER EPSILON */
  { 0x07c6, 0x0396 }, /*                   Greek_ZETA Ζ GREEK CAPITAL LETTER ZETA */
  { 0x07c7, 0x0397 }, /*                    Greek_ETA Η GREEK CAPITAL LETTER ETA */
  { 0x07c8, 0x0398 }, /*                  Greek_THETA Θ GREEK CAPITAL LETTER THETA */
  { 0x07c9, 0x0399 }, /*                   Greek_IOTA Ι GREEK CAPITAL LETTER IOTA */
  { 0x07ca, 0x039a }, /*                  Greek_KAPPA Κ GREEK CAPITAL LETTER KAPPA */
  { 0x07cb, 0x039b }, /*                  Greek_LAMDA Λ GREEK CAPITAL LETTER LAMDA */
  { 0x07cc, 0x039c }, /*                     Greek_MU Μ GREEK CAPITAL LETTER MU */
  { 0x07cd, 0x039d }, /*                     Greek_NU Ν GREEK CAPITAL LETTER NU */
  { 0x07ce, 0x039e }, /*                     Greek_XI Ξ GREEK CAPITAL LETTER XI */
  { 0x07cf, 0x039f }, /*                Greek_OMICRON Ο GREEK CAPITAL LETTER OMICRON */
  { 0x07d0, 0x03a0 }, /*                     Greek_PI Π GREEK CAPITAL LETTER PI */
  { 0x07d1, 0x03a1 }, /*                    Greek_RHO Ρ GREEK CAPITAL LETTER RHO */
  { 0x07d2, 0x03a3 }, /*                  Greek_SIGMA Σ GREEK CAPITAL LETTER SIGMA */
  { 0x07d4, 0x03a4 }, /*                    Greek_TAU Τ GREEK CAPITAL LETTER TAU */
  { 0x07d5, 0x03a5 }, /*                Greek_UPSILON Υ GREEK CAPITAL LETTER UPSILON */
  { 0x07d6, 0x03a6 }, /*                    Greek_PHI Φ GREEK CAPITAL LETTER PHI */
  { 0x07d7, 0x03a7 }, /*                    Greek_CHI Χ GREEK CAPITAL LETTER CHI */
  { 0x07d8, 0x03a8 }, /*                    Greek_PSI Ψ GREEK CAPITAL LETTER PSI */
  { 0x07d9, 0x03a9 }, /*                  Greek_OMEGA Ω GREEK CAPITAL LETTER OMEGA */
  { 0x07e1, 0x03b1 }, /*                  Greek_alpha α GREEK SMALL LETTER ALPHA */
  { 0x07e2, 0x03b2 }, /*                   Greek_beta β GREEK SMALL LETTER BETA */
  { 0x07e3, 0x03b3 }, /*                  Greek_gamma γ GREEK SMALL LETTER GAMMA */
  { 0x07e4, 0x03b4 }, /*                  Greek_delta δ GREEK SMALL LETTER DELTA */
  { 0x07e5, 0x03b5 }, /*                Greek_epsilon ε GREEK SMALL LETTER EPSILON */
  { 0x07e6, 0x03b6 }, /*                   Greek_zeta ζ GREEK SMALL LETTER ZETA */
  { 0x07e7, 0x03b7 }, /*                    Greek_eta η GREEK SMALL LETTER ETA */
  { 0x07e8, 0x03b8 }, /*                  Greek_theta θ GREEK SMALL LETTER THETA */
  { 0x07e9, 0x03b9 }, /*                   Greek_iota ι GREEK SMALL LETTER IOTA */
  { 0x07ea, 0x03ba }, /*                  Greek_kappa κ GREEK SMALL LETTER KAPPA */
  { 0x07eb, 0x03bb }, /*                  Greek_lamda λ GREEK SMALL LETTER LAMDA */
  { 0x07ec, 0x03bc }, /*                     Greek_mu μ GREEK SMALL LETTER MU */
  { 0x07ed, 0x03bd }, /*                     Greek_nu ν GREEK SMALL LETTER NU */
  { 0x07ee, 0x03be }, /*                     Greek_xi ξ GREEK SMALL LETTER XI */
  { 0x07ef, 0x03bf }, /*                Greek_omicron ο GREEK SMALL LETTER OMICRON */
  { 0x07f0, 0x03c0 }, /*                     Greek_pi π GREEK SMALL LETTER PI */
  { 0x07f1, 0x03c1 }, /*                    Greek_rho ρ GREEK SMALL LETTER RHO */
  { 0x07f2, 0x03c3 }, /*                  Greek_sigma σ GREEK SMALL LETTER SIGMA */
  { 0x07f3, 0x03c2 }, /*        Greek_finalsmallsigma ς GREEK SMALL LETTER FINAL SIGMA */
  { 0x07f4, 0x03c4 }, /*                    Greek_tau τ GREEK SMALL LETTER TAU */
  { 0x07f5, 0x03c5 }, /*                Greek_upsilon υ GREEK SMALL LETTER UPSILON */
  { 0x07f6, 0x03c6 }, /*                    Greek_phi φ GREEK SMALL LETTER PHI */
  { 0x07f7, 0x03c7 }, /*                    Greek_chi χ GREEK SMALL LETTER CHI */
  { 0x07f8, 0x03c8 }, /*                    Greek_psi ψ GREEK SMALL LETTER PSI */
  { 0x07f9, 0x03c9 }, /*                  Greek_omega ω GREEK SMALL LETTER OMEGA */
  { 0x08a1, 0x23b7 }, /*                  leftradical ⎷ RADICAL SYMBOL BOTTOM */
  { 0x08a2, 0x250c }, /*               topleftradical ┌ BOX DRAWINGS LIGHT DOWN AND RIGHT */
  { 0x08a3, 0x2500 }, /*               horizconnector ─ BOX DRAWINGS LIGHT HORIZONTAL */
  { 0x08a4, 0x2320 }, /*                  topintegral ⌠ TOP HALF INTEGRAL */
  { 0x08a5, 0x2321 }, /*                  botintegral ⌡ BOTTOM HALF INTEGRAL */
  { 0x08a6, 0x2502 }, /*                vertconnector │ BOX DRAWINGS LIGHT VERTICAL */
  { 0x08a7, 0x23a1 }, /*             topleftsqbracket ⎡ LEFT SQUARE BRACKET UPPER CORNER */
  { 0x08a8, 0x23a3 }, /*             botleftsqbracket ⎣ LEFT SQUARE BRACKET LOWER CORNER */
  { 0x08a9, 0x23a4 }, /*            toprightsqbracket ⎤ RIGHT SQUARE BRACKET UPPER CORNER */
  { 0x08aa, 0x23a6 }, /*            botrightsqbracket ⎦ RIGHT SQUARE BRACKET LOWER CORNER */
  { 0x08ab, 0x239b }, /*                topleftparens ⎛ LEFT PARENTHESIS UPPER HOOK */
  { 0x08ac, 0x239d }, /*                botleftparens ⎝ LEFT PARENTHESIS LOWER HOOK */
  { 0x08ad, 0x239e }, /*               toprightparens ⎞ RIGHT PARENTHESIS UPPER HOOK */
  { 0x08ae, 0x23a0 }, /*               botrightparens ⎠ RIGHT PARENTHESIS LOWER HOOK */
  { 0x08af, 0x23a8 }, /*         leftmiddlecurlybrace ⎨ LEFT CURLY BRACKET MIDDLE PIECE */
  { 0x08b0, 0x23ac }, /*        rightmiddlecurlybrace ⎬ RIGHT CURLY BRACKET MIDDLE PIECE */
/*{ 0x08b1, 0xeeee }, **             topleftsummation ? PUA */
/*{ 0x08b2, 0xeeef }, **             botleftsummation ? PUA */
/*{ 0x08b3, 0xeef0 }, **    topvertsummationconnector ? PUA */
/*{ 0x08b4, 0xeef1 }, **    botvertsummationconnector ? PUA */
/*{ 0x08b5, 0xeef2 }, **            toprightsummation ? PUA */
/*{ 0x08b6, 0xeef3 }, **            botrightsummation ? PUA */
/*{ 0x08b7, 0xeef4 }, **         rightmiddlesummation ? PUA */
  { 0x08bc, 0x2264 }, /*                lessthanequal ≤ LESS-THAN OR EQUAL TO */
  { 0x08bd, 0x2260 }, /*                     notequal ≠ NOT EQUAL TO */
  { 0x08be, 0x2265 }, /*             greaterthanequal ≥ GREATER-THAN OR EQUAL TO */
  { 0x08bf, 0x222b }, /*                     integral ∫ INTEGRAL */
  { 0x08c0, 0x2234 }, /*                    therefore ∴ THEREFORE */
  { 0x08c1, 0x221d }, /*                    variation ∝ PROPORTIONAL TO */
  { 0x08c2, 0x221e }, /*                     infinity ∞ INFINITY */
  { 0x08c5, 0x2207 }, /*                        nabla ∇ NABLA */
  { 0x08c8, 0x223c }, /*                  approximate ∼ TILDE OPERATOR */
  { 0x08c9, 0x2243 }, /*                 similarequal ≃ ASYMPTOTICALLY EQUAL TO */
  { 0x08cd, 0x21d4 }, /*                     ifonlyif ⇔ LEFT RIGHT DOUBLE ARROW */
  { 0x08ce, 0x21d2 }, /*                      implies ⇒ RIGHTWARDS DOUBLE ARROW */
  { 0x08cf, 0x2261 }, /*                    identical ≡ IDENTICAL TO */
  { 0x08d6, 0x221a }, /*                      radical √ SQUARE ROOT */
  { 0x08da, 0x2282 }, /*                   includedin ⊂ SUBSET OF */
  { 0x08db, 0x2283 }, /*                     includes ⊃ SUPERSET OF */
  { 0x08dc, 0x2229 }, /*                 intersection ∩ INTERSECTION */
  { 0x08dd, 0x222a }, /*                        union ∪ UNION */
  { 0x08de, 0x2227 }, /*                   logicaland ∧ LOGICAL AND */
  { 0x08df, 0x2228 }, /*                    logicalor ∨ LOGICAL OR */
  { 0x08ef, 0x2202 }, /*            partialderivative ∂ PARTIAL DIFFERENTIAL */
  { 0x08f6, 0x0192 }, /*                     function ƒ LATIN SMALL LETTER F WITH HOOK */
  { 0x08fb, 0x2190 }, /*                    leftarrow ← LEFTWARDS ARROW */
  { 0x08fc, 0x2191 }, /*                      uparrow ↑ UPWARDS ARROW */
  { 0x08fd, 0x2192 }, /*                   rightarrow → RIGHTWARDS ARROW */
  { 0x08fe, 0x2193 }, /*                    downarrow ↓ DOWNWARDS ARROW */
  { 0x09df, 0x2422 }, /*                        blank ␢ BLANK SYMBOL */
  { 0x09e0, 0x25c6 }, /*                 soliddiamond ◆ BLACK DIAMOND */
  { 0x09e1, 0x2592 }, /*                 checkerboard ▒ MEDIUM SHADE */
  { 0x09e2, 0x2409 }, /*                           ht ␉ SYMBOL FOR HORIZONTAL TABULATION */
  { 0x09e3, 0x240c }, /*                           ff ␌ SYMBOL FOR FORM FEED */
  { 0x09e4, 0x240d }, /*                           cr ␍ SYMBOL FOR CARRIAGE RETURN */
  { 0x09e5, 0x240a }, /*                           lf ␊ SYMBOL FOR LINE FEED */
  { 0x09e8, 0x2424 }, /*                           nl ␤ SYMBOL FOR NEWLINE */
  { 0x09e9, 0x240b }, /*                           vt ␋ SYMBOL FOR VERTICAL TABULATION */
  { 0x09ea, 0x2518 }, /*               lowrightcorner ┘ BOX DRAWINGS LIGHT UP AND LEFT */
  { 0x09eb, 0x2510 }, /*                uprightcorner ┐ BOX DRAWINGS LIGHT DOWN AND LEFT */
  { 0x09ec, 0x250c }, /*                 upleftcorner ┌ BOX DRAWINGS LIGHT DOWN AND RIGHT */
  { 0x09ed, 0x2514 }, /*                lowleftcorner └ BOX DRAWINGS LIGHT UP AND RIGHT */
  { 0x09ee, 0x253c }, /*                crossinglines ┼ BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL */
  { 0x09ef, 0x23ba }, /*               horizlinescan1 ⎺ HORIZONTAL SCAN LINE-1 */
  { 0x09f0, 0x23bb }, /*               horizlinescan3 ⎻ HORIZONTAL SCAN LINE-3 */
  { 0x09f1, 0x2500 }, /*               horizlinescan5 ─ BOX DRAWINGS LIGHT HORIZONTAL */
  { 0x09f2, 0x23bc }, /*               horizlinescan7 ⎼ HORIZONTAL SCAN LINE-7 */
  { 0x09f3, 0x23bd }, /*               horizlinescan9 ⎽ HORIZONTAL SCAN LINE-9 */
  { 0x09f4, 0x251c }, /*                        leftt ├ BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
  { 0x09f5, 0x2524 }, /*                       rightt ┤ BOX DRAWINGS LIGHT VERTICAL AND LEFT */
  { 0x09f6, 0x2534 }, /*                         bott ┴ BOX DRAWINGS LIGHT UP AND HORIZONTAL */
  { 0x09f7, 0x252c }, /*                         topt ┬ BOX DRAWINGS LIGHT DOWN AND HORIZONTAL */
  { 0x09f8, 0x2502 }, /*                      vertbar │ BOX DRAWINGS LIGHT VERTICAL */
  { 0x0aa1, 0x2003 }, /*                      emspace   EM SPACE */
  { 0x0aa2, 0x2002 }, /*                      enspace   EN SPACE */
  { 0x0aa3, 0x2004 }, /*                     em3space   THREE-PER-EM SPACE */
  { 0x0aa4, 0x2005 }, /*                     em4space   FOUR-PER-EM SPACE */
  { 0x0aa5, 0x2007 }, /*                   digitspace   FIGURE SPACE */
  { 0x0aa6, 0x2008 }, /*                   punctspace   PUNCTUATION SPACE */
  { 0x0aa7, 0x2009 }, /*                    thinspace   THIN SPACE */
  { 0x0aa8, 0x200a }, /*                    hairspace   HAIR SPACE */
  { 0x0aa9, 0x2014 }, /*                       emdash — EM DASH */
  { 0x0aaa, 0x2013 }, /*                       endash – EN DASH */
  { 0x0aac, 0x2423 }, /*                  signifblank ␣ OPEN BOX */
  { 0x0aae, 0x2026 }, /*                     ellipsis … HORIZONTAL ELLIPSIS */
  { 0x0aaf, 0x2025 }, /*              doubbaselinedot ‥ TWO DOT LEADER */
  { 0x0ab0, 0x2153 }, /*                     onethird ⅓ VULGAR FRACTION ONE THIRD */
  { 0x0ab1, 0x2154 }, /*                    twothirds ⅔ VULGAR FRACTION TWO THIRDS */
  { 0x0ab2, 0x2155 }, /*                     onefifth ⅕ VULGAR FRACTION ONE FIFTH */
  { 0x0ab3, 0x2156 }, /*                    twofifths ⅖ VULGAR FRACTION TWO FIFTHS */
  { 0x0ab4, 0x2157 }, /*                  threefifths ⅗ VULGAR FRACTION THREE FIFTHS */
  { 0x0ab5, 0x2158 }, /*                   fourfifths ⅘ VULGAR FRACTION FOUR FIFTHS */
  { 0x0ab6, 0x2159 }, /*                     onesixth ⅙ VULGAR FRACTION ONE SIXTH */
  { 0x0ab7, 0x215a }, /*                   fivesixths ⅚ VULGAR FRACTION FIVE SIXTHS */
  { 0x0ab8, 0x2105 }, /*                       careof ℅ CARE OF */
  { 0x0abb, 0x2012 }, /*                      figdash ‒ FIGURE DASH */
  { 0x0abc, 0x2329 }, /*             leftanglebracket 〈 LEFT-POINTING ANGLE BRACKET */
  { 0x0abd, 0x002e }, /*                 decimalpoint . FULL STOP */
  { 0x0abe, 0x232a }, /*            rightanglebracket 〉 RIGHT-POINTING ANGLE BRACKET */
/*  0x0abf                                    marker ? ??? */
  { 0x0ac3, 0x215b }, /*                    oneeighth ⅛ VULGAR FRACTION ONE EIGHTH */
  { 0x0ac4, 0x215c }, /*                 threeeighths ⅜ VULGAR FRACTION THREE EIGHTHS */
  { 0x0ac5, 0x215d }, /*                  fiveeighths ⅝ VULGAR FRACTION FIVE EIGHTHS */
  { 0x0ac6, 0x215e }, /*                 seveneighths ⅞ VULGAR FRACTION SEVEN EIGHTHS */
  { 0x0ac9, 0x2122 }, /*                    trademark ™ TRADE MARK SIGN */
  { 0x0aca, 0x2613 }, /*                signaturemark ☓ SALTIRE */
/*  0x0acb                         trademarkincircle ? ??? */
  { 0x0acc, 0x25c1 }, /*             leftopentriangle ◁ WHITE LEFT-POINTING TRIANGLE */
  { 0x0acd, 0x25b7 }, /*            rightopentriangle ▷ WHITE RIGHT-POINTING TRIANGLE */
  { 0x0ace, 0x25cb }, /*                 emopencircle ○ WHITE CIRCLE */
  { 0x0acf, 0x25af }, /*              emopenrectangle ▯ WHITE VERTICAL RECTANGLE */
  { 0x0ad0, 0x2018 }, /*          leftsinglequotemark ‘ LEFT SINGLE QUOTATION MARK */
  { 0x0ad1, 0x2019 }, /*         rightsinglequotemark ’ RIGHT SINGLE QUOTATION MARK */
  { 0x0ad2, 0x201c }, /*          leftdoublequotemark “ LEFT DOUBLE QUOTATION MARK */
  { 0x0ad3, 0x201d }, /*         rightdoublequotemark ” RIGHT DOUBLE QUOTATION MARK */
  { 0x0ad4, 0x211e }, /*                 prescription ℞ PRESCRIPTION TAKE */
  { 0x0ad5, 0x2030 }, /*                     permille ‰ PER MILLE SIGN */
  { 0x0ad6, 0x2032 }, /*                      minutes ′ PRIME */
  { 0x0ad7, 0x2033 }, /*                      seconds ″ DOUBLE PRIME */
  { 0x0ad9, 0x271d }, /*                   latincross ✝ LATIN CROSS */
/*  0x0ada                                  hexagram ? ??? */
  { 0x0adb, 0x25ac }, /*             filledrectbullet ▬ BLACK RECTANGLE */
  { 0x0adc, 0x25c0 }, /*          filledlefttribullet ◀ BLACK LEFT-POINTING TRIANGLE */
  { 0x0add, 0x25b6 }, /*         filledrighttribullet ▶ BLACK RIGHT-POINTING TRIANGLE */
  { 0x0ade, 0x25cf }, /*               emfilledcircle ● BLACK CIRCLE */
  { 0x0adf, 0x25ae }, /*                 emfilledrect ▮ BLACK VERTICAL RECTANGLE */
  { 0x0ae0, 0x25e6 }, /*             enopencircbullet ◦ WHITE BULLET */
  { 0x0ae1, 0x25ab }, /*           enopensquarebullet ▫ WHITE SMALL SQUARE */
  { 0x0ae2, 0x25ad }, /*               openrectbullet ▭ WHITE RECTANGLE */
  { 0x0ae3, 0x25b3 }, /*              opentribulletup △ WHITE UP-POINTING TRIANGLE */
  { 0x0ae4, 0x25bd }, /*            opentribulletdown ▽ WHITE DOWN-POINTING TRIANGLE */
  { 0x0ae5, 0x2606 }, /*                     openstar ☆ WHITE STAR */
  { 0x0ae6, 0x2022 }, /*           enfilledcircbullet • BULLET */
  { 0x0ae7, 0x25aa }, /*             enfilledsqbullet ▪ BLACK SMALL SQUARE */
  { 0x0ae8, 0x25b2 }, /*            filledtribulletup ▲ BLACK UP-POINTING TRIANGLE */
  { 0x0ae9, 0x25bc }, /*          filledtribulletdown ▼ BLACK DOWN-POINTING TRIANGLE */
  { 0x0aea, 0x261c }, /*                  leftpointer ☜ WHITE LEFT POINTING INDEX */
  { 0x0aeb, 0x261e }, /*                 rightpointer ☞ WHITE RIGHT POINTING INDEX */
  { 0x0aec, 0x2663 }, /*                         club ♣ BLACK CLUB SUIT */
  { 0x0aed, 0x2666 }, /*                      diamond ♦ BLACK DIAMOND SUIT */
  { 0x0aee, 0x2665 }, /*                        heart ♥ BLACK HEART SUIT */
  { 0x0af0, 0x2720 }, /*                 maltesecross ✠ MALTESE CROSS */
  { 0x0af1, 0x2020 }, /*                       dagger † DAGGER */
  { 0x0af2, 0x2021 }, /*                 doubledagger ‡ DOUBLE DAGGER */
  { 0x0af3, 0x2713 }, /*                    checkmark ✓ CHECK MARK */
  { 0x0af4, 0x2717 }, /*                  ballotcross ✗ BALLOT X */
  { 0x0af5, 0x266f }, /*                 musicalsharp ♯ MUSIC SHARP SIGN */
  { 0x0af6, 0x266d }, /*                  musicalflat ♭ MUSIC FLAT SIGN */
  { 0x0af7, 0x2642 }, /*                   malesymbol ♂ MALE SIGN */
  { 0x0af8, 0x2640 }, /*                 femalesymbol ♀ FEMALE SIGN */
  { 0x0af9, 0x260e }, /*                    telephone ☎ BLACK TELEPHONE */
  { 0x0afa, 0x2315 }, /*            telephonerecorder ⌕ TELEPHONE RECORDER */
  { 0x0afb, 0x2117 }, /*          phonographcopyright ℗ SOUND RECORDING COPYRIGHT */
  { 0x0afc, 0x2038 }, /*                        caret ‸ CARET */
  { 0x0afd, 0x201a }, /*           singlelowquotemark ‚ SINGLE LOW-9 QUOTATION MARK */
  { 0x0afe, 0x201e }, /*           doublelowquotemark „ DOUBLE LOW-9 QUOTATION MARK */
/*  0x0aff                                    cursor ? ??? */
  { 0x0ba3, 0x003c }, /*                    leftcaret < LESS-THAN SIGN */
  { 0x0ba6, 0x003e }, /*                   rightcaret > GREATER-THAN SIGN */
  { 0x0ba8, 0x2228 }, /*                    downcaret ∨ LOGICAL OR */
  { 0x0ba9, 0x2227 }, /*                      upcaret ∧ LOGICAL AND */
  { 0x0bc0, 0x00af }, /*                      overbar ¯ MACRON */
  { 0x0bc2, 0x22a4 }, /*                     downtack ⊤ DOWN TACK */
  { 0x0bc3, 0x2229 }, /*                       upshoe ∩ INTERSECTION */
  { 0x0bc4, 0x230a }, /*                    downstile ⌊ LEFT FLOOR */
  { 0x0bc6, 0x005f }, /*                     underbar _ LOW LINE */
  { 0x0bca, 0x2218 }, /*                          jot ∘ RING OPERATOR */
  { 0x0bcc, 0x2395 }, /*                         quad ⎕ APL FUNCTIONAL SYMBOL QUAD */
  { 0x0bce, 0x22a5 }, /*                       uptack ⊥ UP TACK */
  { 0x0bcf, 0x25cb }, /*                       circle ○ WHITE CIRCLE */
  { 0x0bd3, 0x2308 }, /*                      upstile ⌈ LEFT CEILING */
  { 0x0bd6, 0x222a }, /*                     downshoe ∪ UNION */
  { 0x0bd8, 0x2283 }, /*                    rightshoe ⊃ SUPERSET OF */
  { 0x0bda, 0x2282 }, /*                     leftshoe ⊂ SUBSET OF */
  { 0x0bdc, 0x22a3 }, /*                     lefttack ⊣ LEFT TACK */
  { 0x0bfc, 0x22a2 }, /*                    righttack ⊢ RIGHT TACK */
  { 0x0cdf, 0x2017 }, /*         hebrew_doublelowline ‗ DOUBLE LOW LINE */
  { 0x0ce0, 0x05d0 }, /*                 hebrew_aleph א HEBREW LETTER ALEF */
  { 0x0ce1, 0x05d1 }, /*                   hebrew_bet ב HEBREW LETTER BET */
  { 0x0ce2, 0x05d2 }, /*                 hebrew_gimel ג HEBREW LETTER GIMEL */
  { 0x0ce3, 0x05d3 }, /*                 hebrew_dalet ד HEBREW LETTER DALET */
  { 0x0ce4, 0x05d4 }, /*                    hebrew_he ה HEBREW LETTER HE */
  { 0x0ce5, 0x05d5 }, /*                   hebrew_waw ו HEBREW LETTER VAV */
  { 0x0ce6, 0x05d6 }, /*                  hebrew_zain ז HEBREW LETTER ZAYIN */
  { 0x0ce7, 0x05d7 }, /*                  hebrew_chet ח HEBREW LETTER HET */
  { 0x0ce8, 0x05d8 }, /*                   hebrew_tet ט HEBREW LETTER TET */
  { 0x0ce9, 0x05d9 }, /*                   hebrew_yod י HEBREW LETTER YOD */
  { 0x0cea, 0x05da }, /*             hebrew_finalkaph ך HEBREW LETTER FINAL KAF */
  { 0x0ceb, 0x05db }, /*                  hebrew_kaph כ HEBREW LETTER KAF */
  { 0x0cec, 0x05dc }, /*                 hebrew_lamed ל HEBREW LETTER LAMED */
  { 0x0ced, 0x05dd }, /*              hebrew_finalmem ם HEBREW LETTER FINAL MEM */
  { 0x0cee, 0x05de }, /*                   hebrew_mem מ HEBREW LETTER MEM */
  { 0x0cef, 0x05df }, /*              hebrew_finalnun ן HEBREW LETTER FINAL NUN */
  { 0x0cf0, 0x05e0 }, /*                   hebrew_nun נ HEBREW LETTER NUN */
  { 0x0cf1, 0x05e1 }, /*                hebrew_samech ס HEBREW LETTER SAMEKH */
  { 0x0cf2, 0x05e2 }, /*                  hebrew_ayin ע HEBREW LETTER AYIN */
  { 0x0cf3, 0x05e3 }, /*               hebrew_finalpe ף HEBREW LETTER FINAL PE */
  { 0x0cf4, 0x05e4 }, /*                    hebrew_pe פ HEBREW LETTER PE */
  { 0x0cf5, 0x05e5 }, /*             hebrew_finalzade ץ HEBREW LETTER FINAL TSADI */
  { 0x0cf6, 0x05e6 }, /*                  hebrew_zade צ HEBREW LETTER TSADI */
  { 0x0cf7, 0x05e7 }, /*                  hebrew_qoph ק HEBREW LETTER QOF */
  { 0x0cf8, 0x05e8 }, /*                  hebrew_resh ר HEBREW LETTER RESH */
  { 0x0cf9, 0x05e9 }, /*                  hebrew_shin ש HEBREW LETTER SHIN */
  { 0x0cfa, 0x05ea }, /*                   hebrew_taw ת HEBREW LETTER TAV */
  { 0x0da1, 0x0e01 }, /*                   Thai_kokai ก THAI CHARACTER KO KAI */
  { 0x0da2, 0x0e02 }, /*                 Thai_khokhai ข THAI CHARACTER KHO KHAI */
  { 0x0da3, 0x0e03 }, /*                Thai_khokhuat ฃ THAI CHARACTER KHO KHUAT */
  { 0x0da4, 0x0e04 }, /*                Thai_khokhwai ค THAI CHARACTER KHO KHWAI */
  { 0x0da5, 0x0e05 }, /*                 Thai_khokhon ฅ THAI CHARACTER KHO KHON */
  { 0x0da6, 0x0e06 }, /*              Thai_khorakhang ฆ THAI CHARACTER KHO RAKHANG */
  { 0x0da7, 0x0e07 }, /*                  Thai_ngongu ง THAI CHARACTER NGO NGU */
  { 0x0da8, 0x0e08 }, /*                 Thai_chochan จ THAI CHARACTER CHO CHAN */
  { 0x0da9, 0x0e09 }, /*                Thai_choching ฉ THAI CHARACTER CHO CHING */
  { 0x0daa, 0x0e0a }, /*                Thai_chochang ช THAI CHARACTER CHO CHANG */
  { 0x0dab, 0x0e0b }, /*                    Thai_soso ซ THAI CHARACTER SO SO */
  { 0x0dac, 0x0e0c }, /*                 Thai_chochoe ฌ THAI CHARACTER CHO CHOE */
  { 0x0dad, 0x0e0d }, /*                  Thai_yoying ญ THAI CHARACTER YO YING */
  { 0x0dae, 0x0e0e }, /*                 Thai_dochada ฎ THAI CHARACTER DO CHADA */
  { 0x0daf, 0x0e0f }, /*                 Thai_topatak ฏ THAI CHARACTER TO PATAK */
  { 0x0db0, 0x0e10 }, /*                 Thai_thothan ฐ THAI CHARACTER THO THAN */
  { 0x0db1, 0x0e11 }, /*           Thai_thonangmontho ฑ THAI CHARACTER THO NANGMONTHO */
  { 0x0db2, 0x0e12 }, /*              Thai_thophuthao ฒ THAI CHARACTER THO PHUTHAO */
  { 0x0db3, 0x0e13 }, /*                   Thai_nonen ณ THAI CHARACTER NO NEN */
  { 0x0db4, 0x0e14 }, /*                   Thai_dodek ด THAI CHARACTER DO DEK */
  { 0x0db5, 0x0e15 }, /*                   Thai_totao ต THAI CHARACTER TO TAO */
  { 0x0db6, 0x0e16 }, /*                Thai_thothung ถ THAI CHARACTER THO THUNG */
  { 0x0db7, 0x0e17 }, /*               Thai_thothahan ท THAI CHARACTER THO THAHAN */
  { 0x0db8, 0x0e18 }, /*                Thai_thothong ธ THAI CHARACTER THO THONG */
  { 0x0db9, 0x0e19 }, /*                    Thai_nonu น THAI CHARACTER NO NU */
  { 0x0dba, 0x0e1a }, /*                Thai_bobaimai บ THAI CHARACTER BO BAIMAI */
  { 0x0dbb, 0x0e1b }, /*                   Thai_popla ป THAI CHARACTER PO PLA */
  { 0x0dbc, 0x0e1c }, /*                Thai_phophung ผ THAI CHARACTER PHO PHUNG */
  { 0x0dbd, 0x0e1d }, /*                    Thai_fofa ฝ THAI CHARACTER FO FA */
  { 0x0dbe, 0x0e1e }, /*                 Thai_phophan พ THAI CHARACTER PHO PHAN */
  { 0x0dbf, 0x0e1f }, /*                   Thai_fofan ฟ THAI CHARACTER FO FAN */
  { 0x0dc0, 0x0e20 }, /*              Thai_phosamphao ภ THAI CHARACTER PHO SAMPHAO */
  { 0x0dc1, 0x0e21 }, /*                    Thai_moma ม THAI CHARACTER MO MA */
  { 0x0dc2, 0x0e22 }, /*                   Thai_yoyak ย THAI CHARACTER YO YAK */
  { 0x0dc3, 0x0e23 }, /*                   Thai_rorua ร THAI CHARACTER RO RUA */
  { 0x0dc4, 0x0e24 }, /*                      Thai_ru ฤ THAI CHARACTER RU */
  { 0x0dc5, 0x0e25 }, /*                  Thai_loling ล THAI CHARACTER LO LING */
  { 0x0dc6, 0x0e26 }, /*                      Thai_lu ฦ THAI CHARACTER LU */
  { 0x0dc7, 0x0e27 }, /*                  Thai_wowaen ว THAI CHARACTER WO WAEN */
  { 0x0dc8, 0x0e28 }, /*                  Thai_sosala ศ THAI CHARACTER SO SALA */
  { 0x0dc9, 0x0e29 }, /*                  Thai_sorusi ษ THAI CHARACTER SO RUSI */
  { 0x0dca, 0x0e2a }, /*                   Thai_sosua ส THAI CHARACTER SO SUA */
  { 0x0dcb, 0x0e2b }, /*                   Thai_hohip ห THAI CHARACTER HO HIP */
  { 0x0dcc, 0x0e2c }, /*                 Thai_lochula ฬ THAI CHARACTER LO CHULA */
  { 0x0dcd, 0x0e2d }, /*                    Thai_oang อ THAI CHARACTER O ANG */
  { 0x0dce, 0x0e2e }, /*                Thai_honokhuk ฮ THAI CHARACTER HO NOKHUK */
  { 0x0dcf, 0x0e2f }, /*               Thai_paiyannoi ฯ THAI CHARACTER PAIYANNOI */
  { 0x0dd0, 0x0e30 }, /*                   Thai_saraa ะ THAI CHARACTER SARA A */
  { 0x0dd1, 0x0e31 }, /*              Thai_maihanakat ั THAI CHARACTER MAI HAN-AKAT */
  { 0x0dd2, 0x0e32 }, /*                  Thai_saraaa า THAI CHARACTER SARA AA */
  { 0x0dd3, 0x0e33 }, /*                  Thai_saraam ำ THAI CHARACTER SARA AM */
  { 0x0dd4, 0x0e34 }, /*                   Thai_sarai ิ THAI CHARACTER SARA I */
  { 0x0dd5, 0x0e35 }, /*                  Thai_saraii ี THAI CHARACTER SARA II */
  { 0x0dd6, 0x0e36 }, /*                  Thai_saraue ึ THAI CHARACTER SARA UE */
  { 0x0dd7, 0x0e37 }, /*                 Thai_sarauee ื THAI CHARACTER SARA UEE */
  { 0x0dd8, 0x0e38 }, /*                   Thai_sarau ุ THAI CHARACTER SARA U */
  { 0x0dd9, 0x0e39 }, /*                  Thai_sarauu ู THAI CHARACTER SARA UU */
  { 0x0dda, 0x0e3a }, /*                 Thai_phinthu ฺ THAI CHARACTER PHINTHU */
  { 0x0dde, 0x0e3e }, /*       Thai_maihanakat_maitho ฾ Unassigned code point */
  { 0x0ddf, 0x0e3f }, /*                    Thai_baht ฿ THAI CURRENCY SYMBOL BAHT */
  { 0x0de0, 0x0e40 }, /*                   Thai_sarae เ THAI CHARACTER SARA E */
  { 0x0de1, 0x0e41 }, /*                  Thai_saraae แ THAI CHARACTER SARA AE */
  { 0x0de2, 0x0e42 }, /*                   Thai_sarao โ THAI CHARACTER SARA O */
  { 0x0de3, 0x0e43 }, /*           Thai_saraaimaimuan ใ THAI CHARACTER SARA AI MAIMUAN */
  { 0x0de4, 0x0e44 }, /*          Thai_saraaimaimalai ไ THAI CHARACTER SARA AI MAIMALAI */
  { 0x0de5, 0x0e45 }, /*             Thai_lakkhangyao ๅ THAI CHARACTER LAKKHANGYAO */
  { 0x0de6, 0x0e46 }, /*                Thai_maiyamok ๆ THAI CHARACTER MAIYAMOK */
  { 0x0de7, 0x0e47 }, /*               Thai_maitaikhu ็ THAI CHARACTER MAITAIKHU */
  { 0x0de8, 0x0e48 }, /*                   Thai_maiek ่ THAI CHARACTER MAI EK */
  { 0x0de9, 0x0e49 }, /*                  Thai_maitho ้ THAI CHARACTER MAI THO */
  { 0x0dea, 0x0e4a }, /*                  Thai_maitri ๊ THAI CHARACTER MAI TRI */
  { 0x0deb, 0x0e4b }, /*             Thai_maichattawa ๋ THAI CHARACTER MAI CHATTAWA */
  { 0x0dec, 0x0e4c }, /*             Thai_thanthakhat ์ THAI CHARACTER THANTHAKHAT */
  { 0x0ded, 0x0e4d }, /*                Thai_nikhahit ํ THAI CHARACTER NIKHAHIT */
  { 0x0df0, 0x0e50 }, /*                  Thai_leksun ๐ THAI DIGIT ZERO */
  { 0x0df1, 0x0e51 }, /*                 Thai_leknung ๑ THAI DIGIT ONE */
  { 0x0df2, 0x0e52 }, /*                 Thai_leksong ๒ THAI DIGIT TWO */
  { 0x0df3, 0x0e53 }, /*                  Thai_leksam ๓ THAI DIGIT THREE */
  { 0x0df4, 0x0e54 }, /*                   Thai_leksi ๔ THAI DIGIT FOUR */
  { 0x0df5, 0x0e55 }, /*                   Thai_lekha ๕ THAI DIGIT FIVE */
  { 0x0df6, 0x0e56 }, /*                  Thai_lekhok ๖ THAI DIGIT SIX */
  { 0x0df7, 0x0e57 }, /*                 Thai_lekchet ๗ THAI DIGIT SEVEN */
  { 0x0df8, 0x0e58 }, /*                 Thai_lekpaet ๘ THAI DIGIT EIGHT */
  { 0x0df9, 0x0e59 }, /*                  Thai_lekkao ๙ THAI DIGIT NINE */
  { 0x0ea1, 0x3131 }, /*                Hangul_Kiyeog ㄱ HANGUL LETTER KIYEOK */
  { 0x0ea2, 0x3132 }, /*           Hangul_SsangKiyeog ㄲ HANGUL LETTER SSANGKIYEOK */
  { 0x0ea3, 0x3133 }, /*            Hangul_KiyeogSios ㄳ HANGUL LETTER KIYEOK-SIOS */
  { 0x0ea4, 0x3134 }, /*                 Hangul_Nieun ㄴ HANGUL LETTER NIEUN */
  { 0x0ea5, 0x3135 }, /*            Hangul_NieunJieuj ㄵ HANGUL LETTER NIEUN-CIEUC */
  { 0x0ea6, 0x3136 }, /*            Hangul_NieunHieuh ㄶ HANGUL LETTER NIEUN-HIEUH */
  { 0x0ea7, 0x3137 }, /*                Hangul_Dikeud ㄷ HANGUL LETTER TIKEUT */
  { 0x0ea8, 0x3138 }, /*           Hangul_SsangDikeud ㄸ HANGUL LETTER SSANGTIKEUT */
  { 0x0ea9, 0x3139 }, /*                 Hangul_Rieul ㄹ HANGUL LETTER RIEUL */
  { 0x0eaa, 0x313a }, /*           Hangul_RieulKiyeog ㄺ HANGUL LETTER RIEUL-KIYEOK */
  { 0x0eab, 0x313b }, /*            Hangul_RieulMieum ㄻ HANGUL LETTER RIEUL-MIEUM */
  { 0x0eac, 0x313c }, /*            Hangul_RieulPieub ㄼ HANGUL LETTER RIEUL-PIEUP */
  { 0x0ead, 0x313d }, /*             Hangul_RieulSios ㄽ HANGUL LETTER RIEUL-SIOS */
  { 0x0eae, 0x313e }, /*            Hangul_RieulTieut ㄾ HANGUL LETTER RIEUL-THIEUTH */
  { 0x0eaf, 0x313f }, /*           Hangul_RieulPhieuf ㄿ HANGUL LETTER RIEUL-PHIEUPH */
  { 0x0eb0, 0x3140 }, /*            Hangul_RieulHieuh ㅀ HANGUL LETTER RIEUL-HIEUH */
  { 0x0eb1, 0x3141 }, /*                 Hangul_Mieum ㅁ HANGUL LETTER MIEUM */
  { 0x0eb2, 0x3142 }, /*                 Hangul_Pieub ㅂ HANGUL LETTER PIEUP */
  { 0x0eb3, 0x3143 }, /*            Hangul_SsangPieub ㅃ HANGUL LETTER SSANGPIEUP */
  { 0x0eb4, 0x3144 }, /*             Hangul_PieubSios ㅄ HANGUL LETTER PIEUP-SIOS */
  { 0x0eb5, 0x3145 }, /*                  Hangul_Sios ㅅ HANGUL LETTER SIOS */
  { 0x0eb6, 0x3146 }, /*             Hangul_SsangSios ㅆ HANGUL LETTER SSANGSIOS */
  { 0x0eb7, 0x3147 }, /*                 Hangul_Ieung ㅇ HANGUL LETTER IEUNG */
  { 0x0eb8, 0x3148 }, /*                 Hangul_Jieuj ㅈ HANGUL LETTER CIEUC */
  { 0x0eb9, 0x3149 }, /*            Hangul_SsangJieuj ㅉ HANGUL LETTER SSANGCIEUC */
  { 0x0eba, 0x314a }, /*                 Hangul_Cieuc ㅊ HANGUL LETTER CHIEUCH */
  { 0x0ebb, 0x314b }, /*                Hangul_Khieuq ㅋ HANGUL LETTER KHIEUKH */
  { 0x0ebc, 0x314c }, /*                 Hangul_Tieut ㅌ HANGUL LETTER THIEUTH */
  { 0x0ebd, 0x314d }, /*                Hangul_Phieuf ㅍ HANGUL LETTER PHIEUPH */
  { 0x0ebe, 0x314e }, /*                 Hangul_Hieuh ㅎ HANGUL LETTER HIEUH */
  { 0x0ebf, 0x314f }, /*                     Hangul_A ㅏ HANGUL LETTER A */
  { 0x0ec0, 0x3150 }, /*                    Hangul_AE ㅐ HANGUL LETTER AE */
  { 0x0ec1, 0x3151 }, /*                    Hangul_YA ㅑ HANGUL LETTER YA */
  { 0x0ec2, 0x3152 }, /*                   Hangul_YAE ㅒ HANGUL LETTER YAE */
  { 0x0ec3, 0x3153 }, /*                    Hangul_EO ㅓ HANGUL LETTER EO */
  { 0x0ec4, 0x3154 }, /*                     Hangul_E ㅔ HANGUL LETTER E */
  { 0x0ec5, 0x3155 }, /*                   Hangul_YEO ㅕ HANGUL LETTER YEO */
  { 0x0ec6, 0x3156 }, /*                    Hangul_YE ㅖ HANGUL LETTER YE */
  { 0x0ec7, 0x3157 }, /*                     Hangul_O ㅗ HANGUL LETTER O */
  { 0x0ec8, 0x3158 }, /*                    Hangul_WA ㅘ HANGUL LETTER WA */
  { 0x0ec9, 0x3159 }, /*                   Hangul_WAE ㅙ HANGUL LETTER WAE */
  { 0x0eca, 0x315a }, /*                    Hangul_OE ㅚ HANGUL LETTER OE */
  { 0x0ecb, 0x315b }, /*                    Hangul_YO ㅛ HANGUL LETTER YO */
  { 0x0ecc, 0x315c }, /*                     Hangul_U ㅜ HANGUL LETTER U */
  { 0x0ecd, 0x315d }, /*                   Hangul_WEO ㅝ HANGUL LETTER WEO */
  { 0x0ece, 0x315e }, /*                    Hangul_WE ㅞ HANGUL LETTER WE */
  { 0x0ecf, 0x315f }, /*                    Hangul_WI ㅟ HANGUL LETTER WI */
  { 0x0ed0, 0x3160 }, /*                    Hangul_YU ㅠ HANGUL LETTER YU */
  { 0x0ed1, 0x3161 }, /*                    Hangul_EU ㅡ HANGUL LETTER EU */
  { 0x0ed2, 0x3162 }, /*                    Hangul_YI ㅢ HANGUL LETTER YI */
  { 0x0ed3, 0x3163 }, /*                     Hangul_I ㅣ HANGUL LETTER I */
  { 0x0ed4, 0x11a8 }, /*              Hangul_J_Kiyeog ᆨ HANGUL JONGSEONG KIYEOK */
  { 0x0ed5, 0x11a9 }, /*         Hangul_J_SsangKiyeog ᆩ HANGUL JONGSEONG SSANGKIYEOK */
  { 0x0ed6, 0x11aa }, /*          Hangul_J_KiyeogSios ᆪ HANGUL JONGSEONG KIYEOK-SIOS */
  { 0x0ed7, 0x11ab }, /*               Hangul_J_Nieun ᆫ HANGUL JONGSEONG NIEUN */
  { 0x0ed8, 0x11ac }, /*          Hangul_J_NieunJieuj ᆬ HANGUL JONGSEONG NIEUN-CIEUC */
  { 0x0ed9, 0x11ad }, /*          Hangul_J_NieunHieuh ᆭ HANGUL JONGSEONG NIEUN-HIEUH */
  { 0x0eda, 0x11ae }, /*              Hangul_J_Dikeud ᆮ HANGUL JONGSEONG TIKEUT */
  { 0x0edb, 0x11af }, /*               Hangul_J_Rieul ᆯ HANGUL JONGSEONG RIEUL */
  { 0x0edc, 0x11b0 }, /*         Hangul_J_RieulKiyeog ᆰ HANGUL JONGSEONG RIEUL-KIYEOK */
  { 0x0edd, 0x11b1 }, /*          Hangul_J_RieulMieum ᆱ HANGUL JONGSEONG RIEUL-MIEUM */
  { 0x0ede, 0x11b2 }, /*          Hangul_J_RieulPieub ᆲ HANGUL JONGSEONG RIEUL-PIEUP */
  { 0x0edf, 0x11b3 }, /*           Hangul_J_RieulSios ᆳ HANGUL JONGSEONG RIEUL-SIOS */
  { 0x0ee0, 0x11b4 }, /*          Hangul_J_RieulTieut ᆴ HANGUL JONGSEONG RIEUL-THIEUTH */
  { 0x0ee1, 0x11b5 }, /*         Hangul_J_RieulPhieuf ᆵ HANGUL JONGSEONG RIEUL-PHIEUPH */
  { 0x0ee2, 0x11b6 }, /*          Hangul_J_RieulHieuh ᆶ HANGUL JONGSEONG RIEUL-HIEUH */
  { 0x0ee3, 0x11b7 }, /*               Hangul_J_Mieum ᆷ HANGUL JONGSEONG MIEUM */
  { 0x0ee4, 0x11b8 }, /*               Hangul_J_Pieub ᆸ HANGUL JONGSEONG PIEUP */
  { 0x0ee5, 0x11b9 }, /*           Hangul_J_PieubSios ᆹ HANGUL JONGSEONG PIEUP-SIOS */
  { 0x0ee6, 0x11ba }, /*                Hangul_J_Sios ᆺ HANGUL JONGSEONG SIOS */
  { 0x0ee7, 0x11bb }, /*           Hangul_J_SsangSios ᆻ HANGUL JONGSEONG SSANGSIOS */
  { 0x0ee8, 0x11bc }, /*               Hangul_J_Ieung ᆼ HANGUL JONGSEONG IEUNG */
  { 0x0ee9, 0x11bd }, /*               Hangul_J_Jieuj ᆽ HANGUL JONGSEONG CIEUC */
  { 0x0eea, 0x11be }, /*               Hangul_J_Cieuc ᆾ HANGUL JONGSEONG CHIEUCH */
  { 0x0eeb, 0x11bf }, /*              Hangul_J_Khieuq ᆿ HANGUL JONGSEONG KHIEUKH */
  { 0x0eec, 0x11c0 }, /*               Hangul_J_Tieut ᇀ HANGUL JONGSEONG THIEUTH */
  { 0x0eed, 0x11c1 }, /*              Hangul_J_Phieuf ᇁ HANGUL JONGSEONG PHIEUPH */
  { 0x0eee, 0x11c2 }, /*               Hangul_J_Hieuh ᇂ HANGUL JONGSEONG HIEUH */
  { 0x0eef, 0x316d }, /*      Hangul_RieulYeorinHieuh ㅭ HANGUL LETTER RIEUL-YEORINHIEUH */
  { 0x0ef0, 0x3171 }, /*     Hangul_SunkyeongeumMieum ㅱ HANGUL LETTER KAPYEOUNMIEUM */
  { 0x0ef1, 0x3178 }, /*     Hangul_SunkyeongeumPieub ㅸ HANGUL LETTER KAPYEOUNPIEUP */
  { 0x0ef2, 0x317f }, /*               Hangul_PanSios ㅿ HANGUL LETTER PANSIOS */
  { 0x0ef3, 0x3181 }, /*     Hangul_KkogjiDalrinIeung ㆁ HANGUL LETTER YESIEUNG */
  { 0x0ef4, 0x3184 }, /*    Hangul_SunkyeongeumPhieuf ㆄ HANGUL LETTER KAPYEOUNPHIEUPH */
  { 0x0ef5, 0x3186 }, /*           Hangul_YeorinHieuh ㆆ HANGUL LETTER YEORINHIEUH */
  { 0x0ef6, 0x318d }, /*                 Hangul_AraeA ㆍ HANGUL LETTER ARAEA */
  { 0x0ef7, 0x318e }, /*                Hangul_AraeAE ㆎ HANGUL LETTER ARAEAE */
  { 0x0ef8, 0x11eb }, /*             Hangul_J_PanSios ᇫ HANGUL JONGSEONG PANSIOS */
  { 0x0ef9, 0x11f0 }, /*   Hangul_J_KkogjiDalrinIeung ᇰ HANGUL JONGSEONG YESIEUNG */
  { 0x0efa, 0x11f9 }, /*         Hangul_J_YeorinHieuh ᇹ HANGUL JONGSEONG YEORINHIEUH */
  { 0x0eff, 0x20a9 }, /*                   Korean_Won ₩ WON SIGN */
  { 0x13a4, 0x20ac }, /*                         Euro € EURO SIGN */
  { 0x13bc, 0x0152 }, /*                           OE Œ LATIN CAPITAL LIGATURE OE */
  { 0x13bd, 0x0153 }, /*                           oe œ LATIN SMALL LIGATURE OE */
  { 0x13be, 0x0178 }, /*                   Ydiaeresis Ÿ LATIN CAPITAL LETTER Y WITH DIAERESIS */
  { 0x20a0, 0x20a0 }, /*                      EcuSign ₠ EURO-CURRENCY SIGN */
  { 0x20a1, 0x20a1 }, /*                    ColonSign ₡ COLON SIGN */
  { 0x20a2, 0x20a2 }, /*                 CruzeiroSign ₢ CRUZEIRO SIGN */
  { 0x20a3, 0x20a3 }, /*                   FFrancSign ₣ FRENCH FRANC SIGN */
  { 0x20a4, 0x20a4 }, /*                     LiraSign ₤ LIRA SIGN */
  { 0x20a5, 0x20a5 }, /*                     MillSign ₥ MILL SIGN */
  { 0x20a6, 0x20a6 }, /*                    NairaSign ₦ NAIRA SIGN */
  { 0x20a7, 0x20a7 }, /*                   PesetaSign ₧ PESETA SIGN */
  { 0x20a8, 0x20a8 }, /*                    RupeeSign ₨ RUPEE SIGN */
  { 0x20a9, 0x20a9 }, /*                      WonSign ₩ WON SIGN */
  { 0x20aa, 0x20aa }, /*                NewSheqelSign ₪ NEW SHEQEL SIGN */
  { 0x20ab, 0x20ab }, /*                     DongSign ₫ DONG SIGN */
  { 0x20ac, 0x20ac }, /*                     EuroSign € EURO SIGN */
/*{ 0xfd01, 0xe001 }, **               3270_Duplicate ? PUA */
/*{ 0xfd02, 0xe002 }, **               3270_FieldMark ? PUA */
/*{ 0xfd03, 0xe003 }, **                  3270_Right2 ? PUA */
/*{ 0xfd04, 0xe004 }, **                   3270_Left2 ? PUA */
/*{ 0xfd05, 0xe005 }, **                 3270_BackTab ? PUA */
/*{ 0xfd06, 0xe006 }, **                3270_EraseEOF ? PUA */
/*{ 0xfd07, 0xe007 }, **              3270_EraseInput ? PUA */
/*{ 0xfd08, 0xe008 }, **                   3270_Reset ? PUA */
/*{ 0xfd09, 0xe009 }, **                    3270_Quit ? PUA */
/*{ 0xfd0a, 0xe00a }, **                     3270_PA1 ? PUA */
/*{ 0xfd0b, 0xe00b }, **                     3270_PA2 ? PUA */
/*{ 0xfd0c, 0xe00c }, **                     3270_PA3 ? PUA */
/*{ 0xfd0d, 0xe00d }, **                    3270_Test ? PUA */
/*{ 0xfd0e, 0xe00e }, **                    3270_Attn ? PUA */
/*{ 0xfd0f, 0xe00f }, **             3270_CursorBlink ? PUA */
/*{ 0xfd10, 0xe010 }, **               3270_AltCursor ? PUA */
/*{ 0xfd11, 0xe011 }, **                3270_KeyClick ? PUA */
/*{ 0xfd12, 0xe012 }, **                    3270_Jump ? PUA */
/*{ 0xfd13, 0xe013 }, **                   3270_Ident ? PUA */
/*{ 0xfd14, 0xe014 }, **                    3270_Rule ? PUA */
/*{ 0xfd15, 0xe015 }, **                    3270_Copy ? PUA */
/*{ 0xfd16, 0xe016 }, **                    3270_Play ? PUA */
/*{ 0xfd17, 0xe017 }, **                   3270_Setup ? PUA */
/*{ 0xfd18, 0xe018 }, **                  3270_Record ? PUA */
/*{ 0xfd19, 0xe019 }, **            3270_ChangeScreen ? PUA */
/*{ 0xfd1a, 0xe01a }, **              3270_DeleteWord ? PUA */
/*{ 0xfd1b, 0xe01b }, **                3270_ExSelect ? PUA */
/*{ 0xfd1c, 0xe01c }, **            3270_CursorSelect ? PUA */
/*{ 0xfd1d, 0xe01d }, **             3270_PrintScreen ? PUA */
/*{ 0xfd1e, 0xe01e }, **                   3270_Enter ? PUA */
/*{ 0xfe01, 0xe101 }, **                     ISO_Lock ? PUA */
/*{ 0xfe02, 0xe102 }, **             ISO_Level2_Latch ? PUA */
/*{ 0xfe03, 0xe103 }, **             ISO_Level3_Shift ? PUA */
/*{ 0xfe04, 0xe104 }, **             ISO_Level3_Latch ? PUA */
/*{ 0xfe05, 0xe105 }, **              ISO_Level3_Lock ? PUA */
/*{ 0xfe06, 0xe106 }, **              ISO_Group_Latch ? PUA */
/*{ 0xfe07, 0xe107 }, **               ISO_Group_Lock ? PUA */
/*{ 0xfe08, 0xe108 }, **               ISO_Next_Group ? PUA */
/*{ 0xfe09, 0xe109 }, **          ISO_Next_Group_Lock ? PUA */
/*{ 0xfe0a, 0xe10a }, **               ISO_Prev_Group ? PUA */
/*{ 0xfe0b, 0xe10b }, **          ISO_Prev_Group_Lock ? PUA */
/*{ 0xfe0c, 0xe10c }, **              ISO_First_Group ? PUA */
/*{ 0xfe0d, 0xe10d }, **         ISO_First_Group_Lock ? PUA */
/*{ 0xfe0e, 0xe10e }, **               ISO_Last_Group ? PUA */
/*{ 0xfe0f, 0xe10f }, **          ISO_Last_Group_Lock ? PUA */
/*{ 0xfe11, 0xe111 }, **             ISO_Level5_Shift ? PUA */
/*{ 0xfe12, 0xe112 }, **             ISO_Level5_Latch ? PUA */
/*{ 0xfe13, 0xe113 }, **              ISO_Level5_Lock ? PUA */
/*{ 0xfe20, 0xe120 }, **                 ISO_Left_Tab ? PUA */
/*{ 0xfe21, 0xe121 }, **             ISO_Move_Line_Up ? PUA */
/*{ 0xfe22, 0xe122 }, **           ISO_Move_Line_Down ? PUA */
/*{ 0xfe23, 0xe123 }, **          ISO_Partial_Line_Up ? PUA */
/*{ 0xfe24, 0xe124 }, **        ISO_Partial_Line_Down ? PUA */
/*{ 0xfe25, 0xe125 }, **       ISO_Partial_Space_Left ? PUA */
/*{ 0xfe26, 0xe126 }, **      ISO_Partial_Space_Right ? PUA */
/*{ 0xfe27, 0xe127 }, **          ISO_Set_Margin_Left ? PUA */
/*{ 0xfe28, 0xe128 }, **         ISO_Set_Margin_Right ? PUA */
/*{ 0xfe29, 0xe129 }, **      ISO_Release_Margin_Left ? PUA */
/*{ 0xfe2a, 0xe12a }, **     ISO_Release_Margin_Right ? PUA */
/*{ 0xfe2b, 0xe12b }, **     ISO_Release_Both_Margins ? PUA */
/*{ 0xfe2c, 0xe12c }, **         ISO_Fast_Cursor_Left ? PUA */
/*{ 0xfe2d, 0xe12d }, **        ISO_Fast_Cursor_Right ? PUA */
/*{ 0xfe2e, 0xe12e }, **           ISO_Fast_Cursor_Up ? PUA */
/*{ 0xfe2f, 0xe12f }, **         ISO_Fast_Cursor_Down ? PUA */
/*{ 0xfe30, 0xe130 }, **     ISO_Continuous_Underline ? PUA */
/*{ 0xfe31, 0xe131 }, **  ISO_Discontinuous_Underline ? PUA */
/*{ 0xfe32, 0xe132 }, **                ISO_Emphasize ? PUA */
/*{ 0xfe33, 0xe133 }, **            ISO_Center_Object ? PUA */
/*{ 0xfe34, 0xe134 }, **                    ISO_Enter ? PUA */
  { 0xfe50, 0x0300 }, /*                   dead_grave ̀ COMBINING GRAVE ACCENT */
  { 0xfe51, 0x0301 }, /*                   dead_acute ́ COMBINING ACUTE ACCENT */
  { 0xfe52, 0x0302 }, /*              dead_circumflex ̂ COMBINING CIRCUMFLEX ACCENT */
  { 0xfe53, 0x0303 }, /*                   dead_tilde ̃ COMBINING TILDE */
  { 0xfe54, 0x0304 }, /*                  dead_macron ̄ COMBINING MACRON */
  { 0xfe55, 0x0306 }, /*                   dead_breve ̆ COMBINING BREVE */
  { 0xfe56, 0x0307 }, /*                dead_abovedot ̇ COMBINING DOT ABOVE */
  { 0xfe57, 0x0308 }, /*               dead_diaeresis ̈ COMBINING DIAERESIS */
  { 0xfe58, 0x030a }, /*               dead_abovering ̊ COMBINING RING ABOVE */
  { 0xfe59, 0x030b }, /*             dead_doubleacute ̋ COMBINING DOUBLE ACUTE ACCENT */
  { 0xfe5a, 0x030c }, /*                   dead_caron ̌ COMBINING CARON */
  { 0xfe5b, 0x0327 }, /*                 dead_cedilla ̧ COMBINING CEDILLA */
  { 0xfe5c, 0x0328 }, /*                  dead_ogonek ̨ COMBINING OGONEK */
  { 0xfe5d, 0x0345 }, /*                    dead_iota ͅ COMBINING GREEK YPOGEGRAMMENI */
  { 0xfe5e, 0x3099 }, /*            dead_voiced_sound ゙ COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK */
  { 0xfe5f, 0x309a }, /*        dead_semivoiced_sound ゚ COMBINING KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK */
/*{ 0xfe60, 0xe160 }, **                dead_belowdot ? PUA */
/*{ 0xfe61, 0xe161 }, **                    dead_hook ? PUA */
/*{ 0xfe62, 0xe162 }, **                    dead_horn ? PUA */
/*{ 0xfe63, 0xe163 }, **                  dead_stroke ? PUA */
/*{ 0xfe64, 0xe164 }, **              dead_abovecomma ? PUA */
/*{ 0xfe65, 0xe165 }, **      dead_abovereversedcomma ? PUA */
/*{ 0xfe66, 0xe166 }, **             dead_doublegrave ? PUA */
/*{ 0xfe67, 0xe167 }, **               dead_belowring ? PUA */
/*{ 0xfe68, 0xe168 }, **             dead_belowmacron ? PUA */
/*{ 0xfe69, 0xe169 }, **         dead_belowcircumflex ? PUA */
/*{ 0xfe6a, 0xe16a }, **              dead_belowtilde ? PUA */
/*{ 0xfe6b, 0xe16b }, **              dead_belowbreve ? PUA */
/*{ 0xfe6c, 0xe16c }, **          dead_belowdiaeresis ? PUA */
/*{ 0xfe6d, 0xe16d }, **           dead_invertedbreve ? PUA */
/*{ 0xfe6e, 0xe16e }, **              dead_belowcomma ? PUA */
/*{ 0xfe6f, 0xe16f }, **                dead_currency ? PUA */
/*{ 0xfe70, 0xe170 }, **               AccessX_Enable ? PUA */
/*{ 0xfe71, 0xe171 }, **      AccessX_Feedback_Enable ? PUA */
/*{ 0xfe72, 0xe172 }, **            RepeatKeys_Enable ? PUA */
/*{ 0xfe73, 0xe173 }, **              SlowKeys_Enable ? PUA */
/*{ 0xfe74, 0xe174 }, **            BounceKeys_Enable ? PUA */
/*{ 0xfe75, 0xe175 }, **            StickyKeys_Enable ? PUA */
/*{ 0xfe76, 0xe176 }, **             MouseKeys_Enable ? PUA */
/*{ 0xfe77, 0xe177 }, **       MouseKeys_Accel_Enable ? PUA */
/*{ 0xfe78, 0xe178 }, **              Overlay1_Enable ? PUA */
/*{ 0xfe79, 0xe179 }, **              Overlay2_Enable ? PUA */
/*{ 0xfe7a, 0xe17a }, **           AudibleBell_Enable ? PUA */
/*{ 0xfe80, 0xe180 }, **                       dead_a ? PUA */
/*{ 0xfe81, 0xe181 }, **                       dead_A ? PUA */
/*{ 0xfe82, 0xe182 }, **                       dead_e ? PUA */
/*{ 0xfe83, 0xe183 }, **                       dead_E ? PUA */
/*{ 0xfe84, 0xe184 }, **                       dead_i ? PUA */
/*{ 0xfe85, 0xe185 }, **                       dead_I ? PUA */
/*{ 0xfe86, 0xe186 }, **                       dead_o ? PUA */
/*{ 0xfe87, 0xe187 }, **                       dead_O ? PUA */
/*{ 0xfe88, 0xe188 }, **                       dead_u ? PUA */
/*{ 0xfe89, 0xe189 }, **                       dead_U ? PUA */
/*{ 0xfe8a, 0xe18a }, **                   dead_schwa ? PUA */
/*{ 0xfe8b, 0xe18b }, **                   dead_SCHWA ? PUA */
/*{ 0xfe8c, 0xe18c }, **                   dead_greek ? PUA */
/*{ 0xfe8d, 0xe18d }, **                   dead_hamza ? PUA */
/*{ 0xfe90, 0xe190 }, **                 dead_lowline ? PUA */
/*{ 0xfe91, 0xe191 }, **       dead_aboveverticalline ? PUA */
/*{ 0xfe92, 0xe192 }, **       dead_belowverticalline ? PUA */
/*{ 0xfe93, 0xe193 }, **      dead_longsolidusoverlay ? PUA */
/*{ 0xfea0, 0xe1a0 }, **                           ch ? PUA */
/*{ 0xfea1, 0xe1a1 }, **                           Ch ? PUA */
/*{ 0xfea2, 0xe1a2 }, **                           CH ? PUA */
/*{ 0xfea3, 0xe1a3 }, **                          c_h ? PUA */
/*{ 0xfea4, 0xe1a4 }, **                          C_h ? PUA */
/*{ 0xfea5, 0xe1a5 }, **                          C_H ? PUA */
/*{ 0xfed0, 0xe1d0 }, **         First_Virtual_Screen ? PUA */
/*{ 0xfed1, 0xe1d1 }, **          Prev_Virtual_Screen ? PUA */
/*{ 0xfed2, 0xe1d2 }, **          Next_Virtual_Screen ? PUA */
/*{ 0xfed4, 0xe1d4 }, **          Last_Virtual_Screen ? PUA */
/*{ 0xfed5, 0xe1d5 }, **             Terminate_Server ? PUA */
/*{ 0xfee0, 0xe1e0 }, **                 Pointer_Left ? PUA */
/*{ 0xfee1, 0xe1e1 }, **                Pointer_Right ? PUA */
/*{ 0xfee2, 0xe1e2 }, **                   Pointer_Up ? PUA */
/*{ 0xfee3, 0xe1e3 }, **                 Pointer_Down ? PUA */
/*{ 0xfee4, 0xe1e4 }, **               Pointer_UpLeft ? PUA */
/*{ 0xfee5, 0xe1e5 }, **              Pointer_UpRight ? PUA */
/*{ 0xfee6, 0xe1e6 }, **             Pointer_DownLeft ? PUA */
/*{ 0xfee7, 0xe1e7 }, **            Pointer_DownRight ? PUA */
/*{ 0xfee8, 0xe1e8 }, **          Pointer_Button_Dflt ? PUA */
/*{ 0xfee9, 0xe1e9 }, **              Pointer_Button1 ? PUA */
/*{ 0xfeea, 0xe1ea }, **              Pointer_Button2 ? PUA */
/*{ 0xfeeb, 0xe1eb }, **              Pointer_Button3 ? PUA */
/*{ 0xfeec, 0xe1ec }, **              Pointer_Button4 ? PUA */
/*{ 0xfeed, 0xe1ed }, **              Pointer_Button5 ? PUA */
/*{ 0xfeee, 0xe1ee }, **        Pointer_DblClick_Dflt ? PUA */
/*{ 0xfeef, 0xe1ef }, **            Pointer_DblClick1 ? PUA */
/*{ 0xfef0, 0xe1f0 }, **            Pointer_DblClick2 ? PUA */
/*{ 0xfef1, 0xe1f1 }, **            Pointer_DblClick3 ? PUA */
/*{ 0xfef2, 0xe1f2 }, **            Pointer_DblClick4 ? PUA */
/*{ 0xfef3, 0xe1f3 }, **            Pointer_DblClick5 ? PUA */
/*{ 0xfef4, 0xe1f4 }, **            Pointer_Drag_Dflt ? PUA */
/*{ 0xfef5, 0xe1f5 }, **                Pointer_Drag1 ? PUA */
/*{ 0xfef6, 0xe1f6 }, **                Pointer_Drag2 ? PUA */
/*{ 0xfef7, 0xe1f7 }, **                Pointer_Drag3 ? PUA */
/*{ 0xfef8, 0xe1f8 }, **                Pointer_Drag4 ? PUA */
/*{ 0xfef9, 0xe1f9 }, **           Pointer_EnableKeys ? PUA */
/*{ 0xfefa, 0xe1fa }, **           Pointer_Accelerate ? PUA */
/*{ 0xfefb, 0xe1fb }, **          Pointer_DfltBtnNext ? PUA */
/*{ 0xfefc, 0xe1fc }, **          Pointer_DfltBtnPrev ? PUA */
/*{ 0xfefd, 0xe1fd }, **                Pointer_Drag5 ? PUA */
  { 0xff08, 0x0008 }, /*                    BackSpace ^H <control> */
  { 0xff09, 0x0009 }, /*                          Tab ^I <control> */
  { 0xff0a, 0x000a }, /*                     Linefeed ^J <control> */
  { 0xff0b, 0x000b }, /*                        Clear ^K <control> */
  { 0xff0d, 0x000d }, /*                       Return ^M <control> */
  { 0xff13, 0x0013 }, /*                        Pause ^S <control> */
  { 0xff14, 0x0014 }, /*                  Scroll_Lock ^T <control> */
  { 0xff15, 0x0015 }, /*                      Sys_Req ^U <control> */
  { 0xff1b, 0x001b }, /*                       Escape ^[ <control> */
/*{ 0xff20, 0xe220 }, **                    Multi_key ? PUA Multi-key character compose  */
/*{ 0xff21, 0xe221 }, **                        Kanji ? PUA Kanji, Kanji convert  */
/*{ 0xff22, 0xe222 }, **                     Muhenkan ? PUA Cancel Conversion  */
/*{ 0xff23, 0xe223 }, **                  Henkan_Mode ? PUA Start/Stop Conversion  */
/*{ 0xff24, 0xe224 }, **                       Romaji ? PUA to Romaji  */
/*{ 0xff25, 0xe225 }, **                     Hiragana ? PUA to Hiragana  */
/*{ 0xff26, 0xe226 }, **                     Katakana ? PUA to Katakana  */
/*{ 0xff27, 0xe227 }, **            Hiragana_Katakana ? PUA Hiragana/Katakana toggle  */
/*{ 0xff28, 0xe228 }, **                      Zenkaku ? PUA to Zenkaku  */
/*{ 0xff29, 0xe229 }, **                      Hankaku ? PUA to Hankaku  */
/*{ 0xff2a, 0xe22a }, **              Zenkaku_Hankaku ? PUA Zenkaku/Hankaku toggle  */
/*{ 0xff2b, 0xe22b }, **                      Touroku ? PUA Add to Dictionary  */
/*{ 0xff2c, 0xe22c }, **                       Massyo ? PUA Delete from Dictionary  */
/*{ 0xff2d, 0xe22d }, **                    Kana_Lock ? PUA Kana Lock  */
/*{ 0xff2e, 0xe22e }, **                   Kana_Shift ? PUA Kana Shift  */
/*{ 0xff2f, 0xe22f }, **                   Eisu_Shift ? PUA Alphanumeric Shift  */
/*{ 0xff30, 0xe230 }, **                  Eisu_toggle ? PUA Alphanumeric toggle  */
/*{ 0xff31, 0xe231 }, **                       Hangul ? PUA Hangul start/stoptoggle  */
/*{ 0xff32, 0xe232 }, **                 Hangul_Start ? PUA Hangul start  */
/*{ 0xff33, 0xe233 }, **                   Hangul_End ? PUA Hangul end, English start  */
/*{ 0xff34, 0xe234 }, **                 Hangul_Hanja ? PUA Start Hangul->Hanja Conversion  */
/*{ 0xff35, 0xe235 }, **                  Hangul_Jamo ? PUA Hangul Jamo mode  */
/*{ 0xff36, 0xe236 }, **                Hangul_Romaja ? PUA Hangul Romaja mode  */
/*{ 0xff37, 0xe237 }, **             Hangul_Codeinput ? PUA */
/*{ 0xff38, 0xe238 }, **                Hangul_Jeonja ? PUA Jeonja mode  */
/*{ 0xff39, 0xe239 }, **                 Hangul_Banja ? PUA Banja mode  */
/*{ 0xff3a, 0xe23a }, **              Hangul_PreHanja ? PUA Pre Hanja conversion  */
/*{ 0xff3b, 0xe23b }, **             Hangul_PostHanja ? PUA Post Hanja conversion  */
/*{ 0xff3c, 0xe23c }, **       Hangul_SingleCandidate ? PUA */
/*{ 0xff3d, 0xe23d }, **     Hangul_MultipleCandidate ? PUA */
/*{ 0xff3e, 0xe23e }, **     Hangul_PreviousCandidate ? PUA */
/*{ 0xff3f, 0xe23f }, **               Hangul_Special ? PUA Special symbols  */
/*{ 0xff50, 0xe250 }, **                         Home ? PUA */
/*{ 0xff51, 0xe251 }, **                         Left ? PUA Move left, left arrow  */
/*{ 0xff52, 0xe252 }, **                           Up ? PUA Move up, up arrow  */
/*{ 0xff53, 0xe253 }, **                        Right ? PUA Move right, right arrow  */
/*{ 0xff54, 0xe254 }, **                         Down ? PUA Move down, down arrow  */
/*{ 0xff55, 0xe255 }, **                        Prior ? PUA Prior, previous  */
/*{ 0xff56, 0xe256 }, **                         Next ? PUA Next  */
/*{ 0xff57, 0xe257 }, **                          End ? PUA EOL  */
/*{ 0xff58, 0xe258 }, **                        Begin ? PUA BOL  */
/*{ 0xff60, 0xe260 }, **                       Select ? PUA Select, mark  */
/*{ 0xff61, 0xe261 }, **                        Print ? PUA */
/*{ 0xff62, 0xe262 }, **                      Execute ? PUA Execute, run, do  */
/*{ 0xff63, 0xe263 }, **                       Insert ? PUA Insert, insert here  */
/*{ 0xff65, 0xe265 }, **                         Undo ? PUA */
/*{ 0xff66, 0xe266 }, **                         Redo ? PUA Redo, again  */
/*{ 0xff67, 0xe267 }, **                         Menu ? PUA */
/*{ 0xff68, 0xe268 }, **                         Find ? PUA Find, search  */
/*{ 0xff69, 0xe269 }, **                       Cancel ? PUA Cancel, stop, abort, exit  */
/*{ 0xff6a, 0xe26a }, **                         Help ? PUA Help  */
/*{ 0xff6b, 0xe26b }, **                        Break ? PUA */
/*{ 0xff7e, 0xe27e }, **                  Mode_switch ? PUA Character set switch  */
/*{ 0xff7f, 0xe27f }, **                     Num_Lock ? PUA */
/*{ 0xff80, 0xe280 }, **                     KP_Space ? PUA <U+0020 SPACE> */
/*{ 0xff89, 0xe289 }, **                       KP_Tab ? PUA <U+0009 CHARACTER TABULATION> */
/*{ 0xff8d, 0xe28d }, **                     KP_Enter ? PUA <U+000D CARRIAGE RETURN> */
/*{ 0xff91, 0xe291 }, **                        KP_F1 ? PUA PF1, KP_A, ...  */
/*{ 0xff92, 0xe292 }, **                        KP_F2 ? PUA */
/*{ 0xff93, 0xe293 }, **                        KP_F3 ? PUA */
/*{ 0xff94, 0xe294 }, **                        KP_F4 ? PUA */
/*{ 0xff95, 0xe295 }, **                      KP_Home ? PUA */
/*{ 0xff96, 0xe296 }, **                      KP_Left ? PUA */
/*{ 0xff97, 0xe297 }, **                        KP_Up ? PUA */
/*{ 0xff98, 0xe298 }, **                     KP_Right ? PUA */
/*{ 0xff99, 0xe299 }, **                      KP_Down ? PUA */
/*{ 0xff9a, 0xe29a }, **                     KP_Prior ? PUA */
/*{ 0xff9b, 0xe29b }, **                      KP_Next ? PUA */
/*{ 0xff9c, 0xe29c }, **                       KP_End ? PUA */
/*{ 0xff9d, 0xe29d }, **                     KP_Begin ? PUA */
/*{ 0xff9e, 0xe29e }, **                    KP_Insert ? PUA */
/*{ 0xff9f, 0xe29f }, **                    KP_Delete ? PUA */
/*{ 0xffaa, 0xe2aa }, **                  KP_Multiply ? PUA <U+002A ASTERISK> */
/*{ 0xffab, 0xe2ab }, **                       KP_Add ? PUA <U+002B PLUS SIGN> */
/*{ 0xffac, 0xe2ac }, **                 KP_Separator ? PUA <U+002C COMMA> */
/*{ 0xffad, 0xe2ad }, **                  KP_Subtract ? PUA <U+002D HYPHEN-MINUS> */
/*{ 0xffae, 0xe2ae }, **                   KP_Decimal ? PUA <U+002E FULL STOP> */
/*{ 0xffaf, 0xe2af }, **                    KP_Divide ? PUA <U+002F SOLIDUS> */
/*{ 0xffb0, 0xe2b0 }, **                         KP_0 ? PUA <U+0030 DIGIT ZERO> */
/*{ 0xffb1, 0xe2b1 }, **                         KP_1 ? PUA <U+0031 DIGIT ONE> */
/*{ 0xffb2, 0xe2b2 }, **                         KP_2 ? PUA <U+0032 DIGIT TWO> */
/*{ 0xffb3, 0xe2b3 }, **                         KP_3 ? PUA <U+0033 DIGIT THREE> */
/*{ 0xffb4, 0xe2b4 }, **                         KP_4 ? PUA <U+0034 DIGIT FOUR> */
/*{ 0xffb5, 0xe2b5 }, **                         KP_5 ? PUA <U+0035 DIGIT FIVE> */
/*{ 0xffb6, 0xe2b6 }, **                         KP_6 ? PUA <U+0036 DIGIT SIX> */
/*{ 0xffb7, 0xe2b7 }, **                         KP_7 ? PUA <U+0037 DIGIT SEVEN> */
/*{ 0xffb8, 0xe2b8 }, **                         KP_8 ? PUA <U+0038 DIGIT EIGHT> */
/*{ 0xffb9, 0xe2b9 }, **                         KP_9 ? PUA <U+0039 DIGIT NINE> */
/*{ 0xffbd, 0xe2bd }, **                     KP_Equal ? PUA <U+003D EQUALS SIGN> */
/*{ 0xffbe, 0xe2be }, **                           F1 ? PUA */
/*{ 0xffbf, 0xe2bf }, **                           F2 ? PUA */
/*{ 0xffc0, 0xe2c0 }, **                           F3 ? PUA */
/*{ 0xffc1, 0xe2c1 }, **                           F4 ? PUA */
/*{ 0xffc2, 0xe2c2 }, **                           F5 ? PUA */
/*{ 0xffc3, 0xe2c3 }, **                           F6 ? PUA */
/*{ 0xffc4, 0xe2c4 }, **                           F7 ? PUA */
/*{ 0xffc5, 0xe2c5 }, **                           F8 ? PUA */
/*{ 0xffc6, 0xe2c6 }, **                           F9 ? PUA */
/*{ 0xffc7, 0xe2c7 }, **                          F10 ? PUA */
/*{ 0xffc8, 0xe2c8 }, **                          F11 ? PUA */
/*{ 0xffc9, 0xe2c9 }, **                          F12 ? PUA */
/*{ 0xffca, 0xe2ca }, **                          F13 ? PUA */
/*{ 0xffcb, 0xe2cb }, **                          F14 ? PUA */
/*{ 0xffcc, 0xe2cc }, **                          F15 ? PUA */
/*{ 0xffcd, 0xe2cd }, **                          F16 ? PUA */
/*{ 0xffce, 0xe2ce }, **                          F17 ? PUA */
/*{ 0xffcf, 0xe2cf }, **                          F18 ? PUA */
/*{ 0xffd0, 0xe2d0 }, **                          F19 ? PUA */
/*{ 0xffd1, 0xe2d1 }, **                          F20 ? PUA */
/*{ 0xffd2, 0xe2d2 }, **                          F21 ? PUA */
/*{ 0xffd3, 0xe2d3 }, **                          F22 ? PUA */
/*{ 0xffd4, 0xe2d4 }, **                          F23 ? PUA */
/*{ 0xffd5, 0xe2d5 }, **                          F24 ? PUA */
/*{ 0xffd6, 0xe2d6 }, **                          F25 ? PUA */
/*{ 0xffd7, 0xe2d7 }, **                          F26 ? PUA */
/*{ 0xffd8, 0xe2d8 }, **                          F27 ? PUA */
/*{ 0xffd9, 0xe2d9 }, **                          F28 ? PUA */
/*{ 0xffda, 0xe2da }, **                          F29 ? PUA */
/*{ 0xffdb, 0xe2db }, **                          F30 ? PUA */
/*{ 0xffdc, 0xe2dc }, **                          F31 ? PUA */
/*{ 0xffdd, 0xe2dd }, **                          F32 ? PUA */
/*{ 0xffde, 0xe2de }, **                          F33 ? PUA */
/*{ 0xffdf, 0xe2df }, **                          F34 ? PUA */
/*{ 0xffe0, 0xe2e0 }, **                          F35 ? PUA */
/*{ 0xffe1, 0xe2e1 }, **                      Shift_L ? PUA Left shift  */
/*{ 0xffe2, 0xe2e2 }, **                      Shift_R ? PUA Right shift  */
/*{ 0xffe3, 0xe2e3 }, **                    Control_L ? PUA Left control  */
/*{ 0xffe4, 0xe2e4 }, **                    Control_R ? PUA Right control  */
/*{ 0xffe5, 0xe2e5 }, **                    Caps_Lock ? PUA Caps lock  */
/*{ 0xffe6, 0xe2e6 }, **                   Shift_Lock ? PUA Shift lock  */
/*{ 0xffe7, 0xe2e7 }, **                       Meta_L ? PUA Left meta  */
/*{ 0xffe8, 0xe2e8 }, **                       Meta_R ? PUA Right meta  */
/*{ 0xffe9, 0xe2e9 }, **                        Alt_L ? PUA Left alt  */
/*{ 0xffea, 0xe2ea }, **                        Alt_R ? PUA Right alt  */
/*{ 0xffeb, 0xe2eb }, **                      Super_L ? PUA Left super  */
/*{ 0xffec, 0xe2ec }, **                      Super_R ? PUA Right super  */
/*{ 0xffed, 0xe2ed }, **                      Hyper_L ? PUA Left hyper  */
/*{ 0xffee, 0xe2ee }, **                      Hyper_R ? PUA Right hyper  */
/*{ 0xfff1, 0xe2f1 }, **                braille_dot_1 ? PUA */
/*{ 0xfff2, 0xe2f2 }, **                braille_dot_2 ? PUA */
/*{ 0xfff3, 0xe2f3 }, **                braille_dot_3 ? PUA */
/*{ 0xfff4, 0xe2f4 }, **                braille_dot_4 ? PUA */
/*{ 0xfff5, 0xe2f5 }, **                braille_dot_5 ? PUA */
/*{ 0xfff6, 0xe2f6 }, **                braille_dot_6 ? PUA */
/*{ 0xfff7, 0xe2f7 }, **                braille_dot_7 ? PUA */
/*{ 0xfff8, 0xe2f8 }, **                braille_dot_8 ? PUA */
/*{ 0xfff9, 0xe2f9 }, **                braille_dot_9 ? PUA */
/*{ 0xfffa, 0xe2fa }, **               braille_dot_10 ? PUA */
  { 0xffff, 0x007f }, /*                       Delete  DELETE */
  { 0x100000a8, 0x00a8 }, /*              mute_acute ¨ DIAERESIS */
  { 0x100000a9, 0x00a9 }, /*              mute_grave © COPYRIGHT SIGN */
  { 0x100000aa, 0x00aa }, /*        mute_asciicircum ª FEMININE ORDINAL INDICATOR */
  { 0x100000ab, 0x00ab }, /*          mute_diaeresis « LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
  { 0x100000ac, 0x00ac }, /*         mute_asciitilde ¬ NOT SIGN */
  { 0x100000af, 0x00af }, /*                    lira ¯ MACRON */
  { 0x100000be, 0x00be }, /*                 guilder ¾ VULGAR FRACTION THREE QUARTERS */
  { 0x100000ee, 0x00ee }, /*              Ydiaeresis î LATIN SMALL LETTER I WITH CIRCUMFLEX */
  { 0x100000f6, 0x00f6 }, /*               longminus ö LATIN SMALL LETTER O WITH DIAERESIS */
  { 0x100000fc, 0x00fc }, /*                   block ü LATIN SMALL LETTER U WITH DIAERESIS */
/*{ 0x1000fe22, 0xf0e22 }, **               diaeresis ? PUA */
/*{ 0x1000fe27, 0xf0e27 }, **            acute_accent ? PUA */
/*{ 0x1000fe2c, 0xf0e2c }, **          cedilla_accent ? PUA */
/*{ 0x1000fe5e, 0xf0e5e }, **       circumflex_accent ? PUA */
/*{ 0x1000fe60, 0xf0e60 }, **            grave_accent ? PUA */
/*{ 0x1000fe7e, 0xf0e7e }, **                   tilde ? PUA */
/*{ 0x1000feb0, 0xf0eb0 }, **             ring_accent ? PUA */
/*{ 0x1000ff00, 0xf0f00 }, **                 LineDel ? PUA Remove  */
/*{ 0x1000ff01, 0xf0f01 }, **                 CharDel ? PUA */
/*{ 0x1000ff02, 0xf0f02 }, **                    Copy ? PUA */
/*{ 0x1000ff03, 0xf0f03 }, **                     Cut ? PUA */
/*{ 0x1000ff04, 0xf0f04 }, **                   Paste ? PUA */
/*{ 0x1000ff05, 0xf0f05 }, **                    Move ? PUA */
/*{ 0x1000ff06, 0xf0f06 }, **                    Grow ? PUA */
/*{ 0x1000ff07, 0xf0f07 }, **                     Cmd ? PUA */
/*{ 0x1000ff08, 0xf0f08 }, **                   Shell ? PUA */
/*{ 0x1000ff09, 0xf0f09 }, **                 LeftBar ? PUA */
/*{ 0x1000ff0a, 0xf0f0a }, **                RightBar ? PUA */
/*{ 0x1000ff0b, 0xf0f0b }, **                 LeftBox ? PUA */
/*{ 0x1000ff0c, 0xf0f0c }, **                RightBox ? PUA */
/*{ 0x1000ff0d, 0xf0f0d }, **                   UpBox ? PUA */
/*{ 0x1000ff0e, 0xf0f0e }, **                 DownBox ? PUA */
/*{ 0x1000ff0f, 0xf0f0f }, **                     Pop ? PUA */
/*{ 0x1000ff10, 0xf0f10 }, **                    Read ? PUA */
/*{ 0x1000ff11, 0xf0f11 }, **                    Edit ? PUA */
/*{ 0x1000ff12, 0xf0f12 }, **                    Save ? PUA */
/*{ 0x1000ff13, 0xf0f13 }, **                    Exit ? PUA */
/*{ 0x1000ff14, 0xf0f14 }, **                  Repeat ? PUA */
/*{ 0x1000ff48, 0xf0f48 }, **               Modelock1 ? PUA */
/*{ 0x1000ff49, 0xf0f49 }, **               Modelock2 ? PUA */
/*{ 0x1000ff6c, 0xf0f6c }, **                   Reset ? PUA */
/*{ 0x1000ff6d, 0xf0f6d }, **                  System ? PUA */
/*{ 0x1000ff6e, 0xf0f6e }, **                    User ? PUA */
/*{ 0x1000ff6f, 0xf0f6f }, **               ClearLine ? PUA */
/*{ 0x1000ff70, 0xf0f70 }, **              InsertLine ? PUA */
/*{ 0x1000ff71, 0xf0f71 }, **              DeleteLine ? PUA */
/*{ 0x1000ff72, 0xf0f72 }, **              InsertChar ? PUA */
/*{ 0x1000ff73, 0xf0f73 }, **              DeleteChar ? PUA */
/*{ 0x1000ff74, 0xf0f74 }, **                 BackTab ? PUA */
/*{ 0x1000ff75, 0xf0f75 }, **              KP_BackTab ? PUA */
/*{ 0x1000ffa8, 0xf0fa8 }, **            KP_parenleft ? PUA */
/*{ 0x1000ffa9, 0xf0fa9 }, **           KP_parenright ? PUA */
/*{ 0x1004ff02, 0xf1f02 }, **                    Copy ? PUA */
/*{ 0x1004ff03, 0xf1f03 }, **                     Cut ? PUA */
/*{ 0x1004ff04, 0xf1f04 }, **                   Paste ? PUA */
/*{ 0x1004ff07, 0xf1f07 }, **                 BackTab ? PUA */
/*{ 0x1004ff08, 0xf1f08 }, **               BackSpace ? PUA */
/*{ 0x1004ff0b, 0xf1f0b }, **                   Clear ? PUA */
/*{ 0x1004ff1b, 0xf1f1b }, **                  Escape ? PUA */
/*{ 0x1004ff31, 0xf1f31 }, **                 AddMode ? PUA */
/*{ 0x1004ff32, 0xf1f32 }, **            PrimaryPaste ? PUA */
/*{ 0x1004ff33, 0xf1f33 }, **              QuickPaste ? PUA */
/*{ 0x1004ff40, 0xf1f40 }, **                PageLeft ? PUA */
/*{ 0x1004ff41, 0xf1f41 }, **                  PageUp ? PUA */
/*{ 0x1004ff42, 0xf1f42 }, **                PageDown ? PUA */
/*{ 0x1004ff43, 0xf1f43 }, **               PageRight ? PUA */
/*{ 0x1004ff44, 0xf1f44 }, **                Activate ? PUA */
/*{ 0x1004ff45, 0xf1f45 }, **                 MenuBar ? PUA */
/*{ 0x1004ff51, 0xf1f51 }, **                    Left ? PUA */
/*{ 0x1004ff52, 0xf1f52 }, **                      Up ? PUA */
/*{ 0x1004ff53, 0xf1f53 }, **                   Right ? PUA */
/*{ 0x1004ff54, 0xf1f54 }, **                    Down ? PUA */
/*{ 0x1004ff57, 0xf1f57 }, **                 EndLine ? PUA */
/*{ 0x1004ff58, 0xf1f58 }, **               BeginLine ? PUA */
/*{ 0x1004ff59, 0xf1f59 }, **                 EndData ? PUA */
/*{ 0x1004ff5a, 0xf1f5a }, **               BeginData ? PUA */
/*{ 0x1004ff5b, 0xf1f5b }, **                PrevMenu ? PUA */
/*{ 0x1004ff5c, 0xf1f5c }, **                NextMenu ? PUA */
/*{ 0x1004ff5d, 0xf1f5d }, **               PrevField ? PUA */
/*{ 0x1004ff5e, 0xf1f5e }, **               NextField ? PUA */
/*{ 0x1004ff60, 0xf1f60 }, **                  Select ? PUA */
/*{ 0x1004ff63, 0xf1f63 }, **                  Insert ? PUA */
/*{ 0x1004ff65, 0xf1f65 }, **                    Undo ? PUA */
/*{ 0x1004ff67, 0xf1f67 }, **                    Menu ? PUA */
/*{ 0x1004ff69, 0xf1f69 }, **                  Cancel ? PUA */
/*{ 0x1004ff6a, 0xf1f6a }, **                    Help ? PUA */
/*{ 0x1004ff71, 0xf1f71 }, **               SelectAll ? PUA */
/*{ 0x1004ff72, 0xf1f72 }, **             DeselectAll ? PUA */
/*{ 0x1004ff73, 0xf1f73 }, **                Reselect ? PUA */
/*{ 0x1004ff74, 0xf1f74 }, **                  Extend ? PUA */
/*{ 0x1004ff78, 0xf1f78 }, **                 Restore ? PUA */
/*{ 0x1004ffff, 0xf1fff }, **                  Delete ? PUA */
/*{ 0x1005ff00, 0xf2f00 }, **                FA_Grave ? PUA */
/*{ 0x1005ff01, 0xf2f01 }, **               FA_Circum ? PUA */
/*{ 0x1005ff02, 0xf2f02 }, **                FA_Tilde ? PUA */
/*{ 0x1005ff03, 0xf2f03 }, **                FA_Acute ? PUA */
/*{ 0x1005ff04, 0xf2f04 }, **            FA_Diaeresis ? PUA */
/*{ 0x1005ff05, 0xf2f05 }, **              FA_Cedilla ? PUA */
/*{ 0x1005ff10, 0xf2f10 }, **                     F36 ? PUA Labeled F11  */
/*{ 0x1005ff11, 0xf2f11 }, **                     F37 ? PUA Labeled F12  */
/*{ 0x1005ff60, 0xf2f60 }, **                 Sys_Req ? PUA */
/*{ 0x1005ff70, 0xf2f70 }, **                   Props ? PUA */
/*{ 0x1005ff71, 0xf2f71 }, **                   Front ? PUA */
/*{ 0x1005ff72, 0xf2f72 }, **                    Copy ? PUA */
/*{ 0x1005ff73, 0xf2f73 }, **                    Open ? PUA */
/*{ 0x1005ff74, 0xf2f74 }, **                   Paste ? PUA */
/*{ 0x1005ff75, 0xf2f75 }, **                     Cut ? PUA */
/*{ 0x1005ff76, 0xf2f76 }, **             PowerSwitch ? PUA */
/*{ 0x1005ff77, 0xf2f77 }, **        AudioLowerVolume ? PUA */
/*{ 0x1005ff78, 0xf2f78 }, **               AudioMute ? PUA */
/*{ 0x1005ff79, 0xf2f79 }, **        AudioRaiseVolume ? PUA */
/*{ 0x1005ff7a, 0xf2f7a }, **            VideoDegauss ? PUA */
/*{ 0x1005ff7b, 0xf2f7b }, **    VideoLowerBrightness ? PUA */
/*{ 0x1005ff7c, 0xf2f7c }, **    VideoRaiseBrightness ? PUA */
/*{ 0x1005ff7d, 0xf2f7d }, **        PowerSwitchShift ? PUA */
/*{ 0x100810f4, 0xf40f4 }, **          BrightnessAuto ? PUA v3.16   KEY_BRIGHTNESS_AUTO  */
/*{ 0x100810f5, 0xf40f5 }, **              DisplayOff ? PUA v2.6.23 KEY_DISPLAY_OFF  */
/*{ 0x10081166, 0xf4166 }, **                    Info ? PUA KEY_INFO  */
/*{ 0x10081177, 0xf4177 }, **             AspectRatio ? PUA v5.1    KEY_ASPECT_RATIO  */
/*{ 0x10081185, 0xf4185 }, **                     DVD ? PUA KEY_DVD  */
/*{ 0x10081188, 0xf4188 }, **                   Audio ? PUA KEY_AUDIO  */
/*{ 0x10081192, 0xf4192 }, **               ChannelUp ? PUA KEY_CHANNELUP  */
/*{ 0x10081193, 0xf4193 }, **             ChannelDown ? PUA KEY_CHANNELDOWN  */
/*{ 0x1008119b, 0xf419b }, **                   Break ? PUA KEY_BREAK  */
/*{ 0x100811a0, 0xf41a0 }, **              VideoPhone ? PUA v2.6.20 KEY_VIDEOPHONE  */
/*{ 0x100811a4, 0xf41a4 }, **               ZoomReset ? PUA v2.6.20 KEY_ZOOMRESET  */
/*{ 0x100811a6, 0xf41a6 }, **                  Editor ? PUA v2.6.20 KEY_EDITOR  */
/*{ 0x100811a8, 0xf41a8 }, **          GraphicsEditor ? PUA v2.6.20 KEY_GRAPHICSEDITOR  */
/*{ 0x100811a9, 0xf41a9 }, **            Presentation ? PUA v2.6.20 KEY_PRESENTATION  */
/*{ 0x100811aa, 0xf41aa }, **                Database ? PUA v2.6.20 KEY_DATABASE  */
/*{ 0x100811ac, 0xf41ac }, **               Voicemail ? PUA v2.6.20 KEY_VOICEMAIL  */
/*{ 0x100811ad, 0xf41ad }, **             Addressbook ? PUA v2.6.20 KEY_ADDRESSBOOK  */
/*{ 0x100811af, 0xf41af }, **           DisplayToggle ? PUA v2.6.20 KEY_DISPLAYTOGGLE  */
/*{ 0x100811b0, 0xf41b0 }, **              SpellCheck ? PUA v2.6.24 KEY_SPELLCHECK  */
/*{ 0x100811b6, 0xf41b6 }, **             ContextMenu ? PUA v2.6.24 KEY_CONTEXT_MENU  */
/*{ 0x100811b7, 0xf41b7 }, **             MediaRepeat ? PUA v2.6.26 KEY_MEDIA_REPEAT  */
/*{ 0x100811b8, 0xf41b8 }, **            10ChannelsUp ? PUA v2.6.38 KEY_10CHANNELSUP  */
/*{ 0x100811b9, 0xf41b9 }, **          10ChannelsDown ? PUA v2.6.38 KEY_10CHANNELSDOWN  */
/*{ 0x100811ba, 0xf41ba }, **                  Images ? PUA v2.6.39 KEY_IMAGES  */
/*{ 0x100811bc, 0xf41bc }, **      NotificationCenter ? PUA v5.10   KEY_NOTIFICATION_CENTER  */
/*{ 0x100811bd, 0xf41bd }, **             PickupPhone ? PUA v5.10   KEY_PICKUP_PHONE  */
/*{ 0x100811be, 0xf41be }, **             HangupPhone ? PUA v5.10   KEY_HANGUP_PHONE  */
/*{ 0x100811d0, 0xf41d0 }, **                      Fn ? PUA KEY_FN  */
/*{ 0x100811d1, 0xf41d1 }, **                  Fn_Esc ? PUA KEY_FN_ESC  */
/*{ 0x100811e5, 0xf41e5 }, **            FnRightShift ? PUA v5.10   KEY_FN_RIGHT_SHIFT  */
/*{ 0x10081200, 0xf4200 }, **                Numeric0 ? PUA v2.6.28 KEY_NUMERIC_0  */
/*{ 0x10081201, 0xf4201 }, **                Numeric1 ? PUA v2.6.28 KEY_NUMERIC_1  */
/*{ 0x10081202, 0xf4202 }, **                Numeric2 ? PUA v2.6.28 KEY_NUMERIC_2  */
/*{ 0x10081203, 0xf4203 }, **                Numeric3 ? PUA v2.6.28 KEY_NUMERIC_3  */
/*{ 0x10081204, 0xf4204 }, **                Numeric4 ? PUA v2.6.28 KEY_NUMERIC_4  */
/*{ 0x10081205, 0xf4205 }, **                Numeric5 ? PUA v2.6.28 KEY_NUMERIC_5  */
/*{ 0x10081206, 0xf4206 }, **                Numeric6 ? PUA v2.6.28 KEY_NUMERIC_6  */
/*{ 0x10081207, 0xf4207 }, **                Numeric7 ? PUA v2.6.28 KEY_NUMERIC_7  */
/*{ 0x10081208, 0xf4208 }, **                Numeric8 ? PUA v2.6.28 KEY_NUMERIC_8  */
/*{ 0x10081209, 0xf4209 }, **                Numeric9 ? PUA v2.6.28 KEY_NUMERIC_9  */
/*{ 0x1008120a, 0xf420a }, **             NumericStar ? PUA v2.6.28 KEY_NUMERIC_STAR  */
/*{ 0x1008120b, 0xf420b }, **            NumericPound ? PUA v2.6.28 KEY_NUMERIC_POUND  */
/*{ 0x1008120c, 0xf420c }, **                NumericA ? PUA v4.1    KEY_NUMERIC_A  */
/*{ 0x1008120d, 0xf420d }, **                NumericB ? PUA v4.1    KEY_NUMERIC_B  */
/*{ 0x1008120e, 0xf420e }, **                NumericC ? PUA v4.1    KEY_NUMERIC_C  */
/*{ 0x1008120f, 0xf420f }, **                NumericD ? PUA v4.1    KEY_NUMERIC_D  */
/*{ 0x10081210, 0xf4210 }, **             CameraFocus ? PUA v2.6.33 KEY_CAMERA_FOCUS  */
/*{ 0x10081211, 0xf4211 }, **               WPSButton ? PUA v2.6.34 KEY_WPS_BUTTON  */
/*{ 0x10081215, 0xf4215 }, **            CameraZoomIn ? PUA v2.6.39 KEY_CAMERA_ZOOMIN  */
/*{ 0x10081216, 0xf4216 }, **           CameraZoomOut ? PUA v2.6.39 KEY_CAMERA_ZOOMOUT  */
/*{ 0x10081217, 0xf4217 }, **                CameraUp ? PUA v2.6.39 KEY_CAMERA_UP  */
/*{ 0x10081218, 0xf4218 }, **              CameraDown ? PUA v2.6.39 KEY_CAMERA_DOWN  */
/*{ 0x10081219, 0xf4219 }, **              CameraLeft ? PUA v2.6.39 KEY_CAMERA_LEFT  */
/*{ 0x1008121a, 0xf421a }, **             CameraRight ? PUA v2.6.39 KEY_CAMERA_RIGHT  */
/*{ 0x1008121b, 0xf421b }, **             AttendantOn ? PUA v3.10   KEY_ATTENDANT_ON  */
/*{ 0x1008121c, 0xf421c }, **            AttendantOff ? PUA v3.10   KEY_ATTENDANT_OFF  */
/*{ 0x1008121d, 0xf421d }, **         AttendantToggle ? PUA v3.10   KEY_ATTENDANT_TOGGLE  */
/*{ 0x1008121e, 0xf421e }, **            LightsToggle ? PUA v3.10   KEY_LIGHTS_TOGGLE  */
/*{ 0x10081230, 0xf4230 }, **               ALSToggle ? PUA v3.13   KEY_ALS_TOGGLE  */
/*{ 0x10081240, 0xf4240 }, **            Buttonconfig ? PUA v3.16   KEY_BUTTONCONFIG  */
/*{ 0x10081241, 0xf4241 }, **             Taskmanager ? PUA v3.16   KEY_TASKMANAGER  */
/*{ 0x10081242, 0xf4242 }, **                 Journal ? PUA v3.16   KEY_JOURNAL  */
/*{ 0x10081243, 0xf4243 }, **            ControlPanel ? PUA v3.16   KEY_CONTROLPANEL  */
/*{ 0x10081244, 0xf4244 }, **               AppSelect ? PUA v3.16   KEY_APPSELECT  */
/*{ 0x10081245, 0xf4245 }, **             Screensaver ? PUA v3.16   KEY_SCREENSAVER  */
/*{ 0x10081246, 0xf4246 }, **            VoiceCommand ? PUA v3.16   KEY_VOICECOMMAND  */
/*{ 0x10081247, 0xf4247 }, **               Assistant ? PUA v4.13   KEY_ASSISTANT  */
/*{ 0x10081249, 0xf4249 }, **             EmojiPicker ? PUA v5.13   KEY_EMOJI_PICKER  */
/*{ 0x1008124a, 0xf424a }, **                 Dictate ? PUA v5.17   KEY_DICTATE  */
/*{ 0x1008124b, 0xf424b }, **      CameraAccessEnable ? PUA v6.2    KEY_CAMERA_ACCESS_ENABLE  */
/*{ 0x1008124c, 0xf424c }, **     CameraAccessDisable ? PUA v6.2    KEY_CAMERA_ACCESS_DISABLE  */
/*{ 0x1008124d, 0xf424d }, **      CameraAccessToggle ? PUA v6.2    KEY_CAMERA_ACCESS_TOGGLE  */
/*{ 0x10081250, 0xf4250 }, **           BrightnessMin ? PUA v3.16   KEY_BRIGHTNESS_MIN  */
/*{ 0x10081251, 0xf4251 }, **           BrightnessMax ? PUA v3.16   KEY_BRIGHTNESS_MAX  */
/*{ 0x10081260, 0xf4260 }, **      KbdInputAssistPrev ? PUA v3.18   KEY_KBDINPUTASSIST_PREV  */
/*{ 0x10081261, 0xf4261 }, **      KbdInputAssistNext ? PUA v3.18   KEY_KBDINPUTASSIST_NEXT  */
/*{ 0x10081262, 0xf4262 }, ** KbdInputAssistPrevgroup ? PUA v3.18   KEY_KBDINPUTASSIST_PREVGROUP  */
/*{ 0x10081263, 0xf4263 }, ** KbdInputAssistNextgroup ? PUA v3.18   KEY_KBDINPUTASSIST_NEXTGROUP  */
/*{ 0x10081264, 0xf4264 }, **    KbdInputAssistAccept ? PUA v3.18   KEY_KBDINPUTASSIST_ACCEPT  */
/*{ 0x10081265, 0xf4265 }, **    KbdInputAssistCancel ? PUA v3.18   KEY_KBDINPUTASSIST_CANCEL  */
/*{ 0x10081266, 0xf4266 }, **                 RightUp ? PUA v4.7    KEY_RIGHT_UP  */
/*{ 0x10081267, 0xf4267 }, **               RightDown ? PUA v4.7    KEY_RIGHT_DOWN  */
/*{ 0x10081268, 0xf4268 }, **                  LeftUp ? PUA v4.7    KEY_LEFT_UP  */
/*{ 0x10081269, 0xf4269 }, **                LeftDown ? PUA v4.7    KEY_LEFT_DOWN  */
/*{ 0x1008126a, 0xf426a }, **                RootMenu ? PUA v4.7    KEY_ROOT_MENU  */
/*{ 0x1008126b, 0xf426b }, **            MediaTopMenu ? PUA v4.7    KEY_MEDIA_TOP_MENU  */
/*{ 0x1008126c, 0xf426c }, **               Numeric11 ? PUA v4.7    KEY_NUMERIC_11  */
/*{ 0x1008126d, 0xf426d }, **               Numeric12 ? PUA v4.7    KEY_NUMERIC_12  */
/*{ 0x1008126e, 0xf426e }, **               AudioDesc ? PUA v4.7    KEY_AUDIO_DESC  */
/*{ 0x1008126f, 0xf426f }, **                  3DMode ? PUA v4.7    KEY_3D_MODE  */
/*{ 0x10081270, 0xf4270 }, **            NextFavorite ? PUA v4.7    KEY_NEXT_FAVORITE  */
/*{ 0x10081271, 0xf4271 }, **              StopRecord ? PUA v4.7    KEY_STOP_RECORD  */
/*{ 0x10081272, 0xf4272 }, **             PauseRecord ? PUA v4.7    KEY_PAUSE_RECORD  */
/*{ 0x10081273, 0xf4273 }, **                     VOD ? PUA v4.7    KEY_VOD  */
/*{ 0x10081274, 0xf4274 }, **                  Unmute ? PUA v4.7    KEY_UNMUTE  */
/*{ 0x10081275, 0xf4275 }, **             FastReverse ? PUA v4.7    KEY_FASTREVERSE  */
/*{ 0x10081276, 0xf4276 }, **             SlowReverse ? PUA v4.7    KEY_SLOWREVERSE  */
/*{ 0x10081277, 0xf4277 }, **                    Data ? PUA v4.7    KEY_DATA  */
/*{ 0x10081278, 0xf4278 }, **        OnScreenKeyboard ? PUA v4.12   KEY_ONSCREEN_KEYBOARD  */
/*{ 0x10081279, 0xf4279 }, **     PrivacyScreenToggle ? PUA v5.5    KEY_PRIVACY_SCREEN_TOGGLE  */
/*{ 0x1008127a, 0xf427a }, **     SelectiveScreenshot ? PUA v5.6    KEY_SELECTIVE_SCREENSHOT  */
/*{ 0x1008127b, 0xf427b }, **             NextElement ? PUA v5.18   KEY_NEXT_ELEMENT  */
/*{ 0x1008127c, 0xf427c }, **         PreviousElement ? PUA v5.18   KEY_PREVIOUS_ELEMENT  */
/*{ 0x1008127d, 0xf427d }, **   AutopilotEngageToggle ? PUA v5.18   KEY_AUTOPILOT_ENGAGE_TOGGLE  */
/*{ 0x1008127e, 0xf427e }, **            MarkWaypoint ? PUA v5.18   KEY_MARK_WAYPOINT  */
/*{ 0x1008127f, 0xf427f }, **                     Sos ? PUA v5.18   KEY_SOS  */
/*{ 0x10081280, 0xf4280 }, **                NavChart ? PUA v5.18   KEY_NAV_CHART  */
/*{ 0x10081281, 0xf4281 }, **            FishingChart ? PUA v5.18   KEY_FISHING_CHART  */
/*{ 0x10081282, 0xf4282 }, **        SingleRangeRadar ? PUA v5.18   KEY_SINGLE_RANGE_RADAR  */
/*{ 0x10081283, 0xf4283 }, **          DualRangeRadar ? PUA v5.18   KEY_DUAL_RANGE_RADAR  */
/*{ 0x10081284, 0xf4284 }, **            RadarOverlay ? PUA v5.18   KEY_RADAR_OVERLAY  */
/*{ 0x10081285, 0xf4285 }, **        TraditionalSonar ? PUA v5.18   KEY_TRADITIONAL_SONAR  */
/*{ 0x10081286, 0xf4286 }, **            ClearvuSonar ? PUA v5.18   KEY_CLEARVU_SONAR  */
/*{ 0x10081287, 0xf4287 }, **             SidevuSonar ? PUA v5.18   KEY_SIDEVU_SONAR  */
/*{ 0x10081288, 0xf4288 }, **                 NavInfo ? PUA v5.18   KEY_NAV_INFO  */
/*{ 0x10081290, 0xf4290 }, **                  Macro1 ? PUA v5.5    KEY_MACRO1  */
/*{ 0x10081291, 0xf4291 }, **                  Macro2 ? PUA v5.5    KEY_MACRO2  */
/*{ 0x10081292, 0xf4292 }, **                  Macro3 ? PUA v5.5    KEY_MACRO3  */
/*{ 0x10081293, 0xf4293 }, **                  Macro4 ? PUA v5.5    KEY_MACRO4  */
/*{ 0x10081294, 0xf4294 }, **                  Macro5 ? PUA v5.5    KEY_MACRO5  */
/*{ 0x10081295, 0xf4295 }, **                  Macro6 ? PUA v5.5    KEY_MACRO6  */
/*{ 0x10081296, 0xf4296 }, **                  Macro7 ? PUA v5.5    KEY_MACRO7  */
/*{ 0x10081297, 0xf4297 }, **                  Macro8 ? PUA v5.5    KEY_MACRO8  */
/*{ 0x10081298, 0xf4298 }, **                  Macro9 ? PUA v5.5    KEY_MACRO9  */
/*{ 0x10081299, 0xf4299 }, **                 Macro10 ? PUA v5.5    KEY_MACRO10  */
/*{ 0x1008129a, 0xf429a }, **                 Macro11 ? PUA v5.5    KEY_MACRO11  */
/*{ 0x1008129b, 0xf429b }, **                 Macro12 ? PUA v5.5    KEY_MACRO12  */
/*{ 0x1008129c, 0xf429c }, **                 Macro13 ? PUA v5.5    KEY_MACRO13  */
/*{ 0x1008129d, 0xf429d }, **                 Macro14 ? PUA v5.5    KEY_MACRO14  */
/*{ 0x1008129e, 0xf429e }, **                 Macro15 ? PUA v5.5    KEY_MACRO15  */
/*{ 0x1008129f, 0xf429f }, **                 Macro16 ? PUA v5.5    KEY_MACRO16  */
/*{ 0x100812a0, 0xf42a0 }, **                 Macro17 ? PUA v5.5    KEY_MACRO17  */
/*{ 0x100812a1, 0xf42a1 }, **                 Macro18 ? PUA v5.5    KEY_MACRO18  */
/*{ 0x100812a2, 0xf42a2 }, **                 Macro19 ? PUA v5.5    KEY_MACRO19  */
/*{ 0x100812a3, 0xf42a3 }, **                 Macro20 ? PUA v5.5    KEY_MACRO20  */
/*{ 0x100812a4, 0xf42a4 }, **                 Macro21 ? PUA v5.5    KEY_MACRO21  */
/*{ 0x100812a5, 0xf42a5 }, **                 Macro22 ? PUA v5.5    KEY_MACRO22  */
/*{ 0x100812a6, 0xf42a6 }, **                 Macro23 ? PUA v5.5    KEY_MACRO23  */
/*{ 0x100812a7, 0xf42a7 }, **                 Macro24 ? PUA v5.5    KEY_MACRO24  */
/*{ 0x100812a8, 0xf42a8 }, **                 Macro25 ? PUA v5.5    KEY_MACRO25  */
/*{ 0x100812a9, 0xf42a9 }, **                 Macro26 ? PUA v5.5    KEY_MACRO26  */
/*{ 0x100812aa, 0xf42aa }, **                 Macro27 ? PUA v5.5    KEY_MACRO27  */
/*{ 0x100812ab, 0xf42ab }, **                 Macro28 ? PUA v5.5    KEY_MACRO28  */
/*{ 0x100812ac, 0xf42ac }, **                 Macro29 ? PUA v5.5    KEY_MACRO29  */
/*{ 0x100812ad, 0xf42ad }, **                 Macro30 ? PUA v5.5    KEY_MACRO30  */
/*{ 0x100812b0, 0xf42b0 }, **        MacroRecordStart ? PUA v5.5    KEY_MACRO_RECORD_START  */
/*{ 0x100812b1, 0xf42b1 }, **         MacroRecordStop ? PUA v5.5    KEY_MACRO_RECORD_STOP  */
/*{ 0x100812b2, 0xf42b2 }, **        MacroPresetCycle ? PUA v5.5    KEY_MACRO_PRESET_CYCLE  */
/*{ 0x100812b3, 0xf42b3 }, **            MacroPreset1 ? PUA v5.5    KEY_MACRO_PRESET1  */
/*{ 0x100812b4, 0xf42b4 }, **            MacroPreset2 ? PUA v5.5    KEY_MACRO_PRESET2  */
/*{ 0x100812b5, 0xf42b5 }, **            MacroPreset3 ? PUA v5.5    KEY_MACRO_PRESET3  */
/*{ 0x100812b8, 0xf42b8 }, **             KbdLcdMenu1 ? PUA v5.5    KEY_KBD_LCD_MENU1  */
/*{ 0x100812b9, 0xf42b9 }, **             KbdLcdMenu2 ? PUA v5.5    KEY_KBD_LCD_MENU2  */
/*{ 0x100812ba, 0xf42ba }, **             KbdLcdMenu3 ? PUA v5.5    KEY_KBD_LCD_MENU3  */
/*{ 0x100812bb, 0xf42bb }, **             KbdLcdMenu4 ? PUA v5.5    KEY_KBD_LCD_MENU4  */
/*{ 0x100812bc, 0xf42bc }, **             KbdLcdMenu5 ? PUA v5.5    KEY_KBD_LCD_MENU5  */
/*{ 0x1008fe01, 0xf6001 }, **             Switch_VT_1 ? PUA */
/*{ 0x1008fe02, 0xf6002 }, **             Switch_VT_2 ? PUA */
/*{ 0x1008fe03, 0xf6003 }, **             Switch_VT_3 ? PUA */
/*{ 0x1008fe04, 0xf6004 }, **             Switch_VT_4 ? PUA */
/*{ 0x1008fe05, 0xf6005 }, **             Switch_VT_5 ? PUA */
/*{ 0x1008fe06, 0xf6006 }, **             Switch_VT_6 ? PUA */
/*{ 0x1008fe07, 0xf6007 }, **             Switch_VT_7 ? PUA */
/*{ 0x1008fe08, 0xf6008 }, **             Switch_VT_8 ? PUA */
/*{ 0x1008fe09, 0xf6009 }, **             Switch_VT_9 ? PUA */
/*{ 0x1008fe0a, 0xf600a }, **            Switch_VT_10 ? PUA */
/*{ 0x1008fe0b, 0xf600b }, **            Switch_VT_11 ? PUA */
/*{ 0x1008fe0c, 0xf600c }, **            Switch_VT_12 ? PUA */
/*{ 0x1008fe20, 0xf6020 }, **                  Ungrab ? PUA force ungrab                */
/*{ 0x1008fe21, 0xf6021 }, **               ClearGrab ? PUA kill application with grab  */
/*{ 0x1008fe22, 0xf6022 }, **              Next_VMode ? PUA next video mode available   */
/*{ 0x1008fe23, 0xf6023 }, **              Prev_VMode ? PUA prev. video mode available  */
/*{ 0x1008fe24, 0xf6024 }, **           LogWindowTree ? PUA print window tree to log    */
/*{ 0x1008fe25, 0xf6025 }, **             LogGrabInfo ? PUA print all active grabs to log  */
/*{ 0x1008ff01, 0xf6101 }, **                ModeLock ? PUA Mode Switch Lock  */
/*{ 0x1008ff02, 0xf6102 }, **         MonBrightnessUp ? PUA Monitor/panel brightness  */
/*{ 0x1008ff03, 0xf6103 }, **       MonBrightnessDown ? PUA Monitor/panel brightness  */
/*{ 0x1008ff04, 0xf6104 }, **           KbdLightOnOff ? PUA Keyboards may be lit      */
/*{ 0x1008ff05, 0xf6105 }, **         KbdBrightnessUp ? PUA Keyboards may be lit      */
/*{ 0x1008ff06, 0xf6106 }, **       KbdBrightnessDown ? PUA Keyboards may be lit      */
/*{ 0x1008ff07, 0xf6107 }, **      MonBrightnessCycle ? PUA Monitor/panel brightness  */
/*{ 0x1008ff10, 0xf6110 }, **                 Standby ? PUA System into standby mode    */
/*{ 0x1008ff11, 0xf6111 }, **        AudioLowerVolume ? PUA Volume control down         */
/*{ 0x1008ff12, 0xf6112 }, **               AudioMute ? PUA Mute sound from the system  */
/*{ 0x1008ff13, 0xf6113 }, **        AudioRaiseVolume ? PUA Volume control up           */
/*{ 0x1008ff14, 0xf6114 }, **               AudioPlay ? PUA Start playing of audio >    */
/*{ 0x1008ff15, 0xf6115 }, **               AudioStop ? PUA Stop playing audio          */
/*{ 0x1008ff16, 0xf6116 }, **               AudioPrev ? PUA Previous track              */
/*{ 0x1008ff17, 0xf6117 }, **               AudioNext ? PUA Next track                  */
/*{ 0x1008ff18, 0xf6118 }, **                HomePage ? PUA Display user's home page    */
/*{ 0x1008ff19, 0xf6119 }, **                    Mail ? PUA Invoke user's mail program  */
/*{ 0x1008ff1a, 0xf611a }, **                   Start ? PUA Start application           */
/*{ 0x1008ff1b, 0xf611b }, **                  Search ? PUA Search                      */
/*{ 0x1008ff1c, 0xf611c }, **             AudioRecord ? PUA Record audio application    */
/*{ 0x1008ff1d, 0xf611d }, **              Calculator ? PUA Invoke calculator program   */
/*{ 0x1008ff1e, 0xf611e }, **                    Memo ? PUA Invoke Memo taking program  */
/*{ 0x1008ff1f, 0xf611f }, **                ToDoList ? PUA Invoke To Do List program   */
/*{ 0x1008ff20, 0xf6120 }, **                Calendar ? PUA Invoke Calendar program     */
/*{ 0x1008ff21, 0xf6121 }, **               PowerDown ? PUA Deep sleep the system       */
/*{ 0x1008ff22, 0xf6122 }, **          ContrastAdjust ? PUA Adjust screen contrast      */
/*{ 0x1008ff23, 0xf6123 }, **                RockerUp ? PUA Rocker switches exist up    */
/*{ 0x1008ff24, 0xf6124 }, **              RockerDown ? PUA and down                    */
/*{ 0x1008ff25, 0xf6125 }, **             RockerEnter ? PUA and let you press them      */
/*{ 0x1008ff26, 0xf6126 }, **                    Back ? PUA Like back on a browser      */
/*{ 0x1008ff27, 0xf6127 }, **                 Forward ? PUA Like forward on a browser   */
/*{ 0x1008ff28, 0xf6128 }, **                    Stop ? PUA Stop current operation      */
/*{ 0x1008ff29, 0xf6129 }, **                 Refresh ? PUA Refresh the page            */
/*{ 0x1008ff2a, 0xf612a }, **                PowerOff ? PUA Power off system entirely   */
/*{ 0x1008ff2b, 0xf612b }, **                  WakeUp ? PUA Wake up system from sleep   */
/*{ 0x1008ff2c, 0xf612c }, **                   Eject ? PUA Eject device e.g. DVD     */
/*{ 0x1008ff2d, 0xf612d }, **             ScreenSaver ? PUA Invoke screensaver          */
/*{ 0x1008ff2e, 0xf612e }, **                     WWW ? PUA Invoke web browser          */
/*{ 0x1008ff2f, 0xf612f }, **                   Sleep ? PUA Put system to sleep         */
/*{ 0x1008ff30, 0xf6130 }, **               Favorites ? PUA Show favorite locations     */
/*{ 0x1008ff31, 0xf6131 }, **              AudioPause ? PUA Pause audio playing         */
/*{ 0x1008ff32, 0xf6132 }, **              AudioMedia ? PUA Launch media collection app  */
/*{ 0x1008ff33, 0xf6133 }, **              MyComputer ? PUA Display "My Computer" window  */
/*{ 0x1008ff34, 0xf6134 }, **              VendorHome ? PUA Display vendor home web site  */
/*{ 0x1008ff35, 0xf6135 }, **               LightBulb ? PUA Light bulb keys exist        */
/*{ 0x1008ff36, 0xf6136 }, **                    Shop ? PUA Display shopping web site    */
/*{ 0x1008ff37, 0xf6137 }, **                 History ? PUA Show history of web surfing  */
/*{ 0x1008ff38, 0xf6138 }, **                 OpenURL ? PUA Open selected URL            */
/*{ 0x1008ff39, 0xf6139 }, **             AddFavorite ? PUA Add URL to favorites list    */
/*{ 0x1008ff3a, 0xf613a }, **                HotLinks ? PUA Show "hot" links             */
/*{ 0x1008ff3b, 0xf613b }, **        BrightnessAdjust ? PUA Invoke brightness adj. UI    */
/*{ 0x1008ff3c, 0xf613c }, **                 Finance ? PUA Display financial site       */
/*{ 0x1008ff3d, 0xf613d }, **               Community ? PUA Display user's community     */
/*{ 0x1008ff3e, 0xf613e }, **             AudioRewind ? PUA "rewind" audio track         */
/*{ 0x1008ff3f, 0xf613f }, **             BackForward ? PUA ???  */
/*{ 0x1008ff40, 0xf6140 }, **                 Launch0 ? PUA Launch Application           */
/*{ 0x1008ff41, 0xf6141 }, **                 Launch1 ? PUA Launch Application           */
/*{ 0x1008ff42, 0xf6142 }, **                 Launch2 ? PUA Launch Application           */
/*{ 0x1008ff43, 0xf6143 }, **                 Launch3 ? PUA Launch Application           */
/*{ 0x1008ff44, 0xf6144 }, **                 Launch4 ? PUA Launch Application           */
/*{ 0x1008ff45, 0xf6145 }, **                 Launch5 ? PUA Launch Application           */
/*{ 0x1008ff46, 0xf6146 }, **                 Launch6 ? PUA Launch Application           */
/*{ 0x1008ff47, 0xf6147 }, **                 Launch7 ? PUA Launch Application           */
/*{ 0x1008ff48, 0xf6148 }, **                 Launch8 ? PUA Launch Application           */
/*{ 0x1008ff49, 0xf6149 }, **                 Launch9 ? PUA Launch Application           */
/*{ 0x1008ff4a, 0xf614a }, **                 LaunchA ? PUA Launch Application           */
/*{ 0x1008ff4b, 0xf614b }, **                 LaunchB ? PUA Launch Application           */
/*{ 0x1008ff4c, 0xf614c }, **                 LaunchC ? PUA Launch Application           */
/*{ 0x1008ff4d, 0xf614d }, **                 LaunchD ? PUA Launch Application           */
/*{ 0x1008ff4e, 0xf614e }, **                 LaunchE ? PUA Launch Application           */
/*{ 0x1008ff4f, 0xf614f }, **                 LaunchF ? PUA Launch Application           */
/*{ 0x1008ff50, 0xf6150 }, **         ApplicationLeft ? PUA switch to application, left  */
/*{ 0x1008ff51, 0xf6151 }, **        ApplicationRight ? PUA switch to application, right */
/*{ 0x1008ff52, 0xf6152 }, **                    Book ? PUA Launch bookreader            */
/*{ 0x1008ff53, 0xf6153 }, **                      CD ? PUA Launch CD/DVD player         */
/*{ 0x1008ff54, 0xf6154 }, **              Calculater ? PUA Launch Calculater            */
/*{ 0x1008ff55, 0xf6155 }, **                   Clear ? PUA Clear window, screen         */
/*{ 0x1008ff56, 0xf6156 }, **                   Close ? PUA Close window                 */
/*{ 0x1008ff57, 0xf6157 }, **                    Copy ? PUA Copy selection               */
/*{ 0x1008ff58, 0xf6158 }, **                     Cut ? PUA Cut selection                */
/*{ 0x1008ff59, 0xf6159 }, **                 Display ? PUA Output switch key            */
/*{ 0x1008ff5a, 0xf615a }, **                     DOS ? PUA Launch DOS emulation       */
/*{ 0x1008ff5b, 0xf615b }, **               Documents ? PUA Open documents window        */
/*{ 0x1008ff5c, 0xf615c }, **                   Excel ? PUA Launch spread sheet          */
/*{ 0x1008ff5d, 0xf615d }, **                Explorer ? PUA Launch file explorer         */
/*{ 0x1008ff5e, 0xf615e }, **                    Game ? PUA Launch game                  */
/*{ 0x1008ff5f, 0xf615f }, **                      Go ? PUA Go to URL                    */
/*{ 0x1008ff60, 0xf6160 }, **                  iTouch ? PUA Logitech iTouch- don't use   */
/*{ 0x1008ff61, 0xf6161 }, **                  LogOff ? PUA Log off system               */
/*{ 0x1008ff62, 0xf6162 }, **                  Market ? PUA ??                           */
/*{ 0x1008ff63, 0xf6163 }, **                 Meeting ? PUA enter meeting in calendar    */
/*{ 0x1008ff65, 0xf6165 }, **                  MenuKB ? PUA distinguish keyboard from PB  */
/*{ 0x1008ff66, 0xf6166 }, **                  MenuPB ? PUA distinguish PB from keyboard  */
/*{ 0x1008ff67, 0xf6167 }, **                 MySites ? PUA Favourites                   */
/*{ 0x1008ff68, 0xf6168 }, **                     New ? PUA New (folder, document...     */
/*{ 0x1008ff69, 0xf6169 }, **                    News ? PUA News                         */
/*{ 0x1008ff6a, 0xf616a }, **              OfficeHome ? PUA Office home old Staroffice */
/*{ 0x1008ff6b, 0xf616b }, **                    Open ? PUA Open                         */
/*{ 0x1008ff6c, 0xf616c }, **                  Option ? PUA ??  */
/*{ 0x1008ff6d, 0xf616d }, **                   Paste ? PUA Paste                        */
/*{ 0x1008ff6e, 0xf616e }, **                   Phone ? PUA Launch phone; dial number    */
/*{ 0x1008ff70, 0xf6170 }, **                       Q ? PUA Compaq's Q - don't use       */
/*{ 0x1008ff72, 0xf6172 }, **                   Reply ? PUA Reply e.g., mail             */
/*{ 0x1008ff73, 0xf6173 }, **                  Reload ? PUA Reload web page, file, etc.  */
/*{ 0x1008ff74, 0xf6174 }, **           RotateWindows ? PUA Rotate windows e.g. xrandr   */
/*{ 0x1008ff75, 0xf6175 }, **              RotationPB ? PUA don't use                    */
/*{ 0x1008ff76, 0xf6176 }, **              RotationKB ? PUA don't use                    */
/*{ 0x1008ff77, 0xf6177 }, **                    Save ? PUA Save (file, document, state  */
/*{ 0x1008ff78, 0xf6178 }, **                ScrollUp ? PUA Scroll window/contents up    */
/*{ 0x1008ff79, 0xf6179 }, **              ScrollDown ? PUA Scrool window/contentd down  */
/*{ 0x1008ff7a, 0xf617a }, **             ScrollClick ? PUA Use XKB mousekeys instead    */
/*{ 0x1008ff7b, 0xf617b }, **                    Send ? PUA Send mail, file, object      */
/*{ 0x1008ff7c, 0xf617c }, **                   Spell ? PUA Spell checker                */
/*{ 0x1008ff7d, 0xf617d }, **             SplitScreen ? PUA Split window or screen       */
/*{ 0x1008ff7e, 0xf617e }, **                 Support ? PUA Get support ??             */
/*{ 0x1008ff7f, 0xf617f }, **                TaskPane ? PUA Show tasks  */
/*{ 0x1008ff80, 0xf6180 }, **                Terminal ? PUA Launch terminal emulator     */
/*{ 0x1008ff81, 0xf6181 }, **                   Tools ? PUA toolbox of desktop/app.      */
/*{ 0x1008ff82, 0xf6182 }, **                  Travel ? PUA ??  */
/*{ 0x1008ff84, 0xf6184 }, **                  UserPB ? PUA ??  */
/*{ 0x1008ff85, 0xf6185 }, **                 User1KB ? PUA ??  */
/*{ 0x1008ff86, 0xf6186 }, **                 User2KB ? PUA ??  */
/*{ 0x1008ff87, 0xf6187 }, **                   Video ? PUA Launch video player        */
/*{ 0x1008ff88, 0xf6188 }, **             WheelButton ? PUA button from a mouse wheel  */
/*{ 0x1008ff89, 0xf6189 }, **                    Word ? PUA Launch word processor      */
/*{ 0x1008ff8a, 0xf618a }, **                    Xfer ? PUA */
/*{ 0x1008ff8b, 0xf618b }, **                  ZoomIn ? PUA zoom in view, map, etc.    */
/*{ 0x1008ff8c, 0xf618c }, **                 ZoomOut ? PUA zoom out view, map, etc.   */
/*{ 0x1008ff8d, 0xf618d }, **                    Away ? PUA mark yourself as away      */
/*{ 0x1008ff8e, 0xf618e }, **               Messenger ? PUA as in instant messaging    */
/*{ 0x1008ff8f, 0xf618f }, **                  WebCam ? PUA Launch web camera app.     */
/*{ 0x1008ff90, 0xf6190 }, **             MailForward ? PUA Forward in mail            */
/*{ 0x1008ff91, 0xf6191 }, **                Pictures ? PUA Show pictures              */
/*{ 0x1008ff92, 0xf6192 }, **                   Music ? PUA Launch music application   */
/*{ 0x1008ff93, 0xf6193 }, **                 Battery ? PUA Display battery information  */
/*{ 0x1008ff94, 0xf6194 }, **               Bluetooth ? PUA Enable/disable Bluetooth     */
/*{ 0x1008ff95, 0xf6195 }, **                    WLAN ? PUA Enable/disable WLAN          */
/*{ 0x1008ff96, 0xf6196 }, **                     UWB ? PUA Enable/disable UWB	     */
/*{ 0x1008ff97, 0xf6197 }, **            AudioForward ? PUA fast-forward audio track     */
/*{ 0x1008ff98, 0xf6198 }, **             AudioRepeat ? PUA toggle repeat mode           */
/*{ 0x1008ff99, 0xf6199 }, **         AudioRandomPlay ? PUA toggle shuffle mode          */
/*{ 0x1008ff9a, 0xf619a }, **                Subtitle ? PUA cycle through subtitle       */
/*{ 0x1008ff9b, 0xf619b }, **         AudioCycleTrack ? PUA cycle through audio tracks   */
/*{ 0x1008ff9c, 0xf619c }, **              CycleAngle ? PUA cycle through angles         */
/*{ 0x1008ff9d, 0xf619d }, **               FrameBack ? PUA video: go one frame back     */
/*{ 0x1008ff9e, 0xf619e }, **            FrameForward ? PUA video: go one frame forward  */
/*{ 0x1008ff9f, 0xf619f }, **                    Time ? PUA display, or shows an entry for time seeking  */
/*{ 0x1008ffa0, 0xf61a0 }, **                  Select ? PUA Select button on joypads and remotes  */
/*{ 0x1008ffa1, 0xf61a1 }, **                    View ? PUA Show a view options/properties  */
/*{ 0x1008ffa2, 0xf61a2 }, **                 TopMenu ? PUA Go to a top-level menu in a video  */
/*{ 0x1008ffa3, 0xf61a3 }, **                     Red ? PUA Red button                   */
/*{ 0x1008ffa4, 0xf61a4 }, **                   Green ? PUA Green button                 */
/*{ 0x1008ffa5, 0xf61a5 }, **                  Yellow ? PUA Yellow button                */
/*{ 0x1008ffa6, 0xf61a6 }, **                    Blue ? PUA Blue button                  */
/*{ 0x1008ffa7, 0xf61a7 }, **                 Suspend ? PUA Sleep to RAM                 */
/*{ 0x1008ffa8, 0xf61a8 }, **               Hibernate ? PUA Sleep to disk                */
/*{ 0x1008ffa9, 0xf61a9 }, **          TouchpadToggle ? PUA Toggle between touchpad/trackstick  */
/*{ 0x1008ffb0, 0xf61b0 }, **              TouchpadOn ? PUA The touchpad got switched on  */
/*{ 0x1008ffb1, 0xf61b1 }, **             TouchpadOff ? PUA The touchpad got switched off  */
/*{ 0x1008ffb2, 0xf61b2 }, **            AudioMicMute ? PUA Mute the Mic from the system  */
/*{ 0x1008ffb3, 0xf61b3 }, **                Keyboard ? PUA User defined keyboard related action  */
/*{ 0x1008ffb4, 0xf61b4 }, **                    WWAN ? PUA Toggle WWAN LTE, UMTS, etc. radio  */
/*{ 0x1008ffb5, 0xf61b5 }, **                  RFKill ? PUA Toggle radios on/off  */
/*{ 0x1008ffb6, 0xf61b6 }, **             AudioPreset ? PUA Select equalizer preset, e.g. theatre-mode  */
/*{ 0x1008ffb7, 0xf61b7 }, **      RotationLockToggle ? PUA Toggle screen rotation lock on/off  */
/*{ 0x1008ffb8, 0xf61b8 }, **              FullScreen ? PUA Toggle fullscreen  */
/* *INDENT-ON* */
};

long
keysym2ucs(KeySym keysym)
{
    long result = -1;		/* no matching Unicode value found */
    int min = 0;
    int max = sizeof(keysymtab) / sizeof(struct codepair) - 1;

    if ((keysym >= 0x0020 && keysym <= 0x007e) ||
	(keysym >= 0x00a0 && keysym <= 0x00ff)) {
	/* found Latin-1 characters (1:1 mapping) */
	result = (long) keysym;
    } else if ((keysym & 0xff000000) == 0x01000000) {
	/* found directly encoded 24-bit UCS characters */
	result = (long) (keysym & 0x00ffffff);
    } else if (keysym >= 0x08b0 && keysym <= 0x08b7) {
	result = (long) keysym - 0x8b1 + 0xeeee;
    } else {
	/* binary search in table */
	while (max >= min) {
	    int mid = (min + max) / 2;
	    if (keysymtab[mid].keysym < keysym) {
		min = mid + 1;
	    } else if (keysymtab[mid].keysym > keysym) {
		max = mid - 1;
	    } else {
		/* found it in table */
		result = keysymtab[mid].ucs;
		break;
	    }
	}
	if (result == -1) {
	    long value = (long) keysym;
	    if (keysym >= 0xfd00 && keysym <= 0xffff) {
		result = value - 0x1d00;
	    } else if (keysym >= 0x10000000 && keysym <= 0x100000ff) {
		result = value - 0x10000000;
	    } else if (keysym >= 0x1000f000 && keysym <= 0x1000ffff) {
		result = value - 0x1000f000 + 0xf0000;
	    } else if (keysym >= 0x1004f000 && keysym <= 0x1004ffff) {
		result = value - 0x1004f000 + 0xf1000;
	    } else if (keysym >= 0x1005f000 && keysym <= 0x1005ffff) {
		result = value - 0x1005f000 + 0xf2000;
	    } else if (keysym >= 0x10081000 && keysym <= 0x10081fff) {
		result = value - 0x10081000 + 0xf4000;
	    } else if (keysym >= 0x1008fe00 && keysym <= 0x1008ffff) {
		result = value - 0x1008fe00 + 0xf6000;
	    }
	}
    }

    return result;
}
