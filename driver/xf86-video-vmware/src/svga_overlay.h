/* **********************************************************
 * Copyright 2007 VMware, Inc.  All rights reserved.
 * **********************************************************/

/*
 * svga_overlay.h --
 *
 *    Definitions for video-overlay support.
 */

#ifndef _SVGA_OVERLAY_H_
#define _SVGA_OVERLAY_H_

/*
 * Video formats we support
 */

#define VMWARE_FOURCC_YV12 0x32315659 /* 'Y' 'V' '1' '2' */
#define VMWARE_FOURCC_YUY2 0x32595559 /* 'Y' 'U' 'Y' '2' */
#define VMWARE_FOURCC_UYVY 0x59565955 /* 'U' 'Y' 'V' 'Y' */

#define SVGA_ESCAPE_VMWARE_VIDEO             0x00020000

#define SVGA_ESCAPE_VMWARE_VIDEO_SET_REGS    0x00020001
        /* FIFO escape layout:
         * Type, Stream Id, (Register Id, Value) pairs */

#define SVGA_ESCAPE_VMWARE_VIDEO_FLUSH       0x00020002
        /* FIFO escape layout:
         * Type, Stream Id */

typedef struct SVGAEscapeVideoSetRegs {
   struct {
      uint32 cmdType;
      uint32 streamId;
   } header;

   /* May include zero or more items. */
   struct {
      uint32 registerId;
      uint32 value;
   } items[1];
} SVGAEscapeVideoSetRegs;

typedef struct SVGAEscapeVideoFlush {
   uint32 cmdType;
   uint32 streamId;
} SVGAEscapeVideoFlush;

#endif /* _SVGA_OVERLAY_H_ */
