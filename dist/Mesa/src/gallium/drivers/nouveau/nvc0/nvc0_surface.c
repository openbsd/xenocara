/*
 * Copyright 2008 Ben Skeggs
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdint.h>

#include "pipe/p_defines.h"

#include "util/u_inlines.h"
#include "util/u_pack_color.h"
#include "util/u_format.h"
#include "util/u_surface.h"

#include "os/os_thread.h"

#include "nvc0/nvc0_context.h"
#include "nvc0/nvc0_resource.h"

#include "nv50/nv50_defs.xml.h"
#include "nv50/nv50_texture.xml.h"

/* these are used in nv50_blit.h */
#define NV50_ENG2D_SUPPORTED_FORMATS 0xff9ccfe1cce3ccc9ULL
#define NV50_ENG2D_NOCONVERT_FORMATS 0x009cc02000000000ULL
#define NV50_ENG2D_LUMINANCE_FORMATS 0x001cc02000000000ULL
#define NV50_ENG2D_INTENSITY_FORMATS 0x0080000000000000ULL
#define NV50_ENG2D_OPERATION_FORMATS 0x060001c000638000ULL

#define NOUVEAU_DRIVER 0xc0
#include "nv50/nv50_blit.h"

static INLINE uint8_t
nvc0_2d_format(enum pipe_format format, boolean dst, boolean dst_src_equal)
{
   uint8_t id = nvc0_format_table[format].rt;

   /* A8_UNORM is treated as I8_UNORM as far as the 2D engine is concerned. */
   if (!dst && unlikely(format == PIPE_FORMAT_I8_UNORM) && !dst_src_equal)
      return NV50_SURFACE_FORMAT_A8_UNORM;

   /* Hardware values for color formats range from 0xc0 to 0xff,
    * but the 2D engine doesn't support all of them.
    */
   if (nv50_2d_format_supported(format))
      return id;
   assert(dst_src_equal);

   switch (util_format_get_blocksize(format)) {
   case 1:
      return NV50_SURFACE_FORMAT_R8_UNORM;
   case 2:
      return NV50_SURFACE_FORMAT_R16_UNORM;
   case 4:
      return NV50_SURFACE_FORMAT_BGRA8_UNORM;
   case 8:
      return NV50_SURFACE_FORMAT_RGBA16_UNORM;
   case 16:
      return NV50_SURFACE_FORMAT_RGBA32_FLOAT;
   default:
      assert(0);
      return 0;
   }
}

static int
nvc0_2d_texture_set(struct nouveau_pushbuf *push, boolean dst,
                    struct nv50_miptree *mt, unsigned level, unsigned layer,
                    enum pipe_format pformat, boolean dst_src_pformat_equal)
{
   struct nouveau_bo *bo = mt->base.bo;
   uint32_t width, height, depth;
   uint32_t format;
   uint32_t mthd = dst ? NVC0_2D_DST_FORMAT : NVC0_2D_SRC_FORMAT;
   uint32_t offset = mt->level[level].offset;

   format = nvc0_2d_format(pformat, dst, dst_src_pformat_equal);
   if (!format) {
      NOUVEAU_ERR("invalid/unsupported surface format: %s\n",
                  util_format_name(pformat));
      return 1;
   }

   width = u_minify(mt->base.base.width0, level) << mt->ms_x;
   height = u_minify(mt->base.base.height0, level) << mt->ms_y;
   depth = u_minify(mt->base.base.depth0, level);

   /* layer has to be < depth, and depth > tile depth / 2 */

   if (!mt->layout_3d) {
      offset += mt->layer_stride * layer;
      layer = 0;
      depth = 1;
   } else
   if (!dst) {
      offset += nvc0_mt_zslice_offset(mt, level, layer);
      layer = 0;
   }

   if (!nouveau_bo_memtype(bo)) {
      BEGIN_NVC0(push, SUBC_2D(mthd), 2);
      PUSH_DATA (push, format);
      PUSH_DATA (push, 1);
      BEGIN_NVC0(push, SUBC_2D(mthd + 0x14), 5);
      PUSH_DATA (push, mt->level[level].pitch);
      PUSH_DATA (push, width);
      PUSH_DATA (push, height);
      PUSH_DATAh(push, bo->offset + offset);
      PUSH_DATA (push, bo->offset + offset);
   } else {
      BEGIN_NVC0(push, SUBC_2D(mthd), 5);
      PUSH_DATA (push, format);
      PUSH_DATA (push, 0);
      PUSH_DATA (push, mt->level[level].tile_mode);
      PUSH_DATA (push, depth);
      PUSH_DATA (push, layer);
      BEGIN_NVC0(push, SUBC_2D(mthd + 0x18), 4);
      PUSH_DATA (push, width);
      PUSH_DATA (push, height);
      PUSH_DATAh(push, bo->offset + offset);
      PUSH_DATA (push, bo->offset + offset);
   }

#if 0
   if (dst) {
      BEGIN_NVC0(push, SUBC_2D(NVC0_2D_CLIP_X), 4);
      PUSH_DATA (push, 0);
      PUSH_DATA (push, 0);
      PUSH_DATA (push, width);
      PUSH_DATA (push, height);
   }
#endif
   return 0;
}

static int
nvc0_2d_texture_do_copy(struct nouveau_pushbuf *push,
                        struct nv50_miptree *dst, unsigned dst_level,
                        unsigned dx, unsigned dy, unsigned dz,
                        struct nv50_miptree *src, unsigned src_level,
                        unsigned sx, unsigned sy, unsigned sz,
                        unsigned w, unsigned h)
{
   const enum pipe_format dfmt = dst->base.base.format;
   const enum pipe_format sfmt = src->base.base.format;
   int ret;
   boolean eqfmt = dfmt == sfmt;

   if (!PUSH_SPACE(push, 2 * 16 + 32))
      return PIPE_ERROR;

   ret = nvc0_2d_texture_set(push, TRUE, dst, dst_level, dz, dfmt, eqfmt);
   if (ret)
      return ret;

   ret = nvc0_2d_texture_set(push, FALSE, src, src_level, sz, sfmt, eqfmt);
   if (ret)
      return ret;

   IMMED_NVC0(push, NVC0_2D(BLIT_CONTROL), 0x00);
   BEGIN_NVC0(push, NVC0_2D(BLIT_DST_X), 4);
   PUSH_DATA (push, dx << dst->ms_x);
   PUSH_DATA (push, dy << dst->ms_y);
   PUSH_DATA (push, w << dst->ms_x);
   PUSH_DATA (push, h << dst->ms_y);
   BEGIN_NVC0(push, NVC0_2D(BLIT_DU_DX_FRACT), 4);
   PUSH_DATA (push, 0);
   PUSH_DATA (push, 1);
   PUSH_DATA (push, 0);
   PUSH_DATA (push, 1);
   BEGIN_NVC0(push, NVC0_2D(BLIT_SRC_X_FRACT), 4);
   PUSH_DATA (push, 0);
   PUSH_DATA (push, sx << src->ms_x);
   PUSH_DATA (push, 0);
   PUSH_DATA (push, sy << src->ms_x);

   return 0;
}

static void
nvc0_resource_copy_region(struct pipe_context *pipe,
                          struct pipe_resource *dst, unsigned dst_level,
                          unsigned dstx, unsigned dsty, unsigned dstz,
                          struct pipe_resource *src, unsigned src_level,
                          const struct pipe_box *src_box)
{
   struct nvc0_context *nvc0 = nvc0_context(pipe);
   int ret;
   boolean m2mf;
   unsigned dst_layer = dstz, src_layer = src_box->z;

   if (dst->target == PIPE_BUFFER && src->target == PIPE_BUFFER) {
      nouveau_copy_buffer(&nvc0->base,
                          nv04_resource(dst), dstx,
                          nv04_resource(src), src_box->x, src_box->width);
      NOUVEAU_DRV_STAT(&nvc0->screen->base, buf_copy_bytes, src_box->width);
      return;
   }
   NOUVEAU_DRV_STAT(&nvc0->screen->base, tex_copy_count, 1);

   /* 0 and 1 are equal, only supporting 0/1, 2, 4 and 8 */
   assert((src->nr_samples | 1) == (dst->nr_samples | 1));

   m2mf = (src->format == dst->format) ||
      (util_format_get_blocksizebits(src->format) ==
       util_format_get_blocksizebits(dst->format));

   nv04_resource(dst)->status |= NOUVEAU_BUFFER_STATUS_GPU_WRITING;

   if (m2mf) {
      struct nv50_m2mf_rect drect, srect;
      unsigned i;
      unsigned nx = util_format_get_nblocksx(src->format, src_box->width);
      unsigned ny = util_format_get_nblocksy(src->format, src_box->height);

      nv50_m2mf_rect_setup(&drect, dst, dst_level, dstx, dsty, dstz);
      nv50_m2mf_rect_setup(&srect, src, src_level,
                           src_box->x, src_box->y, src_box->z);

      for (i = 0; i < src_box->depth; ++i) {
         nvc0->m2mf_copy_rect(nvc0, &drect, &srect, nx, ny);

         if (nv50_miptree(dst)->layout_3d)
            drect.z++;
         else
            drect.base += nv50_miptree(dst)->layer_stride;

         if (nv50_miptree(src)->layout_3d)
            srect.z++;
         else
            srect.base += nv50_miptree(src)->layer_stride;
      }
      return;
   }

   assert(nv50_2d_dst_format_faithful(dst->format));
   assert(nv50_2d_src_format_faithful(src->format));

   BCTX_REFN(nvc0->bufctx, 2D, nv04_resource(src), RD);
   BCTX_REFN(nvc0->bufctx, 2D, nv04_resource(dst), WR);
   nouveau_pushbuf_bufctx(nvc0->base.pushbuf, nvc0->bufctx);
   nouveau_pushbuf_validate(nvc0->base.pushbuf);

   for (; dst_layer < dstz + src_box->depth; ++dst_layer, ++src_layer) {
      ret = nvc0_2d_texture_do_copy(nvc0->base.pushbuf,
                                    nv50_miptree(dst), dst_level,
                                    dstx, dsty, dst_layer,
                                    nv50_miptree(src), src_level,
                                    src_box->x, src_box->y, src_layer,
                                    src_box->width, src_box->height);
      if (ret)
         break;
   }
   nouveau_bufctx_reset(nvc0->bufctx, 0);
}

static void
nvc0_clear_render_target(struct pipe_context *pipe,
                         struct pipe_surface *dst,
                         const union pipe_color_union *color,
                         unsigned dstx, unsigned dsty,
                         unsigned width, unsigned height)
{
   struct nvc0_context *nvc0 = nvc0_context(pipe);
   struct nouveau_pushbuf *push = nvc0->base.pushbuf;
   struct nv50_surface *sf = nv50_surface(dst);
   struct nv04_resource *res = nv04_resource(sf->base.texture);
   unsigned z;

   if (!PUSH_SPACE(push, 32 + sf->depth))
      return;

   PUSH_REFN (push, res->bo, res->domain | NOUVEAU_BO_WR);

   BEGIN_NVC0(push, NVC0_3D(CLEAR_COLOR(0)), 4);
   PUSH_DATAf(push, color->f[0]);
   PUSH_DATAf(push, color->f[1]);
   PUSH_DATAf(push, color->f[2]);
   PUSH_DATAf(push, color->f[3]);

   BEGIN_NVC0(push, NVC0_3D(SCREEN_SCISSOR_HORIZ), 2);
   PUSH_DATA (push, ( width << 16) | dstx);
   PUSH_DATA (push, (height << 16) | dsty);

   BEGIN_NVC0(push, NVC0_3D(RT_CONTROL), 1);
   PUSH_DATA (push, 1);
   BEGIN_NVC0(push, NVC0_3D(RT_ADDRESS_HIGH(0)), 9);
   PUSH_DATAh(push, res->address + sf->offset);
   PUSH_DATA (push, res->address + sf->offset);
   if (likely(nouveau_bo_memtype(res->bo))) {
      struct nv50_miptree *mt = nv50_miptree(dst->texture);

      PUSH_DATA(push, sf->width);
      PUSH_DATA(push, sf->height);
      PUSH_DATA(push, nvc0_format_table[dst->format].rt);
      PUSH_DATA(push, (mt->layout_3d << 16) |
               mt->level[sf->base.u.tex.level].tile_mode);
      PUSH_DATA(push, dst->u.tex.first_layer + sf->depth);
      PUSH_DATA(push, mt->layer_stride >> 2);
      PUSH_DATA(push, dst->u.tex.first_layer);
   } else {
      if (res->base.target == PIPE_BUFFER) {
         PUSH_DATA(push, 262144);
         PUSH_DATA(push, 1);
      } else {
         PUSH_DATA(push, nv50_miptree(&res->base)->level[0].pitch);
         PUSH_DATA(push, sf->height);
      }
      PUSH_DATA(push, nvc0_format_table[sf->base.format].rt);
      PUSH_DATA(push, 1 << 12);
      PUSH_DATA(push, 1);
      PUSH_DATA(push, 0);
      PUSH_DATA(push, 0);

      IMMED_NVC0(push, NVC0_3D(ZETA_ENABLE), 0);

      /* tiled textures don't have to be fenced, they're not mapped directly */
      nvc0_resource_fence(res, NOUVEAU_BO_WR);
   }

   BEGIN_NIC0(push, NVC0_3D(CLEAR_BUFFERS), sf->depth);
   for (z = 0; z < sf->depth; ++z) {
      PUSH_DATA (push, 0x3c |
                 (z << NVC0_3D_CLEAR_BUFFERS_LAYER__SHIFT));
   }

   nvc0->dirty |= NVC0_NEW_FRAMEBUFFER;
}

static void
nvc0_clear_depth_stencil(struct pipe_context *pipe,
                         struct pipe_surface *dst,
                         unsigned clear_flags,
                         double depth,
                         unsigned stencil,
                         unsigned dstx, unsigned dsty,
                         unsigned width, unsigned height)
{
	struct nvc0_context *nvc0 = nvc0_context(pipe);
	struct nouveau_pushbuf *push = nvc0->base.pushbuf;
	struct nv50_miptree *mt = nv50_miptree(dst->texture);
	struct nv50_surface *sf = nv50_surface(dst);
	uint32_t mode = 0;
	int unk = mt->base.base.target == PIPE_TEXTURE_2D;
	unsigned z;

	if (!PUSH_SPACE(push, 32 + sf->depth))
		return;

	PUSH_REFN (push, mt->base.bo, mt->base.domain | NOUVEAU_BO_WR);

	if (clear_flags & PIPE_CLEAR_DEPTH) {
		BEGIN_NVC0(push, NVC0_3D(CLEAR_DEPTH), 1);
		PUSH_DATAf(push, depth);
		mode |= NVC0_3D_CLEAR_BUFFERS_Z;
	}

	if (clear_flags & PIPE_CLEAR_STENCIL) {
		BEGIN_NVC0(push, NVC0_3D(CLEAR_STENCIL), 1);
		PUSH_DATA (push, stencil & 0xff);
		mode |= NVC0_3D_CLEAR_BUFFERS_S;
	}

	BEGIN_NVC0(push, NVC0_3D(SCREEN_SCISSOR_HORIZ), 2);
	PUSH_DATA (push, ( width << 16) | dstx);
	PUSH_DATA (push, (height << 16) | dsty);

	BEGIN_NVC0(push, NVC0_3D(ZETA_ADDRESS_HIGH), 5);
	PUSH_DATAh(push, mt->base.address + sf->offset);
	PUSH_DATA (push, mt->base.address + sf->offset);
	PUSH_DATA (push, nvc0_format_table[dst->format].rt);
	PUSH_DATA (push, mt->level[sf->base.u.tex.level].tile_mode);
	PUSH_DATA (push, mt->layer_stride >> 2);
	BEGIN_NVC0(push, NVC0_3D(ZETA_ENABLE), 1);
	PUSH_DATA (push, 1);
	BEGIN_NVC0(push, NVC0_3D(ZETA_HORIZ), 3);
	PUSH_DATA (push, sf->width);
	PUSH_DATA (push, sf->height);
	PUSH_DATA (push, (unk << 16) | (dst->u.tex.first_layer + sf->depth));
	BEGIN_NVC0(push, NVC0_3D(ZETA_BASE_LAYER), 1);
	PUSH_DATA (push, dst->u.tex.first_layer);

	BEGIN_NIC0(push, NVC0_3D(CLEAR_BUFFERS), sf->depth);
	for (z = 0; z < sf->depth; ++z) {
		PUSH_DATA (push, mode |
			   (z << NVC0_3D_CLEAR_BUFFERS_LAYER__SHIFT));
	}

	nvc0->dirty |= NVC0_NEW_FRAMEBUFFER;
}

void
nvc0_clear(struct pipe_context *pipe, unsigned buffers,
           const union pipe_color_union *color,
           double depth, unsigned stencil)
{
   struct nvc0_context *nvc0 = nvc0_context(pipe);
   struct nouveau_pushbuf *push = nvc0->base.pushbuf;
   struct pipe_framebuffer_state *fb = &nvc0->framebuffer;
   unsigned i, j, k;
   uint32_t mode = 0;

   /* don't need NEW_BLEND, COLOR_MASK doesn't affect CLEAR_BUFFERS */
   if (!nvc0_state_validate(nvc0, NVC0_NEW_FRAMEBUFFER, 9 + (fb->nr_cbufs * 2)))
      return;

   if (buffers & PIPE_CLEAR_COLOR && fb->nr_cbufs) {
      BEGIN_NVC0(push, NVC0_3D(CLEAR_COLOR(0)), 4);
      PUSH_DATAf(push, color->f[0]);
      PUSH_DATAf(push, color->f[1]);
      PUSH_DATAf(push, color->f[2]);
      PUSH_DATAf(push, color->f[3]);
      if (buffers & PIPE_CLEAR_COLOR0)
         mode =
            NVC0_3D_CLEAR_BUFFERS_R | NVC0_3D_CLEAR_BUFFERS_G |
            NVC0_3D_CLEAR_BUFFERS_B | NVC0_3D_CLEAR_BUFFERS_A;
   }

   if (buffers & PIPE_CLEAR_DEPTH) {
      BEGIN_NVC0(push, NVC0_3D(CLEAR_DEPTH), 1);
      PUSH_DATA (push, fui(depth));
      mode |= NVC0_3D_CLEAR_BUFFERS_Z;
   }

   if (buffers & PIPE_CLEAR_STENCIL) {
      BEGIN_NVC0(push, NVC0_3D(CLEAR_STENCIL), 1);
      PUSH_DATA (push, stencil & 0xff);
      mode |= NVC0_3D_CLEAR_BUFFERS_S;
   }

   if (mode) {
      int zs_layers = 0, color0_layers = 0;
      if (fb->cbufs[0] && (mode & 0x3c))
         color0_layers = fb->cbufs[0]->u.tex.last_layer -
            fb->cbufs[0]->u.tex.first_layer + 1;
      if (fb->zsbuf && (mode & ~0x3c))
         zs_layers = fb->zsbuf->u.tex.last_layer -
            fb->zsbuf->u.tex.first_layer + 1;

      for (j = 0; j < MIN2(zs_layers, color0_layers); j++) {
         BEGIN_NVC0(push, NVC0_3D(CLEAR_BUFFERS), 1);
         PUSH_DATA(push, mode | (j << NVC0_3D_CLEAR_BUFFERS_LAYER__SHIFT));
      }
      for (k = j; k < zs_layers; k++) {
         BEGIN_NVC0(push, NVC0_3D(CLEAR_BUFFERS), 1);
         PUSH_DATA(push, (mode & ~0x3c) | (k << NVC0_3D_CLEAR_BUFFERS_LAYER__SHIFT));
      }
      for (k = j; k < color0_layers; k++) {
         BEGIN_NVC0(push, NVC0_3D(CLEAR_BUFFERS), 1);
         PUSH_DATA(push, (mode & 0x3c) | (k << NVC0_3D_CLEAR_BUFFERS_LAYER__SHIFT));
      }
   }

   for (i = 1; i < fb->nr_cbufs; i++) {
      struct pipe_surface *sf = fb->cbufs[i];
      if (!sf || !(buffers & (PIPE_CLEAR_COLOR0 << i)))
         continue;
      for (j = 0; j <= sf->u.tex.last_layer - sf->u.tex.first_layer; j++) {
         BEGIN_NVC0(push, NVC0_3D(CLEAR_BUFFERS), 1);
         PUSH_DATA (push, (i << 6) | 0x3c |
                    (j << NVC0_3D_CLEAR_BUFFERS_LAYER__SHIFT));
      }
   }
}


/* =============================== BLIT CODE ===================================
 */

struct nvc0_blitter
{
   struct nvc0_program *fp[NV50_BLIT_MAX_TEXTURE_TYPES][NV50_BLIT_MODES];
   struct nvc0_program vp;

   struct nv50_tsc_entry sampler[2]; /* nearest, bilinear */

   pipe_mutex mutex;

   struct nvc0_screen *screen;
};

struct nvc0_blitctx
{
   struct nvc0_context *nvc0;
   struct nvc0_program *fp;
   uint8_t mode;
   uint16_t color_mask;
   uint8_t filter;
   uint8_t render_condition_enable;
   enum pipe_texture_target target;
   struct {
      struct pipe_framebuffer_state fb;
      struct nvc0_rasterizer_stateobj *rast;
      struct nvc0_program *vp;
      struct nvc0_program *tcp;
      struct nvc0_program *tep;
      struct nvc0_program *gp;
      struct nvc0_program *fp;
      unsigned num_textures[5];
      unsigned num_samplers[5];
      struct pipe_sampler_view *texture[2];
      struct nv50_tsc_entry *sampler[2];
      unsigned min_samples;
      uint32_t dirty;
   } saved;
   struct nvc0_rasterizer_stateobj rast;
};

static void
nvc0_blitter_make_vp(struct nvc0_blitter *blit)
{
   static const uint32_t code_nvc0[] =
   {
      0xfff11c26, 0x06000080, /* vfetch b64 $r4:$r5 a[0x80] */
      0xfff01c46, 0x06000090, /* vfetch b96 $r0:$r1:$r2 a[0x90] */
      0x13f01c26, 0x0a7e0070, /* export b64 o[0x70] $r4:$r5 */
      0x03f01c46, 0x0a7e0080, /* export b96 o[0x80] $r0:$r1:$r2 */
      0x00001de7, 0x80000000, /* exit */
   };
   static const uint32_t code_nve4[] =
   {
      0x00000007, 0x20000000, /* sched */
      0xfff11c26, 0x06000080, /* vfetch b64 $r4:$r5 a[0x80] */
      0xfff01c46, 0x06000090, /* vfetch b96 $r0:$r1:$r2 a[0x90] */
      0x13f01c26, 0x0a7e0070, /* export b64 o[0x70] $r4:$r5 */
      0x03f01c46, 0x0a7e0080, /* export b96 o[0x80] $r0:$r1:$r2 */
      0x00001de7, 0x80000000, /* exit */
   };
   static const uint32_t code_gk110[] =
   {
      0x00000000, 0x08000000, /* sched */
      0x401ffc12, 0x7ec7fc00, /* ld b64 $r4d a[0x80] 0x0 0x0 */
      0x481ffc02, 0x7ecbfc00, /* ld b96 $r0t a[0x90] 0x0 0x0 */
      0x381ffc12, 0x7f07fc00, /* st b64 a[0x70] $r4d 0x0 0x0 */
      0x401ffc02, 0x7f0bfc00, /* st b96 a[0x80] $r0t 0x0 0x0 */
      0x001c003c, 0x18000000, /* exit */
   };

   blit->vp.type = PIPE_SHADER_VERTEX;
   blit->vp.translated = TRUE;
   if (blit->screen->base.class_3d >= NVF0_3D_CLASS) {
      blit->vp.code = (uint32_t *)code_gk110; /* const_cast */
      blit->vp.code_size = sizeof(code_gk110);
   } else
   if (blit->screen->base.class_3d >= NVE4_3D_CLASS) {
      blit->vp.code = (uint32_t *)code_nve4; /* const_cast */
      blit->vp.code_size = sizeof(code_nve4);
   } else {
      blit->vp.code = (uint32_t *)code_nvc0; /* const_cast */
      blit->vp.code_size = sizeof(code_nvc0);
   }
   blit->vp.num_gprs = 6;
   blit->vp.vp.edgeflag = PIPE_MAX_ATTRIBS;

   blit->vp.hdr[0]  = 0x00020461; /* vertprog magic */
   blit->vp.hdr[4]  = 0x000ff000; /* no outputs read */
   blit->vp.hdr[6]  = 0x00000073; /* a[0x80].xy, a[0x90].xyz */
   blit->vp.hdr[13] = 0x00073000; /* o[0x70].xy, o[0x80].xyz */
}

static void
nvc0_blitter_make_sampler(struct nvc0_blitter *blit)
{
   /* clamp to edge, min/max lod = 0, nearest filtering */

   blit->sampler[0].id = -1;

   blit->sampler[0].tsc[0] = NV50_TSC_0_SRGB_CONVERSION_ALLOWED |
      (NV50_TSC_WRAP_CLAMP_TO_EDGE << NV50_TSC_0_WRAPS__SHIFT) |
      (NV50_TSC_WRAP_CLAMP_TO_EDGE << NV50_TSC_0_WRAPT__SHIFT) |
      (NV50_TSC_WRAP_CLAMP_TO_EDGE << NV50_TSC_0_WRAPR__SHIFT);
   blit->sampler[0].tsc[1] =
      NV50_TSC_1_MAGF_NEAREST | NV50_TSC_1_MINF_NEAREST | NV50_TSC_1_MIPF_NONE;

   /* clamp to edge, min/max lod = 0, bilinear filtering */

   blit->sampler[1].id = -1;

   blit->sampler[1].tsc[0] = blit->sampler[0].tsc[0];
   blit->sampler[1].tsc[1] =
      NV50_TSC_1_MAGF_LINEAR | NV50_TSC_1_MINF_LINEAR | NV50_TSC_1_MIPF_NONE;
}

static void
nvc0_blit_select_fp(struct nvc0_blitctx *ctx, const struct pipe_blit_info *info)
{
   struct nvc0_blitter *blitter = ctx->nvc0->screen->blitter;

   const enum pipe_texture_target ptarg =
      nv50_blit_reinterpret_pipe_texture_target(info->src.resource->target);

   const unsigned targ = nv50_blit_texture_type(ptarg);
   const unsigned mode = ctx->mode;

   if (!blitter->fp[targ][mode]) {
      pipe_mutex_lock(blitter->mutex);
      if (!blitter->fp[targ][mode])
         blitter->fp[targ][mode] =
            nv50_blitter_make_fp(&ctx->nvc0->base.pipe, mode, ptarg);
      pipe_mutex_unlock(blitter->mutex);
   }
   ctx->fp = blitter->fp[targ][mode];
}

static void
nvc0_blit_set_dst(struct nvc0_blitctx *ctx,
                  struct pipe_resource *res, unsigned level, unsigned layer,
                  enum pipe_format format)
{
   struct nvc0_context *nvc0 = ctx->nvc0;
   struct pipe_context *pipe = &nvc0->base.pipe;
   struct pipe_surface templ;

   if (util_format_is_depth_or_stencil(format))
      templ.format = nv50_blit_zeta_to_colour_format(format);
   else
      templ.format = format;

   templ.u.tex.level = level;
   templ.u.tex.first_layer = templ.u.tex.last_layer = layer;

   if (layer == -1) {
      templ.u.tex.first_layer = 0;
      templ.u.tex.last_layer =
         (res->target == PIPE_TEXTURE_3D ? res->depth0 : res->array_size) - 1;
   }

   nvc0->framebuffer.cbufs[0] = nvc0_miptree_surface_new(pipe, res, &templ);
   nvc0->framebuffer.nr_cbufs = 1;
   nvc0->framebuffer.zsbuf = NULL;
   nvc0->framebuffer.width = nvc0->framebuffer.cbufs[0]->width;
   nvc0->framebuffer.height = nvc0->framebuffer.cbufs[0]->height;
}

static void
nvc0_blit_set_src(struct nvc0_blitctx *ctx,
                  struct pipe_resource *res, unsigned level, unsigned layer,
                  enum pipe_format format, const uint8_t filter)
{
   struct nvc0_context *nvc0 = ctx->nvc0;
   struct pipe_context *pipe = &nvc0->base.pipe;
   struct pipe_sampler_view templ;
   uint32_t flags;
   unsigned s;
   enum pipe_texture_target target;

   target = nv50_blit_reinterpret_pipe_texture_target(res->target);

   templ.format = format;
   templ.u.tex.first_layer = templ.u.tex.last_layer = layer;
   templ.u.tex.first_level = templ.u.tex.last_level = level;
   templ.swizzle_r = PIPE_SWIZZLE_RED;
   templ.swizzle_g = PIPE_SWIZZLE_GREEN;
   templ.swizzle_b = PIPE_SWIZZLE_BLUE;
   templ.swizzle_a = PIPE_SWIZZLE_ALPHA;

   if (layer == -1) {
      templ.u.tex.first_layer = 0;
      templ.u.tex.last_layer =
         (res->target == PIPE_TEXTURE_3D ? res->depth0 : res->array_size) - 1;
   }

   flags = res->last_level ? 0 : NV50_TEXVIEW_SCALED_COORDS;
   flags |= NV50_TEXVIEW_ACCESS_RESOLVE;
   if (filter && res->nr_samples == 8)
      flags |= NV50_TEXVIEW_FILTER_MSAA8;

   nvc0->textures[4][0] = nvc0_create_texture_view(
      pipe, res, &templ, flags, target);
   nvc0->textures[4][1] = NULL;

   for (s = 0; s <= 3; ++s)
      nvc0->num_textures[s] = 0;
   nvc0->num_textures[4] = 1;

   templ.format = nv50_zs_to_s_format(format);
   if (templ.format != format) {
      nvc0->textures[4][1] = nvc0_create_texture_view(
         pipe, res, &templ, flags, target);
      nvc0->num_textures[4] = 2;
   }
}

static void
nvc0_blitctx_prepare_state(struct nvc0_blitctx *blit)
{
   struct nouveau_pushbuf *push = blit->nvc0->base.pushbuf;

   /* TODO: maybe make this a MACRO (if we need more logic) ? */

   if (blit->nvc0->cond_query && !blit->render_condition_enable)
      IMMED_NVC0(push, NVC0_3D(COND_MODE), NVC0_3D_COND_MODE_ALWAYS);

   /* blend state */
   BEGIN_NVC0(push, NVC0_3D(COLOR_MASK(0)), 1);
   PUSH_DATA (push, blit->color_mask);
   IMMED_NVC0(push, NVC0_3D(BLEND_ENABLE(0)), 0);
   IMMED_NVC0(push, NVC0_3D(LOGIC_OP_ENABLE), 0);

   /* rasterizer state */
   IMMED_NVC0(push, NVC0_3D(FRAG_COLOR_CLAMP_EN), 0);
   IMMED_NVC0(push, NVC0_3D(MULTISAMPLE_ENABLE), 0);
   BEGIN_NVC0(push, NVC0_3D(MSAA_MASK(0)), 4);
   PUSH_DATA (push, 0xffff);
   PUSH_DATA (push, 0xffff);
   PUSH_DATA (push, 0xffff);
   PUSH_DATA (push, 0xffff);
   BEGIN_NVC0(push, NVC0_3D(MACRO_POLYGON_MODE_FRONT), 1);
   PUSH_DATA (push, NVC0_3D_MACRO_POLYGON_MODE_FRONT_FILL);
   BEGIN_NVC0(push, NVC0_3D(MACRO_POLYGON_MODE_BACK), 1);
   PUSH_DATA (push, NVC0_3D_MACRO_POLYGON_MODE_BACK_FILL);
   IMMED_NVC0(push, NVC0_3D(POLYGON_SMOOTH_ENABLE), 0);
   IMMED_NVC0(push, NVC0_3D(POLYGON_OFFSET_FILL_ENABLE), 0);
   IMMED_NVC0(push, NVC0_3D(POLYGON_STIPPLE_ENABLE), 0);
   IMMED_NVC0(push, NVC0_3D(CULL_FACE_ENABLE), 0);

   /* zsa state */
   IMMED_NVC0(push, NVC0_3D(DEPTH_TEST_ENABLE), 0);
   IMMED_NVC0(push, NVC0_3D(STENCIL_ENABLE), 0);
   IMMED_NVC0(push, NVC0_3D(ALPHA_TEST_ENABLE), 0);

   /* disable transform feedback */
   IMMED_NVC0(push, NVC0_3D(TFB_ENABLE), 0);
}

static void
nvc0_blitctx_pre_blit(struct nvc0_blitctx *ctx)
{
   struct nvc0_context *nvc0 = ctx->nvc0;
   struct nvc0_blitter *blitter = nvc0->screen->blitter;
   int s;

   ctx->saved.fb.width = nvc0->framebuffer.width;
   ctx->saved.fb.height = nvc0->framebuffer.height;
   ctx->saved.fb.nr_cbufs = nvc0->framebuffer.nr_cbufs;
   ctx->saved.fb.cbufs[0] = nvc0->framebuffer.cbufs[0];
   ctx->saved.fb.zsbuf = nvc0->framebuffer.zsbuf;

   ctx->saved.rast = nvc0->rast;

   ctx->saved.vp = nvc0->vertprog;
   ctx->saved.tcp = nvc0->tctlprog;
   ctx->saved.tep = nvc0->tevlprog;
   ctx->saved.gp = nvc0->gmtyprog;
   ctx->saved.fp = nvc0->fragprog;

   ctx->saved.min_samples = nvc0->min_samples;

   nvc0->rast = &ctx->rast;

   nvc0->vertprog = &blitter->vp;
   nvc0->tctlprog = NULL;
   nvc0->tevlprog = NULL;
   nvc0->gmtyprog = NULL;
   nvc0->fragprog = ctx->fp;

   for (s = 0; s <= 4; ++s) {
      ctx->saved.num_textures[s] = nvc0->num_textures[s];
      ctx->saved.num_samplers[s] = nvc0->num_samplers[s];
      nvc0->textures_dirty[s] = (1 << nvc0->num_textures[s]) - 1;
      nvc0->samplers_dirty[s] = (1 << nvc0->num_samplers[s]) - 1;
   }
   ctx->saved.texture[0] = nvc0->textures[4][0];
   ctx->saved.texture[1] = nvc0->textures[4][1];
   ctx->saved.sampler[0] = nvc0->samplers[4][0];
   ctx->saved.sampler[1] = nvc0->samplers[4][1];

   nvc0->samplers[4][0] = &blitter->sampler[ctx->filter];
   nvc0->samplers[4][1] = &blitter->sampler[ctx->filter];

   for (s = 0; s <= 3; ++s)
      nvc0->num_samplers[s] = 0;
   nvc0->num_samplers[4] = 2;

   nvc0->min_samples = 1;

   ctx->saved.dirty = nvc0->dirty;

   nvc0->textures_dirty[4] |= 3;
   nvc0->samplers_dirty[4] |= 3;

   nouveau_bufctx_reset(nvc0->bufctx_3d, NVC0_BIND_FB);
   nouveau_bufctx_reset(nvc0->bufctx_3d, NVC0_BIND_TEX(4, 0));
   nouveau_bufctx_reset(nvc0->bufctx_3d, NVC0_BIND_TEX(4, 1));

   nvc0->dirty = NVC0_NEW_FRAMEBUFFER | NVC0_NEW_MIN_SAMPLES |
      NVC0_NEW_VERTPROG | NVC0_NEW_FRAGPROG |
      NVC0_NEW_TCTLPROG | NVC0_NEW_TEVLPROG | NVC0_NEW_GMTYPROG |
      NVC0_NEW_TEXTURES | NVC0_NEW_SAMPLERS;
}

static void
nvc0_blitctx_post_blit(struct nvc0_blitctx *blit)
{
   struct nvc0_context *nvc0 = blit->nvc0;
   int s;

   pipe_surface_reference(&nvc0->framebuffer.cbufs[0], NULL);

   nvc0->framebuffer.width = blit->saved.fb.width;
   nvc0->framebuffer.height = blit->saved.fb.height;
   nvc0->framebuffer.nr_cbufs = blit->saved.fb.nr_cbufs;
   nvc0->framebuffer.cbufs[0] = blit->saved.fb.cbufs[0];
   nvc0->framebuffer.zsbuf = blit->saved.fb.zsbuf;

   nvc0->rast = blit->saved.rast;

   nvc0->vertprog = blit->saved.vp;
   nvc0->tctlprog = blit->saved.tcp;
   nvc0->tevlprog = blit->saved.tep;
   nvc0->gmtyprog = blit->saved.gp;
   nvc0->fragprog = blit->saved.fp;

   nvc0->min_samples = blit->saved.min_samples;

   pipe_sampler_view_reference(&nvc0->textures[4][0], NULL);
   pipe_sampler_view_reference(&nvc0->textures[4][1], NULL);

   for (s = 0; s <= 4; ++s) {
      nvc0->num_textures[s] = blit->saved.num_textures[s];
      nvc0->num_samplers[s] = blit->saved.num_samplers[s];
      nvc0->textures_dirty[s] = (1 << nvc0->num_textures[s]) - 1;
      nvc0->samplers_dirty[s] = (1 << nvc0->num_samplers[s]) - 1;
   }
   nvc0->textures[4][0] = blit->saved.texture[0];
   nvc0->textures[4][1] = blit->saved.texture[1];
   nvc0->samplers[4][0] = blit->saved.sampler[0];
   nvc0->samplers[4][1] = blit->saved.sampler[1];

   nvc0->textures_dirty[4] |= 3;
   nvc0->samplers_dirty[4] |= 3;

   if (nvc0->cond_query && !blit->render_condition_enable)
      nvc0->base.pipe.render_condition(&nvc0->base.pipe, nvc0->cond_query,
                                       nvc0->cond_cond, nvc0->cond_mode);

   nouveau_bufctx_reset(nvc0->bufctx_3d, NVC0_BIND_FB);
   nouveau_bufctx_reset(nvc0->bufctx_3d, NVC0_BIND_TEX(4, 0));
   nouveau_bufctx_reset(nvc0->bufctx_3d, NVC0_BIND_TEX(4, 1));

   nvc0->dirty = blit->saved.dirty |
      (NVC0_NEW_FRAMEBUFFER | NVC0_NEW_SCISSOR | NVC0_NEW_SAMPLE_MASK |
       NVC0_NEW_RASTERIZER | NVC0_NEW_ZSA | NVC0_NEW_BLEND |
       NVC0_NEW_TEXTURES | NVC0_NEW_SAMPLERS |
       NVC0_NEW_VERTPROG | NVC0_NEW_FRAGPROG |
       NVC0_NEW_TCTLPROG | NVC0_NEW_TEVLPROG | NVC0_NEW_GMTYPROG |
       NVC0_NEW_TFB_TARGETS);

   nvc0->base.pipe.set_min_samples(&nvc0->base.pipe, blit->saved.min_samples);
}

static void
nvc0_blit_3d(struct nvc0_context *nvc0, const struct pipe_blit_info *info)
{
   struct nvc0_blitctx *blit = nvc0->blit;
   struct nouveau_pushbuf *push = nvc0->base.pushbuf;
   struct pipe_resource *src = info->src.resource;
   struct pipe_resource *dst = info->dst.resource;
   int32_t minx, maxx, miny, maxy;
   int32_t i;
   float x0, x1, y0, y1, z;
   float dz;
   float x_range, y_range;

   blit->mode = nv50_blit_select_mode(info);
   blit->color_mask = nv50_blit_derive_color_mask(info);
   blit->filter = nv50_blit_get_filter(info);
   blit->render_condition_enable = info->render_condition_enable;

   nvc0_blit_select_fp(blit, info);
   nvc0_blitctx_pre_blit(blit);

   nvc0_blit_set_dst(blit, dst, info->dst.level, -1, info->dst.format);
   nvc0_blit_set_src(blit, src, info->src.level, -1, info->src.format,
                     blit->filter);

   nvc0_blitctx_prepare_state(blit);

   nvc0_state_validate(nvc0, ~0, 48);

   x_range = (float)info->src.box.width / (float)info->dst.box.width;
   y_range = (float)info->src.box.height / (float)info->dst.box.height;

   x0 = (float)info->src.box.x - x_range * (float)info->dst.box.x;
   y0 = (float)info->src.box.y - y_range * (float)info->dst.box.y;

   x1 = x0 + 16384.0f * x_range;
   y1 = y0 + 16384.0f * y_range;

   x0 *= (float)(1 << nv50_miptree(src)->ms_x);
   x1 *= (float)(1 << nv50_miptree(src)->ms_x);
   y0 *= (float)(1 << nv50_miptree(src)->ms_y);
   y1 *= (float)(1 << nv50_miptree(src)->ms_y);

   dz = (float)info->src.box.depth / (float)info->dst.box.depth;
   z = (float)info->src.box.z;
   if (nv50_miptree(src)->layout_3d)
      z += 0.5f * dz;

   if (src->last_level > 0) {
      /* If there are mip maps, GPU always assumes normalized coordinates. */
      const unsigned l = info->src.level;
      const float fh = u_minify(src->width0 << nv50_miptree(src)->ms_x, l);
      const float fv = u_minify(src->height0 << nv50_miptree(src)->ms_y, l);
      x0 /= fh;
      x1 /= fh;
      y0 /= fv;
      y1 /= fv;
      if (nv50_miptree(src)->layout_3d) {
         z /= u_minify(src->depth0, l);
         dz /= u_minify(src->depth0, l);
      }
   }

   IMMED_NVC0(push, NVC0_3D(VIEWPORT_TRANSFORM_EN), 0);
   IMMED_NVC0(push, NVC0_3D(VIEW_VOLUME_CLIP_CTRL), 0x2 |
              NVC0_3D_VIEW_VOLUME_CLIP_CTRL_DEPTH_RANGE_0_1);
   BEGIN_NVC0(push, NVC0_3D(VIEWPORT_HORIZ(0)), 2);
   PUSH_DATA (push, nvc0->framebuffer.width << 16);
   PUSH_DATA (push, nvc0->framebuffer.height << 16);

   /* Draw a large triangle in screen coordinates covering the whole
    * render target, with scissors defining the destination region.
    * The vertex is supplied with non-normalized texture coordinates
    * arranged in a way to yield the desired offset and scale.
    */

   minx = info->dst.box.x;
   maxx = info->dst.box.x + info->dst.box.width;
   miny = info->dst.box.y;
   maxy = info->dst.box.y + info->dst.box.height;
   if (info->scissor_enable) {
      minx = MAX2(minx, info->scissor.minx);
      maxx = MIN2(maxx, info->scissor.maxx);
      miny = MAX2(miny, info->scissor.miny);
      maxy = MIN2(maxy, info->scissor.maxy);
   }
   BEGIN_NVC0(push, NVC0_3D(SCISSOR_HORIZ(0)), 2);
   PUSH_DATA (push, (maxx << 16) | minx);
   PUSH_DATA (push, (maxy << 16) | miny);

   for (i = 0; i < info->dst.box.depth; ++i, z += dz) {
      if (info->dst.box.z + i) {
         BEGIN_NVC0(push, NVC0_3D(LAYER), 1);
         PUSH_DATA (push, info->dst.box.z + i);
      }

      IMMED_NVC0(push, NVC0_3D(VERTEX_BEGIN_GL),
                       NVC0_3D_VERTEX_BEGIN_GL_PRIMITIVE_TRIANGLES);

      BEGIN_NVC0(push, NVC0_3D(VTX_ATTR_DEFINE), 4);
      PUSH_DATA (push, 0x74301);
      PUSH_DATAf(push, x0);
      PUSH_DATAf(push, y0);
      PUSH_DATAf(push, z);
      BEGIN_NVC0(push, NVC0_3D(VTX_ATTR_DEFINE), 3);
      PUSH_DATA (push, 0x74200);
      PUSH_DATAf(push, 0.0f);
      PUSH_DATAf(push, 0.0f);
      BEGIN_NVC0(push, NVC0_3D(VTX_ATTR_DEFINE), 4);
      PUSH_DATA (push, 0x74301);
      PUSH_DATAf(push, x1);
      PUSH_DATAf(push, y0);
      PUSH_DATAf(push, z);
      BEGIN_NVC0(push, NVC0_3D(VTX_ATTR_DEFINE), 3);
      PUSH_DATA (push, 0x74200);
      PUSH_DATAf(push, 16384 << nv50_miptree(dst)->ms_x);
      PUSH_DATAf(push, 0.0f);
      BEGIN_NVC0(push, NVC0_3D(VTX_ATTR_DEFINE), 4);
      PUSH_DATA (push, 0x74301);
      PUSH_DATAf(push, x0);
      PUSH_DATAf(push, y1);
      PUSH_DATAf(push, z);
      BEGIN_NVC0(push, NVC0_3D(VTX_ATTR_DEFINE), 3);
      PUSH_DATA (push, 0x74200);
      PUSH_DATAf(push, 0.0f);
      PUSH_DATAf(push, 16384 << nv50_miptree(dst)->ms_y);

      IMMED_NVC0(push, NVC0_3D(VERTEX_END_GL), 0);
   }
   if (info->dst.box.z + info->dst.box.depth - 1)
      IMMED_NVC0(push, NVC0_3D(LAYER), 0);

   nvc0_blitctx_post_blit(blit);

   /* restore viewport */

   BEGIN_NVC0(push, NVC0_3D(VIEWPORT_HORIZ(0)), 2);
   PUSH_DATA (push, nvc0->vport_int[0]);
   PUSH_DATA (push, nvc0->vport_int[1]);
   IMMED_NVC0(push, NVC0_3D(VIEWPORT_TRANSFORM_EN), 1);
}

static void
nvc0_blit_eng2d(struct nvc0_context *nvc0, const struct pipe_blit_info *info)
{
   struct nouveau_pushbuf *push = nvc0->base.pushbuf;
   struct nv50_miptree *dst = nv50_miptree(info->dst.resource);
   struct nv50_miptree *src = nv50_miptree(info->src.resource);
   const int32_t srcx_adj = info->src.box.width < 0 ? -1 : 0;
   const int32_t srcy_adj = info->src.box.height < 0 ? -1 : 0;
   const int dz = info->dst.box.z;
   const int sz = info->src.box.z;
   uint32_t dstw, dsth;
   int32_t dstx, dsty;
   int64_t srcx, srcy;
   int64_t du_dx, dv_dy;
   int i;
   uint32_t mode;
   uint32_t mask = nv50_blit_eng2d_get_mask(info);
   boolean b;

   mode = nv50_blit_get_filter(info) ?
      NVC0_2D_BLIT_CONTROL_FILTER_BILINEAR :
      NVC0_2D_BLIT_CONTROL_FILTER_POINT_SAMPLE;
   mode |= (src->base.base.nr_samples > dst->base.base.nr_samples) ?
      NVC0_2D_BLIT_CONTROL_ORIGIN_CORNER : NVC0_2D_BLIT_CONTROL_ORIGIN_CENTER;

   du_dx = ((int64_t)info->src.box.width << 32) / info->dst.box.width;
   dv_dy = ((int64_t)info->src.box.height << 32) / info->dst.box.height;

   b = info->dst.format == info->src.format;
   nvc0_2d_texture_set(push, 1, dst, info->dst.level, dz, info->dst.format, b);
   nvc0_2d_texture_set(push, 0, src, info->src.level, sz, info->src.format, b);

   if (info->scissor_enable) {
      BEGIN_NVC0(push, NVC0_2D(CLIP_X), 5);
      PUSH_DATA (push, info->scissor.minx << dst->ms_x);
      PUSH_DATA (push, info->scissor.miny << dst->ms_y);
      PUSH_DATA (push, (info->scissor.maxx - info->scissor.minx) << dst->ms_x);
      PUSH_DATA (push, (info->scissor.maxy - info->scissor.miny) << dst->ms_y);
      PUSH_DATA (push, 1); /* enable */
   }

   if (nvc0->cond_query && info->render_condition_enable)
      IMMED_NVC0(push, NVC0_2D(COND_MODE), NVC0_2D_COND_MODE_RES_NON_ZERO);

   if (mask != 0xffffffff) {
      IMMED_NVC0(push, NVC0_2D(ROP), 0xca); /* DPSDxax */
      IMMED_NVC0(push, NVC0_2D(PATTERN_COLOR_FORMAT),
                       NVC0_2D_PATTERN_COLOR_FORMAT_32BPP);
      BEGIN_NVC0(push, NVC0_2D(PATTERN_COLOR(0)), 4);
      PUSH_DATA (push, 0x00000000);
      PUSH_DATA (push, mask);
      PUSH_DATA (push, 0xffffffff);
      PUSH_DATA (push, 0xffffffff);
      IMMED_NVC0(push, NVC0_2D(OPERATION), NVC0_2D_OPERATION_ROP);
   } else
   if (info->src.format != info->dst.format) {
      if (info->src.format == PIPE_FORMAT_R8_UNORM ||
          info->src.format == PIPE_FORMAT_R8_SNORM ||
          info->src.format == PIPE_FORMAT_R16_UNORM ||
          info->src.format == PIPE_FORMAT_R16_SNORM ||
          info->src.format == PIPE_FORMAT_R16_FLOAT ||
          info->src.format == PIPE_FORMAT_R32_FLOAT) {
         mask = 0xffff0000; /* also makes condition for OPERATION reset true */
         BEGIN_NVC0(push, NVC0_2D(BETA4), 2);
         PUSH_DATA (push, mask);
         PUSH_DATA (push, NVC0_2D_OPERATION_SRCCOPY_PREMULT);
      } else
      if (info->src.format == PIPE_FORMAT_A8_UNORM) {
         mask = 0xff000000;
         BEGIN_NVC0(push, NVC0_2D(BETA4), 2);
         PUSH_DATA (push, mask);
         PUSH_DATA (push, NVC0_2D_OPERATION_SRCCOPY_PREMULT);
      }
   }

   if (src->ms_x > dst->ms_x || src->ms_y > dst->ms_y) {
      /* ms_x is always >= ms_y */
      du_dx <<= src->ms_x - dst->ms_x;
      dv_dy <<= src->ms_y - dst->ms_y;
   } else {
      du_dx >>= dst->ms_x - src->ms_x;
      dv_dy >>= dst->ms_y - src->ms_y;
   }

   srcx = (int64_t)(info->src.box.x + srcx_adj) << (src->ms_x + 32);
   srcy = (int64_t)(info->src.box.y + srcy_adj) << (src->ms_y + 32);

   if (src->base.base.nr_samples > dst->base.base.nr_samples) {
      /* center src coorinates for proper MS resolve filtering */
      srcx += (int64_t)1 << (src->ms_x + 31);
      srcy += (int64_t)1 << (src->ms_y + 31);
   }

   dstx = info->dst.box.x << dst->ms_x;
   dsty = info->dst.box.y << dst->ms_y;

   dstw = info->dst.box.width << dst->ms_x;
   dsth = info->dst.box.height << dst->ms_y;

   if (dstx < 0) {
      dstw += dstx;
      srcx -= du_dx * dstx;
      dstx = 0;
   }
   if (dsty < 0) {
      dsth += dsty;
      srcy -= dv_dy * dsty;
      dsty = 0;
   }

   IMMED_NVC0(push, NVC0_2D(BLIT_CONTROL), mode);
   BEGIN_NVC0(push, NVC0_2D(BLIT_DST_X), 4);
   PUSH_DATA (push, dstx);
   PUSH_DATA (push, dsty);
   PUSH_DATA (push, dstw);
   PUSH_DATA (push, dsth);
   BEGIN_NVC0(push, NVC0_2D(BLIT_DU_DX_FRACT), 4);
   PUSH_DATA (push, du_dx);
   PUSH_DATA (push, du_dx >> 32);
   PUSH_DATA (push, dv_dy);
   PUSH_DATA (push, dv_dy >> 32);

   BCTX_REFN(nvc0->bufctx, 2D, &dst->base, WR);
   BCTX_REFN(nvc0->bufctx, 2D, &src->base, RD);
   nouveau_pushbuf_bufctx(nvc0->base.pushbuf, nvc0->bufctx);
   if (nouveau_pushbuf_validate(nvc0->base.pushbuf))
      return;

   for (i = 0; i < info->dst.box.depth; ++i) {
      if (i > 0) {
         /* no scaling in z-direction possible for eng2d blits */
         if (dst->layout_3d) {
            BEGIN_NVC0(push, NVC0_2D(DST_LAYER), 1);
            PUSH_DATA (push, info->dst.box.z + i);
         } else {
            const unsigned z = info->dst.box.z + i;
            BEGIN_NVC0(push, NVC0_2D(DST_ADDRESS_HIGH), 2);
            PUSH_DATAh(push, dst->base.address + z * dst->layer_stride);
            PUSH_DATA (push, dst->base.address + z * dst->layer_stride);
         }
         if (src->layout_3d) {
            /* not possible because of depth tiling */
            assert(0);
         } else {
            const unsigned z = info->src.box.z + i;
            BEGIN_NVC0(push, NVC0_2D(SRC_ADDRESS_HIGH), 2);
            PUSH_DATAh(push, src->base.address + z * src->layer_stride);
            PUSH_DATA (push, src->base.address + z * src->layer_stride);
         }
         BEGIN_NVC0(push, NVC0_2D(BLIT_SRC_Y_INT), 1); /* trigger */
         PUSH_DATA (push, srcy >> 32);
      } else {
         BEGIN_NVC0(push, NVC0_2D(BLIT_SRC_X_FRACT), 4);
         PUSH_DATA (push, srcx);
         PUSH_DATA (push, srcx >> 32);
         PUSH_DATA (push, srcy);
         PUSH_DATA (push, srcy >> 32);
      }
   }
   nvc0_resource_validate(&dst->base, NOUVEAU_BO_WR);
   nvc0_resource_validate(&src->base, NOUVEAU_BO_RD);

   nouveau_bufctx_reset(nvc0->bufctx, NVC0_BIND_2D);

   if (info->scissor_enable)
      IMMED_NVC0(push, NVC0_2D(CLIP_ENABLE), 0);
   if (mask != 0xffffffff)
      IMMED_NVC0(push, NVC0_2D(OPERATION), NVC0_2D_OPERATION_SRCCOPY);
   if (nvc0->cond_query && info->render_condition_enable)
      IMMED_NVC0(push, NVC0_2D(COND_MODE), NVC0_2D_COND_MODE_ALWAYS);
}

static void
nvc0_blit(struct pipe_context *pipe, const struct pipe_blit_info *info)
{
   struct nvc0_context *nvc0 = nvc0_context(pipe);
   boolean eng3d = FALSE;

   if (util_format_is_depth_or_stencil(info->dst.resource->format)) {
      if (!(info->mask & PIPE_MASK_ZS))
         return;
      if (info->dst.resource->format == PIPE_FORMAT_Z32_FLOAT ||
          info->dst.resource->format == PIPE_FORMAT_Z32_FLOAT_S8X24_UINT)
         eng3d = TRUE;
      if (info->filter != PIPE_TEX_FILTER_NEAREST)
         eng3d = TRUE;
   } else {
      if (!(info->mask & PIPE_MASK_RGBA))
         return;
      if (info->mask != PIPE_MASK_RGBA)
         eng3d = TRUE;
   }

   if (nv50_miptree(info->src.resource)->layout_3d) {
      eng3d = TRUE;
   } else
   if (info->src.box.depth != info->dst.box.depth) {
      eng3d = TRUE;
      debug_printf("blit: cannot filter array or cube textures in z direction");
   }

   if (!eng3d && info->dst.format != info->src.format) {
      if (!nv50_2d_dst_format_faithful(info->dst.format)) {
         eng3d = TRUE;
      } else
      if (!nv50_2d_src_format_faithful(info->src.format)) {
         if (!util_format_is_luminance(info->src.format)) {
            if (util_format_is_intensity(info->src.format))
               eng3d = info->src.format != PIPE_FORMAT_I8_UNORM;
            else
            if (!nv50_2d_dst_format_ops_supported(info->dst.format))
               eng3d = TRUE;
            else
               eng3d = !nv50_2d_format_supported(info->src.format);
         }
      } else
      if (util_format_is_luminance_alpha(info->src.format))
         eng3d = TRUE;
   }

   if (info->src.resource->nr_samples == 8 &&
       info->dst.resource->nr_samples <= 1)
      eng3d = TRUE;
#if 0
   /* FIXME: can't make this work with eng2d anymore, at least not on nv50 */
   if (info->src.resource->nr_samples > 1 ||
       info->dst.resource->nr_samples > 1)
      eng3d = TRUE;
#endif
   /* FIXME: find correct src coordinates adjustments */
   if ((info->src.box.width !=  info->dst.box.width &&
        info->src.box.width != -info->dst.box.width) ||
       (info->src.box.height !=  info->dst.box.height &&
        info->src.box.height != -info->dst.box.height))
      eng3d = TRUE;

   if (!eng3d)
      nvc0_blit_eng2d(nvc0, info);
   else
      nvc0_blit_3d(nvc0, info);

   NOUVEAU_DRV_STAT(&nvc0->screen->base, tex_blit_count, 1);
}

static void
nvc0_flush_resource(struct pipe_context *ctx,
                    struct pipe_resource *resource)
{
}

boolean
nvc0_blitter_create(struct nvc0_screen *screen)
{
   screen->blitter = CALLOC_STRUCT(nvc0_blitter);
   if (!screen->blitter) {
      NOUVEAU_ERR("failed to allocate blitter struct\n");
      return FALSE;
   }
   screen->blitter->screen = screen;

   pipe_mutex_init(screen->blitter->mutex);

   nvc0_blitter_make_vp(screen->blitter);
   nvc0_blitter_make_sampler(screen->blitter);

   return TRUE;
}

void
nvc0_blitter_destroy(struct nvc0_screen *screen)
{
   struct nvc0_blitter *blitter = screen->blitter;
   unsigned i, m;

   for (i = 0; i < NV50_BLIT_MAX_TEXTURE_TYPES; ++i) {
      for (m = 0; m < NV50_BLIT_MODES; ++m) {
         struct nvc0_program *prog = blitter->fp[i][m];
         if (prog) {
            nvc0_program_destroy(NULL, prog);
            FREE((void *)prog->pipe.tokens);
            FREE(prog);
         }
      }
   }

   FREE(blitter);
}

boolean
nvc0_blitctx_create(struct nvc0_context *nvc0)
{
   nvc0->blit = CALLOC_STRUCT(nvc0_blitctx);
   if (!nvc0->blit) {
      NOUVEAU_ERR("failed to allocate blit context\n");
      return FALSE;
   }

   nvc0->blit->nvc0 = nvc0;

   nvc0->blit->rast.pipe.half_pixel_center = 1;

   return TRUE;
}

void
nvc0_blitctx_destroy(struct nvc0_context *nvc0)
{
   if (nvc0->blit)
      FREE(nvc0->blit);
}

void
nvc0_init_surface_functions(struct nvc0_context *nvc0)
{
   struct pipe_context *pipe = &nvc0->base.pipe;

   pipe->resource_copy_region = nvc0_resource_copy_region;
   pipe->blit = nvc0_blit;
   pipe->flush_resource = nvc0_flush_resource;
   pipe->clear_render_target = nvc0_clear_render_target;
   pipe->clear_depth_stencil = nvc0_clear_depth_stencil;
}