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


#ifndef _INTEL_TRACEPOINTS_H
#define _INTEL_TRACEPOINTS_H

#include "blorp/blorp_priv.h"
#include "ds/intel_driver_ds.h"

#include "util/perf/u_trace.h"

#ifdef __cplusplus
extern "C" {
#endif


enum intel_gpu_tracepoint {
   INTEL_GPU_TRACEPOINT_FRAME = 1ull << 0,
   INTEL_GPU_TRACEPOINT_QUEUE_ANNOTATION = 1ull << 1,
   INTEL_GPU_TRACEPOINT_BATCH = 1ull << 2,
   INTEL_GPU_TRACEPOINT_CMD_BUFFER = 1ull << 3,
   INTEL_GPU_TRACEPOINT_CMD_BUFFER_ANNOTATION = 1ull << 4,
   INTEL_GPU_TRACEPOINT_XFB = 1ull << 5,
   INTEL_GPU_TRACEPOINT_RENDER_PASS = 1ull << 6,
   INTEL_GPU_TRACEPOINT_BLORP = 1ull << 7,
   INTEL_GPU_TRACEPOINT_WRITE_BUFFER_MARKER = 1ull << 8,
   INTEL_GPU_TRACEPOINT_GENERATE_DRAWS = 1ull << 9,
   INTEL_GPU_TRACEPOINT_GENERATE_COMMANDS = 1ull << 10,
   INTEL_GPU_TRACEPOINT_QUERY_CLEAR_BLORP = 1ull << 11,
   INTEL_GPU_TRACEPOINT_QUERY_CLEAR_CS = 1ull << 12,
   INTEL_GPU_TRACEPOINT_QUERY_COPY_CS = 1ull << 13,
   INTEL_GPU_TRACEPOINT_QUERY_COPY_SHADER = 1ull << 14,
   INTEL_GPU_TRACEPOINT_DRAW = 1ull << 15,
   INTEL_GPU_TRACEPOINT_DRAW_MULTI = 1ull << 16,
   INTEL_GPU_TRACEPOINT_DRAW_INDEXED = 1ull << 17,
   INTEL_GPU_TRACEPOINT_DRAW_INDEXED_MULTI = 1ull << 18,
   INTEL_GPU_TRACEPOINT_DRAW_INDIRECT_BYTE_COUNT = 1ull << 19,
   INTEL_GPU_TRACEPOINT_DRAW_INDIRECT = 1ull << 20,
   INTEL_GPU_TRACEPOINT_DRAW_INDEXED_INDIRECT = 1ull << 21,
   INTEL_GPU_TRACEPOINT_DRAW_INDIRECT_COUNT = 1ull << 22,
   INTEL_GPU_TRACEPOINT_DRAW_INDEXED_INDIRECT_COUNT = 1ull << 23,
   INTEL_GPU_TRACEPOINT_DRAW_MESH = 1ull << 24,
   INTEL_GPU_TRACEPOINT_DRAW_MESH_INDIRECT = 1ull << 25,
   INTEL_GPU_TRACEPOINT_DRAW_MESH_INDIRECT_COUNT = 1ull << 26,
   INTEL_GPU_TRACEPOINT_COMPUTE = 1ull << 27,
   INTEL_GPU_TRACEPOINT_COMPUTE_INDIRECT = 1ull << 28,
   INTEL_GPU_TRACEPOINT_TRACE_COPY = 1ull << 29,
   INTEL_GPU_TRACEPOINT_TRACE_COPY_CB = 1ull << 30,
   INTEL_GPU_TRACEPOINT_AS_BUILD = 1ull << 31,
   INTEL_GPU_TRACEPOINT_AS_BUILD_LEAVES = 1ull << 32,
   INTEL_GPU_TRACEPOINT_AS_MORTON_GENERATE = 1ull << 33,
   INTEL_GPU_TRACEPOINT_AS_MORTON_SORT = 1ull << 34,
   INTEL_GPU_TRACEPOINT_AS_LBVH_BUILD_INTERNAL = 1ull << 35,
   INTEL_GPU_TRACEPOINT_AS_PLOC_BUILD_INTERNAL = 1ull << 36,
   INTEL_GPU_TRACEPOINT_AS_ENCODE = 1ull << 37,
   INTEL_GPU_TRACEPOINT_AS_COPY = 1ull << 38,
   INTEL_GPU_TRACEPOINT_RAYS = 1ull << 39,
   INTEL_GPU_TRACEPOINT_STALL = 1ull << 40,
};

extern uint64_t intel_gpu_tracepoint;

void intel_gpu_tracepoint_config_variable(void);


/*
 * intel_begin_frame
 */
struct trace_intel_begin_frame {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_frame) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_frame(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_frame *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_frame(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , void *cs
);
static ALWAYS_INLINE void trace_intel_begin_frame(
     struct u_trace *ut
   , void *cs
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_FRAME)))
      return;
   __trace_intel_begin_frame(
        ut
      , enabled_traces
      , cs
   );
}

/*
 * intel_end_frame
 */
struct trace_intel_end_frame {
   uint32_t frame;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_frame(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_frame *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_frame(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , void *cs
     , uint32_t frame
);
static ALWAYS_INLINE void trace_intel_end_frame(
     struct u_trace *ut
   , void *cs
   , uint32_t frame
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_FRAME)))
      return;
   __trace_intel_end_frame(
        ut
      , enabled_traces
      , cs
      , frame
   );
}

/*
 * intel_begin_queue_annotation
 */
struct trace_intel_begin_queue_annotation {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_queue_annotation) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_queue_annotation(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_queue_annotation *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_queue_annotation(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , void *cs
);
static ALWAYS_INLINE void trace_intel_begin_queue_annotation(
     struct u_trace *ut
   , void *cs
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_QUEUE_ANNOTATION)))
      return;
   __trace_intel_begin_queue_annotation(
        ut
      , enabled_traces
      , cs
   );
}

/*
 * intel_end_queue_annotation
 */
struct trace_intel_end_queue_annotation {
   uint8_t dummy;
   char str[0];
};
#ifdef HAVE_PERFETTO
void intel_ds_end_queue_annotation(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_queue_annotation *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_queue_annotation(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , void *cs
     , unsigned len
     , const char *str
);
static ALWAYS_INLINE void trace_intel_end_queue_annotation(
     struct u_trace *ut
   , void *cs
   , unsigned len
   , const char *str
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_QUEUE_ANNOTATION)))
      return;
   __trace_intel_end_queue_annotation(
        ut
      , enabled_traces
      , cs
      , len
      , str
   );
}

/*
 * intel_begin_batch
 */
struct trace_intel_begin_batch {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_batch) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_batch(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_batch *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_batch(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_batch(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_BATCH)))
      return;
   __trace_intel_begin_batch(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_batch
 */
struct trace_intel_end_batch {
   uint8_t name;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_batch(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_batch *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_batch(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint8_t name
);
static ALWAYS_INLINE void trace_intel_end_batch(
     struct u_trace *ut
   , uint8_t name
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_BATCH)))
      return;
   __trace_intel_end_batch(
        ut
      , enabled_traces
      , name
   );
}

/*
 * intel_begin_cmd_buffer
 */
struct trace_intel_begin_cmd_buffer {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_cmd_buffer) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_cmd_buffer(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_cmd_buffer *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_cmd_buffer(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_cmd_buffer(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_CMD_BUFFER)))
      return;
   __trace_intel_begin_cmd_buffer(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_cmd_buffer
 */
struct trace_intel_end_cmd_buffer {
   uint8_t level;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_cmd_buffer(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_cmd_buffer *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_cmd_buffer(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint8_t level
);
static ALWAYS_INLINE void trace_intel_end_cmd_buffer(
     struct u_trace *ut
   , uint8_t level
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_CMD_BUFFER)))
      return;
   __trace_intel_end_cmd_buffer(
        ut
      , enabled_traces
      , level
   );
}

/*
 * intel_begin_cmd_buffer_annotation
 */
struct trace_intel_begin_cmd_buffer_annotation {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_cmd_buffer_annotation) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_cmd_buffer_annotation(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_cmd_buffer_annotation *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_cmd_buffer_annotation(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_cmd_buffer_annotation(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_CMD_BUFFER_ANNOTATION)))
      return;
   __trace_intel_begin_cmd_buffer_annotation(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_cmd_buffer_annotation
 */
struct trace_intel_end_cmd_buffer_annotation {
   uint8_t dummy;
   char str[0];
};
#ifdef HAVE_PERFETTO
void intel_ds_end_cmd_buffer_annotation(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_cmd_buffer_annotation *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_cmd_buffer_annotation(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , unsigned len
     , const char *str
);
static ALWAYS_INLINE void trace_intel_end_cmd_buffer_annotation(
     struct u_trace *ut
   , unsigned len
   , const char *str
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_CMD_BUFFER_ANNOTATION)))
      return;
   __trace_intel_end_cmd_buffer_annotation(
        ut
      , enabled_traces
      , len
      , str
   );
}

/*
 * intel_begin_xfb
 */
struct trace_intel_begin_xfb {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_xfb) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_xfb(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_xfb *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_xfb(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_xfb(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_XFB)))
      return;
   __trace_intel_begin_xfb(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_xfb
 */
struct trace_intel_end_xfb {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_end_xfb) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_end_xfb(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_xfb *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_xfb(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_end_xfb(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_XFB)))
      return;
   __trace_intel_end_xfb(
        ut
      , enabled_traces
   );
}

/*
 * intel_begin_render_pass
 */
struct trace_intel_begin_render_pass {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_render_pass) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_render_pass(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_render_pass *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_render_pass(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_render_pass(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_RENDER_PASS)))
      return;
   __trace_intel_begin_render_pass(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_render_pass
 */
struct trace_intel_end_render_pass {
   uint16_t width;
   uint16_t height;
   uint8_t att_count;
   uint8_t msaa;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_render_pass(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_render_pass *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_render_pass(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint16_t width
     , uint16_t height
     , uint8_t att_count
     , uint8_t msaa
);
static ALWAYS_INLINE void trace_intel_end_render_pass(
     struct u_trace *ut
   , uint16_t width
   , uint16_t height
   , uint8_t att_count
   , uint8_t msaa
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_RENDER_PASS)))
      return;
   __trace_intel_end_render_pass(
        ut
      , enabled_traces
      , width
      , height
      , att_count
      , msaa
   );
}

/*
 * intel_begin_blorp
 */
struct trace_intel_begin_blorp {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_blorp) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_blorp(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_blorp *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_blorp(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_blorp(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_BLORP)))
      return;
   __trace_intel_begin_blorp(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_blorp
 */
struct trace_intel_end_blorp {
   enum blorp_op op;
   uint32_t width;
   uint32_t height;
   uint32_t samples;
   enum blorp_shader_pipeline shader_pipe;
   enum isl_format dst_fmt;
   enum isl_format src_fmt;
   uint8_t predicated;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_blorp(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_blorp *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_blorp(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , enum blorp_op op
     , uint32_t width
     , uint32_t height
     , uint32_t samples
     , enum blorp_shader_pipeline shader_pipe
     , enum isl_format dst_fmt
     , enum isl_format src_fmt
     , uint8_t predicated
);
static ALWAYS_INLINE void trace_intel_end_blorp(
     struct u_trace *ut
   , enum blorp_op op
   , uint32_t width
   , uint32_t height
   , uint32_t samples
   , enum blorp_shader_pipeline shader_pipe
   , enum isl_format dst_fmt
   , enum isl_format src_fmt
   , uint8_t predicated
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_BLORP)))
      return;
   __trace_intel_end_blorp(
        ut
      , enabled_traces
      , op
      , width
      , height
      , samples
      , shader_pipe
      , dst_fmt
      , src_fmt
      , predicated
   );
}

/*
 * intel_begin_write_buffer_marker
 */
struct trace_intel_begin_write_buffer_marker {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_write_buffer_marker) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_write_buffer_marker(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_write_buffer_marker *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_write_buffer_marker(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_write_buffer_marker(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_WRITE_BUFFER_MARKER)))
      return;
   __trace_intel_begin_write_buffer_marker(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_write_buffer_marker
 */
struct trace_intel_end_write_buffer_marker {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_end_write_buffer_marker) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_end_write_buffer_marker(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_write_buffer_marker *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_write_buffer_marker(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_end_write_buffer_marker(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_WRITE_BUFFER_MARKER)))
      return;
   __trace_intel_end_write_buffer_marker(
        ut
      , enabled_traces
   );
}

/*
 * intel_begin_generate_draws
 */
struct trace_intel_begin_generate_draws {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_generate_draws) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_generate_draws(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_generate_draws *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_generate_draws(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_generate_draws(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_GENERATE_DRAWS)))
      return;
   __trace_intel_begin_generate_draws(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_generate_draws
 */
struct trace_intel_end_generate_draws {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_end_generate_draws) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_end_generate_draws(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_generate_draws *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_generate_draws(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_end_generate_draws(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_GENERATE_DRAWS)))
      return;
   __trace_intel_end_generate_draws(
        ut
      , enabled_traces
   );
}

/*
 * intel_begin_generate_commands
 */
struct trace_intel_begin_generate_commands {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_generate_commands) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_generate_commands(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_generate_commands *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_generate_commands(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_generate_commands(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_GENERATE_COMMANDS)))
      return;
   __trace_intel_begin_generate_commands(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_generate_commands
 */
struct trace_intel_end_generate_commands {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_end_generate_commands) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_end_generate_commands(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_generate_commands *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_generate_commands(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_end_generate_commands(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_GENERATE_COMMANDS)))
      return;
   __trace_intel_end_generate_commands(
        ut
      , enabled_traces
   );
}

/*
 * intel_begin_query_clear_blorp
 */
struct trace_intel_begin_query_clear_blorp {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_query_clear_blorp) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_query_clear_blorp(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_query_clear_blorp *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_query_clear_blorp(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_query_clear_blorp(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_QUERY_CLEAR_BLORP)))
      return;
   __trace_intel_begin_query_clear_blorp(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_query_clear_blorp
 */
struct trace_intel_end_query_clear_blorp {
   uint32_t count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_query_clear_blorp(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_query_clear_blorp *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_query_clear_blorp(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t count
);
static ALWAYS_INLINE void trace_intel_end_query_clear_blorp(
     struct u_trace *ut
   , uint32_t count
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_QUERY_CLEAR_BLORP)))
      return;
   __trace_intel_end_query_clear_blorp(
        ut
      , enabled_traces
      , count
   );
}

/*
 * intel_begin_query_clear_cs
 */
struct trace_intel_begin_query_clear_cs {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_query_clear_cs) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_query_clear_cs(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_query_clear_cs *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_query_clear_cs(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_query_clear_cs(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_QUERY_CLEAR_CS)))
      return;
   __trace_intel_begin_query_clear_cs(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_query_clear_cs
 */
struct trace_intel_end_query_clear_cs {
   uint32_t count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_query_clear_cs(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_query_clear_cs *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_query_clear_cs(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t count
);
static ALWAYS_INLINE void trace_intel_end_query_clear_cs(
     struct u_trace *ut
   , uint32_t count
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_QUERY_CLEAR_CS)))
      return;
   __trace_intel_end_query_clear_cs(
        ut
      , enabled_traces
      , count
   );
}

/*
 * intel_begin_query_copy_cs
 */
struct trace_intel_begin_query_copy_cs {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_query_copy_cs) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_query_copy_cs(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_query_copy_cs *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_query_copy_cs(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_query_copy_cs(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_QUERY_COPY_CS)))
      return;
   __trace_intel_begin_query_copy_cs(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_query_copy_cs
 */
struct trace_intel_end_query_copy_cs {
   uint32_t count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_query_copy_cs(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_query_copy_cs *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_query_copy_cs(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t count
);
static ALWAYS_INLINE void trace_intel_end_query_copy_cs(
     struct u_trace *ut
   , uint32_t count
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_QUERY_COPY_CS)))
      return;
   __trace_intel_end_query_copy_cs(
        ut
      , enabled_traces
      , count
   );
}

/*
 * intel_begin_query_copy_shader
 */
struct trace_intel_begin_query_copy_shader {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_query_copy_shader) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_query_copy_shader(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_query_copy_shader *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_query_copy_shader(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_query_copy_shader(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_QUERY_COPY_SHADER)))
      return;
   __trace_intel_begin_query_copy_shader(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_query_copy_shader
 */
struct trace_intel_end_query_copy_shader {
   uint32_t count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_query_copy_shader(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_query_copy_shader *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_query_copy_shader(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t count
);
static ALWAYS_INLINE void trace_intel_end_query_copy_shader(
     struct u_trace *ut
   , uint32_t count
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_QUERY_COPY_SHADER)))
      return;
   __trace_intel_end_query_copy_shader(
        ut
      , enabled_traces
      , count
   );
}

/*
 * intel_begin_draw
 */
struct trace_intel_begin_draw {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_draw) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_draw(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_draw *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_draw(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_draw(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW)))
      return;
   __trace_intel_begin_draw(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_draw
 */
struct trace_intel_end_draw {
   uint32_t count;
   uint32_t vs_hash;
   uint32_t fs_hash;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_draw *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_draw(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t count
     , uint32_t vs_hash
     , uint32_t fs_hash
);
static ALWAYS_INLINE void trace_intel_end_draw(
     struct u_trace *ut
   , uint32_t count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW)))
      return;
   __trace_intel_end_draw(
        ut
      , enabled_traces
      , count
      , vs_hash
      , fs_hash
   );
}

/*
 * intel_begin_draw_multi
 */
struct trace_intel_begin_draw_multi {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_draw_multi) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_draw_multi(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_draw_multi *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_draw_multi(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_draw_multi(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_MULTI)))
      return;
   __trace_intel_begin_draw_multi(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_draw_multi
 */
struct trace_intel_end_draw_multi {
   uint32_t count;
   uint32_t vs_hash;
   uint32_t fs_hash;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_multi(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_draw_multi *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_draw_multi(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t count
     , uint32_t vs_hash
     , uint32_t fs_hash
);
static ALWAYS_INLINE void trace_intel_end_draw_multi(
     struct u_trace *ut
   , uint32_t count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_MULTI)))
      return;
   __trace_intel_end_draw_multi(
        ut
      , enabled_traces
      , count
      , vs_hash
      , fs_hash
   );
}

/*
 * intel_begin_draw_indexed
 */
struct trace_intel_begin_draw_indexed {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_draw_indexed) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_draw_indexed(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_draw_indexed *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_draw_indexed(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_draw_indexed(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDEXED)))
      return;
   __trace_intel_begin_draw_indexed(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_draw_indexed
 */
struct trace_intel_end_draw_indexed {
   uint32_t count;
   uint32_t vs_hash;
   uint32_t fs_hash;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indexed(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_draw_indexed *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_draw_indexed(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t count
     , uint32_t vs_hash
     , uint32_t fs_hash
);
static ALWAYS_INLINE void trace_intel_end_draw_indexed(
     struct u_trace *ut
   , uint32_t count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDEXED)))
      return;
   __trace_intel_end_draw_indexed(
        ut
      , enabled_traces
      , count
      , vs_hash
      , fs_hash
   );
}

/*
 * intel_begin_draw_indexed_multi
 */
struct trace_intel_begin_draw_indexed_multi {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_draw_indexed_multi) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_draw_indexed_multi(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_draw_indexed_multi *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_draw_indexed_multi(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_draw_indexed_multi(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDEXED_MULTI)))
      return;
   __trace_intel_begin_draw_indexed_multi(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_draw_indexed_multi
 */
struct trace_intel_end_draw_indexed_multi {
   uint32_t count;
   uint32_t vs_hash;
   uint32_t fs_hash;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indexed_multi(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_draw_indexed_multi *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_draw_indexed_multi(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t count
     , uint32_t vs_hash
     , uint32_t fs_hash
);
static ALWAYS_INLINE void trace_intel_end_draw_indexed_multi(
     struct u_trace *ut
   , uint32_t count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDEXED_MULTI)))
      return;
   __trace_intel_end_draw_indexed_multi(
        ut
      , enabled_traces
      , count
      , vs_hash
      , fs_hash
   );
}

/*
 * intel_begin_draw_indirect_byte_count
 */
struct trace_intel_begin_draw_indirect_byte_count {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_draw_indirect_byte_count) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_draw_indirect_byte_count(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_draw_indirect_byte_count *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_draw_indirect_byte_count(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_draw_indirect_byte_count(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDIRECT_BYTE_COUNT)))
      return;
   __trace_intel_begin_draw_indirect_byte_count(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_draw_indirect_byte_count
 */
struct trace_intel_end_draw_indirect_byte_count {
   uint32_t instance_count;
   uint32_t vs_hash;
   uint32_t fs_hash;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indirect_byte_count(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_draw_indirect_byte_count *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_draw_indirect_byte_count(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t instance_count
     , uint32_t vs_hash
     , uint32_t fs_hash
);
static ALWAYS_INLINE void trace_intel_end_draw_indirect_byte_count(
     struct u_trace *ut
   , uint32_t instance_count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDIRECT_BYTE_COUNT)))
      return;
   __trace_intel_end_draw_indirect_byte_count(
        ut
      , enabled_traces
      , instance_count
      , vs_hash
      , fs_hash
   );
}

/*
 * intel_begin_draw_indirect
 */
struct trace_intel_begin_draw_indirect {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_draw_indirect) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_draw_indirect(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_draw_indirect *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_draw_indirect(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_draw_indirect(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDIRECT)))
      return;
   __trace_intel_begin_draw_indirect(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_draw_indirect
 */
struct trace_intel_end_draw_indirect {
   uint32_t draw_count;
   uint32_t vs_hash;
   uint32_t fs_hash;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indirect(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_draw_indirect *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_draw_indirect(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t draw_count
     , uint32_t vs_hash
     , uint32_t fs_hash
);
static ALWAYS_INLINE void trace_intel_end_draw_indirect(
     struct u_trace *ut
   , uint32_t draw_count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDIRECT)))
      return;
   __trace_intel_end_draw_indirect(
        ut
      , enabled_traces
      , draw_count
      , vs_hash
      , fs_hash
   );
}

/*
 * intel_begin_draw_indexed_indirect
 */
struct trace_intel_begin_draw_indexed_indirect {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_draw_indexed_indirect) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_draw_indexed_indirect(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_draw_indexed_indirect *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_draw_indexed_indirect(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_draw_indexed_indirect(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDEXED_INDIRECT)))
      return;
   __trace_intel_begin_draw_indexed_indirect(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_draw_indexed_indirect
 */
struct trace_intel_end_draw_indexed_indirect {
   uint32_t draw_count;
   uint32_t vs_hash;
   uint32_t fs_hash;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indexed_indirect(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_draw_indexed_indirect *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_draw_indexed_indirect(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t draw_count
     , uint32_t vs_hash
     , uint32_t fs_hash
);
static ALWAYS_INLINE void trace_intel_end_draw_indexed_indirect(
     struct u_trace *ut
   , uint32_t draw_count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDEXED_INDIRECT)))
      return;
   __trace_intel_end_draw_indexed_indirect(
        ut
      , enabled_traces
      , draw_count
      , vs_hash
      , fs_hash
   );
}

/*
 * intel_begin_draw_indirect_count
 */
struct trace_intel_begin_draw_indirect_count {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_draw_indirect_count) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_draw_indirect_count(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_draw_indirect_count *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_draw_indirect_count(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_draw_indirect_count(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDIRECT_COUNT)))
      return;
   __trace_intel_begin_draw_indirect_count(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_draw_indirect_count
 */
struct trace_intel_end_draw_indirect_count {
   uint32_t vs_hash;
   uint32_t fs_hash;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indirect_count(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_draw_indirect_count *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_draw_indirect_count(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , struct u_trace_address draw_count
     , uint32_t vs_hash
     , uint32_t fs_hash
);
static ALWAYS_INLINE void trace_intel_end_draw_indirect_count(
     struct u_trace *ut
   , struct u_trace_address draw_count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDIRECT_COUNT)))
      return;
   __trace_intel_end_draw_indirect_count(
        ut
      , enabled_traces
      , draw_count
      , vs_hash
      , fs_hash
   );
}

/*
 * intel_begin_draw_indexed_indirect_count
 */
struct trace_intel_begin_draw_indexed_indirect_count {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_draw_indexed_indirect_count) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_draw_indexed_indirect_count(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_draw_indexed_indirect_count *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_draw_indexed_indirect_count(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_draw_indexed_indirect_count(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDEXED_INDIRECT_COUNT)))
      return;
   __trace_intel_begin_draw_indexed_indirect_count(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_draw_indexed_indirect_count
 */
struct trace_intel_end_draw_indexed_indirect_count {
   uint32_t vs_hash;
   uint32_t fs_hash;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indexed_indirect_count(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_draw_indexed_indirect_count *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_draw_indexed_indirect_count(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , struct u_trace_address draw_count
     , uint32_t vs_hash
     , uint32_t fs_hash
);
static ALWAYS_INLINE void trace_intel_end_draw_indexed_indirect_count(
     struct u_trace *ut
   , struct u_trace_address draw_count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_INDEXED_INDIRECT_COUNT)))
      return;
   __trace_intel_end_draw_indexed_indirect_count(
        ut
      , enabled_traces
      , draw_count
      , vs_hash
      , fs_hash
   );
}

/*
 * intel_begin_draw_mesh
 */
struct trace_intel_begin_draw_mesh {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_draw_mesh) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_draw_mesh(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_draw_mesh *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_draw_mesh(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_draw_mesh(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_MESH)))
      return;
   __trace_intel_begin_draw_mesh(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_draw_mesh
 */
struct trace_intel_end_draw_mesh {
   uint32_t group_x;
   uint32_t group_y;
   uint32_t group_z;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_mesh(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_draw_mesh *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_draw_mesh(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t group_x
     , uint32_t group_y
     , uint32_t group_z
);
static ALWAYS_INLINE void trace_intel_end_draw_mesh(
     struct u_trace *ut
   , uint32_t group_x
   , uint32_t group_y
   , uint32_t group_z
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_MESH)))
      return;
   __trace_intel_end_draw_mesh(
        ut
      , enabled_traces
      , group_x
      , group_y
      , group_z
   );
}

/*
 * intel_begin_draw_mesh_indirect
 */
struct trace_intel_begin_draw_mesh_indirect {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_draw_mesh_indirect) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_draw_mesh_indirect(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_draw_mesh_indirect *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_draw_mesh_indirect(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_draw_mesh_indirect(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_MESH_INDIRECT)))
      return;
   __trace_intel_begin_draw_mesh_indirect(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_draw_mesh_indirect
 */
struct trace_intel_end_draw_mesh_indirect {
   uint32_t draw_count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_mesh_indirect(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_draw_mesh_indirect *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_draw_mesh_indirect(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t draw_count
);
static ALWAYS_INLINE void trace_intel_end_draw_mesh_indirect(
     struct u_trace *ut
   , uint32_t draw_count
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_MESH_INDIRECT)))
      return;
   __trace_intel_end_draw_mesh_indirect(
        ut
      , enabled_traces
      , draw_count
   );
}

/*
 * intel_begin_draw_mesh_indirect_count
 */
struct trace_intel_begin_draw_mesh_indirect_count {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_draw_mesh_indirect_count) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_draw_mesh_indirect_count(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_draw_mesh_indirect_count *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_draw_mesh_indirect_count(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_draw_mesh_indirect_count(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_MESH_INDIRECT_COUNT)))
      return;
   __trace_intel_begin_draw_mesh_indirect_count(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_draw_mesh_indirect_count
 */
struct trace_intel_end_draw_mesh_indirect_count {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_end_draw_mesh_indirect_count) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_mesh_indirect_count(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_draw_mesh_indirect_count *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_draw_mesh_indirect_count(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , struct u_trace_address draw_count
);
static ALWAYS_INLINE void trace_intel_end_draw_mesh_indirect_count(
     struct u_trace *ut
   , struct u_trace_address draw_count
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_DRAW_MESH_INDIRECT_COUNT)))
      return;
   __trace_intel_end_draw_mesh_indirect_count(
        ut
      , enabled_traces
      , draw_count
   );
}

/*
 * intel_begin_compute
 */
struct trace_intel_begin_compute {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_compute) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_compute(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_compute *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_compute(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_compute(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_COMPUTE)))
      return;
   __trace_intel_begin_compute(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_compute
 */
struct trace_intel_end_compute {
   uint32_t group_x;
   uint32_t group_y;
   uint32_t group_z;
   uint32_t cs_hash;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_compute(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_compute *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_compute(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t group_x
     , uint32_t group_y
     , uint32_t group_z
     , uint32_t cs_hash
);
static ALWAYS_INLINE void trace_intel_end_compute(
     struct u_trace *ut
   , uint32_t group_x
   , uint32_t group_y
   , uint32_t group_z
   , uint32_t cs_hash
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_COMPUTE)))
      return;
   __trace_intel_end_compute(
        ut
      , enabled_traces
      , group_x
      , group_y
      , group_z
      , cs_hash
   );
}

/*
 * intel_begin_compute_indirect
 */
struct trace_intel_begin_compute_indirect {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_compute_indirect) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_compute_indirect(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_compute_indirect *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_compute_indirect(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_compute_indirect(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_COMPUTE_INDIRECT)))
      return;
   __trace_intel_begin_compute_indirect(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_compute_indirect
 */
struct trace_intel_end_compute_indirect {
   uint32_t cs_hash;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_compute_indirect(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_compute_indirect *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_compute_indirect(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , struct u_trace_address size
     , uint32_t cs_hash
);
static ALWAYS_INLINE void trace_intel_end_compute_indirect(
     struct u_trace *ut
   , struct u_trace_address size
   , uint32_t cs_hash
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_COMPUTE_INDIRECT)))
      return;
   __trace_intel_end_compute_indirect(
        ut
      , enabled_traces
      , size
      , cs_hash
   );
}

/*
 * intel_begin_trace_copy
 */
struct trace_intel_begin_trace_copy {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_trace_copy) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_trace_copy(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_trace_copy *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_trace_copy(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_trace_copy(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_TRACE_COPY)))
      return;
   __trace_intel_begin_trace_copy(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_trace_copy
 */
struct trace_intel_end_trace_copy {
   uint32_t count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_trace_copy(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_trace_copy *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_trace_copy(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t count
);
static ALWAYS_INLINE void trace_intel_end_trace_copy(
     struct u_trace *ut
   , uint32_t count
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_TRACE_COPY)))
      return;
   __trace_intel_end_trace_copy(
        ut
      , enabled_traces
      , count
   );
}

/*
 * intel_begin_trace_copy_cb
 */
struct trace_intel_begin_trace_copy_cb {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_trace_copy_cb) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_trace_copy_cb(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_trace_copy_cb *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_trace_copy_cb(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , void *cs
);
static ALWAYS_INLINE void trace_intel_begin_trace_copy_cb(
     struct u_trace *ut
   , void *cs
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_TRACE_COPY_CB)))
      return;
   __trace_intel_begin_trace_copy_cb(
        ut
      , enabled_traces
      , cs
   );
}

/*
 * intel_end_trace_copy_cb
 */
struct trace_intel_end_trace_copy_cb {
   uint32_t count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_trace_copy_cb(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_trace_copy_cb *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_trace_copy_cb(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , void *cs
     , uint32_t count
);
static ALWAYS_INLINE void trace_intel_end_trace_copy_cb(
     struct u_trace *ut
   , void *cs
   , uint32_t count
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_TRACE_COPY_CB)))
      return;
   __trace_intel_end_trace_copy_cb(
        ut
      , enabled_traces
      , cs
      , count
   );
}

/*
 * intel_begin_as_build
 */
struct trace_intel_begin_as_build {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_as_build) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_as_build(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_as_build *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_as_build(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_as_build(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_BUILD)))
      return;
   __trace_intel_begin_as_build(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_as_build
 */
struct trace_intel_end_as_build {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_end_as_build) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_end_as_build(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_as_build *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_as_build(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_end_as_build(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_BUILD)))
      return;
   __trace_intel_end_as_build(
        ut
      , enabled_traces
   );
}

/*
 * intel_begin_as_build_leaves
 */
struct trace_intel_begin_as_build_leaves {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_as_build_leaves) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_as_build_leaves(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_as_build_leaves *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_as_build_leaves(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_as_build_leaves(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_BUILD_LEAVES)))
      return;
   __trace_intel_begin_as_build_leaves(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_as_build_leaves
 */
struct trace_intel_end_as_build_leaves {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_end_as_build_leaves) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_end_as_build_leaves(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_as_build_leaves *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_as_build_leaves(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_end_as_build_leaves(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_BUILD_LEAVES)))
      return;
   __trace_intel_end_as_build_leaves(
        ut
      , enabled_traces
   );
}

/*
 * intel_begin_as_morton_generate
 */
struct trace_intel_begin_as_morton_generate {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_as_morton_generate) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_as_morton_generate(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_as_morton_generate *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_as_morton_generate(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_as_morton_generate(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_MORTON_GENERATE)))
      return;
   __trace_intel_begin_as_morton_generate(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_as_morton_generate
 */
struct trace_intel_end_as_morton_generate {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_end_as_morton_generate) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_end_as_morton_generate(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_as_morton_generate *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_as_morton_generate(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_end_as_morton_generate(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_MORTON_GENERATE)))
      return;
   __trace_intel_end_as_morton_generate(
        ut
      , enabled_traces
   );
}

/*
 * intel_begin_as_morton_sort
 */
struct trace_intel_begin_as_morton_sort {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_as_morton_sort) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_as_morton_sort(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_as_morton_sort *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_as_morton_sort(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_as_morton_sort(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_MORTON_SORT)))
      return;
   __trace_intel_begin_as_morton_sort(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_as_morton_sort
 */
struct trace_intel_end_as_morton_sort {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_end_as_morton_sort) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_end_as_morton_sort(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_as_morton_sort *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_as_morton_sort(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_end_as_morton_sort(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_MORTON_SORT)))
      return;
   __trace_intel_end_as_morton_sort(
        ut
      , enabled_traces
   );
}

/*
 * intel_begin_as_lbvh_build_internal
 */
struct trace_intel_begin_as_lbvh_build_internal {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_as_lbvh_build_internal) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_as_lbvh_build_internal(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_as_lbvh_build_internal *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_as_lbvh_build_internal(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_as_lbvh_build_internal(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_LBVH_BUILD_INTERNAL)))
      return;
   __trace_intel_begin_as_lbvh_build_internal(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_as_lbvh_build_internal
 */
struct trace_intel_end_as_lbvh_build_internal {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_end_as_lbvh_build_internal) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_end_as_lbvh_build_internal(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_as_lbvh_build_internal *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_as_lbvh_build_internal(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_end_as_lbvh_build_internal(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_LBVH_BUILD_INTERNAL)))
      return;
   __trace_intel_end_as_lbvh_build_internal(
        ut
      , enabled_traces
   );
}

/*
 * intel_begin_as_ploc_build_internal
 */
struct trace_intel_begin_as_ploc_build_internal {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_as_ploc_build_internal) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_as_ploc_build_internal(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_as_ploc_build_internal *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_as_ploc_build_internal(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_as_ploc_build_internal(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_PLOC_BUILD_INTERNAL)))
      return;
   __trace_intel_begin_as_ploc_build_internal(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_as_ploc_build_internal
 */
struct trace_intel_end_as_ploc_build_internal {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_end_as_ploc_build_internal) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_end_as_ploc_build_internal(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_as_ploc_build_internal *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_as_ploc_build_internal(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_end_as_ploc_build_internal(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_PLOC_BUILD_INTERNAL)))
      return;
   __trace_intel_end_as_ploc_build_internal(
        ut
      , enabled_traces
   );
}

/*
 * intel_begin_as_encode
 */
struct trace_intel_begin_as_encode {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_as_encode) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_as_encode(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_as_encode *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_as_encode(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_as_encode(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_ENCODE)))
      return;
   __trace_intel_begin_as_encode(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_as_encode
 */
struct trace_intel_end_as_encode {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_end_as_encode) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_end_as_encode(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_as_encode *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_as_encode(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_end_as_encode(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_ENCODE)))
      return;
   __trace_intel_end_as_encode(
        ut
      , enabled_traces
   );
}

/*
 * intel_begin_as_copy
 */
struct trace_intel_begin_as_copy {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_as_copy) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_as_copy(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_as_copy *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_as_copy(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_as_copy(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_COPY)))
      return;
   __trace_intel_begin_as_copy(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_as_copy
 */
struct trace_intel_end_as_copy {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_end_as_copy) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_end_as_copy(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_as_copy *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_as_copy(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_end_as_copy(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_AS_COPY)))
      return;
   __trace_intel_end_as_copy(
        ut
      , enabled_traces
   );
}

/*
 * intel_begin_rays
 */
struct trace_intel_begin_rays {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_rays) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_rays(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_rays *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_rays(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_rays(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_RAYS)))
      return;
   __trace_intel_begin_rays(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_rays
 */
struct trace_intel_end_rays {
   uint32_t group_x;
   uint32_t group_y;
   uint32_t group_z;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_rays(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_rays *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_rays(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t group_x
     , uint32_t group_y
     , uint32_t group_z
);
static ALWAYS_INLINE void trace_intel_end_rays(
     struct u_trace *ut
   , uint32_t group_x
   , uint32_t group_y
   , uint32_t group_z
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_RAYS)))
      return;
   __trace_intel_end_rays(
        ut
      , enabled_traces
      , group_x
      , group_y
      , group_z
   );
}

/*
 * intel_begin_stall
 */
struct trace_intel_begin_stall {
#ifdef __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_stall) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_stall(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_begin_stall *payload,
   const void *indirect_data);
#endif
void __trace_intel_begin_stall(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
);
static ALWAYS_INLINE void trace_intel_begin_stall(
     struct u_trace *ut
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_STALL)))
      return;
   __trace_intel_begin_stall(
        ut
      , enabled_traces
   );
}

/*
 * intel_end_stall
 */
struct trace_intel_end_stall {
   uint32_t flags;
   const char * reason1;
   const char * reason2;
   const char * reason3;
   const char * reason4;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_stall(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   uint16_t tp_idx,
   const void *flush_data,
   const struct trace_intel_end_stall *payload,
   const void *indirect_data);
#endif
void __trace_intel_end_stall(
       struct u_trace *ut
     , enum u_trace_type enabled_traces
     , uint32_t flags
     , intel_ds_stall_cb_t decode_cb
     , const char * reason1
     , const char * reason2
     , const char * reason3
     , const char * reason4
);
static ALWAYS_INLINE void trace_intel_end_stall(
     struct u_trace *ut
   , uint32_t flags
   , intel_ds_stall_cb_t decode_cb
   , const char * reason1
   , const char * reason2
   , const char * reason3
   , const char * reason4
) {
   enum u_trace_type enabled_traces = p_atomic_read_relaxed(&ut->utctx->enabled_traces);
   if (!unlikely(enabled_traces != 0 &&
                 (intel_gpu_tracepoint & INTEL_GPU_TRACEPOINT_STALL)))
      return;
   __trace_intel_end_stall(
        ut
      , enabled_traces
      , flags
      , decode_cb
      , reason1
      , reason2
      , reason3
      , reason4
   );
}

#ifdef __cplusplus
}
#endif

#endif /* _INTEL_TRACEPOINTS_H */
