#!/usr/bin/env perl
# $XTermId: halves.pl,v 1.11 2022/11/17 00:45:00 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2007,2022 by Thomas E. Dickey
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
# Draw a grid of characters (optionally double-width) and modify it using
# overstrike, insert- and delete-characters to see if the double-width
# characters are completely cleared when "partly" modified.
use strict;
use warnings;

use Getopt::Std;

our ( $opt_c, $opt_n, $opt_r, $opt_w );
our ( $lineno, $test_string, $term_width );

# returns the number of columns in the screen
sub screen_width() {
    open( FP, "resize -u |" ) or exit $!;
    my (@input) = <FP>;
    chomp @input;
    close(FP);
    my $result = 80;
    for my $n ( 0 .. $#input ) {
        if ( $input[$n] =~ /^COLUMNS=/ ) {
            $result = $input[$n];
            $result =~ s/^[^=]*=//;
            $result =~ s/;.*//;
            last;
        }
    }
    return $result;
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
            ( $_ <= 32 || $_ > 127 )        # if non-ASCII character...
              ? 32                          # ...just show a blank
              : ( 0xff00 + ( $_ - 32 ) )    # map to "Fullwidth Form"
        } unpack( "C*", $value )
    );                                      # unpack unsigned-char characters
}

sub move_to($) {
    printf "\x1b[%dG", $_[0] + 1;
}

sub delete_char() {
    set_color(2);
    printf "\x1b[%dP", 1;
    set_color(1);
}

sub insert_once($) {
    set_color(2);
    printf "\x1b[%d@", length( $_[0] );
    write_chars( $_[0] );
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
    if ( defined($opt_n) ) {
        printf "%5d ", $number % 99999;
        $length -= 6;
    }

    # if we're printing double-column characters, we have half as much
    # space effectively - but don't forget the remainder, so we can push
    # the characters by single-columns.
    if ( defined($opt_c) ) {
        set_color(1);
        printf "\x1b[K";
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
    printf "\n";
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

$term_width = screen_width();

$test_string =
  "0123456789 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ";

binmode( STDOUT, ":utf8" );
if ( defined($opt_r) ) {
    for ( $lineno = 0 ; ; ++$lineno ) {
        testit($lineno);
    }
}
else {
    for ( $lineno = 0 ; $lineno < 24 ; ++$lineno ) {
        testit($lineno);
    }
}

exit;
