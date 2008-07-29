/*
 * Copyright (c) 2003 NVIDIA, Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define SURFACE_FORMAT                                              0x00000300
#define SURFACE_FORMAT_DEPTH8                                       0x00000001
#define SURFACE_FORMAT_DEPTH15                                      0x00000002
#define SURFACE_FORMAT_DEPTH16                                      0x00000004
#define SURFACE_FORMAT_DEPTH24                                      0x00000006
#define SURFACE_PITCH                                               0x00000304
#define SURFACE_PITCH_SRC                                           15:0
#define SURFACE_PITCH_DST                                           31:16
#define SURFACE_OFFSET_SRC                                          0x00000308
#define SURFACE_OFFSET_DST                                          0x0000030C

#define ROP_SET                                                     0x00002300

#define PATTERN_FORMAT                                              0x00004300
#define PATTERN_FORMAT_DEPTH8                                       0x00000003
#define PATTERN_FORMAT_DEPTH16                                      0x00000001
#define PATTERN_FORMAT_DEPTH24                                      0x00000003
#define PATTERN_COLOR_0                                             0x00004310
#define PATTERN_COLOR_1                                             0x00004314
#define PATTERN_PATTERN_0                                           0x00004318
#define PATTERN_PATTERN_1                                           0x0000431C

#define CLIP_POINT                                                  0x00006300
#define CLIP_POINT_X                                                15:0
#define CLIP_POINT_Y                                                31:16
#define CLIP_SIZE                                                   0x00006304
#define CLIP_SIZE_WIDTH                                             15:0
#define CLIP_SIZE_HEIGHT                                            31:16

#define LINE_FORMAT                                                 0x00008300
#define LINE_FORMAT_DEPTH8                                          0x00000003
#define LINE_FORMAT_DEPTH16                                         0x00000001
#define LINE_FORMAT_DEPTH24                                         0x00000003
#define LINE_COLOR                                                  0x00008304
#define LINE_MAX_LINES                                              16
#define LINE_LINES(i)                                               0x00008400\
                                                                    +(i)*8
#define LINE_LINES_POINT0_X                                         15:0
#define LINE_LINES_POINT0_Y                                         31:16 
#define LINE_LINES_POINT1_X                                         47:32
#define LINE_LINES_POINT1_Y                                         63:48

#define BLIT_POINT_SRC                                              0x0000A300
#define BLIT_POINT_SRC_X                                            15:0
#define BLIT_POINT_SRC_Y                                            31:16
#define BLIT_POINT_DST                                              0x0000A304
#define BLIT_POINT_DST_X                                            15:0
#define BLIT_POINT_DST_Y                                            31:16
#define BLIT_SIZE                                                   0x0000A308
#define BLIT_SIZE_WIDTH                                             15:0
#define BLIT_SIZE_HEIGHT                                            31:16

#define RECT_FORMAT                                                 0x0000C300
#define RECT_FORMAT_DEPTH8                                          0x00000003
#define RECT_FORMAT_DEPTH16                                         0x00000001
#define RECT_FORMAT_DEPTH24                                         0x00000003
#define RECT_SOLID_COLOR                                            0x0000C3FC
#define RECT_SOLID_RECTS_MAX_RECTS                                  32
#define RECT_SOLID_RECTS(i)                                         0x0000C400\
                                                                    +(i)*8
#define RECT_SOLID_RECTS_Y                                          15:0
#define RECT_SOLID_RECTS_X                                          31:16
#define RECT_SOLID_RECTS_HEIGHT                                     47:32
#define RECT_SOLID_RECTS_WIDTH                                      63:48

#define RECT_EXPAND_ONE_COLOR_CLIP                                  0x0000C7EC
#define RECT_EXPAND_ONE_COLOR_CLIP_POINT0_X                         15:0
#define RECT_EXPAND_ONE_COLOR_CLIP_POINT0_Y                         31:16
#define RECT_EXPAND_ONE_COLOR_CLIP_POINT1_X                         47:32
#define RECT_EXPAND_ONE_COLOR_CLIP_POINT1_Y                         63:48
#define RECT_EXPAND_ONE_COLOR_COLOR                                 0x0000C7F4
#define RECT_EXPAND_ONE_COLOR_SIZE                                  0x0000C7F8
#define RECT_EXPAND_ONE_COLOR_SIZE_WIDTH                            15:0
#define RECT_EXPAND_ONE_COLOR_SIZE_HEIGHT                           31:16
#define RECT_EXPAND_ONE_COLOR_POINT                                 0x0000C7FC
#define RECT_EXPAND_ONE_COLOR_POINT_X                               15:0
#define RECT_EXPAND_ONE_COLOR_POINT_Y                               31:16
#define RECT_EXPAND_ONE_COLOR_DATA_MAX_DWORDS                       128
#define RECT_EXPAND_ONE_COLOR_DATA(i)                               0x0000C800\
                                                                    +(i)*4

#define RECT_EXPAND_TWO_COLOR_CLIP                                  0x0000CBE4
#define RECT_EXPAND_TWO_COLOR_CLIP_POINT0_X                         15:0
#define RECT_EXPAND_TWO_COLOR_CLIP_POINT0_Y                         31:16
#define RECT_EXPAND_TWO_COLOR_CLIP_POINT1_X                         47:32
#define RECT_EXPAND_TWO_COLOR_CLIP_POINT1_Y                         63:48
#define RECT_EXPAND_TWO_COLOR_COLOR_0                               0x0000CBEC
#define RECT_EXPAND_TWO_COLOR_COLOR_1                               0x0000CBF0
#define RECT_EXPAND_TWO_COLOR_SIZE_IN                               0x0000CBF4
#define RECT_EXPAND_TWO_COLOR_SIZE_IN_WIDTH                         15:0
#define RECT_EXPAND_TWO_COLOR_SIZE_IN_HEIGHT                        31:16
#define RECT_EXPAND_TWO_COLOR_SIZE_OUT                              0x0000CBF8
#define RECT_EXPAND_TWO_COLOR_SIZE_OUT_WIDTH                        15:0
#define RECT_EXPAND_TWO_COLOR_SIZE_OUT_HEIGHT                       31:16
#define RECT_EXPAND_TWO_COLOR_POINT                                 0x0000CBFC
#define RECT_EXPAND_TWO_COLOR_POINT_X                               15:0
#define RECT_EXPAND_TWO_COLOR_POINT_Y                               31:16
#define RECT_EXPAND_TWO_COLOR_DATA_MAX_DWORDS                       128
#define RECT_EXPAND_TWO_COLOR_DATA(i)                               0x0000CC00\
                                                                    +(i)*4

#define STRETCH_BLIT_FORMAT                                         0x0000E300
#define STRETCH_BLIT_FORMAT_DEPTH8                                  0x00000004
#define STRETCH_BLIT_FORMAT_DEPTH16                                 0x00000007
#define STRETCH_BLIT_FORMAT_DEPTH24                                 0x00000004
#define STRETCH_BLIT_FORMAT_A8R8G8B8                                0x00000003
#define STRETCH_BLIT_FORMAT_X8R8G8B8                                0x00000004
#define STRETCH_BLIT_FORMAT_YUYV                                    0x00000005
#define STRETCH_BLIT_FORMAT_UYVY                                    0x00000006
/* STRETCH_BLIT_OPERATION is only supported on TNT2 and newer */
#define STRETCH_BLIT_OPERATION                                      0x0000E304
#define STRETCH_BLIT_OPERATION_ROP                                  0x00000001
#define STRETCH_BLIT_OPERATION_COPY                                 0x00000003
#define STRETCH_BLIT_OPERATION_BLEND                                0x00000002
#define STRETCH_BLIT_CLIP_POINT                                     0x0000E308
#define STRETCH_BLIT_CLIP_POINT_X                                   15:0 
#define STRETCH_BLIT_CLIP_POINT_Y                                   31:16
#define STRETCH_BLIT_CLIP_POINT                                     0x0000E308
#define STRETCH_BLIT_CLIP_SIZE                                      0x0000E30C
#define STRETCH_BLIT_CLIP_SIZE_WIDTH                                15:0
#define STRETCH_BLIT_CLIP_SIZE_HEIGHT                               31:16
#define STRETCH_BLIT_DST_POINT                                      0x0000E310
#define STRETCH_BLIT_DST_POINT_X                                    15:0
#define STRETCH_BLIT_DST_POINT_Y                                    31:16
#define STRETCH_BLIT_DST_SIZE                                       0x0000E314
#define STRETCH_BLIT_DST_SIZE_WIDTH                                 15:0
#define STRETCH_BLIT_DST_SIZE_HEIGHT                                31:16
#define STRETCH_BLIT_DU_DX                                          0x0000E318
#define STRETCH_BLIT_DV_DY                                          0x0000E31C
#define STRETCH_BLIT_SRC_SIZE                                       0x0000E400
#define STRETCH_BLIT_SRC_SIZE_WIDTH                                 15:0
#define STRETCH_BLIT_SRC_SIZE_HEIGHT                                31:16
#define STRETCH_BLIT_SRC_FORMAT                                     0x0000E404
#define STRETCH_BLIT_SRC_FORMAT_PITCH                               15:0
#define STRETCH_BLIT_SRC_FORMAT_ORIGIN                              23:16
#define STRETCH_BLIT_SRC_FORMAT_ORIGIN_CENTER                       0x00000001
#define STRETCH_BLIT_SRC_FORMAT_ORIGIN_CORNER                       0x00000002
#define STRETCH_BLIT_SRC_FORMAT_FILTER                              31:24
#define STRETCH_BLIT_SRC_FORMAT_FILTER_POINT_SAMPLE                 0x00000000
#define STRETCH_BLIT_SRC_FORMAT_FILTER_BILINEAR                     0x00000001
#define STRETCH_BLIT_SRC_OFFSET                                     0x0000E408
#define STRETCH_BLIT_SRC_POINT                                      0x0000E40C
#define STRETCH_BLIT_SRC_POINT_U                                    15:0
#define STRETCH_BLIT_SRC_POINT_V                                    31:16
