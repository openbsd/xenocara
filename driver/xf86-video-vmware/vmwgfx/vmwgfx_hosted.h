/*
 * Copyright 2013 VMWare, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 * Note: "Hosted" is a term stolen from the xf86-video-intel driver.
 */

#ifndef _VMWGFX_HOSTED_H
#define _VMWGFX_HOSTED_H

#include <xorg-server.h>
#include <xf86.h>


/**
 * struct vmwgfx_hosted - hosting environment private information.
 *
 * This struct is completely opaque to callers and should be defined
 * by the hosting environment.
 */
struct vmwgfx_hosted;

/**
 * struct vmwgfx-hosted-driver - Driver for environments that we can run
 * hosted under.
 *
 * @create: Initialize and create an opaque struct vmwgfx_hosted with
 * environment private information. Should return NULL on failure.
 * @destroy: Undo what's done in @create.
 * @drm_fd: Return a file descriptor opened to DRM.
 * @pre_init: Callback from vmwgfx preInit.
 * @screen_init: Callback from vmwgfx screenInit.
 * @screen_close: Callback from vmwgfx screenClose.
 * @post_damage: Callback from vmwgfx blockHandler. This callback should
 * instruct the hosting environment about damaged windows.
 * @dri_auth: Authenticate a dri client.
 */
struct vmwgfx_hosted_driver {
    struct vmwgfx_hosted *(*create)(ScrnInfoPtr);
    void (*destroy)(struct vmwgfx_hosted *);
    int (*drm_fd)(struct vmwgfx_hosted *, const struct pci_device *);
    Bool (*pre_init)(struct vmwgfx_hosted *, int);
    Bool (*screen_init)(struct vmwgfx_hosted *, ScreenPtr);
    void (*screen_close)(struct vmwgfx_hosted *);
    void (*post_damage)(struct vmwgfx_hosted *);
    int (*dri_auth)(struct vmwgfx_hosted *, ClientPtr client, uint32_t magic);
};

extern const struct vmwgfx_hosted_driver *vmwgfx_hosted_detect(void);
extern void vmwgfx_hosted_modify_flags(uint32_t *flags);

/**
 * vmwgfx_is_hosted - Check whether we're running hosted.
 *
 * @driver: Pointer to a struct vmwgfx_hosted_driver as returned by
 * vmwgfx_hosted_detect()
 *
 */
static inline Bool
vmwgfx_is_hosted(const struct vmwgfx_hosted_driver *driver)
{
    return (driver != NULL);
}

/**
 * vmwgfx_hosted_create - Set up and initialize a struct vmwgfx_hosted
 *
 * @driver: Pointer to a struct vmwgfx_hosted_driver as returned by
 * vmwgfx_hosted_detect()
 * @pScrn: Pointer to a ScrnInfo structure, that has not been populated yet.
 *
 * Convenience wrapper around the hosted_driver function.
 */
static inline struct vmwgfx_hosted*
vmwgfx_hosted_create(const struct vmwgfx_hosted_driver *driver,
		     ScrnInfoPtr pScrn)
{
    if (!vmwgfx_is_hosted(driver))
	return NULL;

    return driver->create(pScrn);
}

/**
 * vmwgfx_hosted_destroy - free a struct vmwgfx_hosted and take down
 * hosted environment.
 *
 * @driver: Pointer to a struct vmwgfx_hosted_driver as returned by
 * vmwgfx_hosted_detect()
 * @hosted: Pointer to a struct vmwgfx_hosted, as returned by
 * vmwgfx_hosted_create()
 *
 * Convenience wrapper around the hosted_driver function.
 */
static inline void
vmwgfx_hosted_destroy(const struct vmwgfx_hosted_driver *driver,
		      struct vmwgfx_hosted *hosted)
{
    if (!vmwgfx_is_hosted(driver))
	return;

    driver->destroy(hosted);
}

/**
 * vmwgfx_hosted_drm_fd - Return a drm file descriptor
 *
 * @driver: Pointer to a struct vmwgfx_hosted_driver as returned by
 * vmwgfx_hosted_detect()
 * @hosted: Pointer to a struct vmwgfx_hosted, as returned by
 * vmwgfx_hosted_create()
 * @pci: Pointer to a valid struct pci_device, describing our device.
 *
 * Convenience wrapper around the hosted_driver function. Returns an
 * invalid file descriptor if we're not hosted.
 */
static inline int
vmwgfx_hosted_drm_fd(const struct vmwgfx_hosted_driver *driver,
		     struct vmwgfx_hosted *hosted,
		     const struct pci_device *pci)
{
    if (!vmwgfx_is_hosted(driver))
	return -1;

    return driver->drm_fd(hosted, pci);
}

/**
 * vmwgfx_hosted_pre_init - Initiate preInit callback.
 *
 * @driver: Pointer to a struct vmwgfx_hosted_driver as returned by
 * vmwgfx_hosted_detect()
 * @hosted: Pointer to a struct vmwgfx_hosted, as returned by
 * vmwgfx_hosted_create()
 * @flags: Flags passed to the vmwgfx preInit function
 *
 * Convenience wrapper around the hosted_driver function. Returns TRUE
 * (success) if not hosted.
 */
static inline Bool
vmwgfx_hosted_pre_init(const struct vmwgfx_hosted_driver *driver,
		       struct vmwgfx_hosted *hosted, int flags)
{
    if (!vmwgfx_is_hosted(driver))
	return TRUE;

    return driver->pre_init(hosted, flags);
}

/**
 * vmwgfx_hosted_screen_init - Initiate screenInit callback.
 *
 * @driver: Pointer to a struct vmwgfx_hosted_driver as returned by
 * vmwgfx_hosted_detect()
 * @hosted: Pointer to a struct vmwgfx_hosted, as returned by
 * vmwgfx_hosted_create()
 * @pScreen: ScreenPtr identifying the screen we're setting up.
 *
 * Convenience wrapper around the hosted_driver function. Returns TRUE
 * (success) if not hosted.
 */
static inline Bool
vmwgfx_hosted_screen_init(const struct vmwgfx_hosted_driver *driver,
			  struct vmwgfx_hosted *hosted, ScreenPtr pScreen)
{
    if (!vmwgfx_is_hosted(driver))
	return TRUE;

    return driver->screen_init(hosted, pScreen);
}

/**
 * vmwgfx_hosted_screen_close - Initiate screenClose callback.
 *
 * @driver: Pointer to a struct vmwgfx_hosted_driver as returned by
 * vmwgfx_hosted_detect()
 * @hosted: Pointer to a struct vmwgfx_hosted, as returned by
 * vmwgfx_hosted_create()
 *
 * Convenience wrapper around the hosted_driver function.
 * Does nothing if not hosted.
 */
static inline void
vmwgfx_hosted_screen_close(const struct vmwgfx_hosted_driver *driver,
			   struct vmwgfx_hosted *hosted)
{
    if (!vmwgfx_is_hosted(driver))
	return;

    driver->screen_close(hosted);
}

/**
 * vmwgfx_hosted_post_damage - Inform the hosting environment about
 * recent rendering
 *
 * @driver: Pointer to a struct vmwgfx_hosted_driver as returned by
 * vmwgfx_hosted_detect()
 * @hosted: Pointer to a struct vmwgfx_hosted, as returned by
 * vmwgfx_hosted_create()
 *
 * Convenience wrapper around the hosted_driver function.
 * Does nothing if not hosted.
 */
static inline void
vmwgfx_hosted_post_damage(const struct vmwgfx_hosted_driver *driver,
			  struct vmwgfx_hosted *hosted)
{
    if (!vmwgfx_is_hosted(driver))
	return;

    driver->post_damage(hosted);
}

/**
 * vmwgfx_hosted_dri_auth - Ask the hosting environment to authenticate a
 * dri client.
 *
 * @driver: Pointer to a struct vmwgfx_hosted_driver as returned by
 * vmwgfx_hosted_detect()
 * @hosted: Pointer to a struct vmwgfx_hosted, as returned by
 * vmwgfx_hosted_create()
 * @client: The client to be authenticated
 * @magic: The drm magic used for authentication
 *
 * Convenience wrapper around the hosted_driver function.
 * Does nothing if not hosted.
 */
static inline int
vmwgfx_hosted_dri_auth(const struct vmwgfx_hosted_driver *driver,
		       struct vmwgfx_hosted *hosted,
		       ClientPtr client,
		       uint32_t magic)
{
    return driver->dri_auth(hosted, client, magic);
}
#endif /* _VMWGFX_HOSTED_H */
