#!/usr/bin/env perl
# $XTermId: query-allowed.pl,v 1.8 2025/06/23 00:09:50 tom Exp $
# -----------------------------------------------------------------------------
# Copyright 2022,2025 by Thomas E. Dickey
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
# Exercise OSC 60 and 61 which report allowed- and disallowed-operations.
# Use OSC 62 to provide some context of the OSC 61 responses.

use strict;

use Getopt::Std;
use IO::Handle;

$| = 1;

our ( $opt_a, $opt_v, $opt_8 );

sub main::HELP_MESSAGE() {
    printf STDERR <<EOF
Usage: $0 [options]
Options:
  -8      use 8-bit controls
  -a      test all settings, resetting
  -v      verbose, show responses
EOF
      ;
    exit 1;
}

$Getopt::Std::STANDARD_HELP_VERSION = 1;
getopts('av8') || &main::HELP_MESSAGE;

our $version = 401;    # xterm #401 supports OSC 62

our $CSI = "\x1b\[";
our $ST  = "\x1b\\";
our $OSC = "\x1b\]";
$CSI = "\x9b" if ($opt_8);
$ST  = "\x9c" if ($opt_8);
$OSC = "\x9d" if ($opt_8);

sub no_reply($) {
    open TTY, "+</dev/tty" or die("Cannot open /dev/tty\n");
    autoflush TTY 1;
    my $old = `stty -g`;
    system "stty raw -echo min 0 time 5";

    print TTY @_;
    close TTY;
    system "stty $old";
}

sub get_reply($) {
    my $query = shift;
    printf "Query: %s\n", visible($query) if $opt_v;
    open TTY, "+</dev/tty" or die("Cannot open /dev/tty\n");
    autoflush TTY 1;
    my $old = `stty -g`;
    system "stty raw -echo min 0 time 5";

    print TTY $query;
    my $reply = <TTY>;
    close TTY;
    system "stty $old";
    if ( defined $reply ) {
        die("^C received\n") if ( "$reply" eq "\003" );
    }
    printf "Reply: %s\n", visible($reply) if $opt_v;
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

sub extract_list($$) {
    my $prefix = shift;
    my $result = shift;
    my @result = ();
    if (    $result ne ""
        and index( $result, $prefix ) == 0
        and index( $result, $ST ) == length($result) - length($ST) )
    {
        $result = substr( $result, length($prefix) );
        $result = substr( $result, 0, length($result) - length($ST) );
        @result = split /,/, $result;
    }
    else {
        $prefix =~ s/;$//;
        my $check = $prefix . $ST;
        printf "? unexpected reply: %s\n", visible($result)
          if $result ne $check;
    }
    return @result;
}

sub report_list($$) {
    my $label  = $_[0];
    my @result = @{ $_[1] };
    if ( $#result >= 0 ) {
        my $prefix = "      ";
        my $result = $prefix . $label . ":";
        $prefix .= " ";
        for my $n ( 0 .. $#result ) {
            my $value = visible( $result[$n] );
            if ( length( $result . $value ) >= 72 ) {
                printf "%s\n", $result;
                $result = $prefix;
            }
            $result .= " " . $value;
        }
        printf "%s\n", $result unless ( $result eq $prefix );
    }
}

sub GetCategories() {
    my $prefix = sprintf( "%s60", $OSC );
    my $result = get_reply( $prefix . $ST );
    return extract_list( $prefix . ";", $result );
}

sub GetDisallowed($) {
    my $category = shift;
    my $prefix   = sprintf( "%s61;", $OSC );
    my $result   = get_reply( $prefix . $category . $ST );
    return extract_list( $prefix, $result );
}

sub GetAllowable($) {
    my $category = shift;
    my $prefix   = sprintf( "%s62;", $OSC );
    my $result   = get_reply( $prefix . $category . $ST );
    return extract_list( $prefix, $result );
}

sub GetVersion() {
    my $result = get_reply( $CSI . ">0q" );
    if ( $result =~ /XTerm\(\d+\)/ ) {
        $version =~ s/^.*XTerm\((\d+).*/$1/;
    }
}

sub doit() {
    my %categories = qw(
      allowColorOps 0
      allowFontOps 0
      allowMouseOps 0
      allowPasteControls 0
      allowTcapOps 0
      allowTitleOps 0
      allowWindowOps 0
    );
    my @list = GetCategories;
    printf "Enabled categories:\n" if $#list >= 0;

    for my $n ( 0 .. $#list ) {
        my $category = $list[$n];
        printf " %3d: %s\n", $n + 1, $category;
        if ( defined $categories{$category} ) {
            $categories{$category} = 1;
        }
        else {
            printf "? unexpected category\n";
            next;
        }
        if ($opt_a) {
            my @disallow = GetDisallowed($category);
            report_list( "Disallow", \@disallow );
            if ( $version > 400 ) {
                my @allowable = GetAllowable($category);
                report_list( "Allowable", \@allowable );
            }
        }
    }
    my $extra = 0;
    for my $keys ( keys %categories ) {
        if ( $categories{$keys} == 0 ) {
            $extra = 1;
            last;
        }
    }
    if ($extra) {
        printf "Disabled categories:\n";
        for my $category ( keys %categories ) {
            if ( $categories{$category} == 0 ) {
                my @allowable = GetAllowable($category);
                report_list( $category, \@allowable );
            }
        }
    }
}

printf "\x1b G" if ($opt_8);

GetVersion;
doit;

printf "\x1b F" if ($opt_8);

1;
