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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <xf86.h>
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include "jstk.h"
#include "jstk_options.h"
#include "StrKeysym.h"





/***********************************************************************
 *
 * jstkGetButtonNumberInMap --
 *
 * Adds a button number to the button map and returns the index
 *
 ***********************************************************************
 */

int
jstkGetButtonNumberInMap(JoystickDevPtr priv,
                         int buttonnumber)
{
    int j;
    for (j=1; j<=priv->buttonmap.size; j++)
        if (priv->buttonmap.map[j] == buttonnumber)
            break;
    if (j > MAXBUTTONS+1) return 0;
    priv->buttonmap.map[j] = buttonnumber;
    if (j > priv->buttonmap.size) priv->buttonmap.size = j;
    return j;
}



/***********************************************************************
 *
 * jstkGetKeyNumberInMap --
 *
 * Adds a KeySym to the keymap and returns the index
 *
 ***********************************************************************
 */

int
jstkGetKeyNumberInMap(JoystickDevPtr priv,
                      KeySym keysym)
{
    int j;
    for (j=0; j<priv->keymap.size; j++)
        if (priv->keymap.map[j] == keysym)
            break;
    if (j >= sizeof(priv->keymap.map)/sizeof(priv->keymap.map[0])) return 0;
    priv->keymap.map[j] = keysym;
    if (j + 1 > priv->keymap.size) priv->keymap.size = j + 1;
    return j;
}


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

static JOYSTICKMAPPING
jstkGetAxisMapping(float *value, const char* param, const char* name) 
{
    if (sscanf(param, "%f", value)==0) {
        if (param[0] == '-')
            *value *= -1.0;
    }
    if (strstr(param, "key") != NULL)
        return MAPPING_KEY;
    else if (strstr(param, "zx") != NULL)
        return MAPPING_ZX;
    else if (strstr(param, "zy") != NULL)
        return MAPPING_ZY;
    else if (strstr(param, "x") != NULL)
        return MAPPING_X;
    else if (strstr(param, "y") != NULL)
        return MAPPING_Y;

    return MAPPING_NONE;
}


/***********************************************************************
 *
 * jstkParseButtonOption --
 *
 * Interpretes one ButtonMappingX option, given in 'org'
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
        button->mapping = MAPPING_NONE;
    } else if (sscanf(param, "button=%d", &value) == 1) {
        button->mapping      = MAPPING_BUTTON;
        button->buttonnumber = jstkGetButtonNumberInMap(priv, value);
    } else if (sscanf(param, "axis=%15s", p) == 1) {
        p[15]='\0';
        fvalue = 1.0f;
        button->mapping = jstkGetAxisMapping(&fvalue, p, name);
        button->amplify = fvalue;
        button->currentspeed = 1.0f;
        if (button->mapping == MAPPING_NONE)
            xf86Msg(X_WARNING, "%s: error parsing axis: %s.\n", 
                    name, p);
    } else if (sscanf(param, "amplify=%f", &fvalue) == 1) {
        button->mapping = MAPPING_SPEED_MULTIPLY;
        button->amplify = fvalue;
    } else if (sscanf(param, "key=%30s", p) == 1) {
        char *current, *next;
        p[30]='\0';
        current = p;
        button->mapping = MAPPING_KEY;

        for (value = 0; value < MAXKEYSPERBUTTON; value++) if (current != NULL) {
            unsigned key;
            next = strchr(current, ',');
	    if (!next) next = strchr(current, '+');
            if (next) *(next++) = '\0';
#ifdef _STRKEYSYM_H_INCLUDED_
            key = XStringToKeysym(current);
            if (key == NoSymbol)
#endif
                key = strtol(current, NULL, 0);
            DBG(3, ErrorF("Parsed %s to %d\n", current, key));
            if (key == 0)
                xf86Msg(X_WARNING, "%s: error parsing key value: %s.\n", 
                        name, current);
            else {
                button->keys[value] = jstkGetKeyNumberInMap(priv, key);
            }
            current = next;
        } else button->keys[value] = 0;
    } else if (strcmp(param, "disable-all") == 0) {
        button->mapping = MAPPING_DISABLE;
    } else if (strcmp(param, "disable-mouse") == 0) {
        button->mapping = MAPPING_DISABLE_MOUSE;
    } else if (strcmp(param, "disable-keys") == 0) {
        button->mapping = MAPPING_DISABLE_KEYS;
    } else {
        xf86Msg(X_WARNING, "%s: error parsing button parameter.\n", 
                name);
    }
    xfree(param);
}


/***********************************************************************
 *
 * jstkParseAxisOption --
 *
 * Interpretes one AxisMappingX option, given in 'org'
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
                axis->type = TYPE_BYVALUE;
            } else if (strcmp(p, "accelerated") == 0) {
                axis->type = TYPE_ACCELERATED;
                axis->currentspeed = 1.0f;
            } else if (strcmp(p, "absolute") == 0) {
                axis->type = TYPE_ABSOLUTE;
            } else if (strcmp(p, "none") == 0) {
                axis->type = TYPE_NONE;
            } else {
                axis->type = TYPE_NONE;
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
            if ((axis->type == TYPE_ABSOLUTE) &&
                ((fvalue <= 1.1)&&(fvalue >= -1.1))) {
                if (axis->mapping == MAPPING_X)
                    fvalue *= (int)screenInfo.screens[0]->width;
                if (axis->mapping == MAPPING_Y)
                    fvalue *= (int)screenInfo.screens[0]->height;
            }
            axis->amplify = fvalue;
            if (axis->mapping == MAPPING_NONE)
                xf86Msg(X_WARNING, "%s: error parsing axis: %s.\n",
                        name, p);
        }else xf86Msg(X_WARNING, "%s: error parsing axis.\n",
                      name);
    }

    if ((tmp = strstr(param, "keylow=")) != NULL) {
        if (sscanf(tmp, "keylow=%30s", p) == 1) {
            char *current, *next;
            unsigned int key;
            p[30]='\0';
            current = p;
            axis->mapping = MAPPING_KEY;
            for (value = 0; value < MAXKEYSPERBUTTON; value++) 
                if (current != NULL) {
                    next = strchr(current, ',');
		    if (!next) next = strchr(current, '+');
                    if (next) *(next++) = '\0';

#ifdef _STRKEYSYM_H_INCLUDED_
                    key = XStringToKeysym(current);
                    if (key == NoSymbol)
#endif
                        key = strtol(current, NULL, 0);
                    DBG(3, ErrorF("Parsed %s to %d\n", current, key));
                    if (key == 0)
                        xf86Msg(X_WARNING, "%s: error parsing keylow value: %s.\n", 
                                name, current);
                    else {
                        axis->keys_low[value] = jstkGetKeyNumberInMap(priv, key);
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
            axis->mapping = MAPPING_KEY;
            for (value = 0; value < MAXKEYSPERBUTTON; value++) 
                if (current != NULL) {
                    next = strchr(current, ',');
		    if (!next) next = strchr(current, '+');
                    if (next) *(next++) = '\0';
                    key = strtol(current, NULL, 0);
#ifdef _STRKEYSYM_H_INCLUDED_
                    key = XStringToKeysym(current);
                    if (key == NoSymbol)
#endif
                        key = strtol(current, NULL, 0);
                    DBG(3, ErrorF("Parsed %s to %d\n", current, key));
                    if (key == 0)
                        xf86Msg(X_WARNING, "%s: error parsing keyhigh value: %s.\n", 
                                name, current);
                    else {
                        axis->keys_high[value] = jstkGetKeyNumberInMap(priv, key);
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
    xfree(param);
}
