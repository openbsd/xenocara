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
#ifndef __XCB_XRM_H__
#define __XCB_XRM_H__

#include <stdbool.h>
#include <xcb/xcb.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup xcb_xrm_database_t XCB XRM Functions
 *
 * These functions are the xcb equivalent of the Xrm* function family in Xlib.
 * They allow the parsing and matching of X resources as well as some utility
 * functions.
 *
 * Here is an example of how this library can be used to retrieve a
 * user-configured resource:
 * @code
 * xcb_connection_t *conn = xcb_connect(NULL, &screennr);
 * if (conn == NULL || xcb_connection_has_error(conn))
 *     err(EXIT_FAILURE, "Could not connect to the X server.");
 *
 * xcb_xrm_database_t *database = xcb_xrm_database_from_default(conn);
 * if (database == NULL)
 *     err(EXIT_FAILURE, "Could not open database");
 *
 * char *value;
 * if (xcb_xrm_resource_get_string(database, "Xft.dpi", NULL, &value) >= 0) {
 *     fprintf(stdout, "Xft.dpi: %s\n", value);
 *     free(value);
 * }
 *
 * xcb_xrm_database_free(database);
 * xcb_disconnect(conn);
 * @endcode
 *
 * @{
 */

/**
 * @struct xcb_xrm_database_t
 * Reference to a database.
 *
 * The database can be loaded in different ways, e.g., from the
 * RESOURCE_MANAGER property by using @ref
 * xcb_xrm_database_from_resource_manager (). All queries for a resource go
 * against a specific database. A database must always be free'd by using @ref
 * xcb_xrm_database_free ().
 *
 * Note that a database is not thread-safe, i.e., multiple threads should not
 * operate on the same database instance. This is especially true for write
 * operations on the database. However, you can use this library in a
 * multi-threaded application as long as the database is thread-local.
 */
typedef struct xcb_xrm_database_t xcb_xrm_database_t;

/**
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
xcb_xrm_database_t *xcb_xrm_database_from_default(xcb_connection_t *conn);

/**
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
xcb_xrm_database_t *xcb_xrm_database_from_resource_manager(xcb_connection_t *conn, xcb_screen_t *screen);

/**
 * Creates a database from the given string.
 * If the database could not be created, this function will return NULL.
 *
 * @param str The resource string.
 * @returns The database described by the resource string.
 *
 * @ingroup xcb_xrm_database_t
 */
xcb_xrm_database_t *xcb_xrm_database_from_string(const char *str);

/**
 * Creates a database from a given file.
 * If the file cannot be found or opened, NULL is returned.
 *
 * @param filename Valid filename.
 * @returns The database described by the file's contents.
 */
xcb_xrm_database_t *xcb_xrm_database_from_file(const char *filename);

/**
 * Returns a string representation of a database.
 * The string is owned by the caller and must be free'd.
 *
 * @param database The database to return in string format.
 * @returns A string representation of the specified database.
 */
char *xcb_xrm_database_to_string(xcb_xrm_database_t *database);

/**
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
void xcb_xrm_database_combine(xcb_xrm_database_t *source_db, xcb_xrm_database_t **target_db, bool override);

/**
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
void xcb_xrm_database_put_resource(xcb_xrm_database_t **database, const char *resource, const char *value);

/**
 * Inserts a new resource into the database.
 * If the resource already exists, the current value will be replaced.
 * If NULL is passed for database, a new and empty database will be created and
 * returned in the pointer.
 *
 * @param database The database to modify.
 * @param line The complete resource specification to insert.
 */
void xcb_xrm_database_put_resource_line(xcb_xrm_database_t **database, const char *line);

/**
 * Destroys the given database.
 *
 * @param database The database to destroy.
 *
 * @ingroup xcb_xrm_database_t
 */
void xcb_xrm_database_free(xcb_xrm_database_t *database);

/**
 * Find the string value of a resource.
 *
 * Note that the string is owned by the caller and must be free'd.
 *
 * @param database The database to query.
 * @param res_name The fully qualified resource name string.
 * @param res_class The fully qualified resource class string. This argument
 * may be left empty / NULL, but if given, it must contain the same number of
 * components as res_name.
 * @param out Out parameter to which the value will be written.
 * @returns 0 if the resource was found, a negative error code otherwise.
 */
int xcb_xrm_resource_get_string(xcb_xrm_database_t *database,
        const char *res_name, const char *res_class, char **out);

/**
 * Find the long value of a resource.
 *
 * @param database The database to query.
 * @param res_name The fully qualified resource name string.
 * @param res_class The fully qualified resource class string. This argument
 * may be left empty / NULL, but if given, it must contain the same number of
 * components as res_name.
 * @param out Out parameter to which the converted value will be written.
 * @returns 0 if the resource was found and converted, -1 if the resource was
 * found but could not be converted and -2 if the resource was not found.
 */
int xcb_xrm_resource_get_long(xcb_xrm_database_t *database,
        const char *res_name, const char *res_class, long *out);

/**
 * Find the bool value of a resource.
 *
 * The conversion to a bool is done by applying the following steps in order:
 *   - If the value can be converted to a long, return the truthiness of the
 *     converted number.
 *   - If the value is one of "true", "on" or "yes" (case-insensitive), return
 *     true.
 *   - If the value is one of "false", "off" or "no" (case-insensitive), return
 *     false.
 *
 * @param database The database to query.
 * @param res_name The fully qualified resource name string.
 * @param res_class The fully qualified resource class string. This argument
 * may be left empty / NULL, but if given, it must contain the same number of
 * components as res_name.
 * @param out Out parameter to which the converted value will be written.
 * @returns 0 if the resource was found and converted, -1 if the resource was
 * found but could not be converted and -2 if the resource was not found.
 */
int xcb_xrm_resource_get_bool(xcb_xrm_database_t *database,
        const char *res_name, const char *res_class, bool *out);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __XCB_XRM_H__ */
