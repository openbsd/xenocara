/* x11-ssh-askpass.c:  A generic X11-based password dialog for OpenSSH.
 * created 1999-Nov-17 03:40 Jim Knoble <jmknoble@jmknoble.cx>
 * autodate: 2001-Feb-14 04:00
 * 
 * by Jim Knoble <jmknoble@jmknoble.cx>
 * Copyright (C) 1999,2000,2001 Jim Knoble
 * 
 * Disclaimer:
 * 
 * The software is provided "as is", without warranty of any kind,
 * express or implied, including but not limited to the warranties of
 * merchantability, fitness for a particular purpose and
 * noninfringement. In no event shall the author(s) be liable for any
 * claim, damages or other liability, whether in an action of
 * contract, tort or otherwise, arising from, out of or in connection
 * with the software or the use or other dealings in the software.
 * 
 * Portions of this code are distantly derived from code in xscreensaver
 * by Jamie Zawinski <jwz@jwz.org>.  That code says:
 * 
 * --------8<------------------------------------------------8<--------
 * xscreensaver, Copyright (c) 1991-1999 Jamie Zawinski <jwz@jwz.org>
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 * --------8<------------------------------------------------8<--------
 * 
 * The remainder of this code falls under the same permissions and
 * provisions as those of the xscreensaver code.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* For (get|set)rlimit() ... */
#include <sys/time.h>
#include <sys/resource.h>
/* ... end */
/* For (get|set)rlimit(), sleep(), and getpid() ... */
#include <unistd.h>
/* ... end */

/* For errno ... */
#include <errno.h>
/* ... end */

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xos.h>
#include <X11/extensions/Xinerama.h>
#include "dynlist.h"
#include "drawing.h"
#include "resources.h"
#include "x11-ssh-askpass.h"

#undef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

char *progname = NULL;
char *progclass = NULL;
XrmDatabase db = 0;

static char *defaults[] = {
#include "SshAskpass_ad.h"
   0
};

void outOfMemory(AppInfo *app, int line)
{
   fprintf(stderr, "%s[%ld]: Aaahhh! I ran out of memory at line %d.\n",
	   app->appName, (long) app->pid, line);
   exit(EXIT_STATUS_NO_MEMORY);
}

void freeIf(void *p)
{
   if (p) {
      free(p);
   }
}

void freeFontIf(AppInfo *app, XFontStruct *f)
{
   if (f) {
      XFreeFont(app->dpy, f);
   }
}

XFontStruct *getFontResource(AppInfo *app, char *instanceName, char *className)
{
   char *fallbackFont = "fixed";
   
   XFontStruct *f = NULL;
   char *s = get_string_resource(instanceName, className);
   f = XLoadQueryFont(app->dpy, (s ? s : fallbackFont));
   if (!f) {
      f = XLoadQueryFont(app->dpy, fallbackFont);
   }
   if (s) {
      free(s);
   }
   return(f);
}

char *getStringResourceWithDefault(char *instanceName, char *className,
				   char *defaultText)
{
   char *s = get_string_resource(instanceName, className);
   if (!s) {
      if (!defaultText) {
	 s = strdup("");
      } else {
	 s = strdup(defaultText);
      }
   }
   return(s);
}

unsigned int getUnsignedIntegerResource(AppInfo *app, char *instanceName,
					char *className,
					unsigned int defaultValue)
{
   int n;
   unsigned int value;
   char c;
   char *s = get_string_resource(instanceName, className);
   char *cp = s;
   
   if (NULL == s) {
      return(defaultValue);
   }
   while ((*cp) && isspace(*cp)) {
      /* Skip whitespace. */
      cp++;
   }
   if (*cp) {
      if (('0' == cp[0]) && cp[1]) {
	 if (('x' == cp[1]) || ('X' == cp[1])) {
	    /* Hex */
	    n = sscanf(cp + 2, "%x %c", &value, &c);
	 } else {
	    /* Octal */
	    n = sscanf(cp + 1, "%o %c", &value, &c);
	 }
	 if (1 == n) {
	    free(s);
	    return(value);
	 }
      } else {
	 /* Unsigned Decimal */
	 n = sscanf(cp, "%u %c", &value, &c);
	 if (1 == n) {
	    free(s);
	    return(value);
	 }
      }
   }
   /* If we get here, no conversion succeeded. */
   fprintf(stderr, "%s[%ld]: invalid value '%s' for %s.\n",
	   app->appName, (long) app->pid, s, instanceName);
   free(s);
   return(defaultValue);
}

/* Default resolution is 75 dots/inch.  1 in = 2.54 cm. */
#define DefaultResolution ((75 * 10000) / 254)
long getResolutionResource(AppInfo *app, char *instanceName, char *className,
			   char *defaultResolutionSpec)
{
   char units[3];
   char *s;
   int n;
   long resolution;
   unsigned int i;
   
   memset(units, 0, sizeof(units));
   s = getStringResourceWithDefault(instanceName, className,
				    defaultResolutionSpec);
   /* NOTE: The width of the %s format must be one less than
    * the length of the units[] array above!
    */
   n = sscanf(s, "%ld / %2s", &resolution, units);
   if (n != 2) {
      fprintf(stderr, "%s[%ld]: invalid value '%s' for %s.\n",
	      app->appName, (long) app->pid, s, instanceName);
      resolution = DefaultResolution;
   } else {
      if (resolution < 0) {
	 /* Resolution specifications should not be negative. */
	 resolution = -(resolution);
      }
      for (i = 0; i < (sizeof(units) - 1); i++) {
	 units[i] = tolower(units[i]);
      }
      if ((0 == strcmp(units, "in")) ||
	  (0 == strcmp(units, "i")) ||
	  (0 == strcmp(units, "\""))) {
	 /* dots/inch */
	 resolution = resolution * 10000 / 254;
      } else if ((0 == strcmp(units, "m")) ||
		 (0 == strcmp(units, "me"))) {
	 /* dots/meter; no conversion necessary */
	 ;
      } else {
	 /* some unit we don't recognize; cringe and stare at the floor */
	 resolution = DefaultResolution;
      }
   }
   free(s);
   return(resolution);
}
#undef DefaultResolution

void calcTextObjectExtents(TextObject *t, XFontStruct *font) {
   if ((!t) || (!(t->text))) {
      return;
   }
   t->textLength = strlen(t->text);
   XTextExtents(font, t->text, t->textLength, &(t->direction),
		&(t->ascent), &(t->descent), &(t->overall));
}

void calcLabelTextExtents(LabelInfo *label)
{
   TextObject *t;
   int first = 1;
   
   if ((!label) || (!(label->fullText)) || (!(label->font))) {
      return;
   }
   t = label->multiText;
   while (NULL != t) {
      if (first) {
         calcTextObjectExtents(t, label->font);
	 first = 0;
      } else
         calcTextObjectExtents(t, label->fixedFont);
      label->w.height += (t->ascent + t->descent);
      if (label->w.width < t->overall.width) {
	 label->w.width = t->overall.width;
      }
      t = t->next;
   }
}

void calcTotalButtonExtents(ButtonInfo *button)
{
   if (!button) {
      return;
   }
   button->w3.w.width = (button->w3.interiorWidth + 
			 (2 * button->w3.shadowThickness));
   button->w3.w.width += (2 * button->w3.borderWidth);
   button->w3.w.height = (button->w3.interiorHeight +
			  (2 * button->w3.shadowThickness));
   button->w3.w.height += (2 * button->w3.borderWidth);
}

void calcButtonExtents(ButtonInfo *button)
{
   if (!button) {
      return;
   }
   calcLabelTextExtents(&(button->label));
   button->w3.interiorWidth = (button->label.w.width +
			       (2 * button->w3.horizontalSpacing));
   button->w3.interiorHeight = (button->label.w.height +
				(2 * button->w3.verticalSpacing));
   calcTotalButtonExtents(button);
}

void balanceButtonExtents(ButtonInfo *button1, ButtonInfo *button2)
{
   if ((!button1) || (!button2)) {
      return;
   }
   button1->w3.interiorWidth = button2->w3.interiorWidth = 
      MAX(button1->w3.interiorWidth, button2->w3.interiorWidth);
   button1->w3.interiorHeight = button2->w3.interiorHeight =
      MAX(button1->w3.interiorHeight, button2->w3.interiorHeight);
   calcTotalButtonExtents(button1);
   calcTotalButtonExtents(button2);
}

void calcButtonLabelPosition(ButtonInfo *button)
{
   if (!button) {
      return;
   }
   button->label.w.x = button->w3.w.x +
      ((button->w3.w.width - button->label.w.width) / 2);
   button->label.w.y = button->w3.w.y +
      ((button->w3.w.height - button->label.w.height) / 2);
}

Dimension scaleXDimension(AppInfo *app, Dimension unscaled)
{
   Dimension scaled;
   
   if (((app->defaultXResolution < app->xResolution) &&
	((app->defaultXResolution + app->xFuzz) < app->xResolution)) ||
       ((app->xResolution < app->defaultXResolution) &&
	((app->xResolution + app->xFuzz) < app->defaultXResolution))) {
      scaled = (unscaled * app->xResolution) / app->defaultXResolution;
   } else {
      scaled = unscaled;
   }
   return(scaled);
}

Dimension scaleYDimension(AppInfo *app, Dimension unscaled)
{
   Dimension scaled;
   
   if (((app->defaultYResolution < app->yResolution) &&
	((app->defaultYResolution + app->yFuzz) < app->yResolution)) ||
       ((app->yResolution < app->defaultYResolution) &&
	((app->yResolution + app->yFuzz) < app->defaultYResolution))) {
      scaled = (unscaled * app->yResolution) / app->defaultYResolution;
   } else {
      scaled = unscaled;
   }
   return(scaled);
}

/* Assumes 's' is non-NULL. */
TextObject *createTextObject(AppInfo *app, char *s)
{
   TextObject *t = malloc(sizeof(*t));
   if (NULL == t) {
      outOfMemory(app, __LINE__);
   }
   memset(t, 0, sizeof(*t));
   if (('\n' == *s) || ('\0' == *s)) {
      t->text = " ";
   } else {
      t->text = s;
   }
   return(t);
}

/* Assumes 'label' object exists and is zeroed. */
void createLabel(AppInfo *app, char *text, LabelInfo *label)
{
   char *substring;
   TextObject *t;
   
   if ((!app) || (!text)) {
      return;
   }
   label->fullText = strdup(text);
   label->multiText = createTextObject(app, label->fullText);
   t = label->multiText;
   substring = strchr(label->fullText, '\n');
   while (NULL != substring) {
      *(substring++) = '\0';
      t->next = createTextObject(app, substring);
      if (t->next) {
	 t = t->next;
      }
      substring = strchr(substring, '\n');
   }
}

void createDialog(AppInfo *app)
{
   DialogInfo *d;
   char *labelText;
   
   if (app->dialog) {
      return;
   }
   d = malloc(sizeof(*d));
   if (NULL == d) {
      outOfMemory(app, __LINE__);
   }
   memset(d, 0, sizeof(*d));

   app->grabKeyboard = 
      get_boolean_resource("grabKeyboard", "GrabKeyboard", True);
   app->grabPointer =
      get_boolean_resource("grabPointer", "GrabPointer", False);
   app->grabServer =
      get_boolean_resource("grabServer", "GrabServer", False);

   /* inputTimeout resource specified in seconds for easy human interface.
    * Convert to milliseconds here.
    */
   app->inputTimeout = (unsigned long) 1000 *
      getUnsignedIntegerResource(app, "inputTimeout", "InputTimeout", 0);
   
   app->defaultXResolution =
      getResolutionResource(app, "defaultXResolution", "DefaultXResolution",
			    "75/in");
   app->defaultYResolution =
      getResolutionResource(app, "defaultYResolution", "DefaultYResolution",
			    "75/in");
   app->xFuzz =
      getResolutionResource(app, "xResolutionFuzz", "XResolutionFuzz", "20/in");
   app->yFuzz =
      getResolutionResource(app, "yResolutionFuzz", "YResolutionFuzz", "20/in");
   
   d->title =
      getStringResourceWithDefault("dialog.title", "Dialog.Title",
				   "OpenSSH Authentication Passphrase Request");
   d->w3.w.foreground =
      get_pixel_resource("foreground", "Foreground",
			 app->dpy, app->colormap, app->black);
   d->w3.w.background =
      get_pixel_resource("background", "Background",
			 app->dpy, app->colormap, app->white);
   d->w3.topShadowColor =
      get_pixel_resource("topShadowColor", "TopShadowColor",
			 app->dpy, app->colormap, app->white);
   d->w3.bottomShadowColor =
      get_pixel_resource("bottomShadowColor", "BottomShadowColor",
			 app->dpy, app->colormap, app->black);
   d->w3.shadowThickness =
      get_integer_resource("shadowThickness", "ShadowThickness", 3);
   d->w3.borderColor =
      get_pixel_resource("borderColor", "BorderColor",
			 app->dpy, app->colormap, app->black);
   d->w3.borderWidth =
      get_integer_resource("borderWidth", "BorderWidth", 1);
   
   d->w3.horizontalSpacing = scaleXDimension(app,
      get_integer_resource("horizontalSpacing", "Spacing", 5));
   d->w3.verticalSpacing = scaleYDimension(app,
      get_integer_resource("verticalSpacing", "Spacing", 6));
   
   if (2 == app->argc) {
      labelText = strdup(app->argv[1]);
   } else {
      labelText =
	 getStringResourceWithDefault("dialog.label", "Dialog.Label",
				      "Please enter your authentication passphrase:");
   }
   createLabel(app, labelText, &(d->label));
   freeIf(labelText);
   d->label.font = getFontResource(app, "dialog.font", "Dialog.Font");
   d->label.fixedFont = getFontResource(app, "dialog.fixedFont", 
       "Dialog.FixedFont");
   calcLabelTextExtents(&(d->label));
   d->label.w.foreground = d->w3.w.foreground;
   d->label.w.background = d->w3.w.background;
   
   d->okButton.w3.w.foreground =
      get_pixel_resource("okButton.foreground", "Button.Foreground",
			 app->dpy, app->colormap, app->black);
   d->okButton.w3.w.background =
      get_pixel_resource("okButton.background", "Button.Background",
			 app->dpy, app->colormap, app->white);
   d->okButton.w3.topShadowColor =
      get_pixel_resource("okButton.topShadowColor", "Button.TopShadowColor",
			 app->dpy, app->colormap, app->white);
   d->okButton.w3.bottomShadowColor =
      get_pixel_resource("okButton.bottomShadowColor",
			 "Button.BottomShadowColor",
			 app->dpy, app->colormap, app->black);
   d->okButton.w3.shadowThickness =
      get_integer_resource("okButton.shadowThickness",
			   "Button.ShadowThickness", 2);
   d->okButton.w3.borderColor =
      get_pixel_resource("okButton.borderColor", "Button.BorderColor",
			 app->dpy, app->colormap, app->black);
   d->okButton.w3.borderWidth =
      get_integer_resource("okButton.borderWidth", "Button.BorderWidth", 1);
   d->okButton.w3.horizontalSpacing = scaleXDimension(app,
      get_integer_resource("okButton.horizontalSpacing", "Button.Spacing", 4));
   d->okButton.w3.verticalSpacing = scaleYDimension(app,
      get_integer_resource("okButton.verticalSpacing", "Button.Spacing", 2));
   labelText =
      getStringResourceWithDefault("okButton.label", "Button.Label", "OK");
   createLabel(app, labelText, &(d->okButton.label));
   freeIf(labelText);
   d->okButton.label.font =
      getFontResource(app, "okButton.font", "Button.Font");
   calcButtonExtents(&(d->okButton));
   d->okButton.label.w.foreground = d->okButton.w3.w.foreground;
   d->okButton.label.w.background = d->okButton.w3.w.background;
   
   d->cancelButton.w3.w.foreground =
      get_pixel_resource("cancelButton.foreground", "Button.Foreground",
			 app->dpy, app->colormap, app->black);
   d->cancelButton.w3.w.background =
      get_pixel_resource("cancelButton.background", "Button.Background",
			 app->dpy, app->colormap, app->white);
   d->cancelButton.w3.topShadowColor =
      get_pixel_resource("cancelButton.topShadowColor",
			 "Button.TopShadowColor",
			 app->dpy, app->colormap, app->white);
   d->cancelButton.w3.bottomShadowColor =
      get_pixel_resource("cancelButton.bottomShadowColor",
			 "Button.BottomShadowColor",
			 app->dpy, app->colormap, app->black);
   d->cancelButton.w3.shadowThickness =
      get_integer_resource("cancelButton.shadowThickness",
			   "Button.ShadowThickness", 2);
   d->cancelButton.w3.borderColor =
      get_pixel_resource("cancelButton.borderColor", "Button.BorderColor",
			 app->dpy, app->colormap, app->black);
   d->cancelButton.w3.borderWidth =
      get_integer_resource("cancelButton.borderWidth", "Button.BorderWidth",
			   1);
   d->cancelButton.w3.horizontalSpacing = scaleXDimension(app,
      get_integer_resource("cancelButton.horizontalSpacing", "Button.Spacing",
			   4));
   d->cancelButton.w3.verticalSpacing = scaleYDimension(app,
      get_integer_resource("cancelButton.verticalSpacing", "Button.Spacing",
			   2));
   labelText =
      getStringResourceWithDefault("cancelButton.label", "Button.Label",
				   "Cancel");
   createLabel(app, labelText, &(d->cancelButton.label));
   freeIf(labelText);
   d->cancelButton.label.font =
      getFontResource(app, "cancelButton.font", "Button.Font");
   calcButtonExtents(&(d->cancelButton));
   d->cancelButton.label.w.foreground = d->cancelButton.w3.w.foreground;
   d->cancelButton.label.w.background = d->cancelButton.w3.w.background;

   balanceButtonExtents(&(d->okButton), &(d->cancelButton));
   
   d->indicator.w3.w.foreground =
      get_pixel_resource("indicator.foreground", "Indicator.Foreground",
			 app->dpy, app->colormap, app->black);
   d->indicator.w3.w.background =
      get_pixel_resource("indicator.background", "Indicator.Background",
			 app->dpy, app->colormap, app->white);
   d->indicator.w3.w.width = scaleXDimension(app,
      get_integer_resource("indicator.width", "Indicator.Width", 15));
   d->indicator.w3.w.height = scaleYDimension(app,
      get_integer_resource("indicator.height", "Indicator.Height", 7));
   d->indicator.w3.topShadowColor =
      get_pixel_resource("indicator.topShadowColor",
			 "Indicator.TopShadowColor",
			 app->dpy, app->colormap, app->white);
   d->indicator.w3.bottomShadowColor =
      get_pixel_resource("indicator.bottomShadowColor",
			 "Indicator.BottomShadowColor",
			 app->dpy, app->colormap, app->black);
   d->indicator.w3.shadowThickness =
      get_integer_resource("indicator.shadowThickness",
			   "Indicator.ShadowThickness", 2);
   d->indicator.w3.borderColor =
      get_pixel_resource("indicator.borderColor", "Indicator.BorderColor",
			 app->dpy, app->colormap, app->black);
   d->indicator.w3.borderWidth =
      get_integer_resource("indicator.borderWidth", "Indicator.BorderWidth",
			   0);
   d->indicator.w3.horizontalSpacing = scaleXDimension(app,
      get_integer_resource("indicator.horizontalSpacing", "Indicator.Spacing",
			   2));
   d->indicator.w3.verticalSpacing =scaleYDimension(app,
      get_integer_resource("indicator.verticalSpacing", "Indicator.Spacing",
			   4));
   d->indicator.minimumCount =
      get_integer_resource("indicator.minimumCount", "Indicator.MinimumCount",
			   8);
   d->indicator.maximumCount =
      get_integer_resource("indicator.maximumCount", "Indicator.MaximumCount",
			   24);
   d->indicator.w3.interiorWidth = d->indicator.w3.w.width;
   d->indicator.w3.interiorHeight = d->indicator.w3.w.height;
   d->indicator.w3.w.width += (2 * d->indicator.w3.shadowThickness);
   d->indicator.w3.w.width += (2 * d->indicator.w3.borderWidth);
   d->indicator.w3.w.height += (2 * d->indicator.w3.shadowThickness);
   d->indicator.w3.w.height += (2 * d->indicator.w3.borderWidth);
   {
      /* Make sure the indicators can all fit on the screen.
       * 80% of the screen width seems fine.
       */
      Dimension maxWidth = (app->screen_width * 8 / 10);
      Dimension extraSpace = ((2 * d->w3.horizontalSpacing) +
			      (2 * d->w3.shadowThickness));
      
      if (d->indicator.maximumCount < 8) {
	 d->indicator.maximumCount = 8;
      }
      if (((d->indicator.maximumCount * d->indicator.w3.w.width) +
	   ((d->indicator.maximumCount - 1) *
	    d->indicator.w3.horizontalSpacing) + extraSpace) > maxWidth) {
	 d->indicator.maximumCount =
	    ((maxWidth - extraSpace - d->indicator.w3.w.width) /
	     (d->indicator.w3.w.width + d->indicator.w3.horizontalSpacing))
	    + 1;
      }
      if (d->indicator.minimumCount <= 6) {
	 d->indicator.minimumCount = 6;
      }
      if (d->indicator.minimumCount > d->indicator.maximumCount) {
	 d->indicator.minimumCount = d->indicator.maximumCount;
      }
   }
   
   {
      /* Calculate the width and horizontal position of things. */
      Dimension labelAreaWidth;
      Dimension buttonAreaWidth;
      Dimension indicatorAreaWidth;
      Dimension extraIndicatorSpace;
      Dimension singleIndicatorSpace;
      Dimension interButtonSpace;
      Dimension w;
      Position leftX;
      int i;
      
      labelAreaWidth = d->label.w.width + (2 * d->w3.horizontalSpacing);
      buttonAreaWidth = ((3 * d->w3.horizontalSpacing) +
			 d->okButton.w3.w.width +
			 d->cancelButton.w3.w.width);
      w = MAX(labelAreaWidth, buttonAreaWidth);
      extraIndicatorSpace = ((2 * d->w3.horizontalSpacing) +
			     d->indicator.w3.w.width);
      singleIndicatorSpace = (d->indicator.w3.w.width +
			      d->indicator.w3.horizontalSpacing);
      d->indicator.count = ((w - extraIndicatorSpace) / singleIndicatorSpace);
      d->indicator.current = 0;
      d->indicator.count++; /* For gatepost indicator in extra space. */
      if (((w - extraIndicatorSpace) % singleIndicatorSpace) >
	  (singleIndicatorSpace / 2)) {
	 d->indicator.count++;
      }
      if (d->indicator.count < d->indicator.minimumCount) {
	 d->indicator.count = d->indicator.minimumCount;
      }
      if (d->indicator.count > d->indicator.maximumCount) {
	 d->indicator.count = d->indicator.maximumCount;
      }
      indicatorAreaWidth = ((singleIndicatorSpace * (d->indicator.count - 1)) +
			    extraIndicatorSpace);
      d->w3.interiorWidth = MAX(w, indicatorAreaWidth);
      d->w3.w.width = d->w3.interiorWidth + (2 * d->w3.shadowThickness);

      leftX = (d->w3.w.width - d->label.w.width) / 2;
      d->label.w.x = leftX;
      
      leftX = ((d->w3.w.width -
	       (d->indicator.count * d->indicator.w3.w.width) -
	       ((d->indicator.count - 1) * d->indicator.w3.horizontalSpacing))
	       / 2);
      {
	 int n = d->indicator.count * sizeof(IndicatorElement);
	 d->indicators = malloc(n);
	 if (NULL == d->indicators) {
	    destroyDialog(app);
	    outOfMemory(app, __LINE__);
	 }
	 memset(d->indicators, 0, n);
      }
      d->indicators[0].parent = &(d->indicator);
      d->indicators[0].w.x = d->indicator.w3.w.x = leftX;
      d->indicators[0].w.width = d->indicator.w3.w.width;
      d->indicators[0].isLit = False;
      for (i = 1; i < d->indicator.count; i++) {
	 d->indicators[i].parent = &(d->indicator);
	 d->indicators[i].w.x = (d->indicators[i - 1].w.x +
				 d->indicator.w3.w.width +
				 d->indicator.w3.horizontalSpacing);
	 d->indicators[i].w.width = d->indicator.w3.w.width;
	 d->indicators[i].isLit = False;
      }
      interButtonSpace = ((d->w3.interiorWidth - d->okButton.w3.w.width -
			   d->cancelButton.w3.w.width) / 3);
      d->okButton.w3.w.x = interButtonSpace + d->w3.shadowThickness;
      d->cancelButton.w3.w.x = (d->okButton.w3.w.x + d->okButton.w3.w.width +
				interButtonSpace);
   }
   {
      /* Calculate the height and vertical position of things. */
      int i;
      
      d->w3.interiorHeight = ((4 * d->w3.verticalSpacing) +
			      (2 * d->indicator.w3.verticalSpacing) +
			      d->label.w.height +
			      d->indicator.w3.w.height +
			      d->okButton.w3.w.height);
      d->w3.w.height = d->w3.interiorHeight + (2 * d->w3.shadowThickness);
      d->label.w.y = d->w3.shadowThickness + d->w3.verticalSpacing;
      d->indicator.w3.w.y = (d->label.w.y + d->label.w.height +
			     d->w3.verticalSpacing +
			     d->indicator.w3.verticalSpacing);
      for (i = 0; i < d->indicator.count; i++) {
	 d->indicators[i].w.y = d->indicator.w3.w.y;
	 d->indicators[i].w.height = d->indicator.w3.w.height;
      }
      d->okButton.w3.w.y = d->cancelButton.w3.w.y =
	 (d->indicator.w3.w.y + d->indicator.w3.w.height +
	  d->w3.verticalSpacing + d->indicator.w3.verticalSpacing);
   }
   calcButtonLabelPosition(&(d->okButton));
   calcButtonLabelPosition(&(d->cancelButton));

   d->w3.w.x = app->screen_xoffset + (app->screen_width - d->w3.w.width) / 2;
   d->w3.w.y = app->screen_yoffset + (app->screen_height - d->w3.w.height) / 3;
   
   app->dialog = d;
}

void destroyLabel(AppInfo *app, LabelInfo *label)
{
   TextObject *thisTextObject;
   TextObject *nextTextObject;
   
   thisTextObject = label->multiText;
   nextTextObject = thisTextObject->next;
   freeIf(thisTextObject);
   while (NULL != nextTextObject) {
      thisTextObject = nextTextObject;
      nextTextObject = thisTextObject->next;
      freeIf(thisTextObject);
   }
   freeIf(label->fullText);
   freeFontIf(app, label->font);
   freeFontIf(app, label->fixedFont);
}

void destroyDialog(AppInfo *app)
{
   DialogInfo *d = app->dialog;
   
   freeIf(d->title);
   freeIf(d->indicators);

   destroyLabel(app, &(d->label));
   destroyLabel(app, &(d->okButton.label));
   destroyLabel(app, &(d->cancelButton.label));
   
   XFree(d->sizeHints);
   XFree(d->wmHints);
   XFree(d->classHints);
   XFree(d->windowName.value);
   
   freeIf(d);
}

void createDialogWindow(AppInfo *app)
{
   XSetWindowAttributes attr;
   unsigned long attrMask = 0;
   DialogInfo *d = app->dialog;
   
   attr.background_pixel = d->w3.w.background;
   attrMask |= CWBackPixel;
   attr.border_pixel = d->w3.borderColor;
   attrMask |= CWBorderPixel;
   attr.cursor = None;
   attrMask |= CWCursor;
   attr.event_mask = app->eventMask;
   attrMask |= CWEventMask;

   d->dialogWindow = XCreateWindow(app->dpy, app->rootWindow,
				   d->w3.w.x, d->w3.w.y,
				   d->w3.w.width, d->w3.w.height,
				   d->w3.borderWidth,
				   DefaultDepthOfScreen(app->screen),
				   InputOutput,
				   DefaultVisualOfScreen(app->screen),
				   attrMask, &attr);
   
   d->sizeHints = XAllocSizeHints();
   if (!(d->sizeHints)) {
      destroyDialog(app);
      outOfMemory(app, __LINE__);
   }
   d->sizeHints->flags = 0;
   d->sizeHints->flags |= PPosition;
   d->sizeHints->flags |= PSize;
   d->sizeHints->min_width = d->w3.w.width;
   d->sizeHints->min_height = d->w3.w.height;
   d->sizeHints->flags |= PMinSize;
   d->sizeHints->max_width = d->w3.w.width;
   d->sizeHints->max_height = d->w3.w.height;
   d->sizeHints->flags |= PMaxSize;
   d->sizeHints->base_width = d->w3.w.width;
   d->sizeHints->base_height = d->w3.w.height;
   d->sizeHints->flags |= PBaseSize;
   
   d->wmHints = XAllocWMHints();
   if (!(d->wmHints)) {
      destroyDialog(app);
      outOfMemory(app, __LINE__);
   }
   d->wmHints->flags = 0;
   d->wmHints->input = True;
   d->wmHints->flags |= InputHint;
   d->wmHints->initial_state = NormalState;
   d->wmHints->flags |= StateHint;

   d->classHints = XAllocClassHint();
   if (!(d->classHints)) {
      destroyDialog(app);
      outOfMemory(app, __LINE__);
   }
   d->classHints->res_name = app->appName;
   d->classHints->res_class = app->appClass;

   if (!XStringListToTextProperty(&(d->title), 1, &(d->windowName))) {
      destroyDialog(app);
      outOfMemory(app, __LINE__);
   }
   XSetWMProperties(app->dpy, d->dialogWindow, &(d->windowName), NULL,
		    app->argv, app->argc, d->sizeHints,
		    d->wmHints, d->classHints);
   XSetTransientForHint(app->dpy, d->dialogWindow, d->dialogWindow);
   
   app->wmDeleteWindowAtom = XInternAtom(app->dpy, "WM_DELETE_WINDOW", False);
   XSetWMProtocols(app->dpy, d->dialogWindow, &(app->wmDeleteWindowAtom), 1);
}

void createGCs(AppInfo *app)
{
   DialogInfo *d = app->dialog;
   
   XGCValues gcv;
   unsigned long gcvMask;
   
   gcvMask = 0;
   gcv.foreground = d->w3.w.background;
   gcvMask |= GCForeground;
   gcv.fill_style = FillSolid;
   gcvMask |= GCFillStyle;
   app->fillGC = XCreateGC(app->dpy, app->rootWindow, gcvMask, &gcv);
   
   gcvMask = 0;
   gcv.foreground = d->w3.borderColor;
   gcvMask |= GCForeground;
   gcv.line_width = d->w3.borderWidth;
   gcvMask |= GCLineWidth;
   gcv.line_style = LineSolid;
   gcvMask |= GCLineStyle;
   gcv.cap_style = CapButt;
   gcvMask |= GCCapStyle;
   gcv.join_style = JoinMiter;
   gcvMask |= GCJoinStyle;
   app->borderGC = XCreateGC(app->dpy, app->rootWindow, gcvMask, &gcv);
   
   gcvMask = 0;
   gcv.foreground = d->label.w.foreground;
   gcvMask |= GCForeground;
   gcv.background = d->label.w.background;
   gcvMask |= GCBackground;
   gcv.font = d->label.font->fid;
   gcvMask |= GCFont;
   app->textGC = XCreateGC(app->dpy, app->rootWindow, gcvMask, &gcv);
   
   gcvMask = 0;
   gcv.foreground = d->indicator.w3.w.foreground;
   gcvMask |= GCForeground;
   gcv.fill_style = FillSolid;
   gcvMask |= GCFillStyle;
   app->brightGC = XCreateGC(app->dpy, app->rootWindow, gcvMask, &gcv);
   
   gcvMask = 0;
   gcv.foreground = d->indicator.w3.w.background;
   gcvMask |= GCForeground;
   gcv.fill_style = FillSolid;
   gcvMask |= GCFillStyle;
   app->dimGC = XCreateGC(app->dpy, app->rootWindow, gcvMask, &gcv);
}

void destroyGCs(AppInfo *app)
{
   XFreeGC(app->dpy, app->fillGC);
   XFreeGC(app->dpy, app->borderGC);
   XFreeGC(app->dpy, app->textGC);
   XFreeGC(app->dpy, app->brightGC);
   XFreeGC(app->dpy, app->dimGC);
}

void paintLabel(AppInfo *app, Drawable draw, LabelInfo label)
{
   TextObject *t;
   Position x;
   Position y;
   int first = 1;

   if (!(label.fullText)) {
      return;
   }
   XSetForeground(app->dpy, app->textGC, label.w.foreground);
   XSetBackground(app->dpy, app->textGC, label.w.background);
   XSetFont(app->dpy, app->textGC, label.font->fid);
   
   t = label.multiText;
   x = label.w.x;
   y = label.w.y + t->ascent;
   while (NULL != t) {
      if (!first) 
	 XSetFont(app->dpy, app->textGC, label.fixedFont->fid);
      else
	 first = 0;
       
      if (t->text) {
	 XDrawString(app->dpy, draw, app->textGC, x, y, t->text,
		     t->textLength);
      }
      y += t->descent;
      t = t->next;
      if (t) {
	 y += t->ascent;
      }
   }
}

void paintButton(AppInfo *app, Drawable draw, ButtonInfo button)
{
   Position x;
   Position y;
   Dimension width;
   Dimension height;
   
   if (button.w3.borderWidth > 0) {
      XSetForeground(app->dpy, app->borderGC, button.w3.borderColor);
      XFillRectangle(app->dpy, draw, app->borderGC, button.w3.w.x,
		     button.w3.w.y, button.w3.w.width, button.w3.w.height);
   }
   if ((button.w3.shadowThickness <= 0) && (button.pressed)) {
      Pixel tmp = button.w3.w.background;
      button.w3.w.background = button.w3.w.foreground;
      button.w3.w.foreground = tmp;
      tmp = button.label.w.background;
      button.label.w.background = button.label.w.foreground;
      button.label.w.foreground = tmp;
   }
   x = (button.w3.w.x + button.w3.borderWidth);
   y = (button.w3.w.y + button.w3.borderWidth);
   width = (button.w3.w.width - (2 * button.w3.borderWidth));
   height = (button.w3.w.height - (2 * button.w3.borderWidth));
   if ((button.w3.shadowThickness > 0) && (button.pressed)) {
      XSetForeground(app->dpy, app->fillGC, button.w3.topShadowColor);
   } else {
      XSetForeground(app->dpy, app->fillGC, button.w3.w.background);
   }
   XFillRectangle(app->dpy, draw, app->fillGC, x, y, width, height);
   if (button.w3.shadowThickness > 0) {
      if (button.pressed) {
	 draw_shaded_rectangle(app->dpy, draw, x, y, width, height,
			       button.w3.shadowThickness,
			       button.w3.bottomShadowColor,
			       button.w3.topShadowColor);
      } else {
	 draw_shaded_rectangle(app->dpy, draw, x, y, width, height,
			       button.w3.shadowThickness,
			       button.w3.topShadowColor,
			       button.w3.bottomShadowColor);
      }
   }
   if ((button.w3.shadowThickness > 0) && (button.pressed)) {
      Dimension pressedAdjustment;
      
      pressedAdjustment = button.w3.shadowThickness / 2;
      if (pressedAdjustment < 1) {
	 pressedAdjustment = 1;
      }
      x = button.label.w.x;
      y = button.label.w.y;
      button.label.w.x += pressedAdjustment;
      button.label.w.y += pressedAdjustment;
      paintLabel(app, draw, button.label);
      button.label.w.x = x;
      button.label.w.y = y;
   } else {
      paintLabel(app, draw, button.label);
   }
   if ((button.w3.shadowThickness <= 0) && (button.pressed)) {
      Pixel tmp = button.w3.w.background;
      button.w3.w.background = button.w3.w.foreground;
      button.w3.w.foreground = tmp;
      tmp = button.label.w.background;
      button.label.w.background = button.label.w.foreground;
      button.label.w.foreground = tmp;
   }
}

void paintIndicator(AppInfo *app, Drawable draw, IndicatorElement indicator)
{
   Position x;
   Position y;
   Dimension width;
   Dimension height;
   GC gc = app->dimGC;
   
   if (indicator.parent->w3.borderWidth > 0) {
      XSetForeground(app->dpy, app->borderGC,
		     indicator.parent->w3.borderColor);
      XFillRectangle(app->dpy, draw, app->borderGC, indicator.w.x,
		     indicator.w.y, indicator.w.width, indicator.w.height);
   }
   if (indicator.isLit) {
      gc = app->brightGC;
   }
   x = (indicator.w.x + indicator.parent->w3.borderWidth);
   y = (indicator.w.y + indicator.parent->w3.borderWidth);
   width = (indicator.w.width - (2 * indicator.parent->w3.borderWidth));
   height = (indicator.w.height - (2 * indicator.parent->w3.borderWidth));
   XFillRectangle(app->dpy, draw, gc, x, y, width, height);
   if (indicator.parent->w3.shadowThickness > 0) {
      draw_shaded_rectangle(app->dpy, draw, x, y, width, height,
			    indicator.parent->w3.shadowThickness,
			    indicator.parent->w3.bottomShadowColor,
			    indicator.parent->w3.topShadowColor);
   }
}

void updateIndicatorElement(AppInfo *app, int i)
{
   DialogInfo *d = app->dialog;
   
   d->indicators[i].isLit = !(d->indicators[i].isLit);
   paintIndicator(app, d->dialogWindow, d->indicators[i]);
}

void updateIndicators(AppInfo *app, int condition)
{
   DialogInfo *d = app->dialog;
   
   if (condition > 0) {
      /* Move forward one. */
      updateIndicatorElement(app, d->indicator.current);
      if (d->indicator.current < (d->indicator.count - 1)) {
	 (d->indicator.current)++;
      } else {
	 d->indicator.current = 0;
      }
   } else if (condition < 0) {
      /* Move backward one. */
      if (d->indicator.current > 0) {
	 (d->indicator.current)--;
      } else {
	 d->indicator.current = d->indicator.count - 1;
      }
      updateIndicatorElement(app, d->indicator.current);
   } else {
      /* Erase them all. */
      int i;
      
      for (i = 0; i < d->indicator.count; i++) {
	 d->indicators[i].isLit = False;
	 paintIndicator(app, d->dialogWindow, d->indicators[i]);
      }
      d->indicator.current = 0;
   }
   XSync(app->dpy, False);
}

void paintDialog(AppInfo *app)
{
   DialogInfo *d = app->dialog;
   Drawable draw = d->dialogWindow;
   int i;
   
   XSetForeground(app->dpy, app->fillGC, d->w3.w.background);
   XFillRectangle(app->dpy, draw, app->fillGC, 0, 0,
		  d->w3.w.width, d->w3.w.height);
   if (d->w3.shadowThickness > 0) {
      draw_shaded_rectangle(app->dpy, draw, 0, 0,
			    d->w3.w.width, d->w3.w.height,
			    d->w3.shadowThickness,
			    d->w3.topShadowColor,
			    d->w3.bottomShadowColor);
   }
   paintLabel(app, draw, d->label);
   for (i = 0; i < d->indicator.count; i++) {
      paintIndicator(app, draw, d->indicators[i]);
   }
   paintButton(app, draw, d->okButton);
   paintButton(app, draw, d->cancelButton);
   XSync(app->dpy, False);
}

void performGrab(AppInfo *app, int grabType, char *grabTypeName,
		 Bool shouldGrab, Bool *isGrabbed) {
   if ((!(shouldGrab)) || (*isGrabbed)) {
      return;
   } else if ((GRAB_KEYBOARD != grabType) && (GRAB_POINTER != grabType)) {
      fprintf(stderr, "%s[%ld]: performGrab: invalid grab type (%d).\n",
	      app->appName, (long) app->pid, grabType);
      return;
   } else {
      int status = GrabInvalidTime;	/* keep gcc -Wall from complaining */
      unsigned int seconds = 0;
      int helpful_message = 0;
      /* keyboard and pointer */
      Window grabWindow = app->dialog->dialogWindow;
      Bool ownerEvents = False;
      Bool pointerMode = GrabModeAsync;
      Bool keyboardMode = GrabModeAsync;
      /* pointer only */
      unsigned int eventMask = ButtonPressMask | ButtonReleaseMask;
      Window confineTo = None;
      Cursor cursor = None;
      
      *isGrabbed = True;
      
      if (NULL == grabTypeName) {
	 fprintf(stderr, "%s[%ld]: performGrab: null grab type name.\n",
		 app->appName, (long) app->pid);
      }

      if (0 == app->grabFailTimeout) {
	 /* Ensure we try to perform the grab at least once. */
	 app->grabFailTimeout = 1;
      }
      while (seconds < app->grabFailTimeout) {
	 XSync(app->dpy, False);
	 switch (grabType) {
	  case GRAB_KEYBOARD:
	    status = XGrabKeyboard(app->dpy, grabWindow, ownerEvents,
				   pointerMode, keyboardMode, CurrentTime);
	    break;
	  case GRAB_POINTER:
	    status = XGrabPointer(app->dpy, grabWindow, ownerEvents,
				  eventMask, pointerMode, keyboardMode,
				  confineTo, cursor, CurrentTime);
	    break;
	 }
	 XSync(app->dpy, False);
	 if (GrabSuccess == status) {
	    if (helpful_message) {
	       fprintf(stderr, "%s[%ld]: Got %s.\n",
		       app->appName, (long) app->pid, grabTypeName);
	    }
	    break;
	 }
	 if (!helpful_message) {
	    fprintf(stderr, "%s[%ld]: Trying to grab %s ...\n",
		    app->appName, (long) app->pid, grabTypeName);
	    helpful_message = 1;
	 }
	 seconds += app->grabRetryInterval;
	 sleep(app->grabRetryInterval);
      }
      if (GrabSuccess != status) {
	 char *reason = "reason unknown";
	 
	 switch (status) {
	  case AlreadyGrabbed:
	    reason = "someone else already has it";
	    break;
	  case GrabFrozen:
	    reason = "someone else has frozen it";
	    break;
	  case GrabInvalidTime:
	    reason = "bad grab time [this shouldn't happen]";
	    break;
	  case GrabNotViewable:
	    reason = "grab not viewable [this shouldn't happen]";
	    break;
	 }
	 fprintf(stderr, "%s[%ld]: Could not grab %s (%s)\n",
		 app->appName, (long) app->pid, grabTypeName, reason);
	 exitApp(app, EXIT_STATUS_ERROR);
      }
   }
}
		 

void grabKeyboard(AppInfo *app)
{
   performGrab(app, GRAB_KEYBOARD, "keyboard", app->grabKeyboard,
	       &(app->isKeyboardGrabbed));
}

void ungrabKeyboard(AppInfo *app)
{
   if (app->grabKeyboard) {
      XUngrabKeyboard(app->dpy, CurrentTime);
   }
}

void grabPointer(AppInfo *app)
{
   performGrab(app, GRAB_POINTER, "pointer", app->grabPointer,
	       &(app->isPointerGrabbed));
}

void ungrabPointer(AppInfo *app)
{
   if (app->grabPointer) {
      XUngrabPointer(app->dpy, CurrentTime);
   }
}

void grabServer(AppInfo *app)
{
   if ((!(app->grabServer)) || (app->isServerGrabbed)) {
      return;
   } else {
      app->isServerGrabbed = True;
      XSync(app->dpy, False);
      XGrabServer(app->dpy);
      XSync(app->dpy, False);
   }
}

void ungrabServer(AppInfo *app)
{
   if (app->grabServer) {
      XUngrabServer(app->dpy);
   }
}

void cleanUp(AppInfo *app)
{
   cancelInputTimeout(app);
   XDestroyWindow(app->dpy, app->dialog->dialogWindow);
   destroyGCs(app);
   destroyDialog(app);
   if (app->buf) {
      memset(app->buf, 0, app->bufSize);
   }
   freeIf(app->buf);
   ungrabPointer(app);
   ungrabKeyboard(app);
   ungrabServer(app);
}

void exitApp(AppInfo *app, int exitCode)
{
   cleanUp(app);
   exit(exitCode);
}

void acceptAction(AppInfo *app)
{
   int status = append_to_buf(&(app->buf), &(app->bufSize),
			      &(app->bufIndex), '\0');
   if (APPEND_FAILURE == status) {
      cleanUp(app);
      outOfMemory(app, __LINE__);
   }
   fputs(app->buf, stdout);
   fputc('\n', stdout);
   exitApp(app, EXIT_STATUS_ACCEPT);
}

void cancelAction(AppInfo *app)
{
   exitApp(app, EXIT_STATUS_CANCEL);
}

void backspacePassphrase(AppInfo *app)
{
   if (0 >= app->bufIndex) {
      XBell(app->dpy, 0);
      return;
   }
   (app->bufIndex)--;
   updateIndicators(app, -1);
}

void erasePassphrase(AppInfo *app)
{
   if (0 >= app->bufIndex) {
      XBell(app->dpy, 0);
      return;
   }
   updateIndicators(app, 0);
   app->bufIndex = 0;
}

void addToPassphrase(AppInfo *app, char c)
{
   int status = append_to_buf(&(app->buf), &(app->bufSize),
			      &(app->bufIndex), c);
   if (APPEND_FAILURE == status) {
      cleanUp(app);
      outOfMemory(app, __LINE__);
   }
   updateIndicators(app, 1);
}

void handleKeyPress(AppInfo *app, XEvent *event)
{
   char s[2];
   int n;
   
   if (event->xkey.send_event) {
      /* Pay no attention to synthetic key events. */
      return;
   }
   cancelInputTimeout(app);
   n = XLookupString(&(event->xkey), s, 1, NULL, NULL);
   
   if (1 != n) {
      return;
   }
   s[1] = '\0';
   switch (s[0]) {
    case '\010':
    case '\177':
      backspacePassphrase(app);
      break;
    case '\025':
    case '\030':
      erasePassphrase(app);
      break;
    case '\012':
    case '\015':
      acceptAction(app);
      break;
    case '\033':
      cancelAction(app);
      break;
    default:
      addToPassphrase(app, s[0]);
      break;
   }
}

Bool eventIsInsideButton(AppInfo *app, XEvent *event, ButtonInfo button)
{
   /* 'gcc -Wall' complains about 'app' being an unused parameter. 
    * Tough.  We might want to use it later, and then we don't have
    * to change it in each place it's called.  Performance won't suffer.
    */
   int status = False;
   int x, y;
   
   switch(event->type) {
    case ButtonPress:
    case ButtonRelease:
      x = event->xbutton.x;
      y = event->xbutton.y;
      break;
    case MotionNotify:
      x = event->xmotion.x;
      y = event->xmotion.y;
      break;
    default:
      return(False);
   }
   if ((x >= (button.w3.w.x + button.w3.borderWidth)) &&
       (x < (button.w3.w.x + button.w3.w.width -
	     (2 * button.w3.borderWidth))) &&
       (y >= (button.w3.w.y + button.w3.borderWidth)) &&
       (y < (button.w3.w.y + button.w3.w.height -
	     (2 * button.w3.borderWidth)))) {
      status = True;
   }
   return(status);
}

void handleButtonPress(AppInfo *app, XEvent *event)
{
   DialogInfo *d = app->dialog;

   cancelInputTimeout(app);
   if (event->xbutton.button != Button1) {
      return;
   }
   if (ButtonPress == event->type) {
      if (eventIsInsideButton(app, event, d->okButton)) {
	 d->pressedButton = OK_BUTTON;
	 d->okButton.pressed = True;
	 paintButton(app, d->dialogWindow, d->okButton);
      } else if (eventIsInsideButton(app, event, d->cancelButton)) {
	 d->pressedButton = CANCEL_BUTTON;
	 d->cancelButton.pressed = True;
	 paintButton(app, d->dialogWindow, d->cancelButton);
      } else {
	 d->pressedButton = NO_BUTTON;
      }
   } else if (ButtonRelease == event->type) {
      if (OK_BUTTON == d->pressedButton) {
	 if (eventIsInsideButton(app, event, d->okButton)) {
	    acceptAction(app);
	 } else {
	    if (d->okButton.pressed) {
	       d->okButton.pressed = False;
	       paintButton(app, d->dialogWindow, d->okButton);
	    }
	 }
      } else if (CANCEL_BUTTON == d->pressedButton) {
	 if (eventIsInsideButton(app, event, d->cancelButton)) {
	    cancelAction(app);
	 } else {
	    if (d->cancelButton.pressed) {
	       d->cancelButton.pressed = False;
	       paintButton(app, d->dialogWindow, d->cancelButton);
	    }
	 }
      }
      d->pressedButton = NO_BUTTON;
   }
}

void handlePointerMotion(AppInfo *app, XEvent *event)
{
   DialogInfo *d = app->dialog;
   
   if (NO_BUTTON == d->pressedButton) {
      return;
   } else if (OK_BUTTON == d->pressedButton) {
      if (eventIsInsideButton(app, event, d->okButton)) {
	 if (!(d->okButton.pressed)) {
	    d->okButton.pressed = True;
	    paintButton(app, d->dialogWindow, d->okButton);
	 }
      } else {
	 if (d->okButton.pressed) {
	    d->okButton.pressed = False;
	    paintButton(app, d->dialogWindow, d->okButton);
	 }
      }
   } else if (CANCEL_BUTTON == d->pressedButton) {
      if (eventIsInsideButton(app, event, d->cancelButton)) {
	 if (!(d->cancelButton.pressed)) {
	    d->cancelButton.pressed = True;
	    paintButton(app, d->dialogWindow, d->cancelButton);
	 }
      } else {
	 if (d->cancelButton.pressed) {
	    d->cancelButton.pressed = False;
	    paintButton(app, d->dialogWindow, d->cancelButton);
	 }
      }
   }
}

void handleInputTimeout(XtPointer data, XtIntervalId *timerId)
{
   /* 'gcc -Wall' complains about 'timerId' being an unused parameter.
    * Tough.  Xt forces us to have it here.  Like it.
    */
   AppInfo *app = (AppInfo *) data;
   if (app->inputTimeoutActive) {
      app->inputTimeoutActive = False;
      fprintf(stderr, "%s[%ld]: *Yawn*...timed out after %lu seconds.\n",
	      app->appName, (long) app->pid, (app->inputTimeout / 1000));
      exitApp(app, EXIT_STATUS_TIMEOUT);
   }
}

void cancelInputTimeout(AppInfo *app)
{
   if (app->inputTimeoutActive) {
      app->inputTimeoutActive = False;
      XtRemoveTimeOut(app->inputTimeoutTimerId);
   }
}

int main(int argc, char **argv)
{
   AppInfo app;
   XEvent event;
   XineramaScreenInfo *screens;
   int nscreens;

   memset(&app, 0, sizeof(app));
   
   progclass = "SshAskpass";
   app.toplevelShell = XtAppInitialize(&(app.appContext), progclass,
					NULL, 0, &argc, argv,
					defaults, NULL, 0);
   app.argc = argc;
   app.argv = argv;
   app.dpy = XtDisplay(app.toplevelShell);
   app.screen = DefaultScreenOfDisplay(app.dpy);
   app.rootWindow = RootWindowOfScreen(app.screen);
   app.black = BlackPixel(app.dpy, DefaultScreen(app.dpy));
   app.white = WhitePixel(app.dpy, DefaultScreen(app.dpy));
   app.colormap = DefaultColormapOfScreen(app.screen);
   app.resourceDb = XtDatabase(app.dpy);
   XtGetApplicationNameAndClass(app.dpy, &progname, &progclass);
   app.appName = progname;
   app.appClass = progclass;
   /* For resources.c. */
   db = app.resourceDb;
   
   /* Seconds after which keyboard/pointer grab fail. */
   app.grabFailTimeout = 5;
   /* Number of seconds to wait between grab attempts. */
   app.grabRetryInterval = 1;
   
   app.pid = getpid();

   {
      struct rlimit resourceLimit;
      int status;
      
      status = getrlimit(RLIMIT_CORE, &resourceLimit);
      if (-1 == status) {
	 fprintf(stderr, "%s[%ld]: getrlimit failed (%s)\n", app.appName,
		 (long) app.pid, strerror(errno));
	 exit(EXIT_STATUS_ERROR);
      }
      resourceLimit.rlim_cur = 0;
      status = setrlimit(RLIMIT_CORE, &resourceLimit);
      if (-1 == status) {
	 fprintf(stderr, "%s[%ld]: setrlimit failed (%s)\n", app.appName,
		 (long) app.pid, strerror(errno));
	 exit(EXIT_STATUS_ERROR);
      }
   }

   app.screen_width = WidthOfScreen(app.screen);
   app.screen_height = HeightOfScreen(app.screen);
   app.screen_xoffset = 0;
   app.screen_yoffset = 0;
   if (XineramaIsActive(app.dpy) &&
      (screens = XineramaQueryScreens(app.dpy, &nscreens)) != NULL &&
      nscreens) {
      app.screen_width = screens[0].width;
      app.screen_height = screens[0].height;
      app.screen_xoffset = screens[0].x_org;
      app.screen_yoffset = screens[0].y_org;
      XFree(screens);
   }

   app.xResolution =
      app.screen_width * 1000 / WidthMMOfScreen(app.screen);
   app.yResolution =
      app.screen_height * 1000 / HeightMMOfScreen(app.screen);

   createDialog(&app);
   createGCs(&app);
   
   app.eventMask = 0;
   app.eventMask |= ExposureMask;
   app.eventMask |= ButtonPressMask;
   app.eventMask |= ButtonReleaseMask;
   app.eventMask |= Button1MotionMask;
   app.eventMask |= KeyPressMask;

   createDialogWindow(&app);
   
   XMapWindow(app.dpy, app.dialog->dialogWindow);
   if (app.inputTimeout > 0) {
      app.inputTimeoutActive = True;
      app.inputTimeoutTimerId =
	 XtAppAddTimeOut(app.appContext, app.inputTimeout,
			 handleInputTimeout, (XtPointer) &app);
   }

   
   while(True) {
      XtAppNextEvent(app.appContext, &event);
      switch (event.type) {
       case Expose:
	 grabServer(&app);
	 grabKeyboard(&app);
	 grabPointer(&app);
	 if (event.xexpose.count) {
	    break;
	 }
	 paintDialog(&app);
	 break;
       case ButtonPress:
       case ButtonRelease:
	 handleButtonPress(&app, &event);
	 break;
       case MotionNotify:
	 handlePointerMotion(&app, &event);
       case KeyPress:
	 handleKeyPress(&app, &event);
	 break;
       case ClientMessage:
	 if ((32 == event.xclient.format) &&
	     ((unsigned long) event.xclient.data.l[0] ==
	      app.wmDeleteWindowAtom)) {
	    cancelAction(&app);
	 }
	 break;
       default:
	 break;
      }
   }

   fprintf(stderr, "%s[%ld]: This should not happen.\n", app.appName,
	   (long) app.pid);
   return(EXIT_STATUS_ANOMALY);
}

