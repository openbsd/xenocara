/* xscreensaver, Copyright (c) 1992, 1997, 1998
 *  Jamie Zawinski <jwz@jwz.org>
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
 *   - Same for get_integer_resource(), get_pixel_resource().
 * 
 *   - 1999-Dec-24 Moved header includes from utils.h to here.
 *     Trimmed unused functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xresource.h>
#include "resources.h"


/* Resource functions.  Assumes: */

extern char *progname;
extern char *progclass;
extern XrmDatabase db;

#ifndef isupper
# define isupper(c)  ((c) >= 'A' && (c) <= 'Z')
#endif
#ifndef _tolower
# define _tolower(c)  ((c) - 'A' + 'a')
#endif

char *
get_string_resource (char *res_name, char *res_class)
{
  XrmValue value;
  char	*type;
  char full_name [1024], full_class [1024];
  int result;

  result = snprintf(full_name, sizeof(full_name), "%s.%s", 
      progname, res_name);
  if (result == -1 || result >= sizeof(full_name)) {
	  fprintf(stderr, "%s: resource name too long: %s.%s\n", progname,
	      progname, res_name);
	  return 0;
  }
  result = snprintf(full_class, sizeof(full_class), "%s.%s", 
      progclass, res_class);
  if (result == -1 || result >= sizeof(full_class)) {
	 fprintf(stderr, "%s: resource name too long: %s.%s\n", progname,
	      progclass, res_class);
	  return 0;
  }
  if (XrmGetResource (db, full_name, full_class, &type, &value))
    {
      char *str = (char *) malloc (value.size + 1);
      strncpy (str, (char *) value.addr, value.size);
      str [value.size] = 0;
      return str;
    }
  return 0;
}

Bool 
get_boolean_resource (char *res_name, char *res_class, Bool default_value)
{
  char *tmp, buf [100];
  char *s = get_string_resource (res_name, res_class);
  char *os = s;
  if (! s) return default_value;
  for (tmp = buf; *s; s++)
    *tmp++ = isupper (*s) ? _tolower (*s) : *s;
  *tmp = 0;
  free (os);

  while (*buf &&
	 (buf[strlen(buf)-1] == ' ' ||
	  buf[strlen(buf)-1] == '\t'))
    buf[strlen(buf)-1] = 0;

  if (!strcmp (buf, "on") || !strcmp (buf, "true") || !strcmp (buf, "yes"))
    return 1;
  if (!strcmp (buf,"off") || !strcmp (buf, "false") || !strcmp (buf,"no"))
    return 0;
  fprintf (stderr, "%s: %s must be boolean, not %s.\n",
	   progname, res_name, buf);
  return default_value;
}

int 
get_integer_resource (char *res_name, char *res_class, int default_value)
{
  int val;
  char c, *s = get_string_resource (res_name, res_class);
  char *ss = s;
  if (!s) return default_value;

  while (*ss && *ss <= ' ') ss++;			/* skip whitespace */

  if (ss[0] == '0' && (ss[1] == 'x' || ss[1] == 'X'))	/* 0x: parse as hex */
    {
      if (1 == sscanf (ss+2, "%x %c", &val, &c))
	{
	  free (s);
	  return val;
	}
    }
  else							/* else parse as dec */
    {
      if (1 == sscanf (ss, "%d %c", &val, &c))
	{
	  free (s);
	  return val;
	}
    }

  fprintf (stderr, "%s: %s must be an integer, not %s.\n",
	   progname, res_name, s);
  free (s);
  return default_value;
}

double
get_float_resource (char *res_name, char *res_class)
{
  double val;
  char c, *s = get_string_resource (res_name, res_class);
  if (! s) return 0.0;
  if (1 == sscanf (s, " %lf %c", &val, &c))
    {
      free (s);
      return val;
    }
  fprintf (stderr, "%s: %s must be a float, not %s.\n",
	   progname, res_name, s);
  free (s);
  return 0.0;
}


unsigned int
get_pixel_resource (char *res_name, char *res_class,
		    Display *dpy, Colormap cmap, unsigned int default_value)
{
  XColor color;
  char *s = get_string_resource (res_name, res_class);
  char *s2;
  if (!s) goto DEFAULT;

  for (s2 = s + strlen(s) - 1; s2 > s; s2--)
    if (*s2 == ' ' || *s2 == '\t')
      *s2 = 0;
    else
      break;

  if (! XParseColor (dpy, cmap, s, &color))
    {
      fprintf (stderr, "%s: can't parse color %s\n", progname, s);
      goto DEFAULT;
    }
  if (! XAllocColor (dpy, cmap, &color))
    {
      fprintf (stderr, "%s: couldn't allocate color %s\n", progname, s);
      goto DEFAULT;
    }
  free (s);
  return color.pixel;
 DEFAULT:
  if (s) free (s);
  return default_value;
}

