#!/bin/sh
# $XTermId: doublechars.sh,v 1.26 2022/04/25 23:25:41 tom Exp $
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
# Illustrate the use of double-size characters by drawing successive lines in
# the commonly used video attributes.
#
# Use the -w option to force the output to wrap.  It will look ugly, because
# the double-high lines will be split.

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

ITAL=no
SAVE=yes
WRAP=no
if test $# != 0 ; then
    while test $# != 0
    do
    	case $1 in
	-i)	ITAL=yes ;;
	-n)	SAVE=no ;;
	-w)	WRAP=yes ;;
	*)
		echo "usage: $0 [-i] [-n] [-w]"
		exit 1
	esac
	shift
    done
fi

if test $SAVE = yes ; then
    exec </dev/tty
    old=`stty -g`
    stty raw -echo min 0  time 5

    $CMD $OPT "${CSI}18t${SUF}" > /dev/tty
    IFS=';' read -r junk high wide

    stty $old

    wide=`echo "$wide"|sed -e 's/t.*//'`
    original=${CSI}8\;${high}\;${wide}t${SUF}

    trap '$CMD $OPT "$original" >/dev/tty; exit 1' 1 2 3 15
    trap '$CMD $OPT "$original" >/dev/tty' 0

fi

if test $WRAP = yes ; then
	# turn on wrapping and force the screen to 80 columns
	$CMD $OPT "${CSI}?7h" >/dev/tty
	$CMD $OPT "${CSI}?40l" >/dev/tty
else
	# force the screen to 132 columns
	$CMD $OPT "${CSI}?40h" >/dev/tty
	$CMD $OPT "${CSI}?3h" >/dev/tty
fi

for SGR in 0 1 4 5 7
do
	$CMD $OPT "${CSI}0;${SGR}m" >/dev/tty
	test "$ITAL" = yes && $CMD $OPT "${CSI}3m" >/dev/tty
	for DBL in 5 3 4 6 5
	do
		$CMD $OPT "${ESC}#${DBL}" >/dev/tty
		echo "The quick brown fox jumps over the lazy dog" >/dev/tty
	done
	echo
done
$CMD $OPT "${CSI}0m" >/dev/tty
