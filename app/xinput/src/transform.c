/*
 * Copyright © 2011 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */


#include "xinput.h"
#include <string.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xinerama.h>


typedef struct Matrix {
    float m[9];
} Matrix;

static void matrix_set(Matrix *m, int row, int col, float val)
{
    m->m[row * 3 + col] = val;
}

static void matrix_set_unity(Matrix *m)
{
    memset(m, 0, sizeof(m->m));
    matrix_set(m, 0, 0, 1);
    matrix_set(m, 1, 1, 1);
    matrix_set(m, 2, 2, 1);
}

#if DEBUG
static void matrix_print(const Matrix *m)
{
    printf("[ %3.3f %3.3f %3.3f ]\n", m->m[0], m->m[1], m->m[2]);
    printf("[ %3.3f %3.3f %3.3f ]\n", m->m[3], m->m[4], m->m[5]);
    printf("[ %3.3f %3.3f %3.3f ]\n", m->m[6], m->m[7], m->m[8]);
}
#endif

static int
apply_matrix(Display *dpy, int deviceid, Matrix *m)
{
    Atom prop_float, prop_matrix;

    union {
        unsigned char *c;
        float *f;
    } data;
    int format_return;
    Atom type_return;
    unsigned long nitems;
    unsigned long bytes_after;

    int rc;

    prop_float = XInternAtom(dpy, "FLOAT", False);
    prop_matrix = XInternAtom(dpy, "Coordinate Transformation Matrix", False);

    if (!prop_float)
    {
        fprintf(stderr, "Float atom not found. This server is too old.\n");
        return EXIT_FAILURE;
    }
    if (!prop_matrix)
    {
        fprintf(stderr, "Coordinate transformation matrix not found. This "
                "server is too old\n");
        return EXIT_FAILURE;
    }

    rc = XIGetProperty(dpy, deviceid, prop_matrix, 0, 9, False, prop_float,
                       &type_return, &format_return, &nitems, &bytes_after,
                       &data.c);
    if (rc != Success || prop_float != type_return || format_return != 32 ||
        nitems != 9 || bytes_after != 0)
    {
        fprintf(stderr, "Failed to retrieve current property values\n");
        return EXIT_FAILURE;
    }

    memcpy(data.f, m->m, sizeof(m->m));

    XIChangeProperty(dpy, deviceid, prop_matrix, prop_float,
                     format_return, PropModeReplace, data.c, nitems);

    XFree(data.c);

    return EXIT_SUCCESS;
}

static void
matrix_s4(Matrix *m, float x02, float x12, float d1, float d2, int main_diag)
{
    matrix_set(m, 0, 2, x02);
    matrix_set(m, 1, 2, x12);

    if (main_diag) {
        matrix_set(m, 0, 0, d1);
        matrix_set(m, 1, 1, d2);
    } else {
        matrix_set(m, 0, 0, 0);
        matrix_set(m, 1, 1, 0);
        matrix_set(m, 0, 1, d1);
        matrix_set(m, 1, 0, d2);
    }
}

#define RR_Reflect_All	(RR_Reflect_X|RR_Reflect_Y)

static void
set_transformation_matrix(Display *dpy, Matrix *m, int offset_x, int offset_y,
                          int screen_width, int screen_height,
                          int rotation)
{
    /* total display size */
    int width = DisplayWidth(dpy, DefaultScreen(dpy));
    int height = DisplayHeight(dpy, DefaultScreen(dpy));

    /* offset */
    float x = 1.0 * offset_x/width;
    float y = 1.0 * offset_y/height;

    /* mapping */
    float w = 1.0 * screen_width/width;
    float h = 1.0 * screen_height/height;

    matrix_set_unity(m);

    /*
     * There are 16 cases:
     * Rotation X Reflection
     * Rotation: 0 | 90 | 180 | 270
     * Reflection: None | X | Y | XY
     *
     * They are spelled out instead of doing matrix multiplication to avoid
     * any floating point errors.
     */
    switch (rotation) {
    case RR_Rotate_0:
    case RR_Rotate_180 | RR_Reflect_All:
        matrix_s4(m, x, y, w, h, 1);
        break;
    case RR_Reflect_X|RR_Rotate_0:
    case RR_Reflect_Y|RR_Rotate_180:
        matrix_s4(m, x + w, y, -w, h, 1);
        break;
    case RR_Reflect_Y|RR_Rotate_0:
    case RR_Reflect_X|RR_Rotate_180:
        matrix_s4(m, x, y + h, w, -h, 1);
        break;
    case RR_Rotate_90:
    case RR_Rotate_270 | RR_Reflect_All: /* left limited - correct in working zone. */
        matrix_s4(m, x + w, y, -w, h, 0);
        break;
    case RR_Rotate_270:
    case RR_Rotate_90 | RR_Reflect_All: /* left limited - correct in working zone. */
        matrix_s4(m, x, y + h, w, -h, 0);
        break;
    case RR_Rotate_90 | RR_Reflect_X: /* left limited - correct in working zone. */
    case RR_Rotate_270 | RR_Reflect_Y: /* left limited - correct in working zone. */
        matrix_s4(m, x, y, w, h, 0);
        break;
    case RR_Rotate_90 | RR_Reflect_Y: /* right limited - correct in working zone. */
    case RR_Rotate_270 | RR_Reflect_X: /* right limited - correct in working zone. */
        matrix_s4(m, x + w, y + h, -w, -h, 0);
        break;
    case RR_Rotate_180:
    case RR_Reflect_All|RR_Rotate_0:
        matrix_s4(m, x + w, y + h, -w, -h, 1);
        break;
    }

#if DEBUG
    matrix_print(m);
#endif
}

/* Caller must free return value */
static XRROutputInfo*
find_output_xrandr(Display *dpy, const char *output_name)
{
    XRRScreenResources *res;
    XRROutputInfo *output_info = NULL;
    int i;
    int found = 0;

    res = XRRGetScreenResources(dpy, DefaultRootWindow(dpy));

    for (i = 0; i < res->noutput && !found; i++)
    {
        output_info = XRRGetOutputInfo(dpy, res, res->outputs[i]);

        if (output_info->crtc && output_info->connection == RR_Connected &&
            strcmp(output_info->name, output_name) == 0)
        {
            found = 1;
            break;
        }

        XRRFreeOutputInfo(output_info);
    }

    XRRFreeScreenResources(res);

    if (!found)
        output_info = NULL;

    return output_info;
}

static int
map_output_xrandr(Display *dpy, int deviceid, const char *output_name)
{
    int rc = EXIT_FAILURE;
    XRRScreenResources *res;
    XRROutputInfo *output_info;

    res = XRRGetScreenResources(dpy, DefaultRootWindow(dpy));
    output_info = find_output_xrandr(dpy, output_name);

    /* crtc holds our screen info, need to compare to actual screen size */
    if (output_info)
    {
        XRRCrtcInfo *crtc_info;
        Matrix m;
        matrix_set_unity(&m);
        crtc_info = XRRGetCrtcInfo (dpy, res, output_info->crtc);
        set_transformation_matrix(dpy, &m, crtc_info->x, crtc_info->y,
                                  crtc_info->width, crtc_info->height,
                                  crtc_info->rotation);
        rc = apply_matrix(dpy, deviceid, &m);
        XRRFreeCrtcInfo(crtc_info);
        XRRFreeOutputInfo(output_info);
    } else
        printf("Unable to find output '%s'. "
                "Output may not be connected.\n", output_name);

    XRRFreeScreenResources(res);

    return rc;
}

static int
map_output_xinerama(Display *dpy, int deviceid, const char *output_name)
{
    const char *prefix = "HEAD-";
    XineramaScreenInfo *screens = NULL;
    int rc = EXIT_FAILURE;
    int event, error;
    int nscreens;
    int head;
    Matrix m;

    if (!XineramaQueryExtension(dpy, &event, &error))
    {
        fprintf(stderr, "Unable to set screen mapping. Xinerama extension not found\n");
        goto out;
    }

    if (strlen(output_name) < strlen(prefix) + 1 ||
        strncmp(output_name, prefix, strlen(prefix)) != 0)
    {
        fprintf(stderr, "Please specify the output name as HEAD-X,"
                "where X is the screen number\n");
        goto out;
    }

    head = output_name[strlen(prefix)] - '0';

    screens = XineramaQueryScreens(dpy, &nscreens);

    if (nscreens == 0)
    {
        fprintf(stderr, "Xinerama failed to query screens.\n");
        goto out;
    } else if (nscreens <= head)
    {
        fprintf(stderr, "Found %d screens, but you requested %s.\n",
                nscreens, output_name);
        goto out;
    }

    matrix_set_unity(&m);
    set_transformation_matrix(dpy, &m,
                              screens[head].x_org, screens[head].y_org,
                              screens[head].width, screens[head].height,
                              RR_Rotate_0);
    rc = apply_matrix(dpy, deviceid, &m);

out:
    XFree(screens);
    return rc;
}

int
map_to_output(Display *dpy, int argc, char *argv[], char *name, char *desc)
{
    char *output_name;
    XIDeviceInfo *info;
    XRROutputInfo *output_info;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: xinput %s %s\n", name, desc);
        return EXIT_FAILURE;
    }

    info = xi2_find_device_info(dpy, argv[0]);
    if (!info)
    {
        fprintf(stderr, "unable to find device '%s'\n", argv[0]);
        return EXIT_FAILURE;
    }

    output_name = argv[1];
    output_info = find_output_xrandr(dpy, output_name);
    if (!output_info)
    {
        /* Output doesn't exist. Is this a (partial) non-RandR setup?  */
        output_info = find_output_xrandr(dpy, "default");
        if (output_info)
        {
            XRRFreeOutputInfo(output_info);
            if (strncmp("HEAD-", output_name, strlen("HEAD-")) == 0)
                return map_output_xinerama(dpy, info->deviceid, output_name);
        }
    } else
        XRRFreeOutputInfo(output_info);

    return map_output_xrandr(dpy, info->deviceid, output_name);
}
