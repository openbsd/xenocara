
/*
 * Copyright (c) 2002 by The XFree86 Project, Inc.
 * Author: Ivan Pascal.
 *
 * Based on the code from bsd_io.c which is
 * Copyright 1992 by Rich Murphey <Rich@Rice.edu>
 * Copyright 1993 by David Dawes <dawes@xfree86.org>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include <X11/X.h>
#include <termios.h>

#include "compiler.h"

#include "xf86.h"
#include "xf86Priv.h"
#include "xf86_OSlib.h"

#include "xf86Xinput.h"
#include "xf86OSKbd.h"
#include "atKeynames.h"
#include "bsd_kbd.h"

extern int priv_open_device(const char *dev);

static KbdProtocolRec protocols[] = {
   {"standard", PROT_STD },
#ifdef WSCONS_SUPPORT
   {"wskbd", PROT_WSCONS },
#endif
   { NULL, PROT_UNKNOWN_KBD }
};

typedef struct {
   struct termios kbdtty;
} BsdKbdPrivRec, *BsdKbdPrivPtr;

#ifdef WSCONS_SUPPORT
static Bool
WSSetVersion(int fd, const char *name)
{
#ifdef WSKBDIO_SETVERSION
    int version = WSKBDIO_EVENT_VERSION;
    if (ioctl(fd, WSKBDIO_SETVERSION, &version) == -1) {
        xf86Msg(X_WARNING, "%s: cannot set version\n", name);
        return FALSE;
    }
#endif
    return TRUE;
}
#endif

static
int KbdInit(InputInfoPtr pInfo, int what)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    BsdKbdPrivPtr priv = (BsdKbdPrivPtr) pKbd->private;

    if (pKbd->isConsole) {
        switch (pKbd->consType) {
#if defined(PCCONS_SUPPORT) || defined(SYSCONS_SUPPORT) || defined (PCVT_SUPPORT)  || defined (WSCONS_SUPPORT)
	    case PCCONS:
	    case SYSCONS:
	    case PCVT:
#if defined WSCONS_SUPPORT
            case WSCONS:
#endif
		tcgetattr(pInfo->fd, &(priv->kbdtty));
#endif
	         break;
        }
    }

    return Success;
}

static void
SetKbdLeds(InputInfoPtr pInfo, int leds)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    int real_leds = 0;

#ifdef LED_CAP
    if (leds & XLED1)  real_leds |= LED_CAP;
#endif
#ifdef LED_NUM
    if (leds & XLED2)  real_leds |= LED_NUM;
#endif
#ifdef LED_SCR
    if (leds & XLED3)  real_leds |= LED_SCR;
    if (leds & XLED4)  real_leds |= LED_SCR;
#endif

    switch (pKbd->consType) {

#ifdef PCCONS_SUPPORT
	case PCCONS:
		break;
#endif
#if defined (SYSCONS_SUPPORT) || defined (PCVT_SUPPORT)
	case SYSCONS:
	case PCVT:
	     ioctl(pInfo->fd, KDSETLED, real_leds);
	     break;
#endif
#if defined(WSCONS_SUPPORT)
        case WSCONS:
             ioctl(pInfo->fd, WSKBDIO_SETLEDS, &real_leds);
             break;
#endif
    }
}

static int
GetKbdLeds(InputInfoPtr pInfo)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    int leds = 0, real_leds = 0;

    switch (pKbd->consType) {

#ifdef PCCONS_SUPPORT
	case PCCONS:
	     break;
#endif
#if defined (SYSCONS_SUPPORT) || defined (PCVT_SUPPORT)
	case SYSCONS:
	case PCVT:
	     ioctl(pInfo->fd, KDGETLED, &real_leds);
	     break;
#endif
#if defined(WSCONS_SUPPORT)
        case WSCONS:
             ioctl(pInfo->fd, WSKBDIO_GETLEDS, &real_leds);
             break;
#endif
    }

#ifdef LED_CAP
    if (real_leds & LED_CAP) leds |= XLED1;
#endif
#ifdef LED_NUM
    if (real_leds & LED_NUM) leds |= XLED2;
#endif
#ifdef LED_SCR
    if (real_leds & LED_SCR) leds |= XLED3;
#endif

    return(leds);
}

static int
KbdOn(InputInfoPtr pInfo, int what)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
#if defined(SYSCONS_SUPPORT) || defined(PCCONS_SUPPORT) || defined(PCVT_SUPPORT) || defined(WSCONS_SUPPORT)
    BsdKbdPrivPtr priv = (BsdKbdPrivPtr) pKbd->private;
    struct termios nTty;
#endif
#ifdef WSCONS_SUPPORT
    int option;
#endif

    if (pKbd->isConsole) {
        switch (pKbd->consType) {

#if defined(SYSCONS_SUPPORT) || defined(PCCONS_SUPPORT) || defined(PCVT_SUPPORT) || defined(WSCONS_SUPPORT)
	    case SYSCONS:
	    case PCCONS:
	    case PCVT:
#ifdef WSCONS_SUPPORT
            case WSCONS:
#endif
		 nTty = priv->kbdtty;
		 nTty.c_iflag = IGNPAR | IGNBRK;
		 nTty.c_oflag = 0;
		 nTty.c_cflag = CREAD | CS8;
		 nTty.c_lflag = 0;
		 nTty.c_cc[VTIME] = 0;
		 nTty.c_cc[VMIN] = 1;
		 cfsetispeed(&nTty, 9600);
		 cfsetospeed(&nTty, 9600);
		 if (tcsetattr(pInfo->fd, TCSANOW, &nTty) < 0) {
			 xf86Msg(X_ERROR, "KbdOn: tcsetattr: %s\n",
			     strerror(errno));
		 }
                 break;
#endif
        }
#if defined (SYSCONS_SUPPORT) || defined (PCVT_SUPPORT) || defined (WSCONS_SUPPORT)
        switch (pKbd->consType) {
	    case SYSCONS:
	    case PCVT:
#ifdef K_CODE
                 if (pKbd->CustomKeycodes)
		     ioctl(pInfo->fd, KDSKBMODE, K_CODE);
	         else
	             ioctl(pInfo->fd, KDSKBMODE, K_RAW);
#else
		 ioctl(pInfo->fd, KDSKBMODE, K_RAW);
#endif
	         break;
#ifdef WSCONS_SUPPORT
            case WSCONS:
                 option = WSKBD_RAW;
                 if (ioctl(pInfo->fd, WSKBDIO_SETMODE, &option) == -1) {
			 FatalError("can't switch keyboard to raw mode. "
				    "Enable support for it in the kernel\n"
				    "or use for example:\n\n"
				    "Option \"Protocol\" \"wskbd\"\n"
				    "Option \"Device\" \"/dev/wskbd0\"\n"
				    "\nin your xorg.conf(5) file\n");
		 }
		 break;
#endif
        }
#endif
    } else {
        switch (pKbd->consType) {
#ifdef WSCONS_SUPPORT
            case WSCONS:
		if ((pKbd->wsKbdDev[0] != 0) && (pInfo->fd == -1)) {
			xf86Msg(X_INFO, "opening %s\n", pKbd->wsKbdDev);
			pInfo->fd = open(pKbd->wsKbdDev, O_RDONLY | O_NONBLOCK | O_EXCL);
			if (pInfo->fd == -1) {
				xf86Msg(X_ERROR, "cannot open \"%s\"\n", pKbd->wsKbdDev);
				return FALSE;
			}
			if (WSSetVersion(pInfo->fd, pInfo->name) == FALSE)
				return FALSE;
		}
		break;
#endif
	}
    }
    return Success;
}

static int
KbdOff(InputInfoPtr pInfo, int what)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    BsdKbdPrivPtr priv = (BsdKbdPrivPtr) pKbd->private;
#ifdef WSCONS_SUPPORT
    int option;
#endif

    if (pKbd->isConsole) {
        switch (pKbd->consType) {
#if defined (SYSCONS_SUPPORT) || defined (PCVT_SUPPORT)
	    case SYSCONS:
	    case PCVT:
	         ioctl(pInfo->fd, KDSKBMODE, K_XLATE);
	         /* FALL THROUGH */
#endif
#if defined(SYSCONS_SUPPORT) || defined(PCCONS_SUPPORT) || defined(PCVT_SUPPORT)
	    case PCCONS:
	         tcsetattr(pInfo->fd, TCSANOW, &(priv->kbdtty));
	         break;
#endif
#ifdef WSCONS_SUPPORT
            case WSCONS:
                 option = WSKBD_TRANSLATED;
                 ioctl(xf86Info.consoleFd, WSKBDIO_SETMODE, &option);
                 tcsetattr(pInfo->fd, TCSANOW, &(priv->kbdtty));
	         break;
#endif
        }
    } else {
         switch (pKbd->consType) {
#ifdef WSCONS_SUPPORT
            case WSCONS:
                 if ((pKbd->wsKbdDev[0] != 0) && (pInfo->fd != -1)) {
			xf86Msg(X_INFO, "closing %s\n", pKbd->wsKbdDev);
			/* need to close the fd while we're gone */
			close(pInfo->fd);
			pInfo->fd = -1;
                 }
	         break;
#endif
        }
    }
    return Success;
}

static void
SoundBell(InputInfoPtr pInfo, int loudness, int pitch, int duration)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
#ifdef WSCONS_SUPPORT
    struct wskbd_bell_data wsb;
#endif

    if (loudness && pitch) {
	switch (pKbd->consType) {
#ifdef PCCONS_SUPPORT
	    case PCCONS:
	         { int data[2];
		   data[0] = pitch;
		   data[1] = (duration * loudness) / 50;
		   ioctl(pInfo->fd, CONSOLE_X_BELL, data);
		   break;
		 }
#endif
#if defined (SYSCONS_SUPPORT) || defined (PCVT_SUPPORT)
	    case SYSCONS:
	    case PCVT:
		 ioctl(pInfo->fd, KDMKTONE,
		 ((1193190 / pitch) & 0xffff) |
		 (((unsigned long)duration*loudness/50)<<16));
		 break;
#endif
#if defined (WSCONS_SUPPORT)
            case WSCONS:
                 wsb.which = WSKBD_BELL_DOALL;
                 wsb.pitch = pitch;
                 wsb.period = duration;
                 wsb.volume = loudness;
                 ioctl(pInfo->fd, WSKBDIO_COMPLEXBELL, &wsb);
                 break;
#endif
	}
    }
}

static void
stdReadInput(InputInfoPtr pInfo)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    unsigned char rBuf[64];
    int nBytes, i;
    if ((nBytes = read( pInfo->fd, (char *)rBuf, sizeof(rBuf))) > 0) {
       for (i = 0; i < nBytes; i++)
	   pKbd->PostEvent(pInfo, rBuf[i] & 0x7f,
                           rBuf[i] & 0x80 ? FALSE : TRUE);
       }
}

#ifdef WSCONS_SUPPORT

static void
WSReadInput(InputInfoPtr pInfo)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    struct wscons_event events[64];
    int type;
    int blocked, n, i;

    if ((n = read( pInfo->fd, events, sizeof(events))) > 0) {
        n /=  sizeof(struct wscons_event);
        for (i = 0; i < n; i++) {
	    type = events[i].type;
	    if (type == WSCONS_EVENT_KEY_UP || type == WSCONS_EVENT_KEY_DOWN) {
		/* It seems better to block SIGIO there */
		blocked = xf86BlockSIGIO();
		pKbd->PostEvent(pInfo, (unsigned int)(events[i].value),
				type == WSCONS_EVENT_KEY_DOWN ? TRUE : FALSE);
		xf86UnblockSIGIO(blocked);
	    }
	} /* for */
    }
}

static void
printWsType(const char *type, const char *name)
{
    xf86Msg(X_PROBED, "%s: Keyboard type: %s\n", name, type);
}
#endif

static Bool
OpenKeyboard(InputInfoPtr pInfo)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    int i;
    KbdProtocolId prot = PROT_UNKNOWN_KBD;
    char *s;

    s = xf86SetStrOption(pInfo->options, "Protocol", NULL);
    for (i = 0; protocols[i].name; i++) {
        if (xf86NameCmp(s, protocols[i].name) == 0) {
           prot = protocols[i].id;
           break;
        }
    }

    switch (prot) {
	case PROT_STD:
           pInfo->read_input = stdReadInput;
           break;
#ifdef WSCONS_SUPPORT
        case PROT_WSCONS:
           pInfo->read_input = WSReadInput;
           break;
#endif
        default:
           xf86Msg(X_ERROR,"\"%s\" is not a valid keyboard protocol name\n", s);
           free(s);
           return FALSE;
    }
    free(s);

    if (prot == PROT_WSCONS)
	s = xf86SetStrOption(pInfo->options, "Device", "/dev/wskbd");
    else
	s = xf86SetStrOption(pInfo->options, "Device", NULL);

    if (s == NULL) {
	pInfo->fd = xf86Info.consoleFd;
	pKbd->isConsole = TRUE;
	pKbd->consType = xf86Info.consType;
	pKbd->wsKbdDev[0] = 0;
    } else {
#ifndef X_PRIVSEP
	pInfo->fd = open(s, O_RDONLY | O_NONBLOCK | O_EXCL);
#else
	pInfo->fd = priv_open_device(s);
#endif
	if (pInfo->fd == -1) {
           xf86Msg(X_ERROR, "%s: cannot open \"%s\"\n", pInfo->name, s);
           free(s);
           return FALSE;
       }
       pKbd->isConsole = FALSE;
       strncpy(pKbd->wsKbdDev, s, 256);
       pKbd->consType = xf86Info.consType;
       free(s);
    }

#ifdef WSCONS_SUPPORT
    if (prot == PROT_WSCONS) {
       pKbd->consType = WSCONS;
       if (WSSetVersion(pInfo->fd, pInfo->name) == FALSE)
	   return FALSE;
       /* Find out keyboard type */
       if (ioctl(pInfo->fd, WSKBDIO_GTYPE, &(pKbd->wsKbdType)) == -1) {
           xf86Msg(X_ERROR, "%s: cannot get keyboard type", pInfo->name);
           close(pInfo->fd);
           return FALSE;
       }
       switch (pKbd->wsKbdType) {
           case WSKBD_TYPE_PC_XT:
               printWsType("XT", pInfo->name);
               break;
           case WSKBD_TYPE_PC_AT:
               printWsType("AT", pInfo->name);
               break;
           case 0:
               /* If wsKbdType==0, no keyboard attached to the mux. Assume USB. */
               xf86Msg(X_WARNING, "%s: No keyboard attached, assuming USB\n",
                                  pInfo->name);
               pKbd->wsKbdType = WSKBD_TYPE_USB;
               /* FALLTHROUGH */
           case WSKBD_TYPE_USB:
               printWsType("USB", pInfo->name);
               break;
#ifdef WSKBD_TYPE_ADB
           case WSKBD_TYPE_ADB:
               printWsType("ADB", pInfo->name);
               break;
#endif
#ifdef WSKBD_TYPE_LK201
           case WSKBD_TYPE_LK201:
               printWsType("LK201", pInfo->name);
               break;
#endif
#ifdef WSKBD_TYPE_LK401
           case WSKBD_TYPE_LK401:
               printWsType("LK-401", pInfo->name);
               break;
#endif
#ifdef WSKBD_TYPE_MAPLE
           case WSKBD_TYPE_MAPLE:
               printWsType("Maple", pInfo->name);
               break;
#endif
#ifdef WSKBD_TYPE_SUN
           case WSKBD_TYPE_SUN:
               printWsType("Sun", pInfo->name);
               break;
#endif
#ifdef WSKBD_TYPE_SUN5
           case WSKBD_TYPE_SUN5:
               printWsType("Sun5", pInfo->name);
               break;
#endif
           default:
               xf86Msg(X_WARNING, "%s: Unsupported wskbd type \"%d\"\n",
                                  pInfo->name, pKbd->wsKbdType);
               printWsType("Unknown wskbd", pInfo->name);
               break;
       }
    }
#endif
    return TRUE;
}

_X_EXPORT Bool
xf86OSKbdPreInit(InputInfoPtr pInfo)
{
    KbdDevPtr pKbd = pInfo->private;

    pKbd->KbdInit	= KbdInit;
    pKbd->KbdOn		= KbdOn;
    pKbd->KbdOff	= KbdOff;
    pKbd->Bell		= SoundBell;
    pKbd->SetLeds	= SetKbdLeds;
    pKbd->GetLeds	= GetKbdLeds;
    pKbd->KbdGetMapping	= KbdGetMapping;

    pKbd->RemapScanCode = NULL;

    pKbd->OpenKeyboard = OpenKeyboard;

    pKbd->private = calloc(sizeof(BsdKbdPrivRec), 1);
    if (pKbd->private == NULL) {
       xf86Msg(X_ERROR,"can't allocate keyboard OS private data\n");
       return FALSE;
    }
    return TRUE;
}
