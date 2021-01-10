#!/usr/bin/env perl
# $XTermId: query-color.pl,v 1.26 2020/12/13 18:17:40 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2012-2019,2020 by Thomas E. Dickey
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
# Test the color-query features of xterm using OSC 4 or OSC 5.

use strict;
use warnings;

use Getopt::Std;
use IO::Handle;

our ( $opt_4, $opt_a, $opt_n, $opt_q, $opt_r, $opt_s, $opt_t );

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('4an:qrst') || die(
    "Usage: $0 [options] [color1[-color2]]\n
Options:\n
  -4      use OSC 4 for special colors rather than OSC 5
  -a      query all \"ANSI\" colors
  -n NUM  assume terminal supports NUM \"ANSI\" colors rather than 256
  -q      quicker results by merging queries
  -r      show reported color in #rrggbb format
  -s      use ^G rather than ST
  -t      show actual color
"
);

our $ST              = $opt_s ? "\007" : "\x1b\\";
our $num_ansi_colors = $opt_n ? $opt_n : 256;

our $last_op = -1;
our $this_op = -1;

our @query_params;

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

sub special2code($) {
    my $param = shift;
    $param = 0 if ( $param =~ /^bold$/i );
    $param = 1 if ( $param =~ /^underline$/i );
    $param = 2 if ( $param =~ /^blink$/i );
    $param = 3 if ( $param =~ /^reverse$/i );
    $param = 4 if ( $param =~ /^italic$/i );
    return $param;
}

sub code2special($) {
    my $param = shift;
    my $result;
    $result = "bold"      if ( $param == 0 );
    $result = "underline" if ( $param == 1 );
    $result = "blink"     if ( $param == 2 );
    $result = "reverse"   if ( $param == 3 );
    $result = "italic"    if ( $param == 4 );
    return $result;
}

sub begin_query() {
    @query_params = ();
}

sub add_param($) {
    $query_params[ $#query_params + 1 ] = $_[0];
}

sub show_reply($) {
    my $reply = shift;
    my $shown = sprintf "data={%s}", &visible($reply);
    my $limit = 30;
    if ( $reply =~ /^\d+;rgb:.*/ ) {
        my $color = $reply;
        $color =~ s/^\d+;rgb://;
        if ( $color =~ /^[[:xdigit:]]{4}(\/[[:xdigit:]]{4}){2}/ ) {
            $color =~ s/..$//;
            $color =~ s/..\///g;
            if ($opt_r) {
                $shown = sprintf "#%s", $color;
                $limit = 7;
            }
        }
        printf "%s", $shown;
        if ( $opt_4 or ( $this_op == 5 ) ) {
            my $num = $reply;
            my $max = $opt_4 ? $num_ansi_colors : 0;
            $num =~ s/;.*//;
            if ( $num >= $max ) {
                my $name = &code2special( $num - $max );
                printf "  %s", $name if ($name);
            }
        }
        if ($opt_t) {
            my $num = $reply;
            $num =~ s/;.*//;
            printf "%*s", $limit - length($shown), " ";
            if ( $num < 8 ) {
                printf "\x1b[%dm", 40 + $num;
            }
            elsif ( $num < 16 ) {
                printf "\x1b[%dm", 100 + $num - 8;
            }
            elsif ( $num < $num_ansi_colors ) {
                printf "\x1b[48;5;%dm", $num;
            }
            else {
            }
            printf "   ";
            printf "\x1b[K";
            printf "\x1b[m";
        }
    }
    else {
        printf "%s", $shown;
    }
}

sub finish_query() {
    my $query;
    my $reply;
    my $n;
    my $st    = $opt_s ? qr/\007/ : qr/\x1b\\/;
    my $osc   = qr/\x1b]$this_op/;
    my $match = qr/^(${osc}.*${st})+$/;

    my $params = sprintf "%s;?;", ( join( ";?;", @query_params ) );
    $query = "\x1b]$this_op;" . $params . $ST;
    $reply = &get_reply($query);

    if ($opt_q) {
        printf "query %s\n", &visible($query);
    }
    else {
        printf "query %s%*s ", &visible($query),
          15 - length( &visible($query) ),
          " ";
    }

    if ( defined $reply ) {
        printf "reply len=%2d ", length($reply);
        if ( $reply =~ /${match}/ ) {
            my @chunks = split /${st}${osc}/, $reply;
            printf "\n" if ( $#chunks > 0 );
            for my $c ( 0 .. $#chunks ) {
                $chunks[$c] =~ s/^${osc}// if ( $c == 0 );
                $chunks[$c] =~ s/${st}$//  if ( $c == $#chunks );
                $chunks[$c] =~ s/^;//;
                printf "%3d: ", $c if ( $#chunks > 0 );
                &show_reply( $chunks[$c] );
                printf "\n" if ( $c < $#chunks );
            }
        }
        else {
            printf "? ";
            &show_reply($reply);
        }
    }
    printf "\n";
}

sub query_color($) {
    my $param = shift;
    my $op    = 4;

    if ( $param !~ /^\d+$/ ) {
        $param = &special2code($param);
        if ( $param !~ /^\d+$/ ) {
            printf STDERR "? not a color name or code: $param\n";
            return;
        }
        if ($opt_4) {
            $param += $num_ansi_colors;
        }
        else {
            $op = 5;
        }
    }
    $this_op = $op;    # FIXME handle mixed OSC 4/5

    &begin_query unless $opt_q;
    &add_param($param);
    &finish_query unless $opt_q;
}

sub query_colors($$) {
    my $lo = shift;
    my $hi = shift;
    if ( $lo =~ /^\d+$/ ) {
        my $n;
        for ( $n = $lo ; $n <= $hi ; ++$n ) {
            &query_color($n);
        }
    }
    else {
        &query_color($lo);
        &query_color($hi) unless ( $hi eq $lo );
    }
}

&begin_query if ($opt_q);

if ( $#ARGV >= 0 ) {
    while ( $#ARGV >= 0 ) {
        if ( $ARGV[0] =~ /-/ ) {
            my @args = split /-/, $ARGV[0];
            &query_colors( $args[0], $args[1] );
        }
        else {
            &query_colors( $ARGV[0], $ARGV[0] );
        }
        shift @ARGV;
    }
}
else {
    &query_colors( 0, $opt_a ? $num_ansi_colors : 7 );
}

&finish_query if ($opt_q);

1;
