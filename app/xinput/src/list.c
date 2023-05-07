/*
 * Copyright 1996 by Frederic Lepied, France. <Frederic.Lepied@sugix.frmug.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is  hereby granted without fee, provided that
 * the  above copyright   notice appear  in   all  copies and  that both  that
 * copyright  notice   and   this  permission   notice  appear  in  supporting
 * documentation, and that   the  name of  the authors  not  be  used  in
 * advertising or publicity pertaining to distribution of the software without
 * specific,  written      prior  permission.     The authors  make  no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * THE AUTHORS DISCLAIM ALL   WARRANTIES WITH REGARD  TO  THIS SOFTWARE,
 * INCLUDING ALL IMPLIED   WARRANTIES OF MERCHANTABILITY  AND   FITNESS, IN NO
 * EVENT  SHALL THE AUTHORS  BE   LIABLE   FOR ANY  SPECIAL, INDIRECT   OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA  OR PROFITS, WHETHER  IN  AN ACTION OF  CONTRACT,  NEGLIGENCE OR OTHER
 * TORTIOUS  ACTION, ARISING    OUT OF OR   IN  CONNECTION  WITH THE USE    OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "xinput.h"
#include <string.h>

static Bool have_utf8(void);

enum print_format {
    FORMAT_NONE,
    FORMAT_SHORT,
    FORMAT_LONG,
    FORMAT_NAME,
    FORMAT_ID,
};


static void
print_info(Display* dpy, XDeviceInfo	*info, enum print_format format)
{
    int			i,j;
    XAnyClassPtr	any;
    XKeyInfoPtr		k;
    XButtonInfoPtr	b;
    XValuatorInfoPtr	v;
    XAxisInfoPtr	a;

    if (format == FORMAT_NAME)
    {
        printf("%s\n", info->name);
        return;
    } else if (format == FORMAT_ID)
    {
        printf("%ld\n", info->id);
        return;
    }

    printf("\"%s\"\tid=%ld\t[", info->name, info->id);

    switch (info->use) {
    case IsXPointer:
       printf("XPointer");
       break;
    case IsXKeyboard:
       printf("XKeyboard");
       break;
    case IsXExtensionDevice:
       printf("XExtensionDevice");
       break;
    case IsXExtensionKeyboard:
       printf("XExtensionKeyboard");
       break;
    case IsXExtensionPointer:
       printf("XExtensionPointer");
       break;
    default:
       printf("Unknown class");
       break;
    }
    printf("]\n");

    if (format == FORMAT_SHORT)
        return;

    if (info->type != None) {
	char *type = XGetAtomName(dpy, info->type);
	printf("\tType is %s\n", type);
	XFree(type);
    }

    if (info->num_classes > 0) {
	any = (XAnyClassPtr) (info->inputclassinfo);
	for (i=0; i<info->num_classes; i++) {
	    switch (any->class) {
	    case KeyClass:
		k = (XKeyInfoPtr) any;
		printf("\tNum_keys is %d\n", k->num_keys);
		printf("\tMin_keycode is %d\n", k->min_keycode);
		printf("\tMax_keycode is %d\n", k->max_keycode);
		break;

	    case ButtonClass:
		b = (XButtonInfoPtr) any;
		printf("\tNum_buttons is %d\n", b->num_buttons);
		break;

	    case ValuatorClass:
		v = (XValuatorInfoPtr) any;
		a = (XAxisInfoPtr) ((char *) v +
				    sizeof (XValuatorInfo));
		printf("\tNum_axes is %d\n", v->num_axes);
		printf("\tMode is %s\n", (v->mode == Absolute) ? "Absolute" : "Relative");
		printf("\tMotion_buffer is %ld\n", v->motion_buffer);
		for (j=0; j<v->num_axes; j++, a++) {
		    printf("\tAxis %d :\n", j);
		    printf("\t\tMin_value is %d\n", a->min_value);
		    printf("\t\tMax_value is %d\n", a->max_value);
		    printf ("\t\tResolution is %d\n", a->resolution);
		}
		break;
	    default:
		printf ("unknown class\n");
	    }
	    any = (XAnyClassPtr) ((char *) any + any->length);
	}
    }
}

static int list_xi1(Display     *display,
                    enum print_format format)
{
    XDeviceInfo		*info;
    int			loop;
    int                 num_devices;

    info = XListInputDevices(display, &num_devices);
    for(loop=0; loop<num_devices; loop++) {
        print_info(display, info+loop, format);
    }
    return EXIT_SUCCESS;
}

#if HAVE_XI2
/* also used from test_xi2.c */
void
print_classes_xi2(Display* display, XIAnyClassInfo **classes,
                  int num_classes)
{
    int i, j;

    printf("\tReporting %d classes:\n", num_classes);
    for (i = 0; i < num_classes; i++)
    {
        printf("\t\tClass originated from: %d. Type: ", classes[i]->sourceid);
        switch(classes[i]->type)
        {
            case XIButtonClass:
                {
                    XIButtonClassInfo *b = (XIButtonClassInfo*)classes[i];
                    char *name;
                    printf("XIButtonClass\n");
                    printf("\t\tButtons supported: %d\n", b->num_buttons);
                    printf("\t\tButton labels:");
                    for (j = 0; j < b->num_buttons; j++)
                    {
                        name = (b->labels[j]) ? XGetAtomName(display, b->labels[j]) : NULL;
                        if (name)
                            printf(" \"%s\"", name);
                        else
                            printf(" None");
                        XFree(name);
                    }
                    printf("\n");
                    printf("\t\tButton state:");
                    for (j = 0; j < b->state.mask_len * 8; j++)
                        if (XIMaskIsSet(b->state.mask, j))
                            printf(" %d", j);
                    printf("\n");

                }
                break;
            case XIKeyClass:
                {
                    XIKeyClassInfo *k = (XIKeyClassInfo*)classes[i];
                    printf("XIKeyClass\n");
                    printf("\t\tKeycodes supported: %d\n", k->num_keycodes);
                }
                break;
            case XIValuatorClass:
                {
                    XIValuatorClassInfo *v = (XIValuatorClassInfo*)classes[i];
                    char *name = v->label ?  XGetAtomName(display, v->label) : NULL;

                    /* Bug in X servers 1.7..1.8.1, mode was | OutOfProximity */
                    v->mode &= DeviceMode;

                    printf("XIValuatorClass\n");
                    printf("\t\tDetail for Valuator %d:\n", v->number);
                    printf("\t\t  Label: %s\n",  (name) ? name : "None");
                    printf("\t\t  Range: %f - %f\n", v->min, v->max);
                    printf("\t\t  Resolution: %d units/m\n", v->resolution);
                    printf("\t\t  Mode: %s\n", v->mode == Absolute ? "absolute" :
                            "relative");
                    if (v->mode == Absolute)
                        printf("\t\t  Current value: %f\n", v->value);
                    XFree(name);
                }
                break;
#if HAVE_XI21
            case XIScrollClass:
                {
                    XIScrollClassInfo *s = (XIScrollClassInfo*)classes[i];

                    printf("XIScrollClass\n");
                    printf("\t\tScroll info for Valuator %d\n", s->number);
                    printf("\t\t  type: %d (%s)\n", s->scroll_type,
                           (s->scroll_type == XIScrollTypeHorizontal) ? "horizontal" :
                              (s->scroll_type == XIScrollTypeVertical) ? "vertical" : "unknown");
                    printf("\t\t  increment: %f\n", s->increment);
                    printf("\t\t  flags: 0x%x", s->flags);
                    if (s->flags) {
                        printf(" (");
                        if (s->flags & XIScrollFlagNoEmulation)
                            printf(" no-emulation ");
                        if (s->flags & XIScrollFlagPreferred)
                            printf(" preferred ");
                        printf(")");
                    }
                    printf("\n");
                }
                break;
#endif
#if HAVE_XI22
            case XITouchClass:
                {
                    XITouchClassInfo *t = (XITouchClassInfo*)classes[i];

                    printf("XITouchClass\n");
                    printf("\t\tTouch mode: %s\n",
                           (t->mode == XIDirectTouch) ? "direct" : "dependent");
                    printf("\t\tMax number of touches: %d\n", t->num_touches);
                }
#endif
        }
    }

    printf("\n");
}

static void
print_info_xi2(Display* display, XIDeviceInfo *dev, enum print_format format)
{
    if (format == FORMAT_NAME)
    {
        printf("%s\n", dev->name);
        return;
    } else if (format == FORMAT_ID)
    {
        printf("%d\n", dev->deviceid);
        return;
    }

    printf("%-40s\tid=%d\t[", dev->name, dev->deviceid);
    switch(dev->use)
    {
        case XIMasterPointer:
            printf("master pointer  (%d)]\n", dev->attachment);
            break;
        case XIMasterKeyboard:
            printf("master keyboard (%d)]\n", dev->attachment);
            break;
        case XISlavePointer:
            printf("slave  pointer  (%d)]\n", dev->attachment);
            break;
        case XISlaveKeyboard:
            printf("slave  keyboard (%d)]\n", dev->attachment);
            break;
        case XIFloatingSlave:
            printf("floating slave]\n");
            break;
    }

    if (format == FORMAT_SHORT)
        return;

    if (!dev->enabled)
        printf("\tThis device is disabled\n");

    print_classes_xi2(display, dev->classes, dev->num_classes);
}


static int
list_xi2(Display *display,
         enum print_format format)
{
    int ndevices;
    int i, j;
    XIDeviceInfo *info, *dev;

    info = XIQueryDevice(display, XIAllDevices, &ndevices);

    for(i = 0; i < ndevices; i++)
    {
        dev = &info[i];
        if (dev->use == XIMasterPointer || dev->use == XIMasterKeyboard)
        {
            if (format == FORMAT_SHORT || format == FORMAT_LONG)
            {
                if (have_utf8())
                    if (dev->use == XIMasterPointer)
                        printf("⎡ ");
                    else
                        printf("⎣ ");
                else
                    printf("+ ");
            }
   
            print_info_xi2(display, dev, format);
            for (j = 0; j < ndevices; j++)
            {
                XIDeviceInfo* sd = &info[j];

                if ((sd->use == XISlavePointer || sd->use == XISlaveKeyboard) &&
                     (sd->attachment == dev->deviceid))
                {
                    if (format == FORMAT_SHORT || format == FORMAT_LONG)
                    {
                        if (have_utf8())
                            printf("%s   ↳ ", dev->use == XIMasterPointer ? "⎜" : " ");
                        else
                            printf("%s   + ", dev->use == XIMasterPointer ? "|" : " ");
                    }
                    print_info_xi2(display, sd, format);
                }
            }
        }
    }

    for (i = 0; i < ndevices; i++)
    {
        dev = &info[i];
        if (dev->use == XIFloatingSlave)
        {
            printf("∼ ");
            print_info_xi2(display, dev, format);
        }
    }


    XIFreeDeviceInfo(info);
    return EXIT_SUCCESS;
}
#endif

int
list(Display	*display,
     int	argc,
     char	*argv[],
     char	*name,
     char	*desc)
{
    enum print_format format = FORMAT_NONE;
    int arg_dev = 1;

    if (argc >= 1)
    {
        if (strcmp(argv[0], "--short") == 0)
            format = FORMAT_SHORT;
        else if (strcmp(argv[0], "--long") == 0)
            format = FORMAT_LONG;
        else if (strcmp(argv[0], "--name-only") == 0)
            format = FORMAT_NAME;
        else if (strcmp(argv[0], "--id-only") == 0)
            format = FORMAT_ID;
        else
            arg_dev--;
    }

    if (argc > arg_dev)
    {
        if (format == FORMAT_NONE)
            format = FORMAT_LONG;
#if HAVE_XI2
        if (xinput_version(display) == XI_2_Major)
        {
            XIDeviceInfo *info = xi2_find_device_info(display, argv[arg_dev]);

            if (!info) {
                fprintf(stderr, "unable to find device %s\n", argv[arg_dev]);
                return EXIT_FAILURE;
            } else {
                print_info_xi2(display, info, format);
                return EXIT_SUCCESS;
            }
        } else
#endif
        {
            XDeviceInfo *info = find_device_info(display, argv[arg_dev], False);

            if (!info) {
                fprintf(stderr, "unable to find device %s\n", argv[arg_dev]);
                return EXIT_FAILURE;
            } else {
                print_info(display, info, format);
                return EXIT_SUCCESS;
            }
        }
    } else {
        if (format == FORMAT_NONE)
            format = FORMAT_SHORT;
#if HAVE_XI2
        if (xinput_version(display) == XI_2_Major)
            return list_xi2(display, format);
#endif
        return list_xi1(display, format);
    }
}

static Bool
have_utf8(void){
    char *t;

    /*
     * There is no standard way to detect UTF-8 capabilities of a
     * given terminal, but this gets pretty close as a good heuristic.
     */
    if ((t = getenv("LC_ALL")) == NULL) {
	if ((t = getenv("LC_CTYPE")) == NULL) {
	    t = getenv("LANG");
	}
    }

    if (t != NULL && (strcasestr(t, "UTF-8") != NULL ||
      strcasestr(t, "UTF8") != NULL))
	return True;

    return False;
}

/* end of list.c */
