/*
 * Copyright 2008 Tungsten Graphics, Inc., Cedar Park, Texas.
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
 *
 * Author: Alan Hourihane <alanh@tungstengraphics.com>
 * Author: Jakob Bornecrantz <wallbraker@gmail.com>
 *
 */

#include "xorg-server.h"
#include <xf86.h>
#include <xf86i2c.h>
#include <xf86Crtc.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#include "vmwgfx_driver.h"

struct output_private
{
    drmModeConnectorPtr drm_connector;

    int c;

    Bool is_implicit;
};

static const char *output_enum_list[] = {
    "Unknown",
    "VGA",
    "DVI",
    "DVI",
    "DVI",
    "Composite",
    "SVIDEO",
    "LVDS",
    "CTV",
    "DIN",
    "DP",
    "HDMI",
    "HDMI",
    "TV",
    "EDP",
    "Virtual",
};

static void
output_create_resources(xf86OutputPtr output)
{
#ifdef RANDR_12_INTERFACE
#endif /* RANDR_12_INTERFACE */
}

static void
output_dpms(xf86OutputPtr output, int mode)
{
}

static xf86OutputStatus
output_detect(xf86OutputPtr output)
{
    modesettingPtr ms = modesettingPTR(output->scrn);
    struct output_private *priv = output->driver_private;
    drmModeConnectorPtr drm_connector;
    xf86OutputStatus status;

    drm_connector = drmModeGetConnector(ms->fd, priv->drm_connector->connector_id);
    if (drm_connector) {
	drmModeFreeConnector(priv->drm_connector);
	priv->drm_connector = drm_connector;
    } else {
	drm_connector = priv->drm_connector;
    }

    switch (drm_connector->connection) {
    case DRM_MODE_CONNECTED:
	status = XF86OutputStatusConnected;
	break;
    case DRM_MODE_DISCONNECTED:
	status = XF86OutputStatusDisconnected;
	break;
    default:
	status = XF86OutputStatusUnknown;
    }

    return status;
}

static DisplayModePtr
output_get_modes(xf86OutputPtr output)
{
    struct output_private *priv = output->driver_private;
    drmModeConnectorPtr drm_connector = priv->drm_connector;
    drmModeModeInfoPtr drm_mode = NULL;
    DisplayModePtr modes = NULL, mode = NULL;
    int i;

    for (i = 0; i < drm_connector->count_modes; i++) {
	drm_mode = &drm_connector->modes[i];
	if (drm_mode) {
	    mode = calloc(1, sizeof(DisplayModeRec));
	    if (!mode)
		continue;
	    mode->Clock = drm_mode->clock;
	    mode->HDisplay = drm_mode->hdisplay;
	    mode->HSyncStart = drm_mode->hsync_start;
	    mode->HSyncEnd = drm_mode->hsync_end;
	    mode->HTotal = drm_mode->htotal;
	    mode->VDisplay = drm_mode->vdisplay;
	    mode->VSyncStart = drm_mode->vsync_start;
	    mode->VSyncEnd = drm_mode->vsync_end;
	    mode->VTotal = drm_mode->vtotal;
	    mode->Flags = drm_mode->flags;
	    mode->HSkew = drm_mode->hskew;
	    mode->VScan = drm_mode->vscan;
	    mode->VRefresh = xf86ModeVRefresh(mode);
	    mode->Private = (void *)drm_mode;
	    mode->type = 0;
	    if (drm_mode->type & DRM_MODE_TYPE_PREFERRED)
		mode->type |= M_T_PREFERRED;
	    if (drm_mode->type & DRM_MODE_TYPE_DRIVER)
		mode->type |= M_T_DRIVER;
	    xf86SetModeDefaultName(mode);
	    modes = xf86ModesAdd(modes, mode);
	    xf86PrintModeline(0, mode);
	}
    }

    return modes;
}

static int
output_mode_valid(xf86OutputPtr output, DisplayModePtr pMode)
{
  //    modesettingPtr ms = modesettingPTR(output->scrn);
    //    CustomizerPtr cust = ms->cust;

#if 0
    if (cust && cust->winsys_check_fb_size &&
	!cust->winsys_check_fb_size(cust, pMode->HDisplay *
				    output->scrn->bitsPerPixel / 8,
				    pMode->VDisplay))
	return MODE_BAD;
#endif
    return MODE_OK;
}

#ifdef RANDR_12_INTERFACE
static Bool
output_set_property(xf86OutputPtr output, Atom property, RRPropertyValuePtr value)
{
    return TRUE;
}
#endif /* RANDR_12_INTERFACE */

#ifdef RANDR_13_INTERFACE
static Bool
output_get_property(xf86OutputPtr output, Atom property)
{
    return TRUE;
}
#endif /* RANDR_13_INTERFACE */

static void
output_destroy(xf86OutputPtr output)
{
    struct output_private *priv = output->driver_private;
    drmModeFreeConnector(priv->drm_connector);
    free(priv);
    output->driver_private = NULL;
}

static const xf86OutputFuncsRec output_funcs = {
    .create_resources = output_create_resources,
#ifdef RANDR_12_INTERFACE
    .set_property = output_set_property,
#endif
#ifdef RANDR_13_INTERFACE
    .get_property = output_get_property,
#endif
    .dpms = output_dpms,
    .detect = output_detect,

    .get_modes = output_get_modes,
    .mode_valid = output_mode_valid,
    .destroy = output_destroy,
};

/**
 * vmwgfx_output_explicit_overlap -- Check for explicit output overlaps
 *
 * This function returns TRUE iff the bounding box in screen space of an
 * exlplicit output overlaps the bounding box in screen space of any other
 * output.
 */
Bool
vmwgfx_output_explicit_overlap(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86OutputPtr output;
    ScreenPtr pScreen = pScrn->pScreen;
    RegionRec output_union;
    RegionRec cur_output;
    RegionRec result;
    struct output_private *priv;
    xf86CrtcPtr crtc;
    Bool overlap = FALSE;
    int i;

    (void) pScreen;
    REGION_NULL(pScreen, &output_union);
    REGION_NULL(pScreen, &cur_output);
    REGION_NULL(pScreen, &result);

    /*
     * Collect a region of implicit outputs. These may overlap.
     */
    for (i = 0; i < config->num_output; i++) {
	output = config->output[i];
	priv = output->driver_private;
	crtc = output->crtc;

	if (!crtc || !crtc->enabled || !priv->is_implicit)
	    continue;

	REGION_RESET(pScreen, &cur_output, &crtc->bounds);
	REGION_UNION(pScreen, &output_union, &output_union, &cur_output);
    }

    /*
     * Explicit outputs may not overlap any other output.
     */
    for (i = 0; i < config->num_output; i++) {
	output = config->output[i];
	priv = output->driver_private;
	crtc = output->crtc;

	if (!crtc || !crtc->enabled || priv->is_implicit)
	    continue;

	REGION_RESET(pScreen, &cur_output, &crtc->bounds);
	REGION_NULL(pScreen, &result);
	REGION_INTERSECT(pScreen, &result, &output_union, &cur_output);
	overlap = REGION_NOTEMPTY(vsaa->pScreen, &result);
	if (overlap)
	    break;

	REGION_UNION(pScreen, &output_union, &output_union, &cur_output);
    }

    REGION_UNINIT(pScreen, &output_union);
    REGION_UNINIT(pScreen, &cur_output);
    REGION_UNINIT(pScreen, &result);

    return overlap;
}

void
xorg_output_init(ScrnInfoPtr pScrn)
{
    modesettingPtr ms = modesettingPTR(pScrn);
    xf86OutputPtr output;
    drmModeResPtr res;
    drmModeConnectorPtr drm_connector = NULL;
    drmModeEncoderPtr drm_encoder = NULL;
    struct output_private *priv;
    char name[32];
    int c, p;

    res = drmModeGetResources(ms->fd);
    if (res == 0) {
	DRV_ERROR("Failed drmModeGetResources\n");
	return;
    }

    for (c = 0; c < res->count_connectors; c++) {
	Bool is_implicit = TRUE;

	drm_connector = drmModeGetConnector(ms->fd, res->connectors[c]);
	if (!drm_connector)
	    goto out;


	for (p = 0; p < drm_connector->count_props; p++) {
	    drmModePropertyPtr prop;

	    prop = drmModeGetProperty(ms->fd, drm_connector->props[p]);

	    if (prop) {

#if 0
	      /*
	       * Disabled until we sort out what the interface should
	       * look like.
	       */

		if (strcmp(prop->name, "implicit placement") == 0) {
		    drmModeConnectorSetProperty(ms->fd,
						drm_connector->connector_id,
						prop->prop_id,
						0);
		    is_implicit = FALSE;
		}
#endif
		drmModeFreeProperty(prop);
	    }
	}

	if (drm_connector->connector_type >=
	    sizeof(output_enum_list) / sizeof(output_enum_list[0]))
	    drm_connector->connector_type = 0;

	snprintf(name, 32, "%s%d",
		 output_enum_list[drm_connector->connector_type],
		 drm_connector->connector_type_id);


	priv = calloc(sizeof(*priv), 1);
	if (!priv) {
	    continue;
	}

	output = xf86OutputCreate(pScrn, &output_funcs, name);
	if (!output) {
	    free(priv);
	    continue;
	}

	priv->is_implicit = is_implicit;

	drm_encoder = drmModeGetEncoder(ms->fd, drm_connector->encoders[0]);
	if (drm_encoder) {
	    output->possible_crtcs = drm_encoder->possible_crtcs;
	    output->possible_clones = drm_encoder->possible_clones;
	} else {
	    output->possible_crtcs = 0;
	    output->possible_clones = 0;
	}
	priv->c = c;
	priv->drm_connector = drm_connector;
	output->driver_private = priv;
	output->subpixel_order = SubPixelHorizontalRGB;
	output->interlaceAllowed = FALSE;
	output->doubleScanAllowed = FALSE;
    }

  out:
    drmModeFreeResources(res);
}

unsigned
xorg_output_get_id(xf86OutputPtr output)
{
    struct output_private *priv = output->driver_private;
    return priv->drm_connector->connector_id;
}

/* vim: set sw=4 ts=8 sts=4: */
