/***************************************************************************

 Copyright 2013 Intel Corporation.  All Rights Reserved.

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/ioctl.h>

#include <pciaccess.h>

#include <xorg-server.h>
#include <xf86.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <xf86_OSproc.h>
#include <i915_drm.h>

#ifdef XSERVER_PLATFORM_BUS
#include <xf86platformBus.h>
#endif

#include "intel_driver.h"

struct intel_device {
	char *master_node;
	char *render_node;
	int fd;
	int open_count;
	int master_count;
};

static int intel_device_key = -1;

static int __intel_get_device_id(int fd)
{
	struct drm_i915_getparam gp;
	int devid = 0;

	memset(&gp, 0, sizeof(gp));
	gp.param = I915_PARAM_CHIPSET_ID;
	gp.value = &devid;

	if (ioctl(fd, DRM_IOCTL_I915_GETPARAM, &gp, sizeof(gp)))
		return 0;

	return devid;
}

int intel_entity_get_devid(int idx)
{
	struct intel_device *dev;

	dev = xf86GetEntityPrivate(idx, intel_device_key)->ptr;
	if (dev == NULL)
		return 0;

	return __intel_get_device_id(dev->fd);
}

static inline struct intel_device *intel_device(ScrnInfoPtr scrn)
{
	if (scrn->entityList == NULL)
		return NULL;

	return xf86GetEntityPrivate(scrn->entityList[0], intel_device_key)->ptr;
}

static inline void intel_set_device(ScrnInfoPtr scrn, struct intel_device *dev)
{
	xf86GetEntityPrivate(scrn->entityList[0], intel_device_key)->ptr = dev;
}

static Bool is_i915_device(int fd)
{
	drm_version_t version;
	char name[5] = "";

	memset(&version, 0, sizeof(version));
	version.name_len = 4;
	version.name = name;

	if (drmIoctl(fd, DRM_IOCTL_VERSION, &version))
		return FALSE;

	return strcmp("i915", name) == 0;
}

static int __intel_check_device(int fd)
{
	int ret;

	/* Confirm that this is a i915.ko device with GEM/KMS enabled */
	ret = is_i915_device(fd);
	if (ret) {
		struct drm_i915_getparam gp;
		gp.param = I915_PARAM_HAS_GEM;
		gp.value = &ret;
		if (drmIoctl(fd, DRM_IOCTL_I915_GETPARAM, &gp))
			ret = FALSE;
	}
	if (ret && !hosted()) {
		struct drm_mode_card_res res;

		memset(&res, 0, sizeof(res));
		if (drmIoctl(fd, DRM_IOCTL_MODE_GETRESOURCES, &res))
			ret = FALSE;
	}

	return ret;
}

static int fd_set_cloexec(int fd)
{
	int flags;

	if (fd == -1)
		return fd;

#ifdef FD_CLOEXEC
	flags = fcntl(fd, F_GETFD);
	if (flags != -1) {
		flags |= FD_CLOEXEC;
		fcntl(fd, F_SETFD, flags);
	}
#endif

	return fd;
}

static int fd_set_nonblock(int fd)
{
	int flags;

	if (fd == -1)
		return fd;

	flags = fcntl(fd, F_GETFD);
	if (flags != -1) {
		flags |= O_NONBLOCK;
		fcntl(fd, F_SETFD, flags);
	}

	return fd;
}

static int __intel_open_device(const struct pci_device *pci, char **path)
{
	int fd;

	if (*path == NULL) {
		char id[20];
		int ret;

		if (pci == NULL)
			return -1;

		snprintf(id, sizeof(id),
			 "pci:%04x:%02x:%02x.%d",
			 pci->domain, pci->bus, pci->dev, pci->func);

		ret = drmCheckModesettingSupported(id);
		if (ret) {
			if (xf86LoadKernelModule("i915"))
				ret = drmCheckModesettingSupported(id);
			if (ret)
				return -1;
			/* Be nice to the user and load fbcon too */
			(void)xf86LoadKernelModule("fbcon");
		}

		fd = drmOpen(NULL, id);
		if (fd != -1) {
			*path = drmGetDeviceNameFromFd(fd);
			if (*path == NULL) {
				close(fd);
				fd = -1;
			}
		}
		fd = fd_set_nonblock(fd);
	} else {
#ifdef O_CLOEXEC
		fd = open(*path, O_RDWR | O_NONBLOCK | O_CLOEXEC);
#else
		fd = -1;
#endif
		if (fd == -1)
			fd = fd_set_cloexec(open(*path, O_RDWR | O_NONBLOCK));
	}

	return fd;
}

static char *find_render_node(int fd)
{
#if defined(USE_RENDERNODE)
	struct stat master, render;
	char buf[128];

	if (fstat(fd, &master))
		return NULL;

	if (!S_ISCHR(master.st_mode))
		return NULL;

	/* Are we a render-node ourselves? */
	if (master.st_rdev & 0x80)
		return NULL;

	sprintf(buf, "/dev/dri/renderD%d", (int)((master.st_rdev | 0x80) & 0xbf));
	if (stat(buf, &render) == 0 &&
	    master.st_mode == render.st_mode &&
	    render.st_rdev == ((master.st_rdev | 0x80) & 0xbf))
		return strdup(buf);
#endif

	return NULL;
}

#if defined(ODEV_ATTRIB_PATH)
static char *get_path(struct xf86_platform_device *dev)
{
	const char *path;

	if (dev == NULL)
		return NULL;

	path = xf86_get_platform_device_attrib(dev, ODEV_ATTRIB_PATH);
	if (path == NULL)
		return NULL;

	return strdup(path);
}

#else

static char *get_path(struct xf86_platform_device *dev)
{
	return NULL;
}
#endif


#if defined(ODEV_ATTRIB_FD) && 0
static int get_fd(struct xf86_platform_device *dev)
{
	const char *str;

	if (dev == NULL)
		return -1;

	str = xf86_get_platform_device_attrib(dev, ODEV_ATTRIB_FD);
	if (str == NULL)
		return -1;

	return atoi(str);
}

#else

static int get_fd(struct xf86_platform_device *dev)
{
	return -1;
}

#endif

int intel_open_device(int entity_num,
		      const struct pci_device *pci,
		      struct xf86_platform_device *platform)
{
	struct intel_device *dev;
	char *local_path;
	int fd;

	if (intel_device_key == -1)
		intel_device_key = xf86AllocateEntityPrivateIndex();
	if (intel_device_key == -1)
		return -1;

	dev = xf86GetEntityPrivate(entity_num, intel_device_key)->ptr;
	if (dev)
		return dev->fd;

	local_path = get_path(platform);

	fd = get_fd(platform);
	if (fd == -1)
		fd = __intel_open_device(pci, &local_path);
	if (fd == -1)
		goto err_path;

	if (!__intel_check_device(fd))
		goto err_close;

	dev = malloc(sizeof(*dev));
	if (dev == NULL)
		goto err_close;

	dev->fd = fd;
	dev->open_count = 0;
	dev->master_count = 0;
	dev->master_node = local_path;
	dev->render_node = find_render_node(fd);
	if (dev->render_node == NULL)
		dev->render_node = dev->master_node;

	/* If hosted under a system compositor, just pretend to be master */
	if (hosted()) {
		dev->open_count++;
		dev->master_count++;
	}

	xf86GetEntityPrivate(entity_num, intel_device_key)->ptr = dev;

	return fd;

err_close:
	close(fd);
err_path:
	free(local_path);
	return -1;
}

int intel_get_device(ScrnInfoPtr scrn)
{
	struct intel_device *dev;
	int ret;

	dev = intel_device(scrn);
	assert(dev && dev->fd != -1);

	if (dev->open_count++ == 0) {
		drmSetVersion sv;
		int retry = 2000;

		assert(!hosted());

		/* Check that what we opened was a master or a
		 * master-capable FD, by setting the version of the
		 * interface we'll use to talk to it.
		 */
		do {
			sv.drm_di_major = 1;
			sv.drm_di_minor = 1;
			sv.drm_dd_major = -1;
			sv.drm_dd_minor = -1;
			ret = drmIoctl(dev->fd, DRM_IOCTL_SET_VERSION, &sv);
			if (ret == 0)
				break;

			usleep(1000);
		} while (--retry);
		if (ret != 0) {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
				   "[drm] failed to set drm interface version: %s [%d].\n",
				   strerror(errno), errno);
			dev->open_count--;
			return -1;
		}
	}

	return dev->fd;
}

const char *intel_get_client_name(ScrnInfoPtr scrn)
{
	struct intel_device *dev = intel_device(scrn);
	assert(dev && dev->render_node);
	return dev->render_node;
}

int intel_get_device_id(ScrnInfoPtr scrn)
{
	struct intel_device *dev = intel_device(scrn);
	assert(dev && dev->fd != -1);
	return __intel_get_device_id(dev->fd);
}

int intel_get_master(ScrnInfoPtr scrn)
{
	struct intel_device *dev = intel_device(scrn);
	int ret;

	assert(dev && dev->fd != -1);

	ret = 0;
	if (dev->master_count++ == 0) {
		int retry = 2000;

		assert(!hosted());
		do {
			ret = drmSetMaster(dev->fd);
			if (ret == 0)
				break;
			usleep(1000);
		} while (--retry);
	}

	return ret;
}

int intel_put_master(ScrnInfoPtr scrn)
{
	struct intel_device *dev = intel_device(scrn);
	int ret;

	assert(dev && dev->fd != -1);

	ret = 0;
	assert(dev->master_count);
	if (--dev->master_count == 0) {
		assert(!hosted());
		assert(drmSetMaster(dev->fd) == 0);
		ret = drmDropMaster(dev->fd);
	}

	return ret;
}

void __intel_uxa_release_device(ScrnInfoPtr scrn)
{
	struct intel_device *dev = intel_device(scrn);
	if (dev && dev->open_count == 0) {
		intel_set_device(scrn, NULL);

		drmClose(dev->fd);
		if (dev->render_node != dev->master_node)
			free(dev->render_node);
		free(dev->master_node);
		free(dev);
	}
}

void intel_put_device(ScrnInfoPtr scrn)
{
	struct intel_device *dev = intel_device(scrn);

	assert(dev && dev->fd != -1);

	assert(dev->open_count);
	if (--dev->open_count)
		return;

	assert(!hosted());
	intel_set_device(scrn, NULL);

	drmClose(dev->fd);
	if (dev->render_node != dev->master_node)
		free(dev->render_node);
	free(dev->master_node);
	free(dev);
}
