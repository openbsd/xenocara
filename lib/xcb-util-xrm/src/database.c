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

#include "database.h"
#include "match.h"
#include "util.h"

#ifndef MAX_INCLUDE_DEPTH
/* We want to limit the maximum depth of (recursive) #include directives. This
 * is to avoid accidental cyclic inclusions which would lead to an endless loop
 * otherwise. */
#define MAX_INCLUDE_DEPTH 100
#endif

/* Forward declarations */
static xcb_xrm_database_t *__xcb_xrm_database_from_string(const char *_str, const char *base, int depth);
static xcb_xrm_database_t *__xcb_xrm_database_from_file(const char *_filename, const char *base, int depth);
static void __xcb_xrm_database_put(xcb_xrm_database_t *database, xcb_xrm_entry_t *entry, bool override);

/*
 * Creates a database similarly to XGetDefault(). For typical applications,
 * this is the recommended way to construct the resource database.
 *
 * The database is created as follows:
 *   - If the RESOURCE_MANAGER property exists on the root window of
 *     screen 0, the database is constructed from it using @ref
 *     xcb_xrm_database_from_resource_manager().
 *   - Otherwise, if $HOME/.Xresources exists, the database is constructed from
 *     it using @ref xcb_xrm_database_from_file().
 *   - Otherwise, if $HOME/.Xdefaults exists, the database is constructed from
 *     it using @ref xcb_xrm_database_from_file().
 *   - If the environment variable XENVIRONMENT is set, the file specified by
 *     it is loaded using @ref xcb_xrm_database_from_file and then combined with
 *     the database using @ref xcb_xrm_database_combine() with override set to
 *     true.
 *     If XENVIRONMENT is not specified, the same is done with
 *     $HOME/.Xdefaults-$HOSTNAME, wherein $HOSTNAME is determined by
 *     gethostname(2).
 *
 * This represents the way XGetDefault() creates the database for the most
 * part, but is not exactly the same. In particular, XGetDefault() does not
 * consider $HOME/.Xresources.
 *
 * @param conn XCB connection.
 * @returns The constructed database. Can return NULL, e.g., if the screen
 * cannot be determined.
 */
xcb_xrm_database_t *xcb_xrm_database_from_default(xcb_connection_t *conn) {
    xcb_screen_t *screen;
    xcb_xrm_database_t *database;
    char *xenvironment;

    screen = xcb_aux_get_screen(conn, 0);
    if (screen == NULL)
        return NULL;

    /* 1. Try to load the database from RESOURCE_MANAGER. */
    database = xcb_xrm_database_from_resource_manager(conn, screen);

    /* 2. Otherwise, try to load the database from $HOME/.Xresources. */
    if (database == NULL) {
        char *xresources = get_home_dir_file(".Xresources");
        database = xcb_xrm_database_from_file(xresources);
        FREE(xresources);
    }

    /* 3. Otherwise, try to load the database from $HOME/.Xdefaults. */
    if (database == NULL) {
        char *xdefaults = get_home_dir_file(".Xdefaults");
        database = xcb_xrm_database_from_file(xdefaults);
        FREE(xdefaults);
    }

    /* 4. If XENVIRONMENT is specified, merge the database defined by that file.
     *    Otherwise, use $HOME/.Xdefaults-$HOSTNAME. */
    if ((xenvironment = getenv("XENVIRONMENT")) != NULL) {
        xcb_xrm_database_t *source = xcb_xrm_database_from_file(xenvironment);
        xcb_xrm_database_combine(source, &database, true);
        xcb_xrm_database_free(source);
    } else {
        char hostname[1024];
        hostname[1023] = '\0';
        if (gethostname(hostname, 1023) == 0) {
            char *name;
            if (asprintf(&name, ".Xdefaults-%s", hostname) >= 0) {
                xcb_xrm_database_t *source;

                char *xdefaults = get_home_dir_file(name);
                FREE(name);

                source = xcb_xrm_database_from_file(xdefaults);
                FREE(xdefaults);

                xcb_xrm_database_combine(source, &database, true);
                xcb_xrm_database_free(source);
            }
        }
    }

    return database;
}

/*
 * Loads the RESOURCE_MANAGER property and creates a database with its
 * contents. If the database could not be created, this function will return
 * NULL.
 *
 * @param conn A working XCB connection.
 * @param screen The xcb_screen_t* screen to use.
 * @returns The database described by the RESOURCE_MANAGER property.
 *
 * @ingroup xcb_xrm_database_t
 */
xcb_xrm_database_t *xcb_xrm_database_from_resource_manager(xcb_connection_t *conn, xcb_screen_t *screen) {
    xcb_xrm_database_t *database;

    char *resources = xcb_util_get_property(conn, screen->root, XCB_ATOM_RESOURCE_MANAGER,
            XCB_ATOM_STRING, 16 * 1024);
    if (resources == NULL) {
        return NULL;
    }

    /* Parse the resource string. */
    database = xcb_xrm_database_from_string(resources);
    FREE(resources);
    return database;
}

/*
 * Creates a database from the given string.
 * If the database could not be created, this function will return NULL.
 *
 * @param str The resource string.
 * @returns The database described by the resource string.
 *
 * @ingroup xcb_xrm_database_t
 */
xcb_xrm_database_t *xcb_xrm_database_from_string(const char *str) {
    return __xcb_xrm_database_from_string(str, NULL, 0);
}

static xcb_xrm_database_t *__xcb_xrm_database_from_string(const char *_str, const char *base, int depth) {
    xcb_xrm_database_t *database;
    char *str;
    int num_continuations = 0;
    char *str_continued;
    char *outwalk;
    char *saveptr = NULL;

    if (_str == NULL)
        return xcb_xrm_database_from_string("");

    str = strdup(_str);
    if (str == NULL)
        return NULL;

    /* Count the number of line continuations. */
    for (char *walk = str; *walk != '\0'; walk++) {
        if (*walk == '\\' && *(walk + 1) == '\n') {
            num_continuations++;
        }
    }

    /* Take care of line continuations. */
    str_continued = calloc(1, strlen(str) + 1 - 2 * num_continuations);
    if (str_continued == NULL) {
        FREE(str);
        return NULL;
    }

    outwalk = str_continued;
    for (char *walk = str; *walk != '\0'; walk++) {
        if (*walk == '\\' && *(walk + 1) == '\n') {
            walk++;
            continue;
        }

        *(outwalk++) = *walk;
    }
    *outwalk = '\0';

    database = calloc(1, sizeof(struct xcb_xrm_database_t));
    if (database == NULL) {
        FREE(str);
        FREE(str_continued);
        return NULL;
    }

    TAILQ_INIT(database);

    for (char *line = strtok_r(str_continued, "\n", &saveptr); line != NULL; line = strtok_r(NULL, "\n", &saveptr)) {
        /* Handle include directives. */
        if (line[0] == '#') {
            int i = 1;

            /* Skip whitespace and quotes. */
            while (line[i] == ' ' || line[i] == '\t')
                i++;

            if (depth < MAX_INCLUDE_DEPTH &&
                    line[i++] == 'i' &&
                    line[i++] == 'n' &&
                    line[i++] == 'c' &&
                    line[i++] == 'l' &&
                    line[i++] == 'u' &&
                    line[i++] == 'd' &&
                    line[i++] == 'e') {
                xcb_xrm_database_t *included;
                char *filename;
                char *copy;
                char *new_base;
                int j = strlen(line) - 1;

                /* Skip whitespace and quotes. */
                while (line[i] == ' ' || line[i] == '\t' || line[i] == '"')
                    i++;
                while (line[j] == ' ' || line[j] == '\t' || line[j] == '"')
                    j--;

                if (j < i) {
                    /* Only whitespace left in this line. */
                    continue;
                }

                line[j+1] = '\0';
                filename = resolve_path(&line[i], base);
                if (filename == NULL)
                    continue;

                /* We need to strdup() the filename since dirname() will modify it. */
                copy = strdup(filename);
                if (copy == NULL) {
                    FREE(filename);
                    continue;
                }

                new_base = dirname(copy);
                if (new_base == NULL) {
                    FREE(filename);
                    FREE(copy);
                    continue;
                }

                included = __xcb_xrm_database_from_file(filename, new_base, depth + 1);
                FREE(filename);
                FREE(copy);

                if (included != NULL) {
                    xcb_xrm_database_combine(included, &database, true);
                    xcb_xrm_database_free(included);
                }

                continue;
            }
        }

        xcb_xrm_database_put_resource_line(&database, line);
    }

    FREE(str);
    FREE(str_continued);
    return database;
}

/*
 * Creates a database from a given file.
 * If the file cannot be found or opened, NULL is returned.
 *
 * @param filename Valid filename.
 * @returns The database described by the file's contents.
 */
xcb_xrm_database_t *xcb_xrm_database_from_file(const char *filename) {
    return __xcb_xrm_database_from_file(filename, NULL, 0);
}

static xcb_xrm_database_t *__xcb_xrm_database_from_file(const char *_filename, const char *base, int depth) {
    char *filename = NULL;
    char *copy = NULL;
    char *new_base = NULL;
    char *content = NULL;
    xcb_xrm_database_t *database = NULL;

    if (_filename == NULL)
        return NULL;

    filename = resolve_path(_filename, base);
    if (filename == NULL)
        return NULL;

    /* We need to strdup() the filename since dirname() will modify it. */
    copy = strdup(filename);
    if (copy == NULL)
        goto done_from_file;

    new_base = dirname(copy);
    if (new_base == NULL)
        goto done_from_file;

    content = file_get_contents(filename);
    if (content == NULL)
        goto done_from_file;

    database = __xcb_xrm_database_from_string(content, new_base, depth);

done_from_file:
    FREE(filename);
    FREE(copy);
    FREE(content);

    return database;
}

/*
 * Returns a string representation of a database.
 * The string is owned by the caller and must be free'd.
 *
 * @param database The database to return in string format.
 * @returns A string representation of the specified database.
 */
char *xcb_xrm_database_to_string(xcb_xrm_database_t *database) {
    char *result = NULL;
    xcb_xrm_entry_t *entry;

    if (database == NULL)
        return NULL;

    TAILQ_FOREACH(entry, database, entries) {
        char *entry_str = __xcb_xrm_entry_to_string(entry);
        char *tmp;
        if (asprintf(&tmp, "%s%s\n", result == NULL ? "" : result, entry_str) < 0) {
            FREE(entry_str);
            FREE(result);
            return NULL;
        }
        FREE(entry_str);
        FREE(result);
        result = tmp;
    }

    return result;
}

/*
 * Combines two databases.
 * The entries from the source database are stored in the target database. If
 * the same specifier already exists in the target database, the value will be
 * overridden if override is set; otherwise, the value is discarded.
 * If NULL is passed for target_db, a new and empty database will be created
 * and returned in the pointer.
 *
 * @param source_db Source database.
 * @param target_db Target database.
 * @param override If true, entries from the source database override entries
 * in the target database using the same resource specifier.
 */
void xcb_xrm_database_combine(xcb_xrm_database_t *source_db, xcb_xrm_database_t **target_db, bool override) {
    xcb_xrm_entry_t *entry;

    if (*target_db == NULL)
        *target_db = xcb_xrm_database_from_string("");
    if (source_db == NULL)
        return;

    if (source_db == *target_db)
        return;

    TAILQ_FOREACH(entry, source_db, entries) {
        xcb_xrm_entry_t *copy = __xcb_xrm_entry_copy(entry);
        __xcb_xrm_database_put(*target_db, copy, override);
    }
}

/*
 * Inserts a new resource into the database.
 * If the resource already exists, the current value will be replaced.
 * If NULL is passed for database, a new and empty database will be created and
 * returned in the pointer.
 *
 * Note that this is not the equivalent of @ref
 * xcb_xrm_database_put_resource_line when concatenating the resource name and
 * value with a colon. For example, if the value starts with a leading space,
 * this must (and will) be replaced with the special '\ ' sequence.
 *
 * @param database The database to modify.
 * @param resource The fully qualified or partial resource specifier.
 * @param value The value of the resource.
 */
void xcb_xrm_database_put_resource(xcb_xrm_database_t **database, const char *resource, const char *value) {
    char *escaped;
    char *line;

    assert(resource != NULL);
    assert(value != NULL);

    if (*database == NULL)
        *database = xcb_xrm_database_from_string("");

    escaped = __xcb_xrm_entry_escape_value(value);
    if (escaped == NULL)
        return;
    if (asprintf(&line, "%s: %s", resource, escaped) < 0) {
        FREE(escaped);
        return;
    }
    FREE(escaped);
    xcb_xrm_database_put_resource_line(database, line);
    FREE(line);
}

/*
 * Inserts a new resource into the database.
 * If the resource already exists, the current value will be replaced.
 * If NULL is passed for database, a new and empty database will be created and
 * returned in the pointer.
 *
 * @param database The database to modify.
 * @param line The complete resource specification to insert.
 */
void xcb_xrm_database_put_resource_line(xcb_xrm_database_t **database, const char *line) {
    xcb_xrm_entry_t *entry;

    assert(line != NULL);

    if (*database == NULL)
        *database = xcb_xrm_database_from_string("");

    /* Ignore comments and directives. The specification guarantees that no
     * whitespace is allowed before these characters. */
    if (line[0] == '!' || line[0] == '#')
        return;

    if (xcb_xrm_entry_parse(line, &entry, false) == 0) {
        __xcb_xrm_database_put(*database, entry, true);
    }
}

/**
 * Destroys the given database.
 *
 * @param database The database to destroy.
 *
 * @ingroup xcb_xrm_database_t
 */
void xcb_xrm_database_free(xcb_xrm_database_t *database) {
    if (database == NULL)
        return;

    while (!TAILQ_EMPTY(database)) {
        xcb_xrm_entry_t *entry = TAILQ_FIRST(database);
        TAILQ_REMOVE(database, entry, entries);
        xcb_xrm_entry_free(entry);
    }

    FREE(database);
}

static void __xcb_xrm_database_put(xcb_xrm_database_t *database, xcb_xrm_entry_t *entry, bool override) {
    xcb_xrm_entry_t *current;

    if (database == NULL || entry == NULL)
        return;

    /* Let's see whether this is a duplicate entry. */
    current = TAILQ_FIRST(database);
    while (current != NULL) {
        xcb_xrm_entry_t *previous = TAILQ_PREV(current, xcb_xrm_database_t, entries);

        if (__xcb_xrm_entry_compare(entry, current) == 0) {
            if (!override) {
                xcb_xrm_entry_free(entry);
                return;
            }

            TAILQ_REMOVE(database, current, entries);
            xcb_xrm_entry_free(current);

            current = previous;
            if (current == NULL)
                current = TAILQ_FIRST(database);
        }

        if (current == NULL)
            break;
        current = TAILQ_NEXT(current, entries);
    }

    TAILQ_INSERT_TAIL(database, entry, entries);
}
