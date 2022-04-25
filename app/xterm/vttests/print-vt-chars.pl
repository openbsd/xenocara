#!/usr/bin/perl -w
# $XTermId: print-vt-chars.pl,v 1.23 2020/12/13 15:05:06 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2018,2020 by Thomas E. Dickey
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
# Print GL and GR, with the same charset (if possible) for testing.

use strict;
use warnings;

$| = 2;

use Getopt::Std;

our ( $opt_L, $opt_l, $opt_R, $opt_r, $opt_v );

our %charsets;
our %caseless;
our $vt_level;

binmode STDOUT;

sub NRC($) {
    printf "\033[?42%s", $_[0] ? "h" : "l";
}

sub LS0($) {
    printf "\017";
}

sub LS1() {
    printf "\016";
}

sub LS1R() {
    printf "\033~";
}

sub LS2() {
    printf "\033n";
}

sub LS2R() {
    printf "\033}";
}

sub LS3() {
    printf "\033o";
}

sub LS3R($) {
    printf "\033|";
}

sub G0($) {
    my %charset = %{ $_[0] };
    printf "\033(%s", $charset{TAG} if ( $charset{HOW} == 0 );
}

sub G1($) {
    my %charset = %{ $_[0] };
    printf "\033)%s", $charset{TAG} if ( $charset{HOW} == 0 );
    printf "\033-%s", $charset{TAG} if ( $charset{HOW} == 1 );
}

sub G2($) {
    my %charset = %{ $_[0] };
    printf "\033*%s", $charset{TAG} if ( $charset{HOW} == 0 );
    printf "\033.%s", $charset{TAG} if ( $charset{HOW} == 1 );
}

sub G3($) {
    my %charset = %{ $_[0] };
    printf "\033+%s", $charset{TAG} if ( $charset{HOW} == 0 );
    printf "\033/%s", $charset{TAG} if ( $charset{HOW} == 1 );
}

sub init_charset($$$$$$) {
    my %charset;
    my $mixed = shift;
    $charset{WHO}     = $mixed;
    $charset{HOW}     = shift;
    $charset{TAG}     = shift;
    $charset{MIN}     = shift;
    $charset{MAX}     = shift;
    $charset{NRC}     = shift;
    $charsets{$mixed} = \%charset;
    my $lower = lc $charset{WHO};
    $caseless{$lower} = $charset{WHO};
}

sub find_charset($) {
    my $mixed = shift;
    my $lower = lc $mixed;
    my %result;
    if ( $caseless{$lower} ) {
        $mixed  = $caseless{$lower};
        %result = %{ $charsets{$mixed} };
        undef %result
          if ( $result{MAX} < $vt_level or $result{MIN} > $vt_level );
    }
    printf STDERR "? no match for $mixed with VT-level $vt_level\n"
      unless %result;
    return \%result;
}

sub failed($) {
    my $msg = shift;
    printf STDERR "? %s\n", $msg;
    exit 1;
}

sub valid_code($) {
    my $code   = shift;
    my $result = 0;
    $result = 1 if ( $code =~ /^[0-3]$/ );
    return $result;
}

sub valid_name($) {
    my $mixed  = shift;
    my $lower  = lc $mixed;
    my $result = 0;
    $result = 1 if ( defined( $caseless{$lower} ) );
    return $result;
}

sub setup_charsets($$$$) {
    my $gl_code = shift;
    my $gl_name = shift;
    my $gr_code = shift;
    my $gr_name = shift;
    my %gl_data = %{ &find_charset($gl_name) };
    my %gr_data = %{ &find_charset($gr_name) };

    return 0 unless %gl_data;
    return 0 unless %gr_data;

    &NRC(1) if ( $gl_data{NRC} or $gr_data{NRC} );

    if ( $gl_code == 0 ) {
        &G0( \%gl_data );
        &LS0;
    }
    elsif ( $gl_code == 1 ) {
        &G1( \%gl_data );
        &LS1;
    }
    elsif ( $gl_code == 2 ) {
        &G2( \%gl_data );
        &LS2;
    }
    elsif ( $gl_code == 3 ) {
        &G3( \%gl_data );
        &LS3;
    }

    if ( $gr_code == 0 ) {
        &G0( \%gr_data );
    }
    elsif ( $gr_code == 1 ) {
        &G1( \%gr_data );
        &LS1R;
    }
    elsif ( $gr_code == 2 ) {
        &G2( \%gr_data );
        &LS2R;
    }
    elsif ( $gr_code == 3 ) {
        &G3( \%gr_data );
        &LS3R;
    }
    return 1;
}

sub cleanup() {
    &setup_charsets( 0, "ASCII", 1, "ASCII" );
    &NRC(0);
}

sub doit($$$$) {
    my $gl_code = shift;
    my $gl_name = shift;
    my $gr_code = shift;
    my $gr_name = shift;

    &failed("Illegal left-code $gl_code")     unless &valid_code($gl_code);
    &failed("Illegal right-code $gr_code")    unless &valid_code($gr_code);
    &failed("Unknown left-charset $gl_name")  unless &valid_name($gl_name);
    &failed("Unknown right charset $gr_name") unless &valid_name($gr_name);

    printf "GL (G%d %s):\n", $gl_code, $gl_name;
    if ( &setup_charsets( $gl_code, $gl_name, $gr_code, $gr_name ) ) {

        for my $c ( 32 .. 127 ) {
            printf "%c", $c;
            printf "\n" if ( ( ( $c - 31 ) % 16 ) == 0 );
        }
        printf "\n";

        &cleanup;
    }

    printf "GR (G%d %s):\n", $gr_code, $gr_name;
    if ( &setup_charsets( $gl_code, $gl_name, $gr_code, $gr_name ) ) {

        for my $c ( 32 .. 127 ) {
            printf "%c", $c + 128;
            printf "\n" if ( ( ( $c - 31 ) % 16 ) == 0 );
        }
        printf "\n";

        &cleanup;
    }
}

sub main::HELP_MESSAGE() {
    printf STDERR <<EOF
Usage: $0 [options]

Options:

 -L code    index 0-3 for GL
 -l name    charset to map to GL
 -R code    index 0-3 for GR
 -r name    charset to map to GR
 -v level   set/override VT-level

Charsets are determined by the VT-level (currently VT${vt_level}xx):
EOF
      ;
    my @known;
    my $known = -1;
    my $width = 0;
    foreach my $key ( sort( keys %charsets ) ) {
        my %charset = %{ $charsets{$key} };
        next if ( $charset{MAX} < $vt_level );
        next if ( $charset{MIN} > $vt_level );
        $known[ ++$known ] = $key;
        $width = length($key) if ( length($key) > $width );
    }
    $width += 3;
    my $cols = int( 78 / $width );
    my $high = int( ( $known + $cols ) / $cols );
    for my $y ( 0 .. $high - 1 ) {
        printf STDERR " ";
        for my $x ( 0 .. $cols - 1 ) {
            my $z = $x * $high + $y;
            next if ( $z > $known );
            printf STDERR "%-*s", $width, $known[$z];
        }
        printf STDERR "\n";
    }
    exit 1;
}

&init_charset( "ASCII",              0, 'B',  1, 9, 0 );
&init_charset( "British",            0, 'A',  1, 9, 0 );
&init_charset( "DEC_Spec_Graphic",   0, '0',  1, 9, 0 );
&init_charset( "DEC_Alt_Chars",      0, '1',  1, 1, 0 );
&init_charset( "DEC_Alt_Graphics",   0, '2',  1, 1, 0 );
&init_charset( "DEC_Supp",           0, '<',  2, 9, 0 );
&init_charset( "Dutch",              0, '4',  2, 9, 1 );
&init_charset( "Finnish",            0, '5',  2, 9, 1 );
&init_charset( "Finnish2",           0, 'C',  2, 9, 1 );
&init_charset( "French",             0, 'R',  2, 9, 1 );
&init_charset( "French2",            0, 'f',  2, 9, 1 );
&init_charset( "French_Canadian",    0, 'Q',  2, 9, 1 );
&init_charset( "German",             0, 'K',  2, 9, 1 );
&init_charset( "Italian",            0, 'Y',  2, 9, 1 );
&init_charset( "Norwegian_Danish2",  0, 'E',  2, 9, 1 );
&init_charset( "Norwegian_Danish3",  0, '6',  2, 9, 1 );
&init_charset( "Spanish",            0, 'Z',  2, 9, 1 );
&init_charset( "Swedish",            0, '7',  2, 9, 1 );
&init_charset( "Swedish2",           0, 'H',  2, 9, 1 );
&init_charset( "Swiss",              0, '=',  2, 9, 1 );
&init_charset( "British_Latin_1",    0, 'A',  3, 9, 1 );
&init_charset( "DEC_Supp_Graphic",   0, '%5', 3, 9, 0 );
&init_charset( "DEC_Technical",      0, '>',  3, 9, 0 );
&init_charset( "French_Canadian2",   0, '9',  3, 9, 1 );
&init_charset( "Norwegian_Danish",   0, '`',  3, 9, 1 );
&init_charset( "Portuguese",         0, '%6', 3, 9, 1 );
&init_charset( "ISO_Greek_Supp",     1, 'F',  5, 9, 0 );
&init_charset( "ISO_Hebrew_Supp",    1, 'H',  5, 9, 0 );
&init_charset( "ISO_Latin_5_Supp",   1, 'M',  5, 9, 0 );
&init_charset( "ISO_Latin_Cyrillic", 1, 'L',  5, 9, 0 );
&init_charset( "Greek",              0, '">', 5, 9, 1 );
&init_charset( "DEC_Greek",          0, '"?', 5, 9, 1 );
&init_charset( "Cyrillic",           0, '&4', 5, 9, 0 );
&init_charset( "DEC_Hebrew",         0, '"4', 5, 9, 0 );
&init_charset( "Hebrew",             0, '%=', 5, 9, 1 );
&init_charset( "Russian",            0, '&5', 5, 9, 1 );
&init_charset( "SCS_NRCS",           0, '%3', 5, 9, 0 );
&init_charset( "Turkish",            0, '%2', 5, 9, 1 );
&init_charset( "DEC_Turkish",        0, '%0', 5, 9, 0 );

$vt_level = 1;    # don't expect much
if ( -t 0 and -t 1 ) {
    my $da2 = `
	old=\$(stty -g);
	stty raw -echo min 0  time 5;
	printf '\033[>c' >/dev/tty;
	read response;
	stty \$old;
	echo "\$response"`;
    if ( $da2 =~ /^\033\[>\d+;\d+;\d+c$/ ) {
        my $Pp = $da2;
        $Pp =~ s/^.*>//;
        $Pp =~ s/;.*$//;
        if ( $Pp == 0 ) {
            $vt_level = 1;
        }
        elsif ( $Pp == 1 or $Pp == 2 ) {
            $vt_level = 2;
        }
        elsif ( $Pp == 18 or $Pp == 19 or $Pp == 24 ) {
            $vt_level = 3;
        }
        elsif ( $Pp == 41 ) {
            $vt_level = 4;
        }
        elsif ( $Pp == 61 or $Pp == 64 or $Pp == 65 ) {
            $vt_level = 5;
        }
    }
}

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('L:l:R:r:v:') || main::HELP_MESSAGE;
$vt_level = $opt_v if ( defined $opt_v );
&failed("VT-level must be 1-5") if ( $vt_level < 1 or $vt_level > 5 );

if ( $#ARGV >= 0 ) {
    while ( $#ARGV >= 0 ) {
        my $name = shift @ARGV;
        &doit(
            defined($opt_L) ? $opt_L : 2,        #
            defined($opt_l) ? $opt_l : $name,    #
            defined($opt_R) ? $opt_R : 3,        #
            defined($opt_r) ? $opt_r : $name
        );
        last
          if (
            defined($opt_L)                      #
            and defined($opt_l)                  #
            and defined($opt_R)                  #
            and defined($opt_r)
          );
    }
}
else {
    &doit(
        defined($opt_L) ? $opt_L : 2,            #
        defined($opt_l) ? $opt_l : "ASCII",      #
        defined($opt_R) ? $opt_R : 3,            #
        defined($opt_r) ? $opt_r : "ASCII"
    );
}

1;
