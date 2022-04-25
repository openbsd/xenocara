#!/usr/bin/env perl
# $XTermId: cursor.pl,v 1.8 2007/12/03 00:56:29 tom Exp $
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
# Read a file (or pipe from a program) and move the cursor around the screen
# in response to h,j,k,l commands so we can see the colors that affect the
# cursor.  Exit on 'q'.  Do forward/backward paging to \E[J markers
# in the data with n,p.  Ignore other characters.
#
# Use this rather than, say, a curses program since it is much easier to
# construct a particular screen using 'script' or echo commands than to
# guarantee the same screen with curses' optimization.

use strict;

use Getopt::Std;
use IO::Handle;

our ( $opt_x );
our ( $row_max, $col_max );
our $old_stty;
our $text_blob;
our $text_1st;
our $text_2nd;
our $text_chop = qw/^\x1b\[H\x1b\[J/;
our $text_mark = "\x1b[H\x1b[J";

sub get_screensize() {
	my @reply = `resize -u`;
	chomp @reply;
	for my $n (0..$#reply) {
		if ( $reply[$n] =~ /=/ ) {
			my $value = $reply[$n];
			$value =~ s/^.*=//;
			if ( $reply[$n] =~ /^COLUMNS.*/ ) {
				$col_max = $value;
			} else {
				$row_max = $value;
			}
		}
	}
}

sub end_cursor($) {
	close TTY;
	system "stty $old_stty";
	print $_[0];
	exit;
}

sub begin_cursor() {
	open TTY, "+</dev/tty" or end_cursor("Cannot open /dev/tty\n");
	autoflush TTY 1;
	$old_stty=`stty -g`;
	system "stty raw -echo min 0 time 1";
}

sub beep() {
	printf "\007";
}

sub get_char() {
	my $reply;
	do {
		$reply=<TTY>;
		# printf "get_char\r\n";
	} while (not defined $reply);
	return $reply;
}

sub move_to($$) {
	my $y = $_[0];
	my $x = $_[1];
	if ( $y < 0 ) {
		$y = 0;
	} elsif ( $x < 0 ) {
		$x = 0;
	} elsif ( $y >= $row_max ) {
		$y -= 1;
	} elsif ( $x >= $col_max ) {
		$x -= 1;
	} else {
		printf "\x1b[%d;%dH", $y + 1, $x + 1;
	}
	return ( $y, $x );
}

sub vxt_cursor() {
	my $ch;
	my $x = 0;
	my $y = 0;
	my @pages = split $text_chop, $text_blob;
	my $page = 1;

my_page:
	move_to ($y, $x);
	printf "%s", $text_mark . $pages[$page];
	move_to ($y, $x);
my_loop:
	for (;;) {
		$ch = get_char();
		if ( $ch eq "q") {
			last my_loop;
		} elsif ( $ch eq "h" ) {
			($y, $x) = move_to($y, $x - 1);
		} elsif ( $ch eq "j" ) {
			($y, $x) = move_to($y + 1, $x);
		} elsif ( $ch eq "k" ) {
			($y, $x) = move_to($y - 1, $x);
		} elsif ( $ch eq "l" ) {
			($y, $x) = move_to($y, $x + 1);
		} elsif ( $ch eq "n" ) {
			if ( $page < $#pages ) {
				$page += 1;
				goto my_page;
			} else {
				beep();
			}
		} elsif ( $ch eq "p" ) {
			if ( $page > 1 ) {
				$page -= 1;
				goto my_page;
			} else {
				beep();
			}
		} else {
			beep();
			# printf "got:%s\r\n", $ch;
		}
	}
}

sub load_text($) {
	my $source = $_[0];
	my $text;
	if ( defined($opt_x) ) {
		$text = `$source`;
	} else {
		$text = `cat $source`;
	}
	$text =~ s/\n/\r\n/g;
	if ( $text !~ $text_chop ) {
		$text = $text_mark . $text;
	}
	$text_blob = $text_blob . $text;
}

&getopts('x') || die();

while ( $#ARGV >= 0 ) {
	load_text ( shift @ARGV );
}

get_screensize();
begin_cursor();
vxt_cursor();
end_cursor("Done\n");
