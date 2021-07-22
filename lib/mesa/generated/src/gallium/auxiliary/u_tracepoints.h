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

#include "util/u_trace.h"

void __trace_surface(struct u_trace *ut
     , const struct pipe_surface * psurf
);
static inline void trace_surface(struct u_trace *ut
     , const struct pipe_surface * psurf
) {
   if (likely(!ut->enabled))
      return;
   __trace_surface(ut
        , psurf
   );
}
void __trace_framebuffer(struct u_trace *ut
     , const struct pipe_framebuffer_state * pfb
);
static inline void trace_framebuffer(struct u_trace *ut
     , const struct pipe_framebuffer_state * pfb
) {
   if (likely(!ut->enabled))
      return;
   __trace_framebuffer(ut
        , pfb
   );
}
void __trace_grid_info(struct u_trace *ut
     , const struct pipe_grid_info * pgrid
);
static inline void trace_grid_info(struct u_trace *ut
     , const struct pipe_grid_info * pgrid
) {
   if (likely(!ut->enabled))
      return;
   __trace_grid_info(ut
        , pgrid
   );
}

#endif /* _U_TRACEPOINTS_H */
