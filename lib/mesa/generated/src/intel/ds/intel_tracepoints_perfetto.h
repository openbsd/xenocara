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


#ifndef _INTEL_TRACEPOINTS_PERFETTO_H
#define _INTEL_TRACEPOINTS_PERFETTO_H

#include <perfetto.h>


static void UNUSED
trace_payload_as_extra_intel_begin_frame(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_frame *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_frame(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_frame *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("frame");

      sprintf(buf, "%u", payload->frame);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_queue_annotation(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_queue_annotation *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_queue_annotation(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_queue_annotation *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("dummy");

      sprintf(buf, "%hhu", payload->dummy);

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("str");

      sprintf(buf, "%s", payload->str);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_batch(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_batch *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_batch(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_batch *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("name");

      sprintf(buf, "%hhu", payload->name);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_cmd_buffer(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_cmd_buffer *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_cmd_buffer(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_cmd_buffer *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("level");

      sprintf(buf, "%hhu", payload->level);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_cmd_buffer_annotation(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_cmd_buffer_annotation *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_cmd_buffer_annotation(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_cmd_buffer_annotation *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("dummy");

      sprintf(buf, "%hhu", payload->dummy);

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("str");

      sprintf(buf, "%s", payload->str);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_xfb(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_xfb *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_xfb(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_xfb *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_begin_render_pass(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_render_pass *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_render_pass(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_render_pass *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("width");

      sprintf(buf, "%hu", payload->width);

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("height");

      sprintf(buf, "%hu", payload->height);

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("att_count");

      sprintf(buf, "%hhu", payload->att_count);

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("msaa");

      sprintf(buf, "%hhu", payload->msaa);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_blorp(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_blorp *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_blorp(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_blorp *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("op");

      sprintf(buf, "%s", blorp_op_to_name(payload->op));

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("width");

      sprintf(buf, "%u", payload->width);

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("height");

      sprintf(buf, "%u", payload->height);

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("samples");

      sprintf(buf, "%u", payload->samples);

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("blorp_pipe");

      sprintf(buf, "%s", blorp_shader_pipeline_to_name(payload->blorp_pipe));

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("dst_fmt");

      sprintf(buf, "%s", isl_format_get_short_name(payload->dst_fmt));

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("src_fmt");

      sprintf(buf, "%s", isl_format_get_short_name(payload->src_fmt));

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_generate_draws(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_generate_draws *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_generate_draws(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_generate_draws *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_begin_draw(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_draw *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_draw(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_draw *payload)
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
trace_payload_as_extra_intel_begin_draw_multi(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_draw_multi *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_draw_multi(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_draw_multi *payload)
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
trace_payload_as_extra_intel_begin_draw_indexed(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_draw_indexed *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_draw_indexed(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_draw_indexed *payload)
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
trace_payload_as_extra_intel_begin_draw_indexed_multi(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_draw_indexed_multi *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_draw_indexed_multi(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_draw_indexed_multi *payload)
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
trace_payload_as_extra_intel_begin_draw_indirect_byte_count(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_draw_indirect_byte_count *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_draw_indirect_byte_count(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_draw_indirect_byte_count *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("instance_count");

      sprintf(buf, "%u", payload->instance_count);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_draw_indirect(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_draw_indirect *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_draw_indirect(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_draw_indirect *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("draw_count");

      sprintf(buf, "%u", payload->draw_count);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_draw_indexed_indirect(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_draw_indexed_indirect *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_draw_indexed_indirect(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_draw_indexed_indirect *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("draw_count");

      sprintf(buf, "%u", payload->draw_count);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_draw_indirect_count(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_draw_indirect_count *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_draw_indirect_count(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_draw_indirect_count *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("max_draw_count");

      sprintf(buf, "%u", payload->max_draw_count);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_draw_indexed_indirect_count(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_draw_indexed_indirect_count *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_draw_indexed_indirect_count(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_draw_indexed_indirect_count *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("max_draw_count");

      sprintf(buf, "%u", payload->max_draw_count);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_draw_mesh(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_draw_mesh *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_draw_mesh(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_draw_mesh *payload)
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
static void UNUSED
trace_payload_as_extra_intel_begin_draw_mesh_indirect(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_draw_mesh_indirect *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_draw_mesh_indirect(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_draw_mesh_indirect *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("draw_count");

      sprintf(buf, "%u", payload->draw_count);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_draw_mesh_indirect_count(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_draw_mesh_indirect_count *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_draw_mesh_indirect_count(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_draw_mesh_indirect_count *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("max_draw_count");

      sprintf(buf, "%u", payload->max_draw_count);

      data->set_value(buf);
   }

}
static void UNUSED
trace_payload_as_extra_intel_begin_compute(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_compute *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_compute(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_compute *payload)
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
static void UNUSED
trace_payload_as_extra_intel_begin_trace_copy(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_trace_copy *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_trace_copy(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_trace_copy *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_begin_stall(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_begin_stall *payload)
{
}
static void UNUSED
trace_payload_as_extra_intel_end_stall(perfetto::protos::pbzero::GpuRenderStageEvent *event,
                                     const struct trace_intel_end_stall *payload)
{
   char buf[128];

   {
      auto data = event->add_extra_data();
      data->set_name("flags");

      sprintf(buf, "0x%x", payload->flags);

      data->set_value(buf);
   }
   {
      auto data = event->add_extra_data();
      data->set_name("reason");

      sprintf(buf, "%s", payload->reason);

      data->set_value(buf);
   }

}

#endif /* _INTEL_TRACEPOINTS_PERFETTO_H */
