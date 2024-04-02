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

#include "si_tracepoints.h"


#define __NEEDS_TRACE_PRIV
#include "util/u_debug.h"
#include "util/perf/u_trace_priv.h"

static const struct debug_control config_control[] = {
   { "draw", SI_GPU_TRACEPOINT_DRAW, },
   { "compute", SI_GPU_TRACEPOINT_COMPUTE, },
   { NULL, 0, },
};
uint64_t si_gpu_tracepoint = 0;

static void
si_gpu_tracepoint_variable_once(void)
{
   uint64_t default_value = 0
     | SI_GPU_TRACEPOINT_DRAW
     | SI_GPU_TRACEPOINT_COMPUTE
     ;

   si_gpu_tracepoint =
      parse_enable_string(getenv("SI_GPU_TRACEPOINT"),
                          default_value,
                          config_control);
}

void
si_gpu_tracepoint_config_variable(void)
{
   static once_flag process_si_gpu_tracepoint_variable_flag = ONCE_FLAG_INIT;

   call_once(&process_si_gpu_tracepoint_variable_flag,
             si_gpu_tracepoint_variable_once);
}

/*
 * si_begin_draw
 */
#define __print_si_begin_draw NULL
#define __print_json_si_begin_draw NULL
static const struct u_tracepoint __tp_si_begin_draw = {
    ALIGN_POT(sizeof(struct trace_si_begin_draw), 8),   /* keep size 64b aligned */
    "si_begin_draw",
    false,
    0,
    __print_si_begin_draw,
    __print_json_si_begin_draw,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *))si_ds_begin_draw,
#endif
};
void __trace_si_begin_draw(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_si_begin_draw entry;
   UNUSED struct trace_si_begin_draw *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_si_begin_draw *)u_trace_append(ut, NULL, &__tp_si_begin_draw) :
      &entry;
}

/*
 * si_end_draw
 */
static void __print_si_end_draw(FILE *out, const void *arg) {
   const struct trace_si_end_draw *__entry =
      (const struct trace_si_end_draw *)arg;
   fprintf(out, ""
      "count=%u, "
         "\n"
   ,__entry->count
   );
}

static void __print_json_si_end_draw(FILE *out, const void *arg) {
   const struct trace_si_end_draw *__entry =
      (const struct trace_si_end_draw *)arg;
   fprintf(out, ""
      "\"count\": \"%u\""
   ,__entry->count
   );
}

static const struct u_tracepoint __tp_si_end_draw = {
    ALIGN_POT(sizeof(struct trace_si_end_draw), 8),   /* keep size 64b aligned */
    "si_end_draw",
    true,
    1,
    __print_si_end_draw,
    __print_json_si_end_draw,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *))si_ds_end_draw,
#endif
};
void __trace_si_end_draw(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t count
) {
   struct trace_si_end_draw entry;
   UNUSED struct trace_si_end_draw *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_si_end_draw *)u_trace_append(ut, NULL, &__tp_si_end_draw) :
      &entry;
   __entry->count = count;
}

/*
 * si_begin_compute
 */
#define __print_si_begin_compute NULL
#define __print_json_si_begin_compute NULL
static const struct u_tracepoint __tp_si_begin_compute = {
    ALIGN_POT(sizeof(struct trace_si_begin_compute), 8),   /* keep size 64b aligned */
    "si_begin_compute",
    false,
    2,
    __print_si_begin_compute,
    __print_json_si_begin_compute,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *))si_ds_begin_compute,
#endif
};
void __trace_si_begin_compute(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
) {
   struct trace_si_begin_compute entry;
   UNUSED struct trace_si_begin_compute *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_si_begin_compute *)u_trace_append(ut, NULL, &__tp_si_begin_compute) :
      &entry;
}

/*
 * si_end_compute
 */
static void __print_si_end_compute(FILE *out, const void *arg) {
   const struct trace_si_end_compute *__entry =
      (const struct trace_si_end_compute *)arg;
   fprintf(out, "group=%ux%ux%u\n"
           , __entry->group_x
           , __entry->group_y
           , __entry->group_z
   );
}

static void __print_json_si_end_compute(FILE *out, const void *arg) {
   const struct trace_si_end_compute *__entry =
      (const struct trace_si_end_compute *)arg;
   fprintf(out, "\"unstructured\": \"group=%ux%ux%u\""
           , __entry->group_x
           , __entry->group_y
           , __entry->group_z
   );
}

static const struct u_tracepoint __tp_si_end_compute = {
    ALIGN_POT(sizeof(struct trace_si_end_compute), 8),   /* keep size 64b aligned */
    "si_end_compute",
    true,
    3,
    __print_si_end_compute,
    __print_json_si_end_compute,
#ifdef HAVE_PERFETTO
    (void (*)(void *pctx, uint64_t, uint16_t, const void *, const void *))si_ds_end_compute,
#endif
};
void __trace_si_end_compute(
     struct u_trace *ut
   , enum u_trace_type enabled_traces
   , uint32_t group_x
   , uint32_t group_y
   , uint32_t group_z
) {
   struct trace_si_end_compute entry;
   UNUSED struct trace_si_end_compute *__entry =
      enabled_traces & U_TRACE_TYPE_REQUIRE_QUEUING ?
      (struct trace_si_end_compute *)u_trace_append(ut, NULL, &__tp_si_end_compute) :
      &entry;
   __entry->group_x = group_x;
   __entry->group_y = group_y;
   __entry->group_z = group_z;
}

