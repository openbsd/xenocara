#!/bin/sh
# $XTermId: run-tic.sh,v 1.11 2019/05/12 00:44:47 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2006-2007,2019 by Thomas E. Dickey
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
# Run tic, either using ncurses' extension feature or filtering out harmless
# messages for the extensions which are otherwise ignored by other versions of
# tic.

MYTEMP=`mktemp -d 2>/dev/null`
if test -z "$MYTEMP"
then
	MYTEMP=${TMPDIR:-/tmp}/run-tic$$
fi
mkdir -p $MYTEMP || failed "cannot mkdir $MYTEMP"
trap "rm -rf $MYTEMP" EXIT INT QUIT HUP TERM

STDERR=$MYTEMP/run-tic$$.log
VER=`tic -V 2>/dev/null`
OPT=

TIC_PROG=tic
TPUT_PROG=tput
INFOCMP_PROG=infocmp
unset TERM
unset TERMINFO_DIRS

PASS1="$@"
PASS2="$@"

case "x$VER" in
*ncurses*)
	OPT="-x"
	# Prefer ncurses 6.1 over 6.0 over any 5, if we can get it, to support
	# large numbers (used in xterm-direct) and large entries (an issue with
	# xterm-nrc).
	case "$VER" in
	*\ [789].*|*\ 6.[123456789].*)
		;;
	*)
		# On systems with only ncurses 5, check for development version
		# of ncurses.
		VER=`infocmp6 -V 2>/dev/null`
		test -n "$VER" && INFOCMP_PROG=infocmp6
		VER=`tic6 -V 2>/dev/null`
		test -n "$VER" && TIC_PROG=tic6
		;;
	esac
	echo "** using tic from $VER"
	# If this is 6.1.20180127 or later and using ABI 6, then it supports
	# entries larger than 4096 bytes (up to 32768).
	case "$VER" in
	*\ [789].*|*\ 6.[123456789].*)
		expect="	cols#100000,"
		cat >$MYTEMP/fake.ti <<EOF
fake|test 32-bit numbers,
$expect
EOF
		TERMINFO=$MYTEMP $TIC_PROG $OPT $MYTEMP/fake.ti 2>/dev/null
		check=`TERMINFO=$MYTEMP TERM=fake $INFOCMP_PROG -1 fake 2>/dev/null |grep "$expect"`
		test "x$check" = "x$expect" || BIG=no
		;;
	*)
		BIG=no
		;;
	esac
	if test "$BIG" = no
	then
		# Trim out the SGR 1006 feature, to keep "xterm-nrc" smaller
		# than 4096 bytes.
		echo "...this version does not support large terminal descriptions"
		PASS2=$MYTEMP/input
		sed -e 's/use=xterm+sm+1006,//' -e '/^[	 ][	 ]*$/d' "$PASS1" >$PASS2
		set $PASS2
	fi
	;;
esac

echo "** $TIC_PROG $OPT" "$PASS1"
$TIC_PROG $OPT "$PASS2" 2>$STDERR
RET=$?

sed -e "s%$PASS2%$PASS1%" $STDERR | \
fgrep -v 'Unknown Capability' | \
fgrep -v 'Capability is not recognized:' | \
fgrep -v 'tic: Warning near line ' >&2
rm -f $STDERR

exit $RET
