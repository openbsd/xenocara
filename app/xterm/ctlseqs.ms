.\"#! troff -ms $1		-*- Nroff -*-
.\" "Xterm Control Sequences" document
.\" $XTermId: ctlseqs.ms,v 1.561 2019/11/02 14:26:14 tom Exp $
.\"
.\"
.\" Copyright 1996-2018,2019 by Thomas E. Dickey
.\"
.\"                         All Rights Reserved
.\"
.\" Permission is hereby granted, free of charge, to any person obtaining a
.\" copy of this software and associated documentation files (the
.\" "Software"), to deal in the Software without restriction, including
.\" without limitation the rights to use, copy, modify, merge, publish,
.\" distribute, sublicense, and/or sell copies of the Software, and to
.\" permit persons to whom the Software is furnished to do so, subject to
.\" the following conditions:
.\"
.\" The above copyright notice and this permission notice shall be included
.\" in all copies or substantial portions of the Software.
.\"
.\" THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
.\" OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
.\" MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
.\" IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
.\" CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
.\" TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
.\" SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
.\"
.\" Except as contained in this notice, the name(s) of the above copyright
.\" holders shall not be used in advertising or otherwise to promote the
.\" sale, use or other dealings in this Software without prior written
.\" authorization.
.\"
.\"
.\" Copyright 1991, 1994 X Consortium
.\"
.\" Permission is hereby granted, free of charge, to any person obtaining
.\" a copy of this software and associated documentation files (the
.\" "Software"), to deal in the Software without restriction, including
.\" without limitation the rights to use, copy, modify, merge, publish,
.\" distribute, sublicense, and/or sell copies of the Software, and to
.\" permit persons to whom the Software is furnished to do so, subject to
.\" the following conditions:
.\"
.\" The above copyright notice and this permission notice shall be
.\" included in all copies or substantial portions of the Software.
.\"
.\" THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
.\" EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
.\" MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
.\" IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
.\" OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
.\" ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
.\" OTHER DEALINGS IN THE SOFTWARE.
.\"
.\" Except as contained in this notice, the name of the X Consortium shall
.\" not be used in advertising or otherwise to promote the sale, use or
.\" other dealings in this Software without prior written authorization
.\" from the X Consortium.
.\"
.\" X Window System is a trademark of X Consortium, Inc.
.\"
.\" Originally written by Edward Moy, University of California,
.\" Berkeley, edmoy@violet.berkeley.edu, for the X.V10R4 xterm.
.\" The X Consortium staff has since updated it for X11.
.\" Updated by Thomas E. Dickey for XFree86 3.2 - XFree86 4.3, and afterward.
.\"
.\" Run this file through troff and use the -ms macro package.
.\"
.ds XT XTerm
.ds xt xterm
.ds LF Patch #350
.ds RF 2019/11/02
.\"
.if n .pl 9999v		\" no page breaks in nroff
.ND
.\" Start a list of controls
.de St
.nr pD \\n[PD]
.nr PD 0
.nr PI 1.0i
.nr VS 16
.sp
..
.\" End a list of controls
.de Ed
.nr PD \\n[pD]
.nr VS 12
.br
..
.\" Bulleted paragraph
.de bP
.ie n  .IP \(bu 4
.el    .IP \(bu 2
..
.\" Normal leading paragraph
.de lP
.if n .sp
.LP
..
.\" Filler before ".IP" (how to pass parameters to that?)
.de iP
.br
.if n .sp
..
.\" Normal internal paragraph
.de sP
.br
.if n .sp
.if t .sp 0.5
..
.\" Section header
.de Sh
.ds RH \\$1
.iP
.SH
\\$1
..
.\" Subsection header
.de Ss
.iP
.if t .sp
.LP
.B
\\$*
.br
..
.\" Escape single quotes in literal strings from groff's Unicode transform.
.ie \n(.g .ds `` \(lq
.el       .ds `` ``
.ie \n(.g .ds '' \(rq
.el       .ds '' ''
.ds CH \" as nothing
.ds LH \*(XT Control Sequences
.nr s 6*\n(PS/10
.ds L \s\nsBEL\s0
.ds E \s\nsESC\s0
.ds T \s\nsTAB\s0
.ds X \s\nsETX\s0
.ds N \s\nsENQ\s0
.ds ET \s\nsETB\s0
.ds C \s\nsCAN\s0
.ds S \s\nsSUB\s0
.\" space between chars
.ie t .ds s \|
.el .ds s " \"
.nr [W \w'\*L'u
.nr w \w'\*E'u
.if \nw>\n([W .nr [W \nw
.nr w \w'\*T'u
.if \nw>\n([W .nr [W \nw
.nr w \w'\*X'u
.if \nw>\n([W .nr [W \nw
.nr w \w'\*N'u
.if \nw>\n([W .nr [W \nw
.nr w \w'\*(ET'u
.if \nw>\n([W .nr [W \nw
.nr w \w'\*C'u
.if \nw>\n([W .nr [W \nw
.nr w \w'\*S'u
.if \nw>\n([W .nr [W \nw
.nr [W +\w'\|\|'u
.de []
.nr w \w'\\$2'
.nr H \\n([Wu-\\nwu
.nr h \\nHu/2u
.\" do fancy box in troff
.ie t .ds \\$1 \(br\v'-1p'\(br\v'1p'\h'\\nhu'\\$2\h'\\nHu-\\nhu'\(br\l'-\\n([Wu\(ul'\v'-1p'\(br\l'-\\n([Wu\(rn'\v'1p'\*s
.el .ds \\$1 \\$2\*s
..
.[] Et \v'-1p'\*X\v'1p'
.[] En \v'-1p'\*N\v'1p'
.[] Be \v'-1p'\*L\v'1p'
.[] AP \v'-1p'\s\nsAPC\s0\v'1p'
.[] Bs \v'-1p'\s\nsBS\s0\v'1p'
.[] Cs \v'-1p'\s\nsCSI\s0\v'1p'
.[] S2 \v'-1p'\s\nsSS2\s0\v'1p'
.[] S3 \v'-1p'\s\nsSS3\s0\v'1p'
.[] SS \v'-1p'\s\nsSOS\s0\v'1p'
.[] Eg \v'-1p'\s\nsEPA\s0\v'1p'
.[] Sg \v'-1p'\s\nsSPA\s0\v'1p'
.[] Dc \v'-1p'\s\nsDCS\s0\v'1p'
.[] Ht \v'-1p'\s\nsHTS\s0\v'1p'
.[] Id \v'-1p'\s\nsIND\s0\v'1p'
.[] Nl \v'-1p'\s\nsNEL\s0\v'1p'
.[] Os \v'-1p'\s\nsOSC\s0\v'1p'
.[] RI \v'-1p'\s\nsRI\s0\v'1p'
.[] PM \v'-1p'\s\nsPM\s0\v'1p'
.[] ST \v'-1p'\s\nsST\s0\v'1p'
.[] Ta \v'-1p'\*T\v'1p'
.[] Lf \v'-1p'\s\nsLF\s0\v'1p'
.[] Vt \v'-1p'\s\nsVT\s0\v'1p'
.[] Ff \v'-1p'\s\nsFF\s0\v'1p'
.[] Np \v'-1p'\s\nsNP\s0\v'1p'
.[] Cr \v'-1p'\s\nsCR\s0\v'1p'
.[] So \v'-1p'\s\nsSO\s0\v'1p'
.[] Sp \v'-1p'\s\nsSP\s0\v'1p'
.[] Si \v'-1p'\s\nsSI\s0\v'1p'
.[] Eb \v'-1p'\*(ET\v'1p'
.[] Ca \v'-1p'\*C\v'1p'
.[] Su \v'-1p'\*S\v'1p'
.[] Es \v'-1p'\*E\v'1p'
.[] Fs \v'-1p'\s\nsFS\s0\v'1p'
.[] Gs \v'-1p'\s\nsGS\s0\v'1p'
.[] Rs \v'-1p'\s\nsRS\s0\v'1p'
.[] Us \v'-1p'\s\nsUS\s0\v'1p'
.[] XX \v'-1p'\s\nsXX\s0\v'1p'
.[] $ $
.[] # #
.[] % %
.[] (( (
.[] ) )
.[] * *
.[] + +
.[] , ,
.[] - \-
.[] . .
.[] 0 0
.[] 1 1
.[] 2 2
.[] 3 3
.[] 4 4
.[] 5 5
.[] 6 6
.[] 7 7
.[] 8 8
.[] 9 9
.[] : :
.[] ; ;
.[] = =
.[] / /
.[] < <
.[] > >
.[] ? ?
.[] @ @
.[] A A
.[] cB B
.[] C C
.[] D D
.[] E E
.[] F F
.[] G G
.[] H H
.[] I I
.[] J J
.[] K K
.[] L L
.[] M M
.[] N N
.[] O O
.[] P P
.[] Q Q
.[] R R
.[] S S
.[] T T
.[] V V
.[] W W
.[] XX X
.[] Y Y
.[] Z Z
.[] [[ [
.[] ]] ]
.[] bS \\e
.[] { {
.[] ~ \(ti
.[] Dq \(dq
.[] & &
.[] ^ \(ha
.[] _ _
.[] qu \&\(aq
.[] ` \`
.[] a a
.[] b b
.[] c c
.[] d d
.[] e e
.[] f f
.[] g g
.[] h h
.[] i i
.[] j j
.[] k k
.[] l l
.[] m m
.[] n n
.[] o o
.[] p p
.[] q q
.[] r r
.[] cs s
.[] t t
.[] u u
.[] v v
.[] w w
.[] x x
.[] y y
.[] z z
.[] | |
.[] } }
.[] ! !
.ds Cc \fIC\fP
.ds Cb \fIC\v'.3m'\h'-.2m'\s-2b\s0\v'-.3m'\fP
.ds Cx \fIC\v'.3m'\h'-.2m'\s-2x\s0\v'-.3m'\fP
.ds Cy \fIC\v'.3m'\h'-.2m'\s-2y\s0\v'-.3m'\fP
.ds Pa \fIP\v'.3m'\h'-.2m'\s-2a\s0\v'-.3m'\fP
.ds Pb \fIP\v'.3m'\h'-.2m'\s-2b\s0\v'-.3m'\fP
.ds Pc \fIP\v'.3m'\h'-.2m'\s-2c\s0\v'-.3m'\fP
.ds Pd \fIP\v'.3m'\h'-.2m'\s-2d\s0\v'-.3m'\fP
.ds Pe \fIP\v'.3m'\h'-.2m'\s-2e\s0\v'-.3m'\fP
.ds Pg \fIP\v'.3m'\h'-.2m'\s-2g\s0\v'-.3m'\fP
.ds Ph \fIP\v'.3m'\h'-.2m'\s-2h\s0\v'-.3m'\fP
.ds Pi \fIP\v'.3m'\h'-.2m'\s-2i\s0\v'-.3m'\fP
.ds Pl \fIP\v'.3m'\h'-.2m'\s-2l\s0\v'-.3m'\fP
.ds Pm \fIP\v'.3m'\h'-.2m'\s-2m\s0\v'-.3m'\fP
.ds Pn \fIP\v'.3m'\h'-.2m'\s-2n\s0\v'-.3m'\fP
.ds Pp \fIP\v'.3m'\h'-.2m'\s-2p\s0\v'-.3m'\fP
.ds Pr \fIP\v'.3m'\h'-.2m'\s-2r\s0\v'-.3m'\fP
.ds Ps \fIP\v'.3m'\h'-.2m'\s-2s\s0\v'-.3m'\fP
.ds Pt \fIP\v'.3m'\h'-.2m'\s-2t\s0\v'-.3m'\fP
.ds Pu \fIP\v'.3m'\h'-.2m'\s-2u\s0\v'-.3m'\fP
.ds Pv \fIP\v'.3m'\h'-.2m'\s-2v\s0\v'-.3m'\fP
.ds Px \fIP\v'.3m'\h'-.2m'\s-2x\s0\v'-.3m'\fP
.ds Py \fIP\v'.3m'\h'-.2m'\s-2y\s0\v'-.3m'\fP
.ds Ix \fIx\fP
.ds Iy \fIy\fP
.ds Iw \fIw\fP
.ds Ih \fIh\fP
.ds Ir \fIr\fP
.ds Ic \fIc\fP
.ie t .nr LL 6.5i
.el .nr LL 72m
.if n .na
.TL
\*(XT Control Sequences
.AU
Edward Moy
.AI
University of California, Berkeley
.sp
Revised by
.AU
Stephen Gildea
.AI
X Consortium (1994)
.AU
Thomas Dickey
.AI
XFree86 Project (1996-2006)
invisible-island.net (2006-2019)
updated for \*(XT \*(LF (\*(RF)
.AU
.
.am BT                  \" add page numbers after first page
.ds CF %
..
.Sh "Definitions"
.LP
Many controls use parameters, shown in italics.
If a control uses a single parameter, only one parameter name is listed.
Some parameters (along with separating \*; characters) may be optional.
Other characters in the control are required.
.\".iP
.IP \*(Cc
A single (required) character.
.\".iP
.IP \*(Ps
A single (usually optional) numeric parameter, composed of one or more digits.
.\".iP
.IP \*(Pm
Any number of single numeric parameters, separated by \*; character(s).
Individual values for the parameters are listed with \*(Ps .
.\".iP
.IP \*(Pt
A text parameter composed of printable characters.
.
.Sh "Control Bytes, Characters, and Sequences"
.LP
ECMA-48 (aka \*(``ISO 6429\*('') documents C1 (8-bit) and C0 (7-bit) codes.
Those are respectively codes 128 to 159 and 0 to 31.
ECMA-48 avoids referring to these codes as characters,
because that term is associated with \fIgraphic characters\fP.
Instead, it uses \*(``bytes\*('' and \*(``codes\*('',
with occasional lapses to \*(``characters\*(''
where the meaning cannot be mistaken.
.LP
Controls (including the escape code 27) are processed once:
.bP
This means that a C1 control can be mistaken for badly-formed UTF-8
when the terminal runs
in UTF-8 mode because C1 controls are valid \fIcontinuation bytes\fP of
a UTF-8 encoded (multibyte) value.
.bP
It is not possible to use a C1 control obtained from decoding the UTF-8 text,
because that would require reprocessing the data.
Consequently there is no ambiguity in the way
this document uses the term \*(``character\*('' to refer to
bytes in a control sequence.
.LP
The order of processing is a necessary consequence of the way ECMA-48
is designed:
.bP
Each byte sent to the terminal can be unambiguously determined to
fall into one of a few categories (C0, C1 and graphic characters).
.bP
ECMA-48 is \fImodal\fP; once it starts processing a control sequence,
the terminal continues until the sequence is complete,
or some byte is found which is not allowed in the sequence.
.bP
Intermediate, parameter and final bytes may
use the same codes as graphic characters,
but they are processed as part of a control sequence and are not actually
graphic characters.
.bP
Eight-bit controls can have intermediate, etc., bytes in the range 160 to 255.
Those can be treated as their counterparts in the range 32 to 127.
.bP
Single-byte controls can be handled separately from multi-byte
control sequences because ECMA-48's rules are unambiguous.
.IP
As a special case, ECMA-48 (section 9) mentions that the control functions
shift-in and shift-out are allowed to occur within a 7-bit multibyte control
sequence because those cannot alter the meaning of the control sequence.
.bP
Some controls (such as \*(Os) introduce a string mode,
which is ended on a \*(ST (string terminator).
.IP
ECMA-48 describes only correct behavior,
telling what types of characters are expected
at each stage of the control sequences.
It says that the action taken in error recovery is implementation-dependent.
\fI\*(XT\fP decodes control sequences using a state machine.
It handles errors in decoding
i.e., unexpected characters,
by resetting to the initial (ground) state.
That is different from the treatment of unimplemented
(but correctly formatted) features.
.IP
If an application does not send the string terminator, that is also an error
from the standpoint of a user.
To accommodate users of those applications, \fI\*(xt\fP has resource
settings which allow workarounds:
.RS
.bP
The Linux console's palette sequences do not use a string terminator.
The \fBbrokenLinuxOSC\fP resource setting tells \fI\*(xt\fP to ignore
those particular sequences.
.bP
The terminal should accept single-byte controls within the string.
But some applications omit a string terminator,
like the Linux console.
The \fBbrokenStringTerm\fP resource setting tells \fI\*(xt\fP to exit
string mode if it decodes a common control character such as carriage return
before the string terminator.
.RE
.
.Sh "C1 (8-Bit) Control Characters"
.LP
The \fI\*(xt\fP program recognizes both 8-bit and 7-bit control characters.
It generates 7-bit controls (by default) or 8-bit if S8C1T is enabled.
The following pairs of 7-bit and 8-bit control characters are equivalent:
.St
.IP \\*(Es\\*D
Index (\*(Id is 0x84).
.iP
.IP \\*(Es\\*E
Next Line (\*(Nl is 0x85).
.iP
.IP \\*(Es\\*H
Tab Set (\*(Ht is 0x88).
.iP
.IP \\*(Es\\*M
Reverse Index (\*(RI is 0x8d).
.iP
.IP \\*(Es\\*N
Single Shift Select of G2 Character Set (\*(S2 is 0x8e), VT220.
This affects next character only.
.iP
.IP \\*(Es\\*O
Single Shift Select of G3 Character Set (\*(S3 is 0x8f), VT220.
This affects next character only.
.iP
.IP \\*(Es\\*P
Device Control String (\*(Dc is 0x90).
.iP
.IP \\*(Es\\*V
Start of Guarded Area (\*(Sg is 0x96).
.iP
.IP \\*(Es\\*W
End of Guarded Area (\*(Eg is 0x97).
.iP
.IP \\*(Es\\*(XX
Start of String (\*(SS is 0x98).
.iP
.IP \\*(Es\\*Z
Return Terminal ID (DECID is 0x9a).
Obsolete form of \*(Cs\*c (DA).
.iP
.IP \\*(Es\\*([[
Control Sequence Introducer (\*(Cs is 0x9b).
.iP
.IP \\*(Es\\*(bS
String Terminator (\*(ST is 0x9c).
.iP
.IP \\*(Es\\*(]]
Operating System Command (\*(Os is 0x9d).
.iP
.IP \\*(Es\\*^
Privacy Message (\*(PM is 0x9e).
.iP
.IP \\*(Es\\*_
Application Program Command (\*(AP is 0x9f).
.Ed
.sp
.LP
These control characters are used in the vtXXX emulation.
.
.Sh "VT100 Mode"
.LP
In this document, \*(``VT100\*('' refers not only to VT100/VT102,
but also to the succession of upward-compatible terminals produced
by DEC (Digital Equipment Corporation)
from the mid-1970s for about twenty years.
For brevity, the document refers to the related models:
  \*(``VT200\*('' as VT220/VT240,
  \*(``VT300\*('' as VT320/VT340,
  \*(``VT400\*('' as VT420, and
  \*(``VT500\*('' as VT510/VT520/VT525.
.LP
Most of these control sequences are standard VT102 control sequences,
but there is support for later DEC VT terminals
(i.e., VT220, VT320, VT420, VT510),
as well as ECMA-48 and \fIaixterm\fP color controls.
The only VT102 feature not supported is auto-repeat,
since the only way X provides for this will affect all windows.
.LP
There are additional control sequences to provide
\fI\*(xt-\fPdependent functions, such as the scrollbar or window size.
Where the function is specified by DEC or ECMA-48, the code assigned
to it is given in parentheses.
.LP
The escape codes to designate and invoke
character sets are specified by ISO 2022 (see that document for a
discussion of character sets).
.LP
Many of the features are optional;
\fI\*(xt\fP can be configured and built without support for them.
.
.Ss Single-character functions
.St
.IP \\*(Be
Bell (\*(Be is Ctrl-G).
.
.iP
.IP \\*(Bs
Backspace (\*(Bs is Ctrl-H).
.
.iP
.IP \\*(Cr
Carriage Return (\*(Cr is Ctrl-M).
.
.iP
.IP \\*(En
Return Terminal Status (\*(En is Ctrl-E).
Default response is an empty string, but may be overridden
by a resource \fBanswerbackString\fP.
.
.iP
.IP \\*(Ff
Form Feed or New Page (\*(Np).
(\*(Ff is Ctrl-L).
\*(Ff is treated the same as \*(Lf.
.
.iP
.IP \\*(Lf
Line Feed or New Line (NL).
(\*(Lf is Ctrl-J).
.
.iP
.IP \\*(Si
Switch to \fIStandard Character Set\fP (Ctrl-O is Shift In or LS0).
This invokes the G0 character set (the default) as GL.
.br
VT200 and up implement LS0.
.
.iP
.IP \\*(So
Switch to \fIAlternate Character Set\fP (Ctrl-N is Shift Out or LS1).
This invokes the G1 character set as GL.
.br
VT200 and up implement LS1.
.
.iP
.IP \\*(Sp
Space.
.
.iP
.IP \\*(Ta
Horizontal Tab (\*(Ht is Ctrl-I).
.
.iP
.IP \\*(Vt
Vertical Tab (\*(Vt is Ctrl-K).
This is treated the same as LF.
.Ed
.
.Ss Controls beginning with \*(Es
.LP
This excludes controls where \*(Es is part of a 7-bit
equivalent to 8-bit C1 controls, ordered by the final character(s).
.St
.IP \\*(Es\\*(Sp\\*F
7-bit controls (S7C1T), VT220.
This tells the terminal to send C1 control characters as 7-bit sequences,
e.g., its responses to queries.
DEC VT200 and up always accept 8-bit control sequences except when
configured for VT100 mode.
.
.iP
.IP \\*(Es\\*(Sp\\*G
8-bit controls (S8C1T), VT220.
This tells the terminal to send C1 control characters as 8-bit sequences,
e.g., its responses to queries.
DEC VT200 and up always accept 8-bit control sequences except when
configured for VT100 mode.
.
.iP
.IP \\*(Es\\*(Sp\\*L
Set ANSI conformance level 1, ECMA-43.
.
.iP
.IP \\*(Es\\*(Sp\\*M
Set ANSI conformance level 2, ECMA-43.
.
.iP
.IP \\*(Es\\*(Sp\\*N
Set ANSI conformance level 3, ECMA-43.
.
.iP
.IP \\*(Es\\*#\\*3
DEC double-height line, top half (DECDHL), VT100.
.
.iP
.IP \\*(Es\\*#\\*4
DEC double-height line, bottom half (DECDHL), VT100.
.
.iP
.IP \\*(Es\\*#\\*5
DEC single-width line (DECSWL), VT100.
.
.iP
.IP \\*(Es\\*#\\*6
DEC double-width line (DECDWL), VT100.
.
.iP
.IP \\*(Es\\*#\\*8
DEC Screen Alignment Test (DECALN), VT100.
.
.iP
.IP \\*(Es\\*%\\*@
Select default character set.
That is ISO 8859-1 (ISO 2022).
.
.iP
.IP \\*(Es\\*%\\*G
Select UTF-8 character set, ISO 2022.
.
.iP
.IP \\*(Es\\*(((\\*(Cc
Designate G0 Character Set, VT100, ISO 2022.
.br
Final character \*(Cc for designating 94-character sets.
In this list,
.RS
.bP
\*0, \*A and \*(cB were introduced in the VT100,
.bP
most were introduced in the VT200 series,
.bP
a few were introduced in the VT300 series, and
.bP
a few more were introduced in the VT500 series.
.RE
.br
The VT220 character sets,
together with a few others (such as Portuguese) are activated by
the National Replacement Character Set (NRCS) controls.
The term \*(``replacement\*('' says that the character set is formed
by replacing some of the characters in a set
(termed the \fIMultinational Character Set\fP)
with more useful ones for a given language.
The ASCII and DEC Supplemental character sets make up the two
halves of the Multinational Character set,
initially mapped to GL and GR.
.br
The valid final characters \*(Cc for this control are:
  \*(Cc = \*A \(-> United Kingdom (UK), VT100.
  \*(Cc = \*(cB \(-> United States (USASCII), VT100.
  \*(Cc = \*C or \*5 \(-> Finnish, VT200.
  \*(Cc = \*H or \*7 \(-> Swedish, VT200.
  \*(Cc = \*K \(-> German, VT200.
  \*(Cc = \*Q or \*9 \(-> French Canadian, VT200.
  \*(Cc = \*R or \*f \(-> French, VT200.
  \*(Cc = \*Y \(-> Italian, VT200.
  \*(Cc = \*Z \(-> Spanish, VT200.
  \*(Cc = \*4 \(-> Dutch, VT200.
  \*(Cc = \*(Dq\*> \(-> Greek, VT500.
  \*(Cc = \*%\*2 \(-> Turkish, VT500.
  \*(Cc = \*%\*6 \(-> Portuguese, VT300.
  \*(Cc = \*%\*= \(-> Hebrew, VT500.
  \*(Cc = \*= \(-> Swiss, VT200.
  \*(Cc = \*`, \*E or \*6 \(-> Norwegian/Danish, VT200.
.br
The final character \*A is a special case,
since the same final character is
used by the VT300-control for the 96-character British Latin-1.
.br
There are a few other 94-character sets:
  \*(Cc = \*0 \(-> DEC Special Character and Line Drawing Set, VT100.
  \*(Cc = \*< \(-> DEC Supplemental, VT200.
  \*(Cc = \*> \(-> DEC Technical, VT300.
.br
These are documented as NRCS:
  \*(Cc = \*(Dq\*4 \(-> DEC Hebrew, VT500.
  \*(Cc = \*(Dq\*? \(-> DEC Greek, VT500.
  \*(Cc = \*%\*0 \(-> DEC Turkish, VT500.
  \*(Cc = \*%\*5 \(-> DEC Supplemental Graphics, VT300.
  \*(Cc = \*&\*4 \(-> DEC Cyrillic, VT500.
.br
The VT520 reference manual lists a few more,
but no documentation has been found for the mappings:
  \*(Cc = \*%\*3 \(-> SCS NRCS, VT500.
  \*(Cc = \*&\*5 \(-> DEC Russian, VT500.
.iP
.IP \\*(Es\\*)\\*(Cc
Designate G1 Character Set, ISO 2022, VT100.
.br
The same character sets apply as for \*(Es\*(((\*(Cc.
.
.iP
.IP \\*(Es\\**\\*(Cc
Designate G2 Character Set, ISO 2022, VT220.
.br
The same character sets apply as for \*(Es\*(((\*(Cc.
.
.iP
.IP \\*(Es\\*+\\*(Cc
Designate G3 Character Set, ISO 2022, VT220.
.br
The same character sets apply as for \*(Es\*(((\*(Cc.
.
.iP
.IP \\*(Es\\*-\\*(Cc
Designate G1 Character Set, VT300.
.br
These controls apply only to 96-character sets.
Unlike the 94-character sets, these can have different values than
ASCII space and DEL for the mapping of 0x20 and 0x7f.
The valid final characters \*(Cc for this control are:
  \*(Cc = \*A \(-> ISO Latin-1 Supplemental, VT300.
  \*(Cc = \*F \(-> ISO Greek Supplemental, VT500.
  \*(Cc = \*H \(-> ISO Hebrew Supplemental, VT500.
  \*(Cc = \*L \(-> ISO Latin-Cyrillic, VT500.
  \*(Cc = \*M \(-> ISO Latin-5 Supplemental, VT500.
.
.iP
.IP \\*(Es\\*.\\*(Cc
Designate G2 Character Set, VT300.
.br
The same character sets apply as for \*(Es\*-\*(Cc.
.
.iP
.IP \\*(Es\\*/\\*(Cc
Designate G3 Character Set, VT300.
.br
The same character sets apply as for \*(Es\*-\*(Cc.
.
.iP
.IP \\*(Es\\*6
Back Index (DECBI), VT420 and up.
.
.iP
.IP \\*(Es\\*7
Save Cursor (DECSC), VT100.
.
.iP
.IP \\*(Es\\*8
Restore Cursor (DECRC), VT100.
.
.iP
.IP \\*(Es\\*9
Forward Index (DECFI), VT420 and up.
.
.iP
.IP \\*(Es\\*=
Application Keypad (DECKPAM).
.
.iP
.IP \\*(Es\\*>
Normal Keypad (DECKPNM), VT100.
.
.iP
.IP \\*(Es\\*F
Cursor to lower left corner of screen.
This is enabled by the \fBhpLowerleftBugCompat\fP resource.
.
.iP
.IP \\*(Es\\*c
Full Reset (RIS), VT100.
.
.iP
.IP \\*(Es\\*l
Memory Lock (per HP terminals).
Locks memory above the cursor.
.
.iP
.IP \\*(Es\\*m
Memory Unlock (per HP terminals).
.
.iP
.IP \\*(Es\\*n
Invoke the G2 Character Set as GL (LS2) as GL.
.
.iP
.IP \\*(Es\\*o
Invoke the G3 Character Set as GL (LS3) as GL.
.
.iP
.IP \\*(Es\\*|
Invoke the G3 Character Set as GR (LS3R).
.
.iP
.IP \\*(Es\\*}
Invoke the G2 Character Set as GR (LS2R).
.
.iP
.IP \\*(Es\\*~
Invoke the G1 Character Set as GR (LS1R), VT100.
.Ed
.
.Ss Application Program-Command functions
.St
.IP \\*(AP\\*(Pt\\*s\\*(ST
None.
\fI\*(xt\fP implements no \*(AP functions; \*(Pt is ignored.
\*(Pt need not be printable characters.
.Ed
.\"
.Ss Device-Control functions
.St
.IP \\*(Dc\\*(Ps\\*s\\*;\\*(Ps\\*s\\*|\\*(Pt\\*s\\*(ST
User-Defined Keys (DECUDK), VT220 and up.
.iP
The first parameter:
  \*(Ps = \*0 \(-> Clear all UDK definitions before starting (default).
  \*(Ps = \*1 \(-> Erase Below (default).
.iP
The second parameter:
  \*(Ps = \*0 \(<- Lock the keys (default).
  \*(Ps = \*1 \(<- Do not lock.
.iP
The third parameter is a ';'-separated list of strings denoting
the key-code separated by a '/' from the hex-encoded key value.
The key codes correspond to the DEC function-key codes (e.g., F6=17).
.
.iP
.IP \\*(Dc\\*$\\*q\\*(Pt\\*s\\*(ST
Request Status String (DECRQSS), VT420 and up.
.br
The string following the \*(``q\*('' is one of the following:
  \*m		\(-> SGR
  \*(Dq\*p	\(-> DECSCL
  \*(Sp\*q	\(-> DECSCUSR
  \*(Dq\*q	\(-> DECSCA
  \*r		\(-> DECSTBM
  \*(cs		\(-> DECSLRM
  \*t		\(-> DECSLPP
  \*$\*|	\(-> DECSCPP
  \**\*|	\(-> DECSNLS
.br
\fI\*(xt\fP responds with
\*(Dc\*1\*$\*r\*(Pt\*s\*(ST
for valid requests, replacing the \*(Pt with the corresponding \*(Cs
string,
or
\*(Dc\*0\*$\*r\*(Pt\*s\*(ST
for invalid requests.
.iP
.IP \\*(Dc\\*(Ps\\*s\\*$\\*t\\*(Pt\\*s\\*(ST
Restore presentation status (DECRSPS), VT320 and up.
The control can be converted from a response from DECCIR or DECTABSR
by changing the first \*(``u\*('' to a \*(``t\*(''
  \*(Ps = \*1 \(-> DECCIR
  \*(Ps = \*2 \(-> DECTABSR
.
.iP
.IP \\*(Dc\\*+\\*Q\\*(Pt\\*s\\*(ST
Request resource values (XTGETXRES), \fI\*(xt\fP.
The string following the \*(``Q\*('' is a list of names
encoded in hexadecimal (2 digits per character)
separated by \*;
which correspond to \fI\*(xt\fP resource names.
Only boolean, numeric and string resources are supported by this query.
.sP
\fI\*(xt\fP responds with
.br
\*(Dc\*1\*+\*R\*(Pt\*s\*(ST
for valid requests, adding to \*(Pt an \*=,
and the value of the corresponding resource that \fI\*(xt\fP is using,
or
.br
\*(Dc\*0\*+\*R\*(Pt\*s\*(ST
for invalid requests.
.br
The strings are encoded in hexadecimal (2 digits per character).
.Ed
.
.iP
.IP \\*(Dc\\*+\\*p\\*(Pt\\*s\\*(ST
Set Termcap/Terminfo Data (XTSETTCAP), \fI\*(xt\fP.
The string following the \*(``p\*('' is a name to use for retrieving data from
the terminal database.
The data will be used for the \*(``tcap\*('' keyboard
configuration's function- and special-keys, as well as by the
Request Termcap/Terminfo String control.
.
.iP
.IP \\*(Dc\\*+\\*q\\*(Pt\\*s\\*(ST
Request Termcap/Terminfo String (XTGETTCAP), \fI\*(xt\fP.
The string following the \*(``q\*('' is a list of names
encoded in hexadecimal (2 digits per character)
separated by \*;
which correspond to termcap or terminfo key names.
.br
A few special features are also recognized, which are not key names:
.RS
.bP
\fICo\fP for termcap colors (or \fIcolors\fP for terminfo colors),
and
.bP
\fITN\fP for termcap name (or \fIname\fP for terminfo name).
.bP
\fIRGB\fP for the ncurses direct-color extension.
.br
Only a terminfo name is provided,
since termcap applications cannot use this information.
.RE
.sP
\fI\*(xt\fP responds with
.br
\*(Dc\*1\*+\*r\*(Pt\*s\*(ST
for valid requests, adding to \*(Pt an \*=,
and the value of the corresponding string that \fI\*(xt\fP would send,
or
.br
\*(Dc\*0\*+\*r\*(Pt\*s\*(ST
for invalid requests.
.br
The strings are encoded in hexadecimal (2 digits per character).
.Ed
.\"
.Ss Functions using \*(Cs, ordered by the final character(s)
.St
.IP \\*(Cs\\*(Ps\\*s\\*@
Insert \*(Ps (Blank) Character(s) (default = 1) (ICH).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*(Sp\\*@
Shift left \*(Ps columns(s) (default = 1) (SL), ECMA-48.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*A
Cursor Up \*(Ps Times (default = 1) (CUU).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*(Sp\\*A
Shift right \*(Ps columns(s) (default = 1) (SR), ECMA-48.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*(cB
Cursor Down \*(Ps Times (default = 1) (CUD).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*C
Cursor Forward \*(Ps Times (default = 1) (CUF).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*D
Cursor Backward \*(Ps Times (default = 1) (CUB).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*E
Cursor Next Line \*(Ps Times (default = 1) (CNL).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*F
Cursor Preceding Line \*(Ps Times (default = 1) (CPL).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*G
Cursor Character Absolute  [column] (default = [row,1]) (CHA).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*;\\*(Ps\\*s\\*H
Cursor Position [row;column] (default = [1,1]) (CUP).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*I
Cursor Forward Tabulation \*(Ps tab stops (default = 1) (CHT).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*J
Erase in Display (ED), VT100.
  \*(Ps = \*0 \(-> Erase Below (default).
  \*(Ps = \*1 \(-> Erase Above.
  \*(Ps = \*2 \(-> Erase All.
  \*(Ps = \*3 \(-> Erase Saved Lines, \fI\*(xt\fP.
.
.iP
.IP \\*(Cs\\*?\\*(Ps\\*s\\*J
Erase in Display (DECSED), VT220.
  \*(Ps = \*0 \(-> Selective Erase Below (default).
  \*(Ps = \*1 \(-> Selective Erase Above.
  \*(Ps = \*2 \(-> Selective Erase All.
  \*(Ps = \*3 \(-> Selective Erase Saved Lines, \fI\*(xt\fP.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*K
Erase in Line (EL), VT100.
  \*(Ps = \*0 \(-> Erase to Right (default).
  \*(Ps = \*1 \(-> Erase to Left.
  \*(Ps = \*2 \(-> Erase All.
.
.iP
.IP \\*(Cs\\*?\\*(Ps\\*s\\*K
Erase in Line (DECSEL), VT220.
  \*(Ps = \*0 \(-> Selective Erase to Right (default).
  \*(Ps = \*1 \(-> Selective Erase to Left.
  \*(Ps = \*2 \(-> Selective Erase All.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*L
Insert \*(Ps Line(s) (default = 1) (IL).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*M
Delete \*(Ps Line(s) (default = 1) (DL).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*P
Delete \*(Ps Character(s) (default = 1) (DCH).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*S
Scroll up \*(Ps lines (default = 1) (SU), VT420, ECMA-48.
.
.iP
.IP \\*(Cs\\*?\\*(Pi\\*s\\*;\\*(Pa\\*s\\*;\\*(Pv\\*s\\*S
Set or request graphics attribute, \fI\*(xt\fP.
If configured to support either \fBSixel Graphics\fP or \fBReGIS Graphics\fP,
\fI\*(xt\fP accepts a three-parameter control sequence, where \*(Pi,
\*(Pa and \*(Pv are the \fIitem\fP, \fIaction\fP and \fIvalue\fP:
.sP
  \*(Pi = \*1 \(-> item is number of color registers.
  \*(Pi = \*2 \(-> item is Sixel graphics geometry (in pixels).
  \*(Pi = \*3 \(-> item is ReGIS graphics geometry (in pixels).
.sP
  \*(Pa = \*1 \(-> read attribute.
  \*(Pa = \*2 \(-> reset to default.
  \*(Pa = \*3 \(-> set to value in \*(Pv.
  \*(Pa = \*4 \(-> read the maximum allowed value.
.sP
  \*(Pv can be omitted except when setting (\*(Pa == \*3).
  \*(Pv = \fIn\fP \(<- A single integer is used for color registers.
  \*(Pv = \fIwidth\fP\*s\*;\fIheight\fP \(<- Two integers for graphics geometry.
.sP
\fI\*(xt\fP replies with a control sequence of the same form:
.ID
\*(Cs\*?\*(Pi\*s\*;\*(Ps\*s\*;\*(Pv\*s\*S
.DE
where \*(Ps is the status:
  \*(Ps = \*0 \(<- success.
  \*(Ps = \*1 \(<- error in \*(Pi.
  \*(Ps = \*2 \(<- error in \*(Pa.
  \*(Ps = \*3 \(<- failure.
.sP
On success, \*(Pv represents the value read or set.
.sP
\fBNotes\fP:
.RS
.bP
The current implementation allows reading the graphics sizes,
but disallows modifying those sizes because that is done once,
using resource-values.
.bP
Graphics geometry is not necessarily the same as \*(``window size\*(''
(see the dtterm window manipulation extensions).
For example, \fI\*(xt\fP limits the maximum graphics geometry at compile time
(1000x1000 as of version 328)
although the window size can be larger.
.bP
While resizing a window will always change the current
graphics geometry, the reverse is not true.
Setting graphics geometry does not affect the window size.
.RE
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*T
Scroll down \*(Ps lines (default = 1) (SD), VT420.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*;\\*(Ps\\*s\\*;\\*(Ps\\*s\\*;\\*(Ps\\*s\\*;\\*(Ps\\*s\\*T
Initiate highlight mouse tracking.
Parameters are [func;startx;starty;firstrow;lastrow].
See the section \fBMouse Tracking\fP.
.
.iP
.IP \\*(Cs\\*>\\*(Pm\\*s\\*T
Reset title mode features to default value, \fI\*(xt\fP.
Normally, \*(``reset\*('' disables the feature.
It is possible to disable the ability to reset features
by compiling a different default for the title modes into \fI\*(xt\fP.
.iP
  \*(Ps = \*0 \(-> Do not set window/icon labels using hexadecimal.
  \*(Ps = \*1 \(-> Do not query window/icon labels using hexadecimal.
  \*(Ps = \*2 \(-> Do not set window/icon labels using UTF-8.
  \*(Ps = \*3 \(-> Do not query window/icon labels using UTF-8.
.iP
(See discussion of \fBTitle Modes\fP).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*(XX
Erase \*(Ps Character(s) (default = 1) (ECH).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*Z
Cursor Backward Tabulation \*(Ps tab stops (default = 1) (CBT).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*^
Scroll down \*(Ps lines (default = 1) (SD), ECMA-48.
.br
This was a publication error in the original ECMA-48 5th edition (1991)
corrected in 2003.
.
.iP
.IP \\*(Cs\\*(Pm\\*s\\*`
Character Position Absolute  [column] (default = [row,1]) (HPA).
.
.iP
.IP \\*(Cs\\*(Pm\\*s\\*a
Character Position Relative  [columns] (default = [row,col+1]) (HPR).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*b
Repeat the preceding graphic character \*(Ps times (REP).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*c
Send Device Attributes (Primary DA).
  \*(Ps = \*0 or omitted \(-> request attributes from terminal.
The response depends on the \fBdecTerminalID\fP resource setting.
  \(->\ \*(Cs\*?\*1\*;\*2\*c (\*(``VT100 with Advanced Video Option\*('')
  \(->\ \*(Cs\*?\*1\*;\*0\*c (\*(``VT101 with No Options\*('')
  \(->\ \*(Cs\*?\*6\*c (\*(``VT102\*('')
  \(->\ \*(Cs\*?\*6\*2\*;\*(Ps\*c (\*(``VT220\*('')
  \(->\ \*(Cs\*?\*6\*3\*;\*(Ps\*c (\*(``VT320\*('')
  \(->\ \*(Cs\*?\*6\*4\*;\*(Ps\*c (\*(``VT420\*('')
.iP
The VT100-style response parameters do not mean anything by themselves.
VT220 (and higher) parameters do,
telling the host what features the terminal supports:
  \*(Ps = \*1 \(-> 132-columns.
  \*(Ps = \*2 \(-> Printer.
  \*(Ps = \*3 \(-> ReGIS graphics.
  \*(Ps = \*4 \(-> Sixel graphics.
  \*(Ps = \*6 \(-> Selective erase.
  \*(Ps = \*8 \(-> User-defined keys.
  \*(Ps = \*9 \(-> National Replacement Character sets.
  \*(Ps = \*1\*5 \(-> Technical characters.
  \*(Ps = \*1\*6 \(-> Locator port.
  \*(Ps = \*1\*7 \(-> Terminal state interrogation.
  \*(Ps = \*1\*8 \(-> User windows.
  \*(Ps = \*2\*1 \(-> Horizontal scrolling.
  \*(Ps = \*2\*2 \(-> ANSI color, e.g., VT525.
  \*(Ps = \*2\*8 \(-> Rectangular editing.
  \*(Ps = \*2\*9 \(-> ANSI text locator (i.e., DEC Locator mode).
.iP
\fI\*(XT\fP supports part of the \fIUser windows\fP feature,
providing a single page (which corresponds to its visible window).
Rather than resizing the font to change the number of lines/columns in
a fixed-size display, \fI\*(xt\fP uses the window extension controls
(DECSNLS, DECSCPP, DECSLPP) to adjust its visible window's size.
The \*(``cursor coupling\*('' controls (DECHCCM, DECPCCM, DECVCCM) are ignored.
.
.iP
.IP \\*(Cs\\*=\\*(Ps\\*s\\*c
Send Device Attributes (Tertiary DA).
  \*(Ps = \*0 \(-> report Terminal Unit ID (default), VT400.
\*(XT uses zeros for the site code and serial number in its DECRPTUI response.
.
.iP
.IP \\*(Cs\\*>\\*(Ps\\*s\\*c
Send Device Attributes (Secondary DA).
  \*(Ps = \*0 or omitted \(-> request the terminal's identification code.
The response depends on the \fBdecTerminalID\fP resource setting.
It should apply only to VT220 and up, but \fI\*(xt\fP extends this to VT100.
  \(-> \*(Cs\*s\*>\*(Pp\*s\*;\*(Pv\*s\*;\*(Pc\*s\*c
.br
where \*(Pp denotes the terminal type
  \*(Pp = \*0 \(-> \*(``VT100\*(''.
  \*(Pp = \*1 \(-> \*(``VT220\*(''.
  \*(Pp = \*2 \(-> \*(``VT240\*(''.
  \*(Pp = \*1\*8 \(-> \*(``VT330\*(''.
  \*(Pp = \*1\*9 \(-> \*(``VT340\*(''.
  \*(Pp = \*2\*4 \(-> \*(``VT320\*(''.
  \*(Pp = \*4\*1 \(-> \*(``VT420\*(''.
  \*(Pp = \*6\*1 \(-> \*(``VT510\*(''.
  \*(Pp = \*6\*4 \(-> \*(``VT520\*(''.
  \*(Pp = \*6\*5 \(-> \*(``VT525\*(''.
.iP
and \*(Pv is the firmware version (for \fI\*(xt\fP, this was originally
the XFree86 patch number, starting with 95).
In a DEC terminal, \*(Pc indicates the ROM cartridge
registration number and is always zero.
.
.iP
.IP \\*(Cs\\*(Pm\\*s\\*d
Line Position Absolute  [row] (default = [1,column]) (VPA).
.
.iP
.IP \\*(Cs\\*(Pm\\*s\\*e
Line Position Relative  [rows] (default = [row+1,column]) (VPR).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*;\\*(Ps\\*s\\*f
Horizontal and Vertical Position [row;column] (default = [1,1]) (HVP).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*g
Tab Clear (TBC).
  \*(Ps = \*0 \(-> Clear Current Column (default).
  \*(Ps = \*3 \(-> Clear All.
.
.iP
.IP \\*(Cs\\*(Pm\\*s\\*h
Set Mode (SM).
  \*(Ps = \*2 \(-> Keyboard Action Mode (AM).
  \*(Ps = \*4 \(-> Insert Mode (IRM).
  \*(Ps = \*1\*2 \(-> Send/receive (SRM).
  \*(Ps = \*2\*0 \(-> Automatic Newline (LNM).
.
.iP
.IP \\*(Cs\\*?\\*(Pm\\*s\\*h
DEC Private Mode Set (DECSET).
  \*(Ps = \*1 \(-> Application Cursor Keys (DECCKM), VT100.
  \*(Ps = \*2 \(-> Designate USASCII for character sets G0-G3 (DECANM), VT100,
and set VT100 mode.
  \*(Ps = \*3 \(-> 132 Column Mode (DECCOLM), VT100.
  \*(Ps = \*4 \(-> Smooth (Slow) Scroll (DECSCLM), VT100.
  \*(Ps = \*5 \(-> Reverse Video (DECSCNM), VT100.
  \*(Ps = \*6 \(-> Origin Mode (DECOM), VT100.
  \*(Ps = \*7 \(-> Auto-wrap Mode (DECAWM), VT100.
  \*(Ps = \*8 \(-> Auto-repeat Keys (DECARM), VT100.
  \*(Ps = \*9 \(-> Send Mouse X & Y on button press.
See the section \fBMouse Tracking\fP.
This is the X10 \fI\*(xt\fP mouse protocol.
  \*(Ps = \*1\*0 \(-> Show toolbar (rxvt).
  \*(Ps = \*1\*2 \(-> Start Blinking Cursor (AT&T 610).
  \*(Ps = \*1\*3 \(-> Start Blinking Cursor (set only via resource or menu).
  \*(Ps = \*1\*4 \(-> Enable XOR of Blinking Cursor control sequence and menu.
  \*(Ps = \*1\*8 \(-> Print form feed (DECPFF), VT220.
  \*(Ps = \*1\*9 \(-> Set print extent to full screen (DECPEX), VT220.
  \*(Ps = \*2\*5 \(-> Show Cursor (DECTCEM), VT220.
  \*(Ps = \*3\*0 \(-> Show scrollbar (rxvt).
  \*(Ps = \*3\*5 \(-> Enable font-shifting functions (rxvt).
  \*(Ps = \*3\*8 \(-> Enter Tektronix Mode (DECTEK), VT240, \fI\*(xt\fP.
  \*(Ps = \*4\*0 \(-> Allow 80 \z\(<-\(-> 132 Mode, \fI\*(xt\fP.
  \*(Ps = \*4\*1 \(-> \fBmore\fP(1) fix (see \fBcurses\fP resource).
  \*(Ps = \*4\*2 \(-> Enable National Replacement Character sets
(DECNRCM), VT220.
  \*(Ps = \*4\*4 \(-> Turn On Margin Bell, \fI\*(xt\fP.
  \*(Ps = \*4\*5 \(-> Reverse-wraparound Mode, \fI\*(xt\fP.
  \*(Ps = \*4\*6 \(-> Start Logging, \fI\*(xt\fP.
This is normally disabled by a compile-time option.
  \*(Ps = \*4\*7 \(-> Use \fIAlternate Screen Buffer\fP, \fI\*(xt\fP.
This may be disabled by the \fBtiteInhibit\fP resource.
  \*(Ps = \*6\*6 \(-> Application keypad (DECNKM), VT320.
  \*(Ps = \*6\*7 \(-> Backarrow key sends backspace (DECBKM), VT340, VT420.  This sets the \fBbackarrowKey\fP resource to \*(``true\*(''.
  \*(Ps = \*6\*9 \(-> Enable left and right margin mode (DECLRMM), VT420 and up.
  \*(Ps = \*8\*0 \(-> Enable \fISixel Scrolling\fP (DECSDM).
  \*(Ps = \*9\*5 \(-> Do not clear screen when DECCOLM is set/reset
(DECNCSM), VT510 and up.
  \*(Ps = \*1\*0\*0\*0 \(-> Send Mouse X & Y on button press and release.
See the section \fBMouse Tracking\fP.
This is the X11 \fI\*(xt\fP mouse protocol.
  \*(Ps = \*1\*0\*0\*1 \(-> Use Hilite Mouse Tracking, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*0\*2 \(-> Use Cell Motion Mouse Tracking, \fI\*(xt\fP.  See the section \fBButton-event tracking\fP.
  \*(Ps = \*1\*0\*0\*3 \(-> Use All Motion Mouse Tracking, \fI\*(xt\fP.  See the section \fBAny-event tracking\fP.
  \*(Ps = \*1\*0\*0\*4 \(-> Send \fBFocusIn/FocusOut\fP events, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*0\*5 \(-> Enable UTF-8 Mouse Mode, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*0\*6 \(-> Enable SGR Mouse Mode, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*0\*7 \(-> Enable \fIAlternate Scroll Mode\fP, \fI\*(xt\fP.
This corresponds to the \fBalternateScroll\fP resource.
  \*(Ps = \*1\*0\*1\*0 \(-> Scroll to bottom on tty output (rxvt).  This sets the \fBscrollTtyOutput\fP resource to \*(``true\*(''.
  \*(Ps = \*1\*0\*1\*1 \(-> Scroll to bottom on key press (rxvt).  This sets the \fBscrollKey\fP resource to \*(``true\*(''.
  \*(Ps = \*1\*0\*1\*5 \(-> Enable urxvt Mouse Mode.
  \*(Ps = \*1\*0\*3\*4 \(-> Interpret \*(``meta\*('' key, \fI\*(xt\fP.
This sets the eighth bit of keyboard input
(and enables the \fBeightBitInput\fP resource).
  \*(Ps = \*1\*0\*3\*5 \(-> Enable special modifiers for Alt and NumLock keys,
\fI\*(xt\fP.
This enables the \fBnumLock\fP resource.
  \*(Ps = \*1\*0\*3\*6 \(-> Send \*(Es  when Meta modifies a key, \fI\*(xt\fP.
This enables the \fBmetaSendsEscape\fP resource.
  \*(Ps = \*1\*0\*3\*7 \(-> Send DEL from the editing-keypad Delete key,
\fI\*(xt\fP.
  \*(Ps = \*1\*0\*3\*9 \(-> Send \*(Es when Alt modifies a key, \fI\*(xt\fP.
This enables the \fBaltSendsEscape\fP resource, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*4\*0 \(-> Keep selection even if not highlighted, \fI\*(xt\fP.
This enables the \fBkeepSelection\fP resource.
  \*(Ps = \*1\*0\*4\*1 \(-> Use the CLIPBOARD selection, \fI\*(xt\fP.
This enables the \fBselectToClipboard\fP resource.
  \*(Ps = \*1\*0\*4\*2 \(-> Enable Urgency window manager hint
when Control-G is received, \fI\*(xt\fP.
This enables the \fBbellIsUrgent\fP resource.
  \*(Ps = \*1\*0\*4\*3 \(-> Enable raising of the window
when Control-G is received, \fI\*(xt\fP.
This enables the \fBpopOnBell\fP resource.
  \*(Ps = \*1\*0\*4\*4 \(-> Reuse the most recent data copied to CLIPBOARD,
\fI\*(xt\fP.
This enables the \fBkeepClipboard\fP resource.
  \*(Ps = \*1\*0\*4\*6 \(-> Enable switching to/from
\fIAlternate Screen Buffer\fP, \fI\*(xt\fP.
This works for terminfo-based systems, updating the \fBtiteInhibit\fP resource.
  \*(Ps = \*1\*0\*4\*7 \(-> Use \fIAlternate Screen Buffer\fP, \fI\*(xt\fP.
This may be disabled by the \fBtiteInhibit\fP resource.
  \*(Ps = \*1\*0\*4\*8 \(-> Save cursor as in DECSC, \fI\*(xt\fP.
This may be disabled by the \fBtiteInhibit\fP resource.
  \*(Ps = \*1\*0\*4\*9 \(-> Save cursor as in DECSC, \fI\*(xt\fP.
After saving the cursor, switch to
the \fIAlternate Screen Buffer\fP, clearing it first.
This may be
disabled by the \fBtiteInhibit\fP resource.
This control combines the effects of the \*1\*0\*4\*7 and \*1\*0\*4\*8 modes.
Use this with terminfo-based applications rather than the \*4\*7 mode.
  \*(Ps = \*1\*0\*5\*0 \(-> Set terminfo/termcap function-key mode, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*5\*1 \(-> Set Sun function-key mode, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*5\*2 \(-> Set HP function-key mode, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*5\*3 \(-> Set SCO function-key mode, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*6\*0 \(-> Set legacy keyboard emulation, i.e, X11R6,
\fI\*(xt\fP.
  \*(Ps = \*1\*0\*6\*1 \(-> Set VT220 keyboard emulation, \fI\*(xt\fP.
  \*(Ps = \*2\*0\*0\*4 \(-> Set bracketed paste mode, \fI\*(xt\fP.
.
.iP
.IP \\*(Cs\\*(Pm\\*s\\*i
Media Copy (MC).
  \*(Ps = \*0 \(-> Print screen (default).
  \*(Ps = \*4 \(-> Turn off printer controller mode.
  \*(Ps = \*5 \(-> Turn on printer controller mode.
  \*(Ps = \*1\*0 \(-> HTML screen dump, \fI\*(xt\fP.
  \*(Ps = \*1\*1 \(-> SVG screen dump, \fI\*(xt\fP.
.
.iP
.IP \\*(Cs\\*?\\*(Pm\\*s\\*i
Media Copy (MC), DEC-specific.
  \*(Ps = \*1 \(-> Print line containing cursor.
  \*(Ps = \*4 \(-> Turn off autoprint mode.
  \*(Ps = \*5 \(-> Turn on autoprint mode.
  \*(Ps = \*1\*0 \(-> Print composed display, ignores DECPEX.
  \*(Ps = \*1\*1 \(-> Print all pages.
.
.iP
.IP \\*(Cs\\*(Pm\\*s\\*l
Reset Mode (RM).
  \*(Ps = \*2 \(-> Keyboard Action Mode (AM).
  \*(Ps = \*4 \(-> Replace Mode (IRM).
  \*(Ps = \*1\*2 \(-> Send/receive (SRM).
  \*(Ps = \*2\*0 \(-> Normal Linefeed (LNM).
.
.iP
.IP \\*(Cs\\*?\\*(Pm\\*s\\*l
DEC Private Mode Reset (DECRST).
  \*(Ps = \*1 \(-> Normal Cursor Keys (DECCKM), VT100.
  \*(Ps = \*2 \(-> Designate VT52 mode (DECANM), VT100.
  \*(Ps = \*3 \(-> 80 Column Mode (DECCOLM), VT100.
  \*(Ps = \*4 \(-> Jump (Fast) Scroll (DECSCLM), VT100.
  \*(Ps = \*5 \(-> Normal Video (DECSCNM), VT100.
  \*(Ps = \*6 \(-> Normal Cursor Mode (DECOM), VT100.
  \*(Ps = \*7 \(-> No Auto-wrap Mode (DECAWM), VT100.
  \*(Ps = \*8 \(-> No Auto-repeat Keys (DECARM), VT100.
  \*(Ps = \*9 \(-> Don't send Mouse X & Y on button press, \fI\*(xt\fP.
  \*(Ps = \*1\*0 \(-> Hide toolbar (rxvt).
  \*(Ps = \*1\*2 \(-> Stop Blinking Cursor (AT&T 610).
  \*(Ps = \*1\*3 \(-> Disable Blinking Cursor (reset only via resource or menu).
  \*(Ps = \*1\*4 \(-> Disable XOR of Blinking Cursor control sequence and menu.
  \*(Ps = \*1\*8 \(-> Don't print form feed (DECPFF).
  \*(Ps = \*1\*9 \(-> Limit print to scrolling region (DECPEX).
  \*(Ps = \*2\*5 \(-> Hide Cursor (DECTCEM), VT220.
  \*(Ps = \*3\*0 \(-> Don't show scrollbar (rxvt).
  \*(Ps = \*3\*5 \(-> Disable font-shifting functions (rxvt).
  \*(Ps = \*4\*0 \(-> Disallow 80 \z\(<-\(-> 132 Mode, \fI\*(xt\fP.
  \*(Ps = \*4\*1 \(-> No \fBmore\fP(1) fix (see \fBcurses\fP resource).
  \*(Ps = \*4\*2 \(-> Disable National Replacement Character sets (DECNRCM),
VT220.
  \*(Ps = \*4\*4 \(-> Turn Off Margin Bell, \fI\*(xt\fP.
  \*(Ps = \*4\*5 \(-> No Reverse-wraparound Mode, \fI\*(xt\fP.
  \*(Ps = \*4\*6 \(-> Stop Logging, \fI\*(xt\fP.
This is normally disabled by a compile-time option.
  \*(Ps = \*4\*7 \(-> Use Normal Screen Buffer, \fI\*(xt\fP.
  \*(Ps = \*6\*6 \(-> Numeric keypad (DECNKM), VT320.
  \*(Ps = \*6\*7 \(-> Backarrow key sends delete (DECBKM), VT340, VT420.  This sets the \fBbackarrowKey\fP resource to \*(``false\*(''.
  \*(Ps = \*6\*9 \(-> Disable left and right margin mode (DECLRMM),
VT420 and up.
  \*(Ps = \*8\*0 \(-> Disable \fISixel Scrolling\fP (DECSDM).
  \*(Ps = \*9\*5 \(-> Clear screen when DECCOLM is set/reset (DECNCSM),
VT510 and up.
  \*(Ps = \*1\*0\*0\*0 \(-> Don't send Mouse X & Y on button press and
release.
See the section \fBMouse Tracking\fP.
  \*(Ps = \*1\*0\*0\*1 \(-> Don't use Hilite Mouse Tracking, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*0\*2 \(-> Don't use Cell Motion Mouse Tracking, \fI\*(xt\fP.  See the section \fBButton-event tracking\fP.
  \*(Ps = \*1\*0\*0\*3 \(-> Don't use All Motion Mouse Tracking, \fI\*(xt\fP.  See the section \fBAny-event tracking\fP.
  \*(Ps = \*1\*0\*0\*4 \(-> Don't send \fBFocusIn/FocusOut\fP events, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*0\*5 \(-> Disable UTF-8 Mouse Mode, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*0\*6 \(-> Disable SGR Mouse Mode, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*0\*7 \(-> Disable \fIAlternate Scroll Mode\fP, \fI\*(xt\fP.
This corresponds to the \fBalternateScroll\fP resource.
  \*(Ps = \*1\*0\*1\*0 \(-> Don't scroll to bottom on tty output (rxvt).  This sets the \fBscrollTtyOutput\fP resource to \*(``false\*(''.
  \*(Ps = \*1\*0\*1\*1 \(-> Don't scroll to bottom on key press (rxvt).  This sets the \fBscrollKey\fP resource to \*(``false\*(''.
  \*(Ps = \*1\*0\*1\*5 \(-> Disable urxvt Mouse Mode.
  \*(Ps = \*1\*0\*3\*4 \(-> Don't interpret \*(``meta\*('' key, \fI\*(xt\fP.
This disables the \fBeightBitInput\fP resource.
  \*(Ps = \*1\*0\*3\*5 \(-> Disable special modifiers for
Alt and NumLock keys, \fI\*(xt\fP.
This disables the \fBnumLock\fP resource.
  \*(Ps = \*1\*0\*3\*6 \(-> Don't send \*(Es when Meta modifies a key,
\fI\*(xt\fP.
This disables the \fBmetaSendsEscape\fP resource.
  \*(Ps = \*1\*0\*3\*7 \(-> Send VT220 Remove from the editing-keypad
\fIDelete\fP key, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*3\*9 \(-> Don't send \*(Es
when Alt modifies a key, \fI\*(xt\fP.
This disables the \fBaltSendsEscape\fP resource.
  \*(Ps = \*1\*0\*4\*0 \(-> Do not keep selection
when not highlighted, \fI\*(xt\fP.
This disables the \fBkeepSelection\fP resource.
  \*(Ps = \*1\*0\*4\*1 \(-> Use the PRIMARY selection, \fI\*(xt\fP.
This disables the \fBselectToClipboard\fP resource.
  \*(Ps = \*1\*0\*4\*2 \(-> Disable Urgency window manager hint
when Control-G is received, \fI\*(xt\fP.
This disables the \fBbellIsUrgent\fP resource.
  \*(Ps = \*1\*0\*4\*3 \(-> Disable raising of the window
when Control-G is received, \fI\*(xt\fP.
This disables the \fBpopOnBell\fP resource.
  \*(Ps = \*1\*0\*4\*6 \(-> Disable switching to/from
\fIAlternate Screen Buffer\fP, \fI\*(xt\fP.
This works for terminfo-based systems, updating the \fBtiteInhibit\fP resource.
If currently using the \fIAlternate Screen Buffer\fP,
\fI\*(xt\fP switches to the Normal Screen Buffer.
  \*(Ps = \*1\*0\*4\*7 \(-> Use Normal Screen Buffer, \fI\*(xt\fP.
Clear the screen first if in the \fIAlternate Screen Buffer\fP.
This may be disabled by the \fBtiteInhibit\fP resource.
  \*(Ps = \*1\*0\*4\*8 \(-> Restore cursor as in DECRC, \fI\*(xt\fP.
This may be disabled by the \fBtiteInhibit\fP resource.
  \*(Ps = \*1\*0\*4\*9 \(-> Use Normal Screen Buffer and restore cursor
as in DECRC, \fI\*(xt\fP.
This may be disabled by the \fBtiteInhibit\fP resource.
This combines the effects of the \*1\*0\*4\*7 and \*1\*0\*4\*8 modes.
Use this with terminfo-based applications rather than the \*4\*7 mode.
  \*(Ps = \*1\*0\*5\*0 \(-> Reset terminfo/termcap function-key mode, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*5\*1 \(-> Reset Sun function-key mode, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*5\*2 \(-> Reset HP function-key mode, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*5\*3 \(-> Reset SCO function-key mode, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*6\*0 \(-> Reset legacy keyboard emulation, i.e, X11R6, \fI\*(xt\fP.
  \*(Ps = \*1\*0\*6\*1 \(-> Reset keyboard emulation to Sun/PC style, \fI\*(xt\fP.
  \*(Ps = \*2\*0\*0\*4 \(-> Reset bracketed paste mode, \fI\*(xt\fP.
.
.iP
.IP \\*(Cs\\*(Pm\\*s\\*m
Character Attributes (SGR).
  \*(Ps = \*0 \(-> Normal (default), VT100.
  \*(Ps = \*1 \(-> Bold, VT100.
  \*(Ps = \*2 \(-> Faint, decreased intensity, ECMA-48 2nd.
  \*(Ps = \*3 \(-> Italicized, ECMA-48 2nd.
  \*(Ps = \*4 \(-> Underlined, VT100.
  \*(Ps = \*5 \(-> Blink, VT100.
.br
This appears as Bold in X11R6 xterm.
  \*(Ps = \*7 \(-> Inverse, VT100.
  \*(Ps = \*8 \(-> Invisible, i.e., hidden, ECMA-48 2nd, VT300.
  \*(Ps = \*9 \(-> Crossed-out characters, ECMA-48 3rd.
  \*(Ps = \*2\*1 \(-> Doubly-underlined, ECMA-48 3rd.
  \*(Ps = \*2\*2 \(-> Normal (neither bold nor faint), ECMA-48 3rd.
  \*(Ps = \*2\*3 \(-> Not italicized, ECMA-48 3rd.
  \*(Ps = \*2\*4 \(-> Not underlined, ECMA-48 3rd.
  \*(Ps = \*2\*5 \(-> Steady (not blinking), ECMA-48 3rd.
  \*(Ps = \*2\*7 \(-> Positive (not inverse), ECMA-48 3rd.
  \*(Ps = \*2\*8 \(-> Visible, i.e., not hidden, ECMA-48 3rd, VT300.
  \*(Ps = \*2\*9 \(-> Not crossed-out, ECMA-48 3rd.
  \*(Ps = \*3\*0 \(-> Set foreground color to Black.
  \*(Ps = \*3\*1 \(-> Set foreground color to Red.
  \*(Ps = \*3\*2 \(-> Set foreground color to Green.
  \*(Ps = \*3\*3 \(-> Set foreground color to Yellow.
  \*(Ps = \*3\*4 \(-> Set foreground color to Blue.
  \*(Ps = \*3\*5 \(-> Set foreground color to Magenta.
  \*(Ps = \*3\*6 \(-> Set foreground color to Cyan.
  \*(Ps = \*3\*7 \(-> Set foreground color to White.
  \*(Ps = \*3\*9 \(-> Set foreground color to default, ECMA-48 3rd.
  \*(Ps = \*4\*0 \(-> Set background color to Black.
  \*(Ps = \*4\*1 \(-> Set background color to Red.
  \*(Ps = \*4\*2 \(-> Set background color to Green.
  \*(Ps = \*4\*3 \(-> Set background color to Yellow.
  \*(Ps = \*4\*4 \(-> Set background color to Blue.
  \*(Ps = \*4\*5 \(-> Set background color to Magenta.
  \*(Ps = \*4\*6 \(-> Set background color to Cyan.
  \*(Ps = \*4\*7 \(-> Set background color to White.
  \*(Ps = \*4\*9 \(-> Set background color to default, ECMA-48 3rd.
.sP
Some of the above note the edition of ECMA-48 which first describes
a feature.
In its successive editions from 1979 to 1991
(\fI2nd\fP 1979, \fI3rd\fP 1984, \fI4th\fP 1986, and \fI5th\fP 1991),
ECMA-48 listed codes through \*6\*5
(skipping several toward the end of the range).
Most of the ECMA-48 codes not implemented in \fI\*(xt\fP
were never implemented in a hardware terminal.
Several (such as \*3\*9 and \*4\*9) are either noted in ECMA-48
as implementation defined, or described in vague terms.
.sP
The successive editions of ECMA-48 give little attention to
changes from one edition to the next,
except to comment on features which have become obsolete.
ECMA-48 1st (1976) is unavailable;
there is no reliable source of information which states whether
\*(``ANSI\*('' color was defined in that edition,
or later (1979).
The VT100 (1978) implemented the most commonly used non-color video attributes
which are given in the 2nd edition.
.sP
While 8-color support is described in ECMA-48 2nd edition,
the VT500 series (introduced in 1993)
were the first DEC terminals implementing \*(``ANSI\*('' color.
The DEC terminal's use of color is known to differ from \fI\*(xt\fP;
useful documentation on this series
became available too late to influence \fI\*(xt\fP.
.sP
If 16-color support is compiled, the following \fIaixterm\fP controls apply.
Assume that \fI\*(xt\fP's resources
are set so that the ISO color codes are the first 8 of a set of 16.
Then the \fIaixterm\fP colors are the bright versions of the ISO colors:
.iP
  \*(Ps = \*9\*0 \(-> Set foreground color to Black.
  \*(Ps = \*9\*1 \(-> Set foreground color to Red.
  \*(Ps = \*9\*2 \(-> Set foreground color to Green.
  \*(Ps = \*9\*3 \(-> Set foreground color to Yellow.
  \*(Ps = \*9\*4 \(-> Set foreground color to Blue.
  \*(Ps = \*9\*5 \(-> Set foreground color to Magenta.
  \*(Ps = \*9\*6 \(-> Set foreground color to Cyan.
  \*(Ps = \*9\*7 \(-> Set foreground color to White.
  \*(Ps = \*1\*0\*0 \(-> Set background color to Black.
  \*(Ps = \*1\*0\*1 \(-> Set background color to Red.
  \*(Ps = \*1\*0\*2 \(-> Set background color to Green.
  \*(Ps = \*1\*0\*3 \(-> Set background color to Yellow.
  \*(Ps = \*1\*0\*4 \(-> Set background color to Blue.
  \*(Ps = \*1\*0\*5 \(-> Set background color to Magenta.
  \*(Ps = \*1\*0\*6 \(-> Set background color to Cyan.
  \*(Ps = \*1\*0\*7 \(-> Set background color to White.
.sP
If \fI\*(xt\fP is compiled with the 16-color support disabled, it supports
the following, from \fIrxvt\fP:
  \*(Ps = \*1\*0\*0 \(-> Set foreground and background color to default.
.sP
\fI\*(XT\fP maintains a color palette
whose entries are identified by an index beginning with zero.
If 88- or 256-color support is compiled, the following apply:
.RS
.bP
All parameters are decimal integers.
.bP
RGB values range from zero (0) to 255.
.bP
ISO-8613-6 has been interpreted in more than one way;
\fI\*(xt\fP allows the semicolons separating the subparameters
in this control to be replaced by colons
(but after the first colon, colons must be used).
.RE
.sP
These ISO-8613-6 controls (marked in ECMA-48 5th edition as
\*(``reserved for future standardization\*('')
are supported by \fI\*(xt\fP:
  \*(Pm = \*3\*8\*;\*2\*;\*(Pi\*s\*;\*(Pr\*s\*;\*(Pg\*s\*;\*(Pb \(-> Set foreground
color using RGB values.
If \fI\*(xt\fP is not compiled with direct-color support,
it uses the closest match in its palette
for the given RGB \*(Pr/\*(Pg/\*(Pb.
The color space identifier \*(Pi is ignored.
  \*(Pm = \*3\*8\*;\*5\*;\*(Ps \(-> Set foreground color to \*(Ps,
using indexed color.
  \*(Pm = \*4\*8\*;\*2\*;\*(Pi\*s\*;\*(Pr\*s\*;\*(Pg\*s\*;\*(Pb \(-> Set background
color using RGB values.
If \fI\*(xt\fP is not compiled with direct-color support,
it uses the closest match in its palette
for the given RGB \*(Pr/\*(Pg/\*(Pb.
The color space identifier \*(Pi is ignored.
  \*(Pm = \*4\*8\*;\*5\*;\*(Ps \(-> Set background color to \*(Ps,
using indexed color.
.sP
This variation on ISO-8613-6 is supported for compatibility with KDE konsole:
  \*(Pm = \*3\*8\*;\*2\*;\*(Pr\*s\*;\*(Pg\*s\*;\*(Pb \(-> Set foreground color
using RGB values.
If \fI\*(xt\fP is not compiled with direct-color support,
it uses the closest match in its palette
for the given RGB \*(Pr/\*(Pg/\*(Pb.
  \*(Pm = \*4\*8\*;\*2\*;\*(Pr\*s\*;\*(Pg\*s\*;\*(Pb \(-> Set background color
using RGB values.
If \fI\*(xt\fP is not compiled with direct-color support,
it uses the closest match in its palette
for the given RGB \*(Pr/\*(Pg/\*(Pb.
.sP
In each case,
if \fI\*(xt\fP is compiled with direct-color support,
and the resource \fBdirectColor\fP is true, then
rather than choosing the closest match,
\fI\*(xt\fP asks the X server to directly render a given color.
.
.iP
.IP \\*(Cs\\*>\\*(Pp\\*s\\*;\*(Pv\\*s\\*m
.IP \\*(Cs\\*>\\*(Pp\\*s\\*m
Set/reset key modifier options, \fI\*(xt\fP.
Set or reset resource-values used by \fI\*(xt\fP to decide whether to
construct escape sequences holding information about the modifiers
pressed with a given key.
.iP
.IP
The first parameter \*(Pp identifies the resource to set/reset.
The second parameter \*(Pv is the value to assign to the resource.
.iP
.IP
If the second parameter is omitted, the resource is reset to its initial value.
Values \*3 and \*5 are reserved for keypad-keys and string-keys.
.iP
  \*(Pp = \*0 \(-> \fBmodifyKeyboard\fP.
  \*(Pp = \*1 \(-> \fBmodifyCursorKeys\fP.
  \*(Pp = \*2 \(-> \fBmodifyFunctionKeys\fP.
  \*(Pp = \*4 \(-> \fBmodifyOtherKeys\fP.
.iP
.IP
If no parameters are given, all resources are reset to their initial values.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*n
Device Status Report (DSR).
  \*(Ps = \*5 \(-> Status Report.
.br
Result (\*(``OK\*('') is
\*(Cs\*0\*n
  \*(Ps = \*6 \(-> Report Cursor Position (CPR) [row;column].
.br
Result is
\*(Cs\*(Ir\*s\*;\*(Ic\*s\*R
.iP
.IP
\fBNote\fP:
it is possible for this sequence to be sent by a function key.
For example, with the default keyboard configuration
the shifted F1 key may send (with shift-, control-, alt-modifiers)
.iP
  \*(Cs\*1\*;\*2\*s\*R, or
  \*(Cs\*1\*;\*5\*s\*R, or
  \*(Cs\*1\*;\*6\*s\*R, etc.
.iP
.IP
The second parameter encodes the modifiers; values range from 2 to 16.
See the section \fBPC-Style Function Keys\fP for the codes.
The \fBmodifyFunctionKeys\fP and \fBmodifyKeyboard\fP resources
can change the form of the string sent from the modified F1 key.
.
.iP
.IP \\*(Cs\\*>\\*(Pm\\*s\\*n
Disable key modifier options, \fI\*(xt\fP.
These modifiers may be enabled via the
\*(Cs\*>\*(Pm\*s\*m
sequence.
This control sequence corresponds to a resource value of \*(``\-1\*('',
which cannot be set with the other sequence.
.iP
.IP
The parameter identifies the resource to be disabled:
.iP
  \*(Ps = \*0 \(-> \fBmodifyKeyboard\fP.
  \*(Ps = \*1 \(-> \fBmodifyCursorKeys\fP.
  \*(Ps = \*2 \(-> \fBmodifyFunctionKeys\fP.
  \*(Ps = \*4 \(-> \fBmodifyOtherKeys\fP.
.iP
If the parameter is omitted, \fBmodifyFunctionKeys\fP is disabled.
When \fBmodifyFunctionKeys\fP is disabled, \fI\*(xt\fP uses the
modifier keys to make an extended sequence of function keys rather
than adding a parameter to each function key to denote the modifiers.
.
.iP
.IP \\*(Cs\\*?\\*(Ps\\*s\\*n
Device Status Report (DSR, DEC-specific).
  \*(Ps = \*6 \(-> Report Cursor Position (DECXCPR).
The response [row;column] is returned as
.br
\*(Cs\*?\*(Ir\*s\*;\*(Ic\*s\*R
.br
(assumes the default page, i.e., \*(``1\*('').
  \*(Ps = \*1\*5 \(-> Report Printer status.
The response is
.br
\*(Cs\*?\*1\*0\*n (ready).
or
.br
\*(Cs\*?\*1\*1\*n (not ready).
  \*(Ps = \*2\*5 \(-> Report UDK status.
The response is
.br
\*(Cs\*?\*2\*0\*n (unlocked)
.br
or
.br
\*(Cs\*?\*2\*1\*n (locked).
  \*(Ps = \*2\*6 \(-> Report Keyboard status.
The response is
.br
\*(Cs\*?\*2\*7\*;\*1\*;\*0\*;\*0\*n (North American).
.iP
.IP
The last two parameters apply to VT300 & up (keyboard ready) and
VT400 & up (LK01) respectively.
.iP
  \*(Ps = \*5\*3 \(-> Report Locator status.
The response is
\*(Cs\*?\*5\*3\*n Locator available, if compiled-in, or
\*(Cs\*?\*5\*0\*n No Locator, if not.
  \*(Ps = \*5\*5 \(-> Report Locator status.
The response is
\*(Cs\*?\*5\*3\*n Locator available, if compiled-in, or
\*(Cs\*?\*5\*0\*n No Locator, if not.
  \*(Ps = \*5\*6 \(-> Report Locator type.
The response is
\*(Cs\*?\*5\*7\*;\*1\*n Mouse, if compiled-in, or
\*(Cs\*?\*5\*7\*;\*0\*n Cannot identify, if not.
  \*(Ps = \*6\*2 \(-> Report macro space (DECMSR).
The response is
\*(Cs\*(Pn\*s\**\*s\*{.
  \*(Ps = \*6\*3 \(-> Report memory checksum (DECCKSR).
The response is
\*(Dc\*(Pt\*s\*!x\*sx\*sx\*sx\*s\*(ST.
.br
    \*(Pt is the request id (from an optional parameter to the request).
    The x's are hexadecimal digits 0-9 and A-F.
  \*(Ps = \*7\*5 \(-> Report data integrity.
The response is
\*(Cs\*?\*7\*0\*n (ready, no errors).
  \*(Ps = \*8\*5 \(-> Report multi-session configuration.
The response is
\*(Cs\*?\*8\*3\*n (not configured for multiple-session operation).
.
.iP
.IP \\*(Cs\\*>\\*(Ps\\*s\\*p
Set resource value \fBpointerMode\fP.
This is used by \fI\*(xt\fP to decide whether to
hide the pointer cursor as the user types.
.iP
Valid values for the parameter:
  \*(Ps = \*0 \(-> never hide the pointer.
  \*(Ps = \*1 \(-> hide if the mouse tracking mode is not enabled.
  \*(Ps = \*2 \(-> always hide the pointer, except when leaving the window.
  \*(Ps = \*3 \(-> always hide the pointer, even if leaving/entering the window.
.iP
If no parameter is given, \fI\*(xt\fP uses the default,
which is \*1.
.
.iP
.IP \\*(Cs\\*!\\*p
Soft terminal reset (DECSTR), VT220 and up.
.
.iP
.IP \\*(Cs\\*(Pl\\*s\\*;\\*(Pc\\*s\\*(Dq\\*p
Set conformance level (DECSCL), VT220 and up.
.iP
The first parameter selects the conformance level.
Valid values are:
  \*(Pl = \*6\*1 \(-> level 1, e.g., VT100.
  \*(Pl = \*6\*2 \(-> level 2, e.g., VT200.
  \*(Pl = \*6\*3 \(-> level 3, e.g., VT300.
  \*(Pl = \*6\*4 \(-> level 4, e.g., VT400.
  \*(Pl = \*6\*5 \(-> level 5, e.g., VT500.
.iP
The second parameter selects the C1 control transmission mode.
This is an optional parameter, ignored in conformance level 1.
Valid values are:
  \*(Pc = \*0 \(-> 8-bit controls.
  \*(Pc = \*1 \(-> 7-bit controls (DEC factory default).
  \*(Pc = \*2 \(-> 8-bit controls.
.iP
The 7-bit and 8-bit control modes can also be set by S7C1T and S8C1T,
but DECSCL is preferred.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*$\\*p
.br
Request ANSI mode (DECRQM).
For VT300 and up, reply DECRPM is
.br
  \*(Cs\*(Ps\*;\*(Pm\*$\*y
.br
where \*(Ps is the mode number as in SM/RM,
and \*(Pm is the mode value:
.br
  0 - not recognized
  1 - set
  2 - reset
  3 - permanently set
  4 - permanently reset
.
.iP
.IP \\*(Cs\\*?\\*(Ps\\*s\\*$\\*p
Request DEC private mode (DECRQM).
For VT300 and up, reply DECRPM is
.br
  \*(Cs\*?\*(Ps\*;\*(Pm\*$\*y
.br
where \*(Ps is the mode number as in DECSET/DECSET,
\*(Pm is the mode value as in the ANSI DECRQM.
.br
Two private modes are read-only (i.e., \*1\*3 and \*1\*4),
provided only for reporting their values using this control sequence.
They correspond to the resources \fBcursorBlink\fP and \fBcursorBlinkXOR\fP.
.
.IP \\*(Cs\\*#\\*p
.br
.IP \\*(Cs\\*(Pm\\*s\\*#\\*p
Push video attributes onto stack (XTPUSHSGR), \fI\*(xt\fP.
This is an alias for \*(Cs\*#\*{,
used to work around language limitations of C#.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*q
Load LEDs (DECLL), VT100.
  \*(Ps = \*0 \(-> Clear all LEDS (default).
  \*(Ps = \*1 \(-> Light Num Lock.
  \*(Ps = \*2 \(-> Light Caps Lock.
  \*(Ps = \*3 \(-> Light Scroll Lock.
  \*(Ps = \*2\*1 \(-> Extinguish Num Lock.
  \*(Ps = \*2\*2 \(-> Extinguish Caps Lock.
  \*(Ps = \*2\*3 \(-> Extinguish Scroll Lock.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*(Sp\\*q
Set cursor style (DECSCUSR), VT520.
  \*(Ps = \*0 \(-> blinking block.
  \*(Ps = \*1 \(-> blinking block (default).
  \*(Ps = \*2 \(-> steady block.
  \*(Ps = \*3 \(-> blinking underline.
  \*(Ps = \*4 \(-> steady underline.
  \*(Ps = \*5 \(-> blinking bar, \fI\*(xt\fP.
  \*(Ps = \*6 \(-> steady bar, \fI\*(xt\fP.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*(Dq\\*q
Select character protection attribute (DECSCA).
Valid values for the parameter:
  \*(Ps = \*0 \(-> DECSED and DECSEL can erase (default).
  \*(Ps = \*1 \(-> DECSED and DECSEL cannot erase.
  \*(Ps = \*2 \(-> DECSED and DECSEL can erase.
.
.iP
.IP \\*(Cs\\*#\\*q
Pop video attributes from stack (XTPOPSGR), \fI\*(xt\fP.
This is an alias for \*(Cs\*#\*},
used to work around language limitations of C#.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*;\\*(Ps\\*s\\*r
Set Scrolling Region [top;bottom] (default = full size of window)
(DECSTBM), VT100.
.
.iP
.IP \\*(Cs\\*?\\*(Pm\\*s\\*r
Restore DEC Private Mode Values.
The value of \*(Ps previously saved is restored.
\*(Ps values are the same as for DECSET.
.
.iP
.IP \\*(Cs\\*(Pt\\*s\\*;\\*(Pl\\*s\\*;\\*(Pb\\*s\\*;\\*(Pr\\*s\\*;\\*(Ps\\*s\\*$\\*r
Change Attributes in Rectangular Area (DECCARA), VT400 and up.
  \*(Pt\*s\*;\*(Pl\*s\*;\*(Pb\*s\*;\*(Pr denotes the rectangle.
  \*(Ps denotes the SGR attributes to change: 0, 1, 4, 5, 7.
.
.iP
.IP \\*(Cs\\*(cs
Save cursor, available only when DECLRMM is disabled (SCOSC, also ANSI.SYS).
.
.iP
.IP \\*(Cs\\*(Pl\\*s\\*;\\*(Pr\\*s\\*(cs
Set left and right margins (DECSLRM), VT420 and up.
This is available only when DECLRMM is enabled.
.
.iP
.IP \\*(Cs\\*?\\*(Pm\\*s\\*(cs
Save DEC Private Mode Values.
\*(Ps values are the same as for DECSET.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*;\\*(Ps\\*s\\*;\\*(Ps\\*s\\*t
Window manipulation (from \fIdtterm\fP, as well as extensions by \fI\*(xt\fP).
These controls may be disabled using the \fBallowWindowOps\fP resource.
.iP
\fI\*(xt\fP uses \fIExtended Window Manager Hints\fP (EWMH) to maximize
the window.
Some window managers have incomplete support for EWMH.
For instance, \fIfvwm\fP, \fIflwm\fP and \fIquartz-wm\fP advertise
support for maximizing windows horizontally or vertically, but
in fact equate those to the maximize operation.
.iP
Valid values for the first (and any additional parameters) are:
  \*(Ps = \*1 \(-> De-iconify window.
  \*(Ps = \*2 \(-> Iconify window.
  \*(Ps = \*3\*;\*s\fIx\*s\*;\*sy\fP \(-> Move window to [x, y].
  \*(Ps = \*4\*;\*s\fIheight\*s\*;\*swidth\fP \(-> Resize the \fI\*(xt\fP
window to given height and width in pixels.
Omitted parameters reuse the current height or width.
Zero parameters use the display's height or width.
  \*(Ps = \*5 \(-> Raise the \fI\*(xt\fP window to the
front of the stacking order.
  \*(Ps = \*6 \(-> Lower the \fI\*(xt\fP window to the
bottom of the stacking order.
  \*(Ps = \*7 \(-> Refresh the \fI\*(xt\fP window.
  \*(Ps = \*8\*;\*s\fIheight\*s\*;\*swidth\fP \(-> Resize the text area to
given height and width in characters.
Omitted parameters reuse the current height or width.
Zero parameters use the display's height or width.
  \*(Ps = \*9\*;\*s\*0 \(-> Restore maximized window.
  \*(Ps = \*9\*;\*s\*1 \(-> Maximize window (i.e., resize to screen size).
  \*(Ps = \*9\*;\*s\*2 \(-> Maximize window vertically.
  \*(Ps = \*9\*;\*s\*3 \(-> Maximize window horizontally.
  \*(Ps = \*1\*0\*;\*s\*0 \(-> Undo full-screen mode.
  \*(Ps = \*1\*0\*;\*s\*1 \(-> Change to full-screen.
  \*(Ps = \*1\*0\*;\*s\*2 \(-> Toggle full-screen.
  \*(Ps = \*1\*1 \(-> Report \fI\*(xt\fP window state.
.br
If the \fI\*(xt\fP window is non-iconified, it returns \*(Cs\*1\*t.
.br
If the \fI\*(xt\fP window is iconified, it returns \*(Cs\*2\*t.
  \*(Ps = \*1\*3 \(-> Report \fI\*(xt\fP window position.
.br
Note: X Toolkit positions can be negative,
but the reported values are unsigned, in the range 0-65535.
Negative values correspond to 32768-65535.
.br
Result is
\*(Cs\*3\*;\*(Ix\*s\*;\*(Iy\*s\*t
  \*(Ps = \*1\*3\*;\*s\*2 \(-> Report \fI\*(xt\fP text-area position.
.br
Result is
\*(Cs\*3\*;\*(Ix\*s\*;\*(Iy\*s\*t
  \*(Ps = \*1\*4 \(-> Report \fI\*(xt\fP text area size in pixels.
.br
Result is
\*(Cs\*s\*4\*;\*s\fIheight\fP\*s\*;\*s\fIwidth\fP\*s\*t
  \*(Ps = \*1\*4\*;\*s\*2 \(-> Report \fI\*(xt\fP window size in pixels.
.br
Normally \fI\*(xt\fP's \fIwindow\fP is larger than its \fItext area\fP,
since it includes the frame (or decoration) applied by the
window manager, as well as the area used by a scroll-bar.
.br
Result is
\*(Cs\*s\*4\*;\*s\fIheight\fP\*s\*;\*s\fIwidth\fP\*s\*t
  \*(Ps = \*1\*5 \(-> Report size of the screen in pixels.
.br
Result is
\*(Cs\*s\*5\*;\*s\fIheight\fP\*s\*;\*s\fIwidth\fP\*s\*t
  \*(Ps = \*1\*6 \(-> Report \fI\*(xt\fP character cell size in pixels.
.br
Result is
\*(Cs\*s\*6\*;\*s\fIheight\fP\*s\*;\*s\fIwidth\fP\*s\*t
  \*(Ps = \*1\*8 \(-> Report the size of the text area in characters.
.br
Result is
\*(Cs\*s\*8\*;\*s\fIheight\fP\*s\*;\*s\fIwidth\fP\*s\*t
  \*(Ps = \*1\*9 \(-> Report the size of the screen in characters.
.br
Result is
\*(Cs\*s\*9\*;\*s\fIheight\fP\*s\*;\*s\fIwidth\fP\*s\*t
  \*(Ps = \*2\*0 \(-> Report \fI\*(xt\fP window's icon label.
.br
Result is
\*(Os\*s\*L\*s\fIlabel\fP\*s\*(ST
  \*(Ps = \*2\*1 \(-> Report \fI\*(xt\fP window's title.
.br
Result is
\*(Os\*s\*l\*s\fIlabel\fP\*s\*(ST
  \*(Ps = \*2\*2\*;\*0 \(-> Save \fI\*(xt\fP icon and window title
on stack.
  \*(Ps = \*2\*2\*;\*1 \(-> Save \fI\*(xt\fP icon title on stack.
  \*(Ps = \*2\*2\*;\*2 \(-> Save \fI\*(xt\fP window title on stack.
  \*(Ps = \*2\*3\*;\*0 \(-> Restore \fI\*(xt\fP icon and window title
from stack.
  \*(Ps = \*2\*3\*;\*1 \(-> Restore \fI\*(xt\fP icon title from stack.
  \*(Ps = \*2\*3\*;\*2 \(-> Restore \fI\*(xt\fP window title from stack.
  \*(Ps >= \*2\*4 \(-> Resize to \*(Ps lines (DECSLPP), VT340 and VT420.
.br
\fI\*(xt\fP adapts this by resizing its window.
.
.iP
.IP \\*(Cs\\*>\\*(Pm\\*s\\*t
This \fI\*(xt\fP control
sets one or more features of the title modes.
Each parameter enables a single feature.
  \*(Ps = \*0 \(-> Set window/icon labels using hexadecimal.
  \*(Ps = \*1 \(-> Query window/icon labels using hexadecimal.
  \*(Ps = \*2 \(-> Set window/icon labels using UTF-8.
  \*(Ps = \*3 \(-> Query window/icon labels using UTF-8.
(See discussion of \fBTitle Modes\fP)
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*(Sp\\*t
Set warning-bell volume (DECSWBV), VT520.
  \*(Ps = \*0 or \*1 \(-> off.
  \*(Ps = \*2, \*3 or \*4 \(-> low.
  \*(Ps = \*5, \*6, \*7, or \*8 \(-> high.
.
.iP
.IP \\*(Cs\\*(Pt\\*s\\*;\\*(Pl\\*s\\*;\\*(Pb\\*s\\*;\\*(Pr\\*s\\*;\\*(Ps\\*s\\*$\\*t
Reverse Attributes in Rectangular Area (DECRARA), VT400 and up.
  \*(Pt\*s\*;\*(Pl\*s\*;\*(Pb\*s\*;\*(Pr denotes the rectangle.
  \*(Ps denotes the attributes to reverse, i.e.,  1, 4, 5, 7.
.
.iP
.IP \\*(Cs\\*u
Restore cursor (SCORC, also ANSI.SYS).
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*(Sp\\*u
Set margin-bell volume (DECSMBV), VT520.
  \*(Ps = \*0, \*5, \*6, \*7, or \*8 \(-> high.
  \*(Ps = \*1 \(-> off.
  \*(Ps = \*2, \*3 or \*4 \(-> low.
.
.iP
.IP \\*(Cs\\*(Pt\\*s\\*;\\*(Pl\\*s\\*;\\*(Pb\\*s\\*;\\*(Pr\\*s\\*;\\*(Pp\\*s\\*;\\*(Pt\\*s\\*;\\*(Pl\\*s\\*;\\*(Pp\\*s\\*$\\*v
Copy Rectangular Area (DECCRA), VT400 and up.
  \*(Pt\*s\*;\*(Pl\*s\*;\*(Pb\*s\*;\*(Pr denotes the rectangle.
  \*(Pp denotes the source page.
  \*(Pt\*s\*;\*(Pl denotes the target location.
  \*(Pp denotes the target page.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*$\\*w
Request presentation state report (DECRQPSR), VT320 and up.
  \*(Ps = \*0 \(-> error.
  \*(Ps = \*1 \(-> cursor information report (DECCIR).
.br
Response is
.br
  \*(Dc\*1\*$\*u\*(Pt\*s\*(ST
.br
Refer to the VT420 programming manual,
which requires six pages to document the data string \*(Pt,
  \*(Ps = \*2 \(-> tab stop report (DECTABSR).
.br
Response is
.br
  \*(Dc\*2\*$\*u\*(Pt\*s\*(ST
.br
The data string \*(Pt is a list of the tab-stops,
separated by \*(``/\*('' characters.
.
.iP
.IP \\*(Cs\\*(Pt\\*s\\*;\\*(Pl\\*s\\*;\\*(Pb\\*s\\*;\\*(Pr\\*s\\*(qu\\*w
Enable Filter Rectangle (DECEFR), VT420 and up.
.br
Parameters are [top;left;bottom;right].
.br
Defines the coordinates of a filter rectangle and activates it.
Anytime the locator is detected outside of the filter rectangle,
an outside rectangle event is generated and the rectangle is disabled.
Filter rectangles are always treated as \*(``one-shot\*('' events.
Any parameters that are omitted default to the current locator position.
If all parameters are omitted, any locator motion will be reported.
DECELR always cancels any prevous rectangle definition.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*x
Request Terminal Parameters (DECREQTPARM).
.br
if \*(Ps is a \*(``0\*('' (default) or \*(``1\*('',
and \fI\*(xt\fR is emulating VT100,
the control sequence elicits a response of the same form
whose parameters describe the terminal:
  \*(Ps \(-> the given \*(Ps incremented by 2.
  \*(Pn = \*1 \(<- no parity.
  \*(Pn = \*1 \(<- eight bits.
  \*(Pn = \*1 \(<- \*2\*8 transmit 38.4k baud.
  \*(Pn = \*1 \(<- \*2\*8 receive 38.4k baud.
  \*(Pn = \*1 \(<- clock multiplier.
  \*(Pn = \*0 \(<- STP flags.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\**\\*x
Select Attribute Change Extent (DECSACE), VT420 and up.
  \*(Ps = \*0 \(-> from start to end position, wrapped.
  \*(Ps = \*1 \(-> from start to end position, wrapped.
  \*(Ps = \*2 \(-> rectangle (exact).
.
.iP
.IP \\*(Cs\\*(Pc\\*s\\*;\\*(Pt\\*s\\*;\\*(Pl\\*s\\*;\\*(Pb\\*s\\*;\\*(Pr\\*s\\*$\\*x
Fill Rectangular Area (DECFRA), VT420 and up.
  \*(Pc is the character to use.
  \*(Pt\*s\*;\*(Pl\*s\*;\*(Pb\*s\*;\*(Pr denotes the rectangle.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*#\\*y
Select checksum extension (XTCHECKSUM), \fI\*(xt\fP.
The bits of \*(Ps modify the calculation of the checksum returned by DECRQCRA:
  \*0 \(-> do not negate the result.
  \*1 \(-> do not report the VT100 video attributes.
  \*2 \(-> do not omit checksum for blanks.
  \*3 \(-> omit checksum for cells not explicitly initialized.
  \*4 \(-> do not mask cell value to 8 bits or ignore combining characters.
  \*5 \(-> do not mask cell value to 7 bits.
.
.iP
.IP \\*(Cs\\*(Pi\\*s\\*;\\*(Pg\\*s\\*;\\*(Pt\\*s\\*;\\*(Pl\\*s\\*;\\*(Pb\\*s\\*;\\*(Pr\\*s\\**\\*y
Request Checksum of Rectangular Area (DECRQCRA), VT420 and up.
Response is
.br
\*(Dc\*(Pi\*s\*!\*~x\*sx\*sx\*sx\*s\*(ST
.br
  \*(Pi is the request id.
  \*(Pg is the page number.
  \*(Pt\*s\*;\*(Pl\*s\*;\*(Pb\*s\*;\*(Pr denotes the rectangle.
  The x's are hexadecimal digits 0-9 and A-F.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*;\\*(Pu\\*s\\*(qu\\*z
Enable Locator Reporting (DECELR).
.br
Valid values for the first parameter:
  \*(Ps = \*0 \(-> Locator disabled (default).
  \*(Ps = \*1 \(-> Locator enabled.
  \*(Ps = \*2 \(-> Locator enabled for one report, then disabled.
.br
The second parameter specifies the coordinate unit for locator reports.
.br
Valid values for the second parameter:
  \*(Pu = \*0 or omitted \(-> default to character cells.
  \*(Pu = \*1 \(<- device physical pixels.
  \*(Pu = \*2 \(<- character cells.
.
.iP
.IP \\*(Cs\\*(Pt\\*s\\*;\\*(Pl\\*s\\*;\\*(Pb\\*s\\*;\\*(Pr\\*s\\*$\\*z
Erase Rectangular Area (DECERA), VT400 and up.
  \*(Pt\*s\*;\*(Pl\*s\*;\*(Pb\*s\*;\*(Pr denotes the rectangle.
.
.iP
.IP \\*(Cs\\*(Pm\\*s\\*(qu\\*{
Select Locator Events (DECSLE).
.br
Valid values for the first (and any additional parameters) are:
  \*(Ps = \*0 \(-> only respond to explicit host requests (DECRQLP).
This is default.
It also cancels any filter rectangle.
  \*(Ps = \*1 \(-> report button down transitions.
  \*(Ps = \*2 \(-> do not report button down transitions.
  \*(Ps = \*3 \(-> report button up transitions.
  \*(Ps = \*4 \(-> do not report button up transitions.
.
.iP
.IP \\*(Cs\\*#\\*{
.br
.IP \\*(Cs\\*(Pm\\*s\\*#\\*{
Push video attributes onto stack (XTPUSHSGR), \fI\*(xt\fP.
The optional parameters correspond to the SGR encoding for video attributes,
except for colors (which do not have a unique SGR code):
  \*(Ps = \*1 \(-> Bold.
  \*(Ps = \*2 \(-> Faint.
  \*(Ps = \*3 \(-> Italicized.
  \*(Ps = \*4 \(-> Underlined.
  \*(Ps = \*5 \(-> Blink.
  \*(Ps = \*7 \(-> Inverse.
  \*(Ps = \*8 \(-> Invisible.
  \*(Ps = \*9 \(-> Crossed-out characters.
  \*(Ps = \*1\*0 \(-> Foreground color.
  \*(Ps = \*1\*1 \(-> Background color.
  \*(Ps = \*2\*1 \(-> Doubly-underlined.
.iP
.IP
If no parameters are given, all of the video attributes are saved.
The stack is limited to 10 levels.
.
.iP
.IP \\*(Cs\\*(Pt\\*s\\*;\\*(Pl\\*s\\*;\\*(Pb\\*s\\*;\\*(Pr\\*s\\*$\\*{
Selective Erase Rectangular Area (DECSERA), VT400 and up.
  \*(Pt\*s\*;\*(Pl\*s\*;\*(Pb\*s\*;\*(Pr denotes the rectangle.
.
.iP
.IP \\*(Cs\\*(Pt\\*s\\*;\\*(Pl\\*s\\*;\\*(Pb\\*s\\*;\\*(Pr\\*s\\*#\\*|
Report selected graphic rendition (XTREPORTSGR), \fI\*(xt\fP.
The response is an SGR sequence which contains the attributes which
are common to all cells in a rectangle.
  \*(Pt\*s\*;\*(Pl\*s\*;\*(Pb\*s\*;\*(Pr denotes the rectangle.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*$\\*|
Select columns per page (DECSCPP), VT340.
  \*(Ps = \*0 \(-> 80 columns, default if \*(Ps omitted.
  \*(Ps = \*8\*0 \(-> 80 columns.
  \*(Ps = \*1\*3\*2 \(-> 132 columns.
.
.iP
.IP \\*(Cs\\*(Ps\\*s\\*(qu\\*|
Request Locator Position (DECRQLP).
.br
Valid values for the parameter are:
  \*(Ps = \*0, 1 or omitted \(-> transmit a single DECLRP locator report.
.sP
If Locator Reporting has been enabled by a DECELR, \fI\*(xt\fP will respond
with a DECLRP Locator Report.
This report is also generated on button
up and down events if they have been enabled with a DECSLE, or when
the locator is detected outside of a filter rectangle, if filter rectangles
have been enabled with a DECEFR.
.sP
  \(<- \*(Cs\*(Pe\*s\*;\*(Pb\*s\*;\*(Pr\*s\*;\*(Pc\*s\*;\*(Pp\*s\*&\*s\*w
.sP
Parameters are [\fIevent\fP;\fIbutton\fP;\fIrow\fP;\fIcolumn\fP;\fIpage\fP].
.br
Valid values for the event:
  \*(Pe = \*0 \(<- locator unavailable - no other parameters sent.
  \*(Pe = \*1 \(<- request - \fI\*(xt\fP received a DECRQLP.
  \*(Pe = \*2 \(<- left button down.
  \*(Pe = \*3 \(<- left button up.
  \*(Pe = \*4 \(<- middle button down.
  \*(Pe = \*5 \(<- middle button up.
  \*(Pe = \*6 \(<- right button down.
  \*(Pe = \*7 \(<- right button up.
  \*(Pe = \*8 \(<- M4 button down.
  \*(Pe = \*9 \(<- M4 button up.
  \*(Pe = \*1\*0 \(<- locator outside filter rectangle.
.br
The \*(``\fIbutton\fP\*('' parameter is a bitmask indicating
which buttons are pressed:
  \*(Pb = \*0 \(<- no buttons down.
  \*(Pb & \*1 \(<- right button down.
  \*(Pb & \*2 \(<- middle button down.
  \*(Pb & \*4 \(<- left button down.
  \*(Pb & \*8 \(<- M4 button down.
.br
The \*(``\fIrow\fP\*('' and \*(``\fIcolumn\fP\*('' parameters
are the coordinates of the locator position in the \fI\*(xt\fP window,
encoded as ASCII decimal.
.br
The \*(``\fIpage\fP\*('' parameter is not used by \*(xt.
.iP
.IP \\*(Cs\\*(Ps\\*s\\**\\*|
Select number of lines per screen (DECSNLS), VT420 and up.
.iP
.IP \\*(Cs\\*#\\*}
Pop video attributes from stack (XTPOPSGR), \fI\*(xt\fP.
Popping restores the video-attributes which were saved using XTPUSHSGR
to their previous state.
.
.iP
.IP \\*(Cs\\*(Pm\\*s\\*(qu\\*}
Insert \*(Ps Column(s) (default = 1) (DECIC), VT420 and up.
.
.iP
.IP \\*(Cs\\*(Pm\\*s\\*(qu\\*~
Delete \*(Ps Column(s) (default = 1) (DECDC), VT420 and up.
.
.Ed
.
.Ss Operating System Commands
.St
.
.IP \\*(Os\\*(Ps\\*s\\*;\\*(Pt\\*s\\*(Be
.iP
.IP \\*(Os\\*(Ps\\*s\\*;\\*(Pt\\*s\\*(ST
Set Text Parameters.
For colors and font,
if \*(Pt is a \*(``?\*('',
the control sequence elicits a response which consists
of the control sequence which would set the corresponding value.
The \fIdtterm\fP control sequences allow you to determine the icon name
and window title.
  \*(Ps = \*0 \(-> Change Icon Name and Window Title to \*(Pt.
  \*(Ps = \*1 \(-> Change Icon Name to \*(Pt.
  \*(Ps = \*2 \(-> Change Window Title to \*(Pt.
  \*(Ps = \*3 \(-> Set X property on top-level window.
\*(Pt should be in the form
\*(``\fIprop=value\fP\*('', or just
\*(``\fIprop\fP\*('' to delete the property.
  \*(Ps = \*4\*;\fIc\fP\*s\*;\fIspec\fP \(-> Change Color Number \fIc\fP to
the color specified by \fIspec\fP.
This can be a name or RGB specification as per \fIXParseColor\fP.
Any number of \fIc\fP/\fIspec\fP pairs may be given.
The color numbers correspond to the ANSI colors 0-7,
their bright versions 8-15,
and if supported, the remainder of the 88-color or 256-color table.
.sP
If a \*(``?\*('' is given rather than a name or RGB specification,
\fI\*(xt\fP replies with a control sequence of the same form which can be used
to set the corresponding color.
Because more than one pair of color number and specification can
be given in one control sequence, \fI\*(xt\fR can make more than one reply.
.sP
  \*(Ps = \*5\*;\fIc\fP\*s\*;\fIspec\fP \(-> Change Special Color Number \fIc\fP to
the color specified by \fIspec\fP.
This can be a name or RGB specification as per \fIXParseColor\fP.
Any number of \fIc\fP/\fIspec\fP pairs may be given.
The special colors can also be set by adding the maximum number of colors
to these codes in an \*(Os\*4 control:
.sP
.in +2n
  \*(Pc = \*0 \(<- resource \fBcolorBD\fP (BOLD).
  \*(Pc = \*1 \(<- resource \fBcolorUL\fP (UNDERLINE).
  \*(Pc = \*2 \(<- resource \fBcolorBL\fP (BLINK).
  \*(Pc = \*3 \(<- resource \fBcolorRV\fP (REVERSE).
  \*(Pc = \*4 \(<- resource \fBcolorIT\fP (ITALIC).
.in -2n
.sP
  \*(Ps = \*6\*;\fIc\fP\*s\*;\fIf\fP \(-> Enable/disable Special Color Number \fIc\fP.
The second parameter tells \fI\*(xt\fP to enable the corresponding color
mode if nonzero, disable it if zero.
\*(Os\*6 is the same as \*(Os\*1\*0\*6.
.sP
The 10 colors (below) which may be set or queried
using \*1\*0 through \*1\*9 are
denoted \fIdynamic colors\fR, since the corresponding control sequences
were the first means for setting \fI\*(xt\fR's colors dynamically,
i.e., after it was started.
They are not the same as the ANSI colors
(however, the dynamic text foreground and background colors
are used when ANSI colors are reset using SGR \*3\*9 and \*4\*9, respectively).
These controls may be disabled using the \fBallowColorOps\fP resource.
At least one parameter is expected for \*(Pt.
Each successive parameter changes the next color in the list.
The value of \*(Ps tells the starting point in the list.
The colors are specified by name or RGB specification as per \fIXParseColor\fP.
.sP
If a \*(``?\*('' is given rather than a name or RGB specification,
\fI\*(xt\fP replies with a control sequence of the same form which can be used
to set the corresponding dynamic color.
Because more than one pair of color number and specification can
be given in one control sequence, \fI\*(xt\fR can make more than one reply.
.sP
  \*(Ps = \*1\*0 \(-> Change VT100 text foreground color to \*(Pt.
  \*(Ps = \*1\*1 \(-> Change VT100 text background color to \*(Pt.
  \*(Ps = \*1\*2 \(-> Change text cursor color to \*(Pt.
  \*(Ps = \*1\*3 \(-> Change mouse foreground color to \*(Pt.
  \*(Ps = \*1\*4 \(-> Change mouse background color to \*(Pt.
  \*(Ps = \*1\*5 \(-> Change Tektronix foreground color to \*(Pt.
  \*(Ps = \*1\*6 \(-> Change Tektronix background color to \*(Pt.
  \*(Ps = \*1\*7 \(-> Change highlight background color to \*(Pt.
  \*(Ps = \*1\*8 \(-> Change Tektronix cursor color to \*(Pt.
  \*(Ps = \*1\*9 \(-> Change highlight foreground color to \*(Pt.
.sP
  \*(Ps = \*4\*6 \(-> Change Log File to \*(Pt.
This is normally disabled by a compile-time option.
.sP
  \*(Ps = \*5\*0 \(-> Set Font to \*(Pt.
These controls may be disabled using the \fBallowFontOps\fP resource.
If \*(Pt begins with a \*(``#\*('', index in the font menu, relative (if the
next character is a plus or minus sign) or absolute.
A number is
expected but not required after the sign (the default is the current
entry for relative, zero for absolute indexing).
.sP
The same rule (plus or minus sign, optional number) is used when
querying the font.
The remainder of \*(Pt is ignored.
.sP
A font can be specified after a \*(``#\*('' index expression,
by adding a space and then the font specifier.
.sP
If the \fBTrueType Fonts\fP menu entry is set (the \fBrenderFont\fP resource),
then this control sets/queries the \fBfaceName\fP resource.
.sP
  \*(Ps = \*5\*1 \(-> reserved for Emacs shell.
.sP
  \*(Ps = \*5\*2 \(-> Manipulate Selection Data.
These controls may be disabled using the \fBallowWindowOps\fP resource.
The parameter \*(Pt is parsed as
.br
	\*(Pc\*s\*;\*(Pd
.br
The first, \*(Pc, may contain zero or more characters from the
set \*c, \*p, \*q, \*(cs, \*0, \*1, \*2, \*3, \*4, \*5, \*6, and \*7.
It is used to construct a list of selection parameters for
clipboard,
primary,
secondary,
select,
or cut buffers 0 through 7 respectively,
in the order given.
If the parameter is empty, \fI\*(xt\fP uses \*(cs\*0,
to specify the configurable primary/clipboard selection and cut buffer 0.
.sP
The second parameter, \*(Pd, gives the selection data.
Normally this is a string encoded in base64 (RFC-4648).
The data becomes the new selection,
which is then available for pasting by other applications.
.sP
If the second parameter is a \*?,
\fI\*(xt\fP replies to the host with the selection
data encoded using the same protocol.
It uses the first selection
found by asking successively
for each item from the list of selection parameters.
.sP
If the second parameter is neither a base64 string nor \*?,
then the selection is cleared.
.sP
  \*(Ps = \*1\*0\*4\*;\fIc\fP \(-> Reset Color Number \fIc\fP.  It is reset to
the color specified by the corresponding X resource.
Any number of \fIc\fP parameters may be given.
These parameters correspond to the ANSI colors 0-7,
their bright versions 8-15,
and if supported, the remainder of the 88-color or 256-color table.
If no parameters are given, the entire table will be reset.
.sP
  \*(Ps = \*1\*0\*5\*;\fIc\fP \(-> Reset Special Color Number \fIc\fP.
It is reset to the color specified by the corresponding X resource.
Any number of \fIc\fP parameters may be given.
These parameters correspond to the special colors which can be set
using an \*(Os\*5 control (or by adding the maximum number of colors
using an \*(Os\*4 control).
.sP
  \*(Ps = \*1\*0\*6\*;\fIc\fP\*s\*;\fIf\fP \(-> Enable/disable Special Color Number \fIc\fP.
The second parameter tells \fI\*(xt\fP to enable the corresponding color
mode if nonzero, disable it if zero.
.sP
.in +2n
  \*(Pc = \*0 \(<- resource \fBcolorBDMode\fP (BOLD).
  \*(Pc = \*1 \(<- resource \fBcolorULMode\fP (UNDERLINE).
  \*(Pc = \*2 \(<- resource \fBcolorBLMode\fP (BLINK).
  \*(Pc = \*3 \(<- resource \fBcolorRVMode\fP (REVERSE).
  \*(Pc = \*4 \(<- resource \fBcolorITMode\fP (ITALIC).
  \*(Pc = \*5 \(<- resource \fBcolorAttrMode\fP (Override ANSI).
.in -2n
.sP
The \fIdynamic colors\fR can also be reset to their default (resource) values:
  \*(Ps = \*1\*1\*0 \(-> Reset VT100 text foreground color.
  \*(Ps = \*1\*1\*1 \(-> Reset VT100 text background color.
  \*(Ps = \*1\*1\*2 \(-> Reset text cursor color.
  \*(Ps = \*1\*1\*3 \(-> Reset mouse foreground color.
  \*(Ps = \*1\*1\*4 \(-> Reset mouse background color.
  \*(Ps = \*1\*1\*5 \(-> Reset Tektronix foreground color.
  \*(Ps = \*1\*1\*6 \(-> Reset Tektronix background color.
  \*(Ps = \*1\*1\*7 \(-> Reset highlight color.
  \*(Ps = \*1\*1\*8 \(-> Reset Tektronix cursor color.
  \*(Ps = \*1\*1\*9 \(-> Reset highlight foreground color.
.sP
  \*(Ps = \*I\*s\*;\fIc\fP \(-> Set icon to file.
Sun shelltool, CDE dtterm.
.br
The file is expected to be XPM format,
and uses the same search logic as the \fBiconHint\fP resource.
.sP
  \*(Ps = \*l\*s\*;\fIc\fP \(-> Set window title.
Sun shelltool, CDE dtterm.
.sP
  \*(Ps = \*L\*s\*;\fIc\fP \(-> Set icon label.
Sun shelltool, CDE dtterm.
.Ed
.
.Ss Privacy Message
.St
.IP \\*(PM\\*(Pt\\*s\\*(ST
\fI\*(xt\fP implements no \*(PM functions; \*(Pt is ignored.
\*(Pt need not be printable characters.
.Ed
.
.Sh "Alt and Meta Keys"
.LP
Many keyboards have keys labeled \*(``Alt\*(''.
Few have keys labeled \*(``Meta\*(''.
However, \fI\*(xt\fP's default translations use the \fIMeta\fP modifier.
Common keyboard configurations assign the \fIMeta\fP modifier
to an \*(``Alt\*('' key.
By using \fIxmodmap\fP one may have the modifier assigned to a different key,
and have \*(``real\*('' alt and meta keys.
Here is an example:
.ID
.ft CW
! put meta on mod3 to distinguish it from alt
keycode 64 = Alt_L
clear mod1
add mod1 = Alt_L
keycode 115 = Meta_L
clear mod3
add mod3 = Meta_L
.ft 1
.DE
.LP
The \fBmetaSendsEscape\fP resource
(and \fBaltSendsEscape\fP if \fBaltIsNotMeta\fP is set)
can be used to control the way the \fIMeta\fP modifier applies to ordinary
keys unless the \fBmodifyOtherKeys\fP resource is set:
.bP
prefix a key with the \*(Es character.
.bP
shift the key from codes 0-127 to 128-255 by adding 128.
.LP
When \fBmodifyOtherKeys\fP is set,
ordinary keys may be sent as escape sequences:
.bP
When \fBmodifyOtherKeys\fP is set to 1,
only the alt- and meta-modifiers apply.
For example, \fIalt-Tab\fP sends 
\*(Cs\*2\*7\*;\*3\*;\*9\*~
(the second parameter is \*(``3\*('' for \fIalt\fP,
and the third parameter is the ASCII value of tab, \*(``9\*('').
.bP
When \fBmodifyOtherKeys\fP is set to 2,
all of the modifiers apply.
For example, \fIshift-Tab\fP sends 
\*(Cs\*2\*7\*;\*2\*;\*9\*~
rather than
\*(Cs\*Z
(the second parameter is \*(``2\*('' for \fIshift\fP).
.LP
The \fBformatOtherKeys\fP resource tells \fI\*n\fP to change the
format of the escape sequences sent when \fBmodifyOtherKeys\fP applies.
When \fBmodifyOtherKeys\fP is set to 1,
for example \fIalt-Tab\fP sends 
\*(Cs\*9\*;\*3\*u
(changing the order of parameters).
One drawback to this format is that applications may confuse it with
\*(Cs\*u (restore-cursor).
.LP
The \fI\*(xt\fP FAQ section
.br
.ID 3
.\" https://invisible-island.net/xterm/xterm.faq.html#xterm_modother
\fIHow can my program distinguish control-I from tab?\fP
.DE
.br
goes into greater detail on this topic.
.LP
The table shows the result for a given character \*(``x\*('' with modifiers
according to the default translations with the resources set on or off.
This assumes \fBaltIsNotMeta\fP is set:
.\" page-eject to work around grohtml bugs
.if t .bp
.TS H
center;
lf3w(2c)	lf3w(2c)	lf3w(2c)	lf3w(2c) .
_
.TH
.T&
l | l | l | l .
key	altSendsEscape	metaSendsEscape	result
_
x	off	off	x
Meta-x	off	off	shift
Alt-x	off	off	shift
Alt+Meta-x	off	off	shift
x	ON	off	x
Meta-x	ON	off	shift
Alt-x	ON	off	\*(Es x
Alt+Meta-x	ON	off	\*(Es shift
x	off	ON	x
Meta-x	off	ON	\*(Es x
Alt-x	off	ON	shift
Alt+Meta-x	off	ON	\*(Es shift
x	ON	ON	x
Meta-x	ON	ON	\*(Es x
Alt-x	ON	ON	\*(Es x
Alt+Meta-x	ON	ON	\*(Es x
_
.TE
.Sh "PC-Style Function Keys"
.LP
If \fI\*(xt\fP does minimal translation of the function keys,
it usually does this
with a PC-style keyboard, so PC-style function keys result.
Sun keyboards are similar to PC keyboards.
Both have cursor and scrolling operations printed on the keypad,
which duplicate the smaller cursor and scrolling keypads.
.LP
X does not predefine NumLock (used for VT220 keyboards) or Alt (used as
an extension for the Sun/PC keyboards) as modifiers.
These keys are recognized as modifiers when enabled
by the \fBnumLock\fP resource,
or by the \*(``DECSET \*1\*0\*3\*5\*('' control sequence.
.LP
The cursor keys transmit the following escape sequences depending on the
mode specified via the DECCKM escape sequence.
.TS H
center;
lf3w(2c)	lf3w(2c)	lf3w(2c) .
Key	Normal	Application
_
.TH
.T&
l | l | l .
Cursor Up	\*(Cs\*A	\*(S3\*A
Cursor Down	\*(Cs\*(cB	\*(S3\*(cB
Cursor Right	\*(Cs\*C	\*(S3\*C
Cursor Left	\*(Cs\*D	\*(S3\*D
_
.TE
The home- and end-keys
(unlike PageUp and other keys also on the 6-key editing keypad)
are considered \*(``cursor keys\*('' by \fI\*(xt\fP.
Their mode is also controlled by the DECCKM escape sequence:
.TS H
center;
lf3w(2c)	lf3w(2c)	lf3w(2c) .
Key	Normal	Application
_
.TH
.T&
l | l | l .
Home	\*(Cs\*H	\*(S3\*H
End	\*(Cs\*F	\*(S3\*F
_
.TE
.LP
The application keypad transmits the following escape sequences depending on the
mode specified via the DECKPNM and DECKPAM escape sequences.
Use the NumLock key to override the application mode.
.LP
Not all keys are present on the Sun/PC keypad (e.g., PF1, Tab),
but are supported by
the program.
.TS H
center;
lf3w(2c)	lf3w(2c)	lf3w(2c)	lf3w(2c)	lf3w(2c) .
Key	Numeric	Application	Terminfo	Termcap
_
.TH
.T&
l | l | l | l | l .
Space	\*(Sp	\*(S3\*(Sp	-	-
Tab	\*(Ta	\*(S3\*I	-	-
Enter	\*(Cr	\*(S3\*M	kent	@8
PF1	\*(S3\*P	\*(S3\*P	kf1	k1
PF2	\*(S3\*Q	\*(S3\*Q	kf2	k2
PF3	\*(S3\*R	\*(S3\*R	kf3	k3
PF4	\*(S3\*S	\*(S3\*S	kf4	k4
* \f1(multiply)\fP	\**	\*(S3\*j	-	-
+ \f1(add)\fP	\*+	\*(S3\*k	-	-
, \f1(comma)\fP	\*,	\*(S3\*l	-	-
- \f1(minus)\fP	\*-	\*(S3\*m	-	-
\&. \f1(Delete)\fP	\*.	\*(Cs\*3\*~	-	-
/ \f1(divide)\fP	\*/	\*(S3\*o	-	-
0 \f1(Insert)\fP	\*0	\*(Cs\*2\*~	-	-
1 \f1(End)\fP	\*1	\*(S3\*F	kc1	K4
2 \f1(DownArrow)\fP	\*2	\*(Cs\*(cB	-	-
3 \f1(PageDown)\fP	\*3	\*(Cs\*6\*~	kc3	K5
4 \f1(LeftArrow)\fP	\*4	\*(Cs\*D	-	-
5 \f1(Begin)\fP	\*5	\*(Cs\*E	kb2	K2
6 \f1(RightArrow)\fP	\*6	\*(Cs\*C	-	-
7 \f1(Home)\fP	\*7	\*(S3\*H	ka1	K1
8 \f1(UpArrow)\fP	\*8	\*(Cs\*A	-	-
9 \f1(PageUp)\fP	\*9	\*(Cs\*5\*~	ka3	K3
= (equal)	\*=	\*(S3\*(XX	-	-
_
.TE
.br
They also provide 12 function keys, as well as a few other special-purpose keys:
.TS H
center;
lf3w(2c)	lf3w(2c) .
Key	Escape Sequence
_
.TH
.T&
l | l .
F1	\*(S3\*P
F2	\*(S3\*Q
F3	\*(S3\*R
F4	\*(S3\*S
F5	\*(Cs\*1\*5\*~
F6	\*(Cs\*1\*7\*~
F7	\*(Cs\*1\*8\*~
F8	\*(Cs\*1\*9\*~
F9	\*(Cs\*2\*0\*~
F10	\*(Cs\*2\*1\*~
F11	\*(Cs\*2\*3\*~
F12	\*(Cs\*2\*4\*~
_
.TE
.sP
Note that F1 through F4 are prefixed with \*(S3,
while the other keys are prefixed with \*(Cs.
Older versions of \fI\*(xt\fP implement different escape sequences
for F1 through F4, with a \*(Cs prefix.
These can be activated by setting the \fBoldXtermFKeys\fP resource.
However, since they do not correspond to any hardware terminal,
they have been deprecated.
(The DEC VT220 reserves F1 through F5 for local functions such as \fISetup\fP).
.TS H
center;
lf3w(2c)	lf3w(2c) .
Key	Escape Sequence
_
.TH
.T&
l | l .
F1	\*(Cs\*1\*1\*~
F2	\*(Cs\*1\*2\*~
F3	\*(Cs\*1\*3\*~
F4	\*(Cs\*1\*4\*~
_
.TE
In normal mode, i.e., a Sun/PC keyboard
when the \fBsunKeyboard\fP resource is false
(and none of the other keyboard resources
such as \fBoldXtermFKeys\fP resource is set),
\fI\*(xt\fP encodes function key modifiers
as parameters appended before the \fIfinal\fP character of the control sequence.
As a special case,
the \*(S3 sent before F1 through F4 is altered to \*(Cs when sending
a function key modifier as a parameter.
.TS H
center;
cf3w(2c)	lf3w(2c) .
Code	Modifiers
_
.TH
.T&
c | l .
2	Shift
3	Alt
4	Shift + Alt
5	Control
6	Shift + Control
7	Alt + Control
8	Shift + Alt + Control
9	Meta
10	Meta + Shift
11	Meta + Alt
12	Meta + Alt + Shift
13	Meta + Ctrl
14	Meta + Ctrl + Shift
15	Meta + Ctrl + Alt
16	Meta + Ctrl + Alt + Shift
_
.TE
For example, shift-F5 would be sent as
\*(Cs\*1\*5\*;\*2\*~
.LP
If the \fBalwaysUseMods\fP resource is set, the Meta modifier also is
recognized, making parameters 9 through 16.
.LP
The codes used for the \fIPC-style function keys\fP were inspired
by a feature of the VT510, referred to in its reference manual as DECFNK.
In the DECFNK scheme, codes 2-8 identify modifiers for function-keys
and cursor-, editing-keypad keys.
Unlike \fI\*(xt\fP, the VT510 limits the modifiers which can be used
with cursor- and editing-keypad keys.
Although the name \*(``DECFNK\*('' implies that it is a mode,
the VT510 manual mentions it only as a feature,
which (like \fI\*(xt\fP) interacts with the DECUDK feature.
Unlike \fI\*(xt\fP, VT510/VT520 provide an extension to DECUDK
(DECPFK and DECPAK)
which apparently was the reason for the feature in those terminals,
i.e., for identifying a programmable key
rather than making it simple for applications to obtain modifier information.
It is not described in the related VT520 manual.
Neither manual was readily available
at the time the feature was added to \fI\*(xt\fP.
.LP
On the other hand, the VT510 and VT520 reference manuals
do document a related feature.
That is its emulation of the SCO console,
which is similar to the \*(``xterm-sco\*('' terminal description.
The SCO console function-keys are less useful to
applications developers than the approach used by \fI\*(xt\fP because
.bP
the relationship between modifiers and the characters sent by function-keys
is not readily apparent, and
.bP
the scheme is not extensible, i.e., it is an \fIad hoc\fP
assignment limited to two modifiers (\fIshift\fP and \fIcontrol\fP).
.Sh "VT220-Style Function Keys"
.LP
However, \fI\*(xt\fP is most useful as a DEC VT102 or VT220 emulator.
Set the \fBsunKeyboard\fP resource to true to force a Sun/PC keyboard
to act like a VT220 keyboard.
.LP
The VT102/VT220 application keypad transmits unique escape sequences in
application mode, which are distinct from the cursor and scrolling keypad:
.TS H
center;
lf3w(2c)	lf3w(2c)	lf3w(2c)	lf3w(2c) .
Key	Numeric	Application	VT100?
_
.TH
.T&
l | l | l | l .
Space	\*(Sp	\*(S3\*(Sp	no
Tab	\*(Ta	\*(S3\*I	no
Enter	\*(Cr	\*(S3\*M	yes
PF1	\*(S3\*P	\*(S3\*P	yes
PF2	\*(S3\*Q	\*(S3\*Q	yes
PF3	\*(S3\*R	\*(S3\*R	yes
PF4	\*(S3\*S	\*(S3\*S	yes
* \f1(multiply)\fP	\**	\*(S3\*j	no
+ \f1(add)\fP	\*+	\*(S3\*k	no
, \f1(comma)\fP	\*,	\*(S3\*l	yes
- \f1(minus)\fP	\*-	\*(S3\*m	yes
\&. \f1(period)\fP	\*.	\*(S3\*n	yes
/ \f1(divide)\fP	\*/	\*(S3\*o	no
0	\*0	\*(S3\*p	yes
1	\*1	\*(S3\*q	yes
2	\*2	\*(S3\*r	yes
3	\*3	\*(S3\*(cs	yes
4	\*4	\*(S3\*t	yes
5	\*5	\*(S3\*u	yes
6	\*6	\*(S3\*v	yes
7	\*7	\*(S3\*w	yes
8	\*8	\*(S3\*x	yes
9	\*9	\*(S3\*y	yes
= (equal)	\*=	\*(S3\*(XX	no
_
.TE
.LP
The VT100/VT220 keypad did not have all of those keys.
They were implemented in \fI\*(xt\fP in X11R1 (1987),
defining a mapping of all X11 keys which might be provided on a keypad.
For instance, a Sun4/II type-4 keyboard provided 
\*(``=\*('' (equal),
\*(``/\*('' (divide), and
\*(``*\*('' (multiply).
.LP
While the VT420 provided the same keypad,
the VT520 used a PC-keyboard.
Because that keyboard's keypad lacks the \*(``,\*('' (comma),
it was not possible to use EDT's delete-character function with the keypad.
\fI\*(XT\fP solves that problem for the VT220-keyboard configuration
by mapping
.sP
  \fICtrl\fP \*+ to \*, and
  \fICtrl\fP \*- to \*-
.LP
The VT220 provides a 6-key editing keypad,
which is analogous to that on the PC keyboard.
It is not affected by DECCKM or DECKPNM/DECKPAM:
.TS H
center;
lf3w(2c)	lf3w(2c)	lf3w(2c) .
Key	Normal	Application
_
.TH
.T&
l | l | l .
\f1Insert\fP	\*(Cs\*2\*~	\*(Cs\*2\*~
\f1Delete\fP	\*(Cs\*3\*~	\*(Cs\*3\*~
\f1Home\fP	\*(Cs\*1\*~	\*(Cs\*1\*~
\f1End\fP	\*(Cs\*4\*~	\*(Cs\*4\*~
\f1PageUp\fP	\*(Cs\*5\*~	\*(Cs\*5\*~
\f1PageDown\fP	\*(Cs\*6\*~	\*(Cs\*6\*~
_
.TE
.LP
The VT220 provides 8 additional function keys.
With a Sun/PC keyboard, access these keys by Control/F1 for F13, etc.
.TS H
center;
lf3w(2c)	lf3w(2c) .
Key	Escape Sequence
_
.TH
.T&
l | l .
F13	\*(Cs\*2\*5\*~
F14	\*(Cs\*2\*6\*~
F15	\*(Cs\*2\*8\*~
F16	\*(Cs\*2\*9\*~
F17	\*(Cs\*3\*1\*~
F18	\*(Cs\*3\*2\*~
F19	\*(Cs\*3\*3\*~
F20	\*(Cs\*3\*4\*~
_
.TE
.Sh "VT52-Style Function Keys"
.LP
A VT52 does not have function keys,
but it does have a numeric keypad and cursor keys.
They differ from the other emulations by the prefix.
Also, the cursor keys do not change:
.TS H
center;
lf3w(2c)	lf3w(2c) .
Key	Normal/Application
_
.TH
.T&
l | l .
Cursor Up	\*(Es\*A
Cursor Down	\*(Es\*(cB
Cursor Right	\*(Es\*C
Cursor Left	\*(Es\*D
_
.TE
The keypad is similar:
.TS H
center;
lf3w(2c)	lf3w(2c)	lf3w(2c)	lf3w(2c) .
Key	Numeric	Application	VT52?
_
.TH
.T&
l | l | l | l .
Space	\*(Sp	\*(Es\*?\*(Sp	no
Tab	\*(Ta	\*(Es\*?\*I	no
Enter	\*(Cr	\*(Es\*?\*M	no
PF1	\*(Es\*P	\*(Es\*P	yes
PF2	\*(Es\*Q	\*(Es\*Q	yes
PF3	\*(Es\*R	\*(Es\*R	yes
PF4	\*(Es\*S	\*(Es\*S	no
* \f1(multiply)\fP	\**	\*(Es\*?\*j	no
+ \f1(add)\fP	\*+	\*(Es\*?\*k	no
, \f1(comma)\fP	\*,	\*(Es\*?\*l	no
- \f1(minus)\fP	\*-	\*(Es\*?\*m	no
\&. \f1(period)\fP	\*.	\*(Es\*?\*n	yes
/ \f1(divide)\fP	\*/	\*(Es\*?\*o	no
0	\*0	\*(Es\*?\*p	yes
1	\*1	\*(Es\*?\*q	yes
2	\*2	\*(Es\*?\*r	yes
3	\*3	\*(Es\*?\*(cs	yes
4	\*4	\*(Es\*?\*t	yes
5	\*5	\*(Es\*?\*u	yes
6	\*6	\*(Es\*?\*v	yes
7	\*7	\*(Es\*?\*w	yes
8	\*8	\*(Es\*?\*x	yes
9	\*9	\*(Es\*?\*y	yes
= (equal)	\*=	\*(Es\*?\*(XX	no
_
.TE
.Sh "Sun-Style Function Keys"
.LP
The \fI\*(xt\fP program provides support for Sun keyboards more directly, by
a menu toggle that causes it to send Sun-style function key codes
rather than VT220.
Note, however, that the \fIsun\fP and \fIVT100\fP emulations are not really
compatible.
For example, their wrap-margin behavior differs.
.LP
Only function keys are altered; keypad and cursor keys are the same.
The emulation responds identically.
See the xterm-sun terminfo entry for details.
.Sh "HP-Style Function Keys"
.LP
Similarly, \fI\*(xt\fP can be compiled to support HP keyboards.
See the xterm-hp terminfo entry for details.
.Sh "The Alternate Screen Buffer"
.LP
\fI\*(XT\fP maintains two screen buffers.
The Normal Screen Buffer allows you to scroll back to view saved lines
of output up to the maximum set by the \fBsaveLines\fP resource.
The \fIAlternate Screen Buffer\fP is exactly as large as the display,
contains no additional saved lines.
When the \fIAlternate Screen Buffer\fP is active,
you cannot scroll back to view saved lines.
\fI\*(XT\fP provides control sequences and menu entries
for switching between the two.
.LP
Most full-screen applications use terminfo or termcap to obtain
strings used to start/stop full-screen mode,
i.e., \fIsmcup\fP and \fIrmcup\fP for terminfo,
or the corresponding \fIti\fP and \fIte\fP for termcap.
The \fBtiteInhibit\fP resource removes the \fIti\fP and \fIte\fP strings
from the TERMCAP string which is set in the environment for some platforms.
That is not done when \fI\*(xt\fP is built with terminfo libraries because
terminfo does not provide the whole text of the termcap data in one piece.
It would not work for terminfo anyway, since terminfo data is not passed
in environment variables;
setting an environment variable in this manner would have no effect on
the application's ability to switch
between \fINormal\fP and \fIAlternate Screen\fP buffers.
Instead, the newer private mode controls
(such as \*1\*0\*4\*9)
for switching between \fINormal\fP
and \fIAlternate Screen\fP buffers simply disable the switching.
They add other features such as clearing the display for the same reason:
to make the details of switching independent of the application that
requests the switch.
.
.Sh "Bracketed Paste Mode"
.LP
When bracketed paste mode is set,
pasted text is bracketed with control sequences
so that the program can differentiate pasted text from typed-in text.
When bracketed paste mode is set,
the program will receive:
   \*(Es\*([[\*2\*0\*0\*~,
.br
followed by the pasted text, followed by
   \*(Es\*([[\*2\*0\*1\*~.
.
.Sh "Title Modes"
.LP
The window- and icon-labels can be set or queried using control sequences.
As a VT220-emulator, \fI\*(xt\fP \*(``should\*('' limit
the character encoding for
the corresponding strings to ISO-8859-1.
Indeed, it used to be the case (and was documented) that
window titles had to be ISO-8859-1.
This is no longer the case.
However, there are many applications which still assume that titles are
set using ISO-8859-1.
So that is the default behavior.
.LP
If \fI\*(xt\fP is running with UTF-8 encoding,
it is possible to use window- and icon-labels encoded using UTF-8.
That is because the underlying X libraries (and many, but not all)
window managers support this feature.
.LP
The \fButf8Title\fP X resource setting tells \fI\*(xt\fP to disable
a reconversion of the title string back to ISO-8859-1,
allowing the title strings to be interpreted as UTF-8.
The same feature can be enabled using the title mode control sequence
described in this summary.
.LP
Separate from the ability to set the titles,
\fI\*(xt\fP provides the ability to query the titles,
returning them either in ISO-8859-1 or UTF-8.
This choice is available only while \fI\*(xt\fP is using UTF-8 encoding.
.LP
Finally, the characters sent to, or returned by a title control
are less constrained than the rest of the control sequences.
To make them more manageable (and constrained), for use in shell scripts,
\fI\*(xt\fP has an optional feature which decodes the string from hexadecimal
(for setting titles) or for encoding the title into hexadecimal when querying
the value.
.
.Sh "Mouse Tracking"
.LP
The VT widget can be set to send the mouse position and other
information on button presses.
These modes are typically used by
editors and other full-screen applications that want to make use of
the mouse.
.LP
There are two sets of mutually exclusive modes:
.bP
mouse protocol
.bP
protocol encoding
.LP
The mouse protocols include
DEC Locator mode, enabled by the
DECELR \*(Cs\*(Ps\*s\*;\*(Ps\*s\*(qu\*s\*z
control sequence, and is not described here
(control sequences are summarized above).
The remaining five modes of the mouse protocols
are each enabled (or disabled) by a different parameter in
the
\*(``DECSET \*(Cs\*?\*(Pm\*s\*h\*(''
or
\*(``DECRST \*(Cs\*?\*(Pm\*s\*l\*(''
control sequence.
.LP
Manifest constants for the parameter values
are defined in \fIxcharmouse.h\fP as follows:
.
.ID
.ft CW
#define SET_X10_MOUSE               9
#define SET_VT200_MOUSE             1000
#define SET_VT200_HIGHLIGHT_MOUSE   1001
#define SET_BTN_EVENT_MOUSE         1002
#define SET_ANY_EVENT_MOUSE         1003
.sP
#define SET_FOCUS_EVENT_MOUSE       1004
.sP
#define SET_EXT_MODE_MOUSE          1005
#define SET_SGR_EXT_MODE_MOUSE      1006
#define SET_URXVT_EXT_MODE_MOUSE    1015
.sP
#define SET_ALTERNATE_SCROLL        1007
.ft 1
.DE
.br
The motion reporting modes are strictly \fI\*(xt\fP extensions, and are not
part of any standard, though they are analogous to the DEC VT200 DECELR
locator reports.
.LP
Normally,
parameters (such as pointer position and button number) for all mouse
tracking escape sequences generated by \fI\*(xt\fP
encode numeric parameters in a single character as
\fIvalue\fP+32.
For example, \*! specifies the value 1.
The upper left character position on the terminal is denoted as 1,1.
This scheme dates back to X10,
though the normal mouse-tracking (from X11) is more elaborate.
.Ss X10 compatibility mode
.LP
X10 compatibility mode sends an escape sequence only on button press,
encoding the location and the mouse button pressed.
It is enabled by specifying parameter 9 to DECSET.
On button press, \fI\*(xt\fP sends
\*(Cs\*M\*(Cb\*(Cx\*(Cy (6 characters).
.bP
\*(Cb is \fIbutton\fP\-1, where \fIbutton\fP is 1, 2 or 3.
.bP
\*(Cx and \*(Cy are the \fIx\fP and \fIy\fP coordinates of the mouse when the
button was pressed.
.Ss Normal tracking mode
.LP
Normal tracking mode sends an escape sequence on both button press and
release.
Modifier key (shift, ctrl, meta) information is also sent.
It is enabled by specifying parameter 1000 to DECSET.
On button press or release, \fI\*(xt\fP sends
\*(Cs\*M\*(Cb\*(Cx\*(Cy.
.bP
The low two bits of \*(Cb encode button information:
0=MB1 pressed, 1=MB2 pressed, 2=MB3 pressed, 3=release.
.bP
The next three bits encode the modifiers which were down when the button was
pressed and are added together:  4=Shift, 8=Meta, 16=Control.
Note however that the shift and control bits are normally unavailable
because \fI\*(xt\fP uses the control modifier with mouse for popup menus,
and the shift modifier is used in the default translations for button events.
The \fIMeta\fP modifier recognized by \fI\*(xt\fP is the \fImod1\fP mask,
and is not necessarily the \*(``Meta\*('' key (see \fBxmodmap\fP(1)).
.bP
\*(Cx and \*(Cy are the x and y coordinates of the mouse event, encoded as
in X10 mode.
.Ss Wheel mice
.LP
Wheel mice may return buttons 4 and 5.
Those buttons are represented by the same event codes
as buttons 1 and 2 respectively,
except that 64 is added to the event code.
Release events for the wheel buttons are not reported.
By default,
the wheel mouse events are translated to \fIscroll-back\fP
and \fIscroll-forw\fP actions.
Those actions normally scroll the whole window,
as if the scrollbar was used.
However if \fIAlternate Scroll\fP mode is set,
then cursor up/down controls are sent when the terminal is displaying
the \fIAlternate Screen Buffer\fP.
The initial state of \fIAlternate Scroll\fP mode is set
using the \fBalternateScroll\fP resource.
.Ss Other buttons
.LP
Additional buttons are encoded like the wheel mice,
.bP
by adding 64 (for buttons 6 and 7), or
.bP
by adding 128 (for buttons 8 through 11).
.LP
Past button 11, the encoding is ambiguous because the same code
may correspond to different button/modifier combinations.
It is not possible to use these buttons (6-11) in \fI\*(xt\fP's
\fBtranslations\fP resource because their names are not in the X Toolkit's
symbol table.
.Ss Highlight tracking
.LP
Mouse highlight tracking notifies a program of a button press, receives a
range of lines from the program, highlights the region covered by
the mouse within that range until button release, and then sends the
program the release coordinates.
It is enabled by specifying parameter 1001 to DECSET.
Highlighting is performed only for button 1, though other button events
can be received.
.sP
\fBWarning\fP:
this mode requires a cooperating program, else \fI\*(xt\fP will hang.
.LP
On button press, the same information as for normal tracking is
generated; \fI\*(xt\fP then waits for the program to send mouse
tracking information.
\fIAll X events are ignored until the proper escape sequence is\fP
\fIreceived from the pty:\fP
.br
\*(Cs\*(Ps\*s\*;\*(Ps\*s\*;\*(Ps\*s\*;\*(Ps\*s\*;\*(Ps\*s\*T
.LP
The parameters are \fIfunc, startx, starty, firstrow,\fP and \fIlastrow\fP:
.bP
\fIfunc\fP is non-zero to initiate highlight tracking and zero to abort.
.bP
\fIstartx\fP and \fIstarty\fP give the starting x and y location for
the highlighted region.
.bP
The ending location tracks the mouse, but
will never be above row \fIfirstrow\fP and will always be above row
\fIlastrow.\fP
(The top of the screen is row 1.)
.LP
When the button is released, \fI\*(xt\fP reports the ending position
one of two ways:
.bP
if the start and end coordinates are the same locations:
.sP
\*(Cs\*t\*(Cx\*(Cy
.bP
otherwise:
.sP
\*(Cs\*T\*(Cx\*(Cy\*(Cx\*(Cy\*(Cx\*(Cy
.LP
The parameters are \fIstartx, starty, endx, endy, mousex,\fP and \fImousey\fP:
.bP
\fIstartx, starty, endx, \fPand\fI endy\fP give the starting and
ending character positions of the region.
.bP
\fImousex\fP and \fImousey\fP
give the location of the mouse at button up, which may not be over a
character.
.Ss Button-event tracking
.LP
Button-event tracking is essentially the same as normal tracking, but
\fI\*(xt\fP also reports button-motion events.
Motion events
are reported only if the mouse pointer has moved to a different character
cell.
It is enabled by specifying parameter 1002 to DECSET.
On button press or release, \fI\*(xt\fP sends the same codes used by normal
tracking mode.
.bP
On button-motion events, \fI\*(xt\fP adds 32 to the event code
(the third character, \*(Cb).
.bP
The other bits of the event code specify
button and modifier keys as in normal mode.
For example, motion into cell x,y with button 1
down is reported as
.sP
\*(Cs\*M\*@\*(Cx\*(Cy
.sP
(\ \*@ = 32 + 0 (button 1) + 32 (motion indicator)\ ).
Similarly, motion with button 3
down is reported as
.sP
\*(Cs\*M\*(cB\*(Cx\*(Cy
.sP
(\ \*(cB = 32 + 2 (button 3) + 32 (motion indicator)\ ).
.Ss Any-event tracking
.LP
Any-event mode is the same as button-event mode, except that all motion
events are reported, even if no mouse button is down.
It is enabled by specifying 1003 to DECSET.
.Ss FocusIn/FocusOut
.LP
FocusIn/FocusOut can be combined with any of the mouse events since
it uses a different protocol.
When set, it causes \fI\*(xt\fP to send
\*(Cs\*I when the terminal gains focus, and
\*(Cs\*O when it loses focus.
.Ss Extended coordinates
.LP
The original X10 mouse protocol limits the \*(Cx and \*(Cy ordinates
to 223 (=255\ -\ 32).
\fI\*(XT\fP supports more than one scheme for extending this range,
by changing the protocol encoding:
.IP "UTF-8 (1005)"
This enables UTF-8 encoding for \*(Cx and \*(Cy under
all tracking modes, expanding the maximum encodable position from 223 to 2015.
For positions less than 95, the resulting output is identical under both modes.
Under extended mouse mode, positions greater than 95
generate \*(``extra\*('' bytes which will confuse
applications which do not treat their input as a UTF-8 stream.
Likewise, \*(Cb will be UTF-8 encoded,
to reduce confusion with wheel mouse events.
.IP
Under normal mouse mode, positions outside (160,94) result in
byte pairs which can be interpreted as a single UTF-8 character;
applications
which do treat their input as UTF-8 will almost certainly be confused
unless extended mouse mode is active.
.IP
This scheme has the drawback that the encoded coordinates will not
pass through \fBluit\fP(1) unchanged,
e.g., for locales using non-UTF-8 encoding.
.IP "SGR (1006)"
The normal mouse response is altered to use
.RS
.bP
\*(Cs\*<
followed by semicolon-separated
.bP
encoded button value,
.bP
\*(Px and \*(Py ordinates and
.bP
a final character which
is \*M for button press
and \*m for button release.
.RE
.IP
The encoded button value in this case does not add 32 since
that was useful only in the X10 scheme for ensuring that the
byte containing the button value is a printable code.
.RS
.bP
The modifiers are encoded in the same way.
.bP
A different final character is used for button release
to resolve the X10 ambiguity regarding which button was released.
.RE
.IP
The highlight tracking responses are also modified to an SGR-like format,
using the same SGR-style scheme and button-encodings.
.IP "URXVT (1015)"
The normal mouse response is altered to use
.RS
.bP
\*(Cs
followed by semicolon-separated
.bP
encoded button value,
.bP
the \*(Px and \*(Py ordinates and final character \*M.
.RE
.IP
This uses the same button encoding as X10, but printing it as
a decimal integer rather than as a single byte.
.IP
However, \*(Cs\*M can be mistaken for DL (delete lines),
while the highlight tracking \*(Cs\*T can be mistaken for SD (scroll down),
and the Window manipulation controls.
For these reasons, the 1015 control is not recommended;
it is not an improvement over 1005.
.
.Sh "Sixel Graphics"
.LP
If \fI\*(xt\fP is configured as
VT240,
VT241,
VT330,
VT340 or
VT382
using the \fBdecTerminalID\fP resource,
it supports Sixel Graphics controls, a palleted bitmap graphics system
using sets of six vertical pixels as the basic element.
.St
.IP \\*(Cs\\*(Ps\\*s\\*c
Send Device Attributes (Primary DA), \fI\*(xt\fP.
\fI\*(xt\fP responds to Send Device Attributes (Primary DA) with these
additional codes:
  \*(Ps = \*4 \(-> Sixel graphics.
.iP
.IP \\*(Cs\\*?\\*(Pm\\*s\\*h
Set Mode, \fI\*(xt\fP.
\fI\*(xt\fP has these additional private Set Mode values:
  \*(Ps = \*8\*0 \(-> Sixel scrolling.
  \*(Ps = \*1\*0\*7\*0 \(-> use private color registers for each graphic.
  \*(Ps = \*8\*4\*5\*2 \(-> Sixel scrolling leaves cursor to right of graphic.
.iP
.IP \\*(Dc\\*(Pa\\*s\\*;\\*(Pb\\*s\\*;\\*(Ph\\*s\\*q\\*s\\*(Ps..\\*(Ps\fP\\*s\\*(ST
Send SIXEL image, DEC graphics terminals, \fI\*(xt\fP.
See:
.ID 3
\fIVT330/VT340 Programmer Reference Manual Volume 2:\fP
\fIGraphics Programming\fP
.\" https://vt100.net/docs/vt3xx-gp/chapter14.html
\fIChapter 14 Graphics Programming\fP
.DE
The sixel data device control string has three positional parameters,
following the \*q with sixel data.
  \*(Pa \(-> pixel aspect ratio
  \*(Pb \(-> background color option
  \*(Ph \(-> horizontal grid size (ignored).
  \*(Ps \(-> sixel data
.Ed
.
.Sh "ReGIS Graphics"
.LP
If \fI\*(xt\fP is configured as
VT125,
VT240,
VT241,
VT330 or
VT340
using the \fBdecTerminalID\fP resource,
it supports Remote Graphic Instruction Set, a graphics description language.
.St
.IP \\*(Cs\\*(Ps\\*s\\*c
Send Device Attributes (Primary DA), DEC graphics terminals, \fI\*(xt\fP.
\fI\*(xt\fP responds to Send Device Attributes (Primary DA) with these
additional codes:
  \*(Ps = \*3 \(-> ReGIS graphics.
.iP
.IP \\*(Cs\\*?\\*(Pm\\*s\\*h
Set Mode, \fI\*(xt\fP.
\fI\*(xt\fP has these additional private Set Mode values:
  \*(Ps = \*1\*0\*7\*0 \(-> use private color registers for each graphic.
.iP
.IP \\*(Dc\\*(Pm\\*s\\*p\\*(Pr..\\*(Pr\fP\\*s\\*(ST
Enter or exit ReGIS, VT300, \fI\*(xt\fP.
See:
.ID 3
\fIVT330/VT340 Programmer Reference Manual Volume 2:\fP
\fIGraphics Programming\fP
.\" https://vt100.net/docs/vt3xx-gp/chapter1.html
\fIChapter 1 Introduction to ReGIS\fP
.DE
The ReGIS data device control string has one positional parameter
with four possible values:
  \*(Pm = 0 \(-> resume command, use fullscreen mode.
  \*(Pm = 1 \(-> start new command, use fullscreen mode.
  \*(Pm = 2 \(-> resume command, use command display mode.
  \*(Pm = 3 \(-> start new command, use command display mode.
.Ed
.
.Sh "Tektronix 4014 Mode"
.LP
Most of these sequences are standard Tektronix 4014 control sequences.
Graph mode supports the 12-bit addressing of the Tektronix 4014.
The major features missing are
the write-through and defocused modes.
This document does not describe the commands used in the various
Tektronix plotting modes but does describe the commands to switch modes.
.LP
Some of the sequences are specific to \fI\*(xt\fP.
The Tektronix emulation was added in X10R4 (1986).
The VT240, introduced two years earlier, also supported Tektronix 4010/4014.
Unlike \fI\*(xt\fP, the VT240 documentation implies
(there is an obvious error in
section 6.9 \*(``Entering and Exiting 4010/4014 Mode\*('')
that exiting back to ANSI mode is done by
resetting private mode \*3\*8 (DECTEK)
rather than \*(Es\*(Et.
A real Tektronix 4014 would not respond to either.
.St
.IP \\*(Be
Bell (Ctrl-G).
.iP
.IP \\*(Bs
Backspace (Ctrl-H).
.iP
.IP \\*(Ta
Horizontal Tab (Ctrl-I).
.iP
.IP \\*(Lf
Line Feed or New Line (Ctrl-J).
.iP
.IP \\*(Vt
Cursor up (Ctrl-K).
.iP
.IP \\*(Ff
Form Feed or New Page (Ctrl-L).
.iP
.IP \\*(Cr
Carriage Return (Ctrl-M).
.iP
.IP \\*(Es\\*(Et
Switch to VT100 Mode (\*(Es Ctrl-C).
.iP
.IP \\*(Es\\*(En
Return Terminal Status (\*(Es Ctrl-E).
.iP
.IP \\*(Es\\*(Ff
PAGE (Clear Screen) (\*(Es Ctrl-L).
.iP
.IP \\*(Es\\*(So
Begin 4015 APL mode (\*(Es Ctrl-N).
This is ignored by \fI\*(xt\fP.
.iP
.IP \\*(Es\\*(Si
End 4015 APL mode (\*(Es Ctrl-O).
This is ignored by \fI\*(xt\fP.
.iP
.IP \\*(Es\\*(Eb
COPY (Save Tektronix Codes to file COPY\fIyyyy-mm-dd.hh:mm:ss\fP).
  \*(Eb (end transmission block) is the same as Ctrl-W.
.iP
.IP \\*(Es\\*(Ca
Bypass Condition (\*(Es Ctrl-X).
.iP
.IP \\*(Es\\*(Su
GIN mode (\*(Es Ctrl-Z).
.iP
.IP \\*(Es\\*(Fs
Special Point Plot Mode (\*(Es Ctrl-\e).
.iP
.IP \\*(Es\\*8
Select Large Character Set.
.iP
.IP \\*(Es\\*9
Select #2 Character Set.
.iP
.IP \\*(Es\\*:
Select #3 Character Set.
.iP
.IP \\*(Es\\*;
Select Small Character Set.
.iP
.IP \\*(Os\\*(Ps\\*s\\*;\\*(Pt\\*s\\*(Be
Set Text Parameters of VT window.
  \*(Ps = \*0 \(-> Change Icon Name and Window Title to \*(Pt.
  \*(Ps = \*1 \(-> Change Icon Name to \*(Pt.
  \*(Ps = \*2 \(-> Change Window Title to \*(Pt.
  \*(Ps = \*4\*6 \(-> Change Log File to \*(Pt.
This is normally disabled by a compile-time option.
.iP
.IP \\*(Es\\*`
Normal Z Axis and Normal (solid) Vectors.
.iP
.IP \\*(Es\\*a
Normal Z Axis and Dotted Line Vectors.
.iP
.IP \\*(Es\\*b
Normal Z Axis and Dot-Dashed Vectors.
.iP
.IP \\*(Es\\*c
Normal Z Axis and Short-Dashed Vectors.
.iP
.IP \\*(Es\\*d
Normal Z Axis and Long-Dashed Vectors.
.iP
.IP \\*(Es\\*h
Defocused Z Axis and Normal (solid) Vectors.
.iP
.IP \\*(Es\\*i
Defocused Z Axis and Dotted Line Vectors.
.iP
.IP \\*(Es\\*j
Defocused Z Axis and Dot-Dashed Vectors.
.iP
.IP \\*(Es\\*k
Defocused Z Axis and Short-Dashed Vectors.
.iP
.IP \\*(Es\\*l
Defocused Z Axis and Long-Dashed Vectors.
.iP
.IP \\*(Es\\*p
Write-Thru Mode and Normal (solid) Vectors.
.iP
.IP \\*(Es\\*q
Write-Thru Mode and Dotted Line Vectors.
.iP
.IP \\*(Es\\*r
Write-Thru Mode and Dot-Dashed Vectors.
.iP
.IP \\*(Es\\*(cs
Write-Thru Mode and Short-Dashed Vectors.
.iP
.IP \\*(Es\\*t
Write-Thru Mode and Long-Dashed Vectors.
.iP
.IP \\*(Fs
Point Plot Mode (Ctrl-\e).
.iP
.IP \\*(Gs
Graph Mode (Ctrl-]).
.iP
.IP \\*(Rs
Incremental Plot Mode (Ctrl-\*^).
.iP
.IP \\*(Us
Alpha Mode (Ctrl-_).
.Ed
.
.
.Sh "VT52 Mode"
.LP
Parameters for cursor movement are at the end of the \*(Es\*Y escape sequence.
Each ordinate is encoded in a single character as \fIvalue\fP+32.
For example, \*! is 1.
The screen coordinate system is 0-based.
.St
.IP \\*(Es\\*<
Exit VT52 mode (Enter VT100 mode).
.iP
.IP \\*(Es\\*=
Enter alternate keypad mode.
.iP
.IP \\*(Es\\*>
Exit alternate keypad mode.
.iP
.IP \\*(Es\\*A
Cursor up.
.iP
.IP \\*(Es\\*(cB
Cursor down.
.iP
.IP \\*(Es\\*C
Cursor right.
.iP
.IP \\*(Es\\*D
Cursor left.
.iP
.IP \\*(Es\\*F
Enter graphics mode.
.iP
.IP \\*(Es\\*G
Exit graphics mode.
.iP
.IP \\*(Es\\*H
Move the cursor to the home position.
.iP
.IP \\*(Es\\*I
Reverse line feed.
.iP
.IP \\*(Es\\*J
Erase from the cursor to the end of the screen.
.iP
.IP \\*(Es\\*K
Erase from the cursor to the end of the line.
.iP
.IP \\*(Es\\*Y\\*(Ps\\*s\\*(Ps
Move the cursor to given row and column.
.iP
.IP \\*(Es\\*Z
Identify.
  \(-> \*(Es\*s\*/\*s\*Z (\*(``I am a VT52.\*('').
.Ed
.
.Sh "Further reading"
.Ss "Technical manuals"
.LP
Manuals for \fIhardware\fP terminals are more readily available than
similarly-detailed documentation for terminal \fIemulators\fP
such as \fIaixterm\fP, \fIshelltool\fP, \fIdtterm\fP.
.LP
However long, the technical manuals have problems:
.bP
DEC's manuals did not provide a comprehensive comparison of the features
in different model.
.IP
Peter Sichel's \fIHost Interface Functions Checklist\fP
spreadsheet is useful for noting
which model introduced a given feature (although there are a few apparent
errors such as the DECRQSS feature cited for VT320
whereas the technical manual omits it).
.bP
Sometimes the manuals disagree.
For example, DEC's standard document (DEC STD 070) for terminals says
that DECSCL performs a \fIsoft\fP reset (DECSTR),
while the VT420 manual says it does a \fIhard\fP reset (RIS).
.bP
Sometimes the manuals are simply incorrect.
For example, testing a DEC VT420 in 1996 showed that the documented
code for a valid or invalid response to DECRQSS was reversed.
.IP
The VT420 test results were incorporated into \fIvttest\fP program.
At the time, DEC STD 070 was not available,
but it also agrees with \fIvttest\fP.
Later, documentation for the DEC VT525 was shown to have the same flaw.
.bP
Not all details are clear even in DEC STD 070
(which is more than twice the length of the
VT520 programmer's reference manual,
and almost three times longer than the VT420 reference manual).
However, as an internal standards document,
DEC STD 070 is more likely to describe the actual behavior of DEC's terminals
than the more polished user's guides.
.LP
That said, here are technical manuals
which have been used in developing \fI\*(xt\fP.
Not all were available initially.
In August 1996 for instance, the technical references were
limited to
EK-VT220-HR-002 and
EK-VT420-UG.002.
Shortly after,
Richard Shuford sent a copy of
EK-VT3XX-TP-001.
Still later (beginning in 2003), Paul Williams' vt100.net site
provided
EK-VT102-UG-003,
EK-VT220-RM-002,
EK-VT420-RM-002,
EK-VT520-RM A01,
EK-VT100-TM-003, and
EK-VT102-UG-003.
The remaining documents were found on the bitsavers site.
.bP
.\" http://www.bitsavers.org/pdf/dec/terminal/vt52/EK-VT5X-OP-001_DECscope_Users_Manual_Mar77.pdf
\fIDECscope User's Manual\fP.
.br
Digital Equipment Corporation
(EK-VT5X-OP-001 1975).
.bP
.\" http://www.bitsavers.org/pdf/dec/terminal/vt100/EK-VT100-TM-003_VT100_Technical_Manual_Jul82.pdf
\fIVT100 Series Video Terminal Technical Manual\fP.
.br
Digital Equipment Corporation
(EK-VT100-TM-003, July 1982).
.bP
.\" https://vt100.net/docs/vt100-ug/
\fIVT100 User Guide\fP.
.br
Digital Equipment Corporation
(EK-VT100-UG-003, June 1981).
.bP
.\" https://vt100.net/docs/vt102-ug/
\fIVT102 User Guide\fP.
.br
Digital Equipment Corporation
(EK-VT102-UG-003, February 1982).
.bP
.\" http://manx-docs.org/details.php/1,2954
\fIVT220 Programmer Pocket Guide\fP.
.br
Digital Equipment Corporation
(EK-VT220-HR-002, July 1984).
.bP
.\" https://vt100.net/docs/vt220-rm/
\fIVT220 Programmer Reference Manual\fP.
.br
Digital Equipment Corporation
(EK-VT220-RM-002, August 1984).
.bP
.\" http://www.bitsavers.org/pdf/dec/terminal/vt240/EK-VT240-RM-002_VT240_Programmer_Reference_Manual_Oct84.pdf
\fIVT240 Programmer Reference Manual\fP.
.br
Digital Equipment Corporation
(EK-VT240-RM-002, October 1984).
.bP
.\" http://www.bitsavers.org/pdf/dec/terminal/vt340/EK-VT3XX-TP-001_VT330_VT340_Text_Programming_Mar87.pdf
\fIVT330/VT340 Programmer Reference Manual\fP
.br
\fIVolume 1: Text Programming\fP.
.br
Digital Equipment Corporation
(EK-VT3XX-TP-001, March 1987).
.bP
.\" https://vt100.net/docs/vt3xx-gp/
\fIVT330/VT340 Programmer Reference Manual\fP
.br
\fIVolume 2: Graphics Programming\fP.
.br
Digital Equipment Corporation
(EK-VT3XX-GP-001, March 1987).
.bP
.\" http://www.bitsavers.org/pdf/dec/terminal/vt420/EK-VT420-UG-001_Installing_and_Using_The_VT420_Video_Terminal_Nov89.pdf
\fIInstalling and Using\fP
.br
\fIThe VT420 Video Terminal\fP
.br
\fI(North American Model)\fP.
.br
Digital Equipment Corporation
(EK-VT420-UG.002, February 1990).
.bP
.\" http://manx-docs.org/collections/mds-199909/cd3/term/vt420rm2.pdf
\fIVT420 Programmer Reference Manual\fP.
.br
Digital Equipment Corporation
(EK-VT420-RM-002, February 1992).
.bP
.\" https://vt100.net/docs/vt510-rm/
\fIVT510 Video Terminal\fP
.br
\fIProgrammer Information\fP.
.br
Digital Equipment Corporation
(EK-VT510-RM B01, November 1993).
.bP
.\" http://www.bitsavers.org/pdf/dec/terminal/vt5xx/EK-VT520-RM_VT520_VT525_Programmer_Information_Jul94.pdf
\fIVT520/VT525 Video Terminal\fP
.br
\fIProgrammer Information\fP.
.br
Digital Equipment Corporation
(EK-VT520-RM A01, July 1994).
.bP
.\" http://www.vaxhaven.com/images/f/f7/EK-PPLV2-PM-B01.pdf
\fIDigital ANSI-Compliant Printing Protocol\fP
.br
\fILevel 2 Programming Reference Manual\fP
.br
Digital Equipment Corporation
(EK-PPLV2-PM B01, August 1994).
.bP
.\" https://vt100.net/manx/details/5,5479
\fI4014 and 4014-1 Computer Display Terminal\fP
.br
\fIUser's Manual\fP.
.br
Tektronix, Inc.
(070-1647-00, November 1979).
.Ss "Standards"
.LP
The DEC terminal family (VT100 through VT525) is upward-compatible,
using standards plus \fIextensions\fP, e.g., \*(``private modes\*(''.
Not all commonly-used features are standard.
For example, scrolling regions are not found in ECMA-48.
.LP
Again, it is possible to find discrepancies in the standards:
.bP
The printed ECMA-48 5th edition (1991)
and the first PDF produced for that edition (April 1998)
state that SD (scroll down) ends with 05/14, i.e., \*^,
which disagrees with DEC's VT420 hardware implementation and
DEC's manuals which use 05/04 \*T.
(A few other terminals such as AT&T 5620 and IBM 5151 also used 05/04,
but the documentation and dates are lacking).
.IP
ECMA created a new PDF in April 2003 which changed that detail to use \*T,
and later in 2008 provided PDFs of the earlier editions which used \*T.
.bP
The VT320, VT420, VT520 manuals claim that DECSCL does a
hard reset (RIS).
.IP
Both the VT220 manual and DEC STD 070 (which documents
levels 1-4 in detail) state that it is a soft reset, e.g., DECSTR.
.LP
Here are the relevant standards:
.bP
.\" https://www.ecma-international.org/publications/standards/Ecma-035.htm
\fIECMA-35: Character Code Structure and Extension Techniques\fP
.br
(6th Edition, December 1994).
.bP
.\" http://www.ecma-international.org/publications/files/ECMA-ST/Ecma-043.pdf
\fIECMA-43: 8-bit Coded Character Set Structure and Rules\fP
.br
(3rd Edition, December 1991).
.\" same as dpANS X3.134.1
.bP
.\" https://www.ecma-international.org/publications/standards/Ecma-048.htm
\fIECMA-48: Control Functions for Coded Character Sets\fP
.br
(5th Edition, June 1991).
.bP
.\" http://www.bitsavers.org/pdf/dec/standards/EL-SM070-00_DEC_STD_070_Video_Systems_Reference_Manual_Dec91.pdf
\fIDEC STD 070 Video Systems Reference Manual\fP.
.br
Digital Equipment Corporation
(A-MN-ELSM070-00-0000 Rev H, December 3, 1991).
.Ss "Miscellaneous"
.LP
A few hardware terminals survived into the 1990s only as terminal emulators.
Documentation for these and other terminal emulators
which have influenced \fI\*(xt\fP
are generally available only in
less-accessible and less-detailed manual pages.
.bP
\fI\*(XT\fP supports control sequences for manipulating its \fIwindow\fP
which were implemented by Sun's \fIshelltool\fP program.
This was part of SunView (SunOS 3.0, 1986).
The change-notes for \fI\*(xt\fP's \fIresize\fP program in X10.4 (1986)
mention its use of these \*(``Sun tty emulation escape sequences\*(''
for resizing the window.
The X10.4 \fI\*(xt\fP program recognized these sequences for resizing
the terminal, except for the iconfig/deiconfy pair.
SunView also introduced the SIGWINCH signal,
used by the X10.4 \fI\*(xt\fP and mentioned in its \fICHANGES\fP file:
.iP
.RS
The window size is passed to the operating system via TIOCSWINSZ (4.3) or
TIOCSSIZE (sun).
A SIGWINCH signal is sent if the vtXXX window is resized.
.RE
.IP
While support for the Sun control-sequences remained in \fIresize\fP,
the next release of \fI\*(xt\fP (X11R1 in 1987)
omitted the code for interpreting them.
.IP
Later, the SunView program was adapted for
the \fIOPEN LOOK\fP environment introduced 1988-1990.
.IP
Still later, in 1995, \fIOPEN LOOK\fP was abandoned in favor of \fICDE\fP.
The \fICDE\fP terminal emulator \fIdtterm\fP implemented those controls,
with a couple of additions.
.IP
Starting in July 1996,
\fI\*(xt\fP re-implemented those control sequences
(based on the \fIdtterm\fP manual pages)
and further extended the group of window controls.
.IP
There were two sets of controls
(\*(Cs\*(Ps[\*s\*;\*(Pm\*s\*;\*(Pm]\*t,
and \*(Os\*(Ps\fItext\fP\*(ST) implemented
by \fIshelltool\fP, documented in appendix E of both
\fIPHIGS Programming Manual\fP (1992), and the unpublished
\fIX Window System User's Guide (OPEN LOOK Edition)\fP (1995).
The \fICDE\fP program kept those,
and added a few new ones.
.TS
l l l l l
_ _ _ _ _
l c c c l.
\fBCode\fR	\fBSun\fR	\fBCDE	\*(XT	Description\fP
\*(Cs\*1\*t	yes	yes	yes	de-iconify
\*(Cs\*2\*t	yes	yes	yes	iconify
\*(Cs\*3\*t	yes	yes	yes	move window to pixel-position
\*(Cs\*4\*t	yes	yes	yes	resize window in pixels
\*(Cs\*5\*t	yes	yes	yes	raise window to front of stack
\*(Cs\*6\*t	yes	yes	yes	raise window to back of stack
\*(Cs\*7\*t	yes	yes	yes	refresh window
\*(Cs\*8\*t	yes	yes	yes	resize window in chars
\*(Cs\*9\*t	-	-	yes	maximize/unmaximize window
\*(Cs\*1\*0\*t	-	-	yes	to/from full-screen
\*(Cs\*1\*1\*t	yes	yes	yes	report if window is iconified
\*(Cs\*1\*2\*t	-	-	-	-
\*(Cs\*1\*3\*t	yes	yes	yes	report window position
\*(Cs\*1\*4\*t	yes	yes	yes	report window size in pixels
\*(Cs\*1\*5\*t	-	-	yes	report screen size in pixels
\*(Cs\*1\*6\*t	-	-	yes	report character cell in pixels
\*(Cs\*1\*7\*t	-	-	-	-
\*(Cs\*1\*8\*t	yes	yes	yes	report window size in chars
\*(Cs\*1\*9\*t	-	-	yes	report screen size in chars
\*(Cs\*2\*0\*t	-	yes	yes	report icon label
\*(Cs\*2\*1\*t	-	yes	yes	report window title
\*(Cs\*2\*2\*t	-	-	yes	save window/icon title
\*(Cs\*2\*3\*t	-	-	yes	restore window/icon title
\*(Cs\*2\*4\*t	-	-	yes	resize window (DECSLPP)
\*(Os\*0\*(ST	-	yes	yes	set window and icon title
\*(Os\*1\*(ST	-	yes	yes	set icon label
\*(Os\*2\*(ST	-	yes	yes	set window title
\*(Os\*3\*(ST	-	n/a	yes	set X server property
\*(Os\*I\*(ST	yes	yes	yes	set icon to file
\*(Os\*l\*(ST	yes	yes	yes	set window title
\*(Os\*L\*(ST	yes	yes	yes	set icon label
.TE
.br
Besides the Sun-derived OSC controls for setting window title and icon label,
\fIdtterm\fP also supported the \fI\*(xt\fP controls for the same feature.
.IP
The \fICDE\fP source was unavailable for inspection until 2012,
so that clarification of the details of the window operations
relied upon \fIvttest\fP.
.bP
The control sequences for saving/restoring the cursor and
for saving/restoring \*(``DEC Private Mode Values\*('' may appear to be
related (since the \*(``save\*('' controls both end with \*(cs),
but that is coincidental.
The latter was introduced in X10.4:
.iP
.RS
.ft C
.na
.hy 0
Most Dec Private mode settings can be save away internally using \\E[?ns,
where n is the same number to set or reset the Dec Private mode.  The
mode can be restored using \\E[?nr.  This can be used in termcap for vi, for
example, to turn off saving of lines, but restore whatever the original
state was on exit.
.hy
.ad
.ft R
.RE
.IP
while the SCOSC/SCORC pair was added in 1995 by XFree86
(and documented long afterwards).
.bP
The \fIaixterm\fP manual page gives the format of the control sequence
for foreground and background colors 8-15,
but does not specify what those colors are.
That is implied by the description's mention of \fIHFT\fP:
.iP
.RS
.na
.hy 0
.ft C
The aixterm command provides a standard terminal type for programs that
do not interact directly with Enhanced X-Windows.
This command provides
an emulation for a VT102 terminal or a high function terminal (HFT).
The VT102 mode is activated by the -v flag.
.ft R
.ad
.RE
.hy
.IP
Unlike \fI\*(xt\fP, there are no resource names for the 16 colors,
leaving the reader to assume that the mapping is hard-coded.
The control sequences for colors 8-15 are not specified by ECMA-48,
but rather (as done in other instances by \fI\*(xt\fP) chosen to
not conflict with current or future standards.
.
.if n .pl \n(nlu+1v
