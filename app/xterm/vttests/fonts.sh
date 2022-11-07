#!/bin/sh
# $XTermId: fonts.sh,v 1.20 2022/04/25 08:19:38 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 1999-2021,2022 by Thomas E. Dickey
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
# Demonstrate control sequence which sets relative fonts.

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

exec </dev/tty
old=`stty -g`
stty raw -echo min 0  time 5

$CMD $OPT "${ESC}]50;?${SUF}" > /dev/tty
read -r original

stty $old
original="${original}${SUF}"

trap '$CMD $OPT "$original" >/dev/tty; exit 1' 1 2 3 15
trap '$CMD $OPT "$original" >/dev/tty' 0

F=1
D=1
T=6
while true
do
    $CMD $OPT "${ESC}]50;#$F${SUF}" >/dev/tty
    sleep 1
    if test .$D = .1 ; then
	test $F = $T && D=-1
    else
	test $F = 1 && D=1
    fi
    F=`expr $F + $D`
done
