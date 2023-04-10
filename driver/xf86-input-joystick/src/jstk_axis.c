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
#include <xf86Xinput.h>
#include <xf86_OSproc.h>
#include <math.h>
#include <stdlib.h>

#include "jstk.h"
#include "jstk_axis.h"
#include "jstk_key.h"


/***********************************************************************
 *
 * jstkAxisTimer --
 *
 * The timer that will generate PointerMove-events. Checks every axis
 * and every button for it's mapping.
 * Return 0, when timer can be stopped, because there is no active
 * movement
 *
 ***********************************************************************
 */
static CARD32
jstkAxisTimer(OsTimerPtr        timer,
              CARD32            atime,
              pointer           arg)
{
    #define NEXTTIMER 15
    DeviceIntPtr          device = (DeviceIntPtr)arg;
    InputInfoPtr          pInfo = device->public.devicePrivate;
    JoystickDevPtr        priv  = pInfo->private;

    int sigstate, i;
    int nexttimer;
    int movex,movey,movezx,movezy;

    nexttimer = 0;
    movex = movey = movezx = movezy = 0;

    sigstate = xf86BlockSIGIO();

    for (i=0; i<MAXAXES; i++) if ((priv->axis[i].value != 0) &&
                                  (priv->axis[i].type != JSTK_TYPE_NONE)) {
        float p1 = 0.0f;     /* Pixels to move cursor */
        float p2 = 0.0f;     /* Pixels to scroll */
        float scale;
        AXIS *axis;
        axis = &priv->axis[i];

        nexttimer = NEXTTIMER;

        if (priv->axis[i].type == JSTK_TYPE_BYVALUE) {
            /* Calculate scale value, so we get a range from 0 to 32768 */
            scale = (32768.0f / (float)(32768 - axis->deadzone));

	    /* NOTE: joysticks with a rectangular field have a
	     * corner position of (32768,32768), joysticks with a
	     * circular field have (23170,23170).
	     *
	     * make sure that diagonal movement feels fast. either:
	     * 1) linear
	     * 
	     *   f(32768) ~= f(23170) + f(23170)
	     *   f(32768) ~= a * f(23170)
	     *          a  = 2.0
	     *
	     *   on circular joysticks, the time needed for xy movement is
	     *   exactly the time needed for x + the time for y separately.
	     *   absolute diagonal travel speed (in cm/s) is 0.707 times as fast, 
	     *   which feels pretty slow.
	     *
	     *   on square joysticks, diagonal travel speed is always 1.41 times
	     *   faster than orthogonal travel speed. time needed for diagonal
	     *   movement is always 0.5 times as long as for orthogonal movement.
	     * 
	     *   the value of a = 2.0 results in a nice, non-linear acceleration.
	     *
	     * or
	     * 2) trigonometric
	     * 
	     *   f(32768) ~= sqrt(f(23170)^2 + f(23170)^2))
	     *   f(32768) ~= a * f(23170)
	     *           a = 1.414
	     *
	     *   on circular joysticks, the absolute pointer travel speed
	     *   (in cm/s) is now the same for both linear and diagonal movement,
	     *   which feels natural. moving diagonally takes 0.707 times the time
	     *   of moving orthogonally.
	     * 
	     *   on square joysticks, values are as in 1)
	     * 
	     *   the value of a = 1.414 results in linear acceleration, which feels
	     *   too slow.
	     * 
	     * to maintain non-linear acceleration, make sure that:
	     *
	     *   a >>= 1.414 
	     *
	     * the following formula achieves results in between,
	     * so it should feel natural on both devices while maintaining a
	     * nice acceleration:
	     * 
	     *   f(32768) ~= 1.620 * f(23170)
	     * 
	     * TODO: make this simpler by using only values -1.0..1.0 and
	     *       provide acceleration graphs.
	     */

            /* How many pixels should this axis move the cursor */
            p1 = (pow((abs((float)axis->value) - (float)axis->deadzone) *
                  scale / 23, 1.4f) + 100.0f) *
                 ((float)NEXTTIMER / 40000.0f);
            /* How many "pixels" should this axis scroll */
            p2 = ((pow((abs((float)axis->value) - (float)axis->deadzone) *
                  scale / 1000.0f, 2.5f)) + 200.0f) *
                 ((float)NEXTTIMER / 200000.0f);
        } else if (axis->type == JSTK_TYPE_ACCELERATED) {
            /* Stop to accelerate at a certain speed */
            if (axis->currentspeed < 100.0f)
               axis->currentspeed = (axis->currentspeed + 3.0f) * 1.07f - 3.0f;
            p1 = axis->currentspeed * (float)NEXTTIMER / 180.0f;
            p2 = p1 / 8.0f;
        }
        if (axis->value < 0) {
            p1 = -p1;
            p2 = -p2;
        }
        p1 *= axis->amplify * priv->amplify;
        p2 *= axis->amplify * priv->amplify;

        /* Apply movement to global amount of pixels to move */
        switch (axis->mapping) {
        case JSTK_MAPPING_X:
        case JSTK_MAPPING_Y:
            axis->subpixel += p1;
            break;
        case JSTK_MAPPING_ZX:
        case JSTK_MAPPING_ZY:
        case JSTK_MAPPING_KEY:
            axis->subpixel += p2;
            break;
        default:
            break;
        }
        if ((int)axis->subpixel != 0) {
            switch (axis->mapping) {
            case JSTK_MAPPING_X:
                movex += (int)axis->subpixel;
                break;
            case JSTK_MAPPING_Y:
                movey += (int)axis->subpixel;
                break;
            case JSTK_MAPPING_ZX:
                movezx += (int)axis->subpixel;
                break;
            case JSTK_MAPPING_ZY:
                movezy += (int)axis->subpixel;
                break;

            case JSTK_MAPPING_KEY: if ((priv->keys_enabled == TRUE) && 
                                  (priv->axis[i].type == JSTK_TYPE_BYVALUE)) {
                int num;
                num = abs((int)axis->subpixel);
                if ((int)axis->subpixel < 0) {
                    for (i=0; i<num; i++) {
                        jstkGenerateKeys(priv->keyboard_device, axis->keys_low, 1);
                        jstkGenerateKeys(priv->keyboard_device, axis->keys_low, 0);
                    }
                } else {
                    for (i=0; i<num; i++) {
                        jstkGenerateKeys(priv->keyboard_device, axis->keys_high, 1);
                        jstkGenerateKeys(priv->keyboard_device, axis->keys_high, 0);
                    }
                }
                break;
            }
            default:
                break;
            }
            axis->subpixel = axis->subpixel - (int)axis->subpixel;
        }
    }

    for (i=0; i<MAXBUTTONS; i++) if (priv->button[i].pressed == 1) {
        float p1;
        float p2;

        if (priv->button[i].currentspeed < 100.0f)
            priv->button[i].currentspeed = 
                (priv->button[i].currentspeed + 3.0f) * 1.07f - 3.0f;
        p1 = priv->button[i].currentspeed * (float)NEXTTIMER / 180.0f *
            priv->button[i].amplify;
        p1 *= priv->amplify;
        p2 = p1 / 8.0f;

        /* Apply movement to amount of pixels to move */
        switch (priv->button[i].mapping) {
        case JSTK_MAPPING_X:
        case JSTK_MAPPING_Y:
            priv->button[i].subpixel += p1;
            nexttimer = NEXTTIMER;
            break;
        case JSTK_MAPPING_ZX:
        case JSTK_MAPPING_ZY:
            priv->button[i].subpixel += p2;
            nexttimer = NEXTTIMER;
            break;
        default:
            break;
        }
        if ((int)priv->button[i].subpixel != 0) {
            switch (priv->button[i].mapping) {
            case JSTK_MAPPING_X:
                movex += (int)priv->button[i].subpixel;
                break;
            case JSTK_MAPPING_Y:
                movey += (int)priv->button[i].subpixel;
                break;
            case JSTK_MAPPING_ZX:
                movezx += (int)priv->button[i].subpixel;
                break;
            case JSTK_MAPPING_ZY:
                movezy += (int)priv->button[i].subpixel;
                break;
            default:
                break;
            }
            priv->button[i].subpixel -= (int)priv->button[i].subpixel;
        }
    }

    /* Actually move the cursor, if there is enough movement in the buffer */
    if ((movex != 0)||(movey != 0)) {
        xf86PostMotionEvent(device, 0, 0, 2, movex, movey);
    }

    /* Generate scrolling events */
    while (movezy >= 1) {  /* down */
        xf86PostButtonEvent(device, 0, 5,
                            1, 0, 0);
        xf86PostButtonEvent(device, 0, 5,
                            0, 0, 0);
        movezy -= 1;
    }
    while (movezy <= -1) { /* up */
        xf86PostButtonEvent(device, 0, 4,
                            1, 0, 0);
        xf86PostButtonEvent(device, 0, 4,
                            0, 0, 0);
        movezy += 1;
    }

    while (movezx >= 1) {  /* right */
        xf86PostButtonEvent(device, 0, 7, 
                            1, 0, 0);
        xf86PostButtonEvent(device, 0, 7, 
                            0, 0, 0);
        movezx -= 1;
    }
    while (movezx <= -1) { /* left */
        xf86PostButtonEvent(device, 0, 6,
                            1, 0, 0);
        xf86PostButtonEvent(device, 0, 6,
                            0, 0, 0);
        movezx += 1;
    }

    if ((priv->mouse_enabled == FALSE) &&
        (priv->keys_enabled == FALSE))
        nexttimer = 0;

    if (nexttimer == 0) { /* No next timer (no subpixel added), stop */
        priv->timerrunning = FALSE;

        for (i=0; i<MAXBUTTONS; i++) priv->button[i].subpixel = 0.0f;
        for (i=0; i<MAXAXES; i++) priv->axis[i].subpixel = 0.0f;

        DBG(2, ErrorF("Stopping Axis Timer\n"));
    }
    xf86UnblockSIGIO (sigstate);
    return nexttimer;
}


/***********************************************************************
 *
 * jstkStartAxisTimer --
 *
 * Starts the timer for the movement.
 * Will already prepare for moving one pixel, for "tipping" the stick
 *
 ***********************************************************************
 */
void
jstkStartAxisTimer(InputInfoPtr device, int number)
{
    int pixel;
    JoystickDevPtr priv = device->private;

    if (priv->timerrunning) return;
    priv->timerrunning = TRUE;

    pixel = 1;
    if (priv->axis[number].value < 0) pixel = -1;
    priv->axis[number].subpixel += pixel;

    DBG(2, ErrorF("Starting Axis Timer (triggered by axis %d)\n", number));
    priv->timer = TimerSet(
        priv->timer, 
        0,         /* Relative */
        1,         /* What about NOW? */
        jstkAxisTimer,
        device->dev);
}

/***********************************************************************
 *
 * jstkStartButtonAxisTimer --
 *
 * Starts the timer for the movement.
 * Will already prepare for moving one pixel, for "tipping" the stick
 *
 ***********************************************************************
 */

void
jstkStartButtonAxisTimer(InputInfoPtr device, int number)
{
    int pixel;
    JoystickDevPtr priv = device->private;

    if (priv->timerrunning) return;
    priv->timerrunning = TRUE;

    pixel = 1;
    if (priv->button[number].amplify < 0) pixel = -1;
    switch (priv->button[number].mapping) {
    case JSTK_MAPPING_X:
    case JSTK_MAPPING_Y:
    case JSTK_MAPPING_ZX:
    case JSTK_MAPPING_ZY:
        priv->button[number].subpixel += pixel;
        break;
    default:
        break;
    }

    DBG(2, ErrorF("Starting Axis Timer (triggered by button %d)\n", number));
    priv->timer = TimerSet(
        priv->timer, 
        0,         /* Relative */
        1,         /* What about NOW? */
        jstkAxisTimer,
        device->dev);
}

/***********************************************************************
 *
 * jstkHandleAbsoluteAxis --
 *
 * Sums up absolute movement of all axes and sets the cursor to the
 * desired Position on the screen. 
 *
 ***********************************************************************
 */
void
jstkHandleAbsoluteAxis(InputInfoPtr device, int number)
{
    JoystickDevPtr priv = device->private;
    int i,x,y;

    x=0;
    y=0;

    for (i=0; i<MAXAXES; i++) 
        if (priv->axis[i].type == JSTK_TYPE_ABSOLUTE)
    {
        float rel;
        int dif;
        
    	rel = 0.0f;
        if (priv->axis[i].value > +priv->axis[i].deadzone)
            rel = (priv->axis[i].value - priv->axis[i].deadzone);
        if (priv->axis[i].value < -priv->axis[i].deadzone)
            rel = (priv->axis[i].value + priv->axis[i].deadzone);

        rel = (rel) / (2.0f * (float)(32768 - priv->axis[i].deadzone));
        /* rel contains numbers between -0.5 and +0.5 now */

        rel *= priv->axis[i].amplify;

        DBG(5, ErrorF("Relative Position of axis %d: %.2f\n", i, rel));

        /* Calculate difference to previous position on screen in pixels */
        dif = (int)(rel - priv->axis[i].previousposition + 0.5f);
        if ((dif >= 1)||(dif <= -1)) {
            if (priv->axis[i].mapping == JSTK_MAPPING_X) {
                x += (dif);
                priv->axis[i].previousposition += (float)dif;
            }
            if (priv->axis[i].mapping == JSTK_MAPPING_Y) {
                y += (int)(dif);
                priv->axis[i].previousposition += (float)dif;
            }
        }
    }
    /* Still move relative, but relative to previous position of the axis */
    if ((x != 0) || (y != 0)) {
        DBG(4, ErrorF("Moving mouse by %dx%d pixels\n", x, y));
        xf86PostMotionEvent(device->dev, 0, 0, 2, x, y);
    }
}





/***********************************************************************
 *
 * jstkPWMAxisTimer --
 *
 * The timer that will generate Key events.
 * The deflection of the axis will control the PERCENT OF TIME the key is
 * down, not the amount of impulses.
 * Return 0, when timer can be stopped.
 *
 ***********************************************************************
 */
static CARD32
jstkPWMAxisTimer(OsTimerPtr        timer,
                 CARD32            atime,
                 pointer           arg)
{
    DeviceIntPtr          device = (DeviceIntPtr)arg;
    InputInfoPtr          pInfo = device->public.devicePrivate;
    JoystickDevPtr        priv  = pInfo->private;

    int sigstate, i;
    int nexttimer;

    nexttimer = 0;

    sigstate = xf86BlockSIGIO();

    for (i=0; i<MAXAXES; i++)
        if (priv->axis[i].timer == timer) /* The timer handles only one axis! Find it. */
    {
        AXIS *axis;
        axis = &priv->axis[i];

        DBG(8, ErrorF("PWM Axis %d value %d (old %d)\n", i, axis->value, axis->oldvalue));

        /* Force key_high down if centered */
        if ((axis->value <= 0) && 
            (axis->oldvalue > 0) &&
            (axis->key_isdown)) 
        {
            DBG(7, ErrorF("PWM Axis %d jumped over. Forcing keys_high up.\n", i));
            jstkGenerateKeys(priv->keyboard_device, 
                axis->keys_high,
                0);
            axis->key_isdown = 0;
        }

        /* Force key_low down if centered */
        if ((axis->value >= 0) && 
            (axis->oldvalue < 0) &&
            (axis->key_isdown))
        {
            DBG(7, ErrorF("PWM Axis %d jumped over. Forcing keys_low up.\n", i));
            jstkGenerateKeys(priv->keyboard_device, 
                axis->keys_low,
                0);
            axis->key_isdown = 0;
        }

        if (axis->value == 0)
            nexttimer = 0;
        else {
            float time_on, time_off;
            float scale;
            KEYSCANCODES *keys;

            if (axis->value < 0)
                keys = &axis->keys_low;
            else keys = &axis->keys_high;

            /* Calculate next timer */
            time_on = (float)(abs(axis->value) - axis->deadzone) / 32768.0;
            time_on *= (32768.0f / (float)(32768 - axis->deadzone));

            time_off = 1.0f - time_on;

            time_on  += 0.01f; /* Ugly but ensures we don't divide by 0 */
            time_off += 0.01f;

            /* Scale both durations, so the smaller always is 50ms */
            scale = 50.0f * axis->amplify;

            if (time_on < time_off)
                scale /= time_on;
            else scale /= time_off;

            time_on  *= scale;
            time_off *= scale;


            if (time_off > 600.0f) {
                /* Might as well just have it down forever */
                DBG(7, ErrorF("PWM Axis %d up time too long (%.0fms). Forcing up)\n", i, time_off));
                if (axis->key_isdown == 1) {
                    axis->key_isdown = 0;
                    jstkGenerateKeys(priv->keyboard_device,
                        *keys,
                        axis->key_isdown);
                }
                nexttimer = 0;
            } else if (time_on > 600.0f) { 
                /* Might as well just have it up forever */
                DBG(7, ErrorF("PWM Axis %d down time too long (%.0fms). Forcing down)\n", i, time_on));
                if (axis->key_isdown == 0) {
                    axis->key_isdown = 1;
                    jstkGenerateKeys(priv->keyboard_device,
                        *keys,
                        axis->key_isdown);
                }
                nexttimer = 0;
            } else {
                /* Flip key state */
                axis->key_isdown = 1 - axis->key_isdown;
                jstkGenerateKeys(priv->keyboard_device,
                    *keys,
                    axis->key_isdown);

                DBG(7, ErrorF("PWM Axis %d state=%d (%.0fms down, %.0fms up).\n", i, axis->key_isdown, time_on, time_off));

                nexttimer = axis->key_isdown ? (int)time_on : (int)time_off;
            }
        }

        if (nexttimer == 0) { /* No next timer, stop */
            axis->timerrunning = FALSE;

            DBG(2, ErrorF("Stopping PWM Axis %d Timer\n", i));
        }
        axis->oldvalue = axis->value;
        break;
    }

    xf86UnblockSIGIO (sigstate);
    return nexttimer;
}


/***********************************************************************
 *
 * jstkStartAxisTimer --
 *
 * Starts the timer for the movement.
 * Will already prepare for moving one pixel, for "tipping" the stick
 *
 ***********************************************************************
 */
void
jstkHandlePWMAxis(InputInfoPtr device, int number)
{
    JoystickDevPtr priv = device->private;
    if (priv->axis[number].timerrunning) return;

    priv->axis[number].timerrunning = TRUE;

    DBG(2, ErrorF("Starting PWM Axis Timer (triggered by axis %d, value %d)\n", 
               number, priv->axis[number].value));
    priv->axis[number].timer = TimerSet(
        priv->axis[number].timer, 
        0,         /* Relative */
        1,         /* What about NOW? */
        jstkPWMAxisTimer,
        device->dev);
}
