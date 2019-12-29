#!/usr/bin/env perl
# $XTermId: sgrPushPop2.pl,v 1.2 2019/05/03 23:59:26 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
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

use strict;
use warnings;
use diagnostics;

use Getopt::Std;

$| = 1;

our ( $opt_b, $opt_n, $opt_r );

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('bn:r:') || die(
    "Usage: $0 [options]\n
Options:\n
  -b      color backgrounds instead of foregrounds
  -n NUM  limit test to NUM rows (default: 9)
  -r NUM  rotate example-columns (e.g, -r1 puts direct-color in middle)
"
);
$opt_n = 9 unless ( defined $opt_n );
$opt_r = 0 unless ( defined $opt_r );

our @xterm_ansi = (
    0x000000,    #black
    0xcd0000,    #red3
    0x00cd00,    #green3
    0xcdcd00,    #yellow3
    0x0000ee,    #blue2
    0xcd00cd,    #magenta3
    0x00cdcd,    #cyan3
    0xe5e5e5     #gray90
);

# The lengths in @example_title differ to ensure that the trailing "END!"
# should be the same color as the middle column, regardless of "-r" rotation.
our $example_title = "COLOR-";
our @example_title = ( "Indexed", "ANSI8", "Direct" );

# demonstrate selective SGR pop by a two-level test where the top-level has
# ANSI colors, while the lower-level iterates over a color test-pattern,
# alternating between direct-color and indexed-colors.

sub choose_fgbg($$) {
    my $fg     = shift;
    my $bg     = shift;
    my $result = $opt_b ? $bg : $fg;
    return $result;
}

sub choose_column($) {
    my $code = shift;
    return ( $code + $opt_r ) % 3;
}

sub pushSGR($) {
    my $params = shift;
    printf "\x1b[%s#{", $params;
}

sub popSGR() {
    printf "\x1b[#}";
}

sub mark_l() {
    printf " {";
}

sub mark_r() {
    printf "} ";
}

sub standard_example() {
    &mark_l;
    my $text = $example_title . $example_title[1];
    for my $n ( 0 .. length($text) - 1 ) {
        printf "\x1b[%dm", ( $n % 7 ) + 1 + &choose_fgbg( 30, 40 );
        printf "%s", substr( $text, $n, 1 );
    }
    &mark_r;
}

# The first 16 colors of xterm-256's palette match the ANSI+aixterm range.
# Do not imitate the bold-colors.
sub indexed_example() {
    &mark_l;
    my $text = $example_title . $example_title[0];
    for my $n ( 0 .. length($text) - 1 ) {
        my $c = ( $n % 7 ) + 1;
        printf "\x1b[%d;5:%dm", &choose_fgbg( 38, 48 ), $c;
        printf "%s", substr( $text, $n, 1 );
    }
    &mark_r;
}

# Imitate the "ANSI" colors from xterm's palette.
# (Again bold colors are not imitated here).
sub direct_example() {
    &mark_l;
    my $text = $example_title . $example_title[2];
    for my $n ( 0 .. length($text) - 1 ) {
        my $c = ( $n % 7 ) + 1;
        my $r = ( $xterm_ansi[$c] / ( 256 * 256 ) ) % 256;
        my $g = ( $xterm_ansi[$c] / (256) ) % 256;
        my $b = ( $xterm_ansi[$c] ) % 256;
        printf "\x1b[%d;2:1:%d:%d:%dm", &choose_fgbg( 38, 48 ), $r, $g, $b;
        printf "%s", substr( $text, $n, 1 );
    }
    &mark_r;
}

sub run_example($) {
    my $column = shift;
    &indexed_example  if ( &choose_column($column) == 0 );
    &standard_example if ( &choose_column($column) == 1 );
    &direct_example   if ( &choose_column($column) == 2 );
}

sub video_name($) {
    my $code   = shift;
    my $result = "?";
    $result = "normal"            if ( $code == 0 );
    $result = "bold"              if ( $code == 1 );
    $result = "faint"             if ( $code == 2 );
    $result = "italicized"        if ( $code == 3 );
    $result = "underlined"        if ( $code == 4 );
    $result = "blink"             if ( $code == 5 );
    $result = "inverse"           if ( $code == 7 );
    $result = "crossed-out"       if ( $code == 9 );
    $result = "double-underlined" if ( $code == 21 );
    return $result;
}

sub reset_video() {
    printf "\x1b[m";
}

sub set_video($) {
    my $row   = shift;
    my $param = "";
    my $cycle = 9;
    $param = 0  if ( ( $row % $cycle ) == 0 );
    $param = 1  if ( ( $row % $cycle ) == 1 );
    $param = 2  if ( ( $row % $cycle ) == 2 );
    $param = 3  if ( ( $row % $cycle ) == 3 );
    $param = 4  if ( ( $row % $cycle ) == 4 );
    $param = 5  if ( ( $row % $cycle ) == 5 );
    $param = 7  if ( ( $row % $cycle ) == 6 );
    $param = 9  if ( ( $row % $cycle ) == 7 );
    $param = 21 if ( ( $row % $cycle ) == 8 );
    printf "%-20s",    &video_name($param);
    printf "\x1b[%dm", $param;
}

printf "\x1b[H\x1b[J";

&pushSGR("");
printf "\x1b[40;37mSetting ambient colors to white-on-black\n";

# The three columns (indexed, ANSI, direct) will look similar.
&pushSGR("");

printf "Testing white-on-black with columns %s,%s,%s\n",
  $example_title[ &choose_column(0) ],
  $example_title[ &choose_column(1) ],
  $example_title[ &choose_column(2) ];

for my $row ( 0 .. $opt_n ) {

    &pushSGR("10;11");    # save/restore only foreground/background color
    &set_video($row);     # this attribute is set for the whole row
    &run_example(0);
    &popSGR;

    &run_example(1);

    &pushSGR("10;11");    # save/restore only foreground/background color
    &run_example(2);
    &popSGR;
    printf "END!";        # this is in the last color used in the middle column
    &reset_video();
    printf "\n";
}

&popSGR;
printf "The ambient colors should still be white-on-black.\n";
&popSGR;
printf "Now we should be back to whatever it was before we got here.\n";

1;
