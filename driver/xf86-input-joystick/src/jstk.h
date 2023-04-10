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

#ifndef __JSTK_H_INCLUDED__
#define __JSTK_H_INCLUDED__

#include <xf86Xinput.h>
#include <X11/extensions/XIproto.h>
#include "joystick-properties.h" /* definitions */


#define MAXBUTTONS 32
#define MAXAXES 32
#define MAXKEYSPERBUTTON 4
#define MIN_KEYCODE 8
#define BUTTONMAP_SIZE 32


/******************************************************************************
 * debugging macro
 *****************************************************************************/
#ifdef DBG
    #undef DBG
#endif

#if DEBUG
    extern char debug_level;
    #define DBG(lvl, f) {if ((lvl) <= debug_level) f;}
#else
    #define DBG(lvl, f)
#endif


#ifndef XI_JOYSTICK
#define XI_JOYSTICK "JOYSTICK"
#endif

#ifndef XI86_SERVER_FD
#define XI86_SERVER_FD 0x20
#endif

typedef enum _JOYSTICKEVENT {
    EVENT_NONE=0,
    EVENT_BUTTON,
    EVENT_AXIS
} JOYSTICKEVENT;

typedef struct _JoystickDevRec *JoystickDevPtr;

typedef int(*jstkOpenDeviceProc)(JoystickDevPtr joystick, Bool probe);
typedef void(*jstkCloseDeviceProc)(JoystickDevPtr joystick);
typedef int(*jstkReadDataProc)(JoystickDevPtr joystick,
                               JOYSTICKEVENT *event, int *number);

typedef unsigned char KEYSCANCODES [MAXKEYSPERBUTTON];

typedef struct _AXIS {
    JSTK_TYPE    type;
    JSTK_MAPPING mapping;
    int             value, oldvalue;
    int             valuator;
    int             deadzone;
    float           currentspeed; /* TYPE_ACCELERATED */
    float           previousposition; /* TYPE_ABSOLUTE */
    float           amplify;
    float           subpixel; /* Pending subpixel movement */

    KEYSCANCODES    keys_low, keys_high;  /* MAPPING_KEY */
    int             key_isdown;
    OsTimerPtr      timer;
    Bool            timerrunning;
} AXIS;

typedef struct _BUTTON {
    JSTK_MAPPING mapping;
    char            pressed;
    int             buttonnumber;    /* MAPPING_BUTTON */
    float           amplify;       /* MAPPING_X/Y/ZX/ZY, 
                                      MAPPING_SPEED_MULTIPLY */
    float           currentspeed;  /* MAPPING_X/Y/ZX/ZY */
    float           subpixel; /* Pending subpixel movement */
    KEYSCANCODES    keys;   /* MAPPING_KEY */
} BUTTON;

typedef struct _JoystickDevRec {
    int          fd;          /* Actual file descriptor */
    jstkOpenDeviceProc open_proc; /* Call for re-open backend */
    jstkCloseDeviceProc close_proc; /* Callback for closing the backend */
    jstkReadDataProc read_proc; /* Callback for reading data from the backend */
    void         *devicedata; /* Extra platform device dependent data */
    char         *device;     /* Name of the device */
    InputInfoPtr joystick_device; /* Back pointer to the joystick device */
    InputInfoPtr keyboard_device; /* Slave device for keyboard events */

    OsTimerPtr   timer;       /* Timer for axis movement */
    Bool         timerrunning;

    Bool         mouse_enabled, keys_enabled;
    float        amplify;     /* Global amplifier of axis movement */

    int          repeat_delay, repeat_interval; /* Key autorepeat */

    CARD8        num_buttons, num_axes; /* Detected number of buttons/axes */

    XkbRMLVOSet rmlvo;
    AXIS axis[MAXAXES];           /* Configuration per axis */
    BUTTON button[MAXBUTTONS];    /* Configuration per button */
} JoystickDevRec;

void jstkCloseDevice(JoystickDevPtr priv);

#endif
