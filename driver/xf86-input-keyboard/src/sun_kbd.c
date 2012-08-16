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
/*
 * Copyright (c) 2004-2009, Oracle and/or its affiliates. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include "xf86.h"
#include "xf86Priv.h"
#include "xf86_OSlib.h"
#include "xf86OSKbd.h"
#include "sun_kbd.h"

#include <sys/stropts.h>
#include <sys/vuid_event.h>
#include <sys/kbd.h>
#include <sys/note.h>	/* needed before including older versions of hid.h */
#include <sys/usb/clients/hid/hid.h>

static int KbdOn(InputInfoPtr pInfo, int what);
static Bool OpenKeyboard(InputInfoPtr pInfo);
static void CloseKeyboard(InputInfoPtr pInfo);

static void
sunKbdSetLeds(InputInfoPtr pInfo, int leds)
{
    int i;
    uchar_t setleds = (uchar_t) (leds & 0xFF);

    SYSCALL(i = ioctl(pInfo->fd, KIOCSLED, &setleds));
    if (i < 0) {
	xf86Msg(X_ERROR, "%s: Failed to set keyboard LED's: %s\n",
                pInfo->name, strerror(errno));
    }
}


static int
sunKbdGetLeds(InputInfoPtr pInfo)
{
    int i;
    uchar_t leds = 0;

    SYSCALL(i = ioctl(pInfo->fd, KIOCGLED, &leds));
    if (i < 0) {
        xf86Msg(X_ERROR, "%s: Failed to get keyboard LED's: %s\n",
                pInfo->name, strerror(errno));
    }
    return (int) leds;
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

    priv->kbdActive	= FALSE;
    priv->otranslation 	= -1;
    priv->odirect 	= -1;

    if (options != NULL) {
	priv->strmod = xf86SetStrOption(options, "StreamsModule", NULL);
    } else {
	priv->strmod 		= NULL;
    }

    i = KbdOn(pInfo, DEVICE_INIT);
    if (i != Success) {
	return i;
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

    return Success;
}


static int
KbdOn(InputInfoPtr pInfo, int what)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    sunKbdPrivPtr priv = (sunKbdPrivPtr) pKbd->private;

    int	ktrans, kdirect, i;
    int io_get_direct = KIOCGDIRECT;
    int io_set_direct = KIOCSDIRECT;

    if (priv->kbdActive) {
	return Success;
    }

    if (pInfo->fd == -1) {
	if (!OpenKeyboard(pInfo)) {
	    return BadImplementation;
	}
    }

    if (priv->strmod) {
	/* Check to see if module is already pushed */
	SYSCALL(i = ioctl(pInfo->fd, I_FIND, priv->strmod));

	if (i == 0) { /* Not already pushed */
	    SYSCALL(i = ioctl(pInfo->fd, I_PUSH, priv->strmod));
	    if (i < 0) {
		xf86Msg(X_ERROR, "%s: cannot push module '%s' onto "
			"keyboard device: %s\n",
			pInfo->name, priv->strmod, strerror(errno));
	    }
	}

#ifdef HIDIOCKMSDIRECT
	if (strcmp(priv->strmod, "usbkbm") == 0) {
	    io_get_direct = HIDIOCKMGDIRECT;
	    io_set_direct = HIDIOCKMSDIRECT;
	}
#endif
    }

    SYSCALL(i = ioctl(pInfo->fd, io_get_direct, &kdirect));
    if (i < 0) {
	xf86Msg(X_ERROR, 
		"%s: Unable to determine keyboard direct setting: %s\n", 
		pInfo->name, strerror(errno));
	return BadImplementation;
    }

    priv->odirect = kdirect;
    kdirect = 1;

    SYSCALL(i = ioctl(pInfo->fd, io_set_direct, &kdirect));
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

    priv->oleds	= sunKbdGetLeds(pInfo);

    /* Allocate here so we don't alloc in ReadInput which may be called
       from SIGIO handler. */
    priv->remove_timer = TimerSet(priv->remove_timer, 0, 0, NULL, NULL);

    priv->kbdActive = TRUE;
    return Success;
}

static int
KbdOff(InputInfoPtr pInfo, int what)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    sunKbdPrivPtr priv = (sunKbdPrivPtr) pKbd->private;

    int i;
    int io_set_direct, kdirect;

    if (priv->remove_timer) {
	TimerFree(priv->remove_timer);
	priv->remove_timer = NULL;
    }

    if (!priv->kbdActive) {
	return Success;
    }

    if (pInfo->fd == -1) {
	priv->kbdActive = FALSE;
	return Success;
    }

    /* restore original state */

    if (priv->oleds != -1) {
	sunKbdSetLeds(pInfo, priv->oleds);
	priv->oleds = -1;
    }
    
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

    io_set_direct = KIOCSDIRECT;
    kdirect = priv->odirect;

#ifdef HIDIOCKMSDIRECT
    if ((priv->strmod != NULL) && (strcmp(priv->strmod, "usbkbm") == 0)) {
	io_set_direct = HIDIOCKMSDIRECT;
	kdirect = 0;
    }
#endif

    if (kdirect != -1) {
	SYSCALL(i = ioctl(pInfo->fd, io_set_direct, &kdirect));
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

    CloseKeyboard(pInfo);
    return Success;
}


static void
SoundKbdBell(InputInfoPtr pInfo, int loudness, int pitch, int duration)
{
    int	kbdCmd, i;
#ifdef KIOCMKTONE
    int cycles;
    int mktonevalue;
#endif

    if (loudness && pitch)
    {
#ifdef KIOCMKTONE
	if (pitch == 0)
	    cycles = UINT16_MAX;
	else if (pitch >= UINT16_MAX)
	    cycles = 0;
	else {
	    cycles = (PIT_HZ + pitch / 2) / pitch;
	    if (cycles > UINT16_MAX)
		cycles = UINT16_MAX;
	}

	mktonevalue = cycles | (((duration * loudness * 20) / 1000) << 16);

	errno = 0;
	SYSCALL(i = ioctl (pInfo->fd, KIOCMKTONE, mktonevalue));
	if (i == 0)
	    return;

	if (errno != EINVAL) {
	    if (errno != EAGAIN)
		xf86Msg(X_ERROR, "%s: Failed to activate bell: %s\n",
			pInfo->name, strerror(errno));
	    return;
	}
#endif

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

static void
CloseKeyboard(InputInfoPtr pInfo)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    sunKbdPrivPtr priv = (sunKbdPrivPtr) pKbd->private;

    close(pInfo->fd);
    pInfo->fd = -1;
    priv->kbdActive = FALSE;
}

/* Called from OsTimer callback, since removing a device from the device
   list or changing pInfo->fd while xf86Wakeup is looping through the list
   causes server crashes */
static CARD32
RemoveKeyboard(OsTimerPtr timer, CARD32 time, pointer arg)
{
    InputInfoPtr pInfo = (InputInfoPtr) arg;

    CloseKeyboard(pInfo);
    xf86DisableDevice(pInfo->dev, TRUE);

    return 0;  /* All done, don't set to run again */
}

static void
ReadInput(InputInfoPtr pInfo)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    sunKbdPrivPtr priv = (sunKbdPrivPtr) pKbd->private;
    Firm_event event[64];
    int        nBytes, i;

    while (TRUE) {
	/* I certainly hope it's not possible to read partial events */
	nBytes = read(pInfo->fd, (char *)event, sizeof(event));
	if (nBytes > 0) {
	    for (i = 0; i < (nBytes / sizeof(Firm_event)); i++) {
		pKbd->PostEvent(pInfo, event[i].id & 0xFF,
				event[i].value == VKEY_DOWN ? TRUE : FALSE);
	    }
	} else if (nBytes == -1) {
	    switch (errno) {
		case EAGAIN: /* Nothing to read now */
		    return;
		case EINTR:  /* Interrupted, try again */
		    break;
		case ENODEV: /* May happen when USB kbd is unplugged */
		    /* We use X_NONE here because it doesn't alloc since we
		       may be called from SIGIO handler */
		    xf86MsgVerb(X_NONE, 0,
				"%s: Device no longer present - removing.\n",
				pInfo->name);
		    xf86RemoveEnabledDevice(pInfo);
		    priv->remove_timer = TimerSet(priv->remove_timer, 0, 1,
						  RemoveKeyboard, pInfo);
		    return;
		default:     /* All other errors */
		    /* We use X_NONE here because it doesn't alloc since we
		       may be called from SIGIO handler */
		    xf86MsgVerb(X_NONE, 0, "%s: Read error: %s\n", pInfo->name,
				strerror(errno));
		    return;
	    }
	} else { /* nBytes == 0, so nothing more to read */
	    return;
	}
    }
}

static Bool
OpenKeyboard(InputInfoPtr pInfo)
{
    char *kbdPath = xf86SetStrOption(pInfo->options, "Device", "/dev/kbd");
    Bool ret;

    pInfo->fd = open(kbdPath, O_RDONLY | O_NONBLOCK);
    
    if (pInfo->fd == -1) {
	xf86Msg(X_ERROR, "%s: cannot open \"%s\"\n", pInfo->name, kbdPath);
	ret = FALSE;
    } else {
	xf86MsgVerb(X_INFO, 3, "%s: Opened device \"%s\"\n", pInfo->name,
		    kbdPath);
	pInfo->read_input = ReadInput;
	ret = TRUE;
	/* in case it wasn't set and we fell back to default */
	xf86ReplaceStrOption(pInfo->options, "Device", kbdPath);
    }

    free(kbdPath);
    return ret;
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
    pKbd->KbdGetMapping = KbdGetMapping;

    pKbd->RemapScanCode = NULL;

    pKbd->OpenKeyboard = OpenKeyboard;

    pKbd->private = calloc(sizeof(sunKbdPrivRec), 1);
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
