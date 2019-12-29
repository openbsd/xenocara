#!/usr/bin/env perl
# $XTermId: query-feature.pl,v 1.6 2019/10/06 23:56:18 tom Exp $
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
# Report features enabled/disabled via resource-settings

# TODO: handle 8-bit controls

use strict;
use warnings;

use Getopt::Std;
use IO::Handle;

our ( $opt_a, $opt_d, $opt_e, $opt_m, $opt_q );

our @query_params;
our @query_result;

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('acdemq') || die(
    "Usage: $0 [options]\n
Options:\n
  -a      (same as -d -e -m)
  -d      query disabled/disallowed features
  -e      query enabled/allowed features
  -m      query modified keys
  -q      quicker results by merging queries
"
);

if (
    $#ARGV < 0
    and not( defined($opt_d)
        or defined($opt_e)
        or defined($opt_m) )
  )
{
    $opt_a = 1;
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

sub hexified($) {
    my $value  = $_[0];
    my $result = "";
    my $n;

    for ( $n = 0 ; $n < length($value) ; ++$n ) {
        $result .= sprintf( "%02X", ord substr( $value, $n, 1 ) );
    }
    return $result;
}

sub begin_query() {
    @query_params = ();
}

sub add_param($) {
    $query_params[ $#query_params + 1 ] = &hexified( $_[0] );
}

sub finish_query() {
    my $reply = &get_reply( "\x1bP+Q" . join( ';', @query_params ) . "\x1b\\" );

    return unless defined $reply;
    while ( $reply =~ /\x1bP1\+R[[:xdigit:]]+[=;][[:xdigit:]]*.*\x1b\\/ ) {
        my $n;
        my $parse;

        $reply =~ s/^\x1bP1\+R//;
        $parse = $reply;
        $reply =~ s/\x1b\\.*$//;
        $parse = substr( $parse, length($reply) );
        $parse =~ s/^\x1b\\//;

        my $result = "";
        my $count  = 0;
        my $state  = 0;
        my $error  = "?";
        for ( $n = 0 ; $n < length($reply) ; ) {
            my $c = substr( $reply, $n, 1 );

            if ( $c eq ';' ) {
                $n += 1;
                printf "%d%s\t%s\n", $count, $error, $result
                  if ( $result ne "" );
                $result = "";
                $state  = 0;
                $error  = "?";
                $count++;
            }
            elsif ( $c eq '=' ) {
                $error = ""
                  if (  $count <= $#query_params
                    and &hexified($result) eq $query_params[$count] );
                $n += 1;
                $result .= $c;
                $state = 1;
            }
            elsif ( $c =~ /[[:punct:]]/ ) {
                $n += 1;
                $result .= $c;
            }
            else {
                my $k = hex substr( $reply, $n, 2 );
                if ( $k == 0x1b ) {
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
                $n += 2;
            }
        }
        printf "%d%s\t%s\n", $count, $error, $result if ( $result ne "" );
        $reply = $parse;
    }
}

sub do_query($) {
    my $name = shift;

    &begin_query unless ($opt_q);
    &add_param($name);
    &finish_query unless ($opt_q);
}

&begin_query if ($opt_q);

while ( $#ARGV >= 0 ) {
    &do_query( shift @ARGV );
}

if ( defined($opt_a) || defined($opt_d) ) {
    &do_query("disallowedColorOps");
    &do_query("disallowedFontOps");
    &do_query("disallowedMouseOps");
    &do_query("disallowedPasteControls");
    &do_query("disallowedTcapOps");
    &do_query("disallowedWindowOps");
}

if ( defined($opt_a) ) {
    &do_query("allowSendEvents");
    &do_query("allowPasteControls");
    &do_query("allowC1Printable");
    &do_query("saveLines");
}

if ( defined($opt_a) || defined($opt_e) ) {
    &do_query("allowColorOps");
    &do_query("allowFontOps");
    &do_query("allowMouseOps");
    &do_query("allowPasteControls");
    &do_query("allowTcapOps");
    &do_query("allowTitleOps");
    &do_query("allowWindowOps");
}

if ( defined($opt_a) || defined($opt_m) ) {
    &do_query("formatOtherKeys");
    &do_query("modifyCursorKeys");
    &do_query("modifyFunctionKeys");
    &do_query("modifyKeyboard");
    &do_query("modifyOtherKeys");
}

&finish_query if ($opt_q);

1;
