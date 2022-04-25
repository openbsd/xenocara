#!/usr/bin/perl -w
# $XTermId: nrcs.pl,v 1.10 2013/09/08 19:46:07 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2013 by Thomas E. Dickey
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
# This is a more direct way of exercising character sets than vttest.

use strict;
use File::Temp qw/ tempdir /;

our $prepare_GL = "\x1bo";       # Invoke the G3 Character Set as GL (LS3).
our $prepare_GR = "\x1b|";       # Invoke the G3 Character Set as GR (LS3R).
our $restore_GL = "\017";        # Invoke the G1 Character Set as GR (LS1R).
our $restore_GR = "\x1b~";       # Invoke the G1 Character Set as GR (LS1R).
our $enable_NRC = "\x1b[?42h";

our $dummy;
our %level;
our %suffix;
our %short_name;
our %long_name;

sub select_G3() {
    printf "\x1b+%s", shift;
}

sub show_charset($) {
    my $alias  = shift;
    my $suffix = $suffix{$alias};
    return if not $suffix;
    printf $enable_NRC;

    &select_G3($suffix);
    printf $prepare_GL;
    printf $prepare_GR;

    printf "GL:\n";
    for my $n ( 32 .. 126 ) {
        print chr($n);
        printf "\n" if ( ( ( $n + 1 ) % 32 ) == 0 );
    }
    printf "\nGR:\n";
    for my $n ( 160 .. 255 ) {
        print chr($n);
        printf "\n" if ( ( ( $n + 1 ) % 32 ) == 0 );
    }

    do {
        $dummy = `sh -c 'read dummy; echo "\$dummy"'`;
        chomp $dummy;
    } until $dummy =~ /^\s*$/;

    printf $restore_GL;
    printf $restore_GR;
}

sub list_charset($$$$) {
    my $level      = shift;
    my $suffix     = shift;
    my $short_name = shift;
    my $long_name  = shift;
    my $alias      = lc $short_name;
    $level{$alias}      = $level;
    $suffix{$alias}     = $suffix;
    $short_name{$alias} = $short_name;
    $long_name{$alias}  = $long_name;
}

sub initialize() {
    &list_charset( 1, '0',  "graphic",      "DEC Line Drawing Set" );
    &list_charset( 2, '<',  "supp",         "DEC Supplementary" );
    &list_charset( 3, '%5', "supp_graphic", "DEC Supplementary Graphics" );
    &list_charset( 3, '>',  "technical",    "DEC Technical" );
    &list_charset( 3, 'A',  "latin_1",      "United Kingdom (UK)" );
    &list_charset( 1, 'B',  "ascii",        "United States (USASCII)" );
    &list_charset( 2, '4',  "dutch",        "Dutch" );
    &list_charset( 2, '5',  "finnish",      "Finnish" );
    &list_charset( 2, 'C',  "finnish2",     "Finnish" );
    &list_charset( 2, 'R',  "french",       "French" );
    &list_charset( 2, 'f',  "french2",      "French" );
    &list_charset( 2, 'Q',  "canadian",     "French Canadian " );
    &list_charset( 2, '9',  "canadian2",    "French Canadian " );
    &list_charset( 2, 'K',  "german",       "German" );
    &list_charset( 2, 'Y',  "italian",      "Italian" );
    &list_charset( 3, '`',  "danish",       "Norwegian/Danish " );
    &list_charset( 2, 'E',  "danish2",      "Norwegian/Danish" );
    &list_charset( 2, '6',  "danish3",      "Norwegian/Danish" );
    &list_charset( 3, '%6', "portuguese",   "Portuguese " );
    &list_charset( 2, 'Z',  "spanish",      "Spanish" );
    &list_charset( 2, '7',  "swedish",      "Swedish" );
    &list_charset( 2, 'H',  "swedish2",     "Swedish" );
    &list_charset( 2, '=',  "swiss",        "Swiss" );
}

sub show_dialog() {
    my $dir      = tempdir( CLEANUP => 1 );
    my $in_file  = "$dir/input";
    my $out_file = "$dir/output";
    my $exe_file = "$dir/script";
    my $rc_file  = "$dir/status";
    my $output   = "";
    my $status;
    do {
        open( FP, ">$in_file" ) || die("cannot create $in_file");
        print FP "#!/bin/sh\n";
        print FP "dialog";
        printf FP "\\\n\t--default-item \"%s\"", $output if ( $output ne "" );
        print FP "\\\n\t--menu \"Select a character set\" 0 0 0 ";

        foreach my $key ( sort keys %short_name ) {
            printf FP "\\\n\t%s \"VT%d00: %s\"", $short_name{$key},
              $level{$key},
              $long_name{$key};
        }
        printf FP "\\\n 2>$out_file\n";
        printf FP "echo \$? >$rc_file\n";
        close FP;
        chmod 0700, $in_file;
        system("$in_file");
        $output = `cat $out_file`;
        $status = `cat $rc_file`;
        chomp $output;
        chomp $status;
        &show_charset($output) if ( $status == 0 );
    } while ( $status ne "" and $status == 0 );
}

&initialize;
if ( $#ARGV >= 0 ) {
    while ( $#ARGV >= 0 ) {
        &show_charset( shift @ARGV );
    }
}
else {
    &show_dialog;
}
