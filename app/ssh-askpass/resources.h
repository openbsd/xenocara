/* xscreensaver, Copyright (c) 1992, 1997 Jamie Zawinski <jwz@jwz.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

/* 1999-Nov-21 Modified by Jim Knoble <jmknoble@jmknoble.cx>.
 * Modifications:
 * 
 *   - Made get_boolean_resource() accept a third parameter, default_value,
 *     which determines the result of get_boolean_resource if either (a)
 *     no such resource exists, or (b) the resource value does not conform
 *     to the syntax of a boolean resource.
 * 
 *   - Same for get_integer_resource().
 * 
 *   - 1999-Dec-24 Trimmed unused functions.
 */

#ifndef __XSCREENSAVER_RESOURCES_H__
#define __XSCREENSAVER_RESOURCES_H__

extern char *get_string_resource (char*,char*);
extern Bool get_boolean_resource (char*,char*,Bool);
extern int get_integer_resource (char*,char*,int);
extern double get_float_resource (char*,char*);
extern unsigned int get_pixel_resource (char*,char*,Display*,Colormap,unsigned int);

#endif /* __XSCREENSAVER_RESOURCES_H__ */
