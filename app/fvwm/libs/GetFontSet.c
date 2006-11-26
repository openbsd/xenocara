/* This file brings from GetFont.c */

#include "../configure.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "fvwmlib.h"

/*
** loads fontset or "fixed" on failure
*/
XFontSet GetFontSetOrFixed(Display *disp, char *fontname)
{
  XFontSet fontset;
  char **ml;
  int mc;
  char *ds;

  if ((fontset = XCreateFontSet(disp,fontname,&ml,&mc,&ds))==NULL)
  {
    fprintf(stderr,
            "[FVWM][GetFontSetOrFixed]: WARNING -- can't get fontset %s, trying 'fixed'\n",
            fontname);
    /* fixed should always be avail, so try that */
    /* plain X11R6.3 hack */
    if ((fontset = XCreateFontSet(disp,"fixed,-*--14-*",&ml,&mc,&ds))==NULL)
    {
      fprintf(stderr,"[FVWM][GetFontSetOrFixed]: ERROR -- can't get fontset 'fixed'\n");
    }
  }
  return fontset;
}

