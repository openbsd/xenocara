#!/usr/bin/env perl
# $XTermId: blink.pl,v 1.2 2007/07/13 00:28:38 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2007 by Thomas E. Dickey
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
# Write a test pattern which includes some blinking text in scattered
# locations, to test scrollback of blinking text.
use strict;

use Getopt::Std;

our ($opt_n, $opt_r, $opt_w);
our ($lineno, $test_string, $term_width);

# returns a string of two-column characters given an ASCII alpha/numeric string
sub double_cells($) {
	my $value = $_[0];
	$value =~ s/ /  /g;
	pack("U*",
	map { ($_ <= 32 || $_ > 127)      # if non-ASCII character...
	       ? 32                       # ...just show a blank
	       : (0xff00 + ($_ - 32))     # map to "Fullwidth Form"
	} unpack("C*", $value));          # unpack unsigned-char characters
}

# vary the starting point of each line, to make a more interesting pattern
sub starts_of($) {
	my $value = $_[0];
	if (defined($opt_w)) {
		# 0,1,1,2,2,3,3,...
		$value = (($value + 1) / 2) % length($test_string);
	} else {
		$value %= length($test_string);
	}
	return $value;
}

# vary the length of each line from $term_width - 5 to $term_width + 5, then
# double it, and then repeat.  That's 22/cycle.
sub length_of($) {
	my $value = $_[0];
	my $cycle = $value % 22;
	if ( $cycle < 11 ) {
		$value = $term_width;
	} else {
		$value = $term_width * 2;
		$cycle /= 2;
	}
	return $value + $cycle - 5;
}

# write the text for the given line-number
sub testit($) {
	my $number = $_[0];
	my $length = length_of($number);
	if ( defined($opt_n) ) {
		printf "%5d ", $number % 99999;
		$length -= 6;
	}
	# if we're printing double-column characters, we have half as much
	# space effectively - but don't forget the remainder, so we can push
	# the characters by single-columns.
	my $starts = starts_of($number);
	if ( defined($opt_w) ) {
		printf " ", if ( ($number % 2 ) != 0);
		$length = ($length + (($number + 1) % 2)) / 2;
	}
	my $string = substr($test_string, $starts);
	while ( length($string) < $length ) {
		$string = $string . $test_string;
	}
	$string = substr($string, 0, $length);
	if ( defined($opt_w) ) {
		$string = double_cells($string);
	}
	printf "%s\n", $string;
}

sub main::HELP_MESSAGE() {
	printf STDERR <<EOF
Usage: $0 [options]

Options:

-n   write line-numbers
-r   repeat indefinitely
-w   write wide-characters
EOF
;
	exit;
}

&getopts('nrw') || die();

$term_width=`tput cols`;

$test_string="0123456789 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ";

binmode(STDOUT, ":utf8");
if ( defined($opt_r) ) {
	for ($lineno = 0; ; ++$lineno) {
		testit($lineno);
	}
} else {
	for ($lineno = 0; $lineno < 24; ++$lineno) {
		testit($lineno);
	}
}

exit;
