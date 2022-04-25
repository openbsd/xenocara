#!/usr/bin/env perl
# $XTermId: palettes.pl,v 1.26 2020/07/01 20:13:58 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2020 by Thomas E. Dickey
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
# Demonstrate how to set palette colors using xterm's control sequences.

# TODO: add "-n" option, to show tput in dry-run mode
# TODO: make some way to optimize-out the Tcolors resets, to focus on tput vs hardcoded

use strict;
use warnings;

use Getopt::Std;
use Term::ReadKey;
use FileHandle;

select(STDERR);
$| = 1;
select(STDOUT);
$| = 1;

our (
    $opt_a, $opt_b, $opt_d, $opt_g, $opt_i,
    $opt_s, $opt_T, $opt_v, $opt_x, $opt_8
);

our ( $CSI, $OSC, $ST );

our %colors;
our @Acolors;
our %Tcolors;
our $dump;

our %Tcolor_names = qw(
  0  foreground
  1  background
  2  cursorColor
  3  pointerForegroundColor
  4  pointerBackgroundColor
  5  tektronixForegroundColor
  6  tektronixBackgroundColor
  7  highlightColor
  8  tektronixCursorColor
  9  highlightForegroundColor
);

our $DARK;

sub isatty() {
    my $result = 0;
    $result = 1 if ( -t 0 and -t 1 and -t 2 );
    return $result;
}

# Adapted from
# https://github.com/altercation/solarized (xresources/solarized)
sub init_solarized() {
    $colors{S_yellow}  = 0xb58900;
    $colors{S_orange}  = 0xcb4b16;
    $colors{S_red}     = 0xdc322f;
    $colors{S_magenta} = 0xd33682;
    $colors{S_violet}  = 0x6c71c4;
    $colors{S_blue}    = 0x268bd2;
    $colors{S_cyan}    = 0x2aa198;
    $colors{S_green}   = 0x859900;

    if ($DARK) {
        $colors{S_base03} = 0x002b36;
        $colors{S_base02} = 0x073642;
        $colors{S_base01} = 0x586e75;
        $colors{S_base00} = 0x657b83;
        $colors{S_base0}  = 0x839496;
        $colors{S_base1}  = 0x93a1a1;
        $colors{S_base2}  = 0xeee8d5;
        $colors{S_base3}  = 0xfdf6e3;
    }
    else {
        $colors{S_base03} = 0xfdf6e3;
        $colors{S_base02} = 0xeee8d5;
        $colors{S_base01} = 0x93a1a1;
        $colors{S_base00} = 0x839496;
        $colors{S_base0}  = 0x657b83;
        $colors{S_base1}  = 0x586e75;
        $colors{S_base2}  = 0x073642;
        $colors{S_base3}  = 0x002b36;
    }

    $Acolors[0]  = $colors{S_base02};
    $Acolors[1]  = $colors{S_red};
    $Acolors[2]  = $colors{S_green};
    $Acolors[3]  = $colors{S_yellow};
    $Acolors[4]  = $colors{S_blue};
    $Acolors[5]  = $colors{S_magenta};
    $Acolors[6]  = $colors{S_cyan};
    $Acolors[7]  = $colors{S_base2};
    $Acolors[9]  = $colors{S_orange};
    $Acolors[8]  = $colors{S_base03};
    $Acolors[10] = $colors{S_base01};
    $Acolors[11] = $colors{S_base00};
    $Acolors[12] = $colors{S_base0};
    $Acolors[13] = $colors{S_violet};
    $Acolors[14] = $colors{S_base1};
    $Acolors[15] = $colors{S_base3};

    $Tcolors{background}             = $colors{S_base03};
    $Tcolors{foreground}             = $colors{S_base0};
    $Tcolors{cursorColor}            = $colors{S_base1};
    $Tcolors{pointerColorBackground} = $colors{S_base01};
    $Tcolors{pointerColorForeground} = $colors{S_base1};
}

# Most of the "themes" are from this source:
# http://web.archive.org/web/20100329130515/http://phraktured.net:80/terminal-colors
# in turn, that cites Aaron Griffin (2007) and uses colortheme.sh, referring to
# https://github.com/Rydgel/archlinux/blob/master/scripts/colortheme.sh
# https://web.archive.org/web/20060630201817/http://frexx.de/xterm-256-notes/
sub init_1() {
    $Tcolors{background} = 0x000000;
    $Tcolors{foreground} = 0xa8a8a8;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0xa80000;
    $Acolors[2]  = 0x00a800;
    $Acolors[3]  = 0xa85400;
    $Acolors[4]  = 0x0000a8;
    $Acolors[5]  = 0xa800a8;
    $Acolors[6]  = 0x00a8a8;
    $Acolors[7]  = 0xa8a8a8;
    $Acolors[8]  = 0x545054;
    $Acolors[9]  = 0xf85450;
    $Acolors[10] = 0x50fc50;
    $Acolors[11] = 0xf8fc50;
    $Acolors[12] = 0x5054f8;
    $Acolors[13] = 0xf854f8;
    $Acolors[14] = 0x50fcf8;
    $Acolors[15] = 0xf8fcf8;
}

sub init_2() {
    $Tcolors{background} = 0x000000;
    $Tcolors{foreground} = 0x7f7f7f;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0x9e1828;
    $Acolors[2]  = 0xaece92;
    $Acolors[3]  = 0x968a38;
    $Acolors[4]  = 0x414171;
    $Acolors[5]  = 0x963c59;
    $Acolors[6]  = 0x418179;
    $Acolors[7]  = 0xbebebe;
    $Acolors[8]  = 0x666666;
    $Acolors[9]  = 0xcf6171;
    $Acolors[10] = 0xc5f779;
    $Acolors[11] = 0xfff796;
    $Acolors[12] = 0x4186be;
    $Acolors[13] = 0xcf9ebe;
    $Acolors[14] = 0x71bebe;
    $Acolors[15] = 0xffffff;
}

sub init_3() {
    $Tcolors{background} = 0x000000;
    $Tcolors{foreground} = 0xcfcfcf;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0xe01010;
    $Acolors[2]  = 0x20ad20;
    $Acolors[3]  = 0xd4c24f;
    $Acolors[4]  = 0x231bb8;
    $Acolors[5]  = 0x9c3885;
    $Acolors[6]  = 0x1dbdb8;
    $Acolors[7]  = 0xfefefe;
    $Acolors[8]  = 0x6a6a6a;
    $Acolors[9]  = 0xe83a3d;
    $Acolors[10] = 0x35e956;
    $Acolors[11] = 0xffff2f;
    $Acolors[12] = 0x3a53f0;
    $Acolors[13] = 0xe628ba;
    $Acolors[14] = 0x1cf5f5;
    $Acolors[15] = 0xffffff;
}

sub init_4() {
    $Tcolors{background} = 0x000000;
    $Tcolors{foreground} = 0xffffff;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0xd36265;
    $Acolors[2]  = 0xaece91;
    $Acolors[3]  = 0xe7e18c;
    $Acolors[4]  = 0x7a7ab0;
    $Acolors[5]  = 0x963c59;
    $Acolors[6]  = 0x418179;
    $Acolors[7]  = 0xbebebe;
    $Acolors[8]  = 0x666666;
    $Acolors[9]  = 0xef8171;
    $Acolors[10] = 0xe5f779;
    $Acolors[11] = 0xfff796;
    $Acolors[12] = 0x4186be;
    $Acolors[13] = 0xef9ebe;
    $Acolors[14] = 0x71bebe;
    $Acolors[15] = 0xffffff;
}

sub init_5() {
    $Tcolors{background} = 0xadaaad;
    $Tcolors{foreground} = 0x000000;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0x640f19;
    $Acolors[2]  = 0x63796b;
    $Acolors[3]  = 0xad7142;
    $Acolors[4]  = 0x4f4f89;
    $Acolors[5]  = 0xb25c7c;
    $Acolors[6]  = 0x52756b;
    $Acolors[7]  = 0xadaaad;
    $Acolors[8]  = 0x525552;
    $Acolors[9]  = 0xa56163;
    $Acolors[10] = 0xcec263;
    $Acolors[11] = 0x73ae70;
    $Acolors[12] = 0x36709f;
    $Acolors[13] = 0xaa829c;
    $Acolors[14] = 0x518989;
    $Acolors[15] = 0xffffef;
}

sub init_6() {
    $Tcolors{background} = 0xbebebe;
    $Tcolors{foreground} = 0x212121;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0xbf7276;
    $Acolors[2]  = 0x86af80;
    $Acolors[3]  = 0x968a38;
    $Acolors[4]  = 0x3673b5;
    $Acolors[5]  = 0x9a70b2;
    $Acolors[6]  = 0x7abecc;
    $Acolors[7]  = 0xdbdbdb;
    $Acolors[8]  = 0x6692af;
    $Acolors[9]  = 0xe5505f;
    $Acolors[10] = 0x87bc87;
    $Acolors[11] = 0xe0d95c;
    $Acolors[12] = 0x1b85d6;
    $Acolors[13] = 0xad73ba;
    $Acolors[14] = 0x338eaa;
    $Acolors[15] = 0xf4f4f4;
}

sub init_7() {
    $Tcolors{background} = 0x676767;
    $Tcolors{foreground} = 0xffffff;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0xbf4646;
    $Acolors[2]  = 0x67b25f;
    $Acolors[3]  = 0xcfc44e;
    $Acolors[4]  = 0x516083;
    $Acolors[5]  = 0xca6eff;
    $Acolors[6]  = 0x92b2f8;
    $Acolors[7]  = 0xd5d5d5;
    $Acolors[8]  = 0x000000;
    $Acolors[9]  = 0xf48a8a;
    $Acolors[10] = 0xa5d79f;
    $Acolors[11] = 0xe1da84;
    $Acolors[12] = 0xa2bbff;
    $Acolors[13] = 0xe2b0ff;
    $Acolors[14] = 0xbacdf8;
    $Acolors[15] = 0xd5d5d5;
}

sub init_8() {
    $Tcolors{background} = 0x101010;
    $Tcolors{foreground} = 0xd3d3d3;

    $Acolors[0]  = 0x101010;
    $Acolors[1]  = 0xcd5c5c;
    $Acolors[2]  = 0x2e8b57;
    $Acolors[3]  = 0xf0e68c;
    $Acolors[4]  = 0xb0c4de;
    $Acolors[5]  = 0xba55d3;
    $Acolors[6]  = 0x4682b4;
    $Acolors[7]  = 0xd3d3d3;
    $Acolors[8]  = 0x4d4d4d;
    $Acolors[9]  = 0xff6a6a;
    $Acolors[10] = 0x8fbc8f;
    $Acolors[11] = 0xfffacd;
    $Acolors[12] = 0x1e90ff;
    $Acolors[13] = 0xdb7093;
    $Acolors[14] = 0x5f9ea0;
    $Acolors[15] = 0xffffff;
}

sub init_9() {
    $Tcolors{background} = 0x1a1a1a;
    $Tcolors{foreground} = 0xd6d6d6;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0x9e1828;
    $Acolors[2]  = 0x008800;
    $Acolors[3]  = 0x968a38;
    $Acolors[4]  = 0x414171;
    $Acolors[5]  = 0x963c59;
    $Acolors[6]  = 0x418179;
    $Acolors[7]  = 0xbebebe;
    $Acolors[8]  = 0x666666;
    $Acolors[9]  = 0xcf6171;
    $Acolors[10] = 0x7cbc8c;
    $Acolors[11] = 0xfff796;
    $Acolors[12] = 0x4186be;
    $Acolors[13] = 0xcf9ebe;
    $Acolors[14] = 0x71bebe;
    $Acolors[15] = 0xffffff;
}

sub init_10() {
    $Tcolors{background} = 0x1a1a1a;
    $Tcolors{foreground} = 0xd6d6d6;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0x98565e;
    $Acolors[2]  = 0x66825d;
    $Acolors[3]  = 0x969176;
    $Acolors[4]  = 0x4d6585;
    $Acolors[5]  = 0x967395;
    $Acolors[6]  = 0x5f7f7b;
    $Acolors[7]  = 0xb3b3b3;
    $Acolors[8]  = 0x737373;
    $Acolors[9]  = 0xcfa3a9;
    $Acolors[10] = 0xcaf7bb;
    $Acolors[11] = 0xfff8bc;
    $Acolors[12] = 0x83a3be;
    $Acolors[13] = 0xbba9cf;
    $Acolors[14] = 0x96cccc;
    $Acolors[15] = 0xffffff;
}

sub init_11() {
    $Tcolors{background} = 0x333333;
    $Tcolors{foreground} = 0xffffff;

    $Acolors[0]  = 0x333333;
    $Acolors[1]  = 0xffa0a0;
    $Acolors[2]  = 0x98fb98;
    $Acolors[3]  = 0xf0e68c;
    $Acolors[4]  = 0x87ceeb;
    $Acolors[5]  = 0xffa0a0;
    $Acolors[6]  = 0x87ceeb;
    $Acolors[7]  = 0xffffff;
    $Acolors[8]  = 0x333333;
    $Acolors[9]  = 0xffa0a0;
    $Acolors[10] = 0x9acd32;
    $Acolors[11] = 0xf0e68c;
    $Acolors[12] = 0x87ceeb;
    $Acolors[13] = 0xffa0a0;
    $Acolors[14] = 0x87ceeb;
    $Acolors[15] = 0xffffff;
}

sub init_12() {
    $Tcolors{foreground} = 0xffffff;
    $Tcolors{background} = 0x000000;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0xbf7276;
    $Acolors[2]  = 0x86af80;
    $Acolors[3]  = 0x968a38;
    $Acolors[4]  = 0x3673b5;
    $Acolors[5]  = 0x9a70b2;
    $Acolors[6]  = 0x7abecc;
    $Acolors[7]  = 0xdbdbdb;
    $Acolors[8]  = 0x6692af;
    $Acolors[9]  = 0xe5505f;
    $Acolors[10] = 0x87bc87;
    $Acolors[11] = 0xe0d95c;
    $Acolors[12] = 0x1b85d6;
    $Acolors[13] = 0xad73ba;
    $Acolors[14] = 0x338eaa;
    $Acolors[15] = 0xf4f4f4;
}

sub init_13() {
    $Tcolors{background} = 0x000000;
    $Tcolors{foreground} = 0xaaaaaa;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0x9e1828;
    $Acolors[2]  = 0xaece92;
    $Acolors[3]  = 0x968a38;
    $Acolors[4]  = 0x414171;
    $Acolors[5]  = 0x963c59;
    $Acolors[6]  = 0x7f9f7f;
    $Acolors[7]  = 0xbebebe;
    $Acolors[8]  = 0x666666;
    $Acolors[9]  = 0xcf6171;
    $Acolors[10] = 0xafc5af;
    $Acolors[11] = 0xf0dfaf;
    $Acolors[12] = 0x8e9fbc;
    $Acolors[13] = 0xdca3a3;
    $Acolors[14] = 0x95c1c5;
    $Acolors[15] = 0xffffff;
}

sub init_14() {
    $Tcolors{background} = 0x959595;
    $Tcolors{foreground} = 0x000000;

    $Acolors[0]  = 0x7f7f7f;
    $Acolors[1]  = 0xcd0000;
    $Acolors[2]  = 0x008b00;
    $Acolors[3]  = 0xeeee00;
    $Acolors[4]  = 0x0000cd;
    $Acolors[5]  = 0xcd00cd;
    $Acolors[6]  = 0x00eeee;
    $Acolors[7]  = 0xfaebd7;
    $Acolors[8]  = 0xe5e5e5;
    $Acolors[9]  = 0x800000;
    $Acolors[10] = 0x005020;
    $Acolors[11] = 0x995500;
    $Acolors[12] = 0x004080;
    $Acolors[13] = 0x443300;
    $Acolors[14] = 0x306080;
    $Acolors[15] = 0xffffff;
}

sub init_15() {
    $Tcolors{background} = 0x1d2b3a;
    $Tcolors{foreground} = 0xbebebe;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0xd36265;
    $Acolors[2]  = 0xaece91;
    $Acolors[3]  = 0xe7e18c;
    $Acolors[4]  = 0x7a7ab0;
    $Acolors[5]  = 0x963c59;
    $Acolors[6]  = 0x418179;
    $Acolors[7]  = 0xbebebe;
    $Acolors[8]  = 0x666666;
    $Acolors[9]  = 0xef8171;
    $Acolors[10] = 0xe5f779;
    $Acolors[11] = 0xfff799;
    $Acolors[12] = 0x4186be;
    $Acolors[13] = 0xef9ebe;
    $Acolors[14] = 0x71bebe;
    $Acolors[15] = 0xffffff;
}

sub init_16() {
    $Tcolors{background} = 0x000000;
    $Tcolors{foreground} = 0xbebebe;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0x9e1828;
    $Acolors[2]  = 0xaece92;
    $Acolors[3]  = 0x968a38;
    $Acolors[4]  = 0x414171;
    $Acolors[5]  = 0x963c59;
    $Acolors[6]  = 0x418179;
    $Acolors[7]  = 0xbebebe;
    $Acolors[8]  = 0x666666;
    $Acolors[9]  = 0xcf6171;
    $Acolors[10] = 0xc5f779;
    $Acolors[11] = 0xfff796;
    $Acolors[12] = 0x4186be;
    $Acolors[13] = 0xcf9ebe;
    $Acolors[14] = 0x71bebe;
    $Acolors[15] = 0xffffff;
}

sub init_17() {
    $Tcolors{background} = 0x000000;
    $Tcolors{foreground} = 0xe5e5e5;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0xff0000;
    $Acolors[2]  = 0x00ff00;
    $Acolors[3]  = 0xffff00;
    $Acolors[4]  = 0x0000ff;
    $Acolors[5]  = 0xff00ff;
    $Acolors[6]  = 0x00ffff;
    $Acolors[7]  = 0xffffff;
    $Acolors[8]  = 0xffd39b;
    $Acolors[9]  = 0xff8247;
    $Acolors[10] = 0xff82ab;
    $Acolors[11] = 0x87cefa;
    $Acolors[12] = 0xffffff;
    $Acolors[13] = 0xffffff;
    $Acolors[14] = 0xffffff;
    $Acolors[15] = 0xffffff;
}

sub init_18() {
    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0x9e1828;
    $Acolors[2]  = 0x5cb247;
    $Acolors[3]  = 0x968a38;
    $Acolors[4]  = 0x4161a0;
    $Acolors[5]  = 0x9b768e;
    $Acolors[6]  = 0x419189;
    $Acolors[7]  = 0xbebebe;
    $Acolors[8]  = 0x666666;
    $Acolors[9]  = 0xcf6171;
    $Acolors[10] = 0xc5f779;
    $Acolors[11] = 0xfff796;
    $Acolors[12] = 0x4186be;
    $Acolors[13] = 0xcf9ebe;
    $Acolors[14] = 0x71bebe;
    $Acolors[15] = 0xdddddd;
}

sub init_19() {
    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0xb07050;
    $Acolors[2]  = 0x12914e;
    $Acolors[3]  = 0xa0a070;
    $Acolors[4]  = 0x3e4581;
    $Acolors[5]  = 0xa070a0;
    $Acolors[6]  = 0x70a0a0;
    $Acolors[7]  = 0xa0a0a0;
    $Acolors[8]  = 0x606060;
    $Acolors[9]  = 0xb07050;
    $Acolors[10] = 0x12914e;
    $Acolors[11] = 0xc0c090;
    $Acolors[12] = 0x3e4581;
    $Acolors[13] = 0xc090c0;
    $Acolors[14] = 0x90c0c0;
    $Acolors[15] = 0xffffff;
}

sub init_20() {
    $Tcolors{foreground} = 0xaaaaaa;
    $Tcolors{background} = 0x000000;

    $Acolors[0]  = 0x303430;
    $Acolors[1]  = 0xbf7979;
    $Acolors[2]  = 0x97b26b;
    $Acolors[3]  = 0xcdcdc1;
    $Acolors[4]  = 0x86a2be;
    $Acolors[5]  = 0xd9b798;
    $Acolors[6]  = 0xa1b5cd;
    $Acolors[7]  = 0xffffff;
    $Acolors[8]  = 0xcdb5cd;
    $Acolors[9]  = 0xf4a45f;
    $Acolors[10] = 0xc5f779;
    $Acolors[11] = 0xffffef;
    $Acolors[12] = 0x98afd9;
    $Acolors[13] = 0xd7d998;
    $Acolors[14] = 0xa1b5cd;
    $Acolors[15] = 0xdedede;
}

sub init_21() {
    $Tcolors{background} = 0x1a1a1a;
    $Tcolors{foreground} = 0xaaaaaa;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0x9e1828;
    $Acolors[2]  = 0x008800;
    $Acolors[3]  = 0xd2bb4b;
    $Acolors[4]  = 0x414171;
    $Acolors[5]  = 0x963c59;
    $Acolors[6]  = 0x418179;
    $Acolors[7]  = 0xbebebe;
    $Acolors[8]  = 0x666666;
    $Acolors[9]  = 0xbc5766;
    $Acolors[10] = 0x61a171;
    $Acolors[11] = 0xe7db52;
    $Acolors[12] = 0x5085af;
    $Acolors[13] = 0xa97a99;
    $Acolors[14] = 0x6ba4a4;
    $Acolors[15] = 0xffffff;
}

sub init_22() {
    $Tcolors{background} = 0x000000;
    $Tcolors{foreground} = 0xbebebe;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0xd36265;
    $Acolors[2]  = 0xaece91;
    $Acolors[3]  = 0xe7e18c;
    $Acolors[4]  = 0x7a7ab0;
    $Acolors[5]  = 0x963c59;
    $Acolors[6]  = 0x7f9f7f;
    $Acolors[7]  = 0xbebebe;
    $Acolors[8]  = 0x666666;
    $Acolors[9]  = 0xef8171;
    $Acolors[10] = 0xe5f779;
    $Acolors[11] = 0xf0dfaf;
    $Acolors[12] = 0x8e9fbc;
    $Acolors[13] = 0xef9ebe;
    $Acolors[14] = 0x71bebe;
    $Acolors[15] = 0xffffff;
}

sub init_23() {
    $Tcolors{background} = 0x0e0e0e;
    $Tcolors{foreground} = 0x4ad5e1;

    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0xdc74d1;
    $Acolors[2]  = 0x0eb8c7;
    $Acolors[3]  = 0xdfe37e;
    $Acolors[4]  = 0x0;        #??
    $Acolors[5]  = 0x9e88f0;
    $Acolors[6]  = 0x73f7ff;
    $Acolors[7]  = 0xe1dddd;
    $Acolors[8]  = 0x8b8f93;
    $Acolors[9]  = 0xdc74d1;
    $Acolors[10] = 0x0eb8c7;
    $Acolors[11] = 0xdfe37e;
    $Acolors[13] = 0x9e88f0;
    $Acolors[14] = 0x73f7ff;
    $Acolors[15] = 0xe1dddd;
}

sub init_24() {
    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0xcd5c5c;
    $Acolors[2]  = 0x8eae71;
    $Acolors[3]  = 0xd2b48c;
    $Acolors[4]  = 0x5f7b8a;
    $Acolors[5]  = 0xcdcdb4;
    $Acolors[6]  = 0x686868;
    $Acolors[7]  = 0xffffff;
    $Acolors[8]  = 0x000000;
    $Acolors[9]  = 0xee6363;
    $Acolors[10] = 0x95c749;
    $Acolors[11] = 0xcdcdc1;
    $Acolors[12] = 0x6b7b8a;
    $Acolors[13] = 0xcdcdb4;
    $Acolors[14] = 0x778798;
    $Acolors[15] = 0xcacaca;
}

sub init_25() {
    $Acolors[0]  = 0x000000;
    $Acolors[1]  = 0x800000;
    $Acolors[2]  = 0x008000;
    $Acolors[3]  = 0xd0d090;
    $Acolors[4]  = 0x000080;
    $Acolors[5]  = 0x800080;
    $Acolors[6]  = 0xa6caf0;
    $Acolors[7]  = 0xd0d0d0;
    $Acolors[8]  = 0xb0b0b0;
    $Acolors[9]  = 0xf08060;
    $Acolors[10] = 0x60f080;
    $Acolors[11] = 0xe0c060;
    $Acolors[12] = 0x80c0e0;
    $Acolors[13] = 0xf0c0f0;
    $Acolors[14] = 0xc0d8f8;
    $Acolors[15] = 0xe0e0e0;
}

# (Griffin) colors ripped from rezza: http://metawire.org/~rezza/index.php
sub init_rezza() {
    $Tcolors{foreground} = 0xdddddd;
    $Tcolors{background} = 0x222222;

    $Acolors[0]  = 0x191919;
    $Acolors[1]  = 0x803232;
    $Acolors[2]  = 0x5b762f;
    $Acolors[3]  = 0xaa9943;
    $Acolors[4]  = 0x324c80;
    $Acolors[5]  = 0x706c9a;
    $Acolors[6]  = 0x92b19e;
    $Acolors[7]  = 0xffffff;
    $Acolors[8]  = 0x252525;
    $Acolors[9]  = 0x982b2b;
    $Acolors[10] = 0x89b83f;
    $Acolors[11] = 0xefef60;
    $Acolors[12] = 0x2b4f98;
    $Acolors[13] = 0x826ab1;
    $Acolors[14] = 0xa1cdcd;
    $Acolors[15] = 0xdddddd;
}

sub init_theme($) {
    my $theme = shift;

    @Acolors = ();
    %Tcolors = ();

    &init_solarized if ( index( "solarized", $theme ) eq 0 );
    &init_rezza     if ( index( "rezza",     $theme ) eq 0 );

    &init_1  if ( $theme eq 1 );
    &init_2  if ( $theme eq 2 );
    &init_3  if ( $theme eq 3 );
    &init_4  if ( $theme eq 4 );
    &init_5  if ( $theme eq 5 );
    &init_6  if ( $theme eq 6 );
    &init_7  if ( $theme eq 7 );
    &init_8  if ( $theme eq 8 );
    &init_9  if ( $theme eq 9 );
    &init_10 if ( $theme eq 10 );
    &init_11 if ( $theme eq 11 );
    &init_12 if ( $theme eq 12 );
    &init_13 if ( $theme eq 13 );
    &init_14 if ( $theme eq 14 );
    &init_15 if ( $theme eq 15 );
    &init_16 if ( $theme eq 16 );
    &init_17 if ( $theme eq 17 );
    &init_18 if ( $theme eq 18 );
    &init_19 if ( $theme eq 19 );
    &init_20 if ( $theme eq 20 );
    &init_21 if ( $theme eq 21 );
    &init_22 if ( $theme eq 22 );
    &init_23 if ( $theme eq 23 );
    &init_24 if ( $theme eq 24 );
    &init_25 if ( $theme eq 25 );
}

sub all_themes() {
    my @result;
    push @result, "solarized";
    push @result, "rezza";
    for my $n ( 1 .. 25 ) {
        push @result, $n;
    }
    return \@result;
}

sub raw() {
    ReadMode 'ultra-raw', 'STDIN';    # allow single-character inputs
}

sub cooked() {
    ReadMode 'normal';
}

sub get_reply($$) {
    my $command = shift;
    my $finish  = shift;
    my $reply   = "";

    &raw;
    printf STDERR "%s", $command;
    my $start = time;
    while ( index( $reply, $finish ) < 0 ) {
        my $test = ReadKey 0.1;
        last if ( time > ( $start + 3 ) );
        next if not defined $test;

        $reply .= $test;
    }
    &cooked;
    return $reply;
}

sub query_color($$) {
    my $ansi = shift;
    my $code = shift;
    my @result;
    my $parms;
    if ($ansi) {
        $parms = sprintf( "4;%d", $code );
    }
    else {
        $parms = sprintf( "%d", 10 + $code );
    }
    my $query = sprintf( "%s%s;?%s", $OSC, $parms, $ST );
    my $reply = &get_reply( $query, $ST );
    if ( $reply =~ /^${OSC}${parms};rgb:/ ) {
        $reply =~ s/^${OSC}${parms};rgb://;
        $reply =~ s/[^[:print:]].*$//;
        if ( $reply =~ /^[[:xdigit:]]+(\/[[:xdigit:]]+)+$/ ) {
            $reply =~ s/([[:xdigit:]]+)/0x$1/g;
            @result = split /\//, $reply;

            # RGB should have 3 values
            for my $n ( 0 .. $#result ) {
                $result[$n] = hex $result[$n];
            }

            # add the limit based on the number of bits needed to print.
            push @result, 2**( 4 * ( length($reply) - 2 ) / 3 );
        }
    }

    # this would occur in case of error
    push @result, $reply unless ( $#result >= 3 );
    return @result;
}

sub get_color_string($$) {
    my $ansi  = shift;
    my $color = shift;
    my $result;
    my @check = &query_color( $ansi, $color );
    if ( $#check == 3 ) {
        while ( $check[3] > 0x10000 ) {
            $check[0] /= ( 16 * 16 );
            $check[1] /= ( 16 * 16 );
            $check[2] /= ( 16 * 16 );
            $check[3] /= ( 16 * 16 * 16 );
        }
        $result = sprintf "( %3d %3d %3d )", $check[0], $check[1], $check[2];
    }
    else {
        $result = sprintf( "{%s}", $check[0] );
    }
    return $result;
}

sub guess_dark() {
    my $result = 0;    # assume black-on-white
    if (&isatty) {
        my @check = &query_color( 0, 1 );
        if ( $#check == 3 ) {
            my $value = ( ( $check[0] ) + ( $check[1] ) + ( $check[2] ) ) / 3;
            printf "%#x / %#x\n", $value, $check[3];
            $result = 1 if ( $value < ( $check[3] / 2 ) );
            printf "%s\n", $result ? "dark" : "light" if ($opt_v);
        }
    }
    return $result;
}

sub for_tput($) {
    my $value = shift;
    $value *= 1000;
    $value /= 255;
    $value *= 256 / 255;
    return sprintf( "%.0f", $value );
}

sub reset_Acolor($) {
    my $number = shift;
    printf STDERR "%s104;%d%s", $OSC, $number, $ST;
    printf "\t%2d ->reset Acolor\n", $number if ($opt_v);
}

sub set_Acolor($) {
    my $number = shift;
    my $result = 0;
    if ( defined $Acolors[$number] ) {
        my $value = $Acolors[$number];
        my $r     = ( ( $value / ( 256 * 256 ) ) % 256 );
        my $g     = ( ( $value / (256) ) % 256 );
        my $b     = ( ( $value / (1) ) % 256 );
        if ($opt_x) {
            system(
                sprintf(
                    "$opt_x $opt_T initc %d %d %d %d",
                    $number, &for_tput($r), &for_tput($g), &for_tput($b)
                )
            );
        }
        else {
            printf STDERR "%s4;%d;rgb:%02X/%02X/%02X%s", $OSC, $number, $r, $g,
              $b,
              $ST;
        }
        printf "\t%2d ->%06X ( %3d %3d %3d )\n", $number, $value, $r, $g, $b
          if ($opt_v);
        $result = 1;
    }
    return $result;
}

sub reset_Tcolor($) {
    my $number = shift;
    my $actual = $Tcolor_names{$number};
    printf STDERR "%s%d%s", $OSC, 110 + $number, $ST;
    printf "\t%2d ->reset Tcolor{%s}\n", $number, $actual
      if ($opt_v);
}

sub set_Tcolor($) {
    my $number = shift;
    my $actual = $Tcolor_names{$number};
    my $result = 0;
    if (%Tcolors) {
        if ( defined $Tcolors{$actual} ) {
            my $value = $Tcolors{$actual};
            my $r     = ( ( $value / ( 256 * 256 ) ) % 256 );
            my $g     = ( ( $value / (256) ) % 256 );
            my $b     = ( ( $value / (1) ) % 256 );
            printf STDERR "%s%d;rgb:%02X/%02X/%02X%s", $OSC, 10 + $number, $r,
              $g, $b, $ST;
            printf "\t%2d ->set Tcolor{%s}\n", $number, $actual
              if ($opt_v);
            $result = 1;
        }
    }
    return $result;
}

sub set_colors() {
    for my $n ( 0 .. 2 ) {
        &reset_Tcolor($n) unless &set_Tcolor($n);
    }
    for my $n ( 0 .. 15 ) {
        &reset_Acolor($n) unless &set_Acolor($n);
    }
    printf STDERR "\007" if (&isatty);
    STDERR->flush;
}

sub reset_colors() {
    for my $n ( 0 .. 2 ) {
        &reset_Tcolor($n);
    }
    printf STDERR "%s104%s", $OSC, $ST;
    printf "\tall ->reset Acolor\n" if ($opt_v);
}

sub dump_colors($) {
    my $theme = shift;
    if ( open( DUMP, ">>", $opt_d ) ) {
        my $state = &get_reply( sprintf( "%s#R", $CSI ), "Q" );
        $state = substr( $state, length($CSI) )
          if ( index( $state, $CSI ) == 0 );
        printf DUMP "State \"%s\"\n",         $state;
        printf DUMP "Palette after \"%s\"\n", $theme;
        if (&isatty) {
            printf DUMP "Tcolors:\n";
            for my $n ( 0 .. 2 ) {
                printf DUMP "\t%s = %s\n", $Tcolor_names{$n},
                  &get_color_string( 0, $n );
            }
            printf DUMP "Acolors:\n";
            for my $n ( 0 .. 15 ) {
                printf DUMP "\t%2d -> %s\n", $n, &get_color_string( 1, $n );
            }
        }
        close DUMP;
    }
}

sub show_colors($) {
    my $theme = shift;
    if ( substr( $theme, 0, 1 ) eq "+" ) {

        # push (or set slot, if number follows)
        if ( $theme =~ /^\+[0-9]$/ ) {
            my $slot = substr( $theme, 1 );
            printf STDERR "%s#%dP", $CSI, $slot + 1;
            printf "\tpush %s\n", $slot if ($opt_v);
        }
        else {
            printf STDERR "%s#P", $CSI;
            printf "\tpush\n" if ($opt_v);
        }
    }
    elsif ( substr( $theme, 0, 1 ) eq "-" ) {

        # pop (or restore from slot, if number follows)
        if ( $theme =~ /^-[0-9]$/ ) {
            my $slot = substr( $theme, 1 );
            printf STDERR "%s#%dQ", $CSI, $slot + 1;
            printf "\tpop %s\n", $slot if ($opt_v);
        }
        else {
            printf STDERR "%s#Q", $CSI;
            printf "\tpop\n" if ($opt_v);
        }
    }
    elsif ( $theme eq "?" ) {

        # query TODO
        printf STDERR "%s#R", $CSI;
        printf "\tquery\n" if ($opt_v);
    }
    else {
        &init_theme($theme);
        &set_colors if ( $#Acolors >= 0 );
        &reset_colors unless ( $#Acolors >= 0 );
    }
    &dump_colors($theme) if ($opt_d);
}

sub show_themes($) {
    my @themes = @{ $_[0] };
    for my $n ( 0 .. $#themes ) {
        &show_colors( $themes[$n] );
        sleep $opt_s if ( ( $n != $#themes ) and &isatty );
    }
}

sub main::HELP_MESSAGE() {
    printf STDERR <<EOF
Usage: $0 [options] [themes]

Options:
  -8      use 8-bit controls
  -a      show all themes
  -b      use BEL rather than ST for terminating strings
  -d FILE dump palette to file after setting colors
  -g      ask terminal for its default background color
  -i      assume terminal colors are reversed, i.e., white-on-black
  -s SECS sleep this long between changes
  -T TERM override "xterm-256color" for TPUT
  -x TPUT use TPUT program rather than hardcoded escapes
  -v      verbose

Themes:
  solarized default
  rezza   named
  1-25    numbered
  0       reset
EOF
      ;
    exit 1;
}

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('abd:gis:T:x:v8') || &main::HELP_MESSAGE;

&main::HELP_MESSAGE if ( $opt_a and ( $#ARGV >= 0 ) );

if ($opt_8) {
    $CSI = "\x9b";
    $OSC = "\x9d";
    $ST  = "\x9c";
}
else {
    $CSI = "\x1b[";
    $OSC = "\x1b]";
    $ST  = "\x1b\\";
}

$ST = "\007" if ($opt_b);

$opt_s = 1 unless ($opt_s);
$opt_T = "-T $opt_T" if ($opt_T);
$opt_T = "-T xterm-256color" unless ($opt_T);

$DARK = 0;
$DARK = 1 if ($opt_i);
$DARK = &guess_dark if ($opt_g);

if ($opt_a) {
    &show_themes(&all_themes);
}
elsif ( $#ARGV >= 0 ) {
    &show_themes( \@ARGV );
}
else {
    &show_colors("solarized");
}

1;
