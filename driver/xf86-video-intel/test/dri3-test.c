/*
 * Copyright (c) 2014 Intel Corporation
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
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xlibint.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/XShm.h>
#if HAVE_X11_EXTENSIONS_SHMPROTO_H
#include <X11/extensions/shmproto.h>
#elif HAVE_X11_EXTENSIONS_SHMSTR_H
#include <X11/extensions/shmstr.h>
#else
#error Failed to find the right header for X11 MIT-SHM protocol definitions
#endif
#include <xf86drm.h>
#include <i915_drm.h>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pciaccess.h>

#include "dri3.h"
#include "../src/i915_pciids.h"

#define ALIGN(x, y) (((x) + (y) - 1) & -(y))
#define PAGE_ALIGN(x) ALIGN(x, 4096)

#define GTT I915_GEM_DOMAIN_GTT
#define CPU I915_GEM_DOMAIN_CPU

static int _x_error_occurred;

static const struct pci_id_match ids[] = {
	INTEL_I830_IDS(020),
	INTEL_I845G_IDS(021),
	INTEL_I85X_IDS(022),
	INTEL_I865G_IDS(023),

	INTEL_I915G_IDS(030),
	INTEL_I915GM_IDS(030),
	INTEL_I945G_IDS(031),
	INTEL_I945GM_IDS(031),

	INTEL_G33_IDS(033),
	INTEL_PINEVIEW_IDS(033),

	INTEL_I965G_IDS(040),
	INTEL_I965GM_IDS(040),

	INTEL_G45_IDS(045),
	INTEL_GM45_IDS(045),

	INTEL_IRONLAKE_D_IDS(050),
	INTEL_IRONLAKE_M_IDS(050),

	INTEL_SNB_D_IDS(060),
	INTEL_SNB_M_IDS(060),

	INTEL_IVB_D_IDS(070),
	INTEL_IVB_M_IDS(070),

	INTEL_HSW_D_IDS(075),
	INTEL_HSW_M_IDS(075),

	INTEL_VLV_D_IDS(071),
	INTEL_VLV_M_IDS(071),

	INTEL_BDW_D_IDS(0100),
	INTEL_BDW_M_IDS(0100),
};

static int i915_gen(int device)
{
	struct drm_i915_getparam gp;
	int devid = 0;
	int n;

	gp.param = I915_PARAM_CHIPSET_ID;
	gp.value = &devid;

	if (drmIoctl(device, DRM_IOCTL_I915_GETPARAM, &gp))
		return 0;

	for (n = 0; n < sizeof(ids)/sizeof(ids[0]); n++) {
		if (devid == ids[n].device_id)
			return ids[n].match_data;
	}

	return 0;
}

static int is_i915_device(int fd)
{
	drm_version_t version;
	char name[5] = "";

	memset(&version, 0, sizeof(version));
	version.name_len = 4;
	version.name = name;

	if (drmIoctl(fd, DRM_IOCTL_VERSION, &version))
		return 0;

	return strcmp("i915", name) == 0;
}

static int is_intel(int fd)
{
	struct drm_i915_getparam gp;
	int ret;

	/* Confirm that this is a i915.ko device with GEM/KMS enabled */
	ret = is_i915_device(fd);
	if (ret) {
		gp.param = I915_PARAM_HAS_GEM;
		gp.value = &ret;
		if (drmIoctl(fd, DRM_IOCTL_I915_GETPARAM, &gp))
			ret = 0;
	}
	return ret;
}

static uint32_t gem_create(int fd, int size)
{
	struct drm_i915_gem_create create;

	create.handle = 0;
	create.size = size;
	(void)drmIoctl(fd, DRM_IOCTL_I915_GEM_CREATE, &create);

	return create.handle;
}

struct local_i915_gem_caching {
	uint32_t handle;
	uint32_t caching;
};

#define LOCAL_I915_GEM_SET_CACHING	0x2f
#define LOCAL_IOCTL_I915_GEM_SET_CACHING DRM_IOW(DRM_COMMAND_BASE + LOCAL_I915_GEM_SET_CACHING, struct local_i915_gem_caching)

static int gem_set_caching(int fd, uint32_t handle, int caching)
{
	struct local_i915_gem_caching arg;

	arg.handle = handle;
	arg.caching = caching;

	return drmIoctl(fd, LOCAL_IOCTL_I915_GEM_SET_CACHING, &arg) == 0;
}

static int gem_export(int fd, uint32_t handle)
{
	struct drm_prime_handle args;

	args.handle = handle;
	args.flags = O_CLOEXEC;

	if (drmIoctl(fd, DRM_IOCTL_PRIME_HANDLE_TO_FD, &args))
		return -1;

	return args.fd;
}

static uint32_t gem_import(int fd, int name)
{
	struct drm_prime_handle args;

	args.fd = name;
	args.flags = 0;
	if (drmIoctl(fd, DRM_IOCTL_PRIME_FD_TO_HANDLE, &args))
		return 0;

	return args.handle;
}

static int gem_write(int fd, uint32_t handle, int offset, void *data, int len)
{
	struct drm_i915_gem_pwrite gem_pwrite;

	gem_pwrite.handle = handle;
	gem_pwrite.offset = offset;
	gem_pwrite.size = len;
	gem_pwrite.data_ptr = (uintptr_t)data;
	return drmIoctl(fd, DRM_IOCTL_I915_GEM_PWRITE, &gem_pwrite);
}

static void *gem_mmap(int fd, uint32_t handle, int size, unsigned prot, int domain)
{
	struct drm_i915_gem_set_domain set_domain;
	void *ptr;

	if (domain == CPU) {
		struct drm_i915_gem_mmap mmap_arg;

		mmap_arg.handle = handle;
		mmap_arg.offset = 0;
		mmap_arg.size = size;
		if (drmIoctl(fd, DRM_IOCTL_I915_GEM_MMAP, &mmap_arg))
			return NULL;

		ptr = (void *)(uintptr_t)mmap_arg.addr_ptr;
	} else {
		struct drm_i915_gem_mmap_gtt mmap_arg;

		mmap_arg.handle = handle;
		if (drmIoctl(fd, DRM_IOCTL_I915_GEM_MMAP_GTT, &mmap_arg))
			return NULL;

		ptr = mmap(0, size, prot, MAP_SHARED, fd, mmap_arg.offset);
		if (ptr == MAP_FAILED)
			return NULL;
	}

	set_domain.handle = handle;
	set_domain.read_domains = domain;
	set_domain.write_domain = prot & PROT_WRITE ? set_domain.read_domains : 0;
	if (drmIoctl(fd, DRM_IOCTL_I915_GEM_SET_DOMAIN, &set_domain)) {
		munmap(ptr, size);
		return NULL;
	}

	return ptr;
}

static void gem_sync(int fd, uint32_t handle, int read)
{
	struct drm_i915_gem_set_domain set_domain;

	set_domain.handle = handle;
	set_domain.read_domains = read;
	set_domain.write_domain = 0;
	drmIoctl(fd, DRM_IOCTL_I915_GEM_SET_DOMAIN, &set_domain);
}

static int gem_get_tiling(int fd, uint32_t handle)
{
	struct drm_i915_gem_get_tiling tiling;

	tiling.handle = handle;
	tiling.tiling_mode = -1;
	(void)drmIoctl(fd, DRM_IOCTL_I915_GEM_GET_TILING, &tiling);
	return tiling.tiling_mode;
}

static void gem_close(int fd, uint32_t handle)
{
	struct drm_gem_close close;

	close.handle = handle;
	(void)drmIoctl(fd, DRM_IOCTL_GEM_CLOSE, &close);
}

static void gem_fill(int fd, uint32_t handle, uint32_t pixel, uint32_t size, int domain)
{
	uint32_t *ptr, s;

	ptr = gem_mmap(fd, handle, size, PROT_READ | PROT_WRITE, domain);
	if (ptr == NULL)
		return;

	for (s = 0; s < size; s += 4)
		ptr[s/4] = pixel;
	munmap(ptr, size);
}

static int check_pixmap(Display *dpy, Pixmap pix,
			int x, int y, uint32_t expected, int bpp)
{
	XImage *image;
	int w = 32 / bpp;

	image = XGetImage(dpy, pix, x - (x % w), y, w, 1, AllPlanes, ZPixmap);
	if (image == NULL)
		return 0;

	if (*(uint32_t *)image->data != expected) {
		printf("pixmap[%d, %d]:%d = %08x\n", x, y, bpp, *(uint32_t *)image->data);
		return 0;
	}
	XDestroyImage(image);

	return 1;
}

static int check_pixel(int fd, uint32_t handle, uint32_t stride, uint32_t size,
		       int x, int y, uint32_t expected, int bpp, int domain)
{
	uint32_t *ptr;
	int w = 32 / bpp;

	assert((stride & 3) == 0);

	ptr = gem_mmap(fd, handle, size, PROT_READ, domain);
	if (ptr == NULL)
		return 0;

	if (ptr[(y*stride + x - (x % w))/4] != expected) {
		printf("pixel[%d, %d]:%d = %08x\n", x, y, bpp, ptr[(y * stride + x)/4]);
		return 0;
	}
	munmap(ptr, size);

	return 1;
}

static GC get_gc(Display *dpy, Drawable d, int depth)
{
	static GC gc[33];
	if (gc[depth] == NULL) {
		XGCValues gcv;

		gcv.graphics_exposures = False;
		gc[depth] = XCreateGC(dpy, d, GCGraphicsExposures, &gcv);
	}
	return gc[depth];
}

static int
can_use_shm(Display *dpy)
{
	int major, minor, has_pixmap;

	if (!XShmQueryExtension(dpy))
		return 0;

	XShmQueryVersion(dpy, &major, &minor, &has_pixmap);
	return has_pixmap;
}

static int gpu_fill(int device, int handle, int width, int height, int pitch, int bpp, int tiling, uint32_t pixel)
{
	struct drm_i915_gem_execbuffer2 execbuf;
	struct drm_i915_gem_relocation_entry gem_reloc[2];
	struct drm_i915_gem_exec_object2 gem_exec[2];
	uint32_t batch[10];
	int gen = i915_gen(device);
	int len = 0;
	int ret;

	if (gen == 0)
		return -ENODEV;

	batch[0] = 2 << 29 | 0x50 << 22;
	batch[0] |= (gen >= 0100 ? 5 : 4);
	batch[1] = pitch;
	if (gen >= 040 && tiling) {
		batch[0] |= 1 << 11;
		batch[1] >>= 2;
	}

	batch[1] |= 0xf0 << 16;
	switch (bpp) {
	default: assert(0);
	case 32: batch[0] |= 1 << 21 | 1 << 20;
		 batch[1] |= 1 << 25; /* RGB8888 */
	case 16: batch[1] |= 1 << 24; /* RGB565 */
	case 8: break;
	}

	batch[2] = 0;
	batch[3] = height << 16 | width;
	batch[4] = 0;
	len = 5;
	if (gen >= 0100)
		batch[len++] = 0;
	batch[len++] = pixel;
	batch[len++] = 0xA << 23;
	if (len & 1)
		len++;

	gem_reloc[0].offset = 4 * sizeof(uint32_t);
	gem_reloc[0].delta = 0;
	gem_reloc[0].target_handle = handle;
	gem_reloc[0].read_domains = I915_GEM_DOMAIN_RENDER;
	gem_reloc[0].write_domain = I915_GEM_DOMAIN_RENDER;
	gem_reloc[0].presumed_offset = 0;

	memset(gem_exec, 0, sizeof(gem_exec));
	gem_exec[0].handle = handle;
	gem_exec[1].handle = gem_create(device, 4096);
	gem_exec[1].relocation_count = 1;
	gem_exec[1].relocs_ptr = (uintptr_t)gem_reloc;

	memset(&execbuf, 0, sizeof(execbuf));
	execbuf.buffers_ptr = (uintptr_t)gem_exec;
	execbuf.buffer_count = 2;
	execbuf.batch_len = len * sizeof(uint32_t);
	execbuf.flags = gen >= 060 ? I915_EXEC_BLT : 0;

	ret = gem_write(device, gem_exec[1].handle, 0, batch, execbuf.batch_len);
	if (ret == 0)
		ret = drmIoctl(device, DRM_IOCTL_I915_GEM_EXECBUFFER2, &execbuf);
	if (ret < 0)
		ret = -errno;

	gem_close(device, gem_exec[1].handle);
	return ret;
}

static int test_shm(Display *dpy, int device,
		    int width, int height)
{
	const int x_loc[] = {0, width/2, width-1};
	const int y_loc[] = {0, height/2, height-1};
	uint32_t pixel = 0xffff00ff;
	XShmSegmentInfo shm;
	Pixmap pixmap;
	uint32_t handle = 0;
	uint32_t *ptr;
	int stride, fd;
	int x, y;
	int line;

	if (!can_use_shm(dpy))
		return 0;

	printf("Creating %dx%d SHM pixmap\n", width, height);
	_x_error_occurred = 0;

	shm.shmid = shmget(IPC_PRIVATE, height * 4*width, IPC_CREAT | 0666);
	if (shm.shmid == -1)
		return 0;

	shm.shmaddr = shmat(shm.shmid, 0, 0);
	if (shm.shmaddr == (char *) -1) {
		shmctl(shm.shmid, IPC_RMID, NULL);
		return 0;
	}

	shm.readOnly = False;
	XShmAttach(dpy, &shm);

	pixmap = XShmCreatePixmap(dpy, DefaultRootWindow(dpy),
				  shm.shmaddr, &shm, width, height, 24);
	XSync(dpy, False);
	shmctl(shm.shmid, IPC_RMID, NULL);

	if (_x_error_occurred) {
		XShmDetach(dpy, &shm);
		shmdt(shm.shmaddr);
		return 0;
	}

	printf("Testing write of %dx%d SHM pixmap via DRI3 fd\n", width, height);

	fd = dri3_create_fd(dpy, pixmap, &stride);
	if (fd < 0) {
		line = __LINE__;
		goto fail;
	}

	handle = gem_import(device, fd);
	close(fd);
	if (handle == 0) {
		line = __LINE__;
		goto fail;
	}

	if (gpu_fill(device, handle, width, height, stride, 32, I915_TILING_NONE, pixel)) {
		line = __LINE__;
		goto fail;
	}

	gem_sync(device, handle, CPU);
	ptr = (uint32_t *)shm.shmaddr;
	for (x = 0; x < sizeof(x_loc)/sizeof(x_loc[0]); x++)
		for (y = 0; y < sizeof(y_loc)/sizeof(y_loc[0]); y++)
			if (ptr[y_loc[y]*width + x_loc[x]] != pixel) {
				printf("pixel[%d, %d]:%d = %08x\n", x, y, 32, ptr[y_loc[y] * width + x_loc[x]]);
				line = __LINE__;
				goto fail;
			}

	for (x = 0; x < sizeof(x_loc)/sizeof(x_loc[0]); x++)
		for (y = 0; y < sizeof(y_loc)/sizeof(y_loc[0]); y++)
			if (!check_pixmap(dpy, pixmap,
					  x_loc[x], y_loc[y],
					  pixel, 32)) {
				line = __LINE__;
				goto fail;
			}

	if (_x_error_occurred) {
		line = __LINE__;
		goto fail;
	}

out:
	gem_close(device, handle);
	XFreePixmap(dpy, pixmap);
	XShmDetach(dpy, &shm);
	shmdt(shm.shmaddr);
	return fd != -1;

fail:
	printf("%s failed at (%dx%d), line %d\n",
	       __func__, width, height, line);
	fd = -1;
	goto out;
}

static int test_read_after_write(Display *dpy, int device,
				 int width, int height, int depth,
				 int domain)
{
	const uint32_t pixel = 0xffff00ff;
	const int x_loc[] = {0, width/2, width-1};
	const int y_loc[] = {0, height/2, height-1};
	Window root = RootWindow(dpy, DefaultScreen(dpy));
	uint32_t src, dst;
	int src_fd, dst_fd;
	int src_stride, src_size;
	int dst_stride, dst_size;
	Pixmap src_pix, dst_pix;
	struct dri3_fence fence;
	int x, y, bpp;

	_x_error_occurred = 0;

	switch (depth) {
	case 8: bpp = 8; break;
	case 16: bpp = 16; break;
	case 24: bpp = 32; break;
	case 32: bpp = 32; break;
	default: return 0;
	}

	src_stride = width * bpp/8;
	src_size = PAGE_ALIGN(src_stride * height);
	printf("Creating %dx%d (source stride=%d, size=%d, domain=%d)\n",
	       width, height, src_stride, src_size, domain);

	src = gem_create(device, src_size);
	if (!src)
		goto fail;

	if (domain == CPU)
		gem_set_caching(device, src, 1);

	gem_fill(device, src, pixel, src_size, domain);

	src_fd = gem_export(device, src);
	if (src_fd < 0)
		goto fail;

	src_pix = dri3_create_pixmap(dpy, root,
				     width, height, depth,
				     src_fd, bpp, src_stride, src_size);

	for (x = 0; x < sizeof(x_loc)/sizeof(x_loc[0]); x++)
		for (y = 0; y < sizeof(y_loc)/sizeof(y_loc[0]); y++)
			if (!check_pixmap(dpy, src_pix,
					  x_loc[x], y_loc[y],
					  pixel, bpp))
				goto fail;
	close(src_fd);

	dst_pix = XCreatePixmap(dpy, root, width, height, depth);
	if (dri3_create_fence(dpy, dst_pix, &fence))
		goto fail;

	dst_fd = dri3_create_fd(dpy, dst_pix, &dst_stride);
	if (dst_fd < 0)
		goto fail;
	dst_size = lseek(dst_fd, 0, SEEK_END);
	printf("Comparing %dx%d (destination stride=%d, size=%d)\n",
	       width, height, dst_stride, dst_size);
	dst = gem_import(device, dst_fd);
	if (dst == 0)
		goto fail;
	close(dst_fd);

	XCopyArea(dpy, src_pix, dst_pix,
		  get_gc(dpy, dst_pix, depth),
		  0, 0, width, height, 0, 0);
	dri3_fence_sync(dpy, &fence);
	dri3_fence_free(dpy, &fence);

	for (x = 0; x < sizeof(x_loc)/sizeof(x_loc[0]); x++)
		for (y = 0; y < sizeof(y_loc)/sizeof(y_loc[0]); y++)
			if (!check_pixel(device, dst, dst_stride, dst_size,
					 x_loc[x], y_loc[y],
					 pixel, bpp, GTT))
				goto fail;

	XFreePixmap(dpy, dst_pix);
	XFreePixmap(dpy, src_pix);

	gem_close(device, src);
	gem_close(device, dst);

	if (_x_error_occurred)
		goto fail;

	return 0;

fail:
	printf("%s failed at (%dx%d), depth=%d, domain=%d\n",
	       __func__, width, height, depth, domain);
	return 1;
}

static XRenderPictFormat *format_for_depth(Display *dpy, int depth)
{
	switch (depth) {
	case 8: return XRenderFindStandardFormat(dpy, PictStandardA8);
	case 24: return XRenderFindStandardFormat(dpy, PictStandardRGB24);
	case 32: return XRenderFindStandardFormat(dpy, PictStandardARGB32);
	default: assert(0); return NULL;
	}
}

static int test_read(Display *dpy, int device,
		     int width, int height,
		     int domain)
{
	const uint32_t pixel = 0xffff00ff;
	const XRenderColor color = { 0xffff, 0x0000, 0xffff, 0xffff };
	const int x_loc[] = {0, width/2, width-1};
	const int y_loc[] = {0, height/2, height-1};
	Window root = RootWindow(dpy, DefaultScreen(dpy));
	uint32_t dst;
	int dst_stride, dst_size, dst_fd;
	Pixmap src_pix, dst_pix;
	Picture src_pic;
	struct dri3_fence fence;
	int depth = 32, bpp = 32;
	int x, y;

	_x_error_occurred = 0;

	dst_stride = width * bpp/8;
	dst_size = PAGE_ALIGN(dst_stride * height);
	printf("Creating %dx%d (destination stride=%d, size=%d, domain=%d)\n",
	       width, height, dst_stride, dst_size, domain);

	dst = gem_create(device, dst_size);
	if (!dst)
		goto fail;

	if (domain == CPU)
		gem_set_caching(device, dst, 1);

	gem_fill(device, dst, ~pixel, dst_size, domain);

	dst_fd = gem_export(device, dst);
	if (dst_fd < 0)
		goto fail;

	dst_pix = dri3_create_pixmap(dpy, root,
				     width, height, depth,
				     dst_fd, bpp, dst_stride, dst_size);
	XSync(dpy, True);
	if (_x_error_occurred)
		goto fail;
	if (dri3_create_fence(dpy, dst_pix, &fence))
		goto fail;

	src_pix = XCreatePixmap(dpy, root, width, height, depth);
	src_pic = XRenderCreatePicture(dpy, src_pix, format_for_depth(dpy, depth), 0, NULL);
	XRenderFillRectangle(dpy, PictOpSrc, src_pic, &color, 0, 0, width, height);
	XCopyArea(dpy, src_pix, dst_pix,
		  get_gc(dpy, dst_pix, depth),
		  0, 0, width, height, 0, 0);
	dri3_fence_sync(dpy, &fence);
	dri3_fence_free(dpy, &fence);

	for (x = 0; x < sizeof(x_loc)/sizeof(x_loc[0]); x++)
		for (y = 0; y < sizeof(y_loc)/sizeof(y_loc[0]); y++)
			if (!check_pixel(device, dst, dst_stride, dst_size,
					 x_loc[x], y_loc[y],
					 pixel, bpp, domain))
				goto fail;

	XFreePixmap(dpy, dst_pix);
	XRenderFreePicture(dpy, src_pic);
	XFreePixmap(dpy, src_pix);

	gem_close(device, dst);

	if (_x_error_occurred)
		goto fail;

	return 0;

fail:
	printf("%s failed at (%dx%d), depth=%d, domain=%d\n",
	       __func__, width, height, depth, domain);
	return 1;
}

static int test_dup_pixmap(Display *dpy, int device)
{
	const uint32_t pixel = 0xffff00ff;
	const XRenderColor color = { 0xffff, 0x0000, 0xffff, 0xffff };
	const XRenderColor inverse = { 0, 0xffff, 0, 0 };
	int width = 400, height = 400;
	const int x_loc[] = {0, width/2, width-1};
	const int y_loc[] = {0, height/2, height-1};
	Window root = RootWindow(dpy, DefaultScreen(dpy));
	uint32_t handle;
	int stride, size, fd;
	Pixmap src_pix, dst_pix;
	Picture src_pic, dst_pic;
	struct dri3_fence fence;
	int depth = 32, bpp = 32;
	int x, y;

	_x_error_occurred = 0;

	printf("%s: Creating %dx%d pixmap\n", __func__, width, height);
	src_pix = XCreatePixmap(dpy, root, width, height, depth);
	src_pic = XRenderCreatePicture(dpy, src_pix, format_for_depth(dpy, depth), 0, NULL);
	fd = dri3_create_fd(dpy, src_pix, &stride);
	if (fd < 0)
		goto fail;

	size = lseek(fd, 0, SEEK_END);
	handle = gem_import(device, fd);

	printf("%s: Creating duplicate from pixmap exported fd\n", __func__);
	dst_pix = dri3_create_pixmap(dpy, root,
				     width, height, depth,
				     fd, bpp, stride, size);
	dst_pic = XRenderCreatePicture(dpy, dst_pix, format_for_depth(dpy, depth), 0, NULL);
	XSync(dpy, True);
	if (_x_error_occurred)
		goto fail;

	printf("%s: Filling src with %08x, reading dst\n", __func__, pixel);
	XRenderFillRectangle(dpy, PictOpSrc, src_pic, &color, 0, 0, width, height);
	for (x = 0; x < sizeof(x_loc)/sizeof(x_loc[0]); x++)
		for (y = 0; y < sizeof(y_loc)/sizeof(y_loc[0]); y++)
			if (!check_pixmap(dpy, dst_pix,
					  x_loc[x], y_loc[y],
					  pixel, 32))
				goto fail;

	printf("%s: Filling dst with %08x, reading src\n", __func__, ~pixel);
	XRenderFillRectangle(dpy, PictOpSrc, dst_pic, &inverse, 0, 0, width, height);
	for (x = 0; x < sizeof(x_loc)/sizeof(x_loc[0]); x++)
		for (y = 0; y < sizeof(y_loc)/sizeof(y_loc[0]); y++)
			if (!check_pixmap(dpy, dst_pix,
					  x_loc[x], y_loc[y],
					  ~pixel, 32))
				goto fail;

	if (dri3_create_fence(dpy, src_pix, &fence))
		goto fail;

	printf("%s: Filling src with %08x, reading fd\n", __func__, pixel);
	XRenderFillRectangle(dpy, PictOpSrc, src_pic, &color, 0, 0, width, height);
	dri3_fence_sync(dpy, &fence);
	dri3_fence_free(dpy, &fence);
	for (x = 0; x < sizeof(x_loc)/sizeof(x_loc[0]); x++)
		for (y = 0; y < sizeof(y_loc)/sizeof(y_loc[0]); y++)
			if (!check_pixel(device, handle, stride, size,
					 x_loc[x], y_loc[y],
					 pixel, bpp, GTT))
				goto fail;

	printf("%s: Filling fd with %08x, reading src\n", __func__, ~pixel);
	if (gpu_fill(device, handle, width, height, stride, 32, gem_get_tiling(device, handle), ~pixel))
		goto fail;
	for (x = 0; x < sizeof(x_loc)/sizeof(x_loc[0]); x++)
		for (y = 0; y < sizeof(y_loc)/sizeof(y_loc[0]); y++)
			if (!check_pixmap(dpy, src_pix,
					  x_loc[x], y_loc[y],
					  ~pixel, 32))
				goto fail;

	if (dri3_create_fence(dpy, dst_pix, &fence))
		goto fail;

	printf("%s: Filling dst with %08x, reading fd\n", __func__, pixel);
	XRenderFillRectangle(dpy, PictOpSrc, dst_pic, &color, 0, 0, width, height);
	dri3_fence_sync(dpy, &fence);
	dri3_fence_free(dpy, &fence);
	for (x = 0; x < sizeof(x_loc)/sizeof(x_loc[0]); x++)
		for (y = 0; y < sizeof(y_loc)/sizeof(y_loc[0]); y++)
			if (!check_pixel(device, handle, stride, size,
					 x_loc[x], y_loc[y],
					 pixel, bpp, GTT))
				goto fail;

	printf("%s: Filling fd with %08x, reading dst\n", __func__, ~pixel);
	if (gpu_fill(device, handle, width, height, stride, 32, gem_get_tiling(device, handle), ~pixel))
		goto fail;
	for (x = 0; x < sizeof(x_loc)/sizeof(x_loc[0]); x++)
		for (y = 0; y < sizeof(y_loc)/sizeof(y_loc[0]); y++)
			if (!check_pixmap(dpy, dst_pix,
					  x_loc[x], y_loc[y],
					  ~pixel, 32))
				goto fail;

	XRenderFreePicture(dpy, src_pic);
	XFreePixmap(dpy, src_pix);

	if (dri3_create_fence(dpy, dst_pix, &fence))
		goto fail;

	printf("%s: Closed original src, filling dst with %08x, reading fd\n", __func__, pixel);
	XRenderFillRectangle(dpy, PictOpSrc, dst_pic, &color, 0, 0, width, height);
	dri3_fence_sync(dpy, &fence);
	dri3_fence_free(dpy, &fence);
	for (x = 0; x < sizeof(x_loc)/sizeof(x_loc[0]); x++)
		for (y = 0; y < sizeof(y_loc)/sizeof(y_loc[0]); y++)
			if (!check_pixel(device, handle, stride, size,
					 x_loc[x], y_loc[y],
					 pixel, bpp, GTT))
				goto fail;

	XRenderFreePicture(dpy, dst_pic);
	XFreePixmap(dpy, dst_pix);

	gem_close(device, handle);

	if (_x_error_occurred)
		goto fail;

	return 0;

fail:
	printf("%s failed at (%dx%d), depth=%d\n",
	       __func__, width, height, depth);
	return 1;
}

static int test_bad_size(Display *dpy, int device)
{
	Window root = RootWindow(dpy, DefaultScreen(dpy));
	uint32_t src;
	int src_fd;
	Pixmap src_pix;
	int line = -1;

	_x_error_occurred = 0;

	src = gem_create(device, 4096);
	if (!src)
		goto fail;

	src_fd = gem_export(device, src);
	if (src_fd < 0)
		goto fail;

	src_pix = dri3_create_pixmap(dpy, root,
				     16, 16, 32,
				     dup(src_fd), 32, 16*4, 4096);
	line = __LINE__;
	XSync(dpy, True);
	if (_x_error_occurred)
		goto fail;
	XFreePixmap(dpy, src_pix);
	_x_error_occurred = 0;

	src_pix = dri3_create_pixmap(dpy, root,
				     32, 32, 32,
				     dup(src_fd), 32, 32*4, 4096);
	line = __LINE__;
	XSync(dpy, True);
	if (_x_error_occurred)
		goto fail;
	XFreePixmap(dpy, src_pix);
	_x_error_occurred = 0;

	src_pix = dri3_create_pixmap(dpy, root,
				     64, 64, 32,
				     dup(src_fd), 32, 64*4, 4096);
	line = __LINE__;
	XSync(dpy, True);
	if (!_x_error_occurred)
		goto fail;
	_x_error_occurred = 0;

	src_pix = dri3_create_pixmap(dpy, root,
				     64, 64, 32,
				     dup(src_fd), 32, 64*4, 64*64*4);
	line = __LINE__;
	XSync(dpy, True);
	if (!_x_error_occurred)
		goto fail;
	_x_error_occurred = 0;

	src_pix = dri3_create_pixmap(dpy, root,
				     INT16_MAX, INT16_MAX, 8,
				     dup(src_fd), 8, INT16_MAX, UINT32_MAX);
	line = __LINE__;
	XSync(dpy, True);
	if (!_x_error_occurred)
		goto fail;
	_x_error_occurred = 0;

	close(src_fd);
	gem_close(device, src);

	return 0;

fail:
	printf("%s failed at line %d\n", __func__, line);
	return 1;
}

static int test_bad_pitch(Display *dpy, int device)
{
	Window root = RootWindow(dpy, DefaultScreen(dpy));
	uint32_t src;
	int src_fd;
	Pixmap src_pix;
	int line = -1;

	_x_error_occurred = 0;

	src = gem_create(device, 4096);
	if (!src)
		goto fail;

	src_fd = gem_export(device, src);
	if (src_fd < 0)
		goto fail;

	src_pix = dri3_create_pixmap(dpy, root,
				     16, 16, 32,
				     dup(src_fd), 32, 16*4, 4096);
	line = __LINE__;
	XSync(dpy, True);
	if (_x_error_occurred)
		goto fail;
	XFreePixmap(dpy, src_pix);
	_x_error_occurred = 0;

	src_pix = dri3_create_pixmap(dpy, root,
				     256, 2, 32,
				     dup(src_fd), 32, 256*4, 4096);
	line = __LINE__;
	XSync(dpy, True);
	if (_x_error_occurred)
		goto fail;
	XFreePixmap(dpy, src_pix);
	_x_error_occurred = 0;

	src_pix = dri3_create_pixmap(dpy, root,
				     256, 2, 32,
				     dup(src_fd), 32, 256, 4096);
	line = __LINE__;
	XSync(dpy, True);
	if (!_x_error_occurred)
		goto fail;
	_x_error_occurred = 0;

	src_pix = dri3_create_pixmap(dpy, root,
				     256, 2, 32,
				     dup(src_fd), 32, 16384, 4096);
	line = __LINE__;
	XSync(dpy, True);
	if (!_x_error_occurred)
		goto fail;
	_x_error_occurred = 0;

	src_pix = dri3_create_pixmap(dpy, root,
				     256, 2, 32,
				     dup(src_fd), 32, 1023, 4096);
	line = __LINE__;
	XSync(dpy, True);
	if (!_x_error_occurred)
		goto fail;
	_x_error_occurred = 0;

	src_pix = dri3_create_pixmap(dpy, root,
				     256, 2, 32,
				     dup(src_fd), 32, 1025, 4096);
	line = __LINE__;
	XSync(dpy, True);
	if (!_x_error_occurred)
		goto fail;
	_x_error_occurred = 0;

	close(src_fd);
	gem_close(device, src);

	return 0;

fail:
	printf("%s failed at line %d\n", __func__, line);
	return 1;
}

static int
_check_error_handler(Display     *display,
		     XErrorEvent *event)
{
	printf("X11 error from display %s, serial=%ld, error=%d, req=%d.%d\n",
	       DisplayString(display),
	       event->serial,
	       event->error_code,
	       event->request_code,
	       event->minor_code);
	_x_error_occurred++;
	return False; /* ignored */
}

int main(void)
{
	Display *dpy;
	int device;
	int error = 0;

	dpy = XOpenDisplay(NULL);
	if (dpy == NULL)
		return 77;

	if (DefaultDepth(dpy, DefaultScreen(dpy)) != 24)
		return 77;

	XSetErrorHandler(_check_error_handler);

	device = dri3_open(dpy);
	if (device < 0)
		return 127;

	if (!is_intel(device))
		return 77;

	printf("Opened Intel DRI3 device\n");

	error += test_bad_size(dpy, device);
	error += test_bad_pitch(dpy, device);

	error += test_shm(dpy, device, 400, 300);
	error += test_shm(dpy, device, 300, 400);

	error += test_read(dpy, device, 400, 200, GTT);
	error += test_read(dpy, device, 4000, 20, GTT);
	error += test_read(dpy, device, 16000, 10, GTT);
	error += test_read(dpy, device, 30000, 10, GTT);

	error += test_read(dpy, device, 200, 400, GTT);
	error += test_read(dpy, device, 20, 4000, GTT);
	error += test_read(dpy, device, 16, 16000, GTT);
	error += test_read(dpy, device, 16, 30000, GTT);

	error += test_read(dpy, device, 400, 200, CPU);
	error += test_read(dpy, device, 4000, 20, CPU);
	error += test_read(dpy, device, 16000, 10, CPU);
	error += test_read(dpy, device, 30000, 10, CPU);

	error += test_read(dpy, device, 200, 400, CPU);
	error += test_read(dpy, device, 20, 4000, CPU);
	error += test_read(dpy, device, 16, 16000, CPU);
	error += test_read(dpy, device, 16, 30000, CPU);

	error += test_read_after_write(dpy, device, 400, 200, 24, GTT);
	error += test_read_after_write(dpy, device, 4000, 20, 24, GTT);
	error += test_read_after_write(dpy, device, 16000, 10, 24, GTT);
	error += test_read_after_write(dpy, device, 30000, 10, 24, GTT);
	error += test_read_after_write(dpy, device, 30000, 10, 8, GTT);

	error += test_read_after_write(dpy, device, 200, 400, 24, GTT);
	error += test_read_after_write(dpy, device, 20, 4000, 24, GTT);
	error += test_read_after_write(dpy, device, 16, 16000, 24, GTT);
	error += test_read_after_write(dpy, device, 16, 30000, 24, GTT);

	error += test_read_after_write(dpy, device, 400, 200, 24, CPU);
	error += test_read_after_write(dpy, device, 4000, 20, 24, CPU);
	error += test_read_after_write(dpy, device, 16000, 10, 24, CPU);
	error += test_read_after_write(dpy, device, 30000, 10, 24, CPU);
	error += test_read_after_write(dpy, device, 30000, 10, 8, CPU);

	error += test_read_after_write(dpy, device, 200, 400, 24, CPU);
	error += test_read_after_write(dpy, device, 20, 4000, 24, CPU);
	error += test_read_after_write(dpy, device, 16, 16000, 24, CPU);
	error += test_read_after_write(dpy, device, 16, 30000, 24, CPU);

	error += test_dup_pixmap(dpy, device);

	return !!error;
}
