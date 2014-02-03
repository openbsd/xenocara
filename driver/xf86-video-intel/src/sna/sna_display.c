/*
 * Copyright © 2007 Red Hat, Inc.
 * Copyright © 2013 Intel Corporation
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Dave Airlie <airlied@redhat.com>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <poll.h>
#include <ctype.h>

#include "sna.h"
#include "sna_reg.h"
#include "fb/fbpict.h"
#include "intel_options.h"

#include <xf86Crtc.h>

#if XF86_CRTC_VERSION >= 3
#define HAS_GAMMA 1
#else
#define HAS_GAMMA 0
#endif

#include <X11/Xatom.h>
#if defined(HAVE_X11_EXTENSIONS_DPMSCONST_H)
#include <X11/extensions/dpmsconst.h>
#else
#define DPMSModeOn 0
#define DPMSModeOff 3
#endif
#include <xf86DDC.h> /* for xf86InterpretEDID */

#include <xf86drm.h>

#ifdef HAVE_VALGRIND
#include <valgrind.h>
#include <memcheck.h>
#endif

/* Minor discrepancy between 32-bit/64-bit ABI in old kernels */
union compat_mode_get_connector{
	struct drm_mode_get_connector conn;
	uint32_t pad[20];
};

#define KNOWN_MODE_FLAGS ((1<<14)-1)

#ifndef MONITOR_EDID_COMPLETE_RAWDATA
#define MONITOR_EDID_COMPLETE_RAWDATA 1
#endif

#ifndef DEFAULT_DPI
#define DEFAULT_DPI 96
#endif

#if 0
#define __DBG DBG
#else
#define __DBG(x)
#endif

extern XF86ConfigPtr xf86configptr;

struct sna_crtc {
	struct drm_mode_modeinfo kmode;
	int dpms_mode;
	PixmapPtr scanout_pixmap;
	struct kgem_bo *bo, *shadow_bo;
	uint32_t cursor;
	bool shadow;
	bool fallback_shadow;
	bool transform;
	uint8_t id;
	uint8_t pipe;
	uint8_t plane;
};

struct sna_property {
	drmModePropertyPtr kprop;
	int num_atoms; /* if range prop, num_atoms == 1; if enum prop, num_atoms == num_enums + 1 */
	Atom *atoms;
};

struct sna_output {
	int id;
	int encoder_idx;

	unsigned int is_panel : 1;

	uint32_t edid_idx;
	uint32_t edid_blob_id;
	uint32_t edid_len;
	void *edid_raw;

	bool has_panel_limits;
	int panel_hdisplay;
	int panel_vdisplay;

	uint32_t dpms_id;
	int dpms_mode;
	char *backlight_iface;
	int backlight_active_level;
	int backlight_max;

	int num_modes;
	struct drm_mode_modeinfo *modes;

	int num_props;
	uint32_t *prop_ids;
	uint64_t *prop_values;
	struct sna_property *props;

};

static inline struct sna_output *to_sna_output(xf86OutputPtr output)
{
	return output->driver_private;
}

static inline int to_connector_id(xf86OutputPtr output)
{
	return to_sna_output(output)->id;
}

static inline struct sna_crtc *to_sna_crtc(xf86CrtcPtr crtc)
{
	return crtc->driver_private;
}

static bool sna_mode_has_pending_events(struct sna *sna)
{
	struct pollfd pfd;
	pfd.fd = sna->kgem.fd;
	pfd.events = POLLIN;
	return poll(&pfd, 1, 0) == 1;
}

static bool sna_mode_wait_for_event(struct sna *sna)
{
	struct pollfd pfd;
	pfd.fd = sna->kgem.fd;
	pfd.events = POLLIN;
	return poll(&pfd, 1, -1) == 1;
}

#define BACKLIGHT_CLASS "/sys/class/backlight"

/* Enough for 10 digits of backlight + '\n' + '\0' */
#define BACKLIGHT_VALUE_LEN 12

static inline uint32_t fb_id(struct kgem_bo *bo)
{
	return bo->delta;
}

uint32_t sna_crtc_id(xf86CrtcPtr crtc)
{
	return to_sna_crtc(crtc)->id;
}

int sna_crtc_to_pipe(xf86CrtcPtr crtc)
{
	return to_sna_crtc(crtc)->pipe;
}

uint32_t sna_crtc_to_plane(xf86CrtcPtr crtc)
{
	return to_sna_crtc(crtc)->plane;
}

#ifndef NDEBUG
static void gem_close(int fd, uint32_t handle);
static void assert_scanout(struct kgem *kgem, struct kgem_bo *bo,
			   int width, int height)
{
	struct drm_mode_fb_cmd info;

	assert(bo->scanout);

	VG_CLEAR(info);
	info.fb_id = bo->delta;

	assert(drmIoctl(kgem->fd, DRM_IOCTL_MODE_GETFB, &info) == 0);
	gem_close(kgem->fd, info.handle);

	assert(width == info.width && height == info.height);
}
#else
#define assert_scanout(k, b, w, h)
#endif

static unsigned get_fb(struct sna *sna, struct kgem_bo *bo,
		       int width, int height)
{
	ScrnInfoPtr scrn = sna->scrn;
	struct drm_mode_fb_cmd arg;

	assert(bo->refcnt);
	assert(bo->proxy == NULL);
	assert(!bo->snoop);
	assert(height * bo->pitch <= kgem_bo_size(bo)); /* XXX crtc offset */
	if (bo->delta) {
		DBG(("%s: reusing fb=%d for handle=%d\n",
		     __FUNCTION__, bo->delta, bo->handle));
		assert_scanout(&sna->kgem, bo, width, height);
		return bo->delta;
	}

	DBG(("%s: create fb %dx%d@%d/%d\n",
	     __FUNCTION__, width, height, scrn->depth, scrn->bitsPerPixel));

	assert(bo->tiling != I915_TILING_Y);
	assert((bo->pitch & 63) == 0);

	VG_CLEAR(arg);
	arg.width = width;
	arg.height = height;
	arg.pitch = bo->pitch;
	arg.bpp = scrn->bitsPerPixel;
	arg.depth = scrn->depth;
	arg.handle = bo->handle;

	assert(sna->scrn->vtSema); /* must be master */
	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_ADDFB, &arg)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "%s: failed to add fb: %dx%d depth=%d, bpp=%d, pitch=%d: %d\n",
			   __FUNCTION__, width, height,
			   scrn->depth, scrn->bitsPerPixel, bo->pitch, errno);
		return 0;
	}
	assert(arg.fb_id != 0);

	bo->scanout = true;
	return bo->delta = arg.fb_id;
}

static uint32_t gem_create(int fd, int size)
{
	struct drm_i915_gem_create create;

	VG_CLEAR(create);
	create.handle = 0;
	create.size = ALIGN(size, 4096);
	(void)drmIoctl(fd, DRM_IOCTL_I915_GEM_CREATE, &create);

	return create.handle;
}

static void gem_close(int fd, uint32_t handle)
{
	struct drm_gem_close close;

	VG_CLEAR(close);
	close.handle = handle;
	(void)drmIoctl(fd, DRM_IOCTL_GEM_CLOSE, &close);
}

#ifdef __OpenBSD__

#include <dev/wscons/wsconsio.h>
#include <xf86Priv.h>

static void
sna_output_backlight_set(xf86OutputPtr output, int level)
{
	struct sna_output *sna_output = output->driver_private;
	struct wsdisplay_param param;

	DBG(("%s: level=%d, max=%d\n", __FUNCTION__,
	     level, sna_output->backlight_max));

	if (!sna_output->backlight_iface)
		return;

	if ((unsigned)level > sna_output->backlight_max)
		level = sna_output->backlight_max;

	VG_CLEAR(param);
	param.param = WSDISPLAYIO_PARAM_BRIGHTNESS;
	param.curval = level;

	if (ioctl(xf86Info.consoleFd, WSDISPLAYIO_SETPARAM, &param) == -1) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			   "Failed to set backlight level: %s\n",
			   strerror(errno));
	}
}

static int
sna_output_backlight_get(xf86OutputPtr output)
{
	struct wsdisplay_param param;

	VG_CLEAR(param);
	param.param = WSDISPLAYIO_PARAM_BRIGHTNESS;

	if (ioctl(xf86Info.consoleFd, WSDISPLAYIO_GETPARAM, &param) == -1) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			   "Failed to get backlight level: %s\n",
			   strerror(errno));
		return -1;
	}

	DBG(("%s: level=%d (max=%d)\n", __FUNCTION__, param.curval, param.max));

	return param.curval;
}

static void
sna_output_backlight_init(xf86OutputPtr output)
{
	struct sna_output *sna_output = output->driver_private;
	struct wsdisplay_param param;

	VG_CLEAR(param);
	param.param = WSDISPLAYIO_PARAM_BRIGHTNESS;

	if (ioctl(xf86Info.consoleFd, WSDISPLAYIO_GETPARAM, &param) == -1)
		return;

	DBG(("%s: found 'wscons'\n", __FUNCTION__));

	sna_output->backlight_iface = strdup("wscons");
	sna_output->backlight_max = param.max;
	sna_output->backlight_active_level = param.curval;
}

#else

static void
sna_output_backlight_set(xf86OutputPtr output, int level)
{
	struct sna_output *sna_output = output->driver_private;
	char path[1024], val[BACKLIGHT_VALUE_LEN];
	int fd, len, ret;

	DBG(("%s: level=%d, max=%d\n", __FUNCTION__,
	     level, sna_output->backlight_max));

	if (!sna_output->backlight_iface)
		return;

	if ((unsigned)level > sna_output->backlight_max)
		level = sna_output->backlight_max;

	len = snprintf(val, BACKLIGHT_VALUE_LEN, "%d\n", level);
	sprintf(path, "%s/%s/brightness",
		BACKLIGHT_CLASS, sna_output->backlight_iface);
	fd = open(path, O_RDWR);
	if (fd == -1) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR, "failed to open %s for backlight "
			   "control: %s\n", path, strerror(errno));
		return;
	}

	ret = write(fd, val, len);
	if (ret == -1) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR, "write to %s for backlight "
			   "control failed: %s\n", path, strerror(errno));
	}

	close(fd);
}

static int
sna_output_backlight_get(xf86OutputPtr output)
{
	struct sna_output *sna_output = output->driver_private;
	char path[1024], val[BACKLIGHT_VALUE_LEN];
	int fd, level;

	sprintf(path, "%s/%s/actual_brightness",
		BACKLIGHT_CLASS, sna_output->backlight_iface);
	fd = open(path, O_RDONLY);
	if (fd == -1) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR, "failed to open %s "
			   "for backlight control: %s\n", path, strerror(errno));
		return -1;
	}

	memset(val, 0, sizeof(val));
	if (read(fd, val, BACKLIGHT_VALUE_LEN) == -1) {
		close(fd);
		return -1;
	}

	close(fd);

	level = atoi(val);
	DBG(("%s: level=%d (max=%d)\n",
	     __FUNCTION__, level, sna_output->backlight_max));

	if (level > sna_output->backlight_max)
		level = sna_output->backlight_max;
	else if (level < 0)
		level = -1;
	return level;
}

static int
sna_output_backlight_get_max(xf86OutputPtr output)
{
	struct sna_output *sna_output = output->driver_private;
	char path[1024], val[BACKLIGHT_VALUE_LEN];
	int fd, max = 0;

	sprintf(path, "%s/%s/max_brightness",
		BACKLIGHT_CLASS, sna_output->backlight_iface);
	fd = open(path, O_RDONLY);
	if (fd == -1) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR, "failed to open %s "
			   "for backlight control: %s\n", path, strerror(errno));
		return -1;
	}

	memset(val, 0, sizeof(val));
	if (read(fd, val, BACKLIGHT_VALUE_LEN) == -1) {
		close(fd);
		return -1;
	}

	close(fd);

	max = atoi(val);
	if (max <= 0)
		max = -1;
	return max;
}

enum {
	PLATFORM,
	FIRMWARE,
	RAW,
	NAMED,
};

static char *
has_user_backlight_override(xf86OutputPtr output)
{
	struct sna_output *sna_output = output->driver_private;
	struct sna *sna = to_sna(output->scrn);
	const char *str;
	int max;

	str = xf86GetOptValString(sna->Options, OPTION_BACKLIGHT);
	if (str == NULL)
		return NULL;

	sna_output->backlight_iface = (char *)str;
	max = sna_output_backlight_get_max(output);
	sna_output->backlight_iface = NULL;
	if (max <= 0) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			   "unrecognised backlight control interface '%s'\n",
			   str);
		return NULL;
	}

	return strdup(str);
}

static char *
has_device_backlight(xf86OutputPtr output, int *best_type)
{
	struct sna_output *sna_output = output->driver_private;
	struct sna *sna = to_sna(output->scrn);
	struct pci_device *pci;
	char path[1024];
	char *best_iface = NULL;
	DIR *dir;
	struct dirent *de;

	pci = xf86GetPciInfoForEntity(sna->pEnt->index);
	if (pci == NULL)
		return NULL;

	snprintf(path, sizeof(path),
		 "/sys/bus/pci/devices/%04x:%02x:%02x.%d/backlight",
		 pci->domain, pci->bus, pci->dev, pci->func);

	DBG(("%s: scanning %s\n", __FUNCTION__, path));
	dir = opendir(path);
	if (dir == NULL)
		return NULL;

	while ((de = readdir(dir))) {
		char buf[100];
		int fd, v;

		if (*de->d_name == '.')
			continue;

		DBG(("%s: %s\n", __FUNCTION__, de->d_name));
		snprintf(path, sizeof(path), "%s/%s/type",
			 BACKLIGHT_CLASS, de->d_name);

		v = -1;
		fd = open(path, O_RDONLY);
		if (fd >= 0) {
			v = read(fd, buf, sizeof(buf)-1);
			close(fd);
		}
		if (v > 0) {
			while (v > 0 && isspace(buf[v-1]))
				v--;
			buf[v] = '\0';

			if (strcmp(buf, "raw") == 0)
				v = RAW;
			else if (strcmp(buf, "platform") == 0)
				v = PLATFORM;
			else if (strcmp(buf, "firmware") == 0)
				v = FIRMWARE;
			else
				v = INT_MAX;
		} else
			v = INT_MAX;

		if (v < *best_type) {
			char *copy;
			int max;

			sna_output->backlight_iface = de->d_name;
			max = sna_output_backlight_get_max(output);
			sna_output->backlight_iface = NULL;
			if (max <= 0)
				continue;

			copy = strdup(de->d_name);
			if (copy) {
				free(best_iface);
				best_iface = copy;
				*best_type = v;
			}
		}
	}
	closedir(dir);

	return best_iface;
}

static char *
has_backlight(xf86OutputPtr output, int *best_type)
{
	static const char *known_interfaces[] = {
		"dell_backlight",
		"gmux_backlight",
		"asus-laptop",
		"asus-nb-wmi",
		"eeepc",
		"thinkpad_screen",
		"mbp_backlight",
		"fujitsu-laptop",
		"sony",
		"samsung",
		"acpi_video1",
		"acpi_video0",
		"intel_backlight",
	};
	struct sna_output *sna_output = output->driver_private;
	char *best_iface = NULL;
	DIR *dir;
	struct dirent *de;

	dir = opendir(BACKLIGHT_CLASS);
	if (dir == NULL)
		return NULL;

	while ((de = readdir(dir))) {
		char path[1024];
		char buf[100];
		int fd, v;

		if (*de->d_name == '.')
			continue;

		snprintf(path, sizeof(path), "%s/%s/type",
			 BACKLIGHT_CLASS, de->d_name);

		v = -1;
		fd = open(path, O_RDONLY);
		if (fd >= 0) {
			v = read(fd, buf, sizeof(buf)-1);
			close(fd);
		}
		if (v > 0) {
			while (v > 0 && isspace(buf[v-1]))
				v--;
			buf[v] = '\0';

			if (strcmp(buf, "raw") == 0)
				v = RAW;
			else if (strcmp(buf, "platform") == 0)
				v = PLATFORM;
			else if (strcmp(buf, "firmware") == 0)
				v = FIRMWARE;
			else
				v = NAMED;
		} else
			v = NAMED;

		/* Fallback to priority list of known iface for old kernels */
		if (v == NAMED) {
			int i;
			for (i = 0; i < ARRAY_SIZE(known_interfaces); i++) {
				if (strcmp(de->d_name, known_interfaces[i]) == 0)
					break;
			}
			v += i;
		}

		if (v < *best_type) {
			char *copy;
			int max;

			/* XXX detect right backlight for multi-GPU/panels */

			sna_output->backlight_iface = de->d_name;
			max = sna_output_backlight_get_max(output);
			sna_output->backlight_iface = NULL;
			if (max <= 0)
				continue;

			copy = strdup(de->d_name);
			if (copy) {
				free(best_iface);
				best_iface = copy;
				*best_type = v;
			}
		}
	}
	closedir(dir);

	return best_iface;
}

static void
sna_output_backlight_init(xf86OutputPtr output)
{
	struct sna_output *sna_output = output->driver_private;
	MessageType from = X_PROBED;
	char *best_iface;
	int best_type;

	best_type = INT_MAX;
	best_iface = has_user_backlight_override(output);
	if (best_iface)
		goto done;

	best_iface = has_device_backlight(output, &best_type);
	if (best_iface)
		goto done;

	best_iface = has_backlight(output, &best_type);
	if (best_iface)
		goto done;

	return;

done:
	sna_output->backlight_iface = best_iface;
	sna_output->backlight_max = sna_output_backlight_get_max(output);
	sna_output->backlight_active_level = sna_output_backlight_get(output);
	switch (best_type) {
	case INT_MAX: best_iface = (char *)"user"; from = X_CONFIG; break;
	case FIRMWARE: best_iface = (char *)"firmware"; break;
	case PLATFORM: best_iface = (char *)"platform"; break;
	case RAW: best_iface = (char *)"raw"; break;
	default: best_iface = (char *)"unknown"; break;
	}
	xf86DrvMsg(output->scrn->scrnIndex, from,
		   "found backlight control interface %s (type '%s')\n",
		   sna_output->backlight_iface, best_iface);
}
#endif

static DisplayModePtr
mode_from_kmode(ScrnInfoPtr scrn,
		const struct drm_mode_modeinfo *kmode,
		DisplayModePtr mode)
{
	DBG(("kmode: %s, clock=%d, %d %d %d %d %d, %d %d %d %d %d, flags=%x, type=%x\n",
	     kmode->name, kmode->clock,
	     kmode->hdisplay, kmode->hsync_start, kmode->hsync_end, kmode->htotal, kmode->hskew,
	     kmode->vdisplay, kmode->vsync_start, kmode->vsync_end, kmode->vtotal, kmode->vscan,
	     kmode->flags, kmode->type));

	mode->status = MODE_OK;

	mode->Clock = kmode->clock;

	mode->HDisplay = kmode->hdisplay;
	mode->HSyncStart = kmode->hsync_start;
	mode->HSyncEnd = kmode->hsync_end;
	mode->HTotal = kmode->htotal;
	mode->HSkew = kmode->hskew;

	mode->VDisplay = kmode->vdisplay;
	mode->VSyncStart = kmode->vsync_start;
	mode->VSyncEnd = kmode->vsync_end;
	mode->VTotal = kmode->vtotal;
	mode->VScan = kmode->vscan;

	mode->Flags = kmode->flags;
	mode->name = strdup(kmode->name);

	if (kmode->type & DRM_MODE_TYPE_DRIVER)
		mode->type = M_T_DRIVER;
	if (kmode->type & DRM_MODE_TYPE_PREFERRED)
		mode->type |= M_T_PREFERRED;

	if (mode->status == MODE_OK && kmode->flags & ~KNOWN_MODE_FLAGS)
		mode->status = MODE_BAD; /* unknown flags => unhandled */

	xf86SetModeCrtc(mode, scrn->adjustFlags);
	return mode;
}

static void
mode_to_kmode(struct drm_mode_modeinfo *kmode, DisplayModePtr mode)
{
	memset(kmode, 0, sizeof(*kmode));

	kmode->clock = mode->Clock;
	kmode->hdisplay = mode->HDisplay;
	kmode->hsync_start = mode->HSyncStart;
	kmode->hsync_end = mode->HSyncEnd;
	kmode->htotal = mode->HTotal;
	kmode->hskew = mode->HSkew;

	kmode->vdisplay = mode->VDisplay;
	kmode->vsync_start = mode->VSyncStart;
	kmode->vsync_end = mode->VSyncEnd;
	kmode->vtotal = mode->VTotal;
	kmode->vscan = mode->VScan;

	kmode->flags = mode->Flags;
	if (mode->name)
		strncpy(kmode->name, mode->name, DRM_DISPLAY_MODE_LEN);
	kmode->name[DRM_DISPLAY_MODE_LEN-1] = 0;
}

static void
sna_crtc_force_outputs_on(xf86CrtcPtr crtc)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
	int i;

	DBG(("%s(pipe=%d), currently? %d\n", __FUNCTION__,
	     to_sna_crtc(crtc)->pipe, to_sna_crtc(crtc)->dpms_mode));

	/* DPMS handling by the kernel is inconsistent, so after setting a
	 * mode on an output presume that we intend for it to be on, or that
	 * the kernel will force it on.
	 *
	 * So force DPMS to be on for all connected outputs, and restore
	 * the backlight.
	 */
	for (i = 0; i < config->num_output; i++) {
		xf86OutputPtr output = config->output[i];

		if (output->crtc != crtc)
			continue;

		output->funcs->dpms(output, DPMSModeOn);
	}

	to_sna_crtc(crtc)->dpms_mode = DPMSModeOn;
#if XF86_CRTC_VERSION >= 3
	crtc->active = TRUE;
#endif
}

static void
sna_crtc_force_outputs_off(xf86CrtcPtr crtc)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
	int i;

	DBG(("%s(pipe=%d), currently? %d\n", __FUNCTION__,
	     to_sna_crtc(crtc)->pipe, to_sna_crtc(crtc)->dpms_mode));

	/* DPMS handling by the kernel is inconsistent, so after setting a
	 * mode on an output presume that we intend for it to be on, or that
	 * the kernel will force it on.
	 *
	 * So force DPMS to be on for all connected outputs, and restore
	 * the backlight.
	 */
	for (i = 0; i < config->num_output; i++) {
		xf86OutputPtr output = config->output[i];

		if (output->crtc != crtc)
			continue;

		output->funcs->dpms(output, DPMSModeOff);
	}

	to_sna_crtc(crtc)->dpms_mode = DPMSModeOff;
}

static bool
sna_crtc_apply(xf86CrtcPtr crtc)
{
	struct sna *sna = to_sna(crtc->scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
	struct drm_mode_crtc arg;
	uint32_t output_ids[32];
	int output_count = 0;
	int i;

	DBG(("%s\n", __FUNCTION__));

	assert(config->num_output < ARRAY_SIZE(output_ids));

	for (i = 0; i < config->num_output; i++) {
		xf86OutputPtr output = config->output[i];

		if (output->crtc != crtc)
			continue;

		assert(output->possible_crtcs & (1 << sna_crtc->pipe) ||
		       xf86IsEntityShared(crtc->scrn->entityList[0]));

		DBG(("%s: attaching output '%s' %d [%d] to crtc:%d (pipe %d) (possible crtc:%x, possible clones:%x)\n",
		     __FUNCTION__, output->name, i, to_connector_id(output),
		     sna_crtc->id, sna_crtc->pipe,
		     (uint32_t)output->possible_crtcs,
		     (uint32_t)output->possible_clones));
		output_ids[output_count] = to_connector_id(output);
		if (++output_count == ARRAY_SIZE(output_ids))
			return false;
	}

	VG_CLEAR(arg);
	arg.crtc_id = sna_crtc->id;
	arg.fb_id = fb_id(sna_crtc->bo);
	if (sna_crtc->transform) {
		arg.x = 0;
		arg.y = 0;
	} else {
		arg.x = crtc->x;
		arg.y = crtc->y;
	}
	arg.set_connectors_ptr = (uintptr_t)output_ids;
	arg.count_connectors = output_count;
	arg.mode = sna_crtc->kmode;
	arg.mode_valid = 1;

	DBG(("%s: applying crtc [%d, pipe=%d] mode=%dx%d+%d+%d@%d, fb=%d%s%s update to %d outputs [%d...]\n",
	     __FUNCTION__, sna_crtc->id, sna_crtc->pipe,
	     arg.mode.hdisplay,
	     arg.mode.vdisplay,
	     arg.x, arg.y,
	     arg.mode.clock,
	     arg.fb_id,
	     sna_crtc->shadow ? " [shadow]" : "",
	     sna_crtc->transform ? " [transformed]" : "",
	     output_count, output_count ? output_ids[0] : 0));

	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_SETCRTC, &arg))
		return false;

	sna_crtc_force_outputs_on(crtc);
	return true;
}

struct wait_for_shadow {
	RegionRec region;
	struct kgem_bo *bo;
};

static bool wait_for_shadow(struct sna *sna, struct sna_pixmap *priv, unsigned flags)
{
	struct wait_for_shadow *wait = priv->move_to_gpu_data;
	struct kgem_bo *bo = wait->bo;
	PixmapPtr pixmap = priv->pixmap;
	DamagePtr damage;
	bool ret = true;

	DBG(("%s: flags=%x, shadow_flip=%d, handle=%d, wait=%d, old=%d\n",
	     __FUNCTION__, flags, sna->mode.shadow_flip,
	     priv->gpu_bo->handle, wait->bo->handle, sna->mode.shadow->handle));

	assert(wait->bo != priv->gpu_bo);

	if (flags == 0 || pixmap != sna->front || !sna->mode.shadow_damage)
		goto done;

	if ((flags & MOVE_WRITE) == 0)
		return true;

	assert(sna->mode.shadow_active);
	assert(bo == sna->mode.shadow);

	assert(priv->gpu_bo->refcnt >= 1);
	sna->mode.shadow = priv->gpu_bo;

	damage = sna->mode.shadow_damage;
	sna->mode.shadow_damage = NULL;

	while (sna->mode.shadow_flip && sna_mode_has_pending_events(sna))
		sna_mode_wakeup(sna);

	if (sna->mode.shadow_flip) {
		bo = kgem_create_2d(&sna->kgem,
				    pixmap->drawable.width,
				    pixmap->drawable.height,
				    pixmap->drawable.bitsPerPixel,
				    priv->gpu_bo->tiling,
				    CREATE_EXACT | CREATE_SCANOUT);
		if (bo != NULL) {
			DBG(("%s: replacing still-attached GPU bo\n",
			     __FUNCTION__));

			kgem_bo_destroy(&sna->kgem, wait->bo);
			RegionUninit(&wait->region);

			wait->region.extents.x1 = 0;
			wait->region.extents.y1 = 0;
			wait->region.extents.x2 = pixmap->drawable.width;
			wait->region.extents.y2 = pixmap->drawable.height;
			wait->region.data = NULL;
		} else {
			while (sna->mode.shadow_flip &&
			       sna_mode_wait_for_event(sna))
				sna_mode_wakeup(sna);

			bo = wait->bo;
		}
	}

	sna->mode.shadow_damage = damage;

	if (flags & MOVE_READ) {
		DBG(("%s: copying existing GPU damage: %ldx(%d, %d), (%d, %d)\n",
		     __FUNCTION__, (long)REGION_NUM_RECTS(&wait->region),
		     wait->region.extents.x1, wait->region.extents.y1,
		     wait->region.extents.x2, wait->region.extents.y2));
		ret = sna->render.copy_boxes(sna, GXcopy,
					     pixmap, priv->gpu_bo, 0, 0,
					     pixmap, bo, 0, 0,
					     REGION_RECTS(&wait->region),
					     REGION_NUM_RECTS(&wait->region),
					     0);
	}

	if (priv->cow)
		sna_pixmap_undo_cow(sna, priv, 0);

	sna_pixmap_unmap(pixmap, priv);
	priv->gpu_bo = bo;

	sna_dri_pixmap_update_bo(sna, pixmap);

done:
	kgem_bo_destroy(&sna->kgem, wait->bo);
	RegionUninit(&wait->region);
	free(wait);

	priv->move_to_gpu_data = NULL;
	priv->move_to_gpu = NULL;

	return ret;
}

static bool sna_mode_enable_shadow(struct sna *sna)
{
	ScreenPtr screen = sna->scrn->pScreen;

	DBG(("%s\n", __FUNCTION__));
	assert(sna->mode.shadow == NULL);
	assert(sna->mode.shadow_damage == NULL);
	assert(sna->mode.shadow_active == 0);

	sna->mode.shadow_damage = DamageCreate(NULL, NULL,
					       DamageReportNone, TRUE,
					       screen, screen);
	if (!sna->mode.shadow_damage)
		return false;

	DamageRegister(&sna->front->drawable, sna->mode.shadow_damage);
	return true;
}

static void sna_mode_disable_shadow(struct sna *sna)
{
	struct sna_pixmap *priv;

	if (!sna->mode.shadow_damage)
		return;

	DBG(("%s\n", __FUNCTION__));

	priv = sna_pixmap(sna->front);
	if (priv->move_to_gpu == wait_for_shadow)
		priv->move_to_gpu(sna, priv, 0);

	DamageUnregister(&sna->front->drawable, sna->mode.shadow_damage);
	DamageDestroy(sna->mode.shadow_damage);
	sna->mode.shadow_damage = NULL;

	if (sna->mode.shadow) {
		kgem_bo_destroy(&sna->kgem, sna->mode.shadow);
		sna->mode.shadow = NULL;
	}

	assert(sna->mode.shadow_active == 0);
}

static bool sna_crtc_enable_shadow(struct sna *sna, struct sna_crtc *crtc)
{
	if (crtc->shadow) {
		assert(sna->mode.shadow_damage && sna->mode.shadow_active);
		return true;
	}

	DBG(("%s: enabling for crtc %d\n", __FUNCTION__, crtc->id));

	if (!sna->mode.shadow_active) {
		if (!sna_mode_enable_shadow(sna))
			return false;
		assert(sna->mode.shadow_damage);
		assert(sna->mode.shadow == NULL);
	}

	crtc->shadow = true;
	sna->mode.shadow_active++;
	return true;
}

static void sna_crtc_disable_shadow(struct sna *sna, struct sna_crtc *crtc)
{
	crtc->fallback_shadow = false;
	if (!crtc->shadow)
		return;

	DBG(("%s: disabling for crtc %d\n", __FUNCTION__, crtc->id));
	assert(sna->mode.shadow_active > 0);

	if (!--sna->mode.shadow_active)
		sna_mode_disable_shadow(sna);

	if (crtc->shadow_bo) {
		kgem_bo_destroy(&sna->kgem, crtc->shadow_bo);
		crtc->shadow_bo = NULL;
	}

	crtc->shadow = false;
}

static void
sna_crtc_disable(xf86CrtcPtr crtc)
{
	struct sna *sna = to_sna(crtc->scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	struct drm_mode_crtc arg;

	if (sna_crtc == NULL)
		return;

	DBG(("%s: disabling crtc [%d, pipe=%d]\n", __FUNCTION__,
	     sna_crtc->id, sna_crtc->pipe));

	sna_crtc_force_outputs_off(crtc);

	memset(&arg, 0, sizeof(arg));
	arg.crtc_id = sna_crtc->id;
	(void)drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_SETCRTC, &arg);

	sna_crtc_disable_shadow(sna, sna_crtc);

	if (sna_crtc->bo) {
		kgem_bo_destroy(&sna->kgem, sna_crtc->bo);
		sna_crtc->bo = NULL;
	}
}

static void update_flush_interval(struct sna *sna)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	int i, max_vrefresh = 0;

	for (i = 0; i < config->num_crtc; i++) {
		xf86CrtcPtr crtc = config->crtc[i];

		if (to_sna_crtc(crtc) == NULL)
			continue;

		if (!crtc->enabled) {
			DBG(("%s: CRTC:%d (pipe %d) disabled\n",
			     __FUNCTION__,i, to_sna_crtc(crtc)->pipe));
			continue;
		}

		if (to_sna_crtc(crtc)->dpms_mode != DPMSModeOn) {
			DBG(("%s: CRTC:%d (pipe %d) turned off\n",
			     __FUNCTION__,i, to_sna_crtc(crtc)->pipe));
			continue;
		}

		DBG(("%s: CRTC:%d (pipe %d) vrefresh=%f\n",
		     __FUNCTION__, i, to_sna_crtc(crtc)->pipe,
		     xf86ModeVRefresh(&crtc->mode)));
		max_vrefresh = max(max_vrefresh, xf86ModeVRefresh(&crtc->mode));
	}

	if (max_vrefresh == 0)
		sna->vblank_interval = 0;
	else
		sna->vblank_interval = 1000 / max_vrefresh; /* Hz -> ms */

	DBG(("max_vrefresh=%d, vblank_interval=%d ms\n",
	       max_vrefresh, sna->vblank_interval));
}

static struct kgem_bo *sna_create_bo_for_fbcon(struct sna *sna,
					       const struct drm_mode_fb_cmd *fbcon)
{
	struct drm_gem_flink flink;
	struct kgem_bo *bo;
	int ret;

	/* Create a new reference for the fbcon so that we can track it
	 * using a normal bo and so that when we call gem_close on it we
	 * delete our reference and not fbcon's!
	 */
	VG_CLEAR(flink);
	flink.handle = fbcon->handle;
	ret = drmIoctl(sna->kgem.fd, DRM_IOCTL_GEM_FLINK, &flink);
	if (ret)
		return NULL;

	bo = kgem_create_for_name(&sna->kgem, flink.name);
	if (bo == NULL)
		return NULL;

	bo->pitch = fbcon->pitch;
	return bo;
}

/* Copy the current framebuffer contents into the front-buffer for a seamless
 * transition from e.g. plymouth.
 */
void sna_copy_fbcon(struct sna *sna)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	struct drm_mode_fb_cmd fbcon;
	PixmapRec scratch;
	struct sna_pixmap *priv;
	struct kgem_bo *bo;
	BoxRec box;
	bool ok;
	int sx, sy;
	int dx, dy;
	int i;

	if (wedged(sna))
		return;

	DBG(("%s\n", __FUNCTION__));
	assert((sna->flags & SNA_IS_HOSTED) == 0);

	priv = sna_pixmap(sna->front);
	assert(priv && priv->gpu_bo);

	/* Scan the connectors for a framebuffer and assume that is the fbcon */
	VG_CLEAR(fbcon);
	fbcon.fb_id = 0;
	for (i = 0; i < config->num_crtc; i++) {
		struct sna_crtc *crtc = to_sna_crtc(config->crtc[i]);
		struct drm_mode_crtc mode;

		if (!crtc)
			continue;

		VG_CLEAR(mode);
		mode.crtc_id = crtc->id;
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETCRTC, &mode))
			continue;
		if (!mode.fb_id)
			continue;

		fbcon.fb_id = mode.fb_id;
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETFB, &fbcon)) {
			fbcon.fb_id = 0;
			continue;
		}
		break;
	}
	if (fbcon.fb_id == 0) {
		DBG(("%s: no fbcon found\n", __FUNCTION__));
		return;
	}

	if (fbcon.fb_id == priv->gpu_bo->delta) {
		DBG(("%s: fb already installed as scanout\n", __FUNCTION__));
		return;
	}

	DBG(("%s: found fbcon, size=%dx%d, depth=%d, bpp=%d\n",
	     __FUNCTION__, fbcon.width, fbcon.height, fbcon.depth, fbcon.bpp));

	bo = sna_create_bo_for_fbcon(sna, &fbcon);
	if (bo == NULL)
		return;

	DBG(("%s: fbcon handle=%d\n", __FUNCTION__, bo->handle));

	scratch.drawable.width = fbcon.width;
	scratch.drawable.height = fbcon.height;
	scratch.drawable.depth = fbcon.depth;
	scratch.drawable.bitsPerPixel = fbcon.bpp;
	scratch.devPrivate.ptr = NULL;

	box.x1 = box.y1 = 0;
	box.x2 = min(fbcon.width, sna->front->drawable.width);
	box.y2 = min(fbcon.height, sna->front->drawable.height);

	sx = dx = 0;
	if (box.x2 < (uint16_t)fbcon.width)
		sx = (fbcon.width - box.x2) / 2;
	if (box.x2 < sna->front->drawable.width)
		dx = (sna->front->drawable.width - box.x2) / 2;

	sy = dy = 0;
	if (box.y2 < (uint16_t)fbcon.height)
		sy = (fbcon.height - box.y2) / 2;
	if (box.y2 < sna->front->drawable.height)
		dy = (sna->front->drawable.height - box.y2) / 2;

	ok = sna->render.copy_boxes(sna, GXcopy,
				    &scratch, bo, sx, sy,
				    sna->front, priv->gpu_bo, dx, dy,
				    &box, 1, 0);
	if (!DAMAGE_IS_ALL(priv->gpu_damage))
		sna_damage_add_box(&priv->gpu_damage, &box);

	kgem_bo_destroy(&sna->kgem, bo);

#if ABI_VIDEODRV_VERSION >= SET_ABI_VERSION(10, 0)
	sna->scrn->pScreen->canDoBGNoneRoot = ok;
#endif
}

static bool use_shadow(struct sna *sna, xf86CrtcPtr crtc)
{
	RRTransformPtr transform;
	PictTransform crtc_to_fb;
	struct pict_f_transform f_crtc_to_fb, f_fb_to_crtc;
	unsigned long pitch_limit;
	struct sna_pixmap *priv;
	BoxRec b;

	assert(sna->scrn->virtualX && sna->scrn->virtualY);

	if (sna->flags & SNA_FORCE_SHADOW) {
		DBG(("%s: forcing shadow\n", __FUNCTION__));
		return true;
	}

	if (to_sna_crtc(crtc)->fallback_shadow) {
		DBG(("%s: fallback shadow\n", __FUNCTION__));
		return true;
	}

	if (sna->scrn->virtualX > sna->mode.kmode->max_width ||
	    sna->scrn->virtualY > sna->mode.kmode->max_height) {
		DBG(("%s: framebuffer too large (%dx%d) > (%dx%d)\n",
		    __FUNCTION__,
		    sna->scrn->virtualX, sna->scrn->virtualY,
		    sna->mode.kmode->max_width,
		    sna->mode.kmode->max_height));
		return true;
	}

	priv = sna_pixmap_force_to_gpu(sna->front, MOVE_READ | MOVE_WRITE);
	if (priv == NULL)
		return true; /* maybe we can create a bo for the scanout? */

	if (sna->kgem.gen == 071)
		pitch_limit = priv->gpu_bo->tiling ? 16 * 1024 : 32 * 1024;
	else if ((sna->kgem.gen >> 3) > 4)
		pitch_limit = 32 * 1024;
	else if ((sna->kgem.gen >> 3) == 4)
		pitch_limit = priv->gpu_bo->tiling ? 16 * 1024 : 32 * 1024;
	else if ((sna->kgem.gen >> 3) == 3)
		pitch_limit = priv->gpu_bo->tiling ? 8 * 1024 : 16 * 1024;
	else
		pitch_limit = 8 * 1024;
	if (priv->gpu_bo->pitch > pitch_limit)
		return true;

	transform = NULL;
	if (crtc->transformPresent)
		transform = &crtc->transform;
	if (RRTransformCompute(crtc->x, crtc->y,
			       crtc->mode.HDisplay, crtc->mode.VDisplay,
			       crtc->rotation, transform,
			       &crtc_to_fb,
			       &f_crtc_to_fb,
			       &f_fb_to_crtc)) {
		DBG(("%s: RandR transform present\n", __FUNCTION__));
		return true;
	}

	/* And finally check that it is entirely visible */
	b.x1 = b.y1 = 0;
	b.x2 = crtc->mode.HDisplay;
	b.y2 = crtc->mode.VDisplay;
	pixman_f_transform_bounds(&f_crtc_to_fb, &b);
	DBG(("%s? bounds (%d, %d), (%d, %d), framebufer %dx%d\n",
	     __FUNCTION__, b.x1, b.y1, b.x2, b.y2,
		 sna->scrn->virtualX, sna->scrn->virtualY));

	if  (b.x1 < 0 || b.y1 < 0 ||
	     b.x2 > sna->scrn->virtualX ||
	     b.y2 > sna->scrn->virtualY) {
		DBG(("%s: scanout is partly outside the framebuffer\n",
		     __FUNCTION__));
		return true;
	}

	return false;
}

static struct kgem_bo *sna_crtc_attach(xf86CrtcPtr crtc)
{
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	ScrnInfoPtr scrn = crtc->scrn;
	struct sna *sna = to_sna(scrn);
	struct kgem_bo *bo;

	sna_crtc->transform = false;
	if (sna_crtc->scanout_pixmap) {
		DBG(("%s: attaching to scanout pixmap\n", __FUNCTION__));

		bo = sna_pixmap_pin(sna_crtc->scanout_pixmap, PIN_SCANOUT);
		if (bo == NULL)
			return NULL;

		if (!get_fb(sna, bo,
			    sna_crtc->scanout_pixmap->drawable.width,
			    sna_crtc->scanout_pixmap->drawable.height))
			return NULL;

		sna_crtc->transform = true;
		return kgem_bo_reference(bo);
	} else if (use_shadow(sna, crtc)) {
		unsigned long tiled_limit;
		int tiling;

		if (!sna_crtc_enable_shadow(sna, sna_crtc))
			return NULL;

		DBG(("%s: attaching to per-crtc pixmap %dx%d\n",
		     __FUNCTION__, crtc->mode.HDisplay, crtc->mode.VDisplay));

		tiling = I915_TILING_X;
		if (sna->kgem.gen == 071)
			tiled_limit = 16 * 1024 * 8;
		else if ((sna->kgem.gen >> 3) > 4)
			tiled_limit = 32 * 1024 * 8;
		else if ((sna->kgem.gen >> 3) == 4)
			tiled_limit = 16 * 1024 * 8;
		else
			tiled_limit = 8 * 1024 * 8;
		if ((unsigned long)crtc->mode.HDisplay * scrn->bitsPerPixel > tiled_limit)
			tiling = I915_TILING_NONE;

		bo = kgem_create_2d(&sna->kgem,
				    crtc->mode.HDisplay, crtc->mode.VDisplay,
				    scrn->bitsPerPixel,
				    tiling, CREATE_SCANOUT);
		if (bo == NULL)
			return NULL;

		if (!get_fb(sna, bo, crtc->mode.HDisplay, crtc->mode.VDisplay)) {
			kgem_bo_destroy(&sna->kgem, bo);
			return NULL;
		}

		sna_crtc->transform = true;
		return bo;
	} else if (sna->flags & SNA_TEAR_FREE) {
		DBG(("%s: tear-free updates requested\n", __FUNCTION__));

		if (!sna_crtc_enable_shadow(sna, sna_crtc))
			return NULL;

		DBG(("%s: attaching to single shadow pixmap\n", __FUNCTION__));
		if (sna->mode.shadow == NULL) {
			BoxRec box;

			bo = kgem_create_2d(&sna->kgem,
					    sna->scrn->virtualX,
					    sna->scrn->virtualY,
					    scrn->bitsPerPixel,
					    I915_TILING_X,
					    CREATE_SCANOUT);
			if (bo == NULL)
				return NULL;

			box.x1 = box.y1 = 0;
			box.x2 = sna->scrn->virtualX;
			box.y2 = sna->scrn->virtualY;

			if (!sna->render.copy_boxes(sna, GXcopy,
						    sna->front, sna_pixmap(sna->front)->gpu_bo, 0, 0,
						    sna->front, bo, 0, 0,
						    &box, 1, COPY_LAST)) {
				kgem_bo_destroy(&sna->kgem, bo);
				return NULL;
			}

			if (!get_fb(sna, bo,
				    sna->scrn->virtualX,
				    sna->scrn->virtualY)) {
				kgem_bo_destroy(&sna->kgem, bo);
				return NULL;
			}

			sna->mode.shadow = bo;
		}

		return kgem_bo_reference(sna->mode.shadow);
	} else {
		DBG(("%s: attaching to framebuffer\n", __FUNCTION__));
		sna_crtc_disable_shadow(sna, sna_crtc);
		bo = sna_pixmap_pin(sna->front, PIN_SCANOUT);
		if (bo == NULL)
			return NULL;

		if (!get_fb(sna, bo, scrn->virtualX, scrn->virtualY))
			return NULL;

		assert(!sna_crtc->shadow);
		return kgem_bo_reference(bo);
	}
}

static void sna_crtc_randr(xf86CrtcPtr crtc)
{
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	struct pict_f_transform f_crtc_to_fb, f_fb_to_crtc;
	PictTransform crtc_to_fb;
	PictFilterPtr filter;
	xFixed *params;
	int nparams;
	RRTransformPtr transform;

	transform = NULL;
	if (crtc->transformPresent)
		transform = &crtc->transform;

	RRTransformCompute(crtc->x, crtc->y,
			   crtc->mode.HDisplay, crtc->mode.VDisplay,
			   crtc->rotation, transform,
			   &crtc_to_fb,
			   &f_crtc_to_fb,
			   &f_fb_to_crtc);

	filter = NULL;
	params = NULL;
	nparams = 0;
	if (sna_crtc->transform) {
#ifdef RANDR_12_INTERFACE
		if (transform) {
			if (transform->nparams) {
				params = malloc(transform->nparams * sizeof(xFixed));
				if (params) {
					memcpy(params, transform->params,
					       transform->nparams * sizeof(xFixed));
					nparams = transform->nparams;
					filter = transform->filter;
				}
			} else
				filter = transform->filter;
		}
#endif
		crtc->transform_in_use = TRUE;
	} else
		crtc->transform_in_use = FALSE;

	crtc->crtc_to_framebuffer = crtc_to_fb;
	crtc->f_crtc_to_framebuffer = f_crtc_to_fb;
	crtc->f_framebuffer_to_crtc = f_fb_to_crtc;

	free(crtc->params);
	crtc->params  = params;
	crtc->nparams = nparams;

	crtc->filter = filter;
	if (filter) {
		crtc->filter_width  = filter->width;
		crtc->filter_height = filter->height;
	} else {
		crtc->filter_width  = 0;
		crtc->filter_height = 0;
	}

	crtc->bounds.x1 = 0;
	crtc->bounds.x2 = crtc->mode.HDisplay;
	crtc->bounds.y1 = 0;
	crtc->bounds.y2 = crtc->mode.VDisplay;
	pixman_f_transform_bounds(&f_crtc_to_fb, &crtc->bounds);

	DBG(("%s: transform? %d, bounds (%d, %d), (%d, %d)\n",
	     __FUNCTION__, crtc->transform_in_use,
	     crtc->bounds.x1, crtc->bounds.y1,
	     crtc->bounds.x2, crtc->bounds.y2));
}

static void
sna_crtc_damage(xf86CrtcPtr crtc)
{
	ScreenPtr screen = crtc->scrn->pScreen;
	struct sna *sna = to_sna(crtc->scrn);
	RegionRec region, *damage;

	region.extents = crtc->bounds;
	region.data = NULL;

	if (region.extents.x1 < 0)
		region.extents.x1 = 0;
	if (region.extents.y1 < 0)
		region.extents.y1 = 0;
	if (region.extents.x2 > screen->width)
		region.extents.x2 = screen->width;
	if (region.extents.y2 > screen->height)
		region.extents.y2 = screen->height;

	DBG(("%s: marking crtc %d as completely damaged (%d, %d), (%d, %d)\n",
	     __FUNCTION__, to_sna_crtc(crtc)->id,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	assert(sna->mode.shadow_damage && sna->mode.shadow_active);
	damage = DamageRegion(sna->mode.shadow_damage);
	RegionUnion(damage, damage, &region);
}

static char *outputs_for_crtc(xf86CrtcPtr crtc, char *outputs, int max)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
	int len, i;

	for (i = len = 0; i < config->num_output; i++) {
		xf86OutputPtr output = config->output[i];

		if (output->crtc != crtc)
			continue;

		len += snprintf(outputs+len, max-len, "%s, ", output->name);
	}
	assert(len >= 2);
	outputs[len-2] = '\0';

	return outputs;
}

static const char *rotation_to_str(Rotation rotation)
{
	switch (rotation & RR_Rotate_All) {
	case 0:
	case RR_Rotate_0: return "normal";
	case RR_Rotate_90: return "left";
	case RR_Rotate_180: return "inverted";
	case RR_Rotate_270: return "right";
	default: return "unknown";
	}
}

static const char *reflection_to_str(Rotation rotation)
{
	switch (rotation & RR_Reflect_All) {
	case 0: return "none";
	case RR_Reflect_X: return "X axis";
	case RR_Reflect_Y: return "Y axis";
	case RR_Reflect_X | RR_Reflect_Y: return "X and Y axes";
	default: return "invalid";
	}
}

static Bool
sna_crtc_set_mode_major(xf86CrtcPtr crtc, DisplayModePtr mode,
			Rotation rotation, int x, int y)
{
	ScrnInfoPtr scrn = crtc->scrn;
	struct sna *sna = to_sna(scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	struct kgem_bo *saved_bo, *bo;
	struct drm_mode_modeinfo saved_kmode;
	bool saved_transform;
	char outputs[256];

	if (mode->HDisplay == 0 || mode->VDisplay == 0)
		return FALSE;

	xf86DrvMsg(crtc->scrn->scrnIndex, X_INFO,
		   "switch to mode %dx%d@%.1f on %s using pipe %d, position (%d, %d), rotation %s, reflection %s\n",
		   mode->HDisplay, mode->VDisplay, xf86ModeVRefresh(mode),
		   outputs_for_crtc(crtc, outputs, sizeof(outputs)), sna_crtc->pipe,
		   x, y, rotation_to_str(rotation), reflection_to_str(rotation));

	assert(mode->HDisplay <= sna->mode.kmode->max_width &&
	       mode->VDisplay <= sna->mode.kmode->max_height);

#if HAS_GAMMA
	drmModeCrtcSetGamma(sna->kgem.fd, sna_crtc->id,
			    crtc->gamma_size,
			    crtc->gamma_red,
			    crtc->gamma_green,
			    crtc->gamma_blue);
#endif

	saved_kmode = sna_crtc->kmode;
	saved_bo = sna_crtc->bo;
	saved_transform = sna_crtc->transform;

	sna_crtc->fallback_shadow = false;
retry: /* Attach per-crtc pixmap or direct */
	bo = sna_crtc_attach(crtc);
	if (bo == NULL)
		return FALSE;

	kgem_bo_submit(&sna->kgem, bo);

	sna_crtc->bo = bo;
	mode_to_kmode(&sna_crtc->kmode, mode);
	if (!sna_crtc_apply(crtc)) {
		kgem_bo_destroy(&sna->kgem, bo);

		if (!sna_crtc->shadow) {
			sna_crtc->fallback_shadow = true;
			goto retry;
		}

		xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR,
			   "failed to set mode: %s\n", strerror(errno));

		sna_crtc->transform = saved_transform;
		sna_crtc->bo = saved_bo;
		sna_crtc->kmode = saved_kmode;
		return FALSE;
	}
	if (saved_bo)
		kgem_bo_destroy(&sna->kgem, saved_bo);

	sna_crtc_randr(crtc);
	if (sna_crtc->shadow)
		sna_crtc_damage(crtc);

	if (scrn->pScreen)
		xf86_reload_cursors(scrn->pScreen);

	return TRUE;
}

static void
sna_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
	struct sna_crtc *priv = to_sna_crtc(crtc);

	DBG(("%s(pipe %d, dpms mode -> %d):= active=%d\n",
	     __FUNCTION__, priv->pipe, mode, mode == DPMSModeOn));
	if (priv->dpms_mode == mode)
		return;

	if (mode == DPMSModeOn) {
		if (priv->bo == NULL &&
		    !sna_crtc_set_mode_major(crtc,
					     &crtc->mode, crtc->rotation,
					     crtc->x, crtc->y))
			sna_crtc_disable(crtc);
	} else
		sna_crtc_disable(crtc);

	if (priv->bo != NULL) {
		priv->dpms_mode = mode;
		update_flush_interval(to_sna(crtc->scrn));
	}
}

void sna_mode_adjust_frame(struct sna *sna, int x, int y)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	xf86CrtcPtr crtc;

	if ((unsigned)config->compat_output >= config->num_output)
		return;

	crtc = config->output[config->compat_output]->crtc;
	if (crtc && crtc->enabled) {
		int saved_x = crtc->x;
		int saved_y = crtc->y;

		crtc->x = x;
		crtc->y = y;
		if (!sna_crtc_set_mode_major(crtc, &crtc->mode,
					     crtc->rotation, x, y)) {
			crtc->x = saved_x;
			crtc->y = saved_y;
		}
	}
}

static void
sna_crtc_hide_cursor(xf86CrtcPtr crtc)
{
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	struct drm_mode_cursor arg;

	__DBG(("%s: CRTC:%d\n", __FUNCTION__, sna_crtc->id));

	VG_CLEAR(arg);
	arg.flags = DRM_MODE_CURSOR_BO;
	arg.crtc_id = sna_crtc->id;
	arg.width = arg.height = 64;
	arg.handle = 0;

	(void)drmIoctl(to_sna(crtc->scrn)->kgem.fd, DRM_IOCTL_MODE_CURSOR, &arg);
}

static void
sna_crtc_show_cursor(xf86CrtcPtr crtc)
{
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	struct drm_mode_cursor arg;

	__DBG(("%s: CRTC:%d\n", __FUNCTION__, sna_crtc->id));

	VG_CLEAR(arg);
	arg.flags = DRM_MODE_CURSOR_BO;
	arg.crtc_id = sna_crtc->id;
	arg.width = arg.height = 64;
	arg.handle = sna_crtc->cursor;

	(void)drmIoctl(to_sna(crtc->scrn)->kgem.fd, DRM_IOCTL_MODE_CURSOR, &arg);
}

static void
sna_crtc_set_cursor_colors(xf86CrtcPtr crtc, int bg, int fg)
{
	__DBG(("%s: CRTC:%d (bg=%x, fg=%x)\n", __FUNCTION__,
	       to_sna_crtc(crtc)->id, bg, fg));
}

static void
sna_crtc_set_cursor_position(xf86CrtcPtr crtc, int x, int y)
{
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	struct drm_mode_cursor arg;

	__DBG(("%s: CRTC:%d (%d, %d)\n", __FUNCTION__, sna_crtc->id, x, y));

	VG_CLEAR(arg);
	arg.flags = DRM_MODE_CURSOR_MOVE;
	arg.crtc_id = sna_crtc->id;
	arg.x = x;
	arg.y = y;
	arg.handle = sna_crtc->cursor;

	(void)drmIoctl(to_sna(crtc->scrn)->kgem.fd, DRM_IOCTL_MODE_CURSOR, &arg);
}

static void
sna_crtc_load_cursor_argb(xf86CrtcPtr crtc, CARD32 *image)
{
	struct drm_i915_gem_pwrite pwrite;

	__DBG(("%s: CRTC:%d\n", __FUNCTION__, to_sna_crtc(crtc)->id));

	VG_CLEAR(pwrite);
	pwrite.handle = to_sna_crtc(crtc)->cursor;
	pwrite.offset = 0;
	pwrite.size = 64*64*4;
	pwrite.data_ptr = (uintptr_t)image;
	(void)drmIoctl(to_sna(crtc->scrn)->kgem.fd, DRM_IOCTL_I915_GEM_PWRITE, &pwrite);
}

static void
sna_crtc_gamma_set(xf86CrtcPtr crtc,
		       CARD16 *red, CARD16 *green, CARD16 *blue, int size)
{
	drmModeCrtcSetGamma(to_sna(crtc->scrn)->kgem.fd,
			    to_sna_crtc(crtc)->id,
			    size, red, green, blue);
}

static void
sna_crtc_destroy(xf86CrtcPtr crtc)
{
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);

	if (sna_crtc == NULL)
		return;

	sna_crtc_hide_cursor(crtc);
	gem_close(to_sna(crtc->scrn)->kgem.fd, sna_crtc->cursor);

	free(sna_crtc);
	crtc->driver_private = NULL;
}

#if HAS_PIXMAP_SHARING
static Bool
sna_crtc_set_scanout_pixmap(xf86CrtcPtr crtc, PixmapPtr pixmap)
{
	DBG(("%s: CRTC:%d, pipe=%d setting scanout pixmap=%ld\n",
	     __FUNCTION__,to_sna_crtc(crtc)->id, to_sna_crtc(crtc)->pipe,
	     pixmap ? pixmap->drawable.serialNumber : 0));
	to_sna_crtc(crtc)->scanout_pixmap = pixmap;
	return TRUE;
}
#endif

static const xf86CrtcFuncsRec sna_crtc_funcs = {
#if XF86_CRTC_VERSION >= 1
	.dpms = sna_crtc_dpms,
#endif
	.set_mode_major = sna_crtc_set_mode_major,
	.set_cursor_colors = sna_crtc_set_cursor_colors,
	.set_cursor_position = sna_crtc_set_cursor_position,
	.show_cursor = sna_crtc_show_cursor,
	.hide_cursor = sna_crtc_hide_cursor,
	.load_cursor_argb = sna_crtc_load_cursor_argb,
	.gamma_set = sna_crtc_gamma_set,
	.destroy = sna_crtc_destroy,
#if HAS_PIXMAP_SHARING
	.set_scanout_pixmap = sna_crtc_set_scanout_pixmap,
#endif
};

static int
sna_crtc_find_plane(struct sna *sna, int pipe)
{
#ifdef DRM_IOCTL_MODE_GETPLANERESOURCES
	struct drm_mode_get_plane_res r;
	uint32_t *planes, id = 0;
	int i;

	VG_CLEAR(r);
	r.count_planes = 0;
	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETPLANERESOURCES, &r))
		return 0;

	if (!r.count_planes)
		return 0;

	planes = malloc(sizeof(uint32_t)*r.count_planes);
	if (planes == NULL)
		return 0;

	r.plane_id_ptr = (uintptr_t)planes;
	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETPLANERESOURCES, &r))
		r.count_planes = 0;

	VG(VALGRIND_MAKE_MEM_DEFINED(planes, sizeof(uint32_t)*r.count_planes));

	for (i = 0; i < r.count_planes; i++) {
		struct drm_mode_get_plane p;

		VG_CLEAR(p);
		p.plane_id = planes[i];
		p.count_format_types = 0;
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETPLANE, &p) == 0) {
			if (p.possible_crtcs & (1 << pipe)) {
				id = p.plane_id;
				break;
			}
		}
	}
	free(planes);

	assert(id);
	return id;
#else
	return 0;
#endif
}

static bool
sna_crtc_init(ScrnInfoPtr scrn, struct sna_mode *mode, int num)
{
	struct sna *sna = to_sna(scrn);
	xf86CrtcPtr crtc;
	struct sna_crtc *sna_crtc;
	struct drm_i915_get_pipe_from_crtc_id get_pipe;

	DBG(("%s\n", __FUNCTION__));

	sna_crtc = calloc(sizeof(struct sna_crtc), 1);
	if (sna_crtc == NULL)
		return false;

	sna_crtc->id = mode->kmode->crtcs[num];
	sna_crtc->dpms_mode = DPMSModeOff;

	VG_CLEAR(get_pipe);
	get_pipe.pipe = 0;
	get_pipe.crtc_id = sna_crtc->id;
	if (drmIoctl(sna->kgem.fd,
		     DRM_IOCTL_I915_GET_PIPE_FROM_CRTC_ID,
		     &get_pipe)) {
		free(sna_crtc);
		return false;
	}
	sna_crtc->pipe = get_pipe.pipe;
	sna_crtc->plane = sna_crtc_find_plane(sna, sna_crtc->pipe);

	if (xf86IsEntityShared(scrn->entityList[0]) &&
	    scrn->confScreen->device->screen != sna_crtc->pipe) {
		free(sna_crtc);
		return true;
	}

	crtc = xf86CrtcCreate(scrn, &sna_crtc_funcs);
	if (crtc == NULL) {
		free(sna_crtc);
		return false;
	}

	sna_crtc->cursor = gem_create(sna->kgem.fd, 64*64*4);
	if (!sna_crtc->cursor) {
		xf86CrtcDestroy(crtc);
		free(sna_crtc);
		return false;
	}
	DBG(("%s: created handle=%d for cursor on CRTC:%d\n",
	     __FUNCTION__, sna_crtc->cursor, sna_crtc->id));

	crtc->driver_private = sna_crtc;
	DBG(("%s: attached crtc[%d] id=%d, pipe=%d\n",
	     __FUNCTION__, num, sna_crtc->id, sna_crtc->pipe));

	return true;
}

static bool
is_panel(int type)
{
	return (type == DRM_MODE_CONNECTOR_LVDS ||
		type == DRM_MODE_CONNECTOR_eDP);
}

static int
find_property(struct sna *sna, struct sna_output *output, const char *name)
{
	struct drm_mode_get_property prop;
	int i;

	VG_CLEAR(prop);
	for (i = 0; i < output->num_props; i++) {
		prop.prop_id = output->prop_ids[i];
		prop.count_values = 0;
		prop.count_enum_blobs = 0;
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETPROPERTY, &prop))
			continue;

		if (strcmp(prop.name, name) == 0)
			return i;
	}

	return -1;
}

static int
find_property_id(struct sna *sna, struct sna_output *output, const char *name)
{
	int idx = find_property(sna, output, name);
	return idx != -1 ? output->prop_ids[idx] : 0;
}

static xf86OutputStatus
sna_output_detect(xf86OutputPtr output)
{
	struct sna *sna = to_sna(output->scrn);
	struct sna_output *sna_output = output->driver_private;
	union compat_mode_get_connector compat_conn;

	DBG(("%s(%s)\n", __FUNCTION__, output->name));

	VG_CLEAR(compat_conn);
	compat_conn.conn.connector_id = sna_output->id;
	sna_output->num_modes = compat_conn.conn.count_modes = 0; /* reprobe */
	compat_conn.conn.count_encoders = 0;
	compat_conn.conn.count_props = sna_output->num_props;
	compat_conn.conn.props_ptr = (uintptr_t)sna_output->prop_ids;
	compat_conn.conn.prop_values_ptr = (uintptr_t)sna_output->prop_values;

	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETCONNECTOR, &compat_conn.conn))
		return XF86OutputStatusUnknown;
	DBG(("%s(%s): num modes %d -> %d, num props %d -> %d\n",
	     __FUNCTION__, output->name,
	     sna_output->num_modes, compat_conn.conn.count_modes,
	     sna_output->num_props, compat_conn.conn.count_props));

	assert(compat_conn.conn.count_props == sna_output->num_props);

	while (compat_conn.conn.count_modes && compat_conn.conn.count_modes != sna_output->num_modes) {
		struct drm_mode_modeinfo *new_modes;
		int old_count;

		old_count = sna_output->num_modes;
		new_modes = realloc(sna_output->modes,
				    sizeof(*sna_output->modes)*compat_conn.conn.count_modes);
		if (new_modes == NULL)
			break;

		sna_output->modes = new_modes;
		sna_output->num_modes = compat_conn.conn.count_modes;
		compat_conn.conn.modes_ptr = (uintptr_t)sna_output->modes;
		compat_conn.conn.count_encoders = 0;
		compat_conn.conn.count_props = 0;
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETCONNECTOR, &compat_conn.conn)) {
			sna_output->num_modes = min(old_count, sna_output->num_modes);
			break;
		}
		VG(VALGRIND_MAKE_MEM_DEFINED(sna_output->modes, sizeof(*sna_output->modes)*sna_output->num_modes));
	}

	DBG(("%s(%s): found %d modes, connection status=%d\n",
	     __FUNCTION__, output->name, sna_output->num_modes, compat_conn.conn.connection));

	switch (compat_conn.conn.connection) {
	case DRM_MODE_CONNECTED:
		return XF86OutputStatusConnected;
	case DRM_MODE_DISCONNECTED:
		return XF86OutputStatusDisconnected;
	default:
	case DRM_MODE_UNKNOWNCONNECTION:
		return XF86OutputStatusUnknown;
	}
}

static Bool
sna_output_mode_valid(xf86OutputPtr output, DisplayModePtr mode)
{
	struct sna_output *sna_output = output->driver_private;
	struct sna *sna = to_sna(output->scrn);

	if (mode->HDisplay > sna->mode.kmode->max_width)
		return MODE_VIRTUAL_X;
	if (mode->VDisplay > sna->mode.kmode->max_height)
		return MODE_VIRTUAL_Y;

	/* Check that we can successfully pin this into the global GTT */
	if ((kgem_can_create_2d(&sna->kgem,
				mode->HDisplay, mode->VDisplay,
				sna->scrn->bitsPerPixel) & KGEM_CAN_CREATE_GTT) == 0)
		return MODE_MEM_VIRT;

	/*
	 * If the connector type is a panel, we will use the panel limit to
	 * verfiy whether the mode is valid.
	 */
	if (sna_output->has_panel_limits) {
		if (mode->HDisplay > sna_output->panel_hdisplay ||
		    mode->VDisplay > sna_output->panel_vdisplay)
			return MODE_PANEL;
	}

	return MODE_OK;
}

static void
sna_output_attach_edid(xf86OutputPtr output)
{
	struct sna *sna = to_sna(output->scrn);
	struct sna_output *sna_output = output->driver_private;
	struct drm_mode_get_blob blob;
	void *old, *raw = NULL;
	xf86MonPtr mon = NULL;

	if (sna_output->edid_idx == -1)
		return;

	raw = sna_output->edid_raw;
	blob.length = sna_output->edid_len;

	if (blob.length && output->MonInfo) {
		old = alloca(blob.length);
		memcpy(old, raw, blob.length);
	} else
		old = NULL;

	blob.blob_id = sna_output->prop_values[sna_output->edid_idx];
	DBG(("%s: attaching EDID id=%d, current=%d\n",
	     __FUNCTION__, blob.blob_id, sna_output->edid_blob_id));
	if (blob.blob_id == sna_output->edid_blob_id && 0) { /* sigh */
		if (output->MonInfo) {
			/* XXX the property keeps on disappearing... */
			RRChangeOutputProperty(output->randr_output,
					       MakeAtom("EDID", strlen("EDID"), TRUE),
					       XA_INTEGER, 8, PropModeReplace,
					       sna_output->edid_len,
					       sna_output->edid_raw,
					       FALSE, FALSE);

			return;
		}

		goto skip_read;
	}

	blob.data = (uintptr_t)raw;
	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETPROPBLOB, &blob))
		goto done;

	DBG(("%s: retrieving blob id=%d, length=%d\n",
	     __FUNCTION__, blob.blob_id, blob.length));

	if (blob.length > sna_output->edid_len) {
		raw = realloc(raw, blob.length);
		if (raw == NULL)
			goto done;

		VG(memset(raw, 0, blob.length));
		blob.data = (uintptr_t)raw;
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETPROPBLOB, &blob))
			goto done;
	}

	if (old &&
	    blob.length == sna_output->edid_len &&
	    memcmp(old, raw, blob.length) == 0) {
		assert(sna_output->edid_raw == raw);
		sna_output->edid_blob_id = blob.blob_id;
		RRChangeOutputProperty(output->randr_output,
				       MakeAtom("EDID", strlen("EDID"), TRUE),
				       XA_INTEGER, 8, PropModeReplace,
				       sna_output->edid_len,
				       sna_output->edid_raw,
				       FALSE, FALSE);
		return;
	}

skip_read:
	if (raw) {
		mon = xf86InterpretEDID(output->scrn->scrnIndex, raw);
		if (mon && blob.length > 128)
			mon->flags |= MONITOR_EDID_COMPLETE_RAWDATA;
	}

done:
	xf86OutputSetEDID(output, mon);
	if (raw) {
		sna_output->edid_raw = raw;
		sna_output->edid_len = blob.length;
		sna_output->edid_blob_id = blob.blob_id;
	}
}

static DisplayModePtr
default_modes(void)
{
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,6,99,900,0)
	return xf86GetDefaultModes();
#else
	return xf86GetDefaultModes(0, 0);
#endif
}

static DisplayModePtr
sna_output_panel_edid(xf86OutputPtr output, DisplayModePtr modes)
{
	xf86MonPtr mon = output->MonInfo;
	DisplayModePtr i, m, preferred = NULL;
	int max_x = 0, max_y = 0;
	float max_vrefresh = 0.0;

	if (mon && GTF_SUPPORTED(mon->features.msc))
		return modes;

	for (m = modes; m; m = m->next) {
		if (m->type & M_T_PREFERRED)
			preferred = m;
		max_x = max(max_x, m->HDisplay);
		max_y = max(max_y, m->VDisplay);
		max_vrefresh = max(max_vrefresh, xf86ModeVRefresh(m));
	}

	max_vrefresh = max(max_vrefresh, 60.0);
	max_vrefresh *= (1 + SYNC_TOLERANCE);

	m = default_modes();
	xf86ValidateModesSize(output->scrn, m, max_x, max_y, 0);

	for (i = m; i; i = i->next) {
		if (xf86ModeVRefresh(i) > max_vrefresh)
			i->status = MODE_VSYNC;
		if (preferred &&
		    i->HDisplay >= preferred->HDisplay &&
		    i->VDisplay >= preferred->VDisplay &&
		    xf86ModeVRefresh(i) >= xf86ModeVRefresh(preferred))
			i->status = MODE_PANEL;
	}

	xf86PruneInvalidModes(output->scrn, &m, FALSE);

	return xf86ModesAdd(modes, m);
}

static char *canonical_mode_name(DisplayModePtr mode)
{
	char tmp[32], *buf;
	int len;

	len = sprintf(tmp, "%dx%d%s",
		      mode->HDisplay, mode->VDisplay,
		      mode->Flags & V_INTERLACE ? "i" : "");
	if ((unsigned)len >= sizeof(tmp))
		return NULL;

	buf = malloc(len + 1);
	if (buf == NULL)
		return NULL;

	return memcpy(buf, tmp, len + 1);
}

static DisplayModePtr
sna_output_get_modes(xf86OutputPtr output)
{
	struct sna_output *sna_output = output->driver_private;
	DisplayModePtr Modes = NULL, Mode, current = NULL;
	int i;

	DBG(("%s(%s)\n", __FUNCTION__, output->name));

	sna_output_attach_edid(output);

	if (output->crtc) {
		struct drm_mode_crtc mode;

		VG_CLEAR(mode);
		mode.crtc_id = to_sna_crtc(output->crtc)->id;

		if (drmIoctl(to_sna(output->scrn)->kgem.fd, DRM_IOCTL_MODE_GETCRTC, &mode) == 0) {
			DBG(("%s: CRTC:%d, pipe=%d: has mode?=%d\n", __FUNCTION__,
			     to_sna_crtc(output->crtc)->id,
			     to_sna_crtc(output->crtc)->pipe,
			     mode.mode_valid && mode.mode.clock));

			if (mode.mode_valid && mode.mode.clock) {
				current = calloc(1, sizeof(DisplayModeRec));
				if (current) {
					mode_from_kmode(output->scrn, &mode.mode, current);
					Modes = xf86ModesAdd(Modes, current);
				}
			}
		}
	}

	DBG(("%s: adding %d probed modes\n", __FUNCTION__, sna_output->num_modes));

	Mode = NULL;
	for (i = 0; i < sna_output->num_modes; i++) {
		if (Mode == NULL)
			Mode = calloc(1, sizeof(DisplayModeRec));
		if (Mode) {
			Mode = mode_from_kmode(output->scrn,
					       &sna_output->modes[i],
					       Mode);

			if (!current || !xf86ModesEqual(Mode, current)) {
				Modes = xf86ModesAdd(Modes, Mode);
				Mode = NULL;
			} else {
				current->name = strdup(Mode->name);
				current->type = Mode->type;
			}
		}
	}
	free(Mode);

	if (current && (current->name == NULL || *current->name == '\0')) {
		char *str = canonical_mode_name(current);
		if (str) {
			free((char *)current->name);
			current->name = str;
		}
	}

	/*
	 * If the connector type is a panel, we will traverse the kernel mode to
	 * get the panel limit. And then add all the standard modes to fake
	 * the fullscreen experience.
	 * If it is incorrect, please fix me.
	 */
	sna_output->has_panel_limits = false;
	if (sna_output->is_panel) {
		sna_output->panel_hdisplay = sna_output->panel_vdisplay = 0;
		for (i = 0; i < sna_output->num_modes; i++) {
			struct drm_mode_modeinfo *m;

			m = &sna_output->modes[i];
			if (m->hdisplay > sna_output->panel_hdisplay)
				sna_output->panel_hdisplay = m->hdisplay;
			if (m->vdisplay > sna_output->panel_vdisplay)
				sna_output->panel_vdisplay = m->vdisplay;
		}
		sna_output->has_panel_limits =
			sna_output->panel_hdisplay &&
			sna_output->panel_vdisplay;

		Modes = sna_output_panel_edid(output, Modes);
	}


	return Modes;
}

static void
sna_output_destroy(xf86OutputPtr output)
{
	struct sna_output *sna_output = output->driver_private;
	int i;

	if (sna_output == NULL)
		return;

	free(sna_output->edid_raw);
	for (i = 0; i < sna_output->num_props; i++) {
		drmModeFreeProperty(sna_output->props[i].kprop);
		free(sna_output->props[i].atoms);
	}
	free(sna_output->props);
	free(sna_output->prop_ids);
	free(sna_output->prop_values);

	free(sna_output->backlight_iface);

	free(sna_output);
	output->driver_private = NULL;
}

static void
sna_output_dpms_backlight(xf86OutputPtr output, int oldmode, int mode)
{
	struct sna_output *sna_output = output->driver_private;

	if (!sna_output->backlight_iface)
		return;

	DBG(("%s(%s) -- %d -> %d\n", __FUNCTION__, output->name, oldmode, mode));

	if (mode == DPMSModeOn) {
		/* If we're going from off->on we may need to turn on the backlight. */
		if (oldmode != DPMSModeOn)
			sna_output_backlight_set(output,
						   sna_output->backlight_active_level);
	} else {
		/* Only save the current backlight value if we're going from on to off. */
		if (oldmode == DPMSModeOn)
			sna_output->backlight_active_level = sna_output_backlight_get(output);
		sna_output_backlight_set(output, 0);
	}
}

static void
sna_output_dpms(xf86OutputPtr output, int dpms)
{
	struct sna *sna = to_sna(output->scrn);
	struct sna_output *sna_output = output->driver_private;

	DBG(("%s(%s): dpms=%d (current: %d), active? %d\n",
	     __FUNCTION__, output->name,
	     dpms, sna_output->dpms_mode,
	     output->crtc != NULL));

	if (sna_output->dpms_mode == dpms)
		return;

	/* Record the value of the backlight before turning
	 * off the display, and reset if after turning it on.
	 * Order is important as the kernel may record and also
	 * reset the backlight across DPMS. Hence we need to
	 * record the value before the kernel modifies it
	 * and reapply it afterwards.
	 */
	if (dpms != DPMSModeOn)
		sna_output_dpms_backlight(output,
					  sna_output->dpms_mode,
					  dpms);

	if (output->crtc &&
	    drmModeConnectorSetProperty(sna->kgem.fd,
					sna_output->id,
					sna_output->dpms_id,
					dpms))
		dpms = sna_output->dpms_mode;

	if (dpms == DPMSModeOn)
		sna_output_dpms_backlight(output,
					  sna_output->dpms_mode,
					  dpms);

	sna_output->dpms_mode = dpms;
}

static bool
sna_property_ignore(drmModePropertyPtr prop)
{
	if (!prop)
		return true;

	/* ignore blob prop */
	if (prop->flags & DRM_MODE_PROP_BLOB)
		return true;

	/* ignore standard property */
	if (!strcmp(prop->name, "EDID") ||
	    !strcmp(prop->name, "DPMS"))
		return true;

	return false;
}

static void
sna_output_create_ranged_atom(xf86OutputPtr output, Atom *atom,
			      const char *name, INT32 min, INT32 max,
			      uint64_t value, Bool immutable)
{
	int err;
	INT32 atom_range[2];

	atom_range[0] = min;
	atom_range[1] = max;

	*atom = MakeAtom(name, strlen(name), TRUE);

	err = RRConfigureOutputProperty(output->randr_output, *atom, FALSE,
					TRUE, immutable, 2, atom_range);
	if (err != 0)
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			   "RRConfigureOutputProperty error, %d\n", err);

	err = RRChangeOutputProperty(output->randr_output, *atom, XA_INTEGER,
				     32, PropModeReplace, 1, &value,
				     FALSE, FALSE);
	if (err != 0)
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			   "RRChangeOutputProperty error, %d\n", err);
}

#define BACKLIGHT_NAME             "Backlight"
#define BACKLIGHT_DEPRECATED_NAME  "BACKLIGHT"
static Atom backlight_atom, backlight_deprecated_atom;

static void
sna_output_create_resources(xf86OutputPtr output)
{
	struct sna *sna = to_sna(output->scrn);
	struct sna_output *sna_output = output->driver_private;
	int i, j, err;

	sna_output->props = calloc(sna_output->num_props,
				   sizeof(struct sna_property));
	if (!sna_output->props)
		return;

	for (i = 0; i < sna_output->num_props; i++) {
		struct sna_property *p = &sna_output->props[i];

		p->kprop = drmModeGetProperty(sna->kgem.fd,
					      sna_output->prop_ids[i]);
		if (sna_property_ignore(p->kprop)) {
			drmModeFreeProperty(p->kprop);
			p->kprop = NULL;
			continue;
		}

		if (p->kprop->flags & DRM_MODE_PROP_RANGE) {
			p->num_atoms = 1;
			p->atoms = calloc(p->num_atoms, sizeof(Atom));
			if (!p->atoms)
				continue;

			sna_output_create_ranged_atom(output, &p->atoms[0],
						      p->kprop->name,
						      p->kprop->values[0],
						      p->kprop->values[1],
						      sna_output->prop_values[i],
						      p->kprop->flags & DRM_MODE_PROP_IMMUTABLE ? TRUE : FALSE);

		} else if (p->kprop->flags & DRM_MODE_PROP_ENUM) {
			p->num_atoms = p->kprop->count_enums + 1;
			p->atoms = calloc(p->num_atoms, sizeof(Atom));
			if (!p->atoms)
				continue;

			p->atoms[0] = MakeAtom(p->kprop->name, strlen(p->kprop->name), TRUE);
			for (j = 1; j <= p->kprop->count_enums; j++) {
				struct drm_mode_property_enum *e = &p->kprop->enums[j-1];
				p->atoms[j] = MakeAtom(e->name, strlen(e->name), TRUE);
			}

			err = RRConfigureOutputProperty(output->randr_output, p->atoms[0],
							FALSE, FALSE,
							p->kprop->flags & DRM_MODE_PROP_IMMUTABLE ? TRUE : FALSE,
							p->num_atoms - 1, (INT32 *)&p->atoms[1]);
			if (err != 0) {
				xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
					   "RRConfigureOutputProperty error, %d\n", err);
			}

			for (j = 0; j < p->kprop->count_enums; j++)
				if (p->kprop->enums[j].value == sna_output->prop_values[i])
					break;
			/* there's always a matching value */
			err = RRChangeOutputProperty(output->randr_output, p->atoms[0],
						     XA_ATOM, 32, PropModeReplace, 1, &p->atoms[j+1],
						     FALSE, FALSE);
			if (err != 0) {
				xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
					   "RRChangeOutputProperty error, %d\n", err);
			}
		}
	}

	if (sna_output->backlight_iface) {
		/* Set up the backlight property, which takes effect
		 * immediately and accepts values only within the
		 * backlight_range.
		 */
		sna_output_create_ranged_atom(output, &backlight_atom,
					      BACKLIGHT_NAME, 0,
					      sna_output->backlight_max,
					      sna_output->backlight_active_level,
					      FALSE);
		sna_output_create_ranged_atom(output,
					      &backlight_deprecated_atom,
					      BACKLIGHT_DEPRECATED_NAME, 0,
					      sna_output->backlight_max,
					      sna_output->backlight_active_level,
					      FALSE);
	}
}

static Bool
sna_output_set_property(xf86OutputPtr output, Atom property,
			RRPropertyValuePtr value)
{
	struct sna *sna = to_sna(output->scrn);
	struct sna_output *sna_output = output->driver_private;
	int i;

	if (property == backlight_atom || property == backlight_deprecated_atom) {
		INT32 val;

		if (value->type != XA_INTEGER || value->format != 32 ||
		    value->size != 1)
		{
			return FALSE;
		}

		val = *(INT32 *)value->data;
		DBG(("%s: setting backlight to %d (max=%d)\n",
		     __FUNCTION__, (int)val, sna_output->backlight_max));
		if (val < 0 || val > sna_output->backlight_max)
			return FALSE;

		if (sna_output->dpms_mode == DPMSModeOn)
			sna_output_backlight_set(output, val);
		sna_output->backlight_active_level = val;
		return TRUE;
	}

	for (i = 0; i < sna_output->num_props; i++) {
		struct sna_property *p = &sna_output->props[i];

		if (p->atoms == NULL || p->atoms[0] != property)
			continue;

		if (p->kprop->flags & DRM_MODE_PROP_RANGE) {
			uint32_t val;

			if (value->type != XA_INTEGER || value->format != 32 ||
			    value->size != 1)
				return FALSE;
			val = *(uint32_t *)value->data;

			drmModeConnectorSetProperty(sna->kgem.fd, sna_output->id,
						    p->kprop->prop_id, (uint64_t)val);
			return TRUE;
		} else if (p->kprop->flags & DRM_MODE_PROP_ENUM) {
			Atom	atom;
			const char	*name;
			int		j;

			if (value->type != XA_ATOM || value->format != 32 || value->size != 1)
				return FALSE;
			memcpy(&atom, value->data, 4);
			name = NameForAtom(atom);
			if (name == NULL)
				return FALSE;

			/* search for matching name string, then set its value down */
			for (j = 0; j < p->kprop->count_enums; j++) {
				if (!strcmp(p->kprop->enums[j].name, name)) {
					drmModeConnectorSetProperty(sna->kgem.fd, sna_output->id,
								    p->kprop->prop_id, p->kprop->enums[j].value);
					return TRUE;
				}
			}
			return FALSE;
		}
	}

	/* We didn't recognise this property, just report success in order
	 * to allow the set to continue, otherwise we break setting of
	 * common properties like EDID.
	 */
	return TRUE;
}

static Bool
sna_output_get_property(xf86OutputPtr output, Atom property)
{
	struct sna_output *sna_output = output->driver_private;
	int err;

	if (property == backlight_atom || property == backlight_deprecated_atom) {
		INT32 val;

		if (!sna_output->backlight_iface)
			return FALSE;

		val = sna_output_backlight_get(output);
		if (val < 0)
			return FALSE;

		err = RRChangeOutputProperty(output->randr_output, property,
					     XA_INTEGER, 32, PropModeReplace, 1, &val,
					     FALSE, FALSE);

		if (err != 0) {
			xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
				   "RRChangeOutputProperty error, %d\n", err);
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

static const xf86OutputFuncsRec sna_output_funcs = {
	.create_resources = sna_output_create_resources,
#ifdef RANDR_12_INTERFACE
	.set_property = sna_output_set_property,
	.get_property = sna_output_get_property,
#endif
	.dpms = sna_output_dpms,
	.detect = sna_output_detect,
	.mode_valid = sna_output_mode_valid,

	.get_modes = sna_output_get_modes,
	.destroy = sna_output_destroy
};

static const int subpixel_conv_table[] = {
	SubPixelUnknown,
	SubPixelHorizontalRGB,
	SubPixelHorizontalBGR,
	SubPixelVerticalRGB,
	SubPixelVerticalBGR,
	SubPixelNone
};

static const char * const output_names[] = {
	/* DRM_MODE_CONNECTOR_Unknown */	"None",
	/* DRM_MODE_CONNECTOR_VGA */		"VGA",
	/* DRM_MODE_CONNECTOR_DVII */		"DVI",
	/* DRM_MODE_CONNECTOR_DVID */		"DVI",
	/* DRM_MODE_CONNECTOR_DVIA */		"DVI",
	/* DRM_MODE_CONNECTOR_Composite */	"Composite",
	/* DRM_MODE_CONNECTOR_SVIDEO */		"TV",
	/* DRM_MODE_CONNECTOR_LVDS */		"LVDS",
	/* DRM_MODE_CONNECTOR_Component */	"CTV",
	/* DRM_MODE_CONNECTOR_9PinDIN */	"DIN",
	/* DRM_MODE_CONNECTOR_DisplayPort */	"DP",
	/* DRM_MODE_CONNECTOR_HDMIA */		"HDMI",
	/* DRM_MODE_CONNECTOR_HDMIB */		"HDMI",
	/* DRM_MODE_CONNECTOR_TV */		"TV",
	/* DRM_MODE_CONNECTOR_eDP */		"eDP",
	/* DRM_MODE_CONNECTOR_VIRTUAL */	"Virtual",
	/* DRM_MODE_CONNECTOR_DSI */		"DSI"
};

static bool
sna_zaphod_match(const char *s, const char *output)
{
	char t[20];
	unsigned int i = 0;

	do {
		/* match any outputs in a comma list, stopping at whitespace */
		switch (*s) {
		case '\0':
			t[i] = '\0';
			return strcmp(t, output) == 0;

		case ',':
			t[i] ='\0';
			if (strcmp(t, output) == 0)
				return TRUE;
			i = 0;
			break;

		case ' ':
		case '\t':
		case '\n':
		case '\r':
			break;

		default:
			t[i++] = *s;
			break;
		}

		s++;
	} while (i < sizeof(t));

	return false;
}

static bool
output_ignored(ScrnInfoPtr scrn, const char *name)
{
	char monitor_name[64];
	const char *monitor;
	XF86ConfMonitorPtr conf;

	snprintf(monitor_name, sizeof(monitor_name), "monitor-%s", name);
	monitor = xf86findOptionValue(scrn->options, monitor_name);
	if (!monitor)
		monitor = name;

	conf = xf86findMonitor(monitor,
			       xf86configptr->conf_monitor_lst);
	if (conf == NULL && XF86_CRTC_CONFIG_PTR(scrn)->num_output == 0)
		conf = xf86findMonitor(scrn->monitor->id,
				       xf86configptr->conf_monitor_lst);
	if (conf == NULL)
		return false;

	return xf86CheckBoolOption(conf->mon_option_lst, "Ignore", 0);
}

static bool
sna_output_init(ScrnInfoPtr scrn, struct sna_mode *mode, int num)
{
	struct sna *sna = to_sna(scrn);
	xf86OutputPtr output;
	union compat_mode_get_connector compat_conn;
	struct drm_mode_get_encoder enc;
	struct drm_mode_modeinfo dummy;
	struct sna_output *sna_output;
	const char *output_name;
	char name[32];
	bool ret = false;
	int i;

	COMPILE_TIME_ASSERT(sizeof(struct drm_mode_get_connector) <= sizeof(compat_conn.pad));

	DBG(("%s(num=%d)\n", __FUNCTION__, num));

	VG_CLEAR(compat_conn);
	VG_CLEAR(enc);

	compat_conn.conn.connector_id = mode->kmode->connectors[num];
	compat_conn.conn.count_props = 0;
	compat_conn.conn.count_modes = 1; /* skip detect */
	compat_conn.conn.modes_ptr = (uintptr_t)&dummy;
	compat_conn.conn.count_encoders = 1;
	compat_conn.conn.encoders_ptr = (uintptr_t)&enc.encoder_id;

	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETCONNECTOR, &compat_conn.conn)) {
		DBG(("%s: GETCONNECTOR failed, ret=%d\n", __FUNCTION__, errno));
		return false;
	}

	if (compat_conn.conn.count_encoders != 1) {
		DBG(("%s: unexpected number [%d] of encoders attached\n",
		     __FUNCTION__, compat_conn.conn.count_encoders));
		return false;
	}

	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETENCODER, &enc)) {
		DBG(("%s: GETENCODER failed, ret=%d\n", __FUNCTION__, errno));
		return false;
	}

	sna_output = calloc(sizeof(struct sna_output), 1);
	if (!sna_output)
		return false;

	sna_output->num_props = compat_conn.conn.count_props;
	sna_output->prop_ids = malloc(sizeof(uint32_t)*compat_conn.conn.count_props);
	sna_output->prop_values = malloc(sizeof(uint64_t)*compat_conn.conn.count_props);
	sna_output->dpms_mode = DPMSModeOff;

	compat_conn.conn.count_encoders = 0;

	compat_conn.conn.count_modes = 1;
	compat_conn.conn.modes_ptr = (uintptr_t)&dummy;

	compat_conn.conn.count_props = sna_output->num_props;
	compat_conn.conn.props_ptr = (uintptr_t)sna_output->prop_ids;
	compat_conn.conn.prop_values_ptr = (uintptr_t)sna_output->prop_values;

	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETCONNECTOR, &compat_conn.conn)) {
		DBG(("%s: second! GETCONNECTOR failed, ret=%d\n", __FUNCTION__, errno));
		goto cleanup;
	}

	/* statically constructed property list */
	assert(sna_output->num_props == compat_conn.conn.count_props);
	VG(VALGRIND_MAKE_MEM_DEFINED(sna_output->prop_ids, sizeof(uint32_t)*sna_output->num_props));
	VG(VALGRIND_MAKE_MEM_DEFINED(sna_output->prop_values, sizeof(uint64_t)*sna_output->num_props));

	if (compat_conn.conn.connector_type < ARRAY_SIZE(output_names))
		output_name = output_names[compat_conn.conn.connector_type];
	else
		output_name = "UNKNOWN";
	snprintf(name, 32, "%s%d", output_name, compat_conn.conn.connector_type_id);

	if (xf86IsEntityShared(scrn->entityList[0])) {
		const char *str;

		str = xf86GetOptValString(sna->Options, OPTION_ZAPHOD);
		if (str && !sna_zaphod_match(str, name)) {
			DBG(("%s: zaphod mismatch, want %s, have %s\n", __FUNCTION__, str, name));
			ret = true;
			goto cleanup;
		}

		if ((enc.possible_crtcs & (1 << scrn->confScreen->device->screen)) == 0) {
			if (str) {
				xf86DrvMsg(scrn->scrnIndex, X_ERROR,
					   "%s is an invalid output for screen (pipe) %d\n",
					   name, scrn->confScreen->device->screen);
			}
			goto cleanup;
		}

		enc.possible_crtcs = 1;
		enc.possible_clones = 0;
	}

	output = xf86OutputCreate(scrn, &sna_output_funcs, name);
	if (!output) {
		/* xf86OutputCreate does not differentiate between
		 * a failure to allocate the output, and a user request
		 * to ignore the output. So reconstruct whether the user
		 * explicitly ignored the output.
		 */
		ret = output_ignored(scrn, name);
		DBG(("%s: create failed, ignored? %d\n", __FUNCTION__, ret));
		goto cleanup;
	}

	sna_output->id = compat_conn.conn.connector_id;
	sna_output->is_panel = is_panel(compat_conn.conn.connector_type);
	sna_output->edid_idx = find_property(sna, sna_output, "EDID");
	sna_output->dpms_id = find_property_id(sna, sna_output, "DPMS");

	output->mm_width = compat_conn.conn.mm_width;
	output->mm_height = compat_conn.conn.mm_height;

	if (compat_conn.conn.subpixel >= ARRAY_SIZE(subpixel_conv_table))
		compat_conn.conn.subpixel = 0;
	output->subpixel_order = subpixel_conv_table[compat_conn.conn.subpixel];
	output->driver_private = sna_output;

	for (i = 0; i < mode->kmode->count_encoders; i++) {
		if (enc.encoder_id == mode->kmode->encoders[i]) {
			sna_output->encoder_idx = i;
			break;
		}
	}

	if (sna_output->is_panel)
		sna_output_backlight_init(output);

	output->possible_crtcs = enc.possible_crtcs;
	output->possible_clones = enc.possible_clones;
	output->interlaceAllowed = TRUE;

	/* stash the active CRTC id for our probe function */
	output->crtc = NULL;
	if (compat_conn.conn.connection == DRM_MODE_CONNECTED)
		output->crtc = (void *)(uintptr_t)enc.crtc_id;

	DBG(("%s: created output '%s' %d [%ld]  (possible crtc:%x, possible clones:%x), edid=%d, dpms=%d, crtc=%lu\n",
	     __FUNCTION__, name, num, (long)sna_output->id,
	     (uint32_t)output->possible_crtcs,
	     (uint32_t)output->possible_clones,
	     sna_output->edid_idx, sna_output->dpms_id,
	     (unsigned long)(uintptr_t)output->crtc));

	return true;

cleanup:
	free(sna_output->prop_ids);
	free(sna_output->prop_values);
	free(sna_output);
	return ret;
}

/* We need to map from kms encoder based possible_clones mask to X output based
 * possible clones masking. Note that for SDVO and on Haswell with DP/HDMI we
 * can have more than one output hanging off the same encoder.
 */
static void
sna_mode_compute_possible_outputs(ScrnInfoPtr scrn)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	unsigned crtc_mask;
	int i, j;

	assert(config->num_output < 32);
	assert(config->num_crtc < 32);

	crtc_mask = (1 << config->num_crtc) - 1;

	/* Convert from encoder numbering to output numbering */
	for (i = 0; i < config->num_output; i++) {
		xf86OutputPtr output = config->output[i];
		unsigned mask = output->possible_clones;
		unsigned clones = 0;

		for (j = 0; j < config->num_output; j++) {
			if (mask & (1 << to_sna_output(config->output[j])->encoder_idx))
				clones |= 1 << j;
		}

		output->possible_clones = clones;
		output->possible_crtcs &= crtc_mask;

		DBG(("%s: updated output '%s' %d [%d] (possible crtc:%x, possible clones:%x)\n",
		     __FUNCTION__, output->name, i, to_connector_id(output),
		     (uint32_t)output->possible_crtcs,
		     (uint32_t)output->possible_clones));
	}
}

static void copy_front(struct sna *sna, PixmapPtr old, PixmapPtr new)
{
	struct sna_pixmap *old_priv, *new_priv;

	DBG(("%s\n", __FUNCTION__));

	if (wedged(sna))
		return;

	old_priv = sna_pixmap_force_to_gpu(old, MOVE_READ);
	if (!old_priv)
		return;

	new_priv = sna_pixmap_force_to_gpu(new, MOVE_WRITE);
	if (!new_priv)
		return;

	if (old_priv->clear) {
		(void)sna->render.fill_one(sna, new, new_priv->gpu_bo,
					   old_priv->clear_color,
					   0, 0,
					   new->drawable.width,
					   new->drawable.height,
					   GXcopy);
		new_priv->clear = true;
		new_priv->clear_color = old_priv->clear_color;
	} else {
		BoxRec box;
		int16_t sx, sy, dx, dy;

		if (new->drawable.width >= old->drawable.width &&
		    new->drawable.height >= old->drawable.height)
		{
			int nx = (new->drawable.width + old->drawable.width - 1) / old->drawable.width;
			int ny = (new->drawable.height + old->drawable.height - 1) / old->drawable.height;

			box.x1 = box.y1 = 0;

			dy = 0;
			for (sy = 0; sy < ny; sy++) {
				box.y2 = old->drawable.height;
				if (box.y2 + dy > new->drawable.height)
					box.y2 = new->drawable.height - dy;

				dx = 0;
				for (sx = 0; sx < nx; sx++) {
					box.x2 = old->drawable.width;
					if (box.x2 + dx > new->drawable.width)
						box.x2 = new->drawable.width - dx;

					(void)sna->render.copy_boxes(sna, GXcopy,
								     old, old_priv->gpu_bo, 0, 0,
								     new, new_priv->gpu_bo, dx, dy,
								     &box, 1, 0);
					dx += old->drawable.width;
				}
				dy += old->drawable.height;
			}
		} else {
			box.x1 = box.y1 = 0;
			box.x2 = min(old->drawable.width, new->drawable.width);
			box.y2 = min(old->drawable.height, new->drawable.height);

			sx = dx = 0;
			if (box.x2 < old->drawable.width)
				sx = (old->drawable.width - box.x2) / 2;
			if (box.x2 < new->drawable.width)
				dx = (new->drawable.width - box.x2) / 2;

			sy = dy = 0;
			if (box.y2 < old->drawable.height)
				sy = (old->drawable.height - box.y2) / 2;
			if (box.y2 < new->drawable.height)
				dy = (new->drawable.height - box.y2) / 2;

			DBG(("%s: copying box (%dx%d) from (%d, %d) to (%d, %d)\n",
			     __FUNCTION__, box.x2, box.y2, sx, sy, dx, dy));

			if (box.x2 != new->drawable.width || box.y2 != new->drawable.height) {
				(void)sna->render.fill_one(sna, new, new_priv->gpu_bo, 0,
							   0, 0,
							   new->drawable.width,
							   new->drawable.height,
							   GXclear);
			}
			(void)sna->render.copy_boxes(sna, GXcopy,
						     old, old_priv->gpu_bo, sx, sy,
						     new, new_priv->gpu_bo, dx, dy,
						     &box, 1, 0);
		}
	}

	if (!DAMAGE_IS_ALL(new_priv->gpu_damage))
		sna_damage_all(&new_priv->gpu_damage,
			       new->drawable.width,
			       new->drawable.height);
}

static Bool
sna_mode_resize(ScrnInfoPtr scrn, int width, int height)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	struct sna *sna = to_sna(scrn);
	ScreenPtr screen = scrn->pScreen;
	PixmapPtr new_front;
	int i;

	DBG(("%s (%d, %d) -> (%d, %d)\n", __FUNCTION__,
	     scrn->virtualX, scrn->virtualY,
	     width, height));
	assert((sna->flags & SNA_IS_HOSTED) == 0);

	if (scrn->virtualX == width && scrn->virtualY == height)
		return TRUE;

	assert(sna->front);
	assert(screen->GetScreenPixmap(screen) == sna->front);

	DBG(("%s: creating new framebuffer %dx%d\n",
	     __FUNCTION__, width, height));

	new_front = screen->CreatePixmap(screen,
					 width, height, scrn->depth,
					 SNA_CREATE_FB);
	if (!new_front)
		return FALSE;

	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "resizing framebuffer to %dx%d\n",
		   width, height);

	for (i = 0; i < config->num_crtc; i++) {
		struct sna_crtc *crtc;

		crtc = to_sna_crtc(config->crtc[i]);
		if (crtc == NULL)
			continue;

		sna_crtc_disable_shadow(sna, crtc);
	}
	assert(sna->mode.shadow_active == 0);
	assert(sna->mode.shadow_damage == NULL);
	assert(sna->mode.shadow == NULL);

	copy_front(sna, sna->front, new_front);

	screen->SetScreenPixmap(new_front);
	assert(screen->GetScreenPixmap(screen) == new_front);
	assert(sna->front == new_front);
	screen->DestroyPixmap(new_front); /* owned by screen now */

	scrn->virtualX = width;
	scrn->virtualY = height;
	scrn->displayWidth = width;

	/* Only update the CRTCs if we are in control */
	if (!scrn->vtSema)
		return TRUE;

	for (i = 0; i < config->num_crtc; i++) {
		xf86CrtcPtr crtc = config->crtc[i];

		if (!crtc->enabled || to_sna_crtc(crtc) == NULL)
			continue;

		if (!sna_crtc_set_mode_major(crtc,
					     &crtc->mode, crtc->rotation,
					     crtc->x, crtc->y))
			sna_crtc_disable(crtc);
	}

	while (sna_mode_has_pending_events(sna))
		sna_mode_wakeup(sna);

	kgem_clean_scanout_cache(&sna->kgem);

	return TRUE;
}

static int do_page_flip(struct sna *sna, struct kgem_bo *bo,
			void *data, int ref_crtc_hw_id)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	int width = sna->scrn->virtualX;
	int height = sna->scrn->virtualY;
	int count = 0;
	int i;

	/*
	 * Queue flips on all enabled CRTCs
	 * Note that if/when we get per-CRTC buffers, we'll have to update this.
	 * Right now it assumes a single shared fb across all CRTCs, with the
	 * kernel fixing up the offset of each CRTC as necessary.
	 *
	 * Also, flips queued on disabled or incorrectly configured displays
	 * may never complete; this is a configuration error.
	 */
	for (i = 0; i < config->num_crtc; i++) {
		struct sna_crtc *crtc = config->crtc[i]->driver_private;
		struct drm_mode_crtc_page_flip arg;

		DBG(("%s: crtc %d active? %d\n",
		     __FUNCTION__, i, crtc && crtc->bo));
		if (crtc == NULL || crtc->bo == NULL)
			continue;

		arg.crtc_id = crtc->id;
		arg.fb_id = get_fb(sna, bo, width, height);
		if (arg.fb_id == 0)
			goto disable;

		/* Only the reference crtc will finally deliver its page flip
		 * completion event. All other crtc's events will be discarded.
		 */
		arg.user_data = (uintptr_t)data;
		arg.user_data |= crtc->pipe == ref_crtc_hw_id;
		arg.flags = DRM_MODE_PAGE_FLIP_EVENT;
		arg.reserved = 0;

		DBG(("%s: crtc %d [ref? %d] --> fb %d\n",
		     __FUNCTION__, crtc->id,
		     crtc->pipe == ref_crtc_hw_id, arg.fb_id));
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_PAGE_FLIP, &arg)) {
			DBG(("%s: flip [fb=%d] on crtc %d [%d] failed - %d\n",
			     __FUNCTION__, arg.fb_id, i, crtc->id, errno));
disable:
			if (count == 0)
				return 0;

			xf86DrvMsg(sna->scrn->scrnIndex, X_ERROR,
				   "%s: page flipping failed, disabling CRTC:%d (pipe=%d)\n",
				   __FUNCTION__, crtc->id, crtc->pipe);
			sna_crtc_disable(config->crtc[i]);
			continue;
		}

		if (crtc->bo != bo) {
			kgem_bo_destroy(&sna->kgem, crtc->bo);
			crtc->bo = kgem_bo_reference(bo);
		}

		count++;
	}

	return count;
}

int
sna_page_flip(struct sna *sna,
	      struct kgem_bo *bo,
	      void *data,
	      int ref_crtc_hw_id)
{
	int count;

	DBG(("%s: handle %d attached\n", __FUNCTION__, bo->handle));
	assert(bo->refcnt);
	assert((sna->flags & SNA_IS_HOSTED) == 0);

	kgem_bo_submit(&sna->kgem, bo);

	/*
	 * Queue flips on all enabled CRTCs
	 * Note that if/when we get per-CRTC buffers, we'll have to update this.
	 * Right now it assumes a single shared fb across all CRTCs, with the
	 * kernel fixing up the offset of each CRTC as necessary.
	 *
	 * Also, flips queued on disabled or incorrectly configured displays
	 * may never complete; this is a configuration error.
	 */
	count = do_page_flip(sna, bo, data, ref_crtc_hw_id);
	DBG(("%s: page flipped %d crtcs\n", __FUNCTION__, count));

	return count;
}

static const xf86CrtcConfigFuncsRec sna_mode_funcs = {
	sna_mode_resize
};

static void set_size_range(struct sna *sna)
{
	/* We lie slightly as we expect no single monitor to exceed the
	 * crtc limits, so if the mode exceeds the scanout restrictions,
	 * we will quietly convert that to per-crtc pixmaps.
	 */
	xf86CrtcSetSizeRange(sna->scrn, 320, 200, INT16_MAX, INT16_MAX);
}

enum { /* XXX copied from hw/xfree86/modes/xf86Crtc.c */
	OPTION_PREFERRED_MODE,
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,14,99,1,0)
	OPTION_ZOOM_MODES,
#endif
	OPTION_POSITION,
	OPTION_BELOW,
	OPTION_RIGHT_OF,
	OPTION_ABOVE,
	OPTION_LEFT_OF,
	OPTION_ENABLE,
	OPTION_DISABLE,
	OPTION_MIN_CLOCK,
	OPTION_MAX_CLOCK,
	OPTION_IGNORE,
	OPTION_ROTATE,
	OPTION_PANNING,
	OPTION_PRIMARY,
	OPTION_DEFAULT_MODES,
};

#if HAS_GAMMA
static void set_gamma(uint16_t *curve, int size, double value)
{
	int i;

	value = 1/value;
	for (i = 0; i < size; i++)
		curve[i] = 256*(size-1)*pow(i/(double)(size-1), value);
}

static void output_set_gamma(xf86OutputPtr output, xf86CrtcPtr crtc)
{
	XF86ConfMonitorPtr mon = output->conf_monitor;

	if (!mon)
		return;

	DBG(("%s: red=%f\n", __FUNCTION__, mon->mon_gamma_red));
	if (mon->mon_gamma_red >= GAMMA_MIN &&
	    mon->mon_gamma_red <= GAMMA_MAX &&
	    mon->mon_gamma_red != 1.0)
		set_gamma(crtc->gamma_red, crtc->gamma_size,
			  mon->mon_gamma_red);

	DBG(("%s: green=%f\n", __FUNCTION__, mon->mon_gamma_green));
	if (mon->mon_gamma_green >= GAMMA_MIN &&
	    mon->mon_gamma_green <= GAMMA_MAX &&
	    mon->mon_gamma_green != 1.0)
		set_gamma(crtc->gamma_green, crtc->gamma_size,
			  mon->mon_gamma_green);

	DBG(("%s: blue=%f\n", __FUNCTION__, mon->mon_gamma_blue));
	if (mon->mon_gamma_blue >= GAMMA_MIN &&
	    mon->mon_gamma_blue <= GAMMA_MAX &&
	    mon->mon_gamma_blue != 1.0)
		set_gamma(crtc->gamma_blue, crtc->gamma_size,
			  mon->mon_gamma_blue);
}

static void crtc_init_gamma(xf86CrtcPtr crtc)
{
	uint16_t *gamma;

	/* Initialize the gamma ramps */
	gamma = NULL;
	if (crtc->gamma_size == 256)
		gamma = crtc->gamma_red;
	if (gamma == NULL)
		gamma = malloc(3 * 256 * sizeof(uint16_t));
	if (gamma) {
		struct sna *sna = to_sna(crtc->scrn);
		struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
		struct drm_mode_crtc_lut lut;
		bool gamma_set = false;

		lut.crtc_id = sna_crtc->id;
		lut.gamma_size = 256;
		lut.red = (uintptr_t)(gamma);
		lut.green = (uintptr_t)(gamma + 256);
		lut.blue = (uintptr_t)(gamma + 2 * 256);
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETGAMMA, &lut) == 0) {
			VG(VALGRIND_MAKE_MEM_DEFINED(gamma, 3*256*sizeof(gamma[0])));
			gamma_set =
				gamma[256 - 1] &&
				gamma[2*256 - 1] &&
				gamma[3*256 - 1];
		}

		DBG(("%s: CRTC:%d, pipe=%d: gamma set?=%d\n",
		     __FUNCTION__, sna_crtc->id, sna_crtc->pipe,
		     gamma_set));
		if (!gamma_set) {
			int i;

			for (i = 0; i < 256; i++) {
				gamma[i] = i << 8;
				gamma[256 + i] = i << 8;
				gamma[2*256 + i] = i << 8;
			}
		}

		if (gamma != crtc->gamma_red) {
			free(crtc->gamma_red);
			crtc->gamma_red = gamma;
			crtc->gamma_green = gamma + 256;
			crtc->gamma_blue = gamma + 2*256;
		}
	}
}
#else
static void output_set_gamma(xf86OutputPtr output, xf86CrtcPtr crtc) { }
static void crtc_init_gamma(xf86CrtcPtr crtc) { }
#endif

static const char *preferred_mode(xf86OutputPtr output)
{
	const char *mode;

	mode = xf86GetOptValString(output->options, OPTION_PREFERRED_MODE);
	if (mode)
		return mode;

	if (output->scrn->display->modes && *output->scrn->display->modes)
		return *output->scrn->display->modes;

	return NULL;
}

static bool sna_probe_initial_configuration(struct sna *sna)
{
	ScrnInfoPtr scrn = sna->scrn;
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	const int user_overrides[] = {
		OPTION_POSITION,
		OPTION_BELOW,
		OPTION_RIGHT_OF,
		OPTION_ABOVE,
		OPTION_LEFT_OF,
		OPTION_ROTATE,
		OPTION_PANNING,
	};
	int width, height;
	int i, j;

	assert((sna->flags & SNA_IS_HOSTED) == 0);

	if (xf86ReturnOptValBool(sna->Options, OPTION_REPROBE, FALSE)) {
		DBG(("%s: user requests reprobing\n", __FUNCTION__));
		return false;
	}

	/* First scan through all outputs and look for user overrides */
	for (i = 0; i < config->num_output; i++) {
		xf86OutputPtr output = config->output[i];

		for (j = 0; j < ARRAY_SIZE(user_overrides); j++) {
			if (xf86GetOptValString(output->options, user_overrides[j])) {
				DBG(("%s: user placement [%d] for %s\n",
				     __FUNCTION__,
				     user_overrides[j],
				     output->name));
				return false;
			}
		}
	}

	/* Copy the existing modes on each CRTCs */
	for (i = 0; i < config->num_crtc; i++) {
		xf86CrtcPtr crtc = config->crtc[i];
		struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
		struct drm_mode_crtc mode;

		if (sna_crtc == NULL)
			continue;

		crtc->enabled = FALSE;
		crtc->desiredMode.status = MODE_NOMODE;

		crtc_init_gamma(crtc);

		/* Retrieve the current mode */
		VG_CLEAR(mode);
		mode.crtc_id = sna_crtc->id;
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETCRTC, &mode))
			continue;

		DBG(("%s: CRTC:%d, pipe=%d: has mode?=%d\n", __FUNCTION__,
		     sna_crtc->id, sna_crtc->pipe,
		     mode.mode_valid && mode.mode.clock));

		if (!mode.mode_valid || mode.mode.clock == 0)
			continue;

		mode_from_kmode(scrn, &mode.mode, &crtc->desiredMode);
		crtc->desiredRotation = RR_Rotate_0;
		crtc->desiredX = mode.x;
		crtc->desiredY = mode.y;
		crtc->desiredTransformPresent = FALSE;
	}

	/* Reconstruct outputs pointing to active CRTC */
	for (i = 0; i < config->num_output; i++) {
		xf86OutputPtr output = config->output[i];
		uint32_t crtc_id;

		if (to_sna_output(output) == NULL) {
			assert(output->crtc == NULL);
			continue;
		}

		crtc_id = (uintptr_t)output->crtc;
		output->crtc = NULL;

		if (crtc_id == 0)
			continue;

		if (xf86ReturnOptValBool(output->options, OPTION_DISABLE, 0))
			continue;

		for (j = 0; j < config->num_crtc; j++) {
			xf86CrtcPtr crtc = config->crtc[j];

			if (to_sna_crtc(crtc) == NULL ||
			    to_sna_crtc(crtc)->id != crtc_id)
				continue;

			if (crtc->desiredMode.status == MODE_OK) {
				DisplayModePtr M;
				const char *pref;

				pref = preferred_mode(output);
				if (pref && strcmp(pref, crtc->desiredMode.name))
					return false;

				xf86DrvMsg(scrn->scrnIndex, X_PROBED,
						"Output %s using initial mode %s on pipe %d\n",
						output->name,
						crtc->desiredMode.name,
						to_sna_crtc(crtc)->pipe);

				output->crtc = crtc;
				crtc->enabled = TRUE;

				if (output->mm_width == 0 || output->mm_height == 0) {
					output->mm_height = (crtc->desiredMode.VDisplay * 254) / (10*DEFAULT_DPI);
					output->mm_width = (crtc->desiredMode.HDisplay * 254) / (10*DEFAULT_DPI);
				}

				output_set_gamma(output, crtc);

				M = calloc(1, sizeof(DisplayModeRec));
				if (M) {
					*M = crtc->desiredMode;
					M->name = strdup(M->name);
					output->probed_modes =
						xf86ModesAdd(output->probed_modes, M);
				}
			}

			break;
		}

		if (j == config->num_crtc) {
			/* Can not find the earlier associated CRTC, bail */
			DBG(("%s: existing setup conflicts with output assignment (Zaphod), reprobing\n",
			     __FUNCTION__));
			return false;
		}
	}

	width = height = 0;
	for (i = 0; i < config->num_crtc; i++) {
		xf86CrtcPtr crtc = config->crtc[i];
		int w, h;

		if (!crtc->enabled)
			continue;

		w = crtc->desiredX + crtc->desiredMode.HDisplay;
		if (w > width)
			width = w;
		h = crtc->desiredY + crtc->desiredMode.VDisplay;
		if (h > height)
			height = h;
	}

	if (!width || !height) {
		width = 1024;
		height = 768;
	}

	scrn->display->frameX0 = 0;
	scrn->display->frameY0 = 0;
	scrn->display->virtualX = width;
	scrn->display->virtualY = height;

	scrn->virtualX = width;
	scrn->virtualY = height;

	xf86SetScrnInfoModes(sna->scrn);
	DBG(("%s: SetScrnInfoModes = %p\n", __FUNCTION__, scrn->modes));
	return scrn->modes != NULL;
}

static void
sanitize_outputs(struct sna *sna)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	int i;

	for (i = 0; i < config->num_output; i++)
		config->output[i]->crtc = NULL;
}

static void
sna_crtc_config_notify(ScreenPtr screen)
{
	DBG(("%s\n", __FUNCTION__));
	sna_mode_update(to_sna_from_screen(screen));
}

#if HAS_PIXMAP_SHARING
#define sna_setup_provider(scrn) xf86ProviderSetup(scrn, NULL, "Intel")
#else
#define sna_setup_provider(scrn)
#endif

bool sna_mode_pre_init(ScrnInfoPtr scrn, struct sna *sna)
{
	struct sna_mode *mode = &sna->mode;
	int num_fake = 0;
	int i;

	if (sna->flags & SNA_IS_HOSTED) {
		sna_setup_provider(scrn);
		return true;
	}

	if (!xf86GetOptValInteger(sna->Options, OPTION_VIRTUAL, &num_fake))
		num_fake = 1;

	mode->kmode = drmModeGetResources(sna->kgem.fd);
	if (mode->kmode) {
		xf86CrtcConfigInit(scrn, &sna_mode_funcs);
		XF86_CRTC_CONFIG_PTR(scrn)->xf86_crtc_notify = sna_crtc_config_notify;

		for (i = 0; i < mode->kmode->count_crtcs; i++)
			if (!sna_crtc_init(scrn, mode, i))
				return false;

		for (i = 0; i < mode->kmode->count_connectors; i++)
			if (!sna_output_init(scrn, mode, i))
				return false;

		if (!xf86IsEntityShared(scrn->entityList[0]))
			sna_mode_compute_possible_outputs(scrn);
	} else {
		if (num_fake == 0)
			num_fake = 1;
	}

	set_size_range(sna);

	if (!sna_mode_fake_init(sna, num_fake))
		return false;

	if (!sna_probe_initial_configuration(sna)) {
		sanitize_outputs(sna);
		xf86InitialConfiguration(scrn, TRUE);
	}

	sna_setup_provider(scrn);
	return scrn->modes != NULL;
}

void
sna_mode_close(struct sna *sna)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	int i;

	/* In order to workaround a kernel bug in not honouring O_NONBLOCK,
	 * check that the fd is readable before attempting to read the next
	 * event from drm.
	 */
	while (sna_mode_has_pending_events(sna))
		sna_mode_wakeup(sna);

	if (sna->flags & SNA_IS_HOSTED)
		return;

	for (i = 0; i < config->num_crtc; i++) {
		struct sna_crtc *crtc;

		crtc = to_sna_crtc(config->crtc[i]);
		if (crtc == NULL)
			continue;

		sna_crtc_disable_shadow(sna, crtc);
	}
}

void
sna_mode_fini(struct sna *sna)
{
}

static bool sna_box_intersect(BoxPtr r, const BoxRec *a, const BoxRec *b)
{
	r->x1 = a->x1 > b->x1 ? a->x1 : b->x1;
	r->x2 = a->x2 < b->x2 ? a->x2 : b->x2;
	if (r->x1 >= r->x2)
		return false;

	r->y1 = a->y1 > b->y1 ? a->y1 : b->y1;
	r->y2 = a->y2 < b->y2 ? a->y2 : b->y2;
	DBG(("%s: (%d, %d), (%d, %d) intersect (%d, %d), (%d, %d) = (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     a->x1, a->y1, a->x2, a->y2,
	     b->x1, b->y1, b->x2, b->y2,
	     r->x1, r->y1, r->x2, r->y2));
	if (r->y1 >= r->y2)
		return false;

	return true;
}

static int sna_box_area(const BoxRec *box)
{
	return (int)(box->x2 - box->x1) * (int)(box->y2 - box->y1);
}

/*
 * Return the crtc covering 'box'. If two crtcs cover a portion of
 * 'box', then prefer 'desired'. If 'desired' is NULL, then prefer the crtc
 * with greater coverage
 */
xf86CrtcPtr
sna_covering_crtc(struct sna *sna, const BoxRec *box, xf86CrtcPtr desired)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	xf86CrtcPtr best_crtc;
	int best_coverage, c;

	if (sna->flags & SNA_IS_HOSTED)
		return NULL;

	/* If we do not own the VT, we do not own the CRTC either */
	if (!sna->scrn->vtSema)
		return NULL;

	DBG(("%s for box=(%d, %d), (%d, %d)\n",
	     __FUNCTION__, box->x1, box->y1, box->x2, box->y2));

	best_crtc = NULL;
	best_coverage = 0;
	for (c = 0; c < config->num_crtc; c++) {
		xf86CrtcPtr crtc = config->crtc[c];
		BoxRec cover_box;
		int coverage;

		if (to_sna_crtc(crtc) == NULL)
			continue;

		/* If the CRTC is off, treat it as not covering */
		if (to_sna_crtc(crtc)->bo == NULL) {
			DBG(("%s: crtc %d off, skipping\n", __FUNCTION__, c));
			continue;
		}

		DBG(("%s: crtc %d: (%d, %d), (%d, %d)\n",
		     __FUNCTION__, c,
		     crtc->bounds.x1, crtc->bounds.y1,
		     crtc->bounds.x2, crtc->bounds.y2));
		if (*(const uint64_t *)box == *(uint64_t *)&crtc->bounds) {
			DBG(("%s: box exactly matches crtc [%d]\n",
			     __FUNCTION__, c));
			return crtc;
		}

		if (!sna_box_intersect(&cover_box, &crtc->bounds, box))
			continue;

		DBG(("%s: box instersects (%d, %d), (%d, %d) of crtc %d\n",
		     __FUNCTION__,
		     cover_box.x1, cover_box.y1,
		     cover_box.x2, cover_box.y2,
		     c));
		if (crtc == desired) {
			DBG(("%s: box is on desired crtc [%p]\n",
			     __FUNCTION__, crtc));
			return crtc;
		}

		coverage = sna_box_area(&cover_box);
		DBG(("%s: box covers %d of crtc %d\n",
		     __FUNCTION__, coverage, c));
		if (coverage > best_coverage) {
			best_crtc = crtc;
			best_coverage = coverage;
		}
	}
	DBG(("%s: best crtc = %p, coverage = %d\n",
	     __FUNCTION__, best_crtc, best_coverage));
	return best_crtc;
}

#define MI_LOAD_REGISTER_IMM			(0x22<<23)

static bool sna_emit_wait_for_scanline_hsw(struct sna *sna,
					   xf86CrtcPtr crtc,
					   int pipe, int y1, int y2,
					   bool full_height)
{
	uint32_t event;
	uint32_t *b;

	if (!sna->kgem.has_secure_batches)
		return false;

	b = kgem_get_batch(&sna->kgem);
	sna->kgem.nbatch += 17;

	switch (pipe) {
	default: assert(0);
	case 0: event = 1 << 0; break;
	case 1: event = 1 << 8; break;
	case 2: event = 1 << 14; break;
	}

	b[0] = MI_LOAD_REGISTER_IMM | 1;
	b[1] = 0x44050; /* DERRMR */
	b[2] = ~event;
	b[3] = MI_LOAD_REGISTER_IMM | 1;
	b[4] = 0xa188; /* FORCEWAKE_MT */
	b[5] = 2 << 16 | 2;

	/* The documentation says that the LOAD_SCAN_LINES command
	 * always comes in pairs. Don't ask me why. */
	switch (pipe) {
	default: assert(0);
	case 0: event = 0 << 19; break;
	case 1: event = 1 << 19; break;
	case 2: event = 4 << 19; break;
	}
	b[8] = b[6] = MI_LOAD_SCAN_LINES_INCL | event;
	b[9] = b[7] = (y1 << 16) | (y2-1);

	switch (pipe) {
	default: assert(0);
	case 0: event = 1 << 0; break;
	case 1: event = 1 << 8; break;
	case 2: event = 1 << 14; break;
	}
	b[10] = MI_WAIT_FOR_EVENT | event;

	b[11] = MI_LOAD_REGISTER_IMM | 1;
	b[12] = 0xa188; /* FORCEWAKE_MT */
	b[13] = 2 << 16;
	b[14] = MI_LOAD_REGISTER_IMM | 1;
	b[15] = 0x44050; /* DERRMR */
	b[16] = ~0;

	sna->kgem.batch_flags |= I915_EXEC_SECURE;
	return true;
}

static bool sna_emit_wait_for_scanline_vlv(struct sna *sna,
					   xf86CrtcPtr crtc,
					   int pipe, int y1, int y2,
					   bool full_height)
{
	uint32_t display_base = 0x180000;
	uint32_t event;
	uint32_t *b;

	return false; /* synchronisation? I've heard of that */

	if (!sna->kgem.has_secure_batches)
		return false;

	assert(y1 >= 0);
	assert(y2 > y1);
	assert(sna->kgem.mode);

	/* Always program one less than the desired value */
	if (--y1 < 0)
		y1 = crtc->bounds.y2;
	y2--;

	b = kgem_get_batch(&sna->kgem);
	sna->kgem.nbatch += 4;

	if (pipe == 0) {
		if (full_height)
			event = MI_WAIT_FOR_PIPEA_SVBLANK;
		else
			event = MI_WAIT_FOR_PIPEA_SCAN_LINE_WINDOW;
	} else {
		if (full_height)
			event = MI_WAIT_FOR_PIPEB_SVBLANK;
		else
			event = MI_WAIT_FOR_PIPEB_SCAN_LINE_WINDOW;
	}
	b[0] = MI_LOAD_REGISTER_IMM | 1;
	b[1] = display_base + 0x70004 + 0x1000 * pipe;
	b[2] = (1 << 31) | (y1 << 16) | y2;
	b[3] = MI_WAIT_FOR_EVENT | event;

	sna->kgem.batch_flags |= I915_EXEC_SECURE;
	return true;
}

static bool sna_emit_wait_for_scanline_ivb(struct sna *sna,
					   xf86CrtcPtr crtc,
					   int pipe, int y1, int y2,
					   bool full_height)
{
	uint32_t event, *b;

	if (!sna->kgem.has_secure_batches)
		return false;

	assert(y1 >= 0);
	assert(y2 > y1);
	assert(sna->kgem.mode);

	/* Always program one less than the desired value */
	if (--y1 < 0)
		y1 = crtc->bounds.y2;
	y2--;

	switch (pipe) {
	default:
		assert(0);
	case 0:
		event = 1 << (full_height ? 3 : 0);
		break;
	case 1:
		event = 1 << (full_height ? 11 : 8);
		break;
	case 2:
		event = 1 << (full_height ? 21 : 14);
		break;
	}

	b = kgem_get_batch(&sna->kgem);

	/* Both the LRI and WAIT_FOR_EVENT must be in the same cacheline */
	if (((sna->kgem.nbatch + 6) >> 4) != (sna->kgem.nbatch + 10) >> 4) {
		int dw = sna->kgem.nbatch + 6;
		dw = ALIGN(dw, 16) - dw;
		while (dw--)
			*b++ = MI_NOOP;
	}

	b[0] = MI_LOAD_REGISTER_IMM | 1;
	b[1] = 0x44050; /* DERRMR */
	b[2] = ~event;
	b[3] = MI_LOAD_REGISTER_IMM | 1;
	b[4] = 0xa188; /* FORCEWAKE_MT */
	b[5] = 2 << 16 | 2;
	b[6] = MI_LOAD_REGISTER_IMM | 1;
	b[7] = 0x70068 + 0x1000 * pipe;
	b[8] = (1 << 31) | (1 << 30) | (y1 << 16) | y2;
	b[9] = MI_WAIT_FOR_EVENT | event;
	b[10] = MI_LOAD_REGISTER_IMM | 1;
	b[11] = 0xa188; /* FORCEWAKE_MT */
	b[12] = 2 << 16;
	b[13] = MI_LOAD_REGISTER_IMM | 1;
	b[14] = 0x44050; /* DERRMR */
	b[15] = ~0;

	sna->kgem.nbatch = b - sna->kgem.batch + 16;

	sna->kgem.batch_flags |= I915_EXEC_SECURE;
	return true;
}

static bool sna_emit_wait_for_scanline_gen6(struct sna *sna,
					    xf86CrtcPtr crtc,
					    int pipe, int y1, int y2,
					    bool full_height)
{
	uint32_t *b;
	uint32_t event;

	if (!sna->kgem.has_secure_batches)
		return false;

	assert(y1 >= 0);
	assert(y2 > y1);
	assert(sna->kgem.mode == KGEM_RENDER);

	/* Always program one less than the desired value */
	if (--y1 < 0)
		y1 = crtc->bounds.y2;
	y2--;

	/* The scanline granularity is 3 bits */
	y1 &= ~7;
	y2 &= ~7;
	if (y2 == y1)
		return false;

	event = 1 << (3*full_height + pipe*8);

	b = kgem_get_batch(&sna->kgem);
	sna->kgem.nbatch += 10;

	b[0] = MI_LOAD_REGISTER_IMM | 1;
	b[1] = 0x44050; /* DERRMR */
	b[2] = ~event;
	b[3] = MI_LOAD_REGISTER_IMM | 1;
	b[4] = 0x4f100; /* magic */
	b[5] = (1 << 31) | (1 << 30) | pipe << 29 | (y1 << 16) | y2;
	b[6] = MI_WAIT_FOR_EVENT | event;
	b[7] = MI_LOAD_REGISTER_IMM | 1;
	b[8] = 0x44050; /* DERRMR */
	b[9] = ~0;

	sna->kgem.batch_flags |= I915_EXEC_SECURE;
	return true;
}

static bool sna_emit_wait_for_scanline_gen4(struct sna *sna,
					    xf86CrtcPtr crtc,
					    int pipe, int y1, int y2,
					    bool full_height)
{
	uint32_t event;
	uint32_t *b;

	if (pipe == 0) {
		if (full_height)
			event = MI_WAIT_FOR_PIPEA_SVBLANK;
		else
			event = MI_WAIT_FOR_PIPEA_SCAN_LINE_WINDOW;
	} else {
		if (full_height)
			event = MI_WAIT_FOR_PIPEB_SVBLANK;
		else
			event = MI_WAIT_FOR_PIPEB_SCAN_LINE_WINDOW;
	}

	b = kgem_get_batch(&sna->kgem);
	sna->kgem.nbatch += 5;

	/* The documentation says that the LOAD_SCAN_LINES command
	 * always comes in pairs. Don't ask me why. */
	b[2] = b[0] = MI_LOAD_SCAN_LINES_INCL | pipe << 20;
	b[3] = b[1] = (y1 << 16) | (y2-1);
	b[4] = MI_WAIT_FOR_EVENT | event;

	return true;
}

static bool sna_emit_wait_for_scanline_gen2(struct sna *sna,
					    xf86CrtcPtr crtc,
					    int pipe, int y1, int y2,
					    bool full_height)
{
	uint32_t *b;

	/*
	 * Pre-965 doesn't have SVBLANK, so we need a bit
	 * of extra time for the blitter to start up and
	 * do its job for a full height blit
	 */
	if (full_height)
		y2 -= 2;

	b = kgem_get_batch(&sna->kgem);
	sna->kgem.nbatch += 5;

	/* The documentation says that the LOAD_SCAN_LINES command
	 * always comes in pairs. Don't ask me why. */
	b[2] = b[0] = MI_LOAD_SCAN_LINES_INCL | pipe << 20;
	b[3] = b[1] = (y1 << 16) | (y2-1);
	b[4] = MI_WAIT_FOR_EVENT | 1 << (1 + 4*pipe);

	return true;
}

bool
sna_wait_for_scanline(struct sna *sna,
		      PixmapPtr pixmap,
		      xf86CrtcPtr crtc,
		      const BoxRec *clip)
{
	bool full_height;
	int y1, y2, pipe;
	bool ret;

	assert(crtc != NULL);
	assert(to_sna_crtc(crtc) != NULL);
	assert(to_sna_crtc(crtc)->bo != NULL);
	assert(pixmap == sna->front);

	if (sna->flags & SNA_NO_VSYNC)
		return false;

	/*
	 * Make sure we don't wait for a scanline that will
	 * never occur
	 */
	y1 = clip->y1 - crtc->bounds.y1;
	if (y1 < 0)
		y1 = 0;
	y2 = clip->y2 - crtc->bounds.y1;
	if (y2 > crtc->bounds.y2 - crtc->bounds.y1)
		y2 = crtc->bounds.y2 - crtc->bounds.y1;
	DBG(("%s: clipped range = %d, %d\n", __FUNCTION__, y1, y2));
	if (y2 <= y1 + 4)
		return false;

	full_height = y1 == 0 && y2 == crtc->bounds.y2 - crtc->bounds.y1;

	if (crtc->mode.Flags & V_INTERLACE) {
		/* DSL count field lines */
		y1 /= 2;
		y2 /= 2;
	}

	pipe = sna_crtc_to_pipe(crtc);
	DBG(("%s: pipe=%d, y1=%d, y2=%d, full_height?=%d\n",
	     __FUNCTION__, pipe, y1, y2, full_height));

	if (sna->kgem.gen >= 0110)
		ret = false;
	else if (sna->kgem.gen >= 075)
		ret = sna_emit_wait_for_scanline_hsw(sna, crtc, pipe, y1, y2, full_height);
	else if (sna->kgem.gen == 071)
		ret = sna_emit_wait_for_scanline_vlv(sna, crtc, pipe, y1, y2, full_height);
	else if (sna->kgem.gen >= 070)
		ret = sna_emit_wait_for_scanline_ivb(sna, crtc, pipe, y1, y2, full_height);
	else if (sna->kgem.gen >= 060)
		ret =sna_emit_wait_for_scanline_gen6(sna, crtc, pipe, y1, y2, full_height);
	else if (sna->kgem.gen >= 040)
		ret = sna_emit_wait_for_scanline_gen4(sna, crtc, pipe, y1, y2, full_height);
	else
		ret = sna_emit_wait_for_scanline_gen2(sna, crtc, pipe, y1, y2, full_height);

	return ret;
}

void sna_mode_update(struct sna *sna)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	int i;

	if (sna->flags & SNA_IS_HOSTED)
		return;

	/* Validate CRTC attachments and force consistency upon the kernel */
	sna->mode.front_active = 0;
	for (i = 0; i < config->num_crtc; i++) {
		xf86CrtcPtr crtc = config->crtc[i];
		struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
		struct drm_mode_crtc mode;
		uint32_t expected;

		if (sna_crtc == NULL)
			continue;

#if XF86_CRTC_VERSION >= 3
		assert(sna_crtc->bo == NULL || crtc->active);
#endif
		expected = sna_crtc->bo ? fb_id(sna_crtc->bo) : 0;

		VG_CLEAR(mode);
		mode.crtc_id = sna_crtc->id;
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETCRTC, &mode))
			continue;

		DBG(("%s: crtc=%d, valid?=%d, fb attached?=%d, expected=%d\n",
		     __FUNCTION__,
		     mode.crtc_id, mode.mode_valid,
		     mode.fb_id, expected));

		if (mode.fb_id != expected)
			sna_crtc_disable(crtc);

		if (sna->front && sna_crtc->bo == __sna_pixmap_get_bo(sna->front))
			sna->mode.front_active++;
	}

	for (i = 0; i < config->num_output; i++) {
		xf86OutputPtr output = config->output[i];
		struct sna_output *sna_output;

		if (output->crtc)
			continue;

		sna_output = to_sna_output(output);
		if (sna_output == NULL)
			continue;

		sna_output->dpms_mode = DPMSModeOff;
	}

	update_flush_interval(sna);
}

void sna_mode_reset(struct sna *sna)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	int i;

	if (sna->flags & SNA_IS_HOSTED)
		return;

	for (i = 0; i < config->num_crtc; i++) {
		struct sna_crtc *sna_crtc = to_sna_crtc(config->crtc[i]);
		if (sna_crtc == NULL)
			continue;

		sna_crtc->dpms_mode = DPMSModeOff;
	}

	for (i = 0; i < config->num_output; i++) {
		struct sna_output *sna_output = to_sna_output(config->output[i]);
		if (sna_output == NULL)
			continue;

		sna_output->dpms_mode = DPMSModeOff;
	}
}

static void transformed_box(BoxRec *box, xf86CrtcPtr crtc)
{
	box->x1 -= crtc->filter_width >> 1;
	box->x2 += crtc->filter_width >> 1;
	box->y1 -= crtc->filter_height >> 1;
	box->y2 += crtc->filter_height >> 1;

	pixman_f_transform_bounds(&crtc->f_framebuffer_to_crtc, box);

	if (box->x1 < 0)
		box->x1 = 0;
	if (box->y1 < 0)
		box->y1 = 0;
	if (box->x2 > crtc->mode.HDisplay)
		box->x2 = crtc->mode.HDisplay;
	if (box->y2 > crtc->mode.VDisplay)
		box->y2 = crtc->mode.VDisplay;
}

static void
sna_crtc_redisplay__fallback(xf86CrtcPtr crtc, RegionPtr region, struct kgem_bo *bo)
{
	struct sna *sna = to_sna(crtc->scrn);
	ScreenPtr screen = sna->scrn->pScreen;
	PictFormatPtr format;
	PicturePtr src, dst;
	PixmapPtr pixmap;
	int error;
	void *ptr;

	DBG(("%s: compositing transformed damage boxes\n", __FUNCTION__));

	ptr = kgem_bo_map__gtt(&sna->kgem, bo);
	if (ptr == NULL)
		return;

	pixmap = sna_pixmap_create_unattached(screen,
					      0, 0, sna->front->drawable.depth);
	if (pixmap == NullPixmap)
		return;

	if (!screen->ModifyPixmapHeader(pixmap,
					crtc->mode.HDisplay,
					crtc->mode.VDisplay,
					sna->front->drawable.depth,
					sna->front->drawable.bitsPerPixel,
					bo->pitch, ptr))
		goto free_pixmap;

	error = sna_render_format_for_depth(sna->front->drawable.depth);
	format = PictureMatchFormat(screen,
				    PIXMAN_FORMAT_DEPTH(error), error);
	if (format == NULL) {
		DBG(("%s: can't find format for depth=%d [%08x]\n",
		     __FUNCTION__, sna->front->drawable.depth,
		     (int)sna_render_format_for_depth(sna->front->drawable.depth)));
		goto free_pixmap;
	}

	src = CreatePicture(None, &sna->front->drawable, format,
			    0, NULL, serverClient, &error);
	if (!src)
		goto free_pixmap;

	error = SetPictureTransform(src, &crtc->crtc_to_framebuffer);
	if (error)
		goto free_src;

	if (crtc->filter)
		SetPicturePictFilter(src, crtc->filter,
				     crtc->params, crtc->nparams);

	dst = CreatePicture(None, &pixmap->drawable, format,
			    0, NULL, serverClient, &error);
	if (!dst)
		goto free_src;

	kgem_bo_sync__gtt(&sna->kgem, bo);

	if (sigtrap_get() == 0) { /* paranoia */
		const BoxRec *b = REGION_RECTS(region);
		int n = REGION_NUM_RECTS(region);
		do {
			BoxRec box;

			box = *b++;
			transformed_box(&box, crtc);

			DBG(("%s: (%d, %d)x(%d, %d) -> (%d, %d), (%d, %d)\n",
			     __FUNCTION__,
			     b[-1].x1, b[-1].y1, b[-1].x2-b[-1].x1, b[-1].y2-b[-1].y1,
			     box.x1, box.y1, box.x2, box.y2));

			fbComposite(PictOpSrc, src, NULL, dst,
				    box.x1, box.y1,
				    0, 0,
				    box.x1, box.y1,
				    box.x2 - box.x1, box.y2 - box.y1);
		} while (--n);
		sigtrap_put();
	}

	FreePicture(dst, None);
free_src:
	FreePicture(src, None);
free_pixmap:
	screen->DestroyPixmap(pixmap);
}

static void
sna_crtc_redisplay__composite(xf86CrtcPtr crtc, RegionPtr region, struct kgem_bo *bo)
{
	struct sna *sna = to_sna(crtc->scrn);
	ScreenPtr screen = crtc->scrn->pScreen;
	struct sna_composite_op tmp;
	PictFormatPtr format;
	PicturePtr src, dst;
	PixmapPtr pixmap;
	BoxPtr b;
	int n, error;

	DBG(("%s: compositing transformed damage boxes\n", __FUNCTION__));

	pixmap = sna_pixmap_create_unattached(screen,
					      0, 0, sna->front->drawable.depth);
	if (pixmap == NullPixmap)
		return;

	if (!screen->ModifyPixmapHeader(pixmap,
					crtc->mode.HDisplay,
					crtc->mode.VDisplay,
					sna->front->drawable.depth,
					sna->front->drawable.bitsPerPixel,
					bo->pitch, NULL))
		goto free_pixmap;

	if (!sna_pixmap_attach_to_bo(pixmap, bo))
		goto free_pixmap;

	error = sna_render_format_for_depth(sna->front->drawable.depth);
	format = PictureMatchFormat(screen,
				    PIXMAN_FORMAT_DEPTH(error), error);
	if (format == NULL) {
		DBG(("%s: can't find format for depth=%d [%08x]\n",
		     __FUNCTION__, sna->front->drawable.depth,
		     (int)sna_render_format_for_depth(sna->front->drawable.depth)));
		goto free_pixmap;
	}

	src = CreatePicture(None, &sna->front->drawable, format,
			    0, NULL, serverClient, &error);
	if (!src)
		goto free_pixmap;

	error = SetPictureTransform(src, &crtc->crtc_to_framebuffer);
	if (error)
		goto free_src;

	if (crtc->filter)
		SetPicturePictFilter(src, crtc->filter,
				     crtc->params, crtc->nparams);

	dst = CreatePicture(None, &pixmap->drawable, format,
			    0, NULL, serverClient, &error);
	if (!dst)
		goto free_src;

	ValidatePicture(src);
	ValidatePicture(dst);

	if (!sna->render.composite(sna,
				   PictOpSrc, src, NULL, dst,
				   0, 0,
				   0, 0,
				   0, 0,
				   crtc->mode.HDisplay, crtc->mode.VDisplay,
				   0, memset(&tmp, 0, sizeof(tmp)))) {
		DBG(("%s: unsupported operation!\n", __FUNCTION__));
		sna_crtc_redisplay__fallback(crtc, region, bo);
		goto free_dst;
	}

	n = REGION_NUM_RECTS(region);
	b = REGION_RECTS(region);
	do {
		BoxRec box;

		box = *b++;
		transformed_box(&box, crtc);

		DBG(("%s: (%d, %d)x(%d, %d) -> (%d, %d), (%d, %d)\n",
		     __FUNCTION__,
		     b[-1].x1, b[-1].y1, b[-1].x2-b[-1].x1, b[-1].y2-b[-1].y1,
		     box.x1, box.y1, box.x2, box.y2));

		tmp.box(sna, &tmp, &box);
	} while (--n);
	tmp.done(sna, &tmp);

free_dst:
	FreePicture(dst, None);
free_src:
	FreePicture(src, None);
free_pixmap:
	screen->DestroyPixmap(pixmap);
}

static void
sna_crtc_redisplay(xf86CrtcPtr crtc, RegionPtr region)
{
	struct sna *sna = to_sna(crtc->scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	struct sna_pixmap *priv = sna_pixmap(sna->front);
	int16_t tx, ty;

	DBG(("%s: crtc %d [pipe=%d], damage (%d, %d), (%d, %d) x %ld\n",
	     __FUNCTION__, sna_crtc->id, sna_crtc->pipe,
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2,
	     (long)RegionNumRects(region)));

	assert(!wedged(sna));

	if (priv->clear) {
		DBG(("%s: clear damage boxes\n", __FUNCTION__));

		RegionTranslate(region, -crtc->bounds.x1, -crtc->bounds.y1);
		sna_blt_fill_boxes(sna, GXcopy,
				   sna_crtc->bo, sna->front->drawable.bitsPerPixel,
				   priv->clear_color,
				   REGION_RECTS(region), REGION_NUM_RECTS(region));
		return;
	}

	if (crtc->filter == NULL &&
	    sna_transform_is_integer_translation(&crtc->crtc_to_framebuffer,
						 &tx, &ty)) {
		PixmapRec tmp;

		DBG(("%s: copy damage boxes\n", __FUNCTION__));

		tmp.drawable.width = crtc->mode.HDisplay;
		tmp.drawable.height = crtc->mode.VDisplay;
		tmp.drawable.depth = sna->front->drawable.depth;
		tmp.drawable.bitsPerPixel = sna->front->drawable.bitsPerPixel;

		if (sna->render.copy_boxes(sna, GXcopy,
					   sna->front, priv->gpu_bo, 0, 0,
					   &tmp, sna_crtc->bo, -tx, -ty,
					   REGION_RECTS(region), REGION_NUM_RECTS(region), 0))
			return;
	}

	if (can_render(sna))
		sna_crtc_redisplay__composite(crtc, region, sna_crtc->bo);
	else
		sna_crtc_redisplay__fallback(crtc, region, sna_crtc->bo);
}

static void set_bo(PixmapPtr pixmap, struct kgem_bo *bo, RegionPtr region)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	struct wait_for_shadow *wait;

	assert((priv->pinned & PIN_PRIME) == 0);
	assert(bo != priv->gpu_bo);
	assert(priv->gpu_bo);

	assert(priv->move_to_gpu == NULL);
	wait = malloc(sizeof(*wait));
	if (wait != NULL) {
		wait->bo = kgem_bo_reference(bo);
		RegionNull(&wait->region);
		RegionCopy(&wait->region, region);

		priv->move_to_gpu = wait_for_shadow;
		priv->move_to_gpu_data = wait;
	}

	priv->pinned |= PIN_SCANOUT;
}

void sna_mode_redisplay(struct sna *sna)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	RegionPtr region;
	int i;

	if (!sna->mode.shadow_damage)
		return;

	DBG(("%s: posting shadow damage? %d (flips pending? %d)\n",
	     __FUNCTION__,
	     !RegionNil(DamageRegion(sna->mode.shadow_damage)),
	     sna->mode.shadow_flip));
	assert((sna->flags & SNA_IS_HOSTED) == 0);
	assert(sna->mode.shadow_active);

	region = DamageRegion(sna->mode.shadow_damage);
	if (RegionNil(region))
		return;

	DBG(("%s: damage: %ldx(%d, %d), (%d, %d)\n",
	     __FUNCTION__, (long)REGION_NUM_RECTS(region),
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2));

	if (sna->mode.shadow_flip) {
		DamagePtr damage;

		damage = sna->mode.shadow_damage;
		sna->mode.shadow_damage = NULL;

		while (sna->mode.shadow_flip && sna_mode_has_pending_events(sna))
			sna_mode_wakeup(sna);

		sna->mode.shadow_damage = damage;
	}

	if (sna->mode.shadow_flip)
		return;

	if (wedged(sna) || !sna_pixmap_move_to_gpu(sna->front, MOVE_READ | MOVE_ASYNC_HINT)) {
		if (!sna_pixmap_move_to_cpu(sna->front, MOVE_READ))
			return;

		for (i = 0; i < config->num_crtc; i++) {
			xf86CrtcPtr crtc = config->crtc[i];
			struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
			RegionRec damage;

			if (sna_crtc == NULL || !sna_crtc->shadow)
				continue;

			assert(crtc->enabled);
			assert(crtc->transform_in_use || sna->flags & SNA_TEAR_FREE);

			damage.extents = crtc->bounds;
			damage.data = NULL;
			RegionIntersect(&damage, &damage, region);
			if (RegionNotEmpty(&damage))
				sna_crtc_redisplay__fallback(crtc, &damage, sna_crtc->bo);
			RegionUninit(&damage);
		}

		RegionEmpty(region);
		return;
	}

	{
		struct sna_pixmap *priv;

		priv = sna_pixmap(sna->front);
		assert(priv != NULL);

		if (priv->move_to_gpu) {
			if (priv->move_to_gpu == wait_for_shadow) {
				/* No damage written to new scanout
				 * (backbuffer), ignore redisplay request
				 * and continue with the current intact
				 * scanout (frontbuffer).
				 */
				RegionEmpty(region);
				return;
			}

			(void)priv->move_to_gpu(sna, priv, 0);
		}

		assert(priv->move_to_gpu == NULL);
	}

	for (i = 0; i < config->num_crtc; i++) {
		xf86CrtcPtr crtc = config->crtc[i];
		struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
		RegionRec damage;

		if (sna_crtc == NULL)
			continue;

		DBG(("%s: crtc[%d] shadow? %d, transformed? %d\n",
		     __FUNCTION__, i,
		     sna_crtc->shadow,
		     sna_crtc->bo != sna->mode.shadow));

		if (!sna_crtc->shadow || sna_crtc->bo == sna->mode.shadow)
			continue;

		assert(crtc->enabled);
		assert(crtc->transform_in_use);

		damage.extents = crtc->bounds;
		damage.data = NULL;

		RegionIntersect(&damage, &damage, region);
		if (RegionNotEmpty(&damage)) {
			if (sna->flags & SNA_TEAR_FREE) {
				struct drm_mode_crtc_page_flip arg;
				struct kgem_bo *bo;

				RegionUninit(&damage);
				damage.extents = crtc->bounds;
				damage.data = NULL;

				bo = sna_crtc->shadow_bo;
				if (bo == NULL)
					bo = kgem_create_2d(&sna->kgem,
							    crtc->mode.HDisplay,
							    crtc->mode.VDisplay,
							    crtc->scrn->bitsPerPixel,
							    sna_crtc->bo->tiling,
							    CREATE_SCANOUT);
				if (bo == NULL)
					goto disable1;

				sna_crtc_redisplay__composite(crtc, &damage, bo);
				kgem_bo_submit(&sna->kgem, bo);

				arg.crtc_id = sna_crtc->id;
				arg.fb_id = get_fb(sna, bo,
						   crtc->mode.HDisplay,
						   crtc->mode.VDisplay);
				if (arg.fb_id == 0)
					goto disable1;

				arg.user_data = 0;
				arg.flags = DRM_MODE_PAGE_FLIP_EVENT;
				arg.reserved = 0;

				if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_PAGE_FLIP, &arg)) {
					BoxRec box;

					DBG(("%s: flip [fb=%d] on crtc %d [%d, pipe=%d] failed - %d\n",
					     __FUNCTION__, arg.fb_id, i, sna_crtc->id, sna_crtc->pipe, errno));
disable1:
					box.x1 = 0;
					box.y1 = 0;
					box.x2 = crtc->mode.HDisplay;
					box.y2 = crtc->mode.VDisplay;

					if (!sna->render.copy_boxes(sna, GXcopy,
								    sna->front, bo, 0, 0,
								    sna->front, sna_crtc->bo, 0, 0,
								    &box, 1, COPY_LAST)) {
						xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR,
							   "%s: page flipping failed, disabling CRTC:%d (pipe=%d)\n",
							   __FUNCTION__, sna_crtc->id, sna_crtc->pipe);
						sna_crtc_disable(crtc);
					}

					continue;
				}

				sna_crtc->shadow_bo = sna_crtc->bo;
				sna_crtc->bo = bo;

				sna->mode.shadow_flip++;
			} else {
				sna_crtc_redisplay(crtc, &damage);
				kgem_scanout_flush(&sna->kgem, sna_crtc->bo);
			}
		}
		RegionUninit(&damage);
	}

	if (sna->mode.shadow) {
		struct kgem_bo *new = __sna_pixmap_get_bo(sna->front);
		struct kgem_bo *old = sna->mode.shadow;

		DBG(("%s: flipping tear-free outputs\n", __FUNCTION__));
		kgem_bo_submit(&sna->kgem, new);

		for (i = 0; i < config->num_crtc; i++) {
			struct sna_crtc *crtc = config->crtc[i]->driver_private;
			struct drm_mode_crtc_page_flip arg;

			if (crtc == NULL)
				continue;

			DBG(("%s: crtc %d [%d, pipe=%d] active? %d\n",
			     __FUNCTION__, i, crtc->id, crtc->pipe, crtc && crtc->bo));
			if (crtc->bo != old)
				continue;

			assert(config->crtc[i]->enabled);
			assert(crtc->dpms_mode == DPMSModeOn);

			arg.crtc_id = crtc->id;
			arg.fb_id = get_fb(sna, new,
					   sna->scrn->virtualX,
					   sna->scrn->virtualY);
			if (arg.fb_id == 0)
				goto disable2;

			arg.user_data = 0;
			arg.flags = DRM_MODE_PAGE_FLIP_EVENT;
			arg.reserved = 0;

			if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_PAGE_FLIP, &arg)) {
				DBG(("%s: flip [fb=%d] on crtc %d [%d, pipe=%d] failed - %d\n",
				     __FUNCTION__, arg.fb_id, i, crtc->id, crtc->pipe, errno));
disable2:
				if (sna->mode.shadow_flip == 0) {
					BoxRec box;

					box.x1 = 0;
					box.y1 = 0;
					box.x2 = sna->scrn->virtualX;
					box.y2 = sna->scrn->virtualY;

					if (sna->render.copy_boxes(sna, GXcopy,
								    sna->front, new, 0, 0,
								    sna->front, old, 0, 0,
								    &box, 1, COPY_LAST)) {
						kgem_submit(&sna->kgem);
						RegionEmpty(region);
					}

					return;
				}

				xf86DrvMsg(sna->scrn->scrnIndex, X_ERROR,
					   "%s: page flipping failed, disabling CRTC:%d (pipe=%d)\n",
					   __FUNCTION__, crtc->id, crtc->pipe);
				sna_crtc_disable(config->crtc[i]);
				continue;
			}
			sna->mode.shadow_flip++;

			kgem_bo_destroy(&sna->kgem, old);
			crtc->bo = kgem_bo_reference(new);
		}

		DBG(("%s: flipped %d outputs, shadow active? %d\n",
		     __FUNCTION__,
		     sna->mode.shadow_flip,
		     sna->mode.shadow ? sna->mode.shadow->handle : 0));

		if (sna->mode.shadow) {
			assert(old == sna->mode.shadow);
			assert(old->refcnt >= 1);
			set_bo(sna->front, old, region);
		}
	} else
		kgem_submit(&sna->kgem);

	RegionEmpty(region);
}

void sna_mode_wakeup(struct sna *sna)
{
	char buffer[1024];
	int len, i;

	/* The DRM read semantics guarantees that we always get only
	 * complete events.
	 */
	len = read(sna->kgem.fd, buffer, sizeof (buffer));
	if (len < (int)sizeof(struct drm_event))
		return;

	DBG(("%s: len=%d\n", __FUNCTION__, len));

	i = 0;
	while (i < len) {
		struct drm_event *e = (struct drm_event *)&buffer[i];
		switch (e->type) {
		case DRM_EVENT_VBLANK:
			sna_dri_vblank_handler(sna, (struct drm_event_vblank *)e);
			break;
		case DRM_EVENT_FLIP_COMPLETE:
			if (((struct drm_event_vblank *)e)->user_data) {
				sna_dri_page_flip_handler(sna, (struct drm_event_vblank *)e);
			} else {
				if (!--sna->mode.shadow_flip)
					sna_mode_redisplay(sna);
			}
			break;
		default:
			break;
		}
		i += e->length;
	}
}
