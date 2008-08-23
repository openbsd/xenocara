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


#define MAXBUTTONS 32
#define MAXAXES 32
#define MAXKEYSPERBUTTON 4
#define MIN_KEYCODE 8


/******************************************************************************
 * debugging macro
 *****************************************************************************/
#ifdef DBG
    #undef DBG
#endif

#if DEBUG
    extern int debug_level;
    #define DBG(lvl, f) {if ((lvl) <= debug_level) f;}
#else
    #define DBG(lvl, f)
#endif

typedef enum _JOYSTICKEVENT {
    EVENT_NONE=0,
    EVENT_BUTTON,
    EVENT_AXIS
} JOYSTICKEVENT;

typedef struct _JoystickDevRec *JoystickDevPtr;

typedef void(*jstkCloseDeviceProc)(JoystickDevPtr joystick);
typedef int(*jstkReadDataProc)(JoystickDevPtr joystick,
                               JOYSTICKEVENT *event, int *number);


typedef enum _JOYSTICKTYPE{
    TYPE_NONE=0,      /* Axis value is not relevant */
    TYPE_BYVALUE,     /* Speed of cursor is relative to amplitude */
    TYPE_ACCELERATED, /* Speed is accelerated */
    TYPE_ABSOLUTE     /* The amplitude defines the cursor position */
} JOYSTICKTYPE;

typedef enum _JOYSTICKMAPPING{
    MAPPING_NONE=0,           /* Nothing */
    MAPPING_X,              /* X-Axis */
    MAPPING_Y,              /* Y-Axis */
    MAPPING_ZX,             /* Horizontal scrolling */
    MAPPING_ZY,             /* Vertical scrolling */
    MAPPING_BUTTON,         /* Mouse button */
    MAPPING_KEY,            /* Keyboard event */
    MAPPING_SPEED_MULTIPLY, /* Will amplify all axis movement */
    MAPPING_DISABLE,        /* Disable mouse and key events */
    MAPPING_DISABLE_MOUSE,  /* Disable only mouse events */
    MAPPING_DISABLE_KEYS    /* Disable only key events */
} JOYSTICKMAPPING;

typedef unsigned int KEYSCANCODES [MAXKEYSPERBUTTON];

typedef struct _AXIS {
    JOYSTICKTYPE    type;
    JOYSTICKMAPPING mapping;
    int             value, oldvalue;
    int             valuator;
    int             deadzone;
    float           currentspeed; /* TYPE_ACCELERATED */
    float           previousposition; /* TYPE_ABSOLUTE */
    float           amplify;
    float           subpixel; /* Pending subpixel movement */
    KEYSCANCODES    keys_low, keys_high;  /* MAPPING_KEY */
} AXIS;

typedef struct _BUTTON {
    JOYSTICKMAPPING mapping;
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
    jstkCloseDeviceProc close_proc; /* Callback for closing the backend */
    jstkReadDataProc read_proc; /* Callback for reading data from the backend */
    void         *devicedata; /* Extra platform device dependend data */
    char         *device;     /* Name of the device */

    OsTimerPtr   timer;       /* Timer for axis movement */
    Bool         timerrunning;

    Bool         mouse_enabled, keys_enabled;
    float        amplify;     /* Global amplifier of axis movement */

    int          repeat_delay, repeat_interval; /* Key autorepeat */

    struct _BUTTONMAP {
        int size;
        CARD8 scrollbutton[4];     /* Logical button numbers for scrollwheel */
        CARD8 map[MAXBUTTONS+1];
    } buttonmap;
    struct _KEYMAP {
        int size;
        KeySym map[256-MIN_KEYCODE];
    } keymap;

    AXIS axis[MAXAXES];           /* Configuration per axis */
    BUTTON button[MAXBUTTONS];    /* Configuration per button */
} JoystickDevRec;

#endif
