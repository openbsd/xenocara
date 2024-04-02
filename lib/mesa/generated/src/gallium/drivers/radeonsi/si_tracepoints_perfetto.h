/*
 * Copyright © 2021 Igalia S.L.
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef _SI_TRACEPOINTS_PERFETTO_H
#define _SI_TRACEPOINTS_PERFETTO_H

#include <perfetto.h>


UNUSED static const char *tracepoint_names[] = {
   "si_begin_draw",
   "si_end_draw",
   "si_begin_compute",
   "si_end_compute",
};

static void UNUSED
trace_payload_as_extra_si_begin_draw(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_si_begin_draw *payload)
{
}
static void UNUSED
trace_payload_as_extra_si_end_draw(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_si_end_draw *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("count");

      sprintf(buf, "%u", payload->count);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_si_begin_compute(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_si_begin_compute *payload)
{
}
static void UNUSED
trace_payload_as_extra_si_end_compute(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_si_end_compute *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("group_x");

      sprintf(buf, "%u", payload->group_x);

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("group_y");

      sprintf(buf, "%u", payload->group_y);

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("group_z");

      sprintf(buf, "%u", payload->group_z);

      data->set_value(buf);
   }

}

#endif /* _SI_TRACEPOINTS_PERFETTO_H */
