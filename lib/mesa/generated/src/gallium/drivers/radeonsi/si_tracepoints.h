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


#ifndef _SI_TRACEPOINTS_H
#define _SI_TRACEPOINTS_H

#include "si_perfetto.h"

#include "util/perf/u_trace.h"

#ifdef __cplusplus
extern "C" {
#endif


enum si_gpu_tracepoint {
   SI_GPU_TRACEPOINT_DRAW = 1ull << 0,
   SI_GPU_TRACEPOINT_COMPUTE = 1ull << 1,
};

extern uint64_t si_gpu_tracepoint;

void si_gpu_tracepoint_config_variable(void);


/*
 * si_begin_draw
 */
struct trace_si_begin_draw {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_si_begin_draw) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void si_ds_begin_draw(
   struct si_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_si_begin_draw *payload);
#endif
void __trace_si_begin_draw(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_si_begin_draw(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (si_gpu_tracepoint & SI_GPU_TRACEPOINT_DRAW)))
      return;
   __trace_si_begin_draw(
        ut
      , enabled_traces
   );
}

/*
 * si_end_draw
 */
struct trace_si_end_draw {
   uint32_t count;
};
#ifdef HAVE_PERFETTO
void si_ds_end_draw(
   struct si_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_si_end_draw *payload);
#endif
void __trace_si_end_draw(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t count
);
static ALWAYS_INLINE void trace_si_end_draw(
     struct u_trace *ut
   , uint32_t count
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (si_gpu_tracepoint & SI_GPU_TRACEPOINT_DRAW)))
      return;
   __trace_si_end_draw(
        ut
      , enabled_traces
      , count
   );
}

/*
 * si_begin_compute
 */
struct trace_si_begin_compute {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_si_begin_compute) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void si_ds_begin_compute(
   struct si_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_si_begin_compute *payload);
#endif
void __trace_si_begin_compute(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_si_begin_compute(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (si_gpu_tracepoint & SI_GPU_TRACEPOINT_COMPUTE)))
      return;
   __trace_si_begin_compute(
        ut
      , enabled_traces
   );
}

/*
 * si_end_compute
 */
struct trace_si_end_compute {
   uint32_t group_x;
   uint32_t group_y;
   uint32_t group_z;
};
#ifdef HAVE_PERFETTO
void si_ds_end_compute(
   struct si_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_si_end_compute *payload);
#endif
void __trace_si_end_compute(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t group_x
     , uint32_t group_y
     , uint32_t group_z
);
static ALWAYS_INLINE void trace_si_end_compute(
     struct u_trace *ut
   , uint32_t group_x
   , uint32_t group_y
   , uint32_t group_z
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (si_gpu_tracepoint & SI_GPU_TRACEPOINT_COMPUTE)))
      return;
   __trace_si_end_compute(
        ut
      , enabled_traces
      , group_x
      , group_y
      , group_z
   );
}

#ifdef __cplusplus
}
#endif

#endif /* _SI_TRACEPOINTS_H */
