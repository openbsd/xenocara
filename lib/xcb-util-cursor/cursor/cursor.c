/*
 * vim:ts=4:sw=4:expandtab
 *
 * Copyright © 2013 Michael Stapelberg
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <xcb/xcb.h>
#include <xcb/xcb_renderutil.h>

#include "cursor.h"
#include "xcb_cursor.h"

/*
 * Parses the root window’s RESOURCE_MANAGER atom contents and stores the
 * attributes declared above in resource_manager_val.
 *
 */
static void parse_resource_manager(xcb_cursor_context_t *c, const xcb_get_property_reply_t *rm_reply) {
    int rm_length;
    char *rm = NULL;
    char *saveptr = NULL;
    char *line = NULL;
    char *sep = NULL;

    if (rm_reply == NULL || (rm_length = xcb_get_property_value_length(rm_reply)) == 0)
        return;

    if (asprintf(&rm, "%.*s", rm_length, (char*)xcb_get_property_value(rm_reply)) == -1)
        return;

    for (char *str = rm; ; str = NULL) {
        if ((line = strtok_r(str, "\n", &saveptr)) == NULL)
            break;
        /* Split the string at the delimiting : */
        if ((sep = strchr(line, ':')) == NULL) {
            /* Invalid line?! */
            free(rm);
            return;
        }
        *(sep++) = '\0';
        while (isspace(*sep))
            sep++;
        /* strdup() may return NULL, which is interpreted later as the key not
         * being available. */
        if (strcmp(line, "Xcursor.theme") == 0) {
            free(c->rm[RM_XCURSOR_THEME]);
            c->rm[RM_XCURSOR_THEME] = strdup(sep);
        } else if (strcmp(line, "Xcursor.size") == 0) {
            free(c->rm[RM_XCURSOR_SIZE]);
            c->rm[RM_XCURSOR_SIZE] = strdup(sep);
        } else if (strcmp(line, "Xft.dpi") == 0) {
            free(c->rm[RM_XFT_DPI]);
            c->rm[RM_XFT_DPI] = strdup(sep);
        }
    }

    free(rm);
}

/*
 * Tries to figure out the cursor size by checking:
 * 1. The environment variable XCURSOR_SIZE
 * 2. The RESOURCE_MANAGER entry Xcursor.size
 * 3. Guess with the RESOURCE_MANAGER entry Xft.dpi * 16 / 72
 * 4. Guess with the display size.
 *
 */
static uint32_t get_default_size(xcb_cursor_context_t *c, xcb_screen_t *screen) {
    char *env;
    uint16_t dim;

    if ((env = getenv("XCURSOR_SIZE")) != NULL)
        return atoi(env);

    if (c->rm[RM_XCURSOR_SIZE] != NULL)
        return atoi(c->rm[RM_XCURSOR_SIZE]);

    if (c->rm[RM_XFT_DPI] != NULL) {
        const int dpi = atoi(c->rm[RM_XFT_DPI]);
        if (dpi > 0)
            return dpi * 16 / 72;
    }

    if (screen->height_in_pixels < screen->width_in_pixels)
        dim = screen->height_in_pixels;
    else
        dim = screen->width_in_pixels;

    return dim / 48;
}

int xcb_cursor_context_new(xcb_connection_t *conn, xcb_screen_t *screen, xcb_cursor_context_t **ctx) {
    xcb_cursor_context_t *c;
    const xcb_query_extension_reply_t *ext;
    xcb_get_property_cookie_t rm_cookie;
    xcb_get_property_reply_t *rm_reply;
    xcb_render_query_pict_formats_cookie_t pf_cookie;
    xcb_render_query_version_cookie_t ver_cookie;

    if ((*ctx = calloc(1, sizeof(struct xcb_cursor_context_t))) == NULL)
        return -errno;

    c = *ctx;
    c->conn = conn;
    c->root = screen->root;
    c->render_version = RV_NONE;

    ext = xcb_get_extension_data(conn, &xcb_render_id);

    // XXX: Is it maybe necessary to ever use long_offset != 0?
    // XXX: proper length? xlib seems to use 100 MB o_O
    rm_cookie = xcb_get_property(conn, 0, c->root, XCB_ATOM_RESOURCE_MANAGER, XCB_ATOM_STRING, 0, 16 * 1024);
    if (ext && ext->present) {
        ver_cookie = xcb_render_query_version(conn, XCB_RENDER_MAJOR_VERSION, XCB_RENDER_MINOR_VERSION);
        pf_cookie = xcb_render_query_pict_formats(conn);
    }
    c->cursor_font = xcb_generate_id(conn);
    xcb_open_font(conn, c->cursor_font, strlen("cursor"), "cursor");

    rm_reply = xcb_get_property_reply(conn, rm_cookie, NULL);
    parse_resource_manager(c, rm_reply);
    free(rm_reply);

    if (ext && ext->present) {
        xcb_render_query_version_reply_t *reply = xcb_render_query_version_reply(conn, ver_cookie, NULL);

        if (reply && (reply->major_version >= 1 || reply->minor_version >= 8))
            c->render_version = RV_ANIM_CURSOR;
        else if (reply && (reply->major_version >= 1 || reply->minor_version >= 5))
            c->render_version = RV_CURSOR;
        free(reply);

        c->pf_reply = xcb_render_query_pict_formats_reply(conn, pf_cookie, NULL);
        c->pict_format = xcb_render_util_find_standard_format(c->pf_reply, XCB_PICT_STANDARD_ARGB_32);
    }

    c->size = get_default_size(c, screen);

    return 0;
}

void xcb_cursor_context_free(xcb_cursor_context_t *c) {
    free(c->rm[RM_XCURSOR_THEME]);
    free(c->rm[RM_XCURSOR_SIZE]);
    free(c->rm[RM_XFT_DPI]);
    free(c->pf_reply);
    free(c);
}
