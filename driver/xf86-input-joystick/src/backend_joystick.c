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
 * This provides the backend for Linux joystick devices.
 * Usable in FreeBSD with the linux_js module.
 * Devices are usually /dev/input/js?
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
#include <linux/joystick.h>

#include <xf86.h>
#include <xf86_OSproc.h>

#include "jstk.h"
#include "backend_joystick.h"


static int jstkReadData_joystick(JoystickDevPtr joystick,
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
jstkOpenDevice_joystick(JoystickDevPtr joystick, Bool probe)
{
    char joy_name[128];
    unsigned char axes, buttons;
    int driver_version;

    if (joystick->fd == -1) {
        if ((joystick->fd = open(joystick->device, O_RDONLY | O_NDELAY, 0)) < 0) {
            xf86Msg(X_ERROR, "Cannot open joystick '%s' (%s)\n",
                    joystick->device, strerror(errno));
            return -1;
        }
    }

    if (ioctl(joystick->fd, JSIOCGVERSION, &driver_version) == -1) {
        xf86Msg(X_ERROR, "Joystick: ioctl JSIOCGVERSION on '%s' failed: %s\n", 
                joystick->device, strerror(errno));
        jstkCloseDevice(joystick);
        return -1;
    }
    if ((driver_version >> 16) < 1) {
        xf86Msg(X_WARNING, "Joystick: Driver version is only %d.%d.%d\n",
                driver_version >> 16,
                (driver_version >> 8) & 0xff,
                driver_version & 0xff);
    }

    if (ioctl(joystick->fd, JSIOCGAXES, &axes) == -1) {
        xf86Msg(X_ERROR, "Joystick: ioctl JSIOCGAXES on '%s' failed: %s\n", 
                joystick->device, strerror(errno));
        jstkCloseDevice(joystick);
        return -1;
    }

    if (ioctl(joystick->fd, JSIOCGBUTTONS, &buttons) == -1) {
        xf86Msg(X_ERROR, "Joystick: ioctl JSIOCGBUTTONS on '%s' failed: %s\n", 
                joystick->device, strerror(errno));
        jstkCloseDevice(joystick);
        return -1;
    }

    if (ioctl(joystick->fd, JSIOCGNAME(128), joy_name) == -1) {
        xf86Msg(X_ERROR, "Joystick: ioctl JSIOCGNAME on '%s' failed: %s\n", 
                  joystick->device, strerror(errno));
        jstkCloseDevice(joystick);
        return -1;
    }

    if (probe == TRUE) {
        xf86Msg(X_INFO, "Joystick: %s. %d axes, %d buttons\n", 
                joy_name, axes, buttons);
    }

    if (buttons > MAXBUTTONS)
        buttons = MAXBUTTONS;
    if (axes > MAXAXES)
        axes = MAXAXES;
    joystick->num_buttons = buttons;
    joystick->num_axes = axes;

    joystick->open_proc = jstkOpenDevice_joystick;
    joystick->read_proc = jstkReadData_joystick;
    joystick->close_proc = jstkCloseDevice;
    return joystick->fd;
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
jstkReadData_joystick(JoystickDevPtr joystick,
                      JOYSTICKEVENT *event,
                      int *number)
{
    struct js_event js;
    if (event != NULL) *event = EVENT_NONE;
    if (xf86ReadSerial(joystick->fd, &js, sizeof(struct js_event)) !=
        sizeof(struct js_event))
        return 0;

    switch(js.type & ~JS_EVENT_INIT) {
    case JS_EVENT_BUTTON:
        if (js.number < MAXBUTTONS)
        {
            if (joystick->button[js.number].pressed != js.value) {
                joystick->button[js.number].pressed = js.value;
                if (event != NULL) *event = EVENT_BUTTON;
                if (number != NULL) *number = js.number;
            }
        }
        break;
    case JS_EVENT_AXIS:
        if (js.number < MAXAXES) {
            if (abs(js.value) < joystick->axis[js.number].deadzone) {
                /* We only want one event when in deadzone */
                if (joystick->axis[js.number].value != 0) {
                    joystick->axis[js.number].value = 0;
                    if (event != NULL) *event = EVENT_AXIS;
                    if (number != NULL) *number = js.number;
                }
            }else{
                joystick->axis[js.number].value = js.value;
                if (event != NULL) *event = EVENT_AXIS;
                if (number != NULL) *number = js.number;
            }
        }
        break;
    }
    return 1;
}
