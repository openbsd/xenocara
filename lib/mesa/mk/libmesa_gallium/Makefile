#	$OpenBSD: Makefile,v 1.2 2021/07/22 11:10:12 jsg Exp $

LIB=	mesa_gallium

NOPROFILE=

SRCS=	st_atifs_to_nir.c \
	st_atom_array.c \
	st_atom_atomicbuf.c \
	st_atom_blend.c \
	st_atom.c \
	st_atom_clip.c \
	st_atom_constbuf.c \
	st_atom_depth.c \
	st_atom_framebuffer.c \
	st_atom_image.c \
	st_atom_msaa.c \
	st_atom_pixeltransfer.c \
	st_atom_rasterizer.c \
	st_atom_sampler.c \
	st_atom_scissor.c \
	st_atom_shader.c \
	st_atom_stipple.c \
	st_atom_storagebuf.c \
	st_atom_tess.c \
	st_atom_texture.c \
	st_atom_viewport.c \
	st_cb_bitmap.c \
	st_cb_bitmap_shader.c \
	st_cb_blit.c \
	st_cb_bufferobjects.c \
	st_cb_clear.c \
	st_cb_compute.c \
	st_cb_condrender.c \
	st_cb_copyimage.c \
	st_cb_drawpixels.c \
	st_cb_drawpixels_shader.c \
	st_cb_drawtex.c \
	st_cb_eglimage.c \
	st_cb_fbo.c \
	st_cb_feedback.c \
	st_cb_flush.c \
	st_cb_memoryobjects.c \
	st_cb_msaa.c \
	st_cb_perfmon.c \
	st_cb_perfquery.c \
	st_cb_program.c \
	st_cb_queryobj.c \
	st_cb_rasterpos.c \
	st_cb_readpixels.c \
	st_cb_strings.c \
	st_cb_semaphoreobjects.c \
	st_cb_syncobj.c \
	st_cb_texturebarrier.c \
	st_cb_texture.c \
	st_cb_viewport.c \
	st_cb_xformfb.c \
	st_context.c \
	st_copytex.c \
	st_debug.c \
	st_draw.c \
	st_draw_feedback.c \
	st_extensions.c \
	st_format.c \
	st_gen_mipmap.c \
	st_glsl_to_ir.cpp \
	st_glsl_to_nir.cpp \
	st_glsl_to_tgsi.cpp \
	st_glsl_to_tgsi_array_merge.cpp \
	st_glsl_to_tgsi_private.cpp \
	st_glsl_to_tgsi_temprename.cpp \
	st_manager.c \
	st_nir_builtins.c \
	st_nir_lower_builtin.c \
	st_nir_lower_tex_src_plane.c \
	st_pbo.c \
	st_program.c \
	st_sampler_view.c \
	st_scissor.c \
	st_shader_cache.c \
	st_texture.c \
	st_tgsi_lower_depth_clamp.c \
	st_tgsi_lower_yuv.c \
	st_vdpau.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa/state_tracker \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mesa/main \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/nir

DPADD+=		${.CURDIR}/../libmesa_common/${__objdir}/libmesa_common.a \
		${.CURDIR}/../libglsl/${__objdir}/libglsl.a

.if ${WITH_SSE41} == "yes"
DPADD+=		${.CURDIR}/../libmesa_sse41/${__objdir}/libmesa_sse41.a
.endif

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/mesa/state_tracker
