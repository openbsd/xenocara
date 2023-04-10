/*
 * Copyright 2007-2008 by Sascha Hlusiak. <saschahlusiak@freedesktop.org>     
 *                                                                            
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is  hereby granted without fee, provided that
 * the  above copyright   notice appear  in   all  copies and  that both  that
 * copyright  notice   and   this  permission   notice  appear  in  supporting
 * documentation, and that   the  name of  Sascha   Hlusiak  not  be  used  in
 * advertising or publicity pertaining to distribution of the software without
 * specific,  written      prior  permission.     Sascha   Hlusiak   makes  no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.                   
 *                                                                            
 * SASCHA  HLUSIAK  DISCLAIMS ALL   WARRANTIES WITH REGARD  TO  THIS SOFTWARE,
 * INCLUDING ALL IMPLIED   WARRANTIES OF MERCHANTABILITY  AND   FITNESS, IN NO
 * EVENT  SHALL SASCHA  HLUSIAK  BE   LIABLE   FOR ANY  SPECIAL, INDIRECT   OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA  OR PROFITS, WHETHER  IN  AN ACTION OF  CONTRACT,  NEGLIGENCE OR OTHER
 * TORTIOUS  ACTION, ARISING    OUT OF OR   IN  CONNECTION  WITH THE USE    OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

/**
 * This provides the backend for Linux evdev devices.
 * Devices are usually /dev/input/event?
 **/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <linux/input.h>

#include <xf86.h>
#include <xf86_OSproc.h>

#include "jstk.h"
#include "backend_evdev.h"


#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array) ((array[LONG(bit)] >> OFF(bit)) & 1)


struct jstk_evdev_data {
    struct jstk_evdev_axis_data {
        int number;
        int min, max;
    } axis[ABS_MAX];
    int key[KEY_MAX];
};


static void jstkCloseDevice_evdev(JoystickDevPtr joystick);
static int jstkReadData_evdev(JoystickDevPtr joystick,
                              JOYSTICKEVENT *event,
                              int *number);

/***********************************************************************
 *
 * jstkOpenDevice --
 *
 * Open and initialize a joystick device. The device name is
 * taken from JoystickDevPtr 
 * Returns the filedescriptor, or -1 in case of error
 *
 ***********************************************************************
 */

int
jstkOpenDevice_evdev(JoystickDevPtr joystick, Bool probe)
{
    int driver_version;
    char name[256];
    char uniq[256];
    struct input_id id;
    struct jstk_evdev_data *evdevdata;
    unsigned long button_bits[NBITS(ABS_MAX)];
    unsigned long key_bits[NBITS(KEY_MAX)];
    int axes, buttons, j;

    if (joystick->fd == -1) {
        if ((joystick->fd = open(joystick->device, O_RDONLY | O_NDELAY, 0)) < 0) {
            xf86Msg(X_ERROR, "Cannot open joystick '%s' (%s)\n",
                    joystick->device, strerror(errno));
            return -1;
        }
    }

    if (ioctl(joystick->fd, EVIOCGVERSION, &driver_version) == -1) {
        xf86Msg(X_ERROR, "Joystick: ioctl EVIOCGVERSION on '%s' failed: %s\n", 
                joystick->device, strerror(errno));
        jstkCloseDevice_evdev(joystick);
        return -1;
    }

    if (ioctl(joystick->fd, EVIOCGID, &id) == -1) {
        xf86Msg(X_ERROR, "Joystick: ioctl EVIOCGID on '%s' failed: %s\n",
                joystick->device, strerror(errno));
        jstkCloseDevice_evdev(joystick);
        return -1;
    }


    memset(button_bits, 0, sizeof(button_bits));
    if (ioctl(joystick->fd, EVIOCGBIT(EV_ABS, ABS_MAX), button_bits) == -1)
    {
        xf86Msg(X_ERROR, "Joystick: ioctl EVIOCGBIT on '%s' failed: %s\n",
                joystick->device, strerror(errno));
        jstkCloseDevice_evdev(joystick);
        return -1;
    }

    evdevdata = (struct jstk_evdev_data*)
                malloc(sizeof(struct jstk_evdev_data));
    joystick->devicedata = (void*) evdevdata;

    for (axes=0; axes<ABS_MAX; axes++)
    {
        evdevdata->axis[axes].number = -1;
        evdevdata->axis[axes].min = 0;
        evdevdata->axis[axes].max = 1;
    }
    for (buttons=0; buttons<KEY_MAX; buttons++)
    {
        evdevdata->key[buttons] = -1;
    }

    axes = 0; /* Our logical index */
    for (j = 0; j < ABS_MAX; j++)
        if (test_bit(j, button_bits))
        {
            struct input_absinfo absinfo;
            if (ioctl(joystick->fd, EVIOCGABS(j), &absinfo) == -1) {
                xf86Msg(X_ERROR, "Joystick: ioctl EVIOCGABS on '%s' failed: %s\n",
                        joystick->device, strerror(errno));
                jstkCloseDevice_evdev(joystick);
                return -1;
            }
            evdevdata->axis[j].number = axes; /* physical -> logical mapping */
            evdevdata->axis[j].min = absinfo.minimum;
            evdevdata->axis[j].max = absinfo.maximum;
            DBG(3, ErrorF("Axis %d: phys %d min %d max %d\n",
               axes, j, absinfo.minimum, absinfo.maximum));

            axes++;
        }

    memset(key_bits, 0, sizeof(key_bits));
    if (ioctl(joystick->fd, EVIOCGBIT(EV_KEY, KEY_MAX), key_bits) == -1)
    {
        xf86Msg(X_ERROR, "Joystick: ioctl EVIOCGBIT on '%s' failed: %s\n",
                joystick->device, strerror(errno));
        jstkCloseDevice_evdev(joystick);
        return -1;
    }
    buttons = 0; /* Our logical index */
    for (j = 0; j < KEY_MAX; j++)
        if (test_bit(j, key_bits))
        {
            evdevdata->key[j] = buttons;
            DBG(3, ErrorF("Button %d: phys %d\n", buttons, j));
            buttons++;
        }

    if (ioctl(joystick->fd, EVIOCGNAME(sizeof(name)), name) == -1)
        strcpy(name, "No name");

    if (ioctl(joystick->fd, EVIOCGUNIQ(sizeof(uniq)), uniq) == -1)
        strcpy(uniq, "No name");

    if (probe == TRUE) {
        xf86Msg(X_INFO, "Joystick: %s. bus 0x%x vendor 0x%x product 0x%x version 0x%x\n",
	    name, id.bustype, id.vendor, id.product, id.version);
        xf86Msg(X_INFO, "Joystick: found %d axes, %d buttons\n", axes, buttons);
    }

    joystick->open_proc = jstkOpenDevice_evdev;
    joystick->read_proc = jstkReadData_evdev;
    joystick->close_proc = jstkCloseDevice_evdev;

    if (buttons > MAXBUTTONS)
        buttons = MAXBUTTONS;
    if (axes > MAXAXES)
        axes = MAXAXES;
    joystick->num_buttons = buttons;
    joystick->num_axes = axes;
    return joystick->fd;
}


/***********************************************************************
 *
 * jstkCloseDevice --
 *
 * close the handle.
 *
 ***********************************************************************
 */

static void
jstkCloseDevice_evdev(JoystickDevPtr joystick)
{
    jstkCloseDevice(joystick);
    if (joystick->devicedata) {
        free(joystick->devicedata);
        joystick->devicedata = NULL;
    }
}


/***********************************************************************
 *
 * jstkReadData --
 *
 * Reads data from fd and stores it in the JoystickDevRec struct
 * fills in the type of event and the number of the button/axis
 * return 1 if success, 0 otherwise. Success does not necessarily
 * mean that there is a new event waiting.
 *
 ***********************************************************************
 */

static int
jstkReadData_evdev(JoystickDevPtr joystick,
                   JOYSTICKEVENT *event,
                   int *number)
{
    struct input_event iev;
    signed int value;
    struct jstk_evdev_data *data;
    if (event != NULL) *event = EVENT_NONE;
    if (xf86ReadSerial(joystick->fd, &iev, sizeof(struct input_event)) !=
        sizeof(struct input_event))
        return 0;
    if (!joystick->devicedata) return 0; /* we are supposed to have devicedata */
    data = (struct jstk_evdev_data*)joystick->devicedata;

    DBG(10, if (iev.type)
        ErrorF("Event (evdev): type: 0x%04X, code: 0x%04X, value: 0x%04X\n", 
               (unsigned int)iev.type,
               (unsigned int)iev.code,
               (int)iev.value));

    switch(iev.type) {
    case EV_KEY:
        if ((data->key[iev.code] >= 0) &&
            (data->key[iev.code] < MAXBUTTONS))
        {
            if (joystick->button[data->key[iev.code]].pressed != iev.value)
            {
                joystick->button[data->key[iev.code]].pressed = iev.value;
                if (event != NULL) *event = EVENT_BUTTON;
                if (number != NULL) *number = data->key[iev.code];
            }
        }
        break;
    case EV_ABS:
        if (iev.code < ABS_MAX) {
            struct jstk_evdev_axis_data *axis;
            axis = &data->axis[iev.code];
            if ((axis->number >= 0) && (axis->number < MAXAXES)) {
		/* NOTE: controllers report totally different ranges:
		 * - 0..256, with a center of 127 (Logitech Dual Action axes)
		 * - 0..256 with a center of 0 (XBox left/right triggers)
		 * - -32768..32768 with a center of 0 (XBox axes)
		 * 
		 * These ranges will ALL be scaled to -32768..32768, with
		 * the center value to be assumed 0. This is for compatibility
		 * with the legacy joystick module, which reports values in
		 * the same range.
		 * 
		 * The value is also important for the deadzone, which can be 
		 * configured by the user and is in -32768..32768 range.
		 * 
		 * TODO: how to respect center value, so that that XBox triggers
		 *       and logitech axes report idle, when not moved?
		 * TODO: report all values as -1.0f..1.0f, but this would possibly
		 *       break config file semantics.
		 */
		    
		float v = (float) iev.value;
		v = (v - (float)axis->min) * 65535.0f
			/ (axis->max - axis->min) - 32768.0f;
		value = (int) v;

                if (abs(value) < joystick->axis[axis->number].deadzone) {
                    /* We only want one event when in deadzone */
                    if (joystick->axis[axis->number].value != 0) {
                        joystick->axis[axis->number].value = 0;
                        if (event != NULL) *event = EVENT_AXIS;
                        if (number != NULL) *number = axis->number;
                    }
                }else{
                    joystick->axis[axis->number].value = value;
                    if (event != NULL) *event = EVENT_AXIS;
                    if (number != NULL) *number = axis->number;
                }
            }
        }
        break;
    default:
        DBG(3, if (iev.type)
                   ErrorF("Unhandled evdev event: type: 0x%04X, code: 0x%04X, "
                          "value: 0x%04X\n", 
                  (unsigned int)iev.type, 
                  (unsigned int)iev.code,
                  (int)iev.value));
    }
    return 1;
}
