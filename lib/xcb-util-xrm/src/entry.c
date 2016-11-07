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

#include "entry.h"
#include "util.h"

#define BUFFER_SIZE 1024

/**
 * Appends a single character to the current buffer.
 * If the buffer is not yet initialized or has been invalidated, it will be set up.
 *
 */
static void xcb_xrm_append_char(xcb_xrm_entry_t *entry, xcb_xrm_entry_parser_state_t *state,
        const char str) {
    ptrdiff_t offset;

    if (state->buffer_pos == NULL) {
        FREE(state->buffer);
        state->buffer = calloc(1, BUFFER_SIZE);
        state->buffer_pos = state->buffer;
        if (state->buffer == NULL) {
            /* Let's ignore this character and try again next time. */
            return;
        }
    }

    /* Increase the buffer if necessary. */
    offset = state->buffer_pos - state->buffer;
    if (offset % BUFFER_SIZE == BUFFER_SIZE - 1) {
        state->buffer = realloc(state->buffer, offset + BUFFER_SIZE + 1);
        state->buffer_pos = state->buffer + offset;
    }

    *(state->buffer_pos++) = str;
}

/**
 * Insert a new component of the given type.
 * This function does not check whether there is an open buffer.
 *
 */
static void xcb_xrm_insert_component(xcb_xrm_entry_t *entry, xcb_xrm_component_type_t type,
        xcb_xrm_binding_type_t binding_type, const char *str) {
    xcb_xrm_component_t *new = calloc(1, sizeof(struct xcb_xrm_component_t));
    if (new == NULL)
        return;

    if (str != NULL) {
        new->name = strdup(str);
        if (new->name == NULL) {
            FREE(new);
            return;
        }
    }

    new->type = type;
    new->binding_type = binding_type;
    TAILQ_INSERT_TAIL(&(entry->components), new, components);
}

/**
 * Finalize the current buffer by writing it into a component if necessary.
 * This function also resets the buffer to a clean slate.
 *
 */
static void xcb_xrm_finalize_component(xcb_xrm_entry_t *entry, xcb_xrm_entry_parser_state_t *state) {
    if (state->buffer_pos != NULL && state->buffer_pos != state->buffer) {
        *(state->buffer_pos) = '\0';
        xcb_xrm_insert_component(entry, CT_NORMAL, state->current_binding_type, state->buffer);
    }

    FREE(state->buffer);
    /* No need to handle NULL for this calloc call. */
    state->buffer = calloc(1, BUFFER_SIZE);
    state->buffer_pos = state->buffer;
    state->current_binding_type = BT_TIGHT;
}

/*
 * Parses a specific resource string.
 *
 * @param str The resource string.
 * @param entry A return struct that will contain the parsed resource. The
 * memory will be allocated dynamically, so it must be freed.
 * @param resource_only If true, only components of type CT_NORMAL are allowed.
 *
 * @return 0 on success, a negative error code otherwise.
 *
 */
int xcb_xrm_entry_parse(const char *_str, xcb_xrm_entry_t **_entry, bool resource_only) {
    char *str;
    xcb_xrm_entry_t *entry = NULL;
    xcb_xrm_component_t *last;
    char *value;
    char *value_walk;

    xcb_xrm_entry_parser_state_t state = {
        .chunk = CS_INITIAL,
        .current_binding_type = BT_TIGHT,
    };

    /* Copy the input string since it's const. */
    str = strdup(_str);
    if (str == NULL)
        return -FAILURE;

    /* This is heavily overestimated, but we'll just keep it simple here.
     * While this does not account for replacement of magic values, those only
     * make the resulting string shorter than the input, so we're okay. */
    value = calloc(1, strlen(str));
    if (value == NULL) {
        FREE(str);
        return -FAILURE;
    }
    value_walk = value;

    /* Allocate memory for the return parameter. */
    *_entry = calloc(1, sizeof(struct xcb_xrm_entry_t));
    if (_entry == NULL) {
        FREE(str);
        FREE(value);
        return -FAILURE;
    }

    entry = *_entry;
    TAILQ_INIT(&(entry->components));

    for (char *walk = str; *walk != '\0'; walk++) {
        switch (*walk) {
            case '.':
            case '*':
                state.chunk = MAX(state.chunk, CS_COMPONENTS);
                if (state.chunk >= CS_PRE_VALUE_WHITESPACE) {
                    goto process_normally;
                }

                if (*walk == '*' && resource_only) {
                    goto done_error;
                }

                xcb_xrm_finalize_component(entry, &state);
                state.current_binding_type = (*walk == '.') ? BT_TIGHT : BT_LOOSE;
                break;
            case '?':
                state.chunk = MAX(state.chunk, CS_COMPONENTS);
                if (state.chunk >= CS_PRE_VALUE_WHITESPACE) {
                    goto process_normally;
                }

                if (resource_only) {
                    goto done_error;
                }

                xcb_xrm_insert_component(entry, CT_WILDCARD, state.current_binding_type, NULL);
                break;
            case ' ':
            case '\t':
                /* Spaces are only allowed in the value, but spaces between the
                 * ':' and the value are omitted. */
                if (state.chunk <= CS_PRE_VALUE_WHITESPACE) {
                    break;
                }

                goto process_normally;
            case ':':
                if (resource_only) {
                    goto done_error;
                }

                if (state.chunk == CS_INITIAL) {
                    goto done_error;
                } else if (state.chunk == CS_COMPONENTS) {
                    xcb_xrm_finalize_component(entry, &state);
                    state.chunk = CS_PRE_VALUE_WHITESPACE;
                    break;
                } else if (state.chunk >= CS_PRE_VALUE_WHITESPACE) {
                    state.chunk = CS_VALUE;
                    goto process_normally;
                }
                break;
            default:
process_normally:
                state.chunk = MAX(state.chunk, CS_COMPONENTS);

                if (state.chunk == CS_PRE_VALUE_WHITESPACE) {
                    state.chunk = CS_VALUE;
                }

                if (state.chunk == CS_COMPONENTS) {
                    if ((*walk != '_' && *walk != '-') &&
                            (*walk < '0' || *walk > '9') &&
                            (*walk < 'a' || *walk > 'z') &&
                            (*walk < 'A' || *walk > 'Z')) {
                        goto done_error;
                    }
                }

                if (state.chunk < CS_VALUE) {
                    xcb_xrm_append_char(entry, &state, *walk);
                } else {
                    if (*walk == '\\') {
                        if (*(walk + 1) == ' ') {
                            *(value_walk++) = ' ';
                            walk++;
                        } else if (*(walk + 1) == '\t') {
                            *(value_walk++) = '\t';
                            walk++;
                        } else if (*(walk + 1) == '\\') {
                            *(value_walk++) = '\\';
                            walk++;
                        } else if (*(walk + 1) == 'n') {
                            *(value_walk++) = '\n';
                            walk++;
                        } else if (isdigit(*(walk + 1)) && isdigit(*(walk + 2)) && isdigit(*(walk + 3)) &&
                                *(walk + 1) < '8' && *(walk + 2) < '8' && *(walk + 3) < '8') {
                            *(value_walk++) = (*(walk + 1) - '0') * 64 + (*(walk + 2) - '0') * 8 + (*(walk + 3) - '0');
                            walk += 3;
                        } else {
                            *(value_walk++) = *walk;
                        }
                    } else {
                        *(value_walk++) = *walk;
                    }
                }

                break;
        }
    }

    if (state.chunk == CS_VALUE) {
        *value_walk = '\0';
        entry->value = strdup(value);
        if (entry->value == NULL)
            goto done_error;
    } else if (!resource_only) {
        /* Return error if there was no value for this entry. */
        goto done_error;
    } else {
        /* Since in the case of resource_only we never went into CS_VALUE, we
         * need to finalize the last component. */
        xcb_xrm_finalize_component(entry, &state);
    }

    /* Assert that this entry actually had a resource component. */
    if ((last = TAILQ_LAST(&(entry->components), components_head)) == NULL) {
        goto done_error;
    }

    /* Assert that the last component is not a wildcard. */
    if (last->type != CT_NORMAL) {
        goto done_error;
    }

    FREE(str);
    FREE(value);
    FREE(state.buffer);
    return 0;

done_error:
    FREE(str);
    FREE(value);
    FREE(state.buffer);

    xcb_xrm_entry_free(entry);
    *_entry = NULL;
    return -1;
}

/*
 * Returns the number of components of the given entry.
 *
 */
int __xcb_xrm_entry_num_components(xcb_xrm_entry_t *entry) {
    int result = 0;

    xcb_xrm_component_t *current;
    TAILQ_FOREACH(current, &(entry->components), components) {
        result++;
    }

    return result;
}

/*
 * Compares the two entries.
 * Returns 0 if they are the same and a negative error code otherwise.
 *
 */
int __xcb_xrm_entry_compare(xcb_xrm_entry_t *first, xcb_xrm_entry_t *second) {
    xcb_xrm_component_t *comp_first = TAILQ_FIRST(&(first->components));
    xcb_xrm_component_t *comp_second = TAILQ_FIRST(&(second->components));

    while (comp_first != NULL && comp_second != NULL) {
        if (comp_first->type != comp_second->type)
            return -FAILURE;

        if (comp_first->binding_type != comp_second->binding_type)
            return -FAILURE;

        if (comp_first->type == CT_NORMAL && strcmp(comp_first->name, comp_second->name) != 0)
            return -FAILURE;

        comp_first = TAILQ_NEXT(comp_first, components);
        comp_second = TAILQ_NEXT(comp_second, components);
    }

    /* At this point, at least one of the two is NULL. If they aren't both
     * NULL, they have a different number of components and cannot be equal. */
    if (comp_first != comp_second) {
        return -FAILURE;
    }

    return SUCCESS;
}

/*
 * Returns a string representation of this entry.
 *
 */
char *__xcb_xrm_entry_to_string(xcb_xrm_entry_t *entry) {
    char *result = NULL;
    char *value_buf;
    char *escaped_value;
    xcb_xrm_component_t *component;
    bool is_first = true;

    assert(entry != NULL);
    TAILQ_FOREACH(component, &(entry->components), components) {
        char *tmp;
        if (asprintf(&tmp, "%s%s%s", result == NULL ? "" : result,
                (is_first && component->binding_type == BT_TIGHT)
                    ? ""
                    : (component->binding_type == BT_TIGHT ? "." : "*"),
                component->type == CT_NORMAL ? component->name : "?") < 0) {
            FREE(result);
            return NULL;
        }
        FREE(result);
        result = tmp;

        is_first = false;
    }

    escaped_value = __xcb_xrm_entry_escape_value(entry->value);
    if (asprintf(&value_buf, "%s: %s", result, escaped_value) < 0) {
        FREE(escaped_value);
        FREE(result);
        return NULL;
    }
    FREE(escaped_value);
    FREE(result);
    result = value_buf;

    return result;
}

/*
 * Copy the entry.
 *
 */
xcb_xrm_entry_t *__xcb_xrm_entry_copy(xcb_xrm_entry_t *entry) {
    xcb_xrm_entry_t *copy;
    xcb_xrm_component_t *component;

    assert(entry != NULL);

    copy = calloc(1, sizeof(struct xcb_xrm_entry_t));
    if (copy == NULL)
        return NULL;

    copy->value = strdup(entry->value);
    if (copy->value == NULL) {
        FREE(copy);
        return NULL;
    }

    TAILQ_INIT(&(copy->components));
    TAILQ_FOREACH(component, &(entry->components), components) {
        xcb_xrm_component_t *new = calloc(1, sizeof(struct xcb_xrm_component_t));
        if (new == NULL) {
            xcb_xrm_entry_free(copy);
            return NULL;
        }

        new->name = strdup(component->name);
        if (new->name == NULL) {
            xcb_xrm_entry_free(copy);
            FREE(new);
            return NULL;
        }

        new->type = component->type;
        new->binding_type = component->binding_type;
        TAILQ_INSERT_TAIL(&(copy->components), new, components);
    }

    return copy;
}

/*
 * Escapes magic values.
 *
 */
char *__xcb_xrm_entry_escape_value(const char *value) {
    char *escaped;
    char *outwalk;
    int new_size = strlen(value) + 1;

    if (value[0] == ' ' || value[0] == '\t')
        new_size++;
    for (const char *walk = value; *walk != '\0'; walk++) {
        if (*walk == '\n' || *walk == '\\')
            new_size++;
    }

    escaped = calloc(1, new_size);
    if (escaped == NULL)
        return NULL;

    outwalk = escaped;
    if (value[0] == ' ' || value[0] == '\t') {
        *(outwalk++) = '\\';
    }
    for (const char *walk = value; *walk != '\0'; walk++) {
        if (*walk == '\n') {
            *(outwalk++) = '\\';
            *(outwalk++) = 'n';
        } else if (*walk == '\\') {
            *(outwalk++) = '\\';
            *(outwalk++) = '\\';
        } else {
            *(outwalk++) = *walk;
        }
    }
    *outwalk = '\0';

    return escaped;
}

/*
 * Frees the given entry.
 *
 * @param entry The entry to be freed.
 *
 */
void xcb_xrm_entry_free(xcb_xrm_entry_t *entry) {
    if (entry == NULL)
        return;

    FREE(entry->value);
    while (!TAILQ_EMPTY(&(entry->components))) {
        xcb_xrm_component_t *component = TAILQ_FIRST(&(entry->components));
        FREE(component->name);
        TAILQ_REMOVE(&(entry->components), component, components);
        FREE(component);
    }

    FREE(entry);
    return;
}
