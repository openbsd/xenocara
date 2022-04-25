#!/usr/bin/env perl
# $XTermId: wrap.pl,v 1.12 2007/07/13 00:15:28 tom Exp $
# -----------------------------------------------------------------------------
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
# Generates a series of wrapping lines, according to the terminal width.
# The wrapping text optionally includes double-width or other characters
# encoded in UTF-8.
use strict;

use Getopt::Std;

our ($opt_i, $opt_n, $opt_r, $opt_w);
our ($lineno, $test_string, $term_width);

# Return a string of two-column characters given an ASCII alpha/numeric string
sub double_cells($) {
	my $value = $_[0];
	$value =~ s/ /  /g;
	pack("U*",
	map { ($_ <= 32 || $_ > 127)      # if non-ASCII character...
	       ? 32                       # ...just show a blank
	       : (0xff00 + ($_ - 32))     # map to "Fullwidth Form"
	} unpack("C*", $value));          # unpack unsigned-char characters
}

# Insert a character using escape sequences to push the existing text to the
# right, write the actual character and then move left one column so succeeding
# calls will do the same.  This will not cause the pushed-text to wrap, but
# will exercise the right-margin logic in other ways.
#
# Since this script does not modify the autowrap mode, you can reset that
# outside the script and compare the default (unwrapped) versus the "-i"
# option.
sub insert_char($$) {
	my $value = $_[0];
	my $final = $_[1];
	my $cells = defined($opt_w) ? 2 : 1;
	printf "\x1b[%d@", $cells;
	printf "%s", defined($opt_w) ? double_cells($value) : $value;
	if ( ! $final ) {
		printf "\x1b[%dD", $cells;
	}
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

# Vary the length of each line from $term_width - 5 to $term_width + 5, then
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

# Write the text for the given line-number.
sub testit($) {
	my $number = $_[0];
	my $length = length_of($number);
	if ( defined($opt_n) ) {
		printf "%5d ", $number % 99999;
		$length -= 6;
	}
	# If we're printing double-column characters, we have half as much
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
	if ( defined($opt_i) ) {
		my ($n, $c);
		for ($n = length($string) - 1; $n >= 0; $n--) {
			insert_char(substr($string, $n, 1), $n == 0);
		}
		printf "\n";
	} else {
		if ( defined($opt_w) ) {
			$string = double_cells($string);
		}
		printf "%s\n", $string;
	}
}

sub main::HELP_MESSAGE() {
	printf STDERR <<EOF
Usage: $0 [options]

Options:

-i   construct lines by inserting characters on the left
-n   write line-numbers
-r   repeat indefinitely
-w   write wide-character test-string
EOF
;
	exit;
}

&getopts('inrw') || die();

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
