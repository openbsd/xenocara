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
#include <limits.h>

#include <X11/Xresource.h>

#include "tests_utils.h"

/* Forward declarations */
static int test_get_resource(void);
static int test_convert(void);
static void setup(void);
static void cleanup(void);

static char *check_get_resource_xlib(const char *str_database, const char *res_name, const char *res_class);
static int check_get_resource(const char *database, const char *res_name, const char *res_class, const char *value,
        bool expected_xlib_mismatch);
static int check_convert_to_long(const char *value, const long expected, int expected_return_code);
static int check_convert_to_bool(const char *value, const bool expected, int expected_return_code);

int main(void) {
    bool err = false;

    setup();
    err |= test_get_resource();
    cleanup();

    err |= test_convert();

    return err;
}

static int test_get_resource(void) {
    bool err = false;

    /* Non-matches / Errors */
    err |= check_get_resource("", "", "", NULL, false);
    err |= check_get_resource("", NULL, "", NULL, false);
    err |= check_get_resource("", "", NULL, NULL, false);
    err |= check_get_resource("", NULL, NULL, NULL, false);
    /* Xlib returns the match here, despite the query violating the specs. */
    err |= check_get_resource("First.second: 1", "First.second", "First.second.third", NULL, true);
    err |= check_get_resource("", "First.second", "", NULL, false);
    err |= check_get_resource("First.second: 1", "First.third", "", NULL, false);
    err |= check_get_resource("First.second: 1", "First", "", NULL, false);
    err |= check_get_resource("First: 1", "First.second", "", NULL, false);
    err |= check_get_resource("First.?.fourth: 1", "First.second.third.fourth", "", NULL, false);
    err |= check_get_resource("First*?.third: 1", "First.third", "", NULL, false);
    err |= check_get_resource("First: 1", "first", "", NULL, false);
    err |= check_get_resource("First: 1", "", "first", NULL, false);
    /* Duplicate entries */
    err |= check_get_resource(
            "First: 1\n"
            "First: 2\n"
            "First: 3\n",
            "First", "", "3", false);
    err |= check_get_resource(
            "First: 1\n"
            "Second: 2\n"
            "Second: 3\n"
            "Third: 4\n",
            "Second", "", "3", false);

    /* Basic matching */
    err |= check_get_resource("First: 1", "First", "", "1", false);
    err |= check_get_resource("First.second: 1", "First.second", "", "1", false);
    err |= check_get_resource("?.second: 1", "First.second", "", "1", false);
    err |= check_get_resource("First.?.third: 1", "First.second.third", "", "1", false);
    err |= check_get_resource("First.?.?.fourth: 1", "First.second.third.fourth", "", "1", false);
    err |= check_get_resource("*second: 1", "First.second", "", "1", false);
    err |= check_get_resource(".second: 1", "First.second", "", NULL, false);
    err |= check_get_resource("*third: 1", "First.second.third", "", "1", false);
    err |= check_get_resource("First*second: 1", "First.second", "", "1", false);
    err |= check_get_resource("First*third: 1", "First.second.third", "", "1", false);
    err |= check_get_resource("First*fourth: 1", "First.second.third.fourth", "", "1", false);
    err |= check_get_resource("First*?.third: 1", "First.second.third", "", "1", false);
    err |= check_get_resource("First: 1", "Second", "First", "1", false);
    err |= check_get_resource("First.second: 1", "First.third", "first.second", "1", false);
    err |= check_get_resource("First.second.third: 1", "First.third.third", "first.second.fourth", "1", false);
    err |= check_get_resource("First*third*fifth: 1", "First.second.third.fourth.third.fifth", "", "1", false);
    err |= check_get_resource("First: x\\\ny", "First", "", "xy", false);
    err |= check_get_resource("! First: x", "First", "", NULL, false);
    err |= check_get_resource("# First: x", "First", "", NULL, false);
    err |= check_get_resource("First:", "First", "", "", false);
    err |= check_get_resource("First: ", "First", "", "", false);
    err |= check_get_resource("First: \t ", "First", "", "", false);
    /* Consecutive bindings */
    err |= check_get_resource("*.bar: 1", "foo.foo.bar", "", "1", false);
    err |= check_get_resource("...bar: 1", "foo.bar", "", NULL, false);
    err |= check_get_resource("...bar: 1", "foo.foo.foo.bar", "", NULL, false);
    err |= check_get_resource("***bar: 1", "foo.bar", "", "1", false);
    err |= check_get_resource(".*.bar: 1", "foo.bar", "", "1", false);
    err |= check_get_resource(".*.bar: 1", "foo.foo.bar", "", "1", false);
    err |= check_get_resource("..*bar: 1", "foo.foo.foo.foo.bar", "", "1", false);
    err |= check_get_resource("a.*.z: 1", "a.b.c.d.e.f.z", "", "1", false);
    err |= check_get_resource("a...z: 1", "a.z", "", "1", false);
    err |= check_get_resource("a...z: 1", "a.b.z", "", NULL, false);
    /* Matching among multiple entries */
    err |= check_get_resource(
            "First: 1\n"
            "Second: 2\n",
            "First", "", "1", false);
    err |= check_get_resource(
            "First: 1\n"
            "Second: 2\n",
            "Second", "", "2", false);
    /* Greediness */
    err |= check_get_resource("a*c.e: 1", "a.b.c.d.c.e", "", "1", false);
    err |= check_get_resource("a*c.e: 1", "a.b.c.c.e", "", "1", false);
    err |= check_get_resource("a*?.e: 1", "a.b.c.e", "", "1", false);
    err |= check_get_resource("a*c*e: 1", "a.b.c.d.c.d.e.d.e", "", "1", false);

    /* Precedence rules */
    /* Rule 1 */
    err |= check_get_resource(
            "First.second.third: 1\n"
            "First*third: 2\n",
            "First.second.third", "", "1", false);
    err |= check_get_resource(
            "First*third: 2\n"
            "First.second.third: 1\n",
            "First.second.third", "", "1", false);
    err |= check_get_resource(
            "First.second.third: 1\n"
            "First*third: 2\n",
            "x.x.x", "First.second.third", "1", false);
    err |= check_get_resource(
            "First*third: 2\n"
            "First.second.third: 1\n",
            "x.x.x", "First.second.third", "1", false);

    /* Rule 2 */
    err |= check_get_resource(
            "First.second: 1\n"
            "First.third: 2\n",
            "First.second", "First.third", "1", false);
    err |= check_get_resource(
            "First.third: 2\n"
            "First.second: 1\n",
            "First.second", "First.third", "1", false);
    err |= check_get_resource(
            "First.second.third: 1\n"
            "First.?.third: 2\n",
            "First.second.third", "", "1", false);
    err |= check_get_resource(
            "First.?.third: 2\n"
            "First.second.third: 1\n",
            "First.second.third", "", "1", false);
    err |= check_get_resource(
            "First.second.third: 1\n"
            "First.?.third: 2\n",
            "x.x.x", "First.second.third", "1", false);
    err |= check_get_resource(
            "First.?.third: 2\n"
            "First.second.third: 1\n",
            "x.x.x", "First.second.third", "1", false);
    /* Rule 3 */
    err |= check_get_resource(
            "First.second: 1\n"
            "First*second: 2\n",
            "First.second", "", "1", false);
    err |= check_get_resource(
            "First*second: 2\n"
            "First.second: 1\n",
            "First.second", "", "1", false);

    /* Some real world examples. May contain duplicates to the above tests. */

    /* From the specification:
     * https://tronche.com/gui/x/xlib/resource-manager/matching-rules.html */
    err |= check_get_resource(
            "xmh*Paned*activeForeground: red\n"
            "*incorporate.Foreground: blue\n"
            "xmh.toc*Command*activeForeground: green\n"
            "xmh.toc*?.Foreground: white\n"
            "xmh.toc*Command.activeForeground: black",
            "xmh.toc.messagefunctions.incorporate.activeForeground",
            "Xmh.Paned.Box.Command.Foreground",
            "black", false);
    err |= check_get_resource("urxvt*background: [95]#000", "urxvt.background", "", "[95]#000", false);
    err |= check_get_resource("urxvt*scrollBar_right:true", "urxvt.scrollBar_right", "", "true", false);
    err |= check_get_resource("urxvt*cutchars:    '\"'()*<>[]{|}", "urxvt.cutchars", "", "'\"'()*<>[]{|}", false);
    err |= check_get_resource("urxvt.keysym.Control-Shift-Up: perl:font:increment", "urxvt.keysym.Control-Shift-Up",
            "", "perl:font:increment", false);
    err |= check_get_resource("rofi.normal: #000000, #000000, #000000, #000000", "rofi.normal", "",
            "#000000, #000000, #000000, #000000", false);

    return err;
}

static int test_convert(void) {
    bool err = false;

    err |= check_convert_to_bool(NULL, false, -2);
    err |= check_convert_to_bool("", false, -1);
    err |= check_convert_to_bool("0", false, 0);
    err |= check_convert_to_bool("1", true, 0);
    err |= check_convert_to_bool("10", true, 0);
    err |= check_convert_to_bool("true", true, 0);
    err |= check_convert_to_bool("TRUE", true, 0);
    err |= check_convert_to_bool("false", false, 0);
    err |= check_convert_to_bool("FALSE", false, 0);
    err |= check_convert_to_bool("on", true, 0);
    err |= check_convert_to_bool("ON", true, 0);
    err |= check_convert_to_bool("off", false, 0);
    err |= check_convert_to_bool("OFF", false, 0);
    err |= check_convert_to_bool("yes", true, 0);
    err |= check_convert_to_bool("YES", true, 0);
    err |= check_convert_to_bool("no", false, 0);
    err |= check_convert_to_bool("NO", false, 0);
    err |= check_convert_to_bool("abc", false, -1);

    err |= check_convert_to_long(NULL, LONG_MIN, -2);
    err |= check_convert_to_long("", LONG_MIN, -1);
    err |= check_convert_to_long("0", 0, 0);
    err |= check_convert_to_long("1", 1, 0);
    err |= check_convert_to_long("-1", -1, 0);
    err |= check_convert_to_long("100", 100, 0);

    return err;
}

static char *check_get_resource_xlib(const char *str_database, const char *res_name, const char *res_class) {
    int res_code;
    char *res_type;
    XrmValue res_value;
    char *result;

    XrmDatabase database = XrmGetStringDatabase(str_database);
    res_code = XrmGetResource(database, res_name, res_class, &res_type, &res_value);

    if (res_code) {
        result = strdup((char *)res_value.addr);
    } else {
        result = NULL;
    }

    XrmDestroyDatabase(database);
    return result;
}

static int check_get_resource(const char *str_database, const char *res_name, const char *res_class, const char *value,
        bool expected_xlib_mismatch) {
    xcb_xrm_database_t *database;

    bool err = false;
    char *xcb_value;
    char *xlib_value;

    fprintf(stderr, "== Assert that getting resource <%s> / <%s> returns <%s>\n",
            res_name, res_class, value);

    database = xcb_xrm_database_from_string(str_database);
    if (xcb_xrm_resource_get_string(database, res_name, res_class, &xcb_value) < 0) {
        if (value != NULL) {
            fprintf(stderr, "xcb_xrm_resource_get_string() returned NULL\n");
            err = true;
        }

        if (!expected_xlib_mismatch) {
            xlib_value = check_get_resource_xlib(str_database, res_name, res_class);
            err |= check_strings(NULL, xlib_value, "Returned NULL, but Xlib returned <%s>\n", xlib_value);
            if (xlib_value != NULL)
                free(xlib_value);
        }

        goto done_get_resource;
    }

    err |= check_strings(value, xcb_value, "Expected <%s>, but got <%s>\n", value, xcb_value);
    free(xcb_value);

    if (!expected_xlib_mismatch) {
        /* And for good measure, also compare it against Xlib. */
        xlib_value = check_get_resource_xlib(str_database, res_name, res_class);
        err |= check_strings(value, xlib_value, "Xlib returns <%s>, but expected <%s>\n",
                xlib_value, value);
        if (xlib_value != NULL)
            free(xlib_value);
    }

done_get_resource:
    xcb_xrm_database_free(database);
    return err;
}

static int check_convert_to_long(const char *value, const long expected, int expected_return_code) {
    char *db_str = NULL;
    long actual;
    int actual_return_code;
    xcb_xrm_database_t *database;

    fprintf(stderr, "== Assert that <%s> is converted to long value <%ld>\n", value, expected);

    if (value != NULL)
        asprintf(&db_str, "x: %s\n", value);

    database = xcb_xrm_database_from_string(db_str);
    free(db_str);
    actual_return_code = xcb_xrm_resource_get_long(database, "x", NULL, &actual);
    xcb_xrm_database_free(database);

    return check_ints(expected_return_code, actual_return_code, "Expected <%d>, but found <%d>\n",
            expected_return_code, actual_return_code) ||
        check_longs(expected, actual, "Expected <%ld>, but found <%ld>\n", expected, actual);
}

static int check_convert_to_bool(const char *value, const bool expected, const int expected_return_code) {
    char *db_str = NULL;
    bool actual;
    int actual_return_code;
    xcb_xrm_database_t *database;

    fprintf(stderr, "== Assert that <%s> is converted to boolean value <%d>\n", value, expected);

    if (value != NULL)
        asprintf(&db_str, "x: %s\n", value);

    database = xcb_xrm_database_from_string(db_str);
    free(db_str);
    actual_return_code = xcb_xrm_resource_get_bool(database, "x", NULL, &actual);
    xcb_xrm_database_free(database);

    return check_ints(expected_return_code, actual_return_code, "Expected <%d>, but found <%d>\n",
            expected_return_code, actual_return_code) ||
        check_ints(expected, actual, "Expected <%d>, but found <%d>\n", expected, actual);
}

static void setup(void) {
    XrmInitialize();
}

static void cleanup(void) {
}
