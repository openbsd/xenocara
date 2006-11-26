#ifndef __SAVAGE_STREAMS_H__

# define __SAVAGE_STREAMS_H__

/**************************************
   S3 streams processor
**************************************/

#define EXT_MISC_CTRL2              0x67

/* New streams */

/* CR67[2] = 1 : enable secondary stream 1 */
#define ENABLE_STREAM1              0x04
/* CR67[1] = 1 : enable secondary stream 2 */
#define ENABLE_STREAM2              0x02
/* mask to clear CR67[2,1] */
#define NO_STREAMS                  0xF9
/* CR67[3] = 1 : Mem-mapped regs */
#define USE_MM_FOR_PRI_STREAM       0x08

#define HDM_SHIFT	16
#define HDSCALE_4	(2 << HDM_SHIFT)
#define HDSCALE_8	(3 << HDM_SHIFT)
#define HDSCALE_16	(4 << HDM_SHIFT)
#define HDSCALE_32	(5 << HDM_SHIFT)
#define HDSCALE_64	(6 << HDM_SHIFT)

/* Old Streams */

#define ENABLE_STREAMS_OLD	    0x0c
#define NO_STREAMS_OLD		    0xf3
/* CR69[0] = 1 : Mem-mapped regs */
#define USE_MM_FOR_PRI_STREAM_OLD   0x01


/*
 * There are two different streams engines used in the Savage line.
 * The old engine is in the 3D, 4, Pro, and Twister.
 * The new engine is in the 2000, MX, IX, and Super.
 */


/* streams registers for old engine */
#define PSTREAM_CONTROL_REG		0x8180
#define COL_CHROMA_KEY_CONTROL_REG	0x8184
#define SSTREAM_CONTROL_REG		0x8190
#define CHROMA_KEY_UPPER_BOUND_REG	0x8194
#define SSTREAM_STRETCH_REG		0x8198
#define COLOR_ADJUSTMENT_REG		0x819C
#define BLEND_CONTROL_REG		0x81A0
#define PSTREAM_FBADDR0_REG		0x81C0
#define PSTREAM_FBADDR1_REG		0x81C4
#define PSTREAM_STRIDE_REG		0x81C8
#define DOUBLE_BUFFER_REG		0x81CC
/* updated by peterzhu,original define is DOUBLE_BUFFER_REG*/
#define MULTIPLE_BUFFER_REG             0x81CC
#define SSTREAM_FBADDR0_REG		0x81D0
#define SSTREAM_FBADDR1_REG		0x81D4
#define SSTREAM_STRIDE_REG		0x81D8
#define SSTREAM_VSCALE_REG		0x81E0
#define SSTREAM_VINITIAL_REG		0x81E4
#define SSTREAM_LINES_REG		0x81E8
#define STREAMS_FIFO_REG		0x81EC
#define PSTREAM_WINDOW_START_REG	0x81F0
#define PSTREAM_WINDOW_SIZE_REG		0x81F4
#define SSTREAM_WINDOW_START_REG	0x81F8
#define SSTREAM_WINDOW_SIZE_REG		0x81FC
#define FIFO_CONTROL			0x8200
#define PSTREAM_FBSIZE_REG		0x8300
#define SSTREAM_FBSIZE_REG		0x8304
#define SSTREAM_FBADDR2_REG		0x8308

#define OS_XY(x,y)	(((x)<<16)|(y+1)) /*(((x+1)<<16)|(y+1))*/
#define OS_WH(x,y)	(((x-1)<<16)|(y))

/* Streams Processor macros */
#define H_Shift                 0
#define H_Mask                  (((1L << 11) - 1) << H_Shift)
#define W_Shift                 16
#define W_Mask                  (((1L << 11) - 1) << W_Shift)
                                                                                                                    
#define Y_Shift                 0
#define Y_Mask                  (((1L << 11) - 1) << Y_Shift)
#define X_Shift                 16
#define X_Mask                  (((1L << 11) - 1) << X_Shift)
                                                                                                                    
#define XY(x,y)      ((((x+1)<<X_Shift)&X_Mask) | (((y+1)<<Y_Shift)&Y_Mask))
#define WH(w,h)      ((((w-1)<<W_Shift)&W_Mask) | (((h)<<H_Shift)&H_Mask))

#define HSCALING_Shift    0
#define HSCALING_Mask     (((1L << 16)-1) << HSCALING_Shift)
#define HSCALING(w0,w1)   ((((unsigned int)(((double)w0/(double)w1) * (1 << 15))) \
                               << HSCALING_Shift) \
                           & HSCALING_Mask)
                                                                                                                    
#define VSCALING_Shift    0
#define VSCALING_Mask     (((1L << 20)-1) << VSCALING_Shift)
#define VSCALING(h0,h1)   ((((unsigned int) (((double)h0/(double)h1) * (1 << 15))) \
                               << VSCALING_Shift) \
                           & VSCALING_Mask)

/* New Streams Processor */
/* Stream Processor 1 */

/* Primary Stream 1 Frame Buffer Address 0 */
#define PRI_STREAM_FBUF_ADDR0           0x81c0
/* Primary Stream 1 Frame Buffer Address 0 */
#define PRI_STREAM_FBUF_ADDR1           0x81c4
/* Primary Stream 1 Stride */
#define PRI_STREAM_STRIDE               0x81c8
/* Primary Stream 1 Frame Buffer Size */
#define PRI_STREAM_BUFFERSIZE           0x8214

/* Secondary stream 1 Color/Chroma Key Control */
#define SEC_STREAM_CKEY_LOW             0x8184
/* Secondary stream 1 Chroma Key Upper Bound */
#define SEC_STREAM_CKEY_UPPER           0x8194
/* Blend Control of Secondary Stream 1 & 2 */
#define BLEND_CONTROL                   0x8190
/* Secondary Stream 1 Color conversion/Adjustment 1 */
#define SEC_STREAM_COLOR_CONVERT1       0x8198
/* Secondary Stream 1 Color conversion/Adjustment 2 */
#define SEC_STREAM_COLOR_CONVERT2       0x819c
/* Secondary Stream 1 Color conversion/Adjustment 3 */
#define SEC_STREAM_COLOR_CONVERT3       0x81e4
/* Secondary Stream 1 Horizontal Scaling */
#define SEC_STREAM_HSCALING             0x81a0
/* Secondary Stream 1 Frame Buffer Size */
#define SEC_STREAM_BUFFERSIZE           0x81a8
/* Secondary Stream 1 Horizontal Scaling Normalization (2K only) */
#define SEC_STREAM_HSCALE_NORMALIZE	0x81ac
/* Secondary Stream 1 Horizontal Scaling */
#define SEC_STREAM_VSCALING             0x81e8
/* Secondary Stream 1 Frame Buffer Address 0 */
#define SEC_STREAM_FBUF_ADDR0           0x81d0
/* Secondary Stream 1 Frame Buffer Address 1 */
#define SEC_STREAM_FBUF_ADDR1           0x81d4
/* Secondary Stream 1 Frame Buffer Address 2 */
#define SEC_STREAM_FBUF_ADDR2           0x81ec
/* Secondary Stream 1 Stride */
#define SEC_STREAM_STRIDE               0x81d8
/* Secondary Stream 1 Window Start Coordinates */
#define SEC_STREAM_WINDOW_START         0x81f8
/* Secondary Stream 1 Window Size */
#define SEC_STREAM_WINDOW_SZ            0x81fc
/* Secondary Streams Tile Offset */
#define SEC_STREAM_TILE_OFF             0x821c
/* Secondary Stream 1 Opaque Overlay Control */
#define SEC_STREAM_OPAQUE_OVERLAY       0x81dc


/* Stream Processor 2 */

/* Primary Stream 2 Frame Buffer Address 0 */
#define PRI_STREAM2_FBUF_ADDR0          0x81b0
/* Primary Stream 2 Frame Buffer Address 1 */
#define PRI_STREAM2_FBUF_ADDR1          0x81b4
/* Primary Stream 2 Stride */
#define PRI_STREAM2_STRIDE              0x81b8
/* Primary Stream 2 Frame Buffer Size */
#define PRI_STREAM2_BUFFERSIZE          0x8218

/* Secondary Stream 2 Color/Chroma Key Control */
#define SEC_STREAM2_CKEY_LOW            0x8188
/* Secondary Stream 2 Chroma Key Upper Bound */
#define SEC_STREAM2_CKEY_UPPER          0x818c
/* Secondary Stream 2 Horizontal Scaling */
#define SEC_STREAM2_HSCALING            0x81a4
/* Secondary Stream 2 Horizontal Scaling */
#define SEC_STREAM2_VSCALING            0x8204
/* Secondary Stream 2 Frame Buffer Size */
#define SEC_STREAM2_BUFFERSIZE          0x81ac
/* Secondary Stream 2 Frame Buffer Address 0 */
#define SEC_STREAM2_FBUF_ADDR0          0x81bc
/* Secondary Stream 2 Frame Buffer Address 1 */
#define SEC_STREAM2_FBUF_ADDR1          0x81e0
/* Secondary Stream 2 Frame Buffer Address 2 */
#define SEC_STREAM2_FBUF_ADDR2          0x8208
/* Multiple Buffer/LPB and Secondary Stream 2 Stride */
#define SEC_STREAM2_STRIDE_LPB          0x81cc
/* Secondary Stream 2 Color conversion/Adjustment 1 */
#define SEC_STREAM2_COLOR_CONVERT1      0x81f0
/* Secondary Stream 2 Color conversion/Adjustment 2 */
#define SEC_STREAM2_COLOR_CONVERT2      0x81f4
/* Secondary Stream 2 Color conversion/Adjustment 3 */
#define SEC_STREAM2_COLOR_CONVERT3      0x8200
/* Secondary Stream 2 Window Start Coordinates */
#define SEC_STREAM2_WINDOW_START        0x820c
/* Secondary Stream 2 Window Size */
#define SEC_STREAM2_WINDOW_SZ           0x8210
/* Secondary Stream 2 Opaque Overlay Control */
#define SEC_STREAM2_OPAQUE_OVERLAY      0x8180

/* savage 2000 */
#define SEC_STREAM_COLOR_CONVERT0_2000       0x8198
#define SEC_STREAM_COLOR_CONVERT1_2000       0x819c
#define SEC_STREAM_COLOR_CONVERT2_2000       0x81e0
#define SEC_STREAM_COLOR_CONVERT3_2000       0x81e4
#define SEC_STREAM_SRC_START_2000            0x818c
#define SEC_STREAM_SRC_SIZE_2000             0x81a8
#define SEC_STREAM_BUFFERSIZE_2000           0x81a4
#define S_SRC_H_Mask            0x00000fff
#define S_SRC_W_Shift           16
#define S_SRC_W_Mask            0x0fff0000
#define SRCSIZE(w,h)   (((w <<S_SRC_W_Shift) & S_SRC_W_Mask) | (h & S_SRC_H_Mask))
#define SRCSTART(x,y)  (((x <<S_SRC_W_Shift) & 0x7ff0000) | (y & 0x000007ff))
#define VSCALING_2000(h0,h1)   ((unsigned int) (((float)h0/(float)h1) * (float)(65536.0) ))
#define HSCALING_NORMALIZE(h0,h1)  ((unsigned int) ((float)2048.0 * ((float)h0/ (float)h1))) << 16
#define HSCALING_2000(w0,w1)   ((unsigned int) (((float)w0/ (float)w1) * (float)(65536.0)))
#define XY_2000(x,y)      ((((x)<<X_Shift)&X_Mask) | (((y)<<Y_Shift)&Y_Mask))
#define WH_2000(w,h)      ((((w)<<W_Shift)&W_Mask) | (((h)<<H_Shift)&H_Mask))

#define BASE_PAD 0xf

#define  STREAMS_MODE32 0x7
#define  STREAMS_MODE24 0x6
#define  STREAMS_MODE16 0x5 /* @@@ */

#define SSTREAMS_MODE(bpp) (bpp > 16 ? (bpp > 24 ? STREAMS_MODE32 :\
				       STREAMS_MODE24) : STREAMS_MODE16)

#endif /*__SAVAGE_STREAMS_H__*/
