/**************************************************************************
 * Copyright © 2016 VMware, Inc., Palo Alto, CA., USA
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_LIBUDEV
#include "vmwgfx_driver.h"
#include <xf86Crtc.h>
#include "vmwgfx_rr_inlines.h"
#include "../src/common_compat.h"

#ifndef X_DEBUG
#define X_DEBUG X_NOTICE
#endif

/**
 * struct vmwgfx_layout_box - Struct representing a GUI layout rect
 *
 * @x: X value of the origin.
 * @y: Y value of the origin.
 * @width: Width of the rect.
 * @height: Height of the rect.
 */
struct vmwgfx_layout_box {
    int x, y, width, height;
};

/**
 * struct vmwgfx_layout - Struct representing a complete GUI layout
 *
 * @connected: Number of connected outputs.
 * @root_width: Width of full desktop.
 * @root_height: Height of full desktop.
 * @boxes: Array of GUI layout rects.
 */
struct vmwgfx_layout {
    int connected;
    int root_width;
    int root_height;
    struct vmwgfx_layout_box boxes[];
};

/**
 * vmwgfx_layout_debug - Log debug info of a layout struct.
 *
 * @pScrn: ScrnInfoPtr: Pointer to the ScrnInfo struct for the screen the
 * layout should be logged for.
 * @l1: Pointer to a valid struct vmwgfx_layout.
 */
static void
vmwgfx_layout_debug(ScrnInfoPtr pScrn, const struct vmwgfx_layout *l1)
{
    int i;

    xf86DrvMsg(pScrn->scrnIndex, X_DEBUG, "New layout.\n");
    for (i = 0; i < l1->connected; ++i)
	xf86DrvMsg(pScrn->scrnIndex, X_DEBUG,
		   "%d: %d %d %d %d\n", i, l1->boxes[i].x,
		   l1->boxes[i].y, l1->boxes[i].width, l1->boxes[i].height);
    xf86DrvMsg(pScrn->scrnIndex, X_DEBUG, "\n");
}

/**
 * vmwgfx_layouts_equal - Determine whether two layouts are equal.
 *
 * @l1: Pointer to the first struct vmwgfx_layout.
 * @l2: Pointer to the second struct vmwgfx_layout.
 *
 * Returns: TRUE if the layouts are equal. FALSE otherwise.
 */
static Bool
vmwgfx_layouts_equal(const struct vmwgfx_layout *l1,
		     const struct vmwgfx_layout *l2)
{
    if (l1->connected != l2->connected)
	return FALSE;

    if (!l1->connected)
	return TRUE;

    return !memcmp(l1->boxes, l2->boxes,
		   l1->connected*sizeof(struct vmwgfx_layout_box));
}

/**
 * vmwgfx_layout_from_kms - Construct a struct vmwgfx_layout from KMS info.
 *
 * @pScrn: Pointer to a ScrnInfo struct.
 *
 * Returns: A pointer to a newly allocated struct vmwgfx_layout if
 * successful. NULL otherwise.
 */
struct vmwgfx_layout *
vmwgfx_layout_from_kms(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i, connected;
    struct vmwgfx_layout *layout;
    size_t size;
    int min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;

    for (i = 0; i < config->num_output; ++i) {
	xf86OutputPtr output = config->output[i];

	if (!vmwgfx_output_has_origin(output))
	    return NULL;

	if (output->status != XF86OutputStatusConnected)
	    break;
    }
    connected = i;

    size = offsetof(struct vmwgfx_layout, boxes) +
	connected * sizeof(struct vmwgfx_layout_box);
    layout = calloc(1, size);
    if (!layout)
	return NULL;

    layout->connected = connected;
    for (i = 0; i < connected; ++i) {
	struct vmwgfx_layout_box *box = &layout->boxes[i];
	xf86OutputPtr output = config->output[i];
	DisplayModePtr mode = output->probed_modes;

	if (mode == NULL) {
	    free(layout);
	    return NULL;
	}

	vmwgfx_output_origin(output, &box->x, &box->y);
	box->width = output->probed_modes->HDisplay;
	box->height = output->probed_modes->VDisplay;
	min_x = min(min_x, box->x);
	min_y = min(min_y, box->y);
	max_x = max(max_x, box->x + box->width);
	max_y = max(max_y, box->y + box->height);
    }

    layout->root_width = max_x;
    layout->root_height = max_y;

    return layout;
}

/**
 * vmwgfx_layout_configuration - Set up the screen modesetting configuration
 * from a struct vmwgfx_layout.
 *
 * @pScrn: Pointer to a ScrnInfo struct.
 * @layout: Layout to use for the new configuration.
 *
 * Sets up a new modesetting configuration. Note that the configuration needs
 * to be committed using xf86SetDesiredModes().
 */
void
vmwgfx_layout_configuration(ScrnInfoPtr pScrn, struct vmwgfx_layout *layout)
{
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);
    struct vmwgfx_layout_box *box;
    xf86OutputPtr output;
    xf86CrtcPtr crtc;
    int i, j;

    for (j = 0; j < config->num_crtc; ++j) {
	crtc = config->crtc[j];
	crtc->enabled = FALSE;
    }

    for (i = 0, box = layout->boxes; i < config->num_output; ++i, ++box) {
	output = config->output[i];
	output->crtc = NULL;
	if (i >= layout->connected)
	    continue;

	for (j = 0; j < config->num_crtc; ++j) {
	    crtc = config->crtc[j];
	    if (!crtc->enabled && (output->possible_crtcs & (1 << j))) {
		crtc->enabled = TRUE;
		output->crtc = crtc;
		break;
	    }
	}

	if (!output->crtc)
	    continue;

	crtc = output->crtc;
	xf86SaveModeContents(&crtc->desiredMode, output->probed_modes);
	crtc->desiredRotation = RR_Rotate_0;
	crtc->desiredX = box->x;
	crtc->desiredY = box->y;
	crtc->desiredTransformPresent = FALSE;
    }
}

/**
 * vmwgfx_layout_handler - Obtain and set a new layout.
 *
 * @pScrn: Pointer to a ScrnInfo struct.
 *
 * Obtains a new layout from DRM. If the layout differs from the current one,
 * Try to set the new layout. If that fails, (typically due to root pixmap
 * resizing issues) try hard to revert to the old layout. Finally
 * update RandR in a way that tries to block racing display managers
 * from setting up the layout in a different way.
 */
void
vmwgfx_layout_handler(ScrnInfoPtr pScrn)
{

    ScreenPtr pScreen = xf86ScrnToScreen(pScrn);
    modesettingPtr ms = modesettingPTR(pScrn);
    struct vmwgfx_layout *layout;

    if (!pScreen)
	return;

    /*
     * Construct a layout from the new information and determine whether we
     * need to take action
     */
    layout = vmwgfx_layout_from_kms(pScrn);
    if (layout && (!ms->layout || !vmwgfx_layouts_equal(ms->layout, layout))) {
	vmwgfx_layout_debug(pScrn, layout);
	vmwgfx_outputs_off(pScrn);
	xf86DisableUnusedFunctions(pScrn);
	if (!vmwgfx_rr_screen_set_size(pScreen, layout->root_width,
				       layout->root_height)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Resizing screen failed.\n");
	    vmwgfx_outputs_on(pScrn);
	    free(layout);
	} else {
	    vmwgfx_layout_configuration(pScrn, layout);
	    if (ms->layout)
	      free(ms->layout);
	    ms->layout = layout;
	}
	xf86SetDesiredModes(pScrn);
	vmwgfx_notify_rr(pScreen);
    } else if (layout) {
	free(layout);
    }
}

#endif /* HAVE_LIBUDEV */
