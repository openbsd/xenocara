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
#include "externals.h"

#include "match.h"
#include "util.h"

/* Forward declarations */
static int __match_matches(int num_components, xcb_xrm_component_t *cur_comp_db,
        xcb_xrm_component_t *cur_comp_name, xcb_xrm_component_t *cur_comp_class,
        bool has_class, int position, xcb_xrm_match_ignore_t ignore, xcb_xrm_match_t **match);
static xcb_xrm_match_flags_t __match_matches_component(xcb_xrm_component_t *comp_db,
        xcb_xrm_component_t *comp_name, xcb_xrm_component_t *comp_class);
static int __match_compare(int length, xcb_xrm_match_t *best, xcb_xrm_match_t *candidate);
static xcb_xrm_match_t *__match_new(int length);
static void __match_copy(xcb_xrm_match_t *src, xcb_xrm_match_t *dest, int length);
static void __match_free(xcb_xrm_match_t *match);

/*
 * Finds the matching entry in the database given a full name / class query string.
 *
 */
int __xcb_xrm_match(xcb_xrm_database_t *database, xcb_xrm_entry_t *query_name, xcb_xrm_entry_t *query_class,
        xcb_xrm_resource_t *resource) {
    xcb_xrm_match_t *best_match = NULL;
    xcb_xrm_entry_t *cur_entry = TAILQ_FIRST(database);

    int num = __xcb_xrm_entry_num_components(query_name);

    while (cur_entry != NULL) {
        xcb_xrm_match_t *cur_match = NULL;

        /* First we check whether the current database entry even matches. */
        bool has_class = query_class != NULL;
        xcb_xrm_component_t *first_comp_name = TAILQ_FIRST(&(query_name->components));
        xcb_xrm_component_t *first_comp_class = has_class ? TAILQ_FIRST(&(query_class->components)) : NULL;
        xcb_xrm_component_t *first_comp_db = TAILQ_FIRST(&(cur_entry->components));
        if (__match_matches(num, first_comp_db, first_comp_name, first_comp_class, has_class,
                    0, MI_UNDECIDED, &cur_match) == 0) {
            cur_match->entry = cur_entry;

            /* The first matching entry is the first one we pick as the best matching entry. */
            if (best_match == NULL) {
                best_match = cur_match;
            } else {
                /* Otherwise, check whether this match is better than the current best. */
                if (__match_compare(num, best_match, cur_match) == 0) {
                    __match_free(best_match);
                    best_match = cur_match;
                } else {
                    __match_free(cur_match);
                }
            }
        } else {
            __match_free(cur_match);
        }

        /* Advance to the next database entry. */
        cur_entry = TAILQ_NEXT(cur_entry, entries);
    }

    if (best_match != NULL) {
        resource->value = strdup(best_match->entry->value);
        if (resource->value == NULL) {
            __match_free(best_match);
            return -FAILURE;
        }

        __match_free(best_match);
        return SUCCESS;
    }

    return -FAILURE;
}

static int __match_matches(int num_components, xcb_xrm_component_t *cur_comp_db,
        xcb_xrm_component_t *cur_comp_name, xcb_xrm_component_t *cur_comp_class,
        bool has_class, int position, xcb_xrm_match_ignore_t ignore, xcb_xrm_match_t **match) {
    xcb_xrm_match_flags_t comp_match;

    if (*match == NULL) {
        *match = __match_new(num_components);
        if (*match == NULL)
            return -FAILURE;
    }

    /* Check if we reached the end of the recursion. */
    if (cur_comp_name == NULL || (has_class && cur_comp_class == NULL) || cur_comp_db == NULL) {
        if (cur_comp_db == NULL && cur_comp_name == NULL && (!has_class || cur_comp_class == NULL)) {
            return SUCCESS;
        }

        return -FAILURE;
    }

    comp_match = __match_matches_component(cur_comp_db, cur_comp_name, cur_comp_class);

    /* If we have a matching component in a loose binding, we need to continue
     * matching both normally and ignoring this match. */
    if (ignore == MI_UNDECIDED && cur_comp_db->binding_type == BT_LOOSE &&
            (comp_match & MF_NAME || comp_match & MF_CLASS || comp_match & MF_WILDCARD)) {

        /* Store references / copies to the current parameters for the second call. */
        xcb_xrm_component_t *copy_comp_db = cur_comp_db;
        xcb_xrm_component_t *copy_comp_name = cur_comp_name;
        xcb_xrm_component_t *copy_comp_class = cur_comp_class;
        xcb_xrm_match_t *copy_match = __match_new(num_components);
        __match_copy(*match, copy_match, num_components);

        /* First, we try to match normally. */
        if (__match_matches(num_components, cur_comp_db, cur_comp_name, cur_comp_class, has_class,
                    position, MI_DO_NOT_IGNORE, match) == 0) {
            __match_free(copy_match);
            return SUCCESS;
        }

        /* We had no success the first time around, so let's try to reset and
         * go again, but this time ignoring this match. */
        __match_free(*match);
        *match = copy_match;
        if (__match_matches(num_components, copy_comp_db, copy_comp_name, copy_comp_class, has_class,
                    position, MI_IGNORE, match) == 0) {
            return SUCCESS;
        }

        /* Give up. */
        return -FAILURE;
    }

    /* Store the match flags on the match so we can use them later for precedence evaluation. */
    (*match)->flags[position] = comp_match;
    if (comp_match == MF_NONE)
        return -FAILURE;

#define ADVANCE(entry) do {                      \
    if (entry != NULL)                           \
        entry = TAILQ_NEXT((entry), components); \
} while (0)

    /* We have matched this component, so advance to the next one. */
    ADVANCE(cur_comp_name);
    ADVANCE(cur_comp_class);
    /* We advance the pointer to the database entry component if both of the following are true:
     *  1. This match isn't ignored due to a loose binding path.
     *  2. It was an actual match and not skipped due to a loose binding. */
    if (ignore != MI_IGNORE &&
            (comp_match & MF_NAME || comp_match & MF_CLASS || comp_match & MF_WILDCARD)) {
        ADVANCE(cur_comp_db);
    }

#undef ADVANCE

    /* Recursively descend to the next component. */
    return __match_matches(num_components, cur_comp_db, cur_comp_name, cur_comp_class, has_class,
            position + 1, MI_UNDECIDED, match);
}

static xcb_xrm_match_flags_t __match_matches_component(xcb_xrm_component_t *comp_db,
        xcb_xrm_component_t *comp_name, xcb_xrm_component_t *comp_class) {
    xcb_xrm_match_flags_t result = MF_NONE;
    if (comp_db->binding_type == BT_LOOSE)
        result = MF_PRECEDING_LOOSE;

    if (comp_db->type == CT_NORMAL) {
        if (strcmp(comp_db->name, comp_name->name) == 0) {
            result |= MF_NAME;
        } else if (comp_class != NULL && strcmp(comp_db->name, comp_class->name) == 0) {
            result |= MF_CLASS;
        } else {
            if (comp_db->binding_type == BT_TIGHT)
                return MF_NONE;

            /* We remove this flag again because we need to apply
             * it to the last component in the matching chain for
             * the loose binding. */
            result &= ~MF_PRECEDING_LOOSE;
            result |= MF_SKIPPED;
        }
    } else {
        result |= MF_WILDCARD;
    }

    return result;
}

static int __match_compare(int length, xcb_xrm_match_t *best, xcb_xrm_match_t *candidate) {
    for (int i = 0; i < length; i++) {
        xcb_xrm_match_flags_t mt_best = best->flags[i];
        xcb_xrm_match_flags_t mt_candidate = candidate->flags[i];

        /* Precedence rule #1: Matching components, including '?', outweigh '*'. */
        if (mt_best & MF_SKIPPED &&
                (mt_candidate & MF_NAME || mt_candidate & MF_CLASS || mt_candidate & MF_WILDCARD))
            return SUCCESS;

        /* Precedence rule #2: Matching name outweighs both matching class and '?'.
         *                     Matching class outweighs '?'. */
        if ((mt_best & MF_CLASS || mt_best & MF_WILDCARD) && mt_candidate & MF_NAME)
            return SUCCESS;

        if (mt_best & MF_WILDCARD && mt_candidate & MF_CLASS)
            return SUCCESS;

        /* Precedence rule #3: A preceding exact match outweighs a preceding '*'. */
        if (mt_best & MF_PRECEDING_LOOSE && !(mt_candidate & MF_PRECEDING_LOOSE))
            return SUCCESS;
    }

    return -FAILURE;
}

static xcb_xrm_match_t *__match_new(int length) {
    xcb_xrm_match_t *match = calloc(1, sizeof(struct xcb_xrm_match_t));
    if (match == NULL)
        return NULL;

    match->entry = NULL;
    match->flags = calloc(1, length * sizeof(xcb_xrm_match_flags_t));
    if (match->flags == NULL) {
        FREE(match);
        return NULL;
    }

    return match;
}

static void __match_copy(xcb_xrm_match_t *src, xcb_xrm_match_t *dest, int length) {
    memcpy(dest->flags, src->flags, length * sizeof(xcb_xrm_match_flags_t));
}

static void __match_free(xcb_xrm_match_t *match) {
    FREE(match->flags);
    FREE(match);
}
