/*
 * Copyright © 2011,2012,2013 Intel Corporation
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
 *
 * Authors:
 *    Chris Wilson <chris@chris-wilson.co.uk>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sna.h"
#include "sna_render.h"
#include "sna_render_inline.h"
#include "gen4_source.h"
#include "gen4_render.h"

bool
gen4_channel_init_solid(struct sna *sna,
			struct sna_composite_channel *channel,
			uint32_t color)
{
	channel->filter = PictFilterNearest;
	channel->repeat = RepeatNormal;
	channel->is_affine = true;
	channel->is_solid  = true;
	channel->is_opaque = (color >> 24) == 0xff;
	channel->transform = NULL;
	channel->width  = 1;
	channel->height = 1;
	channel->pict_format = PICT_a8r8g8b8;
	channel->card_format = GEN4_SURFACEFORMAT_B8G8R8A8_UNORM;

	channel->bo = sna_render_get_solid(sna, color);

	channel->scale[0]  = channel->scale[1]  = 1;
	channel->offset[0] = channel->offset[1] = 0;
	return channel->bo != NULL;
}

bool
gen4_channel_init_linear(struct sna *sna,
			 PicturePtr picture,
			 struct sna_composite_channel *channel,
			 int x, int y,
			 int w, int h,
			 int dst_x, int dst_y)
{
	PictLinearGradient *linear =
		(PictLinearGradient *)picture->pSourcePict;
	pixman_fixed_t tx, ty;
	float x0, y0, sf;
	float dx, dy;

	DBG(("%s: p1=(%f, %f), p2=(%f, %f), src=(%d, %d), dst=(%d, %d), size=(%d, %d)\n",
	     __FUNCTION__,
	     pixman_fixed_to_double(linear->p1.x), pixman_fixed_to_double(linear->p1.y),
	     pixman_fixed_to_double(linear->p2.x), pixman_fixed_to_double(linear->p2.y),
	     x, y, dst_x, dst_y, w, h));

	if (linear->p2.x == linear->p1.x && linear->p2.y == linear->p1.y)
		return 0;

	if (!sna_transform_is_affine(picture->transform)) {
		DBG(("%s: fallback due to projective transform\n",
		     __FUNCTION__));
		return sna_render_picture_fixup(sna, picture, channel,
						x, y, w, h, dst_x, dst_y);
	}

	channel->bo = sna_render_get_gradient(sna, (PictGradient *)linear);
	if (!channel->bo)
		return 0;

	channel->filter = PictFilterNearest;
	channel->repeat = picture->repeat ? picture->repeatType : RepeatNone;
	channel->width  = channel->bo->pitch / 4;
	channel->height = 1;
	channel->pict_format = PICT_a8r8g8b8;
	channel->card_format = GEN4_SURFACEFORMAT_B8G8R8A8_UNORM;
	channel->is_linear = 1;
	channel->is_affine = 1;

	channel->scale[0]  = channel->scale[1]  = 1;
	channel->offset[0] = channel->offset[1] = 0;

	if (sna_transform_is_translation(picture->transform, &tx, &ty)) {
		dx = pixman_fixed_to_double(linear->p2.x - linear->p1.x);
		dy = pixman_fixed_to_double(linear->p2.y - linear->p1.y);

		x0 = pixman_fixed_to_double(linear->p1.x);
		y0 = pixman_fixed_to_double(linear->p1.y);

		if (tx | ty) {
			x0 -= pixman_fixed_to_double(tx);
			y0 -= pixman_fixed_to_double(ty);
		}
	} else {
		struct pixman_f_vector p1, p2;
		struct pixman_f_transform m, inv;

		pixman_f_transform_from_pixman_transform(&m, picture->transform);
		DBG(("%s: transform = [%f %f %f, %f %f %f, %f %f %f]\n",
		     __FUNCTION__,
		     m.m[0][0], m.m[0][1], m.m[0][2],
		     m.m[1][0], m.m[1][1], m.m[1][2],
		     m.m[2][0], m.m[2][1], m.m[2][2]));
		if (!pixman_f_transform_invert(&inv, &m))
			return 0;

		p1.v[0] = pixman_fixed_to_double(linear->p1.x);
		p1.v[1] = pixman_fixed_to_double(linear->p1.y);
		p1.v[2] = 1.;
		pixman_f_transform_point(&inv, &p1);

		p2.v[0] = pixman_fixed_to_double(linear->p2.x);
		p2.v[1] = pixman_fixed_to_double(linear->p2.y);
		p2.v[2] = 1.;
		pixman_f_transform_point(&inv, &p2);

		DBG(("%s: untransformed: p1=(%f, %f, %f), p2=(%f, %f, %f)\n",
		     __FUNCTION__,
		     p1.v[0], p1.v[1], p1.v[2],
		     p2.v[0], p2.v[1], p2.v[2]));

		dx = p2.v[0] - p1.v[0];
		dy = p2.v[1] - p1.v[1];

		x0 = p1.v[0];
		y0 = p1.v[1];
	}

	sf = dx*dx + dy*dy;
	dx /= sf;
	dy /= sf;

	channel->u.linear.dx = dx;
	channel->u.linear.dy = dy;
	channel->u.linear.offset = -dx*(x0+dst_x-x) + -dy*(y0+dst_y-y);

	channel->embedded_transform.matrix[0][0] = pixman_double_to_fixed(dx);
	channel->embedded_transform.matrix[0][1] = pixman_double_to_fixed(dy);
	channel->embedded_transform.matrix[0][2] = pixman_double_to_fixed(channel->u.linear.offset);

	channel->embedded_transform.matrix[1][0] = 0;
	channel->embedded_transform.matrix[1][1] = 0;
	channel->embedded_transform.matrix[1][2] = pixman_double_to_fixed(.5);

	channel->embedded_transform.matrix[2][0] = 0;
	channel->embedded_transform.matrix[2][1] = 0;
	channel->embedded_transform.matrix[2][2] = pixman_fixed_1;

	channel->transform = &channel->embedded_transform;

	DBG(("%s: dx=%f, dy=%f, offset=%f\n",
	     __FUNCTION__, dx, dy, channel->u.linear.offset));

	return channel->bo != NULL;
}
