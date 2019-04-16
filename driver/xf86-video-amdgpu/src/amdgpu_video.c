
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "amdgpu_drv.h"
#include "amdgpu_glamor.h"
#include "amdgpu_probe.h"
#include "amdgpu_video.h"
#include "amdgpu_pixmap.h"

#include "xf86.h"
#include "dixstruct.h"

/* DPMS */
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#include <X11/extensions/Xv.h>
#include "fourcc.h"

#define OFF_DELAY       250	/* milliseconds */
#define FREE_DELAY      15000

#define OFF_TIMER       0x01
#define FREE_TIMER      0x02
#define CLIENT_VIDEO_ON 0x04

static void amdgpu_box_intersect(BoxPtr dest, BoxPtr a, BoxPtr b)
{
	dest->x1 = a->x1 > b->x1 ? a->x1 : b->x1;
	dest->x2 = a->x2 < b->x2 ? a->x2 : b->x2;
	dest->y1 = a->y1 > b->y1 ? a->y1 : b->y1;
	dest->y2 = a->y2 < b->y2 ? a->y2 : b->y2;

	if (dest->x1 >= dest->x2 || dest->y1 >= dest->y2)
		dest->x1 = dest->x2 = dest->y1 = dest->y2 = 0;
}

static void amdgpu_crtc_box(xf86CrtcPtr crtc, BoxPtr crtc_box)
{
	if (crtc->enabled) {
		crtc_box->x1 = crtc->x;
		crtc_box->x2 =
		    crtc->x + xf86ModeWidth(&crtc->mode, crtc->rotation);
		crtc_box->y1 = crtc->y;
		crtc_box->y2 =
		    crtc->y + xf86ModeHeight(&crtc->mode, crtc->rotation);
	} else
		crtc_box->x1 = crtc_box->x2 = crtc_box->y1 = crtc_box->y2 = 0;
}

static int amdgpu_box_area(BoxPtr box)
{
	return (int)(box->x2 - box->x1) * (int)(box->y2 - box->y1);
}

Bool amdgpu_crtc_is_enabled(xf86CrtcPtr crtc)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	return drmmode_crtc->dpms_mode == DPMSModeOn;
}

xf86CrtcPtr
amdgpu_pick_best_crtc(ScrnInfoPtr pScrn, Bool consider_disabled,
		      int x1, int x2, int y1, int y2)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	int coverage, best_coverage, c, cd;
	BoxRec box, crtc_box, cover_box;
	RROutputPtr primary_output = NULL;
	xf86CrtcPtr best_crtc = NULL, primary_crtc = NULL;

	if (!pScrn->vtSema)
		return NULL;

	box.x1 = x1;
	box.x2 = x2;
	box.y1 = y1;
	box.y2 = y2;
	best_coverage = 0;

	/* Prefer the CRTC of the primary output */
	if (dixPrivateKeyRegistered(rrPrivKey))
	{
		primary_output = RRFirstOutput(pScrn->pScreen);
	}
	if (primary_output && primary_output->crtc)
		primary_crtc = primary_output->crtc->devPrivate;

	/* first consider only enabled CRTCs
	 * then on second pass consider disabled ones
	 */
	for (cd = 0; cd < (consider_disabled ? 2 : 1); cd++) {
		for (c = 0; c < xf86_config->num_crtc; c++) {
			xf86CrtcPtr crtc = xf86_config->crtc[c];

			if (!cd && !amdgpu_crtc_is_enabled(crtc))
				continue;

			amdgpu_crtc_box(crtc, &crtc_box);
			amdgpu_box_intersect(&cover_box, &crtc_box, &box);
			coverage = amdgpu_box_area(&cover_box);
			if (coverage > best_coverage ||
			    (coverage == best_coverage &&
			     crtc == primary_crtc)) {
				best_crtc = crtc;
				best_coverage = coverage;
			}
		}
		if (best_crtc)
			break;
	}

	return best_crtc;
}

void AMDGPUInitVideo(ScreenPtr pScreen)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	AMDGPUInfoPtr info = AMDGPUPTR(pScrn);
	XF86VideoAdaptorPtr *adaptors, *newAdaptors = NULL;
	XF86VideoAdaptorPtr texturedAdaptor = NULL;
	int num_adaptors;

	num_adaptors = xf86XVListGenericAdaptors(pScrn, &adaptors);
	newAdaptors =
	    malloc((num_adaptors + 2) * sizeof(*newAdaptors));
	if (!newAdaptors)
		return;

	memcpy(newAdaptors, adaptors,
	       num_adaptors * sizeof(XF86VideoAdaptorPtr));
	adaptors = newAdaptors;

	if (info->use_glamor) {
		texturedAdaptor = amdgpu_glamor_xv_init(pScreen, 16);
		if (texturedAdaptor != NULL) {
			adaptors[num_adaptors++] = texturedAdaptor;
			xf86DrvMsg(pScrn->scrnIndex, X_INFO,
				   "Set up textured video (glamor)\n");
		} else
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
				   "Failed to set up textured video (glamor)\n");
	}

	if (num_adaptors)
		xf86XVScreenInit(pScreen, adaptors, num_adaptors);

	if (newAdaptors)
		free(newAdaptors);

}
