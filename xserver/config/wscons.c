/*
 * Copyright (c) 2011 Matthieu Herrb
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

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <dev/wscons/wsconsio.h>
#include <dev/wscons/wsksymdef.h>

#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "input.h"
#include "inputstr.h"
#include "os.h"
#include "config-backends.h"

#define WSCONS_KBD_DEVICE "/dev/wskbd"
#define WSCONS_MOUSE_PREFIX "/dev/wsmouse"

#define KB_OVRENC \
	{ KB_UK,	"gb" }, \
	{ KB_SV,	"se" }, \
	{ KB_SG,	"ch" }, \
	{ KB_SF,	"ch" }, \
	{ KB_LA,	"latam" }, \
	{ KB_CF,	"ca" }

struct nameint {
  int val;
  char *name;
} kbdenc[] = { KB_OVRENC, KB_ENCTAB, { 0 } };

struct nameint kbdvar[] = {
	{ KB_NODEAD | KB_SG,	"de_nodeadkeys" },
	{ KB_NODEAD | KB_SF,	"fr_nodeadkeys" },
	{ KB_SF,		"fr" },
	{ KB_DVORAK | KB_CF,	"fr-dvorak" },
	{ KB_DVORAK | KB_FR,	"bepo" },
	{ KB_DVORAK,		"dvorak" },
	{ KB_CF,		"fr-legacy" },
	{ KB_NODEAD,		"nodeadkeys" },
	{ 0 }
};

struct nameint kbdopt[] = {
	{ KB_SWAPCTRLCAPS, "ctrl:swapcaps" },
	{ 0 }
};

struct nameint kbdtype[] = {
	{ WSKBD_TYPE_ZAURUS,	"zaurus" },
	{ 0 }
};

extern int priv_open_device(const char *);

static void
wscons_add_keyboard(void)
{
	InputAttributes attrs = {};
	DeviceIntPtr dev = NULL;
	InputOption *options = NULL, *tmpo;
	char *config_info = NULL;
	int fd, i, rc;
	unsigned int type;
	kbd_t wsenc = 0;

	/* Find keyboard configuration */
	fd = priv_open_device(WSCONS_KBD_DEVICE);
	if (fd == -1) {
		LogMessage(X_ERROR, "wskbd: open %s: %s\n",
		    WSCONS_KBD_DEVICE, strerror(errno));
		return;
	}
	if (ioctl(fd, WSKBDIO_GETENCODING, &wsenc) == -1) {
		LogMessage(X_WARNING, "wskbd: ioctl(WSKBDIO_GETENCODING) "
		    "failed: %s\n", strerror(errno));
		close(fd);
		return;
	}
	if (ioctl(fd, WSKBDIO_GTYPE, &type) == -1) {
		LogMessage(X_WARNING, "wskbd: ioctl(WSKBDIO_GTYPE) "
		    "failed: %s\n", strerror(errno));
		close(fd);
		return;
	}
	close (fd);

	options = calloc(sizeof(*options), 1);
	if (!options)
		return;

	options->key = strdup("_source");
	options->value = strdup("server/wscons");
	if (!options->key || !options->value)
		return;

	LogMessage(X_INFO, "config/wscons: checking input device %s\n",
	    WSCONS_KBD_DEVICE);
	add_option(&options, "name", WSCONS_KBD_DEVICE);
	add_option(&options, "driver", "kbd");

	config_info = Xprintf("wscons:%s", WSCONS_KBD_DEVICE);
	if (!config_info)
		goto unwind;
	if (KB_ENCODING(wsenc) == KB_USER) {
		/* Ignore wscons "user" layout */
		LogMessageVerb(X_INFO, 3, "wskbd: ignoring \"user\" layout\n");
		goto kbd_config_done;
	}
	for  (i = 0; kbdenc[i].val; i++)
		if(KB_ENCODING(wsenc) == kbdenc[i].val) {
			LogMessageVerb(X_INFO, 3, "wskbd: using layout %s\n",
			    kbdenc[i].name);
			add_option(&options, "xkb_layout", kbdenc[i].name);
			break;
		}
	for (i = 0; kbdvar[i].val; i++)
		if (wsenc == kbdvar[i].val ||
		    KB_VARIANT(wsenc) == kbdvar[i].val) {
			LogMessageVerb(X_INFO, 3, "wskbd: using variant %s\n",
			    kbdvar[i].name);
			add_option(&options, "xkb_variant", kbdvar[i].name);
			break;
		}
	for (i = 0; kbdopt[i].val; i++)
		if (KB_VARIANT(wsenc) == kbdopt[i].val) {
			LogMessageVerb(X_INFO, 3, "wskbd: using option %s\n",
			    kbdopt[i].name);
			add_option(&options, "xkb_options", kbdopt[i].name);
			break;
		}
	for (i = 0; kbdtype[i].val; i++)
		if (type == kbdtype[i].val) {
			LogMessageVerb(X_INFO, 3, "wskbd: using type %s\n",
			    kbdtype[i].name);
			add_option(&options, "xkb_type", kbdtype[i].name);
			break;
		}

kbd_config_done:
	attrs.flags |= ATTR_KEYBOARD;
	rc = NewInputDeviceRequest(options, &attrs, &dev);
	if (rc != Success)
		goto unwind;

	for (; dev; dev = dev->next) {
		free(dev->config_info);
		dev->config_info = strdup(config_info);
	}
unwind:
	while (!dev && (tmpo = options)) {
		options = tmpo->next;
		free(tmpo->key);
		free(tmpo->value);
		free(tmpo);
    }
}

static void
wscons_add_pointer(const char *path, const char *driver, int flags)
{
	InputAttributes attrs = {};
	DeviceIntPtr dev = NULL;
	InputOption *options = NULL, *tmpo;
	char *config_info = NULL;
	int rc;

	config_info = Xprintf("wscons:%s", path);
	if (!config_info)
		return;
	options = calloc(sizeof(*options), 1);
	if (!options)
		return;

	options->key = strdup("_source");
	options->value = strdup("server/wscons");
	if (!options->key || !options->value)
		return;
	add_option(&options, "name", strdup(path));
	add_option(&options, "driver", strdup(driver));
	add_option(&options, "device", strdup(path));
	LogMessage(X_INFO, "config/wscons: checking input device %s\n", path);
	attrs.flags |= flags;
	rc = NewInputDeviceRequest(options, &attrs, &dev);
	if (rc != Success)
		goto unwind;

	for (; dev; dev = dev->next) {
		free(dev->config_info);
		dev->config_info = strdup(config_info);
	}
unwind:
	while (!dev && (tmpo = options)) {
		options = tmpo->next;
		free(tmpo->key);
		free(tmpo->value);
		free(tmpo);
    }
}

static void
wscons_add_pointers(void)
{
	char devname[256];
	int fd, i, wsmouse_type;

	/* Check pointing devices */
	for (i = 0; i < 4; i++) {
		snprintf(devname, sizeof(devname), "%s%d",
		    WSCONS_MOUSE_PREFIX, i);
		LogMessageVerb(X_INFO, 10, "wsmouse: checking %s\n", devname);
		fd = priv_open_device(devname);
		if (fd == -1) {
			LogMessageVerb(X_WARNING, 10, "%s: %s\n", devname,
			    strerror(errno));
			continue;
		}
		if (ioctl(fd, WSMOUSEIO_GTYPE, &wsmouse_type) != 0) {
			LogMessageVerb(X_WARNING, 10,
			    "%s: WSMOUSEIO_GTYPE failed\n", devname);
			close(fd);
			continue;
		}
		close(fd);
		switch (wsmouse_type) {
		case WSMOUSE_TYPE_SYNAPTICS:
			wscons_add_pointer(devname, "synaptics",
			    ATTR_TOUCHPAD);
			break;
		case WSMOUSE_TYPE_TPANEL:
			wscons_add_pointer(devname, "ws", ATTR_TOUCHSCREEN);
			break;
		default:
			break;
		}
	}
	/* Add a default entry catching all other mux elements as "mouse" */
	wscons_add_pointer(WSCONS_MOUSE_PREFIX, "mouse", ATTR_POINTER);
}

int
config_wscons_init(void)
{
	wscons_add_keyboard();
	wscons_add_pointers();
	return 1;
}

void
config_wscons_fini(void)
{
	/* Not much to do ? */
}
