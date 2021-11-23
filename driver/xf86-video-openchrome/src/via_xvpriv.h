/*
 * Copyright 2006-2015 The Openchrome Project
 *                     [https://www.freedesktop.org/wiki/Openchrome]
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
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _VIA_XVPRIV_H_
#define _VIA_XVPRIV_H_ 1

#include "xf86xv.h"


/*
 * Alignment macro functions
 */
#define ALIGN_TO(f, alignment) (((f) + ((alignment)-1)) & ~((alignment)-1))

/*
 * FOURCC definitions
 */

#define FOURCC_XVMC     (('C' << 24) + ('M' << 16) + ('V' << 8) + 'X')
#define FOURCC_RV15     (('5' << 24) + ('1' << 16) + ('V' << 8) + 'R')
#define FOURCC_RV16     (('6' << 24) + ('1' << 16) + ('V' << 8) + 'R')
#define FOURCC_RV32     (('2' << 24) + ('3' << 16) + ('V' << 8) + 'R')

/* Definition for dwFlags */
#define DDOVER_KEYDEST     1
#define DDOVER_INTERLEAVED 2
#define DDOVER_BOB         4

#define FOURCC_HQVSW   0x34565148  /*HQV4*/

#define MEM_BLOCKS      4

enum
{ XV_ADAPT_SWOV = 0,
    XV_ADAPT_NUM
};

typedef enum
{
    xve_none = 0,
    xve_bandwidth,
    xve_dmablit,
    xve_mem,
    xve_general,
    xve_adaptor,
    xve_numerr
} XvError;

#define VIA_MAX_XV_PORTS 1

typedef struct
{
    unsigned char xv_adaptor;
    unsigned char xv_portnum;
    int adaptor;
    int brightness;
    int saturation;
    int contrast;
    int hue;
    RegionRec clip;
    CARD32 colorKey;
    Bool autoPaint;

    CARD32 FourCC;		       /* from old SurfaceDesc -- passed down from viaPutImageG */

    /* store old video source & dst data */
    short old_src_x;
    short old_src_y;
    short old_src_w;
    short old_src_h;

    short old_drw_x;
    short old_drw_y;
    short old_drw_w;
    short old_drw_h;

    void *xvmc_priv;

    /*
     * For PCI DMA image transfer to frame-buffer memory.
     */

    unsigned char *dmaBounceBuffer;
    unsigned dmaBounceStride;
    unsigned dmaBounceLines;
    XvError xvErr;

} viaPortPrivRec, *viaPortPrivPtr;

/*
 * Structures for create surface
 */
typedef struct _SWDEVICE
{
 unsigned char * lpSWOverlaySurface[2];   /* Max 2 Pointers to SW Overlay Surface*/
 unsigned long  dwSWPhysicalAddr[2];     /*Max 2 Physical address to SW Overlay Surface */
 unsigned long  dwSWCbPhysicalAddr[2];  /* Physical address to SW Cb Overlay Surface, for YV12 format use */
 unsigned long  dwSWCrPhysicalAddr[2];  /* Physical address to SW Cr Overlay Surface, for YV12 format use */
 unsigned long  dwHQVAddr[3];             /* Physical address to HQV surface -- CLE_C0   */
 /*unsigned long  dwHQVAddr[2];*/             /*Max 2 Physical address to SW HQV Overlay Surface*/
 unsigned long  dwWidth;                  /*SW Source Width, not changed*/
 unsigned long  dwHeight;                 /*SW Source Height, not changed*/
 unsigned long  dwPitch;                  /*SW frame buffer pitch*/
 unsigned long  gdwSWSrcWidth;           /*SW Source Width, changed if window is out of screen*/
 unsigned long  gdwSWSrcHeight;          /*SW Source Height, changed if window is out of screen*/
 unsigned long  gdwSWDstWidth;           /*SW Destination Width*/
 unsigned long  gdwSWDstHeight;          /*SW Destination Height*/
 unsigned long  gdwSWDstLeft;            /*SW Position : Left*/
 unsigned long  gdwSWDstTop;             /*SW Position : Top*/
 unsigned long  dwDeinterlaceMode;        /*BOB / WEAVE*/
}SWDEVICE;
typedef SWDEVICE * LPSWDEVICE;

typedef struct _DDUPDATEOVERLAY
{
    CARD32 SrcLeft;
    CARD32 SrcTop;
    CARD32 SrcRight;
    CARD32 SrcBottom;

    CARD32 DstLeft;
    CARD32 DstTop;
    CARD32 DstRight;
    CARD32 DstBottom;

    unsigned long     dwFlags;        /* flags */
    unsigned long     dwColorSpaceLowValue;
} DDUPDATEOVERLAY;
typedef DDUPDATEOVERLAY * LPDDUPDATEOVERLAY;

typedef struct
{
    CARD32         dwWidth;
    CARD32         dwHeight;
    CARD32         dwOffset;
    CARD32         dwUVoffset;
    CARD32         dwFlipTime;
    CARD32         dwFlipTag;
    CARD32         dwStartAddr;
    CARD32         dwV1OriWidth;
    CARD32         dwV1OriHeight;
    CARD32         dwV1OriPitch;
    CARD32         dwV1SrcWidth;
    CARD32         dwV1SrcHeight;
    CARD32         dwV1SrcLeft;
    CARD32         dwV1SrcRight;
    CARD32         dwV1SrcTop;
    CARD32         dwV1SrcBot;
    CARD32         dwSPWidth;
    CARD32         dwSPHeight;
    CARD32         dwSPLeft;
    CARD32         dwSPRight;
    CARD32         dwSPTop;
    CARD32         dwSPBot;
    CARD32         dwSPOffset;
    CARD32         dwSPstartAddr;
    CARD32         dwDisplayPictStruct;
    CARD32         dwDisplayBuffIndex;        /* Display buffer Index. 0 to ( dwBufferNumber -1) */
    CARD32         dwFetchAlignment;
    CARD32         dwSPPitch;
    unsigned long  dwHQVAddr[3];          /* CLE_C0 */
    /*unsigned long   dwHQVAddr[2];*/
    CARD32         dwMPEGDeinterlaceMode; /* default value : VIA_DEINTERLACE_WEAVE */
    CARD32         dwMPEGProgressiveMode; /* default value : VIA_PROGRESSIVE */
    CARD32         dwHQVheapInfo;         /* video memory heap of the HQV buffer */
    CARD32         dwVideoControl;        /* video control flag */
    CARD32         dwminifyH;              /* Horizontal minify factor */
    CARD32         dwminifyV;              /* Vertical minify factor */
    CARD32         dwMpegDecoded;
} OVERLAYRECORD;

typedef struct  {
    unsigned long   gdwVideoFlagSW;
    unsigned long   gdwVideoFlagMPEG;
    unsigned long   gdwAlphaEnabled;        /* For Alpha blending use*/

    struct buffer_object  *HQVMem;
    struct buffer_object  *SWfbMem;

    CARD32 SrcFourCC;
    DDUPDATEOVERLAY UpdateOverlayBackup;    /* For HQVcontrol func use
                        // To save MPEG updateoverlay info.*/

/* device struct */
    SWDEVICE   SWDevice;
    OVERLAYRECORD   overlayRecordV1;
    OVERLAYRECORD   overlayRecordV3;

    Bool MPEG_ON;
    Bool SWVideo_ON;

/*To solve the bandwidth issue */
    unsigned long   gdwUseExtendedFIFO;

/* For panning mode use */
    int panning_x;
    int panning_y;
    int oldPanningX;
    int oldPanningY;

/* Maximum resolution with interpolation */
    unsigned long maxWInterp;
    unsigned long maxHInterp;

} swovRec, *swovPtr;

extern unsigned viaNumXvPorts;

#endif /* _VIA_XVPRIV_H_ */
