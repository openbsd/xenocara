#!/usr/bin/env perl
# $XTermId: under-latin.pl,v 1.6 2020/01/31 00:16:52 tom Exp $
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
# Print a text-test pattern using Latin-1 characters that have these features:
#	a) accents
#	b) descenders
#	c) underlining

use strict;
use warnings;

use Getopt::Std;
use Term::ReadKey;

$| = 1;

our ( $opt_b, $opt_i, $opt_u );

our $ROWS = 24;
our $COLS = 4;

our @sample;

sub underlined($$) {
    my $text = shift;
    my $code = shift;
    $text = sprintf "\033[4m%s\033[24m", $text if ($code);
    return $text;
}

sub print_row($) {
    my $y     = shift;
    my $cells = $y * 5;
    for my $x ( 0 .. $COLS ) {
        printf "%s",
          &underlined( $sample[ $cells % 2 ], ( $cells % 4 ) > 1 ? 1 : 0 );
        ++$cells;
    }
}

sub main::HELP_MESSAGE() {
    printf STDERR <<EOF
Usage: $0 [options]

Options:

-b         write a pattern in bold-text
-i         write a pattern in italic-text
-u         write text in UTF-8
EOF
      ;
    exit;
}

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('biu') || &main::HELP_MESSAGE;

if ( -t 0 ) {
    my $size = `stty size`;
    chomp $size;
    if ( $size =~ /^\d+\s+\d+$/ ) {
        my @size = split /\s+/, $size;
        $ROWS = $size[0];
        $COLS = $size[1] - 4;
        $COLS /= 2 if ( $opt_b or $opt_i );
        $COLS /= 7;
        $COLS = int($COLS) - 1;
    }
}

binmode( STDOUT, ":utf8" ) if ($opt_u);

$sample[0] = sprintf "%c%c%c%c%c%c%c", 192, 193, 194, 195, 196, 197, 198;
$sample[1] = sprintf "gjpqy%c%c", 199, 255;

for my $y ( 0 .. ( $ROWS - 1 ) ) {
    printf "%3d ", $y + 1;
    printf "\033[1m" if ( $opt_b and $opt_i );
    &print_row($y);
    printf "\033[22m" if ( $opt_b and $opt_i );
    if ( $opt_b or $opt_i ) {
        printf "\033[%dm", $opt_i ? 3 : 1;
        &print_row($y);
        printf "\033[%dm", $opt_i ? 23 : 22;
    }
    printf "\n" unless ( $y + 1 >= $ROWS );
}

if ( -t 1 ) {
    printf "\033[m";
    ReadMode 'cbreak';
    my $key = ReadKey(30);
    ReadMode 'normal';
}
printf "\n";

1;
