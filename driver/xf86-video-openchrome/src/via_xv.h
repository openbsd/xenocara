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
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _VIA_VIDEO_H_
#define _VIA_VIDEO_H_ 1

/*
 * I N C L U D E S
 */

/*#define   XV_DEBUG        1            write log msg to /var/log/XFree86.0.log */
#define   COLOR_KEY       1	       /* set color key value from driver layer */

#define HW_3123

/* Definition for VideoStatus */
#define VIDEO_NULL                  0x00000000
#define VIDEO_SWOV_SURFACE_CREATED  0x00000001
#define VIDEO_SWOV_ON               0x00000002

#define SINGLE_3205_100 0.41
#define SINGLE_3205_133 0.70

#define VIDEO_BPP 2


#define V1_COMMAND_FIRE               0x80000000  /* V1 commands fire */
#define V3_COMMAND_FIRE               0x40000000  /* V3 commands fire */

typedef struct
{
    CARD32 interruptflag;	       /* 200 */
    CARD32 ramtab;		       /* 204 */
    CARD32 alphawin_hvstart;	       /* 208 */
    CARD32 alphawin_size;	       /* 20c */
    CARD32 alphawin_ctl;	       /* 210 */
    CARD32 crt_startaddr;	       /* 214 */
    CARD32 crt_startaddr_2;	       /* 218 */
    CARD32 alphafb_stride;	       /* 21c */
    CARD32 color_key;		       /* 220 */
    CARD32 alphafb_addr;	       /* 224 */
    CARD32 chroma_low;		       /* 228 */
    CARD32 chroma_up;		       /* 22c */
    CARD32 video1_ctl;		       /* 230 */
    CARD32 video1_fetch;	       /* 234 */
    CARD32 video1y_addr1;	       /* 238 */
    CARD32 video1_stride;	       /* 23c */
    CARD32 video1_hvstart;	       /* 240 */
    CARD32 video1_size;		       /* 244 */
    CARD32 video1y_addr2;	       /* 248 */
    CARD32 video1_zoom;		       /* 24c */
    CARD32 video1_mictl;	       /* 250 */
    CARD32 video1y_addr0;	       /* 254 */
    CARD32 video1_fifo;		       /* 258 */
    CARD32 video1y_addr3;	       /* 25c */
    CARD32 hi_control;		       /* 260 */
    CARD32 snd_color_key;	       /* 264 */
    CARD32 v3alpha_prefifo;	       /* 268 */
    CARD32 v1_source_w_h;	       /* 26c */
    CARD32 hi_transparent_color;       /* 270 */
    CARD32 v_display_temp;	       /* 274 :No use */
    CARD32 v3alpha_fifo;	       /* 278 */
    CARD32 v3_source_width;	       /* 27c */
    CARD32 dummy1;		       /* 280 */
    CARD32 video1_CSC1;		       /* 284 */
    CARD32 video1_CSC2;		       /* 288 */
    CARD32 video1u_addr0;	       /* 28c */
    CARD32 video1_opqctl;	       /* 290 */
    CARD32 video3_opqctl;	       /* 294 */
    CARD32 compose;		       /* 298 */
    CARD32 dummy2;		       /* 29c */
    CARD32 video3_ctl;		       /* 2a0 */
    CARD32 video3_addr0;	       /* 2a4 */
    CARD32 video3_addr1;	       /* 2a8 */
    CARD32 video3_stride;	       /* 2ac */
    CARD32 video3_hvstart;	       /* 2b0 */
    CARD32 video3_size;		       /* 2b4 */
    CARD32 v3alpha_fetch;	       /* 2b8 */
    CARD32 video3_zoom;		       /* 2bc */
    CARD32 video3_mictl;	       /* 2c0 */
    CARD32 video3_CSC1;		       /* 2c4 */
    CARD32 video3_CSC2;		       /* 2c8 */
    CARD32 v3_display_temp;	       /* 2cc */
    CARD32 reserved[5];		       /* 2d0 */
    CARD32 video1u_addr1;	       /* 2e4 */
    CARD32 video1u_addr2;	       /* 2e8 */
    CARD32 video1u_addr3;	       /* 2ec */
    CARD32 video1v_addr0;	       /* 2f0 */
    CARD32 video1v_addr1;	       /* 2f4 */
    CARD32 video1v_addr2;	       /* 2f8 */
    CARD32 video1v_addr3;	       /* 2fc */
} video_via_regs;

#define vmmtr volatile video_via_regs *

#endif /* _VIA_VIDEO_H_ */
