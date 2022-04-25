#!/bin/sh
# $XTermId: bounce.sh,v 1.3 2020/01/12 22:51:16 tom Exp $
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
# iconify/deiconify, recording the window properties
LOG=1
logit() {
	sleep 2
	[ -n "$WINDOWID" ] && xprop -id $WINDOWID >bounce${LOG}.log
	LOG=`expr $LOG + 1`
}

[ -n "$WINDOWID" ] && rm -f bounce*.log

printf 'Hello ...\n'
logit

printf '\033[2t'
logit

printf '\033[1t'
logit

printf 'World!\n'
logit

[ -n "$WINDOWID" ] && ls -l bounce*.log
