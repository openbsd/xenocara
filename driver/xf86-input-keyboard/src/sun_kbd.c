/*
 * Copyright 1990,91 by Thomas Roell, Dinkelscherben, Germany
 * Copyright 1993 by David Dawes <dawes@XFree86.org>
 * Copyright 1999 by David Holland <davidh@iquest.net)
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the names of Thomas Roell, David Dawes, and David Holland not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  Thomas Roell, David Dawes, and
 * David Holland make no representations about the suitability of this software
 * for any purpose.  It is provided "as is" without express or implied
 * warranty.
 *
 * THOMAS ROELL, DAVID DAWES, AND DAVID HOLLAND DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL THOMAS ROELL, DAVID DAWES, OR DAVID HOLLAND
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/* Copyright 2004-2007 Sun Microsystems, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
 * OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
 * INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * Except as contained in this notice, the name of a copyright holder
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * of the copyright holder.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86Priv.h"
#include "xf86_OSlib.h"
#include "xf86OSKbd.h"
#include "sun_kbd.h"
#include "atKeynames.h"

#include <sys/stropts.h>
#include <sys/vuid_event.h>
#include <sys/kbd.h>

static void
sunKbdSetLeds(InputInfoPtr pInfo, int leds)
{
    int i;

    SYSCALL(i = ioctl(pInfo->fd, KIOCSLED, &leds));
    if (i < 0) {
	xf86Msg(X_ERROR, "%s: Failed to set keyboard LED's: %s\n",
                pInfo->name, strerror(errno));
    }
}


static int
sunKbdGetLeds(InputInfoPtr pInfo)
{
    int i, leds = 0;

    SYSCALL(i = ioctl(pInfo->fd, KIOCGLED, &leds));
    if (i < 0) {
        xf86Msg(X_ERROR, "%s: Failed to get keyboard LED's: %s\n",
                pInfo->name, strerror(errno));
    }
    return leds;
}


/*
 * Save initial keyboard state.  This is called at the start of each server
 * generation.
 */
static int
KbdInit(InputInfoPtr pInfo, int what)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    sunKbdPrivPtr priv = (sunKbdPrivPtr) pKbd->private;
    pointer options = pInfo->options;
    
    int	ktype, klayout, i;
    const char *ktype_name;

    priv->otranslation 	= -1;
    priv->odirect 	= -1;

    if (options != NULL) {
	priv->strmod = xf86SetStrOption(options, "StreamsModule", NULL);
    } else {
	priv->strmod 		= NULL;
    }

    if (priv->strmod) {
	SYSCALL(i = ioctl(pInfo->fd, I_PUSH, priv->strmod));
	if (i < 0) {
	    xf86Msg(X_ERROR,
		    "%s: cannot push module '%s' onto keyboard device: %s\n",
		    pInfo->name, priv->strmod, strerror(errno));
	}
    }
    
    SYSCALL(i = ioctl(pInfo->fd, KIOCTYPE, &ktype));
    if (i < 0) {
	xf86Msg(X_ERROR, "%s: Unable to determine keyboard type: %s\n", 
		pInfo->name, strerror(errno));
	return BadImplementation;
    }
    
    SYSCALL(i = ioctl(pInfo->fd, KIOCLAYOUT, &klayout));
    if (i < 0) {	
	xf86Msg(X_ERROR, "%s: Unable to determine keyboard layout: %s\n", 
		pInfo->name, strerror(errno));
	return BadImplementation;
    }
    
    switch (ktype) {
    case KB_SUN3:
	ktype_name = "Sun Type 3"; break;
    case KB_SUN4:
	ktype_name = "Sun Type 4/5/6"; break;
    case KB_USB:
	ktype_name = "USB"; break;
    case KB_PC:
	ktype_name = "PC"; break;
    default:
	ktype_name = "Unknown"; break;
    }

    xf86Msg(X_PROBED, "%s: Keyboard type: %s (%d)\n",
	    pInfo->name, ktype_name, ktype);
    xf86Msg(X_PROBED, "%s: Keyboard layout: %d\n", pInfo->name, klayout);

    priv->ktype 	= ktype;
    priv->oleds 	= sunKbdGetLeds(pInfo);

    return Success;
}


static int
KbdOn(InputInfoPtr pInfo, int what)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    sunKbdPrivPtr priv = (sunKbdPrivPtr) pKbd->private;

    int	ktrans, kdirect, i;

    SYSCALL(i = ioctl(pInfo->fd, KIOCGDIRECT, &kdirect));
    if (i < 0) {
	xf86Msg(X_ERROR, 
		"%s: Unable to determine keyboard direct setting: %s\n", 
		pInfo->name, strerror(errno));
	return BadImplementation;
    }

    priv->odirect = kdirect;
    kdirect = 1;

    SYSCALL(i = ioctl(pInfo->fd, KIOCSDIRECT, &kdirect));
    if (i < 0) {
	xf86Msg(X_ERROR, "%s: Failed turning keyboard direct mode on: %s\n",
			pInfo->name, strerror(errno));
	return BadImplementation;
    }

    /* Setup translation */

    SYSCALL(i = ioctl(pInfo->fd, KIOCGTRANS, &ktrans));
    if (i < 0) {
	xf86Msg(X_ERROR, 
		"%s: Unable to determine keyboard translation mode: %s\n", 
		pInfo->name, strerror(errno));
	return BadImplementation;
    }

    priv->otranslation = ktrans;
    ktrans = TR_UNTRANS_EVENT;

    SYSCALL(i = ioctl(pInfo->fd, KIOCTRANS, &ktrans));
    if (i < 0) {	
	xf86Msg(X_ERROR, "%s: Failed setting keyboard translation mode: %s\n",
			pInfo->name, strerror(errno));
	return BadImplementation;
    }

    /* If Caps Lock or Num Lock LEDs are on when server starts,
     * send a fake key down on those keys to set the server state
     * to match the LED's.
     */
    if ( priv->oleds & (LED_CAPS_LOCK | LED_NUM_LOCK) ) {
	int capslock = -1;
	int numlock = -1;
	int j;
	TransMapPtr kmap = pKbd->scancodeMap;
	
	for (j = kmap->begin; j < kmap->end ; j++) {
	    switch (kmap->map[j]) {
	        case KEY_CapsLock:
		    capslock = j;
		    break;
	    	case KEY_NumLock:	
		    numlock = j;
		    break;
	        default:
		    /* nothing to do */
		    break;
	    }
	    if ((capslock >= 0) && (numlock >= 0)) {
		break;
	    }
	}
	if ((priv->oleds & LED_CAPS_LOCK) && (capslock > 0)) {
	    pKbd->PostEvent(pInfo, capslock, TRUE);	/* Press */
	    pKbd->PostEvent(pInfo, capslock, FALSE);	/* Release */
	}
	if ((priv->oleds & LED_NUM_LOCK) && (numlock > 0)) {
	    pKbd->PostEvent(pInfo, numlock, TRUE);	/* Press */
	    pKbd->PostEvent(pInfo, numlock, FALSE);	/* Release */
	}
    }
    
    return Success;
}

static int
KbdOff(InputInfoPtr pInfo, int what)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    sunKbdPrivPtr priv = (sunKbdPrivPtr) pKbd->private;

    int i;

    /* restore original state */
    
    sunKbdSetLeds(pInfo, priv->oleds);
    
    if (priv->otranslation != -1) {
        SYSCALL(i = ioctl(pInfo->fd, KIOCTRANS, &priv->otranslation));
	if (i < 0) {
	    xf86Msg(X_ERROR,
		    "%s: Unable to restore keyboard translation mode: %s\n",
		    pInfo->name, strerror(errno));
	    return BadImplementation;
	}
	priv->otranslation = -1;
    }

    if (priv->odirect != -1) {
        SYSCALL(i = ioctl(pInfo->fd, KIOCSDIRECT, &priv->odirect));
	if (i < 0) {
	    xf86Msg(X_ERROR,
		    "%s: Unable to restore keyboard direct setting: %s\n",
		    pInfo->name, strerror(errno));
	    return BadImplementation;
	}
	priv->odirect = -1;
    }

    if (priv->strmod) {
	SYSCALL(i = ioctl(pInfo->fd, I_POP, priv->strmod));
	if (i < 0) {
            xf86Msg(X_WARNING,
		    "%s: cannot pop module '%s' off keyboard device: %s\n",
		    pInfo->name, priv->strmod, strerror(errno));
	}
    }

    return Success;
}


static void
SoundKbdBell(InputInfoPtr pInfo, int loudness, int pitch, int duration)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    sunKbdPrivPtr priv = (sunKbdPrivPtr) pKbd->private;

    int	kbdCmd, i;

    if (loudness && pitch)
    {
 	kbdCmd = KBD_CMD_BELL;
		
	SYSCALL(i = ioctl (pInfo->fd, KIOCCMD, &kbdCmd));
	if (i < 0) {
	    xf86Msg(X_ERROR, "%s: Failed to activate bell: %s\n",
                pInfo->name, strerror(errno));
	}
	
	usleep(duration * loudness * 20);
	
	kbdCmd = KBD_CMD_NOBELL;
	SYSCALL(i = ioctl (pInfo->fd, KIOCCMD, &kbdCmd));
	if (i < 0) {
	     xf86Msg(X_ERROR, "%s: Failed to deactivate bell: %s\n",
                pInfo->name, strerror(errno));
	}
    }
}

static void
SetKbdLeds(InputInfoPtr pInfo, int leds)
{
    int real_leds = sunKbdGetLeds(pInfo);

    real_leds &= ~(LED_CAPS_LOCK | LED_NUM_LOCK | LED_SCROLL_LOCK | LED_COMPOSE);

    if (leds & XLED1)  real_leds |= LED_CAPS_LOCK;
    if (leds & XLED2)  real_leds |= LED_NUM_LOCK;
    if (leds & XLED3)  real_leds |= LED_SCROLL_LOCK;
    if (leds & XLED4)  real_leds |= LED_COMPOSE;
    
    sunKbdSetLeds(pInfo, real_leds);
}

static int
GetKbdLeds(InputInfoPtr pInfo)
{
    int leds = 0;
    int real_leds = sunKbdGetLeds(pInfo);

    if (real_leds & LED_CAPS_LOCK)	leds |= XLED1;
    if (real_leds & LED_NUM_LOCK)	leds |= XLED2;
    if (real_leds & LED_SCROLL_LOCK)	leds |= XLED3;
    if (real_leds & LED_COMPOSE)	leds |= XLED4;
	
    return leds;
}

/* ARGSUSED0 */
static void
SetKbdRepeat(InputInfoPtr pInfo, char rad)
{
    /* Nothing to do */
}

static void
ReadInput(InputInfoPtr pInfo)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    Firm_event event[64];
    int        nBytes, i;

    /* I certainly hope its not possible to read partial events */

    if ((nBytes = read(pInfo->fd, (char *)event, sizeof(event))) > 0)
    {
        for (i = 0; i < (nBytes / sizeof(Firm_event)); i++) {
	    pKbd->PostEvent(pInfo, event[i].id & 0xFF,
			    event[i].value == VKEY_DOWN ? TRUE : FALSE);
	}
    }
}

static Bool
OpenKeyboard(InputInfoPtr pInfo)
{
    const char *kbdPath = NULL;
    const char *defaultKbd = "/dev/kbd";

    if (pInfo->options != NULL) {
	kbdPath = xf86SetStrOption(pInfo->options, "Device", NULL);
    }
    if (kbdPath == NULL) {
        kbdPath = defaultKbd;
    }

    pInfo->fd = open(kbdPath, O_RDONLY | O_NONBLOCK);
    
    if (pInfo->fd == -1) {
        xf86Msg(X_ERROR, "%s: cannot open \"%s\"\n", pInfo->name, kbdPath);
    } else {
	xf86MsgVerb(X_INFO, 3, "%s: Opened device \"%s\"\n", pInfo->name,
		    kbdPath);
    }
    
    if ((kbdPath != NULL) && (kbdPath != defaultKbd)) {
	xfree(kbdPath);
    }

    if (pInfo->fd == -1) {
	return FALSE;
    } else {
	pInfo->read_input = ReadInput;
	return TRUE;
    }
}

_X_EXPORT Bool
xf86OSKbdPreInit(InputInfoPtr pInfo)
{
    KbdDevPtr pKbd = pInfo->private;

    pKbd->KbdInit       = KbdInit;
    pKbd->KbdOn         = KbdOn;
    pKbd->KbdOff        = KbdOff;
    pKbd->Bell          = SoundKbdBell;
    pKbd->SetLeds       = SetKbdLeds;
    pKbd->GetLeds       = GetKbdLeds;
    pKbd->SetKbdRepeat  = SetKbdRepeat;
    pKbd->KbdGetMapping = KbdGetMapping;

    pKbd->RemapScanCode = NULL;
    pKbd->GetSpecialKey = NULL;
    pKbd->SpecialKey    = NULL;

    pKbd->OpenKeyboard = OpenKeyboard;

    pKbd->vtSwitchSupported = FALSE;
    pKbd->CustomKeycodes = FALSE;

    pKbd->private = xcalloc(sizeof(sunKbdPrivRec), 1);
    if (pKbd->private == NULL) {
       xf86Msg(X_ERROR,"can't allocate keyboard OS private data\n");
       return FALSE;
    } else {
	sunKbdPrivPtr priv = (sunKbdPrivPtr) pKbd->private;
	priv->otranslation = -1;
	priv->odirect = -1;
    }

    return TRUE;
}
