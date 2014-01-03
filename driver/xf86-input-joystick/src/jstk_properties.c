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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include <xf86Module.h>

#include <misc.h>
#include <xf86.h>
#include <X11/Xatom.h>
#include <xf86Xinput.h>
#include <exevents.h>

#include "jstk.h"
#include "jstk_properties.h"
#include "joystick-properties.h" /* definitions of properties */


static Atom prop_debuglevel = 0;
static Atom prop_numbuttons = 0;
static Atom prop_numaxes = 0;
static Atom prop_mouse_enabled = 0;
static Atom prop_keys_enabled  = 0;
static Atom prop_axis_deadzone = 0;
static Atom prop_axis_type = 0;
static Atom prop_axis_mapping = 0;
static Atom prop_axis_amplify = 0;
static Atom prop_axis_keys_low = 0;
static Atom prop_axis_keys_high = 0;
static Atom prop_button_mapping = 0;
static Atom prop_button_buttonnumber = 0;
static Atom prop_button_amplify = 0;
static Atom prop_button_keys = 0;


#ifndef XATOM_FLOAT
#define XATOM_FLOAT "FLOAT"
#endif
static Atom float_type = 0;


static int
jstkSetProperty(DeviceIntPtr pJstk, Atom atom, XIPropertyValuePtr val,
                BOOL checkonly)
{
    InputInfoPtr  pInfo = pJstk->public.devicePrivate;
    JoystickDevPtr priv = pInfo->private;
    int i, j;

    if (atom == prop_debuglevel)
    {
#if DEBUG
        if (val->size != 1 || val->format != 8 || val->type != XA_INTEGER)
            return BadMatch;
        if (!checkonly)
        {
            debug_level = *((INT8*)val->data);
            ErrorF("JOYSTICK: DebugLevel set to %d\n", debug_level);
        }
#endif
    }else if (atom == prop_numbuttons)
    {
        if (val->size != 1 || val->format != 8 || val->type != XA_INTEGER)
            return BadMatch;
        if ((*((INT8*)val->data)) != priv->num_buttons)
            return BadMatch;
        return Success;
    }else if (atom == prop_numaxes)
    {
        if (val->size != 1 || val->format != 8 || val->type != XA_INTEGER)
            return BadMatch;
        if ((*((INT8*)val->data)) != priv->num_axes)
            return BadMatch;
        return Success;
    }else if (atom == prop_mouse_enabled)
    {
        if (val->size != 1 || val->format != 8 || val->type != XA_INTEGER)
            return BadMatch;
        if (!checkonly)
        {
            priv->mouse_enabled = (*((INT8*)val->data)) != 0;
            DBG(1, ErrorF("mouse_enabled set to %d\n", priv->mouse_enabled));
        }
    }else if (atom == prop_keys_enabled)
    {
        if (val->size != 1 || val->format != 8 || val->type != XA_INTEGER)
            return BadMatch;
        if (!checkonly)
        {
            priv->keys_enabled = (*((INT8*)val->data)) != 0;
            DBG(1, ErrorF("keys_enabled set to %d\n", priv->keys_enabled));
        }
    }else if (atom == prop_axis_deadzone)
    {
        INT32 *values;
        if (val->size != priv->num_axes || val->format != 32 || val->type != XA_INTEGER)
            return BadMatch;
        values = (INT32*)val->data;
        for (i =0; i<val->size; i++) /* Fail, if one value is out of range */ 
            if (values[i] > 30000 || values[i] < -30000)
                return BadValue;
        if (!checkonly)
        {
            for (i =0; i<val->size; i++) {
                priv->axis[i].deadzone = (values[i]<0)?(-values[i]):(values[i]);
                DBG(1, ErrorF("Deadzone of axis %d set to %d\n",i, priv->axis[i].deadzone));
            }
        }
    }else if (atom == prop_axis_type)
    {
        INT8 *values;
        if (val->size != priv->num_axes || val->format != 8 || val->type != XA_INTEGER)
            return BadMatch;
        if (!checkonly)
        {
            values = (INT8*)val->data;
            for (i =0; i<val->size; i++) {
                priv->axis[i].type = values[i];
                DBG(1, ErrorF("Type of axis %d set to %d\n",i, priv->axis[i].type));
            }
        }
    }else if (atom == prop_axis_mapping)
    {
        INT8 *values;
        if (val->size != priv->num_axes || val->format != 8 || val->type != XA_INTEGER)
            return BadMatch;
        if (!checkonly)
        {
            values = (INT8*)val->data;
            for (i =0; i<val->size; i++) {
                priv->axis[i].mapping = values[i];
                DBG(1, ErrorF("Mapping of axis %d set to %d\n",i, priv->axis[i].mapping));
            }
        }
    }else if (atom == prop_axis_amplify)
    {
        float *values;
        if (val->size != priv->num_axes || val->format != 32 || val->type != float_type)
            return BadMatch;
        if (!checkonly)
        {
            values = (float*)val->data;
            for (i =0; i<val->size; i++) {
                priv->axis[i].amplify = values[i];
                DBG(1, ErrorF("Amplify of axis %d set to %.3f\n", i, priv->axis[i].amplify));
            }
        }
    }else if (atom == prop_axis_keys_low)
    {
        unsigned char *values;
        if (val->size != priv->num_axes*MAXKEYSPERBUTTON || val->format != 8 || val->type != XA_INTEGER)
            return BadMatch;
        if (!checkonly)
        {
            values = (unsigned char*)val->data;
            for (i =0; i<val->size/MAXKEYSPERBUTTON; i++) {
                DBG(1, ErrorF("key_low of axis %d set to ", i));
                for (j = 0; j<MAXKEYSPERBUTTON; j++) {
                    priv->axis[i].keys_low[j] = values[i*MAXKEYSPERBUTTON+j];
                    DBG(1, ErrorF("%d ", priv->axis[i].keys_low[j]));
                }
                DBG(1, ErrorF("\n"));
            }
        }
    }else if (atom == prop_axis_keys_high)
    {
        unsigned char *values;
        if (val->size != priv->num_axes*MAXKEYSPERBUTTON || val->format != 8 || val->type != XA_INTEGER)
            return BadMatch;
        if (!checkonly)
        {
            values = (unsigned char*)val->data;
            for (i =0; i<val->size/MAXKEYSPERBUTTON; i++) {
                DBG(1, ErrorF("key_high of axis %d set to ", i));
                for (j = 0; j<MAXKEYSPERBUTTON; j++) {
                    priv->axis[i].keys_high[j] = values[i*MAXKEYSPERBUTTON+j];
                    DBG(1, ErrorF("%d ", priv->axis[i].keys_high[j]));
                }
                DBG(1, ErrorF("\n"));
            }
        }
    }else if (atom == prop_button_mapping)
    {
        INT8 *values;
        if (val->size != priv->num_buttons || val->format != 8 || val->type != XA_INTEGER)
            return BadMatch;
        if (!checkonly)
        {
            values = (INT8*)val->data;
            for (i =0; i<val->size; i++) {
                priv->button[i].mapping = values[i];
                DBG(1, ErrorF("Mapping of button %d set to %d\n",i, priv->button[i].mapping));
            }
        }
    }else if (atom == prop_button_buttonnumber)
    {
        CARD8 *values;
        if (val->size != priv->num_buttons || val->format != 8 || val->type != XA_INTEGER)
            return BadMatch;
        values = (CARD8*)val->data;
        for (i = 0; i<val->size; i++) {
            if (values[i] > BUTTONMAP_SIZE) {
               DBG(1, ErrorF("Button number out of range (0..%d): %d\n",
                   BUTTONMAP_SIZE, values[i]));
               return BadValue;
            }
        }
        if (!checkonly)
        {
            for (i = 0; i<val->size; i++) {
                priv->button[i].buttonnumber =
                    values[i];
                DBG(1, ErrorF("Button number of button %d set to %d\n",
                            i,
                            priv->button[i].buttonnumber));
            }
        }
        return Success;
    }else if (atom == prop_button_amplify)
    {
        float *values;
        if (val->size != priv->num_buttons || val->format != 32 || val->type != float_type)
            return BadMatch;
        if (!checkonly)
        {
            values = (float*)val->data;
            for (i =0; i<val->size; i++) {
                priv->button[i].amplify = values[i];
                DBG(1, ErrorF("Amplify of button %d set to %.3f\n", i, priv->button[i].amplify));
            }
        }
    }else if (atom == prop_button_keys)
    {
        unsigned char *values;
        if (val->size != priv->num_buttons*MAXKEYSPERBUTTON || val->format != 8 || val->type != XA_INTEGER)
            return BadMatch;
        if (!checkonly)
        {
            values = (unsigned char*)val->data;
            for (i = 0; i<val->size/MAXKEYSPERBUTTON; i++) {
                DBG(1, ErrorF("keys of button %d set to ", i));
                for (j = 0; j<MAXKEYSPERBUTTON; j++) {
                    priv->button[i].keys[j] = values[i*MAXKEYSPERBUTTON+j];
                    DBG(1, ErrorF("%d ", priv->button[i].keys[j]));
                }
                DBG(1, ErrorF("\n"));
            }
        }
    }

    /* property not handled, report success */
    return Success;
}

Bool
jstkInitProperties(DeviceIntPtr pJstk, JoystickDevPtr priv)
{
    INT32 axes_values32[MAXAXES];
    INT8  axes_values8[MAXAXES*MAXKEYSPERBUTTON];
    INT8  button_values8[MAXBUTTONS*MAXKEYSPERBUTTON];
    float axes_floats[MAXAXES];
    float button_floats[MAXBUTTONS];
    int i, j;

    XIRegisterPropertyHandler(pJstk, jstkSetProperty, NULL, NULL);

    float_type = XIGetKnownProperty(XATOM_FLOAT);
    if (!float_type) {
	float_type = MakeAtom(XATOM_FLOAT, strlen(XATOM_FLOAT), TRUE);
	if (!float_type) {
	    xf86Msg(X_WARNING, "%s: Failed to init float atom. "
			       "Disabling support for float properties.\n", pJstk->name);
	}
    }


#ifdef DEBUG
    /* Debug Level */
    prop_debuglevel = MakeAtom(JSTK_PROP_DEBUGLEVEL, strlen(JSTK_PROP_DEBUGLEVEL), TRUE);
    XIChangeDeviceProperty(pJstk, prop_debuglevel, XA_INTEGER, 8,
                                PropModeReplace, 1,
                                &debug_level,
                                FALSE);
    XISetDevicePropertyDeletable(pJstk, prop_debuglevel, FALSE);
#endif

    /* priv->num_buttons */
    prop_numbuttons = MakeAtom(JSTK_PROP_NUMBUTTONS, strlen(JSTK_PROP_NUMBUTTONS), TRUE);
    XIChangeDeviceProperty(pJstk, prop_numbuttons, XA_INTEGER, 8,
                                PropModeReplace, 1,
                                &priv->num_buttons,
                                FALSE);
    XISetDevicePropertyDeletable(pJstk, prop_numbuttons, FALSE);


    /* priv->num_axes */
    prop_numaxes = MakeAtom(JSTK_PROP_NUMAXES, strlen(JSTK_PROP_NUMAXES), TRUE);
    XIChangeDeviceProperty(pJstk, prop_numaxes, XA_INTEGER, 8,
                                PropModeReplace, 1,
                                &priv->num_axes,
                                FALSE);
    XISetDevicePropertyDeletable(pJstk, prop_numaxes, FALSE);


    /* priv->mouse_enabled */
    prop_mouse_enabled = MakeAtom(JSTK_PROP_MOUSE_ENABLED, strlen(JSTK_PROP_MOUSE_ENABLED), TRUE);
    XIChangeDeviceProperty(pJstk, prop_mouse_enabled, XA_INTEGER, 8,
                                PropModeReplace, 1,
                                &priv->mouse_enabled,
                                FALSE);
    XISetDevicePropertyDeletable(pJstk, prop_mouse_enabled, FALSE);

    /* priv->keys_enabled */
    prop_keys_enabled = MakeAtom(JSTK_PROP_KEYS_ENABLED, strlen(JSTK_PROP_KEYS_ENABLED), TRUE);
    XIChangeDeviceProperty(pJstk, prop_keys_enabled, XA_INTEGER, 8,
                                PropModeReplace, 1,
                                &priv->keys_enabled,
                                FALSE);
    XISetDevicePropertyDeletable(pJstk, prop_keys_enabled, FALSE);

    /* priv->axis[].deadzone */
    for (i=0;i<priv->num_axes;i++)
        axes_values32[i] = priv->axis[i].deadzone;
    prop_axis_deadzone = MakeAtom(JSTK_PROP_AXIS_DEADZONE, strlen(JSTK_PROP_AXIS_DEADZONE), TRUE);
    XIChangeDeviceProperty(pJstk, prop_axis_deadzone, XA_INTEGER, 32,
                                PropModeReplace, priv->num_axes,
                                axes_values32,
                                FALSE);
    XISetDevicePropertyDeletable(pJstk, prop_axis_deadzone, FALSE);

    /* priv->axis[].type */
    for (i=0;i<priv->num_axes;i++)
        axes_values8[i] = priv->axis[i].type;
    prop_axis_type = MakeAtom(JSTK_PROP_AXIS_TYPE, strlen(JSTK_PROP_AXIS_TYPE), TRUE);
    XIChangeDeviceProperty(pJstk, prop_axis_type, XA_INTEGER, 8,
                                PropModeReplace, priv->num_axes,
                                axes_values8,
                                FALSE);
    XISetDevicePropertyDeletable(pJstk, prop_axis_type, FALSE);

    /* priv->axis[].mapping */
    for (i=0;i<priv->num_axes;i++)
        axes_values8[i] = (INT8)priv->axis[i].mapping;
    prop_axis_mapping = MakeAtom(JSTK_PROP_AXIS_MAPPING, strlen(JSTK_PROP_AXIS_MAPPING), TRUE);
    XIChangeDeviceProperty(pJstk, prop_axis_mapping, XA_INTEGER, 8,
                                PropModeReplace, priv->num_axes,
                                axes_values8,
                                FALSE);
    XISetDevicePropertyDeletable(pJstk, prop_axis_mapping, FALSE);

    /* priv->axis[].amplify */
    if (float_type) {
	for (i=0;i<priv->num_axes;i++)
	    axes_floats[i] = priv->axis[i].amplify;
	prop_axis_amplify = MakeAtom(JSTK_PROP_AXIS_AMPLIFY, strlen(JSTK_PROP_AXIS_AMPLIFY), TRUE);
	XIChangeDeviceProperty(pJstk, prop_axis_amplify, float_type, 32,
				PropModeReplace, priv->num_axes, axes_floats,
				FALSE);
	XISetDevicePropertyDeletable(pJstk, prop_axis_amplify, FALSE);
    }

    /* priv->axis[].keys_low */
    for (i=0;i<priv->num_axes;i++)
        for (j=0;j<MAXKEYSPERBUTTON;j++)
            axes_values8[i*MAXKEYSPERBUTTON+j] = (INT8)priv->axis[i].keys_low[j];
    prop_axis_keys_low = MakeAtom(JSTK_PROP_AXIS_KEYS_LOW, strlen(JSTK_PROP_AXIS_KEYS_LOW), TRUE);
    XIChangeDeviceProperty(pJstk, prop_axis_keys_low, XA_INTEGER, 8,
                                PropModeReplace, priv->num_axes*MAXKEYSPERBUTTON,
                                axes_values8,
                                FALSE);
    XISetDevicePropertyDeletable(pJstk, prop_axis_keys_low, FALSE);

    /* priv->axis[].keys_high */
    for (i=0;i<priv->num_axes;i++)
        for (j=0;j<MAXKEYSPERBUTTON;j++)
            axes_values8[i*MAXKEYSPERBUTTON+j] = (INT8)priv->axis[i].keys_high[j];
    prop_axis_keys_high = MakeAtom(JSTK_PROP_AXIS_KEYS_HIGH, strlen(JSTK_PROP_AXIS_KEYS_HIGH), TRUE);
    XIChangeDeviceProperty(pJstk, prop_axis_keys_high, XA_INTEGER, 8,
                                PropModeReplace, priv->num_axes*MAXKEYSPERBUTTON,
                                axes_values8,
                                FALSE);
    XISetDevicePropertyDeletable(pJstk, prop_axis_keys_high, FALSE);


    /* priv->button[].mapping */
    for (i=0;i<priv->num_buttons;i++)
        button_values8[i] = (INT8)priv->button[i].mapping;
    prop_button_mapping = MakeAtom(JSTK_PROP_BUTTON_MAPPING, strlen(JSTK_PROP_BUTTON_MAPPING), TRUE);
    XIChangeDeviceProperty(pJstk, prop_button_mapping, XA_INTEGER, 8,
                                PropModeReplace, priv->num_buttons,
                                button_values8,
                                FALSE);
    XISetDevicePropertyDeletable(pJstk, prop_button_mapping, FALSE);

    /* priv->button[].buttonnumber */
    for (i=0;i<priv->num_buttons;i++) {
        int idx = priv->button[i].buttonnumber;
        if (idx>=0 && idx<=MAXBUTTONS)
            button_values8[i] = (CARD8)idx;
        else button_values8[i] = 0;
    }
    prop_button_buttonnumber = MakeAtom(JSTK_PROP_BUTTON_BUTTONNUMBER, strlen(JSTK_PROP_BUTTON_BUTTONNUMBER), TRUE);
    XIChangeDeviceProperty(pJstk, prop_button_buttonnumber, XA_INTEGER, 8,
                                PropModeReplace, priv->num_buttons,
                                button_values8,
                                FALSE);
    XISetDevicePropertyDeletable(pJstk, prop_button_buttonnumber, FALSE);

    /* priv->button[].amplify */
    if (float_type) {
	for (i=0;i<priv->num_buttons;i++)
	    button_floats[i] = priv->button[i].amplify;
	prop_button_amplify = MakeAtom(JSTK_PROP_BUTTON_AMPLIFY, strlen(JSTK_PROP_BUTTON_AMPLIFY), TRUE);
	XIChangeDeviceProperty(pJstk, prop_button_amplify, float_type, 32,
				PropModeReplace, priv->num_buttons, button_floats,
				FALSE);
	XISetDevicePropertyDeletable(pJstk, prop_button_amplify, FALSE);
    }

    /* priv->button[].keys */
    for (i=0;i<priv->num_buttons;i++)
        for (j=0;j<MAXKEYSPERBUTTON;j++)
            button_values8[i*MAXKEYSPERBUTTON+j] = (INT8)priv->button[i].keys[j];
    prop_button_keys = MakeAtom(JSTK_PROP_BUTTON_KEYS, strlen(JSTK_PROP_BUTTON_KEYS), TRUE);
    XIChangeDeviceProperty(pJstk, prop_button_keys, XA_INTEGER, 8,
                                PropModeReplace, priv->num_buttons*MAXKEYSPERBUTTON,
                                button_values8,
                                FALSE);
    XISetDevicePropertyDeletable(pJstk, prop_button_keys, FALSE);

    return TRUE;
}
