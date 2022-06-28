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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xorg-server.h"
#include <xf86drm.h>
#include <xf86str.h>
#include <randrstr.h>
#include <xf86Crtc.h>
#include <X11/Xatom.h>
#include <dix.h>

#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#include "vmwgfx_driver.h"

/**
 * struct output_prop - Structure representing an output property.
 *
 * @mode_prop: Pointer to the corresponding drmModeProperty or
 * NULL if the property doesn't have a DRM counterpart.
 * @value: The value of the property.
 * @num_atoms: Number of atoms in the @atoms array.
 * @atoms: Atoms used by this poperty.
 * @index: Index into the drm connector prop_values array.
 */
struct output_prop {
    drmModePropertyPtr mode_prop;
    uint64_t value;
    int num_atoms;
    Atom *atoms;
    int index;
};

struct output_private
{
    drmModeConnectorPtr drm_connector;
    int num_props;
    struct output_prop *props;
    int c;
    int suggested_x;
    int suggested_y;
    int implicit_placement;
    xf86CrtcPtr saved_crtc;
    Bool saved_crtc_enabled;
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

/**
 * vmwgfx_output_has_origin - Whether we've detected layout info on the DRM
 * connector.
 *
 * @output: The output to consider.
 *
 * Returns: TRUE if the corresponding DRM connector has layout info.
 * FALSE otherwise.
 */
Bool
vmwgfx_output_has_origin(xf86OutputPtr output)
{
    struct output_private *vmwgfx_output = output->driver_private;

    return vmwgfx_output->suggested_x != -1 &&
	vmwgfx_output->suggested_y != -1;
}

/**
 * vmwgfx_output_origin - Get the origin for an output in the GUI layout.
 *
 * @output: The output to consider.
 * @x: Outputs the x coordinate of the origin.
 * @y: Outputs the y coordinate of the origin.
 */
void
vmwgfx_output_origin(xf86OutputPtr output, int *x, int *y)
{
    struct output_private *vmwgfx_output = output->driver_private;

    *x = vmwgfx_output->props[vmwgfx_output->suggested_x].value;
    *y = vmwgfx_output->props[vmwgfx_output->suggested_y].value;
}

/**
 * vmwgfx_output_is_implicit - Whether an output uses implicit placement
 *
 * output: The output to consider.
 *
 * Returns: TRUE if the output uses implicit placement. False otherwise.
 */
static Bool
vmwgfx_output_is_implicit(xf86OutputPtr output)
{
    struct output_private *vmwgfx_output = output->driver_private;

    if (vmwgfx_output->implicit_placement == -1)
	return TRUE;

    return !!vmwgfx_output->props[vmwgfx_output->implicit_placement].value;
}

/**
 * output_property_ignore - Function to determine whether to ignore or
 * to re-export a drm property.
 *
 * @prop: Pointer to the drmModeProperty to consider
 *
 * RETURNS: TRUE if the property should be re-exported. FALSE otherwise.
 */
static Bool
output_property_ignore(drmModePropertyPtr prop)
{
    if (!prop)
	return TRUE;
    /* ignore blob prop */
    if (prop->flags & DRM_MODE_PROP_BLOB)
	return TRUE;
    /* ignore standard property */
    if (!strcmp(prop->name, "EDID") ||
	!strcmp(prop->name, "DPMS") ||
	!strcmp(prop->name, "dirty"))
	return TRUE;

    return FALSE;
}

static void
output_create_resources(xf86OutputPtr output)
{
    modesettingPtr ms = modesettingPTR(output->scrn);
    struct output_private *vmwgfx_output = output->driver_private;
    drmModeConnectorPtr drm_connector = vmwgfx_output->drm_connector;
    drmModePropertyPtr drmmode_prop;
    int i, j, err;

    vmwgfx_output->props = calloc(drm_connector->count_props,
				  sizeof(struct output_prop));
    if (!vmwgfx_output->props)
	return;

    vmwgfx_output->num_props = 0;
    for (i = 0, j = 0; i < drm_connector->count_props; i++) {
	drmmode_prop = drmModeGetProperty(ms->fd, drm_connector->props[i]);
	if (output_property_ignore(drmmode_prop)) {
	    drmModeFreeProperty(drmmode_prop);
	    continue;
	}
	vmwgfx_output->props[j].index = i;
	vmwgfx_output->props[j].mode_prop = drmmode_prop;
	vmwgfx_output->props[j].value = drm_connector->prop_values[i];
	if (!strcmp(drmmode_prop->name,"suggested X"))
	    vmwgfx_output->suggested_x = j;
	if (!strcmp(drmmode_prop->name,"suggested Y"))
	    vmwgfx_output->suggested_y = j;
	if (!strcmp(drmmode_prop->name,"implicit_placement"))
	    vmwgfx_output->implicit_placement = j;
	vmwgfx_output->num_props++;
	j++;
    }

    for (i = 0; i < vmwgfx_output->num_props; i++) {
	struct output_prop *p = &vmwgfx_output->props[i];
	drmmode_prop = p->mode_prop;

	if (drmmode_prop->flags & DRM_MODE_PROP_RANGE) {
	    INT32 qrange[2];
	    INT32 value = p->value;

	    p->num_atoms = 1;
	    p->atoms = calloc(p->num_atoms, sizeof(Atom));
	    if (!p->atoms)
		continue;
	    p->atoms[0] = MakeAtom(drmmode_prop->name, strlen(drmmode_prop->name), TRUE);
	    qrange[0] = drmmode_prop->values[0];
	    qrange[1] = drmmode_prop->values[1];
	    err = RRConfigureOutputProperty(output->randr_output, p->atoms[0],
		    FALSE, TRUE,
		    drmmode_prop->flags & DRM_MODE_PROP_IMMUTABLE ? TRUE : FALSE,
		    2, qrange);
	    if (err != 0) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			"RRConfigureOutputProperty error, %d\n", err);
	    }
	    err = RRChangeOutputProperty(output->randr_output, p->atoms[0],
		    XA_INTEGER, 32, PropModeReplace, 1, &value, FALSE, TRUE);
	    if (err != 0) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			"RRChangeOutputProperty error, %d\n", err);
	    }
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
    modesettingPtr ms = modesettingPTR(output->scrn);
    struct output_private *vmwgfx_output = output->driver_private;
    int i;

    for (i = 0; i < vmwgfx_output->num_props; i++) {
	struct output_prop *p = &vmwgfx_output->props[i];

	if (p->atoms[0] != property)
	    continue;

	if (p->mode_prop->flags & DRM_MODE_PROP_RANGE) {
	    uint32_t val;

	    if (value->type != XA_INTEGER || value->format != 32 ||
		    value->size != 1)
		return FALSE;
	    val = *(uint32_t *)value->data;
	    p->value = val;
	    drmModeConnectorSetProperty
		(ms->fd, vmwgfx_output->drm_connector->connector_id,
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

	    /* search for matching name string, then set its value down */
	    for (j = 0; j < p->mode_prop->count_enums; j++) {
		if (!strcmp(p->mode_prop->enums[j].name, name)) {
		    p->value = p->mode_prop->enums[j].value;
		    drmModeConnectorSetProperty
			(ms->fd, vmwgfx_output->drm_connector->connector_id,
			 p->mode_prop->prop_id, p->value);
		    return TRUE;
		}
	    }
	}
    }

    return TRUE;
}
#endif /* RANDR_12_INTERFACE */

/**
 * vmwgfx_output_property_scan - Update a single property on a single output
 * @output: Pointer to the output to consider.
 * @p: The property to update.
 *
 * Reads the property value from the drm connector corresponding to
 * @output and notifies the RandR code of the new value, sending out an
 * event if the new value doesn't match the old one. Finally updates @p
 * with the new value.
 */
static Bool
vmwgfx_output_property_scan(xf86OutputPtr output,
			    struct output_prop *p)
{
    struct output_private *vmwgfx_output = output->driver_private;
    uint32_t value = vmwgfx_output->drm_connector->prop_values[p->index];
    int err = 0;

#ifdef RANDR_13_INTERFACE
    if (p->mode_prop->flags & DRM_MODE_PROP_RANGE) {
	err = RRChangeOutputProperty(output->randr_output,
				     p->atoms[0], XA_INTEGER, 32,
				     PropModeReplace, 1, &value,
				     value != p->value, FALSE);
    } else if (p->mode_prop->flags & DRM_MODE_PROP_ENUM) {
	int j;

	/* search for matching name string, then set its value down */
	for (j = 0; j < p->mode_prop->count_enums; j++) {
	    if (p->mode_prop->enums[j].value == value)
		break;
	}

	err = RRChangeOutputProperty(output->randr_output, p->atoms[0],
				     XA_ATOM, 32, PropModeReplace, 1,
				     &p->atoms[j+1], value != p->value,
				     FALSE);
    }
#endif /* RANDR_13_INTERFACE */
    if (!err)
	p->value = value;

    return !err;
}

#ifdef RANDR_13_INTERFACE
static Bool
output_get_property(xf86OutputPtr output, Atom property)
{
    modesettingPtr ms = modesettingPTR(output->scrn);
    struct output_private *vmwgfx_output = output->driver_private;
    int i;

    if (output->scrn->vtSema) {
	drmModeConnectorPtr drm_connector =
	    drmModeGetConnector(ms->fd,
				vmwgfx_output->drm_connector->connector_id);

	if (drm_connector) {
	    drmModeFreeConnector(vmwgfx_output->drm_connector);
	    vmwgfx_output->drm_connector = drm_connector;
	}
    }

    for (i = 0; i < vmwgfx_output->num_props; i++) {
	struct output_prop *p = &vmwgfx_output->props[i];
	if (p->atoms[0] != property)
	    continue;

	return vmwgfx_output_property_scan(output, p);
    }

    return FALSE;
}
#endif /* RANDR_13_INTERFACE */

static void
output_destroy(xf86OutputPtr output)
{
    struct output_private *priv = output->driver_private;
    int i;

    for (i = 0; i < priv->num_props; i++) {
	drmModeFreeProperty(priv->props[i].mode_prop);
	free(priv->props[i].atoms);
    }
    free(priv->props);

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
	crtc = output->crtc;

	if (!crtc || !crtc->enabled || !vmwgfx_output_is_implicit(output))
	    continue;

	REGION_RESET(pScreen, &cur_output, &crtc->bounds);
	REGION_UNION(pScreen, &output_union, &output_union, &cur_output);
    }

    /*
     * Explicit outputs may not overlap any other output.
     */
    for (i = 0; i < config->num_output; i++) {
	output = config->output[i];
	crtc = output->crtc;

	if (!crtc || !crtc->enabled || vmwgfx_output_is_implicit(output))
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
    int c;

    res = drmModeGetResources(ms->fd);
    if (res == 0) {
	DRV_ERROR("Failed drmModeGetResources\n");
	return;
    }

    for (c = 0; c < res->count_connectors; c++) {
	drm_connector = drmModeGetConnector(ms->fd, res->connectors[c]);
	if (!drm_connector)
	    goto out;

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

	priv->suggested_x = -1;
	priv->suggested_y = -1;
	priv->implicit_placement = -1;

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

#ifdef HAVE_LIBUDEV

/**
 * vmwgfx_output_properties_scan - Update all properties on all outputs
 * on this screen.
 * @pScrn: Pointer to the ScrnInfo structure for this screen.
 *
 * Updates all connector info from DRM and then calls
 * vmwgfx_output_property_scan() for all properties on all connectors.
 */
static void
vmwgfx_output_properties_scan(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);
    modesettingPtr ms = modesettingPTR(pScrn);
    int i;

    for (i = 0; i < config->num_output; i++) {
	xf86OutputPtr output = config->output[i];
	struct output_private *vmwgfx_output = output->driver_private;
	int j;

	if (output->scrn->vtSema) {
	    int id = vmwgfx_output->drm_connector->connector_id;

	    if (vmwgfx_output->drm_connector)
		drmModeFreeConnector(vmwgfx_output->drm_connector);
	    vmwgfx_output->drm_connector = drmModeGetConnector(ms->fd, id);
	}

	if (!vmwgfx_output->drm_connector)
	    continue;

	for (j = 0; j < vmwgfx_output->num_props; j++) {
	    struct output_prop *p = &vmwgfx_output->props[j];

	    (void) vmwgfx_output_property_scan(output, p);
	}
    }
}

/**
 * vmwgfx_outputs_off - Mark all crtc / output pairs as disabled and save
 * their configuration.
 *
 * @pScrn: Pointer to a ScrnInfo struct.
 *
 * Note that to commit this to the display system, a call to this function
 * should be followed by a call to xf86DisableUnusedFunctions()
 */
void
vmwgfx_outputs_off(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    for (i = 0; i < config->num_output; ++i) {
	xf86OutputPtr output = config->output[i];
	struct output_private *vmwgfx_output = output->driver_private;

	vmwgfx_output->saved_crtc = output->crtc;
	if (output->crtc) {
	    vmwgfx_output->saved_crtc_enabled = output->crtc->enabled;
	    output->crtc->enabled = FALSE;
	    output->crtc = NULL;
	}
    }
}

/**
 * vmwgfx_outputs_on - Reset crtc / output pairs to a configuation saved
 * using vmwgfx_output_off.
 *
 * @pScrn: Pointer to a ScrnInfo struct.
 *
 * Note that to commit the setup to the display system, a call to this
 * function should be followed by a call to xf86SetDesiredModes().
 */
void
vmwgfx_outputs_on(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    for (i = 0; i < config->num_output; ++i) {
	xf86OutputPtr output = config->output[i];
	struct output_private *vmwgfx_output = output->driver_private;

	if (vmwgfx_output->saved_crtc) {
	    output->crtc = vmwgfx_output->saved_crtc;
	    output->crtc->enabled = vmwgfx_output->saved_crtc_enabled;
	}
    }
}

/**
 * vmwgfx_handle uevent - Property update callback
 *
 * @fd: File descriptor for the uevent
 * @closure: Pointer to the driver-private per-screen data cast to a void *
 */
static void
vmwgfx_handle_uevents(int fd, void *closure)
{
    ScrnInfoPtr scrn = closure;
    modesettingPtr ms = modesettingPTR(scrn);
    struct udev_device *dev;
    ScreenPtr pScreen = xf86ScrnToScreen(scrn);

    dev = udev_monitor_receive_device(ms->uevent_monitor);
    if (!dev)
	return;

    /* Read new properties, connection status and preferred modes from DRM. */
    vmwgfx_output_properties_scan(scrn);

    if (pScreen)
	RRGetInfo(pScreen, TRUE);

    if (ms->autoLayout)
	vmwgfx_layout_handler(scrn);

    udev_device_unref(dev);
}
#endif  /* HAVE_LIBUDEV */

/**
 * vmwgfx_uevent_init - Initialize the property update monitor
 *
 * @scrn: Pointer to the ScrnInfo for this screen
 * @ms: Pointer to the driver private per-screen data
 */
void vmwgfx_uevent_init(ScrnInfoPtr scrn, modesettingPtr ms)
{
#ifdef HAVE_LIBUDEV
    struct udev *u;
    struct udev_monitor *mon;

    u = udev_new();
    if (!u)
	return;
    mon = udev_monitor_new_from_netlink(u, "udev");
    if (!mon) {
	udev_unref(u);
	return;
    }

    if (udev_monitor_filter_add_match_subsystem_devtype(mon,
							"drm",
							"drm_minor") < 0 ||
	udev_monitor_enable_receiving(mon) < 0) {
	udev_monitor_unref(mon);
	udev_unref(u);
	return;
    }

    ms->uevent_handler = xf86AddGeneralHandler(udev_monitor_get_fd(mon),
					       vmwgfx_handle_uevents,
					       scrn);

    ms->uevent_monitor = mon;
#endif  /* HAVE_LIBUDEV */
}

/**
 * vmwgfx_uevent_fini - Close the property update monitor
 *
 * @scrn: Pointer to the ScrnInfo for this screen
 * @ms: Pointer to the driver private per-screen data
 */
void vmwgfx_uevent_fini(ScrnInfoPtr scrn, modesettingPtr ms)
{
#ifdef HAVE_LIBUDEV
    if (ms->uevent_handler) {
	struct udev *u = udev_monitor_get_udev(ms->uevent_monitor);

	xf86RemoveGeneralHandler(ms->uevent_handler);

	udev_monitor_unref(ms->uevent_monitor);
	udev_unref(u);
    }
#endif /* HAVE_LIBUDEV */
}

/* vim: set sw=4 ts=8 sts=4: */
