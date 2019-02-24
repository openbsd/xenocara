#! /usr/bin/perl -w
# $XTermId: gen-charsets.pl,v 1.2 2018/08/22 22:59:15 tom Exp $
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
#
# Translate a Unicode mapping, e.g., for one of the ISO-8859-x codepages,
# into the form used in charsets.c for converting characters.
use strict;

$| = 1;

sub do_file($) {
    my $file  = shift;
    my $undef = hex(0x2426);
    open( FP, $file ) || do {
        print STDERR "Can't open $file: $!\n";
        return;
    };
    my @data = <FP>;
    close(FP);
    my $name = $file;
    $name =~ s,^.*/,,;
    $name =~ s/\..*$//;
    $name =~ s/^(8859)/ISO-$1/;
    $name =~ s/-/_/g;
    printf "#define map_%s(code) \\\n", $name;
    printf "\tswitch (code) { \\\n";
    my @target;
    my @noteof;

    for my $n ( 0 .. $#data ) {
        chomp $data[$n];
        $data[$n] =~ s/^\s*//;
        $data[$n] =~ s/\s*$//;
        next if ( $data[$n] =~ /^#/ );
        next if ( $data[$n] eq "" );
        if ( $data[$n] !~ /^0x[[:xdigit:]]+\s+0x[[:xdigit:]]+\s*#/i ) {
            printf STDERR "?? %d:%s\n", $n + 1, $data[$n];
            next;
        }

        my $source = $data[$n];
        $source =~ s/\s.*//;
        $source = hex($source);
        next if ( $source < 160 or $source > 255 );
        $source -= 128;

        my $target = $data[$n];
        $target =~ s/^[^\s]+\s+(0x[[:xdigit:]]+).*$/$1/i;
        $target = hex($target);

        my $noteof = $data[$n];
        $noteof =~ s/^[^#]+#\s*//;

        $target[$source] = $target;
        $noteof[$source] = $noteof;
    }
    my $lo = $target[32]  ? 32  : 33;
    my $hi = $target[127] ? 127 : 126;
    for my $n ( $lo .. $hi ) {
        if ( defined $target[$n] ) {
            printf "\t    UNI(0x%02x, 0x%04x);\t/* %s */ \\\n", $n,
              $target[$n], $noteof[$n];
        }
        else {
            printf "\t    XXX(0x%02x, UNDEF);\t/* undefined */ \\\n", $n;
        }
    }
    printf "\t}\n";
}

while ( $#ARGV >= 0 ) {
    &do_file( shift @ARGV );
}

1;
