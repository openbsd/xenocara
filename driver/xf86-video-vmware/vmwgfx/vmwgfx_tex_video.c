/*
 * Copyright 2009-2011 VMWare, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 * Author: Zack Rusin <zackr@vmware.com>
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "vmwgfx_driver.h"
#include "vmwgfx_drmi.h"
#include "vmwgfx_saa.h"
#include "../src/common_compat.h"

#include <xf86xv.h>
#include <X11/extensions/Xv.h>
#include <fourcc.h>
#include <xa_tracker.h>
#include <xa_context.h>
#include <math.h>

static CONST_ABI_16_0 char xv_adapt_name[] = "XA G3D Textured Video";

/*Xxx get these from pipe's texture limits */
#define IMAGE_MAX_WIDTH		2048
#define IMAGE_MAX_HEIGHT	2048

#define RES_720P_X 1280
#define RES_720P_Y 720


#define MAKE_ATOM(a) MakeAtom(a, sizeof(a) - 1, TRUE)

/*
 * ITU-R BT.601, BT.709 transfer matrices.
 * [R', G', B'] values are in the range [0, 1], Y' is in the range [0,1]
 * and [Pb, Pr] components are in the range [-0.5, 0.5].
 *
 * The matrices are transposed to fit the xa conversion matrix format.
 */

static const float bt_601[] = {
    1.f, 1.f, 1.f, 0.f,
    0.f, -0.344136f, 1.772f, 0.f,
    1.402f,  -0.714136f, 0.f, 0.f
};

static const float bt_709[] = {
    1.f, 1.f, 1.f, 0.f,
    0.f, -0.187324f, 1.8556f, 0.f,
    1.5748f, -0.468124f, 0.f, 0.f
};

static Atom xvBrightness, xvContrast, xvSaturation, xvHue;
static CONST_ABI_16_TO_19 char xv_brightness_name[] = "XV_BRIGHTNESS";
static CONST_ABI_16_TO_19 char xv_contrast_name[] = "XV_CONTRAST";
static CONST_ABI_16_TO_19 char xv_saturation_name[] = "XV_SATURATION";
static CONST_ABI_16_TO_19 char xv_hue_name[] = "XV_HUE";
static CONST_ABI_16_TO_19 char xv_image_name[] = "XV_IMAGE";

#define NUM_TEXTURED_ATTRIBUTES 4
static const XF86AttributeRec TexturedAttributes[NUM_TEXTURED_ATTRIBUTES] = {
    {XvSettable | XvGettable, -1000, 1000, xv_brightness_name},
    {XvSettable | XvGettable, -1000, 1000, xv_contrast_name},
    {XvSettable | XvGettable, -1000, 1000, xv_saturation_name},
    {XvSettable | XvGettable, -1000, 1000, xv_hue_name}
};

#define NUM_FORMATS 3
static XF86VideoFormatRec Formats[NUM_FORMATS] = {
   {15, TrueColor}, {16, TrueColor}, {24, TrueColor}
};

static XF86VideoEncodingRec DummyEncoding[1] = {
   {
      0,
      xv_image_name,
      IMAGE_MAX_WIDTH, IMAGE_MAX_HEIGHT,
      {1, 1}
   }
};

#define NUM_IMAGES 3
static XF86ImageRec Images[NUM_IMAGES] = {
   XVIMAGE_UYVY,
   XVIMAGE_YUY2,
   XVIMAGE_YV12,
};

struct xorg_xv_port_priv {
    struct xa_tracker *xat;
    struct xa_context *r;
    struct xa_fence *fence;

    RegionRec clip;

    int brightness;
    int contrast;
    int saturation;
    int hue;

    int current_set;
    struct xa_surface *yuv[2][3];

    struct xa_surface *bounce;
    struct xa_box bounce_box;
    struct xa_picture *src_pic;
    struct xa_picture *dst_pic;
    struct xa_composite *comp;

    int drm_fd;

    Bool hdtv;
    float uv_offset;
    float uv_scale;
    float y_offset;
    float y_scale;
    float rgb_offset;
    float rgb_scale;
    float sinhue;
    float coshue;
    float cm[16];
};

/*
 * vmwgfx_update_conversion_matrix - Compute the effective color conversion
 * matrix.
 *
 * Applies yuv- and resulting rgb scales and offsets to compute the correct
 * color conversion matrix. These scales and offsets are properties of the
 * video stream and can be adjusted using XV properties as well.
 */
static void
vmwgfx_update_conversion_matrix(struct xorg_xv_port_priv *priv)
{
    int i;
    float *cm = priv->cm;
    static const float *bt;

    bt = (priv->hdtv) ? bt_709 : bt_601;

    memcpy(cm, bt, sizeof(bt_601));

    /*
     * Apply hue rotation
     */
    cm[4] = priv->coshue * bt[4] - priv->sinhue * bt[8];
    cm[8] = priv->sinhue * bt[4] + priv->coshue * bt[8];
    cm[5] = priv->coshue * bt[5] - priv->sinhue * bt[9];
    cm[9] = priv->sinhue * bt[5] + priv->coshue * bt[9];
    cm[6] = priv->coshue * bt[6] - priv->sinhue * bt[10];
    cm[10] = priv->sinhue * bt[6] + priv->coshue * bt[10];

    /*
     * Adjust for yuv scales in input and rgb scale in the converted output.
     */
    for(i = 0; i < 3; ++i) {
	cm[i] *= (priv->y_scale*priv->rgb_scale);
	cm[i+4] *= (priv->uv_scale*priv->rgb_scale);
	cm[i+8] *= (priv->uv_scale*priv->rgb_scale);
    }

    /*
     * Adjust for yuv offsets in input and rgb offset in the converted output.
     */
    for (i = 0; i < 3; ++i)
	cm[i+12] = -cm[i]*priv->y_offset - (cm[i+4] + cm[i+8])*priv->uv_offset
	    - priv->rgb_offset*priv->rgb_scale;

    /*
     * Alpha is 1, unconditionally.
     */
    cm[15] = 1.f;
}

/**
 * vmwgfx_video_free_comp - free members used for composite bounce blit
 *
 * @priv: Pointer to the port private
 *
 * Frees any port priv resources allocated for a composite bounce blit.
 */
static void
vmwgfx_video_free_comp(struct xorg_xv_port_priv *priv)
{
    if (priv->dst_pic)
	free(priv->dst_pic);
    if (priv->src_pic)
	free(priv->src_pic);
    if (priv->comp)
	free(priv->comp);

    priv->dst_pic = NULL;
    priv->src_pic = NULL;
    priv->comp = NULL;
}

static void
stop_video(ScrnInfoPtr pScrn, pointer data, Bool shutdown)
{
   struct xorg_xv_port_priv *priv = (struct xorg_xv_port_priv *)data;
   int i, j;

   REGION_EMPTY(pScrn->pScreen, &priv->clip);
   if (shutdown) {

       /*
	* No need to destroy the xa context or xa tracker since
	* they are copied from the screen resources.
	*/

       xa_fence_destroy(priv->fence);
       priv->fence = NULL;
       vmwgfx_video_free_comp(priv);

       for (i=0; i<3; ++i) {
	   for (j=0; j<2; ++j) {
	       if (priv->yuv[i]) {
		   xa_surface_destroy(priv->yuv[j][i]);
		   priv->yuv[j][i] = NULL;
	       }
	       if (priv->bounce) {
		   xa_surface_destroy(priv->bounce);
		   priv->bounce = NULL;
	       }
	   }
       }
   }
}

static int
set_port_attribute(ScrnInfoPtr pScrn,
                   Atom attribute, INT32 value, pointer data)
{
   struct xorg_xv_port_priv *priv = (struct xorg_xv_port_priv *)data;

   if (attribute == xvBrightness) {
       if ((value < -1000) || (value > 1000))
	   return BadValue;

       priv->brightness = value;
       priv->y_offset = -((float) value)/1000.f;

   } else if (attribute == xvContrast) {
       if ((value < -1000) || (value > 1000))
	   return BadValue;

       priv->contrast = value;
       priv->rgb_scale = ((float) value + 1000.f)/1000.f;

   } else if (attribute == xvSaturation) {
       if ((value < -1000) || (value > 1000))
	   return BadValue;

       priv->saturation = value;
       priv->uv_scale = ((float) value + 1000.f)/1000.f;

   } else if (attribute == xvHue) {
       double hue_angle;

       if ((value < -1000) || (value > 1000))
	   return BadValue;

       priv->hue = value;
       hue_angle = (double) value * M_PI / 1000.;
       priv->sinhue = sin(hue_angle);
       priv->coshue = cos(hue_angle);

   } else
      return BadMatch;

   vmwgfx_update_conversion_matrix(priv);
   return Success;
}

static int
get_port_attribute(ScrnInfoPtr pScrn,
                   Atom attribute, INT32 * value, pointer data)
{
    struct xorg_xv_port_priv *priv = (struct xorg_xv_port_priv *)data;

    if (attribute == xvBrightness)
	*value = priv->brightness;
    else if (attribute == xvContrast)
	*value = priv->contrast;
    else if (attribute == xvSaturation)
	*value = priv->saturation;
    else if (attribute == xvHue)
	*value = priv->hue;
    else
	return BadMatch;

    return Success;
}

static void
query_best_size(ScrnInfoPtr pScrn,
                Bool motion,
                short vid_w, short vid_h,
                short drw_w, short drw_h,
                unsigned int *p_w, unsigned int *p_h, pointer data)
{
   if (vid_w > (drw_w << 1))
      drw_w = vid_w >> 1;
   if (vid_h > (drw_h << 1))
      drw_h = vid_h >> 1;

   *p_w = drw_w;
   *p_h = drw_h;
}

static int
check_yuv_surfaces(struct xorg_xv_port_priv *priv,  int id,
		   int width, int height)
{
    struct xa_surface **yuv = priv->yuv[priv->current_set];
    int ret = 0;
    int i;

    for (i=0; i<3; ++i) {

	/*
	 * Adjust u,v texture size and DMA buffer to what's required by
	 * the format.
	 */
	if (i == 1) {
	    switch(id) {
	    case FOURCC_YV12:
		height /= 2;
		/* Fall through */
	    case FOURCC_YUY2:
	    case FOURCC_UYVY:
		width /= 2;
		break;
	    default:
		break;
	    }
	}

	if (!yuv[i])
	    yuv[i] = xa_surface_create(priv->xat, width, height, 8,
				       xa_type_yuv_component,
				       xa_format_unknown, 0);
	else
	    ret = xa_surface_redefine(yuv[i], width, height, 8,
				      xa_type_yuv_component,
				      xa_format_unknown, 0, 0);
	if (ret || !yuv[i])
	    return BadAlloc;

    }
    return Success;
}


/**
 * vmwgfx_video_setup_comp - Set up port priv members for a composite
 * bounce blit
 *
 * @priv: Pointer to the port priv.
 * @format: XA format of the destination drawable surface.
 *
 * Tries to allocate and set up port priv resources to perform a composite
 * bounce blit (except the bounce surface itself). On failure, all resources
 * are freed. On success, TRUE is returned and @priv::comp is non-NULL. If
 * resources are already allocated, update them for the current @format.
 */
static Bool
vmwgfx_video_setup_comp(struct xorg_xv_port_priv *priv, enum xa_formats format)
{
    const struct xa_composite_allocation *alloc;
    struct xa_picture *pic;

    if (priv->comp) {
	if (priv->src_pic->pict_format != format) {
	    priv->src_pic->pict_format = format;
	    priv->dst_pic->pict_format = format;
	    if (xa_composite_check_accelerated(priv->comp) != XA_ERR_NONE) {
		vmwgfx_video_free_comp(priv);
		return FALSE;
	    }
	}
	return TRUE;
    }

    alloc = xa_composite_allocation();
    priv->comp = calloc(1, alloc->xa_composite_size);
    priv->src_pic = calloc(1, alloc->xa_picture_size);
    priv->dst_pic = calloc(1, alloc->xa_picture_size);

    if (!priv->comp || !priv->src_pic || !priv->dst_pic) {
	vmwgfx_video_free_comp(priv);
	return FALSE;
    }

    pic = priv->src_pic;
    pic->pict_format = format;
    pic->wrap = xa_wrap_clamp_to_border;
    pic->filter = xa_filter_linear;

    *priv->dst_pic = *pic;

    priv->comp->src = priv->src_pic;
    priv->comp->dst = priv->dst_pic;
    priv->comp->op = xa_op_src;
    priv->comp->no_solid = 1;

    if (xa_composite_check_accelerated(priv->comp) != XA_ERR_NONE) {
	vmwgfx_video_free_comp(priv);
	return FALSE;
    }

    return TRUE;
}

/**
 * vmwgfx_video_setup_coord_matrix - Set up a bounce - to destination
 * transformation matrix
 *
 * @src_x: Upper left corner of source image as given to putImage.
 * @src_y: Upper left corner of source image as given to putImage.
 * @src_w: Width of source image.
 * @src_h: Height of source image.
 * @dst_x: Upper left corner of destination image as given to putImage.
 * @dst_y: Upper left corner of destination image as given to putImage.
 * @dst_w: Width of destination image.
 * @dst_h: Height of destination image.
 * @bounce_w: Width of bounce surface.
 * @bounce_h: Height of bounce surface.
 * @mat: Pointer to transformation matrix.
 *
 * Computes a transformation matrix that can be used by the XA composite API
 * to transform between the destination coordinate space and the bounce
 * surface coordinate space. Scaling and translation.
 */
static void
vmwgfx_video_setup_coord_matrix(int src_x, int src_y, int src_w, int src_h,
				int dst_x, int dst_y, int dst_w, int dst_h,
				int bounce_w, int bounce_h,
				float *mat)
{
    if (dst_w == 0 || dst_h == 0 || src_w == 0 || src_h == 0) {
	mat[0] = mat[6] = 0.;
	mat[4] = mat[7] = 0.;
	mat[8] = 1.f;
	return;
    }

    mat[0] = (float) bounce_w / (float) dst_w;
    mat[6] = (float) src_x * (float) bounce_w / (float) src_w -
      (float) dst_x * mat[0];
    mat[4] = (float) bounce_h / (float) dst_h;
    mat[7] = (float) src_y * (float) bounce_h / (float) src_h -
      (float) dst_y * mat[4];
    mat[8] = 1.f;
}

/**
 * vmwgfx_video_bounce_surface: (Re)Allocate a bounce surface if desired.
 *
 * @priv: Pointer to the port private.
 * @d_with: Width of destination image.
 * @d_height: Height of destination image.
 * @width: Width of source video image.
 * @height: Height of source video image.
 * @dst_format: Format of destination drawable surface.
 *
 * If the destination video image is a suitable factor larger than the source
 * video image. Allocate a bounce RGB surface that will be the target of the
 * more expensive color conversion blit, and source of the scaling blit.
 */
static Bool
vmwgfx_video_bounce_surface(struct xorg_xv_port_priv *priv,
			    int d_width, int d_height,
			    int width, int height,
			    enum xa_formats dst_format)
{
    float bounce_area, dst_area;

    if (d_width < width)
	width = d_width;

    if (d_height < height)
	height = d_height;

    bounce_area = (float) width * (float) height;
    dst_area = (float) d_width * (float) d_height;

    if (dst_area > bounce_area * 1.5f &&
	vmwgfx_video_setup_comp(priv, dst_format)) {

	if (!priv->bounce) {
	    priv->bounce = xa_surface_create(priv->xat, width, height,
					     xa_format_depth(dst_format),
					     xa_format_type(dst_format),
					     dst_format, XA_FLAG_RENDER_TARGET);
	} else {
	    if (xa_surface_redefine(priv->bounce, width, height,
				    xa_format_depth(dst_format),
				    xa_format_type(dst_format),
				    dst_format,
				    XA_FLAG_RENDER_TARGET, 0) != XA_ERR_NONE) {
		xa_surface_destroy(priv->bounce);
		priv->bounce = NULL;
	    }
	}

    } else {
	xa_surface_destroy(priv->bounce);
	priv->bounce = NULL;
    }

    if (priv->bounce) {
	priv->bounce_box.x1 = 0;
	priv->bounce_box.x2 = width;
	priv->bounce_box.y1 = 0;
	priv->bounce_box.y2 = height;
    }

    return TRUE;
}

static int
query_image_attributes(ScrnInfoPtr pScrn,
                       int id,
                       unsigned short *w, unsigned short *h,
                       int *pitches, int *offsets)
{
   int size, tmp;

   if (*w > IMAGE_MAX_WIDTH)
      *w = IMAGE_MAX_WIDTH;
   if (*h > IMAGE_MAX_HEIGHT)
      *h = IMAGE_MAX_HEIGHT;

   *w = (*w + 1) & ~1;
   if (offsets)
      offsets[0] = 0;

   switch (id) {
   case FOURCC_YV12:
      *h = (*h + 1) & ~1;
      size = (*w + 3) & ~3;
      if (pitches) {
         pitches[0] = size;
      }
      size *= *h;
      if (offsets) {
         offsets[1] = size;
      }
      tmp = ((*w >> 1) + 3) & ~3;
      if (pitches) {
         pitches[1] = pitches[2] = tmp;
      }
      tmp *= (*h >> 1);
      size += tmp;
      if (offsets) {
         offsets[2] = size;
      }
      size += tmp;
      break;
   case FOURCC_UYVY:
   case FOURCC_YUY2:
   default:
      size = *w << 1;
      if (pitches)
	 pitches[0] = size;
      size *= *h;
      break;
   }

   return size;
}

static int
copy_packed_data(ScrnInfoPtr pScrn,
                 struct xorg_xv_port_priv *port,
                 int id,
                 unsigned char *buf,
                 int left,
                 int top,
                 unsigned short w, unsigned short h)
{
    int i;
   struct xa_surface **yuv = port->yuv[port->current_set];
   char *ymap, *vmap, *umap;
   unsigned char _y1, _y2, u, v;
   int yidx, uidx, vidx;
   int y_array_size = w * h;
   int ret = BadAlloc;

   ymap = xa_surface_map(port->r, yuv[0], XA_MAP_WRITE);
   if (!ymap)
       return BadAlloc;
   umap = xa_surface_map(port->r, yuv[1], XA_MAP_WRITE);
   if (!umap)
       goto out_no_umap;
   vmap = xa_surface_map(port->r, yuv[2], XA_MAP_WRITE);
   if (!vmap)
       goto out_no_vmap;


   yidx = uidx = vidx = 0;

   switch (id) {
   case FOURCC_YV12: {
      int pitches[3], offsets[3];
      unsigned char *yp, *up, *vp;
      query_image_attributes(pScrn, FOURCC_YV12,
                             &w, &h, pitches, offsets);

      yp = buf + offsets[0];
      vp = buf + offsets[1];
      up = buf + offsets[2];
      memcpy(ymap, yp, w*h);
      memcpy(vmap, vp, w*h/4);
      memcpy(umap, up, w*h/4);
      break;
   }
   case FOURCC_UYVY:
      for (i = 0; i < y_array_size; i +=2 ) {
         /* extracting two pixels */
         u  = buf[0];
         _y1 = buf[1];
         v  = buf[2];
         _y2 = buf[3];
         buf += 4;

         ymap[yidx++] = _y1;
         ymap[yidx++] = _y2;
         umap[uidx++] = u;
         vmap[vidx++] = v;
      }
      break;
   case FOURCC_YUY2:
      for (i = 0; i < y_array_size; i +=2 ) {
         /* extracting two pixels */
         _y1 = buf[0];
         u  = buf[1];
         _y2 = buf[2];
         v  = buf[3];

         buf += 4;

         ymap[yidx++] = _y1;
         ymap[yidx++] = _y2;
         umap[uidx++] = u;
         vmap[vidx++] = v;
      }
      break;
   default:
       ret = BadAlloc;
       break;
   }

   ret = Success;
   xa_surface_unmap(yuv[2]);
  out_no_vmap:
   xa_surface_unmap(yuv[1]);
  out_no_umap:
   xa_surface_unmap(yuv[0]);

   return ret;
}


static int
display_video(ScreenPtr pScreen, struct xorg_xv_port_priv *pPriv, int id,
              RegionPtr dstRegion,
              int src_x, int src_y, int src_w, int src_h,
              int dst_x, int dst_y, int dst_w, int dst_h,
	      int width, int height,
              PixmapPtr pPixmap)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pPixmap);
    Bool hdtv;
    RegionRec reg;
    int ret = BadAlloc;
    int blit_ret;

    REGION_NULL(pScreen, &reg);

    if (!vmwgfx_hw_accel_validate(pPixmap, 0, XA_FLAG_RENDER_TARGET, 0, &reg))
	goto out_no_dst;

   hdtv = ((src_w >= RES_720P_X) && (src_h >= RES_720P_Y));
   if (hdtv != pPriv->hdtv) {
       pPriv->hdtv = hdtv;
       vmwgfx_update_conversion_matrix(pPriv);
   }

#ifdef COMPOSITE

   /*
    * For redirected windows, we need to fix up the destination coordinates.
    */

   REGION_TRANSLATE(pScreen, dstRegion, -pPixmap->screen_x,
		    -pPixmap->screen_y);
   dst_x -= pPixmap->screen_x;
   dst_y -= pPixmap->screen_y;
#endif

   /*
    * Throttle on previous blit.
    */

   if (pPriv->fence) {
       (void) xa_fence_wait(pPriv->fence, 1000000000ULL);
       xa_fence_destroy(pPriv->fence);
       pPriv->fence = NULL;
   }

   (void) vmwgfx_video_bounce_surface(pPriv, dst_w, dst_h, width, height,
				      xa_surface_format(vpix->hw));

   DamageRegionAppend(&pPixmap->drawable, dstRegion);

   if (pPriv->bounce) {
       BoxPtr b;
       int i;

       /*
	* If we have a bounce buffer, First perform a (potentially down-
	* scaling) color conversion blit with an expensive shader.
	*/
       blit_ret = xa_yuv_planar_blit(pPriv->r, 0, 0, src_w, src_h,
				     src_x, src_y,
				     pPriv->bounce_box.x2,
				     pPriv->bounce_box.y2,
				     &pPriv->bounce_box, 1,
				     pPriv->cm,
				     pPriv->bounce,
				     pPriv->yuv[pPriv->current_set]);

       if (blit_ret)
	   goto out_blit;

       /*
	* Then an upscaling blit with a cheap shader. Note that we never
	* scale up a dimenstion that was previously downscaled in the color
	* conversion blit.
	*/
       pPriv->src_pic->srf = pPriv->bounce;
       pPriv->dst_pic->srf = vpix->hw;
       vmwgfx_video_setup_coord_matrix(src_x, src_y, src_w, src_h,
				       dst_x, dst_y, dst_w, dst_h,
				       pPriv->bounce_box.x2,
				       pPriv->bounce_box.y2,
				       pPriv->src_pic->transform);
       pPriv->src_pic->has_transform = 1;

       if (xa_composite_prepare(pPriv->r, pPriv->comp) != XA_ERR_NONE) {
	   blit_ret = 1;
	   goto out_blit;
       }

       b =  REGION_RECTS(dstRegion);
       for (i = 0; i < REGION_NUM_RECTS(dstRegion); ++i, ++b) {
	       xa_composite_rect(pPriv->r, b->x1, b->y1, 0, 0,
				 b->x1, b->y1, b->x2 - b->x1,
				 b->y2 - b->y1);
       }

       xa_composite_done(pPriv->r);
   } else {
       /*
	* Perform color conversion and scaling in the same blit.
	*/
       blit_ret = xa_yuv_planar_blit(pPriv->r, src_x, src_y, src_w, src_h,
				     dst_x, dst_y, dst_w, dst_h,
				     (struct xa_box *)REGION_RECTS(dstRegion),
				     REGION_NUM_RECTS(dstRegion),
				     pPriv->cm,
				     vpix->hw,
				     pPriv->yuv[pPriv->current_set ]);
   }

  out_blit:

   saa_pixmap_dirty(pPixmap, TRUE, dstRegion);
   DamageRegionProcessPending(&pPixmap->drawable);
   ret = Success;

   if (!blit_ret) {
       ret = Success;
       pPriv->fence = xa_fence_get(pPriv->r);
   } else
       ret = BadAlloc;

   out_no_dst:
   REGION_UNINIT(pScreen, &reg);
   return ret;
}

static int
put_image(ScrnInfoPtr pScrn,
          short src_x, short src_y,
          short drw_x, short drw_y,
          short src_w, short src_h,
          short drw_w, short drw_h,
          int id, unsigned char *buf,
          short width, short height,
          Bool sync, RegionPtr clipBoxes, pointer data,
          DrawablePtr pDraw)
{
   struct xorg_xv_port_priv *pPriv = (struct xorg_xv_port_priv *) data;
   ScreenPtr pScreen = xf86ScrnToScreen(pScrn);
   PixmapPtr pPixmap;
   INT32 x1, x2, _y1, _y2;
   BoxRec dstBox;
   int ret;

   /* Clip */
   x1 = src_x;
   x2 = src_x + src_w;
   _y1 = src_y;
   _y2 = src_y + src_h;

   dstBox.x1 = drw_x;
   dstBox.x2 = drw_x + drw_w;
   dstBox.y1 = drw_y;
   dstBox.y2 = drw_y + drw_h;

   if (!xf86XVClipVideoHelper(&dstBox, &x1, &x2, &_y1, &_y2, clipBoxes,
			      width, height))
      return Success;

   ret = check_yuv_surfaces(pPriv, id, width, height);
   if (ret)
       return ret;

   ret = copy_packed_data(pScrn, pPriv, id, buf,
			  src_x, src_y, width, height);
   if (ret)
       return ret;

   if (pDraw->type == DRAWABLE_WINDOW) {
      pPixmap = (*pScreen->GetWindowPixmap)((WindowPtr)pDraw);
   } else {
      pPixmap = (PixmapPtr)pDraw;
   }

   display_video(pScrn->pScreen, pPriv, id, clipBoxes,
                 src_x, src_y, src_w, src_h,
                 drw_x, drw_y,
                 drw_w, drw_h,
		 width, height, pPixmap);

   pPriv->current_set = (pPriv->current_set + 1) & 1;
   return Success;
}

static struct xorg_xv_port_priv *
port_priv_create(struct xa_tracker *xat, struct xa_context *r,
		 int drm_fd)
{
   struct xorg_xv_port_priv *priv = NULL;

   priv = calloc(1, sizeof(struct xorg_xv_port_priv));

   if (!priv)
      return NULL;

   priv->r = r;
   priv->xat = xat;
   priv->drm_fd = drm_fd;
   REGION_NULL(pScreen, &priv->clip);
   priv->hdtv = FALSE;
   priv->uv_offset = 0.5f;
   priv->uv_scale = 1.f;
   priv->y_offset = 0.f;
   priv->y_scale = 1.f;
   priv->rgb_offset = 0.f;
   priv->rgb_scale = 1.f;
   priv->sinhue = 0.f;
   priv->coshue = 1.f;

   vmwgfx_update_conversion_matrix(priv);

   return priv;
}

static void
vmwgfx_free_textured_adaptor(XF86VideoAdaptorPtr adaptor, Bool free_ports)
{
    if (free_ports) {
	int i;

	for(i=0; i<adaptor->nPorts; ++i) {
	    free(adaptor->pPortPrivates[i].ptr);
	}
    }

    free(adaptor->pAttributes);
    free(adaptor->pPortPrivates);
    xf86XVFreeVideoAdaptorRec(adaptor);
}

static XF86VideoAdaptorPtr
xorg_setup_textured_adapter(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   modesettingPtr ms = modesettingPTR(pScrn);
   XF86VideoAdaptorPtr adapt;
   XF86AttributePtr attrs;
   DevUnion *dev_unions;
   int nports = 16, i;
   int nattributes;
   struct xa_context *xar;

   /*
    * Use the XA default context since we don't expect the X server
    * to render from multiple threads.
    */

   xar = xa_context_default(ms->xat);
   nattributes = NUM_TEXTURED_ATTRIBUTES;

   adapt = calloc(1, sizeof(XF86VideoAdaptorRec));
   dev_unions = calloc(nports, sizeof(DevUnion));
   attrs = calloc(nattributes, sizeof(XF86AttributeRec));
   if (adapt == NULL || dev_unions == NULL || attrs == NULL) {
      free(adapt);
      free(dev_unions);
      free(attrs);
      return NULL;
   }

   adapt->type = XvWindowMask | XvInputMask | XvImageMask;
   adapt->flags = 0;
   adapt->name = xv_adapt_name;
   adapt->nEncodings = 1;
   adapt->pEncodings = DummyEncoding;
   adapt->nFormats = NUM_FORMATS;
   adapt->pFormats = Formats;
   adapt->nPorts = 0;
   adapt->pPortPrivates = dev_unions;
   adapt->nAttributes = nattributes;
   adapt->pAttributes = attrs;
   memcpy(attrs, TexturedAttributes, nattributes * sizeof(XF86AttributeRec));
   adapt->nImages = NUM_IMAGES;
   adapt->pImages = Images;
   adapt->PutVideo = NULL;
   adapt->PutStill = NULL;
   adapt->GetVideo = NULL;
   adapt->GetStill = NULL;
   adapt->StopVideo = stop_video;
   adapt->SetPortAttribute = set_port_attribute;
   adapt->GetPortAttribute = get_port_attribute;
   adapt->QueryBestSize = query_best_size;
   adapt->PutImage = put_image;
   adapt->QueryImageAttributes = query_image_attributes;


   for (i = 0; i < nports; i++) {
       struct xorg_xv_port_priv *priv =
	  port_priv_create(ms->xat, xar, ms->fd);

      adapt->pPortPrivates[i].ptr = (pointer) (priv);
      adapt->nPorts++;
   }

   return adapt;
}

void
xorg_xv_init(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   modesettingPtr ms = modesettingPTR(pScrn);
   XF86VideoAdaptorPtr *adaptors, *new_adaptors = NULL;
   XF86VideoAdaptorPtr textured_adapter = NULL, overlay_adaptor = NULL;
   int num_adaptors;

   num_adaptors = xf86XVListGenericAdaptors(pScrn, &adaptors);
   new_adaptors = malloc((num_adaptors + 2) * sizeof(XF86VideoAdaptorPtr *));
   if (new_adaptors == NULL)
       return;

   memcpy(new_adaptors, adaptors, num_adaptors * sizeof(XF86VideoAdaptorPtr));
   adaptors = new_adaptors;

   /* Add the adaptors supported by our hardware.  First, set up the atoms
    * that will be used by both output adaptors.
    */
   xvBrightness = MAKE_ATOM("XV_BRIGHTNESS");
   xvContrast = MAKE_ATOM("XV_CONTRAST");
   xvSaturation = MAKE_ATOM("XV_SATURATION");
   xvHue = MAKE_ATOM("XV_HUE");

   if (ms->xat) {
       textured_adapter = xorg_setup_textured_adapter(pScreen);
       if (textured_adapter)
	   adaptors[num_adaptors++] = textured_adapter;
   } else {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		  "No 3D acceleration. Not setting up textured video.\n");
   }

   overlay_adaptor = vmw_video_init_adaptor(pScrn);
   if (overlay_adaptor)
       adaptors[num_adaptors++] = overlay_adaptor;

   if (num_adaptors) {
       Bool ret;
       ret = xf86XVScreenInit(pScreen, adaptors, num_adaptors);
       if (textured_adapter)
	   vmwgfx_free_textured_adaptor(textured_adapter, !ret);
       if (overlay_adaptor)
	   vmw_video_free_adaptor(overlay_adaptor, !ret);
       if (!ret)
	   xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		      "Failed to initialize Xv.\n");
   } else {
       xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		  "Disabling Xv because no adaptors could be initialized.\n");
   }
}
