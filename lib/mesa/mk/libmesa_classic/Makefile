#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:40 jsg Exp $

LIB=	mesa_classic

NOPROFILE=

SRCS=	m_xform.c \
	t_context.c \
	t_draw.c \
	t_pipeline.c \
	t_rebase.c \
	t_split.c \
	t_split_copy.c \
	t_split_inplace.c \
	t_vb_fog.c \
	t_vb_light.c \
	t_vb_normals.c \
	t_vb_points.c \
	t_vb_program.c \
	t_vb_render.c \
	t_vb_texgen.c \
	t_vb_texmat.c \
	t_vb_vertex.c \
	t_vertex.c \
	t_vertex_generic.c \
	t_vertex_sse.c \
	t_vp_build.c \
	s_aaline.c \
	s_aatriangle.c \
	s_alpha.c \
	s_atifragshader.c \
	s_bitmap.c \
	s_blend.c \
	s_blit.c \
	s_clear.c \
	s_context.c \
	s_copypix.c \
	s_depth.c \
	s_drawpix.c \
	s_feedback.c \
	s_fog.c \
	s_fragprog.c \
	s_lines.c \
	s_logic.c \
	s_masking.c \
	s_points.c \
	s_renderbuffer.c \
	s_span.c \
	s_stencil.c \
	s_texcombine.c \
	s_texfetch.c \
	s_texfilter.c \
	s_texrender.c \
	s_texture.c \
	s_triangle.c \
	s_zoom.c \
	ss_context.c \
	ss_triangle.c \
	driverfuncs.c \
	meta_blit.c \
	meta_generate_mipmap.c \
	meta.c \
	x86_xform.c \
	3dnow.c \
	sse.c \
	x86sse.c \
	sparc.c \
	x86-64.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mesa/main \
		-I${MESA_SRC}/src/mesa/math \
		-I${MESA_SRC}/src/mesa/tnl \
		-I${MESA_SRC}/src/mesa/swrast \
		-I${MESA_SRC}/src/mesa/swrast_setup \
		-I${MESA_SRC}/src/mesa/drivers/common \
		-I${MESA_SRC}/src/mesa/x86/rtasm \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
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

.PATH:	${MESA_SRC}/src/mesa/math
.PATH:	${MESA_SRC}/src/mesa/tnl
.PATH:	${MESA_SRC}/src/mesa/swrast
.PATH:	${MESA_SRC}/src/mesa/swrast_setup
.PATH:	${MESA_SRC}/src/mesa/drivers/common
.PATH:	${MESA_SRC}/src/mesa/x86
.PATH:	${MESA_SRC}/src/mesa/x86/rtasm
.PATH:	${MESA_SRC}/src/mesa/sparc
.PATH:	${MESA_SRC}/src/mesa/x86-64
