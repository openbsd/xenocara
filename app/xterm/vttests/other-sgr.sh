#!/bin/sh
# $XTermId: other-sgr.sh,v 1.1 2018/09/12 22:45:06 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2018 by Thomas E. Dickey
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
# Show non-VTxx SGRs combined with the conventional VT100/VT220 SGRs

ESC=""
CMD='/bin/echo'
OPT='-n'
SUF=''
TMP=/tmp/xterm$$
eval '$CMD $OPT >$TMP || echo fail >$TMP' 2>/dev/null
( test ! -f $TMP || test -s $TMP ) &&
for verb in printf print ; do
    rm -f $TMP
    eval '$verb "\c" >$TMP || echo fail >$TMP' 2>/dev/null
    if test -f $TMP ; then
	if test ! -s $TMP ; then
	    CMD="$verb"
	    OPT=
	    SUF='\c'
	    break
	fi
    fi
done
rm -f $TMP

if ( trap "echo exit" EXIT 2>/dev/null ) >/dev/null
then
    trap '$CMD $OPT "[0m"; exit' EXIT HUP INT TRAP TERM
else
    trap '$CMD $OPT "[0m"; exit' 0    1   2   5    15
fi

echo "[0m"
while true
do
    # blink(5) and conceal(8) are omitted because they are distracting, but the
    # case-statement handles those if the for-statement includes them.
    for GRP in 0 1 4 7
    do
	case $GRP in
	0) attr="normal  ";;
	1) attr="bold    ";;
	4) attr="under   ";;
	5) attr="blink   ";;
	7) attr="reverse ";;
	8) attr="conceal ";;
	esac
	for NUL in "0" "21"
	do
	    for ROW in $NUL "2" "3" "9" "2;3" "2;9" "3;9" "2;3;9"
	    do
		case $ROW in
		"0")     rlabel="normal  ";;
		"21")    rlabel="double  ";;
		"2")     rlabel="dim     ";;
		"3")     rlabel="italic  ";;
		"2;3")   rlabel="di/it   ";;
		"9")     rlabel="crossout";;
		"2;9")   rlabel="di/cr   ";;
		"3;9")   rlabel="it/cr   ";;
		"2;3;9") rlabel="di/it/cr";;
		*)       rlabel="UNKNOWN ";;
		esac
		# video attributes from the first two columns intentionally
		# "bleed through" to the other columns to help show some of
		# the possible combinations of attributes.
		$CMD $OPT "$GRP:[${GRP}m$attr"
		$CMD $OPT "[${ROW}m$rlabel"
		for COL in $NUL "3" "9" "2;3" "2;9" "3;9" "2;3;9"
		do
		    END=""
		    case $COL in
		    "0")     clabel="normal  ";;
		    "21")    clabel="double  "; END="[24m";;
		    "2")     clabel="dim     "; END="[22m";;
		    "3")     clabel="italic  "; END="[23m";;
		    "2;3")   clabel="di/it   "; END="[22;23m";;
		    "9")     clabel="crossout"; END="[29m";;
		    "2;9")   clabel="di/cr   "; END="[22;29m";;
		    "3;9")   clabel="it/cr   "; END="[23;29m";;
		    "2;3;9") clabel="di/it/cr"; END="[23;29m";;
		    *)       clabel="UNKNOWN ";;
		    esac
		    # The remaining columns (try to) reset their respective
		    # attribute, to make the result somewhat readable.
		    $CMD $OPT "[${COL}m$clabel${END}"
		done
		echo "[0m:$GRP"
	    done
	done
	[ -t 1 ] && sleep 3
    done
    [ -t 1 ] || break
done
