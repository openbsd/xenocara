#!/usr/bin/env perl
# $XTermId: vt52chars.pl,v 1.1 2019/07/08 20:27:21 tom Exp $
# -----------------------------------------------------------------------------
# Copyright 2019 by Thomas E. Dickey
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
# show the vt52 graphic characters, annotatated in a table.

sub clear() {
    printf "\033H";    # home
    printf "\033J";    # erase
}

sub move($$) {
    my $y = shift;
    my $x = shift;
    printf "\033Y%c%c", $y + 32, $x + 32;
}

sub start_vt52() {
    printf "\033[?2l";
}

sub stop_vt52() {
    printf "\033<";
}

sub show_char() {
    my $value  = shift;
    my $string = shift;
    my $chr    = $value - 0140;
    return if ( $chr < 0 );    # not supported by xterm
    $value &= 0xff;
    my $y = 2 + int( $chr % 16 );
    my $x = 6 + int( $chr / 16 ) * 40;
    &move( $y, $x );
    printf "%03o \033F%s\033G %s", $value, chr($value), $string;
}

sub show_table() {
    &clear;
    &move( 0, 28 );
    printf "VT52 graphic characters";
    &show_char( 0140, "reserved" );
    &show_char( 0141, "solid rectangle" );
    &show_char( 0142, "1/" );
    &show_char( 0143, "3/" );
    &show_char( 0144, "5/" );
    &show_char( 0145, "7/" );
    &show_char( 0146, "degrees" );
    &show_char( 0147, "plus or minus" );
    &show_char( 0150, "right arrow" );
    &show_char( 0151, "ellipsis" );
    &show_char( 0152, "divide by" );
    &show_char( 0153, "down arrow" );
    &show_char( 0154, "bar at scan 0" );
    &show_char( 0155, "bar at scan 1" );
    &show_char( 0156, "bar at scan 2" );
    &show_char( 0157, "bar at scan 3" );
    &show_char( 0160, "bar at scan 4" );
    &show_char( 0161, "bar at scan 5" );
    &show_char( 0162, "bar at scan 6" );
    &show_char( 0163, "bar at scan 7" );
    &show_char( 0164, "subscript 0" );
    &show_char( 0165, "subscript 1" );
    &show_char( 0166, "subscript 2" );
    &show_char( 0167, "subscript 3" );
    &show_char( 0170, "subscript 4" );
    &show_char( 0171, "subscript 5" );
    &show_char( 0172, "subscript 6" );
    &show_char( 0173, "subscript 7" );
    &show_char( 0174, "subscript 8" );
    &show_char( 0175, "subscript 9" );
    &show_char( 0176, "paragraph" );
    &move( 19, 6 );
    printf "BAR[\033F\154\155\156\157\160\161\162\163\033G]";
    &move( 23, 0 );
}

&start_vt52;
&show_table;
&stop_vt52;

1;
