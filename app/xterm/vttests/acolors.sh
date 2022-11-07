#!/bin/sh
# $XTermId: acolors.sh,v 1.16 2022/04/25 08:19:38 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2002-2021,2022 by Thomas E. Dickey
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
# Demonstrate the use of the control sequence for changing ANSI colors.

ESC=""
CSI="${ESC}["
CMD='/bin/echo'
OPT='-n'
SUF=''
: "${TMPDIR=/tmp}"
TMP=`(mktemp "$TMPDIR/xterm.XXXXXXXX") 2>/dev/null` || TMP="$TMPDIR/xterm$$"
eval '$CMD $OPT >$TMP || echo fail >$TMP' 2>/dev/null
{ test ! -f "$TMP" || test -s "$TMP"; } &&
for verb in "printf" "print" ; do
    rm -f "$TMP"
    eval '$verb "\c" >$TMP || echo fail >$TMP' 2>/dev/null
    if test -f "$TMP" ; then
	if test ! -s "$TMP" ; then
	    CMD="$verb"
	    OPT=
	    SUF='\c'
	    break
	fi
    fi
done
rm -f "$TMP"

LIST="00 30 80 d0 ff"

exec </dev/tty
old=`stty -g`
stty raw -echo min 0  time 5

$CMD $OPT "${ESC}]4;4;?${SUF}" > /dev/tty
read -r original
stty $old
original=${original}${SUF}

trap '$CMD $OPT "$original" >/dev/tty; exit 1' 1 2 3 15
trap '$CMD $OPT "$original" >/dev/tty' 0

echo "${CSI}0;1;34mThis message is BLUE"
echo "${CSI}0;1;31mThis message is RED ${CSI}0;31m(sometimes)"
echo "${CSI}0;1;32mThis message is GREEN${CSI}0m"
while true
do
    for R in $LIST
    do
	for G in $LIST
	do
	    for B in $LIST
	    do
		# color "9" is bold-red
		test "$R" != 00 && test "$G" = 00 && test "$B" = 00 && $CMD $OPT "" >/dev/tty
		$CMD $OPT "${ESC}]4;9;rgb:$R/$G/$B${SUF}" >/dev/tty
		sleep 1
	    done
	done
    done
done
