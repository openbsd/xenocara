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

#include "intel_tracepoints.h"

#include "intel_driver_ds.h"
#include "vulkan/vulkan_core.h"

#define __NEEDS_TRACE_PRIV
#include "util/u_debug.h"
#include "util/perf/u_trace_priv.h"

static const struct debug_control config_control[] = {
   { "frame", INTEL_GPU_TRACEPOINT_FRAME, },
   { "queue_annotation", INTEL_GPU_TRACEPOINT_QUEUE_ANNOTATION, },
   { "batch", INTEL_GPU_TRACEPOINT_BATCH, },
   { "cmd_buffer", INTEL_GPU_TRACEPOINT_CMD_BUFFER, },
   { "cmd_buffer_annotation", INTEL_GPU_TRACEPOINT_CMD_BUFFER_ANNOTATION, },
   { "xfb", INTEL_GPU_TRACEPOINT_XFB, },
   { "render_pass", INTEL_GPU_TRACEPOINT_RENDER_PASS, },
   { "blorp", INTEL_GPU_TRACEPOINT_BLORP, },
   { "write_buffer_marker", INTEL_GPU_TRACEPOINT_WRITE_BUFFER_MARKER, },
   { "generate_draws", INTEL_GPU_TRACEPOINT_GENERATE_DRAWS, },
   { "generate_commands", INTEL_GPU_TRACEPOINT_GENERATE_COMMANDS, },
   { "query_clear_blorp", INTEL_GPU_TRACEPOINT_QUERY_CLEAR_BLORP, },
   { "query_clear_cs", INTEL_GPU_TRACEPOINT_QUERY_CLEAR_CS, },
   { "query_copy_cs", INTEL_GPU_TRACEPOINT_QUERY_COPY_CS, },
   { "query_copy_shader", INTEL_GPU_TRACEPOINT_QUERY_COPY_SHADER, },
   { "draw", INTEL_GPU_TRACEPOINT_DRAW, },
   { "draw_multi", INTEL_GPU_TRACEPOINT_DRAW_MULTI, },
   { "draw_indexed", INTEL_GPU_TRACEPOINT_DRAW_INDEXED, },
   { "draw_indexed_multi", INTEL_GPU_TRACEPOINT_DRAW_INDEXED_MULTI, },
   { "draw_indirect_byte_count", INTEL_GPU_TRACEPOINT_DRAW_INDIRECT_BYTE_COUNT, },
   { "draw_indirect", INTEL_GPU_TRACEPOINT_DRAW_INDIRECT, },
   { "draw_indexed_indirect", INTEL_GPU_TRACEPOINT_DRAW_INDEXED_INDIRECT, },
   { "draw_indirect_count", INTEL_GPU_TRACEPOINT_DRAW_INDIRECT_COUNT, },
   { "draw_indexed_indirect_count", INTEL_GPU_TRACEPOINT_DRAW_INDEXED_INDIRECT_COUNT, },
   { "draw_mesh", INTEL_GPU_TRACEPOINT_DRAW_MESH, },
   { "draw_mesh_indirect", INTEL_GPU_TRACEPOINT_DRAW_MESH_INDIRECT, },
   { "draw_mesh_indirect_count", INTEL_GPU_TRACEPOINT_DRAW_MESH_INDIRECT_COUNT, },
   { "compute", INTEL_GPU_TRACEPOINT_COMPUTE, },
   { "compute_indirect", INTEL_GPU_TRACEPOINT_COMPUTE_INDIRECT, },
   { "trace_copy", INTEL_GPU_TRACEPOINT_TRACE_COPY, },
   { "trace_copy_cb", INTEL_GPU_TRACEPOINT_TRACE_COPY_CB, },
   { "as_build", INTEL_GPU_TRACEPOINT_AS_BUILD, },
   { "as_build_leaves", INTEL_GPU_TRACEPOINT_AS_BUILD_LEAVES, },
   { "as_morton_generate", INTEL_GPU_TRACEPOINT_AS_MORTON_GENERATE, },
   { "as_morton_sort", INTEL_GPU_TRACEPOINT_AS_MORTON_SORT, },
   { "as_lbvh_build_internal", INTEL_GPU_TRACEPOINT_AS_LBVH_BUILD_INTERNAL, },
   { "as_ploc_build_internal", INTEL_GPU_TRACEPOINT_AS_PLOC_BUILD_INTERNAL, },
   { "as_encode", INTEL_GPU_TRACEPOINT_AS_ENCODE, },
   { "as_copy", INTEL_GPU_TRACEPOINT_AS_COPY, },
   { "rays", INTEL_GPU_TRACEPOINT_RAYS, },
   { "stall", INTEL_GPU_TRACEPOINT_STALL, },
   { NULL, 0, },
};
uint64_t intel_gpu_tracepoint = 0;

static void
intel_gpu_tracepoint_variable_once(void)
{
   uint64_t default_value = 0
     | INTEL_GPU_TRACEPOINT_FRAME
     | INTEL_GPU_TRACEPOINT_QUEUE_ANNOTATION
     | INTEL_GPU_TRACEPOINT_BATCH
     | INTEL_GPU_TRACEPOINT_CMD_BUFFER
     | INTEL_GPU_TRACEPOINT_CMD_BUFFER_ANNOTATION
     | INTEL_GPU_TRACEPOINT_XFB
     | INTEL_GPU_TRACEPOINT_RENDER_PASS
     | INTEL_GPU_TRACEPOINT_BLORP
     | INTEL_GPU_TRACEPOINT_WRITE_BUFFER_MARKER
     | INTEL_GPU_TRACEPOINT_GENERATE_DRAWS
     | INTEL_GPU_TRACEPOINT_GENERATE_COMMANDS
     | INTEL_GPU_TRACEPOINT_QUERY_CLEAR_BLORP
     | INTEL_GPU_TRACEPOINT_QUERY_CLEAR_CS
     | INTEL_GPU_TRACEPOINT_QUERY_COPY_CS
     | INTEL_GPU_TRACEPOINT_QUERY_COPY_SHADER
     | INTEL_GPU_TRACEPOINT_DRAW
     | INTEL_GPU_TRACEPOINT_DRAW_MULTI
     | INTEL_GPU_TRACEPOINT_DRAW_INDEXED
     | INTEL_GPU_TRACEPOINT_DRAW_INDEXED_MULTI
     | INTEL_GPU_TRACEPOINT_DRAW_INDIRECT_BYTE_COUNT
     | INTEL_GPU_TRACEPOINT_DRAW_INDIRECT
     | INTEL_GPU_TRACEPOINT_DRAW_INDEXED_INDIRECT
     | INTEL_GPU_TRACEPOINT_DRAW_INDIRECT_COUNT
     | INTEL_GPU_TRACEPOINT_DRAW_INDEXED_INDIRECT_COUNT
     | INTEL_GPU_TRACEPOINT_DRAW_MESH
     | INTEL_GPU_TRACEPOINT_DRAW_MESH_INDIRECT
     | INTEL_GPU_TRACEPOINT_DRAW_MESH_INDIRECT_COUNT
     | INTEL_GPU_TRACEPOINT_COMPUTE
     | INTEL_GPU_TRACEPOINT_COMPUTE_INDIRECT
     | INTEL_GPU_TRACEPOINT_TRACE_COPY
     | INTEL_GPU_TRACEPOINT_TRACE_COPY_CB
     | INTEL_GPU_TRACEPOINT_AS_BUILD
     | INTEL_GPU_TRACEPOINT_AS_BUILD_LEAVES
     | INTEL_GPU_TRACEPOINT_AS_MORTON_GENERATE
     | INTEL_GPU_TRACEPOINT_AS_MORTON_SORT
     | INTEL_GPU_TRACEPOINT_AS_LBVH_BUILD_INTERNAL
     | INTEL_GPU_TRACEPOINT_AS_PLOC_BUILD_INTERNAL
     | INTEL_GPU_TRACEPOINT_AS_ENCODE
     | INTEL_GPU_TRACEPOINT_AS_COPY
     | INTEL_GPU_TRACEPOINT_RAYS
     ;

   intel_gpu_tracepoint =
      parse_enable_string(getenv("INTEL_GPU_TRACEPOINT"),
                          default_value,
                          config_control);
}

void
intel_gpu_tracepoint_config_variable(void)
{
   static once_flag process_intel_gpu_tracepoint_variable_flag = ONCE_FLAG_INIT;

   call_once(&process_intel_gpu_tracepoint_variable_flag,
             intel_gpu_tracepoint_variable_once);
}

/*
 * intel_begin_frame
 */
#define __print_intel_begin_frame NULL
#define __print_json_intel_begin_frame NULL
static const struct u_tracepoint __tp_intel_begin_frame = {
    "intel_begin_frame",
    ALIGN_POT(sizeof(struct trace_intel_begin_frame), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    0,
    __print_intel_begin_frame,
    __print_json_intel_begin_frame,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_frame,
#endif
};
void __trace_intel_begin_frame(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , void *cs
) {
   struct trace_intel_begin_frame entry;
   UNUSED struct trace_intel_begin_frame *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_frame *)u_trace_appendv(ut, cs, &__tp_intel_begin_frame,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_frame
 */
static void __print_intel_end_frame(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_frame *__entry =
      (const struct trace_intel_end_frame *)arg;
   fprintf(out, ""
      "frame=%u, "
         "\n"
   ,__entry->frame
   );
}

static void __print_json_intel_end_frame(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_frame *__entry =
      (const struct trace_intel_end_frame *)arg;
   fprintf(out, ""
      "\"frame\": \"%u\""
   ,__entry->frame
   );
}

static const struct u_tracepoint __tp_intel_end_frame = {
    "intel_end_frame",
    ALIGN_POT(sizeof(struct trace_intel_end_frame), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    1,
    __print_intel_end_frame,
    __print_json_intel_end_frame,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_frame,
#endif
};
void __trace_intel_end_frame(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , void *cs
   , uint32_t frame
) {
   struct trace_intel_end_frame entry;
   UNUSED struct trace_intel_end_frame *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_frame *)u_trace_appendv(ut, cs, &__tp_intel_end_frame,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->frame = frame;
}

/*
 * intel_begin_queue_annotation
 */
#define __print_intel_begin_queue_annotation NULL
#define __print_json_intel_begin_queue_annotation NULL
static const struct u_tracepoint __tp_intel_begin_queue_annotation = {
    "intel_begin_queue_annotation",
    ALIGN_POT(sizeof(struct trace_intel_begin_queue_annotation), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    2,
    __print_intel_begin_queue_annotation,
    __print_json_intel_begin_queue_annotation,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_queue_annotation,
#endif
};
void __trace_intel_begin_queue_annotation(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , void *cs
) {
   struct trace_intel_begin_queue_annotation entry;
   UNUSED struct trace_intel_begin_queue_annotation *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_queue_annotation *)u_trace_appendv(ut, cs, &__tp_intel_begin_queue_annotation,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_queue_annotation
 */
static void __print_intel_end_queue_annotation(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_queue_annotation *__entry =
      (const struct trace_intel_end_queue_annotation *)arg;
   fprintf(out, ""
      "str=%s, "
         "\n"
   ,__entry->str
   );
}

static void __print_json_intel_end_queue_annotation(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_queue_annotation *__entry =
      (const struct trace_intel_end_queue_annotation *)arg;
   fprintf(out, ""
      "\"str\": \"%s\""
   ,__entry->str
   );
}

static const struct u_tracepoint __tp_intel_end_queue_annotation = {
    "intel_end_queue_annotation",
    ALIGN_POT(sizeof(struct trace_intel_end_queue_annotation), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    3,
    __print_intel_end_queue_annotation,
    __print_json_intel_end_queue_annotation,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_queue_annotation,
#endif
};
void __trace_intel_end_queue_annotation(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , void *cs
   , unsigned len
   , const char *str
) {
   struct trace_intel_end_queue_annotation entry;
   UNUSED struct trace_intel_end_queue_annotation *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_queue_annotation *)u_trace_appendv(ut, cs, &__tp_intel_end_queue_annotation,
                                                    0
                                                    + len + 1
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->dummy = 0;
   strncpy(__entry->str, str, len + 1);
}

/*
 * intel_begin_batch
 */
#define __print_intel_begin_batch NULL
#define __print_json_intel_begin_batch NULL
static const struct u_tracepoint __tp_intel_begin_batch = {
    "intel_begin_batch",
    ALIGN_POT(sizeof(struct trace_intel_begin_batch), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    4,
    __print_intel_begin_batch,
    __print_json_intel_begin_batch,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_batch,
#endif
};
void __trace_intel_begin_batch(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_batch entry;
   UNUSED struct trace_intel_begin_batch *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_batch *)u_trace_appendv(ut, NULL, &__tp_intel_begin_batch,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_batch
 */
static void __print_intel_end_batch(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_batch *__entry =
      (const struct trace_intel_end_batch *)arg;
   fprintf(out, ""
      "name=%hhu, "
         "\n"
   ,__entry->name
   );
}

static void __print_json_intel_end_batch(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_batch *__entry =
      (const struct trace_intel_end_batch *)arg;
   fprintf(out, ""
      "\"name\": \"%hhu\""
   ,__entry->name
   );
}

static const struct u_tracepoint __tp_intel_end_batch = {
    "intel_end_batch",
    ALIGN_POT(sizeof(struct trace_intel_end_batch), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    5,
    __print_intel_end_batch,
    __print_json_intel_end_batch,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_batch,
#endif
};
void __trace_intel_end_batch(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint8_t name
) {
   struct trace_intel_end_batch entry;
   UNUSED struct trace_intel_end_batch *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_batch *)u_trace_appendv(ut, NULL, &__tp_intel_end_batch,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->name = name;
}

/*
 * intel_begin_cmd_buffer
 */
#define __print_intel_begin_cmd_buffer NULL
#define __print_json_intel_begin_cmd_buffer NULL
static const struct u_tracepoint __tp_intel_begin_cmd_buffer = {
    "intel_begin_cmd_buffer",
    ALIGN_POT(sizeof(struct trace_intel_begin_cmd_buffer), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    6,
    __print_intel_begin_cmd_buffer,
    __print_json_intel_begin_cmd_buffer,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_cmd_buffer,
#endif
};
void __trace_intel_begin_cmd_buffer(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_cmd_buffer entry;
   UNUSED struct trace_intel_begin_cmd_buffer *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_cmd_buffer *)u_trace_appendv(ut, NULL, &__tp_intel_begin_cmd_buffer,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_cmd_buffer
 */
static void __print_intel_end_cmd_buffer(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_cmd_buffer *__entry =
      (const struct trace_intel_end_cmd_buffer *)arg;
   fprintf(out, ""
      "level=%hhu, "
         "\n"
   ,__entry->level
   );
}

static void __print_json_intel_end_cmd_buffer(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_cmd_buffer *__entry =
      (const struct trace_intel_end_cmd_buffer *)arg;
   fprintf(out, ""
      "\"level\": \"%hhu\""
   ,__entry->level
   );
}

static const struct u_tracepoint __tp_intel_end_cmd_buffer = {
    "intel_end_cmd_buffer",
    ALIGN_POT(sizeof(struct trace_intel_end_cmd_buffer), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    7,
    __print_intel_end_cmd_buffer,
    __print_json_intel_end_cmd_buffer,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_cmd_buffer,
#endif
};
void __trace_intel_end_cmd_buffer(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint8_t level
) {
   struct trace_intel_end_cmd_buffer entry;
   UNUSED struct trace_intel_end_cmd_buffer *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_cmd_buffer *)u_trace_appendv(ut, NULL, &__tp_intel_end_cmd_buffer,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->level = level;
}

/*
 * intel_begin_cmd_buffer_annotation
 */
#define __print_intel_begin_cmd_buffer_annotation NULL
#define __print_json_intel_begin_cmd_buffer_annotation NULL
static const struct u_tracepoint __tp_intel_begin_cmd_buffer_annotation = {
    "intel_begin_cmd_buffer_annotation",
    ALIGN_POT(sizeof(struct trace_intel_begin_cmd_buffer_annotation), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    8,
    __print_intel_begin_cmd_buffer_annotation,
    __print_json_intel_begin_cmd_buffer_annotation,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_cmd_buffer_annotation,
#endif
};
void __trace_intel_begin_cmd_buffer_annotation(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_cmd_buffer_annotation entry;
   UNUSED struct trace_intel_begin_cmd_buffer_annotation *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_cmd_buffer_annotation *)u_trace_appendv(ut, NULL, &__tp_intel_begin_cmd_buffer_annotation,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_cmd_buffer_annotation
 */
static void __print_intel_end_cmd_buffer_annotation(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_cmd_buffer_annotation *__entry =
      (const struct trace_intel_end_cmd_buffer_annotation *)arg;
   fprintf(out, ""
      "str=%s, "
         "\n"
   ,__entry->str
   );
}

static void __print_json_intel_end_cmd_buffer_annotation(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_cmd_buffer_annotation *__entry =
      (const struct trace_intel_end_cmd_buffer_annotation *)arg;
   fprintf(out, ""
      "\"str\": \"%s\""
   ,__entry->str
   );
}

static const struct u_tracepoint __tp_intel_end_cmd_buffer_annotation = {
    "intel_end_cmd_buffer_annotation",
    ALIGN_POT(sizeof(struct trace_intel_end_cmd_buffer_annotation), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    9,
    __print_intel_end_cmd_buffer_annotation,
    __print_json_intel_end_cmd_buffer_annotation,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_cmd_buffer_annotation,
#endif
};
void __trace_intel_end_cmd_buffer_annotation(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , unsigned len
   , const char *str
) {
   struct trace_intel_end_cmd_buffer_annotation entry;
   UNUSED struct trace_intel_end_cmd_buffer_annotation *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_cmd_buffer_annotation *)u_trace_appendv(ut, NULL, &__tp_intel_end_cmd_buffer_annotation,
                                                    0
                                                    + len + 1
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->dummy = 0;
   strncpy(__entry->str, str, len + 1);
}

/*
 * intel_begin_xfb
 */
#define __print_intel_begin_xfb NULL
#define __print_json_intel_begin_xfb NULL
static const struct u_tracepoint __tp_intel_begin_xfb = {
    "intel_begin_xfb",
    ALIGN_POT(sizeof(struct trace_intel_begin_xfb), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    10,
    __print_intel_begin_xfb,
    __print_json_intel_begin_xfb,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_xfb,
#endif
};
void __trace_intel_begin_xfb(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_xfb entry;
   UNUSED struct trace_intel_begin_xfb *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_xfb *)u_trace_appendv(ut, NULL, &__tp_intel_begin_xfb,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_xfb
 */
#define __print_intel_end_xfb NULL
#define __print_json_intel_end_xfb NULL
static const struct u_tracepoint __tp_intel_end_xfb = {
    "intel_end_xfb",
    ALIGN_POT(sizeof(struct trace_intel_end_xfb), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    11,
    __print_intel_end_xfb,
    __print_json_intel_end_xfb,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_xfb,
#endif
};
void __trace_intel_end_xfb(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_end_xfb entry;
   UNUSED struct trace_intel_end_xfb *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_xfb *)u_trace_appendv(ut, NULL, &__tp_intel_end_xfb,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_begin_render_pass
 */
#define __print_intel_begin_render_pass NULL
#define __print_json_intel_begin_render_pass NULL
static const struct u_tracepoint __tp_intel_begin_render_pass = {
    "intel_begin_render_pass",
    ALIGN_POT(sizeof(struct trace_intel_begin_render_pass), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    12,
    __print_intel_begin_render_pass,
    __print_json_intel_begin_render_pass,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_render_pass,
#endif
};
void __trace_intel_begin_render_pass(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_render_pass entry;
   UNUSED struct trace_intel_begin_render_pass *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_render_pass *)u_trace_appendv(ut, NULL, &__tp_intel_begin_render_pass,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_render_pass
 */
static void __print_intel_end_render_pass(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_render_pass *__entry =
      (const struct trace_intel_end_render_pass *)arg;
   fprintf(out, ""
      "width=%hu, "
      "height=%hu, "
      "att_count=%hhu, "
      "msaa=%hhu, "
         "\n"
   ,__entry->width
   ,__entry->height
   ,__entry->att_count
   ,__entry->msaa
   );
}

static void __print_json_intel_end_render_pass(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_render_pass *__entry =
      (const struct trace_intel_end_render_pass *)arg;
   fprintf(out, ""
      "\"width\": \"%hu\""
         ", "
      "\"height\": \"%hu\""
         ", "
      "\"att_count\": \"%hhu\""
         ", "
      "\"msaa\": \"%hhu\""
   ,__entry->width
   ,__entry->height
   ,__entry->att_count
   ,__entry->msaa
   );
}

static const struct u_tracepoint __tp_intel_end_render_pass = {
    "intel_end_render_pass",
    ALIGN_POT(sizeof(struct trace_intel_end_render_pass), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    13,
    __print_intel_end_render_pass,
    __print_json_intel_end_render_pass,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_render_pass,
#endif
};
void __trace_intel_end_render_pass(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint16_t width
   , uint16_t height
   , uint8_t att_count
   , uint8_t msaa
) {
   struct trace_intel_end_render_pass entry;
   UNUSED struct trace_intel_end_render_pass *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_render_pass *)u_trace_appendv(ut, NULL, &__tp_intel_end_render_pass,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->width = width;
   __entry->height = height;
   __entry->att_count = att_count;
   __entry->msaa = msaa;
}

/*
 * intel_begin_blorp
 */
#define __print_intel_begin_blorp NULL
#define __print_json_intel_begin_blorp NULL
static const struct u_tracepoint __tp_intel_begin_blorp = {
    "intel_begin_blorp",
    ALIGN_POT(sizeof(struct trace_intel_begin_blorp), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    14,
    __print_intel_begin_blorp,
    __print_json_intel_begin_blorp,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_blorp,
#endif
};
void __trace_intel_begin_blorp(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_blorp entry;
   UNUSED struct trace_intel_begin_blorp *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_blorp *)u_trace_appendv(ut, NULL, &__tp_intel_begin_blorp,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_blorp
 */
static void __print_intel_end_blorp(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_blorp *__entry =
      (const struct trace_intel_end_blorp *)arg;
   fprintf(out, ""
      "op=%s, "
      "width=%u, "
      "height=%u, "
      "samples=%u, "
      "shader_pipe=%s, "
      "dst_fmt=%s, "
      "src_fmt=%s, "
      "predicated=%hhu, "
         "\n"
   ,blorp_op_to_name(__entry->op)
   ,__entry->width
   ,__entry->height
   ,__entry->samples
   ,blorp_shader_pipeline_to_name(__entry->shader_pipe)
   ,isl_format_get_short_name(__entry->dst_fmt)
   ,isl_format_get_short_name(__entry->src_fmt)
   ,__entry->predicated
   );
}

static void __print_json_intel_end_blorp(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_blorp *__entry =
      (const struct trace_intel_end_blorp *)arg;
   fprintf(out, ""
      "\"op\": \"%s\""
         ", "
      "\"width\": \"%u\""
         ", "
      "\"height\": \"%u\""
         ", "
      "\"samples\": \"%u\""
         ", "
      "\"shader_pipe\": \"%s\""
         ", "
      "\"dst_fmt\": \"%s\""
         ", "
      "\"src_fmt\": \"%s\""
         ", "
      "\"predicated\": \"%hhu\""
   ,blorp_op_to_name(__entry->op)
   ,__entry->width
   ,__entry->height
   ,__entry->samples
   ,blorp_shader_pipeline_to_name(__entry->shader_pipe)
   ,isl_format_get_short_name(__entry->dst_fmt)
   ,isl_format_get_short_name(__entry->src_fmt)
   ,__entry->predicated
   );
}

static const struct u_tracepoint __tp_intel_end_blorp = {
    "intel_end_blorp",
    ALIGN_POT(sizeof(struct trace_intel_end_blorp), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    15,
    __print_intel_end_blorp,
    __print_json_intel_end_blorp,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_blorp,
#endif
};
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
) {
   struct trace_intel_end_blorp entry;
   UNUSED struct trace_intel_end_blorp *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_blorp *)u_trace_appendv(ut, NULL, &__tp_intel_end_blorp,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->op = op;
   __entry->width = width;
   __entry->height = height;
   __entry->samples = samples;
   __entry->shader_pipe = shader_pipe;
   __entry->dst_fmt = dst_fmt;
   __entry->src_fmt = src_fmt;
   __entry->predicated = predicated;
}

/*
 * intel_begin_write_buffer_marker
 */
#define __print_intel_begin_write_buffer_marker NULL
#define __print_json_intel_begin_write_buffer_marker NULL
static const struct u_tracepoint __tp_intel_begin_write_buffer_marker = {
    "intel_begin_write_buffer_marker",
    ALIGN_POT(sizeof(struct trace_intel_begin_write_buffer_marker), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    16,
    __print_intel_begin_write_buffer_marker,
    __print_json_intel_begin_write_buffer_marker,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_write_buffer_marker,
#endif
};
void __trace_intel_begin_write_buffer_marker(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_write_buffer_marker entry;
   UNUSED struct trace_intel_begin_write_buffer_marker *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_write_buffer_marker *)u_trace_appendv(ut, NULL, &__tp_intel_begin_write_buffer_marker,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_write_buffer_marker
 */
#define __print_intel_end_write_buffer_marker NULL
#define __print_json_intel_end_write_buffer_marker NULL
static const struct u_tracepoint __tp_intel_end_write_buffer_marker = {
    "intel_end_write_buffer_marker",
    ALIGN_POT(sizeof(struct trace_intel_end_write_buffer_marker), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    17,
    __print_intel_end_write_buffer_marker,
    __print_json_intel_end_write_buffer_marker,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_write_buffer_marker,
#endif
};
void __trace_intel_end_write_buffer_marker(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_end_write_buffer_marker entry;
   UNUSED struct trace_intel_end_write_buffer_marker *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_write_buffer_marker *)u_trace_appendv(ut, NULL, &__tp_intel_end_write_buffer_marker,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_begin_generate_draws
 */
#define __print_intel_begin_generate_draws NULL
#define __print_json_intel_begin_generate_draws NULL
static const struct u_tracepoint __tp_intel_begin_generate_draws = {
    "intel_begin_generate_draws",
    ALIGN_POT(sizeof(struct trace_intel_begin_generate_draws), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    18,
    __print_intel_begin_generate_draws,
    __print_json_intel_begin_generate_draws,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_generate_draws,
#endif
};
void __trace_intel_begin_generate_draws(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_generate_draws entry;
   UNUSED struct trace_intel_begin_generate_draws *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_generate_draws *)u_trace_appendv(ut, NULL, &__tp_intel_begin_generate_draws,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_generate_draws
 */
#define __print_intel_end_generate_draws NULL
#define __print_json_intel_end_generate_draws NULL
static const struct u_tracepoint __tp_intel_end_generate_draws = {
    "intel_end_generate_draws",
    ALIGN_POT(sizeof(struct trace_intel_end_generate_draws), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    19,
    __print_intel_end_generate_draws,
    __print_json_intel_end_generate_draws,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_generate_draws,
#endif
};
void __trace_intel_end_generate_draws(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_end_generate_draws entry;
   UNUSED struct trace_intel_end_generate_draws *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_generate_draws *)u_trace_appendv(ut, NULL, &__tp_intel_end_generate_draws,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_begin_generate_commands
 */
#define __print_intel_begin_generate_commands NULL
#define __print_json_intel_begin_generate_commands NULL
static const struct u_tracepoint __tp_intel_begin_generate_commands = {
    "intel_begin_generate_commands",
    ALIGN_POT(sizeof(struct trace_intel_begin_generate_commands), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    20,
    __print_intel_begin_generate_commands,
    __print_json_intel_begin_generate_commands,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_generate_commands,
#endif
};
void __trace_intel_begin_generate_commands(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_generate_commands entry;
   UNUSED struct trace_intel_begin_generate_commands *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_generate_commands *)u_trace_appendv(ut, NULL, &__tp_intel_begin_generate_commands,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_generate_commands
 */
#define __print_intel_end_generate_commands NULL
#define __print_json_intel_end_generate_commands NULL
static const struct u_tracepoint __tp_intel_end_generate_commands = {
    "intel_end_generate_commands",
    ALIGN_POT(sizeof(struct trace_intel_end_generate_commands), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    21,
    __print_intel_end_generate_commands,
    __print_json_intel_end_generate_commands,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_generate_commands,
#endif
};
void __trace_intel_end_generate_commands(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_end_generate_commands entry;
   UNUSED struct trace_intel_end_generate_commands *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_generate_commands *)u_trace_appendv(ut, NULL, &__tp_intel_end_generate_commands,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_begin_query_clear_blorp
 */
#define __print_intel_begin_query_clear_blorp NULL
#define __print_json_intel_begin_query_clear_blorp NULL
static const struct u_tracepoint __tp_intel_begin_query_clear_blorp = {
    "intel_begin_query_clear_blorp",
    ALIGN_POT(sizeof(struct trace_intel_begin_query_clear_blorp), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    22,
    __print_intel_begin_query_clear_blorp,
    __print_json_intel_begin_query_clear_blorp,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_query_clear_blorp,
#endif
};
void __trace_intel_begin_query_clear_blorp(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_query_clear_blorp entry;
   UNUSED struct trace_intel_begin_query_clear_blorp *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_query_clear_blorp *)u_trace_appendv(ut, NULL, &__tp_intel_begin_query_clear_blorp,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_query_clear_blorp
 */
static void __print_intel_end_query_clear_blorp(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_query_clear_blorp *__entry =
      (const struct trace_intel_end_query_clear_blorp *)arg;
   fprintf(out, ""
      "count=%u, "
         "\n"
   ,__entry->count
   );
}

static void __print_json_intel_end_query_clear_blorp(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_query_clear_blorp *__entry =
      (const struct trace_intel_end_query_clear_blorp *)arg;
   fprintf(out, ""
      "\"count\": \"%u\""
   ,__entry->count
   );
}

static const struct u_tracepoint __tp_intel_end_query_clear_blorp = {
    "intel_end_query_clear_blorp",
    ALIGN_POT(sizeof(struct trace_intel_end_query_clear_blorp), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    23,
    __print_intel_end_query_clear_blorp,
    __print_json_intel_end_query_clear_blorp,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_query_clear_blorp,
#endif
};
void __trace_intel_end_query_clear_blorp(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t count
) {
   struct trace_intel_end_query_clear_blorp entry;
   UNUSED struct trace_intel_end_query_clear_blorp *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_query_clear_blorp *)u_trace_appendv(ut, NULL, &__tp_intel_end_query_clear_blorp,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->count = count;
}

/*
 * intel_begin_query_clear_cs
 */
#define __print_intel_begin_query_clear_cs NULL
#define __print_json_intel_begin_query_clear_cs NULL
static const struct u_tracepoint __tp_intel_begin_query_clear_cs = {
    "intel_begin_query_clear_cs",
    ALIGN_POT(sizeof(struct trace_intel_begin_query_clear_cs), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    24,
    __print_intel_begin_query_clear_cs,
    __print_json_intel_begin_query_clear_cs,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_query_clear_cs,
#endif
};
void __trace_intel_begin_query_clear_cs(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_query_clear_cs entry;
   UNUSED struct trace_intel_begin_query_clear_cs *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_query_clear_cs *)u_trace_appendv(ut, NULL, &__tp_intel_begin_query_clear_cs,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_query_clear_cs
 */
static void __print_intel_end_query_clear_cs(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_query_clear_cs *__entry =
      (const struct trace_intel_end_query_clear_cs *)arg;
   fprintf(out, ""
      "count=%u, "
         "\n"
   ,__entry->count
   );
}

static void __print_json_intel_end_query_clear_cs(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_query_clear_cs *__entry =
      (const struct trace_intel_end_query_clear_cs *)arg;
   fprintf(out, ""
      "\"count\": \"%u\""
   ,__entry->count
   );
}

static const struct u_tracepoint __tp_intel_end_query_clear_cs = {
    "intel_end_query_clear_cs",
    ALIGN_POT(sizeof(struct trace_intel_end_query_clear_cs), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    25,
    __print_intel_end_query_clear_cs,
    __print_json_intel_end_query_clear_cs,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_query_clear_cs,
#endif
};
void __trace_intel_end_query_clear_cs(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t count
) {
   struct trace_intel_end_query_clear_cs entry;
   UNUSED struct trace_intel_end_query_clear_cs *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_query_clear_cs *)u_trace_appendv(ut, NULL, &__tp_intel_end_query_clear_cs,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->count = count;
}

/*
 * intel_begin_query_copy_cs
 */
#define __print_intel_begin_query_copy_cs NULL
#define __print_json_intel_begin_query_copy_cs NULL
static const struct u_tracepoint __tp_intel_begin_query_copy_cs = {
    "intel_begin_query_copy_cs",
    ALIGN_POT(sizeof(struct trace_intel_begin_query_copy_cs), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    26,
    __print_intel_begin_query_copy_cs,
    __print_json_intel_begin_query_copy_cs,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_query_copy_cs,
#endif
};
void __trace_intel_begin_query_copy_cs(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_query_copy_cs entry;
   UNUSED struct trace_intel_begin_query_copy_cs *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_query_copy_cs *)u_trace_appendv(ut, NULL, &__tp_intel_begin_query_copy_cs,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_query_copy_cs
 */
static void __print_intel_end_query_copy_cs(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_query_copy_cs *__entry =
      (const struct trace_intel_end_query_copy_cs *)arg;
   fprintf(out, ""
      "count=%u, "
         "\n"
   ,__entry->count
   );
}

static void __print_json_intel_end_query_copy_cs(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_query_copy_cs *__entry =
      (const struct trace_intel_end_query_copy_cs *)arg;
   fprintf(out, ""
      "\"count\": \"%u\""
   ,__entry->count
   );
}

static const struct u_tracepoint __tp_intel_end_query_copy_cs = {
    "intel_end_query_copy_cs",
    ALIGN_POT(sizeof(struct trace_intel_end_query_copy_cs), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    27,
    __print_intel_end_query_copy_cs,
    __print_json_intel_end_query_copy_cs,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_query_copy_cs,
#endif
};
void __trace_intel_end_query_copy_cs(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t count
) {
   struct trace_intel_end_query_copy_cs entry;
   UNUSED struct trace_intel_end_query_copy_cs *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_query_copy_cs *)u_trace_appendv(ut, NULL, &__tp_intel_end_query_copy_cs,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->count = count;
}

/*
 * intel_begin_query_copy_shader
 */
#define __print_intel_begin_query_copy_shader NULL
#define __print_json_intel_begin_query_copy_shader NULL
static const struct u_tracepoint __tp_intel_begin_query_copy_shader = {
    "intel_begin_query_copy_shader",
    ALIGN_POT(sizeof(struct trace_intel_begin_query_copy_shader), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    28,
    __print_intel_begin_query_copy_shader,
    __print_json_intel_begin_query_copy_shader,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_query_copy_shader,
#endif
};
void __trace_intel_begin_query_copy_shader(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_query_copy_shader entry;
   UNUSED struct trace_intel_begin_query_copy_shader *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_query_copy_shader *)u_trace_appendv(ut, NULL, &__tp_intel_begin_query_copy_shader,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_query_copy_shader
 */
static void __print_intel_end_query_copy_shader(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_query_copy_shader *__entry =
      (const struct trace_intel_end_query_copy_shader *)arg;
   fprintf(out, ""
      "count=%u, "
         "\n"
   ,__entry->count
   );
}

static void __print_json_intel_end_query_copy_shader(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_query_copy_shader *__entry =
      (const struct trace_intel_end_query_copy_shader *)arg;
   fprintf(out, ""
      "\"count\": \"%u\""
   ,__entry->count
   );
}

static const struct u_tracepoint __tp_intel_end_query_copy_shader = {
    "intel_end_query_copy_shader",
    ALIGN_POT(sizeof(struct trace_intel_end_query_copy_shader), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    29,
    __print_intel_end_query_copy_shader,
    __print_json_intel_end_query_copy_shader,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_query_copy_shader,
#endif
};
void __trace_intel_end_query_copy_shader(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t count
) {
   struct trace_intel_end_query_copy_shader entry;
   UNUSED struct trace_intel_end_query_copy_shader *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_query_copy_shader *)u_trace_appendv(ut, NULL, &__tp_intel_end_query_copy_shader,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->count = count;
}

/*
 * intel_begin_draw
 */
#define __print_intel_begin_draw NULL
#define __print_json_intel_begin_draw NULL
static const struct u_tracepoint __tp_intel_begin_draw = {
    "intel_begin_draw",
    ALIGN_POT(sizeof(struct trace_intel_begin_draw), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    30,
    __print_intel_begin_draw,
    __print_json_intel_begin_draw,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_draw,
#endif
};
void __trace_intel_begin_draw(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_draw entry;
   UNUSED struct trace_intel_begin_draw *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_draw *)u_trace_appendv(ut, NULL, &__tp_intel_begin_draw,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_draw
 */
static void __print_intel_end_draw(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw *__entry =
      (const struct trace_intel_end_draw *)arg;
   fprintf(out, ""
      "count=%u, "
      "vs_hash=%#x, "
      "fs_hash=%#x, "
         "\n"
   ,__entry->count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static void __print_json_intel_end_draw(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw *__entry =
      (const struct trace_intel_end_draw *)arg;
   fprintf(out, ""
      "\"count\": \"%u\""
         ", "
      "\"vs_hash\": \"%#x\""
         ", "
      "\"fs_hash\": \"%#x\""
   ,__entry->count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static const struct u_tracepoint __tp_intel_end_draw = {
    "intel_end_draw",
    ALIGN_POT(sizeof(struct trace_intel_end_draw), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    31,
    __print_intel_end_draw,
    __print_json_intel_end_draw,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_draw,
#endif
};
void __trace_intel_end_draw(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   struct trace_intel_end_draw entry;
   UNUSED struct trace_intel_end_draw *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_draw *)u_trace_appendv(ut, NULL, &__tp_intel_end_draw,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->count = count;
   __entry->vs_hash = vs_hash;
   __entry->fs_hash = fs_hash;
}

/*
 * intel_begin_draw_multi
 */
#define __print_intel_begin_draw_multi NULL
#define __print_json_intel_begin_draw_multi NULL
static const struct u_tracepoint __tp_intel_begin_draw_multi = {
    "intel_begin_draw_multi",
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_multi), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    32,
    __print_intel_begin_draw_multi,
    __print_json_intel_begin_draw_multi,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_draw_multi,
#endif
};
void __trace_intel_begin_draw_multi(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_draw_multi entry;
   UNUSED struct trace_intel_begin_draw_multi *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_draw_multi *)u_trace_appendv(ut, NULL, &__tp_intel_begin_draw_multi,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_draw_multi
 */
static void __print_intel_end_draw_multi(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_multi *__entry =
      (const struct trace_intel_end_draw_multi *)arg;
   fprintf(out, ""
      "count=%u, "
      "vs_hash=%#x, "
      "fs_hash=%#x, "
         "\n"
   ,__entry->count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static void __print_json_intel_end_draw_multi(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_multi *__entry =
      (const struct trace_intel_end_draw_multi *)arg;
   fprintf(out, ""
      "\"count\": \"%u\""
         ", "
      "\"vs_hash\": \"%#x\""
         ", "
      "\"fs_hash\": \"%#x\""
   ,__entry->count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static const struct u_tracepoint __tp_intel_end_draw_multi = {
    "intel_end_draw_multi",
    ALIGN_POT(sizeof(struct trace_intel_end_draw_multi), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    33,
    __print_intel_end_draw_multi,
    __print_json_intel_end_draw_multi,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_draw_multi,
#endif
};
void __trace_intel_end_draw_multi(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   struct trace_intel_end_draw_multi entry;
   UNUSED struct trace_intel_end_draw_multi *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_draw_multi *)u_trace_appendv(ut, NULL, &__tp_intel_end_draw_multi,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->count = count;
   __entry->vs_hash = vs_hash;
   __entry->fs_hash = fs_hash;
}

/*
 * intel_begin_draw_indexed
 */
#define __print_intel_begin_draw_indexed NULL
#define __print_json_intel_begin_draw_indexed NULL
static const struct u_tracepoint __tp_intel_begin_draw_indexed = {
    "intel_begin_draw_indexed",
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indexed), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    34,
    __print_intel_begin_draw_indexed,
    __print_json_intel_begin_draw_indexed,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_draw_indexed,
#endif
};
void __trace_intel_begin_draw_indexed(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_draw_indexed entry;
   UNUSED struct trace_intel_begin_draw_indexed *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_draw_indexed *)u_trace_appendv(ut, NULL, &__tp_intel_begin_draw_indexed,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_draw_indexed
 */
static void __print_intel_end_draw_indexed(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indexed *__entry =
      (const struct trace_intel_end_draw_indexed *)arg;
   fprintf(out, ""
      "count=%u, "
      "vs_hash=%#x, "
      "fs_hash=%#x, "
         "\n"
   ,__entry->count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static void __print_json_intel_end_draw_indexed(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indexed *__entry =
      (const struct trace_intel_end_draw_indexed *)arg;
   fprintf(out, ""
      "\"count\": \"%u\""
         ", "
      "\"vs_hash\": \"%#x\""
         ", "
      "\"fs_hash\": \"%#x\""
   ,__entry->count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static const struct u_tracepoint __tp_intel_end_draw_indexed = {
    "intel_end_draw_indexed",
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indexed), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    35,
    __print_intel_end_draw_indexed,
    __print_json_intel_end_draw_indexed,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_draw_indexed,
#endif
};
void __trace_intel_end_draw_indexed(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   struct trace_intel_end_draw_indexed entry;
   UNUSED struct trace_intel_end_draw_indexed *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_draw_indexed *)u_trace_appendv(ut, NULL, &__tp_intel_end_draw_indexed,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->count = count;
   __entry->vs_hash = vs_hash;
   __entry->fs_hash = fs_hash;
}

/*
 * intel_begin_draw_indexed_multi
 */
#define __print_intel_begin_draw_indexed_multi NULL
#define __print_json_intel_begin_draw_indexed_multi NULL
static const struct u_tracepoint __tp_intel_begin_draw_indexed_multi = {
    "intel_begin_draw_indexed_multi",
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indexed_multi), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    36,
    __print_intel_begin_draw_indexed_multi,
    __print_json_intel_begin_draw_indexed_multi,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_draw_indexed_multi,
#endif
};
void __trace_intel_begin_draw_indexed_multi(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_draw_indexed_multi entry;
   UNUSED struct trace_intel_begin_draw_indexed_multi *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_draw_indexed_multi *)u_trace_appendv(ut, NULL, &__tp_intel_begin_draw_indexed_multi,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_draw_indexed_multi
 */
static void __print_intel_end_draw_indexed_multi(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indexed_multi *__entry =
      (const struct trace_intel_end_draw_indexed_multi *)arg;
   fprintf(out, ""
      "count=%u, "
      "vs_hash=%#x, "
      "fs_hash=%#x, "
         "\n"
   ,__entry->count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static void __print_json_intel_end_draw_indexed_multi(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indexed_multi *__entry =
      (const struct trace_intel_end_draw_indexed_multi *)arg;
   fprintf(out, ""
      "\"count\": \"%u\""
         ", "
      "\"vs_hash\": \"%#x\""
         ", "
      "\"fs_hash\": \"%#x\""
   ,__entry->count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static const struct u_tracepoint __tp_intel_end_draw_indexed_multi = {
    "intel_end_draw_indexed_multi",
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indexed_multi), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    37,
    __print_intel_end_draw_indexed_multi,
    __print_json_intel_end_draw_indexed_multi,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_draw_indexed_multi,
#endif
};
void __trace_intel_end_draw_indexed_multi(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   struct trace_intel_end_draw_indexed_multi entry;
   UNUSED struct trace_intel_end_draw_indexed_multi *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_draw_indexed_multi *)u_trace_appendv(ut, NULL, &__tp_intel_end_draw_indexed_multi,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->count = count;
   __entry->vs_hash = vs_hash;
   __entry->fs_hash = fs_hash;
}

/*
 * intel_begin_draw_indirect_byte_count
 */
#define __print_intel_begin_draw_indirect_byte_count NULL
#define __print_json_intel_begin_draw_indirect_byte_count NULL
static const struct u_tracepoint __tp_intel_begin_draw_indirect_byte_count = {
    "intel_begin_draw_indirect_byte_count",
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indirect_byte_count), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    38,
    __print_intel_begin_draw_indirect_byte_count,
    __print_json_intel_begin_draw_indirect_byte_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_draw_indirect_byte_count,
#endif
};
void __trace_intel_begin_draw_indirect_byte_count(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_draw_indirect_byte_count entry;
   UNUSED struct trace_intel_begin_draw_indirect_byte_count *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_draw_indirect_byte_count *)u_trace_appendv(ut, NULL, &__tp_intel_begin_draw_indirect_byte_count,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_draw_indirect_byte_count
 */
static void __print_intel_end_draw_indirect_byte_count(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indirect_byte_count *__entry =
      (const struct trace_intel_end_draw_indirect_byte_count *)arg;
   fprintf(out, ""
      "instance_count=%u, "
      "vs_hash=%#x, "
      "fs_hash=%#x, "
         "\n"
   ,__entry->instance_count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static void __print_json_intel_end_draw_indirect_byte_count(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indirect_byte_count *__entry =
      (const struct trace_intel_end_draw_indirect_byte_count *)arg;
   fprintf(out, ""
      "\"instance_count\": \"%u\""
         ", "
      "\"vs_hash\": \"%#x\""
         ", "
      "\"fs_hash\": \"%#x\""
   ,__entry->instance_count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static const struct u_tracepoint __tp_intel_end_draw_indirect_byte_count = {
    "intel_end_draw_indirect_byte_count",
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indirect_byte_count), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    39,
    __print_intel_end_draw_indirect_byte_count,
    __print_json_intel_end_draw_indirect_byte_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_draw_indirect_byte_count,
#endif
};
void __trace_intel_end_draw_indirect_byte_count(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t instance_count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   struct trace_intel_end_draw_indirect_byte_count entry;
   UNUSED struct trace_intel_end_draw_indirect_byte_count *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_draw_indirect_byte_count *)u_trace_appendv(ut, NULL, &__tp_intel_end_draw_indirect_byte_count,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->instance_count = instance_count;
   __entry->vs_hash = vs_hash;
   __entry->fs_hash = fs_hash;
}

/*
 * intel_begin_draw_indirect
 */
#define __print_intel_begin_draw_indirect NULL
#define __print_json_intel_begin_draw_indirect NULL
static const struct u_tracepoint __tp_intel_begin_draw_indirect = {
    "intel_begin_draw_indirect",
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indirect), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    40,
    __print_intel_begin_draw_indirect,
    __print_json_intel_begin_draw_indirect,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_draw_indirect,
#endif
};
void __trace_intel_begin_draw_indirect(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_draw_indirect entry;
   UNUSED struct trace_intel_begin_draw_indirect *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_draw_indirect *)u_trace_appendv(ut, NULL, &__tp_intel_begin_draw_indirect,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_draw_indirect
 */
static void __print_intel_end_draw_indirect(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indirect *__entry =
      (const struct trace_intel_end_draw_indirect *)arg;
   fprintf(out, ""
      "draw_count=%u, "
      "vs_hash=%#x, "
      "fs_hash=%#x, "
         "\n"
   ,__entry->draw_count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static void __print_json_intel_end_draw_indirect(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indirect *__entry =
      (const struct trace_intel_end_draw_indirect *)arg;
   fprintf(out, ""
      "\"draw_count\": \"%u\""
         ", "
      "\"vs_hash\": \"%#x\""
         ", "
      "\"fs_hash\": \"%#x\""
   ,__entry->draw_count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static const struct u_tracepoint __tp_intel_end_draw_indirect = {
    "intel_end_draw_indirect",
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indirect), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    41,
    __print_intel_end_draw_indirect,
    __print_json_intel_end_draw_indirect,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_draw_indirect,
#endif
};
void __trace_intel_end_draw_indirect(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t draw_count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   struct trace_intel_end_draw_indirect entry;
   UNUSED struct trace_intel_end_draw_indirect *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_draw_indirect *)u_trace_appendv(ut, NULL, &__tp_intel_end_draw_indirect,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->draw_count = draw_count;
   __entry->vs_hash = vs_hash;
   __entry->fs_hash = fs_hash;
}

/*
 * intel_begin_draw_indexed_indirect
 */
#define __print_intel_begin_draw_indexed_indirect NULL
#define __print_json_intel_begin_draw_indexed_indirect NULL
static const struct u_tracepoint __tp_intel_begin_draw_indexed_indirect = {
    "intel_begin_draw_indexed_indirect",
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indexed_indirect), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    42,
    __print_intel_begin_draw_indexed_indirect,
    __print_json_intel_begin_draw_indexed_indirect,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_draw_indexed_indirect,
#endif
};
void __trace_intel_begin_draw_indexed_indirect(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_draw_indexed_indirect entry;
   UNUSED struct trace_intel_begin_draw_indexed_indirect *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_draw_indexed_indirect *)u_trace_appendv(ut, NULL, &__tp_intel_begin_draw_indexed_indirect,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_draw_indexed_indirect
 */
static void __print_intel_end_draw_indexed_indirect(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indexed_indirect *__entry =
      (const struct trace_intel_end_draw_indexed_indirect *)arg;
   fprintf(out, ""
      "draw_count=%u, "
      "vs_hash=%#x, "
      "fs_hash=%#x, "
         "\n"
   ,__entry->draw_count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static void __print_json_intel_end_draw_indexed_indirect(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indexed_indirect *__entry =
      (const struct trace_intel_end_draw_indexed_indirect *)arg;
   fprintf(out, ""
      "\"draw_count\": \"%u\""
         ", "
      "\"vs_hash\": \"%#x\""
         ", "
      "\"fs_hash\": \"%#x\""
   ,__entry->draw_count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static const struct u_tracepoint __tp_intel_end_draw_indexed_indirect = {
    "intel_end_draw_indexed_indirect",
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indexed_indirect), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    43,
    __print_intel_end_draw_indexed_indirect,
    __print_json_intel_end_draw_indexed_indirect,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_draw_indexed_indirect,
#endif
};
void __trace_intel_end_draw_indexed_indirect(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t draw_count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   struct trace_intel_end_draw_indexed_indirect entry;
   UNUSED struct trace_intel_end_draw_indexed_indirect *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_draw_indexed_indirect *)u_trace_appendv(ut, NULL, &__tp_intel_end_draw_indexed_indirect,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->draw_count = draw_count;
   __entry->vs_hash = vs_hash;
   __entry->fs_hash = fs_hash;
}

/*
 * intel_begin_draw_indirect_count
 */
#define __print_intel_begin_draw_indirect_count NULL
#define __print_json_intel_begin_draw_indirect_count NULL
static const struct u_tracepoint __tp_intel_begin_draw_indirect_count = {
    "intel_begin_draw_indirect_count",
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indirect_count), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    44,
    __print_intel_begin_draw_indirect_count,
    __print_json_intel_begin_draw_indirect_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_draw_indirect_count,
#endif
};
void __trace_intel_begin_draw_indirect_count(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_draw_indirect_count entry;
   UNUSED struct trace_intel_begin_draw_indirect_count *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_draw_indirect_count *)u_trace_appendv(ut, NULL, &__tp_intel_begin_draw_indirect_count,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_draw_indirect_count
 */
static void __print_intel_end_draw_indirect_count(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indirect_count *__entry =
      (const struct trace_intel_end_draw_indirect_count *)arg;
   const uint32_t *__draw_count = (const uint32_t *) ((char *)indirect + 0);
   fprintf(out, ""
      "draw_count=%u, "
      "vs_hash=%#x, "
      "fs_hash=%#x, "
         "\n"
   ,*__draw_count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static void __print_json_intel_end_draw_indirect_count(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indirect_count *__entry =
      (const struct trace_intel_end_draw_indirect_count *)arg;
   const uint32_t *__draw_count = (const uint32_t *) ((char *)indirect + 0);
   fprintf(out, ""
      "\"draw_count\": \"%u\""
         ", "
      "\"vs_hash\": \"%#x\""
         ", "
      "\"fs_hash\": \"%#x\""
   ,*__draw_count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static const struct u_tracepoint __tp_intel_end_draw_indirect_count = {
    "intel_end_draw_indirect_count",
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indirect_count), 8),   /* keep size 64b aligned */
    0
    + sizeof(uint32_t)
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    45,
    __print_intel_end_draw_indirect_count,
    __print_json_intel_end_draw_indirect_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_draw_indirect_count,
#endif
};
void __trace_intel_end_draw_indirect_count(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , struct u_trace_address draw_count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   struct trace_intel_end_draw_indirect_count entry;
   struct u_trace_address indirects[] = {
      draw_count,
   };
   uint8_t indirect_sizes[] = {
      sizeof(uint32_t),
   };
   UNUSED struct trace_intel_end_draw_indirect_count *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_draw_indirect_count *)u_trace_appendv(ut, NULL, &__tp_intel_end_draw_indirect_count,
                                                    0
                                                    ,
                                                    ARRAY_SIZE(indirects), indirects, indirect_sizes
                                                    ) :
      &entry;
   __entry->vs_hash = vs_hash;
   __entry->fs_hash = fs_hash;
}

/*
 * intel_begin_draw_indexed_indirect_count
 */
#define __print_intel_begin_draw_indexed_indirect_count NULL
#define __print_json_intel_begin_draw_indexed_indirect_count NULL
static const struct u_tracepoint __tp_intel_begin_draw_indexed_indirect_count = {
    "intel_begin_draw_indexed_indirect_count",
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indexed_indirect_count), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    46,
    __print_intel_begin_draw_indexed_indirect_count,
    __print_json_intel_begin_draw_indexed_indirect_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_draw_indexed_indirect_count,
#endif
};
void __trace_intel_begin_draw_indexed_indirect_count(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_draw_indexed_indirect_count entry;
   UNUSED struct trace_intel_begin_draw_indexed_indirect_count *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_draw_indexed_indirect_count *)u_trace_appendv(ut, NULL, &__tp_intel_begin_draw_indexed_indirect_count,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_draw_indexed_indirect_count
 */
static void __print_intel_end_draw_indexed_indirect_count(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indexed_indirect_count *__entry =
      (const struct trace_intel_end_draw_indexed_indirect_count *)arg;
   const uint32_t *__draw_count = (const uint32_t *) ((char *)indirect + 0);
   fprintf(out, ""
      "draw_count=%u, "
      "vs_hash=%#x, "
      "fs_hash=%#x, "
         "\n"
   ,*__draw_count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static void __print_json_intel_end_draw_indexed_indirect_count(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_indexed_indirect_count *__entry =
      (const struct trace_intel_end_draw_indexed_indirect_count *)arg;
   const uint32_t *__draw_count = (const uint32_t *) ((char *)indirect + 0);
   fprintf(out, ""
      "\"draw_count\": \"%u\""
         ", "
      "\"vs_hash\": \"%#x\""
         ", "
      "\"fs_hash\": \"%#x\""
   ,*__draw_count
   ,__entry->vs_hash
   ,__entry->fs_hash
   );
}

static const struct u_tracepoint __tp_intel_end_draw_indexed_indirect_count = {
    "intel_end_draw_indexed_indirect_count",
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indexed_indirect_count), 8),   /* keep size 64b aligned */
    0
    + sizeof(uint32_t)
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    47,
    __print_intel_end_draw_indexed_indirect_count,
    __print_json_intel_end_draw_indexed_indirect_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_draw_indexed_indirect_count,
#endif
};
void __trace_intel_end_draw_indexed_indirect_count(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , struct u_trace_address draw_count
   , uint32_t vs_hash
   , uint32_t fs_hash
) {
   struct trace_intel_end_draw_indexed_indirect_count entry;
   struct u_trace_address indirects[] = {
      draw_count,
   };
   uint8_t indirect_sizes[] = {
      sizeof(uint32_t),
   };
   UNUSED struct trace_intel_end_draw_indexed_indirect_count *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_draw_indexed_indirect_count *)u_trace_appendv(ut, NULL, &__tp_intel_end_draw_indexed_indirect_count,
                                                    0
                                                    ,
                                                    ARRAY_SIZE(indirects), indirects, indirect_sizes
                                                    ) :
      &entry;
   __entry->vs_hash = vs_hash;
   __entry->fs_hash = fs_hash;
}

/*
 * intel_begin_draw_mesh
 */
#define __print_intel_begin_draw_mesh NULL
#define __print_json_intel_begin_draw_mesh NULL
static const struct u_tracepoint __tp_intel_begin_draw_mesh = {
    "intel_begin_draw_mesh",
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_mesh), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    48,
    __print_intel_begin_draw_mesh,
    __print_json_intel_begin_draw_mesh,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_draw_mesh,
#endif
};
void __trace_intel_begin_draw_mesh(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_draw_mesh entry;
   UNUSED struct trace_intel_begin_draw_mesh *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_draw_mesh *)u_trace_appendv(ut, NULL, &__tp_intel_begin_draw_mesh,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_draw_mesh
 */
static void __print_intel_end_draw_mesh(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_mesh *__entry =
      (const struct trace_intel_end_draw_mesh *)arg;
   fprintf(out, ""
      "group_x=%u, "
      "group_y=%u, "
      "group_z=%u, "
         "\n"
   ,__entry->group_x
   ,__entry->group_y
   ,__entry->group_z
   );
}

static void __print_json_intel_end_draw_mesh(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_mesh *__entry =
      (const struct trace_intel_end_draw_mesh *)arg;
   fprintf(out, ""
      "\"group_x\": \"%u\""
         ", "
      "\"group_y\": \"%u\""
         ", "
      "\"group_z\": \"%u\""
   ,__entry->group_x
   ,__entry->group_y
   ,__entry->group_z
   );
}

static const struct u_tracepoint __tp_intel_end_draw_mesh = {
    "intel_end_draw_mesh",
    ALIGN_POT(sizeof(struct trace_intel_end_draw_mesh), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    49,
    __print_intel_end_draw_mesh,
    __print_json_intel_end_draw_mesh,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_draw_mesh,
#endif
};
void __trace_intel_end_draw_mesh(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t group_x
   , uint32_t group_y
   , uint32_t group_z
) {
   struct trace_intel_end_draw_mesh entry;
   UNUSED struct trace_intel_end_draw_mesh *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_draw_mesh *)u_trace_appendv(ut, NULL, &__tp_intel_end_draw_mesh,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->group_x = group_x;
   __entry->group_y = group_y;
   __entry->group_z = group_z;
}

/*
 * intel_begin_draw_mesh_indirect
 */
#define __print_intel_begin_draw_mesh_indirect NULL
#define __print_json_intel_begin_draw_mesh_indirect NULL
static const struct u_tracepoint __tp_intel_begin_draw_mesh_indirect = {
    "intel_begin_draw_mesh_indirect",
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_mesh_indirect), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    50,
    __print_intel_begin_draw_mesh_indirect,
    __print_json_intel_begin_draw_mesh_indirect,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_draw_mesh_indirect,
#endif
};
void __trace_intel_begin_draw_mesh_indirect(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_draw_mesh_indirect entry;
   UNUSED struct trace_intel_begin_draw_mesh_indirect *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_draw_mesh_indirect *)u_trace_appendv(ut, NULL, &__tp_intel_begin_draw_mesh_indirect,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_draw_mesh_indirect
 */
static void __print_intel_end_draw_mesh_indirect(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_mesh_indirect *__entry =
      (const struct trace_intel_end_draw_mesh_indirect *)arg;
   fprintf(out, ""
      "draw_count=%u, "
         "\n"
   ,__entry->draw_count
   );
}

static void __print_json_intel_end_draw_mesh_indirect(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_draw_mesh_indirect *__entry =
      (const struct trace_intel_end_draw_mesh_indirect *)arg;
   fprintf(out, ""
      "\"draw_count\": \"%u\""
   ,__entry->draw_count
   );
}

static const struct u_tracepoint __tp_intel_end_draw_mesh_indirect = {
    "intel_end_draw_mesh_indirect",
    ALIGN_POT(sizeof(struct trace_intel_end_draw_mesh_indirect), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    51,
    __print_intel_end_draw_mesh_indirect,
    __print_json_intel_end_draw_mesh_indirect,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_draw_mesh_indirect,
#endif
};
void __trace_intel_end_draw_mesh_indirect(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t draw_count
) {
   struct trace_intel_end_draw_mesh_indirect entry;
   UNUSED struct trace_intel_end_draw_mesh_indirect *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_draw_mesh_indirect *)u_trace_appendv(ut, NULL, &__tp_intel_end_draw_mesh_indirect,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->draw_count = draw_count;
}

/*
 * intel_begin_draw_mesh_indirect_count
 */
#define __print_intel_begin_draw_mesh_indirect_count NULL
#define __print_json_intel_begin_draw_mesh_indirect_count NULL
static const struct u_tracepoint __tp_intel_begin_draw_mesh_indirect_count = {
    "intel_begin_draw_mesh_indirect_count",
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_mesh_indirect_count), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    52,
    __print_intel_begin_draw_mesh_indirect_count,
    __print_json_intel_begin_draw_mesh_indirect_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_draw_mesh_indirect_count,
#endif
};
void __trace_intel_begin_draw_mesh_indirect_count(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_draw_mesh_indirect_count entry;
   UNUSED struct trace_intel_begin_draw_mesh_indirect_count *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_draw_mesh_indirect_count *)u_trace_appendv(ut, NULL, &__tp_intel_begin_draw_mesh_indirect_count,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_draw_mesh_indirect_count
 */
static void __print_intel_end_draw_mesh_indirect_count(FILE *out, const void *arg, const void *indirect) {
   const uint32_t *__draw_count = (const uint32_t *) ((char *)indirect + 0);
   fprintf(out, ""
      "draw_count=%u, "
         "\n"
   ,*__draw_count
   );
}

static void __print_json_intel_end_draw_mesh_indirect_count(FILE *out, const void *arg, const void *indirect) {
   const uint32_t *__draw_count = (const uint32_t *) ((char *)indirect + 0);
   fprintf(out, ""
      "\"draw_count\": \"%u\""
   ,*__draw_count
   );
}

static const struct u_tracepoint __tp_intel_end_draw_mesh_indirect_count = {
    "intel_end_draw_mesh_indirect_count",
    ALIGN_POT(sizeof(struct trace_intel_end_draw_mesh_indirect_count), 8),   /* keep size 64b aligned */
    0
    + sizeof(uint32_t)
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    53,
    __print_intel_end_draw_mesh_indirect_count,
    __print_json_intel_end_draw_mesh_indirect_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_draw_mesh_indirect_count,
#endif
};
void __trace_intel_end_draw_mesh_indirect_count(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , struct u_trace_address draw_count
) {
   struct trace_intel_end_draw_mesh_indirect_count entry;
   struct u_trace_address indirects[] = {
      draw_count,
   };
   uint8_t indirect_sizes[] = {
      sizeof(uint32_t),
   };
   UNUSED struct trace_intel_end_draw_mesh_indirect_count *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_draw_mesh_indirect_count *)u_trace_appendv(ut, NULL, &__tp_intel_end_draw_mesh_indirect_count,
                                                    0
                                                    ,
                                                    ARRAY_SIZE(indirects), indirects, indirect_sizes
                                                    ) :
      &entry;
}

/*
 * intel_begin_compute
 */
#define __print_intel_begin_compute NULL
#define __print_json_intel_begin_compute NULL
static const struct u_tracepoint __tp_intel_begin_compute = {
    "intel_begin_compute",
    ALIGN_POT(sizeof(struct trace_intel_begin_compute), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    54,
    __print_intel_begin_compute,
    __print_json_intel_begin_compute,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_compute,
#endif
};
void __trace_intel_begin_compute(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_compute entry;
   UNUSED struct trace_intel_begin_compute *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_compute *)u_trace_appendv(ut, NULL, &__tp_intel_begin_compute,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_compute
 */
static void __print_intel_end_compute(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_compute *__entry =
      (const struct trace_intel_end_compute *)arg;
   fprintf(out, ""
      "group_x=%u, "
      "group_y=%u, "
      "group_z=%u, "
      "cs_hash=%#x, "
         "\n"
   ,__entry->group_x
   ,__entry->group_y
   ,__entry->group_z
   ,__entry->cs_hash
   );
}

static void __print_json_intel_end_compute(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_compute *__entry =
      (const struct trace_intel_end_compute *)arg;
   fprintf(out, ""
      "\"group_x\": \"%u\""
         ", "
      "\"group_y\": \"%u\""
         ", "
      "\"group_z\": \"%u\""
         ", "
      "\"cs_hash\": \"%#x\""
   ,__entry->group_x
   ,__entry->group_y
   ,__entry->group_z
   ,__entry->cs_hash
   );
}

static const struct u_tracepoint __tp_intel_end_compute = {
    "intel_end_compute",
    ALIGN_POT(sizeof(struct trace_intel_end_compute), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_CS,
    55,
    __print_intel_end_compute,
    __print_json_intel_end_compute,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_compute,
#endif
};
void __trace_intel_end_compute(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t group_x
   , uint32_t group_y
   , uint32_t group_z
   , uint32_t cs_hash
) {
   struct trace_intel_end_compute entry;
   UNUSED struct trace_intel_end_compute *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_compute *)u_trace_appendv(ut, NULL, &__tp_intel_end_compute,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->group_x = group_x;
   __entry->group_y = group_y;
   __entry->group_z = group_z;
   __entry->cs_hash = cs_hash;
}

/*
 * intel_begin_compute_indirect
 */
#define __print_intel_begin_compute_indirect NULL
#define __print_json_intel_begin_compute_indirect NULL
static const struct u_tracepoint __tp_intel_begin_compute_indirect = {
    "intel_begin_compute_indirect",
    ALIGN_POT(sizeof(struct trace_intel_begin_compute_indirect), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    56,
    __print_intel_begin_compute_indirect,
    __print_json_intel_begin_compute_indirect,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_compute_indirect,
#endif
};
void __trace_intel_begin_compute_indirect(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_compute_indirect entry;
   UNUSED struct trace_intel_begin_compute_indirect *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_compute_indirect *)u_trace_appendv(ut, NULL, &__tp_intel_begin_compute_indirect,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_compute_indirect
 */
static void __print_intel_end_compute_indirect(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_compute_indirect *__entry =
      (const struct trace_intel_end_compute_indirect *)arg;
   const VkDispatchIndirectCommand *__size = (const VkDispatchIndirectCommand *) ((char *)indirect + 0);
   fprintf(out, ""
      "size=%ux%ux%u, "
      "cs_hash=%#x, "
         "\n"
   ,__size->x, __size->y, __size->z
   ,__entry->cs_hash
   );
}

static void __print_json_intel_end_compute_indirect(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_compute_indirect *__entry =
      (const struct trace_intel_end_compute_indirect *)arg;
   const VkDispatchIndirectCommand *__size = (const VkDispatchIndirectCommand *) ((char *)indirect + 0);
   fprintf(out, ""
      "\"size\": \"%ux%ux%u\""
         ", "
      "\"cs_hash\": \"%#x\""
   ,__size->x, __size->y, __size->z
   ,__entry->cs_hash
   );
}

static const struct u_tracepoint __tp_intel_end_compute_indirect = {
    "intel_end_compute_indirect",
    ALIGN_POT(sizeof(struct trace_intel_end_compute_indirect), 8),   /* keep size 64b aligned */
    0
    + sizeof(VkDispatchIndirectCommand)
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_CS,
    57,
    __print_intel_end_compute_indirect,
    __print_json_intel_end_compute_indirect,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_compute_indirect,
#endif
};
void __trace_intel_end_compute_indirect(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , struct u_trace_address size
   , uint32_t cs_hash
) {
   struct trace_intel_end_compute_indirect entry;
   struct u_trace_address indirects[] = {
      size,
   };
   uint8_t indirect_sizes[] = {
      sizeof(VkDispatchIndirectCommand),
   };
   UNUSED struct trace_intel_end_compute_indirect *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_compute_indirect *)u_trace_appendv(ut, NULL, &__tp_intel_end_compute_indirect,
                                                    0
                                                    ,
                                                    ARRAY_SIZE(indirects), indirects, indirect_sizes
                                                    ) :
      &entry;
   __entry->cs_hash = cs_hash;
}

/*
 * intel_begin_trace_copy
 */
#define __print_intel_begin_trace_copy NULL
#define __print_json_intel_begin_trace_copy NULL
static const struct u_tracepoint __tp_intel_begin_trace_copy = {
    "intel_begin_trace_copy",
    ALIGN_POT(sizeof(struct trace_intel_begin_trace_copy), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    58,
    __print_intel_begin_trace_copy,
    __print_json_intel_begin_trace_copy,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_trace_copy,
#endif
};
void __trace_intel_begin_trace_copy(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_trace_copy entry;
   UNUSED struct trace_intel_begin_trace_copy *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_trace_copy *)u_trace_appendv(ut, NULL, &__tp_intel_begin_trace_copy,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_trace_copy
 */
static void __print_intel_end_trace_copy(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_trace_copy *__entry =
      (const struct trace_intel_end_trace_copy *)arg;
   fprintf(out, ""
      "count=%u, "
         "\n"
   ,__entry->count
   );
}

static void __print_json_intel_end_trace_copy(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_trace_copy *__entry =
      (const struct trace_intel_end_trace_copy *)arg;
   fprintf(out, ""
      "\"count\": \"%u\""
   ,__entry->count
   );
}

static const struct u_tracepoint __tp_intel_end_trace_copy = {
    "intel_end_trace_copy",
    ALIGN_POT(sizeof(struct trace_intel_end_trace_copy), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    59,
    __print_intel_end_trace_copy,
    __print_json_intel_end_trace_copy,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_trace_copy,
#endif
};
void __trace_intel_end_trace_copy(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t count
) {
   struct trace_intel_end_trace_copy entry;
   UNUSED struct trace_intel_end_trace_copy *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_trace_copy *)u_trace_appendv(ut, NULL, &__tp_intel_end_trace_copy,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->count = count;
}

/*
 * intel_begin_trace_copy_cb
 */
#define __print_intel_begin_trace_copy_cb NULL
#define __print_json_intel_begin_trace_copy_cb NULL
static const struct u_tracepoint __tp_intel_begin_trace_copy_cb = {
    "intel_begin_trace_copy_cb",
    ALIGN_POT(sizeof(struct trace_intel_begin_trace_copy_cb), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    60,
    __print_intel_begin_trace_copy_cb,
    __print_json_intel_begin_trace_copy_cb,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_trace_copy_cb,
#endif
};
void __trace_intel_begin_trace_copy_cb(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , void *cs
) {
   struct trace_intel_begin_trace_copy_cb entry;
   UNUSED struct trace_intel_begin_trace_copy_cb *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_trace_copy_cb *)u_trace_appendv(ut, cs, &__tp_intel_begin_trace_copy_cb,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_trace_copy_cb
 */
static void __print_intel_end_trace_copy_cb(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_trace_copy_cb *__entry =
      (const struct trace_intel_end_trace_copy_cb *)arg;
   fprintf(out, ""
      "count=%u, "
         "\n"
   ,__entry->count
   );
}

static void __print_json_intel_end_trace_copy_cb(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_trace_copy_cb *__entry =
      (const struct trace_intel_end_trace_copy_cb *)arg;
   fprintf(out, ""
      "\"count\": \"%u\""
   ,__entry->count
   );
}

static const struct u_tracepoint __tp_intel_end_trace_copy_cb = {
    "intel_end_trace_copy_cb",
    ALIGN_POT(sizeof(struct trace_intel_end_trace_copy_cb), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    61,
    __print_intel_end_trace_copy_cb,
    __print_json_intel_end_trace_copy_cb,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_trace_copy_cb,
#endif
};
void __trace_intel_end_trace_copy_cb(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , void *cs
   , uint32_t count
) {
   struct trace_intel_end_trace_copy_cb entry;
   UNUSED struct trace_intel_end_trace_copy_cb *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_trace_copy_cb *)u_trace_appendv(ut, cs, &__tp_intel_end_trace_copy_cb,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->count = count;
}

/*
 * intel_begin_as_build
 */
#define __print_intel_begin_as_build NULL
#define __print_json_intel_begin_as_build NULL
static const struct u_tracepoint __tp_intel_begin_as_build = {
    "intel_begin_as_build",
    ALIGN_POT(sizeof(struct trace_intel_begin_as_build), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    62,
    __print_intel_begin_as_build,
    __print_json_intel_begin_as_build,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_as_build,
#endif
};
void __trace_intel_begin_as_build(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_as_build entry;
   UNUSED struct trace_intel_begin_as_build *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_as_build *)u_trace_appendv(ut, NULL, &__tp_intel_begin_as_build,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_as_build
 */
#define __print_intel_end_as_build NULL
#define __print_json_intel_end_as_build NULL
static const struct u_tracepoint __tp_intel_end_as_build = {
    "intel_end_as_build",
    ALIGN_POT(sizeof(struct trace_intel_end_as_build), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_OF_PIPE,
    63,
    __print_intel_end_as_build,
    __print_json_intel_end_as_build,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_as_build,
#endif
};
void __trace_intel_end_as_build(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_end_as_build entry;
   UNUSED struct trace_intel_end_as_build *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_as_build *)u_trace_appendv(ut, NULL, &__tp_intel_end_as_build,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_begin_as_build_leaves
 */
#define __print_intel_begin_as_build_leaves NULL
#define __print_json_intel_begin_as_build_leaves NULL
static const struct u_tracepoint __tp_intel_begin_as_build_leaves = {
    "intel_begin_as_build_leaves",
    ALIGN_POT(sizeof(struct trace_intel_begin_as_build_leaves), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    64,
    __print_intel_begin_as_build_leaves,
    __print_json_intel_begin_as_build_leaves,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_as_build_leaves,
#endif
};
void __trace_intel_begin_as_build_leaves(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_as_build_leaves entry;
   UNUSED struct trace_intel_begin_as_build_leaves *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_as_build_leaves *)u_trace_appendv(ut, NULL, &__tp_intel_begin_as_build_leaves,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_as_build_leaves
 */
#define __print_intel_end_as_build_leaves NULL
#define __print_json_intel_end_as_build_leaves NULL
static const struct u_tracepoint __tp_intel_end_as_build_leaves = {
    "intel_end_as_build_leaves",
    ALIGN_POT(sizeof(struct trace_intel_end_as_build_leaves), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_CS_OR_NOOP,
    65,
    __print_intel_end_as_build_leaves,
    __print_json_intel_end_as_build_leaves,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_as_build_leaves,
#endif
};
void __trace_intel_end_as_build_leaves(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_end_as_build_leaves entry;
   UNUSED struct trace_intel_end_as_build_leaves *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_as_build_leaves *)u_trace_appendv(ut, NULL, &__tp_intel_end_as_build_leaves,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_begin_as_morton_generate
 */
#define __print_intel_begin_as_morton_generate NULL
#define __print_json_intel_begin_as_morton_generate NULL
static const struct u_tracepoint __tp_intel_begin_as_morton_generate = {
    "intel_begin_as_morton_generate",
    ALIGN_POT(sizeof(struct trace_intel_begin_as_morton_generate), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    66,
    __print_intel_begin_as_morton_generate,
    __print_json_intel_begin_as_morton_generate,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_as_morton_generate,
#endif
};
void __trace_intel_begin_as_morton_generate(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_as_morton_generate entry;
   UNUSED struct trace_intel_begin_as_morton_generate *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_as_morton_generate *)u_trace_appendv(ut, NULL, &__tp_intel_begin_as_morton_generate,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_as_morton_generate
 */
#define __print_intel_end_as_morton_generate NULL
#define __print_json_intel_end_as_morton_generate NULL
static const struct u_tracepoint __tp_intel_end_as_morton_generate = {
    "intel_end_as_morton_generate",
    ALIGN_POT(sizeof(struct trace_intel_end_as_morton_generate), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_CS_OR_NOOP,
    67,
    __print_intel_end_as_morton_generate,
    __print_json_intel_end_as_morton_generate,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_as_morton_generate,
#endif
};
void __trace_intel_end_as_morton_generate(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_end_as_morton_generate entry;
   UNUSED struct trace_intel_end_as_morton_generate *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_as_morton_generate *)u_trace_appendv(ut, NULL, &__tp_intel_end_as_morton_generate,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_begin_as_morton_sort
 */
#define __print_intel_begin_as_morton_sort NULL
#define __print_json_intel_begin_as_morton_sort NULL
static const struct u_tracepoint __tp_intel_begin_as_morton_sort = {
    "intel_begin_as_morton_sort",
    ALIGN_POT(sizeof(struct trace_intel_begin_as_morton_sort), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    68,
    __print_intel_begin_as_morton_sort,
    __print_json_intel_begin_as_morton_sort,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_as_morton_sort,
#endif
};
void __trace_intel_begin_as_morton_sort(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_as_morton_sort entry;
   UNUSED struct trace_intel_begin_as_morton_sort *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_as_morton_sort *)u_trace_appendv(ut, NULL, &__tp_intel_begin_as_morton_sort,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_as_morton_sort
 */
#define __print_intel_end_as_morton_sort NULL
#define __print_json_intel_end_as_morton_sort NULL
static const struct u_tracepoint __tp_intel_end_as_morton_sort = {
    "intel_end_as_morton_sort",
    ALIGN_POT(sizeof(struct trace_intel_end_as_morton_sort), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_CS_OR_NOOP,
    69,
    __print_intel_end_as_morton_sort,
    __print_json_intel_end_as_morton_sort,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_as_morton_sort,
#endif
};
void __trace_intel_end_as_morton_sort(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_end_as_morton_sort entry;
   UNUSED struct trace_intel_end_as_morton_sort *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_as_morton_sort *)u_trace_appendv(ut, NULL, &__tp_intel_end_as_morton_sort,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_begin_as_lbvh_build_internal
 */
#define __print_intel_begin_as_lbvh_build_internal NULL
#define __print_json_intel_begin_as_lbvh_build_internal NULL
static const struct u_tracepoint __tp_intel_begin_as_lbvh_build_internal = {
    "intel_begin_as_lbvh_build_internal",
    ALIGN_POT(sizeof(struct trace_intel_begin_as_lbvh_build_internal), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    70,
    __print_intel_begin_as_lbvh_build_internal,
    __print_json_intel_begin_as_lbvh_build_internal,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_as_lbvh_build_internal,
#endif
};
void __trace_intel_begin_as_lbvh_build_internal(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_as_lbvh_build_internal entry;
   UNUSED struct trace_intel_begin_as_lbvh_build_internal *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_as_lbvh_build_internal *)u_trace_appendv(ut, NULL, &__tp_intel_begin_as_lbvh_build_internal,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_as_lbvh_build_internal
 */
#define __print_intel_end_as_lbvh_build_internal NULL
#define __print_json_intel_end_as_lbvh_build_internal NULL
static const struct u_tracepoint __tp_intel_end_as_lbvh_build_internal = {
    "intel_end_as_lbvh_build_internal",
    ALIGN_POT(sizeof(struct trace_intel_end_as_lbvh_build_internal), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_CS_OR_NOOP,
    71,
    __print_intel_end_as_lbvh_build_internal,
    __print_json_intel_end_as_lbvh_build_internal,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_as_lbvh_build_internal,
#endif
};
void __trace_intel_end_as_lbvh_build_internal(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_end_as_lbvh_build_internal entry;
   UNUSED struct trace_intel_end_as_lbvh_build_internal *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_as_lbvh_build_internal *)u_trace_appendv(ut, NULL, &__tp_intel_end_as_lbvh_build_internal,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_begin_as_ploc_build_internal
 */
#define __print_intel_begin_as_ploc_build_internal NULL
#define __print_json_intel_begin_as_ploc_build_internal NULL
static const struct u_tracepoint __tp_intel_begin_as_ploc_build_internal = {
    "intel_begin_as_ploc_build_internal",
    ALIGN_POT(sizeof(struct trace_intel_begin_as_ploc_build_internal), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    72,
    __print_intel_begin_as_ploc_build_internal,
    __print_json_intel_begin_as_ploc_build_internal,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_as_ploc_build_internal,
#endif
};
void __trace_intel_begin_as_ploc_build_internal(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_as_ploc_build_internal entry;
   UNUSED struct trace_intel_begin_as_ploc_build_internal *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_as_ploc_build_internal *)u_trace_appendv(ut, NULL, &__tp_intel_begin_as_ploc_build_internal,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_as_ploc_build_internal
 */
#define __print_intel_end_as_ploc_build_internal NULL
#define __print_json_intel_end_as_ploc_build_internal NULL
static const struct u_tracepoint __tp_intel_end_as_ploc_build_internal = {
    "intel_end_as_ploc_build_internal",
    ALIGN_POT(sizeof(struct trace_intel_end_as_ploc_build_internal), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_CS_OR_NOOP,
    73,
    __print_intel_end_as_ploc_build_internal,
    __print_json_intel_end_as_ploc_build_internal,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_as_ploc_build_internal,
#endif
};
void __trace_intel_end_as_ploc_build_internal(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_end_as_ploc_build_internal entry;
   UNUSED struct trace_intel_end_as_ploc_build_internal *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_as_ploc_build_internal *)u_trace_appendv(ut, NULL, &__tp_intel_end_as_ploc_build_internal,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_begin_as_encode
 */
#define __print_intel_begin_as_encode NULL
#define __print_json_intel_begin_as_encode NULL
static const struct u_tracepoint __tp_intel_begin_as_encode = {
    "intel_begin_as_encode",
    ALIGN_POT(sizeof(struct trace_intel_begin_as_encode), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    74,
    __print_intel_begin_as_encode,
    __print_json_intel_begin_as_encode,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_as_encode,
#endif
};
void __trace_intel_begin_as_encode(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_as_encode entry;
   UNUSED struct trace_intel_begin_as_encode *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_as_encode *)u_trace_appendv(ut, NULL, &__tp_intel_begin_as_encode,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_as_encode
 */
#define __print_intel_end_as_encode NULL
#define __print_json_intel_end_as_encode NULL
static const struct u_tracepoint __tp_intel_end_as_encode = {
    "intel_end_as_encode",
    ALIGN_POT(sizeof(struct trace_intel_end_as_encode), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_CS_OR_NOOP,
    75,
    __print_intel_end_as_encode,
    __print_json_intel_end_as_encode,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_as_encode,
#endif
};
void __trace_intel_end_as_encode(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_end_as_encode entry;
   UNUSED struct trace_intel_end_as_encode *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_as_encode *)u_trace_appendv(ut, NULL, &__tp_intel_end_as_encode,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_begin_as_copy
 */
#define __print_intel_begin_as_copy NULL
#define __print_json_intel_begin_as_copy NULL
static const struct u_tracepoint __tp_intel_begin_as_copy = {
    "intel_begin_as_copy",
    ALIGN_POT(sizeof(struct trace_intel_begin_as_copy), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    76,
    __print_intel_begin_as_copy,
    __print_json_intel_begin_as_copy,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_as_copy,
#endif
};
void __trace_intel_begin_as_copy(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_as_copy entry;
   UNUSED struct trace_intel_begin_as_copy *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_as_copy *)u_trace_appendv(ut, NULL, &__tp_intel_begin_as_copy,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_as_copy
 */
#define __print_intel_end_as_copy NULL
#define __print_json_intel_end_as_copy NULL
static const struct u_tracepoint __tp_intel_end_as_copy = {
    "intel_end_as_copy",
    ALIGN_POT(sizeof(struct trace_intel_end_as_copy), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_CS_OR_NOOP,
    77,
    __print_intel_end_as_copy,
    __print_json_intel_end_as_copy,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_as_copy,
#endif
};
void __trace_intel_end_as_copy(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_end_as_copy entry;
   UNUSED struct trace_intel_end_as_copy *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_as_copy *)u_trace_appendv(ut, NULL, &__tp_intel_end_as_copy,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_begin_rays
 */
#define __print_intel_begin_rays NULL
#define __print_json_intel_begin_rays NULL
static const struct u_tracepoint __tp_intel_begin_rays = {
    "intel_begin_rays",
    ALIGN_POT(sizeof(struct trace_intel_begin_rays), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    78,
    __print_intel_begin_rays,
    __print_json_intel_begin_rays,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_rays,
#endif
};
void __trace_intel_begin_rays(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_rays entry;
   UNUSED struct trace_intel_begin_rays *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_rays *)u_trace_appendv(ut, NULL, &__tp_intel_begin_rays,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_rays
 */
static void __print_intel_end_rays(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_rays *__entry =
      (const struct trace_intel_end_rays *)arg;
   fprintf(out, ""
      "group_x=%u, "
      "group_y=%u, "
      "group_z=%u, "
         "\n"
   ,__entry->group_x
   ,__entry->group_y
   ,__entry->group_z
   );
}

static void __print_json_intel_end_rays(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_rays *__entry =
      (const struct trace_intel_end_rays *)arg;
   fprintf(out, ""
      "\"group_x\": \"%u\""
         ", "
      "\"group_y\": \"%u\""
         ", "
      "\"group_z\": \"%u\""
   ,__entry->group_x
   ,__entry->group_y
   ,__entry->group_z
   );
}

static const struct u_tracepoint __tp_intel_end_rays = {
    "intel_end_rays",
    ALIGN_POT(sizeof(struct trace_intel_end_rays), 8),   /* keep size 64b aligned */
    0
    ,
    INTEL_DS_TRACEPOINT_FLAG_END_CS,
    79,
    __print_intel_end_rays,
    __print_json_intel_end_rays,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_rays,
#endif
};
void __trace_intel_end_rays(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t group_x
   , uint32_t group_y
   , uint32_t group_z
) {
   struct trace_intel_end_rays entry;
   UNUSED struct trace_intel_end_rays *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_rays *)u_trace_appendv(ut, NULL, &__tp_intel_end_rays,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->group_x = group_x;
   __entry->group_y = group_y;
   __entry->group_z = group_z;
}

/*
 * intel_begin_stall
 */
#define __print_intel_begin_stall NULL
#define __print_json_intel_begin_stall NULL
static const struct u_tracepoint __tp_intel_begin_stall = {
    "intel_begin_stall",
    ALIGN_POT(sizeof(struct trace_intel_begin_stall), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    80,
    __print_intel_begin_stall,
    __print_json_intel_begin_stall,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_begin_stall,
#endif
};
void __trace_intel_begin_stall(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_intel_begin_stall entry;
   UNUSED struct trace_intel_begin_stall *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_begin_stall *)u_trace_appendv(ut, NULL, &__tp_intel_begin_stall,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
}

/*
 * intel_end_stall
 */
static void __print_intel_end_stall(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_stall *__entry =
      (const struct trace_intel_end_stall *)arg;
   fprintf(out, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s : %s%s%s%s%s%s%s\n"
           , (__entry->flags & INTEL_DS_DEPTH_CACHE_FLUSH_BIT) ? "+depth_flush" : ""
           , (__entry->flags & INTEL_DS_DATA_CACHE_FLUSH_BIT) ? "+dc_flush" : ""
           , (__entry->flags & INTEL_DS_HDC_PIPELINE_FLUSH_BIT) ? "+hdc_flush" : ""
           , (__entry->flags & INTEL_DS_RENDER_TARGET_CACHE_FLUSH_BIT) ? "+rt_flush" : ""
           , (__entry->flags & INTEL_DS_TILE_CACHE_FLUSH_BIT) ? "+tile_flush" : ""
           , (__entry->flags & INTEL_DS_STATE_CACHE_INVALIDATE_BIT) ? "+state_inval" : ""
           , (__entry->flags & INTEL_DS_CONST_CACHE_INVALIDATE_BIT) ? "+const_inval" : ""
           , (__entry->flags & INTEL_DS_VF_CACHE_INVALIDATE_BIT) ? "+vf_inval" : ""
           , (__entry->flags & INTEL_DS_TEXTURE_CACHE_INVALIDATE_BIT) ? "+tex_inval" : ""
           , (__entry->flags & INTEL_DS_INST_CACHE_INVALIDATE_BIT) ? "+ic_inval" : ""
           , (__entry->flags & INTEL_DS_STALL_AT_SCOREBOARD_BIT) ? "+pb_stall" : ""
           , (__entry->flags & INTEL_DS_DEPTH_STALL_BIT) ? "+depth_stall" : ""
           , (__entry->flags & INTEL_DS_CS_STALL_BIT) ? "+cs_stall" : ""
           , (__entry->flags & INTEL_DS_UNTYPED_DATAPORT_CACHE_FLUSH_BIT) ? "+udp_flush" : ""
           , (__entry->flags & INTEL_DS_PSS_STALL_SYNC_BIT) ? "+pss_stall" : ""
           , (__entry->flags & INTEL_DS_END_OF_PIPE_BIT) ? "+eop" : ""
           , (__entry->flags & INTEL_DS_CCS_CACHE_FLUSH_BIT) ? "+ccs_flush" : ""
           , (__entry->reason1) ? __entry->reason1 : "unknown"
           , (__entry->reason2) ? "; " : ""
           , (__entry->reason2) ? __entry->reason2 : ""
           , (__entry->reason3) ? "; " : ""
           , (__entry->reason3) ? __entry->reason3 : ""
           , (__entry->reason4) ? "; " : ""
           , (__entry->reason4) ? __entry->reason4 : ""
   );
}

static void __print_json_intel_end_stall(FILE *out, const void *arg, const void *indirect) {
   const struct trace_intel_end_stall *__entry =
      (const struct trace_intel_end_stall *)arg;
   fprintf(out, "\"unstructured\": \"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s : %s%s%s%s%s%s%s\""
           , (__entry->flags & INTEL_DS_DEPTH_CACHE_FLUSH_BIT) ? "+depth_flush" : ""
           , (__entry->flags & INTEL_DS_DATA_CACHE_FLUSH_BIT) ? "+dc_flush" : ""
           , (__entry->flags & INTEL_DS_HDC_PIPELINE_FLUSH_BIT) ? "+hdc_flush" : ""
           , (__entry->flags & INTEL_DS_RENDER_TARGET_CACHE_FLUSH_BIT) ? "+rt_flush" : ""
           , (__entry->flags & INTEL_DS_TILE_CACHE_FLUSH_BIT) ? "+tile_flush" : ""
           , (__entry->flags & INTEL_DS_STATE_CACHE_INVALIDATE_BIT) ? "+state_inval" : ""
           , (__entry->flags & INTEL_DS_CONST_CACHE_INVALIDATE_BIT) ? "+const_inval" : ""
           , (__entry->flags & INTEL_DS_VF_CACHE_INVALIDATE_BIT) ? "+vf_inval" : ""
           , (__entry->flags & INTEL_DS_TEXTURE_CACHE_INVALIDATE_BIT) ? "+tex_inval" : ""
           , (__entry->flags & INTEL_DS_INST_CACHE_INVALIDATE_BIT) ? "+ic_inval" : ""
           , (__entry->flags & INTEL_DS_STALL_AT_SCOREBOARD_BIT) ? "+pb_stall" : ""
           , (__entry->flags & INTEL_DS_DEPTH_STALL_BIT) ? "+depth_stall" : ""
           , (__entry->flags & INTEL_DS_CS_STALL_BIT) ? "+cs_stall" : ""
           , (__entry->flags & INTEL_DS_UNTYPED_DATAPORT_CACHE_FLUSH_BIT) ? "+udp_flush" : ""
           , (__entry->flags & INTEL_DS_PSS_STALL_SYNC_BIT) ? "+pss_stall" : ""
           , (__entry->flags & INTEL_DS_END_OF_PIPE_BIT) ? "+eop" : ""
           , (__entry->flags & INTEL_DS_CCS_CACHE_FLUSH_BIT) ? "+ccs_flush" : ""
           , (__entry->reason1) ? __entry->reason1 : "unknown"
           , (__entry->reason2) ? "; " : ""
           , (__entry->reason2) ? __entry->reason2 : ""
           , (__entry->reason3) ? "; " : ""
           , (__entry->reason3) ? __entry->reason3 : ""
           , (__entry->reason4) ? "; " : ""
           , (__entry->reason4) ? __entry->reason4 : ""
   );
}

static const struct u_tracepoint __tp_intel_end_stall = {
    "intel_end_stall",
    ALIGN_POT(sizeof(struct trace_intel_end_stall), 8),   /* keep size 64b aligned */
    0
    ,
    0,
    81,
    __print_intel_end_stall,
    __print_json_intel_end_stall,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *, const void *))intel_ds_end_stall,
#endif
};
void __trace_intel_end_stall(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t flags
   , intel_ds_stall_cb_t decode_cb
   , const char * reason1
   , const char * reason2
   , const char * reason3
   , const char * reason4
) {
   struct trace_intel_end_stall entry;
   UNUSED struct trace_intel_end_stall *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_intel_end_stall *)u_trace_appendv(ut, NULL, &__tp_intel_end_stall,
                                                    0
                                                    ,
                                                    0, NULL, NULL
                                                    ) :
      &entry;
   __entry->flags = decode_cb(flags);
   __entry->reason1 = reason1;
   __entry->reason2 = reason2;
   __entry->reason3 = reason3;
   __entry->reason4 = reason4;
}

