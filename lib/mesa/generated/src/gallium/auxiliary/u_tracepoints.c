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

#include "pipe/p_state.h"
#include "util/format/u_format.h"

#include "u_tracepoints.h"

#define __NEEDS_TRACE_PRIV
#include "util/u_trace_priv.h"

/*
 * surface
 */
struct __payload_surface {
         uint16_t width;
         uint16_t height;
         uint8_t nr_samples;
         const char * format;
};
static void __print_surface(FILE *out, const void *arg) {
   const struct __payload_surface *__entry =
      (const struct __payload_surface *)arg;
   fprintf(out, "%ux%u@%u, fmt=%s\n"
           , __entry->width
           , __entry->height
           , __entry->nr_samples
           , __entry->format
   );
}
static const struct u_tracepoint __tp_surface = {
    ALIGN_POT(sizeof(struct __payload_surface), 8),   /* keep size 64b aligned */
    "surface",
    __print_surface,
};
void __trace_surface(struct u_trace *ut
     , const struct pipe_surface * psurf
) {
   struct __payload_surface *__entry =
      (struct __payload_surface *)u_trace_append(ut, &__tp_surface);
   (void)__entry;
        __entry->width = psurf->width;
        __entry->height = psurf->height;
        __entry->nr_samples = psurf->nr_samples;
        __entry->format = util_format_short_name(psurf->format);
}

/*
 * framebuffer
 */
struct __payload_framebuffer {
         uint16_t width;
         uint16_t height;
         uint8_t layers;
         uint8_t samples;
         uint8_t nr_cbufs;
};
static void __print_framebuffer(FILE *out, const void *arg) {
   const struct __payload_framebuffer *__entry =
      (const struct __payload_framebuffer *)arg;
   fprintf(out, "%ux%ux%u@%u, nr_cbufs: %u\n"
           , __entry->width
           , __entry->height
           , __entry->layers
           , __entry->samples
           , __entry->nr_cbufs
   );
}
static const struct u_tracepoint __tp_framebuffer = {
    ALIGN_POT(sizeof(struct __payload_framebuffer), 8),   /* keep size 64b aligned */
    "framebuffer",
    __print_framebuffer,
};
void __trace_framebuffer(struct u_trace *ut
     , const struct pipe_framebuffer_state * pfb
) {
   struct __payload_framebuffer *__entry =
      (struct __payload_framebuffer *)u_trace_append(ut, &__tp_framebuffer);
   (void)__entry;
        __entry->width = pfb->width;
        __entry->height = pfb->height;
        __entry->layers = pfb->layers;
        __entry->samples = pfb->samples;
        __entry->nr_cbufs = pfb->nr_cbufs;
}

/*
 * grid_info
 */
struct __payload_grid_info {
         uint8_t work_dim;
         uint16_t block_x;
         uint16_t block_y;
         uint16_t block_z;
         uint16_t grid_x;
         uint16_t grid_y;
         uint16_t grid_z;
};
static void __print_grid_info(FILE *out, const void *arg) {
   const struct __payload_grid_info *__entry =
      (const struct __payload_grid_info *)arg;
   fprintf(out, "work_dim=%u, block=%ux%ux%u, grid=%ux%ux%u\n"
           , __entry->work_dim
           , __entry->block_x
           , __entry->block_y
           , __entry->block_z
           , __entry->grid_x
           , __entry->grid_y
           , __entry->grid_z
   );
}
static const struct u_tracepoint __tp_grid_info = {
    ALIGN_POT(sizeof(struct __payload_grid_info), 8),   /* keep size 64b aligned */
    "grid_info",
    __print_grid_info,
};
void __trace_grid_info(struct u_trace *ut
     , const struct pipe_grid_info * pgrid
) {
   struct __payload_grid_info *__entry =
      (struct __payload_grid_info *)u_trace_append(ut, &__tp_grid_info);
   (void)__entry;
        __entry->work_dim = pgrid->work_dim;
        __entry->block_x = pgrid->block[0];
        __entry->block_y = pgrid->block[1];
        __entry->block_z = pgrid->block[2];
        __entry->grid_x = pgrid->grid[0];
        __entry->grid_y = pgrid->grid[1];
        __entry->grid_z = pgrid->grid[2];
}

