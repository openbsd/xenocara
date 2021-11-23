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

#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "xf86str.h"
#include "X11/Xatom.h"
#include "micmap.h"
#include "xf86cmap.h"
#include "xf86DDC.h"

#include <xf86drm.h>
#include "xf86Crtc.h"
#include "via_driver.h"

/* DPMS */
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

static void
drmmode_ConvertFromKMode(ScrnInfoPtr pScrn, drmModeModeInfo *kmode,
                            DisplayModePtr mode)
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

    mode->Flags = kmode->flags; //& FLAG_BITS;
    mode->name = strdup(kmode->name);

    if (kmode->type & DRM_MODE_TYPE_DRIVER)
        mode->type = M_T_DRIVER;
    if (kmode->type & DRM_MODE_TYPE_PREFERRED)
        mode->type |= M_T_PREFERRED;
    xf86SetModeCrtc(mode, pScrn->adjustFlags);
}

static void
drmmode_ConvertToKMode(ScrnInfoPtr pScrn, drmModeModeInfo *kmode,
                        DisplayModePtr mode)
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

    kmode->flags = mode->Flags; //& FLAG_BITS;
    if (mode->name)
        strncpy(kmode->name, mode->name, DRM_DISPLAY_MODE_LEN);
    kmode->name[DRM_DISPLAY_MODE_LEN-1] = 0;
}

static void
drmmode_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
#if 0
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
    drmmode_ptr drmmode = drmmode_crtc->drmmode;

    /* bonghits in the randr 1.2 - uses dpms to disable crtc - bad buzz */
    if (mode == DPMSModeOff) {
        drmModeSetCrtc(drmmode->fd, drmmode_crtc->mode_crtc->crtc_id,
	                    0, 0, 0, NULL, 0, NULL);
    }
#endif
}

static Bool
drmmode_set_mode_major(xf86CrtcPtr crtc, DisplayModePtr mode,
                        Rotation rotation, int x, int y)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
    drmmode_ptr drmmode = drmmode_crtc->drmmode;
    ScrnInfoPtr pScrn = crtc->scrn;
    int output_count = 0, ret, i;
    uint32_t *output_ids = NULL;
    drmModeModeInfo kmode;

    if (!mode || !xf86CrtcRotate(crtc))
        return FALSE;

    output_ids = calloc(sizeof(uint32_t), xf86_config->num_output);
    if (!output_ids)
        return FALSE;

    for (i = 0; i < xf86_config->num_output; i++) {
        xf86OutputPtr output = xf86_config->output[i];
        drmmode_output_private_ptr drmmode_output;

        if (output->crtc != crtc)
            continue;

        drmmode_output = output->driver_private;
        output_ids[output_count] = drmmode_output->mode_output->connector_id;
        output_count++;
    }

    drmmode_ConvertToKMode(crtc->scrn, &kmode, mode);

    if (drmmode->fb_id == 0) {
        ret = drmModeAddFB(drmmode->fd, pScrn->virtualX, pScrn->virtualY,
                            pScrn->depth, pScrn->bitsPerPixel,
                            pScrn->virtualX *
                            ((pScrn->bitsPerPixel + 7) >> 3),
                            drmmode->front_bo->handle,
                            &drmmode->fb_id);
        if (ret < 0) {
            ErrorF("failed to add fb %d\n", ret);
            goto done;
        }
    }

    ret = drmModeSetCrtc(drmmode->fd, drmmode_crtc->mode_crtc->crtc_id,
                         drmmode->fb_id, x, y, output_ids, output_count, &kmode);
    if (ret) {
        xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR, "failed to set mode: %s",
                   strerror(-ret));
        goto done;
    }

    if (crtc->scrn->pScreen)
        xf86CrtcSetScreenSubpixelOrder(crtc->scrn->pScreen);

    /* go through all the outputs and force DPMS them back on? */
    for (i = 0; i < xf86_config->num_output; i++) {
        xf86OutputPtr output = xf86_config->output[i];

        if (output->crtc != crtc)
            continue;

        output->funcs->dpms(output, DPMSModeOn);
    }

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,7,0,0,0)
    crtc->funcs->gamma_set(crtc, crtc->gamma_red, crtc->gamma_green,
                           crtc->gamma_blue, crtc->gamma_size);
#endif

done:
    free(output_ids);
    return (ret < 0 ? FALSE : TRUE);
}

static void
drmmode_set_cursor_colors (xf86CrtcPtr crtc, int bg, int fg)
{
}

static void
drmmode_set_cursor_position (xf86CrtcPtr crtc, int x, int y)
{
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
    drmmode_ptr drmmode = drmmode_crtc->drmmode;

    drmModeMoveCursor(drmmode->fd, drmmode_crtc->mode_crtc->crtc_id, x, y);
}

static void
drmmode_hide_cursor (xf86CrtcPtr crtc)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
    xf86CursorInfoPtr cursor_info = xf86_config->cursor_info;
    drmmode_ptr drmmode = drmmode_crtc->drmmode;

    drmModeSetCursor(drmmode->fd, drmmode_crtc->mode_crtc->crtc_id, 0,
                        cursor_info->MaxWidth, cursor_info->MaxHeight);
}

static void
drmmode_show_cursor (xf86CrtcPtr crtc)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
    xf86CursorInfoPtr cursor_info = xf86_config->cursor_info;
    uint32_t handle = drmmode_crtc->cursor_bo->handle;
    drmmode_ptr drmmode = drmmode_crtc->drmmode;

    drmModeSetCursor(drmmode->fd, drmmode_crtc->mode_crtc->crtc_id, handle,
                        cursor_info->MaxWidth, cursor_info->MaxHeight);
}

static void
drmmode_load_cursor_argb (xf86CrtcPtr crtc, CARD32 *image)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
    xf86CursorInfoPtr cursor_info = xf86_config->cursor_info;
    uint32_t handle = drmmode_crtc->cursor_bo->handle, *ptr;

    /* cursor should be mapped already */
    ptr = drm_bo_map(crtc->scrn, drmmode_crtc->cursor_bo);
    memcpy(ptr, image, drmmode_crtc->cursor_bo->size);
    drm_bo_unmap(crtc->scrn, drmmode_crtc->cursor_bo);

    if (drmModeSetCursor(drmmode_crtc->drmmode->fd, drmmode_crtc->mode_crtc->crtc_id,
                            handle, cursor_info->MaxWidth, cursor_info->MaxHeight)) {
        drmmode_ptr drmmode = drmmode_crtc->drmmode;

        cursor_info->MaxWidth = cursor_info->MaxHeight = 0;
        drmmode->hwcursor = FALSE;
    }
}

static void
drmmode_crtc_gamma_set(xf86CrtcPtr crtc, uint16_t *red, uint16_t *green,
                      uint16_t *blue, int size)
{
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
    drmmode_ptr drmmode = drmmode_crtc->drmmode;

    drmModeCrtcSetGamma(drmmode->fd, drmmode_crtc->mode_crtc->crtc_id,
                        size, red, green, blue);
}

static const xf86CrtcFuncsRec drmmode_crtc_funcs = {
    .dpms                   = drmmode_crtc_dpms,
    .set_mode_major         = drmmode_set_mode_major,
    .set_cursor_colors      = drmmode_set_cursor_colors,
    .set_cursor_position    = drmmode_set_cursor_position,
    .show_cursor            = drmmode_show_cursor,
    .hide_cursor            = drmmode_hide_cursor,
    .load_cursor_argb       = drmmode_load_cursor_argb,
    .gamma_set              = drmmode_crtc_gamma_set,
    .destroy                = NULL,
};

static void
drmmode_crtc_init(ScrnInfoPtr pScrn, drmmode_ptr drmmode, int num)
{
    drmmode_crtc_private_ptr drmmode_crtc;
    xf86CrtcPtr crtc;

    crtc = xf86CrtcCreate(pScrn, &drmmode_crtc_funcs);
    if (crtc == NULL)
        return;

    drmmode_crtc = xnfcalloc(sizeof(drmmode_crtc_private_rec), 1);
    drmmode_crtc->mode_crtc = drmModeGetCrtc(drmmode->fd, drmmode->mode_res->crtcs[num]);
    drmmode_crtc->drmmode = drmmode;
    crtc->driver_private = drmmode_crtc;
}

/*
 * Handle KMS xf86Outputs
 */
static Bool
drmmode_property_ignore(drmModePropertyPtr prop)
{
    if (!prop)
        return TRUE;

    /* ignore blob prop */
    if (prop->flags & DRM_MODE_PROP_BLOB)
        return TRUE;

    /* ignore standard property */
    if (!strcmp(prop->name, "EDID") ||
        !strcmp(prop->name, "DPMS"))
        return TRUE;

    return FALSE;
}

static void
drmmode_output_dpms(xf86OutputPtr output, int mode)
{
	drmmode_output_private_ptr drmmode_output = output->driver_private;
	drmModeConnectorPtr koutput = drmmode_output->mode_output;
	drmmode_ptr drmmode = drmmode_output->drmmode;

	drmModeConnectorSetProperty(drmmode->fd, koutput->connector_id,
	                            drmmode_output->dpms_enum_id, mode);
	return;
}

static void
drmmode_output_create_resources(xf86OutputPtr output)
{
    drmmode_output_private_ptr drmmode_output = output->driver_private;
    drmModeConnectorPtr mode_output = drmmode_output->mode_output;
    drmmode_ptr drmmode = drmmode_output->drmmode;
    drmModePropertyPtr drmmode_prop;
    int i, j, err;

    drmmode_output->props = calloc(mode_output->count_props, sizeof(drmmode_prop_rec));
    if (!drmmode_output->props)
        return;

    drmmode_output->num_props = 0;
    for (i = 0, j = 0; i < mode_output->count_props; i++) {
	    drmmode_prop = drmModeGetProperty(drmmode->fd, mode_output->props[i]);

        if (drmmode_property_ignore(drmmode_prop)) {
	        drmModeFreeProperty(drmmode_prop);
            continue;
        }
        drmmode_output->props[j].mode_prop = drmmode_prop;
        drmmode_output->props[j].value = mode_output->prop_values[i];
        drmmode_output->num_props++;
        j++;
    }

    for (i = 0; i < drmmode_output->num_props; i++) {
        drmmode_prop_ptr p = &drmmode_output->props[i];
        drmmode_prop = p->mode_prop;

        if (drmmode_prop->flags & DRM_MODE_PROP_RANGE) {
            INT32 prop_range[2];
            INT32 value = p->value;

            p->num_atoms = 1;
            p->atoms = calloc(p->num_atoms, sizeof(Atom));
            if (!p->atoms)
                continue;
            p->atoms[0] = MakeAtom(drmmode_prop->name, strlen(drmmode_prop->name), TRUE);
            prop_range[0] = drmmode_prop->values[0];
            prop_range[1] = drmmode_prop->values[1];
            err = RRConfigureOutputProperty(output->randr_output, p->atoms[0],
                                            FALSE, TRUE,
                                            drmmode_prop->flags & DRM_MODE_PROP_IMMUTABLE ? TRUE : FALSE,
                                            2, prop_range);
            if (err != 0) {
                xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
                            "RRConfigureOutputProperty error, %d\n", err);
            }
            err = RRChangeOutputProperty(output->randr_output, p->atoms[0],
                                        XA_INTEGER, 32, PropModeReplace, 1,
                                        &value, FALSE, TRUE);
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
                                        XA_ATOM, 32, PropModeReplace, 1,
                                        &p->atoms[j+1], FALSE, TRUE);
            if (err != 0) {
                xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
                            "RRChangeOutputProperty error, %d\n", err);
            }
        }
    }
}

static Bool
drmmode_output_set_property(xf86OutputPtr output, Atom property,
                            RRPropertyValuePtr value)
{
    drmmode_output_private_ptr drmmode_output = output->driver_private;
    drmmode_ptr drmmode = drmmode_output->drmmode;
    int i;

    for (i = 0; i < drmmode_output->num_props; i++) {
        drmmode_prop_ptr p = &drmmode_output->props[i];

        if (p->atoms[0] != property)
            continue;

        if (p->mode_prop->flags & DRM_MODE_PROP_RANGE) {
            uint32_t val;

            if (value->type != XA_INTEGER || value->format != 32 ||
                value->size != 1)
                return FALSE;
            val = *(uint32_t *)value->data;

            drmModeConnectorSetProperty(drmmode->fd, drmmode_output->output_id,
                                        p->mode_prop->prop_id, (uint64_t)val);
            return TRUE;
        } else if (p->mode_prop->flags & DRM_MODE_PROP_ENUM) {
            const char *name;
            Atom atom;
            int	j;

            if (value->type != XA_ATOM || value->format != 32 || value->size != 1)
                return FALSE;
            memcpy(&atom, value->data, 4);
            name = NameForAtom(atom);

            /* search for matching name string, then set its value down */
            for (j = 0; j < p->mode_prop->count_enums; j++) {
                if (!strcmp(p->mode_prop->enums[j].name, name)) {
                    drmModeConnectorSetProperty(drmmode->fd, drmmode_output->output_id,
                    p->mode_prop->prop_id, p->mode_prop->enums[j].value);
                    return TRUE;
                }
            }
        }
    }
    return TRUE;
}

static Bool
drmmode_output_get_property(xf86OutputPtr output, Atom property)
{
    return TRUE;
}

static xf86OutputStatus
drmmode_output_detect(xf86OutputPtr output)
{
    /* go to the hw and retrieve a new output struct */
    drmmode_output_private_ptr drmmode_output = output->driver_private;
    drmmode_ptr drmmode = drmmode_output->drmmode;
    xf86OutputStatus status;

    drmModeFreeConnector(drmmode_output->mode_output);
    drmmode_output->mode_output = drmModeGetConnector(drmmode->fd, drmmode_output->output_id);

    switch (drmmode_output->mode_output->connection) {
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
drmmode_output_mode_valid(xf86OutputPtr output, DisplayModePtr pModes)
{
    return MODE_OK;
}

static DisplayModePtr
drmmode_output_get_modes(xf86OutputPtr output)
{
    drmmode_output_private_ptr drmmode_output = output->driver_private;
    drmModeConnectorPtr koutput = drmmode_output->mode_output;
    drmmode_ptr drmmode = drmmode_output->drmmode;
    DisplayModePtr Modes = NULL, Mode;
    drmModePropertyPtr props;
    xf86MonPtr mon = NULL;
    int i;

    /* look for an EDID property */
    for (i = 0; i < koutput->count_props; i++) {
        props = drmModeGetProperty(drmmode->fd, koutput->props[i]);
        if (props && (props->flags & DRM_MODE_PROP_BLOB)) {
            if (!strcmp(props->name, "EDID")) {
                if (drmmode_output->edid_blob)
                    drmModeFreePropertyBlob(drmmode_output->edid_blob);
                drmmode_output->edid_blob = drmModeGetPropertyBlob(drmmode->fd, koutput->prop_values[i]);
            }
            drmModeFreeProperty(props);
        }
    }

    if (drmmode_output->edid_blob) {
        mon = xf86InterpretEDID(output->scrn->scrnIndex,
                                drmmode_output->edid_blob->data);
        if (mon && drmmode_output->edid_blob->length > 128)
            mon->flags |= MONITOR_EDID_COMPLETE_RAWDATA;
    }
    xf86OutputSetEDID(output, mon);

    /* modes should already be available */
    for (i = 0; i < koutput->count_modes; i++) {
        Mode = xnfalloc(sizeof(DisplayModeRec));

        drmmode_ConvertFromKMode(output->scrn, &koutput->modes[i], Mode);
        Modes = xf86ModesAdd(Modes, Mode);

    }
    return Modes;
}

static void
drmmode_output_destroy(xf86OutputPtr output)
{
    drmmode_output_private_ptr drmmode_output = output->driver_private;
    int i;

    if (drmmode_output->edid_blob)
        drmModeFreePropertyBlob(drmmode_output->edid_blob);

    for (i = 0; i < drmmode_output->num_props; i++) {
        drmModeFreeProperty(drmmode_output->props[i].mode_prop);
        free(drmmode_output->props[i].atoms);
    }

    for (i = 0; i < drmmode_output->mode_output->count_encoders; i++) {
        drmModeFreeEncoder(drmmode_output->mode_encoders[i]);
        free(drmmode_output->mode_encoders);
    }
    free(drmmode_output->props);
    drmModeFreeConnector(drmmode_output->mode_output);
    free(drmmode_output);
    output->driver_private = NULL;
}

static const xf86OutputFuncsRec drmmode_output_funcs = {
    .dpms               = drmmode_output_dpms,
    .create_resources   = drmmode_output_create_resources,
#ifdef RANDR_12_INTERFACE
    .set_property       = drmmode_output_set_property,
    .get_property       = drmmode_output_get_property,
#endif
    .detect             = drmmode_output_detect,
    .mode_valid         = drmmode_output_mode_valid,
    .get_modes          = drmmode_output_get_modes,
    .destroy            = drmmode_output_destroy
};

static int subpixel_conv_table[7] = {
    0,
    SubPixelUnknown,
    SubPixelHorizontalRGB,
    SubPixelHorizontalBGR,
    SubPixelVerticalRGB,
    SubPixelVerticalBGR,
    SubPixelNone
};

const char *output_names[] = {
    "None",
    "VGA",
    "DVI",
    "DVI",
    "DVI",
    "Composite",
    "S-video",
    "LVDS",
    "CTV",
    "DIN",
    "DisplayPort",
    "HDMI",
    "HDMI",
    "TV",
    "eDP"
};

static void
drmmode_output_init(ScrnInfoPtr pScrn, drmmode_ptr drmmode, int num)
{
    xf86OutputPtr output;
    drmModeConnectorPtr koutput;
    drmModeEncoderPtr *kencoders = NULL;
    drmmode_output_private_ptr drmmode_output;
    drmModePropertyPtr props;
    char name[32];
    int i;

    koutput = drmModeGetConnector(drmmode->fd, drmmode->mode_res->connectors[num]);
    if (!koutput)
        return;

    kencoders = calloc(sizeof(drmModeEncoderPtr), koutput->count_encoders);
    if (!kencoders) {
        goto out_free_encoders;
    }

    for (i = 0; i < koutput->count_encoders; i++) {
        kencoders[i] = drmModeGetEncoder(drmmode->fd, koutput->encoders[i]);
        if (!kencoders[i]) {
            goto out_free_encoders;
        }
    }

    /* need to do smart conversion here for compat with non-kms ATI driver */
    snprintf(name, 32, "%s-%d", output_names[koutput->connector_type], koutput->connector_type_id - 1);

    output = xf86OutputCreate (pScrn, &drmmode_output_funcs, name);
    if (!output) {
        goto out_free_encoders;
    }

    drmmode_output = calloc(sizeof(drmmode_output_private_rec), 1);
    if (!drmmode_output) {
        xf86OutputDestroy(output);
        goto out_free_encoders;
    }

    drmmode_output->output_id = drmmode->mode_res->connectors[num];
    drmmode_output->mode_output = koutput;
    drmmode_output->mode_encoders = kencoders;
    drmmode_output->drmmode = drmmode;
    output->mm_width = koutput->mmWidth;
    output->mm_height = koutput->mmHeight;

    output->subpixel_order = subpixel_conv_table[koutput->subpixel];
    output->interlaceAllowed = TRUE;
    output->doubleScanAllowed = TRUE;
    output->driver_private = drmmode_output;

    output->possible_crtcs = 0x7f;
    for (i = 0; i < koutput->count_encoders; i++)
        output->possible_crtcs &= kencoders[i]->possible_crtcs;

    /* work out the possible clones later */
    output->possible_clones = 0;

    for (i = 0; i < koutput->count_props; i++) {
        props = drmModeGetProperty(drmmode->fd, koutput->props[i]);
        if (props && (props->flags & DRM_MODE_PROP_ENUM)) {
            if (!strcmp(props->name, "DPMS")) {
                drmmode_output->dpms_enum_id = koutput->props[i];
                drmModeFreeProperty(props);
                break;
            }
            drmModeFreeProperty(props);
        }
    }

    return;
out_free_encoders:
    if (kencoders){
        for (i = 0; i < koutput->count_encoders; i++)
            drmModeFreeEncoder(kencoders[i]);
        free(kencoders);
    }
    drmModeFreeConnector(koutput);
}

static uint32_t
find_clones(ScrnInfoPtr scrn, xf86OutputPtr output)
{
    drmmode_output_private_ptr drmmode_output = output->driver_private, clone_drmout;
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    xf86OutputPtr clone_output;
    int index_mask = 0, i;

    if (drmmode_output->enc_clone_mask == 0)
        return index_mask;

    for (i = 0; i < xf86_config->num_output; i++) {
        clone_output = xf86_config->output[i];
        clone_drmout = clone_output->driver_private;
        if (output == clone_output)
            continue;
        if (clone_drmout->enc_mask == 0)
            continue;
        if (drmmode_output->enc_clone_mask == clone_drmout->enc_mask)
            index_mask |= (1 << i);
    }
    return index_mask;
}

static void
drmmode_clones_init(ScrnInfoPtr scrn, drmmode_ptr drmmode)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    int i, j;

    for (i = 0; i < xf86_config->num_output; i++) {
        xf86OutputPtr output = xf86_config->output[i];
        drmmode_output_private_ptr drmmode_output;

        drmmode_output = output->driver_private;
        drmmode_output->enc_clone_mask = 0xff;
        /* and all the possible encoder clones for this output together */
        for (j = 0; j < drmmode_output->mode_output->count_encoders; j++) {
            int k;

            for (k = 0; k < drmmode->mode_res->count_encoders; k++) {
                if (drmmode->mode_res->encoders[k] == drmmode_output->mode_encoders[j]->encoder_id)
                    drmmode_output->enc_mask |= (1 << k);
            }

            drmmode_output->enc_clone_mask &= drmmode_output->mode_encoders[j]->possible_clones;
        }
    }

    for (i = 0; i < xf86_config->num_output; i++) {
        xf86OutputPtr output = xf86_config->output[i];
        output->possible_clones = find_clones(scrn, output);
    }
}

static const
xf86CrtcConfigFuncsRec via_xf86crtc_config_funcs = {
    via_xf86crtc_resize
};

Bool drmmode_pre_init(ScrnInfoPtr pScrn, drmmode_ptr drmmode)
{
    int i;
    Bool ret;

    xf86CrtcConfigInit(pScrn, &via_xf86crtc_config_funcs);

    drmmode->scrn = pScrn;
    drmmode->mode_res = drmModeGetResources(drmmode->fd);
    if (!drmmode->mode_res)
        return FALSE;

    xf86CrtcSetSizeRange(pScrn, 320, 200, drmmode->mode_res->max_width, drmmode->mode_res->max_height);
    for (i = 0; i < drmmode->mode_res->count_crtcs; i++)
        if (!xf86IsEntityShared(pScrn->entityList[0]) || pScrn->confScreen->device->screen == i)
            drmmode_crtc_init(pScrn, drmmode, i);

    for (i = 0; i < drmmode->mode_res->count_connectors; i++)
        drmmode_output_init(pScrn, drmmode, i);

    /* workout clones */
    drmmode_clones_init(pScrn, drmmode);
    ret = xf86InitialConfiguration(pScrn, TRUE);

    return ret;
}

#ifdef HAVE_LIBUDEV
static void
drmmode_handle_uevents(int fd, void *closure)
{
    drmmode_ptr drmmode = closure;
    ScrnInfoPtr scrn = drmmode->scrn;
    struct udev_device *dev;

    dev = udev_monitor_receive_device(drmmode->uevent_monitor);
    if (!dev)
        return;

    RRGetInfo(xf86ScrnToScreen(scrn), TRUE);
    udev_device_unref(dev);
}
#endif

void drmmode_uevent_init(ScrnInfoPtr scrn, drmmode_ptr drmmode)
{
#ifdef HAVE_LIBUDEV
    struct udev_monitor *mon;
    struct udev *u;

    u = udev_new();
    if (!u)
        return;

    mon = udev_monitor_new_from_netlink(u, "udev");
    if (!mon) {
        udev_unref(u);
        return;
    }

    if (udev_monitor_filter_add_match_subsystem_devtype(mon, "drm", "drm_minor") < 0 ||
            udev_monitor_enable_receiving(mon) < 0) {
        udev_monitor_unref(mon);
        udev_unref(u);
        return;
    }

    drmmode->uevent_handler = xf86AddGeneralHandler(udev_monitor_get_fd(mon),
                                                    drmmode_handle_uevents,
                                                    drmmode);
    drmmode->uevent_monitor = mon;
#endif
}

void drmmode_uevent_fini(ScrnInfoPtr scrn, drmmode_ptr drmmode)
{
#ifdef HAVE_LIBUDEV
    if (drmmode->uevent_handler) {
        struct udev *u = udev_monitor_get_udev(drmmode->uevent_monitor);

        xf86RemoveGeneralHandler(drmmode->uevent_handler);

        udev_monitor_unref(drmmode->uevent_monitor);
        udev_unref(u);
    }
#endif
}
