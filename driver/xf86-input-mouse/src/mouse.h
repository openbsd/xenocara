/* $XFree86: xc/programs/Xserver/hw/xfree86/input/mouse/mouse.h,v 1.13 2003/11/03 05:11:49 tsi Exp $ */

/*
 * Copyright (c) 1997-1999 by The XFree86 Project, Inc.
 */

#ifndef MOUSE_H_
#define MOUSE_H_

#include "xf86OSmouse.h"

const char * xf86MouseProtocolIDToName(MouseProtocolID id);
MouseProtocolID xf86MouseProtocolNameToID(const char *name);

#endif
