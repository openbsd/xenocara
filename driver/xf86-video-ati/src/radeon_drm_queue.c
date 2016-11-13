/*
 * Copyright © 2007 Red Hat, Inc.
 * Copyright © 2015 Advanced Micro Devices, Inc.
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

#include <xorg-server.h>

#include "radeon.h"
#include "radeon_drm_queue.h"
#include "radeon_list.h"


struct radeon_drm_queue_entry {
    struct xorg_list list;
    uint64_t id;
    uintptr_t seq;
    void *data;
    ClientPtr client;
    xf86CrtcPtr crtc;
    radeon_drm_handler_proc handler;
    radeon_drm_abort_proc abort;
};

static int radeon_drm_queue_refcnt;
static struct xorg_list radeon_drm_queue;
static uintptr_t radeon_drm_queue_seq;


/*
 * Handle a DRM event
 */
void
radeon_drm_queue_handler(int fd, unsigned int frame, unsigned int sec,
			 unsigned int usec, void *user_ptr)
{
	uintptr_t seq = (uintptr_t)user_ptr;
	struct radeon_drm_queue_entry *e, *tmp;

	xorg_list_for_each_entry_safe(e, tmp, &radeon_drm_queue, list) {
		if (e->seq == seq) {
			xorg_list_del(&e->list);
			if (e->handler)
				e->handler(e->crtc, frame,
					   (uint64_t)sec * 1000000 + usec,
					   e->data);
			else
				e->abort(e->crtc, e->data);
			free(e);
			break;
		}
	}
}

/*
 * Enqueue a potential drm response; when the associated response
 * appears, we've got data to pass to the handler from here
 */
uintptr_t
radeon_drm_queue_alloc(xf86CrtcPtr crtc, ClientPtr client,
		       uint64_t id, void *data,
		       radeon_drm_handler_proc handler,
		       radeon_drm_abort_proc abort)
{
    struct radeon_drm_queue_entry *e;

    e = calloc(1, sizeof(struct radeon_drm_queue_entry));
    if (!e)
	return NULL;

    if (!radeon_drm_queue_seq)
	radeon_drm_queue_seq = 1;
    e->seq = radeon_drm_queue_seq++;
    e->client = client;
    e->crtc = crtc;
    e->id = id;
    e->data = data;
    e->handler = handler;
    e->abort = abort;

    xorg_list_add(&e->list, &radeon_drm_queue);

    return e->seq;
}

/*
 * Abort one queued DRM entry, removing it
 * from the list, calling the abort function and
 * freeing the memory
 */
static void
radeon_drm_abort_one(struct radeon_drm_queue_entry *e)
{
    xorg_list_del(&e->list);
    e->abort(e->crtc, e->data);
    free(e);
}

/*
 * Abort drm queue entries for a client
 *
 * NOTE: This keeps the entries in the list until the DRM event arrives,
 * but then it calls the abort functions instead of the handler
 * functions.
 */
void
radeon_drm_abort_client(ClientPtr client)
{
    struct radeon_drm_queue_entry *e;

    xorg_list_for_each_entry(e, &radeon_drm_queue, list) {
	if (e->client == client)
	    e->handler = NULL;
    }
}

/*
 * Abort specific drm queue entry
 */
void
radeon_drm_abort_entry(uintptr_t seq)
{
    struct radeon_drm_queue_entry *e, *tmp;

    xorg_list_for_each_entry_safe(e, tmp, &radeon_drm_queue, list) {
	if (e->seq == seq) {
	    radeon_drm_abort_one(e);
	    break;
	}
    }
}

/*
 * Abort specific drm queue entry by ID
 */
void
radeon_drm_abort_id(uint64_t id)
{
    struct radeon_drm_queue_entry *e, *tmp;

    xorg_list_for_each_entry_safe(e, tmp, &radeon_drm_queue, list) {
	if (e->id == id) {
	    radeon_drm_abort_one(e);
	    break;
	}
    }
}

/*
 * Initialize the DRM event queue
 */
void
radeon_drm_queue_init()
{
    if (radeon_drm_queue_refcnt++)
	return;

    xorg_list_init(&radeon_drm_queue);
}

/*
 * Deinitialize the DRM event queue
 */
void
radeon_drm_queue_close(ScrnInfoPtr scrn)
{
    struct radeon_drm_queue_entry *e, *tmp;

    xorg_list_for_each_entry_safe(e, tmp, &radeon_drm_queue, list) {
	if (e->crtc->scrn == scrn)
	    radeon_drm_abort_one(e);
    }

    radeon_drm_queue_refcnt--;
}
