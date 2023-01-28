/* Copyright (C) 2020 Google, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */


#ifndef _U_TRACEPOINTS_H
#define _U_TRACEPOINTS_H

#include "pipe/p_state.h"
#include "util/format/u_format.h"

#include "util/perf/u_trace.h"

#ifdef __cplusplus
extern "C" {
#endif




/*
 * surface
 */
struct trace_surface {
   uint16_t width;
   uint16_t height;
   uint8_t nr_samples;
   const char * format;
};
void __trace_surface(
       struct u_trace *ut
     , void *cs
     , const struct pipe_surface * psurf
);
static ALWAYS_INLINE void trace_surface(
     struct u_trace *ut
   , void *cs
   , const struct pipe_surface * psurf
) {
   if (!unlikely(u_trace_instrument() &&
                 true))
      return;
   __trace_surface(
        ut
      , cs
      , psurf
   );
}

/*
 * framebuffer
 */
struct trace_framebuffer {
   uint16_t width;
   uint16_t height;
   uint8_t layers;
   uint8_t samples;
   uint8_t nr_cbufs;
};
void __trace_framebuffer(
       struct u_trace *ut
     , void *cs
     , const struct pipe_framebuffer_state * pfb
);
static ALWAYS_INLINE void trace_framebuffer(
     struct u_trace *ut
   , void *cs
   , const struct pipe_framebuffer_state * pfb
) {
   if (!unlikely(u_trace_instrument() &&
                 true))
      return;
   __trace_framebuffer(
        ut
      , cs
      , pfb
   );
}

/*
 * grid_info
 */
struct trace_grid_info {
   uint8_t work_dim;
   uint16_t block_x;
   uint16_t block_y;
   uint16_t block_z;
   uint16_t grid_x;
   uint16_t grid_y;
   uint16_t grid_z;
};
void __trace_grid_info(
       struct u_trace *ut
     , void *cs
     , const struct pipe_grid_info * pgrid
);
static ALWAYS_INLINE void trace_grid_info(
     struct u_trace *ut
   , void *cs
   , const struct pipe_grid_info * pgrid
) {
   if (!unlikely(u_trace_instrument() &&
                 true))
      return;
   __trace_grid_info(
        ut
      , cs
      , pgrid
   );
}

#ifdef __cplusplus
}
#endif

#endif /* _U_TRACEPOINTS_H */
