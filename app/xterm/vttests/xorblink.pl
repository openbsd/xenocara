#!/usr/bin/env perl
# $XTermId: xorblink.pl,v 1.16 2017/12/24 21:03:54 tom Exp $
# -----------------------------------------------------------------------------
# Copyright 2017 by Thomas E. Dickey
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
# walk through the different states of cursor-blinking, with annotation
#
# Manual:
#        +bc     turn off text cursor blinking.  This overrides the cursorBlink
#                resource.
#
#        -bc     turn on text cursor blinking.  This overrides the cursorBlink
#                resource.
#
#        cursorBlink (class CursorBlink)
#                Specifies whether to make the cursor blink.  The default is
#                "false".
#
#                Xterm-dev uses two variables to determine whether the cursor
#                blinks.  One is set by this resource.  The other is set by
#                control sequences (private mode 12 and DECSCUSR).  Xterm-dev
#                tests the XOR of the two variables.
#
#               Enable Blinking Cursor (resource cursorblink)
#                      Enable (or disable) the blinking-cursor feature.  This
#                      corresponds to the -bc option and the cursorBlink
#                      resource.  There is also an escape sequence (see Xterm-
#                      dev Control Sequences).  The menu entry and the escape
#                      sequence states are XOR'd: if both are enabled, the
#                      cursor will not blink, if only one is enabled, the cursor
#                      will blink.
#
#        set-cursorblink(on/off/toggle)
#                This action sets, unsets or toggles the cursorBlink resource.
#                It is also invoked from the cursorblink entry in vtMenu.
#
# Control sequences:
#
# CSI ? Pm h
#           DEC Private Mode Set (DECSET).
#             Ps = 1 2  -> Start Blinking Cursor (att610).
#
# CSI ? Pm l
#           DEC Private Mode Reset (DECRST).
#             Ps = 1 2  -> Stop Blinking Cursor (att610).
#
# CSI Ps SP q
#           Set cursor style (DECSCUSR, VT520).
#             Ps = 0  -> blinking block.
#             Ps = 1  -> blinking block (default).
#             Ps = 2  -> steady block.
#             Ps = 3  -> blinking underline.
#             Ps = 4  -> steady underline.
#             Ps = 5  -> blinking bar (xterm).
#             Ps = 6  -> steady bar (xterm).
#
use strict;

use Term::ReadKey;

use IO::Handle;
STDERR->autoflush(1);
STDOUT->autoflush(1);

our %DECSET = (
    "\e[?12h", "Start Blinking Cursor (AT&T 610)",
    "\e[?12l", "Stop Blinking Cursor (AT&T 610)"
);

our %DECSCUSR = (
    "\e[0 q",
    "blinking block",
    "\e[1 q",
    "blinking block (default)",
    "\e[2 q",
    "steady block",
    "\e[3 q",
    "blinking underline",
    "\e[4 q",
    "steady underline",
    "\e[5 q",
    "blinking bar (xterm)",
    "\e[6 q",
    "steady bar (xterm)"
);

sub show($$) {
    my $seq = shift;
    my $txt = shift;
    printf "%s -> %s\n", &visible($seq), $txt;
}

sub get_reply($$) {
    my $seq = shift;
    my $end = shift;
    printf STDERR "%s", $seq;
    my $key;
    my $result = "";
    $key = ReadKey(0);
    $result .= $key;
    if ( $key eq "\e" ) {

        while (1) {
            $key = ReadKey(100);
            $result .= $key;
            next if ( length($result) < length($end) );
            last if ( substr( $result, -length($end) ) eq $end );
        }
    }
    return $result;
}

sub mode_value($) {
    my $value = shift;
    if ( $value eq 1 ) {
        $value = "set";
    }
    elsif ( $value eq 2 ) {
        $value = "reset";
    }
    elsif ( $value eq 3 ) {
        $value = "*set";
    }
    elsif ( $value eq 4 ) {
        $value = "*reset";
    }
    else {
        $value = &visible( "?" . $value );
    }
    return $value;
}

sub DECRQM($) {
    my $mode     = shift;
    my $sequence = sprintf( "\e[?%d\$p", $mode );
    my $reply    = &get_reply( $sequence, "y" );
    if ( $reply =~ /^\e\[\?$mode;\d+\$y$/ ) {
        $reply =~ s/^\e\[\?$mode;(\d+)\$y$/$1/;
    }
    return &mode_value($reply);
}

sub DECRQSS($) {
    my $request  = shift;
    my $ending   = "\e\\";
    my $sequence = sprintf( "\eP\$q%s$ending", $request );
    my $reply    = &get_reply( $sequence, $ending );

    # xterm responds with
    # DCS 1 $ r Pt ST for valid requests,
    # DCS 0 $ r Pt ST for invalid requests.
    #if ( $reply =~ /^\eP1\$r.*$ending$/ ) {
    if ( $reply =~ /^\eP1\$r\d+ q\e\\$/ ) {
        $reply =~ s/^\eP1\$r(\d+) q\e\\$/$1/;
    }
    return &visible($reply);
}

sub get_key() {
    my $key;
    do {
        $key = ReadKey(0);
        if ( $key eq "\e" ) {
            while ( ReadKey(10) !~ /[@-~]/ ) {
                #
            }
        }
    } while ( $key eq "\e" );
    return $key;
}

sub visible($) {
    my $txt = shift;
    $txt =~ s/\e/\\e/g;
    $txt =~ s/\a/\\a/g;
    return $txt;
}

sub test($$) {
    my $set = shift;
    my $msg = shift;

    ReadMode 'raw';

    printf STDERR "%s\t[", &visible($set);

    # save the cursor position
    printf STDERR "\e7";

    # send the escape sequence
    printf STDERR "%s", $set;

    # print the description
    printf STDERR "X] ";

    printf STDERR " [C=%s,",  &DECRQSS(" q");
    printf STDERR "B=%s,",    &DECRQM(12);
    printf STDERR "M=%s,%s]", &DECRQM(13), &DECRQM(14);
    printf STDERR " %s",      $msg;
    printf STDERR "\e[0J";

    # restore the cursor position
    printf STDERR "\e8";

    # wait for any key
    my $key = &get_key;
    ReadMode 'restore';

    # print newline
    printf STDERR "\n";

    # A backspace response makes the current line reprint (to test menus)
    return ( $key ne "\b" and $key ne "\177" ) ? 1 : 0;
}

if ( -t STDOUT ) {
    printf "Legend:\n";
    printf "  C = cursor shape (1,2 block, 3,4 underline, 5,6 left-bar)\n";
    printf "  B = escape-sequence blink\n";
    printf "  M = menu blink and XOR mode\n";
    printf "\n";
    printf "An asterisk means the mode is always set or reset.\n";
    printf "Press any key to proceed; press backspace to reprint line.\n";
    printf "\n";
    my @DECSET   = sort keys %DECSET;
    my @DECSCUSR = sort keys %DECSCUSR;

    for ( my $h = 0 ; $h <= $#DECSET ; ++$h ) {
        $h-- unless &test( $DECSET[$h], $DECSET{ $DECSET[$h] } );
    }
    for my $l ( 0 .. $#DECSCUSR ) {
        $l-- unless &test( $DECSCUSR[$l], $DECSCUSR{ $DECSCUSR[$l] } );
    }
}
else {
    printf "DECSET (AT&T 610 blinking cursor):\n";
    for my $key ( sort keys %DECSET ) {
        &show( $key, $DECSET{$key} );
    }

    printf "DECSCUSR:\n";
    for my $key ( sort keys %DECSCUSR ) {
        &show( $key, $DECSCUSR{$key} );
    }
}
1;
