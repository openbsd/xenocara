#!/usr/bin/env perl
# $XTermId: vxt-insdelln,v 1.10 2022/10/10 17:05:38 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2009,2022 by Thomas E. Dickey
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
# Tests insert/delete-line feature in xterm.  This applies only to the
# visible screen (saved-lines are unaffected).
#
# TODO:
#	add option to wrap the test-pattern
#	use scrolling-margins to help fill-in a chunk
use strict;
use warnings;

use Getopt::Std;

# do this so output from successive calls to this script won't get in the
# wrong order:
use IO::Handle;
STDERR->autoflush(1);
STDOUT->autoflush(1);

our ( $opt_c,  $opt_n,       $opt_r,       $opt_w );
our ( $lineno, $test_string, $term_height, $term_width );

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
    my $code = $_[0];
    if ( defined($opt_c) ) {
        if ( $code == 3 ) {
            printf "\x1b[1;33;42m";    # yellow-on-green
        }
        elsif ( $code == 2 ) {
            printf "\x1b[0;31;45m";    # red-on-magenta
        }
        elsif ( $code == 1 ) {
            printf "\x1b[0;36;44m";    # cyan-on-blue
        }
        else {
            printf "\x1b[0;39;49m";
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
    upper_left();
    printf "\x1b[J";
}

sub clr_to_eol() {
    printf "\x1b[K";
}

sub lower_left() {
    printf "\x1b[%dH", $term_height;
}

sub upper_left() {
    printf "\x1b[H";
}

sub move_to($) {
    printf "\x1b[%dG", $_[0] + 1;
}

sub insert_lines($) {

    #lower_left;
    if ( $_[0] ) {
        printf "\x1b[%dL", $_[0];
    }
    else {
        printf "\x1b[L";
    }
}

sub delete_lines($) {
    if ( $_[0] ) {
        printf "\x1b[%dM", $_[0];
    }
    else {
        printf "\x1b[M";
    }
}

sub delete_char() {
    set_color(2);
    printf "\x1b[%dP", 1;
    set_color(1);
}

sub insert_once($) {
    my $text = shift;
    set_color(2);
    printf "\x1b[%d@", length($text);
    write_chars($text);
}

sub insert_mode($) {
    set_color(2);
    printf "\x1b[%dP", length( $_[0] );
    printf "\x1b[4h";
    write_chars( $_[0] );
    printf "\x1b[4l";
}

sub write_chars($) {
    set_color(3);
    printf "%s", $_[0];
    set_color(1);
}

# vary the starting point of each line, to make a more interesting pattern
sub starts_of($) {
    my $value = $_[0];
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
sub testit($) {
    my $number = $_[0];
    my $length = $term_width;

    # use delete-lines to "pull" the screen up, like scrolling.
    select( undef, undef, undef, 0.1 );
    if ( ( ( $number / $term_height ) % 2 ) != 0 ) {
        upper_left;
        insert_lines(1);
    }
    else {
        upper_left;
        delete_lines(1);
        lower_left;
    }
    if ( defined($opt_n) ) {
        printf "%5d ", $number % 99999;
        $length -= 6;
    }

    # if we're printing double-column characters, we have half as much
    # space effectively - but don't forget the remainder, so we can push
    # the characters by single-columns.
    if ( defined($opt_c) ) {
        set_color(1);
        clr_to_eol();
    }
    my $starts = starts_of($number);
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
        $string = double_cells($string);
    }
    printf "%s", $string;

    # now - within the line - modify it
    move_to( ( 4 * $term_width ) / 5 );
    insert_mode("XX");
    move_to( ( 3 * $term_width ) / 5 );
    delete_char();
    move_to( ( 2 * $term_width ) / 5 );
    insert_once('~');
    move_to( ( 1 * $term_width ) / 5 );
    write_chars('~');
    move_to(0);
    set_color(0);
}

sub main::HELP_MESSAGE() {
    printf STDERR <<EOF
Usage: $0 [options]

Options:

-c   use color
-n   write line-numbers
-r   repeat indefinitely
-w   write wide-characters
EOF
      ;
    exit;
}

&getopts('cnrw') || die();

$term_height = screen_height();
$term_width  = screen_width();

$test_string =
  "0123456789 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ";

binmode( STDOUT, ":utf8" );
clear_screen();
if ( defined($opt_r) ) {
    for ( $lineno = 0 ; ; ++$lineno ) {
        testit($lineno);
    }
}
else {
    for ( $lineno = 0 ; $lineno < $term_height * 2 ; ++$lineno ) {
        testit($lineno);
    }
}
lower_left();
clr_to_eol();

exit;
