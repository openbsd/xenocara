/*
 * Copyright © 2009 Matthieu Herrb <matthieu@herrb.eu>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <dev/wscons/wsconsio.h>

/* #undef DEBUG */
#define DEBUG
#undef DBG
#ifdef DEBUG
extern int ws_debug_level;
# define DBG(lvl, f) { if ((lvl) <= ws_debug_level) f;}
#else
# define DBG(lvl, f)
#endif

#define NAXES		2	/* X and Y axes only */
#define NBUTTONS	32	/* max theoretical buttons */
#define DFLTBUTTONS	3	/* default number of buttons */

#define WS_NOMAP	0

#define NWSEVENTS	16	/* size of buffer */

/* axis specific data for wheel */
typedef struct {
	int negative;
	int positive;
	int traveled_distance;
} WheelAxis, *WheelAxisPtr;

typedef struct {
	unsigned int buttons;
	int dx, dy, dz, dw;
	int ax, ay;
} wsHwState;

typedef struct WSDevice {
	char *devName;			/* device name */
	int type;			/* ws device type */
	unsigned int lastButtons;	/* last state of buttons */
	int old_ax, old_ay;
	int min_x, max_x, min_y, max_y;	/* coord space */
	int swap_axes;
	int raw;
	int inv_x, inv_y;
	int screen_no;
	WheelAxis Z;
	WheelAxis W;
	struct wsmouse_calibcoords coords; /* mirror of the kernel values */

	/* # of buttons and config-file specified button mapping */
	unsigned int buttons;
	unsigned char btnmap[NBUTTONS + 1];

	/* Middle mouse button emulation */
	struct {
		BOOL enabled;
		BOOL pending;		/* timer waiting? */
		int buttonstate;	/* phys. button state */
		int state;		/* state machine (see emumb.c) */
		Time expires;		/* time of expiry */
		Time timeout;
	} emulateMB;

	/* Mouse wheel emulation */
	struct {
		BOOL enabled;
		int button;
		int button_state;
		int inertia;
		WheelAxis X;
		WheelAxis Y;
		Time expires;		/* time of expiry */
		Time timeout;
	} emulateWheel;

	struct wscons_event	events[NWSEVENTS];
	size_t			events_count;
	size_t			events_pos;
} WSDeviceRec, *WSDevicePtr;

/* Middle mouse button emulation */
extern int wsmbEmuTimer(InputInfoPtr);
extern BOOL wsmbEmuFilterEvent(InputInfoPtr, int, BOOL);
extern void wsmbEmuWakeupHandler(pointer, int, pointer);
extern void wsmbEmuBlockHandler(pointer, struct timeval **, pointer);
extern void wsmbEmuPreInit(InputInfoPtr);
extern void wsmbEmuOn(InputInfoPtr);
extern void wsmbEmuFinalize(InputInfoPtr);
extern void wsmbEmuInitProperty(DeviceIntPtr);

/* Mouse wheel emulation */
extern void wsWheelEmuPreInit(InputInfoPtr);
extern BOOL wsWheelEmuFilterButton(InputInfoPtr, unsigned int, int);
extern BOOL wsWheelEmuFilterMotion(InputInfoPtr, int, int);
extern void wsWheelEmuInitProperty(DeviceIntPtr);

extern void wsWheelHandleButtonMap(InputInfoPtr, WheelAxisPtr, const char *,
    const char *);
extern void wsButtonClicks(InputInfoPtr, int, int);
