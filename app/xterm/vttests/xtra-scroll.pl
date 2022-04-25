#!/usr/bin/env perl
# $XTermId: xtra-scroll.pl,v 1.12 2021/09/03 18:34:50 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2021 by Thomas E. Dickey
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
# Interactively test screen-updates which can exercise the cdXtraScroll and
# tiXtraScroll features.

use strict;
use warnings;

use Getopt::Std;
use Term::ReadKey;
use I18N::Langinfo qw(langinfo CODESET);

$! = 1;

our $target = "";

our $encoding = lc( langinfo( CODESET() ) );
our ($opt_8);

our $dirty       = 1;    # nonzero if the screen should be painted
our $mode_margin = 0;    # nonzero if left/right margin mode enabled
our $mode_origin = 0;    # nonzero if origin-mode in effect
our $mode_screen = 0;    # nonzero if using alternate screen
our $pos_x       = 0;    # current cursor-Y, absolute
our $pos_y       = 0;    # current cursor-X, absolute
our $term_high;          # terminal's height
our $term_wide;          # terminal's width
our $CSI         = "\x1b[";
our $crlf        = "\r\n";
our $text_sample = "THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG ";
our $text_filler = "";
our %margins;

sub raw() {
    ReadMode 'ultra-raw', 'STDIN';    # allow single-character inputs
}

sub cooked() {
    ReadMode 'normal';
}

sub utf8_sample() {
    my $text = "";
    for my $n ( 0 .. length($text_sample) ) {
        my $chr = substr( $text_sample, $n, 1 );
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
    return $text;
}

sub next_x($) {
    my $value = shift;
    if ($mode_margin) {
        $value = $margins{R} if ( $value < $margins{R} );
        $value = $margins{L} if ( $value > $margins{L} );
    }
    else {
        $value = $value % $term_wide;
    }
    return $value;
}

sub next_y($) {
    my $value = shift;
    if ($mode_origin) {
        $value = $margins{B} if ( $value < $margins{T} );
        $value = $margins{T} if ( $value > $margins{B} );
    }
    else {
        $value = $value % $term_high;
    }
    return $value;
}

sub move() {
    my $y = $pos_y;
    if ($mode_origin) {
        my $min_y = ( $margins{T} >= 0 ) ? $margins{T} : 0;
        my $two_y = $min_y + 1;    # scrolling region is at least 2 lines
        my $max_y = ( $margins{B} >= $two_y ) ? $margins{B} : $two_y;
        $y = $max_y if ( $y > $max_y );
        $y -= $min_y;              # convert to relative ordinate
    }
    $y = 0 if ( $y < 0 );
    printf STDERR "%s%d;%dH", $CSI, 1 + $y, 1 + $pos_x;
}

sub home() {
    printf STDERR "%sH", $CSI;
    $pos_x = 0;
    $pos_y = 0;
    &move;
}

sub erase_display($) {
    my $mode = shift;
    printf STDERR "%s%sJ", $CSI, $mode;
}

sub erase_line($) {
    my $mode = shift;
    printf STDERR "%s%sK", $CSI, $mode;
}

sub toggle($) {
    my $value = shift;
    return ( $value == 0 ) ? 1 : 0;
}

################################################################################

sub set_margin_mode($) {
    my $mode = shift;
    printf STDERR "%s?69%s", $CSI, ( $mode == 0 ) ? "l" : "h";
    $mode_margin = $mode;
}

################################################################################

sub set_origin_mode($) {
    my $mode = shift;
    printf STDERR "%s?6%s", $CSI, ( $mode == 0 ) ? "l" : "h";
    $mode_origin = $mode;
}

################################################################################

sub set_screen_mode($) {
    my $mode = shift;
    printf STDERR "%s?1049%s", $CSI, ( $mode == 0 ) ? "l" : "h";
    $mode_screen = $mode;
}

################################################################################

sub do_tb_margins($$) {
    my $param_T = "";
    my $param_B = "";
    $param_T = sprintf( "%d", 1 + $margins{T} ) if ( $margins{T} >= 0 );
    $param_B = sprintf( "%d", 1 + $margins{B} )
      if ( $margins{B} > $margins{T} );
    printf STDERR "%s%s;%sr", $CSI, $param_T, $param_B;
    &move;
}

sub undo_tb_margins() {
    &do_tb_margins( -1, -1 );
}

sub redo_tb_margins() {
    &do_tb_margins( $margins{T}, $margins{B} );
}

sub set_tb_margins($$) {
    my $reset = ( not defined $margins{T} or not defined $margins{B} ) ? 1 : 0;
    my $old_T = 1;
    my $old_B = $term_high;
    $old_T = $margins{T} if ( defined $margins{T} );
    $old_B = $margins{B} if ( defined $margins{B} );
    $margins{T} = shift;
    $margins{B} = shift;
    if ( $reset == 0 ) {
        $reset = 1 if ( $old_T != $margins{T} );
        $reset = 1 if ( $old_B != $margins{B} );
    }
    &redo_tb_margins if ( $reset == 1 );
}

################################################################################

sub do_lr_margins($$) {
    my $param_L = "";
    my $param_R = "";
    $param_L = sprintf( "%d", 1 + $margins{L} ) if ( $margins{L} >= 0 );
    $param_R = sprintf( "%d", 1 + $margins{R} )
      if ( $margins{R} > $margins{T} );
    printf STDERR "%s%s;%ss", $CSI, $param_L, $param_R;
    &move;
}

sub undo_lr_margins() {
    &do_lr_margins( -1, -1 );
}

sub redo_lr_margins() {
    &do_lr_margins( $margins{L}, $margins{R} );
}

sub set_lr_margins($$) {
    my $reset = ( not defined $margins{L} or not defined $margins{R} ) ? 1 : 0;
    my $old_L = 1;
    my $old_R = $term_high;
    $old_L = $margins{L} if ( defined $margins{L} );
    $old_R = $margins{R} if ( defined $margins{R} );
    $margins{L} = shift;
    $margins{R} = shift;
    if ( $reset == 0 ) {
        $reset = 1 if ( $old_L != $margins{L} );
        $reset = 1 if ( $old_R != $margins{R} );
    }
    &redo_lr_margins if ( $reset == 1 );
}

################################################################################

sub has_tb_margins() {
    my $result = 0;
    $result = 1 if ( $margins{T} != 1 );
    $result = 1 if ( $margins{B} != $term_high );
    return $result;
}

sub repaint($) {
    my $erase  = shift;
    my $save_x = $pos_x;
    my $save_y = $pos_y;
    $dirty = 0;
    if ($erase) {
        &home;
        &erase_display(2);
    }
    if ( $text_filler ne "" ) {
        if ( $mode_origin and &has_tb_margins ) {
            my @rows = split /$crlf/, $text_filler;
            for my $row ( 0 .. $#rows ) {
                next unless ( $row >= $margins{T} );
                next unless ( $row <= $margins{B} );
                printf STDERR "%s$crlf", $rows[$row];
            }
        }
        else {
            printf STDERR "%s$crlf", $text_filler;
        }
    }
    else {
        my $cells = 0;
        my $limit = $term_high * $term_wide;
        while ( $cells < $limit ) {
            my $sample = ( $encoding eq "utf-8" ) ? &utf8_sample : $text_sample;
            printf STDERR "%s", $sample;
            $cells += length($sample);
        }
    }
    $pos_x = $save_x;
    $pos_y = $save_y;
    &move;
}

sub initialize() {
    if ( $encoding eq "utf-8" ) {
        binmode( STDOUT, ":utf8" );
        binmode( STDERR, ":utf8" );
    }
    if ($opt_8) {
        if ( $encoding eq "utf-8" ) {
            undef $opt_8;
            printf "...ignoring -8 option since locale uses %s\n", $encoding;
        }
        else {
            printf STDERR "\x1b G";
            $CSI = "\x9b";
        }
    }

    &raw;

    my @term_size = GetTerminalSize( \*STDERR );
    $term_wide = 80;
    $term_wide = $term_size[0] if ( $#term_size >= 0 );
    $term_wide = 80 if ( $term_wide <= 0 );
    $term_high = 24;
    $term_high = $term_size[1] if ( $#term_size >= 1 );
    $term_high = 24 if ( $term_high <= 0 );

    &set_margin_mode(0);
    &set_origin_mode(0);
    &set_screen_mode(0);

    &set_tb_margins( -1, -1 );
    &set_lr_margins( 1, $term_wide );

    &home;
    &erase_display("2");
}

sub cleanup() {
    &cooked;

    printf STDERR "\x1b F" if ($opt_8);

    &set_margin_mode(0);
    &set_origin_mode(0);
    &set_screen_mode(0);

    &undo_tb_margins;

    $pos_x = 1;
    $pos_y = $term_high - 2;
    &move;
    &erase_display("");
}

sub beep() {
    printf STDERR "\a";
}

sub main::HELP_MESSAGE() {
    printf STDERR <<EOF
Usage: $0 [options] [datafile]
Options:
  -8      use 8-bit controls
EOF
      ;
    exit 1;
}

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('8') || &main::HELP_MESSAGE;
$#ARGV <= 0   || &main::HELP_MESSAGE;

# provide for reading file containing text to repaint
if ( $#ARGV == 0 ) {
    if ( open( FP, $ARGV[0] ) ) {
        my @lines = <FP>;
        chomp @lines;
        close FP;
        $text_filler = join( $crlf, @lines );
    }
}

printf "encoding $encoding\n";

&initialize();

while (1) {
    my $cmd;

    printf "\r\nCommand (? for help):" if ( $dirty != 0 );
    $cmd = ReadKey 0;
    if ( not $cmd ) {
        sleep 1;
    }
    elsif ( $cmd eq "?" ) {
        $dirty = 1;
        &home;
        &erase_display(2);
        printf $crlf
          . "General:"
          . $crlf
          . " ? (help),"
          . " q (quit)"
          . $crlf
          . "Clear:"
          . $crlf
          . " C (entire screen),"
          . " c (screen-below),"
          . " E (entire line),"
          . " e (line-right)"
          . $crlf . "Fill:"
          . $crlf
          . " @ (margin-box),"
          . " # (prompt-char)"
          . $crlf
          . "Move cursor:\r\n"
          . " h,j,k,l (vi-like),"
          . " H (to home)."
          . $crlf
          . "Set margin using current position:"
          . $crlf
          . " T (top),"
          . " B (bottom),"
          . " L (left),"
          . " R (right)"
          . $crlf
          . "Reset modes"
          . $crlf
          . " M (margins)"
          . $crlf
          . "Toggle modes"
          . $crlf
          . " A (alternate-screen),"
          . " O (origin-mode)"
          . " | (left/right-mode)"
          . $crlf
          . "Print sample:"
          . " form-feed (repaint)";
    }
    elsif ( $cmd eq "\033" ) {

        # try to ignore special-keys
        my $count = 0;
        while (1) {
            $cmd = ReadKey 0;
            $count++;
            next if ( $count == 1 and $cmd eq "O" );
            next unless ( $cmd =~ /^[A-~]$/ );
            $cmd = ReadKey 0;
            last;
        }
    }
    elsif ( $cmd eq "q" ) {
        last;
    }
    elsif ( index( "CcEe@#hjklHMTBLRAO|\f", $cmd ) >= 0 ) {
        my $was_dirty = $dirty;
        &repaint(1) if ( $dirty != 0 );
        if ( $cmd eq "C" ) {
            &home;
            &erase_display("2");
        }
        elsif ( $cmd eq "c" ) {
            &erase_display("");
        }
        elsif ( $cmd eq "E" ) {
            &erase_line("2");
        }
        elsif ( $cmd eq "e" ) {
            &erase_line("");
        }
        elsif ( $cmd eq "@" ) {

            # FIXME
        }
        elsif ( $cmd eq "#" ) {
            $text_sample = ReadKey 0;
            if ( $text_filler ne "" ) {
                my $save_filler = $text_filler;
                $text_filler =~ s/[^\d\s]/$text_sample/g;
                &repaint(0);
                $text_filler = $save_filler;
            }
            else {
                &repaint(0);
            }
        }
        elsif ( $cmd eq "h" ) {
            $pos_x = &next_x( $pos_x - 1 );
            &move;
        }
        elsif ( $cmd eq "j" ) {
            $pos_y = &next_y( $pos_y + 1 );
            &move;
        }
        elsif ( $cmd eq "k" ) {
            $pos_y = &next_y( $pos_y - 1 );
            &move;
        }
        elsif ( $cmd eq "l" ) {
            $pos_x = &next_x( $pos_x + 1 );
            &move;
        }
        elsif ( $cmd eq "H" ) {
            &home;
        }
        elsif ( $cmd eq "M" ) {
            &set_tb_margins( -1, -1 );
            &set_lr_margins( -1, -1 );
            &repaint(0);
        }
        elsif ( $cmd eq "T" ) {
            &set_tb_margins( $pos_y, $margins{B} );
        }
        elsif ( $cmd eq "B" ) {
            &set_tb_margins( $margins{T}, $pos_y );
        }
        elsif ( $cmd eq "L" ) {
            &set_lr_margins( $pos_x, $margins{R} );
        }
        elsif ( $cmd eq "R" ) {
            &set_lr_margins( $margins{L}, $pos_x );
        }
        elsif ( $cmd eq "A" ) {
            &set_screen_mode( &toggle($mode_screen) );
            &repaint(1);
        }
        elsif ( $cmd eq "O" ) {
            &set_origin_mode( &toggle($mode_origin) );
        }
        elsif ( $cmd eq "|" ) {
            &set_margin_mode( &toggle($mode_margin) );
        }
        elsif ( $cmd eq "\f" ) {
            &repaint(1) unless ($was_dirty);
        }
        else {
            &beep;
            $dirty = 2;
        }
    }
    else {
        &beep;
    }
}

&cleanup;
printf " ...quit\r\n";

1;
