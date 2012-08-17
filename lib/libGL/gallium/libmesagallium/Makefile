# $OpenBSD: Makefile,v 1.2 2012/08/17 13:58:00 mpi Exp $

LIB=	mesagallium

MESA=		${.CURDIR}/../../../../dist/Mesa/src/mesa
MAPI=		${.CURDIR}/../../../../dist/Mesa/src/mapi
GLSL=		${.CURDIR}/../../../../dist/Mesa/src/glsl
GALLIUM=	${.CURDIR}/../../../../dist/Mesa/src/gallium

MESA_INCLUDE=	${.CURDIR}/../../../../dist/Mesa/include

CPPFLAGS+= \
	-I. \
	-I${MAPI} \
	-I${MESA_INCLUDE} \
	-I${MESA_INCLUDE}/GL/internal \
	-I${MESA} \
	-I${MESA}/main \
	-I${MESA}/math \
	-I${MESA}/drivers/dri/common \
	-I${GLSL} \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/include \
	-I${GALLIUM}/state_trackers/dri/common \
	-I${X11BASE}/include \
	-DPTHREADS \
	-DHAVE_ALIAS \
	-D_REENTRANT \
	-DIN_DRI_DRIVER \
	-DHAVE_POSIX_MEMALIGN \
	-D__NOT_HAVE_DRM_H

.if ${MACHINE_ARCH} == "i386"
CPPFLAGS += \
	-I${MESA}/x86 \
	-DUSE_X86_ASM \
	-DUSE_MMX_ASM \
	-DUSE_3DNOW_ASM \
	-DUSE_SSE_ASM
.elif ${MACHINE_ARCH} == "amd64"
CPPFLAGS += \
	-I${MESA}/x86 \
	-D__GLX_ALIGN64 -DUSE_X86_64_ASM
.endif

# this is part of MAIN_SOURCES
MAIN_ES_SOURCES = \
	api_exec_es1.c \
	api_exec_es2.c

# main
MAIN_SOURCES = \
	api_arrayelt.c \
	api_exec.c \
	api_loopback.c \
	api_noop.c \
	api_validate.c \
	accum.c \
	arbprogram.c \
	atifragshader.c \
	attrib.c \
	arrayobj.c \
	blend.c \
	bufferobj.c \
	buffers.c \
	clear.c \
	clip.c \
	colortab.c \
	condrender.c \
	context.c \
	convolve.c \
	cpuinfo.c \
	debug.c \
	depth.c \
	depthstencil.c \
	dlist.c \
	dlopen.c \
	drawpix.c \
	drawtex.c \
	enable.c \
	enums.c \
	eval.c \
	execmem.c \
	extensions.c \
	fbobject.c \
	feedback.c \
	ffvertex_prog.c \
	fog.c \
	formats.c \
	framebuffer.c \
	get.c \
	getstring.c \
	hash.c \
	hint.c \
	histogram.c \
	image.c \
	imports.c \
	light.c \
	lines.c \
	matrix.c \
	mipmap.c \
	mm.c \
	multisample.c \
	nvprogram.c \
	pack.c \
	pbo.c \
	pixel.c \
	pixelstore.c \
	pixeltransfer.c \
	points.c \
	polygon.c \
	queryobj.c \
	querymatrix.c \
	rastpos.c \
	readpix.c \
	remap.c \
	renderbuffer.c \
	samplerobj.c \
	scissor.c \
	shaderapi.c \
	shaderobj.c \
	shared.c \
	state.c \
	stencil.c \
	syncobj.c \
	texcompress.c \
	texcompress_rgtc.c \
	texcompress_s3tc.c \
	texcompress_fxt1.c \
	texenv.c \
	texfetch.c \
	texformat.c \
	texgen.c \
	texgetimage.c \
	teximage.c \
	texobj.c \
	texpal.c \
	texparam.c \
	texstate.c \
	texstore.c \
	texturebarrier.c \
	transformfeedback.c \
	uniforms.c \
	varray.c \
	version.c \
	viewport.c \
	vtxfmt.c \
	${MAIN_ES_SOURCES}

MAIN_CXX_SOURCES = \
	ff_fragment_shader.cpp

# math
MATH_SOURCES= \
	m_debug_clip.c \
	m_debug_norm.c \
	m_debug_xform.c \
	m_eval.c \
	m_matrix.c \
	m_translate.c \
	m_vector.c

VBO_SOURCES = \
	vbo_context.c \
	vbo_exec.c \
	vbo_exec_api.c \
	vbo_exec_array.c \
	vbo_exec_draw.c \
	vbo_exec_eval.c \
	vbo_rebase.c \
	vbo_split.c \
	vbo_split_copy.c \
	vbo_split_inplace.c \
	vbo_save.c \
	vbo_save_api.c \
	vbo_save_draw.c \
	vbo_save_loopback.c 

STATETRACKER_SOURCES = \
	st_atom.c \
	st_atom_blend.c \
	st_atom_clip.c \
	st_atom_constbuf.c \
	st_atom_depth.c \
	st_atom_framebuffer.c \
	st_atom_msaa.c \
	st_atom_pixeltransfer.c \
	st_atom_sampler.c \
	st_atom_scissor.c \
	st_atom_shader.c \
	st_atom_rasterizer.c \
	st_atom_stipple.c \
	st_atom_texture.c \
	st_atom_viewport.c \
	st_cb_accum.c \
	st_cb_bitmap.c \
	st_cb_blit.c \
	st_cb_bufferobjects.c \
	st_cb_clear.c \
	st_cb_condrender.c \
	st_cb_flush.c \
	st_cb_drawpixels.c \
	st_cb_drawtex.c \
	st_cb_eglimage.c \
	st_cb_fbo.c \
	st_cb_feedback.c \
	st_cb_program.c \
	st_cb_queryobj.c \
	st_cb_rasterpos.c \
	st_cb_readpixels.c \
	st_cb_syncobj.c \
	st_cb_strings.c \
	st_cb_texture.c \
	st_cb_texturebarrier.c \
	st_cb_viewport.c \
	st_cb_xformfb.c \
	st_context.c \
	st_debug.c \
	st_draw.c \
	st_draw_feedback.c \
	st_extensions.c \
	st_format.c \
	st_gen_mipmap.c \
	st_manager.c \
	st_mesa_to_tgsi.c \
	st_program.c \
	st_texture.c

PROGRAM_SOURCES = \
	arbprogparse.c \
	hash_table.c \
	lex.yy.c \
	nvfragparse.c \
	nvvertparse.c \
	program.c \
	program_parse.tab.c \
	program_parse_extra.c \
	prog_cache.c \
	prog_execute.c \
	prog_instruction.c \
	prog_noise.c \
	prog_optimize.c \
	prog_parameter.c \
	prog_parameter_layout.c \
	prog_print.c \
	prog_statevars.c \
	prog_uniform.c \
	programopt.c \
	register_allocate.c \
	symbol_table.c

SHADER_CXX_SOURCES = \
	ir_to_mesa.cpp \
	sampler.cpp

.if ${MACHINE_ARCH} == i386
ASM_C_SOURCES= \
	common_x86.c \
.elif ${MACHINE_ARCH} == powerpc
ASM_C_SOURCES= \
	common_ppc.c
.endif

# drisw
DRISW_SOURCES = \
	dri_sw_winsys.c \
        dri_context.c \
        dri_screen.c \
        dri_drawable.c \
        drisw.c

MESAGALLIUM_SOURCES = \
	${MAIN_SOURCES}		\
	${MATH_SOURCES}		\
	${VBO_SOURCES}		\
	${PROGRAM_SOURCES}	\
	${STATETRACKER_SOURCES} \
	${ASM_C_SOURCES}	\
	${DRISW_SOURCES}

MESAGALLIUM_CXX_SOURCES = \
	${MAIN_CXX_SOURCES} \
	${SHADER_CXX_SOURCES}

SRCS=	${MESAGALLIUM_SOURCES}	\
	${MESAGALLIUM_CXX_SOURCES} \
	${ASM_SOURCES} \
	_atexit.c

all: lib${LIB}_pic.a

obj: _xenocara_obj

install:
	@echo "Not installing lib${LIB}"

clean: 
	rm -f lib${LIB}_pic.a ${OBJS}

cleandir:	clean

.include <bsd.xorg.mk>

.if ${MACHINE_ARCH} == i386
.PATH: ${MESA}/x86
.endif
.if ${MACHINE_ARCH} == powerpc
.PATH: ${MESA}/ppc
.endif
.PATH: ${MESA}/main
.PATH: ${MESA}/math
.PATH: ${MESA}/state_tracker
.PATH: ${MESA}/program
.PATH: ${MESA}/vbo
.PATH: ${GALLIUM}/winsys/sw/dri
.PATH: ${GALLIUM}/state_trackers/dri/sw
.PATH: ${GALLIUM}/state_trackers/dri/common
.PATH: ${.CURDIR}/generated
