#!/usr/bin/env perl
# $XTermId: utf8.pl,v 1.12 2022/07/08 18:32:43 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2012-2018,2022 by Thomas E. Dickey
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
# display the given Unicode characters, given their hex or decimal values.

use warnings FATAL => "overflow";
no warnings "portable";
use strict;
use Encode 'encode_utf8';
use Text::CharWidth qw(mbswidth);

$| = 1;

sub num_bytes($) {
    my $char  = shift;
    my $value = length( Encode::encode_utf8($char) );
    my $result =
      ( $value <= 0
        ? "no bytes"
        : ( $value > 1 ? sprintf( "%d bytes", $value ) : "1 bytes" ) );
    return $result;
}

sub num_cells($) {
    my $char  = shift;
    my $value = mbswidth($char);
    my $result =
      ( $value <= 0
        ? "no cells"
        : ( $value > 1 ? sprintf( "%d cells", $value ) : "1 cell" ) );
    return $result;
}

sub pad_column($) {
    my $char  = shift;
    my $value = mbswidth($char);
    $value = 0 if ( $value < 0);
    my $result = sprintf( "%.*s", 3 - $value, "    ");
    return $result;
}

sub vxt_utf8($) {
    my $arg = $_[0];
    my $hex = $arg;
    my $dec = $arg;
    if ( $arg =~ /^u\+[[:xdigit:]]+$/i ) {
        $hex =~ s/^../0x/;
        $dec = hex($hex);
    }
    elsif ( $arg =~ /^0x[[:xdigit:]]+$/i ) {
        $dec = hex($hex);
    }
    elsif ( $arg =~ /^[[:xdigit:]]+$/i ) {
        $dec = hex($hex);
    }
    else {
        printf STDERR "? not a codepoint: $dec\n";
        return;
    }
    my $chr  = chr($dec);
    my $type = (
        $chr =~ /\p{isPrint}/
        ? (
            $chr =~ /\p{isAlpha}/
            ? "alpha"
            : (
                $chr =~ /\p{isPunct}/
                ? "punct"
                : (
                    $chr =~ /\p{isDigit}/
                    ? "digit"
                    : "printing"
                )
            )
          )
        : (
            $chr =~ /\p{isCntrl}/
            ? "cntrl"
            : "nonprinting"
        )
    );
    printf "%d ->%#x ->{%s}%s(%s %s %s)\n", $dec, $dec, $chr,
    &pad_column($chr),
      &num_bytes($chr),
      &num_cells($chr),
      $type;
}

binmode( STDOUT, ":utf8" );
while ( $#ARGV >= 0 ) {
    vxt_utf8( shift @ARGV );
}

1;
