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

#ifndef _JOYSTICK_PROPERTIES_
#define _JOYSTICK_PROPERTIES_

/**
 * Properties exported by the joystick driver. These properties are
 * recognized by the driver and will change its behavior when modified.
 */



/** To be used with property JSTK_PROP_AXIS_TYPE **/
typedef enum _JSTK_TYPE {
    JSTK_TYPE_NONE=0,      /* Axis value is not relevant */
    JSTK_TYPE_BYVALUE,     /* Speed of cursor is relative to amplitude */
    JSTK_TYPE_ACCELERATED, /* Speed is accelerated */
    JSTK_TYPE_ABSOLUTE     /* The amplitude defines the cursor position */
} JSTK_TYPE;


/** To be used with properties JSTK_PROP_AXIS_MAPPING, JSTK_PROP_BUTTON_MAPPING */
typedef enum _JSTK_MAPPING {
    JSTK_MAPPING_NONE=0,         /* Nothing */
    JSTK_MAPPING_X,              /* X-Axis */
    JSTK_MAPPING_Y,              /* Y-Axis */
    JSTK_MAPPING_ZX,             /* Horizontal scrolling */
    JSTK_MAPPING_ZY,             /* Vertical scrolling */
    JSTK_MAPPING_BUTTON,         /* Mouse button */
    JSTK_MAPPING_KEY,            /* Keyboard event */
    JSTK_MAPPING_SPEED_MULTIPLY, /* Will amplify all axis movement */
    JSTK_MAPPING_DISABLE,        /* Disable mouse and key events */
    JSTK_MAPPING_DISABLE_MOUSE,  /* Disable only mouse events */
    JSTK_MAPPING_DISABLE_KEYS    /* Disable only key events */
} JSTK_MAPPING;



/** Controls the verbosity of the driver */
/* 8 bit (0..20) */
#define JSTK_PROP_DEBUGLEVEL "Debug Level"

/** Number of buttons found on device */
/* 8 bit (0..MAXBUTTONS), read-only */
#define JSTK_PROP_NUMBUTTONS "Buttons"

/** Number of axes found on device */
/* 8 bit (0..MAXAXES), read-only */
#define JSTK_PROP_NUMAXES "Axes"

/** Generate pointer movement or button events */
/* 8 bit (0 or 1) */
#define JSTK_PROP_MOUSE_ENABLED "Generate Mouse Events"

/** Generate key events */
/* 8 bit (0 or 1) */
#define JSTK_PROP_KEYS_ENABLED "Generate Key Events"

/** Set the dead zone of each axis */
/* 32 bit (0..30000), for each axis*/
#define JSTK_PROP_AXIS_DEADZONE   "Axis Deadzone"

/** Set axis type to none, byvalue, accelerated, absolute */
/* 8 bit, one of enum _JSTK_TYPE per axis*/
#define JSTK_PROP_AXIS_TYPE   "Axis Type"

/** Set mapping of axis to none, x, y, zx, zy, key */
/* 8 bit, one of enum _JSTK_MAPPING per axis */
#define JSTK_PROP_AXIS_MAPPING   "Axis Mapping"

/** Set movement factor of axis (default 1.0f) */
/* FLOAT[MAXAXES], movement amplify per axis */
#define JSTK_PROP_AXIS_AMPLIFY "Axis Amplify"

/** Scancodes for axis in low position */
/* 8 bit, 4 per axis */
#define JSTK_PROP_AXIS_KEYS_LOW "Axis Keys (low)"

/** Scancodes for axis in high position */
/* 8 bit, 4 per axis */
#define JSTK_PROP_AXIS_KEYS_HIGH "Axis Keys (high)"

/** Set the mapping of each button to
    none, x, y, zx, zy, button, key, speed_multiply,
    disable, disable_mouse, disable_keys */
/* 8 bit, one of enum _JSTK_MAPPING per button */
#define JSTK_PROP_BUTTON_MAPPING   "Button Mapping"

/** Set the logical button to report for this physical button */
/* 8 bit (0..32), logical button number per button */
#define JSTK_PROP_BUTTON_BUTTONNUMBER "Button Number"

/** Set amplify factor of button (default 1.0f) */
/* FLOAT[MAXBUTTONS], amplify value per button */
#define JSTK_PROP_BUTTON_AMPLIFY "Button Amplify"

/** Scancodes for button */
/* 8 bit, 4 per button */
#define JSTK_PROP_BUTTON_KEYS "Button Keys"


#endif /* _JOYSTICK_PROPERTIES_ */
