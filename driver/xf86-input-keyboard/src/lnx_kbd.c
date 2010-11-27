
/*
 * Copyright (c) 2002 by The XFree86 Project, Inc.
 * Author: Ivan Pascal.
 *
 * Based on the code from lnx_io.c which is
 * Copyright 1992 by Orest Zborowski <obz@Kodak.com>
 * Copyright 1993 by David Dawes <dawes@xfree86.org>
 *
 * Portions based on kbdrate.c from util-linux 2.9t, which is
 * Copyright 1992 Rickard E. Faith.  Distributed under the GPL.
 * This program comes with ABSOLUTELY NO WARRANTY.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include <X11/X.h>

#include "compiler.h"

#include "xf86.h"
#include "xf86Priv.h"
#include "xf86_OSlib.h"

#include "xf86Xinput.h"
#include "xf86OSKbd.h"
#include "atKeynames.h"
#include "lnx_kbd.h"

#define KBC_TIMEOUT 250        /* Timeout in ms for sending to keyboard controller */

static KbdProtocolRec protocols[] = {
   {"standard", PROT_STD },
   { NULL, PROT_UNKNOWN_KBD }
};

extern Bool VTSwitchEnabled;

static void
SoundBell(InputInfoPtr pInfo, int loudness, int pitch, int duration)
{
	if (loudness && pitch)
	{
		ioctl(pInfo->fd, KDMKTONE,
		      ((1193190 / pitch) & 0xffff) |
		      (((unsigned long)duration *
			loudness / 50) << 16));
	}
}

static void
SetKbdLeds(InputInfoPtr pInfo, int leds)
{
    int real_leds = 0;

#ifdef LED_CAP
    if (leds & XLED1)  real_leds |= LED_CAP;
    if (leds & XLED2)  real_leds |= LED_NUM;
    if (leds & XLED3)  real_leds |= LED_SCR;
#ifdef LED_COMP
    if (leds & XLED4)  real_leds |= LED_COMP;
#else
    if (leds & XLED4)  real_leds |= LED_SCR;
#endif
#endif
    ioctl(pInfo->fd, KDSETLED, real_leds);
}

static int
GetKbdLeds(InputInfoPtr pInfo)
{
    char real_leds;
    int leds = 0;

    ioctl(pInfo->fd, KDGETLED, &real_leds);

    if (real_leds & LED_CAP) leds |= XLED1;
    if (real_leds & LED_NUM) leds |= XLED2;
    if (real_leds & LED_SCR) leds |= XLED3;

    return(leds);
}

typedef struct {
   int kbdtrans;
   struct termios kbdtty;
} LnxKbdPrivRec, *LnxKbdPrivPtr;

static int
KbdInit(InputInfoPtr pInfo, int what)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    LnxKbdPrivPtr priv = (LnxKbdPrivPtr) pKbd->private;

    if (pKbd->isConsole) {
        ioctl (pInfo->fd, KDGKBMODE, &(priv->kbdtrans));
        tcgetattr (pInfo->fd, &(priv->kbdtty));
    }
    if (!pKbd->CustomKeycodes) {
        pKbd->RemapScanCode = ATScancode;
    }

    return Success;
}

static int
KbdOn(InputInfoPtr pInfo, int what)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    LnxKbdPrivPtr priv = (LnxKbdPrivPtr) pKbd->private;
    struct termios nTty;

    if (pKbd->isConsole) {
	if (pKbd->CustomKeycodes)
	    ioctl(pInfo->fd, KDSKBMODE, K_MEDIUMRAW);
	else
	    ioctl(pInfo->fd, KDSKBMODE, K_RAW);

	nTty = priv->kbdtty;
	nTty.c_iflag = (IGNPAR | IGNBRK) & (~PARMRK) & (~ISTRIP);
	nTty.c_oflag = 0;
	nTty.c_cflag = CREAD | CS8;
	nTty.c_lflag = 0;
	nTty.c_cc[VTIME]=0;
	nTty.c_cc[VMIN]=1;
	cfsetispeed(&nTty, 9600);
	cfsetospeed(&nTty, 9600);
	tcsetattr(pInfo->fd, TCSANOW, &nTty);
    }
    return Success;
}

static int
KbdOff(InputInfoPtr pInfo, int what)
{
    KbdDevPtr pKbd = (KbdDevPtr) pInfo->private;
    LnxKbdPrivPtr priv = (LnxKbdPrivPtr) pKbd->private;

    if (pKbd->isConsole) {
	ioctl(pInfo->fd, KDSKBMODE, priv->kbdtrans);
	tcsetattr(pInfo->fd, TCSANOW, &(priv->kbdtty));
    }
    return Success;
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
        default:
           xf86Msg(X_ERROR,"\"%s\" is not a valid keyboard protocol name\n", s);
           free(s);
           return FALSE;
    }

    xf86Msg(X_CONFIG, "%s: Protocol: %s\n", pInfo->name, s);
    free(s);

    s = xf86SetStrOption(pInfo->options, "Device", NULL);
    if (s == NULL) {
       pInfo->fd = xf86Info.consoleFd;
       pKbd->isConsole = TRUE;
    } else {
       pInfo->fd = open(s, O_RDONLY | O_NONBLOCK | O_EXCL);
       if (pInfo->fd == -1) {
           xf86Msg(X_ERROR, "%s: cannot open \"%s\"\n", pInfo->name, s);
           free(s);
           return FALSE;
       }
       pKbd->isConsole = FALSE;
       free(s);
    }

    if (pKbd->isConsole)
         pKbd->vtSwitchSupported = TRUE;

    return TRUE;
}

_X_EXPORT Bool
xf86OSKbdPreInit(InputInfoPtr pInfo)
{
    KbdDevPtr pKbd = pInfo->private;

    pKbd->KbdInit       = KbdInit;
    pKbd->KbdOn         = KbdOn;
    pKbd->KbdOff        = KbdOff;
    pKbd->Bell          = SoundBell;
    pKbd->SetLeds       = SetKbdLeds;
    pKbd->GetLeds       = GetKbdLeds;
    pKbd->KbdGetMapping = KbdGetMapping;

    pKbd->RemapScanCode = NULL;

    pKbd->OpenKeyboard = OpenKeyboard;
    pKbd->vtSwitchSupported = FALSE;

    pKbd->private = calloc(sizeof(LnxKbdPrivRec), 1);
    if (pKbd->private == NULL) {
       xf86Msg(X_ERROR,"can't allocate keyboard OS private data\n");
       return FALSE;
    }

#if defined(__powerpc__)
  {
    FILE *f;
    f = fopen("/proc/sys/dev/mac_hid/keyboard_sends_linux_keycodes","r");
    if (f) {
        if (fgetc(f) == '0')
            pKbd->CustomKeycodes = TRUE;
        fclose(f);
    }
  }
#endif
    return TRUE;
}
