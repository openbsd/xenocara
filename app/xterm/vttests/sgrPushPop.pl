#!/usr/bin/env perl
# $XTermId: sgrPushPop.pl,v 1.10 2018/08/02 21:09:46 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm, contributed by Dan Thompson
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

use strict;
use warnings;

# This script demonstrates the utility of the [non-standard] SGR push/pop
# control sequences. The ability to save (on a stack) the current SGR
# attributes (fg/bg color, bold, etc.) and then later restore them allows
# [SGR-containing] text from independent sources to be easily composed
# together, without requiring any sort of global coordination.

our (
    $pushSgr,     $popSgr,     $lib1Fmt,
    $lib2Fmt,     $redOnBlack, $blackOnYellow,
    $blueOnGreen, $bg,         $fg
);

$pushSgr = "\x1b[#{";
$popSgr  = "\x1b[#}";

# lib1Fmt represents a "top-level" program, and controls the current "ambient"
# fg/bg colors.
$lib1Fmt = "\x1b[48;5;%sm\x1b[38;5;%sm%03.3d/%03.3d ";

# lib2Fmt represents some intermediate library. Note that it contains no SGR
# control sequences at all.
$lib2Fmt = "Test stack: %s, %s, %s";

# The following represent individual bits of colorized data that come from
# other, "leaf-level" libraries.
$redOnBlack    = $pushSgr . "\x1b[1;31m\x1b[40m" . "redOnBlack" . $popSgr;
$blackOnYellow = $pushSgr . "\x1b[30m\x1b[4;43m" . "blackOnYellow" . $popSgr;
$blueOnGreen   = $pushSgr . "\x1b[34m\x1b[42m" . "blueOnGreen" . $popSgr;

printf $pushSgr;
printf "\x1b[40;37mSetting ambient colors to white-on-black\n";
printf $pushSgr;

for ( $bg = 0 ; $bg < 16 ; $bg++ ) {
    for ( $fg = 0 ; $fg < 16 ; $fg++ ) {
        printf $pushSgr;
        printf $lib1Fmt, $fg, $bg, $fg, $bg;
        printf $lib2Fmt, $redOnBlack, $blackOnYellow, $blueOnGreen;
        print " something else";
        printf $popSgr;    # keep the newline from bleeding color
        print "\n";
    }
    print "\n";
}
printf $popSgr;
printf "The ambient colors should still be white-on-black.\n";
printf $popSgr;
printf "Now we should be back to whatever it was before we got here.\n";

1;
