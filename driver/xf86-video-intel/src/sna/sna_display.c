/*
 * Copyright © 2007 Red Hat, Inc.
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

#include <xorgVersion.h>
#include <X11/Xatom.h>
#include <X11/extensions/dpmsconst.h>
#include <xf86drm.h>
#include <xf86DDC.h> /* for xf86InterpretEDID */

#include "sna.h"
#include "sna_reg.h"
#include "fb/fbpict.h"

#include "intel_options.h"

#define KNOWN_MODE_FLAGS ((1<<14)-1)

#if 0
#define __DBG DBG
#else
#define __DBG(x)
#endif

struct sna_crtc {
	struct drm_mode_modeinfo kmode;
	int dpms_mode;
	PixmapPtr scanout_pixmap;
	struct kgem_bo *bo;
	uint32_t cursor;
	bool shadow;
	bool fallback_shadow;
	bool transform;
	uint8_t id;
	uint8_t pipe;
	uint8_t plane;
	struct list link;
};

struct sna_property {
	drmModePropertyPtr mode_prop;
	uint64_t value;
	int num_atoms; /* if range prop, num_atoms == 1; if enum prop, num_atoms == num_enums + 1 */
	Atom *atoms;
};

struct sna_output {
	int id;
	drmModeConnectorPtr mode_output;
	int num_props;
	struct sna_property *props;

	bool has_panel_limits;
	int panel_hdisplay;
	int panel_vdisplay;

	int dpms_mode;
	char *backlight_iface;
	int backlight_active_level;
	int backlight_max;
	struct list link;
};

static inline struct sna_crtc *to_sna_crtc(xf86CrtcPtr crtc)
{
	return crtc->driver_private;
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

static unsigned get_fb(struct sna *sna, struct kgem_bo *bo,
		       int width, int height)
{
	ScrnInfoPtr scrn = sna->scrn;
	struct drm_mode_fb_cmd arg;

	assert(bo->refcnt);
	assert(bo->proxy == NULL);
	if (bo->delta) {
		DBG(("%s: reusing fb=%d for handle=%d\n",
		     __FUNCTION__, bo->delta, bo->handle));
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

static void
sna_output_backlight_set(xf86OutputPtr output, int level)
{
	struct sna_output *sna_output = output->driver_private;
	char path[1024], val[BACKLIGHT_VALUE_LEN];
	int fd, len, ret;

	DBG(("%s: level=%d\n", __FUNCTION__, level));

	if (level > sna_output->backlight_max)
		level = sna_output->backlight_max;
	if (!sna_output->backlight_iface || level < 0)
		return;

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
	char *str;
	int max;

	str = xf86GetOptValString(sna->Options, OPTION_BACKLIGHT);
	if (str == NULL)
		return NULL;

	sna_output->backlight_iface = str;
	max = sna_output_backlight_get_max(output);
	sna_output->backlight_iface = NULL;
	if (max <= 0) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			   "unrecognised backlight control interface '%s'\n",
			   str);
		return NULL;
	}

	return str;
}

static char *
has_device_backlight(xf86OutputPtr output, int *best_type)
{
	struct sna_output *sna_output = output->driver_private;
	struct sna *sna = to_sna(output->scrn);
	struct pci_device *pci = sna->PciInfo;
	char path[1024];
	char *best_iface = NULL;
	DIR *dir;
	struct dirent *de;

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
	case INT_MAX: best_iface = "user"; from = X_CONFIG; break;
	case FIRMWARE: best_iface = "firmware"; break;
	case PLATFORM: best_iface = "platform"; break;
	case RAW: best_iface = "raw"; break;
	default: best_iface = "unknown"; break;
	}
	xf86DrvMsg(output->scrn->scrnIndex, from,
		   "found backlight control interface %s (type '%s')\n",
		   sna_output->backlight_iface, best_iface);
}

static void
mode_from_kmode(ScrnInfoPtr scrn,
		drmModeModeInfoPtr kmode,
		DisplayModePtr	mode)
{
	memset(mode, 0, sizeof(DisplayModeRec));
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

	xf86SetModeCrtc (mode, scrn->adjustFlags);
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

static bool sna_crtc_is_bound(struct sna *sna, xf86CrtcPtr crtc)
{
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	struct drm_mode_crtc mode;

	if (!sna_crtc->bo)
		return false;

	VG_CLEAR(mode);
	mode.crtc_id = sna_crtc->id;
	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETCRTC, &mode))
		return false;

	DBG(("%s: crtc=%d, mode valid?=%d, fb attached?=%d\n", __FUNCTION__,
	     mode.crtc_id, mode.mode_valid, fb_id(sna_crtc->bo) == mode.fb_id));
	return mode.mode_valid && fb_id(sna_crtc->bo) == mode.fb_id;
}

static void
sna_crtc_force_outputs_on(xf86CrtcPtr crtc)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
	int i;

	/* DPMS handling by the kernel is inconsistent, so after setting a
	 * mode on an output presume that we intend for it to be on, or that
	 * the kernel will force it on.
	 *
	 * So force DPMS to be on for all connected outputs, and restore
	 * the backlight.
	 */
	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr output = xf86_config->output[i];

		if (output->crtc != crtc)
			continue;

		output->funcs->dpms(output, DPMSModeOn);
	}

	to_sna_crtc(crtc)->dpms_mode = DPMSModeOn;
}

static bool
sna_crtc_apply(xf86CrtcPtr crtc)
{
	struct sna *sna = to_sna(crtc->scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
	struct drm_mode_crtc arg;
	uint32_t output_ids[16];
	int output_count = 0;
	int i;

	DBG(("%s\n", __FUNCTION__));
	kgem_bo_submit(&sna->kgem, sna_crtc->bo);

	assert(xf86_config->num_output < ARRAY_SIZE(output_ids));

	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr output = xf86_config->output[i];
		struct sna_output *sna_output;

		if (output->crtc != crtc)
			continue;

		sna_output = output->driver_private;
		output_ids[output_count] =
			sna_output->mode_output->connector_id;
		output_count++;
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

	DBG(("%s: applying crtc [%d] mode=%dx%d+%d+%d@%d, fb=%d%s%s update to %d outputs\n",
	     __FUNCTION__, sna_crtc->id,
	     arg.mode.hdisplay,
	     arg.mode.vdisplay,
	     arg.x, arg.y,
	     arg.mode.clock,
	     arg.fb_id,
	     sna_crtc->shadow ? " [shadow]" : "",
	     sna_crtc->transform ? " [transformed]" : "",
	     output_count));

	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_SETCRTC, &arg))
		return false;

	sna_crtc_force_outputs_on(crtc);
	return true;
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
	if (!sna->mode.shadow_damage)
		return;

	DBG(("%s\n", __FUNCTION__));

	DamageUnregister(&sna->front->drawable, sna->mode.shadow_damage);
	DamageDestroy(sna->mode.shadow_damage);
	sna->mode.shadow_damage = NULL;

	if (sna->mode.shadow) {
		kgem_bo_destroy(&sna->kgem, sna->mode.shadow);
		sna->mode.shadow = NULL;
	}

	sna->mode.shadow_active = 0;
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

	crtc->shadow = false;
}

static void
sna_crtc_disable(xf86CrtcPtr crtc)
{
	struct sna *sna = to_sna(crtc->scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	struct drm_mode_crtc arg;

	DBG(("%s: disabling crtc [%d]\n", __FUNCTION__, sna_crtc->id));

	memset(&arg, 0, sizeof(arg));
	arg.crtc_id = sna_crtc->id;
	(void)drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_SETCRTC, &arg);

	sna_crtc_disable_shadow(sna, sna_crtc);

	if (sna_crtc->bo) {
		kgem_bo_destroy(&sna->kgem, sna_crtc->bo);
		sna_crtc->bo = NULL;
	}

	sna_crtc->dpms_mode = DPMSModeOff;
}

static void update_flush_interval(struct sna *sna)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	int i, max_vrefresh = 0;

	for (i = 0; i < xf86_config->num_crtc; i++) {
		xf86CrtcPtr crtc = xf86_config->crtc[i];

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

void sna_mode_disable_unused(struct sna *sna)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	int i;

	DBG(("%s\n", __FUNCTION__));

	/* Force consistency between kernel and ourselves */
	for (i = 0; i < xf86_config->num_crtc; i++) {
		if (!xf86_config->crtc[i]->enabled)
			sna_crtc_disable(xf86_config->crtc[i]);
	}
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
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	struct drm_mode_fb_cmd fbcon;
	PixmapPtr scratch;
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

	priv = sna_pixmap(sna->front);
	assert(priv && priv->gpu_bo);

	/* Scan the connectors for a framebuffer and assume that is the fbcon */
	VG_CLEAR(fbcon);
	fbcon.fb_id = 0;
	for (i = 0; i < xf86_config->num_crtc; i++) {
		struct sna_crtc *crtc = to_sna_crtc(xf86_config->crtc[i]);
		struct drm_mode_crtc mode;

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

	/* Wrap the fbcon in a pixmap so that we select the right formats
	 * in the render copy in case we need to preserve the fbcon
	 * across a depth change upon starting X.
	 */
	scratch = GetScratchPixmapHeader(sna->scrn->pScreen,
					fbcon.width, fbcon.height,
					fbcon.depth, fbcon.bpp,
					0, NULL);
	if (scratch == NullPixmap)
		return;

	box.x1 = box.y1 = 0;
	box.x2 = min(fbcon.width, sna->front->drawable.width);
	box.y2 = min(fbcon.height, sna->front->drawable.height);

	bo = sna_create_bo_for_fbcon(sna, &fbcon);
	if (bo == NULL)
		goto cleanup_scratch;

	DBG(("%s: fbcon handle=%d\n", __FUNCTION__, bo->handle));

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
				    scratch, bo, sx, sy,
				    sna->front, priv->gpu_bo, dx, dy,
				    &box, 1, 0);
	if (!DAMAGE_IS_ALL(priv->gpu_damage))
		sna_damage_add_box(&priv->gpu_damage, &box);

	kgem_bo_destroy(&sna->kgem, bo);

	sna->scrn->pScreen->canDoBGNoneRoot = ok;

cleanup_scratch:
	FreeScratchPixmapHeader(scratch);
}

static bool use_shadow(struct sna *sna, xf86CrtcPtr crtc)
{
	RRTransformPtr transform;
	PictTransform crtc_to_fb;
	struct pict_f_transform f_crtc_to_fb, f_fb_to_crtc;
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
		if (!sna_crtc_enable_shadow(sna, sna_crtc))
			return NULL;

		DBG(("%s: attaching to per-crtc pixmap %dx%d\n",
		     __FUNCTION__, crtc->mode.HDisplay, crtc->mode.VDisplay));

		bo = kgem_create_2d(&sna->kgem,
				    crtc->mode.HDisplay, crtc->mode.VDisplay,
				    scrn->bitsPerPixel,
				    I915_TILING_X, CREATE_SCANOUT);
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
			bo = kgem_create_2d(&sna->kgem,
					    sna->scrn->virtualX,
					    sna->scrn->virtualY,
					    scrn->bitsPerPixel,
					    I915_TILING_X,
					    CREATE_SCANOUT);
			if (bo == NULL)
				return NULL;

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

	if (mode->HDisplay == 0 || mode->VDisplay == 0)
		return FALSE;

	xf86DrvMsg(crtc->scrn->scrnIndex, X_INFO,
		   "switch to mode %dx%d on crtc %d (pipe %d)\n",
		   mode->HDisplay, mode->VDisplay,
		   sna_crtc->id, sna_crtc->pipe);

	DBG(("%s(crtc=%d [pipe=%d] rotation=%d, x=%d, y=%d, mode=%dx%d@%d)\n",
	     __FUNCTION__, sna_crtc->id, sna_crtc->pipe, rotation, x, y,
	     mode->HDisplay, mode->VDisplay, mode->Clock));

	assert(mode->HDisplay <= sna->mode.kmode->max_width &&
	       mode->VDisplay <= sna->mode.kmode->max_height);

	crtc->funcs->gamma_set(crtc,
			       crtc->gamma_red,
			       crtc->gamma_green,
			       crtc->gamma_blue,
			       crtc->gamma_size);

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
	struct sna *sna = to_sna(crtc->scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	struct drm_mode_cursor arg;

	__DBG(("%s: CRTC:%d\n", __FUNCTION__, sna_crtc->id));

	VG_CLEAR(arg);
	arg.flags = DRM_MODE_CURSOR_BO;
	arg.crtc_id = sna_crtc->id;
	arg.width = arg.height = 64;
	arg.handle = 0;

	(void)drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_CURSOR, &arg);
}

static void
sna_crtc_show_cursor(xf86CrtcPtr crtc)
{
	struct sna *sna = to_sna(crtc->scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	struct drm_mode_cursor arg;

	__DBG(("%s: CRTC:%d\n", __FUNCTION__, sna_crtc->id));

	VG_CLEAR(arg);
	arg.flags = DRM_MODE_CURSOR_BO;
	arg.crtc_id = sna_crtc->id;
	arg.width = arg.height = 64;
	arg.handle = sna_crtc->cursor;

	(void)drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_CURSOR, &arg);
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
	struct sna *sna = to_sna(crtc->scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	struct drm_mode_cursor arg;

	__DBG(("%s: CRTC:%d (%d, %d)\n", __FUNCTION__, sna_crtc->id, x, y));

	VG_CLEAR(arg);
	arg.flags = DRM_MODE_CURSOR_MOVE;
	arg.crtc_id = sna_crtc->id;
	arg.x = x;
	arg.y = y;
	arg.handle = sna_crtc->cursor;

	(void)drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_CURSOR, &arg);
}

static void
sna_crtc_load_cursor_argb(xf86CrtcPtr crtc, CARD32 *image)
{
	struct sna *sna = to_sna(crtc->scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	struct drm_i915_gem_pwrite pwrite;

	__DBG(("%s: CRTC:%d\n", __FUNCTION__, sna_crtc->id));

	VG_CLEAR(pwrite);
	pwrite.handle = sna_crtc->cursor;
	pwrite.offset = 0;
	pwrite.size = 64*64*4;
	pwrite.data_ptr = (uintptr_t)image;
	(void)drmIoctl(sna->kgem.fd, DRM_IOCTL_I915_GEM_PWRITE, &pwrite);
}

static void
sna_crtc_gamma_set(xf86CrtcPtr crtc,
		       CARD16 *red, CARD16 *green, CARD16 *blue, int size)
{
	struct sna *sna = to_sna(crtc->scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);

	drmModeCrtcSetGamma(sna->kgem.fd, sna_crtc->id,
			    size, red, green, blue);
}

static void
sna_crtc_destroy(xf86CrtcPtr crtc)
{
	struct sna *sna = to_sna(crtc->scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);

	sna_crtc_hide_cursor(crtc);
	gem_close(sna->kgem.fd, sna_crtc->cursor);

	list_del(&sna_crtc->link);
	free(sna_crtc);

	crtc->driver_private = NULL;
}

#if HAS_PIXMAP_SHARING
static Bool
sna_set_scanout_pixmap(xf86CrtcPtr crtc, PixmapPtr pixmap)
{
	DBG(("%s: CRTC:%d, pipe=%d setting scanout pixmap=%ld\n",
	     __FUNCTION__,to_sna_crtc(crtc)->id, to_sna_crtc(crtc)->pipe,
	     pixmap ? pixmap->drawable.serialNumber : 0));
	to_sna_crtc(crtc)->scanout_pixmap = pixmap;
	return TRUE;
}
#endif

static const xf86CrtcFuncsRec sna_crtc_funcs = {
	.dpms = sna_crtc_dpms,
	.set_mode_major = sna_crtc_set_mode_major,
	.set_cursor_colors = sna_crtc_set_cursor_colors,
	.set_cursor_position = sna_crtc_set_cursor_position,
	.show_cursor = sna_crtc_show_cursor,
	.hide_cursor = sna_crtc_hide_cursor,
	.load_cursor_argb = sna_crtc_load_cursor_argb,
	.gamma_set = sna_crtc_gamma_set,
	.destroy = sna_crtc_destroy,
#if HAS_PIXMAP_SHARING
	.set_scanout_pixmap = sna_set_scanout_pixmap,
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

static void
sna_crtc_init(ScrnInfoPtr scrn, struct sna_mode *mode, int num)
{
	struct sna *sna = to_sna(scrn);
	xf86CrtcPtr crtc;
	struct sna_crtc *sna_crtc;
	struct drm_i915_get_pipe_from_crtc_id get_pipe;

	DBG(("%s\n", __FUNCTION__));

	sna_crtc = calloc(sizeof(struct sna_crtc), 1);
	if (sna_crtc == NULL)
		return;

	sna_crtc->id = mode->kmode->crtcs[num];
	sna_crtc->dpms_mode = DPMSModeOff;

	VG_CLEAR(get_pipe);
	get_pipe.pipe = 0;
	get_pipe.crtc_id = sna_crtc->id;
	if (drmIoctl(sna->kgem.fd,
		     DRM_IOCTL_I915_GET_PIPE_FROM_CRTC_ID,
		     &get_pipe)) {
		free(sna_crtc);
		return;
	}
	sna_crtc->pipe = get_pipe.pipe;
	sna_crtc->plane = sna_crtc_find_plane(sna, sna_crtc->pipe);

	if (xf86IsEntityShared(scrn->entityList[0]) &&
	    scrn->confScreen->device->screen != sna_crtc->pipe) {
		free(sna_crtc);
		return;
	}

	crtc = xf86CrtcCreate(scrn, &sna_crtc_funcs);
	if (crtc == NULL) {
		free(sna_crtc);
		return;
	}

	crtc->driver_private = sna_crtc;

	sna_crtc->cursor = gem_create(sna->kgem.fd, 64*64*4);
	DBG(("%s: created handle=%d for cursor on CRTC:%d\n",
	     __FUNCTION__, sna_crtc->cursor, sna_crtc->id));

	list_add(&sna_crtc->link, &mode->crtcs);

	DBG(("%s: attached crtc[%d] id=%d, pipe=%d\n",
	     __FUNCTION__, num, sna_crtc->id, sna_crtc->pipe));
}

static bool
is_panel(int type)
{
	return (type == DRM_MODE_CONNECTOR_LVDS ||
		type == DRM_MODE_CONNECTOR_eDP);
}

static xf86OutputStatus
sna_output_detect(xf86OutputPtr output)
{
	/* go to the hw and retrieve a new output struct */
	struct sna *sna = to_sna(output->scrn);
	struct sna_output *sna_output = output->driver_private;
	xf86OutputStatus status;

	DBG(("%s\n", __FUNCTION__));

	drmModeFreeConnector(sna_output->mode_output);
	sna_output->mode_output =
		drmModeGetConnector(sna->kgem.fd, sna_output->id);

	switch (sna_output->mode_output->connection) {
	case DRM_MODE_CONNECTED:
		status = XF86OutputStatusConnected;
		break;
	case DRM_MODE_DISCONNECTED:
		status = XF86OutputStatusDisconnected;
		break;
	default:
	case DRM_MODE_UNKNOWNCONNECTION:
		status = XF86OutputStatusUnknown;
		break;
	}
	return status;
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
	drmModeConnectorPtr koutput = sna_output->mode_output;
	void *raw = NULL;
	int raw_length = 0;
	xf86MonPtr mon = NULL;
	int i;

	/* look for an EDID property */
	for (i = 0; i < koutput->count_props; i++) {
		struct drm_mode_get_property prop;
		struct drm_mode_get_blob blob;
		void *tmp;

		VG_CLEAR(prop);
		prop.prop_id = koutput->props[i];
		prop.count_values = 0;
		prop.count_enum_blobs = 0;
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETPROPERTY, &prop))
			continue;

		if (!(prop.flags & DRM_MODE_PROP_BLOB))
			continue;

		if (strcmp(prop.name, "EDID"))
			continue;

		if (koutput->prop_values[i] == 0)
			continue;

		VG_CLEAR(blob);
		blob.length = 0;
		blob.data = 0;
		blob.blob_id = koutput->prop_values[i];

		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETPROPBLOB, &blob))
			continue;

		DBG(("%s: retreiving blob (property %d, id=%d, value=%ld), length=%d\n",
		     __FUNCTION__, i, koutput->props[i], (long)koutput->prop_values[i],
		     blob.length));

		tmp = malloc(blob.length);
		if (tmp == NULL)
			continue;

		VG(memset(tmp, 0, blob.length));
		blob.data = (uintptr_t)tmp;
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETPROPBLOB, &blob)) {
			free(tmp);
			continue;
		}

		free(raw);
		raw = tmp;
		raw_length = blob.length;
	}

	if (raw) {
		mon = xf86InterpretEDID(output->scrn->scrnIndex, raw);
		if (mon && raw_length > 128)
			mon->flags |= MONITOR_EDID_COMPLETE_RAWDATA;
	}

	xf86OutputSetEDID(output, mon);
	free(raw);
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

	m = xf86GetDefaultModes();
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

static DisplayModePtr
sna_output_get_modes(xf86OutputPtr output)
{
	struct sna_output *sna_output = output->driver_private;
	drmModeConnectorPtr koutput = sna_output->mode_output;
	DisplayModePtr Modes = NULL;
	int i;

	DBG(("%s\n", __FUNCTION__));

	sna_output_attach_edid(output);

	/* modes should already be available */
	for (i = 0; i < koutput->count_modes; i++) {
		DisplayModePtr Mode;

		Mode = calloc(1, sizeof(DisplayModeRec));
		if (Mode) {
			mode_from_kmode(output->scrn, &koutput->modes[i], Mode);
			Modes = xf86ModesAdd(Modes, Mode);
		}
	}

	/*
	 * If the connector type is a panel, we will traverse the kernel mode to
	 * get the panel limit. And then add all the standard modes to fake
	 * the fullscreen experience.
	 * If it is incorrect, please fix me.
	 */
	sna_output->has_panel_limits = false;
	if (is_panel(koutput->connector_type)) {
		sna_output->panel_hdisplay = sna_output->panel_vdisplay = 0;
		for (i = 0; i < koutput->count_modes; i++) {
			drmModeModeInfo *mode_ptr;

			mode_ptr = &koutput->modes[i];
			if (mode_ptr->hdisplay > sna_output->panel_hdisplay)
				sna_output->panel_hdisplay = mode_ptr->hdisplay;
			if (mode_ptr->vdisplay > sna_output->panel_vdisplay)
				sna_output->panel_vdisplay = mode_ptr->vdisplay;
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

	for (i = 0; i < sna_output->num_props; i++) {
		drmModeFreeProperty(sna_output->props[i].mode_prop);
		free(sna_output->props[i].atoms);
	}
	free(sna_output->props);

	drmModeFreeConnector(sna_output->mode_output);
	sna_output->mode_output = NULL;

	free(sna_output->backlight_iface);

	list_del(&sna_output->link);
	free(sna_output);

	output->driver_private = NULL;
}

static void
sna_output_dpms_backlight(xf86OutputPtr output, int oldmode, int mode)
{
	struct sna_output *sna_output = output->driver_private;

	if (!sna_output->backlight_iface)
		return;

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
	drmModeConnectorPtr koutput = sna_output->mode_output;
	int i;

	DBG(("%s: dpms=%d\n", __FUNCTION__, dpms));

	if (dpms != DPMSModeOn)
		kgem_submit(&sna->kgem);

	for (i = 0; i < koutput->count_props; i++) {
		struct drm_mode_get_property prop;

		VG_CLEAR(prop);
		prop.prop_id = koutput->props[i];
		prop.count_values = 0;
		prop.count_enum_blobs = 0;
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETPROPERTY, &prop))
			continue;

		if (strcmp(prop.name, "DPMS"))
			continue;

		/* Record thevalue of the backlight before turning
		 * off the display, and reset if after turnging it on.
		 * Order is important as the kernel may record and also
		 * reset the backlight across DPMS. Hence we need to
		 * record the value before the kernel modifies it
		 * and reapply it afterwards.
		 */
		if (dpms == DPMSModeOff)
			sna_output_dpms_backlight(output,
						  sna_output->dpms_mode,
						  dpms);

		drmModeConnectorSetProperty(sna->kgem.fd,
					    sna_output->id,
					    prop.prop_id,
					    dpms);

		if (dpms != DPMSModeOff)
			sna_output_dpms_backlight(output,
						  sna_output->dpms_mode,
						  dpms);

		sna_output->dpms_mode = dpms;
		break;
	}
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
				     32, PropModeReplace, 1, &value, FALSE,
				     TRUE);
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
	drmModeConnectorPtr mode_output = sna_output->mode_output;
	int i, j, err;

	sna_output->props = calloc(mode_output->count_props,
				     sizeof(struct sna_property));
	if (!sna_output->props)
		return;

	sna_output->num_props = 0;
	for (i = j = 0; i < mode_output->count_props; i++) {
		drmModePropertyPtr drmmode_prop;

		drmmode_prop = drmModeGetProperty(sna->kgem.fd,
						  mode_output->props[i]);
		if (sna_property_ignore(drmmode_prop)) {
			drmModeFreeProperty(drmmode_prop);
			continue;
		}

		sna_output->props[j].mode_prop = drmmode_prop;
		sna_output->props[j].value = mode_output->prop_values[i];
		j++;
	}
	sna_output->num_props = j;

	for (i = 0; i < sna_output->num_props; i++) {
		struct sna_property *p = &sna_output->props[i];
		drmModePropertyPtr drmmode_prop = p->mode_prop;

		if (drmmode_prop->flags & DRM_MODE_PROP_RANGE) {
			p->num_atoms = 1;
			p->atoms = calloc(p->num_atoms, sizeof(Atom));
			if (!p->atoms)
				continue;

			sna_output_create_ranged_atom(output, &p->atoms[0],
						      drmmode_prop->name,
						      drmmode_prop->values[0],
						      drmmode_prop->values[1],
						      p->value,
						      drmmode_prop->flags & DRM_MODE_PROP_IMMUTABLE ? TRUE : FALSE);

		} else if (drmmode_prop->flags & DRM_MODE_PROP_ENUM) {
			p->num_atoms = drmmode_prop->count_enums + 1;
			p->atoms = calloc(p->num_atoms, sizeof(Atom));
			if (!p->atoms)
				continue;

			p->atoms[0] = MakeAtom(drmmode_prop->name, strlen(drmmode_prop->name), TRUE);
			for (j = 1; j <= drmmode_prop->count_enums; j++) {
				struct drm_mode_property_enum *e = &drmmode_prop->enums[j-1];
				p->atoms[j] = MakeAtom(e->name, strlen(e->name), TRUE);
			}

			err = RRConfigureOutputProperty(output->randr_output, p->atoms[0],
							FALSE, FALSE,
							drmmode_prop->flags & DRM_MODE_PROP_IMMUTABLE ? TRUE : FALSE,
							p->num_atoms - 1, (INT32 *)&p->atoms[1]);
			if (err != 0) {
				xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
					   "RRConfigureOutputProperty error, %d\n", err);
			}

			for (j = 0; j < drmmode_prop->count_enums; j++)
				if (drmmode_prop->enums[j].value == p->value)
					break;
			/* there's always a matching value */
			err = RRChangeOutputProperty(output->randr_output, p->atoms[0],
						     XA_ATOM, 32, PropModeReplace, 1, &p->atoms[j+1], FALSE, TRUE);
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
		if (val < 0 || val > sna_output->backlight_max)
			return FALSE;

		if (sna_output->dpms_mode == DPMSModeOn)
			sna_output_backlight_set(output, val);
		sna_output->backlight_active_level = val;
		return TRUE;
	}

	for (i = 0; i < sna_output->num_props; i++) {
		struct sna_property *p = &sna_output->props[i];

		if (p->atoms[0] != property)
			continue;

		if (p->mode_prop->flags & DRM_MODE_PROP_RANGE) {
			uint32_t val;

			if (value->type != XA_INTEGER || value->format != 32 ||
			    value->size != 1)
				return FALSE;
			val = *(uint32_t *)value->data;

			drmModeConnectorSetProperty(sna->kgem.fd, sna_output->id,
						    p->mode_prop->prop_id, (uint64_t)val);
			return TRUE;
		} else if (p->mode_prop->flags & DRM_MODE_PROP_ENUM) {
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
			for (j = 0; j < p->mode_prop->count_enums; j++) {
				if (!strcmp(p->mode_prop->enums[j].name, name)) {
					drmModeConnectorSetProperty(sna->kgem.fd, sna_output->id,
								    p->mode_prop->prop_id, p->mode_prop->enums[j].value);
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

		if (! sna_output->backlight_iface)
			return FALSE;

		val = sna_output_backlight_get(output);
		if (val < 0)
			return FALSE;

		err = RRChangeOutputProperty(output->randr_output, property,
					     XA_INTEGER, 32, PropModeReplace, 1, &val,
					     FALSE, TRUE);
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

static const int subpixel_conv_table[7] = {
	0,
	SubPixelUnknown,
	SubPixelHorizontalRGB,
	SubPixelHorizontalBGR,
	SubPixelVerticalRGB,
	SubPixelVerticalBGR,
	SubPixelNone
};

static const char *output_names[] = {
	"None",
	"VGA",
	"DVI",
	"DVI",
	"DVI",
	"Composite",
	"TV",
	"LVDS",
	"CTV",
	"DIN",
	"DP",
	"HDMI",
	"HDMI",
	"TV",
	"eDP",
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

static void
sna_output_init(ScrnInfoPtr scrn, struct sna_mode *mode, int num)
{
	struct sna *sna = to_sna(scrn);
	xf86OutputPtr output;
	drmModeConnectorPtr koutput;
	struct drm_mode_get_encoder enc;
	struct sna_output *sna_output;
	const char *output_name;
	char name[32];

	koutput = drmModeGetConnector(sna->kgem.fd,
				      mode->kmode->connectors[num]);
	if (!koutput)
		return;

	VG_CLEAR(enc);
	enc.encoder_id = koutput->encoders[0];
	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETENCODER, &enc))
		goto cleanup_connector;

	if (koutput->connector_type < ARRAY_SIZE(output_names))
		output_name = output_names[koutput->connector_type];
	else
		output_name = "UNKNOWN";
	snprintf(name, 32, "%s%d", output_name, koutput->connector_type_id);

	if (xf86IsEntityShared(scrn->entityList[0])) {
		const char *str;

		str = xf86GetOptValString(sna->Options, OPTION_ZAPHOD);
		if (str && !sna_zaphod_match(str, name))
			goto cleanup_connector;

		if ((enc.possible_crtcs & (1 << scrn->confScreen->device->screen)) == 0) {
			if (str) {
				xf86DrvMsg(scrn->scrnIndex, X_ERROR,
					   "%s is an invalid output for screen (pipe) %d\n",
					   name, scrn->confScreen->device->screen);
			}
			goto cleanup_connector;
		}

		enc.possible_crtcs = 1;
		enc.possible_clones = 0;
	}

	output = xf86OutputCreate(scrn, &sna_output_funcs, name);
	if (!output)
		goto cleanup_connector;

	sna_output = calloc(sizeof(struct sna_output), 1);
	if (!sna_output)
		goto cleanup_output;

	sna_output->id = mode->kmode->connectors[num];
	sna_output->mode_output = koutput;

	output->mm_width = koutput->mmWidth;
	output->mm_height = koutput->mmHeight;

	output->subpixel_order = subpixel_conv_table[koutput->subpixel];
	output->driver_private = sna_output;

	if (is_panel(koutput->connector_type))
		sna_output_backlight_init(output);

	output->possible_crtcs = enc.possible_crtcs;
	output->possible_clones = enc.possible_clones;
	output->interlaceAllowed = TRUE;

	list_add(&sna_output->link, &mode->outputs);

	return;

cleanup_output:
	xf86OutputDestroy(output);
cleanup_connector:
	drmModeFreeConnector(koutput);
}

/* The kernel reports possible encoder clones, whereas X uses a list of
 * possible connector clones. This is works when we have a 1:1 mapping
 * between encoders and connectors, but breaks for Haswell which has a pair
 * of DP/HDMI connectors hanging off a single encoder.
 */
static void
sna_mode_compute_possible_clones(ScrnInfoPtr scrn)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	unsigned clones[32] = { 0 };
	int i, j;

	assert(config->num_output <= 32);

	/* Convert from encoder numbering to output numbering */
	for (i = 0; i < config->num_output; i++) {
		unsigned mask = config->output[i]->possible_clones;
		for (j = 0; mask != 0; j++, mask >>= 1) {
			if ((mask & 1) == 0)
				continue;

			clones[j] |= 1 << i;
		}
	}

	for (i = 0; i < config->num_output; i++)
		config->output[i]->possible_clones = clones[i];
}

struct sna_visit_set_pixmap_window {
	PixmapPtr old, new;
};

static int
sna_visit_set_window_pixmap(WindowPtr window, pointer data)
{
    struct sna_visit_set_pixmap_window *visit = data;
    ScreenPtr screen = window->drawable.pScreen;

    if (screen->GetWindowPixmap(window) == visit->old) {
	    screen->SetWindowPixmap(window, visit->new);
	    return WT_WALKCHILDREN;
    }

    return WT_DONTWALKCHILDREN;
}

static void copy_front(struct sna *sna, PixmapPtr old, PixmapPtr new)
{
	struct sna_pixmap *old_priv, *new_priv;
	int16_t sx, sy, dx, dy;
	BoxRec box;

	DBG(("%s\n", __FUNCTION__));

	if (wedged(sna))
		return;

	old_priv = sna_pixmap_force_to_gpu(old, MOVE_READ);
	if (!old_priv)
		return;

	new_priv = sna_pixmap_force_to_gpu(new, MOVE_WRITE);
	if (!new_priv)
		return;

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

	if (!DAMAGE_IS_ALL(new_priv->gpu_damage))
		sna_damage_all(&new_priv->gpu_damage,
			       new->drawable.width,
			       new->drawable.height);
}

static Bool
sna_crtc_resize(ScrnInfoPtr scrn, int width, int height)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	struct sna *sna = to_sna(scrn);
	ScreenPtr screen = scrn->pScreen;
	PixmapPtr old_front, new_front;
	int i;

	DBG(("%s (%d, %d) -> (%d, %d)\n", __FUNCTION__,
	     scrn->virtualX, scrn->virtualY,
	     width, height));

	if (scrn->virtualX == width && scrn->virtualY == height)
		return TRUE;

	assert(sna->front);
	assert(screen->GetScreenPixmap(screen) == sna->front);

	DBG(("%s: creating new framebuffer %dx%d\n",
	     __FUNCTION__, width, height));

	old_front = sna->front;
	new_front = screen->CreatePixmap(screen,
					 width, height, scrn->depth,
					 SNA_CREATE_FB);
	if (!new_front)
		return FALSE;

	for (i = 0; i < xf86_config->num_crtc; i++)
		sna_crtc_disable_shadow(sna, to_sna_crtc(xf86_config->crtc[i]));
	assert(sna->mode.shadow_active == 0);
	assert(sna->mode.shadow_damage == NULL);
	assert(sna->mode.shadow == NULL);

	copy_front(sna, sna->front, new_front);

	sna->front = new_front;
	scrn->virtualX = width;
	scrn->virtualY = height;
	scrn->displayWidth = width;

	for (i = 0; i < xf86_config->num_crtc; i++) {
		xf86CrtcPtr crtc = xf86_config->crtc[i];

		if (!crtc->enabled)
			continue;

		if (!sna_crtc_set_mode_major(crtc,
					     &crtc->mode, crtc->rotation,
					     crtc->x, crtc->y))
			sna_crtc_disable(crtc);
	}

	if (screen->root) {
		struct sna_visit_set_pixmap_window visit;

		visit.old = old_front;
		visit.new = sna->front;
		TraverseTree(screen->root, sna_visit_set_window_pixmap, &visit);
		assert(screen->GetWindowPixmap(screen->root) == sna->front);
	}
	screen->SetScreenPixmap(sna->front);
	assert(screen->GetScreenPixmap(screen) == sna->front);

	screen->DestroyPixmap(old_front);

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
		     __FUNCTION__, i, crtc->bo != NULL));
		if (crtc->bo == NULL)
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

	kgem_submit(&sna->kgem);

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

static const xf86CrtcConfigFuncsRec sna_crtc_config_funcs = {
	sna_crtc_resize
};

static void set_size_range(struct sna *sna)
{
	/* We lie slightly as we expect no single monitor to exceed the
	 * crtc limits, so if the mode exceeds the scanout restrictions,
	 * we will quietly convert that to per-crtc pixmaps.
	 */
	xf86CrtcSetSizeRange(sna->scrn, 320, 200, INT16_MAX, INT16_MAX);
}

bool sna_mode_pre_init(ScrnInfoPtr scrn, struct sna *sna)
{
	struct sna_mode *mode = &sna->mode;
	int i;

	list_init(&mode->crtcs);
	list_init(&mode->outputs);

	xf86CrtcConfigInit(scrn, &sna_crtc_config_funcs);

	mode->kmode = drmModeGetResources(sna->kgem.fd);
	if (!mode->kmode) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "failed to get resources: %s\n", strerror(errno));
		return false;
	}

	set_size_range(sna);

	for (i = 0; i < mode->kmode->count_crtcs; i++)
		sna_crtc_init(scrn, mode, i);

	for (i = 0; i < mode->kmode->count_connectors; i++)
		sna_output_init(scrn, mode, i);

	if (!xf86IsEntityShared(scrn->entityList[0]))
		sna_mode_compute_possible_clones(scrn);

#if HAS_PIXMAP_SHARING
	xf86ProviderSetup(scrn, NULL, "Intel");
#endif
	xf86InitialConfiguration(scrn, TRUE);

	return true;
}

static Bool sna_mode_has_pending_events(struct sna *sna)
{
	struct pollfd pfd;
	pfd.fd = sna->kgem.fd;
	pfd.events = POLLIN;
	return poll(&pfd, 1, 0) == 1;
}

void
sna_mode_close(struct sna *sna)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	int i;

	/* In order to workaround a kernel bug in not honouring O_NONBLOCK,
	 * check that the fd is readable before attempting to read the next
	 * event from drm.
	 */
	if (sna_mode_has_pending_events(sna))
		sna_mode_wakeup(sna);

	for (i = 0; i < xf86_config->num_crtc; i++)
		sna_crtc_disable_shadow(sna, to_sna_crtc(xf86_config->crtc[i]));
}

void
sna_mode_fini(struct sna *sna)
{
#if 0
	while (!list_is_empty(&mode->crtcs)) {
		xf86CrtcDestroy(list_first_entry(&mode->crtcs,
						 struct sna_crtc,
						 link)->crtc);
	}

	while (!list_is_empty(&mode->outputs)) {
		xf86OutputDestroy(list_first_entry(&mode->outputs,
						   struct sna_output,
						   link)->output);
	}
#endif
}

static bool sna_box_intersect(BoxPtr r, const BoxRec *a, const BoxRec *b)
{
	r->x1 = a->x1 > b->x1 ? a->x1 : b->x1;
	r->x2 = a->x2 < b->x2 ? a->x2 : b->x2;
	r->y1 = a->y1 > b->y1 ? a->y1 : b->y1;
	r->y2 = a->y2 < b->y2 ? a->y2 : b->y2;
	DBG(("%s: (%d, %d), (%d, %d) intersect (%d, %d), (%d, %d) = (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     a->x1, a->y1, a->x2, a->y2,
	     b->x1, b->y1, b->x2, b->y2,
	     r->x1, r->y1, r->x2, r->y2));
	return r->x2 > r->x1 && r->y2 > r->y1;
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
sna_covering_crtc(ScrnInfoPtr scrn,
		  const BoxRec *box,
		  xf86CrtcPtr desired)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	xf86CrtcPtr best_crtc;
	int best_coverage, c;

	/* If we do not own the VT, we do not own the CRTC either */
	if (!scrn->vtSema)
		return NULL;

	DBG(("%s for box=(%d, %d), (%d, %d)\n",
	     __FUNCTION__, box->x1, box->y1, box->x2, box->y2));

	best_crtc = NULL;
	best_coverage = 0;
	for (c = 0; c < xf86_config->num_crtc; c++) {
		xf86CrtcPtr crtc = xf86_config->crtc[c];
		BoxRec cover_box;
		int coverage;

		/* If the CRTC is off, treat it as not covering */
		if (to_sna_crtc(crtc)->bo == NULL) {
			DBG(("%s: crtc %d off, skipping\n", __FUNCTION__, c));
			continue;
		}

		DBG(("%s: crtc %d: (%d, %d), (%d, %d)\n",
		     __FUNCTION__, c,
		     crtc->bounds.x1, crtc->bounds.y1,
		     crtc->bounds.x2, crtc->bounds.y2));

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

static bool sna_emit_wait_for_scanline_gen7(struct sna *sna,
					    int pipe, int y1, int y2,
					    bool full_height)
{
	uint32_t *b;

	if (!sna->kgem.has_secure_batches)
		return false;

	assert(y1 >= 0);
	assert(y2 > y1);
	assert(sna->kgem.mode);

	b = kgem_get_batch(&sna->kgem, 16);
	b[0] = MI_LOAD_REGISTER_IMM | 1;
	b[1] = 0x44050; /* DERRMR */
	b[2] = ~(1 << (3*full_height + pipe*8));
	b[3] = MI_LOAD_REGISTER_IMM | 1;
	b[4] = 0xa188; /* FORCEWAKE_MT */
	b[5] = 2 << 16 | 2;
	b[6] = MI_LOAD_REGISTER_IMM | 1;
	b[7] = 0x70068 + 0x1000 * pipe;
	b[8] = (1 << 31) | (1 << 30) | (y1 << 16) | (y2 - 1);
	b[9] = MI_WAIT_FOR_EVENT | 1 << (3*full_height + pipe*5);
	b[10] = MI_LOAD_REGISTER_IMM | 1;
	b[11] = 0xa188; /* FORCEWAKE_MT */
	b[12] = 2 << 16;
	b[13] = MI_LOAD_REGISTER_IMM | 1;
	b[14] = 0x44050; /* DERRMR */
	b[15] = ~0;
	kgem_advance_batch(&sna->kgem, 16);

	sna->kgem.batch_flags |= I915_EXEC_SECURE;
	return true;
}

static bool sna_emit_wait_for_scanline_gen6(struct sna *sna,
					    int pipe, int y1, int y2,
					    bool full_height)
{
	uint32_t *b;

	if (!sna->kgem.has_secure_batches)
		return false;

	assert(y1 >= 0);
	assert(y2 > y1);
	assert(sna->kgem.mode);

	b = kgem_get_batch(&sna->kgem, 10);
	b[0] = MI_LOAD_REGISTER_IMM | 1;
	b[1] = 0x44050; /* DERRMR */
	b[2] = ~(1 << (3*full_height + pipe*8));
	b[3] = MI_LOAD_REGISTER_IMM | 1;
	b[4] = 0x4f100; /* magic */
	b[5] = (1 << 31) | (1 << 30) | pipe << 29 | (y1 << 16) | (y2 - 1);
	b[6] = MI_WAIT_FOR_EVENT | 1 << (3*full_height + pipe*5);
	b[7] = MI_LOAD_REGISTER_IMM | 1;
	b[8] = 0x44050; /* DERRMR */
	b[9] = ~0;
	kgem_advance_batch(&sna->kgem, 10);

	sna->kgem.batch_flags |= I915_EXEC_SECURE;
	return true;
}

static bool sna_emit_wait_for_scanline_gen4(struct sna *sna,
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

	b = kgem_get_batch(&sna->kgem, 5);
	/* The documentation says that the LOAD_SCAN_LINES command
	 * always comes in pairs. Don't ask me why. */
	b[2] = b[0] = MI_LOAD_SCAN_LINES_INCL | pipe << 20;
	b[3] = b[1] = (y1 << 16) | (y2-1);
	b[4] = MI_WAIT_FOR_EVENT | event;
	kgem_advance_batch(&sna->kgem, 5);

	return true;
}

static bool sna_emit_wait_for_scanline_gen2(struct sna *sna,
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

	b = kgem_get_batch(&sna->kgem, 5);
	/* The documentation says that the LOAD_SCAN_LINES command
	 * always comes in pairs. Don't ask me why. */
	b[2] = b[0] = MI_LOAD_SCAN_LINES_INCL | pipe << 20;
	b[3] = b[1] = (y1 << 16) | (y2-1);
	if (pipe == 0)
		b[4] = MI_WAIT_FOR_EVENT | MI_WAIT_FOR_PIPEA_SCAN_LINE_WINDOW;
	else
		b[4] = MI_WAIT_FOR_EVENT | MI_WAIT_FOR_PIPEB_SCAN_LINE_WINDOW;
	kgem_advance_batch(&sna->kgem, 5);

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

	assert(crtc);
	assert(to_sna_crtc(crtc)->bo != NULL);
	assert(pixmap == sna->front);

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
	if (y2 <= y1)
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

	if (sna->kgem.gen >= 0100)
		ret = false;
	else if (sna->kgem.gen >= 070)
		ret = sna_emit_wait_for_scanline_gen7(sna, pipe, y1, y2, full_height);
	else if (sna->kgem.gen >= 060)
		ret =sna_emit_wait_for_scanline_gen6(sna, pipe, y1, y2, full_height);
	else if (sna->kgem.gen >= 040)
		ret = sna_emit_wait_for_scanline_gen4(sna, pipe, y1, y2, full_height);
	else
		ret = sna_emit_wait_for_scanline_gen2(sna, pipe, y1, y2, full_height);

	return ret;
}

void sna_mode_update(struct sna *sna)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	int i;

	/* Validate CRTC attachments */
	for (i = 0; i < xf86_config->num_crtc; i++) {
		xf86CrtcPtr crtc = xf86_config->crtc[i];
		if (!crtc->active || !sna_crtc_is_bound(sna, crtc))
			sna_crtc_disable(crtc);
	}

	update_flush_interval(sna);
}

static void
sna_crtc_redisplay__fallback(xf86CrtcPtr crtc, RegionPtr region)
{
	struct sna *sna = to_sna(crtc->scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	ScreenPtr screen = sna->scrn->pScreen;
	PictFormatPtr format;
	PicturePtr src, dst;
	PixmapPtr pixmap;
	BoxPtr b;
	int n, error;
	void *ptr;

	DBG(("%s: compositing transformed damage boxes\n", __FUNCTION__));

	ptr = kgem_bo_map__gtt(&sna->kgem, sna_crtc->bo);
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
					sna_crtc->bo->pitch, ptr))
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

	kgem_bo_sync__gtt(&sna->kgem, sna_crtc->bo);
	n = REGION_NUM_RECTS(region);
	b = REGION_RECTS(region);
	do {
		BoxRec box;

		box = *b++;
		box.x1 -= crtc->filter_width >> 1;
		box.x2 += crtc->filter_width >> 1;
		box.y1 -= crtc->filter_height >> 1;
		box.y2 += crtc->filter_height >> 1;
		pixman_f_transform_bounds(&crtc->f_framebuffer_to_crtc, & box);

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

	FreePicture(dst, None);
free_src:
	FreePicture(src, None);
free_pixmap:
	screen->DestroyPixmap(pixmap);
}

static void
sna_crtc_redisplay__composite(xf86CrtcPtr crtc, RegionPtr region)
{
	struct sna *sna = to_sna(crtc->scrn);
	struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
	ScreenPtr screen = sna->scrn->pScreen;
	struct sna_composite_op tmp;
	PictFormatPtr format;
	PicturePtr src, dst;
	PixmapPtr pixmap;
	BoxPtr b;
	int n, error;

	DBG(("%s: compositing transformed damage boxes\n", __FUNCTION__));

	pixmap = sna_pixmap_create_unattached(screen,
					      crtc->mode.HDisplay,
					      crtc->mode.VDisplay,
					      sna->front->drawable.depth);
	if (pixmap == NullPixmap)
		return;

	if (!sna_pixmap_attach_to_bo(pixmap, sna_crtc->bo))
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
				   0, 0,
				   memset(&tmp, 0, sizeof(tmp)))) {
		DBG(("%s: unsupported operation!\n", __FUNCTION__));
		sna_crtc_redisplay__fallback(crtc, region);
		goto free_dst;
	}

	n = REGION_NUM_RECTS(region);
	b = REGION_RECTS(region);
	do {
		BoxRec box;

		box = *b++;
		box.x1 -= crtc->filter_width >> 1;
		box.x2 += crtc->filter_width >> 1;
		box.y1 -= crtc->filter_height >> 1;
		box.y2 += crtc->filter_height >> 1;
		pixman_f_transform_bounds(&crtc->f_framebuffer_to_crtc, & box);

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
	int16_t tx, ty;

	DBG(("%s: crtc %d [pipe=%d], damage (%d, %d), (%d, %d) x %d\n",
	     __FUNCTION__, sna_crtc->id, sna_crtc->pipe,
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2,
	     REGION_NUM_RECTS(region)));

	assert(!wedged(sna));

	if (crtc->filter == NULL &&
	    sna_transform_is_integer_translation(&crtc->crtc_to_framebuffer,
						 &tx, &ty)) {
		PixmapRec tmp;

		DBG(("%s: copy damage boxes\n", __FUNCTION__));

		tmp.drawable.width = crtc->mode.HDisplay;
		tmp.drawable.height = crtc->mode.VDisplay;
		tmp.drawable.depth = sna->front->drawable.depth;
		tmp.drawable.bitsPerPixel = sna->front->drawable.bitsPerPixel;

		/* XXX for tear-free we may want to try copying to a back
		 * and flipping.
		 */

		if (sna->render.copy_boxes(sna, GXcopy,
					   sna->front, sna_pixmap_get_bo(sna->front), 0, 0,
					   &tmp, sna_crtc->bo, -tx, -ty,
					   REGION_RECTS(region), REGION_NUM_RECTS(region), 0))
			return;
	}

	sna_crtc_redisplay__composite(crtc, region);
}

void sna_mode_redisplay(struct sna *sna)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	RegionPtr region;
	int i;

	if (!sna->mode.shadow_damage)
		return;

	DBG(("%s: posting shadow damage\n", __FUNCTION__));
	assert(sna->mode.shadow_active);

	region = DamageRegion(sna->mode.shadow_damage);
	if (RegionNil(region))
		return;

	if (!sna_pixmap_move_to_gpu(sna->front, MOVE_READ)) {
		if (!sna_pixmap_move_to_cpu(sna->front, MOVE_READ))
			return;

		for (i = 0; i < config->num_crtc; i++) {
			xf86CrtcPtr crtc = config->crtc[i];
			struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
			RegionRec damage;

			if (!sna_crtc->shadow)
				continue;

			assert(crtc->enabled);
			assert(crtc->transform_in_use);

			damage.extents = crtc->bounds;
			damage.data = NULL;
			RegionIntersect(&damage, &damage, region);
			if (RegionNotEmpty(&damage))
				sna_crtc_redisplay__fallback(crtc, &damage);
			RegionUninit(&damage);
		}

		RegionEmpty(region);
		return;
	}

	for (i = 0; i < config->num_crtc; i++) {
		xf86CrtcPtr crtc = config->crtc[i];
		struct sna_crtc *sna_crtc = to_sna_crtc(crtc);
		RegionRec damage;

		if (!sna_crtc->shadow || sna_crtc->bo == sna->mode.shadow)
			continue;

		assert(crtc->enabled);
		assert(crtc->transform_in_use);

		damage.extents = crtc->bounds;
		damage.data = NULL;
		RegionIntersect(&damage, &damage, region);
		if (RegionNotEmpty(&damage)) {
			sna_crtc_redisplay(crtc, &damage);
			__kgem_flush(&sna->kgem, sna_crtc->bo);
		}
		RegionUninit(&damage);
	}

	if (!sna->mode.shadow) {
		kgem_submit(&sna->kgem);
		RegionEmpty(region);
		return;
	}

	if (sna->mode.shadow_flip == 0) {
		struct kgem_bo *new = sna_pixmap_get_bo(sna->front);
		struct kgem_bo *old = sna->mode.shadow;

		DBG(("%s: flipping tear-free outputs\n", __FUNCTION__));
		kgem_bo_submit(&sna->kgem, new);

		for (i = 0; i < config->num_crtc; i++) {
			struct sna_crtc *crtc = config->crtc[i]->driver_private;
			struct drm_mode_crtc_page_flip arg;

			DBG(("%s: crtc %d [%d, pipe=%d] active? %d\n",
			     __FUNCTION__, i, crtc->id, crtc->pipe, crtc->bo != NULL));
			if (crtc->bo != old)
				continue;

			assert(config->crtc[i]->enabled);
			assert(crtc->dpms_mode == DPMSModeOn);

			arg.crtc_id = crtc->id;
			arg.fb_id = get_fb(sna, new,
					   sna->scrn->virtualX,
					   sna->scrn->virtualY);
			if (arg.fb_id == 0)
				goto disable;

			arg.user_data = 0;
			arg.flags = DRM_MODE_PAGE_FLIP_EVENT;
			arg.reserved = 0;

			if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_PAGE_FLIP, &arg)) {
				DBG(("%s: flip [fb=%d] on crtc %d [%d, pipe=%d] failed - %d\n",
				     __FUNCTION__, arg.fb_id, i, crtc->id, crtc->pipe, errno));
disable:
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

		if (sna->mode.shadow) {
			while (sna->mode.shadow_flip)
				sna_mode_wakeup(sna);
			(void)sna->render.copy_boxes(sna, GXcopy,
						     sna->front, new, 0, 0,
						     sna->front, old, 0, 0,
						     REGION_RECTS(region),
						     REGION_NUM_RECTS(region),
						     COPY_LAST);
			kgem_submit(&sna->kgem);

			sna_pixmap(sna->front)->gpu_bo = old;
			sna_dri_pixmap_update_bo(sna, sna->front);

			sna->mode.shadow = new;
			new->flush = old->flush;
		}

		RegionEmpty(region);
	}
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
			if (((struct drm_event_vblank *)e)->user_data)
				sna_dri_page_flip_handler(sna, (struct drm_event_vblank *)e);
			else
				sna->mode.shadow_flip--;
			break;
		default:
			break;
		}
		i += e->length;
	}
}
