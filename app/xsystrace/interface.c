/* $OpenBSD: interface.c,v 1.1.1.1 2006/11/26 10:58:43 matthieu Exp $ */
/*
 * Copyright (c) 2002 Matthieu Herrb and Niels Provos
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above
 *	copyright notice, this list of conditions and the following
 *	disclaimer in the documentation and/or other materials provided
 *	with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SimpleMenu.h>
#include <time.h>

#include "callbacks.h"
#include "interface.h"

static String formNames[] = {
	"title-form",
	"processInfo-form",
	"syscallInfo-form",
	"status-form",
	"errorCode-form",
	"specialProc-form",
	"buttons-form",
};
#define NUM_FORMS (sizeof(formNames)/sizeof(char *))


XtAppContext appContext;
Atom wm_delete_window;
Widget forms[NUM_FORMS], errorCodeMenu, errorCodeText,
    filterText, filterMenu, filterPopup,
    wizardButton, timeline, pName,
    pId, policyName, syscallName, status, reviewButton;
volatile Boolean done;

Widget
makeForm(Widget parent)
{
	Widget form;
	Widget errorCodePopup, sme, w;
	time_t when;
	int i;

	form = XtVaCreateWidget("form", formWidgetClass, parent, NULL);
	for (i = 0; i < NUM_FORMS; i++) {
		forms[i] = XtCreateManagedWidget(formNames[i],
		    formWidgetClass, form, NULL, 0);
	}
	XtVaCreateManagedWidget("title-label", labelWidgetClass,
	    forms[0], NULL);
	when = time(NULL);
	timeline = XtVaCreateManagedWidget("date-label", labelWidgetClass,
	    forms[0], NULL);
	XtVaCreateManagedWidget("processInfo-label", labelWidgetClass,
	    forms[1], NULL);
	XtVaCreateManagedWidget("processName-label", labelWidgetClass,
	    forms[1], NULL);
	pName = XtVaCreateManagedWidget("processName-value", labelWidgetClass,
	    forms[1], NULL);
	XtVaCreateManagedWidget("processPid-label", labelWidgetClass,
	    forms[1], NULL);
	pId = XtVaCreateManagedWidget("processPid-value", labelWidgetClass,
	    forms[1], NULL);
	XtVaCreateManagedWidget("policyName-label", labelWidgetClass,
	    forms[1], NULL);
	policyName = XtVaCreateManagedWidget("policyName-value",
	    labelWidgetClass, forms[1], NULL);
	XtVaCreateManagedWidget("syscallInfo-label", labelWidgetClass,
	    forms[2], NULL);
	syscallName = XtVaCreateManagedWidget("syscallInfo-value",
	    labelWidgetClass, forms[2], NULL);
	XtVaCreateManagedWidget("filter-label", labelWidgetClass,
	    forms[2], NULL);
	filterText = XtVaCreateManagedWidget("filter-text",
	    asciiTextWidgetClass, forms[2], XtNeditType, XawtextEdit,
	    NULL, 0);
	filterMenu = XtCreateManagedWidget("filter-menu-button",
	    menuButtonWidgetClass, forms[2], NULL, 0);
	filterPopup = XtCreatePopupShell("filter-menu", simpleMenuWidgetClass,
	    forms[2], NULL, 0);

	wizardButton = XtVaCreateManagedWidget("filter-button",
	    commandWidgetClass, forms[2], NULL, 0);
	XtAddCallback(wizardButton, XtNcallback, on_wizard_clicked, NULL);

	XtVaCreateManagedWidget("status-label", labelWidgetClass,
	    forms[3], NULL);
	status = XtVaCreateManagedWidget("status-value", labelWidgetClass,
	    forms[3], NULL);
	XtVaCreateManagedWidget("errorCode-label", labelWidgetClass,
	    forms[4], NULL);
	errorCodeText = XtVaCreateManagedWidget("errorCode-text",
	    asciiTextWidgetClass, forms[4], XtNeditType, XawtextEdit,
	    NULL, 0);
	errorCodeMenu = XtCreateManagedWidget("errorCode-button",
	    menuButtonWidgetClass, forms[4], NULL, 0);
	errorCodePopup = XtCreatePopupShell("menu", simpleMenuWidgetClass,
	    forms[4], NULL, 0);
	sme = XtCreateManagedWidget("eperm", smeBSBObjectClass,
	    errorCodePopup, NULL, 0);
	XtAddCallback(sme, XtNcallback, on_error_select, (XtPointer)"eperm");
	sme = XtCreateManagedWidget("enoent", smeBSBObjectClass,
	    errorCodePopup, NULL, 0);
	XtAddCallback(sme, XtNcallback, on_error_select, (XtPointer)"enoent");
	sme = XtCreateManagedWidget("einval", smeBSBObjectClass,
	    errorCodePopup, NULL, 0);
	XtAddCallback(sme, XtNcallback, on_error_select, (XtPointer)"einval");
	sme = XtCreateManagedWidget("enotdir", smeBSBObjectClass,
	    errorCodePopup, NULL, 0);
	XtAddCallback(sme, XtNcallback, on_error_select, (XtPointer)"enotdir");
	sme = XtCreateManagedWidget("eio", smeBSBObjectClass,
	    errorCodePopup, NULL, 0);
	XtAddCallback(sme, XtNcallback, on_error_select, (XtPointer)"eio");
	sme = XtCreateManagedWidget("enxio", smeBSBObjectClass,
	    errorCodePopup, NULL, 0);
	XtAddCallback(sme, XtNcallback, on_error_select, (XtPointer)"enxio");
	sme = XtCreateManagedWidget("eaccess", smeBSBObjectClass,
	    errorCodePopup, NULL, 0);
	XtAddCallback(sme, XtNcallback, on_error_select, (XtPointer)"eaccess");
	/* Initial value */
	on_error_select(errorCodeMenu, "eperm", NULL);

	XtVaCreateManagedWidget("specialProc-label", labelWidgetClass,
	    forms[5], NULL);
	w = XtVaCreateManagedWidget("kill-button", commandWidgetClass,
	    forms[5], NULL);
	XtAddCallback(w, XtNcallback, on_killbutton_clicked, NULL);

	reviewButton = XtVaCreateManagedWidget("review-button",
	    commandWidgetClass, forms[5], NULL);
	XtAddCallback(reviewButton, XtNcallback, on_reviewbutton_clicked,
	    (XtPointer)parent);

	w = XtVaCreateManagedWidget("auto-button", commandWidgetClass,
	    forms[5], NULL);
	XtAddCallback(w, XtNcallback, on_detachbutton_clicked, NULL);

	w = XtVaCreateManagedWidget("deny-button", commandWidgetClass,
	    forms[6], NULL);
	XtAddCallback(w, XtNcallback, on_denyone_clicked, NULL);
	w = XtVaCreateManagedWidget("allow-button", commandWidgetClass,
	    forms[6], NULL);
	XtAddCallback(w, XtNcallback, on_permitonce_clicked, NULL);

	w = XtVaCreateManagedWidget("deny-all-button", commandWidgetClass,
	    forms[6], NULL);
	XtAddCallback(w, XtNcallback, on_deny_clicked, NULL);
	w = XtVaCreateManagedWidget("allow-all-button", commandWidgetClass,
	    forms[6], NULL);
	XtAddCallback(w, XtNcallback, on_permit_clicked, NULL);

	XtManageChild(form);

	return form;
}

/*
 * Widget positionning
 */
static void
position_near(Widget shell, int x, int y)
{
	int max_x, max_y;
	Dimension width, height, border;
	int gravity;

	/* some of this is copied from CenterWidgetOnPoint in Xaw/TextPop.c */

	XtVaGetValues(shell, XtNwidth, &width, XtNheight, &height,
	    XtNborderWidth, &border, NULL);

	width += 2 * border;
	height += 2 * border;

	max_x = WidthOfScreen(XtScreen(shell));
	max_y = HeightOfScreen(XtScreen(shell));

	/* set gravity hint based on position on screen */
	gravity = 1;
	if (x > max_x/3) gravity += 1;
	if (x > max_x*2/3) gravity += 1;
	if (y > max_y/3) gravity += 3;
	if (y > max_y*2/3) gravity += 3;

	max_x -= width;
	max_y -= height;

	x -= ( (Position) width/2 );
	if (x < 0) x = 0;
	if (x > max_x) x = max_x;

	y -= ( (Position) height/2 );
	if (y < 0) y = 0;
	if ( y > max_y ) y = max_y;

	XtVaSetValues(shell, XtNx, (Position)x, XtNy, (Position)y,
	    XtNwinGravity, gravity, NULL);
}

void
position_near_mouse(Widget shell)
{
	int x, y;
	Window root, child;
	int winx, winy;
	unsigned int mask;

	XQueryPointer(XtDisplay(shell), XtWindow(shell),
	    &root, &child, &x, &y, &winx, &winy, &mask);
	position_near(shell, x, y);
}

void
position_near_center(Widget shell)
{
	position_near(shell, WidthOfScreen(XtScreen(shell))/2,
	    HeightOfScreen(XtScreen(shell))/2);
}

