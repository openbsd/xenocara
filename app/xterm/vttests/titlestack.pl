#!/usr/bin/env perl
# $XTermId: titlestack.pl,v 1.29 2019/09/20 00:50:10 tom Exp $
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
# Test the title-stack and title-mode options of xterm.

# TODO: add test for arbitrary x property
# TODO: allow -g and -v options to toggle interactively

use strict;
use warnings;

use Getopt::Std;
use Encode qw(decode encode);
use Term::ReadKey;
use I18N::Langinfo qw(langinfo CODESET);

our $target = "";

our $encoding = lc( langinfo( CODESET() ) );
our $wm_name;
our ( $opt_b, $opt_g, $opt_v, $opt_8 );

our @titlestack;    # stack of title-strings, using current encoding
our @item_stack;    # selector used when doing a push
our @mode_stack;    # titleModes in effect when titlestack was loaded
our $SP;            # stack-pointer
our $TM;            # current titleModes, in various combinations

our $utf8_sample = 0;

our $CSI = "\x1b[";
our $OSC = "\x1b]";
our $ST  = "\x1b\\";

sub SendHEX()  { return ( $TM & 1 ) ? 1 : 0; }
sub ReadHEX()  { return ( $TM & 2 ) ? 1 : 0; }
sub SendUTF8() { return ( $TM & 4 ) ? 1 : 0; }
sub ReadUTF8() { return ( $TM & 8 ) ? 1 : 0; }

sub to_hex($) {
    my $value  = shift;
    my $result = "";
    my $n;

    for ( $n = 0 ; $n < length($value) ; ++$n ) {
        $result .= sprintf( "%02X", ord substr( $value, $n, 1 ) );
    }
    return $result;
}

sub from_hex($) {
    my $value  = shift;
    my $result = "";
    if ( $value =~ /^[[:xdigit:]]+$/ and ( length($value) % 2 ) == 0 ) {
        my $octets = "";
        for ( my $n = 0 ; $n < length($value) ; $n += 2 ) {
            my $pair = substr( $value, $n, 2 );
            my $data = hex $pair;
            $octets .= chr($data);
        }
        $result = decode( &ReadUTF8 ? "utf-8" : "iso-8859-1", $octets );
    }
    else {
        $result = $value;
    }
    return $result;
}

sub show_string($) {
    my $value = shift;
    my $n;
    my $octets =
      encode( ( ( $encoding eq "utf-8" ) ? "utf-8" : "iso-8859-1" ), $value );

    my $result = "";
    for ( $n = 0 ; $n < length($octets) ; $n += 1 ) {
        my $c = ord substr( $octets, $n, 1 );
        if ( $c == ord '\\' ) {
            $result .= "\\\\";
        }
        elsif ( $c == 0x1b ) {
            $result .= "\\E";
        }
        elsif ( $c == 0x7f ) {
            $result .= "^?";
        }
        elsif ( $c == 32 ) {
            $result .= "\\s";
        }
        elsif ( $c < 32 ) {
            $result .= sprintf( "^%c", $c + 64 );
        }
        elsif ( $c > 128 ) {
            $result .= sprintf( "\\%03o", $c );
        }
        else {
            $result .= chr($c);
        }
    }

    printf "%s\r\n", $result;
}

sub send_command($) {
    my $command = shift;
    if ($opt_v) {
        printf "send: ";
        &show_string($command);
    }
    print STDERR encode( &SendUTF8 ? "utf-8" : "iso-8859-1", $command );
}

sub get_reply($) {
    my $command = shift;
    my $reply   = "";

    &send_command($command);
    my $start = time;
    while (1) {
        my $test = ReadKey 1;
        last if not defined $test;
        last if ( time > ( $start + 1 ) );

        $reply .= $test;
    }
    if ($opt_v) {
        printf "read: ";
        &show_string($reply);
    }
    return $reply;
}

sub get_title($) {
    my $icon   = shift;
    my $reply  = &get_reply( sprintf( "%s%dt", $CSI, $icon ? 20 : 21 ) );
    my $prefix = $icon ? "L" : "l";

    if ( $opt_8 and ( $reply =~ /^$CSI/ ) ) {
        $reply =~ s/^${CSI}//;
        $reply =~ s/${ST}$//;
    }
    else {
        $reply =~ s/^\x1b//;
        $reply =~ s/^[\[\]]//;
        if ( index( $reply, $ST ) >= 0 ) {
            $reply =~ s/\x1b\\$//;
        }
        else {
            $reply =~ s/\007$//;
        }
    }
    if ( $reply =~ /^$prefix/ ) {
        $reply =~ s/^$prefix//;
        if (&ReadHEX) {
            $reply = &from_hex($reply);
        }
    }
    else {
        $reply = "?" . $reply;
    }
    return $reply;
}

sub raw() {
    ReadMode 'ultra-raw', 'STDIN';    # allow single-character inputs
}

sub cooked() {
    ReadMode 'normal';
}

sub read_cmd($) {
    my $command = shift;
    my @result;
    if ( open( my $fp, "$command |" ) ) {
        binmode( $fp, ":utf8" ) if ( $encoding eq "utf-8" );
        @result = <$fp>;
        close($fp);
        chomp @result;
    }
    return @result;
}

sub which_modes($) {
    my $modes  = shift;
    my $result = "";
    if ( $modes & 3 ) {
        $result .= "put" if ( ( $modes & 3 ) == 1 );
        $result .= "get" if ( ( $modes & 3 ) == 2 );
        $result .= "p/q" if ( ( $modes & 3 ) == 3 );
        $result .= " hex";
    }
    if ( $modes & 12 ) {
        $modes /= 4;
        $result .= "," unless ( $result eq "" );
        $result .= "put" if ( ( $modes & 3 ) == 1 );
        $result .= "get" if ( ( $modes & 3 ) == 2 );
        $result .= "p/q" if ( ( $modes & 3 ) == 3 );
        $result .= " utf";
    }
    $result = "default" if ( $result eq "" );
    return $result;
}

sub which_tmode($$) {
    my $set    = shift;
    my $mode   = shift;
    my $result = "";
    $result = "set window/icon labels using hexadecimal"   if ( $mode == 0 );
    $result = "query window/icon labels using hexadecimal" if ( $mode == 1 );
    $result = "set window/icon labels using UTF-8"         if ( $mode == 2 );
    $result = "query window/icon labels using UTF-8"       if ( $mode == 3 );
    $result = "do not " . $result if ( $set == 0 and $result ne "" );
    return $result;
}

sub get_tmode($) {
    my $set    = shift;
    my $help   = 0;
    my $result = "?";
    while ( $result !~ /^[0123]$/ ) {
        $result = ReadKey 0;
        if ( $result eq "q" ) {
            $result = -1;
            last;
        }
        elsif ( $result eq "?" and not $help ) {
            for my $n ( 0 .. 3 ) {
                printf "\r\n\t%s = %s", $n, &which_tmode( $set, $n );
            }
            printf "\r\n\t:";
            $help = 1;
        }
    }
    if ( $result >= 0 ) {
        printf "[%s]\r\n\t:", &which_tmode( $set, $result );
    }
    return $result;
}

sub which_item($) {
    my $code   = shift;
    my $result = "";
    $result = "both" if ( $code == 0 );
    $result = "icon" if ( $code == 1 );
    $result = "name" if ( $code == 2 );
    return $result;
}

sub which_selector($) {
    my $code   = shift;
    my $result = "";
    $result = "both titles"  if ( $code == 0 );
    $result = "icon title"   if ( $code == 1 );
    $result = "window title" if ( $code == 2 );
    return $result;
}

sub get_selector() {
    my $result = "?";
    my $help   = 0;
    printf "\t:";
    while ( $result !~ /^[012]$/ ) {
        $result = ReadKey 0;
        if ( $result eq "q" ) {
            $result = -1;
            last;
        }
        elsif ( $result eq "l" ) {
            $result = 2;
        }
        elsif ( $result eq "L" ) {
            $result = 1;
        }
        elsif ( $result eq "?" and not $help ) {
            for my $n ( 0 .. 2 ) {
                printf "\r\n\t%d = %s", $n, &which_selector($n);
            }
            printf "\r\n\t:";
            $help = 1;
        }
    }
    if ( $result >= 0 ) {
        printf "[%s]\r\n\t:", &which_selector($result);
    }
    return $result;
}

sub display_info() {

    # use xprop to get properties
    my $command = "xprop";
    if ( $ENV{WINDOWID} ) {
        my $windowid = $ENV{WINDOWID};
        $command .= " -id " . $windowid if ( $windowid ne "" );
    }
    else {
        printf "...xprop\r\n";
    }
    my @props = &read_cmd($command);
    for my $n ( 0 .. $#props ) {
        printf "\t%s\r\n", $props[$n]
          if ( index( $props[$n], "WM_NAME(" ) >= 0
            or index( $props[$n], "WM_ICON_NAME(" ) >= 0 );
    }

    # use escape sequences to get corresponding information
    printf "... Icon title:%s\r\n",   &get_title(1);
    printf "... Window title:%s\r\n", &get_title(0);

    # show title-stack (and modes used for each level)
    printf "... Modes[%s]\r\n",  &which_modes($TM);
    printf "... Stack(%d):\r\n", $SP;
    for my $n ( 0 .. $SP ) {
        printf "\t%d [%s:%s]%s\r\n", $n, &which_item( $item_stack[$n] ),
          &which_modes( $mode_stack[$n] ), $titlestack[$n];
    }
}

sub set_titlemode($) {
    my $set  = shift;
    my $opts = "";
    my $opt;
    printf "\t:";
    while ( ( $opt = &get_tmode($set) ) >= 0 ) {
        $TM |= ( 1 << $opt ) if ($set);
        $TM &= ~( 1 << $opt ) unless ($set);
        $opts .= ";" unless ( $opts eq "" );
        $opts .= $opt;
    }
    if ( $opts ne "" ) {
        &send_command( sprintf( "%s>%s%s", $CSI, $opts, $set ? "t" : "T" ) );
    }
}

sub utf8_sample($) {
    my $item = shift;
    my $last = 4;
    my $text;
    if ( ( $item % $last ) == 0 ) {
        my $chars = "THE QUICK BROWN FOX\nJUMPED OVER THE LAZY DOG";
        $text = "";
        for my $n ( 0 .. length($chars) ) {
            my $chr = substr( $chars, $n, 1 );
            if ( $chr eq " " ) {
                $chr = "  ";
            }
            elsif ( ord($chr) < 32 ) {

                # leave control characters as-is
            }
            else {
                $chr = chr( 0xff00 + ord($chr) - 32 );
            }
            $text .= $chr;
        }
    }
    elsif ( ( $item % $last ) == 1 ) {
        $text = chr(0x442) . chr(0x435) . chr(0x441) . chr(0x442);
    }
    elsif ( ( $item % $last ) == 2 ) {
        for my $chr ( 0x391 .. 0x3a9 ) {
            $text .= chr($chr);
        }
    }
    elsif ( ( $item % $last ) == 3 ) {
        for my $chr ( 0x3b1 .. 0x3c9 ) {
            $text .= chr($chr);
        }
    }
    return $text;
}

sub set_titletext() {
    my $opt = &get_selector;
    if ( $opt >= 0 ) {
        my $text;
        if ($opt_g) {

            if (&SendUTF8) {
                $text = &utf8_sample( $utf8_sample++ );
            }
            else {
                # ugly code, but mapping the a/e/i/o/u uppercase accented
                # characters that repeat.
                my $a_chars = chr(192) . chr(193) . chr(194) . chr(196);
                my $e_chars = "";
                my $i_chars = " ";
                my $o_chars = chr(210) . chr(211) . chr(212) . chr(214);
                my $u_chars = "";
                my $gap     = " " . chr(215) . " ";
                for my $chr ( 0 .. 3 ) {
                    $e_chars .= chr( $chr + 200 );
                    $i_chars .= chr( $chr + 204 ) . " ";
                    $u_chars .= chr( $chr + 217 );
                }
                $text =
                    $a_chars
                  . $gap
                  . $e_chars
                  . $gap
                  . $i_chars
                  . $gap
                  . $o_chars
                  . $gap
                  . $u_chars;
            }
            printf "%s\r\n", $text;
        }
        else {
            &cooked;
            $text = ReadLine 0;
            chomp $text;
            &raw;
        }
        $titlestack[$SP] = $text;
        $item_stack[$SP] = $opt;
        $mode_stack[$SP] = $TM;
        if (&SendHEX) {
            my $octets =
              encode( ( &SendUTF8 ? "utf-8" : "iso-8859-1" ), $text );
            $text = &to_hex($octets);
        }
        &send_command( sprintf( "%s%s;%s%s", $OSC, $opt, $text, $ST ) );
    }
}

sub save_title() {
    my $opt = &get_selector;
    if ( $opt >= 0 ) {
        &send_command( sprintf( "%s22;%st", $CSI, $opt ) );
        ++$SP;
        $titlestack[$SP] = $titlestack[ $SP - 1 ];
        $item_stack[$SP] = $opt;
        $mode_stack[$SP] = $mode_stack[ $SP - 1 ];
    }
}

sub restore_title($) {
    my $set = shift;
    my $opt = &get_selector unless ($set);
    if ( $opt >= 0 and $SP > 0 ) {
        $opt = $item_stack[$SP] if ($set);
        &send_command( sprintf( "%s23;%st", $CSI, $opt ) );
        $SP--;
    }
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

sub get_WM_NAME() {
    $wm_name = "missing WM_NAME";
    my $supwin = `xprop -root '_NET_SUPPORTING_WM_CHECK'`;
    if ( $supwin ne "" ) {
        $supwin =~ s/^.*(0x[[:xdigit:]]+).*/$1/;
        $wm_name = &get_xprop( $supwin, "_NET_WM_NAME" );
        $wm_name = "unknown" if ( $wm_name eq "" );
        printf "** using \"$wm_name\" window manager\n";
    }
}

sub main::HELP_MESSAGE() {
    printf STDERR <<EOF
Usage: $0 [options]
Options:
  -8      use 8-bit controls
  -b      use BEL rather than ST for terminating strings
  -g      generate title-strings rather than prompting
  -v      verbose
EOF
      ;
    exit 1;
}

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('bgv8') || &main::HELP_MESSAGE;

$ST = "\007" if ($opt_b);

$titlestack[ $SP = 0 ] = "unknown";
$item_stack[$SP] = 0;
$mode_stack[$SP] = $TM = 0;

binmode( STDOUT, ":utf8" ) if ( $encoding eq "utf-8" );
if ($opt_8) {
    if ( $encoding eq "utf-8" ) {
        undef $opt_8;
        printf "...ignoring -8 option since locale uses %s\n", $encoding;
    }
    else {
        printf STDERR "\x1b G";
        $CSI = "\x9b";
        $OSC = "\x9d";
        $ST  = "\x9c";
    }
}

&get_WM_NAME;

&raw;
&raw;
while (1) {
    my $cmd;

    printf "\r\nCommand (? for help):";
    $cmd = ReadKey 0;
    if ( not $cmd ) {
        sleep 1;
    }
    elsif ( $cmd eq "?" ) {
        printf "\r\n? help,"
          . " d=display,"
          . " m/M=set/reset mode,"
          . " p=set title,"
          . " q=quit,"
          . " r=restore,"
          . " s=save\r\n";
    }
    elsif ( $cmd eq "#" ) {
        printf " ...comment\r\n\t#";
        &cooked;
        ReadLine 0;
        &raw;
    }
    elsif ( $cmd eq "!" ) {
        printf " ...shell\r\n";
        &cooked;
        system( $ENV{SHELL} );
        &raw;
    }
    elsif ( $cmd eq "d" ) {
        printf " ...display\r\n";
        &display_info;
    }
    elsif ( $cmd eq "p" ) {
        printf " ...set text\r\n";
        &set_titletext;
    }
    elsif ( $cmd eq "q" ) {
        printf " ...quit\r\n";
        last;
    }
    elsif ( $cmd eq "s" ) {
        printf " ...save title\r\n";
        &save_title;
    }
    elsif ( $cmd eq "r" ) {
        printf " ...restore title\r\n";
        &restore_title(0);
    }
    elsif ( $cmd eq "m" ) {
        printf " ...set title mode\r\n";
        &set_titlemode(1);
    }
    elsif ( $cmd eq "M" ) {
        printf " ...reset title mode\r\n";
        &set_titlemode(0);
    }
}

# when unstacking here, just use the selector used for the push
while ( $SP > 0 ) {
    &restore_title(1);
}

&send_command( sprintf( "%s>T", $CSI ) );    # reset title-modes to default

&cooked;

printf "\x1b F" if ($opt_8);
