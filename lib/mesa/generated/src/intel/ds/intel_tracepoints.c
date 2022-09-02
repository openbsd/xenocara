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

#include "intel_driver_ds.h"

#include "intel_tracepoints.h"

#define __NEEDS_TRACE_PRIV
#include "util/perf/u_trace_priv.h"

/*
 * intel_begin_batch
 */
#define __print_intel_begin_batch NULL
static const struct u_tracepoint __tp_intel_begin_batch = {
    ALIGN_POT(sizeof(struct trace_intel_begin_batch), 8),   /* keep size 64b aligned */
    "intel_begin_batch",
    false,
    __print_intel_begin_batch,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_batch,
#endif
};
void __trace_intel_begin_batch(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_batch *__entry =
      (struct trace_intel_begin_batch *)u_trace_append(ut, cs, &__tp_intel_begin_batch);
   (void)__entry;
}

/*
 * intel_end_batch
 */
static void __print_intel_end_batch(FILE *out, const void *arg) {
   const struct trace_intel_end_batch *__entry =
      (const struct trace_intel_end_batch *)arg;
   fprintf(out, ""
      "name=%hhu, "
         "\n"
   ,__entry->name
   );
}
static const struct u_tracepoint __tp_intel_end_batch = {
    ALIGN_POT(sizeof(struct trace_intel_end_batch), 8),   /* keep size 64b aligned */
    "intel_end_batch",
    false,
    __print_intel_end_batch,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_batch,
#endif
};
void __trace_intel_end_batch(
     struct u_trace *ut, void *cs
   , uint8_t name
) {
   struct trace_intel_end_batch *__entry =
      (struct trace_intel_end_batch *)u_trace_append(ut, cs, &__tp_intel_end_batch);
   __entry->name = name;
}

/*
 * intel_begin_cmd_buffer
 */
#define __print_intel_begin_cmd_buffer NULL
static const struct u_tracepoint __tp_intel_begin_cmd_buffer = {
    ALIGN_POT(sizeof(struct trace_intel_begin_cmd_buffer), 8),   /* keep size 64b aligned */
    "intel_begin_cmd_buffer",
    false,
    __print_intel_begin_cmd_buffer,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_cmd_buffer,
#endif
};
void __trace_intel_begin_cmd_buffer(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_cmd_buffer *__entry =
      (struct trace_intel_begin_cmd_buffer *)u_trace_append(ut, cs, &__tp_intel_begin_cmd_buffer);
   (void)__entry;
}

/*
 * intel_end_cmd_buffer
 */
static void __print_intel_end_cmd_buffer(FILE *out, const void *arg) {
   const struct trace_intel_end_cmd_buffer *__entry =
      (const struct trace_intel_end_cmd_buffer *)arg;
   fprintf(out, ""
      "level=%hhu, "
         "\n"
   ,__entry->level
   );
}
static const struct u_tracepoint __tp_intel_end_cmd_buffer = {
    ALIGN_POT(sizeof(struct trace_intel_end_cmd_buffer), 8),   /* keep size 64b aligned */
    "intel_end_cmd_buffer",
    false,
    __print_intel_end_cmd_buffer,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_cmd_buffer,
#endif
};
void __trace_intel_end_cmd_buffer(
     struct u_trace *ut, void *cs
   , uint8_t level
) {
   struct trace_intel_end_cmd_buffer *__entry =
      (struct trace_intel_end_cmd_buffer *)u_trace_append(ut, cs, &__tp_intel_end_cmd_buffer);
   __entry->level = level;
}

/*
 * intel_begin_render_pass
 */
#define __print_intel_begin_render_pass NULL
static const struct u_tracepoint __tp_intel_begin_render_pass = {
    ALIGN_POT(sizeof(struct trace_intel_begin_render_pass), 8),   /* keep size 64b aligned */
    "intel_begin_render_pass",
    false,
    __print_intel_begin_render_pass,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_render_pass,
#endif
};
void __trace_intel_begin_render_pass(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_render_pass *__entry =
      (struct trace_intel_begin_render_pass *)u_trace_append(ut, cs, &__tp_intel_begin_render_pass);
   (void)__entry;
}

/*
 * intel_end_render_pass
 */
static void __print_intel_end_render_pass(FILE *out, const void *arg) {
   const struct trace_intel_end_render_pass *__entry =
      (const struct trace_intel_end_render_pass *)arg;
   fprintf(out, ""
      "width=%hu, "
      "height=%hu, "
      "att_count=%hhu, "
      "msaa=%hhu, "
      "subpass_count=%u, "
         "\n"
   ,__entry->width
   ,__entry->height
   ,__entry->att_count
   ,__entry->msaa
   ,__entry->subpass_count
   );
}
static const struct u_tracepoint __tp_intel_end_render_pass = {
    ALIGN_POT(sizeof(struct trace_intel_end_render_pass), 8),   /* keep size 64b aligned */
    "intel_end_render_pass",
    true,
    __print_intel_end_render_pass,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_render_pass,
#endif
};
void __trace_intel_end_render_pass(
     struct u_trace *ut, void *cs
   , uint16_t width
   , uint16_t height
   , uint8_t att_count
   , uint8_t msaa
   , uint32_t subpass_count
) {
   struct trace_intel_end_render_pass *__entry =
      (struct trace_intel_end_render_pass *)u_trace_append(ut, cs, &__tp_intel_end_render_pass);
   __entry->width = width;
   __entry->height = height;
   __entry->att_count = att_count;
   __entry->msaa = msaa;
   __entry->subpass_count = subpass_count;
}

/*
 * intel_begin_dyn_render_pass
 */
#define __print_intel_begin_dyn_render_pass NULL
static const struct u_tracepoint __tp_intel_begin_dyn_render_pass = {
    ALIGN_POT(sizeof(struct trace_intel_begin_dyn_render_pass), 8),   /* keep size 64b aligned */
    "intel_begin_dyn_render_pass",
    false,
    __print_intel_begin_dyn_render_pass,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_dyn_render_pass,
#endif
};
void __trace_intel_begin_dyn_render_pass(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_dyn_render_pass *__entry =
      (struct trace_intel_begin_dyn_render_pass *)u_trace_append(ut, cs, &__tp_intel_begin_dyn_render_pass);
   (void)__entry;
}

/*
 * intel_end_dyn_render_pass
 */
static void __print_intel_end_dyn_render_pass(FILE *out, const void *arg) {
   const struct trace_intel_end_dyn_render_pass *__entry =
      (const struct trace_intel_end_dyn_render_pass *)arg;
   fprintf(out, ""
      "width=%hu, "
      "height=%hu, "
      "att_count=%hhu, "
      "msaa=%hhu, "
      "suspend=%hhu, "
      "resume=%hhu, "
         "\n"
   ,__entry->width
   ,__entry->height
   ,__entry->att_count
   ,__entry->msaa
   ,__entry->suspend
   ,__entry->resume
   );
}
static const struct u_tracepoint __tp_intel_end_dyn_render_pass = {
    ALIGN_POT(sizeof(struct trace_intel_end_dyn_render_pass), 8),   /* keep size 64b aligned */
    "intel_end_dyn_render_pass",
    true,
    __print_intel_end_dyn_render_pass,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_dyn_render_pass,
#endif
};
void __trace_intel_end_dyn_render_pass(
     struct u_trace *ut, void *cs
   , uint16_t width
   , uint16_t height
   , uint8_t att_count
   , uint8_t msaa
   , uint8_t suspend
   , uint8_t resume
) {
   struct trace_intel_end_dyn_render_pass *__entry =
      (struct trace_intel_end_dyn_render_pass *)u_trace_append(ut, cs, &__tp_intel_end_dyn_render_pass);
   __entry->width = width;
   __entry->height = height;
   __entry->att_count = att_count;
   __entry->msaa = msaa;
   __entry->suspend = suspend;
   __entry->resume = resume;
}

/*
 * intel_begin_blorp
 */
#define __print_intel_begin_blorp NULL
static const struct u_tracepoint __tp_intel_begin_blorp = {
    ALIGN_POT(sizeof(struct trace_intel_begin_blorp), 8),   /* keep size 64b aligned */
    "intel_begin_blorp",
    false,
    __print_intel_begin_blorp,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_blorp,
#endif
};
void __trace_intel_begin_blorp(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_blorp *__entry =
      (struct trace_intel_begin_blorp *)u_trace_append(ut, cs, &__tp_intel_begin_blorp);
   (void)__entry;
}

/*
 * intel_end_blorp
 */
static void __print_intel_end_blorp(FILE *out, const void *arg) {
   const struct trace_intel_end_blorp *__entry =
      (const struct trace_intel_end_blorp *)arg;
   fprintf(out, ""
      "width=%u, "
      "height=%u, "
      "hiz_op=%s, "
      "fast_clear_op=%s, "
      "blorp_type=%s, "
      "blorp_pipe=%s, "
         "\n"
   ,__entry->width
   ,__entry->height
   ,isl_aux_op_to_name(__entry->hiz_op)
   ,isl_aux_op_to_name(__entry->fast_clear_op)
   ,blorp_shader_type_to_name(__entry->blorp_type)
   ,blorp_shader_pipeline_to_name(__entry->blorp_pipe)
   );
}
static const struct u_tracepoint __tp_intel_end_blorp = {
    ALIGN_POT(sizeof(struct trace_intel_end_blorp), 8),   /* keep size 64b aligned */
    "intel_end_blorp",
    true,
    __print_intel_end_blorp,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_blorp,
#endif
};
void __trace_intel_end_blorp(
     struct u_trace *ut, void *cs
   , uint32_t width
   , uint32_t height
   , enum isl_aux_op hiz_op
   , enum isl_aux_op fast_clear_op
   , enum blorp_shader_type shader_type
   , enum blorp_shader_pipeline shader_pipe
) {
   struct trace_intel_end_blorp *__entry =
      (struct trace_intel_end_blorp *)u_trace_append(ut, cs, &__tp_intel_end_blorp);
   __entry->width = width;
   __entry->height = height;
   __entry->hiz_op = hiz_op;
   __entry->fast_clear_op = fast_clear_op;
   __entry->blorp_type = shader_type;
   __entry->blorp_pipe = shader_pipe;
}

/*
 * intel_begin_draw
 */
#define __print_intel_begin_draw NULL
static const struct u_tracepoint __tp_intel_begin_draw = {
    ALIGN_POT(sizeof(struct trace_intel_begin_draw), 8),   /* keep size 64b aligned */
    "intel_begin_draw",
    false,
    __print_intel_begin_draw,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_draw,
#endif
};
void __trace_intel_begin_draw(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_draw *__entry =
      (struct trace_intel_begin_draw *)u_trace_append(ut, cs, &__tp_intel_begin_draw);
   (void)__entry;
}

/*
 * intel_end_draw
 */
static void __print_intel_end_draw(FILE *out, const void *arg) {
   const struct trace_intel_end_draw *__entry =
      (const struct trace_intel_end_draw *)arg;
   fprintf(out, ""
      "count=%u, "
         "\n"
   ,__entry->count
   );
}
static const struct u_tracepoint __tp_intel_end_draw = {
    ALIGN_POT(sizeof(struct trace_intel_end_draw), 8),   /* keep size 64b aligned */
    "intel_end_draw",
    true,
    __print_intel_end_draw,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_draw,
#endif
};
void __trace_intel_end_draw(
     struct u_trace *ut, void *cs
   , uint32_t count
) {
   struct trace_intel_end_draw *__entry =
      (struct trace_intel_end_draw *)u_trace_append(ut, cs, &__tp_intel_end_draw);
   __entry->count = count;
}

/*
 * intel_begin_draw_multi
 */
#define __print_intel_begin_draw_multi NULL
static const struct u_tracepoint __tp_intel_begin_draw_multi = {
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_multi), 8),   /* keep size 64b aligned */
    "intel_begin_draw_multi",
    false,
    __print_intel_begin_draw_multi,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_draw_multi,
#endif
};
void __trace_intel_begin_draw_multi(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_draw_multi *__entry =
      (struct trace_intel_begin_draw_multi *)u_trace_append(ut, cs, &__tp_intel_begin_draw_multi);
   (void)__entry;
}

/*
 * intel_end_draw_multi
 */
static void __print_intel_end_draw_multi(FILE *out, const void *arg) {
   const struct trace_intel_end_draw_multi *__entry =
      (const struct trace_intel_end_draw_multi *)arg;
   fprintf(out, ""
      "count=%u, "
         "\n"
   ,__entry->count
   );
}
static const struct u_tracepoint __tp_intel_end_draw_multi = {
    ALIGN_POT(sizeof(struct trace_intel_end_draw_multi), 8),   /* keep size 64b aligned */
    "intel_end_draw_multi",
    true,
    __print_intel_end_draw_multi,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_draw_multi,
#endif
};
void __trace_intel_end_draw_multi(
     struct u_trace *ut, void *cs
   , uint32_t count
) {
   struct trace_intel_end_draw_multi *__entry =
      (struct trace_intel_end_draw_multi *)u_trace_append(ut, cs, &__tp_intel_end_draw_multi);
   __entry->count = count;
}

/*
 * intel_begin_draw_indexed
 */
#define __print_intel_begin_draw_indexed NULL
static const struct u_tracepoint __tp_intel_begin_draw_indexed = {
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indexed), 8),   /* keep size 64b aligned */
    "intel_begin_draw_indexed",
    false,
    __print_intel_begin_draw_indexed,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_draw_indexed,
#endif
};
void __trace_intel_begin_draw_indexed(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_draw_indexed *__entry =
      (struct trace_intel_begin_draw_indexed *)u_trace_append(ut, cs, &__tp_intel_begin_draw_indexed);
   (void)__entry;
}

/*
 * intel_end_draw_indexed
 */
static void __print_intel_end_draw_indexed(FILE *out, const void *arg) {
   const struct trace_intel_end_draw_indexed *__entry =
      (const struct trace_intel_end_draw_indexed *)arg;
   fprintf(out, ""
      "count=%u, "
         "\n"
   ,__entry->count
   );
}
static const struct u_tracepoint __tp_intel_end_draw_indexed = {
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indexed), 8),   /* keep size 64b aligned */
    "intel_end_draw_indexed",
    true,
    __print_intel_end_draw_indexed,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_draw_indexed,
#endif
};
void __trace_intel_end_draw_indexed(
     struct u_trace *ut, void *cs
   , uint32_t count
) {
   struct trace_intel_end_draw_indexed *__entry =
      (struct trace_intel_end_draw_indexed *)u_trace_append(ut, cs, &__tp_intel_end_draw_indexed);
   __entry->count = count;
}

/*
 * intel_begin_draw_indexed_multi
 */
#define __print_intel_begin_draw_indexed_multi NULL
static const struct u_tracepoint __tp_intel_begin_draw_indexed_multi = {
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indexed_multi), 8),   /* keep size 64b aligned */
    "intel_begin_draw_indexed_multi",
    false,
    __print_intel_begin_draw_indexed_multi,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_draw_indexed_multi,
#endif
};
void __trace_intel_begin_draw_indexed_multi(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_draw_indexed_multi *__entry =
      (struct trace_intel_begin_draw_indexed_multi *)u_trace_append(ut, cs, &__tp_intel_begin_draw_indexed_multi);
   (void)__entry;
}

/*
 * intel_end_draw_indexed_multi
 */
static void __print_intel_end_draw_indexed_multi(FILE *out, const void *arg) {
   const struct trace_intel_end_draw_indexed_multi *__entry =
      (const struct trace_intel_end_draw_indexed_multi *)arg;
   fprintf(out, ""
      "count=%u, "
         "\n"
   ,__entry->count
   );
}
static const struct u_tracepoint __tp_intel_end_draw_indexed_multi = {
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indexed_multi), 8),   /* keep size 64b aligned */
    "intel_end_draw_indexed_multi",
    true,
    __print_intel_end_draw_indexed_multi,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_draw_indexed_multi,
#endif
};
void __trace_intel_end_draw_indexed_multi(
     struct u_trace *ut, void *cs
   , uint32_t count
) {
   struct trace_intel_end_draw_indexed_multi *__entry =
      (struct trace_intel_end_draw_indexed_multi *)u_trace_append(ut, cs, &__tp_intel_end_draw_indexed_multi);
   __entry->count = count;
}

/*
 * intel_begin_draw_indirect_byte_count
 */
#define __print_intel_begin_draw_indirect_byte_count NULL
static const struct u_tracepoint __tp_intel_begin_draw_indirect_byte_count = {
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indirect_byte_count), 8),   /* keep size 64b aligned */
    "intel_begin_draw_indirect_byte_count",
    false,
    __print_intel_begin_draw_indirect_byte_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_draw_indirect_byte_count,
#endif
};
void __trace_intel_begin_draw_indirect_byte_count(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_draw_indirect_byte_count *__entry =
      (struct trace_intel_begin_draw_indirect_byte_count *)u_trace_append(ut, cs, &__tp_intel_begin_draw_indirect_byte_count);
   (void)__entry;
}

/*
 * intel_end_draw_indirect_byte_count
 */
static void __print_intel_end_draw_indirect_byte_count(FILE *out, const void *arg) {
   const struct trace_intel_end_draw_indirect_byte_count *__entry =
      (const struct trace_intel_end_draw_indirect_byte_count *)arg;
   fprintf(out, ""
      "instance_count=%u, "
         "\n"
   ,__entry->instance_count
   );
}
static const struct u_tracepoint __tp_intel_end_draw_indirect_byte_count = {
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indirect_byte_count), 8),   /* keep size 64b aligned */
    "intel_end_draw_indirect_byte_count",
    true,
    __print_intel_end_draw_indirect_byte_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_draw_indirect_byte_count,
#endif
};
void __trace_intel_end_draw_indirect_byte_count(
     struct u_trace *ut, void *cs
   , uint32_t instance_count
) {
   struct trace_intel_end_draw_indirect_byte_count *__entry =
      (struct trace_intel_end_draw_indirect_byte_count *)u_trace_append(ut, cs, &__tp_intel_end_draw_indirect_byte_count);
   __entry->instance_count = instance_count;
}

/*
 * intel_begin_draw_indirect
 */
#define __print_intel_begin_draw_indirect NULL
static const struct u_tracepoint __tp_intel_begin_draw_indirect = {
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indirect), 8),   /* keep size 64b aligned */
    "intel_begin_draw_indirect",
    false,
    __print_intel_begin_draw_indirect,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_draw_indirect,
#endif
};
void __trace_intel_begin_draw_indirect(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_draw_indirect *__entry =
      (struct trace_intel_begin_draw_indirect *)u_trace_append(ut, cs, &__tp_intel_begin_draw_indirect);
   (void)__entry;
}

/*
 * intel_end_draw_indirect
 */
static void __print_intel_end_draw_indirect(FILE *out, const void *arg) {
   const struct trace_intel_end_draw_indirect *__entry =
      (const struct trace_intel_end_draw_indirect *)arg;
   fprintf(out, ""
      "draw_count=%u, "
         "\n"
   ,__entry->draw_count
   );
}
static const struct u_tracepoint __tp_intel_end_draw_indirect = {
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indirect), 8),   /* keep size 64b aligned */
    "intel_end_draw_indirect",
    true,
    __print_intel_end_draw_indirect,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_draw_indirect,
#endif
};
void __trace_intel_end_draw_indirect(
     struct u_trace *ut, void *cs
   , uint32_t draw_count
) {
   struct trace_intel_end_draw_indirect *__entry =
      (struct trace_intel_end_draw_indirect *)u_trace_append(ut, cs, &__tp_intel_end_draw_indirect);
   __entry->draw_count = draw_count;
}

/*
 * intel_begin_draw_indexed_indirect
 */
#define __print_intel_begin_draw_indexed_indirect NULL
static const struct u_tracepoint __tp_intel_begin_draw_indexed_indirect = {
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indexed_indirect), 8),   /* keep size 64b aligned */
    "intel_begin_draw_indexed_indirect",
    false,
    __print_intel_begin_draw_indexed_indirect,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_draw_indexed_indirect,
#endif
};
void __trace_intel_begin_draw_indexed_indirect(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_draw_indexed_indirect *__entry =
      (struct trace_intel_begin_draw_indexed_indirect *)u_trace_append(ut, cs, &__tp_intel_begin_draw_indexed_indirect);
   (void)__entry;
}

/*
 * intel_end_draw_indexed_indirect
 */
static void __print_intel_end_draw_indexed_indirect(FILE *out, const void *arg) {
   const struct trace_intel_end_draw_indexed_indirect *__entry =
      (const struct trace_intel_end_draw_indexed_indirect *)arg;
   fprintf(out, ""
      "draw_count=%u, "
         "\n"
   ,__entry->draw_count
   );
}
static const struct u_tracepoint __tp_intel_end_draw_indexed_indirect = {
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indexed_indirect), 8),   /* keep size 64b aligned */
    "intel_end_draw_indexed_indirect",
    true,
    __print_intel_end_draw_indexed_indirect,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_draw_indexed_indirect,
#endif
};
void __trace_intel_end_draw_indexed_indirect(
     struct u_trace *ut, void *cs
   , uint32_t draw_count
) {
   struct trace_intel_end_draw_indexed_indirect *__entry =
      (struct trace_intel_end_draw_indexed_indirect *)u_trace_append(ut, cs, &__tp_intel_end_draw_indexed_indirect);
   __entry->draw_count = draw_count;
}

/*
 * intel_begin_draw_indirect_count
 */
#define __print_intel_begin_draw_indirect_count NULL
static const struct u_tracepoint __tp_intel_begin_draw_indirect_count = {
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indirect_count), 8),   /* keep size 64b aligned */
    "intel_begin_draw_indirect_count",
    false,
    __print_intel_begin_draw_indirect_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_draw_indirect_count,
#endif
};
void __trace_intel_begin_draw_indirect_count(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_draw_indirect_count *__entry =
      (struct trace_intel_begin_draw_indirect_count *)u_trace_append(ut, cs, &__tp_intel_begin_draw_indirect_count);
   (void)__entry;
}

/*
 * intel_end_draw_indirect_count
 */
static void __print_intel_end_draw_indirect_count(FILE *out, const void *arg) {
   const struct trace_intel_end_draw_indirect_count *__entry =
      (const struct trace_intel_end_draw_indirect_count *)arg;
   fprintf(out, ""
      "max_draw_count=%u, "
         "\n"
   ,__entry->max_draw_count
   );
}
static const struct u_tracepoint __tp_intel_end_draw_indirect_count = {
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indirect_count), 8),   /* keep size 64b aligned */
    "intel_end_draw_indirect_count",
    true,
    __print_intel_end_draw_indirect_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_draw_indirect_count,
#endif
};
void __trace_intel_end_draw_indirect_count(
     struct u_trace *ut, void *cs
   , uint32_t max_draw_count
) {
   struct trace_intel_end_draw_indirect_count *__entry =
      (struct trace_intel_end_draw_indirect_count *)u_trace_append(ut, cs, &__tp_intel_end_draw_indirect_count);
   __entry->max_draw_count = max_draw_count;
}

/*
 * intel_begin_draw_indexed_indirect_count
 */
#define __print_intel_begin_draw_indexed_indirect_count NULL
static const struct u_tracepoint __tp_intel_begin_draw_indexed_indirect_count = {
    ALIGN_POT(sizeof(struct trace_intel_begin_draw_indexed_indirect_count), 8),   /* keep size 64b aligned */
    "intel_begin_draw_indexed_indirect_count",
    false,
    __print_intel_begin_draw_indexed_indirect_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_draw_indexed_indirect_count,
#endif
};
void __trace_intel_begin_draw_indexed_indirect_count(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_draw_indexed_indirect_count *__entry =
      (struct trace_intel_begin_draw_indexed_indirect_count *)u_trace_append(ut, cs, &__tp_intel_begin_draw_indexed_indirect_count);
   (void)__entry;
}

/*
 * intel_end_draw_indexed_indirect_count
 */
static void __print_intel_end_draw_indexed_indirect_count(FILE *out, const void *arg) {
   const struct trace_intel_end_draw_indexed_indirect_count *__entry =
      (const struct trace_intel_end_draw_indexed_indirect_count *)arg;
   fprintf(out, ""
      "max_draw_count=%u, "
         "\n"
   ,__entry->max_draw_count
   );
}
static const struct u_tracepoint __tp_intel_end_draw_indexed_indirect_count = {
    ALIGN_POT(sizeof(struct trace_intel_end_draw_indexed_indirect_count), 8),   /* keep size 64b aligned */
    "intel_end_draw_indexed_indirect_count",
    true,
    __print_intel_end_draw_indexed_indirect_count,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_draw_indexed_indirect_count,
#endif
};
void __trace_intel_end_draw_indexed_indirect_count(
     struct u_trace *ut, void *cs
   , uint32_t max_draw_count
) {
   struct trace_intel_end_draw_indexed_indirect_count *__entry =
      (struct trace_intel_end_draw_indexed_indirect_count *)u_trace_append(ut, cs, &__tp_intel_end_draw_indexed_indirect_count);
   __entry->max_draw_count = max_draw_count;
}

/*
 * intel_begin_compute
 */
#define __print_intel_begin_compute NULL
static const struct u_tracepoint __tp_intel_begin_compute = {
    ALIGN_POT(sizeof(struct trace_intel_begin_compute), 8),   /* keep size 64b aligned */
    "intel_begin_compute",
    false,
    __print_intel_begin_compute,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_compute,
#endif
};
void __trace_intel_begin_compute(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_compute *__entry =
      (struct trace_intel_begin_compute *)u_trace_append(ut, cs, &__tp_intel_begin_compute);
   (void)__entry;
}

/*
 * intel_end_compute
 */
static void __print_intel_end_compute(FILE *out, const void *arg) {
   const struct trace_intel_end_compute *__entry =
      (const struct trace_intel_end_compute *)arg;
   fprintf(out, "group=%ux%ux%u\n"
           , __entry->group_x
           , __entry->group_y
           , __entry->group_z
   );
}
static const struct u_tracepoint __tp_intel_end_compute = {
    ALIGN_POT(sizeof(struct trace_intel_end_compute), 8),   /* keep size 64b aligned */
    "intel_end_compute",
    true,
    __print_intel_end_compute,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_compute,
#endif
};
void __trace_intel_end_compute(
     struct u_trace *ut, void *cs
   , uint32_t group_x
   , uint32_t group_y
   , uint32_t group_z
) {
   struct trace_intel_end_compute *__entry =
      (struct trace_intel_end_compute *)u_trace_append(ut, cs, &__tp_intel_end_compute);
   __entry->group_x = group_x;
   __entry->group_y = group_y;
   __entry->group_z = group_z;
}

/*
 * intel_begin_stall
 */
#define __print_intel_begin_stall NULL
static const struct u_tracepoint __tp_intel_begin_stall = {
    ALIGN_POT(sizeof(struct trace_intel_begin_stall), 8),   /* keep size 64b aligned */
    "intel_begin_stall",
    false,
    __print_intel_begin_stall,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_begin_stall,
#endif
};
void __trace_intel_begin_stall(
     struct u_trace *ut, void *cs
) {
   struct trace_intel_begin_stall *__entry =
      (struct trace_intel_begin_stall *)u_trace_append(ut, cs, &__tp_intel_begin_stall);
   (void)__entry;
}

/*
 * intel_end_stall
 */
static void __print_intel_end_stall(FILE *out, const void *arg) {
   const struct trace_intel_end_stall *__entry =
      (const struct trace_intel_end_stall *)arg;
   fprintf(out, "%s%s%s%s%s%s%s%s%s%s%s%s%s : %s\n"
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
           , __entry->reason ? __entry->reason : "unknown"
   );
}
static const struct u_tracepoint __tp_intel_end_stall = {
    ALIGN_POT(sizeof(struct trace_intel_end_stall), 8),   /* keep size 64b aligned */
    "intel_end_stall",
    false,
    __print_intel_end_stall,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, const void *, const void *))intel_ds_end_stall,
#endif
};
void __trace_intel_end_stall(
     struct u_trace *ut, void *cs
   , uint32_t flags
   , intel_ds_stall_cb_t decode_cb
   , const char * reason
) {
   struct trace_intel_end_stall *__entry =
      (struct trace_intel_end_stall *)u_trace_append(ut, cs, &__tp_intel_end_stall);
   __entry->flags = decode_cb(flags);
   __entry->reason = reason;
}

