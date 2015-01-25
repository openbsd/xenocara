/*
 * Copyright (c) 2004-2005, 2008-2010, Oracle and/or its affiliates.
 * All rights reserved.
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
/*
 * Copyright 1999-2001 The XFree86 Project, Inc.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the XFree86 Project shall
 * not be used in advertising or otherwise to promote the sale, use or other
 * dealings in this Software without prior written authorization from the
 * XFree86 Project.
 */

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#include "xorg-server.h"
#include "xf86.h"
#include "xf86_OSlib.h"
#include "mouse.h"
#include "xisb.h"
#include "mipointer.h"
#include "xf86Crtc.h"
#include <sys/stropts.h>
#include <sys/vuid_event.h>
#include <sys/msio.h>

/* Wheel mouse support in VUID drivers in Solaris 9 updates & Solaris 10 */
#ifdef WHEEL_DEVID /* Defined in vuid_event.h if VUID wheel support present */
# define HAVE_VUID_WHEEL
#endif
#ifdef HAVE_VUID_WHEEL
# include <sys/vuid_wheel.h>
#endif

/* Support for scaling absolute coordinates to screen size in
 * Solaris 10 updates and beyond */
#if !defined(HAVE_ABSOLUTE_MOUSE_SCALING)
# ifdef MSIOSRESOLUTION /* Defined in msio.h if scaling support present */
#  define HAVE_ABSOLUTE_MOUSE_SCALING
# endif
#endif

/* Names of protocols that are handled internally here. */

static const char *internalNames[] = {
        "VUID",
        NULL
};

static const char *solarisMouseDevs[] = {
    /* Device file:     Protocol:                       */
    "/dev/mouse",       "VUID",         /* USB or SPARC */
#if defined(__i386) || defined(__x86)
    "/dev/kdmouse",     "PS/2",         /* PS/2 */
#endif
    NULL
};

typedef struct _VuidMseRec {
    struct _VuidMseRec *next;
    InputInfoPtr        pInfo;
    Firm_event          event;
    unsigned char *     buffer;
    char *              strmod;
    Bool(*wrapped_device_control)(DeviceIntPtr device, int what);
#ifdef HAVE_ABSOLUTE_MOUSE_SCALING
    Ms_screen_resolution         absres;
#endif
    OsTimerPtr          remove_timer;   /* Callback for removal on ENODEV */
} VuidMseRec, *VuidMsePtr;

static VuidMsePtr       vuidMouseList = NULL;

static int  vuidMouseProc(DeviceIntPtr pPointer, int what);
static void vuidReadInput(InputInfoPtr pInfo);

#ifdef HAVE_ABSOLUTE_MOUSE_SCALING
# include "compat-api.h"

static void vuidMouseSendScreenSize(ScreenPtr pScreen, VuidMsePtr pVuidMse);
static void vuidMouseAdjustFrame(ADJUST_FRAME_ARGS_DECL);

static int vuidMouseGeneration = 0;

#if HAS_DEVPRIVATEKEYREC
static DevPrivateKeyRec vuidMouseScreenIndex;
#else
static int vuidMouseScreenIndex;
#endif /* HAS_DEVPRIVATEKEYREC */

#define vuidMouseGetScreenPrivate(s) ( \
    dixLookupPrivate(&(s)->devPrivates, &vuidMouseScreenIndex))
#define vuidMouseSetScreenPrivate(s,p) \
    dixSetPrivate(&(s)->devPrivates, &vuidMouseScreenIndex, (void *) p)
#endif /* HAVE_ABSOLUTE_MOUSE_SCALING */

static inline
VuidMsePtr getVuidMsePriv(InputInfoPtr pInfo)
{
    VuidMsePtr m = vuidMouseList;

    while ((m != NULL) && (m->pInfo != pInfo)) {
        m = m->next;
    }

    return m;
}

/* Called from OsTimer callback, since removing a device from the device
   list or changing pInfo->fd while xf86Wakeup is looping through the list
   causes server crashes */
static CARD32
vuidRemoveMouse(OsTimerPtr timer, CARD32 now, pointer arg)
{
    InputInfoPtr pInfo = (InputInfoPtr) arg;

    xf86DisableDevice(pInfo->dev, TRUE);

    return 0;  /* All done, don't set to run again */
}

/*
 * Initialize and enable the mouse wheel, if present.
 *
 * Returns 1 if mouse wheel was successfully enabled.
 * Returns 0 if an error occurred or if there is no mouse wheel.
 */
static int
vuidMouseWheelInit(InputInfoPtr pInfo)
{
#ifdef HAVE_VUID_WHEEL
    wheel_state wstate;
    int nwheel = -1;
    int i;

    wstate.vers = VUID_WHEEL_STATE_VERS;
    wstate.id = 0;
    wstate.stateflags = (uint32_t) -1;

    SYSCALL(i = ioctl(pInfo->fd, VUIDGWHEELCOUNT, &nwheel));
    if (i != 0)
        return (0);

    SYSCALL(i = ioctl(pInfo->fd, VUIDGWHEELSTATE, &wstate));
    if (i != 0) {
        xf86Msg(X_WARNING, "%s: couldn't get wheel state\n", pInfo->name);
        return (0);
    }

    wstate.stateflags |= VUID_WHEEL_STATE_ENABLED;

    SYSCALL(i = ioctl(pInfo->fd, VUIDSWHEELSTATE, &wstate));
    if (i != 0) {
        xf86Msg(X_WARNING, "%s: couldn't enable wheel\n", pInfo->name);
        return (0);
    }

    return (1);
#else
    return (0);
#endif
}


/* This function is called when the protocol is "VUID". */
static Bool
vuidPreInit(InputInfoPtr pInfo, const char *protocol, int flags)
{
    MouseDevPtr pMse = pInfo->private;
    VuidMsePtr pVuidMse;

    /* Ensure we don't add the same device twice */
    if (getVuidMsePriv(pInfo) != NULL)
        return TRUE;

    pVuidMse = calloc(sizeof(VuidMseRec), 1);
    if (pVuidMse == NULL) {
        xf86Msg(X_ERROR, "%s: cannot allocate VuidMouseRec\n", pInfo->name);
        free(pMse);
        return FALSE;
    }

    pVuidMse->buffer = (unsigned char *)&pVuidMse->event;
    pVuidMse->strmod = xf86SetStrOption(pInfo->options, "StreamsModule", NULL);

    /* Setup the local procs. */
    pVuidMse->wrapped_device_control = pInfo->device_control;
    pInfo->device_control = vuidMouseProc;
    pInfo->read_input = vuidReadInput;

    pMse->xisbscale = sizeof(Firm_event);

#ifdef HAVE_ABSOLUTE_MOUSE_SCALING
    pVuidMse->absres.height = pVuidMse->absres.width = 0;
#endif
    pVuidMse->pInfo = pInfo;
    pVuidMse->next = vuidMouseList;
    vuidMouseList = pVuidMse;

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 12
    pInfo->flags |= XI86_CONFIGURED;
#endif
    return TRUE;
}

static void
vuidFlushAbsEvents(InputInfoPtr pInfo, int absX, int absY,
                   Bool *absXset, Bool *absYset)
{
#ifdef DEBUG
    ErrorF("vuidFlushAbsEvents: %d,%d (set: %d, %d)\n", absX, absY,
           *absXset, *absYset);
#endif
    if ((*absXset) && (*absYset)) {
        xf86PostMotionEvent(pInfo->dev,
                            /* is_absolute: */    TRUE,
                            /* first_valuator: */ 0,
                            /* num_valuators: */  2,
                            absX, absY);
    } else if (*absXset) {
        xf86PostMotionEvent(pInfo->dev,
                            /* is_absolute: */    TRUE,
                            /* first_valuator: */ 0,
                            /* num_valuators: */  1,
                            absX);
    } else if (*absYset) {
        xf86PostMotionEvent(pInfo->dev,
                            /* is_absolute: */    TRUE,
                            /* first_valuator: */ 1,
                            /* num_valuators: */  1,
                            absY);
    }

    *absXset = FALSE;
    *absYset = FALSE;
}

static void
vuidReadInput(InputInfoPtr pInfo)
{
    MouseDevPtr pMse;
    VuidMsePtr pVuidMse;
    int buttons;
    int dx = 0, dy = 0, dz = 0, dw = 0;
    unsigned int n;
    unsigned char *pBuf;
    int absX = 0, absY = 0;
    Bool absXset = FALSE, absYset = FALSE;

    pMse = pInfo->private;
    pVuidMse = getVuidMsePriv(pInfo);
    buttons = pMse->lastButtons;
    pBuf = pVuidMse->buffer;
    n = 0;

    do {
        n = read(pInfo->fd, pBuf, sizeof(Firm_event));

        if (n == 0) {
            break;
        } else if (n == -1) {
            switch (errno) {
                case EAGAIN: /* Nothing to read now */
                    n = 0;   /* End loop, go on to flush events & return */
                    continue;
                case EINTR:  /* Interrupted, try again */
                    continue;
                case ENODEV: /* May happen when USB mouse is unplugged */
                    /* We use X_NONE here because it didn't alloc since we
                       may be called from SIGIO handler. No longer true for
                       sigsafe logging, but matters for older servers  */
                    LogMessageVerbSigSafe(X_NONE, 0,
                                          "%s: Device no longer present - removing.\n",
                                          pInfo->name);
                    xf86RemoveEnabledDevice(pInfo);
                    pVuidMse->remove_timer =
                        TimerSet(pVuidMse->remove_timer, 0, 1,
                                 vuidRemoveMouse, pInfo);
                    return;
                default:     /* All other errors */
                    /* We use X_NONE here because it didn't alloc since we
                       may be called from SIGIO handler. No longer true for
                       sigsafe logging, but matters for older servers  */
                    LogMessageVerbSigSafe(X_NONE, 0, "%s: Read error: %s\n",
                                          pInfo->name, strerror(errno));
                    return;
            }
        } else if (n != sizeof(Firm_event)) {
            xf86Msg(X_WARNING, "%s: incomplete packet, size %d\n",
                        pInfo->name, n);
        }

#ifdef DEBUG
        LogMessageVerbSigSafe("vuidReadInput: event type: %d value: %d\n",
                              pVuidMse->event.id, pVuidMse->event.value);
#endif

        if (pVuidMse->event.id >= BUT_FIRST && pVuidMse->event.id <= BUT_LAST) {
            /* button */
            int butnum = pVuidMse->event.id - BUT_FIRST;

            if (butnum < 3)
                butnum = 2 - butnum;
            if (!pVuidMse->event.value)
                buttons &= ~(1 << butnum);
            else
                buttons |= (1 << butnum);
        } else if (pVuidMse->event.id >= VLOC_FIRST &&
                   pVuidMse->event.id <= VLOC_LAST) {
            /* axis */
            int delta = pVuidMse->event.value;
            switch(pVuidMse->event.id) {
            case LOC_X_DELTA:
                dx += delta;
                break;
            case LOC_Y_DELTA:
                dy -= delta;
                break;
            case LOC_X_ABSOLUTE:
                if (absXset) {
                    vuidFlushAbsEvents(pInfo, absX, absY, &absXset, &absYset);
                }
                absX = delta;
                absXset = TRUE;
                break;
            case LOC_Y_ABSOLUTE:
                if (absYset) {
                    vuidFlushAbsEvents(pInfo, absX, absY, &absXset, &absYset);
                }
                absY = delta;
                absYset = TRUE;
                break;
            }
        }
#ifdef HAVE_VUID_WHEEL
        else if (vuid_in_range(VUID_WHEEL, pVuidMse->event.id)) {
            if (vuid_id_offset(pVuidMse->event.id) == 0)
                dz -= VUID_WHEEL_GETDELTA(pVuidMse->event.value);
            else
                dw -= VUID_WHEEL_GETDELTA(pVuidMse->event.value);
        }
#endif
#ifdef HAVE_ABSOLUTE_MOUSE_SCALING
        else if (pVuidMse->event.id == MOUSE_TYPE_ABSOLUTE) {
            ScreenPtr   ptrCurScreen;

            /* force sending absolute resolution scaling ioctl */
            pVuidMse->absres.height = pVuidMse->absres.width = 0;
            ptrCurScreen = miPointerGetScreen(pInfo->dev);
            vuidMouseSendScreenSize(ptrCurScreen, pVuidMse);
        }
#endif

    } while (n != 0);

    if (absXset || absYset) {
        vuidFlushAbsEvents(pInfo, absX, absY, &absXset, &absYset);
    }

    pMse->PostEvent(pInfo, buttons, dx, dy, dz, dw);
    return;
}

#ifdef HAVE_ABSOLUTE_MOUSE_SCALING
static void vuidMouseSendScreenSize(ScreenPtr pScreen, VuidMsePtr pVuidMse)
{
    InputInfoPtr pInfo = pVuidMse->pInfo;
    ScrnInfoPtr pScr = XF86SCRNINFO(pScreen);
    int result;

    if ((pVuidMse->absres.width != pScr->virtualX) ||
        (pVuidMse->absres.height != pScr->virtualY))
    {
        pVuidMse->absres.width = pScr->virtualX;
        pVuidMse->absres.height = pScr->virtualY;

        do {
            result = ioctl(pInfo->fd, MSIOSRESOLUTION, &(pVuidMse->absres));
        } while ( (result != 0) && (errno == EINTR) );

        if (result != 0) {
            LogMessageVerbSigSafe(X_WARNING, -1,
                                  "%s: couldn't set absolute mouse scaling resolution: %s\n",
                                  pInfo->name, strerror(errno));
#ifdef DEBUG
        } else {
            LogMessageVerbSigSafe(X_INFO,
                                  "%s: absolute mouse scaling resolution set to %d x %d\n",
                                  pInfo->name,
                                  pVuidMse->absres.width,
                                  pVuidMse->absres.height);
#endif
        }
    }
}

static void vuidMouseAdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
      SCRN_INFO_PTR(arg);
      ScreenPtr         pScreen = xf86ScrnToScreen(pScrn);
      xf86AdjustFrameProc *wrappedAdjustFrame
          = (xf86AdjustFrameProc *) vuidMouseGetScreenPrivate(pScreen);
      VuidMsePtr        m;
      ScreenPtr         ptrCurScreen;

      if (wrappedAdjustFrame) {
          pScrn->AdjustFrame = wrappedAdjustFrame;
          (*pScrn->AdjustFrame)(ADJUST_FRAME_ARGS(pScrn, x, y));
          pScrn->AdjustFrame = vuidMouseAdjustFrame;
      }

      for (m = vuidMouseList; m != NULL ; m = m->next) {
          ptrCurScreen = miPointerGetScreen(m->pInfo->dev);
          if (ptrCurScreen == pScreen)
          {
              vuidMouseSendScreenSize(pScreen, m);
          }
      }
}

static void vuidMouseCrtcNotify(ScreenPtr pScreen)
{
    xf86_crtc_notify_proc_ptr wrappedCrtcNotify
        = (xf86_crtc_notify_proc_ptr) vuidMouseGetScreenPrivate(pScreen);
    VuidMsePtr       m;
    ScreenPtr        ptrCurScreen;

    if (wrappedCrtcNotify)
        wrappedCrtcNotify(pScreen);

    for (m = vuidMouseList; m != NULL ; m = m->next) {
        ptrCurScreen = miPointerGetScreen(m->pInfo->dev);
        if (ptrCurScreen == pScreen) {
            vuidMouseSendScreenSize(pScreen, m);
        }
    }
}
#endif /* HAVE_ABSOLUTE_MOUSE_SCALING */


static int
vuidMouseProc(DeviceIntPtr pPointer, int what)
{
    InputInfoPtr pInfo;
    MouseDevPtr pMse;
    VuidMsePtr pVuidMse;
    int ret = Success;
    int i;

    pInfo = pPointer->public.devicePrivate;
    pMse = pInfo->private;
    pMse->device = pPointer;

    pVuidMse = getVuidMsePriv(pInfo);
    if (pVuidMse == NULL) {
        return BadImplementation;
    }

    switch (what) {

    case DEVICE_INIT:
#ifdef HAVE_ABSOLUTE_MOUSE_SCALING

#if HAS_DEVPRIVATEKEYREC
        if (!dixRegisterPrivateKey(&vuidMouseScreenIndex, PRIVATE_SCREEN, 0))
                return BadAlloc;
#endif  /* HAS_DEVPRIVATEKEYREC */

        if (vuidMouseGeneration != serverGeneration) {
                for (i = 0; i < screenInfo.numScreens; i++) {
                    ScreenPtr pScreen = screenInfo.screens[i];
                    ScrnInfoPtr pScrn = XF86SCRNINFO(pScreen);
                    if (xf86CrtcConfigPrivateIndex != -1) {
                        xf86_crtc_notify_proc_ptr pCrtcNotify
                            = xf86_wrap_crtc_notify(pScreen,
                                                    vuidMouseCrtcNotify);
                        vuidMouseSetScreenPrivate(pScreen, pCrtcNotify);
                    } else {
                        vuidMouseSetScreenPrivate(pScreen,
                                                  pScrn->AdjustFrame);
                        pScrn->AdjustFrame = vuidMouseAdjustFrame;
                    }
                }
            vuidMouseGeneration = serverGeneration;
        }
#endif
        ret = pVuidMse->wrapped_device_control(pPointer, what);
        break;

    case DEVICE_ON:
        ret = pVuidMse->wrapped_device_control(pPointer, DEVICE_ON);

        if ((ret == Success) && (pInfo->fd != -1)) {
            int fmt = VUID_FIRM_EVENT;

            if (pVuidMse->strmod) {
                /* Check to see if module is already pushed */
                SYSCALL(i = ioctl(pInfo->fd, I_FIND, pVuidMse->strmod));

                if (i == 0) { /* Not already pushed */
                    SYSCALL(i = ioctl(pInfo->fd, I_PUSH, pVuidMse->strmod));
                    if (i < 0) {
                        xf86Msg(X_WARNING, "%s: cannot push module '%s' "
                                "onto mouse device: %s\n", pInfo->name,
                                pVuidMse->strmod, strerror(errno));
                        free(pVuidMse->strmod);
                        pVuidMse->strmod = NULL;
                    }
                }
            }

            SYSCALL(i = ioctl(pInfo->fd, VUIDSFORMAT, &fmt));
            if (i < 0) {
                xf86Msg(X_WARNING,
                        "%s: cannot set mouse device to VUID mode: %s\n",
                        pInfo->name, strerror(errno));
            }
            vuidMouseWheelInit(pInfo);
#ifdef HAVE_ABSOLUTE_MOUSE_SCALING
            vuidMouseSendScreenSize(screenInfo.screens[0], pVuidMse);
#endif
            xf86FlushInput(pInfo->fd);

            /* Allocate here so we don't alloc in ReadInput which may be called
               from SIGIO handler. */
            if (pVuidMse->remove_timer == NULL) {
                pVuidMse->remove_timer = TimerSet(pVuidMse->remove_timer,
                                                  0, 0, NULL, NULL);
            }
        }
        break;

    case DEVICE_OFF:
    case DEVICE_CLOSE:
        if (pInfo->fd != -1) {
            if (pVuidMse->strmod) {
                SYSCALL(i = ioctl(pInfo->fd, I_POP, pVuidMse->strmod));
                if (i == -1) {
                    xf86Msg(X_WARNING,
                      "%s: cannot pop module '%s' off mouse device: %s\n",
                      pInfo->name, pVuidMse->strmod, strerror(errno));
                }
            }
        }
        if (pVuidMse->remove_timer) {
            TimerFree(pVuidMse->remove_timer);
            pVuidMse->remove_timer = NULL;
        }
        ret = pVuidMse->wrapped_device_control(pPointer, what);
        break;

    default: /* Should never be called, but just in case */
        ret = pVuidMse->wrapped_device_control(pPointer, what);
        break;
    }
    return ret;
}

static Bool
sunMousePreInit(InputInfoPtr pInfo, const char *protocol, int flags)
{
    /* The protocol is guaranteed to be one of the internalNames[] */
    if (xf86NameCmp(protocol, "VUID") == 0) {
        return vuidPreInit(pInfo, protocol, flags);
    }
    return TRUE;
}

static const char **
BuiltinNames(void)
{
    return internalNames;
}

static Bool
CheckProtocol(const char *protocol)
{
    int i;

    for (i = 0; internalNames[i]; i++)
        if (xf86NameCmp(protocol, internalNames[i]) == 0)
            return TRUE;

    return FALSE;
}

static const char *
DefaultProtocol(void)
{
    return "Auto";
}

static Bool
solarisMouseAutoProbe(InputInfoPtr pInfo, const char **protocol,
        const char **device)
{
    const char **pdev, **pproto;
    int fd = -1;
    Bool found;
    char *strmod;

    if (*device == NULL) {
        /* Check to see if xorg.conf or HAL specified a device to use */
        *device = xf86CheckStrOption(pInfo->options, "Device", NULL);
    }

    if (*device != NULL) {
        strmod = xf86CheckStrOption(pInfo->options, "StreamsModule", NULL);
        if (strmod) {
            /* if a device name is already known, and a StreamsModule is
               specified to convert events to VUID, then we don't need to
               probe further */
            *protocol = "VUID";
            return TRUE;
        }
    }


    for (pdev = solarisMouseDevs; *pdev; pdev += 2) {
        pproto = pdev + 1;
        if ((*protocol != NULL) && (strcmp(*protocol, "Auto") != 0) &&
          (*pproto != NULL) && (strcmp(*pproto, *protocol) != 0)) {
            continue;
        }
        if ((*device != NULL) && (strcmp(*device, *pdev) != 0)) {
            continue;
        }
        SYSCALL (fd = open(*pdev, O_RDWR | O_NONBLOCK));
        if (fd == -1) {
#ifdef DEBUG
            ErrorF("Cannot open %s (%s)\n", pdev, strerror(errno));
#endif
        } else {
            found = TRUE;
            if ((*pproto != NULL) && (strcmp(*pproto, "VUID") == 0)) {
                int i, r;
                SYSCALL(r = ioctl(fd, VUIDGFORMAT, &i));
                if (r < 0) {
                    found = FALSE;
                }
            }
            close(fd);
            if (found == TRUE) {
                if (*pproto != NULL) {
                    *protocol = *pproto;
                }
                *device = *pdev;
                return TRUE;
            }
        }
    }
    return FALSE;
}

static const char *
SetupAuto(InputInfoPtr pInfo, int *protoPara)
{
    const char *pdev = NULL;
    const char *pproto = NULL;
    MouseDevPtr pMse = pInfo->private;

    if (pInfo->fd == -1) {
        /* probe to find device/protocol to use */
        if (solarisMouseAutoProbe(pInfo, &pproto, &pdev) != FALSE) {
            /* Set the Device option. */
            pInfo->options =
             xf86AddNewOption(pInfo->options, "Device", pdev);
            xf86Msg(X_INFO, "%s: Setting Device option to \"%s\"\n",
              pInfo->name, pdev);
        }
    } else if (pMse->protocolID == PROT_AUTO) {
        pdev = xf86CheckStrOption(pInfo->options,
                "Device", NULL);
        if ((solarisMouseAutoProbe(pInfo, &pproto, &pdev) != FALSE) &&
            (pproto != NULL))
            sunMousePreInit(pInfo, pproto, 0);
    }
    return pproto;
}

static const char *
FindDevice(InputInfoPtr pInfo, const char *protocol, int flags)
{
    const char *pdev = NULL;
    const char *pproto = protocol;

    if (solarisMouseAutoProbe(pInfo, &pproto, &pdev) != FALSE) {
        /* Set the Device option. */
        pInfo->options =
          xf86AddNewOption(pInfo->options, "Device", pdev);
        xf86Msg(X_INFO, "%s: Setting Device option to \"%s\"\n",
          pInfo->name, pdev);
    }
    return pdev;
}

static int
SupportedInterfaces(void)
{
    /* XXX This needs to be checked. */
    return MSE_SERIAL | MSE_BUS | MSE_PS2 | MSE_AUTO | MSE_XPS2 | MSE_MISC;
}

OSMouseInfoPtr
OSMouseInit(int flags)
{
    OSMouseInfoPtr p;

    p = calloc(sizeof(OSMouseInfoRec), 1);
    if (!p)
        return NULL;
    p->SupportedInterfaces = SupportedInterfaces;
    p->BuiltinNames = BuiltinNames;
    p->CheckProtocol = CheckProtocol;
    p->PreInit = sunMousePreInit;
    p->DefaultProtocol = DefaultProtocol;
    p->SetupAuto = SetupAuto;
    p->FindDevice = FindDevice;

    return p;
}

