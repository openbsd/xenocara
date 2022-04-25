#!/usr/bin/env perl
# $XTermId: acs.pl,v 1.3 2018/04/22 15:14:45 tom Exp $
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
# Assuming at least 24x80, print an array of the printable characters and their
# mapping to alternate character-set.  This also requires cursor-addressing.

use strict;
use warnings;
use diagnostics;

sub failed() {
    my $args = shift;
    printf STDERR "? %s\n", $args;
    exit 1;
}

sub need() {
    my $cap   = shift;
    my $check = `tput $cap 0 0`;
    chomp $check;
    &failed("missing capability: $cap") if ( $check eq "" );
}

&need("cup");
&need("clear");
&need("smacs");
&need("rmacs");

system("tput clear");
system("tput enacs");
my $smacs = `tput smacs`;
my $rmacs = `tput rmacs`;

for my $row ( 0 .. 15 ) {
    for my $col ( 0 .. 5 ) {
        my $ch = 32 + $row + $col * 16;
        last if ( $ch >= 127 );
        system( sprintf( "tput cup %d %d", $row, 4 + $col * 12 ) );
        my $xx = chr($ch);
        printf '%2x:%s{%s%s%s}', $ch, $xx, $smacs, $xx . $xx . $xx . $xx,
          $rmacs;
    }
}

printf "\n";

1;
