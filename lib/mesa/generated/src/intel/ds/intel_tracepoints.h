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



/*
 * intel_begin_batch
 */
struct trace_intel_begin_batch {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_batch *payload);
#endif
void __trace_intel_begin_batch(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_batch(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_batch(
        ut, cs
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
   const void *flush_data,
   const struct trace_intel_end_batch *payload);
#endif
void __trace_intel_end_batch(
       struct u_trace *ut, void *cs
     , uint8_t name
);
static inline void trace_intel_end_batch(
     struct u_trace *ut, void *cs
   , uint8_t name
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_batch(
        ut, cs
      , name
   );
}

/*
 * intel_begin_cmd_buffer
 */
struct trace_intel_begin_cmd_buffer {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_cmd_buffer *payload);
#endif
void __trace_intel_begin_cmd_buffer(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_cmd_buffer(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_cmd_buffer(
        ut, cs
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
   const void *flush_data,
   const struct trace_intel_end_cmd_buffer *payload);
#endif
void __trace_intel_end_cmd_buffer(
       struct u_trace *ut, void *cs
     , uint8_t level
);
static inline void trace_intel_end_cmd_buffer(
     struct u_trace *ut, void *cs
   , uint8_t level
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_cmd_buffer(
        ut, cs
      , level
   );
}

/*
 * intel_begin_render_pass
 */
struct trace_intel_begin_render_pass {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_render_pass *payload);
#endif
void __trace_intel_begin_render_pass(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_render_pass(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_render_pass(
        ut, cs
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
   uint32_t subpass_count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_render_pass(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_render_pass *payload);
#endif
void __trace_intel_end_render_pass(
       struct u_trace *ut, void *cs
     , uint16_t width
     , uint16_t height
     , uint8_t att_count
     , uint8_t msaa
     , uint32_t subpass_count
);
static inline void trace_intel_end_render_pass(
     struct u_trace *ut, void *cs
   , uint16_t width
   , uint16_t height
   , uint8_t att_count
   , uint8_t msaa
   , uint32_t subpass_count
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_render_pass(
        ut, cs
      , width
      , height
      , att_count
      , msaa
      , subpass_count
   );
}

/*
 * intel_begin_dyn_render_pass
 */
struct trace_intel_begin_dyn_render_pass {
#ifdef  __cplusplus
   /* avoid warnings about empty struct size mis-match in C vs C++..
    * the size mis-match is harmless because (a) nothing will deref
    * the empty struct, and (b) the code that cares about allocating
    * sizeof(struct trace_intel_begin_dyn_render_pass) (and wants this to be zero
    * if there is no payload) is C
    */
   uint8_t dummy;
#endif
};
#ifdef HAVE_PERFETTO
void intel_ds_begin_dyn_render_pass(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_begin_dyn_render_pass *payload);
#endif
void __trace_intel_begin_dyn_render_pass(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_dyn_render_pass(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_dyn_render_pass(
        ut, cs
   );
}

/*
 * intel_end_dyn_render_pass
 */
struct trace_intel_end_dyn_render_pass {
   uint16_t width;
   uint16_t height;
   uint8_t att_count;
   uint8_t msaa;
   uint8_t suspend;
   uint8_t resume;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_dyn_render_pass(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_dyn_render_pass *payload);
#endif
void __trace_intel_end_dyn_render_pass(
       struct u_trace *ut, void *cs
     , uint16_t width
     , uint16_t height
     , uint8_t att_count
     , uint8_t msaa
     , uint8_t suspend
     , uint8_t resume
);
static inline void trace_intel_end_dyn_render_pass(
     struct u_trace *ut, void *cs
   , uint16_t width
   , uint16_t height
   , uint8_t att_count
   , uint8_t msaa
   , uint8_t suspend
   , uint8_t resume
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_dyn_render_pass(
        ut, cs
      , width
      , height
      , att_count
      , msaa
      , suspend
      , resume
   );
}

/*
 * intel_begin_blorp
 */
struct trace_intel_begin_blorp {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_blorp *payload);
#endif
void __trace_intel_begin_blorp(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_blorp(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_blorp(
        ut, cs
   );
}

/*
 * intel_end_blorp
 */
struct trace_intel_end_blorp {
   uint32_t width;
   uint32_t height;
   enum isl_aux_op hiz_op;
   enum isl_aux_op fast_clear_op;
   enum blorp_shader_type blorp_type;
   enum blorp_shader_pipeline blorp_pipe;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_blorp(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_blorp *payload);
#endif
void __trace_intel_end_blorp(
       struct u_trace *ut, void *cs
     , uint32_t width
     , uint32_t height
     , enum isl_aux_op hiz_op
     , enum isl_aux_op fast_clear_op
     , enum blorp_shader_type shader_type
     , enum blorp_shader_pipeline shader_pipe
);
static inline void trace_intel_end_blorp(
     struct u_trace *ut, void *cs
   , uint32_t width
   , uint32_t height
   , enum isl_aux_op hiz_op
   , enum isl_aux_op fast_clear_op
   , enum blorp_shader_type shader_type
   , enum blorp_shader_pipeline shader_pipe
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_blorp(
        ut, cs
      , width
      , height
      , hiz_op
      , fast_clear_op
      , shader_type
      , shader_pipe
   );
}

/*
 * intel_begin_draw
 */
struct trace_intel_begin_draw {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_draw *payload);
#endif
void __trace_intel_begin_draw(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_draw(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_draw(
        ut, cs
   );
}

/*
 * intel_end_draw
 */
struct trace_intel_end_draw {
   uint32_t count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_draw *payload);
#endif
void __trace_intel_end_draw(
       struct u_trace *ut, void *cs
     , uint32_t count
);
static inline void trace_intel_end_draw(
     struct u_trace *ut, void *cs
   , uint32_t count
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_draw(
        ut, cs
      , count
   );
}

/*
 * intel_begin_draw_multi
 */
struct trace_intel_begin_draw_multi {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_draw_multi *payload);
#endif
void __trace_intel_begin_draw_multi(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_draw_multi(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_draw_multi(
        ut, cs
   );
}

/*
 * intel_end_draw_multi
 */
struct trace_intel_end_draw_multi {
   uint32_t count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_multi(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_draw_multi *payload);
#endif
void __trace_intel_end_draw_multi(
       struct u_trace *ut, void *cs
     , uint32_t count
);
static inline void trace_intel_end_draw_multi(
     struct u_trace *ut, void *cs
   , uint32_t count
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_draw_multi(
        ut, cs
      , count
   );
}

/*
 * intel_begin_draw_indexed
 */
struct trace_intel_begin_draw_indexed {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_draw_indexed *payload);
#endif
void __trace_intel_begin_draw_indexed(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_draw_indexed(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_draw_indexed(
        ut, cs
   );
}

/*
 * intel_end_draw_indexed
 */
struct trace_intel_end_draw_indexed {
   uint32_t count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indexed(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_draw_indexed *payload);
#endif
void __trace_intel_end_draw_indexed(
       struct u_trace *ut, void *cs
     , uint32_t count
);
static inline void trace_intel_end_draw_indexed(
     struct u_trace *ut, void *cs
   , uint32_t count
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_draw_indexed(
        ut, cs
      , count
   );
}

/*
 * intel_begin_draw_indexed_multi
 */
struct trace_intel_begin_draw_indexed_multi {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_draw_indexed_multi *payload);
#endif
void __trace_intel_begin_draw_indexed_multi(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_draw_indexed_multi(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_draw_indexed_multi(
        ut, cs
   );
}

/*
 * intel_end_draw_indexed_multi
 */
struct trace_intel_end_draw_indexed_multi {
   uint32_t count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indexed_multi(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_draw_indexed_multi *payload);
#endif
void __trace_intel_end_draw_indexed_multi(
       struct u_trace *ut, void *cs
     , uint32_t count
);
static inline void trace_intel_end_draw_indexed_multi(
     struct u_trace *ut, void *cs
   , uint32_t count
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_draw_indexed_multi(
        ut, cs
      , count
   );
}

/*
 * intel_begin_draw_indirect_byte_count
 */
struct trace_intel_begin_draw_indirect_byte_count {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_draw_indirect_byte_count *payload);
#endif
void __trace_intel_begin_draw_indirect_byte_count(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_draw_indirect_byte_count(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_draw_indirect_byte_count(
        ut, cs
   );
}

/*
 * intel_end_draw_indirect_byte_count
 */
struct trace_intel_end_draw_indirect_byte_count {
   uint32_t instance_count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indirect_byte_count(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_draw_indirect_byte_count *payload);
#endif
void __trace_intel_end_draw_indirect_byte_count(
       struct u_trace *ut, void *cs
     , uint32_t instance_count
);
static inline void trace_intel_end_draw_indirect_byte_count(
     struct u_trace *ut, void *cs
   , uint32_t instance_count
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_draw_indirect_byte_count(
        ut, cs
      , instance_count
   );
}

/*
 * intel_begin_draw_indirect
 */
struct trace_intel_begin_draw_indirect {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_draw_indirect *payload);
#endif
void __trace_intel_begin_draw_indirect(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_draw_indirect(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_draw_indirect(
        ut, cs
   );
}

/*
 * intel_end_draw_indirect
 */
struct trace_intel_end_draw_indirect {
   uint32_t draw_count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indirect(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_draw_indirect *payload);
#endif
void __trace_intel_end_draw_indirect(
       struct u_trace *ut, void *cs
     , uint32_t draw_count
);
static inline void trace_intel_end_draw_indirect(
     struct u_trace *ut, void *cs
   , uint32_t draw_count
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_draw_indirect(
        ut, cs
      , draw_count
   );
}

/*
 * intel_begin_draw_indexed_indirect
 */
struct trace_intel_begin_draw_indexed_indirect {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_draw_indexed_indirect *payload);
#endif
void __trace_intel_begin_draw_indexed_indirect(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_draw_indexed_indirect(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_draw_indexed_indirect(
        ut, cs
   );
}

/*
 * intel_end_draw_indexed_indirect
 */
struct trace_intel_end_draw_indexed_indirect {
   uint32_t draw_count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indexed_indirect(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_draw_indexed_indirect *payload);
#endif
void __trace_intel_end_draw_indexed_indirect(
       struct u_trace *ut, void *cs
     , uint32_t draw_count
);
static inline void trace_intel_end_draw_indexed_indirect(
     struct u_trace *ut, void *cs
   , uint32_t draw_count
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_draw_indexed_indirect(
        ut, cs
      , draw_count
   );
}

/*
 * intel_begin_draw_indirect_count
 */
struct trace_intel_begin_draw_indirect_count {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_draw_indirect_count *payload);
#endif
void __trace_intel_begin_draw_indirect_count(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_draw_indirect_count(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_draw_indirect_count(
        ut, cs
   );
}

/*
 * intel_end_draw_indirect_count
 */
struct trace_intel_end_draw_indirect_count {
   uint32_t max_draw_count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indirect_count(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_draw_indirect_count *payload);
#endif
void __trace_intel_end_draw_indirect_count(
       struct u_trace *ut, void *cs
     , uint32_t max_draw_count
);
static inline void trace_intel_end_draw_indirect_count(
     struct u_trace *ut, void *cs
   , uint32_t max_draw_count
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_draw_indirect_count(
        ut, cs
      , max_draw_count
   );
}

/*
 * intel_begin_draw_indexed_indirect_count
 */
struct trace_intel_begin_draw_indexed_indirect_count {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_draw_indexed_indirect_count *payload);
#endif
void __trace_intel_begin_draw_indexed_indirect_count(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_draw_indexed_indirect_count(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_draw_indexed_indirect_count(
        ut, cs
   );
}

/*
 * intel_end_draw_indexed_indirect_count
 */
struct trace_intel_end_draw_indexed_indirect_count {
   uint32_t max_draw_count;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_draw_indexed_indirect_count(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_draw_indexed_indirect_count *payload);
#endif
void __trace_intel_end_draw_indexed_indirect_count(
       struct u_trace *ut, void *cs
     , uint32_t max_draw_count
);
static inline void trace_intel_end_draw_indexed_indirect_count(
     struct u_trace *ut, void *cs
   , uint32_t max_draw_count
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_draw_indexed_indirect_count(
        ut, cs
      , max_draw_count
   );
}

/*
 * intel_begin_compute
 */
struct trace_intel_begin_compute {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_compute *payload);
#endif
void __trace_intel_begin_compute(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_compute(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_compute(
        ut, cs
   );
}

/*
 * intel_end_compute
 */
struct trace_intel_end_compute {
   uint32_t group_x;
   uint32_t group_y;
   uint32_t group_z;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_compute(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_compute *payload);
#endif
void __trace_intel_end_compute(
       struct u_trace *ut, void *cs
     , uint32_t group_x
     , uint32_t group_y
     , uint32_t group_z
);
static inline void trace_intel_end_compute(
     struct u_trace *ut, void *cs
   , uint32_t group_x
   , uint32_t group_y
   , uint32_t group_z
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_compute(
        ut, cs
      , group_x
      , group_y
      , group_z
   );
}

/*
 * intel_begin_stall
 */
struct trace_intel_begin_stall {
#ifdef  __cplusplus
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
   const void *flush_data,
   const struct trace_intel_begin_stall *payload);
#endif
void __trace_intel_begin_stall(
       struct u_trace *ut, void *cs
);
static inline void trace_intel_begin_stall(
     struct u_trace *ut, void *cs
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_begin_stall(
        ut, cs
   );
}

/*
 * intel_end_stall
 */
struct trace_intel_end_stall {
   uint32_t flags;
   const char * reason;
};
#ifdef HAVE_PERFETTO
void intel_ds_end_stall(
   struct intel_ds_device *dev,
   uint64_t ts_ns,
   const void *flush_data,
   const struct trace_intel_end_stall *payload);
#endif
void __trace_intel_end_stall(
       struct u_trace *ut, void *cs
     , uint32_t flags
     , intel_ds_stall_cb_t decode_cb
     , const char * reason
);
static inline void trace_intel_end_stall(
     struct u_trace *ut, void *cs
   , uint32_t flags
   , intel_ds_stall_cb_t decode_cb
   , const char * reason
) {
   if (!unlikely(ut->enabled || ut_trace_instrument || ut_perfetto_enabled))
      return;
   __trace_intel_end_stall(
        ut, cs
      , flags
      , decode_cb
      , reason
   );
}

#ifdef __cplusplus
}
#endif

#endif /* _INTEL_TRACEPOINTS_H */
