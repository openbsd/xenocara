/*
 * Copyright 1998-2008 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2003 S3 Graphics, Inc. All Rights Reserved.
 * Copyright 2006 Thomas Hellström. All Rights Reserved.
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
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * 2D acceleration functions for the VIA/S3G UniChrome IGPs.
 *
 * Mostly rewritten, and modified for EXA support, by Thomas Hellström.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <X11/Xarch.h>
#include "miline.h"

#include <GL/gl.h>
#include <sys/mman.h>

#include "via_driver.h"
#include "via_regs.h"
#include "via_dmabuffer.h"
#include "via_rop.h"

/*
 * Use PCI MMIO to flush the command buffer when AGP DMA is not available.
 */
static void
viaDumpDMA(ViaCommandBuffer *cb)
{
    register CARD32 *bp = cb->buf;
    CARD32 *endp = bp + cb->pos;

    while (bp != endp) {
        if (((bp - cb->buf) & 3) == 0) {
            ErrorF("\n %04lx: ", (unsigned long)(bp - cb->buf));
        }
        ErrorF("0x%08x ", (unsigned)*bp++);
    }
    ErrorF("\n");
}

void
viaFlushPCI(ViaCommandBuffer *cb)
{
    register CARD32 *bp = cb->buf;
    CARD32 transSetting;
    CARD32 *endp = bp + cb->pos;
    unsigned loop = 0;
    register CARD32 offset = 0;
    register CARD32 value;
    VIAPtr pVia = VIAPTR(cb->pScrn);

    while (bp < endp) {
        if (*bp == HALCYON_HEADER2) {
            if (++bp == endp)
                return;
            VIASETREG(VIA_REG_TRANSET, transSetting = *bp++);
            while (bp < endp) {
                if ((transSetting != HC_ParaType_CmdVdata)
                    && ((*bp == HALCYON_HEADER2)
                        || (*bp & HALCYON_HEADER1MASK) == HALCYON_HEADER1))
                    break;
                VIASETREG(VIA_REG_TRANSPACE, *bp++);
            }
        } else if ((*bp & HALCYON_HEADER1MASK) == HALCYON_HEADER1) {

            while (bp < endp) {
                if (*bp == HALCYON_HEADER2)
                    break;
                if (offset == 0) {
                    /*
                     * Not doing this wait will probably stall the processor
                     * for an unacceptable amount of time in VIASETREG while
                     * other high priority interrupts may be pending.
                     */
                    switch (pVia->Chipset) {
                    case VIA_VX800:
                    case VIA_VX855:
                    case VIA_VX900:
                        while ((VIAGETREG(VIA_REG_STATUS) &
                                (VIA_CMD_RGTR_BUSY_H5 | VIA_2D_ENG_BUSY_H5)) &&
                                (loop++ < MAXLOOP)) ;
                        break;

                    case VIA_P4M890:
                    case VIA_K8M890:
                    case VIA_P4M900:
                        while ((VIAGETREG(VIA_REG_STATUS) &
                                (VIA_CMD_RGTR_BUSY | VIA_2D_ENG_BUSY)) &&
                                (loop++ < MAXLOOP)) ;
                        break;

                    default:
                        while (!(VIAGETREG(VIA_REG_STATUS) & VIA_VR_QUEUE_EMPTY) &&
                                (loop++ < MAXLOOP)) ;
                        while ((VIAGETREG(VIA_REG_STATUS) &
                                (VIA_CMD_RGTR_BUSY | VIA_2D_ENG_BUSY)) &&
                                (loop++ < MAXLOOP)) ;
                    }
                }
                offset = (*bp++ & 0x0FFFFFFF) << 2;
                value = *bp++;
                VIASETREG(offset, value);
            }
        } else {
            ErrorF("Command stream parser error.\n");
        }
    }
    cb->pos = 0;
    cb->mode = 0;
    cb->has3dState = FALSE;
}

#ifdef HAVE_DRI
/*
 * Flush the command buffer using DRM. If in PCI mode, we can bypass DRM,
 * but not for command buffers that contain 3D engine state, since then
 * the DRM command verifier will lose track of the 3D engine state.
 */
static void
viaFlushDRIEnabled(ViaCommandBuffer *cb)
{
    ScrnInfoPtr pScrn = cb->pScrn;
    VIAPtr pVia = VIAPTR(pScrn);
    char *tmp = (char *)cb->buf;
    int tmpSize;
    drm_via_cmdbuffer_t b;

    /* Align end of command buffer for AGP DMA. */
    OUT_RING_H1(0x2f8, 0x67676767);
    if (pVia->agpDMA && cb->mode == 2 && cb->rindex != HC_ParaType_CmdVdata
        && (cb->pos & 1)) {
        OUT_RING(HC_DUMMY);
    }

    tmpSize = cb->pos * sizeof(CARD32);
    if (pVia->agpDMA || (pVia->directRenderingType && cb->has3dState)) {
        cb->mode = 0;
        cb->has3dState = FALSE;
        while (tmpSize > 0) {
            b.size = (tmpSize > VIA_DMASIZE) ? VIA_DMASIZE : tmpSize;
            tmpSize -= b.size;
            b.buf = tmp;
            tmp += b.size;
            if (drmCommandWrite(pVia->drmmode.fd, ((pVia->agpDMA)
                                              ? DRM_VIA_CMDBUFFER :
                                              DRM_VIA_PCICMD), &b, sizeof(b))) {
                ErrorF("DRM command buffer submission failed.\n");
                viaDumpDMA(cb);
                return;
            }
        }
        cb->pos = 0;
    } else {
        viaFlushPCI(cb);
    }
}
#endif

/*
 * Initialize a command buffer. Some fields are currently not used since they
 * are intended for Unichrome Pro group A video commands.
 */
static int
viaSetupCBuffer(ScrnInfoPtr pScrn, ViaCommandBuffer *cb, unsigned size)
{
#ifdef HAVE_DRI
    VIAPtr pVia = VIAPTR(pScrn);
#endif

    cb->pScrn = pScrn;
    cb->bufSize = ((size == 0) ? VIA_DMASIZE : size) >> 2;
    cb->buf = (CARD32 *) calloc(cb->bufSize, sizeof(CARD32));
    if (!cb->buf)
        return BadAlloc;
    cb->waitFlags = 0;
    cb->pos = 0;
    cb->mode = 0;
    cb->header_start = 0;
    cb->rindex = 0;
    cb->has3dState = FALSE;
    cb->flushFunc = viaFlushPCI;
#ifdef HAVE_DRI
    if (pVia->directRenderingType == DRI_1) {
        cb->flushFunc = viaFlushDRIEnabled;
    }
#endif
    return Success;
}

/*
 * Free resources associated with a command buffer.
 */
static void
viaTearDownCBuffer(ViaCommandBuffer *cb)
{
    if (cb && cb->buf) {
        free(cb->buf);
        cb->buf = NULL;
    }
}

/*
 * Update our 2D state (TwoDContext) with a new mode.
 */
Bool
viaAccelSetMode(int bpp, ViaTwodContext * tdc)
{
    switch (bpp) {
    case 16:
        tdc->mode = VIA_GEM_16bpp;
        tdc->bytesPPShift = 1;
        return TRUE;
    case 32:
        tdc->mode = VIA_GEM_32bpp;
        tdc->bytesPPShift = 2;
        return TRUE;
    case 8:
        tdc->mode = VIA_GEM_8bpp;
        tdc->bytesPPShift = 0;
        return TRUE;
    default:
        tdc->bytesPPShift = 0;
        return FALSE;
    }
}

/*
 * Switch 2D state clipping on.
 */
void
viaSetClippingRectangle(ScrnInfoPtr pScrn, int x1, int y1, int x2, int y2)
{
    VIAPtr pVia = VIAPTR(pScrn);
    ViaTwodContext *tdc = &pVia->td;

    tdc->clipping = TRUE;
    tdc->clipX1 = (x1 & 0xFFFF);
    tdc->clipY1 = y1;
    tdc->clipX2 = (x2 & 0xFFFF);
    tdc->clipY2 = y2;
}

/*
 * Check if we need to force upload of the whole 3D state (when other
 * clients or subsystems have touched the 3D engine). Also tell DRI
 * clients and subsystems that we have touched the 3D engine.
 */
Bool
viaCheckUpload(ScrnInfoPtr pScrn, Via3DState * v3d)
{
    VIAPtr pVia = VIAPTR(pScrn);
    Bool forceUpload;

    forceUpload = (pVia->lastToUpload != v3d);
    pVia->lastToUpload = v3d;

#ifdef HAVE_DRI
    if (pVia->directRenderingType == DRI_1) {
        volatile drm_via_sarea_t *saPriv = (drm_via_sarea_t *)
                DRIGetSAREAPrivate(pScrn->pScreen);
        int myContext = DRIGetContext(pScrn->pScreen);

        forceUpload = forceUpload || (saPriv->ctxOwner != myContext);
        saPriv->ctxOwner = myContext;
    }
#endif
    return forceUpload;
}

Bool
viaOrder(CARD32 val, CARD32 * shift)
{
    *shift = 0;

    while (val > (1 << *shift))
        (*shift)++;
    return (val == (1 << *shift));
}

/*
 * Helper for bitdepth expansion.
 */
CARD32
viaBitExpandHelper(CARD32 pixel, CARD32 bits)
{
    CARD32 component, mask, tmp;

    component = pixel & ((1 << bits) - 1);
    mask = (1 << (8 - bits)) - 1;
    tmp = component << (8 - bits);
    return ((component & 1) ? (tmp | mask) : tmp);
}

/*
 * Extract the components from a pixel of the given format to an argb8888 pixel. * This is used to extract data from one-pixel repeat pixmaps.
 * Assumes little endian.
 */
void
viaPixelARGB8888(unsigned format, void *pixelP, CARD32 * argb8888)
{
    CARD32 bits, shift, pixel, bpp;

    bpp = PICT_FORMAT_BPP(format);

    if (bpp <= 8) {
        pixel = *((CARD8 *) pixelP);
    } else if (bpp <= 16) {
        pixel = *((CARD16 *) pixelP);
    } else {
        pixel = *((CARD32 *) pixelP);
    }

    switch (PICT_FORMAT_TYPE(format)) {
        case PICT_TYPE_A:
            bits = PICT_FORMAT_A(format);
            *argb8888 = viaBitExpandHelper(pixel, bits) << 24;
            return;
        case PICT_TYPE_ARGB:
            shift = 0;
            bits = PICT_FORMAT_B(format);
            *argb8888 = viaBitExpandHelper(pixel, bits);
            shift += bits;
            bits = PICT_FORMAT_G(format);
            *argb8888 |= viaBitExpandHelper(pixel >> shift, bits) << 8;
            shift += bits;
            bits = PICT_FORMAT_R(format);
            *argb8888 |= viaBitExpandHelper(pixel >> shift, bits) << 16;
            shift += bits;
            bits = PICT_FORMAT_A(format);
            *argb8888 |= ((bits) ? viaBitExpandHelper(pixel >> shift,
                                                      bits) : 0xFF) << 24;
            return;
        case PICT_TYPE_ABGR:
            shift = 0;
            bits = PICT_FORMAT_B(format);
            *argb8888 = viaBitExpandHelper(pixel, bits) << 16;
            shift += bits;
            bits = PICT_FORMAT_G(format);
            *argb8888 |= viaBitExpandHelper(pixel >> shift, bits) << 8;
            shift += bits;
            bits = PICT_FORMAT_R(format);
            *argb8888 |= viaBitExpandHelper(pixel >> shift, bits);
            shift += bits;
            bits = PICT_FORMAT_A(format);
            *argb8888 |= ((bits) ? viaBitExpandHelper(pixel >> shift,
                                                      bits) : 0xFF) << 24;
            return;
        default:
            break;
    }
    return;
}

Bool
viaExpandablePixel(int format)
{
    int formatType = PICT_FORMAT_TYPE(format);

    return (formatType == PICT_TYPE_A ||
            formatType == PICT_TYPE_ABGR || formatType == PICT_TYPE_ARGB);
}

#ifdef VIA_DEBUG_COMPOSITE
void
viaExaCompositePictDesc(PicturePtr pict, char *string, int n)
{
    char format[20];
    char size[20];

    if (!pict) {
        snprintf(string, n, "None");
        return;
    }

    switch (pict->format) {
        case PICT_x8r8g8b8:
            snprintf(format, 20, "RGB8888");
            break;
        case PICT_a8r8g8b8:
            snprintf(format, 20, "ARGB8888");
            break;
        case PICT_r5g6b5:
            snprintf(format, 20, "RGB565  ");
            break;
        case PICT_x1r5g5b5:
            snprintf(format, 20, "RGB555  ");
            break;
        case PICT_a8:
            snprintf(format, 20, "A8      ");
            break;
        case PICT_a1:
            snprintf(format, 20, "A1      ");
            break;
        default:
            snprintf(format, 20, "0x%x", (int)pict->format);
            break;
    }

    if (pict->pDrawable) {
       snprintf(size, 20, "%dx%d%s", pict->pDrawable->width,
                pict->pDrawable->height, pict->repeat ? " R" : "");

       snprintf(string, n, "0x%lx: fmt %s (%s)", (long)pict->pDrawable, format,
                size);
    }
}

void
viaExaPrintCompositeInfo(char *info, CARD8 op, PicturePtr pSrc, PicturePtr pMask,
                        PicturePtr pDst)
{
    char sop[20];
    char srcdesc[40], maskdesc[40], dstdesc[40];

    switch (op) {
	case PictOpClear:
		sprintf(sop, "PictOpClear ");
		break;
	case PictOpSrc:
		sprintf(sop, "PictOpSrc ");
		break;
	case PictOpDst:
		sprintf(sop, "PictOpDst ");
		break;
	case PictOpOver:
		sprintf(sop, "PictOpOver ");
		break;
	case PictOpOutReverse:
		sprintf(sop, "PictOpOutReverse ");
		break;
	case PictOpAdd:
		sprintf(sop, "PictOpAdd ");
		break;
	default:
		sprintf(sop, "PictOp%d ", op);
    }

    viaExaCompositePictDesc(pSrc, srcdesc, 40);
    viaExaCompositePictDesc(pMask, maskdesc, 40);
    viaExaCompositePictDesc(pDst, dstdesc, 40);

    ErrorF("Composite fallback: %s, \n"
           "                    op %s, \n"
           "                    src  %s, \n"
           "                    mask %s, \n"
           "                    dst  %s, \n", info, sop, srcdesc, maskdesc, dstdesc);
}
#endif /* VIA_DEBUG_COMPOSITE */

/*
 * Wait for acceleration engines idle. An expensive way to sync.
 */
void
viaAccelSync(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    int loop = 0;

    mem_barrier();

    switch (pVia->Chipset) {
        case VIA_VX800:
        case VIA_VX855:
        case VIA_VX900:
            while ((VIAGETREG(VIA_REG_STATUS) &
                    (VIA_CMD_RGTR_BUSY_H5 | VIA_2D_ENG_BUSY_H5 | VIA_3D_ENG_BUSY_H5))
                   && (loop++ < MAXLOOP)) ;
            break;
        case VIA_P4M890:
        case VIA_K8M890:
        case VIA_P4M900:
            while ((VIAGETREG(VIA_REG_STATUS) &
                    (VIA_CMD_RGTR_BUSY | VIA_2D_ENG_BUSY | VIA_3D_ENG_BUSY))
                   && (loop++ < MAXLOOP)) ;
            break;
        default:
            while (!(VIAGETREG(VIA_REG_STATUS) & VIA_VR_QUEUE_EMPTY)
                   && (loop++ < MAXLOOP)) ;

            while ((VIAGETREG(VIA_REG_STATUS) &
                    (VIA_CMD_RGTR_BUSY | VIA_2D_ENG_BUSY | VIA_3D_ENG_BUSY))
                   && (loop++ < MAXLOOP)) ;
            break;
    }
}

/*
 * Wait for the value to get blitted, or in the PCI case for engine idle.
 */
static void
viaAccelWaitMarker(ScreenPtr pScreen, int marker)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    CARD32 uMarker = marker;

    if (pVia->agpDMA) {
        while ((pVia->lastMarkerRead - uMarker) > (1 << 24))
            pVia->lastMarkerRead = *(CARD32 *) pVia->markerBuf;
    } else {
        viaAccelSync(pScrn);
    }
}

#ifdef HAVE_DRI
static int
viaAccelDMADownload(ScrnInfoPtr pScrn, unsigned long fbOffset,
                    unsigned srcPitch, unsigned char *dst,
                    unsigned dstPitch, unsigned w, unsigned h)
{
    VIAPtr pVia = VIAPTR(pScrn);
    drm_via_dmablit_t blit[2], *curBlit;
    unsigned char *sysAligned = NULL;
    Bool doSync[2], useBounceBuffer;
    unsigned pitch, numLines[2];
    int curBuf, err, i, ret, blitHeight;

    ret = 0;

    useBounceBuffer = (((unsigned long)dst & 15) || (dstPitch & 15));
    doSync[0] = FALSE;
    doSync[1] = FALSE;
    curBuf = 1;
    blitHeight = h;
    pitch = dstPitch;
    if (useBounceBuffer) {
        pitch = ALIGN_TO(dstPitch, 16);
        blitHeight = VIA_DMA_DL_SIZE / pitch;
    }

    while (doSync[0] || doSync[1] || h != 0) {
        curBuf = 1 - curBuf;
        curBlit = &blit[curBuf];
        if (doSync[curBuf]) {

            do {
                err = drmCommandWrite(pVia->drmmode.fd, DRM_VIA_BLIT_SYNC,
                                      &curBlit->sync, sizeof(curBlit->sync));
            } while (err == -EAGAIN);

            if (err)
                return err;

            doSync[curBuf] = FALSE;
            if (useBounceBuffer) {
                for (i = 0; i < numLines[curBuf]; ++i) {
                    memcpy(dst, curBlit->mem_addr, w);
                    dst += dstPitch;
                    curBlit->mem_addr += pitch;
                }
            }
        }

        if (h == 0)
            continue;

        curBlit->num_lines = (h > blitHeight) ? blitHeight : h;
        h -= curBlit->num_lines;
        numLines[curBuf] = curBlit->num_lines;

        sysAligned =
                (unsigned char *)pVia->dBounce + (curBuf * VIA_DMA_DL_SIZE);
        sysAligned = (unsigned char *)
                ALIGN_TO((unsigned long)sysAligned, 16);

        curBlit->mem_addr = (useBounceBuffer) ? sysAligned : dst;
        curBlit->line_length = w;
        curBlit->mem_stride = pitch;
        curBlit->fb_addr = fbOffset;
        curBlit->fb_stride = srcPitch;
        curBlit->to_fb = 0;
        fbOffset += curBlit->num_lines * srcPitch;

        do {
            err = drmCommandWriteRead(pVia->drmmode.fd, DRM_VIA_DMA_BLIT, curBlit,
                                      sizeof(*curBlit));
        } while (err == -EAGAIN);

        if (err) {
            ret = err;
            h = 0;
            continue;
        }

        doSync[curBuf] = TRUE;
    }

    return ret;
}

/*
 * Use PCI DMA if we can. If the system alignments don't match, we're using
 * an aligned bounce buffer for pipelined PCI DMA and memcpy.
 * Throughput for large transfers is around 65 MB/s.
 */
static Bool
viaExaDownloadFromScreen(PixmapPtr pSrc, int x, int y, int w, int h,
                         char *dst, int dst_pitch)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pSrc->drawable.pScreen);
    unsigned wBytes = (pSrc->drawable.bitsPerPixel * w + 7) >> 3;
    unsigned srcPitch = exaGetPixmapPitch(pSrc), srcOffset;
    char *bounceAligned = NULL;
    VIAPtr pVia = VIAPTR(pScrn);
    unsigned totSize;

    if (!w || !h)
        return TRUE;

    srcOffset = x * pSrc->drawable.bitsPerPixel;
    if (srcOffset & 3)
        return FALSE;
    srcOffset = exaGetPixmapOffset(pSrc) + y * srcPitch + (srcOffset >> 3);

    totSize = wBytes * h;

    exaWaitSync(pScrn->pScreen);
    if (totSize < VIA_MIN_DOWNLOAD) {
        bounceAligned = (char *) drm_bo_map(pScrn, pVia->drmmode.front_bo) + srcOffset;

        while (h--) {
            memcpy(dst, bounceAligned, wBytes);
            dst += dst_pitch;
            bounceAligned += srcPitch;
        }
        return TRUE;
    }

    if (!pVia->directRenderingType)
        return FALSE;

    if ((srcPitch & 3) || (srcOffset & 3)) {
        ErrorF("VIA EXA download src_pitch misaligned\n");
        return FALSE;
    }

    if (viaAccelDMADownload(pScrn, srcOffset, srcPitch, (unsigned char *)dst,
                            dst_pitch, wBytes, h))
        return FALSE;

    return TRUE;
}

/*
 * Upload to framebuffer memory using memcpy to AGP pipelined with a
 * 3D engine texture operation from AGP to framebuffer. The AGP buffers (2)
 * should be kept rather small for optimal pipelining.
 */
static Bool
viaExaTexUploadToScreen(PixmapPtr pDst, int x, int y, int w, int h, char *src,
                        int src_pitch)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
    unsigned dstPitch = exaGetPixmapPitch(pDst), dstOffset;
    unsigned wBytes = (w * pDst->drawable.bitsPerPixel + 7) >> 3;
    int i, sync[2], yOffs, bufH, bufOffs, height, format;
    CARD32 texWidth, texHeight, texPitch;
    VIAPtr pVia = VIAPTR(pScrn);
    Via3DState *v3d = &pVia->v3d;
    char *dst, *texAddr;
    Bool buf;

    if (!w || !h)
        return TRUE;

    if (wBytes * h < VIA_MIN_TEX_UPLOAD) {
        dstOffset = x * pDst->drawable.bitsPerPixel;
        if (dstOffset & 3)
            return FALSE;

        dst = (char *) drm_bo_map(pScrn, pVia->drmmode.front_bo) +
                        (exaGetPixmapOffset(pDst) + y * dstPitch +
                        (dstOffset >> 3));
        exaWaitSync(pScrn->pScreen);

        while (h--) {
            memcpy(dst, src, wBytes);
            dst += dstPitch;
            src += src_pitch;
        }
        return TRUE;
    }

    if (!pVia->texAGPBuffer->ptr)
        return FALSE;

    switch (pDst->drawable.bitsPerPixel) {
        case 32:
            format = PICT_a8r8g8b8;
            break;
        case 16:
            format = PICT_r5g6b5;
            break;
        default:
            return FALSE;
    }

    dstOffset = exaGetPixmapOffset(pDst);

    if (pVia->nPOT[0]) {
        texPitch = ALIGN_TO(wBytes, 32);
        height = VIA_AGP_UPL_SIZE / texPitch;
    } else {
        viaOrder(wBytes, &texPitch);
        if (texPitch < 3)
            texPitch = 3;
        height = VIA_AGP_UPL_SIZE >> texPitch;
        texPitch = 1 << texPitch;
    }

    if (height > 1024)
        height = 1024;
    viaOrder(w, &texWidth);
    texWidth = 1 << texWidth;

    texHeight = height << 1;
    bufOffs = texPitch * height;
    texAddr = (char *) drm_bo_map(pScrn, pVia->texAGPBuffer);

    v3d->setDestination(v3d, dstOffset, dstPitch, format);
    v3d->setDrawing(v3d, 0x0c, 0xFFFFFFFF, 0x000000FF, 0x00);
    v3d->setFlags(v3d, 1, TRUE, TRUE, FALSE);
    if (!v3d->setTexture(v3d, 0, (unsigned long) texAddr, texPitch,
                         pVia->nPOT[0], texWidth, texHeight, format,
                         via_single, via_single, via_src, TRUE))
        return FALSE;

    v3d->emitState(v3d, &pVia->cb, viaCheckUpload(pScrn, v3d));
    v3d->emitClipRect(v3d, &pVia->cb, 0, 0, pDst->drawable.width,
                      pDst->drawable.height);

    buf = 1;
    yOffs = 0;
    sync[0] = -1;
    sync[1] = -1;

    while (h) {
        buf = (buf) ? 0 : 1;
        bufH = (h > height) ? height : h;
        dst = texAddr + ((buf) ? bufOffs : 0);

        if (sync[buf] >= 0)
            pVia->exaDriverPtr->WaitMarker(pScrn->pScreen, sync[buf]);

        for (i = 0; i < bufH; ++i) {
            memcpy(dst, src, wBytes);
            dst += texPitch;
            src += src_pitch;
        }

        v3d->emitQuad(v3d, &pVia->cb, x, y + yOffs, 0, (buf) ? height : 0, 0,
                      0, w, bufH);

        sync[buf] = pVia->exaDriverPtr->MarkSync(pScrn->pScreen);

        h -= bufH;
        yOffs += bufH;
    }

    if (sync[buf] >= 0)
        pVia->exaDriverPtr->WaitMarker(pScrn->pScreen, sync[buf]);

    return TRUE;
}

#endif /* HAVE_DRI */

#define EXAOPT_MIGRATION_HEURISTIC  0

Bool
viaInitExa(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    ExaDriverPtr pExa = exaDriverAlloc();
    Bool nPOTSupported = TRUE;
    VIAPtr pVia = VIAPTR(pScrn);

    /*
     * nPOT textures. DRM versions below 2.11.0 don't allow them.
     * Also some CLE266 hardware may not allow nPOT textures for
     * texture engine 1. We need to figure that out.
     */
#ifdef HAVE_DRI
    nPOTSupported = ((!pVia->directRenderingType) ||
                     (pVia->drmVerMajor > 2) ||
                     ((pVia->drmVerMajor == 2) && (pVia->drmVerMinor >= 11)));
#endif
    pVia->nPOT[0] = nPOTSupported;
    pVia->nPOT[1] = nPOTSupported;

    if (Success != viaSetupCBuffer(pScrn, &pVia->cb, 0)) {
        pVia->NoAccel = TRUE;
        return FALSE;
    }

    if (!pExa)
        return FALSE;

    memset(pExa, 0, sizeof(*pExa));

    pExa->exa_major = EXA_VERSION_MAJOR;
    pExa->exa_minor = EXA_VERSION_MINOR;
    pExa->memoryBase = pVia->FBBase;
    pExa->memorySize = pVia->FBFreeEnd;
    pExa->offScreenBase = pScrn->virtualY * pVia->Bpl;
    pExa->pixmapOffsetAlign = 32;
    pExa->pixmapPitchAlign = 16;
    pExa->flags = EXA_OFFSCREEN_PIXMAPS |
            (pVia->nPOT[1] ? 0 : EXA_OFFSCREEN_ALIGN_POT);


    /*  HW Limitation are described here:
     *
     *  1. H2/H5/H6 2D source and destination:
     *     Pitch: (1 << 14) - 1 = 16383
     *     Dimension: (1 << 12) = 4096
     *     X, Y position: (1 << 12) - 1 = 4095.
     *
     *  2. H2 3D engine Render target:
     *     Pitch: (1 << 14) - 1 = 16383
     *     Clip Rectangle: 0 - 2047
     *
     *  3. H5/H6 3D engine Render target:
     *     Pitch: ((1 << 10) - 1)*32 = 32736
     *     Clip Rectangle: Color Window, 12bits. As Spec saied: 0 - 2048
     *                     Scissor is the same as color window.
     */
    pExa->maxX = 2047;
    pExa->maxY = 2047;
    pExa->WaitMarker = viaAccelWaitMarker;

    switch (pVia->Chipset) {
    case VIA_VX800:
    case VIA_VX855:
    case VIA_VX900:
        pExa->MarkSync = viaAccelMarkSync_H6;
        pExa->PrepareSolid = viaExaPrepareSolid_H6;
        pExa->Solid = viaExaSolid_H6;
        pExa->DoneSolid = viaExaDoneSolidCopy_H6;
        pExa->PrepareCopy = viaExaPrepareCopy_H6;
        pExa->Copy = viaExaCopy_H6;
        pExa->DoneCopy = viaExaDoneSolidCopy_H6;
        break;
    default:
        pExa->MarkSync = viaAccelMarkSync_H2;
        pExa->PrepareSolid = viaExaPrepareSolid_H2;
        pExa->Solid = viaExaSolid_H2;
        pExa->DoneSolid = viaExaDoneSolidCopy_H2;
        pExa->PrepareCopy = viaExaPrepareCopy_H2;
        pExa->Copy = viaExaCopy_H2;
        pExa->DoneCopy = viaExaDoneSolidCopy_H2;
        break;
    }

#ifdef HAVE_DRI
    if (pVia->directRenderingType == DRI_1) {
#ifdef linux
        pExa->DownloadFromScreen = viaExaDownloadFromScreen;
#endif /* linux */
        switch (pVia->Chipset) {
        case VIA_K8M800:
        case VIA_KM400:
            pExa->UploadToScreen = NULL; //viaExaTexUploadToScreen;
            break;
        default:
            pExa->UploadToScreen = NULL; //viaExaUploadToScreen;
            break;
        }
    }
#endif /* HAVE_DRI */

    if (!pVia->noComposite) {
        switch (pVia->Chipset) {
        case VIA_VX800:
        case VIA_VX855:
        case VIA_VX900:
            pExa->CheckComposite = viaExaCheckComposite_H6;
            pExa->PrepareComposite = viaExaPrepareComposite_H6;
            pExa->Composite = viaExaComposite_H6;
            pExa->DoneComposite = viaExaDoneSolidCopy_H6;
            break;
        default:
            pExa->CheckComposite = viaExaCheckComposite_H2;
            pExa->PrepareComposite = viaExaPrepareComposite_H2;
            pExa->Composite = viaExaComposite_H2;
            pExa->DoneComposite = viaExaDoneSolidCopy_H2;
            break;
        }
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "[EXA] Disabling EXA accelerated composite.\n");
    }

    if (!exaDriverInit(pScreen, pExa)) {
        free(pExa);
        return FALSE;
    }

    pVia->exaDriverPtr = pExa;
    viaInit3DState(&pVia->v3d);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                "[EXA] Enabled EXA acceleration.\n");
    return TRUE;
}

/*
 * Allocate a command buffer and  buffers for accelerated upload, download,
 * and EXA scratch area. The scratch area resides primarily in AGP memory,
 * but reverts to FB if AGP is not available.
 */
void
viaFinishInitAccel(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    int size;

#ifdef HAVE_DRI
    if (pVia->directRenderingType && pVia->useEXA) {

        pVia->dBounce = calloc(VIA_DMA_DL_SIZE * 2, 1);

        if (!pVia->IsPCI) {

            /* Allocate upload and scratch space. */
            if (pVia->exaDriverPtr->UploadToScreen == viaExaTexUploadToScreen) {
                size = VIA_AGP_UPL_SIZE * 2;

                pVia->texAGPBuffer = drm_bo_alloc(pScrn, size, 32, TTM_PL_FLAG_TT);
                if (pVia->texAGPBuffer) {
                    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                               "Allocated %u kiB of AGP memory for "
                               "system-to-framebuffer transfer.\n",
                               size / 1024);
                    pVia->texAGPBuffer->offset = (pVia->texAGPBuffer->offset + 31) & ~31;
                }
            }

            size = pVia->exaScratchSize * 1024;
            pVia->scratchBuffer = drm_bo_alloc(pScrn, size, 32, TTM_PL_FLAG_TT);
            if (pVia->scratchBuffer) {
                xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                           "Allocated %u kiB of AGP memory for "
                           "EXA scratch area.\n", size / 1024);
                pVia->scratchOffset =
                        (pVia->scratchBuffer->offset + 31) & ~31;
                pVia->scratchAddr = drm_bo_map(pScrn, pVia->scratchBuffer);
            }
        }
    }
#endif /* HAVE_DRI */
    if (!pVia->scratchAddr && pVia->useEXA) {
        size = pVia->exaScratchSize * 1024 + 32;
        pVia->scratchBuffer = drm_bo_alloc(pScrn, size, 32, TTM_PL_FLAG_SYSTEM);

        if (pVia->scratchBuffer) {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "Allocated %u kiB of framebuffer memory for "
                       "EXA scratch area.\n", pVia->exaScratchSize);
            pVia->scratchOffset = pVia->scratchBuffer->offset;
            pVia->scratchAddr = drm_bo_map(pScrn, pVia->scratchBuffer);
        }
    }
    memset(pVia->markerBuf, 0, pVia->exa_sync_bo->size);
}

/*
 * Free the used acceleration resources.
 */
void
viaExitAccel(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);

    viaAccelSync(pScrn);
    viaTearDownCBuffer(&pVia->cb);

    if (pVia->useEXA) {
#ifdef HAVE_DRI
        if (pVia->directRenderingType == DRI_1) {
            if (pVia->texAGPBuffer) {
                drm_bo_free(pScrn, pVia->texAGPBuffer);
                pVia->texAGPBuffer = NULL;
            }

            if (pVia->scratchBuffer) {
                drm_bo_free(pScrn, pVia->scratchBuffer);
                pVia->scratchBuffer = NULL;
            }
        }
        if (pVia->dBounce)
            free(pVia->dBounce);
#endif /* HAVE_DRI */
        if (pVia->scratchBuffer) {
            drm_bo_free(pScrn, pVia->scratchBuffer);
            pVia->scratchBuffer = NULL;
        }
        if (pVia->vq_bo) {
            drm_bo_unmap(pScrn, pVia->vq_bo);
            drm_bo_free(pScrn, pVia->vq_bo);
        }
        if (pVia->exa_sync_bo) {
            drm_bo_unmap(pScrn, pVia->exa_sync_bo);
            drm_bo_free(pScrn, pVia->exa_sync_bo);
        }
        if (pVia->exaDriverPtr) {
            exaDriverFini(pScreen);
        }
        free(pVia->exaDriverPtr);
        pVia->exaDriverPtr = NULL;
        return;
    }
}
