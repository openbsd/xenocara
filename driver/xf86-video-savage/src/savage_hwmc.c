/*
 * Copyright 1998-2003 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2003 S3 Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * VIA, S3 GRAPHICS, AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "compiler.h"
#include "xf86Pci.h"
#include "xf86fbman.h"
#include "regionstr.h"

#include "savage_driver.h"
#include "savage_dri.h"

#include "xf86xv.h"
#include "xf86xvmc.h"
#include <X11/extensions/Xv.h>
#include <X11/extensions/XvMC.h>
#ifdef HAVE_XAA_H
#include "xaalocal.h"
#endif
#include "dixstruct.h"
#include "fourcc.h"

#define SAVAGE_MAX_SURFACES    5 
#define SAVAGE_MAX_SUBPICTURES 1

#define XVMC_IDCT_8BIT         0x80000000


int SAVAGEXvMCCreateContext (ScrnInfoPtr pScrn, XvMCContextPtr pContext,
                           int *num_priv, long **priv );
void SAVAGEXvMCDestroyContext (ScrnInfoPtr pScrn, XvMCContextPtr pContext);

int SAVAGEXvMCCreateSurface (ScrnInfoPtr pScrn, XvMCSurfacePtr pSurf,
                           int *num_priv, long **priv );
void SAVAGEXvMCDestroySurface (ScrnInfoPtr pScrn, XvMCSurfacePtr pSurf);

int SAVAGEXvMCCreateSubpicture (ScrnInfoPtr pScrn, XvMCSubpicturePtr pSurf,
                               int *num_priv, long **priv );
void SAVAGEXvMCDestroySubpicture (ScrnInfoPtr pScrn, XvMCSubpicturePtr pSurf);


typedef struct {
  drm_context_t drmcontext;
  unsigned int fbBase;
  unsigned int MMIOHandle;
  unsigned int MMIOSize;
  unsigned int SurfaceHandle;
  unsigned int SurfaceOffset;
  unsigned int SurfaceSize;
  unsigned int DCTBlockHandle;
  unsigned int DCTBlockOffset;
  unsigned int DCTBlockSize;
  unsigned int ApertureHandle;
  unsigned int ApertureSize;
  unsigned int bitsPerPixel;
  unsigned int frameX0;
  unsigned int frameY0;
  unsigned int IOBase;
  unsigned int displayWidth;
  char busIdString[10];
  char pad[2];
} SAVAGEXvMCCreateContextRec;


static int yv12_subpicture_index_list[1] = 
{
  FOURCC_IA44
};

static XF86MCImageIDList yv12_subpicture_list =
{
  1,
  yv12_subpicture_index_list
};
 
static XF86MCSurfaceInfoRec savage_YV12_mpg2_surface =
{
    FOURCC_YV12,  
    XVMC_CHROMA_FORMAT_420,
    0,
    720,
    576,
    720,
    576,
    XVMC_MPEG_2,
    XVMC_OVERLAID_SURFACE | XVMC_INTRA_UNSIGNED | XVMC_BACKEND_SUBPICTURE | XVMC_IDCT_8BIT,
    &yv12_subpicture_list
};

static XF86MCSurfaceInfoPtr ppSI[1] = 
{
    (XF86MCSurfaceInfoPtr)&savage_YV12_mpg2_surface,
};

/* List of subpicture types that we support */
static XF86ImageRec ia44_subpicture = XVIMAGE_IA44;

static XF86ImagePtr savage_subpicture_list[1] =
{
  (XF86ImagePtr)&ia44_subpicture,
};

/* Fill in the device dependent adaptor record. 
 * This is named "SAVAGE Video Overlay" because this code falls under the
 * XV extenstion, the name must match or it won't be used.
 *
 * Surface and Subpicture - see above
 * Function pointers to functions below
 */
static XF86MCAdaptorRec pAdapt = 
{
  "Savage Streams Engine",	/* name */
  1,				/* num_surfaces */
  ppSI,				/* surfaces */
  1,				/* num_subpictures */
  savage_subpicture_list,		/* subpictures */
  (xf86XvMCCreateContextProcPtr)SAVAGEXvMCCreateContext,
  (xf86XvMCDestroyContextProcPtr)SAVAGEXvMCDestroyContext,
  (xf86XvMCCreateSurfaceProcPtr)SAVAGEXvMCCreateSurface,
  (xf86XvMCDestroySurfaceProcPtr)SAVAGEXvMCDestroySurface,
  (xf86XvMCCreateSubpictureProcPtr)SAVAGEXvMCCreateSubpicture,
  (xf86XvMCDestroySubpictureProcPtr)SAVAGEXvMCDestroySubpicture
};

static XF86MCAdaptorPtr ppAdapt[1] = 
{
	(XF86MCAdaptorPtr)&pAdapt
};

/**************************************************************************
 *
 *  SAVAGEInitMC
 *
 *  Initialize the hardware motion compenstation extention for this 
 *  hardware. The initialization routines want the address of the pointers
 *  to the structures, not the address of the structures. This means we
 *  allocate (or create static?) the pointer memory and pass that 
 *  address. This seems a little convoluted.
 *
 *  We need to allocate memory for the device depended adaptor record. 
 *  This is what holds the pointers to all our device functions.
 *
 *  We need to map the overlay registers into the drm.
 *
 *  We need to map the surfaces into the drm.
 *
 *  Inputs:
 *    Screen pointer
 *
 *  Outputs:
 *    None, this calls the device independent screen initialization 
 *    function.
 *
 *  Revisions:
 *  
 **************************************************************************/
Bool SAVAGEInitMC(ScreenPtr pScreen)
{
  ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
  SavagePtr pSAVAGE = SAVPTR(pScrn);
  DRIInfoPtr pDRIInfo = pSAVAGE->pDRIInfo;
  SAVAGEDRIPtr pSAVAGEDriPriv = (SAVAGEDRIPtr)pDRIInfo->devPrivate;
  int i;
  unsigned int offset;

  /* Clear the Surface Allocation */
  for(i=0; i<SAVAGE_MAX_SURFACES; i++) {
	pSAVAGE->surfaceAllocation[i] = 0;
  } 

  if(pSAVAGE->hwmcSize == 0)
  {
    xf86DrvMsg(X_ERROR, pScrn->scrnIndex,
        "SAVAGEInitMC: There is not enough memory!\n");
    return FALSE;
  }

  offset = pSAVAGE->hwmcOffset + pSAVAGE->FbRegion.base;

  if(drmAddMap(pSAVAGE->drmFD, offset, pSAVAGE->hwmcSize,
               DRM_FRAME_BUFFER, 0, &pSAVAGEDriPriv->xvmcSurfHandle) < 0)
  {
      
    xf86DrvMsg(X_ERROR, pScrn->scrnIndex,
        "SAVAGEInitMC: Cannot add map to drm!\n");
    return FALSE;
  } 

  return xf86XvMCScreenInit(pScreen, 1, ppAdapt);
}

/**************************************************************************
 *
 *  SAVAGEXvMCCreateContext
 *
 *  Some info about the private data:
 *
 *  Set *num_priv to the number of 32bit words that make up the size of
 *  of the data that priv will point to.
 *
 *  *priv = (long *) calloc (elements, sizeof(element))
 *  *num_priv = (elements * sizeof(element)) >> 2;
 *
 **************************************************************************/

int SAVAGEXvMCCreateContext (ScrnInfoPtr pScrn, XvMCContextPtr pContext,
                            int *num_priv, long **priv )
{
  SavagePtr pSAVAGE = SAVPTR(pScrn);
  DRIInfoPtr pDRIInfo = pSAVAGE->pDRIInfo;
  SAVAGEDRIPtr pSAVAGEDriPriv = (SAVAGEDRIPtr)pDRIInfo->devPrivate;
  SAVAGEDRIServerPrivatePtr pSAVAGEDRIServer = pSAVAGE->DRIServerInfo;
  SAVAGEXvMCCreateContextRec *contextRec;
  vgaHWPtr hwp = VGAHWPTR(pScrn);


  if(!pSAVAGE->directRenderingEnabled) {
    xf86DrvMsg(X_ERROR, pScrn->scrnIndex,
        "SAVAGEXvMCCreateContext: Cannot use XvMC without DRI!\n");
    return BadAlloc;
  }

  /* Context Already in use! */
  if(pSAVAGE->xvmcContext) {
    xf86DrvMsg(X_WARNING, pScrn->scrnIndex,
        "SAVAGEXvMCCreateContext: 2 XvMC Contexts Attempted, not supported.\n");
    return BadAlloc;
  }

  *priv = calloc(1,sizeof(SAVAGEXvMCCreateContextRec));
  contextRec = (SAVAGEXvMCCreateContextRec *)*priv;

  if(!*priv) {
    *num_priv = 0;
    return(BadAlloc);
  }

  *num_priv = sizeof(SAVAGEXvMCCreateContextRec) >> 2;

  if(drmCreateContext(pSAVAGE->drmFD, &(contextRec->drmcontext) ) < 0) {
    xf86DrvMsg(X_ERROR, pScrn->scrnIndex,
        "SAVAGEXvMCCreateContext: Unable to create DRMContext!\n");
    free(*priv);
    return(BadAlloc);
  }

  drmAuthMagic(pSAVAGE->drmFD, pContext->flags);

  pSAVAGE->xvmcContext = contextRec->drmcontext;
  contextRec->fbBase = pScrn->memPhysBase;

  contextRec->MMIOHandle = pSAVAGEDRIServer->registers.handle;
  contextRec->MMIOSize = pSAVAGEDRIServer->registers.size;

  contextRec->DCTBlockHandle = pSAVAGEDRIServer->agpTextures.handle;
  contextRec->DCTBlockOffset = pSAVAGEDRIServer->agpTextures.offset;
  contextRec->DCTBlockSize = pSAVAGEDRIServer->agpTextures.size;

  contextRec->SurfaceHandle = pSAVAGEDriPriv->xvmcSurfHandle;
  contextRec->SurfaceOffset = pSAVAGE->hwmcOffset;
  contextRec->SurfaceSize = pSAVAGE->hwmcSize;

  contextRec->ApertureHandle = pSAVAGEDriPriv->apertureHandle;
  contextRec->ApertureSize = pSAVAGEDriPriv->apertureSize;

  contextRec->bitsPerPixel = pScrn->bitsPerPixel;
  contextRec->frameX0 = pScrn->frameX0;
  contextRec->frameY0 = pScrn->frameY0;
  contextRec->IOBase = hwp->IOBase;
  contextRec->displayWidth = pScrn->displayWidth;


  strncpy (contextRec->busIdString, pDRIInfo->busIdString, 9);

  return Success;
}


int SAVAGEXvMCCreateSurface (ScrnInfoPtr pScrn, XvMCSurfacePtr pSurf,
                           int *num_priv, long **priv )
{
  SavagePtr pSAVAGE = SAVPTR(pScrn);
  int i;
  /* This size is used for flip, mixer, subpicture and palette buffers*/
  unsigned int offset = ((786*576*2 + 2048)*5 + 2048) & 0xfffff800; 

  *priv = (long *)calloc(2,sizeof(long));

  if(!*priv) {
    xf86DrvMsg(X_ERROR, pScrn->scrnIndex,
        "SAVAGEXvMCCreateSurface: Unable to allocate memory!\n");
    *num_priv = 0;
    return (BadAlloc);
  }
  *num_priv = 1;

  /* Surface Arrangement is different based on 6 or 7 Surfaces */
  for(i = 0; i < SAVAGE_MAX_SURFACES; i++) {
      if(!pSAVAGE->surfaceAllocation[i]) {
         pSAVAGE->surfaceAllocation[i] = pSurf->surface_id;
         (*priv)[0] =  offset + (576 * 786 * 2 + 2048) * i;
         /* UV data starts at 0 offset, each set is 288k * */
         return Success;
      }
  }

  (*priv)[0] = 0;
  return BadAlloc;
}

int SAVAGEXvMCCreateSubpicture (ScrnInfoPtr pScrn, XvMCSubpicturePtr pSubp,
                              int *num_priv, long **priv )
{
  SavagePtr pSAVAGE = SAVPTR(pScrn);
  int i;

  *priv = (long *)calloc(1,sizeof(long));

  if(!*priv) {
    xf86DrvMsg(X_ERROR, pScrn->scrnIndex,
        "SAVAGEXvMCCreateSubpicture: Unable to allocate memory!\n");
    *num_priv = 0;
    return (BadAlloc);
  }

  *num_priv = 1;

  for(i = SAVAGE_MAX_SURFACES; i < SAVAGE_MAX_SURFACES + SAVAGE_MAX_SUBPICTURES; i++) {  
      if(!pSAVAGE->surfaceAllocation[i]) {
          pSAVAGE->surfaceAllocation[i] = pSubp->subpicture_id;
          (*priv)[0] = ( 576*1024 * i);
          return Success;
     }
  }

  (*priv)[0] = 0;
  return BadAlloc;
}

void SAVAGEXvMCDestroyContext (ScrnInfoPtr pScrn, XvMCContextPtr pContext)
{
  SavagePtr pSAVAGE = SAVPTR(pScrn);

  drmDestroyContext(pSAVAGE->drmFD,pSAVAGE->xvmcContext);
  pSAVAGE->xvmcContext = 0;
}

void SAVAGEXvMCDestroySurface (ScrnInfoPtr pScrn, XvMCSurfacePtr pSurf)
{
  SavagePtr pSAVAGE = SAVPTR(pScrn);
  int i;

  for(i=0; i<SAVAGE_MAX_SURFACES; i++) {
    if(pSAVAGE->surfaceAllocation[i] == pSurf->surface_id) {
      pSAVAGE->surfaceAllocation[i] = 0;
      return;
    }
  }
  return;
}

void SAVAGEXvMCDestroySubpicture (ScrnInfoPtr pScrn, XvMCSubpicturePtr pSubp)
{
  SavagePtr pSAVAGE = SAVPTR(pScrn);
  int i;

  for(i = SAVAGE_MAX_SURFACES; i < SAVAGE_MAX_SURFACES + SAVAGE_MAX_SUBPICTURES; i++) {
    if(pSAVAGE->surfaceAllocation[i] == pSubp->subpicture_id) {
      pSAVAGE->surfaceAllocation[i] = 0;
      return;
    }
  }
  return;
}






