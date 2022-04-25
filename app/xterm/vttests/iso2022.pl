#!/usr/bin/env perl
# $XTermId: iso2022.pl,v 1.5 2021/02/13 01:24:32 tom Exp $
# -----------------------------------------------------------------------------
# Copyright 2021 by Thomas E. Dickey
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
# show ISO-2022 characters, by default GL and GR, optionally G1/G2/G3

use strict;
use warnings;

use Getopt::Std;

$| = 1;

our ( $opt_k, $opt_n, $opt_q, $opt_s );

sub doit() {
    my $GL = "";
    printf "-- vile: fk=8bit\n";
    my $lo = 0;
    if ($opt_q) {
        $GL =
            "`1234567890-=\n"
          . "qwertyuiop[]\\\n"
          . "asdfghjkl;'\n"
          . "zxcvbnm,./\n"
          . "~!@#$%^&*()_+\n"
          . "QWERTYUIOP{}|\n"
          . "ASDFGHJKL:\"\n"
          . "ZXCVBNM<>?";
    }
    else {
        $lo = $opt_k ? 0 : 32;
        for my $n ( $lo .. 127 ) {
            $GL .= chr($n);
            $GL .= "\n" if ( ( ( $n - 31 ) % 16 ) == 0 );
        }
    }
    my $GR = "";
    for my $n ( 0 .. ( length($GL) - 1 ) ) {
        my $c = substr( $GL, $n, 1 );
        if ( ord($c) == 10 and ( not $opt_k or $n != 10 ) ) {
            $GR .= $c;
        }
        else {
            $GR .= chr( ord($c) + 128 );
        }
    }
    if ($opt_s) {
        $GL =~ s/([^\n])/ $1/g;
        $GR =~ s/([^\n])/ $1/g;
    }
    printf "GL:\n%s\n", $GL;
    printf "GR:\n%s\n", $GR;
}

sub main::HELP_MESSAGE() {
    printf STDERR <<EOF
Usage: $0 [options]

Options:
    -k  assume 128-159 are printable.
    -n  print hex value before each character
    -q  use QWERTY
    -s  space between characters
EOF
      ;
    exit 1;
}
$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('knqs') || main::HELP_MESSAGE;

&doit;

1;
