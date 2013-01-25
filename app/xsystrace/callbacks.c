/* $OpenBSD: callbacks.c,v 1.2 2013/01/25 22:33:54 brad Exp $ */
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
#include <X11/StringDefs.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Shell.h>

#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <err.h>

#include "callbacks.h"
#include "interface.h"
#include "policy.h"

#define MAX_WIZARDS 100

typedef enum {
	WAIT_SYSCALL,
	WAIT_POLICY,
	WAIT_ACK,
	WAIT_WIZARD,
} notificatition_state_t;

static notificatition_state_t state = WAIT_SYSCALL;
static Widget policyText, wizardText;
static char *errorcode;
static String templateList[MAX_WIZARDS];
static int nTemplates = 0;

static void TextAppend(Widget, char *, int);
static void TextReplace(Widget, int, int, XawTextBlock *);
static long TextLength(Widget);
static void make_wizard(Widget);

XtInputId inputId;

static void
debugprintf(char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	if (getenv("NOTIFICATION_DEBUG") != NULL) {
		vfprintf(stderr, format, ap);
	}
	va_end(ap);
}

static char *
freadline(char *line, size_t size, int fd)
{
	char *p = line;
	ssize_t n;

	for (;;) {
		if (size == 0)
			return NULL;

		if ((n = read(fd, p, 1)) <= 0) {
			debugprintf("got null line n=%ld state %d\n", n, state);
			XtRemoveInput(inputId);
			return NULL;
		}
		if (*p == '\n')
			break;

		size--;
		p++;
	}

	*p = '\0';

	debugprintf("state %d got line: %s\n", state, line);
	return (line);
}

void
getInput(XtPointer clientData, int *file, XtInputId *inputId)
{
	char line[_POSIX2_LINE_MAX], *p;
	char *name, *id, *polname, *filters;
	time_t curtime;
	int nfilters;
	Widget sme;
	struct policy_list *pl, *next;
	struct plist *items;

	if (freadline(line, sizeof(line), *file) == NULL) {
		done = True;
		return;
	}

	switch (state) {
	case WAIT_POLICY:
		/* Reading policy for current syscall */
		if (strcmp(line, "WRONG") == 0) {
			state = WAIT_ACK;
			return;
		}
		TextAppend(policyText, line, strlen(line));
		TextAppend(policyText, "\n", 1);
		return;

	case WAIT_WIZARD:
		/* Reading template list */
		if (strcmp(line, "WRONG") == 0) {
			if (nTemplates != 0) {
				make_wizard(wizardButton);
			}
			state = WAIT_ACK;
			return;
		}
		if (!isdigit(line[0])) {
			return;
		}
		templateList[nTemplates++] = XtNewString(line);
		return;

	case WAIT_ACK:
		/* Waiting for user action on current syscall */
		if (strcmp(line, "OKAY") == 0) {
			XtUnmapWidget((Widget)clientData);
			state = WAIT_SYSCALL;
			return;
		}
		if (strcmp(line, "WRONG") == 0) {
			return;
		}
		XtVaSetValues(status, XtNlabel, line, NULL);
		return;

	case WAIT_SYSCALL:
		/* Waiting for next syscall */
		p = line;
		name = strsep(&p, ",");
		if (p == NULL || *p == '\0')
			errx(1, "Bad input line");
		p++;
		strsep(&p, " ");
		if (p == NULL || *p == '\0')
			errx(1, "Bad input line");
		id = strsep(&p, "(");
		if (p == NULL || *p == '\0')
			errx(1, "Bad input line");
		strsep(&p, ":");
		if (p == NULL || *p == '\0')
			errx(1, "Bad input line");
		p++;
		polname = strsep(&p, ",");
		if (p == NULL || *p == '\0')
			errx(1, "Bad input line");
		strsep(&p, ":");
		if (p == NULL || *p == '\0')
			errx(1, "Bad input line");
		p++;
		filters = strsep(&p, ",");
		if (p == NULL || *p == '\0')
			errx(1, "Bad input line");
		nfilters = atoi(filters);
		strsep(&p, ":");
		if (p == NULL || *p == '\0')
			errx(1, "Bad input line");
		p++;

		XtVaSetValues(pName, XtNlabel, name, NULL);
		XtVaSetValues(pId, XtNlabel, id, NULL);
		XtVaSetValues(policyName, XtNlabel, polname, NULL);
		XtVaSetValues(syscallName, XtNlabel, p, NULL);
		XtVaSetValues(status, XtNlabel, "", NULL);
		if (nfilters) {
			XtVaSetValues(wizardButton, XtNsensitive, FALSE,
			    NULL);
			XtVaSetValues(reviewButton, XtNsensitive, TRUE,
			    NULL);
		} else {
			XtVaSetValues(wizardButton, XtNsensitive, TRUE,
			    NULL);
			XtVaSetValues(reviewButton, XtNsensitive, FALSE,
			    NULL);
		}
		XtDestroyWidget(filterPopup);
		filterPopup = XtCreatePopupShell("menu",
		    simpleMenuWidgetClass, forms[2], NULL, 0);

		items = make_policy_suggestion(p);
		pl = SIMPLEQ_FIRST(items);
		on_filter_select(filterText, pl->line,
				 NULL);
		for (pl = SIMPLEQ_FIRST(items); pl != SIMPLEQ_END(items);
		     pl = next) {
			next = SIMPLEQ_NEXT(pl, next);
			sme = XtCreateManagedWidget(pl->line,
			    smeBSBObjectClass, filterPopup, NULL, 0);
			XtAddCallback(sme, XtNcallback, on_filter_select,
			    (XtPointer)XtNewString(pl->line));
			free_policy(pl);
		}
		free(items);

		curtime = time(NULL);
		snprintf(line, sizeof(line), "%.25s", ctime(&curtime));
		XtVaSetValues(timeline, XtNlabel, line, NULL);

		position_near_center((Widget)clientData);
		XtMapWidget((Widget)clientData);
		state = WAIT_ACK;
		return;
	} /* switch */
}

void
on_filter_select(Widget w, XtPointer userData, XtPointer clientData)
{
	char *filter;
	XawTextBlock block;

	filter = (char *)userData;
	debugprintf("filter: %s\n", filter);
	block.ptr = filter;
	block.firstPos = 0;
	block.length = strlen(filter);
	block.format = FMT8BIT;
	TextReplace(filterText, 0, TextLength(filterText), &block);
}

void
on_error_select(Widget w, XtPointer userData, XtPointer clientData)
{
	XawTextBlock block;

	errorcode = (char *)userData;
	block.ptr = errorcode;
	block.firstPos = 0;
	block.length = strlen(errorcode);
	block.format = FMT8BIT;
	TextReplace(errorCodeText, 0, TextLength(errorCodeText), &block);
}

void
on_error_entry_changed(Widget w, XEvent *event, String *params,
    Cardinal *num_params)
{
	Arg args[1];

	XtSetArg(args[0], XtNstring, &errorcode);
	XtGetValues(w, args, 1);
	debugprintf("new error code %s\n", errorcode);
}

void
on_denyone_clicked(Widget w, XtPointer closure, XtPointer clientData)
{
	Arg args[1];

	XtSetArg(args[0], XtNstring, &errorcode);
	XtGetValues(w, args, 1);

	debugprintf("deny-now[%s]\n", errorcode);
	printf("deny-now[%s]\n", errorcode);
}

void
on_permitonce_clicked(Widget w, XtPointer closure, XtPointer clientData)
{
	printf("permit-now\n");
}

void
on_deny_clicked(Widget w, XtPointer closure, XtPointer clientData)
{
	Arg args[1];

	XtSetArg(args[0], XtNstring, &errorcode);
	XtGetValues(w, args, 1);

	debugprintf("deny[%s]\n", errorcode);
	printf("deny[%s]\n", errorcode);
}

void
on_permit_clicked(Widget w, XtPointer closure, XtPointer clientData)
{
	printf("permit\n");
}

void
on_filter_entry_changed(Widget w, XEvent *event, String *params,
    Cardinal *num_params)
{
	Arg args[1];
	char *name;

	XtSetArg(args[0], XtNstring, &name);
	XtGetValues(filterText, args, 1);

	printf("%s\n", name);
}

void
on_detachbutton_clicked(Widget w, XtPointer closure, XtPointer clientData)
{
	printf("detach\n");
}

static long
TextLength(Widget w)
{
	return XawTextSourceScan (XawTextGetSource (w),
				  (XawTextPosition) 0,
				  XawstAll, XawsdRight, 1, TRUE);
}

static void
TextReplace(Widget w, int start, int end, XawTextBlock *block)
{
	Arg arg;
	Widget source;
	XawTextEditType edit_mode;

	source = XawTextGetSource (w);
	XtSetArg (arg, XtNeditType, &edit_mode);
	XtGetValues (source, &arg, ONE);
	XtSetArg (arg, XtNeditType, XawtextEdit);
	XtSetValues (source, &arg, ONE);
	XawTextReplace (w, start, end, block);
	XtSetArg (arg, XtNeditType, edit_mode);
	XtSetValues (source, &arg, ONE);
}

static void
TextAppend(Widget w, char *s, int len)
{
	long last, current;
	XawTextBlock block;

	current = XawTextGetInsertionPoint (w);
	last = TextLength (w);
	block.ptr = s;
	block.firstPos = 0;
	block.length = len;
	block.format = FMT8BIT;
	TextReplace (w, last, last, &block);
	if (current == last)
		XawTextSetInsertionPoint (w, last + block.length);
}


void
on_reviewbutton_clicked(Widget w, XtPointer closure, XtPointer clientData)
{
	Widget shell, form, b;

	printf("review\n");

	XtVaSetValues(reviewButton, XtNsensitive, FALSE, NULL);
	shell = XtVaCreatePopupShell("Review", transientShellWidgetClass,
	    w, NULL, 0);
	form = XtCreateManagedWidget("review-form", formWidgetClass, shell,
	    NULL, 0);
	XtCreateManagedWidget("review-label", labelWidgetClass, form,
	    NULL, 0);
	policyText = XtCreateManagedWidget("review-text", asciiTextWidgetClass,
	    form, NULL, 0);
	b = XtCreateManagedWidget("done-button", commandWidgetClass, form,
	    NULL, 0);
	XtAddCallback(b, XtNcallback, on_done_button, (XtPointer)shell);
	XtRealizeWidget(shell);
	XSetWMProtocols(XtDisplay(shell), XtWindow(shell),
	    &wm_delete_window, 1);
	XtPopup(shell, XtGrabNone);
	state = WAIT_POLICY;
}

void
on_done_button(Widget w, XtPointer closure, XtPointer clientData)
{
	Widget shell = (Widget)closure;

	XtVaSetValues(reviewButton, XtNsensitive, TRUE, NULL);
	XtPopdown(shell);
	XtDestroyWidget(shell);
}

static void
make_wizard(Widget w)
{
	Widget shell, top, form, ok, cancel;

	XtVaSetValues(wizardButton, XtNsensitive, FALSE, NULL);

	shell = XtVaCreatePopupShell("Systrace - Wizard",
	    transientShellWidgetClass, w, NULL, 0);
	top  = XtCreateManagedWidget("wizard-top", formWidgetClass,
	    shell, NULL, 0);
	form = XtCreateManagedWidget("wizard-help-form", formWidgetClass,
	    top, NULL, 0);
	XtCreateManagedWidget("wizard-help-label", labelWidgetClass, form,
	    NULL, 0);
	XtCreateManagedWidget("wizard-help-text", labelWidgetClass, form,
	    NULL, 0);
	form = XtCreateManagedWidget("wizard-form", formWidgetClass, top,
	    NULL, 0);
	XtCreateManagedWidget("wizard-label", labelWidgetClass, form,
	    NULL, 0);
	wizardText = XtCreateManagedWidget("wizard-text", listWidgetClass,
	    form, NULL, 0);
	XawListChange(wizardText, templateList,
	    nTemplates, 0, True);
	XawListHighlight(wizardText, nTemplates-1);
	ok = XtCreateManagedWidget("wizard-ok-button", commandWidgetClass,
	    form, NULL, 0);
	cancel = XtCreateManagedWidget("wizard-cancel-button",
	    commandWidgetClass, form, NULL, 0);
	XtAddCallback(ok, XtNcallback, on_wizard_ok_clicked, (XtPointer)shell);
	XtAddCallback(cancel, XtNcallback, on_wizard_cancel_clicked,
	    (XtPointer)shell);
	XtRealizeWidget(shell);
	XSetWMProtocols(XtDisplay(shell), XtWindow(shell),
	    &wm_delete_window, 1);
	XtPopup(shell, XtGrabNone);
}

void
on_wizard_clicked(Widget w, XtPointer closure, XtPointer clientData)
{

	nTemplates = 0;
	printf("templates\n");
	state = WAIT_WIZARD;
}

static void
wizard_cleanup(Widget w)
{
	int i;

	for (i = 0; i < nTemplates; i++) {
		XtFree(templateList[i]);
	}
	nTemplates = 0;

	XtVaSetValues(wizardButton, XtNsensitive, TRUE, NULL);

	XtPopdown(w);
	XtDestroyWidget(w);
}

void
on_wizard_ok_clicked(Widget w, XtPointer closure, XtPointer clientData)
{
	Widget shell = (Widget)closure;
	XawListReturnStruct *lr;

	lr = XawListShowCurrent(wizardText);
	if (lr != NULL && lr->list_index != XAW_LIST_NONE) {
		printf("template %d\n", lr->list_index + 1);
	}
	wizard_cleanup(shell);
}

void
on_wizard_cancel_clicked(Widget w, XtPointer closure, XtPointer clientData)
{
	Widget shell = (Widget)closure;

	wizard_cleanup(shell);
}

void
on_killbutton_clicked(Widget w, XtPointer closure, XtPointer clientData)
{
	printf("kill\n");
}
