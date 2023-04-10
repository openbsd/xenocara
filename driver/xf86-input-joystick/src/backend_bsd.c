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
 * This provides the backend for USB-HIDs for NetBSD, OpenBSD and FreeBSD
 * Needs the uhid module loaded. Device names are /dev/uhid?
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
#include <stdlib.h>

#include <xf86.h>
#include <xf86_OSproc.h>

#include <usbhid.h>
#include <dev/usb/usb.h>
#include <dev/usb/usbhid.h>
#ifdef HAVE_DEV_HID_HID_H
    #include <dev/hid/hid.h>
#endif
#ifdef HAVE_DEV_USB_USB_IOCTL_H
    #include <dev/usb/usb_ioctl.h>
#endif

#include "jstk.h"
#include "backend_bsd.h"


struct jstk_bsd_hid_data {
    int dlen;                                /* Length of one data chunk */
    char *data_buf;                          /* Data buffer with right size */
    struct hid_item axis_item[MAXAXES];      /* Axis HID items */
    struct hid_item button_item[MAXBUTTONS]; /* Button HID items */
    struct hid_item hat_item[MAXAXES];       /* HID items for hats */
    int hats;                                /* Number of hats */
    int hotdata;                             /* Is unprocessed data available
                                                in data_buf? */
};

static void jstkCloseDevice_bsd(JoystickDevPtr joystick);
static int jstkReadData_bsd(JoystickDevPtr joystick,
                            JOYSTICKEVENT *event,
                            int *number);



/***********************************************************************
 *
 * jstkOpenDevice --
 *
 * Open and initialize a joystick device
 * Returns the filedescriptor, or -1 in case of error
 *
 ***********************************************************************
 */

int
jstkOpenDevice_bsd(JoystickDevPtr joystick, Bool probe)
{
    int cur_axis;
    int is_joystick, report_id = 0;
    int got_something;
    struct hid_data *d;
    struct hid_item h;
    report_desc_t rd;
    struct jstk_bsd_hid_data *bsddata;

    if (joystick->fd == -1) {
        if ((joystick->fd = open(joystick->device, O_RDWR | O_NDELAY, 0)) < 0) {
            xf86Msg(X_ERROR, "Cannot open joystick '%s' (%s)\n",
                    joystick->device, strerror(errno));
            return -1;
        }
    }

    if ((rd = hid_get_report_desc(joystick->fd)) == 0) {
        xf86Msg(X_ERROR, "Joystick: hid_get_report_desc failed: %s\n",
                strerror(errno));
        jstkCloseDevice_bsd(joystick);
        return -1;
    }

    if (ioctl(joystick->fd, USB_GET_REPORT_ID, &report_id) < 0) {
        xf86Msg(X_ERROR, "Joystick: ioctl USB_GET_REPORT_ID failed: %s\n",
                strerror(errno));
        jstkCloseDevice_bsd(joystick);
        return -1;
    }

    bsddata = (struct jstk_bsd_hid_data*)
              malloc(sizeof(struct jstk_bsd_hid_data));
    joystick->devicedata = (void*) bsddata;

    bsddata->dlen = hid_report_size(rd, hid_input, report_id);

    if ((bsddata->data_buf = malloc(bsddata->dlen)) == NULL) {
        fprintf(stderr, "error: couldn't malloc %d bytes\n", bsddata->dlen);
        hid_dispose_report_desc(rd);
        jstkCloseDevice_bsd(joystick);
        return -1;
    }

    is_joystick = 0;
    got_something = 0;
    cur_axis = 0;
    bsddata->hats = 0;
    joystick->num_axes = 0;
    joystick->num_buttons = 0;

    for (d = hid_start_parse(rd, 1 << hid_input, report_id);
         hid_get_item(d, &h); )
    {
        int usage, page;

        page = HID_PAGE(h.usage);
        usage = HID_USAGE(h.usage);

        is_joystick = is_joystick ||
                      (h.kind == hid_collection &&
                       page == HUP_GENERIC_DESKTOP &&
                       (usage == HUG_JOYSTICK || usage == HUG_GAME_PAD));

        if (h.kind != hid_input)
            continue;

        if (!is_joystick)
            continue;

        if (page == HUP_GENERIC_DESKTOP) {
            if (usage == HUG_HAT_SWITCH) {
                if ((bsddata->hats < MAXAXES) && (joystick->num_axes <= MAXAXES-2)) {
                    got_something = 1;
                    memcpy(&bsddata->hat_item[bsddata->hats], &h, sizeof(h));
                    bsddata->hats++;
                    joystick->num_axes += 2;
                }
            } else {
                if (joystick->num_axes < MAXAXES) {
                    got_something = 1;
                    memcpy(&bsddata->axis_item[cur_axis], &h, sizeof(h));
                    cur_axis++;
                    joystick->num_axes++;
                }
            }
        } else if (page == HUP_BUTTON) {
            if (joystick->num_buttons < MAXBUTTONS) {
                got_something = 1;
                memcpy(&bsddata->button_item[joystick->num_buttons], &h, sizeof(h));
                joystick->num_buttons++;
            }
	}
    }
    hid_end_parse(d);

    if (!got_something) {
        free(bsddata->data_buf);
        xf86Msg(X_ERROR, "Joystick: Didn't find any usable axes.\n");
        jstkCloseDevice_bsd(joystick);
        return -1;
    }

    bsddata->hotdata = 0;
    if (probe == TRUE) {
        xf86Msg(X_INFO, "Joystick: %d buttons, %d axes\n", 
                joystick->num_buttons, joystick->num_axes);
    }

    joystick->open_proc = jstkOpenDevice_bsd;
    joystick->read_proc = jstkReadData_bsd;
    joystick->close_proc = jstkCloseDevice_bsd;

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
jstkCloseDevice_bsd(JoystickDevPtr joystick)
{
    jstkCloseDevice(joystick);
    if (joystick->devicedata != NULL) {
        if (((struct jstk_bsd_hid_data*)joystick->devicedata)->data_buf)
            free(((struct jstk_bsd_hid_data*)joystick->devicedata)->data_buf);
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
jstkReadData_bsd(JoystickDevPtr joystick,
             JOYSTICKEVENT *event,
             int *number)
{
    int j,d;
    struct jstk_bsd_hid_data *bsddata = 
        (struct jstk_bsd_hid_data*)(joystick->devicedata);

    if (event != NULL) *event = EVENT_NONE;
    if (bsddata->hotdata == 0) {
        j= xf86ReadSerial(joystick->fd,
                          bsddata->data_buf,
                          bsddata->dlen);
        if (j != bsddata->dlen) {
            ErrorF("Read: %d byte! Should be %d\n",j,bsddata->dlen);
            return 0;
        }
        bsddata->hotdata = 1;
    }

    for (j=0; j<joystick->num_axes - (bsddata->hats * 2); j++) {
        d = hid_get_data(bsddata->data_buf, &bsddata->axis_item[j]);
        /* Scale the range to our expected range of -32768 to 32767 */
        d = d - (bsddata->axis_item[j].logical_maximum 
                 - bsddata->axis_item[j].logical_minimum) / 2;
        d = d * 65536 / (bsddata->axis_item[j].logical_maximum 
                         - bsddata->axis_item[j].logical_minimum);
        if (abs(d) < joystick->axis[j].deadzone) d = 0;
        if (d != joystick->axis[j].value) {
            joystick->axis[j].value = d;
            if (event != NULL) *event = EVENT_AXIS;
            if (number != NULL) *number = j;
            return 2;
        }
    }

    for (j=0; j<bsddata->hats; j++) {
        int a;
        int v1_data[9] = 
            { 0, 32767, 32767, 32767, 0, -32768, -32768, -32768, 0 };
        int v2_data[9] =
            { -32768, -32768, 0, 32767, 32767, 32767, 0, -32767, 0 };

        a = j*2 + joystick->num_axes - bsddata->hats *2;
        d = hid_get_data(bsddata->data_buf, &bsddata->hat_item[j]) 
            - bsddata->hat_item[j].logical_minimum;
        if (joystick->axis[a].value != v1_data[d]) {
            joystick->axis[a].value = v1_data[d];
            if (event != NULL) *event = EVENT_AXIS;
            if (number != NULL) *number = a;
            return 2;
        }
        if (joystick->axis[a+1].value != v2_data[d]) {
            joystick->axis[a+1].value = v2_data[d];
            if (event != NULL) *event = EVENT_AXIS;
            if (number != NULL) *number = a+1;
            return 2;
        }
    }

    for (j=0; j<joystick->num_buttons; j++) {
        int pressed;
        d = hid_get_data(bsddata->data_buf, &bsddata->button_item[j]);
        pressed = (d == bsddata->button_item[j].logical_minimum) ? 0 : 1;
        if (pressed != joystick->button[j].pressed) {
            joystick->button[j].pressed = pressed;
            if (event != NULL) *event = EVENT_BUTTON;
            if (number != NULL) *number = j;
            return 2;
        }
    }

    bsddata->hotdata = 0;
    return 1;
}
