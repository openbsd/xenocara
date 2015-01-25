/*
 * Copyright 2007 by VMware, Inc.
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */

/*
 * vmwarevideo.c --
 *
 *      Xv extension support.
 *      See http://www.xfree86.org/current/DESIGN16.html
 *
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "vmware.h"
#include "vmware_common.h"
#include "xf86xv.h"
#include "fourcc.h"
#include "svga_escape.h"
#include "svga_overlay.h"

#include <X11/extensions/Xv.h>

#ifndef HAVE_XORG_SERVER_1_5_0
#include <xf86_ansic.h>
#include <xf86_libc.h>
#endif


#define HAVE_FILLKEYHELPERDRAWABLE \
    ((GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 2) ||  \
     ((GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) == 1) && \
      (GET_ABI_MINOR(ABI_VIDEODRV_VERSION) >= 2)))

#if HAVE_FILLKEYHELPERDRAWABLE
#include <damage.h>
#endif

#define MAKE_ATOM(a) MakeAtom(a, sizeof(a) - 1, TRUE)

/*
 * Used to pack structs
 */
#define PACKED __attribute__((__packed__))

/*
 * Number of videos that can be played simultaneously
 */
#define VMWARE_VID_NUM_PORTS 1

/*
 * Using a dark shade as the default colorKey
 */
#define VMWARE_VIDEO_COLORKEY 0x100701

/*
 * Maximum dimensions
 */
#define VMWARE_VID_MAX_WIDTH    2048
#define VMWARE_VID_MAX_HEIGHT   2048

#define VMWARE_VID_NUM_ENCODINGS 1
static XF86VideoEncodingRec vmwareVideoEncodings[] =
{
    {
       0,
       "XV_IMAGE",
       VMWARE_VID_MAX_WIDTH, VMWARE_VID_MAX_HEIGHT,
       {1, 1}
    }
};

#define VMWARE_VID_NUM_FORMATS 2
static XF86VideoFormatRec vmwareVideoFormats[] =
{
    { 16, TrueColor},
    { 24, TrueColor}
};

#define VMWARE_VID_NUM_IMAGES 3
static XF86ImageRec vmwareVideoImages[] =
{
    XVIMAGE_YV12,
    XVIMAGE_YUY2,
    XVIMAGE_UYVY
};

#define VMWARE_VID_NUM_ATTRIBUTES 2
static XF86AttributeRec vmwareVideoAttributes[] =
{
    {
        XvGettable | XvSettable,
        0x000000,
        0xffffff,
        "XV_COLORKEY"
    },
    {
        XvGettable | XvSettable,
        0,
        1,
        "XV_AUTOPAINT_COLORKEY"
    }
};

/*
 * Video frames are stored in a circular list of buffers.
 */
#define VMWARE_VID_NUM_BUFFERS 1
/*
 * Defines the structure used to hold and pass video data to the host
 */
typedef struct {
   uint32  dataOffset;
   pointer data;
} VMWAREVideoBuffer;

typedef struct {
   uint32 size;
   uint32 offset;
} VMWAREOffscreenRec, *VMWAREOffscreenPtr;

/*
 * Trivial offscreen manager that allocates memory from the
 * bottom of the VRAM.
 */
static VMWAREOffscreenRec offscreenMgr;

/*
 * structs that reside in fmt_priv.
 */
typedef struct {
    int pitches[3];
    int offsets[3];
} VMWAREVideoFmtData;

/*
 * Structure representing a specific video stream.
 */
struct VMWAREVideoRec {
   uint32             streamId;
   /*
    * Function prototype same as XvPutImage.
    */
   int                (*play)(ScrnInfoPtr, struct VMWAREVideoRec *,
                              short, short, short, short, short,
                              short, short, short, int, unsigned char*,
                              short, short, RegionPtr, DrawablePtr);
   /*
    * Offscreen memory region used to pass video data to the host.
    */
   VMWAREOffscreenPtr fbarea;
   VMWAREVideoBuffer  bufs[VMWARE_VID_NUM_BUFFERS];
   uint8              currBuf;
   uint32             size;
   uint32             colorKey;
   Bool               isAutoPaintColorkey;
   uint32             flags;
   RegionRec          clipBoxes;
   VMWAREVideoFmtData *fmt_priv;
};

typedef struct VMWAREVideoRec VMWAREVideoRec;
typedef VMWAREVideoRec *VMWAREVideoPtr;

/*
 * Callback functions
 */
#if (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 1)
static int vmwareXvPutImage(ScrnInfoPtr pScrn, short src_x, short src_y,
                            short drw_x, short drw_y, short src_w, short src_h,
                            short drw_w, short drw_h, int image,
                            unsigned char *buf, short width, short height,
                            Bool sync, RegionPtr clipBoxes, pointer data,
                            DrawablePtr dst);
#else
static int vmwareXvPutImage(ScrnInfoPtr pScrn, short src_x, short src_y,
                            short drw_x, short drw_y, short src_w, short src_h,
                            short drw_w, short drw_h, int image,
                            unsigned char *buf, short width, short height,
                            Bool sync, RegionPtr clipBoxes, pointer data);
#endif
static void vmwareStopVideo(ScrnInfoPtr pScrn, pointer data, Bool Cleanup);
static int vmwareQueryImageAttributes(ScrnInfoPtr pScrn, int format,
                                      unsigned short *width,
                                      unsigned short *height, int *pitches,
                                      int *offsets);
static int vmwareSetPortAttribute(ScrnInfoPtr pScrn, Atom attribute,
                                  INT32 value, pointer data);
static int vmwareGetPortAttribute(ScrnInfoPtr pScrn, Atom attribute,
                                  INT32 *value, pointer data);
static void vmwareQueryBestSize(ScrnInfoPtr pScrn, Bool motion,
                                short vid_w, short vid_h, short drw_w,
                                short drw_h, unsigned int *p_w,
                                unsigned int *p_h, pointer data);

/*
 * Local functions for video streams
 */
static XF86VideoAdaptorPtr vmwareVideoSetup(ScrnInfoPtr pScrn);
static int vmwareVideoInitStream(ScrnInfoPtr pScrn, VMWAREVideoPtr pVid,
                                 short src_x, short src_y, short drw_x,
                                 short drw_y, short src_w, short src_h,
                                 short drw_w, short drw_h, int format,
                                 unsigned char *buf, short width,
                                 short height, RegionPtr clipBoxes,
				 DrawablePtr draw);
static int vmwareVideoInitAttributes(ScrnInfoPtr pScrn, VMWAREVideoPtr pVid,
                                     int format, unsigned short width,
                                     unsigned short height);
static int vmwareVideoPlay(ScrnInfoPtr pScrn, VMWAREVideoPtr pVid,
                           short src_x, short src_y, short drw_x,
                           short drw_y, short src_w, short src_h,
                           short drw_w, short drw_h, int format,
                           unsigned char *buf, short width,
                           short height, RegionPtr clipBoxes,
			   DrawablePtr draw);
static void vmwareVideoFlush(VMWAREPtr pVMWARE, uint32 streamId);
static void vmwareVideoSetOneReg(VMWAREPtr pVMWARE, uint32 streamId,
                                 uint32 regId, uint32 value);
static void vmwareVideoEndStream(ScrnInfoPtr pScrn, VMWAREVideoPtr pVid);

/*
 * Offscreen memory manager functions
 */
static void vmwareOffscreenInit(void);
static VMWAREOffscreenPtr vmwareOffscreenAllocate(VMWAREPtr pVMWARE,
                                                  uint32 size);
static void vmwareOffscreenFree(VMWAREOffscreenPtr memptr);


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareCheckVideoSanity --
 *
 *    Ensures that on ModeSwitch the offscreen memory used
 *    by the Xv streams doesn't become part of the guest framebuffer.
 *
 * Results:
 *    None
 *
 * Side effects:
 *    If it is found that the offscreen used by video streams  lies
 *    within the range of the framebuffer(after ModeSwitch) then the video
 *    streams will be stopped.
 *
 *-----------------------------------------------------------------------------
 */

void
vmwareCheckVideoSanity(ScrnInfoPtr pScrn)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    VMWAREVideoPtr pVid;

   if (offscreenMgr.size == 0 ||
       offscreenMgr.offset > pVMWARE->FbSize) {
       return ;
   }

   pVid = (VMWAREVideoPtr) &pVMWARE->videoStreams[VMWARE_VID_NUM_PORTS];
   vmwareStopVideo(pScrn, pVid, TRUE);
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareOffscreenInit --
 *
 *    Initializes the trivial Offscreen memory manager.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    Initializes the Offscreen manager meta-data structure.
 *
 *-----------------------------------------------------------------------------
 */

static void
vmwareOffscreenInit(void)
{
    offscreenMgr.size = 0;
    offscreenMgr.offset  = 0;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareOffscreenAllocate --
 *
 *    Allocates offscreen memory.
 *    Memory is allocated from the bottom part of the VRAM.
 *    The memory manager is trivial iand can handle only 1 video-stream.
 *    ----------
 *    |        |
 *    |  FB    |
 *    |        |
 *    |---------
 *    |        |
 *    |        |
 *    |--------|
 *    | Offscr |
 *    |--------|
 *
 *      VRAM
 *
 * Results:
 *    Pointer to the allocated Offscreen memory.
 *
 * Side effects:
 *    Updates the Offscreen memory manager meta-data structure.
 *
 *-----------------------------------------------------------------------------
 */

static VMWAREOffscreenPtr
vmwareOffscreenAllocate(VMWAREPtr pVMWARE, uint32 size)
{
    VMWAREOffscreenPtr memptr;

    if ((pVMWARE->videoRam - pVMWARE->FbSize - pVMWARE->fbPitch - 7) < size) {
        return NULL;
    }

    memptr = malloc(sizeof(VMWAREOffscreenRec));
    if (!memptr) {
        return NULL;
    }
    memptr->size = size;
    memptr->offset  = (pVMWARE->videoRam - size) & ~7;

    VmwareLog(("vmwareOffscreenAllocate: Offset:%x", memptr->offset));

    offscreenMgr.size = memptr->size;
    offscreenMgr.offset = memptr->offset;
    return memptr;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareOffscreenFree --
 *
 *    Frees the allocated offscreen memory.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    Updates the Offscreen memory manager meta-data structure.
 *
 *-----------------------------------------------------------------------------
 */

static void
vmwareOffscreenFree(VMWAREOffscreenPtr memptr)
{
    if (memptr) {
        free(memptr);
    }

    offscreenMgr.size = 0;
    offscreenMgr.offset = 0;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareVideoEnabled --
 *
 *    Checks if Video FIFO and Escape FIFO cap are enabled.
 *
 * Results:
 *    TRUE if required caps are enabled, FALSE otherwise.
 *
 * Side effects:
 *    None.
 *
 *-----------------------------------------------------------------------------
 */

Bool
vmwareVideoEnabled(VMWAREPtr pVMWARE)
{
    return ((pVMWARE->vmwareCapability & SVGA_CAP_EXTENDED_FIFO) &&
            (pVMWARE->vmwareFIFO[SVGA_FIFO_CAPABILITIES] &
             (SVGA_FIFO_CAP_VIDEO | SVGA_FIFO_CAP_ESCAPE)));
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareVideoInit --
 *
 *    Initializes Xv support.
 *
 * Results:
 *    TRUE on success, FALSE on error.
 *
 * Side effects:
 *    Xv support is initialized. Memory is allocated for all supported
 *    video streams.
 *
 *-----------------------------------------------------------------------------
 */

Bool
vmwareVideoInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    XF86VideoAdaptorPtr *overlayAdaptors, *newAdaptors = NULL;
    XF86VideoAdaptorPtr newAdaptor = NULL;
    int numAdaptors;

    TRACEPOINT

    vmwareOffscreenInit();

    numAdaptors = xf86XVListGenericAdaptors(pScrn, &overlayAdaptors);

    newAdaptor = vmwareVideoSetup(pScrn);
    if (!newAdaptor) {
        VmwareLog(("Failed to initialize Xv extension \n"));
        return FALSE;
    }

    if (!numAdaptors) {
        numAdaptors = 1;
        overlayAdaptors = &newAdaptor;
    } else {
         newAdaptors = malloc((numAdaptors + 1) *
                              sizeof(XF86VideoAdaptorPtr*));
         if (!newAdaptors) {
            xf86XVFreeVideoAdaptorRec(newAdaptor);
            return FALSE;
         }

         memcpy(newAdaptors, overlayAdaptors,
                numAdaptors * sizeof(XF86VideoAdaptorPtr));
         newAdaptors[numAdaptors++] = newAdaptor;
         overlayAdaptors = newAdaptors;
    }

    if (!xf86XVScreenInit(pScreen, overlayAdaptors, numAdaptors)) {
        VmwareLog(("Failed to initialize Xv extension\n"));
        xf86XVFreeVideoAdaptorRec(newAdaptor);
        return FALSE;
    }

    if (newAdaptors) {
        free(newAdaptors);
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "Initialized VMware Xv extension successfully.\n");
    return TRUE;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareVideoEnd --
 *
 *    Unitializes video.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    pVMWARE->videoStreams = NULL
 *
 *-----------------------------------------------------------------------------
 */

void
vmwareVideoEnd(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    VMWAREVideoPtr pVid;
    int i;

    TRACEPOINT

    /*
     * Video streams are allocated after the DevUnion array
     * (see VideoSetup)
     */
    pVid = (VMWAREVideoPtr) &pVMWARE->videoStreams[VMWARE_VID_NUM_PORTS];
    for (i = 0; i < VMWARE_VID_NUM_PORTS; ++i) {
        vmwareVideoEndStream(pScrn, &pVid[i]);
	REGION_UNINIT(pScreen, &pVid[i].clipBoxes);
    }

    free(pVMWARE->videoStreams);
    pVMWARE->videoStreams = NULL;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareVideoSetup --
 *
 *    Initializes a XF86VideoAdaptor structure with the capabilities and
 *    functions supported by this video driver.
 *
 * Results:
 *    On success initialized XF86VideoAdaptor struct or NULL on error
 *
 * Side effects:
 *    None.
 *
 *-----------------------------------------------------------------------------
 */

static XF86VideoAdaptorPtr
vmwareVideoSetup(ScrnInfoPtr pScrn)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    XF86VideoAdaptorPtr adaptor;
    VMWAREVideoPtr pPriv;
    DevUnion *du;
    int i;

    TRACEPOINT

    adaptor = xf86XVAllocateVideoAdaptorRec(pScrn);
    if (!adaptor) {
        VmwareLog(("Not enough memory\n"));
        return NULL;
    }
    du = calloc(1, VMWARE_VID_NUM_PORTS *
        (sizeof(DevUnion) + sizeof(VMWAREVideoRec)));

    if (!du) {
        VmwareLog(("Not enough memory.\n"));
        xf86XVFreeVideoAdaptorRec(adaptor);
        return NULL;
    }

    adaptor->type = XvInputMask | XvImageMask | XvWindowMask;
    adaptor->flags = VIDEO_OVERLAID_IMAGES | VIDEO_CLIP_TO_VIEWPORT;
    adaptor->name = "VMware Video Engine";
    adaptor->nEncodings = VMWARE_VID_NUM_ENCODINGS;
    adaptor->pEncodings = vmwareVideoEncodings;
    adaptor->nFormats = VMWARE_VID_NUM_FORMATS;
    adaptor->pFormats = vmwareVideoFormats;
    adaptor->nPorts = VMWARE_VID_NUM_PORTS;

    pPriv = (VMWAREVideoPtr) &du[VMWARE_VID_NUM_PORTS];
    adaptor->pPortPrivates = du;

    for (i = 0; i < VMWARE_VID_NUM_PORTS; ++i) {
        pPriv[i].streamId = i;
        pPriv[i].play = vmwareVideoInitStream;
        pPriv[i].flags = SVGA_VIDEO_FLAG_COLORKEY;
        pPriv[i].colorKey = VMWARE_VIDEO_COLORKEY;
        pPriv[i].isAutoPaintColorkey = TRUE;
	REGION_NULL(pScreen, &pPriv[i].clipBoxes);
        adaptor->pPortPrivates[i].ptr = &pPriv[i];
    }
    pVMWARE->videoStreams = du;

    adaptor->nAttributes = VMWARE_VID_NUM_ATTRIBUTES;
    adaptor->pAttributes = vmwareVideoAttributes;

    adaptor->nImages = VMWARE_VID_NUM_IMAGES;
    adaptor->pImages = vmwareVideoImages;

    adaptor->PutVideo = NULL;
    adaptor->PutStill = NULL;
    adaptor->GetVideo = NULL;
    adaptor->GetStill = NULL;
    adaptor->StopVideo = vmwareStopVideo;
    adaptor->SetPortAttribute = vmwareSetPortAttribute;
    adaptor->GetPortAttribute = vmwareGetPortAttribute;
    adaptor->QueryBestSize = vmwareQueryBestSize;
    adaptor->PutImage = vmwareXvPutImage;
    adaptor->QueryImageAttributes = vmwareQueryImageAttributes;

    return adaptor;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareVideoInitStream --
 *
 *    Initializes a video stream in response to the first PutImage() on a
 *    video stream. The process goes as follows:
 *    - Figure out characteristics according to format
 *    - Allocate offscreen memory
 *    - Pass on video to Play() functions
 *
 * Results:
 *    Success or XvBadAlloc on failure.
 *
 * Side effects:
 *    Video stream is initialized and its first frame sent to the host
 *    (done by VideoPlay() function called at the end)
 *
 *-----------------------------------------------------------------------------
 */

static int
vmwareVideoInitStream(ScrnInfoPtr pScrn, VMWAREVideoPtr pVid,
                      short src_x, short src_y, short drw_x,
                      short drw_y, short src_w, short src_h,
                      short drw_w, short drw_h, int format,
                      unsigned char *buf, short width,
                      short height, RegionPtr clipBoxes,
		      DrawablePtr draw)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    int i;

    TRACEPOINT

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "Initializing Xv video-stream with id:%d format:%d\n",
                pVid->streamId, format);

    pVid->size = vmwareVideoInitAttributes(pScrn, pVid, format, width,
                                           height);

    if (pVid->size == -1) {
        VmwareLog(("Could not initialize 0x%x video stream\n", format));
        return XvBadAlloc;
    }

    pVid->play = vmwareVideoPlay;

    pVid->fbarea = vmwareOffscreenAllocate(pVMWARE,
                       pVid->size * VMWARE_VID_NUM_BUFFERS);

    if (!pVid->fbarea) {
       VmwareLog(("Could not allocate offscreen memory\n"));
       vmwareVideoEndStream(pScrn, pVid);
       return BadAlloc;
    }

    pVid->bufs[0].dataOffset = pVid->fbarea->offset;
    pVid->bufs[0].data = pVMWARE->FbBase + pVid->bufs[0].dataOffset;

    for (i = 1; i < VMWARE_VID_NUM_BUFFERS; ++i) {
        pVid->bufs[i].dataOffset = pVid->bufs[i-1].dataOffset + pVid->size;
        pVid->bufs[i].data = pVMWARE->FbBase + pVid->bufs[i].dataOffset;
    }
    pVid->currBuf = 0;

    REGION_COPY(pScrn->pScreen, &pVid->clipBoxes, clipBoxes);

    if (pVid->isAutoPaintColorkey) {
	BoxPtr boxes = REGION_RECTS(&pVid->clipBoxes);
	int nBoxes = REGION_NUM_RECTS(&pVid->clipBoxes);

#if HAVE_FILLKEYHELPERDRAWABLE
	if (draw->type == DRAWABLE_WINDOW) {
	    xf86XVFillKeyHelperDrawable(draw, pVid->colorKey, clipBoxes);
	    DamageDamageRegion(draw, clipBoxes);
	} else {
	    xf86XVFillKeyHelper(pScrn->pScreen, pVid->colorKey, clipBoxes);
        }
#else
        xf86XVFillKeyHelper(pScrn->pScreen, pVid->colorKey, clipBoxes);
#endif
	/**
	 * Force update to paint the colorkey before the overlay flush.
	 */

	while(nBoxes--)
	    vmwareSendSVGACmdUpdate(pVMWARE, boxes++);
    }

    VmwareLog(("Got offscreen region, offset %d, size %d "
               "(yuv size in bytes: %d)\n",
               pVid->fbarea->offset, pVid->fbarea->size, pVid->size));

    return pVid->play(pScrn, pVid, src_x, src_y, drw_x, drw_y, src_w, src_h,
                      drw_w, drw_h, format, buf, width, height, clipBoxes,
		      draw);
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareVideoInitAttributes --
 *
 *    Fetches the format specific attributes using QueryImageAttributes().
 *
 * Results:
 *    size of the YUV frame on success and -1 on error.
 *
 * Side effects:
 *    The video stream gets the format specific attributes(fmtData).
 *
 *-----------------------------------------------------------------------------
 */

static int
vmwareVideoInitAttributes(ScrnInfoPtr pScrn, VMWAREVideoPtr pVid,
                          int format, unsigned short width,
                          unsigned short height)
{
    int size;
    VMWAREVideoFmtData *fmtData;

    TRACEPOINT

    fmtData = calloc(1, sizeof(VMWAREVideoFmtData));
    if (!fmtData) {
        return -1;
    }

    size = vmwareQueryImageAttributes(pScrn, format, &width, &height,
                                      fmtData->pitches, fmtData->offsets);
    if (size == -1) {
        free(fmtData);
        return -1;
    }

    pVid->fmt_priv = fmtData;
    return size;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareVideoPlay --
 *
 *    Sends all the attributes associated with the video frame using the
 *    FIFO ESCAPE mechanism to the host.
 *
 * Results:
 *    Always returns Success.
 *
 * Side effects:
 *    None.
 *
 *-----------------------------------------------------------------------------
 */

static int
vmwareVideoPlay(ScrnInfoPtr pScrn, VMWAREVideoPtr pVid,
                short src_x, short src_y, short drw_x,
                short drw_y, short src_w, short src_h,
                short drw_w, short drw_h, int format,
                unsigned char *buf, short width,
                short height, RegionPtr clipBoxes,
		DrawablePtr draw)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    uint32 *fifoItem;
    int i, regId;
    struct PACKED _item {
        uint32 regId;
        uint32 value;
    };

    struct PACKED _body {
        uint32 escape;
        uint32 streamId;
        /* Old hosts can not handle more then these regs */
        struct _item items[SVGA_VIDEO_DATA_GMRID];
    };

    struct PACKED _cmdSetRegs {
        uint32 cmd;
        uint32 nsid;
        uint32 size;
        struct _body body;
    };

    struct _cmdSetRegs cmdSetRegs;
    struct _item *items;
    int size;
    VMWAREVideoFmtData *fmtData;
    unsigned short w, h;

    w = width;
    h = height;
    fmtData = pVid->fmt_priv;

    size = vmwareQueryImageAttributes(pScrn, format, &w, &h,
                                      fmtData->pitches, fmtData->offsets);

    if (size > pVid->size) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Increase in size of Xv video "
                   "frame streamId:%d.\n", pVid->streamId);
        vmwareStopVideo(pScrn, pVid, TRUE);
        return pVid->play(pScrn, pVid, src_x, src_y, drw_x, drw_y, src_w,
                          src_h, drw_w, drw_h, format, buf, width, height,
                          clipBoxes, draw);
    }

    pVid->size = size;
    memcpy(pVid->bufs[pVid->currBuf].data, buf, pVid->size);

    cmdSetRegs.cmd = SVGA_CMD_ESCAPE;
    cmdSetRegs.nsid = SVGA_ESCAPE_NSID_VMWARE;
    cmdSetRegs.size = sizeof(cmdSetRegs.body);
    cmdSetRegs.body.escape = SVGA_ESCAPE_VMWARE_VIDEO_SET_REGS;
    cmdSetRegs.body.streamId = pVid->streamId;

    items = cmdSetRegs.body.items;
    for (i = SVGA_VIDEO_ENABLED; i < SVGA_VIDEO_DATA_GMRID; i++) {
        items[i].regId = i;
    }

    items[SVGA_VIDEO_ENABLED].value = TRUE;
    items[SVGA_VIDEO_DATA_OFFSET].value =
        pVid->bufs[pVid->currBuf].dataOffset;
    items[SVGA_VIDEO_SIZE].value = pVid->size;
    items[SVGA_VIDEO_FORMAT].value = format;
    items[SVGA_VIDEO_WIDTH].value = w;
    items[SVGA_VIDEO_HEIGHT].value = h;
    items[SVGA_VIDEO_SRC_X].value = src_x;
    items[SVGA_VIDEO_SRC_Y].value = src_y;
    items[SVGA_VIDEO_SRC_WIDTH].value = src_w;
    items[SVGA_VIDEO_SRC_HEIGHT].value = src_h;
    items[SVGA_VIDEO_DST_X].value = drw_x;
    items[SVGA_VIDEO_DST_Y].value = drw_y;
    items[SVGA_VIDEO_DST_WIDTH]. value = drw_w;
    items[SVGA_VIDEO_DST_HEIGHT].value = drw_h;
    items[SVGA_VIDEO_COLORKEY].value = pVid->colorKey;
    items[SVGA_VIDEO_FLAGS].value = pVid->flags;

    for (i = 0, regId = SVGA_VIDEO_PITCH_1; i < 3; i++, regId++) {
        items[regId].value = fmtData->pitches[i];
    }

    fifoItem = (uint32 *) &cmdSetRegs;
    for (i = 0; i <  sizeof(cmdSetRegs) / sizeof(uint32); i++) {
        vmwareWriteWordToFIFO(pVMWARE, fifoItem[i]);
    }

    /*
     *  Update the clipList and paint the colorkey, if required.
     */
    if (!vmwareIsRegionEqual(&pVid->clipBoxes, clipBoxes)) {
        REGION_COPY(pScrn->pScreen, &pVid->clipBoxes, clipBoxes);
        if (pVid->isAutoPaintColorkey) {
	    BoxPtr boxes = REGION_RECTS(&pVid->clipBoxes);
	    int nBoxes = REGION_NUM_RECTS(&pVid->clipBoxes);

#if HAVE_FILLKEYHELPERDRAWABLE
	    xf86XVFillKeyHelperDrawable(draw, pVid->colorKey, clipBoxes);
#else
	    xf86XVFillKeyHelper(pScrn->pScreen, pVid->colorKey, clipBoxes);
#endif
	    /**
	     * Force update to paint the colorkey before the overlay flush.
	     */

	    while(nBoxes--)
		vmwareSendSVGACmdUpdate(pVMWARE, boxes++);

        }
    }

    vmwareVideoFlush(pVMWARE, pVid->streamId);

    pVid->currBuf = ++pVid->currBuf & (VMWARE_VID_NUM_BUFFERS - 1);

    return Success;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareVideoFlush --
 *
 *    Sends the VIDEO_FLUSH command (FIFO ESCAPE mechanism) asking the host
 *    to play the video stream or end it.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 *
 *-----------------------------------------------------------------------------
 */

static void
vmwareVideoFlush(VMWAREPtr pVMWARE, uint32 streamId)
{
    struct PACKED _body {
        uint32 escape;
        uint32 streamId;
    };

    struct PACKED _cmdFlush {
        uint32 cmd;
        uint32 nsid;
        uint32 size;
        struct _body body;
    };

    struct _cmdFlush cmdFlush;
    uint32 *fifoItem;
    int i;

    cmdFlush.cmd = SVGA_CMD_ESCAPE;
    cmdFlush.nsid = SVGA_ESCAPE_NSID_VMWARE;
    cmdFlush.size = sizeof(cmdFlush.body);
    cmdFlush.body.escape = SVGA_ESCAPE_VMWARE_VIDEO_FLUSH;
    cmdFlush.body.streamId = streamId;

    fifoItem = (uint32 *) &cmdFlush;
    for (i = 0; i < sizeof(cmdFlush) / sizeof(uint32); i++) {
        vmwareWriteWordToFIFO(pVMWARE, fifoItem[i]);
    }
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareVideoSetOneReg --
 *
 *    Sets one video register using the FIFO ESCAPE mechanidm.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 *-----------------------------------------------------------------------------
 */

static void
vmwareVideoSetOneReg(VMWAREPtr pVMWARE, uint32 streamId,
                     uint32 regId, uint32 value)
{
    struct PACKED _item {
        uint32 regId;
        uint32 value;
    };

    struct PACKED _body {
        uint32 escape;
        uint32 streamId;
        struct _item item;
    };

    struct PACKED _cmdSetRegs {
        uint32 cmd;
        uint32 nsid;
        uint32 size;
        struct _body body;
    };

    struct _cmdSetRegs cmdSetRegs;
    int i;
    uint32 *fifoItem;

    cmdSetRegs.cmd = SVGA_CMD_ESCAPE;
    cmdSetRegs.nsid = SVGA_ESCAPE_NSID_VMWARE;
    cmdSetRegs.size = sizeof(cmdSetRegs.body);
    cmdSetRegs.body.escape = SVGA_ESCAPE_VMWARE_VIDEO_SET_REGS;
    cmdSetRegs.body.streamId = streamId;
    cmdSetRegs.body.item.regId = regId;
    cmdSetRegs.body.item.value = value;

    fifoItem = (uint32 *) &cmdSetRegs;
    for (i = 0; i < sizeof(cmdSetRegs) / sizeof(uint32); i++) {
        vmwareWriteWordToFIFO(pVMWARE, fifoItem[i]);
    }
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareVideoEndStream --
 *
 *    Frees up all resources (if any) taken by a video stream.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    Same as above.
 *
 *-----------------------------------------------------------------------------
 */

static void
vmwareVideoEndStream(ScrnInfoPtr pScrn, VMWAREVideoPtr pVid)
{
    uint32 id, colorKey, flags;
    Bool isAutoPaintColorkey;

    if (pVid->fmt_priv) {
        free(pVid->fmt_priv);
    }

    if (pVid->fbarea) {
        vmwareOffscreenFree(pVid->fbarea);
        pVid->fbarea =  NULL;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "Terminating Xv video-stream id:%d\n", pVid->streamId);
    /*
     * reset stream for next video
     */
    id = pVid->streamId;
    colorKey = pVid->colorKey;
    flags = pVid->flags;
    isAutoPaintColorkey = pVid->isAutoPaintColorkey;

    memset(pVid, 0, sizeof(*pVid));

    pVid->streamId = id;
    pVid->play = vmwareVideoInitStream;
    pVid->colorKey = colorKey;
    pVid->flags = flags;
    pVid->isAutoPaintColorkey = isAutoPaintColorkey;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareXvPutImage --
 *
 *    Main video playback function. It copies the passed data which is in
 *    the specified format (e.g. FOURCC_YV12) into the overlay.
 *
 *    If sync is TRUE the driver should not return from this
 *    function until it is through reading the data from buf.
 *
 *    There are two function prototypes to cope with the API change in X.org
 *    7.1
 *
 * Results:
 *    Success or XvBadAlloc on failure
 *
 * Side effects:
 *    Video stream will be played(initialized if 1st frame) on success
 *    or will fail on error.
 *
 *-----------------------------------------------------------------------------
 */

#if (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 1)
static int
vmwareXvPutImage(ScrnInfoPtr pScrn, short src_x, short src_y,
                 short drw_x, short drw_y, short src_w, short src_h,
                 short drw_w, short drw_h, int format,
                 unsigned char *buf, short width, short height,
                 Bool sync, RegionPtr clipBoxes, pointer data,
                 DrawablePtr dst)
#else
static int
vmwareXvPutImage(ScrnInfoPtr pScrn, short src_x, short src_y,
                 short drw_x, short drw_y, short src_w, short src_h,
                 short drw_w, short drw_h, int format,
                 unsigned char *buf, short width, short height,
                 Bool sync, RegionPtr clipBoxes, pointer data)
#endif
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    VMWAREVideoPtr pVid = data;

    TRACEPOINT

    if (!vmwareVideoEnabled(pVMWARE)) {
        return XvBadAlloc;
    }

#if (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 1)
    return pVid->play(pScrn, pVid, src_x, src_y, drw_x, drw_y, src_w, src_h,
                      drw_w, drw_h, format, buf, width, height, clipBoxes,
		      dst);
#else
    return pVid->play(pScrn, pVid, src_x, src_y, drw_x, drw_y, src_w, src_h,
                      drw_w, drw_h, format, buf, width, height, clipBoxes,
		      NULL);
#endif
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareStopVideo --
 *
 *    Called when we should stop playing video for a particular stream. If
 *    Cleanup is FALSE, the "stop" operation is only temporary, and thus we
 *    don't do anything. If Cleanup is TRUE we kill the video stream by
 *    sending a message to the host and freeing up the stream.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    See above.
 *
 *-----------------------------------------------------------------------------
 */

static void
vmwareStopVideo(ScrnInfoPtr pScrn, pointer data, Bool Cleanup)
{
    VMWAREVideoPtr pVid = data;
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    TRACEPOINT

    if (!vmwareVideoEnabled(pVMWARE)) {
        return;
    }

    REGION_EMPTY(pScrn->pScreen, &pVid->clipBoxes);

    if (!Cleanup) {
        VmwareLog(("vmwareStopVideo: Cleanup is FALSE.\n"));
        return;
    }
    vmwareVideoSetOneReg(pVMWARE, pVid->streamId,
                         SVGA_VIDEO_ENABLED, FALSE);

    vmwareVideoFlush(pVMWARE, pVid->streamId);
    vmwareVideoEndStream(pScrn, pVid);

}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareQueryImageAttributes --
 *
 *    From the spec: This function is called to let the driver specify how data
 *    for a particular image of size width by height should be stored.
 *    Sometimes only the size and corrected width and height are needed. In
 *    that case pitches and offsets are NULL.
 *
 * Results:
 *    The size of the memory required for the image, or -1 on error.
 *
 * Side effects:
 *    None.
 *
 *-----------------------------------------------------------------------------
 */

static int
vmwareQueryImageAttributes(ScrnInfoPtr pScrn, int format,
                           unsigned short *width, unsigned short *height,
                           int *pitches, int *offsets)
{
    INT32 size, tmp;

    TRACEPOINT

    if (*width > VMWARE_VID_MAX_WIDTH) {
        *width = VMWARE_VID_MAX_WIDTH;
    }
    if (*height > VMWARE_VID_MAX_HEIGHT) {
        *height = VMWARE_VID_MAX_HEIGHT;
    }

    *width = (*width + 1) & ~1;
    if (offsets != NULL) {
        offsets[0] = 0;
    }

    switch (format) {
       case FOURCC_YV12:
           *height = (*height + 1) & ~1;
           size = (*width + 3) & ~3;
           if (pitches) {
               pitches[0] = size;
           }
           size *= *height;
           if (offsets) {
               offsets[1] = size;
           }
           tmp = ((*width >> 1) + 3) & ~3;
           if (pitches) {
                pitches[1] = pitches[2] = tmp;
           }
           tmp *= (*height >> 1);
           size += tmp;
           if (offsets) {
               offsets[2] = size;
           }
           size += tmp;
           break;
       case FOURCC_UYVY:
       case FOURCC_YUY2:
           size = *width * 2;
           if (pitches) {
               pitches[0] = size;
           }
           size *= *height;
           break;
       default:
           VmwareLog(("Query for invalid video format %d\n", format));
           return -1;
    }
    return size;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareSetPortAttribute --
 *
 *    From the spec: A port may have particular attributes such as colorKey, hue,
 *    saturation, brightness or contrast. Xv clients set these
 *    attribute values by sending attribute strings (Atoms) to the server.
 *
 * Results:
 *    Success if the attribute exists and XvBadAlloc otherwise.
 *
 * Side effects:
 *    The respective attribute gets the new value.
 *
 *-----------------------------------------------------------------------------
 */

static int
vmwareSetPortAttribute(ScrnInfoPtr pScrn, Atom attribute,
                       INT32 value, pointer data)
{
    VMWAREVideoPtr pVid = (VMWAREVideoPtr) data;
    Atom xvColorKey = MAKE_ATOM("XV_COLORKEY");
    Atom xvAutoPaint = MAKE_ATOM("XV_AUTOPAINT_COLORKEY");

    if (attribute == xvColorKey) {
        VmwareLog(("Set colorkey:0x%x\n", value));
        pVid->colorKey = value;
    } else if (attribute == xvAutoPaint) {
        VmwareLog(("Set autoPaint: %s\n", value? "TRUE": "FALSE"));
        pVid->isAutoPaintColorkey = value;
    } else {
        return XvBadAlloc;
    }

    return Success;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareGetPortAttribute --
 *
 *    From the spec: A port may have particular attributes such as hue,
 *    saturation, brightness or contrast. Xv clients get these
 *    attribute values by sending attribute strings (Atoms) to the server
 *
 * Results:
 *    Success if the attribute exists and XvBadAlloc otherwise.
 *
 * Side effects:
 *    "value" contains the requested attribute on success.
 *
 *-----------------------------------------------------------------------------
 */

static int
vmwareGetPortAttribute(ScrnInfoPtr pScrn, Atom attribute,
                       INT32 *value, pointer data)
{
    VMWAREVideoPtr pVid = (VMWAREVideoPtr) data;
    Atom xvColorKey = MAKE_ATOM("XV_COLORKEY");
    Atom xvAutoPaint = MAKE_ATOM("XV_AUTOPAINT_COLORKEY");

    if (attribute == xvColorKey) {
        *value = pVid->colorKey;
    } else if (attribute == xvAutoPaint) {
        *value = pVid->isAutoPaintColorkey;
    } else {
        return XvBadAlloc;
    }

    return Success;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareQueryBestSize --
 *
 *    From the spec: QueryBestSize provides the client with a way to query what
 *    the destination dimensions would end up being if they were to request
 *    that an area vid_w by vid_h from the video stream be scaled to rectangle
 *    of drw_w by drw_h on the screen. Since it is not expected that all
 *    hardware will be able to get the target dimensions exactly, it is
 *    important that the driver provide this function.
 *
 *    This function seems to never be called, but to be on the safe side
 *    we apply the same logic that QueryImageAttributes has for width
 *    and height
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None
 *
 *-----------------------------------------------------------------------------
 */

static void
vmwareQueryBestSize(ScrnInfoPtr pScrn, Bool motion,
                    short vid_w, short vid_h, short drw_w,
                    short drw_h, unsigned int *p_w,
                    unsigned int *p_h, pointer data)
{
    *p_w = (drw_w + 1) & ~1;
    *p_h = drw_h;

    return;
}

