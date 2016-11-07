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
#ifndef __ENTRY_H__
#define __ENTRY_H__

/** Defines where the parser is currently at. */
typedef enum {
    /* Reading initial workspace before anything else. */
    CS_INITIAL = 0,
    /* Reading the resource path. */
    CS_COMPONENTS = 1,
    /* Reading whitespace between ':' and the value. */
    CS_PRE_VALUE_WHITESPACE = 2,
    /* Reading the resource's value. */
    CS_VALUE = 3
} xcb_xrm_entry_parser_chunk_status_t;

/** Specifies the type of a component. */
typedef enum {
    /* A "normal" component, i.e., a name/class is given. */
    CT_NORMAL = 0,
    /* A wildcard component ("?"). */
    CT_WILDCARD = 1,
} xcb_xrm_component_type_t;

/** The binding type of a component. */
typedef enum {
    BT_TIGHT = 0,
    BT_LOOSE = 1
} xcb_xrm_binding_type_t;

/** One component of a resource, either in the name or class. */
typedef struct xcb_xrm_component_t {
    /* The type of this component. */
    xcb_xrm_component_type_t type;
    /* The binding type of this component. */
    xcb_xrm_binding_type_t binding_type;
    /* This component's name. Only useful if the type is CT_NORMAL. */
    char *name;

    TAILQ_ENTRY(xcb_xrm_component_t) components;
} xcb_xrm_component_t;

/** Used in xcb_xrm_entry_parse. */
typedef struct xcb_xrm_entry_parser_state_t {
    xcb_xrm_entry_parser_chunk_status_t chunk;
    char *buffer;
    char *buffer_pos;
    xcb_xrm_binding_type_t current_binding_type;
} xcb_xrm_entry_parser_state_t;

/**
 * Parsed structure for a single entry in the xrm database, e.g. representing
 * the parsted state of
 *     Application*class?subclass.resource.
 */
typedef struct xcb_xrm_entry_t {
    /* The value of this entry. */
    char *value;

    /* The individual components making up this entry. */
    TAILQ_HEAD(components_head, xcb_xrm_component_t) components;

    TAILQ_ENTRY(xcb_xrm_entry_t) entries;
} xcb_xrm_entry_t;

/**
 * Parses a specific resource string.
 *
 * @param str The resource string.
 * @param entry A return struct that will contain the parsed resource. The
 * memory will be allocated dynamically, so it must be freed.
 * @param resource_only If true, no wildcards are allowed and only a resource
 * name is parsed.
 *
 * @return 0 on success, a negative error code otherwise.
 *
 */
int xcb_xrm_entry_parse(const char *str, xcb_xrm_entry_t **entry, bool resource_only);

/**
 * Returns the number of components of the given entry.
 *
 */
int __xcb_xrm_entry_num_components(xcb_xrm_entry_t *entry);

/**
 * Compares the two entries.
 * Returns 0 if they are the same and a negative error code otherwise.
 *
 */
int __xcb_xrm_entry_compare(xcb_xrm_entry_t *first, xcb_xrm_entry_t *second);

/**
 * Returns a string representation of this entry.
 *
 */
char *__xcb_xrm_entry_to_string(xcb_xrm_entry_t *entry);

/**
 * Copy the entry.
 *
 */
xcb_xrm_entry_t *__xcb_xrm_entry_copy(xcb_xrm_entry_t *entry);

/**
 * Escapes magic values.
 *
 */
char *__xcb_xrm_entry_escape_value(const char *value);

/**
 * Frees the given entry.
 *
 * @param entry The entry to be freed.
 *
 */
void xcb_xrm_entry_free(xcb_xrm_entry_t *entry);

#endif /* __ENTRY_H__ */
