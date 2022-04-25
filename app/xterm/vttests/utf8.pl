#!/usr/bin/env perl
# $XTermId: utf8.pl,v 1.5 2018/12/14 09:25:47 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2012,2018 by Thomas E. Dickey
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

use strict;
use Encode 'encode_utf8';

sub vxt_utf8($) {
	my $arg = $_[0];
	my $hex = $arg;
	my $dec = $arg;
	if ( $arg =~ /^u\+[[:xdigit:]]+/i ) {
		$hex =~ s/^../0x/;
		$dec = hex($hex);
	} elsif ( $arg !~ /^0x[[:xdigit:]]+$/i ) {
		$hex = sprintf "%04X", $arg;
	} elsif ( $arg !~ /^u\+[[:xdigit:]]+$/i ) {
		$hex =~ s/^u\+//i;
		$hex = sprintf "%04X", $arg;
	} else {
		$dec = hex($hex);
	}
	my $chr = chr($dec);
	my $type = ( $chr =~ /\p{isPrint}/
		     ? ( $chr =~ /\p{isAlpha}/
			 ? "alpha"
			 : ( $chr =~ /\p{isPunct}/
			     ? "punct"
			     : ( $chr =~ /\p{isDigit}/
			         ? "digit"
				 : "printing" ) ) )
		     : ( $chr =~ /\p{isCntrl}/
		         ? "cntrl"
			 : "nonprinting" ) );
	printf "%d ->%#x ->{%s} (%d bytes, %s)\n", $dec, $dec, $chr, length(Encode::encode_utf8($dec)), $type;
}

binmode(STDOUT, ":utf8");
while ( $#ARGV >= 0 ) {
	vxt_utf8 ( shift @ARGV );
}
exit;
