#! /usr/bin/env perl
# $XTermId: gen-charsets.pl,v 1.37 2024/02/09 01:11:52 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2018-2023,2024 by Thomas E. Dickey
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
# Format/maintain xterm's charsets.h header.

use strict;
use warnings;

use Getopt::Std;

$| = 1;

our ( $opt_d, $opt_i, $opt_v, $opt_x );
our $undef     = hex(0x2426);
our $head_file = "charsets.h";
our $data_file = "charsets.dat";
our @import;

our %wide_chars = qw(
  map_DEC_Cyrillic 1
  map_DEC_Greek_Supp 1
  map_DEC_Hebrew_Supp 1
  map_DEC_Technical 1
  map_DEC_Turkish_Supp 1
  map_ISO_Greek_Supp 1
  map_ISO_Hebrew 1
  map_ISO_Latin_2 1
  map_ISO_Latin_5 1
  map_ISO_Latin_Cyrillic 1
  map_NRCS_Greek 1
  map_NRCS_Hebrew 1
  map_NRCS_Turkish 1
);

our $note_1 = "\
#ifndef included_charsets_h
#define included_charsets_h 1";

our $note_2 = "\
#ifndef PUA
#define PUA(n) (0xEEEE + (n))
#endif
#define UNDEF  0x2426\t\t/* rendered as a backwards \"?\" */";

our $note_3 = "\
#if OPT_WIDE_CHARS
#define begin_CODEPAGE(size) \\
\tif (!(xw->flags & NATIONAL)) { \\
\t    screen->utf8_nrc_mode++; \\
\t}
#define end_CODEPAGE() \\
\tif (!(xw->flags & NATIONAL)) { \\
\t    screen->utf8_nrc_mode--; \\
\t}
#else
#define begin_CODEPAGE(size)\t/* nothing */
#define end_CODEPAGE()\t\t/* nothing */
#endif";

sub read_file($) {
    my $file = shift;
    open( FP, $file ) || do {
        print STDERR "Can't open $file: $!\n";
        return;
    };
    my @data = <FP>;
    close(FP);
    chomp @data;
    return @data;
}

# Translate a Unicode mapping, e.g., for one of the ISO-8859-x codepages,
# into the form used in charsets.c for converting characters.
sub do_import($) {
    my $file = shift;
    my @data = &read_file($file);
    my $name = $file;
    $name =~ s,^.*/,,;
    $name =~ s/\..*$//;
    $name =~ s/^(8859)/ISO-$1/;
    $name =~ s/-/_/g;
    my @target;
    my @noteof;

    $import[ $#import + 1 ] = sprintf "map_%s", $name;
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
            $import[ $#import + 1 ] = sprintf "\t0x%02x\t0x%04x\t\t# %s", $n,
              $target[$n], $noteof[$n];
        }
        else {
            $import[ $#import + 1 ] = sprintf "\t0x%02x\tUNDEF\t\t# undefined",
              $n;
        }
    }
    if ($opt_v) {
        for my $n ( 0 .. $#import ) {
            printf "%s\n", $import[$n];
        }
    }
}

sub add_text($$) {
    my @head = @{ $_[0] };
    my @note = split /\n/, $_[1];
    for my $n ( 0 .. $#note ) {
        $head[ $#head + 1 ] = $note[$n];
    }
    return @head;
}

sub end_note($$) {
    my @head = @{ $_[0] };
    my $note = $_[1];
    $head[ $#head + 1 ] = " */";
    my $notes;
    if ( $note == 1 ) {
        $notes = $note_1;
    }
    elsif ( $note == 2 ) {
        $notes = $note_2;
    }
    elsif ( $note == 3 ) {
        $notes = $note_3;
    }
    else {
        $notes = "";
    }
    return &add_text( \@head, $notes );
}

sub hex_of($) {
    my $text = shift;
    if ($text) {
        $text =~ s/^(0x|u\+)//i;
        $text = "0x" . $text if ( $text =~ /^[[:xdigit:]]+$/ );
    }
    return $text;
}

sub add($$) {
    my @data = @{ $_[0] };
    my $text = $_[1];
    $data[ $#data + 1 ] = $text;
    return @data;
}

sub add_unmap($$) {
    my @head  = @{ $_[0] };
    my %unmap = %{ $_[1] };
    my %noted = %{ $_[2] };
    my $title = $_[3];
    my $macro = "un$title";
    $macro .= "(code,dft)" unless ( $macro =~ /\(code/ );
    $macro =~ s/code\)/code,dft\)/;
    @head = &add( \@head, "" );

    if (%unmap) {
        my @codes = sort keys %unmap;

        if ( $#codes > 0 ) {
            @head = &add( \@head, "#define $macro \\" );
            @head = &add( \@head, "\tswitch (code) { \\" );
            for my $code ( sort keys %unmap ) {
                my $note = $noted{$code};
                my $pads = " ";
                if ( $title =~ /_NRCS_/ ) {
                    $pads = sprintf( "%*s", 17 - length($code), " " );
                    $note =~ s/\t/ /;
                }
                @head = &add(
                    \@head,
                    sprintf(
                        "\t    MAP(%s,%s%s)%s \\",
                        $code, $pads, $unmap{$code}, $note
                    )
                );
            }
            @head = &add( \@head, "\t    default: dft; break; \\" );
            @head = &add( \@head, "\t}" );
        }
        else {
            @head = &add( \@head, "#define $macro /* nothing? */" );
        }
    }
    else {
        @head = &add( \@head, "#define $macro /* nothing */" );
    }
    return @head;
}

# Read the current charsets data file, and format a new charsets.h file.
sub do_update($) {
    my $file = shift;
    my @data = &read_file($file);
    return unless ( $#data >= 0 );
    my @head;
    my %noted;
    my %unmap;
    my $title = "";
    my $state = 0;
    my $ended = "";
    my $extra = "";
    my $notes = 0;
    my $codep = 0;

    for my $n ( 0 .. $#data ) {
        my $data = $data[$n];
        if ( $data =~ /^\s*#/ ) {
            @head = &add( \@head, "/*" ) unless ( $state == 1 );
            $data =~ s/#/ */;
            @head  = &add( \@head, $data );
            $state = 1;
        }
        elsif ( $data =~ /^\s*$/ ) {
            @head = &end_note( \@head, $notes++ ) if ( $state == 1 );

            if ( $state >= 2 ) {
                @head = &add( \@head, $ended );
                @head = &add_unmap( \@head, \%unmap, \%noted, $title );
                @head = &add( \@head, $extra ) if ( $extra ne "" );
            }
            @head = &add( \@head, "" );

            $title = "";
            %unmap = ();
            $state = 0;
            $ended = "";
            $extra = "";
        }
        elsif ( $data =~ /^map_/ ) {
            $title = $data;
            @head  = &end_note( \@head, $notes++ ) if ( $state == 1 );
            $state = 2;
            $codep = 0;
            $codep = 94 if ( $data =~ /_DEC_/ );
            $codep = 96 if ( $data =~ /_ISO_/ );

            $data .= "(code)" unless ( $data =~ /\(code/ );

            @head = &add( \@head, sprintf( "#define %s \\",           $data ) );
            @head = &add( \@head, sprintf( "\tbegin_CODEPAGE(%d) \\", $codep ) )
              if ($codep);
            @head = &add( \@head, "\tswitch (code) { \\" );

            $ended = $codep ? "\t} \\\n\tend_CODEPAGE()" : "\t}";

            # special case for map_DEC_Supp_Graphic
            if ( $data =~ /\(code,dft\)/ ) {
                $ended = "\t    default: dft; break; \\\n" . $ended;
                $extra = "\n#if OPT_WIDE_CHARS";
            }
        }
        elsif (
            $data =~ /^\s+(0x)?[[:xdigit:]]{2}
                            \s+(BLANK
                               |PUA\(\d\)
                               |UNDEF
                               |XK_\w+
                               |(0x|U\+)?[[:xdigit:]]{1,4})\s*/x
          )
        {
            @head  = &add( \@head, " */" ) if ( $state == 1 );
            $state = 3;
            my $note = "";
            if ( $data =~ /#/ ) {
                $note = $data;
                $note =~ s/[^#]*#\s*//;
                $note = "\t/* $note */" if ( $note ne "" );
                $data =~ s/\s*#.*//;
            }
            $data =~ s/\s+/ /g;
            $data =~ s/^ //;
            $data =~ s/ $//;
            my @fields = split /\s/, $data;
            my $source = &hex_of( $fields[0] );
            my $target = &hex_of( $fields[1] );
            my $intern = &hex_of( $fields[2] );
            my $macros = "UNI";
            $macros = "MAP" if ( $target =~ /^XK_/ );
            $macros = "XXX" if ( $target eq "UNDEF" );
            $macros = "XXX" if ( $target =~ /PUA\(\d\)/ );

            if ( $target ne $source ) {
                $intern = $source unless ($intern);
            }
            my $item = sprintf( "    %s(%s, %s)", $macros, $source, $target );

            # fix formatting for the XK_-based VT220 definitions
            if (    $codep == 0
                and $title !~ /(Greek|Hebrew|Turkish)/
                and index( $note, "\t/*" ) == 0 )
            {
                my $pads = 24 - length($item);
                $item .= "\t" if ( $pads > 0 );
            }
            @head = &add( \@head, sprintf( "\t%s%s \\", $item, $note ) );

            if ( defined $intern ) {
                if ( $source ne $intern ) {
                    $unmap{$source} = $intern;
                    $noted{$source} = $note;
                }
            }
        }
        else {
            printf STDERR "? unexpected data:\n\t%s\n", $data;
        }
    }
    if ( $state >= 2 ) {
        @head = &add( \@head, $ended );
        @head = &add_unmap( \@head, \%unmap, \%noted, $title );
        @head = &add( \@head, $extra ) if ( $extra ne "" );
    }
    @head = &add( \@head, "#else" );
    foreach my $key ( sort keys %wide_chars ) {
        @head =
          &add( \@head, sprintf( "#define %s(code)\t/* nothing */", $key ) );
        @head = &add( \@head, sprintf( "#define un%s(code,dft) dft", $key ) );
    }
    @head = &add( \@head, "#endif /* OPT_WIDE_CHARS */" );
    @head = &add( \@head, "" );
    @head = &add( \@head, "#endif /* included_charsets_h */" );

    my $origin = $file;
    $origin =~ s/\.dat\b/.h/;
    my $update = $origin . ".new";
    unlink $update;
    open( my $fh, ">", $update )
      or die "Can't open > $update.txt: $!";
    for my $n ( 0 .. $#head ) {
        printf $fh "%s\n", $head[$n];
    }
    close $fh;

    system("diff -u $origin $update") if $opt_v;
    rename $update, $origin if $opt_x;
    unlink $update;
}

sub main::HELP_MESSAGE() {
    printf STDERR <<EOF
Usage: $0 [options]

Options:
 -d       debug
 -i       import charset data from Unicode file
 -v       verbose
 -x       update charsets.h from $data_file
EOF
      ;
    exit 1;
}

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('i:vx') || &main::HELP_MESSAGE;
$#ARGV >= 0 && &main::HELP_MESSAGE;

&do_import($opt_i) if ($opt_i);
&do_update($data_file);

1;
