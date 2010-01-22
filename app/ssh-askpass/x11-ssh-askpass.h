/* x11-ssh-askpass.h:  A generic X11-based password dialog for OpenSSH.
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

#ifndef H_X11_SSH_ASKPASS
#define H_X11_SSH_ASKPASS

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>

#define EXIT_STATUS_ACCEPT	0
#define EXIT_STATUS_CANCEL	1
#define EXIT_STATUS_NO_MEMORY	2
#define EXIT_STATUS_ERROR	3
#define EXIT_STATUS_TIMEOUT	4
#define EXIT_STATUS_ANOMALY	127

typedef struct
{
   Pixel foreground;
   Pixel background;
   Dimension width;
   Dimension height;
   Position x;
   Position y;
} WidgetInfo;

typedef struct
{
   WidgetInfo w;
   Pixel topShadowColor;
   Pixel bottomShadowColor;
   Dimension shadowThickness;
   Pixel borderColor;
   Dimension borderWidth;
   Dimension interiorWidth;
   Dimension interiorHeight;
   Dimension horizontalSpacing;
   Dimension verticalSpacing;
} Widget3dInfo;

typedef struct TextObjectStruct
{
   char *text;
   int textLength;
   int direction;
   int ascent;
   int descent;
   XCharStruct overall;
   struct TextObjectStruct *next;
} TextObject;

typedef struct
{
   char *fullText;
   XFontStruct *font;
   XFontStruct *fixedFont;
   TextObject *multiText;
   WidgetInfo w;
} LabelInfo;

typedef struct
{
   Widget3dInfo w3;
   LabelInfo label;
   Bool pressed;
} ButtonInfo;

typedef struct
{
   Widget3dInfo w3;
   int count;
   int current;
   int minimumCount;
   int maximumCount;
} MasterIndicatorInfo;

typedef struct
{
   MasterIndicatorInfo *parent;
   WidgetInfo w;
   Bool isLit;
} IndicatorElement;

typedef struct
{
   Window dialogWindow;
   
   XSizeHints *sizeHints;
   XWMHints *wmHints;
   XClassHint *classHints;
   XTextProperty windowName;
   
   char *title;
   Widget3dInfo w3;
   
   LabelInfo label;

   MasterIndicatorInfo indicator;
   IndicatorElement *indicators;
   
   ButtonInfo okButton;
   ButtonInfo cancelButton;
   
   int pressedButton;
} DialogInfo;

#define NO_BUTTON	0
#define OK_BUTTON	1
#define CANCEL_BUTTON	2

typedef struct 
{
   char *appName;
   char *appClass;
   
   int argc;
   char **argv;
   
   pid_t pid;
   
   char *buf;
   int bufSize;
   int bufIndex;

   Display *dpy;
   Screen *screen;
   long screen_width;
   long screen_height;
   long screen_xoffset;
   long screen_yoffset;
   Window rootWindow;
   Pixel black;
   Pixel white;
   Colormap colormap;

   /* Resolution measurements are normalized to dots/meter. */
   long xResolution;
   long yResolution;
   long defaultXResolution;
   long defaultYResolution;
   long xFuzz;
   long yFuzz;
   
   XtAppContext appContext;
   Widget toplevelShell;
   XrmDatabase resourceDb;
   
   Atom wmDeleteWindowAtom;
   
   GC fillGC;
   GC borderGC;
   GC textGC;
   GC brightGC;
   GC dimGC;
   
   long eventMask;
   
   Bool grabKeyboard;
   Bool grabPointer;
   Bool grabServer;
   Bool isKeyboardGrabbed;
   Bool isPointerGrabbed;
   Bool isServerGrabbed;
   unsigned int grabFailTimeout;
   unsigned int grabRetryInterval;
   
   unsigned long inputTimeout;
   XtIntervalId inputTimeoutTimerId;
   Bool inputTimeoutActive;
   
   DialogInfo *dialog;
} AppInfo;

void outOfMemory(AppInfo *app, int line);
void freeIf(void *p);
void freeFontIf(AppInfo *app, XFontStruct *f);

XFontStruct *getFontResource(AppInfo *app, char *instanceName, char *className);
char *getStringResourceWithDefault(char *instanceName, char *className,
				   char *defaultText);
unsigned int getUnsignedIntegerResource(AppInfo *app, char *instanceName,
					char *className,
					unsigned int defaultValue);
long getResolutionResource(AppInfo *app, char *instanceName, char *className,
			   char *defaultResolutionSpec);

void calcLabelTextExtents(LabelInfo *label);
void calcTotalButtonExtents(ButtonInfo *button);
void calcButtonExtents(ButtonInfo *button);
void balanceButtonExtents(ButtonInfo *button1, ButtonInfo *button2);
void calcButtonLabelPosition(ButtonInfo *button);

Dimension scaleXDimension(AppInfo *app, Dimension unscaled);
Dimension scaleYDimension(AppInfo *app, Dimension unscaled);

void createDialog(AppInfo *app);
void destroyDialog(AppInfo *app);
void createDialogWindow(AppInfo *app);
void createGCs(AppInfo *app);
void destroyGCs(AppInfo *app);

void paintLabel(AppInfo *app, Drawable draw, LabelInfo label);
void paintButton(AppInfo *app, Drawable draw, ButtonInfo button);
void paintIndicator(AppInfo *app, Drawable draw, IndicatorElement indicator);
void updateIndicatorElement(AppInfo *app, int i);
void updateIndicators(AppInfo *app, int condition);
void paintDialog(AppInfo *app);

#define GRAB_KEYBOARD	0
#define GRAB_POINTER	1
void performGrab(AppInfo *app, int grabType, char *grabTypeName,
		 Bool shouldGrab, Bool *isGrabbed);

void grabKeyboard(AppInfo *app);
void ungrabKeyboard(AppInfo *app);
void grabPointer(AppInfo *app);
void ungrabPointer(AppInfo *app);
void grabServer(AppInfo *app);
void ungrabServer(AppInfo *app);

void cleanUp(AppInfo *app);
void exitApp(AppInfo *app, int exitCode);

void acceptAction(AppInfo *app);
void cancelAction(AppInfo *app);

void backspacePassphrase(AppInfo *app);
void erasePassphrase(AppInfo *app);
void addToPassphrase(AppInfo *app, char c);

void handleKeyPress(AppInfo *app, XEvent *event);
Bool eventIsInsideButton(AppInfo *app, XEvent *event, ButtonInfo button);
void handleButtonPress(AppInfo *app, XEvent *event);
void handlePointerMotion(AppInfo *app, XEvent *event);

void handleInputTimeout(XtPointer data, XtIntervalId *timerId);
void cancelInputTimeout(AppInfo *app);

#endif /* H_X11_SSH_ASKPASS */
