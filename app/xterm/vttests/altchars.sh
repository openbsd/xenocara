#!/bin/sh
# $XTermId: altchars.sh,v 1.4 2020/10/29 00:32:02 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2020 by Thomas E. Dickey
# 
#                         All Rights Reserved
# 
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
# 
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# 
# Except as contained in this notice, the name(s) of the above copyright
# holders shall not be used in advertising or otherwise to promote the
# sale, use or other dealings in this Software without prior written
# authorization.
# -----------------------------------------------------------------------------
echo
tput enacs
tput smacs
#define ACS_ULCORNER	NCURSES_ACS('l') /* upper left corner */
#define ACS_LLCORNER	NCURSES_ACS('m') /* lower left corner */
#define ACS_URCORNER	NCURSES_ACS('k') /* upper right corner */
#define ACS_LRCORNER	NCURSES_ACS('j') /* lower right corner */
#define ACS_LTEE	NCURSES_ACS('t') /* tee pointing right */
#define ACS_RTEE	NCURSES_ACS('u') /* tee pointing left */
#define ACS_BTEE	NCURSES_ACS('v') /* tee pointing up */
#define ACS_TTEE	NCURSES_ACS('w') /* tee pointing down */
#define ACS_HLINE	NCURSES_ACS('q') /* horizontal line */
#define ACS_VLINE	NCURSES_ACS('x') /* vertical line */
#define ACS_PLUS	NCURSES_ACS('n') /* large plus or crossover */
cat <<'EOF'
XlqqqqwqqqqwqqqqwqqqqkX
XxaaaaxaaaaxaaaaxaaaaxX
XtqqqqnqqqqnqqqqnqqqquX
XxaaaaxaaaaxaaaaxaaaaxX
XmqqqqvqqqqvqqqqvqqqqjX

X _`abcdefghijklmn X
X opqrstuvwxyz{|}~ X
X ................ X
EOF
tput rmacs
cat <<'EOF'
+----+----+----+----+
EOF
