#!/usr/bin/env perl
# $XTermId: modify-keys.pl,v 1.89 2019/10/29 00:22:23 tom Exp $
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
# Print a table to illustrate the modifyOtherKeys resource choices.
#
# Some of the key combinations are unavailable unless certain translations
# resource settings are suppressed.  This command helped to verify those:
#	xterm -xrm '*omitTranslation:fullscreen,scroll-lock,shift-fonts'
#
# Additionally, a test-script was written to exercise xterm when the
# "Allow SendEvents" feature is enabled, in combination with keys sent by
# commands like this:
#	xdotool key --window XXX shift 2>/dev/null
#
# A curses application running in the target xterm showed the received data
# in the terminfo-style format used in this script.

# TODO factor in the backspace/delete meta/alt/escape resource-settings
# TODO show keycodes via "xmodmap -pk" as alternative to xkbcomp
# TODO show different sort-order (code, sym, xkb)
# TODO use U+xxxx codepoints in keysymdef.h for rendering plain text
# TODO optionally show 2**N, e.g., 4 (shift+control), 8 (shift+alt+control) or 16 (+meta) modifiers
# TODO optionally show -c (cursor) -e (edit) -f (function-keys) with modifiers

use strict;
use warnings;

use Getopt::Std;

$| = 1;

our ( $opt_d, $opt_h, $opt_k, $opt_K, $opt_l, $opt_m, $opt_o, $opt_u, $opt_v );

our $REPORT;
our @headers;
our @nolinks = ();
our ( $xkb_layout, $xkb_model );
our $keyfile = "/usr/include/X11/keysymdef.h";

our @keyNames;    # xkb's notion of key-names (undocumented)
our %keySyms;     # all keysyms, hashed by name
our %keyCodes;    # all keysyms, hashed by keycode
our %uniCodes;    # keysym Unicode values, hashed by keycode
our %uniNames;    # keysym Unicode descriptions, hashed by keycode
our @keyTypes;    # XkbKeyTypeRec
our @symCache;    # keysyms defined in keysymdef.h which might be used
our @symMap;      # index into symCache from keyNames
our %keysUsed;    # report derived from @symMap, etc.
our %linkUsed;    # check for uniqueness of html anchor-names

our $MAXMODS = 8; # maximum for modifier-param
our %Shifted;     # map keycode to shifted-keycode seen by xterm

# imitate /usr/include/X11/X.h
our $ShiftMask   = 1;
our $LockMask    = 2;
our $ControlMask = 4;
our $AltMask     = 8;     # assume mod1=alt
our $MetaMask    = 16;    # assume mod2=meta

our %editKeys = qw(
  XK_Delete             1
  XK_Prior              1
  XK_Next               1
  XK_Insert             1
  XK_Find               1
  XK_Select             1
  XK_KP_Delete          1
  XK_KP_Insert          1
  XK_ISO_Left_Tab       1
);

sub failed($) {
    printf STDERR "%s\n", $_[0];
    exit 1;
}

# prefer hex with 4 digit for hash keys
sub toCode($) {
    my $value = shift;
    $value = sprintf( "0x%04x", $value ) if ( $value =~ /^\d+$/ );
    return $value;
}

sub codeOf($) {
    my $value  = shift;
    my $result = 0;
    &failed("missing keysym") unless ( defined $value );
    if ( $value =~ /^\d+$/ ) {
        $result = $value;
    }
    elsif ( $value =~ /^0x[[:xdigit:]]+$/i ) {
        $result = hex $value;
    }
    elsif ( $value =~ /^XK_/ ) {
        $result = hex $keySyms{$value};
    }
    else {
        &failed("not a keysym: $value");
    }
    return $result;
}

# macros from <X11/Xutil.h>

sub IsKeypadKey($) {
    my $code = &codeOf( $_[0] );
    my $result = ( ( $code >= &codeOf("XK_KP_Space") )
          and ( $code <= &codeOf("XK_KP_Equal") ) ) ? 1 : 0;
    return $result;
}

sub IsPrivateKeypadKey($) {
    my $code = &codeOf( $_[0] );
    my $result =
      ( ( $code >= 0x11000000 ) and ( $code <= 0x1100FFFF ) ) ? 1 : 0;
    return $result;
}

sub IsCursorKey($) {
    my $code = &codeOf( $_[0] );
    my $result =
      ( ( $code >= &codeOf("XK_Home") ) and ( $code < &codeOf("XK_Select") ) )
      ? 1
      : 0;
    return $result;
}

sub IsPFKey($) {
    my $code = &codeOf( $_[0] );
    my $result =
      ( ( $code >= &codeOf("XK_KP_F1") ) and ( $code <= &codeOf("XK_KP_F4") ) )
      ? 1
      : 0;
    return $result;
}

sub IsFunctionKey($) {
    my $code = &codeOf( $_[0] );
    my $result =
      ( ( $code >= &codeOf("XK_F1") ) and ( $code <= &codeOf("XK_F35") ) )
      ? 1
      : 0;
    return $result;
}

sub IsMiscFunctionKey($) {
    my $code = &codeOf( $_[0] );
    my $result =
      ( ( $code >= &codeOf("XK_Select") ) and ( $code <= &codeOf("XK_Break") ) )
      ? 1
      : 0;
    return $result;
}

sub IsModifierKey($) {
    my $code   = &codeOf( $_[0] );
    my $result = (
        (
                  ( $code >= &codeOf("XK_Shift_L") )
              and ( $code <= &codeOf("XK_Hyper_R") )
        )
          or (  ( $code >= &codeOf("XK_ISO_Lock") )
            and ( $code <= &codeOf("XK_ISO_Level5_Lock") ) )
          or ( $code == &codeOf("XK_Mode_switch") )
          or ( $code == &codeOf("XK_Num_Lock") )
    ) ? 1 : 0;
    return $result;
}

# debugging/reporting

# Xutil.h's macros do not cover the whole range of special keys, which are not
# actually printable.
sub IsSpecialKey($) {
    my $code = &codeOf( $_[0] );
    my $result =
      ( ( $code >= 0xff00 ) and ( $code <= 0xffff ) )
      ? 1
      : 0;
    return $result;
}

sub VisibleChar($) {
    my $ch     = shift;
    my $ord    = ord $ch;
    my $result = $ch;
    if ( $ord < 32 ) {
        if ( $ord == 8 ) {
            $result = '\b';
        }
        elsif ( $ord == 9 ) {
            $result = '\t';
        }
        elsif ( $ord == 10 ) {
            $result = '\n';
        }
        elsif ( $ord == 12 ) {
            $result = '\f';
        }
        elsif ( $ord == 13 ) {
            $result = '\r';
        }
        elsif ( $ord == 27 ) {
            $result = '\E';
        }
        else {
            $result = sprintf( "^%c", $ord + 64 );
        }
    }
    elsif ( $ord == 32 ) {
        $result = '\s';
    }
    elsif ( $ord == 94 ) {
        $result = '\^';
    }
    elsif ( $ord == 92 ) {
        $result = '\\\\';
    }
    elsif ( $ord == 127 ) {
        $result = '^?';
    }
    return $result;
}

sub IsShift($$) {
    my $code   = shift;
    my $state  = shift;    # 0/1=normal, 2=shift
    my $result = 0;
    if ( ( ( $state - 1 ) & 1 ) != 0 ) {
        if ( $Shifted{$code} ) {
            return 1 if ( $Shifted{$code} ne $code );
        }
    }
    return 0;
}

sub TypeOf($) {
    my $code   = &toCode( $_[0] );
    my $result = "other";
    $result = "special"  if ( &IsSpecialKey($code) );
    $result = "keypad"   if ( &IsKeypadKey($code) );
    $result = "*keypad"  if ( &IsPrivateKeypadKey($code) );
    $result = "cursor"   if ( &IsCursorKey($code) );
    $result = "pf-key"   if ( &IsPFKey($code) );
    $result = "func-key" if ( &IsFunctionKey($code) );
    $result = "misc-key" if ( &IsMiscFunctionKey($code) );
    $result = "edit-key" if ( &IsEditFunctionKey($code) );
    $result = "modifier" if ( &IsModifierKey($code) );
    return $result;
}

sub KeyToS($$) {
    my $code   = &codeOf( $_[0] );
    my $state  = $_[1];
    my $result = "";

    $code = &codeOf( $Shifted{ $_[0] } ) if ( &IsShift( $_[0], $state ) );
    my $type = &TypeOf( &toCode($code) );

    if ( $type ne "other" ) {
        $result = ( $type eq "special" ) ? "-ignore-" : "?";
    }
    elsif ($opt_u) {
        $result = sprintf( "\\E[%d;%du", $code, $state );
    }
    else {
        $result = sprintf( "\\E[27;%d;%d~", $state, $code );
    }
    return $result;
}

sub ParamToQ($) {
    my $param  = shift;
    my $result = shift;
    $param--;
    $result .= ( $param & 1 ) ? 's' : '-';
    $result .= ( $param & 2 ) ? 'a' : '-';
    $result .= ( $param & 4 ) ? 'c' : '-';
    $result .= ( $param & 8 ) ? 'm' : '-';
    return $result;
}

sub ParamToS($) {
    my $param  = shift;
    my $result = "";
    if ( $param-- > 1 ) {
        $result .= "+Shift" if ( $param & 1 );
        $result .= "+Alt"   if ( $param & 2 );
        $result .= "+Ctrl"  if ( $param & 4 );
        $result .= "+Meta"  if ( $param & 8 );
        $result =~ s/^\+//;
    }
    return $result;
}

sub StateToS($) {
    my $state  = shift;
    my $result = "";
    $result .= "+Shift" if ( $state & $ShiftMask );
    $result .= "+Lock"  if ( $state & $LockMask );
    $result .= "+Ctrl"  if ( $state & $ControlMask );
    $result .= "+Alt"   if ( $state & $AltMask );
    $result .= "+Meta"  if ( $state & $MetaMask );
    $result =~ s/^\+//;
    return $result;
}

# macros/functions in xterm's input.c

sub Masked($$) {
    my $value  = shift;
    my $mask   = shift;
    my $result = ( ($value) & ( ~($mask) ) );
    return $result;
}

sub IsPredefinedKey($) {
    my $code   = &codeOf( $_[0] );
    my $result = 0;
    if ( $keySyms{"XK_ISO_Lock"} ) {
        $result =
          ( $code >= &codeOf("XK_ISO_Lock") and $code <= &codeOf("XK_Delete") )
          ? 1
          : 0;
    }
    else {
        $result =
          ( $code >= &codeOf("XK_BackSpace") and $code <= &codeOf("XK_Delete") )
          ? 1
          : 0;
    }
    return $result;
}

sub IsTabKey($) {
    my $code   = &codeOf( $_[0] );
    my $result = 0;
    if ( $keySyms{"XK_ISO_Left_Tab"} ) {
        $result =
          ( $code == &codeOf("XK_Tab") || $code == &codeOf("XK_ISO_Left_Tab") );
    }
    else {
        $result = ( $code == &codeOf("XK_Tab") ) ? 1 : 0;
    }
    return $result;
}

sub IsEditFunctionKey($) {
    my $code   = shift;
    my $result = 0;
    if ( $keyCodes{$code} ) {
        my $name = $keyCodes{$code};
        $result = 1 if ( $editKeys{$name} );
    }
    return $result;
}

sub IS_CTRL($) {
    my $code = &codeOf( $_[0] );
    my $result = ( $code < 32 || ( $code >= 0x7f && $code <= 0x9f ) );
    return $result;
}

sub IsControlInput($) {
    my $code   = &codeOf( $_[0] );
    my $result = 0;
    $result = 1 if ( $code >= 0x40 && $code <= 0x7f );
    return $result;
}

sub IsControlOutput($) {
    my $code   = shift;
    my $result = 0;
    $result = 1 if &IS_CTRL($code);
    return $result;
}

sub IsControlAlias($$) {
    my $code   = shift;
    my $state  = shift;
    my $result = 0;

    $code = &toCode($code);
    $code = &toCode( &AliasedKey($code) );
    if ( hex $code < 256 ) {
        $result = &IS_CTRL($code);

        # In xterm, this function does not directly test evt_state, but relies
        # upon kd.strbuf converted by Xutf8LookupString or XmbLookupString
        # (ultimately in _XTranslateKeysym).
        #
        # See https://www.mail-archive.com/xorg@lists.x.org/msg04434.html
        #
        # xterm does its own special cases for XK_BackSpace
        if ( $state & $ControlMask ) {
            my $ch = chr &codeOf($code);
            $result = 1 if ( &IsTabKey($code) );
            $result = 1 if ( &IsControlInput($code) );
            $result = 1 if ( $ch =~ /^[\/ 2-8]$/ );
        }
    }
    return $result;
}

sub computeMaskedModifier($$) {
    my $state  = shift;
    my $mask   = shift;
    my $result = &xtermStateToParam( &Masked( $state, $mask ) );
    return $result;
}

sub xtermStateToParam($) {
    my $state       = shift;
    my $modify_parm = 1;

    $modify_parm += 1 if ( $state & $ShiftMask );
    $modify_parm += 2 if ( $state & $AltMask );
    $modify_parm += 4 if ( $state & $ControlMask );
    $modify_parm += 8 if ( $state & $MetaMask );
    $modify_parm = 0 if ( $modify_parm == 1 );
    return $modify_parm;
}

sub ParamToState($) {
    my $modify_parm = shift;
    my $state       = 0;
    $modify_parm-- if ( $modify_parm > 0 );
    $state |= $ShiftMask   if ( $modify_parm & 1 );
    $state |= $AltMask     if ( $modify_parm & 2 );
    $state |= $ControlMask if ( $modify_parm & 4 );
    $state |= $MetaMask    if ( $modify_parm & 8 );
    return $state;
}

sub allowedCharModifiers($$) {
    my $other_key = shift;
    my $state     = shift;
    my $code      = shift;
    my $result = $state & ( $ShiftMask | $AltMask | $ControlMask | $MetaMask );

    # If modifyOtherKeys is off or medium (0 or 1), moderate its effects by
    # excluding the common cases for modifiers.
    if ( $other_key <= 1 ) {
        my $sym = $keyCodes{$code};
        if (    &IsControlInput($code)
            and &Masked( $result, $ControlMask ) == 0 )
        {
            # These keys are already associated with the control-key
            if ( $other_key == 0 ) {
                $result &= ~$ControlMask;
            }
        }
        elsif ( $sym eq "XK_Tab" || $sym eq "XK_Return" ) {
            #
        }
        elsif ( &IsControlAlias( $code, $state ) ) {

            # Things like "^_" work here...
            if ( &Masked( $result, ( $ControlMask | $ShiftMask ) ) == 0 ) {
                if ( $sym =~ /^XK_[34578]$/ or $sym eq "XK_slash" ) {
                    $result = 0 if ( $state == $ControlMask );
                }
                else {
                    $result = 0;
                }
            }
        }
        elsif ( !&IsControlOutput($code) && !&IsPredefinedKey($code) ) {

            # Printable keys are already associated with the shift-key
            if ( !( $result & $ControlMask ) ) {
                $result &= ~$ShiftMask;
            }
        }

        # TODO:
        #       result = filterAltMeta(result,
        #                              xw->work.meta_mods,
        #                              TScreenOf(xw)->meta_sends_esc, kd);
        #       if (TScreenOf(xw)->alt_is_not_meta) {
        #           result = filterAltMeta(result,
        #                                  xw->work.alt_mods,
        #                                  TScreenOf(xw)->alt_sends_esc, kd);
        #       }
    }
    return $result;
}

# Some details are omitted (e.g., the backspace/delete toggle), but this gives
# the general sense of the corresponding function in xterm's input.c
sub ModifyOtherKeys($$$$) {
    my $code        = shift;    # the keycode to test
    my $other_key   = shift;    # "modifyOtherKeys" resource
    my $modify_parm = shift;    # 0=unmodified, 2=shift, etc
    my $state       = shift;    # mask of modifiers, e.g., ControlMask
    my $result      = 0;

    $modify_parm = 0 if ( $modify_parm == 1 );

    if ( &IsModifierKey($code) ) {

        # xterm filters out bare modifiers (ignore)
    }
    elsif (&IsFunctionKey($code)
        or &IsEditFunctionKey($code)
        or &IsKeypadKey($code)
        or &IsCursorKey($code)
        or &IsPFKey($code)
        or &IsMiscFunctionKey($code)
        or &IsPrivateKeypadKey($code) )
    {
        # Exclude the keys already covered by a modifier.
    }
    elsif ( $state > 0 ) {
        my $sym = "";
        $sym = $keyCodes{$code} if ( $keyCodes{$code} );

        # TODO:
        #if (IsBackarrowToggle(keyboard, kd->keysym, state)) {
        #    kd->keysym = XK_Delete;
        #    UIntClr(state, ControlMask);
        #}
        if ( !&IsPredefinedKey($code) ) {
            $state = &allowedCharModifiers( $other_key, $state, $code );
        }
        if ( $state != 0 ) {
            if ( $other_key == 1 ) {
                if (   $sym eq "XK_BackSpace"
                    or $sym eq "XK_Delete" )
                {
                }
                elsif ( $sym eq "XK_ISO_Left_Tab" ) {
                    $result = 1
                      if ( &computeMaskedModifier( $state, $ShiftMask ) );
                }
                elsif ($sym eq "XK_Return"
                    or $sym eq "XK_Tab" )
                {
                    $result = ( $modify_parm != 0 );
                }
                else {
                    if ( &IsControlInput($code) ) {
                        if ( $state == $ControlMask or $state == $ShiftMask ) {
                            $result = 0;
                        }
                        else {
                            $result = ( $modify_parm != 0 );
                        }
                    }
                    elsif ( &IsControlAlias( $code, $state ) ) {
                        if ( $state == $ShiftMask ) {
                            $result = 0;
                        }
                        elsif ( &computeMaskedModifier( $state, $ControlMask ) )
                        {
                            $result = 1;
                        }
                    }
                    else {
                        $result = 1;
                    }
                }
                if ($result) {    # second case in xterm's Input()
                    $result = 0
                      if ( &allowedCharModifiers( $other_key, $state, $code ) ==
                        0 );
                }
            }
            elsif ( $other_key == 2 ) {
                if ( $sym eq "XK_BackSpace" ) {

                    # strip ControlMask as per IsBackarrowToggle()
                    $result = 1
                      if ( &computeMaskedModifier( $state, $ControlMask ) );
                }
                elsif ( $sym eq "XK_Delete" ) {

                    $result = ( &xtermStateToParam($state) != 0 );
                }
                elsif ( $sym eq "XK_ISO_Left_Tab" ) {
                    $result = 1
                      if ( &computeMaskedModifier( $state, $ShiftMask ) );
                }
                elsif ( $sym eq "XK_Return" or $sym eq "XK_Tab" ) {

                    $result = ( $modify_parm != 0 );
                }
                else {
                    if ( &IsControlInput($code) ) {
                        $result = 1;
                    }
                    elsif ( $state == $ShiftMask ) {
                        $result = ( $sym eq "XK_space" or $sym eq "XK_Return" );
                    }
                    elsif ( &computeMaskedModifier( $state, $ShiftMask ) ) {
                        $result = 1;
                    }
                }
            }
        }
    }
    return $result;
}

# See IsControlAlias. This handles some of the special cases where the keycode
# seen or used by xterm is not the same as the actual keycode.
sub AliasedKey($) {
    my $code   = &toCode( $_[0] );
    my $result = &codeOf($code);
    my $sym    = $keyCodes{$code};
    if ($sym) {
        $result = 8  if ( $sym eq "XK_BackSpace" );
        $result = 9  if ( $sym eq "XK_Tab" );
        $result = 13 if ( $sym eq "XK_Return" );
        $result = 27 if ( $sym eq "XK_Escape" );
    }
    return $result;
}

# Returns a short display for shift/control/alt modifiers applied to the
# keycode to show which are affected by "modifyOtherKeys" at the given level in
# $other_key
sub CheckOtherKey($$) {
    my $code      = shift;
    my $other_key = shift;
    my $modified  = 0;
    my $result    = "";
    for my $modify_parm ( 1 .. $MAXMODS ) {
        my $state = &ParamToState($modify_parm);
        if ( &ModifyOtherKeys( $code, $other_key, $modify_parm, $state ) ) {
            $modified++;
            $result .= "*";
        }
        else {
            $result .= "-";
        }
    }
    return $modified ? $result : "-(skip)-";
}

# Use the return-string from CheckOtherKeys as a template for deciding which
# keys to render as escape-sequences.
sub ShowOtherKeys($$$) {
    my $code = &AliasedKey( $_[0] );
    my $mode = $_[1];                  # modifyOtherKeys: 0, 1 or 2
    my $show = $_[2];
    my $type = &TypeOf( $_[0] );
    my @result;

    # index for $show[] can be tested with a bit-mask:
    # 1 = shift
    # 2 = alt
    # 4 = ctrl
    # 8 = meta
    for my $c ( 0 .. length($show) - 1 ) {
        my $rc = substr( $show, $c, 1 );
        if ( $rc eq "*" ) {
            $result[$c] = &KeyToS( &toCode($code), $c + 1 );
        }
        elsif ( $type eq "other" or ( $type eq "special" and $code < 256 ) ) {
            my $map   = $code;
            my $tmp   = &toCode($code);
            my $chr   = chr hex $tmp;
            my $shift = ( $c & 1 );
            my $cntrl = ( $c & 4 );

            # TODO - can this be simplified using xkb groups?
            if ( $chr =~ /^[`345678]$/ and ( $c & 4 ) != 0 ) {
                if ($shift) {
                    $map = 30      if ( $chr eq "`" );
                    $map = ord "#" if ( $chr eq "3" );
                    $map = ord '$' if ( $chr eq "4" );
                    $map = ord "%" if ( $chr eq "5" );
                    $map = 30      if ( $chr eq "6" );
                    $map = ord "&" if ( $chr eq "7" );
                    $map = ord "*" if ( $chr eq "8" );
                }
                else {
                    $map = 0   if ( $chr eq "`" );
                    $map = 27  if ( $chr eq "3" );
                    $map = 28  if ( $chr eq "4" );
                    $map = 29  if ( $chr eq "5" );
                    $map = 30  if ( $chr eq "6" );
                    $map = 31  if ( $chr eq "7" );
                    $map = 127 if ( $chr eq "8" );
                }
            }
            else {
                $map = &codeOf( $Shifted{$tmp} )
                  if ( defined( $Shifted{$tmp} ) and $shift );
                if ($cntrl) {
                    if ( $chr =~ /^[190:<=>.,+*()'&%\$#"!]$/ ) {

                        # ignore
                    }
                    elsif ( $chr =~ /^[2]$/ ) {
                        $map = 0;
                    }
                    elsif ( $chr =~ /^[:;]$/ ) {
                        $map = 27 if ( $mode > 0 );
                    }
                    elsif ( $chr eq '-' ) {
                        $map = 31 if ($shift);
                    }
                    elsif ( $chr eq '/' ) {
                        $map = $shift ? 127 : 31 if ( $mode == 0 );
                        $map = 31 if ( not $shift and $mode == 1 );
                    }
                    elsif ( $chr eq '?' ) {
                        $map = 127;
                    }
                    else {
                        $map = ( $code & 0x1f ) if ( $code < 128 );
                    }
                }
            }
            $result[$c] = &VisibleChar( chr $map );
        }
        elsif ( $type eq "special" ) {
            $result[$c] = "-ignore-";
        }
        else {
            $result[$c] = sprintf( "%d:%s", $c + 1, $type );
        }
    }
    return @result;
}

sub readfile($) {
    my $data = shift;
    my @data;
    if ( open my $fp, $data ) {
        @data = <$fp>;
        close $fp;
        chomp @data;
    }
    return @data;
}

sub readpipe($) {
    my $cmd = shift;
    return &readfile("$cmd 2>/dev/null |");
}

sub trim($) {
    my $text = shift;
    $text =~ s/^\s+//;
    $text =~ s/\s+$//;
    $text =~ s/\s+/ /g;
    return $text;
}

sub html_ref($) {
    my $header = shift;
    my $anchor = lc &trim($header);
    $anchor =~ s/\s/_/g;
    return $anchor;
}

sub rightarrow() {
    return $opt_h ? "&#8594;" : "->";
}

sub safe_html($) {
    my $text = shift;
    if ($opt_h) {
        $text =~ s/\&/\&amp;/g;
        $text =~ s/\</\&lt;/g;
        $text =~ s/\</\&gt;/g;
        if ( length($text) == 1 ) {
            my $s = "";
            for my $n ( 0 .. length($text) - 1 ) {
                my $ch = substr( $text, $n, 1 );
                my $ord = ord($ch);
                $s .= sprintf( "&#%d;", $ord ) if ( $ord >= 128 );
                $s .= $ch if ( $ord < 128 );
            }
            $text = $s;
        }
    }
    return $text;
}

sub begin_report() {
    if ($opt_o) {
        open( $REPORT, '>', $opt_o ) or &failed("cannot open $opt_o");
        select $REPORT;
    }
    if ($opt_h) {
        printf <<EOF
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN">

<html>
<head>
  <meta name="generator" content="$0">

  <title>XTERM - Modified "Other" Keys ($xkb_layout-$xkb_model)</title>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <meta name="keywords" content="xterm, special keys">
  <meta name="description" content="This is an example of xterm's modifyOtherKeys feature">
</head>

<body>
EOF
          ;
    }
}

sub end_report() {
    if ($opt_h) {
        my $output = "output.html";
        $output = $opt_o if ($opt_o);
        printf <<EOF
<div class="nav">
  <ul>
    <li class="nav-top"><a href="$output">(top)</a></li>
EOF
          ;
        for my $h ( 0 .. $#headers ) {
            printf "<li><a href=\"#%s\">%s</a></li>\n",
              &html_ref( $headers[$h] ), $headers[$h];
        }
        printf <<EOF
    </ul>
</div>
EOF
          ;
    }
    if ($opt_o) {
        select STDOUT;
        close $REPORT;
    }
}

sub begin_section($) {
    my $header = shift;
    $headers[ $#headers + 1 ] = $header;
    if ($opt_h) {
        printf "<h2><a name=\"%s\">%s</a></h2>\n", &html_ref($header), $header;
    }
    else {
        printf "\n";
        printf "%s:\n", $header;
    }
    printf STDERR "** %s\n", $header if ($opt_o);
}

sub begin_table() {
    my $title = shift;
    &begin_section($title);
    if ($opt_h) {
        printf "<table border=\"1\" summary=\"$title\">\n";
    }
}

sub end_table() {
    if ($opt_h) {
        printf "</table>\n";
    }
}

sub tt_cell($) {
    my $text = shift;
    return sprintf "<tt>%s</tt>", $text;
}

sub td_any($) {
    my $text = shift;
    return sprintf "<td>%s</td>", &tt_cell($text);
}

sub td_left($) {
    my $text = shift;
    return sprintf "<td align=\"left\">%s</td>", &tt_cell($text);
}

sub td_right($) {
    my $text = shift;
    return sprintf "<td align=\"right\">%s</td>", &tt_cell($text);
}

sub padded($$) {
    my $size = shift;
    my $text = shift;
    $text = sprintf( "%*s",  $size, $text ) if ( $size > 0 );
    $text = sprintf( "%-*s", $size, $text ) if ( $size < 0 );
    $text =~ s/ /&nbsp;/g if ($opt_h);
    return $text;
}

sub print_head() {
    my $argc = $#_;
    if ($opt_h) {
        printf "<tr>";
        for ( my $n = 0 ; $n <= $argc ; $n += 2 ) {
            my $size = $_[$n];
            my $text = &padded( $size, $_[ $n + 1 ] );
            printf "<th>%s</th>", $text;
        }
        printf "</tr>\n";
    }
    else {
        for ( my $n = 0 ; $n <= $argc ; $n += 2 ) {
            my $size = $_[$n];
            my $text = &padded( $size, $_[ $n + 1 ] );
            printf "%s", $text;
            printf " " if ( $n < $argc );
        }
        printf "\n";
    }
}

sub link_data($$) {
    my $thisis = shift;
    my $thatis = shift;
    my $column = shift;
    my $symbol = shift;
    my %result;
    $result{THISIS} = $thisis;    # current table name
    $result{THATIS} = $thatis;    # name of target table for link
    $result{COLUMN} = $column;    # column counting from 0
    $result{SYMBOL} = $symbol;
    return \%result;
}

sub unique_link($$) {
    my $thisis = shift;
    my $symbol = shift;
    my $unique = 0;
    for my $n ( 0 .. length($symbol) - 1 ) {
        $unique += ord substr( $symbol, $n, 1 );
    }
    return sprintf( "%s:%s.%x", $thisis, $symbol, $unique );
}

# print a row in the table, using pairs of lengths and strings:
# + Right-align lengths greater than zero and pad;
# + Left-align lengths less than zero, pad.
# + For the special case of zero, just left align without padding.
sub print_data() {
    my $argc = $#_;
    if ($opt_h) {
        my @links = @{ $_[0] };
        printf "<tr>";
        my $col = 0;
        for ( my $n = 1 ; $n <= $argc ; $n += 2 ) {
            my $size = $_[$n];
            my $text = &padded( $size, $_[ $n + 1 ] );
            if ( $#links >= 0 ) {
                for my $l ( 0 .. $#links ) {
                    my %obj = %{ $links[$l] };    # link_data
                    if ( $obj{COLUMN} == $col ) {
                        my $props = "";
                        my $value = &unique_link( $obj{THISIS}, $obj{SYMBOL} );

                        # The symbol-map from xkbcomp has duplicates because
                        # different modifier combinations can produce the same
                        # keysym.  Since it appears that the slots that the
                        # user would expect are filled in first, just ignoring
                        # the duplicate works well enought.
                        if ( not $linkUsed{$value} ) {
                            $props .= " name=\"$value\"";
                            $linkUsed{$value} = 1;
                        }
                        $value = &unique_link( $obj{THATIS}, $obj{SYMBOL} );
                        $props .= " href=\"#$value\"";
                        my $tail = $text;
                        $text =~ s/(\&nbsp;)+$//;
                        $tail = substr( $tail, length($text) );
                        $text =
                          sprintf( "<a %s>%s</a>%s", $props, $text, $tail );
                        last;
                    }
                }
            }
            printf "%s",
                ( $size > 0 )  ? &td_right($text)
              : ( $size == 0 ) ? &td_any($text)
              :                  &td_left($text);
            ++$col;
        }
        printf "</tr>\n";
    }
    else {
        for ( my $n = 1 ; $n <= $argc ; $n += 2 ) {
            my $size = $_[$n];
            my $text = &padded( $size, $_[ $n + 1 ] );
            printf "%s", $text;
            printf " " if ( $n < $argc );
        }
        printf "\n";
    }
}

sub begin_preformatted($) {
    my $title = shift;
    &begin_section($title);
    printf "<pre>\n" if ($opt_h);
}

sub end_preformatted() {
    printf "</pre>\n" if ($opt_h);
}

sub do_localectl($) {
    my $report = shift;
    my $cmd    = "localectl status";
    my @data   = &readpipe($cmd);
    &begin_table("Output of $cmd") if ($report);
    for my $n ( 0 .. $#data ) {

        # let command-line parameters override localectl output, for reports
        $data[$n] =~ s/^(\s+X11 Layout:\s+).*$/$1$opt_l/ if ($opt_l);
        $data[$n] =~ s/^(\s+X11 Model:\s+).*$/$1$opt_m/  if ($opt_m);
        my @fields = split /:\s*/, $data[$n];
        next unless ( $#fields == 1 );
        if ($report) {
            if ($opt_h) {
                printf "<tr>%s%s</tr>\n",
                  &td_right( $fields[0] ),
                  &td_left( $fields[1] );
            }
            else {
                printf "%s\n", $data[$n];
            }
        }
        $xkb_layout = $fields[1] if ( $fields[0] =~ /x11 layout/i );
        $xkb_model  = $fields[1] if ( $fields[0] =~ /x11 model/i );
    }
    if ($report) {
        &end_table;
    }
}

sub do_keysymdef() {
    my @data    = &readfile($keyfile);
    my $lenSyms = 0;
    for my $n ( 0 .. $#data ) {
        my $value = &trim( $data[$n] );
        next unless ( $value =~ /^#define\s+XK_/ );
        my $name = $value;
        $name =~ s/^#define\s+//;
        $value = $name;
        $name =~ s/\s.*//;
        $value =~ s/^[^\s]+\s+//;
        my $note = $value;
        $value =~ s/\s.*//;

        $note =~ s/^[^\s]+\s*//;
        if ( $note !~ /\b(alias|deprecated)\b/ ) {

            if ( $note =~ /\/*.*\bU\+[[:xdigit:]]{4,8}.*\*\// ) {
                next if ( $note =~ /\(U\+/ );
                my $code = $note;
                $code =~ s/^.*\bU\+([[:xdigit:]]+).*/$1/;
                $note =~ s/^\/\*[([:space:]]*//;
                $note =~ s/[)[:space:]]*\*\/$//;
                $uniNames{$value} = $note;
                $uniCodes{$value} = hex $code;
            }
        }
        $lenSyms        = length($name) if ( length($name) > $lenSyms );
        $value          = lc $value;
        $keySyms{$name} = $value;
        $keyCodes{$value} = $name unless ( $keyCodes{$value} );
        printf "keySyms{$name} = '$value', keyCodes{$value} = $name\n"
          if ($opt_d);
    }
    my $tmpfile = $keyfile;
    $tmpfile =~ s/^.*\///;
    &begin_table("Symbols from $tmpfile");
    my @keys = keys %keySyms;
    &print_data( \@nolinks, 5, sprintf( "%d", $#keys ),
        0, sprintf( "keysyms are defined (longest %d)", $lenSyms ) );
    @keys = keys %keyCodes;
    &print_data( \@nolinks, 5, sprintf( "%d", $#keys ),
        0, "keycodes are defined" );
    @keys = keys %uniCodes;
    &print_data( \@nolinks, 5, sprintf( "%d", $#keys ),
        0, "keycodes are equated to Unicode" );
    &end_table;
}

# For what it's worth, there is a C library (xkbfile) which could be used,
# but there is no documentation and would not actually solve the problem at
# hand.
#
# setxkbmap -model pc105 -layout us -print | xkbcomp - -C -o -
sub do_xkbcomp() {
    my @data =
      &readpipe( "setxkbmap "
          . "-model $xkb_model "
          . "-layout $xkb_layout -print "
          . "| xkbcomp - -C -o -" );
    my $state = -1;
    my $type  = {};
    for my $n ( 0 .. $#data ) {
        if ( $data[$n] =~ /static.*\bkeyNames\[.*{/ ) {
            $state = 0;
            next;
        }
        if ( $data[$n] =~ /static.*\bsymCache\[.*{/ ) {
            $state = 1;
            next;
        }
        if ( $data[$n] =~ /static.*\bsymMap\[.*{/ ) {
            $state = 2;
            next;
        }
        if ( $data[$n] =~ /static.*\bdflt_types\[.*{/ ) {
            $state = 3;
            next;
        }
        if ( $state >= 0 ) {
            if ( $data[$n] =~ /^\s*};/ ) {
                printf "# %s\n", $data[$n] if ($opt_d);
                $state = -1;
                next;
            }
            printf "* %s\n", $data[$n] if ($opt_d);
        }

        # parse data in "keyNames[NUM_KEYS]"
        if ( $state == 0 ) {
            my $text = $data[$n];
            my $name;
            while ( $text =~ /^.*".*".*$/ ) {
                $text =~ s/^[^"]*//;
                $name = $text;
                $name =~ s/"\s+}.*//;
                $name =~ s/"//g;
                $keyNames[ $#keyNames + 1 ] = $name;
                printf "keyNames[%d] = '%s'\n", $#keyNames,
                  $keyNames[$#keyNames]
                  if ($opt_v);
                $text =~ s/^"[^"]*"//;
            }
        }

        # parse data in "symCache[NUM_SYMBOLS]"
        elsif ( $state == 1 ) {
            my $text = $data[$n];
            my $name;
            while ( $text =~ /[[:alnum:]_]/ ) {
                $text =~ s/^[^[[:alnum:]_]*//;
                $name = $text;
                $name =~ s/[^[[:alnum:]_].*//;
                $symCache[ $#symCache + 1 ] = $name;
                printf "symCache[%d] = %s\n", $#symCache, $symCache[$#symCache]
                  if ($opt_v);
                $text =~ s/^[[:alnum:]_]+//;
            }
        }

        # parse data in "symMap[NUM_KEYS]"
        elsif ( $state == 2 ) {
            my $text = $data[$n];
            my $code;
            while ( $text =~ /[{].*[}]/ ) {
                my %obj;
                $text =~ s/^[^{]*[{]\s*//;
                $code = $text;
                $code =~ s/[^[[:alnum:]].*//;
                $text =~ s/[[:alnum:]]+\s*,\s*//;
                $obj{TYPE} = $code;    # KeyType
                my %tmp = %{ $keyTypes[$code] };
                $tmp{USED} += 1;
                $keyTypes[$code] = \%tmp;
                $code = $text;
                $code =~ s/[^[[:alnum:]].*//;
                $text =~ s/[[:alnum:]]+\s*,\s*//;
                $obj{USED} = hex $code;    # 0/1 for used/unused
                $code = $text;
                $code =~ s/[^[[:alnum:]].*//;
                $obj{CODE} = $code;        # index in symCache[]
                $text =~ s/[[:alnum:]]+\s*//;
                $symMap[ $#symMap + 1 ] = \%obj;
                printf "symMap[%d] = {%d,%d,%d}\n", $#symMap, $obj{TYPE},
                  $obj{USED}, $obj{CODE}
                  if ($opt_v);
            }
        }

        # parse data in "dflt_types[]"
        elsif ( $state == 3 ) {
            my $text = &trim( $data[$n] );
            if ( $text =~ /^\s*[}](,)?$/ ) {
                $type->{USED}               = 0;
                $keyTypes[ $#keyTypes + 1 ] = $type;
                $type                       = {};
            }
            elsif ( $text =~ /^\d+,$/ ) {
                $text =~ s/,//;
                $type->{SIZE} = $text;
            }
            elsif ( $text =~ /^None,\s+lnames_[[:alnum:]_]+$/ ) {
                $text =~ s/^None,\s+lnames_//;
                $type->{NAME} = $text;
            }
            elsif ( $text =~ /^\s*[{].*[}],\s*$/ ) {
                $text =~ s/^\s*[{]\s*([^,]+),.*/$1/;
                $type->{MODS} = $text;
            }
        }
    }
    &begin_table("Summary from xkbcomp");
    &print_data( \@nolinks, 5, sprintf( "%d", $#keyNames + 1 ), 0, "keyNames" );
    &print_data( \@nolinks, 5, sprintf( "%d", $#keyTypes + 1 ), 0, "keyTypes" );
    &print_data( \@nolinks, 5, sprintf( "%d", $#symCache + 1 ), 0, "symCache" );
    &print_data( \@nolinks, 5, sprintf( "%d", $#symMap + 1 ),   0, "symMap" );
    &end_table;
}

# Report keysymdef.h without the deprecated stuff, and sorted by keycode.
sub report_keysymdef() {
    &begin_table("Key symbols");
    &print_head( 0, "Code", 0, "Category", 0, "Symbol" );

    # sort by numeric keycode rather than string
    my @keyCodes = keys %keyCodes;
    my @sortCodes;
    for my $c ( 0 .. $#keyCodes ) {
        $sortCodes[$c] = sprintf "%08X", hex $keyCodes[$c];
    }
    @sortCodes = sort @sortCodes;
    for my $c ( 0 .. $#sortCodes ) {
        my $code = sprintf( "0x%04x", hex $sortCodes[$c] );
        my $sym = $keyCodes{$code};
        &print_data( \@nolinks, 9, $code, -8, &TypeOf($code), 0, $sym );
    }
    &end_table;
}

sub report_key_types() {
    &begin_table("Key types");
    &print_head( 5, "Type", 5, "Used", 5, "Levels", 0, "Name" );
    for my $t ( 0 .. $#keyTypes ) {
        my %type = %{ $keyTypes[$t] };
        next if ( $type{USED} == 0 and not $opt_v );
        &print_data(
            \@nolinks, 5, sprintf( "%d", $t ), 5,
            sprintf( "%d", $type{USED} ), 5, sprintf( "%d", $type{SIZE} ), 0,
            $type{NAME}
        );
    }
    &end_table;
}

sub report_modified_keys() {
    my @codes = sort keys %keysUsed;
    my $width = 14;
    &begin_table("Other modifiable keycodes");
    &print_head(
        0,       "Code",   0,       "Symbol", 0,       "Actual",
        -$width, "Mode 0", -$width, "Mode 1", -$width, "Mode 2"
    );
    $width = 0 if ($opt_h);
    for my $c ( 0 .. $#codes ) {
        next unless ( $codes[$c] ne "" );
        my @links;
        my $sym = $keysUsed{ $codes[$c] };
        $links[0] = &link_data( "summary", "detailed", 1, $sym );
        &print_data(
            \@links,
            6,   $codes[$c],                         #
            -20, $keysUsed{ $codes[$c] },            #
            -6,  sprintf( "%d", hex $codes[$c] ),    #
            -$width, &CheckOtherKey( $codes[$c], 0 ),    #
            -$width, &CheckOtherKey( $codes[$c], 1 ),    #
            -$width, &CheckOtherKey( $codes[$c], 2 )
        );
    }
    &end_table;
    &begin_preformatted("Modify-param to/from state");
    for my $param ( 0 .. $MAXMODS ) {
        my $state = &ParamToState($param);
        my $check = &xtermStateToParam($state);
        printf " PARAM %d %s %d %s %d (%s)\n", $param, &rightarrow,    #
          $state, &rightarrow,                                         #
          $check, &ParamToS($param);
    }
    &end_preformatted;
    &begin_preformatted("State to/from modify-param");
    for my $state ( 0 .. 15 ) {
        my $param = &xtermStateToParam($state);
        my $check = &ParamToState($param);
        printf " STATE %d %s %d %s %d (%s)\n",                         #
          $state, &rightarrow,                                         #
          $param, &rightarrow,                                         #
          $check, &StateToS($state);
    }
    &end_preformatted;
}

# Make a report showing user- and program-modes.
sub report_otherkey_escapes() {
    my @codes = sort keys %keysUsed;
    my $width = 14;
    &begin_table("Other modified-key escapes");
    &print_head(
        0,       "Code",   0,       "Symbol", 0,       "Actual",
        -$width, "Mode 0", -$width, "Mode 1", -$width, "Mode 2"
    );
    $width = 0 if ($opt_h);
    for my $c ( 0 .. $#codes ) {
        next unless ( $codes[$c] ne "" );
        my $level0 = &CheckOtherKey( $codes[$c], 0 );
        my $level1 = &CheckOtherKey( $codes[$c], 1 );
        my $level2 = &CheckOtherKey( $codes[$c], 2 );
        my @level0 = &ShowOtherKeys( $codes[$c], 0, $level0 );
        my @level1 = &ShowOtherKeys( $codes[$c], 1, $level1 );
        my @level2 = &ShowOtherKeys( $codes[$c], 2, $level2 );
        my @links;
        my $sym = $keysUsed{ $codes[$c] };
        $links[0] = &link_data( "detailed", "symmap", 1, $sym );
        &print_data(
            \@links,    #
            -6,  $codes[$c],                         #
            -20, $keysUsed{ $codes[$c] },            #
            -6,  sprintf( "%d", hex $codes[$c] ),    #
            -$width, $level0,                        #
            -$width, $level1,                        #
            -$width, $level2
        );

        for my $r ( 0 .. $#level0 ) {
            &print_data(
                \@nolinks,                           #
                -6,  &ParamToQ( $r + 1 ),            #
                -20, "",                             #
                -6,  "",                             #
                -$width, &safe_html( $level0[$r] ),  #
                -$width, &safe_html( $level1[$r] ),  #
                -$width, &safe_html( $level2[$r] )
            );
        }
    }
    &end_table;
}

sub report_keys_used() {
    &begin_table("Key map");
    &print_head(
        5, "Type",                                   #
        0, "Level",                                  #
        0, "Name",                                   #
        6, "Code",                                   #
        0,
        "Symbol"
    );
    for my $m ( 0 .. $#symMap ) {
        my %obj = %{ $symMap[$m] };
        next unless ( $obj{USED} );
        my $sym = $symCache[ $obj{CODE} ];
        next if ( $sym eq "NoSymbol" );
        my $code = "";
        $code = $keySyms{$sym} if ( $keySyms{$sym} );
        next if ( $code eq "" );
        $keysUsed{$code} = $sym;
        my %type = %{ $keyTypes[ $obj{TYPE} ] };
        my @links;
        $links[0] = &link_data( "symmap", "summary", 4, $sym );
        &print_data(
            \@links,
            5, sprintf( "%d",   $obj{TYPE} ),     #
            5, sprintf( "1/%d", $type{SIZE} ),    #
            -4, $keyNames[$m],                    #
            6,  $code,                            #
            0,  $sym
        );

        my $base = $code;
        $Shifted{$code} = $code unless ( $Shifted{$code} );

        for my $t ( 1 .. $type{SIZE} - 1 ) {
            $sym             = $symCache[ $obj{CODE} + $t ];
            $code            = "";
            $code            = $keySyms{$sym} if ( $keySyms{$sym} );
            $keysUsed{$code} = $sym;
            $links[0] = &link_data( "symmap", "summary", 4, $sym );
            &print_data(
                \@links,
                5,  "",                                         #
                5,  sprintf( "%d/%d", $t + 1, $type{SIZE} ),    #
                -4, "",                                         #
                6,  $code,                                      #
                0,  $sym
            );
            @links = ();

            # The shift-modifier could be used in custom groups, but the only
            # built-in ones that appear relevant are TWO_LEVEL and ALPHABETIC,
            # which have two levels.  This records the shifted code for a given
            # base.
            if (    $type{SIZE} == 2
                and $type{MODS}
                and index( $type{MODS}, "ShiftMask" ) >= 0 )
            {
                if ( $t == 1 ) {
                    $Shifted{$base} = $code;
                }
                elsif ( not $Shifted{$code} ) {
                    $Shifted{$code} = $code;
                }
            }
        }
    }
    &end_table;
}

sub KeyClasses($) {
    my $hex   = shift;
    my $alias = &IsControlAlias( $hex, $ControlMask ) ? "alias" : "";
    my $cntrl = &IS_CTRL($hex) ? "cntrl" : "";
    my $ctl_i = &IsControlInput($hex) ? "ctl_i" : "";
    my $ctl_o = &IsControlOutput($hex) ? "ctl_o" : "";
    my $this  = sprintf( "%-5s %-5s %-5s %-5s %-8s",
        $alias, $cntrl, $ctl_i, $ctl_o, &TypeOf($hex) );
}

sub report_key_classes() {
    &begin_table("Keycode-classes");
    my $base = -1;
    my $last = "";
    my $next = 65535;
    my $form = " [%8s .. %-8s] %s\n";
    &print_head( 0, "First", 0, "Last", 0, "Classes" ) if ($opt_h);
    for my $code ( 0 .. $next ) {
        my $hex  = &toCode($code);
        my $this = &KeyClasses($hex);
        if ( $base < 0 ) {
            $base = 0;
            $last = $this;
        }
        elsif ( $this ne $last ) {
            printf $form, &toCode($base), &toCode( $code - 1 ), $last
              unless ($opt_h);
            &print_data( \@nolinks, 0, &toCode($base), 0, &toCode( $code - 1 ),
                0, $last )
              if ($opt_h);
            $base = $code;
            $last = $this;
        }
    }
    printf $form, &toCode($base), &toCode($next), $last unless ($opt_h);
    &print_data( \@nolinks, 0, &toCode($base), 0, &toCode($next), 0, $last )
      if ($opt_h);
    &end_table;
}

sub main::HELP_MESSAGE() {
    printf STDERR <<EOF
Usage: $0 [options]

Options:
  -d      debug
  -h      write report with html-markup
  -k      dump keysyms/keycodes from $keyfile
  -K      dump keycode-classes
  -l XXX  use XXX for Xkb layout (default $xkb_layout)
  -m XXX  use XXX for Xkb model (default $xkb_model)
  -o XXX  write report to the file XXX.
  -u      use CSI u format for escapes
  -v      verbose

EOF
      ;
    exit 1;
}

binmode( STDOUT, ":utf8" );

&do_localectl(0);

$Getopt::Std::STANDARD_HELP_VERSION = 1;
&getopts('dhKkl:m:o:uv') || &main::HELP_MESSAGE;
$opt_v = 1 if ($opt_d);

&begin_report;

&do_localectl(1);

$xkb_layout = $opt_l if ($opt_l);
$xkb_model  = $opt_m if ($opt_m);

&do_keysymdef;
&report_keysymdef if ($opt_k);

&do_xkbcomp;

&report_key_classes if ($opt_K);

&report_key_types;
&report_keys_used;
&report_modified_keys;
&report_otherkey_escapes;

&end_report;

1;
