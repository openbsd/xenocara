/*
 * vim:ts=4:sw=4:expandtab
 *
 * Copyright © 2013 Michael Stapelberg
 * Copyright © 2002 Keith Packard
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
#include <unistd.h>

#include <xcb/xcb.h>
#include <xcb/render.h>
#include <xcb/xcb_image.h>

#include "cursor.h"
#include "xcb_cursor.h"

static const char *cursor_path(struct xcb_cursor_context_t *c) {
    if (c->path == NULL) {
        c->path = getenv("XCURSOR_PATH");
        if (c->path == NULL)
            c->path = "~/.icons:/usr/share/icons:/usr/share/pixmaps:/usr/X11R6/lib/X11/icons";
    }
    return c->path;
}

static const char *next_path(const char *path) {
    const char *colon = strchr(path, ':');
    return (colon ? colon + 1 : NULL);
}

/*
 * _XcursorThemeInherits was directly copied from libxcursor so as to not break
 * compatibility.
 *
 */
#define XcursorWhite(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define XcursorSep(c) ((c) == ';' || (c) == ',')

static char *
_XcursorThemeInherits (const char *full)
{
    char    line[8192];
    char    *result = NULL;
    FILE    *f;

    if (!full)
        return NULL;

    f = fopen (full, "r");
    if (f)
    {
        while (fgets (line, sizeof (line), f))
        {
            if (!strncmp (line, "Inherits", 8))
            {
                char    *l = line + 8;
                char    *r;
                while (*l == ' ') l++;
                if (*l != '=') continue;
                l++;
                while (*l == ' ') l++;
                result = malloc (strlen (l));
                if (result)
                {
                    r = result;
                    while (*l)
                    {
                        while (XcursorSep(*l) || XcursorWhite (*l)) l++;
                        if (!*l)
                            break;
                        if (r != result)
                            *r++ = ':';
                        while (*l && !XcursorWhite(*l) &&
                               !XcursorSep(*l))
                            *r++ = *l++;
                    }
                    *r++ = '\0';
                }
                break;
            }
        }
        fclose (f);
    }
    return result;
}

/*
 * Tries to open the cursor file “name” in the “theme”/cursors subfolder of
 * each component of cursor_path(). When the file cannot be found, but a file
 * “index.theme” in the component is present, the Inherits= key will be
 * extracted and open_cursor_file calls itself recursively to search the
 * specified inherited themes, too.
 *
 */
static int open_cursor_file(xcb_cursor_context_t *c, const char *theme, const char *name, int *scan_core) {
    int fd = -1;
    char *inherits = NULL;

    *scan_core = -1;

    if (strcmp(theme, "core") == 0 &&
        (*scan_core = cursor_shape_to_id(name)) >= 0) {
        return -1;
    }

    if (c->home == NULL)
        if ((c->home = getenv("HOME")) == NULL)
            return -1;

    for (const char *path = cursor_path(c);
         (path != NULL && fd == -1);
         ) {
        const char *sep = strchr(path, ':');
        const int pathlen = (sep ? (sep - path) : strlen(path));
        char *themedir = NULL;
        char *full = NULL;
        if (*path == '~') {
            if (asprintf(&themedir, "%s%.*s/%s", c->home, pathlen - 1, path + 1, theme) == -1)
                return -1;
        } else {
            if (asprintf(&themedir, "%.*s/%s", pathlen, path, theme) == -1)
                return -1;
        }
        if (asprintf(&full, "%s/%s/%s", themedir, "cursors", name) == -1) {
            free(themedir);
            return -1;
        }
        fd = open(full, O_RDONLY);
        free(full);
        if (fd == -1 && inherits == NULL) {
            if (asprintf(&full, "%s/index.theme", themedir) == -1) {
                free(themedir);
                return -1;
            }
            inherits = _XcursorThemeInherits(full);
            free(full);
        }
        free(themedir);
        path = (sep ? sep + 1 : NULL);
    }

    for (const char *path = inherits;
         (path != NULL && fd == -1);
         (path = next_path(path))) {
        fd = open_cursor_file(c, path, name, scan_core);
    }

    if (inherits != NULL)
        free(inherits);

    return fd;
}

xcb_cursor_t xcb_cursor_load_cursor(xcb_cursor_context_t *c, const char *name) {
    /* The character id of the X11 "cursor" font when falling back to un-themed
     * cursors. */
    int core_char = -1;
    int fd = -1;
    xcint_image_t *images;
    int nimg = 0;
    xcb_pixmap_t pixmap = XCB_NONE;
    xcb_gcontext_t gc = XCB_NONE;
    uint32_t last_width = 0;
    uint32_t last_height = 0;
    xcb_cursor_t cid = XCB_NONE;

    // NB: if !render_present, fd will be -1 and thus the next if statement
    // will trigger the fallback.
    if (c->render_version != RV_NONE) {
        if (c->rm[RM_XCURSOR_THEME])
            fd = open_cursor_file(c, c->rm[RM_XCURSOR_THEME], name, &core_char);

        if (fd == -1 && core_char == -1)
            fd = open_cursor_file(c, "default", name, &core_char);
    }

    if (fd == -1 || core_char > -1) {
        if (core_char == -1)
            core_char = cursor_shape_to_id(name);
        if (core_char == -1)
            return XCB_NONE;

        cid = xcb_generate_id(c->conn);
        xcb_create_glyph_cursor(c->conn, cid, c->cursor_font, c->cursor_font, core_char, core_char + 1, 0, 0, 0, 65535, 65535, 65535);
        return cid;
    }

    if (parse_cursor_file(c, fd, &images, &nimg) < 0) {
        close(fd);
        return XCB_NONE;
    }

    close(fd);

    /* create a cursor from it */
    xcb_render_animcursorelt_t elements[nimg];
    xcb_render_picture_t pic = xcb_generate_id(c->conn);

    for (int n = 0; n < nimg; n++) {
        xcint_image_t *i = &(images[n]);
        xcb_image_t *img = xcb_image_create_native(c->conn, i->width, i->height, XCB_IMAGE_FORMAT_Z_PIXMAP, 32, NULL, (i->width * i->height * sizeof(uint32_t)), (uint8_t*)i->pixels);

        if (pixmap == XCB_NONE ||
            (i->width != last_width) ||
            (i->height != last_height)) {
            if (pixmap == XCB_NONE) {
                pixmap = xcb_generate_id(c->conn);
                gc = xcb_generate_id(c->conn);
            } else {
                xcb_free_pixmap(c->conn, pixmap);
                xcb_free_gc(c->conn, gc);
            }

            xcb_create_pixmap(c->conn, 32, pixmap, c->root, i->width, i->height);
            xcb_create_gc(c->conn, gc, pixmap, 0, NULL);

            last_width = i->width;
            last_height = i->height;
        }

        xcb_image_put(c->conn, pixmap, gc, img, 0, 0, 0);

        xcb_render_create_picture(c->conn, pic, pixmap, c->pict_format->id, 0, NULL);

        elements[n].cursor = xcb_generate_id(c->conn);
        elements[n].delay = i->delay;

        xcb_render_create_cursor(c->conn, elements[n].cursor, pic, i->xhot, i->yhot);

        xcb_render_free_picture(c->conn, pic);
        xcb_image_destroy(img);
        free(i->pixels);
    }

    xcb_free_pixmap(c->conn, pixmap);
    xcb_free_gc(c->conn, gc);
    free(images);

    if (nimg == 1 || c->render_version == RV_CURSOR) {
        /* non-animated cursor or no support for animated cursors */
        return elements[0].cursor;
    } else {
        cid = xcb_generate_id(c->conn);
        xcb_render_create_anim_cursor (c->conn, cid, nimg, elements);

        for (int n = 0; n < nimg; n++) {
            xcb_free_cursor(c->conn, elements[n].cursor);
        }

        return cid;
    }
}
