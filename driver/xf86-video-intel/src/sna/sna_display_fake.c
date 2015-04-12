/*
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
 *	Chris Wilson <chris@chris-wilson.co.uk>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sna.h"

static bool add_fake_output(struct sna *sna, bool late);

static void
sna_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
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
	char outputs[256];

	xf86DrvMsg(crtc->scrn->scrnIndex, X_INFO,
		   "switch to mode %dx%d on %s, position (%d, %d), rotation %s, reflection %s\n",
		   mode->HDisplay, mode->VDisplay,
		   outputs_for_crtc(crtc, outputs, sizeof(outputs)),
		   x, y, rotation_to_str(rotation), reflection_to_str(rotation));

	return TRUE;
}

static void
sna_crtc_gamma_set(xf86CrtcPtr crtc,
		       CARD16 *red, CARD16 *green, CARD16 *blue, int size)
{
}

static void
sna_crtc_destroy(xf86CrtcPtr crtc)
{
}

static const xf86CrtcFuncsRec sna_crtc_funcs = {
	.dpms = sna_crtc_dpms,
	.set_mode_major = sna_crtc_set_mode_major,
	.gamma_set = sna_crtc_gamma_set,
	.destroy = sna_crtc_destroy,
};

static void
sna_output_create_resources(xf86OutputPtr output)
{
}

static Bool
sna_output_set_property(xf86OutputPtr output, Atom property,
			    RRPropertyValuePtr value)
{
	return TRUE;
}

static Bool
sna_output_get_property(xf86OutputPtr output, Atom property)
{
	return FALSE;
}

static void
sna_output_dpms(xf86OutputPtr output, int dpms)
{
}

static xf86OutputStatus
sna_output_detect(xf86OutputPtr output)
{
	DBG(("%s(%s) has user modes? %d\n",
	     __FUNCTION__, output->name,
	     output->randr_output && output->randr_output->numUserModes));

	if (output->randr_output && output->randr_output->numUserModes) {
		xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(output->scrn);

		if (xf86_config->output[xf86_config->num_output-1] == output)
			add_fake_output(to_sna(output->scrn), true);

		return XF86OutputStatusConnected;
	}

	return XF86OutputStatusDisconnected;
}

static Bool
sna_output_mode_valid(xf86OutputPtr output, DisplayModePtr mode)
{
	if (mode->type & M_T_DEFAULT)
		return MODE_BAD;

	return MODE_OK;
}

static DisplayModePtr
sna_output_get_modes(xf86OutputPtr output)
{
	return NULL;
}

static void
sna_output_destroy(xf86OutputPtr output)
{
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

static Bool
sna_mode_resize(ScrnInfoPtr scrn, int width, int height)
{
	ScreenPtr screen = scrn->pScreen;
	PixmapPtr new_front;

	DBG(("%s (%d, %d) -> (%d, %d)\n", __FUNCTION__,
	     scrn->virtualX, scrn->virtualY,
	     width, height));

	if (scrn->virtualX == width && scrn->virtualY == height)
		return TRUE;

	assert(to_sna_from_screen(screen)->front);
	assert(screen->GetScreenPixmap(screen) == to_sna_from_screen(screen)->front);

	DBG(("%s: creating new framebuffer %dx%d\n",
	     __FUNCTION__, width, height));

	new_front = screen->CreatePixmap(screen,
					 width, height, scrn->depth,
					 0);
	if (!new_front)
		return FALSE;

	scrn->virtualX = width;
	scrn->virtualY = height;
	scrn->displayWidth = width;

	screen->SetScreenPixmap(new_front);
	assert(screen->GetScreenPixmap(screen) == new_front);
	assert(to_sna_from_screen(screen)->front == new_front);

	screen->DestroyPixmap(new_front);

	return TRUE;
}

static const xf86CrtcConfigFuncsRec sna_mode_funcs = {
	sna_mode_resize
};

static bool add_fake_output(struct sna *sna, bool late)
{
	ScrnInfoPtr scrn = sna->scrn;
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	xf86OutputPtr output;
	xf86CrtcPtr crtc;
	RROutputPtr clones[32];
	RRCrtcPtr crtcs[32];
	char buf[80];
	int i, len;

	if (sna->mode.num_fake >= 32)
		return false;

	DBG(("%s(late=%d, num_fake=%d)\n", __FUNCTION__, late, sna->mode.num_fake+1));

	crtc = xf86CrtcCreate(scrn, &sna_crtc_funcs);
	if (crtc == NULL)
		return false;

	len = sprintf(buf, "VIRTUAL%d", sna->mode.num_fake+1);
	output = xf86OutputCreate(scrn, &sna_output_funcs, buf);
	if (!output) {
		xf86CrtcDestroy(crtc);
		return false;
	}

	output->mm_width = 0;
	output->mm_height = 0;
	output->interlaceAllowed = FALSE;
	output->subpixel_order = SubPixelNone;

	output->possible_crtcs = ~((1 << sna->mode.num_real_crtc) - 1);
	output->possible_clones = ~((1 << sna->mode.num_real_output) - 1);

	if (late) {
		ScreenPtr screen = xf86ScrnToScreen(scrn);

		crtc->randr_crtc = RRCrtcCreate(screen, crtc);
		output->randr_output = RROutputCreate(screen, buf, len, output);
		if (crtc->randr_crtc == NULL || output->randr_output == NULL) {
			xf86OutputDestroy(output);
			xf86CrtcDestroy(crtc);
			return false;
		}

		RRPostPendingProperties(output->randr_output);

		for (i = sna->mode.num_real_output; i < xf86_config->num_output; i++)
			clones[i - sna->mode.num_real_output] = xf86_config->output[i]->randr_output;
		assert(i - sna->mode.num_real_output == sna->mode.num_fake + 1);

		for (i = sna->mode.num_real_crtc; i < xf86_config->num_crtc; i++)
			crtcs[i - sna->mode.num_real_crtc] = xf86_config->crtc[i]->randr_crtc;
		assert(i - sna->mode.num_real_crtc == sna->mode.num_fake + 1);

		for (i = sna->mode.num_real_output; i < xf86_config->num_output; i++) {
			RROutputPtr rr_output = xf86_config->output[i]->randr_output;

			if (!RROutputSetCrtcs(rr_output, crtcs, sna->mode.num_fake + 1) ||
			    !RROutputSetClones(rr_output, clones, sna->mode.num_fake + 1))
				goto err;
		}

		RRCrtcSetRotations(crtc->randr_crtc,
				   RR_Rotate_All | RR_Reflect_All);
	}

	sna->mode.num_fake++;
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "Enabled output %s\n",
		   output->name);
	return true;

err:
	for (i = 0; i < xf86_config->num_output; i++) {
		output = xf86_config->output[i];
		if (output->driver_private)
			continue;

		xf86OutputDestroy(output);
	}

	for (i = 0; i < xf86_config->num_crtc; i++) {
		crtc = xf86_config->crtc[i];
		if (crtc->driver_private)
			continue;
		xf86CrtcDestroy(crtc);
	}
	sna->mode.num_fake = -1;
	return false;
}

bool sna_mode_fake_init(struct sna *sna, int num_fake)
{
	bool ret;

	if (num_fake == 0)
		return true;

	if (sna->mode.num_real_crtc == 0) {
		xf86CrtcConfigInit(sna->scrn, &sna_mode_funcs);
		xf86CrtcSetSizeRange(sna->scrn, 1, 1, INT16_MAX, INT16_MAX);
	}

	ret = true;
	while (ret && num_fake--)
		ret = add_fake_output(sna, false);
	return ret;
}
