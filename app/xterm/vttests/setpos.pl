#!/usr/bin/env perl
# $XTermId: setpos.pl,v 1.18 2019/05/26 23:19:29 tom Exp $
# -----------------------------------------------------------------------------
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
# Exercise CSI 3/13 t which set/get the window position.

use strict;

use Getopt::Std;
use IO::Handle;

$| = 1;

our ( $opt_a, $opt_n, $opt_p, $opt_v, $opt_x, $opt_8 );
our $default_y = 100;
our $default_x = 150;

sub main::HELP_MESSAGE() {
    printf STDERR <<EOF
Usage: $0 [options]
Options:
  -8      use 8-bit controls
  -a      test position/report for middle and four corners
  -n N    repeat unless -a option used (default: 3)
  -p Y,X  use this position rather than $default_y,$default_x
  -v      verbose
  -x      report xwininfo's position for \$WINDOWID
EOF
      ;
    exit 1;
}

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('an:p:vx8') || &main::HELP_MESSAGE;

our $repeat = 3;
$repeat = $opt_n if ($opt_n);
&main::HELP_MESSAGE unless ( $repeat =~ /^\d+$/ );

our $CSI = "\x1b\[";
$CSI = "\x9b" if ($opt_8);

if ($opt_p) {
    &main::HELP_MESSAGE unless ( $opt_p =~ /^[-]?\d+,[-]?\d+$/ );
    my @coord = split /,/, $opt_p;
    $default_y = $coord[0];
    $default_x = $coord[1];
}

our $wm_name = "unknown";
our @extents;

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

sub read_cmd($) {
    my $cmd = shift;
    my @result;
    if ( open my $fh, "$cmd |" ) {
        @result = <$fh>;
        close $fh;
        chomp @result;
    }
    return @result;
}

sub get_xprop($$) {
    my $id   = shift;
    my $name = shift;
    my @data = &read_cmd("xprop -id $id");
    my $prop = "";
    for my $n ( 0 .. $#data ) {
        if ( $data[$n] =~ /$name\([^)]+\) =/ ) {
            $prop = $data[$n];
            $prop =~ s/^[^=]*=\s*//;
            $prop =~ s/"//g;
            last;
        }
    }
    return $prop;
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

sub limited($) {
    my $value = shift;
    if ( $value >= 65536 ) {
        $value %= 65536;
    }
    if ( $value >= 32768 ) {
        $value -= 65536;
    }
    return $value;
}

sub check_position($$$) {
    my $name   = shift;
    my $expect = shift;
    my $actual = shift;
    printf " ?%s:%d", $name, $expect if ( $expect != $actual );
}

sub report_position() {
    my $reply = &get_reply( sprintf "%s13t", $CSI );
    my $status = 0;
    my @result;
    if ( index( $reply, $CSI ) == 0 ) {
        $reply = substr( $reply, length($CSI) );
        $status = 1;
    }
    if ( $reply =~ /^3;\d+;\d+t$/ ) {
        my $y = $reply;
        $y =~ s/^3;(\d+);.*/$1/;
        my $x = $reply;
        $x =~ s/^3;\d+;(\d+).*/$1/;
        $result[0] = &limited($y);
        $result[1] = &limited($x);
        printf "OK ->%s ->%d,%d", &visible($reply), $result[0], $result[1];
    }
    else {
        printf "ERR ->%s", &visible($reply);
    }
    if ( $opt_x and $ENV{WINDOWID} ) {
        my @actual = `xwininfo -id $ENV{WINDOWID} | grep " upper-left [XY]:"`;
        for my $n ( 0 .. $#actual ) {
            $actual[$n] =~ s/^.*:\s+//;
        }
        if ( $#actual == 3 ) {
            printf " abs(%d,%d) rel(%d,%d)", $actual[0], $actual[1],
              $actual[2], $actual[3]
              if ($opt_v);
            my $expect_y;
            my $expect_x;
            if ( $wm_name =~ /^gnome/i ) {
                $expect_x = $actual[0] - ( $extents[0] + $extents[1] );
                $expect_y = $actual[1] - ( $extents[2] + $extents[3] );
            }
            elsif ( $#extents == 3
                and ( $wm_name !~ /^fvwm/i )
                and ( $wm_name !~ /^enlightenment/i ) )
            {
                $expect_x = $actual[0] - ( $extents[0] );
                $expect_y = $actual[1] - ( $extents[2] );
            }
            else {
                $expect_x = $actual[0] - $actual[2];
                $expect_y = $actual[1] - $actual[3];
            }
            if ( $#result > 0 ) {
                &check_position( "X", $expect_x, $result[0] );
                &check_position( "Y", $expect_y, $result[1] );
            }
        }
    }
    printf "\n";
    return @result;
}

sub update_position() {
    my @pos = @{ $_[0] };
    printf "** update %d,%d\n", $pos[0], $pos[1];
    $pos[0] += 65536 if ( $pos[0] < 0 );
    $pos[1] += 65536 if ( $pos[1] < 0 );
    &no_reply( sprintf "%s3;%d;%dt", $CSI, $pos[0], $pos[1] );
}

sub update_and_report($) {
    my @pos = @{ $_[0] };
    &update_position( \@pos );
    sleep 1 if ($opt_a);
    return &report_position;
}

sub get_screensize() {
    my $reply = &get_reply( sprintf "%s15t", $CSI );
    my @result;
    if ( index( $reply, $CSI ) == 0 ) {
        $reply = substr( $reply, length($CSI) );
        if ( $reply =~ /^5;\d+;\d+t$/ ) {
            my $y = $reply;
            $y =~ s/^5;(\d+);.*/$1/;
            my $x = $reply;
            $x =~ s/^5;\d+;(\d+).*/$1/;
            $result[0] = $x;
            $result[1] = $y;
        }
    }
    return @result;
}

sub doit() {
    my @old = &report_position;
    if ($opt_a) {
        my @size = &get_screensize;
        if (@size) {
            printf "Screen %dx%d\n", $size[0], $size[1];
            my $ulx = -$default_x;
            my $uly = -$default_y;
            my $lrx = $size[0] - $default_x;
            my $lry = $size[1] - $default_y;
            &update_and_report( [ $ulx, $uly ] );
            &update_and_report( [ $ulx, $lry ] );
            &update_and_report( [ $lrx, $lry ] );
            &update_and_report( [ $lrx, $uly ] );
            &update_position( \@old );
        }
    }
    else {
        my @pos = ( $default_y, $default_x );
        for my $n ( 1 .. $repeat ) {
            @pos = &update_and_report( \@pos );
        }
    }
}

printf "\x1b G" if ($opt_8);

if ( $opt_x and $ENV{WINDOWID} ) {
    my $extents = &get_xprop( $ENV{WINDOWID}, "_NET_FRAME_EXTENTS" );
    if ( $extents ne "" ) {
        @extents = split /,\s*/, $extents;
        printf "** has EWMH extents: $extents\n";
        my $supwin = `xprop -root '_NET_SUPPORTING_WM_CHECK'`;
        if ( $supwin ne "" ) {
            $supwin =~ s/^.*(0x[[:xdigit:]]+).*/$1/;
            $wm_name = &get_xprop( $supwin, "_NET_WM_NAME" );
            $wm_name = "unknown" unless ( $wm_name ne "" );
            printf "** using \"$wm_name\"\n";
        }
    }
}

&doit;

printf "\x1b F" if ($opt_8);

1;
