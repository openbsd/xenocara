/* Copyright © 2016 Ingo Bürk
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
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>

#include "tests_utils.h"

/* Forward declarations */
static int test_put_resource(void);
static int test_combine_databases(void);
static int test_from_file(void);
static void setup(void);
static void cleanup(void);

xcb_connection_t *conn;
xcb_screen_t *screen;

int main(void) {
    bool err = false;

    setup();
    err |= test_put_resource();
    err |= test_combine_databases();
    err |= test_from_file();
    cleanup();

    return err;
}

static int test_put_resource(void) {
    bool err = false;

    xcb_xrm_database_t *database = NULL;
    xcb_xrm_database_put_resource(&database, "First", "1");
    xcb_xrm_database_put_resource(&database, "First*second", "2");
    xcb_xrm_database_put_resource(&database, "Third", "  a\\ b\nc d\te ");
    xcb_xrm_database_put_resource(&database, "Fourth", "\t\ta\\ b\nc d\te ");
    err |= check_database(database,
            "First: 1\n"
            "First*second: 2\n"
            "Third: \\  a\\\\ b\\nc d\te \n"
            "Fourth: \\\t\ta\\\\ b\\nc d\te \n");

    xcb_xrm_database_put_resource(&database, "First", "3");
    xcb_xrm_database_put_resource(&database, "First*second", "4");
    xcb_xrm_database_put_resource(&database, "Third", "x");
    xcb_xrm_database_put_resource(&database, "Fourth", "x");
    err |= check_database(database,
            "First: 3\n"
            "First*second: 4\n"
            "Third: x\n"
            "Fourth: x\n");

    xcb_xrm_database_put_resource_line(&database, "Second:xyz");
    xcb_xrm_database_put_resource_line(&database, "Third:  xyz");
    xcb_xrm_database_put_resource_line(&database, "*Fifth.sixth*seventh.?.eigth*?*last: xyz");
    err |= check_database(database,
            "First: 3\n"
            "First*second: 4\n"
            "Fourth: x\n"
            "Second: xyz\n"
            "Third: xyz\n"
            "*Fifth.sixth*seventh.?.eigth*?*last: xyz\n");

    xcb_xrm_database_free(database);
    return err;
}

static int test_combine_databases(void) {
    bool err = false;

    xcb_xrm_database_t *source_db;
    xcb_xrm_database_t *target_db;

    source_db = xcb_xrm_database_from_string(
            "a1.b1*c1: 1\n"
            "a2.b2: 2\n"
            "a3: 3\n");
    target_db = xcb_xrm_database_from_string(
            "a3: 0\n"
            "a1.b1*c1: 0\n"
            "a4.?.b4: 0\n");
    xcb_xrm_database_combine(source_db, &target_db, false);
    err |= check_database(target_db,
            "a3: 0\n"
            "a1.b1*c1: 0\n"
            "a4.?.b4: 0\n"
            "a2.b2: 2\n");
    xcb_xrm_database_free(source_db);
    xcb_xrm_database_free(target_db);

    source_db = xcb_xrm_database_from_string(
            "a1.b1*c1: 1\n"
            "a2.b2: 2\n"
            "a3: 3\n");
    target_db = xcb_xrm_database_from_string(
            "a3: 0\n"
            "a1.b1*c1: 0\n"
            "a4.?.b4: 0\n");
    xcb_xrm_database_combine(source_db, &target_db, true);
    err |= check_database(target_db,
            "a4.?.b4: 0\n"
            "a1.b1*c1: 1\n"
            "a2.b2: 2\n"
            "a3: 3\n");
    xcb_xrm_database_free(source_db);
    xcb_xrm_database_free(target_db);

    return err;
}

static void set_env_var_to_path(const char *var, const char *srcdir, const char *path) {
    char *buffer;
    asprintf(&buffer, "%s/%s", srcdir, path);
    setenv(var, buffer, true);
    free(buffer);
}

static int test_from_file(void) {
    bool err = false;
    xcb_xrm_database_t *database;
    char *path;
    const char *srcdir;

    /* Set by automake, needed for out-of-tree builds */
    srcdir = getenv("srcdir");
    if (srcdir == NULL)
        srcdir = ".";

    /* Test xcb_xrm_database_from_file with relative #include directives */
    asprintf(&path, "%s/tests/resources/1/xresources1", srcdir);
    database = xcb_xrm_database_from_file(path);
    free(path);
    err |= check_database(database,
            "First: 1\n"
            "Third: 3\n"
            "Second: 2\n");
    xcb_xrm_database_free(database);

    /* Test that the inclusion depth is limited */
    asprintf(&path, "%s/tests/resources/3/loop.xresources", srcdir);
    database = xcb_xrm_database_from_file(path);
    free(path);
    err |= check_database(database,
            "First: 1\n"
            "Second: 2\n");
    xcb_xrm_database_free(database);

    /* Test xcb_xrm_database_from_default for resolution of $HOME. */
    set_env_var_to_path("HOME", srcdir, "tests/resources/2");
    set_env_var_to_path("XENVIRONMENT", srcdir, "tests/resources/2/xenvironment");
    database = xcb_xrm_database_from_default(conn);
    err |= check_database(database,
            "First: 1\n"
            "Second: 2\n");
    xcb_xrm_database_free(database);

    /* Test xcb_xrm_database_from_resource_manager. */
    xcb_change_property_checked(conn, XCB_PROP_MODE_REPLACE, screen->root, XCB_ATOM_RESOURCE_MANAGER,
            XCB_ATOM_STRING, 8, strlen("First: 1\n*Second: 2") + 1, "First: 1\n*Second: 2\0");
    xcb_flush(conn);
    database = xcb_xrm_database_from_resource_manager(conn, screen);
    err |= check_database(database,
            "First: 1\n"
            "*Second: 2\n");
    xcb_xrm_database_free(database);

    return err;
}

static void setup(void) {
    int screennr;
    conn = xcb_connect(NULL, &screennr);
    if (xcb_connection_has_error(conn)) {
        fprintf(stderr, "Failed to connect to X11 server.\n");
        exit(EXIT_FAILURE);
    }

    screen = xcb_aux_get_screen(conn, 0);
}

static void cleanup(void) {
    xcb_generic_event_t *ev;

    xcb_aux_sync(conn);
    while ((ev = xcb_poll_for_event(conn))) {
        if (ev->response_type == 0) {
            puts("X11 error occurred");
            exit(EXIT_FAILURE);
        }
        free(ev);
    }
    if (xcb_connection_has_error(conn)) {
        fprintf(stderr, "The X11 connection broke at runtime.\n");
        exit(EXIT_FAILURE);
    }
    xcb_disconnect(conn);
}
