#!/usr/bin/env perl
# $XTermId: query-status.pl,v 1.10 2021/11/11 21:31:48 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2017-2019,2021 by Thomas E. Dickey
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
# Test the status features of xterm using DECRQSS.
#
# TODO: use Term::ReadKey rather than system/stty

use strict;
use warnings;

use Getopt::Std;
use IO::Handle;

our ( $opt_a, $opt_c, $opt_d, $opt_8 );

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('acd8') || die(
    "Usage: $0 [options] [suffixes]\n
Options:\n
  -a      test ANSI colors with SGR controls
  -c      test cursor appearance with DECSCUSR controls
  -d      test direct colors with SGR controls
  -8      use 8-bit controls

Options which use C1 controls may not work with UTF-8.
"
);

our $ST  = $opt_8 ? "\x9c" : "\x1b\\";
our $CSI = $opt_8 ? "\x9a" : "\x1b[";

our %suffixes;
$suffixes{DECSCA}   = '"q';
$suffixes{DECSCL}   = '"p';
$suffixes{DECSTBM}  = 'r';
$suffixes{DECSLRM}  = 's';
$suffixes{SGR}      = 'm';
$suffixes{DECSCUSR} = ' q';

sub get_reply($) {
    open TTY, "+</dev/tty" or die("Cannot open /dev/tty\n");
    autoflush TTY 1;
    my $old = `stty -g`;
    system "stty raw -echo min 0 time 5";

    print TTY @_;
    my $reply = <TTY>;
    close TTY;
    system "stty $old";
    if ( defined $reply ) {
        die("^C received\n") if ( "$reply" eq "\003" );
    }
    return $reply;
}

sub visible($) {
    my $reply = $_[0];
    my $n;
    my $result = "";
    for ( $n = 0 ; $n < length($reply) ; ) {
        my $c = substr( $reply, $n, 1 );
        if ( $c =~ /[[:print:]]/ ) {
            $result .= $c;
        }
        else {
            my $k = ord substr( $reply, $n, 1 );
            if ( ord $k == 0x1b ) {
                $result .= "\\E";
            }
            elsif ( $k == 0x7f ) {
                $result .= "^?";
            }
            elsif ( $k == 32 ) {
                $result .= "\\s";
            }
            elsif ( $k < 32 ) {
                $result .= sprintf( "^%c", $k + 64 );
            }
            elsif ( $k > 128 ) {
                $result .= sprintf( "\\%03o", $k );
            }
            else {
                $result .= chr($k);
            }
        }
        $n += 1;
    }

    return $result;
}

sub query_one($) {
    my $name = shift;

    return unless $suffixes{$name};

    my $suffix = $suffixes{$name};
    my $prefix = $opt_8 ? "\x90" : "\x1bP";
    my $st     = $opt_8 ? "\x9c" : qr/\x1b\\/;
    my $DCS    = qr/${prefix}/;
    my $match  = qr/${DCS}.*${st}/;
    my $reply  = get_reply( $prefix . '$q' . $suffix . $ST );

    printf "%-10s query{%s}%*s", $name,    #
      &visible($suffix),                   #
      4 - length($suffix), " ";

    if ( defined $reply ) {
        printf "%2d ", length($reply);
        if ( $reply =~ /${match}/ ) {

            $reply =~ s/^${DCS}//;
            $reply =~ s/^;//;
            $reply =~ s/${st}$//;
        }
        else {
            printf "? ";
        }

        printf "{%s}", visible($reply);
    }
    printf "\n";
}

sub ansi_color($) {
    my $color = shift;
    return $color;
}

sub direct_color($) {
    my $color  = shift;
    my $result = "8:2:";
    $result .= ( $color & 4 ) ? ":255" : ":0";
    $result .= ( $color & 2 ) ? ":255" : ":0";
    $result .= ( $color & 1 ) ? ":255" : ":0";
    return $result;
}

sub default_colors() {
    return "39;49";
}

printf "\x1b G" if ($opt_8);

if ( $#ARGV >= 0 ) {
    while ( $#ARGV >= 0 ) {
        &query_one( shift @ARGV );
    }
}
elsif ($opt_a) {
    for my $fg ( 0 .. 7 ) {
        printf "%s3%sm", $CSI, &ansi_color($fg);
        for my $bg ( 0 .. 7 ) {
            printf "%s4%sm", $CSI, &ansi_color($bg);
            &query_one("SGR");
        }
    }
    printf "%s%sm", $CSI, &default_colors;
}
elsif ($opt_c) {
    for my $c ( 0 .. 6 ) {
        printf "%s%d q", $CSI, $c;
        &query_one("DECSCUSR");
    }
    printf "%s q", $CSI;
}
elsif ($opt_d) {
    for my $fg ( 0 .. 7 ) {
        printf "%s3%sm", $CSI, &direct_color($fg);
        for my $bg ( 0 .. 7 ) {
            printf "%s4%sm", $CSI, &direct_color($bg);
            &query_one("SGR");
        }
    }
    printf "%s39;49m", $CSI;
}
else {
    for my $key ( sort keys %suffixes ) {
        &query_one($key);
    }
}

printf "\x1b F" if ($opt_8);
