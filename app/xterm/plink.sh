#!/bin/sh
# $XTermId: plink.sh,v 1.17 2022/03/13 18:27:29 Ryan.Schmidt Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2001-2021,2022 by Thomas E. Dickey
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
#
# Reduce the number of dynamic libraries used to link an executable.
LINKIT=
ASNEED=no
NO_LTO=

# gcc's link-time optimization is very slow - reduce usage of that.
case "$*" in
*-flto=*|-flto\ )
	NO_LTO=-fno-lto
	;;
esac

: "${TMPDIR=/tmp}"

while [ $# != 0 ]
do
	if [ $ASNEED = no ] && [ -n "$LINKIT" ]
	then
		ASNEED=yes
		OPT=-Wl,-as-needed
		warned=`mktemp "$TMPDIR/xterm.XXXXXXXX"`
		trap "rm -f $warned; exit 1" 1 2 3 15
		trap "rm -f $warned" 0
		if ( eval $LINKIT $OPT $NO_LTO "$@" >"$warned" 2>&1 )
		then
			WARNED=`cat "$warned"`
			rm -f "$warned"
			case ".$WARNED" in
			*Warning*|*nsupported*|*nrecognized*|*nknown*)
				;;
			*)
				LINKIT="$LINKIT $OPT $*"
				break
				;;
			esac
		else
			rm -f "$warned"
		fi
	fi

	OPT="$1"
	shift
	case $OPT in
	-k*)
		OPT=`echo "$OPT" | sed -e 's/^-k/-l/'`
		echo "always use $OPT (cannot test if needed)"
		LINKIT="$LINKIT $OPT"
		;;
	-l*)
		echo "testing if $OPT is needed"
		if ( eval $LINKIT $NO_LTO "$@" >/dev/null 2>&1 )
		then
			: echo ...no
		else
			echo ...yes
			LINKIT="$LINKIT $OPT"
		fi
		;;
	*)
		LINKIT="$LINKIT $OPT"
		;;
	esac
done
eval $LINKIT
