/*
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of Red Hat
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.  Red
 * Hat makes no representations about the suitability of this software
 * for any purpose.  It is provided "as is" without express or implied
 * warranty.
 *
 * THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
 * NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef	_SYNAPTICSSTR_H_
#define _SYNAPTICSSTR_H_

#include "synproto.h"

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 18
#define LogMessageVerbSigSafe xf86MsgVerb
#endif

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) > 19
#define NO_DRIVER_SCALING 1
#elif GET_ABI_MAJOR(ABI_XINPUT_VERSION) == 19 && GET_ABI_MINOR(ABI_XINPUT_VERSION) >= 2
/* as of 19.2, the server takes device resolution into account when scaling
   relative events from abs device, so we must not scale in synaptics. */
#define NO_DRIVER_SCALING 1
#endif

#ifdef DBG
#undef DBG
#endif

#ifdef DEBUG
#define DBG(verb, ...) \
    xf86MsgVerb(X_INFO, verb, __VA_ARGS__)
#else
#define DBG(verb, msg, ...)     /* */
#endif

/******************************************************************************
 *		Definitions
 *					structs, typedefs, #defines, enums
 *****************************************************************************/
#define SYNAPTICS_MOVE_HISTORY	5
#define SYNAPTICS_MAX_TOUCHES	10
#define SYN_MAX_BUTTONS 12      /* Max number of mouse buttons */

/* Minimum and maximum values for scroll_button_repeat */
#define SBR_MIN 10
#define SBR_MAX 1000

enum OffState {
    TOUCHPAD_ON = 0,
    TOUCHPAD_OFF = 1,
    TOUCHPAD_TAP_OFF = 2,
};

enum TapEvent {
    RT_TAP = 0,                 /* Right top corner */
    RB_TAP,                     /* Right bottom corner */
    LT_TAP,                     /* Left top corner */
    LB_TAP,                     /* Left bottom corner */
    F1_TAP,                     /* Non-corner tap, one finger */
    F2_TAP,                     /* Non-corner tap, two fingers */
    F3_TAP,                     /* Non-corner tap, three fingers */
    MAX_TAP
};

enum ClickFingerEvent {
    F1_CLICK1 = 0,              /* Click left, one finger */
    F2_CLICK1,                  /* Click left, two fingers */
    F3_CLICK1,                  /* Click left, three fingers */
    MAX_CLICK
};


typedef struct _SynapticsMoveHist {
    int x, y;
    CARD32 millis;
} SynapticsMoveHistRec;

typedef struct _SynapticsTouchAxis {
    const char *label;
    int min;
    int max;
    int res;
} SynapticsTouchAxisRec;

enum FingerState {              /* Note! The order matters. Compared with < operator. */
    FS_BLOCKED = -1,
    FS_UNTOUCHED = 0,           /* this is 0 so it's the initialized value. */
    FS_TOUCHED = 1,
    FS_PRESSED = 2,
};

enum MovingState {
    MS_FALSE,
    MS_TOUCHPAD_RELATIVE,
};

enum MidButtonEmulation {
    MBE_OFF,                    /* No button pressed */
    MBE_LEFT,                   /* Left button pressed, waiting for right button or timeout */
    MBE_RIGHT,                  /* Right button pressed, waiting for left button or timeout */
    MBE_MID,                    /* Left and right buttons pressed, waiting for both buttons
                                   to be released */
    MBE_TIMEOUT,                /* Waiting for both buttons to be released. */
    MBE_LEFT_CLICK,             /* Emulate left button click. */
    MBE_RIGHT_CLICK,            /* Emulate right button click. */
};

/* See docs/tapndrag.dia for a state machine diagram */
enum TapState {
    TS_START,                   /* No tap/drag in progress */
    TS_1,                       /* After first touch */
    TS_MOVE,                    /* Pointer movement enabled */
    TS_2A,                      /* After first release */
    TS_2B,                      /* After second/third/... release */
    TS_SINGLETAP,               /* After timeout after first release */
    TS_3,                       /* After second touch */
    TS_DRAG,                    /* Pointer drag enabled */
    TS_4,                       /* After release when "locked drags" enabled */
    TS_5,                       /* After touch when "locked drags" enabled */
    TS_CLICKPAD_MOVE,           /* After left button press on a clickpad */
};

enum TapButtonState {
    TBS_BUTTON_UP,              /* "Virtual tap button" is up */
    TBS_BUTTON_DOWN,            /* "Virtual tap button" is down */
};

enum TouchpadModel {
    MODEL_UNKNOWN = 0,
    MODEL_SYNAPTICS,
    MODEL_ALPS,
    MODEL_APPLETOUCH,
    MODEL_ELANTECH,
    MODEL_UNIBODY_MACBOOK
};

enum SoftButtonAreas {
    NO_BUTTON_AREA = -1,
    BOTTOM_BUTTON_AREA = 0,
    BOTTOM_RIGHT_BUTTON_AREA = 0,
    BOTTOM_MIDDLE_BUTTON_AREA = 1,
    TOP_BUTTON_AREA = 2,
    TOP_RIGHT_BUTTON_AREA = 2,
    TOP_MIDDLE_BUTTON_AREA = 3
};

enum SoftButtonAreaEdges {
    LEFT = 0,
    RIGHT = 1,
    TOP = 2,
    BOTTOM = 3
};

typedef struct _SynapticsParameters {
    /* Parameter data */
    int left_edge, right_edge, top_edge, bottom_edge;   /* edge coordinates absolute */
    int finger_low, finger_high, finger_press;  /* finger detection values in Z-values */
    int tap_time;
    int tap_move;               /* max. tapping time and movement in packets and coord. */
    int single_tap_timeout;     /* timeout to recognize a single tap */
    int tap_time_2;             /* max. tapping time for double taps */
    int click_time;             /* The duration of a single click */
    Bool clickpad;              /* Device is a has integrated buttons */
    Bool has_secondary_buttons; /* Device has a top soft-button area */
    int clickpad_ignore_motion_time; /* Ignore motion for X ms after a click */
    int emulate_mid_button_time;        /* Max time between left and right button presses to
                                           emulate a middle button press. */
    int emulate_twofinger_z;    /* pressure threshold to emulate two finger touch (for Alps) */
    int emulate_twofinger_w;    /* Finger width threshold to emulate two finger touch */
    int scroll_dist_vert;       /* Scrolling distance in absolute coordinates */
    int scroll_dist_horiz;      /* Scrolling distance in absolute coordinates */
    Bool scroll_edge_vert;      /* Enable/disable vertical scrolling on right edge */
    Bool scroll_edge_horiz;     /* Enable/disable horizontal scrolling on left edge */
    Bool scroll_edge_corner;    /* Enable/disable continuous edge scrolling when in the corner */
    Bool scroll_twofinger_vert; /* Enable/disable vertical two-finger scrolling */
    Bool scroll_twofinger_horiz;        /* Enable/disable horizontal two-finger scrolling */
    double min_speed, max_speed, accl;  /* movement parameters */

    Bool updown_button_scrolling;       /* Up/Down-Button scrolling or middle/double-click */
    Bool leftright_button_scrolling;    /* Left/right-button scrolling, or two lots of middle button */
    Bool updown_button_repeat;  /* If up/down button being used to scroll, auto-repeat? */
    Bool leftright_button_repeat;       /* If left/right button being used to scroll, auto-repeat? */
    int scroll_button_repeat;   /* time, in milliseconds, between scroll events being
                                 * sent when holding down scroll buttons */
    int touchpad_off;           /* Switches the touchpad off
                                 * 0 : Not off
                                 * 1 : Off
                                 * 2 : Only tapping and scrolling off
                                 */
    Bool locked_drags;          /* Enable locked drags */
    int locked_drag_time;       /* timeout for locked drags */
    int tap_action[MAX_TAP];    /* Button to report on tap events */
    int click_action[MAX_CLICK];        /* Button to report on click with fingers */
    Bool circular_scrolling;    /* Enable circular scrolling */
    double scroll_dist_circ;    /* Scrolling angle radians */
    int circular_trigger;       /* Trigger area for circular scrolling */
    Bool circular_pad;          /* Edge has an oval or circular shape */
    Bool palm_detect;           /* Enable Palm Detection */
    int palm_min_width;         /* Palm detection width */
    int palm_min_z;             /* Palm detection depth */
    double coasting_speed;      /* Coasting threshold scrolling speed in scrolls/s */
    double coasting_friction;   /* Number of scrolls per second per second to change coasting speed */
    int press_motion_min_z;     /* finger pressure at which minimum pressure motion factor is applied */
    int press_motion_max_z;     /* finger pressure at which maximum pressure motion factor is applied */
    double press_motion_min_factor;     /* factor applied on speed when finger pressure is at minimum */
    double press_motion_max_factor;     /* factor applied on speed when finger pressure is at minimum */
    Bool grab_event_device;     /* grab event device for exclusive use? */
    Bool tap_and_drag_gesture;  /* Switches the tap-and-drag gesture on/off */
    unsigned int resolution_horiz;      /* horizontal resolution of touchpad in units/mm */
    unsigned int resolution_vert;       /* vertical resolution of touchpad in units/mm */
    int area_left_edge, area_right_edge, area_top_edge, area_bottom_edge;       /* area coordinates absolute */
    int softbutton_areas[4][4]; /* soft button area coordinates, 0 => right, 1 => middle , 2 => secondary right, 3 => secondary middle button */
    int hyst_x, hyst_y;         /* x and y width of hysteresis box */
} SynapticsParameters;

struct _SynapticsPrivateRec {
    SynapticsParameters synpara;        /* Default parameter settings, read from
                                           the X config file */
    struct SynapticsProtocolOperations *proto_ops;
    void *proto_data;           /* protocol-specific data */

    struct SynapticsHwState *hwState;

    const char *device;         /* device node */
    CARD32 timer_time;          /* when timer last fired */
    OsTimerPtr timer;           /* for up/down-button repeat, tap processing, etc */

    struct CommData comm;

    struct SynapticsHwState *local_hw_state;    /* used in place of local hw state variables */

    SynapticsMoveHistRec move_hist[SYNAPTICS_MOVE_HISTORY];     /* movement history */
    int hist_index;             /* Last added entry in move_hist[] */
    int hyst_center_x;          /* center x of hysteresis */
    int hyst_center_y;          /* center y of hysteresis */
    struct {
        int last_x;             /* last x-scroll position */
        int last_y;             /* last y-scroll position */
        double delta_x;         /* accumulated horiz scroll delta */
        double delta_y;         /* accumulated vert scroll delta */
        double last_a;          /* last angle-scroll position */
        CARD32 last_millis;     /* time last scroll event posted */
        double coast_speed_x;   /* Horizontal coasting speed in scrolls/s */
        double coast_speed_y;   /* Vertical coasting speed in scrolls/s */
        double coast_delta_x;   /* Accumulated horizontal coast delta */
        double coast_delta_y;   /* Accumulated vertical coast delta */
        int packets_this_scroll;        /* Events received for this scroll */
    } scroll;
    int count_packet_finger;    /* packet counter with finger on the touchpad */
    int button_delay_millis;    /* button delay for 3rd button emulation */
    Bool prev_up;               /* Previous up button value, for double click emulation */
    enum FingerState finger_state;      /* previous finger state */
    CARD32 last_motion_millis;  /* time of the last motion */
    enum SoftButtonAreas last_button_area;    /* Last button area we were in */
    int clickpad_click_millis;  /* Time of last clickpad click */

    enum TapState tap_state;    /* State of tap processing */
    int tap_max_fingers;        /* Max number of fingers seen since entering start state */
    int tap_button;             /* Which button started the tap processing */
    enum TapButtonState tap_button_state;       /* Current tap action */
    SynapticsMoveHistRec touch_on;      /* data when the touchpad is touched/released */

    enum MovingState moving_state;      /* previous moving state */
    Bool vert_scroll_edge_on;   /* Keeps track of currently active scroll modes */
    Bool horiz_scroll_edge_on;  /* Keeps track of currently active scroll modes */
    Bool vert_scroll_twofinger_on;      /* Keeps track of currently active scroll modes */
    Bool horiz_scroll_twofinger_on;     /* Keeps track of currently active scroll modes */
    Bool circ_scroll_on;        /* Keeps track of currently active scroll modes */
    Bool circ_scroll_vert;      /* True: Generate vertical scroll events
                                   False: Generate horizontal events */
    double frac_x, frac_y;      /* absolute -> relative fraction */
    enum MidButtonEmulation mid_emu_state;      /* emulated 3rd button */
    int repeatButtons;          /* buttons for repeat */
    int nextRepeat;             /* Time when to trigger next auto repeat event */
    int lastButtons;            /* last state of the buttons */
    int prev_z;                 /* previous z value, for palm detection */
    int prevFingers;            /* previous numFingers, for transition detection */
    int avg_width;              /* weighted average of previous fingerWidth values */
#ifndef NO_DRIVER_SCALING
    double horiz_coeff;         /* normalization factor for x coordintes */
    double vert_coeff;          /* normalization factor for y coordintes */
#endif

    int minx, maxx, miny, maxy; /* min/max dimensions as detected */
    int minp, maxp, minw, maxw; /* min/max pressure and finger width as detected */
    int resx, resy;             /* resolution of coordinates as detected in units/mm */
    Bool has_left;              /* left button detected for this device */
    Bool has_right;             /* right button detected for this device */
    Bool has_middle;            /* middle button detected for this device */
    Bool has_double;            /* double click detected for this device */
    Bool has_triple;            /* triple click detected for this device */
    Bool has_pressure;          /* device reports pressure */
    Bool has_width;             /* device reports finger width */
    Bool has_scrollbuttons;     /* device has physical scrollbuttons */
    Bool has_semi_mt;           /* device is only semi-multitouch capable */

    enum TouchpadModel model;   /* The detected model */
    unsigned short id_vendor;   /* vendor id */
    unsigned short id_product;  /* product id */

    int scroll_axis_horiz;      /* Horizontal smooth-scrolling axis */
    int scroll_axis_vert;       /* Vertical smooth-scrolling axis */
    ValuatorMask *scroll_events_mask;   /* ValuatorMask for smooth-scrolling */

    Bool has_touch;             /* Device has multitouch capabilities */
    int max_touches;            /* Number of touches supported */
    int num_mt_axes;            /* Number of multitouch axes other than X, Y */
    SynapticsTouchAxisRec *touch_axes;  /* Touch axis information other than X, Y */
    int num_slots;              /* Number of touch slots allocated */
    int *open_slots;            /* Array of currently open touch slots */
    int num_active_touches;     /* Number of active touches on device */
};

#endif                          /* _SYNAPTICSSTR_H_ */
