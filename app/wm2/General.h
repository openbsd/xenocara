
#ifndef _GENERAL_H_
#define _GENERAL_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

#ifdef _POSIX_SOURCE
#undef _POSIX_SOURCE
#endif

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <X11/extensions/shape.h>

// True and False are defined in Xlib.h
typedef char Boolean;

#define NewString(x) (strdup(x))

#ifndef SIGNAL_CALLBACK_TYPE
#define SIGNAL_CALLBACK_TYPE (void (*)(int))
#endif

#define signal(x,y)     \
  do { \
    struct sigaction sAct; \
    (void)sigemptyset(&sAct.sa_mask); \
    sAct.sa_flags = 0; \
    sAct.sa_handler = (SIGNAL_CALLBACK_TYPE(y)); \
    (void)sigaction((x), &sAct, NULL); \
  } while (0)

#include "Config.h"

class Atoms {
public:
    static Atom wm_state;
    static Atom wm_changeState;
    static Atom wm_protocols;
    static Atom wm_delete;
    static Atom wm_takeFocus;
    static Atom wm_colormaps;
    static Atom wm2_running;
};

extern Boolean ignoreBadWindowErrors; // tidiness hack

#endif
