
#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef INITARGS
#define INITARGS void
#endif

#ifdef HAVE_X11_EXTENSIONS_SHAPEPROTO_H
#include <X11/extensions/shapeproto.h>
#else
#define _SHAPE_SERVER_  /* don't want Xlib structures */
#include <X11/extensions/shapestr.h>
#endif

#ifdef MULTIBUFFER
extern void MultibufferExtensionInit(INITARGS);
#define _MULTIBUF_SERVER_	/* don't want Xlib structures */
#include <X11/extensions/multibufst.h>
#endif

#ifdef XTEST
extern void XTestExtensionInit(INITARGS);
#ifdef HAVE_X11_EXTENSIONS_XTESTPROTO_H
#include <X11/extensions/xtestproto.h>
#else
#define _XTEST_SERVER_
#include <X11/extensions/XTest.h>
#include <X11/extensions/xteststr.h>
#endif
#endif

#if 1
extern void XTestExtension1Init(INITARGS);
#endif

#ifdef SCREENSAVER
extern void ScreenSaverExtensionInit (INITARGS);
#include <X11/extensions/saver.h>
#endif

#ifdef XF86VIDMODE
extern void	XFree86VidModeExtensionInit(INITARGS);
#define _XF86VIDMODE_SERVER_
#include <X11/extensions/xf86vmstr.h>
#endif

#ifdef XFreeXDGA
extern void XFree86DGAExtensionInit(INITARGS);
extern void XFree86DGARegister(INITARGS);
#define _XF86DGA_SERVER_
#include <X11/extensions/xf86dgastr.h>
#endif

#ifdef DPMSExtension
extern void DPMSExtensionInit(INITARGS);
#ifdef HAVE_X11_EXTENSIONS_DPMSCONST_H
#include <X11/extensions/dpmsconst.h>
#else
#include <X11/extensions/dpmsstr.h>
#endif
#endif

#ifdef XV
extern void XvExtensionInit(INITARGS);
extern void XvMCExtensionInit(INITARGS);
extern void XvRegister(INITARGS);
#include <X11/extensions/Xv.h>
#include <X11/extensions/XvMC.h>
#endif

#ifdef RES
extern void ResExtensionInit(INITARGS);
#include <X11/extensions/XResproto.h>
#endif

#ifdef SHM
extern void ShmExtensionInit(INITARGS);
#ifdef HAVE_X11_EXTENSIONS_SHMPROTO_H
#include <X11/extensions/shmproto.h>
#else
#include <X11/extensions/shmstr.h>
#endif
extern void ShmRegisterFuncs(
    ScreenPtr pScreen,
    ShmFuncsPtr funcs);
#endif

#ifdef XSELINUX
extern void SELinuxExtensionInit(INITARGS);
#include "xselinux.h"
#endif

#ifdef XEVIE
extern void XevieExtensionInit(INITARGS);
#endif

#if 1
extern void SecurityExtensionInit(INITARGS);
#endif

#if 1
extern void PanoramiXExtensionInit(int argc, char *argv[]);
#endif

#if 1
extern void XkbExtensionInit(INITARGS);
#endif
