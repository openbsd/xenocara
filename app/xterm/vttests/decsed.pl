#!/usr/bin/env perl
# $XTermId: decsed.pl,v 1.6 2015/02/22 01:37:20 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2015 by Thomas E. Dickey
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
# Exercise DECSED (erase display) with or without DECSCA (protect against
# DECSED).
use strict;

use Getopt::Std;

our ( $opt_c,  $opt_n,       $opt_p,     $opt_w );
our ( $lineno, $test_string, $term_wide, $term_high );

sub move($$) {
    my $y = shift;
    my $x = shift;
    printf "\x1b[%d;%dH", $y, $x;
}

sub protect($) {
    my $code = shift;
    printf "\x1b[%d\"q", $code;
}

sub set_color($) {
    my $code = shift;
    if ( $code == 1 ) {
        printf "\x1b[0;36;44m";    # cyan-on-blue
    }
    else {
        printf "\x1b[0;39;49m";
    }
}

# returns a string of two-column characters given an ASCII alpha/numeric string
sub double_cells($) {
    my $value = $_[0];
    $value =~ s/ /  /g;
    pack(
        "U*",
        map {
            ( $_ <= 32 || $_ > 127 )    # if non-ASCII character...
              ? 32                      # ...just show a blank
              : ( 0xff00 + ( $_ - 32 ) )    # map to "Fullwidth Form"
          } unpack( "C*", $value )
    );                                      # unpack unsigned-char characters
}

# write the text for the given line-number
sub fill_line($$) {
    my $number = shift;
    my $offset = shift;
    my $length = $opt_w ? ( $term_wide / 2 ) : $term_wide;
    my $actual;
    my $margin = 0;
    $actual = $length;
    my $string = $test_string;
    while ( ( $opt_w ? ( 2 * length($string) ) : length($string) ) <
        ( $offset + $length ) )
    {
        $string = $string . $test_string;
    }
    $string = substr( $string, $offset, $length );
    $string = double_cells($string) if ($opt_w);
    printf "%s", $string;

    printf "\n";
    return ++$offset;
}

sub main::HELP_MESSAGE() {
    printf STDERR <<EOF
Usage: $0 [options] DECSED [y [x]]

The test
    fills the screen (-n for normal, -w for wide characters, default DECALN)
    then positions to the given y,x (default is middle of screen),
    writes a '*' at the cursor position
    moves back to given y,x again
    erases with the DECSED value (0=below, 1=above, 2=all=default),
    moves the cursor up/down one line to avoid overwriting by prompt

Options:

-c   use color
-n   write normal-characters rather than using DECALN
-p   protect screen against erasure using DECSCA (DECALN is unprotected)
-w   write wide-characters rather than using DECALN
EOF
      ;
    exit;
}

&getopts('cnpw') || &main::HELP_MESSAGE;

$term_wide = `tput cols`;
$term_high = `tput lines`;

$test_string =
  "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";

my $parm_DECSED = ( $#ARGV >= 0 ) ? $ARGV[0] : 2;
my $parm_ycoord = ( $#ARGV >= 1 ) ? $ARGV[1] : $term_high / 2;
my $parm_xcoord = ( $#ARGV >= 2 ) ? $ARGV[2] : $term_wide / 2;

binmode( STDOUT, ":utf8" );
&protect(1)   if ($opt_p);
&set_color(1) if ($opt_c);
if ( $opt_n or $opt_w ) {
    my $offset = 0;
    for ( $lineno = 0 ; $lineno < $term_high - 1 ; ++$lineno ) {
        $offset = &fill_line( $lineno, $offset );
    }
}
else {
    printf "\x1b#8";    # DECALN
}
&move( $parm_ycoord, $parm_xcoord );
printf '*';
&move( $parm_ycoord, $parm_xcoord );
printf "\x1b[?%dJ", $parm_DECSED;
if ( $parm_DECSED == 0 ) {
    &move( $parm_ycoord + 1, $parm_xcoord );
}
elsif ( $parm_DECSED == 1 ) {
    &move( $parm_ycoord - 1, $parm_xcoord );
}
&set_color(0) if ($opt_c);
&protect(0)   if ($opt_p);

exit;
