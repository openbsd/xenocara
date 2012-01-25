
#ifndef RADEON_VBO_H
#define RADEON_VBO_H

extern void radeon_vb_no_space(ScrnInfoPtr pScrn, struct radeon_vbo_object *vbo, int vert_size);
extern void radeon_vbo_init_lists(ScrnInfoPtr pScrn);
extern void radeon_vbo_free_lists(ScrnInfoPtr pScrn);
extern void radeon_vbo_flush_bos(ScrnInfoPtr pScrn);
extern void radeon_vbo_get(ScrnInfoPtr pScrn, struct radeon_vbo_object *vbo);
extern void radeon_vbo_put(ScrnInfoPtr pScrn, struct radeon_vbo_object *vbo);

static inline void radeon_vbo_check(ScrnInfoPtr pScrn,
				    struct radeon_vbo_object *vbo,
				    int vert_size)
{

    if ((vbo->vb_offset + (vbo->verts_per_op * vert_size)) > vbo->vb_total) {
	radeon_vb_no_space(pScrn, vbo, vert_size);
    }
}

static inline void *
radeon_vbo_space(ScrnInfoPtr pScrn,
		 struct radeon_vbo_object *vbo,
		 int vert_size)
{
#if defined(XF86DRM_MODE)
    RADEONInfoPtr info = RADEONPTR(pScrn);
#endif
    void *vb;

    /* we've ran out of space in the vertex buffer - need to get a
       new one */
    radeon_vbo_check(pScrn, vbo, vert_size);

    vbo->vb_op_vert_size = vert_size;
#if defined(XF86DRM_MODE)
    if (info->cs)
	vb = (pointer)((char *)vbo->vb_bo->ptr + vbo->vb_offset);
    else
#endif
	vb = (pointer)((char *)vbo->vb_ptr + vbo->vb_offset);
    return vb;
}

static inline void radeon_vbo_commit(ScrnInfoPtr pScrn,
				     struct radeon_vbo_object *vbo)
{

    vbo->vb_offset += vbo->verts_per_op * vbo->vb_op_vert_size;
}

#endif
