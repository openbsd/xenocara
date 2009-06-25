#!/usr/bin/perl -w
# -*- Mode: perl; indent-tabs-mode: nil; c-basic-offset: 2  -*-

# Perl script to create a ChangeLog entry with names of files
# and functions from a svn diff.
#
# Darin Adler <darin@bentspoon.com>, started 20 April 2000
# Java support added by Maciej Stachowiak <mjs@eazel.com>
# Adapted for subversion by Thomas Hellstrom <unichrome-at-shipmail-dot-org>
# last updated 11 May 2005
#
# (Someone put a license in here, like maybe GPL.)
#
# TODO:
#   For new files, just say "New file" instead of listing
#     function names.
#   List functions that have been removed too.
#   Decide what a good logical order is for the changed files
#     other than a normal text "sort" (top level first?)
#     (group directories?) (.h before .c?)
#   Leave a diff file behind if asked, but in unified format.
#   Handle C++ and yacc source files too (other languages?).
#   Help merge when there are ChangeLog conflicts or if there's
#     already a partly written ChangeLog entry.
#   Add command line option to put the ChangeLog into a separate
#     file or just spew it out stdout.
#   Figure out how to allow -z options from .cvsrc to work without
#     letting other bad options work. Currently the -f disables
#     everything from the .cvsrc.
#   Add CVS version numbers for each file too (can't do that until
#     the changes are checked in, though).
#   Work around diff stupidity where deleting a function that starts
#     with a comment makes diff think that the following function
#     has been changed (if the following function starts with a comment
#     with the same first line, such as /**)
#   Work around diff stupidity where deleting an entire function and
#     the blank lines before it makes diff think you've changed the
#     previous function.

use diagnostics;
use strict;

use English;
use Text::Wrap;

# For each file, build a list of modified lines.
# Use line numbers from the "after" side of each diff.
print STDERR "  Running svn diff to find changes.\n";
my %changed_line_ranges;
my $file;
open DIFF, "svn diff --diff-cmd diff -x -N |" or die "The svn diff failed: $OS_ERROR.\n";
while (<DIFF>)
  {
    $file = $1 if /^Index: (\S+)$/;
    if (defined $file
        and $file ne "ChangeLog"
        and (/^\d+(,\d+)?[acd](\d+)(,(\d+))?/ or /^Binary files/) )
      {
        push @{$changed_line_ranges{$file}}, [ $2, $4 || $2 ];
      }
  }
close DIFF;
if (!%changed_line_ranges)
  {
    print STDERR "  No changes found.\n";
    exit;
  }

# For each ".c" file, convert line range to function list.
print STDERR "  Extracting affected function names from C source files.\n";
my %function_lists;
foreach my $file (keys %changed_line_ranges)
  {
    # An empty function list still indicates that something changed.
    $function_lists{$file} = "";

    # Only look for function names in .c files.
    next unless $file =~ /\.(c|java)/;

    # Find all the functions in the file.
    open SOURCE, $file or next;
    my @function_ranges = get_function_line_ranges(\*SOURCE, $file);
    close SOURCE;

    # Find all the modified functions.
    my @functions;
    my %saw_function;
    my @change_ranges = (@{$changed_line_ranges{$file}}, []);
    my @change_range = (0, 0);
    FUNCTION: foreach my $function_range_ref (@function_ranges)
      {
        my @function_range = @$function_range_ref;

        # Advance to successive change ranges.
        for (;; @change_range = @{shift @change_ranges})
          {
            last FUNCTION unless @change_range;

            # If past this function, move on to the next one.
            next FUNCTION if $change_range[0] > $function_range[1];

            # If an overlap with this function range, record the function name.
            if ($change_range[1] >= $function_range[0]
                and $change_range[0] <= $function_range[1])
              {
                if (!$saw_function{$function_range[2]})
                  {
                    $saw_function{$function_range[2]} = 1;
                    push @functions, $function_range[2];
                  }
                next FUNCTION;
              }
          }
      }

    # Format the list of functions now.
    $function_lists{$file} = " (" . join("), (", @functions) . "):" if @functions;
  }

# Get some pieces of the ChangeLog we are about to write.
my $date = sprintf "%d-%02d-%02d",
  1900 + (localtime $BASETIME)[5], # year
  1 + (localtime $BASETIME)[4], # month
  (localtime $BASETIME)[3]; # day within month
my $name = $ENV{CHANGE_LOG_NAME}
  || $ENV{REAL_NAME}
  || (getpwuid $REAL_USER_ID)[6]
  || "set REAL_NAME environment variable";
my $email_address = $ENV{CHANGE_LOG_EMAIL_ADDRESS}
  || $ENV{EMAIL_ADDRESS}
  || "set EMAIL_ADDRESS environment variable";

# Find the change logs.
my %has_log;
my %files;
foreach my $file (sort keys %function_lists)
  {
    my $prefix = $file;
    my $has_log = 0;
    while ($prefix)
      {
        $prefix =~ s-/[^/]+/?$-/- or $prefix = "";
        $has_log = $has_log{$prefix};
        if (!defined $has_log)
          {
            $has_log = -f "${prefix}ChangeLog";
            $has_log{$prefix} = $has_log;
          }
        last if $has_log;
      }
    if (!$has_log)
      {
        print STDERR "No ChangeLog found for $file.\n";
      }
    else
      {
        push @{$files{$prefix}}, $file;
      }
  }

# Get the latest ChangeLog files from svn.
my $logs = "";
foreach my $prefix (sort keys %files)
  {
    $logs .= " ${prefix}ChangeLog";
  }
if ($logs)
  {
    print STDERR "  Updating ChangeLog files from svn repository.\n";
    open ERRORS, "svn update$logs |" or die "The svn update of ChangeLog files failed: $OS_ERROR.\n";
    print STDERR "    $ARG" while <ERRORS>;
    close ERRORS;
  }


# Write out a new ChangeLog file.
foreach my $prefix (sort keys %files)
  {
    print STDERR "  Editing the ${prefix}ChangeLog file.\n";
    open OLD_CHANGE_LOG, "${prefix}ChangeLog" or die "Could not open ${prefix}ChangeLog file: $OS_ERROR.\n";
    # It's less efficient to read the whole thing into memory than it would be
    # to read it while we prepend to it later, but I like doing this part first.
    my @old_change_log = <OLD_CHANGE_LOG>;
    close OLD_CHANGE_LOG;
    open CHANGE_LOG, "> ${prefix}ChangeLog" or die "Could not write ${prefix}ChangeLog\n.";
    print CHANGE_LOG "$date  $name  <$email_address>\n\n";
    print CHANGE_LOG "\treviewed by: <delete if not using a buddy>\n\n";
    foreach my $file (sort @{$files{$prefix}})
      {
        my $file_stem = substr $file, length $prefix;
        my $lines = wrap("\t", "\t", "XX$file_stem:$function_lists{$file}");
        $lines =~ s/^\tXX/\t* /;
        print CHANGE_LOG "$lines\n";
      }
    print CHANGE_LOG "\n", @old_change_log;
    close CHANGE_LOG;
    print STDERR "  Done editing ${prefix}ChangeLog.\n";
  }

# Done.
exit;

sub get_function_line_ranges
  {
    my ($file_handle, $file_name) = @_;

    if ($file_name =~ /\.c$/) {
        return get_function_line_ranges_for_c ($file_handle, $file_name);
    } elsif ($file_name =~ /\.java$/) {
        return get_function_line_ranges_for_java ($file_handle, $file_name);
    }
    return ();
  }

# Read a file and get all the line ranges of the things that look like C functions.
# A function name is the last word before an open parenthesis before the outer
# level open brace. A function starts at the first character after the last close
# brace or semicolon before the function name and ends at the close brace.
# Comment handling is simple-minded but will work for all but pathological cases.
#
# Result is a list of triples: [ start_line, end_line, function_name ].

sub get_function_line_ranges_for_c
  {
    my ($file_handle, $file_name) = @_;

    my @ranges;

    my $in_comment = 0;
    my $in_macro = 0;
    my $in_parentheses = 0;
    my $in_braces = 0;

    my $word = "";

    my $potential_start = 0;
    my $potential_name = "";

    my $start = 0;
    my $name = "";

    while (<$file_handle>)
      {
        # Handle continued multi-line comment.
        if ($in_comment)
          {
            next unless s-.*\*/--;
            $in_comment = 0;
          }

        # Handle continued macro.
        if ($in_macro)
          {
            $in_macro = 0 unless /\\$/;
            next;
          }

        # Handle start of macro (or any preprocessor directive).
        if (/^\s*\#/)
          {
            $in_macro = 1 if /^([^\\]|\\.)*\\$/;
            next;
          }

        # Handle comments and quoted text.
        while (m-(/\*|//|\'|\")-) # \' and \" keep emacs perl mode happy
          {
            my $match = $1;
            if ($match eq "/*")
              {
                if (!s-/\*.*?\*/--)
                  {
                    s-/\*.*--;
                    $in_comment = 1;
                  }
              }
            elsif ($match eq "//")
              {
                s-//.*--;
              }
            else # ' or "
              {
                if (!s-$match([^\\]|\\.)*?$match--)
                  {
                    warn "mismatched quotes at line $INPUT_LINE_NUMBER in $file_name\n";
                    s-$match.*--;
                  }
              }
          }

        # Find function names.
        while (m-(\w+|[(){};])-g)
          {
            # Open parenthesis.
            if ($1 eq "(")
              {
                $potential_name = $word unless $in_parentheses;
                $in_parentheses++;
                next;
              }

            # Close parenthesis.
            if ($1 eq ")")
              {
                $in_parentheses--;
                next;
              }

            # Open brace.
            if ($1 eq "{")
              {
                # Promote potiential name to real function name at the
                # start of the outer level set of braces (function body?).
                if (!$in_braces and $potential_start)
                  {
                    $start = $potential_start;
                    $name = $potential_name;
                  }

                $in_braces++;
                next;
              }

            # Close brace.
            if ($1 eq "}")
              {
                $in_braces--;

                # End of an outer level set of braces.
                # This could be a function body.
                if (!$in_braces and $name)
                  {
                    push @ranges, [ $start, $INPUT_LINE_NUMBER, $name ];
                    $name = "";
                  }

                $potential_start = 0;
                $potential_name = "";
                next;
              }

            # Semicolon.
            if ($1 eq ";")
              {
                $potential_start = 0;
                $potential_name = "";
                next;
              }

            # Word.
            $word = $1;
            if (!$in_parentheses)
              {
                $potential_start = 0;
                $potential_name = "";
              }
            if (!$potential_start)
              {
                $potential_start = $INPUT_LINE_NUMBER;
                $potential_name = "";
              }
          }
      }

    warn "mismatched braces in $file_name\n" if $in_braces;
    warn "mismatched parentheses in $file_name\n" if $in_parentheses;

    return @ranges;
  }



# Read a file and get all the line ranges of the things that look like Java
# classes, interfaces and methods.
#
# A class or interface name is the word that immediately follows
# `class' or `interface' when followed by an open curly brace and not
# a semicolon. It can appear at the top level, or inside another class
# or interface block, but not inside a function block
#
# A class or interface starts at the first character after the first close
# brace or after the function name and ends at the close brace.
#
# A function name is the last word before an open parenthesis before
# an open brace rather than a semicolon. It can appear at top level or
# inside a class or interface block, but not inside a function block.
#
# A function starts at the first character after the first close
# brace or after the function name and ends at the close brace.
#
# Comment handling is simple-minded but will work for all but pathological cases.
#
# Result is a list of triples: [ start_line, end_line, function_name ].

sub get_function_line_ranges_for_java
  {
    my ($file_handle, $file_name) = @_;

    my @current_scopes;

    my @ranges;

    my $in_comment = 0;
    my $in_macro = 0;
    my $in_parentheses = 0;
    my $in_braces = 0;
    my $in_non_block_braces = 0;
    my $class_or_interface_just_seen = 0;

    my $word = "";

    my $potential_start = 0;
    my $potential_name = "";
    my $potential_name_is_class_or_interface = 0;

    my $start = 0;
    my $name = "";
    my $current_name_is_class_or_interface = 0;

    while (<$file_handle>)
      {
        # Handle continued multi-line comment.
        if ($in_comment)
          {
            next unless s-.*\*/--;
            $in_comment = 0;
          }

        # Handle continued macro.
        if ($in_macro)
          {
            $in_macro = 0 unless /\\$/;
            next;
          }

        # Handle start of macro (or any preprocessor directive).
        if (/^\s*\#/)
          {
            $in_macro = 1 if /^([^\\]|\\.)*\\$/;
            next;
          }

        # Handle comments and quoted text.
        while (m-(/\*|//|\'|\")-) # \' and \" keep emacs perl mode happy
          {
            my $match = $1;
            if ($match eq "/*")
              {
                if (!s-/\*.*?\*/--)
                  {
                    s-/\*.*--;
                    $in_comment = 1;
                  }
              }
            elsif ($match eq "//")
              {
                s-//.*--;
              }
            else # ' or "
              {
                if (!s-$match([^\\]|\\.)*?$match--)
                  {
                    warn "mismatched quotes at line $INPUT_LINE_NUMBER in $file_name\n";
                    s-$match.*--;
                  }
              }
          }

        # Find function names.
        while (m-(\w+|[(){};])-g)
          {
            # Open parenthesis.
            if ($1 eq "(")
              {
                if (!$in_parentheses) {
                    $potential_name = $word;
                    $potential_name_is_class_or_interface = 0;
                }
                $in_parentheses++;
                next;
              }

            # Close parenthesis.
            if ($1 eq ")")
              {
                $in_parentheses--;
                next;
              }

            # Open brace.
            if ($1 eq "{")
              {
                # Promote potiential name to real function name at the
                # start of the outer level set of braces (function/class/interface body?).
                if (!$in_non_block_braces
                    and (!$in_braces or $current_name_is_class_or_interface)
                    and $potential_start)
                  {
                    if ($name)
                      {
                          push @ranges, [ $start, ($INPUT_LINE_NUMBER - 1),
                                          join ('.', @current_scopes) ];
                      }


                    $current_name_is_class_or_interface = $potential_name_is_class_or_interface;

                    $start = $potential_start;
                    $name = $potential_name;

                    push (@current_scopes, $name);
                  } else {
                      $in_non_block_braces++;
                  }

                $potential_name = "";
                $potential_start = 0;

                $in_braces++;
                next;
              }

            # Close brace.
            if ($1 eq "}")
              {
                $in_braces--;

                # End of an outer level set of braces.
                # This could be a function body.
                if (!$in_non_block_braces)
                  {
                    if ($name)
                      {
                        push @ranges, [ $start, $INPUT_LINE_NUMBER,
                                        join ('.', @current_scopes) ];

                        pop (@current_scopes);

                        if (@current_scopes)
                          {
                            $current_name_is_class_or_interface = 1;

                            $start = $INPUT_LINE_NUMBER + 1;
                            $name =  $current_scopes[$#current_scopes-1];
                          }
                        else
                          {
                            $current_name_is_class_or_interface = 0;
                            $start = 0;
                            $name =  "";
                          }
                    }
                  }
                else
                  {
                    $in_non_block_braces-- if $in_non_block_braces;
                  }

                $potential_start = 0;
                $potential_name = "";
                next;
              }

            # Semicolon.
            if ($1 eq ";")
              {
                $potential_start = 0;
                $potential_name = "";
                next;
              }

            if ($1 eq "class" or $1 eq "interface")
              {
                $class_or_interface_just_seen = 1;
                next;
              }

            # Word.
            $word = $1;
            if (!$in_parentheses)
              {
                if ($class_or_interface_just_seen) {
                    $potential_name = $word;
                    $potential_start = $INPUT_LINE_NUMBER;
                    $class_or_interface_just_seen = 0;
                    $potential_name_is_class_or_interface = 1;
                    next;
                }
              }
            if (!$potential_start)
              {
                $potential_start = $INPUT_LINE_NUMBER;
                $potential_name = "";
              }
            $class_or_interface_just_seen = 0;
          }
      }

    warn "mismatched braces in $file_name\n" if $in_braces;
    warn "mismatched parentheses in $file_name\n" if $in_parentheses;

    return @ranges;
  }
