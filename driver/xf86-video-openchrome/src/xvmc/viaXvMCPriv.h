/*****************************************************************************
 * VIA Unichrome XvMC extension client lib.
 *
 * Copyright (c) 2004 Thomas Hellström. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHOR(S) OR COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _VIAXVMCPRIV_H
#define _VIAXVMCPRIV_H 1

#include <X11/Xlibint.h>
#include <X11/extensions/XvMC.h>
#include <X11/extensions/XvMClib.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include "vldXvMC.h"
#include "via_xvmc.h"

typedef struct
{
    int x;
    int y;
    int w;
    int h;
} XvMCRegion;

extern Status _xvmc_create_context(Display * dpy, XvMCContext * context,
    int *priv_count, uint ** priv_data);
extern Status _xvmc_destroy_context(Display * dpy, XvMCContext * context);
extern Status _xvmc_create_surface(Display * dpy, XvMCContext * context,
    XvMCSurface * surface, int *priv_count, uint ** priv_data);
extern Status _xvmc_destroy_surface(Display * dpy, XvMCSurface * surface);
extern Status _xvmc_create_subpicture(Display * dpy, XvMCContext * context,
    XvMCSubpicture * subpicture, int *priv_count, uint ** priv_data);
extern Status _xvmc_destroy_subpicture(Display * dpy,
    XvMCSubpicture * subpicture);

#define VIA_SUBPIC_PALETTE_SIZE 16     /*Number of colors in subpicture palette */
#define VIA_CBUFFERSIZE 4096	       /*Hardware command buffer size */
#define VIA_MAX_BUFS 2		       /*Number of YUV buffers per surface */
#define VIA_MAX_RENDSURF 3	       /*Maximum numbers of surfaces per context
				        * that can answer RENDERING to a rendering
				        * query */

typedef enum
{
    context_drawHash,
    context_lowLevel,
    context_mutex,
    context_sAreaMap,
    context_fbMap,
    context_mmioMap,
    context_drmContext,
    context_fd,
    context_driConnection,
    context_context,
    context_none
} ContextRes;

typedef struct
{
    unsigned ctxNo;		       /* XvMC private context reference number */
    pthread_mutex_t ctxMutex;	       /* Mutex for multi-threading. Not used */
    drm_context_t drmcontext;	       /* The drm context */
    drm_handle_t fbOffset;	       /* Handle to drm frame-buffer area */
    drm_handle_t mmioOffset;	       /* Handle to drm mmio area */
    drm_handle_t sAreaOffset;	       /* Handle to drm shared memory area */
    unsigned fbSize;		       /* Size of drm frame-buffer area */
    unsigned mmioSize;		       /* Size of drm mmio area */
    unsigned sAreaSize;		       /* Size of drm shared memory area */
    unsigned sAreaPrivOffset;	       /* Offset in sarea to private part */
    drmAddress fbAddress;	       /* Virtual address of frame buffer area */
    drmAddress mmioAddress;	       /* Virtual address of mmio area */
    drmAddress sAreaAddress;	       /* Virtual address of shared memory area */
    char busIdString[21];	       /* Busid of video card */
    unsigned yStride;		       /* Y stride of surfaces in this context */
    int fd;			       /* FD for connection to drm module */
    unsigned char intra_quantiser_matrix[64];
    unsigned char non_intra_quantiser_matrix[64];
    unsigned char chroma_intra_quantiser_matrix[64];
    unsigned char chroma_non_intra_quantiser_matrix[64];
    unsigned rendSurf[VIA_MAX_RENDSURF];	/* Which surfaces answer rendering to
						 * a rendering query */
    int decoderOn;		       /* Decoder switched on ? */
    int intraLoaded;		       /* Intra quantiser matrix loaded in
				        * decoder? */
    int nonIntraLoaded;		       /* Non-Intra quantiser matrix loaded
				        * in decoder */
    int chromaIntraLoaded;
    int chromaNonIntraLoaded;
    int haveDecoder;		       /* Does this context own decoder? */
    int attribChanged;		       /* Attributes have changed and need to
				        * be uploaded to Xv at next frame
				        * display */
    drmLockPtr hwLock;		       /* Virtual address Pointer to the
				        * heavyweight drm hardware lock */
    unsigned xvMCPort;		       /* XvMC private port. Corresponds to
				        * an Xv port, but not by number */
    ViaXvMCAttrHolder attrib;	       /* This contexts attributes and their
				        * values */
    XvAttribute attribDesc[VIA_NUM_XVMC_ATTRIBUTES];	/* Attribute decriptions */
    int useAGP;			       /* Use the AGP ringbuffer to upload data to the chip */
    void *xl;			       /* Lowlevel context. Opaque to us. */
    int haveXv;			       /* Have I initialized the Xv
				        * connection for this surface? */
    XvImage *xvImage;		       /* Fake Xv Image used for command
				        * buffer transport to the X server */
    GC  gc;			       /* X GC needed for displaying */
    Drawable draw;		       /* Drawable to undisplay from */
    XvPortID port;		       /* Xv Port ID when displaying */
    int lastSrfDisplaying;
    ContextRes resources;
    CARD32 timeStamp;
    CARD32 videoTimeStamp;
    XID id;
    unsigned screen;
    unsigned depth;
    unsigned stride;
    XVisualInfo visualInfo;
    void *drawHash;
    CARD32 chipId;
    XvMCRegion sRegion;
    XvMCRegion dRegion;
} ViaXvMCContext;

typedef struct
{
    pthread_mutex_t subMutex;	       /* Currently not used. */
    unsigned srfNo;		       /* XvMC private surface number */
    unsigned offset;		       /* Offset into frame-buffer area */
    unsigned stride;		       /* Storage stride */
    unsigned width;		       /* Width */
    unsigned height;		       /* Height */
    CARD32 palette[VIA_SUBPIC_PALETTE_SIZE];	/* YUV Palette */
    ViaXvMCContext *privContext;       /* Pointer to context private data */
    int ia44;			       /* IA44 or AI44 format */
    int needsSync;
    CARD32 timeStamp;
} ViaXvMCSubPicture;

typedef struct
{
    pthread_mutex_t srfMutex;	       /* For multithreading. Not used. */
    pthread_cond_t bufferAvailable;    /* For multithreading. Not used. */
    unsigned srfNo;		       /* XvMC private surface numbers */
    unsigned numBuffers;	       /* Number of picture buffers */
    unsigned curBuf;		       /* Which is the current buffer? */
    unsigned offsets[VIA_MAX_BUFS];    /* Offsets of picture buffers
				        * into the frame-buffer area */
    unsigned yStride;		       /* Stride of YUV420 Y component. */
    unsigned width;		       /* Dimensions */
    unsigned height;
    int progressiveSequence;	       /* Mpeg progressive picture? Hmm? */
    ViaXvMCContext *privContext;       /* XvMC context private part. */
    ViaXvMCSubPicture *privSubPic;     /* Subpicture to be blended when
				        * displaying. NULL if none. */
    int needsSync;
    int syncMode;
    CARD32 timeStamp;
    int topFieldFirst;
} ViaXvMCSurface;

/*
 * Take and release the global drm hardware lock.
 */

#define HW_LOCK(ctx)						\
    DRM_LOCK((ctx)->fd,(ctx)->hwLock,(ctx)->drmcontext,0)
#define HW_UNLOCK(ctx)						\
    DRM_UNLOCK((ctx)->fd,(ctx->hwLock),(ctx)->drmcontext)

/*
 * Low-level Mpeg functions in viaLowLevel.c
 */

#define VIABLIT_TRANSCOPY 0
#define VIABLIT_COPY 1
#define VIABLIT_FILL 2

#endif
