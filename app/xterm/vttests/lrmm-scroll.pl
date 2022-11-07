#!/usr/bin/env perl
# $XTermId: lrmm-scroll.pl,v 1.14 2022/10/10 17:07:48 tom Exp $
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
# Tests scroll left/right feature in xterm, optionally using margins.  This
# applies only to the visible screen (saved-lines are unaffected).
#

use warnings;
use strict;
use diagnostics;

use Term::ReadKey;
use Getopt::Std;

# do this so output from successive calls to this script won't get in the
# wrong order:
use IO::Handle;
STDERR->autoflush(1);
STDOUT->autoflush(1);

our ( $opt_8, $opt_c, $opt_l, $opt_o, $opt_r, $opt_s, $opt_w, $opt_x );
our ( $margins, $test_state, $test_string, $test_width );
our ( $term_height, $term_width );

our $CSI = "\033[";

our @resize;

sub read_resize($) {
    my $field  = shift;
    my $result = shift;
    if ( $#resize < 0 ) {
        open( FP, "resize -u |" ) or exit $!;
        @resize = <FP>;
        chomp @resize;
        close(FP);
    }
    for my $n ( 0 .. $#resize ) {
        if ( $resize[$n] =~ /^$field=/ ) {
            $result = $resize[$n];
            $result =~ s/^[^=]*=//;
            $result =~ s/;.*//;
            last;
        }
    }
    return $result;
}

# returns the number of rows in the screen
sub screen_height() {
    return &read_resize( "LINES", 24 );
}

# returns the number of columns in the screen
sub screen_width() {
    return &read_resize( "COLUMNS", 80 );
}

sub set_color($) {
    my $code = shift;
    if ( defined($opt_c) ) {
        if ( $code == 3 ) {
            printf "%s1;33;42m", $CSI;    # yellow-on-green
        }
        elsif ( $code == 2 ) {
            printf "%s0;31;45m", $CSI;    # red-on-magenta
        }
        elsif ( $code == 1 ) {
            printf "%s0;36;44m", $CSI;    # cyan-on-blue
        }
        else {
            printf "%s0;39;49m", $CSI;
        }
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

sub clear_screen() {
    &upper_left;
    printf "%sJ", $CSI;
}

sub clr_to_eol() {
    printf "%sK", $CSI;
}

sub lower_left() {
    printf "%s%dH", $CSI, $term_height;
}

sub upper_left() {
    printf "%sH", $CSI;
}

sub move_to($) {
    my $value = shift;
    $value += ( $opt_l - 1 ) if ( $margins and not $opt_o );
    printf "%s%dG", $CSI, $value + 1;
}

sub bak_scroll($) {
    my $value = shift;

    if ($value) {
        printf "%s%dS", $CSI, $value;
    }
    else {
        printf "%sS", $CSI;
    }
}

sub delete_char() {
    &set_color(2);
    printf "%s%dP", $CSI, 1;
    &set_color(1);
}

sub insert_once($) {
    my $value = shift;
    &set_color(2);
    printf "%s%d@", $CSI, length($value);
    &write_chars($value);
}

sub insert_mode($) {
    my $value = shift;
    &set_color(2);
    printf "%s%dP", $CSI, length($value);
    printf "%s4h", $CSI;
    &write_chars($value);
    printf "%s4l", $CSI;
}

sub write_chars($) {
    &set_color(3);
    printf "%s", $_[0];
    &set_color(1);
}

# vary the starting point of each line, to make a more interesting pattern
sub starts_of($) {
    my $value = shift;
    if ( defined($opt_w) ) {

        # 0,1,1,2,2,3,3,...
        $value = ( ( $value + 1 ) / 2 ) % length($test_string);
    }
    else {
        $value %= length($test_string);
    }
    return $value;
}

# write the text for the given line-number
sub show_line($) {
    my $number = shift;
    my $length = $test_width;

    # use delete-lines to "pull" the screen up, like scrolling.
    select( undef, undef, undef, 0.05 ) if ($opt_s);
    &lower_left;
    &bak_scroll(1);

    # if we're printing double-column characters, we have half as much
    # space effectively - but don't forget the remainder, so we can push
    # the characters by single-columns.
    if ( defined($opt_c) ) {
        &set_color(1);
        printf "%s%dX", $CSI, $length if ($margins);
        &clr_to_eol unless ($margins);
    }
    my $starts = &starts_of($number);
    if ( defined($opt_w) ) {
        printf " ", if ( ( $number % 2 ) != 0 );
        $length = ( $length - ( ($number) % 2 ) ) / 2;
    }
    my $string = substr( $test_string, $starts );
    while ( length($string) < $length ) {
        $string = $string . $test_string;
    }
    $string = substr( $string, 0, $length );
    if ( defined($opt_w) ) {
        $string = &double_cells($string);
    }
    printf "%s", $string;

    # now - within the line - modify it
    if ($opt_x) {
        &move_to( ( 4 * $test_width ) / 5 );
        &insert_mode("XX");
        &move_to( ( 3 * $test_width ) / 5 );
        &delete_char;
        &move_to( ( 2 * $test_width ) / 5 );
        &insert_once('~');
        &move_to( ( 1 * $test_width ) / 5 );
        &write_chars('~');
        &move_to(0);
    }
    &set_color(0);
}

sub show_pattern() {
    &set_color(0);
    &clear_screen;
    for ( my $lineno = 0 ; $lineno < $term_height ; ++$lineno ) {
        &show_line($lineno);
    }
}

sub scroll_left($) {
    my $value = shift;
    printf "%s%d @", $CSI, $value;
}

sub scroll_right($) {
    my $value = shift;
    printf "%s%d A", $CSI, $value;
}

sub show_help() {
    &finish_test;
    &clear_screen;
    printf <<EOF;
Key assignments:\r
\r
?            shows this screen\r
l, backspace scrolls left\r
r, space     scrolls right\r
^L           resets the scrolling\r
q            quits the demo\r
\r
Press any key to continue...\r
EOF
    my $key = ReadKey 0;
    &start_test;
    &show_pattern;
}

sub start_test() {
    &clear_screen;

    printf "\x1b G" if ($opt_8);
    if ($margins) {
        printf "%s?6h", $CSI if ($opt_o);
        printf "%s?69h", $CSI;
        printf "%s%d;%ds", $CSI, $opt_l, $opt_r;
    }
}

sub finish_test() {
    printf "%s?6;69l", $CSI if ($margins);
    printf "\x1b F" if ($opt_8);

    &lower_left;
    &clr_to_eol;
}

sub do_test() {
    $test_state %= $test_width;

    my $key = ReadKey 0;

    &show_pattern;
    &move_to( 0, $test_state );

    my $result = 1;
    if ( $key eq "q" or $key eq "\033" ) {
        $result = 0;
    }
    elsif ( $key eq " " or $key eq "l" ) {
        &set_color(1);
        &scroll_left( ++$test_state );
    }
    elsif ( $key eq "\b" or $key eq "r" ) {
        &set_color(1);
        &scroll_right( ++$test_state );
    }
    elsif ( $key eq "?" ) {
        &show_help;
    }
    elsif ( $key eq "\f" ) {
        $test_state = 0;
    }
    return $result;
}

sub testit() {
    ReadMode 'ultra-raw';
    $test_state = 0;
    &show_pattern;
    do {
    } while (&do_test);
    ReadMode 'restore';
    &set_color(0);
}

sub main::HELP_MESSAGE() {
    printf STDERR <<EOF
Usage: $0 [options]

Options:

-8     use 8-bit C1 controls
-c     use color
-l COL specify left margin
-r COL specify right margin
-o     enable origin-mode with margins
-s     slow down test-setup
-w     write wide-characters
-x     modify test-string with inserted/deleted cells
EOF
      ;
    exit 1;
}

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('8cl:or:swx') || &main::HELP_MESSAGE;

$term_height = &screen_height;
$term_width  = &screen_width;

&main::HELP_MESSAGE if ( $opt_8 and $opt_w );
$CSI     = "\x9b" if ($opt_8);
$margins = 1      if ( $opt_l or $opt_r );
$opt_l   = 1      if ( $margins and not $opt_l );
$opt_r = $term_width if ( $margins and not $opt_l );

$test_width = $term_width;
$test_width = ( $opt_r - $opt_l + 1 ) if ($margins);

$test_string =
  "0123456789 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ";

binmode( STDOUT, ":utf8" ) unless ($opt_8);

&start_test;
&testit;
&finish_test;

1;
