/*
 * vim:ts=4:sw=4:expandtab
 *
 * Copyright © 2016 Ingo Bürk
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the names of the authors or their
 * institutions shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization from the authors.
 *
 */
#ifndef __MATCH_H__
#define __MATCH_H__

#include "database.h"
#include "resource.h"
#include "entry.h"

/** Information about a matched component. */
typedef enum xcb_xrm_match_flags_t {
    MF_NONE = 1 << 0,

    /* The component was matched on the name. */
    MF_NAME = 1 << 1,
    /* The component was matched on the class. */
    MF_CLASS = 1 << 2,
    /* The component was matched via a '?' wildcard. */
    MF_WILDCARD = 1 << 3,
    /* The component was matched as part of a loose binding. */
    MF_SKIPPED = 1 << 4,

    /* This component was preceded by a loose binding. */
    MF_PRECEDING_LOOSE = 1 << 5,
} xcb_xrm_match_flags_t;

/**
 * Helper enum to decide whether a component in a loose binding shall be
 * skipped even if it matches.
 */
typedef enum xcb_xrm_match_ignore_t {
    MI_UNDECIDED,
    MI_IGNORE,
    MI_DO_NOT_IGNORE,
} xcb_xrm_match_ignore_t;

typedef struct xcb_xrm_match_t {
    /* Reference to the database entry this match refers to. */
	xcb_xrm_entry_t *entry;
    /* An array where the n-th element describes how the n-th element of the
     * query strings was matched. */
	xcb_xrm_match_flags_t *flags;
} xcb_xrm_match_t;

/**
 * Finds the matching entry in the database given a full name / class query string.
 *
 */
int __xcb_xrm_match(xcb_xrm_database_t *database, xcb_xrm_entry_t *query_name, xcb_xrm_entry_t *query_class,
        xcb_xrm_resource_t *resource);

#endif /* __MATCH_H__ */
