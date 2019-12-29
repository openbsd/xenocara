#!/usr/bin/env perl
# $XTermId: query-dynamic.pl,v 1.6 2019/05/19 08:56:11 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
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
# Test the color-query features of xterm for dynamic-colors

use strict;
use warnings;

use Getopt::Std;
use IO::Handle;

our ( $opt_q, $opt_s, $opt_8 );

our @query_params;

our @color_names = (
    "VT100 text foreground color",
    "VT100 text background color",
    "text cursor color",
    "mouse foreground color",
    "mouse background color",
    "Tektronix foreground color",
    "Tektronix background color",
    "highlight background color",
    "Tektronix cursor color",
    "highlight foreground color"
);

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('qs8') || die(
    "Usage: $0 [options]\n
Options:\n
  -q      quicker results by merging queries
  -s      use ^G rather than ST
  -8      use 8-bit controls
"
);

our $OSC = "\x1b\]";
$OSC = "\x9d" if ($opt_8);
our $ST = $opt_8 ? "\x9c" : ( $opt_s ? "\007" : "\x1b\\" );

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

sub begin_query() {
    @query_params = ();
}

sub add_param($) {
    $query_params[ $#query_params + 1 ] = $_[0];
}

sub show_reply($) {
    my $reply = shift;
    printf "data={%s}", &visible($reply);
}

sub finish_query($) {
    return unless (@query_params);

    my $reply;
    my $n;
    my $st = $opt_8 ? qr/\x9c/ : ( $opt_s ? qr/\007/ : qr/\x1b\\/ );
    my $osc = $opt_8 ? qr/\x9d/ : qr/\x1b]/;
    my $match = qr/${osc}.*${st}/;

    my $params = join( ";", @query_params );
    $params =~ s/\d+/?/g;
    $params = sprintf( "%d;%s", $query_params[0], $params );
    $reply = &get_reply( $OSC . $params . $ST );

    printf "query{%s}", &visible($params);

    if ( defined $reply ) {
        printf " len=%2d ", length($reply);
        if ( $reply =~ /${match}/ ) {
            my @chunks = split /${st}${osc}/, $reply;
            printf "\n" if ( $#chunks > 0 );
            for my $c ( 0 .. $#chunks ) {
                $chunks[$c] =~ s/^${osc}// if ( $c == 0 );
                $chunks[$c] =~ s/${st}$//  if ( $c == $#chunks );
                my $param = $chunks[$c];
                $param =~ s/^(\d+);.*/$1/;
                $param = -1 unless ( $param =~ /^\d+$/ );
                $chunks[$c] =~ s/^\d+;//;
                printf "\t%d: ", $param if ( $#chunks > 0 );
                &show_reply( $chunks[$c] );
                printf " %s", $color_names[ $param - 10 ]
                  if (  ( $param >= 10 )
                    and ( ( $param - 10 ) <= $#color_names ) );
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

    &begin_query unless $opt_q;
    if ( $#query_params >= 0
        and ( $param != $query_params[$#query_params] + 1 ) )
    {
        &finish_query;
        &begin_query;
    }
    &add_param($param);
    &finish_query unless $opt_q;
}

sub query_colors($$) {
    my $lo = shift;
    my $hi = shift;
    my $n;
    for ( $n = $lo ; $n <= $hi ; ++$n ) {
        &query_color($n);
    }
}

printf "\x1b G" if ($opt_8);

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
    &query_colors( 10, 19 );
}

&finish_query if ($opt_q);

printf "\x1b F" if ($opt_8);

1;
