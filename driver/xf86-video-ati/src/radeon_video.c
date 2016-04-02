
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "radeon.h"
#include "radeon_glamor.h"
#include "radeon_reg.h"
#include "radeon_probe.h"
#include "radeon_video.h"

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

#define OFF_DELAY       250  /* milliseconds */
#define FREE_DELAY      15000

#define OFF_TIMER       0x01
#define FREE_TIMER      0x02
#define CLIENT_VIDEO_ON 0x04

#define GET_PORT_PRIVATE(pScrn) \
   (RADEONPortPrivPtr)((RADEONPTR(pScrn))->adaptor->pPortPrivates[0].ptr)

static void
radeon_box_intersect(BoxPtr dest, BoxPtr a, BoxPtr b)
{
    dest->x1 = a->x1 > b->x1 ? a->x1 : b->x1;
    dest->x2 = a->x2 < b->x2 ? a->x2 : b->x2;
    dest->y1 = a->y1 > b->y1 ? a->y1 : b->y1;
    dest->y2 = a->y2 < b->y2 ? a->y2 : b->y2;

    if (dest->x1 >= dest->x2 || dest->y1 >= dest->y2)
	dest->x1 = dest->x2 = dest->y1 = dest->y2 = 0;
}

static void
radeon_crtc_box(xf86CrtcPtr crtc, BoxPtr crtc_box)
{
    if (crtc->enabled) {
	crtc_box->x1 = crtc->x;
	crtc_box->x2 = crtc->x + xf86ModeWidth(&crtc->mode, crtc->rotation);
	crtc_box->y1 = crtc->y;
	crtc_box->y2 = crtc->y + xf86ModeHeight(&crtc->mode, crtc->rotation);
    } else
	crtc_box->x1 = crtc_box->x2 = crtc_box->y1 = crtc_box->y2 = 0;
}

static int
radeon_box_area(BoxPtr box)
{
    return (int) (box->x2 - box->x1) * (int) (box->y2 - box->y1);
}

Bool radeon_crtc_is_enabled(xf86CrtcPtr crtc)
{
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
    return drmmode_crtc->dpms_mode == DPMSModeOn;
}

xf86CrtcPtr
radeon_pick_best_crtc(ScrnInfoPtr pScrn, Bool consider_disabled,
		      int x1, int x2, int y1, int y2)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int			coverage, best_coverage, c, cd;
    BoxRec		box, crtc_box, cover_box;
    RROutputPtr         primary_output = NULL;
    xf86CrtcPtr         best_crtc = NULL, primary_crtc = NULL;

    if (!pScrn->vtSema)
	return NULL;

    box.x1 = x1;
    box.x2 = x2;
    box.y1 = y1;
    box.y2 = y2;
    best_coverage = 0;

    /* Prefer the CRTC of the primary output */
#ifdef HAS_DIXREGISTERPRIVATEKEY
    if (dixPrivateKeyRegistered(rrPrivKey))
#endif
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

	    if (!cd && !radeon_crtc_is_enabled(crtc))
		continue;

	    radeon_crtc_box(crtc, &crtc_box);
	    radeon_box_intersect(&cover_box, &crtc_box, &box);
	    coverage = radeon_box_area(&cover_box);
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


void RADEONInitVideo(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr    info = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    XF86VideoAdaptorPtr *adaptors, *newAdaptors = NULL;
    XF86VideoAdaptorPtr texturedAdaptor = NULL;
    int num_adaptors;

    /* no overlay or 3D on RN50 */
    if (info->ChipFamily == CHIP_FAMILY_RV100 && !pRADEONEnt->HasCRTC2)
	    return;

    num_adaptors = xf86XVListGenericAdaptors(pScrn, &adaptors);
    newAdaptors = malloc((num_adaptors + 2) * sizeof(*newAdaptors));
    if (newAdaptors == NULL)
	return;

    memcpy(newAdaptors, adaptors, num_adaptors * sizeof(XF86VideoAdaptorPtr));
    adaptors = newAdaptors;

    if (info->use_glamor) {
        texturedAdaptor = radeon_glamor_xv_init(pScreen, 16);
	if (texturedAdaptor != NULL) {
	    adaptors[num_adaptors++] = texturedAdaptor;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Set up textured video (glamor)\n");
	} else
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to set up textured video (glamor)\n");
    } else if ((info->ChipFamily < CHIP_FAMILY_RS400)
	|| (info->directRenderingEnabled)
	) {
	texturedAdaptor = RADEONSetupImageTexturedVideo(pScreen);
	if (texturedAdaptor != NULL) {
	    adaptors[num_adaptors++] = texturedAdaptor;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Set up textured video\n");
	} else
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to set up textured video\n");
    } else
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Textured video requires CP on R5xx/R6xx/R7xx/IGP\n");

    if(num_adaptors)
	xf86XVScreenInit(pScreen, adaptors, num_adaptors);

    if(texturedAdaptor) {
	XF86MCAdaptorPtr xvmcAdaptor = RADEONCreateAdaptorXvMC(pScreen, (char *)texturedAdaptor->name);
	if(xvmcAdaptor) {
	    if(!xf86XvMCScreenInit(pScreen, 1, &xvmcAdaptor))
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "[XvMC] Failed to initialize extension.\n");
	    else
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[XvMC] Extension initialized.\n");
	}
    }

    if(newAdaptors)
	free(newAdaptors);

}

void
RADEONCopyData(
  ScrnInfoPtr pScrn,
  unsigned char *src,
  unsigned char *dst,
  unsigned int srcPitch,
  unsigned int dstPitch,
  unsigned int h,
  unsigned int w,
  unsigned int bpp
){
    /* Get the byte-swapping right for big endian systems */
    if ( bpp == 2 ) {
	w *= 2;
	bpp = 1;
    }

    {
	int swap = RADEON_HOST_DATA_SWAP_NONE;

#if X_BYTE_ORDER == X_BIG_ENDIAN
	switch(bpp) {
	case 2:
	    swap = RADEON_HOST_DATA_SWAP_16BIT;
	    break;
	case 4:
	    swap = RADEON_HOST_DATA_SWAP_32BIT;
	    break;
	}
#endif

	w *= bpp;

	if (dstPitch == w && dstPitch == srcPitch)
	    RADEONCopySwap(dst, src, h * dstPitch, swap);
	else {
	    while (h--) {
		RADEONCopySwap(dst, src, w, swap);
		src += srcPitch;
		dst += dstPitch;
	    }
	}
    }
}


void
RADEONCopyMungedData(
   ScrnInfoPtr pScrn,
   unsigned char *src1,
   unsigned char *src2,
   unsigned char *src3,
   unsigned char *dst1,
   unsigned int srcPitch,
   unsigned int srcPitch2,
   unsigned int dstPitch,
   unsigned int h,
   unsigned int w
){
    uint32_t *dst;
    uint8_t *s1, *s2, *s3;
    int i, j;

    w /= 2;

    for( j = 0; j < h; j++ ) {
	dst = (pointer)dst1;
	s1 = src1;  s2 = src2;  s3 = src3;
	i = w;
	while( i > 4 ) {
	    dst[0] = cpu_to_le32(s1[0] | (s1[1] << 16) | (s3[0] << 8) | (s2[0] << 24));
	    dst[1] = cpu_to_le32(s1[2] | (s1[3] << 16) | (s3[1] << 8) | (s2[1] << 24));
	    dst[2] = cpu_to_le32(s1[4] | (s1[5] << 16) | (s3[2] << 8) | (s2[2] << 24));
	    dst[3] = cpu_to_le32(s1[6] | (s1[7] << 16) | (s3[3] << 8) | (s2[3] << 24));
	    dst += 4; s2 += 4; s3 += 4; s1 += 8;
	    i -= 4;
	}
	while( i-- ) {
	    dst[0] = cpu_to_le32(s1[0] | (s1[1] << 16) | (s3[0] << 8) | (s2[0] << 24));
	    dst++; s2++; s3++;
	    s1 += 2;
	}
	
	dst1 += dstPitch;
	src1 += srcPitch;
	if( j & 1 ) {
	    src2 += srcPitch2;
	    src3 += srcPitch2;
	}	
    }
}
