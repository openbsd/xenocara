#!/usr/bin/env perl
# $XTermId: bold-italics.pl,v 1.1 2019/09/22 19:44:57 tom Exp $
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
# Test bold-italics for single- and double-width characters.

use strict;
use warnings;

use I18N::Langinfo qw(langinfo CODESET);

our $encoding    = lc( langinfo( CODESET() ) );
our $single_text = "ABCDEFGH";
our $double_text = "";

sub showcase($$) {
    my $testcase = shift;
    my $sgr_code = shift;
    printf "\033[%sm", $sgr_code;
    printf "%-8s%s\n", $testcase, $single_text;
    printf "%-8s%s\n", " ", $double_text if ( $encoding eq "utf-8" );
    printf "\033[%sm", "0";
}

sub doit() {
    my $bold    = "1";
    my $italics = "3";
    &showcase( "Normal",  "0" );
    &showcase( "Bold",    $bold );
    &showcase( "Italics", $italics );
    &showcase( "Both:BI", "$bold;$italics" );
}

if ( $encoding eq "utf-8" ) {
    binmode( STDOUT, ":utf8" );
    for my $n ( 0 .. length($single_text) - 1 ) {
        my $chr = substr( $single_text, $n, 1 );
        $double_text .= chr( 0xff00 + ord($chr) - 32 );
    }
}
&doit;

1;
