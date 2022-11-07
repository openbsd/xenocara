#!/bin/sh
# $XTermId: tab0.sh,v 1.6 2022/04/25 22:49:46 tom Exp $
# -----------------------------------------------------------------------------
# Copyright 2019,2022 by Thomas E. Dickey
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
# Demonstrate hard-tabs.

: "${TABS:=tabs}"
: "${TPUT:=tput}"

show() {
	printf "Tabs %s:" "$1"
	read -r ignore
	p=0
	while [ "$p" -lt "$wide" ]
	do
		printf "%s+----%d" "----" "`expr 1 + \( "$p" / 10 \)`"
		p=`expr $p + 10`
	done
	printf '\n'
	p=1
	printf " "
	while [ "$p" -lt "$wide" ]
	do
		printf "%*s" "$1" "*"
		p=`expr "$p" + "$1"`
	done
	printf '\n'
	p=0
	while [ "$p" -lt "$wide" ]
	do
		printf '\t+'
		p=`expr "$p" + "$1"`
	done
	printf '\n'
	printf "...done"
	read -r ignore
}

# enable hard tabs, disable autowrap.
initialize() {
	"$TPUT" "$1"
	clear
	stty tabs
	printf '\033[?7l'
}

setup() {
	initialize reset
}

# Turn hard tabs off, reenable autowrap.
restore() {
	stty -tabs
	printf '\033[?7h'
}

wide=`$TPUT cols`

# If the terminal honors VT100 RIS, try that as a "hard reset" to get the
# power-on behavior.
for name in rs1 rs2
do
	value=`$TPUT $name | sed -e 's//ESC:/g'`
	case "$value" in
	*ESC:c*)
		;;
	*)
		value=
		;;
	esac
	if [ -n "$value" ]
	then
		initialize $name
		printf 'Testing after tput %s\r\n' "$name"
		show	8
		break
	fi
done

# The following tests use the normal "reset" behavior.
setup
$TABS	-8
show	8
$TABS	-4
show	4

# Some terminal emulators are known to be buggy, and "reset" does not get them
# to reset the tab-stops.
setup
show	8
restore
