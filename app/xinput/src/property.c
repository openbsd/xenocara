/*
 * Copyright 2007 Peter Hutterer
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the author shall
 * not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization
 * from the author.
 *
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xatom.h>
#include <X11/extensions/XIproto.h>

#include "xinput.h"

static void
print_property(Display *dpy, XDevice* dev, Atom property)
{
    Atom                act_type;
    char                *name;
    int                 act_format;
    unsigned long       nitems, bytes_after;
    unsigned char       *data, *ptr;
    int                 j, done = False, size;

    name = XGetAtomName(dpy, property);
    printf("\t%s (%ld):\t", name, property);

    if (XGetDeviceProperty(dpy, dev, property, 0, 1000, False,
                           AnyPropertyType, &act_type, &act_format,
                           &nitems, &bytes_after, &data) == Success)
    {
        Atom float_atom = XInternAtom(dpy, "FLOAT", True);

        ptr = data;

        switch(act_format)
        {
            case 8: size = sizeof(char); break;
            case 16: size = sizeof(short); break;
            case 32: size = sizeof(long); break;
        }

        for (j = 0; j < nitems; j++)
        {
            switch(act_type)
            {
                case XA_INTEGER:
                    switch(act_format)
                    {
                        case 8:
                            printf("%d", *((char*)ptr));
                            break;
                        case 16:
                            printf("%d", *((short*)ptr));
                            break;
                        case 32:
                            printf("%ld", *((long*)ptr));
                            break;
                    }
                    break;
                case XA_STRING:
                    if (act_format != 8)
                    {
                        printf("Unknown string format.\n");
                        done = True;
                        break;
                    }
                    printf("\"%s\"", ptr);
                    j += strlen((char*)ptr); /* The loop's j++ jumps over the
                                                terminating 0 */
                    ptr += strlen((char*)ptr); /* ptr += size below jumps over
                                                  the terminating 0 */
                    break;
                case XA_ATOM:
                    printf("\"%s\"", XGetAtomName(dpy, *(Atom*)ptr));
                    break;
                default:
                    if (float_atom != None && act_type == float_atom)
                    {
                        printf("%f", *((float*)ptr));
                        break;
                    }

                    printf("\t... of unknown type %s\n",
                            XGetAtomName(dpy, act_type));
                    done = True;
                    break;
            }

            ptr += size;

            if (j < nitems - 1)
                printf(", ");
            if (done == True)
                break;
        }
        printf("\n");
        XFree(data);
    } else
        printf("\tFetch failure\n");

}

int list_props(Display *dpy, int argc, char** argv, char* name, char *desc)
{
    XDeviceInfo *info;
    XDevice     *dev;
    int          i;
    int         nprops;
    Atom        *props;

    if (argc == 0)
    {
        fprintf(stderr, "Usage: xinput %s %s\n", name, desc);
        return EXIT_FAILURE;
    }

    for (i = 0; i < argc; i++)
    {
        info = find_device_info(dpy, argv[i], False);
        if (!info)
        {
            fprintf(stderr, "unable to find device %s\n", argv[i]);
            continue;
        }

        dev = XOpenDevice(dpy, info->id);
        if (!dev)
        {
            fprintf(stderr, "unable to open device '%s'\n", info->name);
            continue;
        }

        props = XListDeviceProperties(dpy, dev, &nprops);
        if (!nprops)
        {
            printf("Device '%s' does not report any properties.\n", info->name);
            continue;
        }

        printf("Device '%s':\n", info->name);
        while(nprops--)
        {
            print_property(dpy, dev, props[nprops]);
        }

        XFree(props);
        XCloseDevice(dpy, dev);
    }
    return EXIT_SUCCESS;
}

int
set_int_prop(Display *dpy, int argc, char** argv, char* n, char *desc)
{
    XDeviceInfo *info;
    XDevice     *dev;
    Atom         prop;
    char        *name;
    int          i;
    Bool         is_atom = True;
    char        *data;
    int          format, nelements =  0;

    if (argc < 3)
    {
        fprintf(stderr, "Usage: xinput %s %s\n", n, desc);
        return EXIT_FAILURE;
    }

    info = find_device_info(dpy, argv[0], False);
    if (!info)
    {
        fprintf(stderr, "unable to find device %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    dev = XOpenDevice(dpy, info->id);
    if (!dev)
    {
        fprintf(stderr, "unable to open device %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    name = argv[1];

    for(i = 0; i < strlen(name); i++) {
	if (!isdigit(name[i])) {
            is_atom = False;
	    break;
	}
    }

    if (!is_atom)
        prop = XInternAtom(dpy, name, False);
    else
        prop = atoi(name);

    nelements = argc - 3;
    format    = atoi(argv[2]);
    if (format != 8 && format != 16 && format != 32)
    {
        fprintf(stderr, "Invalid format %d\n", format);
        return EXIT_FAILURE;
    }

    data = calloc(nelements, sizeof(long));
    for (i = 0; i < nelements; i++)
    {
        switch(format)
        {
            case 8:
                *(((char*)data) + i) = atoi(argv[3 + i]);
                break;
            case 16:
                *(((short*)data) + i) = atoi(argv[3 + i]);
                break;
            case 32:
                *(((long*)data) + i) = atoi(argv[3 + i]);
                break;
        }
    }

    XChangeDeviceProperty(dpy, dev, prop, XA_INTEGER, format, PropModeReplace,
                          (unsigned char*)data, nelements);

    free(data);
    XCloseDevice(dpy, dev);
    return EXIT_SUCCESS;
}

int
set_float_prop(Display *dpy, int argc, char** argv, char* n, char *desc)
{
    XDeviceInfo *info;
    XDevice     *dev;
    Atom         prop, float_atom;
    char        *name;
    int          i;
    Bool         is_atom = True;
    long        *data;
    int          nelements =  0;
    char*        endptr;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: xinput %s %s\n", n, desc);
        return EXIT_FAILURE;
    }

    info = find_device_info(dpy, argv[0], False);
    if (!info)
    {
        fprintf(stderr, "unable to find device %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    dev = XOpenDevice(dpy, info->id);
    if (!dev)
    {
        fprintf(stderr, "unable to open device %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    name = argv[1];

    for(i = 0; i < strlen(name); i++) {
	if (!isdigit(name[i])) {
            is_atom = False;
	    break;
	}
    }

    if (!is_atom)
        prop = XInternAtom(dpy, name, False);
    else
        prop = atoi(name);

    nelements = argc - 2;

    float_atom = XInternAtom(dpy, "FLOAT", False);

    if (float_atom == (Atom)0)
    {
	fprintf(stderr, "no FLOAT atom present in server\n");
	return EXIT_FAILURE;
    }

    if (sizeof(float) != 4)
    {
	fprintf(stderr, "sane FP required\n");
	return EXIT_FAILURE;
    }

    data = calloc(nelements, sizeof(long));
    for (i = 0; i < nelements; i++)
    {
        *((float*)(data + i)) = strtod(argv[2 + i], &endptr);
	if(endptr == argv[2 + i]){
	    fprintf(stderr, "argument %s could not be parsed\n", argv[2 + i]);
	    return EXIT_FAILURE;
	}
    }

    XChangeDeviceProperty(dpy, dev, prop, float_atom, 32, PropModeReplace,
                          (unsigned char*)data, nelements);

    free(data);
    XCloseDevice(dpy, dev);
    return EXIT_SUCCESS;
}


int watch_props(Display *dpy, int argc, char** argv, char* n, char *desc)
{
    XDevice     *dev;
    XDeviceInfo *info;
    XEvent      ev;
    XDevicePropertyNotifyEvent *dpev;
    char        *name;
    int         type_prop;
    XEventClass cls_prop;

    if (list_props(dpy, argc, argv, n, desc) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    info = find_device_info(dpy, argv[0], False);
    if (!info)
    {
        fprintf(stderr, "unable to find device %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    dev = XOpenDevice(dpy, info->id);
    if (!dev)
    {
        fprintf(stderr, "unable to open device '%s'\n", info->name);
        return EXIT_FAILURE;
    }

    DevicePropertyNotify(dev, type_prop, cls_prop);
    XSelectExtensionEvent(dpy, DefaultRootWindow(dpy), &cls_prop, 1);

    while(1)
    {
        XNextEvent(dpy, &ev);

        dpev = (XDevicePropertyNotifyEvent*)&ev;
        if (dpev->type != type_prop)
            continue;

        name = XGetAtomName(dpy, dpev->atom);
        printf("Property '%s' changed.\n", name);
        print_property(dpy, dev, dpev->atom);
    }

    XCloseDevice(dpy, dev);
}

int delete_prop(Display *dpy, int argc, char** argv, char* n, char *desc)
{
    XDevice     *dev;
    XDeviceInfo *info;
    char        *name;
    int         i;
    Bool        is_atom = True;
    Atom        prop;

    info = find_device_info(dpy, argv[0], False);
    if (!info)
    {
        fprintf(stderr, "unable to find device %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    dev = XOpenDevice(dpy, info->id);
    if (!dev)
    {
        fprintf(stderr, "unable to open device '%s'\n", info->name);
        return EXIT_FAILURE;
    }

    name = argv[1];

    for(i = 0; i < strlen(name); i++) {
	if (!isdigit(name[i])) {
            is_atom = False;
	    break;
	}
    }

    if (!is_atom)
        prop = XInternAtom(dpy, name, False);
    else
        prop = atoi(name);

    XDeleteDeviceProperty(dpy, dev, prop);

    XCloseDevice(dpy, dev);
    return EXIT_SUCCESS;
}

int
set_atom_prop(Display *dpy, int argc, char** argv, char* n, char *desc)
{
    XDeviceInfo *info;
    XDevice     *dev;
    Atom         prop;
    char        *name;
    int          i, j;
    Bool         is_atom = True;
    Atom        *data;
    int          nelements =  0;

    if (argc < 3)
    {
        fprintf(stderr, "Usage: xinput %s %s\n", n, desc);
        return EXIT_FAILURE;
    }

    info = find_device_info(dpy, argv[0], False);
    if (!info)
    {
        fprintf(stderr, "unable to find device %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    dev = XOpenDevice(dpy, info->id);
    if (!dev)
    {
        fprintf(stderr, "unable to open device %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    name = argv[1];

    for(i = 0; i < strlen(name); i++) {
	if (!isdigit(name[i])) {
            is_atom = False;
	    break;
	}
    }

    if (!is_atom)
        prop = XInternAtom(dpy, name, False);
    else
        prop = atoi(name);

    nelements = argc - 2;
    data = calloc(nelements, sizeof(Atom));
    for (i = 0; i < nelements; i++)
    {
        is_atom = True;
        name = argv[2 + i];
        for(j = 0; j < strlen(name); j++) {
            if (!isdigit(name[j])) {
                is_atom = False;
                break;
            }
        }

        if (!is_atom)
            data[i] = XInternAtom(dpy, name, False);
        else
        {
            data[i] = atoi(name);
            XFree(XGetAtomName(dpy, data[i]));
        }
    }

    XChangeDeviceProperty(dpy, dev, prop, XA_ATOM, 32, PropModeReplace,
                          (unsigned char*)data, nelements);

    free(data);
    XCloseDevice(dpy, dev);
    return EXIT_SUCCESS;
}


