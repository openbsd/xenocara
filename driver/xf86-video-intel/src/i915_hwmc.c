/*
 * Copyright © 2006 Intel Corporation
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
 *    Xiang Haihao <haihao.xiang@intel.com>
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Resources.h"
#include "compiler.h"
#include "xf86PciInfo.h"
#include "xf86Pci.h"
#include "xf86fbman.h"
#include "regionstr.h"

#include "i830.h"
#include "i830_dri.h"
#include "i830_video.h"
#include "xf86xv.h"
#include "xf86xvmc.h"
#include <X11/extensions/Xv.h>
#include <X11/extensions/XvMC.h>
#include "xaa.h"
#include "xaalocal.h"
#include "dixstruct.h"
#include "fourcc.h"

#if defined(X_NEED_XVPRIV_H) || defined (_XF86_FOURCC_H_)
#include "xf86xvpriv.h"
#endif

#define _INTEL_XVMC_SERVER_
#include "i915_hwmc.h"

#define I915_XVMC_MAX_BUFFERS 2
#define I915_XVMC_MAX_CONTEXTS 4
#define I915_XVMC_MAX_SURFACES 20

typedef struct _I915XvMCSurfacePriv
{
    i830_memory *surface;
    unsigned long offsets[I915_XVMC_MAX_BUFFERS];
    drm_handle_t surface_handle;
} I915XvMCSurfacePriv;

typedef struct _I915XvMCContextPriv
{
    i830_memory *mcStaticIndirectState;
    drm_handle_t sis_handle;
    i830_memory *mcSamplerState;
    drm_handle_t ssb_handle;
    i830_memory *mcMapState;
    drm_handle_t msb_handle;
    i830_memory *mcPixelShaderProgram;
    drm_handle_t psp_handle;
    i830_memory *mcPixelShaderConstants;
    drm_handle_t psc_handle;
    i830_memory *mcCorrdata;
    drm_handle_t corrdata_handle;
} I915XvMCContextPriv;

typedef struct _I915XvMC
{
    XID contexts[I915_XVMC_MAX_CONTEXTS];
    XID surfaces[I915_XVMC_MAX_SURFACES];
    I915XvMCSurfacePriv *sfprivs[I915_XVMC_MAX_SURFACES];
    I915XvMCContextPriv *ctxprivs[I915_XVMC_MAX_CONTEXTS];
    int ncontexts,nsurfaces;
    PutImageFuncPtr savePutImage;
} I915XvMC, *I915XvMCPtr;

#define ARRARY_SIZE(a) (sizeof(a) / sizeof(a[0]))

/*
static int yv12_subpicture_index_list[2] =
{
    FOURCC_IA44,
    FOURCC_AI44
};

static XF86MCImageIDList yv12_subpicture_list =
{
    ARRARY_SIZE(yv12_subpicture_index_list),
    yv12_subpicture_index_list
};
 */

static XF86MCSurfaceInfoRec i915_YV12_mpg2_surface =
{
    FOURCC_YV12,
    XVMC_CHROMA_FORMAT_420,
    0,
    720,
    576,
    720,
    576,
    XVMC_MPEG_2,
    /* XVMC_OVERLAID_SURFACE | XVMC_SUBPICTURE_INDEPENDENT_SCALING,*/
    0,
    /* &yv12_subpicture_list*/
    NULL,
};

static XF86MCSurfaceInfoRec i915_YV12_mpg1_surface =
{
    FOURCC_YV12,
    XVMC_CHROMA_FORMAT_420,
    0,
    720,
    576,
    720,
    576,
    XVMC_MPEG_1,
    /* XVMC_OVERLAID_SURFACE | XVMC_SUBPICTURE_INDEPENDENT_SCALING,*/
    0,
    /* &yv12_subpicture_list*/
    NULL,
};

static XF86MCSurfaceInfoPtr ppSI[2] =
{
    (XF86MCSurfaceInfoPtr)&i915_YV12_mpg2_surface,
    (XF86MCSurfaceInfoPtr)&i915_YV12_mpg1_surface
};

#if 0
/* List of subpicture types that we support */
static XF86ImageRec ia44_subpicture = XVIMAGE_IA44;
static XF86ImageRec ai44_subpicture = XVIMAGE_AI44;

static XF86ImagePtr i915_subpicture_list[2] =
{
    (XF86ImagePtr)&ia44_subpicture,
    (XF86ImagePtr)&ai44_subpicture
};
#endif

/*
 * Init and clean up the screen private parts of XvMC.
 */
static void initI915XvMC(I915XvMCPtr xvmc)
{
    unsigned int i;

    for (i = 0; i < I915_XVMC_MAX_CONTEXTS; i++) {
        xvmc->contexts[i] = 0;
        xvmc->ctxprivs[i] = NULL;
    }

    for (i = 0; i < I915_XVMC_MAX_SURFACES; i++) {
        xvmc->surfaces[i] = 0;
        xvmc->sfprivs[i] = NULL;
    }
    xvmc->ncontexts = 0;
    xvmc->nsurfaces = 0;
}

static void cleanupI915XvMC(I915XvMCPtr xvmc)
{
    int i;

    for (i = 0; i < I915_XVMC_MAX_CONTEXTS; i++) {
        xvmc->contexts[i] = 0;
        if (xvmc->ctxprivs[i]) {
            xfree(xvmc->ctxprivs[i]);
            xvmc->ctxprivs[i] = NULL;
        }
    }

    for (i = 0; i < I915_XVMC_MAX_SURFACES; i++) {
        xvmc->surfaces[i] = 0;
        if (xvmc->sfprivs[i]) {
            xfree(xvmc->sfprivs[i]);
            xvmc->sfprivs[i] = NULL;
        }
    }
}

static Bool i915_map_xvmc_buffers(ScrnInfoPtr pScrn, I915XvMCContextPriv *ctxpriv)
{
    I830Ptr pI830 = I830PTR(pScrn);

    if (drmAddMap(pI830->drmSubFD,
                  (drm_handle_t)(ctxpriv->mcStaticIndirectState->offset + pI830->LinearAddr),
                  ctxpriv->mcStaticIndirectState->size, DRM_AGP, 0,
                  (drmAddress)&ctxpriv->sis_handle) < 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[drm] drmAddMap(sis_handle) failed!\n");
        return FALSE;
    }

    if (drmAddMap(pI830->drmSubFD,
                  (drm_handle_t)(ctxpriv->mcSamplerState->offset + pI830->LinearAddr),
                  ctxpriv->mcSamplerState->size, DRM_AGP, 0,
                  (drmAddress)&ctxpriv->ssb_handle) < 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[drm] drmAddMap(ssb_handle) failed!\n");
        return FALSE;
    }

    if (drmAddMap(pI830->drmSubFD,
                  (drm_handle_t)(ctxpriv->mcMapState->offset + pI830->LinearAddr),
                  ctxpriv->mcMapState->size, DRM_AGP, 0,
                  (drmAddress)&ctxpriv->msb_handle) < 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[drm] drmAddMap(msb_handle) failed!\n");
        return FALSE;
    }

    if (drmAddMap(pI830->drmSubFD,
                  (drm_handle_t)(ctxpriv->mcPixelShaderProgram->offset + pI830->LinearAddr),
                  ctxpriv->mcPixelShaderProgram->size, DRM_AGP, 0,
                  (drmAddress)&ctxpriv->psp_handle) < 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[drm] drmAddMap(psp_handle) failed!\n");
        return FALSE;
    }

    if (drmAddMap(pI830->drmSubFD,
                  (drm_handle_t)(ctxpriv->mcPixelShaderConstants->offset + pI830->LinearAddr),
                  ctxpriv->mcPixelShaderConstants->size, DRM_AGP, 0,
                  (drmAddress)&ctxpriv->psc_handle) < 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[drm] drmAddMap(psc_handle) failed!\n");
        return FALSE;
    }

    if (drmAddMap(pI830->drmSubFD,
                  (drm_handle_t)(ctxpriv->mcCorrdata->offset + pI830->LinearAddr),
                  ctxpriv->mcCorrdata->size, DRM_AGP, 0,
                  (drmAddress)&ctxpriv->corrdata_handle) < 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[drm] drmAddMap(corrdata_handle) failed!\n");
        return FALSE;
    }

        
    return TRUE;
}

static void i915_unmap_xvmc_buffers(ScrnInfoPtr pScrn, I915XvMCContextPriv *ctxpriv)
{
    I830Ptr pI830 = I830PTR(pScrn);
    
    if (ctxpriv->sis_handle) {
        drmRmMap(pI830->drmSubFD, ctxpriv->sis_handle);
        ctxpriv->sis_handle = 0;
    }

    if (ctxpriv->ssb_handle) {
        drmRmMap(pI830->drmSubFD, ctxpriv->ssb_handle);
        ctxpriv->ssb_handle = 0;
    }

    if (ctxpriv->msb_handle) {
        drmRmMap(pI830->drmSubFD, ctxpriv->msb_handle);
        ctxpriv->msb_handle = 0;
    }

    if (ctxpriv->psp_handle) {
        drmRmMap(pI830->drmSubFD, ctxpriv->psp_handle);
        ctxpriv->psp_handle = 0;
    }

    if (ctxpriv->psc_handle) {
        drmRmMap(pI830->drmSubFD, ctxpriv->psc_handle);
        ctxpriv->psc_handle = 0;
    }

    if (ctxpriv->corrdata_handle) {
        drmRmMap(pI830->drmSubFD, ctxpriv->corrdata_handle);
        ctxpriv->corrdata_handle = 0;
    }

}

static Bool i915_allocate_xvmc_buffers(ScrnInfoPtr pScrn, I915XvMCContextPriv *ctxpriv)
{
    I830Ptr pI830 = I830PTR(pScrn);
    int flags = ALIGN_BOTH_ENDS;

    if (IS_I915G(pI830) || IS_I915GM(pI830) ||
	    IS_I945G(pI830) || IS_I945GM(pI830))
        flags |= NEED_PHYSICAL_ADDR;

    if (!i830_allocate_xvmc_buffer(pScrn, "[XvMC]Static Indirect State",
                                  &(ctxpriv->mcStaticIndirectState), 4 * 1024,
                                  flags)) {
        return FALSE;
    }

    if (!i830_allocate_xvmc_buffer(pScrn, "[XvMC]Sampler State",
                                  &(ctxpriv->mcSamplerState), 4 * 1024,
                                  flags)) {
        return FALSE;
    }

    if (!i830_allocate_xvmc_buffer(pScrn, "[XvMC]Map State",
                                  &(ctxpriv->mcMapState), 4 * 1024,
                                  flags)) {
        return FALSE;
    }

    if (!i830_allocate_xvmc_buffer(pScrn, "[XvMC]Pixel Shader Program",
                                  &(ctxpriv->mcPixelShaderProgram), 4 * 1024,
                                  flags)) {
        return FALSE;
    }

    if (!i830_allocate_xvmc_buffer(pScrn, "[XvMC]Pixel Shader Constants",
                                  &(ctxpriv->mcPixelShaderConstants), 4 * 1024,
                                  flags)) {
        return FALSE;
    }

    if (!i830_allocate_xvmc_buffer(pScrn, "[XvMC]Correction Data Buffer", 
                                   &(ctxpriv->mcCorrdata), 512 * 1024,
                                   ALIGN_BOTH_ENDS)) {
        return FALSE;
    }

    if (0)
	i830_describe_allocations(pScrn, 1, "");

    return TRUE;
}

static void i915_free_xvmc_buffers(ScrnInfoPtr pScrn, I915XvMCContextPriv *ctxpriv)
{
    if (ctxpriv->mcStaticIndirectState) {
        i830_free_memory(pScrn, ctxpriv->mcStaticIndirectState);
        ctxpriv->mcStaticIndirectState = NULL;
    }

    if (ctxpriv->mcSamplerState) {
        i830_free_memory(pScrn, ctxpriv->mcSamplerState);
        ctxpriv->mcSamplerState = NULL;
    }

    if (ctxpriv->mcMapState) {
        i830_free_memory(pScrn, ctxpriv->mcMapState);
        ctxpriv->mcMapState = NULL;
    }

    if (ctxpriv->mcPixelShaderProgram) {
        i830_free_memory(pScrn, ctxpriv->mcPixelShaderProgram);
        ctxpriv->mcPixelShaderProgram = NULL;
    }

    if (ctxpriv->mcPixelShaderConstants) {
        i830_free_memory(pScrn, ctxpriv->mcPixelShaderConstants);
        ctxpriv->mcPixelShaderConstants = NULL;
    }

    if (ctxpriv->mcCorrdata) {
        i830_free_memory(pScrn, ctxpriv->mcCorrdata);
        ctxpriv->mcCorrdata = NULL;
    }

}

/*
 *  i915_xvmc_create_context
 *
 *  Some info about the private data:
 *
 *  Set *num_priv to the number of 32bit words that make up the size of
 *  of the data that priv will point to.
 *
 *  *priv = (long *) xcalloc (elements, sizeof(element))
 *  *num_priv = (elements * sizeof(element)) >> 2;
 *
 **************************************************************************/

static int i915_xvmc_create_context (ScrnInfoPtr pScrn, XvMCContextPtr pContext,
                                  int *num_priv, long **priv )
{
    I830Ptr pI830 = I830PTR(pScrn);
    DRIInfoPtr pDRIInfo = pI830->pDRIInfo;
    I830DRIPtr pI830DRI = pDRIInfo->devPrivate;
    I915XvMCCreateContextRec *contextRec = NULL;
    I915XvMCPtr pXvMC = (I915XvMCPtr)xvmc_driver->devPrivate;
    I915XvMCContextPriv *ctxpriv = NULL;
    int i;

    *priv = NULL;
    *num_priv = 0;

    if (!pI830->XvMCEnabled) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[XvMC] i915: XvMC disabled!\n");
        return BadAlloc;
    }

    for (i = 0; i < I915_XVMC_MAX_CONTEXTS; i++) {
        if (!pXvMC->contexts[i])
            break;
    }

    if (i == I915_XVMC_MAX_CONTEXTS ||
	    pXvMC->ncontexts >= I915_XVMC_MAX_CONTEXTS) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[XvMC] i915: Out of contexts.\n");
        return BadAlloc;
    }

    *priv = xcalloc(1, sizeof(I915XvMCCreateContextRec));
    contextRec = (I915XvMCCreateContextRec *)*priv;

    if (!*priv) {
        *num_priv = 0;
        return BadAlloc;
    }

    *num_priv = sizeof(I915XvMCCreateContextRec) >> 2;

    ctxpriv = (I915XvMCContextPriv *)xcalloc(1, sizeof(I915XvMCContextPriv));

    if (!ctxpriv) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[XvMC] i915: Unable to allocate memory!\n");
        xfree(*priv);
        *priv = NULL;
        *num_priv = 0;
        return BadAlloc;
    }

    if (!i915_allocate_xvmc_buffers(pScrn, ctxpriv)) {
        i915_free_xvmc_buffers(pScrn, ctxpriv);
        xfree(ctxpriv);
        ctxpriv = NULL;
        xfree(*priv);
        *priv = NULL;
        *num_priv = 0;
        return BadAlloc;
    }

    if (!i915_map_xvmc_buffers(pScrn, ctxpriv)) {
        i915_unmap_xvmc_buffers(pScrn, ctxpriv);
        i915_free_xvmc_buffers(pScrn, ctxpriv);
        xfree(ctxpriv);
        ctxpriv = NULL;
        xfree(*priv);
        *priv = NULL;
        *num_priv = 0;
        return BadAlloc;
    }

    /* common context items */
    contextRec->comm.type = xvmc_driver->flag;
    contextRec->comm.sarea_size = pDRIInfo->SAREASize;
    contextRec->comm.batchbuffer.offset = xvmc_driver->batch->offset;
    contextRec->comm.batchbuffer.size = xvmc_driver->batch->size;
    contextRec->comm.batchbuffer.handle = xvmc_driver->batch_handle;

    /* i915 private context */
    contextRec->ctxno = i;
    contextRec->sis.handle = ctxpriv->sis_handle;
    contextRec->sis.offset = ctxpriv->mcStaticIndirectState->offset;
    contextRec->sis.size = ctxpriv->mcStaticIndirectState->size;
    contextRec->sis.bus_addr = ctxpriv->mcStaticIndirectState->bus_addr;
    contextRec->ssb.handle = ctxpriv->ssb_handle;
    contextRec->ssb.offset = ctxpriv->mcSamplerState->offset;
    contextRec->ssb.size = ctxpriv->mcSamplerState->size;
    contextRec->ssb.bus_addr = ctxpriv->mcSamplerState->bus_addr;
    contextRec->msb.handle = ctxpriv->msb_handle;
    contextRec->msb.offset = ctxpriv->mcMapState->offset;
    contextRec->msb.size = ctxpriv->mcMapState->size;
    contextRec->msb.bus_addr = ctxpriv->mcMapState->bus_addr;
    contextRec->psp.handle = ctxpriv->psp_handle;
    contextRec->psp.offset = ctxpriv->mcPixelShaderProgram->offset;
    contextRec->psp.size = ctxpriv->mcPixelShaderProgram->size;
    contextRec->psp.bus_addr = ctxpriv->mcPixelShaderProgram->bus_addr;
    contextRec->psc.handle = ctxpriv->psc_handle;
    contextRec->psc.offset = ctxpriv->mcPixelShaderConstants->offset;
    contextRec->psc.size = ctxpriv->mcPixelShaderConstants->size;
    contextRec->psc.bus_addr = ctxpriv->mcPixelShaderConstants->bus_addr;
    contextRec->corrdata.handle = ctxpriv->corrdata_handle;
    contextRec->corrdata.offset = ctxpriv->mcCorrdata->offset;
    contextRec->corrdata.size = ctxpriv->mcCorrdata->size;
    contextRec->sarea_priv_offset = sizeof(XF86DRISAREARec);
    contextRec->deviceID = pI830DRI->deviceID;

    pXvMC->ncontexts++;
    pXvMC->contexts[i] = pContext->context_id;
    pXvMC->ctxprivs[i] = ctxpriv;

    return Success;
}

static int i915_xvmc_create_surface (ScrnInfoPtr pScrn, XvMCSurfacePtr pSurf,
                                 int *num_priv, long **priv )
{
    I830Ptr pI830 = I830PTR(pScrn);
    I915XvMCPtr pXvMC = (I915XvMCPtr)xvmc_driver->devPrivate;
    I915XvMCSurfacePriv *sfpriv = NULL;
    I915XvMCCreateSurfaceRec *surfaceRec = NULL;
    XvMCContextPtr ctx = NULL;
    unsigned int srfno;
    unsigned long bufsize;

    if (!pI830->XvMCEnabled) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[XvMC] i915: XvMC disabled!\n");
        return BadAlloc;
    }

    *priv = NULL;
    *num_priv = 0;

    for (srfno = 0; srfno < I915_XVMC_MAX_SURFACES; ++srfno) {
        if (!pXvMC->surfaces[srfno])
            break;
    }

    if (srfno == I915_XVMC_MAX_SURFACES ||
	    pXvMC->nsurfaces >= I915_XVMC_MAX_SURFACES) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[XvMC] i915: Too many surfaces !\n");
        return BadAlloc;
    }

    *priv = xcalloc(1, sizeof(I915XvMCCreateSurfaceRec));
    surfaceRec = (I915XvMCCreateSurfaceRec *)*priv;

    if (!*priv) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[XvMC] i915:Unable to allocate surface priv ret memory!\n");
        return BadAlloc;
    }

    *num_priv = sizeof(I915XvMCCreateSurfaceRec) >> 2;
    sfpriv = (I915XvMCSurfacePriv *)xcalloc(1, sizeof(I915XvMCSurfacePriv));

    if (!sfpriv) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[XvMC] i915: Unable to allocate surface priv memory!\n");
        xfree(*priv);
        *priv = NULL;
        *num_priv = 0;
        return BadAlloc;
    }

    ctx = pSurf->context;
    bufsize = SIZE_YUV420(ctx->width, ctx->height);

    if (!i830_allocate_xvmc_buffer(pScrn, "XvMC surface",
                                   &(sfpriv->surface), bufsize,
                                   ALIGN_BOTH_ENDS)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[XvMC] i915 : Failed to allocate XvMC surface space!\n");
        xfree(sfpriv);
        xfree(*priv);
        *priv = NULL;
        *num_priv = 0;
        return BadAlloc;
    }

    if (0)
	i830_describe_allocations(pScrn, 1, "");

    if (drmAddMap(pI830->drmSubFD,
                  (drm_handle_t)(sfpriv->surface->offset + pI830->LinearAddr),
                  sfpriv->surface->size, DRM_AGP, 0,
                  (drmAddress)&sfpriv->surface_handle) < 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[drm] drmAddMap(surface_handle) failed!\n");
        i830_free_memory(pScrn, sfpriv->surface);
        xfree(sfpriv);
        xfree(*priv);
        *priv = NULL;
        *num_priv = 0;
        return BadAlloc;
    }

    surfaceRec->srfno = srfno;
    surfaceRec->srf.handle = sfpriv->surface_handle;
    surfaceRec->srf.offset = sfpriv->surface->offset;
    surfaceRec->srf.size = sfpriv->surface->size;

    pXvMC->surfaces[srfno] = pSurf->surface_id;
    pXvMC->sfprivs[srfno]= sfpriv;
    pXvMC->nsurfaces++;

    return Success;
}

static int i915_xvmc_create_subpict(ScrnInfoPtr pScrn, XvMCSubpicturePtr pSubp,
                                     int *num_priv, long **priv )
{
    I830Ptr pI830 = I830PTR(pScrn);
    I915XvMCPtr pXvMC = (I915XvMCPtr)xvmc_driver->devPrivate;
    I915XvMCSurfacePriv *sfpriv = NULL;
    I915XvMCCreateSurfaceRec *surfaceRec = NULL;
    XvMCContextPtr ctx = NULL;
    unsigned int srfno;
    unsigned int bufsize;

    *priv = NULL;
    *num_priv = 0;

    for (srfno = 0; srfno < I915_XVMC_MAX_SURFACES; ++srfno) {
        if (!pXvMC->surfaces[srfno])
            break;
    }

    if (srfno == I915_XVMC_MAX_SURFACES ||
	    pXvMC->nsurfaces >= I915_XVMC_MAX_SURFACES) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[XvMC] i915: Too many surfaces !\n");
        return BadAlloc;
    }

    *priv = xcalloc(1, sizeof(I915XvMCCreateSurfaceRec));
    surfaceRec = (I915XvMCCreateSurfaceRec *)*priv;     

    if (!*priv) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[XvMC] i915: Unable to allocate memory!\n");
        return BadAlloc;
    }

    *num_priv = sizeof(I915XvMCCreateSurfaceRec) >> 2;
    sfpriv = (I915XvMCSurfacePriv *)xcalloc(1, sizeof(I915XvMCSurfacePriv));

    if (!sfpriv) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[XvMC] i915: Unable to allocate memory!\n");
        xfree(*priv);
        *priv = NULL;
        *num_priv = 0;
        return BadAlloc;
    }

    ctx = pSubp->context;
    bufsize = SIZE_XX44(ctx->width, ctx->height);

    if (!i830_allocate_xvmc_buffer(pScrn, "XvMC surface", 
                                   &(sfpriv->surface), bufsize,
                                   ALIGN_BOTH_ENDS)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[XvMC] I915XvMCCreateSurface: Failed to allocate XvMC surface space!\n");
        xfree(sfpriv);
        xfree(*priv);
        *priv = NULL;
        *num_priv = 0;
        return BadAlloc;
    }

    if (drmAddMap(pI830->drmSubFD,
                  (drm_handle_t)(sfpriv->surface->offset + pI830->LinearAddr),
                  sfpriv->surface->size, DRM_AGP, 0,
                  (drmAddress)&sfpriv->surface_handle) < 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[drm] drmAddMap(surface_handle) failed!\n");
        i830_free_memory(pScrn, sfpriv->surface);
        xfree(sfpriv);
        xfree(*priv);
        *priv = NULL;
        *num_priv = 0;
        return BadAlloc;
    }

    surfaceRec->srfno = srfno;
    surfaceRec->srf.handle = sfpriv->surface_handle;
    surfaceRec->srf.offset = sfpriv->surface->offset;
    surfaceRec->srf.size = sfpriv->surface->size;

    pXvMC->sfprivs[srfno] = sfpriv;
    pXvMC->surfaces[srfno] = pSubp->subpicture_id;
    pXvMC->nsurfaces++;

    return Success;
}

static void i915_xvmc_destroy_context (ScrnInfoPtr pScrn,
					XvMCContextPtr pContext)
{
    I915XvMCPtr pXvMC = (I915XvMCPtr)xvmc_driver->devPrivate;
    int i;

    for (i = 0; i < I915_XVMC_MAX_CONTEXTS; i++) {
        if (pXvMC->contexts[i] == pContext->context_id) {
            i915_unmap_xvmc_buffers(pScrn, pXvMC->ctxprivs[i]);
            i915_free_xvmc_buffers(pScrn, pXvMC->ctxprivs[i]);
            xfree(pXvMC->ctxprivs[i]);
            pXvMC->ctxprivs[i] = 0;
            pXvMC->ncontexts--;
            pXvMC->contexts[i] = 0;
            return;
        }
    }

    return;
}

static void i915_xvmc_destroy_surface (ScrnInfoPtr pScrn, XvMCSurfacePtr pSurf)
{
    I830Ptr pI830 = I830PTR(pScrn);
    I915XvMCPtr pXvMC = (I915XvMCPtr)xvmc_driver->devPrivate;
    int i;

    for (i = 0; i < I915_XVMC_MAX_SURFACES; i++) {
        if (pXvMC->surfaces[i] == pSurf->surface_id) {
            drmRmMap(pI830->drmSubFD, pXvMC->sfprivs[i]->surface_handle);
            i830_free_memory(pScrn, pXvMC->sfprivs[i]->surface);
            xfree(pXvMC->sfprivs[i]);
            pXvMC->nsurfaces--;
            pXvMC->sfprivs[i] = 0;
            pXvMC->surfaces[i] = 0;
            return;
        }
    }

    return;
}

static void i915_xvmc_destroy_subpict (ScrnInfoPtr pScrn,
					XvMCSubpicturePtr pSubp)
{
    I830Ptr pI830 = I830PTR(pScrn);
    I915XvMCPtr pXvMC = (I915XvMCPtr)xvmc_driver->devPrivate;
    int i;

    for (i = 0; i < I915_XVMC_MAX_SURFACES; i++) {
        if (pXvMC->surfaces[i] == pSubp->subpicture_id) {
            drmRmMap(pI830->drmSubFD, pXvMC->sfprivs[i]->surface_handle);
            i830_free_memory(pScrn, pXvMC->sfprivs[i]->surface);
            xfree(pXvMC->sfprivs[i]);
            pXvMC->nsurfaces--;
            pXvMC->sfprivs[i] = 0;
            pXvMC->surfaces[i] = 0;
            return;
        }
    }

    return;
}

static int i915_xvmc_put_image(ScrnInfoPtr pScrn,
	short src_x, short src_y,
	short drw_x, short drw_y, short src_w,
	short src_h, short drw_w, short drw_h,
	int id, unsigned char *buf, short width,
	short height, Bool sync, RegionPtr clipBoxes, pointer data,
	DrawablePtr pDraw)
{
    I830Ptr pI830 = I830PTR(pScrn);
    I915XvMCPtr pXvMC = (I915XvMCPtr)xvmc_driver->devPrivate;
    struct intel_xvmc_command *xvmc_cmd = (struct intel_xvmc_command *)buf;
    int ret;

    if (pI830->XvMCEnabled) {
        if (FOURCC_XVMC == id) {
            switch (xvmc_cmd->command) {
            case INTEL_XVMC_COMMAND_DISPLAY:
		if ((xvmc_cmd->srfNo >= I915_XVMC_MAX_SURFACES) ||
			!pXvMC->surfaces[xvmc_cmd->srfNo] ||
			!pXvMC->sfprivs[xvmc_cmd->srfNo]) {
		    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			    "[XvMC] i915 put image: Invalid parameters!\n");
		    return 1;
		}

		buf = pI830->FbBase +
		    pXvMC->sfprivs[xvmc_cmd->srfNo]->surface->offset;
		id = xvmc_cmd->real_id;
		pI830->IsXvMCSurface = 1;
		break;
            default:
                return 0;
            }
        }
    }

    ret = pXvMC->savePutImage(pScrn, src_x, src_y, drw_x, drw_y, src_w, src_h,
                        drw_w, drw_h, id, buf, width, height, sync, clipBoxes,
			data, pDraw);
    pI830->IsXvMCSurface = 0;
    return ret;
}

static Bool i915_xvmc_init(ScrnInfoPtr pScrn, XF86VideoAdaptorPtr XvAdapt)
{
    I915XvMCPtr pXvMC;

    pXvMC = (I915XvMCPtr)xcalloc(1, sizeof(I915XvMC));
    if (!pXvMC) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "[XvMC] alloc driver private failed!\n");
        return FALSE;
    }
    xvmc_driver->devPrivate = (void*)pXvMC;
    if (!intel_xvmc_init_batch(pScrn)) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		"[XvMC] fail to init batch buffer\n");
	xfree(pXvMC);
	return FALSE;
    }
    initI915XvMC(pXvMC);

    /* set up wrappers */
    pXvMC->savePutImage = XvAdapt->PutImage;
    XvAdapt->PutImage = i915_xvmc_put_image;
    return TRUE;
}

static void i915_xvmc_fini(ScrnInfoPtr pScrn)
{
    I915XvMCPtr pXvMC = (I915XvMCPtr)xvmc_driver->devPrivate;

    cleanupI915XvMC(pXvMC);
    intel_xvmc_fini_batch(pScrn);
    xfree(xvmc_driver->devPrivate);
}

/* Fill in the device dependent adaptor record.
 * This is named "Intel(R) Textured Video" because this code falls under the
 * XV extenstion, the name must match or it won't be used.
 *
 * Surface and Subpicture - see above
 * Function pointers to functions below
 */
static XF86MCAdaptorRec pAdapt =
{
    .name		= "Intel(R) Textured Video",
    .num_surfaces	= ARRARY_SIZE(ppSI),
    .surfaces		= ppSI,
#if 0
    .num_subpictures	= ARRARY_SIZE(i915_subpicture_list),
    .subpictures	= i915_subpicture_list,
#endif
    .num_subpictures	= 0,
    .subpictures	= NULL,
    .CreateContext	= (xf86XvMCCreateContextProcPtr) i915_xvmc_create_context,
    .DestroyContext	= (xf86XvMCDestroyContextProcPtr) i915_xvmc_destroy_context,
    .CreateSurface	= (xf86XvMCCreateSurfaceProcPtr) i915_xvmc_create_surface,
    .DestroySurface	= (xf86XvMCDestroySurfaceProcPtr) i915_xvmc_destroy_surface,
    .CreateSubpicture	= (xf86XvMCCreateSubpictureProcPtr) i915_xvmc_create_subpict,
    .DestroySubpicture	= (xf86XvMCDestroySubpictureProcPtr) i915_xvmc_destroy_subpict,
};

/* new xvmc driver interface */
struct intel_xvmc_driver i915_xvmc_driver = {
    .name		= "i915_xvmc",
    .adaptor		= &pAdapt,
    .flag		= XVMC_I915_MPEG2_MC,
    .init		= i915_xvmc_init,
    .fini		= i915_xvmc_fini,
};
