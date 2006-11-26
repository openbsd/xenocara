/* ********************************************************************** */

/* xvertext, Copyright (c) 1992 Alan Richardson (mppa3@uk.ac.sussex.syma)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both the
 * copyright notice and this permission notice appear in supporting
 * documentation.  All work developed as a consequence of the use of
 * this program should duly acknowledge such use. No representations are
 * made about the suitability of this software for any purpose.  It is
 * provided "as is" without express or implied warranty.
 */

/* ********************************************************************** */


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Rotated.h"


/* ---------------------------------------------------------------------- */


int xv_errno;

static char *my_strdup(char *);
static char *my_strtok(char *, char *);


/* ---------------------------------------------------------------------- */  


/* *** Routine to mimic `strdup()' (some machines don't have it) *** */

static char *my_strdup(char *str)
{
  char *s;
  size_t len;

  if (str == NULL) return NULL;

  len = strlen(str) + 1;
  s = (char *)malloc(len);
  /* this error is highly unlikely ... */
  if (s == NULL) {
    fprintf(stderr, "Fatal error: my_strdup(): Couldn't do malloc (gulp!)\n");
    exit(1); 
  }

  strlcpy(s, str, len);
  return s;
}


/* ---------------------------------------------------------------------- */


/* *** Routine to replace `strtok' : this one returns a zero
       length string if it encounters two consecutive delimiters *** */

static char *my_strtok(char *str1, char *str2)
{
  char *ret;
  int i, j, stop;
  static int start, len;
  static char *stext;

  /* this error should never occur ... */
  if (str2 == NULL) {
    fprintf(stderr,
	    "Fatal error: my_strtok(): recieved null delimiter string\n");
    exit(1);
  }

  /* initialise if str1 not NULL ... */
  if (str1 != NULL) {
    start = 0;
    stext = str1;
    len = strlen(str1);
  }

  /* run out of tokens ? ... */
  if (start >= len) return NULL;

  /* loop through characters ... */
  for (i = start; i < len; i++) {

    /* loop through delimiters ... */
    stop = 0;
    for (j = 0; j < strlen(str2); j++)
      if (stext[i] == str2[j]) stop = 1;
 
    if (stop) break;
  }

  stext[i] = '\0';
  ret = stext + start;
  start = i+1;

  return ret;
}


/* ---------------------------------------------------------------------- */
  

/* *** Routine to return version/copyright information *** */

float XRotVersion(char *str, int n)
{
  if (str != NULL) strncpy(str, XV_COPYRIGHT, n);
  return XV_VERSION;
}


/* ---------------------------------------------------------------------- */


/* *** Load the rotated version of a given font *** */
 
XRotFontStruct *XRotLoadFont(Display *dpy, char *fontname, float angle)
{
  char val;
  XImage *I1, *I2;
  Pixmap canvas;
  Window root;
  int screen;
  GC font_gc;
  char text[3];/*, errstr[300];*/
  XFontStruct *fontstruct;
  XRotFontStruct *rotfont;
  int ichar, i, j, index, boxlen = 60, dir;
  int vert_w, vert_h, vert_len, bit_w, bit_h, bit_len;
  int min_char, max_char;
  unsigned char *vertdata, *bitdata;
  int ascent, descent, lbearing, rbearing;
  int on = 1, off = 0;

  /* make angle positive ... */
  if (angle < 0) do angle += 360; while (angle < 0);

  /* get nearest vertical or horizontal direction ... */
  dir = (int)((angle+45.)/90.)%4;

  /* useful macros ... */
  screen = DefaultScreen(dpy);
  root = DefaultRootWindow(dpy);

  /* create the depth 1 canvas bitmap ... */
  canvas = XCreatePixmap(dpy, root, boxlen, boxlen, 1);
 
  /* create a GC ... */
  font_gc = XCreateGC(dpy, canvas, 0, 0);
  XSetBackground(dpy, font_gc, off);

  /* load the font ... */
  fontstruct = XLoadQueryFont(dpy, fontname);
  if (fontstruct == NULL) {
    xv_errno = XV_NOFONT;
    return NULL;
  }
 
  XSetFont(dpy, font_gc, fontstruct->fid);

  /* allocate space for rotated font ... */
  rotfont = (XRotFontStruct *)malloc((unsigned)sizeof(XRotFontStruct));
  if (rotfont == NULL) {
    xv_errno = XV_NOMEM;
    return NULL;
  }
   
  /* determine which characters are defined in font ... */
  min_char = fontstruct->min_char_or_byte2; 
  max_char = fontstruct->max_char_or_byte2;
 
  /* we only want printing characters ... */
  if (min_char<32)  min_char = 32;
  if (max_char>126) max_char = 126;
     
  /* some overall font data ... */
  rotfont->name = my_strdup(fontname);
  rotfont->dir = dir;
  rotfont->min_char = min_char;
  rotfont->max_char = max_char;
  rotfont->max_ascent = fontstruct->max_bounds.ascent;
  rotfont->max_descent = fontstruct->max_bounds.descent;   
  rotfont->height = rotfont->max_ascent+rotfont->max_descent;

  /* remember xfontstruct for `normal' text ... */
  if (dir == 0) rotfont->xfontstruct = fontstruct;

  else {
    /* font needs rotation ... */
    /* loop through each character ... */
    for (ichar = min_char; ichar <= max_char; ichar++) {

      index = ichar-fontstruct->min_char_or_byte2;
 
      /* per char dimensions ... */
      ascent =   rotfont->per_char[ichar-32].ascent = 
	fontstruct->per_char[index].ascent;
      descent =  rotfont->per_char[ichar-32].descent = 
	fontstruct->per_char[index].descent;
      lbearing = rotfont->per_char[ichar-32].lbearing = 
	fontstruct->per_char[index].lbearing;
      rbearing = rotfont->per_char[ichar-32].rbearing = 
	fontstruct->per_char[index].rbearing;
      rotfont->per_char[ichar-32].width = 
	fontstruct->per_char[index].width;

      /* some space chars have zero body, but a bitmap can't have ... */
      if (!ascent && !descent)   
	ascent =   rotfont->per_char[ichar-32].ascent =   1;
      if (!lbearing && !rbearing) 
	rbearing = rotfont->per_char[ichar-32].rbearing = 1;

      /* glyph width and height when vertical ... */
      vert_w = rbearing-lbearing;
      vert_h = ascent+descent;

      /* width in bytes ... */
      vert_len = (vert_w-1)/8+1;   
 
      XSetForeground(dpy, font_gc, off);
      XFillRectangle(dpy, canvas, font_gc, 0, 0, boxlen, boxlen);

      /* draw the character centre top right on canvas ... */
      snprintf(text, sizeof(text), "%c", ichar);
      XSetForeground(dpy, font_gc, on);
      XDrawImageString(dpy, canvas, font_gc, boxlen/2 - lbearing,
		       boxlen/2 - descent, text, 1);

      /* reserve memory for first XImage ... */
      vertdata = (unsigned char *) malloc((unsigned)(vert_len*vert_h));
      if (vertdata == NULL) {
	xv_errno = XV_NOMEM;
	return NULL;
      }
  
      /* create the XImage ... */
      I1 = XCreateImage(dpy, DefaultVisual(dpy, screen), 1, XYBitmap,
			0, (char *)vertdata, vert_w, vert_h, 8, 0);

      if (I1 == NULL) {
	xv_errno = XV_NOXIMAGE;
	return NULL;
      }
  
      I1->byte_order = I1->bitmap_bit_order = MSBFirst;
   
      /* extract character from canvas ... */
      XGetSubImage(dpy, canvas, boxlen/2, boxlen/2-vert_h,
		   vert_w, vert_h, 1, XYPixmap, I1, 0, 0);
      I1->format = XYBitmap; 
 
      /* width, height of rotated character ... */
      if (dir == 2) { 
	bit_w = vert_w;
	bit_h = vert_h; 
      } else {
	bit_w = vert_h;
	bit_h = vert_w; 
      }

      /* width in bytes ... */
      bit_len = (bit_w-1)/8 + 1;

      rotfont->per_char[ichar-32].glyph.bit_w = bit_w;
      rotfont->per_char[ichar-32].glyph.bit_h = bit_h;

      /* reserve memory for the rotated image ... */
      bitdata = (unsigned char *)calloc((unsigned)(bit_h*bit_len), 1);
      if (bitdata == NULL) {
	xv_errno = XV_NOMEM;
	return NULL;
      }

      /* create the image ... */
      I2 = XCreateImage(dpy, DefaultVisual(dpy, screen), 1, XYBitmap, 0,
			(char *)bitdata, bit_w, bit_h, 8, 0); 
 
      if (I2 == NULL) {
	xv_errno = XV_NOXIMAGE;
	return NULL;
      }

      I2->byte_order = I2->bitmap_bit_order = MSBFirst;
 
      /* map vertical data to rotated character ... */
      for (j = 0; j < bit_h; j++) {
	for (i = 0; i < bit_w; i++) {
	  /* map bits ... */
	  if (dir == 1)
	    val = vertdata[i*vert_len + (vert_w-j-1)/8] &
	      (128>>((vert_w-j-1)%8));
   
	  else if (dir == 2)
	    val = vertdata[(vert_h-j-1)*vert_len + (vert_w-i-1)/8] &
	      (128>>((vert_w-i-1)%8));
                    
	  else 
	    val = vertdata[(vert_h-i-1)*vert_len + j/8] & 
	      (128>>(j%8));
        
	  if (val) 
	    bitdata[j*bit_len + i/8] = bitdata[j*bit_len + i/8] |
	      (128>>(i%8));
	}
      }
   
      /* create this character's bitmap ... */
      rotfont->per_char[ichar-32].glyph.bm = 
	XCreatePixmap(dpy, root, bit_w, bit_h, 1);
     
      /* put the image into the bitmap ... */
      XPutImage(dpy, rotfont->per_char[ichar-32].glyph.bm, 
		font_gc, I2, 0, 0, 0, 0, bit_w, bit_h);
  
      /* free the image and data ... */
      XDestroyImage(I1);
      XDestroyImage(I2);
      /*      free((char *)bitdata);  -- XDestroyImage does this
	      free((char *)vertdata);*/
    }

    XFreeFont(dpy, fontstruct);
  }

  /* free pixmap and GC ... */
  XFreePixmap(dpy, canvas);
  XFreeGC(dpy, font_gc);

  return rotfont;
}


/* ---------------------------------------------------------------------- */


/* *** Free the resources associated with a rotated font *** */

void XRotUnloadFont(Display *dpy, XRotFontStruct *rotfont)
{
  int ichar;

  if (rotfont->dir == 0) XFreeFont(dpy, rotfont->xfontstruct);

  else
    /* loop through each character, freeing its pixmap ... */
    for (ichar = rotfont->min_char-32; ichar <= rotfont->max_char-32; ichar++)
      XFreePixmap(dpy, rotfont->per_char[ichar].glyph.bm);

  /* rotfont should never be referenced again ... */
  free((char *)rotfont->name);
  free((char *)rotfont);
}


/* ---------------------------------------------------------------------- */
   

/* *** Return the width of a string *** */

int XRotTextWidth(XRotFontStruct *rotfont, char *str, int len)
{
  int i, width = 0, ichar;

  if (str == NULL) return 0;

  if (rotfont->dir == 0)
    width = XTextWidth(rotfont->xfontstruct, str, strlen(str));

  else
    for (i = 0; i<len; i++) {
      ichar = str[i]-32;
  
      /* make sure it's a printing character ... */
      if (ichar >= 0 && ichar<95) 
	width += rotfont->per_char[ichar].width;
    }

  return width;
}


/* ---------------------------------------------------------------------- */


/* *** A front end to XRotPaintString : mimics XDrawString *** */

void XRotDrawString(Display *dpy, XRotFontStruct *rotfont, Drawable drawable,
		    GC gc, int x, int y, char *str, int len)
{            
  static GC my_gc = 0;
  int i, xp, yp, dir, ichar;

  if (str == NULL || len<1) return;

  dir = rotfont->dir;
  if (my_gc == 0) my_gc = XCreateGC(dpy, drawable, 0, 0);

  XCopyGC(dpy, gc, GCForeground|GCBackground, my_gc);

  /* a horizontal string is easy ... */
  if (dir == 0) {
    XSetFillStyle(dpy, my_gc, FillSolid);
    XSetFont(dpy, my_gc, rotfont->xfontstruct->fid);
    XDrawString(dpy, drawable, my_gc, x, y, str, len);
    return;
  }

  /* vertical or upside down ... */

  XSetFillStyle(dpy, my_gc, FillStippled);

  /* loop through each character in string ... */
  for (i = 0; i<len; i++) {
    ichar = str[i]-32;

    /* make sure it's a printing character ... */
    if (ichar >= 0 && ichar<95) {
      /* suitable offset ... */
      if (dir == 1) {
	xp = x-rotfont->per_char[ichar].ascent;
	yp = y-rotfont->per_char[ichar].rbearing; 
      }
      else if (dir == 2) {
	xp = x-rotfont->per_char[ichar].rbearing;
	yp = y-rotfont->per_char[ichar].descent+1; 
      }
      else {
	xp = x-rotfont->per_char[ichar].descent+1;  
	yp = y+rotfont->per_char[ichar].lbearing; 
      }
                   
      /* draw the glyph ... */
      XSetStipple(dpy, my_gc, rotfont->per_char[ichar].glyph.bm);
    
      XSetTSOrigin(dpy, my_gc, xp, yp);
      
      XFillRectangle(dpy, drawable, my_gc, xp, yp,
		     rotfont->per_char[ichar].glyph.bit_w,
		     rotfont->per_char[ichar].glyph.bit_h);
    
      /* advance position ... */
      if (dir == 1)
	y -= rotfont->per_char[ichar].width;
      else if (dir == 2)
	x -= rotfont->per_char[ichar].width;
      else 
	y += rotfont->per_char[ichar].width;
    }
  }
}

  
    
/* ---------------------------------------------------------------------- */


/* *** A front end to XRotPaintAlignedString : uses XRotDrawString *** */

void XRotDrawAlignedString(Display *dpy, XRotFontStruct *rotfont,
			   Drawable drawable, GC gc, int x, int y,
			   char *text, int align)
{  
  int xp = 0, yp = 0, dir;
  int i, nl = 1, max_width = 0, this_width;
  char *str1, *str2 = "\n\0", *str3;

  if (text == NULL) 
    return;
  
  dir = rotfont->dir;

  /* count number of sections in string ... */
  for (i = 0; i<strlen(text); i++) 
    if (text[i] == '\n') 
      nl++;

  /* find width of longest section ... */
  str1 = my_strdup(text);
  str3 = my_strtok(str1, str2);
  max_width = XRotTextWidth(rotfont, str3, strlen(str3));

  do {
    str3 = my_strtok((char *)NULL, str2);
    if (str3 != NULL)
      if (XRotTextWidth(rotfont, str3, strlen(str3))>max_width)
	max_width = XRotTextWidth(rotfont, str3, strlen(str3));
  }
  while (str3 != NULL);
 
  /* calculate vertical starting point according to alignment policy and
     rotation angle ... */
  if (dir == 0) {
    if (align == TLEFT || align == TCENTRE || align == TRIGHT)
      yp = y+rotfont->max_ascent;

    else if (align == BLEFT || align == BCENTRE || align == BRIGHT)
      yp = y-(nl-1)*rotfont->height - rotfont->max_descent;

    else 
      yp = y-(nl-1)/2*rotfont->height + rotfont->max_ascent -
	rotfont->height/2 - ((nl%2 == 0)?rotfont->height/2:0); 
  }

  else if (dir == 1) {
    if (align == TLEFT || align == TCENTRE || align == TRIGHT)
      xp = x+rotfont->max_ascent;

    else if (align == BLEFT || align == BCENTRE || align == BRIGHT)
      xp = x-(nl-1)*rotfont->height - rotfont->max_descent;

    else 
      xp = x-(nl-1)/2*rotfont->height + rotfont->max_ascent -
	rotfont->height/2 - ((nl%2 == 0)?rotfont->height/2:0); 
  }

  else if (dir == 2) {
    if (align == TLEFT || align == TCENTRE || align == TRIGHT)
      yp = y-rotfont->max_ascent;
     
    else if (align == BLEFT || align == BCENTRE || align == BRIGHT)
      yp = y+(nl-1)*rotfont->height + rotfont->max_descent;
     
    else 
      yp = y+(nl-1)/2*rotfont->height - rotfont->max_ascent +
	rotfont->height/2 + ((nl%2 == 0)?rotfont->height/2:0); 
  }

  else {
    if (align == TLEFT || align == TCENTRE || align == TRIGHT)
      xp = x-rotfont->max_ascent;
    
    else if (align == BLEFT || align == BCENTRE || align == BRIGHT)
      xp = x+(nl-1)*rotfont->height + rotfont->max_descent;
  
    else 
      xp = x+(nl-1)/2*rotfont->height - rotfont->max_ascent +
	rotfont->height/2 + ((nl%2 == 0)?rotfont->height/2:0); 
  }

  free(str1);
  str1 = my_strdup(text);
  str3 = my_strtok(str1, str2);
  
  /* loop through each section in the string ... */
  do {
    /* width of this section ... */
    this_width = XRotTextWidth(rotfont, str3, strlen(str3));

    /* horizontal alignment ... */
    if (dir == 0) {
      if (align == TLEFT || align == MLEFT || align == BLEFT)
	xp = x;
  
      else if (align == TCENTRE || align == MCENTRE || align == BCENTRE)
	xp = x-this_width/2;
 
      else 
	xp = x-max_width; 
    }   

    else if (dir == 1) {
      if (align == TLEFT || align == MLEFT || align == BLEFT)
	yp = y;

      else if (align == TCENTRE || align == MCENTRE || align == BCENTRE)
	yp = y+this_width/2;

      else 
	yp = y+max_width; 
    }

    else if (dir == 2) {
      if (align == TLEFT || align == MLEFT || align == BLEFT)
	xp = x;
  
      else if (align == TCENTRE || align == MCENTRE || align == BCENTRE)
	xp = x+this_width/2;
 
      else 
	xp = x+max_width; 
    }

    else {
      if (align == TLEFT || align == MLEFT || align == BLEFT)  
	yp = y;
     
      else if (align == TCENTRE || align == MCENTRE || align == BCENTRE)
	yp = y-this_width/2;
     
      else 
	yp = y-max_width; 
    }

    /* draw the section ... */
    XRotDrawString(dpy, rotfont, drawable, gc, xp, yp,
		   str3, strlen(str3));

    str3 = my_strtok((char *)NULL, str2);

    /* advance position ... */
    if (dir == 0)
      yp += rotfont->height;
    else if (dir == 1)
      xp += rotfont->height;
    else if (dir == 2)
      yp -= rotfont->height;
    else 
      xp -= rotfont->height;
  }
  while (str3 != NULL);

  free(str1);
}

