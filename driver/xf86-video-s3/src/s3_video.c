/*
 *      Copyright 2001  Ani Joshi <ajoshi@unixbox.com>
 * 
 *      XFree86 4.x driver for S3 chipsets
 * 
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation and
 * that the name of Ani Joshi not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Ani Joshi makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as-is" without express or implied warranty.
 *                 
 * ANI JOSHI DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL ANI JOSHI BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"

#include "compiler.h"

#include "s3.h"
#include "s3_reg.h"


#define CLIENT_VIDEO_ON	0x04
#define S3_MAX_PORTS	1
#define NUM_FORMATS_OVERLAY 4
#define NUM_FORMATS_TEXTURE 4


static XF86VideoAdaptorPtr S3AllocAdaptor(ScrnInfoPtr pScrn);
static XF86VideoAdaptorPtr S3SetupImageVideoOverlay(ScreenPtr);
static int S3SetPortAttributeOverlay(ScrnInfoPtr, Atom, INT32, pointer);
static int S3GetPortAttributeOverlay(ScrnInfoPtr, Atom ,INT32 *, pointer);
static void S3StopVideo(ScrnInfoPtr, pointer, Bool);
static void S3QueryBestSize(ScrnInfoPtr, Bool, short, short, short, short,
                            unsigned int *, unsigned int *, pointer);
static int  S3PutImage(ScrnInfoPtr, short, short, short, short, short,
                       short, short, short, int, unsigned char*, short,
                       short, Bool, RegionPtr, pointer, DrawablePtr);
static int  S3QueryImageAttributes(ScrnInfoPtr, int, unsigned short *,
                        	   unsigned short *,  int *, int *);
static void S3ResetVideoOverlay(ScrnInfoPtr);
static FBLinearPtr S3XVMemAlloc(ScrnInfoPtr pScrn, pointer pVideo, int size);


void S3InitVideo(ScreenPtr pScreen)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	S3Ptr pS3 = S3PTR(pScrn);
	XF86VideoAdaptorPtr *adaptors, *newAdaptors = NULL;
	XF86VideoAdaptorPtr newAdaptor = NULL;
	int num_adaptors;

	newAdaptor = S3SetupImageVideoOverlay(pScreen);
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using overlay video\n");

	num_adaptors = xf86XVListGenericAdaptors(pScrn, &adaptors);

    	if(newAdaptor) {
        	if(!num_adaptors) {
            		num_adaptors = 1;
            		adaptors = &newAdaptor;
        	} else {
            		newAdaptors =  /* need to free this someplace */
                		malloc((num_adaptors + 1) * sizeof(XF86VideoAdaptorPtr*));   
            		if(newAdaptors) {
                		memcpy(newAdaptors, adaptors, num_adaptors *
                                       sizeof(XF86VideoAdaptorPtr));
                		newAdaptors[num_adaptors] = newAdaptor;
                		adaptors = newAdaptors;
                		num_adaptors++;
           		} 
        	}
    	}

    	if(num_adaptors)
        	xf86XVScreenInit(pScreen, adaptors, num_adaptors);
            
        free(newAdaptors);
}


/* client libraries expect an encoding */
static XF86VideoEncodingRec DummyEncoding[2] =  
{
 	{   /* overlay limit */
   		0,    
   		"XV_IMAGE",
   		1024, 1024,
   		{1, 1}
 	},
 	{  /* texture limit */
   		0,
   		"XV_IMAGE",
   		2046, 2046,  
   		{1, 1}
 	}
};


static FBLinearPtr S3XVMemAlloc(ScrnInfoPtr pScrn, pointer pVideo, int size)
{
	FBLinearPtr pLinear = (FBLinearPtr)pVideo;
	ScreenPtr pScreen = pScrn->pScreen;

	if (pLinear) {
		if ((pLinear->size >= size) ||
		    xf86ResizeOffscreenLinear(pLinear, size)) {
			pLinear->MoveLinearCallback = NULL;
			pLinear->RemoveLinearCallback = NULL;
			return pLinear;
		}
		xf86FreeOffscreenLinear(pLinear);
	}
	pLinear = xf86AllocateOffscreenLinear(pScreen, size, 16, 
					      NULL, NULL, NULL);
	
	if (!pLinear) {
		int maxSize;
		
		xf86QueryLargestOffscreenLinear(pScreen, &maxSize, 16,
						PRIORITY_EXTREME);
		if (maxSize < size)
			return NULL;
		
		xf86PurgeUnlockedOffscreenAreas(pScreen);
		pLinear = xf86AllocateOffscreenLinear(pScreen, size, 16, 
						      NULL, NULL, NULL);
	}
	return pLinear;
}

static XF86VideoFormatRec Formats[NUM_FORMATS_TEXTURE] =
{
  	{16, TrueColor}, {24, TrueColor},
	{16, DirectColor}, {24, DirectColor}
};



#define NUM_IMAGES 3
   
static XF86ImageRec Images[NUM_IMAGES] =
{    
  	XVIMAGE_YUY2,
  	/* As in mga, YV12 & I420 are converted to YUY2 on the fly by */
  	/* copy over conversion. */
  	XVIMAGE_YV12,
  	XVIMAGE_I420
        /* XVIMAGE_UYVY */
};



static int S3SetPortAttributeOverlay(ScrnInfoPtr pScrn, Atom attribute,
				     INT32 value, pointer data)
{
	return BadMatch;
}


static int S3GetPortAttributeOverlay(ScrnInfoPtr pScrn, Atom attribute,
				     INT32 *value, pointer data)
{
	return BadMatch;
}



static void S3QueryBestSize(ScrnInfoPtr pScrn, Bool motion, short vid_w,
			    short vid_h, short drw_w, short drw_h,
                            unsigned int *p_w, unsigned int *p_h,
			    pointer data)
{
	*p_w = drw_w;
	*p_h = drw_h;
}



static void S3ResetVideoOverlay(ScrnInfoPtr pScrn)
{
}


static XF86VideoAdaptorPtr S3AllocAdaptor(ScrnInfoPtr pScrn)
{
	S3Ptr pS3 = S3PTR(pScrn);
	XF86VideoAdaptorPtr adapt;
	S3PortPrivPtr pPriv;
	int i;

    	if(!(adapt = xf86XVAllocateVideoAdaptorRec(pScrn)))
        	return NULL;
   
    	if(!(pPriv = calloc(1, sizeof(S3PortPrivRec)  +
			     (sizeof(DevUnion) * S3_MAX_PORTS))))
    	{
        	free(adapt);
        	return NULL;
    	}
    
    	adapt->pPortPrivates = (DevUnion*)(&pPriv[1]);

    	for(i = 0; i < S3_MAX_PORTS; i++)
        	adapt->pPortPrivates[i].val = i;

	pPriv->colorKey = (1 << pScrn->offset.red) | 
		(1 << pScrn->offset.green) |
		(((pScrn->mask.blue >> pScrn->offset.blue) - 1) << pScrn->offset.blue);

	pPriv->videoStatus = 0;
	pPriv->lastPort = -1;

	pS3->adaptor = adapt;
	pS3->portPrivate = pPriv;
	
	return adapt;
}


static XF86VideoAdaptorPtr S3SetupImageVideoOverlay(ScreenPtr pScreen)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	S3Ptr pS3 = S3PTR(pScrn);
	XF86VideoAdaptorPtr adapt;

	adapt = S3AllocAdaptor(pScrn);
	if (adapt == NULL)
		return NULL;

    	adapt->type = XvWindowMask | XvInputMask | XvImageMask;
    	adapt->flags = VIDEO_OVERLAID_IMAGES | VIDEO_CLIP_TO_VIEWPORT;
    	adapt->name = "S3 Backend Scaler";
    	adapt->nEncodings = 1;
    	adapt->pEncodings = &DummyEncoding[0];
    	adapt->nFormats = NUM_FORMATS_OVERLAY;
    	adapt->pFormats = Formats;
    	adapt->nPorts = 1;
    	adapt->pAttributes = NULL /*Attributes*/;
	adapt->nImages = 3;
	adapt->nAttributes = 0;

    	adapt->pImages = Images;
    	adapt->PutVideo = NULL;
    	adapt->PutStill = NULL;
    	adapt->GetVideo = NULL;
    	adapt->GetStill = NULL;
    	adapt->StopVideo = S3StopVideo;  
    	/* Empty Attrib functions - required anyway */
    	adapt->SetPortAttribute = S3SetPortAttributeOverlay;
    	adapt->GetPortAttribute = S3GetPortAttributeOverlay;
    	adapt->QueryBestSize = S3QueryBestSize;
    	adapt->PutImage = S3PutImage;
    	adapt->QueryImageAttributes = S3QueryImageAttributes;
 
    	/* gotta uninit this someplace */
	REGION_NULL(pScreen, &(pS3->portPrivate->clip));
       
    	S3ResetVideoOverlay(pScrn);
       
    	return adapt;
}   


static void S3StopVideo(ScrnInfoPtr pScrn, pointer data, Bool exit)
{
	S3Ptr pS3 = S3PTR(pScrn);
	S3PortPrivPtr pPriv = pS3->portPrivate;

	REGION_EMPTY(pScrn->pScreen, &pPriv->clip);

	if (exit) {
		SET_FIFO_CNTL(0x00080000 | FIFO_PS24_SS0);

		if (pPriv->videoStatus & CLIENT_VIDEO_ON) {
			WaitVSync();
			SET_SSTREAM_CNTL(0x03000000);
			SET_SSTREAM_FBADDR0(0x00000000);
			SET_SSTREAM_FBADDR1(0x00000000);
			SET_SSTREAM_STRIDE(0x00000001);
			SET_SSTREAM_START(0x07ff07ff);
			SET_SSTREAM_WIND(0x00010001);

			SET_CHROMA_KEY(0x00000000);
			SET_SSTRETCH(0x00000000);
			SET_OPAQUE_OVERLAY(0x40000000);
			SET_K1_VSCALE(0x00000000);
			SET_K2_VSCALE(0x00000000);
			SET_DDA_VERT(0x00000000);
			SET_BLEND_CNTL(0x01000000);
			WaitVSync();
		}

		if (pPriv->area) {
			xf86FreeOffscreenLinear(pPriv->area);
	        	pPriv->area = NULL;
		}

		pPriv->videoStatus = 0;
	}
}

static void S3DisplayVideoOverlay(ScrnInfoPtr pScrn, int id, int offset,
				  short width, short height, int pitch,
				  int x1, int y1, int x2, int y2,
				  BoxPtr dstBox, short src_w, short src_h,
				  short drw_w, short drw_h)
{
	S3Ptr pS3 = S3PTR(pScrn);
	S3PortPrivPtr pPriv = pS3->portPrivate;
	int tmp;

	if (drw_w == src_w)
		tmp = 0;
	else
		tmp = 2;

	SET_SSTREAM_CNTL((tmp << 28) | 0x01000000 |
			 ((((src_w - 1) << 1) - (drw_w - 1)) & 0xfff));
	SET_SSTRETCH(((src_w - 1) & 0x7ff) | 
		     (((src_w - drw_w) & 0x7ff) << 16));
	SET_BLEND_CNTL(0x05000000);
	SET_SSTREAM_FBADDR0(offset & 0x3fffff);
	SET_SSTREAM_FBADDR1(offset & 0x3fffff);
	SET_SSTREAM_STRIDE(pitch & 0xfff);
	SET_SSTREAM_START(((dstBox->x1 + 1) << 16) | (dstBox->y1 + 1));
	SET_SSTREAM_WIND((((drw_w - 1) << 16) | drw_h) & 0x7ff07ff);

	SET_K1_VSCALE(src_h - 1);
	SET_K2_VSCALE((src_h - drw_h) & 0x7ff);
	SET_DDA_VERT(((~drw_h - 1)) & 0xfff);

	SET_CHROMA_KEY(0x10000000 |
		       ((pScrn->weight.red-1) << 24) |
		       ((pPriv->colorKey & pScrn->mask.red) >> pScrn->offset.red) <<
		       (16 + 8-pScrn->weight.red) |
		       ((pPriv->colorKey & pScrn->mask.green) >> pScrn->offset.green) <<
		       (8 + 8-pScrn->weight.green) |
		       ((pPriv->colorKey & pScrn->mask.blue) >> pScrn->offset.blue) <<
		       (8-pScrn->weight.blue));

	SET_FIFO_CNTL(0x00080000 | pS3->Streams_FIFO);
}

static int S3PutImage(ScrnInfoPtr pScrn, short src_x, short src_y,
		      short drw_x, short drw_y, short src_w, short src_h,
		      short drw_w, short drw_h, int id, unsigned char *buf,
		      short width, short height, Bool sync, 
		      RegionPtr clipBoxes, pointer data, DrawablePtr pDraw)
{
	S3Ptr pS3 = S3PTR(pScrn);
	S3PortPrivPtr pPriv = pS3->portPrivate;
   	INT32 x1, x2, y1, y2;
   	CARD8 *dst_start; 
   	int offset, offsetV = 0, offsetU = 0;
   	int srcPitch, srcPitchUV = 0, dstPitch, dstSize;
   	int top, bottom, right, left, npixels, nlines;
   	BoxRec dstBox;
	CARD32 tmp;
	int cpp = (pScrn->bitsPerPixel + 7) >> 3;

	/* Clip */
	x1 = src_x;
	x2 = src_x + src_w;
	y1 = src_y;
	y2 = src_y + src_h;
   
	dstBox.x1 = drw_x;
	dstBox.x2 = drw_x + drw_w;
	dstBox.y1 = drw_y;
	dstBox.y2 = drw_y + drw_h;
   
	if(!xf86XVClipVideoHelper(&dstBox, &x1, &x2, &y1, &y2, clipBoxes, 
				  width, height))
		return Success;
   
	dstBox.x1 -= pScrn->frameX0;
	dstBox.x2 -= pScrn->frameX0;
	dstBox.y1 -= pScrn->frameY0;
	dstBox.y2 -= pScrn->frameY0;
        
	/* requested size in bytes */
	dstPitch = ((width << 1) + 15) & ~15;
	dstSize = dstPitch * height;

	pPriv->area = S3XVMemAlloc(pScrn, pPriv->area,
				   (dstSize + cpp - 1) / cpp);
	if (!pPriv->area) 
		return BadAlloc;

	offset = pPriv->area->offset * cpp;
	dst_start = pS3->FBBase + offset;

	switch (id) {
        case FOURCC_YV12:
        case FOURCC_I420:
		left = (x1 >> 16) & ~1;
		right = ((x2 + 0x1ffff) >> 16) & ~1;
		top = (y1 >> 16) & ~1;
		bottom = ((y2 + 0x1ffff) >> 16) & ~1;

		if ((right < width) && ((x1 & 0x1ffff) <= (x2 & 0x1ffff)))
			right += 2;
		if ((bottom < height) && ((y1 & 0x1ffff) <= (y2 & 0x1ffff)))
			bottom += 2;

		npixels = right - left;
		nlines = bottom - top;

		srcPitch = (width + 3) & ~3;
		offsetV = srcPitch * height;
		srcPitchUV = ((width >> 1) + 3) & ~3;
		offsetU = ((height >> 1) * srcPitchUV) + offsetV;

		tmp = ((top >> 1) * srcPitchUV) + (left >> 1);
		offsetV += tmp;
		offsetU += tmp;

		if (id == FOURCC_I420)
		{
			tmp = offsetV;
			offsetV = offsetU;
			offsetU = tmp;
		}

		dst_start += top * dstPitch + (left << 1);

		xf86XVCopyYUV12ToPacked(buf + (top * srcPitch) + left,
					buf + offsetV, buf + offsetU, 
					dst_start, srcPitch, srcPitchUV,
					dstPitch, nlines, npixels);
		break;

        case FOURCC_UYVY:
        case FOURCC_YUY2:
        default:
		left = (x1 >> 16) & ~1;
		right = ((x2 + 0x1ffff) >> 16) & ~1;
		top = y1 >> 16;
		bottom = (y2 + 0x0ffff) >> 16;

		if ((right < width) && ((x1 & 0x1ffff) <= (x2 & 0x1ffff)))
			right += 2;
		if ((bottom < height) && ((y1 & 0x0ffff) <= (y2 & 0x0ffff)))
			bottom++;

		npixels = right - left;
		nlines = bottom - top;

		srcPitch = width << 1;
		buf += (top * srcPitch) + (left << 1);
		dst_start += top * dstPitch + (left << 1);

		xf86XVCopyPacked(buf, dst_start, srcPitch, dstPitch, 
				 nlines, npixels);
		break;
	}

	/* update cliplist */
	if(!REGION_EQUAL(pScrn->pScreen, &pPriv->clip, clipBoxes)) {
		REGION_COPY(pScrn->pScreen, &pPriv->clip, clipBoxes);
		/* draw these */
		xf86XVFillKeyHelper(pScrn->pScreen, pPriv->colorKey, 
				    clipBoxes);
	}
   
	offset += (left << 1) + (top * dstPitch);
	S3DisplayVideoOverlay(pScrn, id, offset, width, height, dstPitch,
			      x1, y1, x2, y2, &dstBox, 
			      src_w, src_h, drw_w, drw_h);
   
	pPriv->videoStatus = CLIENT_VIDEO_ON;
	return Success;
}       



static int S3QueryImageAttributes(ScrnInfoPtr pScrn, int id,
				  unsigned short *w, unsigned short *h,
				  int *pitches, int *offsets)
{
	int size, tmp;
	
	*w = (*w + 1) & ~1;
	if(offsets) offsets[0] = 0;
	
	switch(id) {
	case FOURCC_YV12:
	case FOURCC_I420:
		*h = (*h + 1) & ~1;
		size = (*w + 3) & ~3;
		if(pitches) pitches[0] = size;
		size *= *h;
		if(offsets) offsets[1] = size;
		tmp = ((*w >> 1) + 3) & ~3;
		if(pitches) pitches[1] = pitches[2] = tmp;
		tmp *= (*h >> 1);
		size += tmp;
		if(offsets) offsets[2] = size;
		size += tmp;
		break;
	case FOURCC_UYVY:
	case FOURCC_YUY2:
	default:
		size = *w << 1;
		if(pitches) pitches[0] = size;
		size *= *h;
		break;
	} 
        
	return size;
}


void S3InitStreams(ScrnInfoPtr pScrn, DisplayModePtr mode)
{       
        S3Ptr pS3 = S3PTR(pScrn);
        unsigned int pst_wind = (mode->HDisplay-1) << 16 | (mode->VDisplay);
        
	WaitVSync();

	switch (pScrn->bitsPerPixel) {
	case 8: 
		SET_PSTREAM_CNTL(0x00000000);
		break;
	case 15:
		SET_PSTREAM_CNTL(0x03000000);
		break;
	case 16: 
		SET_PSTREAM_CNTL(0x05000000);
		break;
	case 24:
		SET_PSTREAM_CNTL(0x06000000);
		break;
	case 32:
		SET_PSTREAM_CNTL(0x07000000);
		break;
	}

	SET_PSTREAM_FBADDR0(0x00000000);
	SET_PSTREAM_FBADDR1(0x00000000);
	
	SET_PSTREAM_STRIDE(pS3->s3BppDisplayWidth & 0x0fff);

        SET_PSTREAM_WIND(pst_wind & 0x07ff07ff);
        SET_PSTREAM_START(0x00010001);

        SET_CHROMA_KEY(0x00000000);
	SET_SSTRETCH(0x00000000);
        SET_BLEND_CNTL(0x01000000);
	SET_DOUBLE_BUFFER(0x00000000);

        SET_SSTREAM_CNTL(0x03000000);
	SET_SSTREAM_FBADDR0(0x00000000);
	SET_SSTREAM_FBADDR1(0x00000000);
        SET_SSTREAM_STRIDE(0x00000001);
        SET_SSTREAM_START(0x07ff07ff);
        SET_SSTREAM_WIND(0x00010001);

        SET_OPAQUE_OVERLAY(0x40000000);
	SET_K1_VSCALE(0x00000000);
	SET_K2_VSCALE(0x00000000);
	SET_DDA_VERT(0x00000000);

	/*
	  ps thr | ss thr | ss fifo slots
	  set primary stream FIFO to 24 slots and 12 slots for threshold 
	*/
	SET_FIFO_CNTL(0x00080000 | FIFO_PS24_SS0);
}       

