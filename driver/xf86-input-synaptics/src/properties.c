/*
 * Copyright © 2008-2012 Red Hat, Inc.
 *
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
 *
 * Authors: Peter Hutterer
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include "xf86Module.h"

#include <X11/Xatom.h>
#include <xf86.h>
#include <xf86Xinput.h>
#include <exevents.h>

#include "synapticsstr.h"
#include "synaptics-properties.h"

#ifndef XATOM_FLOAT
#define XATOM_FLOAT "FLOAT"
#endif

#ifndef XI_PROP_PRODUCT_ID
#define XI_PROP_PRODUCT_ID "Device Product ID"
#endif

#ifndef XI_PROP_DEVICE_NODE
#define XI_PROP_DEVICE_NODE "Device Node"
#endif

static Atom float_type;

Atom prop_edges = 0;
Atom prop_finger = 0;
Atom prop_tap_time = 0;
Atom prop_tap_move = 0;
Atom prop_tap_durations = 0;
Atom prop_clickpad = 0;
Atom prop_middle_timeout = 0;
Atom prop_twofinger_pressure = 0;
Atom prop_twofinger_width = 0;
Atom prop_scrolldist = 0;
Atom prop_scrolledge = 0;
Atom prop_scrolltwofinger = 0;
Atom prop_speed = 0;
Atom prop_edgemotion_pressure = 0;
Atom prop_edgemotion_speed = 0;
Atom prop_edgemotion_always = 0;
Atom prop_buttonscroll = 0;
Atom prop_buttonscroll_repeat = 0;
Atom prop_buttonscroll_time = 0;
Atom prop_off = 0;
Atom prop_lockdrags = 0;
Atom prop_lockdrags_time = 0;
Atom prop_tapaction = 0;
Atom prop_clickaction = 0;
Atom prop_circscroll = 0;
Atom prop_circscroll_dist = 0;
Atom prop_circscroll_trigger = 0;
Atom prop_circpad = 0;
Atom prop_palm = 0;
Atom prop_palm_dim = 0;
Atom prop_coastspeed = 0;
Atom prop_pressuremotion = 0;
Atom prop_pressuremotion_factor = 0;
Atom prop_grab = 0;
Atom prop_gestures = 0;
Atom prop_capabilities = 0;
Atom prop_resolution = 0;
Atom prop_area = 0;
Atom prop_softbutton_areas = 0;
Atom prop_secondary_softbutton_areas = 0;
Atom prop_noise_cancellation = 0;
Atom prop_product_id = 0;
Atom prop_device_node = 0;

static Atom
InitTypedAtom(DeviceIntPtr dev, char *name, Atom type, int format, int nvalues,
              int *values)
{
    int i;
    Atom atom;
    uint8_t val_8[9];           /* we never have more than 9 values in an atom */
    uint16_t val_16[9];
    uint32_t val_32[9];
    pointer converted;

    for (i = 0; i < nvalues; i++) {
        switch (format) {
        case 8:
            val_8[i] = values[i];
            break;
        case 16:
            val_16[i] = values[i];
            break;
        case 32:
            val_32[i] = values[i];
            break;
        }
    }

    switch (format) {
    case 8:
        converted = val_8;
        break;
    case 16:
        converted = val_16;
        break;
    case 32:
    default:
        converted = val_32;
        break;
    }

    atom = MakeAtom(name, strlen(name), TRUE);
    XIChangeDeviceProperty(dev, atom, type, format, PropModeReplace, nvalues,
                           converted, FALSE);
    XISetDevicePropertyDeletable(dev, atom, FALSE);
    return atom;
}

static Atom
InitAtom(DeviceIntPtr dev, char *name, int format, int nvalues, int *values)
{
    return InitTypedAtom(dev, name, XA_INTEGER, format, nvalues, values);
}

static Atom
InitFloatAtom(DeviceIntPtr dev, char *name, int nvalues, float *values)
{
    Atom atom;

    atom = MakeAtom(name, strlen(name), TRUE);
    XIChangeDeviceProperty(dev, atom, float_type, 32, PropModeReplace,
                           nvalues, values, FALSE);
    XISetDevicePropertyDeletable(dev, atom, FALSE);
    return atom;
}

static void
InitSoftButtonProperty(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    SynapticsParameters *para = &priv->synpara;
    int values[8];

    values[0] = para->softbutton_areas[BOTTOM_RIGHT_BUTTON_AREA][LEFT];
    values[1] = para->softbutton_areas[BOTTOM_RIGHT_BUTTON_AREA][RIGHT];
    values[2] = para->softbutton_areas[BOTTOM_RIGHT_BUTTON_AREA][TOP];
    values[3] = para->softbutton_areas[BOTTOM_RIGHT_BUTTON_AREA][BOTTOM];
    values[4] = para->softbutton_areas[BOTTOM_MIDDLE_BUTTON_AREA][LEFT];
    values[5] = para->softbutton_areas[BOTTOM_MIDDLE_BUTTON_AREA][RIGHT];
    values[6] = para->softbutton_areas[BOTTOM_MIDDLE_BUTTON_AREA][TOP];
    values[7] = para->softbutton_areas[BOTTOM_MIDDLE_BUTTON_AREA][BOTTOM];
    prop_softbutton_areas =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_SOFTBUTTON_AREAS, 32, 8, values);

    if (!para->has_secondary_buttons)
        return;

    values[0] = para->softbutton_areas[TOP_RIGHT_BUTTON_AREA][LEFT];
    values[1] = para->softbutton_areas[TOP_RIGHT_BUTTON_AREA][RIGHT];
    values[2] = para->softbutton_areas[TOP_RIGHT_BUTTON_AREA][TOP];
    values[3] = para->softbutton_areas[TOP_RIGHT_BUTTON_AREA][BOTTOM];
    values[4] = para->softbutton_areas[TOP_MIDDLE_BUTTON_AREA][LEFT];
    values[5] = para->softbutton_areas[TOP_MIDDLE_BUTTON_AREA][RIGHT];
    values[6] = para->softbutton_areas[TOP_MIDDLE_BUTTON_AREA][TOP];
    values[7] = para->softbutton_areas[TOP_MIDDLE_BUTTON_AREA][BOTTOM];

    if (values[0] || values[1] || values[2] || values[4] ||
        values[5] || values[6] || values[7])
        prop_secondary_softbutton_areas =
            InitAtom(pInfo->dev, SYNAPTICS_PROP_SECONDARY_SOFTBUTTON_AREAS, 32, 8, values);
}

void
InitDeviceProperties(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    SynapticsParameters *para = &priv->synpara;
    int values[9];              /* we never have more than 9 values in an atom */
    float fvalues[4];           /* never have more than 4 float values */

    float_type = XIGetKnownProperty(XATOM_FLOAT);
    if (!float_type) {
        float_type = MakeAtom(XATOM_FLOAT, strlen(XATOM_FLOAT), TRUE);
        if (!float_type) {
            xf86IDrvMsg(pInfo, X_ERROR, "Failed to init float atom. "
                        "Disabling property support.\n");
            return;
        }
    }

    values[0] = para->left_edge;
    values[1] = para->right_edge;
    values[2] = para->top_edge;
    values[3] = para->bottom_edge;

    prop_edges = InitAtom(pInfo->dev, SYNAPTICS_PROP_EDGES, 32, 4, values);

    values[0] = para->finger_low;
    values[1] = para->finger_high;
    values[2] = 0;

    prop_finger = InitAtom(pInfo->dev, SYNAPTICS_PROP_FINGER, 32, 3, values);
    prop_tap_time =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_TAP_TIME, 32, 1, &para->tap_time);
    prop_tap_move =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_TAP_MOVE, 32, 1, &para->tap_move);

    values[0] = para->single_tap_timeout;
    values[1] = para->tap_time_2;
    values[2] = para->click_time;

    prop_tap_durations =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_TAP_DURATIONS, 32, 3, values);
    prop_clickpad =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_CLICKPAD, 8, 1, &para->clickpad);
    prop_middle_timeout =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_MIDDLE_TIMEOUT, 32, 1,
                 &para->emulate_mid_button_time);
    prop_twofinger_pressure =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_TWOFINGER_PRESSURE, 32, 1,
                 &para->emulate_twofinger_z);
    prop_twofinger_width =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_TWOFINGER_WIDTH, 32, 1,
                 &para->emulate_twofinger_w);

    values[0] = para->scroll_dist_vert;
    values[1] = para->scroll_dist_horiz;
    prop_scrolldist =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_SCROLL_DISTANCE, 32, 2, values);

    values[0] = para->scroll_edge_vert;
    values[1] = para->scroll_edge_horiz;
    values[2] = para->scroll_edge_corner;
    prop_scrolledge =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_SCROLL_EDGE, 8, 3, values);
    values[0] = para->scroll_twofinger_vert;
    values[1] = para->scroll_twofinger_horiz;
    prop_scrolltwofinger =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_SCROLL_TWOFINGER, 8, 2, values);

    fvalues[0] = para->min_speed;
    fvalues[1] = para->max_speed;
    fvalues[2] = para->accl;
    fvalues[3] = 0;
    prop_speed = InitFloatAtom(pInfo->dev, SYNAPTICS_PROP_SPEED, 4, fvalues);

    if (priv->has_scrollbuttons) {
        values[0] = para->updown_button_scrolling;
        values[1] = para->leftright_button_scrolling;
        prop_buttonscroll =
            InitAtom(pInfo->dev, SYNAPTICS_PROP_BUTTONSCROLLING, 8, 2, values);

        values[0] = para->updown_button_repeat;
        values[1] = para->leftright_button_repeat;
        prop_buttonscroll_repeat =
            InitAtom(pInfo->dev, SYNAPTICS_PROP_BUTTONSCROLLING_REPEAT, 8, 2,
                     values);
        prop_buttonscroll_time =
            InitAtom(pInfo->dev, SYNAPTICS_PROP_BUTTONSCROLLING_TIME, 32, 1,
                     &para->scroll_button_repeat);
    }

    prop_off =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_OFF, 8, 1, &para->touchpad_off);
    prop_lockdrags =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_LOCKED_DRAGS, 8, 1,
                 &para->locked_drags);
    prop_lockdrags_time =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_LOCKED_DRAGS_TIMEOUT, 32, 1,
                 &para->locked_drag_time);

    memcpy(values, para->tap_action, MAX_TAP * sizeof(int));
    prop_tapaction =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_TAP_ACTION, 8, MAX_TAP, values);

    memcpy(values, para->click_action, MAX_CLICK * sizeof(int));
    prop_clickaction =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_CLICK_ACTION, 8, MAX_CLICK, values);

    prop_circscroll =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_CIRCULAR_SCROLLING, 8, 1,
                 &para->circular_scrolling);

    fvalues[0] = para->scroll_dist_circ;
    prop_circscroll_dist =
        InitFloatAtom(pInfo->dev, SYNAPTICS_PROP_CIRCULAR_SCROLLING_DIST, 1,
                      fvalues);

    prop_circscroll_trigger =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_CIRCULAR_SCROLLING_TRIGGER, 8, 1,
                 &para->circular_trigger);
    prop_circpad =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_CIRCULAR_PAD, 8, 1,
                 &para->circular_pad);
    prop_palm =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_PALM_DETECT, 8, 1,
                 &para->palm_detect);

    values[0] = para->palm_min_width;
    values[1] = para->palm_min_z;

    prop_palm_dim =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_PALM_DIMENSIONS, 32, 2, values);

    fvalues[0] = para->coasting_speed;
    fvalues[1] = para->coasting_friction;
    prop_coastspeed =
        InitFloatAtom(pInfo->dev, SYNAPTICS_PROP_COASTING_SPEED, 2, fvalues);

    values[0] = para->press_motion_min_z;
    values[1] = para->press_motion_max_z;
    prop_pressuremotion =
        InitTypedAtom(pInfo->dev, SYNAPTICS_PROP_PRESSURE_MOTION, XA_CARDINAL,
                      32, 2, values);

    fvalues[0] = para->press_motion_min_factor;
    fvalues[1] = para->press_motion_max_factor;

    prop_pressuremotion_factor =
        InitFloatAtom(pInfo->dev, SYNAPTICS_PROP_PRESSURE_MOTION_FACTOR, 2,
                      fvalues);

    prop_grab =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_GRAB, 8, 1,
                 &para->grab_event_device);

    values[0] = para->tap_and_drag_gesture;
    prop_gestures = InitAtom(pInfo->dev, SYNAPTICS_PROP_GESTURES, 8, 1, values);

    values[0] = priv->has_left;
    values[1] = priv->has_middle;
    values[2] = priv->has_right;
    values[3] = priv->has_double;
    values[4] = priv->has_triple;
    values[5] = priv->has_pressure;
    values[6] = priv->has_width;
    prop_capabilities =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_CAPABILITIES, 8, 7, values);

    values[0] = para->resolution_vert;
    values[1] = para->resolution_horiz;
    prop_resolution =
        InitAtom(pInfo->dev, SYNAPTICS_PROP_RESOLUTION, 32, 2, values);

    values[0] = para->area_left_edge;
    values[1] = para->area_right_edge;
    values[2] = para->area_top_edge;
    values[3] = para->area_bottom_edge;
    prop_area = InitAtom(pInfo->dev, SYNAPTICS_PROP_AREA, 32, 4, values);

    if (para->clickpad)
        InitSoftButtonProperty(pInfo);

    values[0] = para->hyst_x;
    values[1] = para->hyst_y;
    prop_noise_cancellation = InitAtom(pInfo->dev,
                                       SYNAPTICS_PROP_NOISE_CANCELLATION, 32, 2,
                                       values);

    /* only init product_id property if we actually know them */
    if (priv->id_vendor || priv->id_product) {
        values[0] = priv->id_vendor;
        values[1] = priv->id_product;
        prop_product_id =
            InitAtom(pInfo->dev, XI_PROP_PRODUCT_ID, 32, 2, values);
    }

    if (priv->device) {
        prop_device_node =
            MakeAtom(XI_PROP_DEVICE_NODE, strlen(XI_PROP_DEVICE_NODE), TRUE);
        XIChangeDeviceProperty(pInfo->dev, prop_device_node, XA_STRING, 8,
                               PropModeReplace, strlen(priv->device),
                               (pointer) priv->device, FALSE);
        XISetDevicePropertyDeletable(pInfo->dev, prop_device_node, FALSE);
    }

}

int
SetProperty(DeviceIntPtr dev, Atom property, XIPropertyValuePtr prop,
            BOOL checkonly)
{
    InputInfoPtr pInfo = dev->public.devicePrivate;
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    SynapticsParameters *para = &priv->synpara;
    SynapticsParameters tmp;

    /* If checkonly is set, no parameters may be changed. So just let the code
     * change temporary variables and forget about it. */
    if (checkonly) {
        tmp = *para;
        para = &tmp;
    }

    if (property == prop_edges) {
        INT32 *edges;

        if (prop->size != 4 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        edges = (INT32 *) prop->data;
        if (edges[0] > edges[1] || edges[2] > edges[3])
            return BadValue;

        para->left_edge = edges[0];
        para->right_edge = edges[1];
        para->top_edge = edges[2];
        para->bottom_edge = edges[3];

    }
    else if (property == prop_finger) {
        INT32 *finger;

        if (prop->size != 3 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        finger = (INT32 *) prop->data;
        if (finger[0] > finger[1])
            return BadValue;

        para->finger_low = finger[0];
        para->finger_high = finger[1];
    }
    else if (property == prop_tap_time) {
        if (prop->size != 1 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        para->tap_time = *(INT32 *) prop->data;

    }
    else if (property == prop_tap_move) {
        if (prop->size != 1 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        para->tap_move = *(INT32 *) prop->data;
    }
    else if (property == prop_tap_durations) {
        INT32 *timeouts;

        if (prop->size != 3 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        timeouts = (INT32 *) prop->data;

        para->single_tap_timeout = timeouts[0];
        para->tap_time_2 = timeouts[1];
        para->click_time = timeouts[2];
    }
    else if (property == prop_clickpad) {
        BOOL value;

        if (prop->size != 1 || prop->format != 8 || prop->type != XA_INTEGER)
            return BadMatch;

        value = *(BOOL *) prop->data;
        if (!para->clickpad && value && !prop_softbutton_areas)
            InitSoftButtonProperty(pInfo);
        else if (para->clickpad && !value && prop_softbutton_areas) {
            XIDeleteDeviceProperty(dev, prop_softbutton_areas, FALSE);
            prop_softbutton_areas = 0;
        }

        para->clickpad = *(BOOL *) prop->data;
    }
    else if (property == prop_middle_timeout) {
        if (prop->size != 1 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        para->emulate_mid_button_time = *(INT32 *) prop->data;
    }
    else if (property == prop_twofinger_pressure) {
        if (prop->size != 1 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        para->emulate_twofinger_z = *(INT32 *) prop->data;
    }
    else if (property == prop_twofinger_width) {
        if (prop->size != 1 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        para->emulate_twofinger_w = *(INT32 *) prop->data;
    }
    else if (property == prop_scrolldist) {
        INT32 *dist;

        if (prop->size != 2 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        dist = (INT32 *) prop->data;
        if (dist[0] == 0 || dist[1] == 0)
            return BadValue;

        if (para->scroll_dist_vert != dist[0]) {
            para->scroll_dist_vert = dist[0];
            SetScrollValuator(dev, priv->scroll_axis_vert, SCROLL_TYPE_VERTICAL,
                              para->scroll_dist_vert, 0);
        }
        if (para->scroll_dist_horiz != dist[1]) {
            para->scroll_dist_horiz = dist[1];
            SetScrollValuator(dev, priv->scroll_axis_horiz,
                              SCROLL_TYPE_HORIZONTAL, para->scroll_dist_horiz,
                              0);
        }
    }
    else if (property == prop_scrolledge) {
        CARD8 *edge;

        if (prop->size != 3 || prop->format != 8 || prop->type != XA_INTEGER)
            return BadMatch;

        edge = (BOOL *) prop->data;
        para->scroll_edge_vert = edge[0];
        para->scroll_edge_horiz = edge[1];
        para->scroll_edge_corner = edge[2];
    }
    else if (property == prop_scrolltwofinger) {
        CARD8 *twofinger;

        if (prop->size != 2 || prop->format != 8 || prop->type != XA_INTEGER)
            return BadMatch;

        twofinger = (BOOL *) prop->data;
        para->scroll_twofinger_vert = twofinger[0];
        para->scroll_twofinger_horiz = twofinger[1];
    }
    else if (property == prop_speed) {
        float *speed;

        if (prop->size != 4 || prop->format != 32 || prop->type != float_type)
            return BadMatch;

        speed = (float *) prop->data;
        para->min_speed = speed[0];
        para->max_speed = speed[1];
        para->accl = speed[2];
    }
    else if (property == prop_buttonscroll) {
        BOOL *scroll;

        if (!priv->has_scrollbuttons)
            return BadMatch;

        if (prop->size != 2 || prop->format != 8 || prop->type != XA_INTEGER)
            return BadMatch;

        scroll = (BOOL *) prop->data;
        para->updown_button_scrolling = scroll[0];
        para->leftright_button_scrolling = scroll[1];

    }
    else if (property == prop_buttonscroll_repeat) {
        BOOL *repeat;

        if (!priv->has_scrollbuttons)
            return BadMatch;

        if (prop->size != 2 || prop->format != 8 || prop->type != XA_INTEGER)
            return BadMatch;

        repeat = (BOOL *) prop->data;
        para->updown_button_repeat = repeat[0];
        para->leftright_button_repeat = repeat[1];
    }
    else if (property == prop_buttonscroll_time) {
        if (!priv->has_scrollbuttons)
            return BadMatch;

        if (prop->size != 1 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        para->scroll_button_repeat = *(INT32 *) prop->data;

    }
    else if (property == prop_off) {
        CARD8 off;

        if (prop->size != 1 || prop->format != 8 || prop->type != XA_INTEGER)
            return BadMatch;

        off = *(CARD8 *) prop->data;

        if (off > 2)
            return BadValue;

        para->touchpad_off = off;
    }
    else if (property == prop_gestures) {
        BOOL *gestures;

        if (prop->size != 1 || prop->format != 8 || prop->type != XA_INTEGER)
            return BadMatch;

        gestures = (BOOL *) prop->data;
        para->tap_and_drag_gesture = gestures[0];
    }
    else if (property == prop_lockdrags) {
        if (prop->size != 1 || prop->format != 8 || prop->type != XA_INTEGER)
            return BadMatch;

        para->locked_drags = *(BOOL *) prop->data;
    }
    else if (property == prop_lockdrags_time) {
        if (prop->size != 1 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        para->locked_drag_time = *(INT32 *) prop->data;
    }
    else if (property == prop_tapaction) {
        int i;
        CARD8 *action;

        if (prop->size > MAX_TAP || prop->format != 8 ||
            prop->type != XA_INTEGER)
            return BadMatch;

        action = (CARD8 *) prop->data;

        for (i = 0; i < MAX_TAP; i++)
            para->tap_action[i] = action[i];
    }
    else if (property == prop_clickaction) {
        int i;
        CARD8 *action;

        if (prop->size > MAX_CLICK || prop->format != 8 ||
            prop->type != XA_INTEGER)
            return BadMatch;

        action = (CARD8 *) prop->data;

        for (i = 0; i < MAX_CLICK; i++)
            para->click_action[i] = action[i];
    }
    else if (property == prop_circscroll) {
        if (prop->size != 1 || prop->format != 8 || prop->type != XA_INTEGER)
            return BadMatch;

        para->circular_scrolling = *(BOOL *) prop->data;

    }
    else if (property == prop_circscroll_dist) {
        float circdist;

        if (prop->size != 1 || prop->format != 32 || prop->type != float_type)
            return BadMatch;

        circdist = *(float *) prop->data;
        if (circdist == 0)
            return BadValue;

        para->scroll_dist_circ = circdist;
    }
    else if (property == prop_circscroll_trigger) {
        int trigger;

        if (prop->size != 1 || prop->format != 8 || prop->type != XA_INTEGER)
            return BadMatch;

        trigger = *(CARD8 *) prop->data;
        if (trigger > 8)
            return BadValue;

        para->circular_trigger = trigger;

    }
    else if (property == prop_circpad) {
        if (prop->size != 1 || prop->format != 8 || prop->type != XA_INTEGER)
            return BadMatch;

        para->circular_pad = *(BOOL *) prop->data;
    }
    else if (property == prop_palm) {
        if (prop->size != 1 || prop->format != 8 || prop->type != XA_INTEGER)
            return BadMatch;

        para->palm_detect = *(BOOL *) prop->data;
    }
    else if (property == prop_palm_dim) {
        INT32 *dim;

        if (prop->size != 2 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        dim = (INT32 *) prop->data;

        para->palm_min_width = dim[0];
        para->palm_min_z = dim[1];
    }
    else if (property == prop_coastspeed) {
        float *coast_speeds;

        if (prop->size != 2 || prop->format != 32 || prop->type != float_type)
            return BadMatch;

        coast_speeds = (float *) prop->data;
        para->coasting_speed = coast_speeds[0];
        para->coasting_friction = coast_speeds[1];
    }
    else if (property == prop_pressuremotion) {
        CARD32 *press;

        if (prop->size != 2 || prop->format != 32 || prop->type != XA_CARDINAL)
            return BadMatch;

        press = (CARD32 *) prop->data;
        if (press[0] > press[1])
            return BadValue;

        para->press_motion_min_z = press[0];
        para->press_motion_max_z = press[1];
    }
    else if (property == prop_pressuremotion_factor) {
        float *press;

        if (prop->size != 2 || prop->format != 32 || prop->type != float_type)
            return BadMatch;

        press = (float *) prop->data;
        if (press[0] > press[1])
            return BadValue;

        para->press_motion_min_factor = press[0];
        para->press_motion_max_factor = press[1];
    }
    else if (property == prop_grab) {
        if (prop->size != 1 || prop->format != 8 || prop->type != XA_INTEGER)
            return BadMatch;

        para->grab_event_device = *(BOOL *) prop->data;
    }
    else if (property == prop_capabilities) {
        /* read-only */
        return BadValue;
    }
    else if (property == prop_resolution) {
        /* read-only */
        return BadValue;
    }
    else if (property == prop_area) {
        INT32 *area;

        if (prop->size != 4 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        area = (INT32 *) prop->data;
        if ((((area[0] != 0) && (area[1] != 0)) && (area[0] > area[1])) ||
            (((area[2] != 0) && (area[3] != 0)) && (area[2] > area[3])))
            return BadValue;

        para->area_left_edge = area[0];
        para->area_right_edge = area[1];
        para->area_top_edge = area[2];
        para->area_bottom_edge = area[3];
    }
    else if (property == prop_softbutton_areas) {
        int *areas;

        if (prop->size != 8 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        areas = (int *) prop->data;
        if (!SynapticsIsSoftButtonAreasValid(areas))
            return BadValue;

        memcpy(para->softbutton_areas[BOTTOM_RIGHT_BUTTON_AREA], areas, 4 * sizeof(int));
        memcpy(para->softbutton_areas[BOTTOM_MIDDLE_BUTTON_AREA], areas + 4, 4 * sizeof(int));
    }
    else if (property == prop_secondary_softbutton_areas) {
        int *areas;

        if (prop->size != 8 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        areas = (int *) prop->data;
        if (!SynapticsIsSoftButtonAreasValid(areas))
            return BadValue;

        memcpy(para->softbutton_areas[TOP_RIGHT_BUTTON_AREA], areas, 4 * sizeof(int));
        memcpy(para->softbutton_areas[TOP_MIDDLE_BUTTON_AREA], areas + 4, 4 * sizeof(int));
    }
    else if (property == prop_noise_cancellation) {
        INT32 *hyst;

        if (prop->size != 2 || prop->format != 32 || prop->type != XA_INTEGER)
            return BadMatch;

        hyst = (INT32 *) prop->data;
        if (hyst[0] < 0 || hyst[1] < 0)
            return BadValue;
        para->hyst_x = hyst[0];
        para->hyst_y = hyst[1];
    }
    else if (property == prop_product_id || property == prop_device_node)
        return BadValue;        /* read-only */
    else { /* unknown property */
        if (strcmp(SYNAPTICS_PROP_SOFTBUTTON_AREAS, NameForAtom(property)) == 0)
        {
            prop_softbutton_areas = property;
            if (SetProperty(dev, property, prop, checkonly) != Success)
                prop_softbutton_areas = 0;
            else if (!checkonly)
                XISetDevicePropertyDeletable(dev, property, FALSE);
        }
    }

    return Success;
}
