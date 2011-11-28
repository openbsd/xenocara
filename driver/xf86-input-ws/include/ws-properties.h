/*
 * Copyright (c) 2009 Matthieu Herrb <matthieu@herrb.eu>
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

#ifndef _WS_PROPERTIES_H
#define _WS_PROPERTIES_H

/* Middle mouse button emulation */
/* BOOL */
#define WS_PROP_MIDBUTTON "WS Pointer Middle Button Emulation"
/* CARD32 */
#define WS_PROP_MIDBUTTON_TIMEOUT "WS Pointer Middle Button Timeout"

/* Mouse wheel emulation */
/* BOOL */
#define WS_PROP_WHEEL "WS Pointer Wheel Emulation"
/* CARD8, 4 values [x up, x down, y up, y down], 0 to disable a value */
#define WS_PROP_WHEEL_AXES "WS Pointer Wheel Emulation Axes"
/* CARD16 */
#define WS_PROP_WHEEL_INERTIA "WS Pointer Wheel Emulation Inertia"
/* CARD32 */
#define WS_PROP_WHEEL_TIMEOUT "WS Pointer Wheel Emulation Timeout"
/* CARD8, value range 0-32, 0 to always scroll */
#define WS_PROP_WHEEL_BUTTON "WS Pointer Wheel Emulation Button"

/* Run-time calibration */
/* CARD32, 4 values [minx, maxx, miny, maxy], or no values for unset */
#define WS_PROP_CALIBRATION "WS Pointer Axis Calibration"

/* Swap x and y axis. */
/* BOOL */
#define WS_PROP_SWAP_AXES "WS Pointer Axes Swap"

#endif
