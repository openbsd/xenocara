/***************************************************************************

 Copyright 2014 Intel Corporation.  All Rights Reserved.

 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sub license, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice (including the
 next paragraph) shall be included in all copies or substantial portions
 of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 IN NO EVENT SHALL INTEL, AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 **************************************************************************/

#ifndef BACKLIGHT_H
#define BACKLIGHT_H

enum backlight_type {
	BL_NONE = -1,
	BL_PLATFORM,
	BL_FIRMWARE,
	BL_RAW,
	BL_NAMED,
};

struct backlight {
	char *iface;
	enum backlight_type type;
	int max;
	int pid, fd;
};

enum backlight_type backlight_exists(const char *iface);

void backlight_init(struct backlight *backlight);
int backlight_open(struct backlight *backlight, char *iface);
int backlight_set(struct backlight *backlight, int level);
int backlight_get(struct backlight *backlight);
void backlight_disable(struct backlight *backlight);
void backlight_close(struct backlight *backlight);

struct pci_device;
char *backlight_find_for_device(struct pci_device *pci);

#endif /* BACKLIGHT_H */
