#!/usr/bin/env perl
# $XTermId: report-sgr.pl,v 1.32 2018/08/08 09:15:39 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2018 by Thomas E. Dickey
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
# Test the report-sgr option of xterm.

# TODO: add "-8" option, for 8-bit controls

use strict;
use warnings;

use Getopt::Long qw(:config auto_help no_ignore_case);
use Pod::Usage;
use Term::ReadKey;

our ( $opt_colors, $opt_direct, $opt_help, $opt_man );

our $csi = "\033[";
our $osc = "\033]";
our $st  = "\033\\";

our @sgr_names = qw(
  Normal
  Bold
  Faint
  Italicized
  Underlined
  Blink
  Fast-Blink
  Inverse
  Invisible
  Crossed-Out
);

our ( $row_max, $col_max );
our ( $mark,    $top_row );

our $cur_sgr = 0;

# indexed colors, e.g., "ANSI"
our %indexed_f = qw ( default 1 c 7 );
our %indexed_b = qw ( default 1 c 0 );

# direct colors
our %direct_f = qw ( default 0 r 255 g 0 b 0 );
our %direct_b = qw ( default 0 r 0 g 0 b 255 );

our $which_value = "video-attributes";
our $which_color = "red";

our ( $row_1st, $col_1st, $row_now, $col_now );

sub beep() {
    printf "\a";
}

sub cup($$) {
    my $r = shift;
    my $c = shift;
    printf "%s%d;%dH", $csi, $r, $c;
}

sub el($) {
    printf "%s%sK", $csi, $_[0];
}

sub ed($) {
    printf "%s%sJ", $csi, $_[0];
}

sub sgr($) {
    printf "%s%sm", $csi, $_[0];
}

sub same_rgb($$) {
    my %c1     = %{ $_[0] };
    my %c2     = %{ $_[1] };
    my $result = 1;
    $result = 0 if ( $c1{r} ne $c2{r} );
    $result = 0 if ( $c1{g} ne $c2{g} );
    $result = 0 if ( $c1{b} ne $c2{b} );
    return $result;
}

sub color_name($) {
    my $code = shift;
    my $result;
    if ($opt_direct) {
        $result = $code;
    }
    else {
        if ( $code < 0 ) {
            $result = "default";
        }
        else {
            $result = $code;
        }
    }
    return $result;
}

sub color_code($$) {
    my $isfg   = shift;
    my $result = "";
    my $base   = $isfg ? 30 : 40;
    if ($opt_direct) {
        $result = sprintf "%d:2", $base + 8;
        if ($isfg) {
            $result .= sprintf ":%d:%d:%d",    #
              $direct_f{r},                    #
              $direct_f{g},                    #
              $direct_f{b};
        }
        else {
            $result .= sprintf ":%d:%d:%d",    #
              $direct_b{r},                    #
              $direct_b{g},                    #
              $direct_b{b};
        }
    }
    else {
        my %data = $isfg ? %indexed_f : %indexed_b;
        if ( &is_default( \%data ) ) {
            $result = $base + 9;
        }
        else {
            if ( $opt_colors <= 16 ) {
                $base += 60 if ( $data{c} >= 8 );
                $result = $base + $data{c};
            }
            else {
                $result = sprintf "%d:5:%d", $base + 8, $data{c};
            }
        }
    }
    return $result;
}

sub show_string($) {
    my $value = $_[0];
    my $n;

    $value = "" unless $value;
    my $result = "";
    for ( $n = 0 ; $n < length($value) ; $n += 1 ) {
        my $c = ord substr( $value, $n, 1 );
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

    return $result;
}

sub get_reply($) {
    my $command = $_[0];
    my $reply   = "";

    print STDOUT $command;
    autoflush STDOUT 1;
    while (1) {
        my $test = ReadKey 0.02;
        last if not defined $test;

        $reply .= $test;
    }
    return $reply;
}

sub show_status() {
    &cup( 1, 1 );
    &el(2);
    my $show = "";
    my $parm = "";
    if ($mark) {
        my $r1 = ( $row_now > $row_1st ) ? $row_1st : $row_now;
        my $r2 = ( $row_now < $row_1st ) ? $row_1st : $row_now;
        my $c1 = ( $col_now > $col_1st ) ? $col_1st : $col_now;
        my $c2 = ( $col_now < $col_1st ) ? $col_1st : $col_now;
        $show = sprintf "[%d,%d] [%d,%d] ", $r1, $c1, $r2, $c2;
        $parm = sprintf "%d;%d;%d;%d",      $r1, $c1, $r2, $c2;
    }
    else {
        $show = sprintf "[%d,%d] ", $row_now, $col_now;
        $parm = sprintf "%d;%d;%d;%d",    #
          $row_now, $col_now,    #
          $row_now, $col_now;
    }
    my $send = sprintf "%s%s#|", $csi, $parm;
    printf "%s %s ", $show, &show_string($send);
    &cup( $row_now, $col_now );
    my $reply = &get_reply($send);
    &cup( 2, 1 );
    &el(2);
    printf "read %s", &show_string($reply);
    &cup( $row_now, $col_now );
}

sub toggle_default() {
    if ($opt_direct) {
        if ( $which_value =~ /^f/ ) {
            $direct_f{default} = !$direct_f{default};
        }
        elsif ( $which_value =~ /^b/ ) {
            $direct_b{default} = !$direct_b{default};
        }
        else {
            &beep;
        }
    }
    else {
        if ( $which_value =~ /^f/ ) {
            $indexed_f{default} = !$indexed_f{default};
        }
        elsif ( $which_value =~ /^b/ ) {
            $indexed_b{default} = !$indexed_b{default};
        }
        else {
            &beep;
        }
    }

    &show_example;
}

sub is_default($) {
    my $result = 0;
    my %data   = %{ $_[0] };
    $result = ( $data{default} != 0 );
    return $result;
}

sub change_color($$) {
    my $inc  = $_[0];
    my %data = %{ $_[1] };
    my $name = $_[2];
    $data{$name} = ( $data{$name} + $opt_colors + $inc ) % $opt_colors;
    return %data;
}

sub set_which_value($) {
    $which_value = shift;
    &show_example;
}

sub set_which_color($) {
    $which_color = shift;
    &show_example;
}

sub change_value($) {
    my $inc = shift;
    if ( $which_value =~ /^v/ ) {
        $cur_sgr = ( $cur_sgr + 10 + $inc ) % 10;
    }
    elsif ( $which_value =~ /^f/ ) {
        if ($opt_direct) {
            %direct_f = &change_color( $inc, \%direct_f, "r" )
              if ( $which_color =~ /^r/ );
            %direct_f = &change_color( $inc, \%direct_f, "g" )
              if ( $which_color =~ /^g/ );
            %direct_f = &change_color( $inc, \%direct_f, "b" )
              if ( $which_color =~ /^b/ );
        }
        else {
            %indexed_f = &change_color( $inc, \%indexed_f, "c" );
        }
    }
    elsif ( $which_value =~ /^b/ ) {
        if ($opt_direct) {
            %direct_b = &change_color( $inc, \%direct_b, "r" )
              if ( $which_color =~ /^r/ );
            %direct_b = &change_color( $inc, \%direct_b, "g" )
              if ( $which_color =~ /^g/ );
            %direct_b = &change_color( $inc, \%direct_b, "b" )
              if ( $which_color =~ /^b/ );
        }
        else {
            %indexed_b = &change_color( $inc, \%indexed_b, "c" );
        }
    }
    &show_example;
}

sub show_example() {
    &cup( $top_row, 1 );
    my $init = "0";
    if ($opt_direct) {
        $init .= sprintf ";%s", &color_code(1);
        $init .= sprintf ";%s", &color_code(0);
    }
    else {
        $init .= sprintf ";%s", &color_code(1)
          unless ( &is_default( \%indexed_f ) );
        $init .= sprintf ";%s", &color_code(0)
          unless ( &is_default( \%indexed_b ) );
    }
    &ed(0);
    for my $n ( 0 .. 9 ) {
        my $mode = $n;
        $mode = $init if ( $n == 0 );
        &cup( $n + $top_row, 1 );
        if ($opt_direct) {
            &sgr($init);
            &sgr( &same_rgb( \%direct_f, \%direct_b ) ? "0" : $init );
        }
        else {
            &sgr( $indexed_f{c} eq $indexed_b{c} ? "0" : $init );
        }
        printf "%s SGR %d: %-12s",    #
          ( $cur_sgr == $n ) ? "-->" : "   ",    #
          $n, $sgr_names[$n];
        $mode .= ";$cur_sgr" unless ( $cur_sgr eq "0" );
        &sgr($mode);
        printf                                   #
          "abcdefghijklmnopqrstuvwxyz" .         #
          "ABCDEFGHIJKLMNOPQRSTUVWXYZ",          #
          "0123456789";
    }
    &sgr(0);
    my $end = $top_row + 11;
    &cup( $end++, 1 );
    printf 'Change %s with "<" or ">".',
      ( $opt_direct and ( $which_value !~ /^v/ ) )
      ? ( sprintf "%s(%s)", $which_value, $which_color )
      : $which_value;
    &cup( $end++, 1 );
    printf "Current SGR %d (%s)", $cur_sgr, $sgr_names[$cur_sgr];
    if ($opt_direct) {
        &cup( $end++, 1 );

        printf "Colors: direct";
        &cup( $end++, 1 );

        if ( &is_default( \%direct_f ) ) {
            printf "       fg( default )";
        }
        else {
            printf "       fg( r=%s, g=%s, b=%s )",    #
              &color_name( $direct_f{r} ),             #
              &color_name( $direct_f{g} ),             #
              &color_name( $direct_f{b} );
        }
        &cup( $end++, 1 );

        if ( &is_default( \%direct_b ) ) {
            printf "       bg( default )";
        }
        else {
            printf "       bg( r=%s, g=%s, b=%s )",    #
              &color_name( $direct_b{r} ),             #
              &color_name( $direct_b{g} ),             #
              &color_name( $direct_b{b} );
        }
    }
    else {
        &cup( $end++, 1 );
        printf "Colors: indexed";
        if ( &is_default( \%indexed_f ) ) {
            printf ", fg=default";
        }
        else {
            printf ", fg=%s", &color_name( $indexed_f{c} );
        }
        if ( &is_default( \%indexed_b ) ) {
            printf ", bg=default";
        }
        else {
            printf ", bg=%s", &color_name( $indexed_b{c} );
        }
    }
    &cup( $end++, 1 );
    printf ' ("q" to quit, "?" for help)';
}

sub init_screensize() {
    $row_max = 24;
    $col_max = 80;
    &cup( 9999, 9999 );
    my $result = &get_reply( $csi . "6n" );
    if ( $result =~ /^$csi[[:digit:];]+R$/ ) {
        $result =~ s/^$csi[;]*//;
        $result =~ s/[;]*R$//;
        my @params = split /;/, $result;
        if ( $#params == 1 ) {
            $row_max = $params[0];
            $col_max = $params[1];
        }
    }
    &cup( 1, 1 );
}

sub startup_screen() {
    ReadMode 'ultra-raw', 'STDIN';
}

sub restore_screen() {
    &sgr(0);
    printf "%s102%s", $osc, $st if ($opt_direct);
    &cup( $row_max, 1 );
    ReadMode 'restore', 'STDIN';
}

GetOptions( 'colors=i', 'help|?', 'direct', 'man' ) || pod2usage(2);
pod2usage(1) if $opt_help;
pod2usage( -verbose => 2 ) if $opt_man;

$opt_colors = ( $opt_direct ? 256 : 8 ) unless ($opt_colors);
$opt_colors = 8 if ( $opt_colors < 8 );

&startup_screen;

&init_screensize;

$mark    = 0;
$top_row = 4;
$row_now = $row_1st = $top_row;
$col_now = $col_1st = 1;

&ed(2);
&show_example;

while (1) {
    my $cmd;

    &show_status;
    &cup( $row_now, $col_now );
    $cmd = ReadKey 0;
    if ( $cmd eq "?" ) {
        &restore_screen;
        system( $0 . " -man" );
        &startup_screen;
        &show_example;
        $cmd = ReadKey 0;
    }
    elsif ( $cmd eq " " ) {
        $mark    = ( $mark != 0 ) ? 0 : 1;
        $row_1st = $row_now;
        $col_1st = $col_now;
    }
    elsif ( $cmd eq chr(12) ) {
        &show_example;
    }
    elsif ( $cmd eq "h" ) {
        $col_now-- if ( $col_now > 1 );
    }
    elsif ( $cmd eq "j" ) {
        $row_now++ if ( $row_now < $row_max );
    }
    elsif ( $cmd eq "k" ) {
        $row_now-- if ( $row_now > 1 );
    }
    elsif ( $cmd eq "l" ) {
        $col_now++ if ( $col_now < $col_max );
    }
    elsif ( $cmd eq "q" ) {
        &restore_screen;
        printf "\r\n...quit\r\n";
        last;
    }
    elsif ( $cmd eq "=" ) {
        &cup( $row_now = $row_1st + $cur_sgr, $col_now = 24 );
    }
    elsif ( $cmd eq "v" ) {
        &set_which_value("video-attributes (SGR)");
    }
    elsif ( $cmd eq "f" ) {
        &set_which_value("foreground");
    }
    elsif ( $cmd eq "b" ) {
        &set_which_value("background");
    }
    elsif ( $cmd eq "d" ) {
        &toggle_default;
    }
    elsif ( $cmd eq "<" ) {
        &change_value(-1);
    }
    elsif ( $cmd eq ">" ) {
        &change_value(1);
    }
    elsif ( $opt_direct and ( $cmd eq "R" ) ) {
        &set_which_color("red");
    }
    elsif ( $opt_direct and ( $cmd eq "G" ) ) {
        &set_which_color("green");
    }
    elsif ( $opt_direct and ( $cmd eq "B" ) ) {
        &set_which_color("blue");
    }
    else {
        &beep;
    }
}

1;

__END__

=head1 NAME

report-sgr.pl - demonstrate xterm's report-SGR control sequence

=head1 SYNOPSIS

report-sgr.pl [options]

  Options:
    -help            brief help message
    -direct          use direct-colors, rather than indexed

=head1 OPTIONS

=over 8

=item B<-help>

Print a brief help message and exit.

=item B<-man>

Print the extended help message and exit.

=item B<-direct>

Use direct-colors (e.g., an RGB value), rather than indexed (e.g., ANSI colors).

=back

=head1 DESCRIPTION

B<report-sgr> displays a normal line, as well as one for each SGR code 1-9,
with a test-string showing the effect of the SGR.  Two SGR codes can be
combined, as well as foreground and background colors.

=head1 Commands

=over 8

=item B<q>

Quit the program with B<q>.  It will ignore B<^C> and other control characters.

=item B<h>, B<j>, B<k>, B<l>

As you move the cursor around the screen (with vi-style h,j,k,l characters),
the script sends an XTREPORTSGR control to the terminal, asking what the video
attributes are for the currently selected cell.  The script displays the result
on the second line of the screen.

=item B<space>

XTREPORTSGR returns an SGR control sequence which could be used to set the
terminal's current video attributes to match the attributes found in all cells
of the rectangle specified by this script.  Use the spacebar to toggle the mark
which denotes one corner of the rectangle.  The current cursor position is the
other corner.

=item B<=>

Move the cursor to the first cell of the test-data for the currently selected
SGR code (the one with B<-->>).

=item B<v>

Select the video-attribute mode.

=item B<f>

Select the foreground-color mode.

=item B<b>

Select the background-color mode.

=item B<R>

When direct-colors are chosen, select the red-component of
the currently selected foreground or background mode.

=item B<G>

When direct-colors are chosen, select the green-component of
the currently selected foreground or background mode.

=item B<B>

When direct-colors are chosen, select the blue-component of
the currently selected foreground or background mode.

=item B<d>

Toggle between the selected colors and the terminal's default colors.

=item B<<>

Decrease the index of video-attribute to combine, or the color value
depending on the selected mode.

=item B<>>

Increase the index of video-attribute to combine, or the color value
depending on the selected mode.

=item B<^L>

Repaint the screen.

=back

=cut
