#!/usr/bin/env perl
# $XTermId: erase.pl,v 1.4 2007/07/18 21:15:08 tom Exp $
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
# Generate a test-pattern, erasing parts of the text on each line.
# The test-pattern optionally includes double-width or other characters
# encoded in UTF-8.
use strict;

use Getopt::Std;

our ($opt_c, $opt_n, $opt_r, $opt_w);
our ($lineno, $test_string, $term_width, $term_height);

sub set_color($) {
	my $code = $_[0];
	if (defined($opt_c)) {
		if ($code == 3) {
			printf "\x1b[1;33;42m";	# yellow-on-green
		} elsif ($code == 2) {
			printf "\x1b[0;31;45m";	# red-on-magenta
		} elsif ($code == 1) {
			printf "\x1b[0;36;44m";	# cyan-on-blue
		} else {
			printf "\x1b[0;39;49m";
		}
	}
}

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

sub erase_left() {
	set_color(2);
	printf "\x1b[1K";
	set_color(1);
}

sub erase_right() {
	set_color(2);
	printf "\x1b[0K";
	set_color(1);
}

sub erase_middle($) {
	set_color(3);
	printf "\x1b[%dX", $_[0];
	set_color(1);
}

sub move_to($) {
	printf "\x1b[%dG", $_[0] + 1;
}

# write the text for the given line-number
sub testit($) {
	my $number = $_[0];
	my $length = $term_width;
	my $actual;
	my $margin = 0;
	if ( defined($opt_n) ) {
		$margin = 6;
		move_to($margin);
		$length -= $margin;
	}
	$actual = $length;
	if (defined($opt_c)) {
		set_color(1);
		erase_right();
	}
	if ( defined($opt_w) ) {
		$length /= 2;
	}
	my $string = $test_string;
	while ( length($string) < $length ) {
		$string = $string . $test_string;
	}
	$string = substr($string, 0, $length);
	if ( defined($opt_w) ) {
		$string = double_cells($string);
	}
	printf "%s", $string;

	move_to($margin + ($number % ($actual / 3)));
	erase_left();

	move_to($margin + ((2 * $actual) / 3) + ($number % ($actual / 3)));
	erase_right();

	move_to($margin + ((1 * $actual) / 3) + ($number % ($actual / 3)));
	erase_middle($actual / 10);

	set_color(0);
	if ( defined($opt_n) ) {
		move_to(0);
		printf "%5d ", $number % 99999;
	}
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

$term_width=`tput cols`;
$term_height=`tput lines`;

$test_string="0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";

binmode(STDOUT, ":utf8");
if ( defined($opt_r) ) {
	for ($lineno = 0; ; ++$lineno) {
		testit($lineno);
	}
} else {
	for ($lineno = 0; $lineno < $term_height - 1; ++$lineno) {
		testit($lineno);
	}
}

exit;
