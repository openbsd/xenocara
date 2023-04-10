/*
 * Copyright 2007 - 2008    by Sascha Hlusiak. <saschahlusiak@freedesktop.org>
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <xf86.h>
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include "jstk.h"
#include "jstk_options.h"




/***********************************************************************
 *
 * jstkGetAxisMapping --
 *
 * Parses strings like:
 * x, +y, -zx, 3x, 3.5zy, -8x 
 * And returns the mapping and stores the optional factor
 * In the float referenced by 'value'
 *
 ***********************************************************************
 */

static JSTK_MAPPING
jstkGetAxisMapping(float *value, const char* param, const char* name) 
{
    if (sscanf(param, "%f", value)==0) {
        if (param[0] == '-')
            *value *= -1.0;
    }
    if (strstr(param, "key") != NULL)
        return JSTK_MAPPING_KEY;
    else if (strstr(param, "zx") != NULL)
        return JSTK_MAPPING_ZX;
    else if (strstr(param, "zy") != NULL)
        return JSTK_MAPPING_ZY;
    else if (strstr(param, "x") != NULL)
        return JSTK_MAPPING_X;
    else if (strstr(param, "y") != NULL)
        return JSTK_MAPPING_Y;

    return JSTK_MAPPING_NONE;
}


/***********************************************************************
 *
 * jstkParseButtonOption --
 *
 * Interprets one ButtonMappingX option, given in 'org'
 * stores the result in *button
 * name is the name of the InputDevice
 *
 ***********************************************************************
 */

void
jstkParseButtonOption(const char* org,
                      JoystickDevPtr priv,
                      int number,
                      const char* name)
{
    char *param;
    int value;
    float fvalue;
    char p[64];
    BUTTON* button;

    button = &priv->button[number];

    param = xstrdup(org);
/*    for (tmp = param; *tmp; tmp++) *tmp = tolower(*tmp); */

    if (strcmp(param, "none") == 0) {
        button->mapping = JSTK_MAPPING_NONE;
    } else if (sscanf(param, "button=%d", &value) == 1) {
        if (value<0 || value >BUTTONMAP_SIZE) {
            xf86Msg(X_WARNING, "%s: button number out of range (0..%d): %d.\n", 
                    name, BUTTONMAP_SIZE,  value);
        } else {
            button->mapping      = JSTK_MAPPING_BUTTON;
            button->buttonnumber = value;
        }
    } else if (sscanf(param, "axis=%15s", p) == 1) {
        p[15]='\0';
        fvalue = 1.0f;
        button->mapping = jstkGetAxisMapping(&fvalue, p, name);
        button->amplify = fvalue;
        button->currentspeed = 1.0f;
        if (button->mapping == JSTK_MAPPING_NONE)
            xf86Msg(X_WARNING, "%s: error parsing axis: %s.\n", 
                    name, p);
    } else if (sscanf(param, "amplify=%f", &fvalue) == 1) {
        button->mapping = JSTK_MAPPING_SPEED_MULTIPLY;
        button->amplify = fvalue;
    } else if (sscanf(param, "key=%30s", p) == 1) {
        char *current, *next;
        p[30]='\0';
        current = p;
        button->mapping = JSTK_MAPPING_KEY;

        for (value = 0; value < MAXKEYSPERBUTTON; value++) if (current != NULL) {
            unsigned int key;
            next = strchr(current, ',');
            if (!next) next = strchr(current, '+');
            if (next) *(next++) = '\0';
            key = strtol(current, NULL, 0);
            DBG(3, ErrorF("Parsed %s to %d\n", current, key));
            if (key == 0)
                xf86Msg(X_WARNING, "%s: error parsing key value: %s.\n", 
                        name, current);
            else {
                button->keys[value] = key;
            }
            current = next;
        } else button->keys[value] = 0;
    } else if (strcmp(param, "disable-all") == 0) {
        button->mapping = JSTK_MAPPING_DISABLE;
    } else if (strcmp(param, "disable-mouse") == 0) {
        button->mapping = JSTK_MAPPING_DISABLE_MOUSE;
    } else if (strcmp(param, "disable-keys") == 0) {
        button->mapping = JSTK_MAPPING_DISABLE_KEYS;
    } else {
        xf86Msg(X_WARNING, "%s: error parsing button parameter.\n", 
                name);
    }
    free(param);
}


/***********************************************************************
 *
 * jstkParseAxisOption --
 *
 * Interprets one AxisMappingX option, given in 'org'
 * stores the result in *axis
 * name is the name of the InputDevice
 *
 ***********************************************************************
 */

void
jstkParseAxisOption(const char* org, 
                    JoystickDevPtr priv,
                    AXIS *axis,
                    const char *name)
{
    char *param;
    char *tmp;
    int value;
    float fvalue;
    char p[64];
    param = xstrdup(org);
/*    for (tmp     for (tmp = param; *tmp; tmp++) *tmp = tolower(*tmp);
= param; *tmp; tmp++) *tmp = tolower(*tmp); */

    if ((tmp=strstr(param, "mode=")) != NULL) {
        if (sscanf(tmp, "mode=%15s", p) == 1) {
            p[15] = '\0';
            if (strcmp(p, "relative") == 0) {
                axis->type = JSTK_TYPE_BYVALUE;
            } else if (strcmp(p, "accelerated") == 0) {
                axis->type = JSTK_TYPE_ACCELERATED;
                axis->currentspeed = 1.0f;
            } else if (strcmp(p, "absolute") == 0) {
                axis->type = JSTK_TYPE_ABSOLUTE;
            } else if (strcmp(p, "none") == 0) {
                axis->type = JSTK_TYPE_NONE;
            } else {
                axis->type = JSTK_TYPE_NONE;
                xf86Msg(X_WARNING, "%s: \"%s\": error parsing mode.\n", 
                        name, param);
            }
        } else xf86Msg(X_WARNING, "%s: \"%s\": error parsing mode.\n", 
                       name, param);
    }

    if ((tmp = strstr(param, "axis=")) != NULL) {
        if (sscanf(tmp, "axis=%15s", p) == 1) {
            p[15] = '\0';
            fvalue = 1.0f;
            axis->mapping = jstkGetAxisMapping(&fvalue, p, name);
            if ((axis->type == JSTK_TYPE_ABSOLUTE) &&
                ((fvalue <= 1.1)&&(fvalue >= -1.1))) {
                if (axis->mapping == JSTK_MAPPING_X)
                    fvalue *= (int)screenInfo.screens[0]->width;
                if (axis->mapping == JSTK_MAPPING_Y)
                    fvalue *= (int)screenInfo.screens[0]->height;
            }
            axis->amplify = fvalue;
            if (axis->mapping == JSTK_MAPPING_NONE)
                xf86Msg(X_WARNING, "%s: error parsing axis: %s.\n",
                        name, p);
        }else xf86Msg(X_WARNING, "%s: error parsing axis.\n",
                      name);
    }

    if ((tmp = strstr(param, "valuator")) != NULL ) {
        axis->valuator = 0; /* Will be renumbered appropriately on DEVICE_INIT */
    }

    if ((tmp = strstr(param, "keylow=")) != NULL) {
        if (sscanf(tmp, "keylow=%30s", p) == 1) {
            char *current, *next;
            unsigned int key;
            p[30]='\0';
            current = p;
            axis->mapping = JSTK_MAPPING_KEY;
            for (value = 0; value < MAXKEYSPERBUTTON; value++) 
                if (current != NULL) {
                    next = strchr(current, ',');
		    if (!next) next = strchr(current, '+');
                    if (next) *(next++) = '\0';

                    key = strtol(current, NULL, 0);
                    DBG(3, ErrorF("Parsed %s to %d\n", current, key));
                    if (key == 0)
                        xf86Msg(X_WARNING, "%s: error parsing keylow value: %s.\n", 
                                name, current);
                    else {
                        axis->keys_low[value] = key;
                    }
                    current = next;
                } else axis->keys_low[value] = 0;
        }
    }

    if ((tmp = strstr(param, "keyhigh=")) != NULL) {
        if (sscanf(tmp, "keyhigh=%30s", p) == 1) {
            char *current, *next;
            unsigned int key;
            p[30]='\0';
            current = p;
            axis->mapping = JSTK_MAPPING_KEY;
            for (value = 0; value < MAXKEYSPERBUTTON; value++) 
                if (current != NULL) {
                    next = strchr(current, ',');
                    if (!next) next = strchr(current, '+');
                    if (next) *(next++) = '\0';
                    key = strtol(current, NULL, 0);
                    key = strtol(current, NULL, 0);
                    DBG(3, ErrorF("Parsed %s to %d\n", current, key));
                    if (key == 0)
                        xf86Msg(X_WARNING, "%s: error parsing keyhigh value: %s.\n", 
                                name, current);
                    else {
                        axis->keys_high[value] = key;
                    }
                    current = next;
                } else axis->keys_high[value] = 0;
        }
    }

    if ((tmp = strstr(param, "deadzone=")) != NULL ) {
        if (sscanf(tmp, "deadzone=%d", &value) == 1) {
            value = (value < 0) ? (-value) : (value);
            if (value > 30000)
                xf86Msg(X_WARNING, 
                        "%s: deadzone of %d seems unreasonable. Ignored.\n", 
                        name, value);
            else axis->deadzone = value;
        }else xf86Msg(X_WARNING, "%s: error parsing deadzone.\n", 
                      name);
    }
    free(param);
}
