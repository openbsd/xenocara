# $OpenBSD: Makefile.inc,v 1.11 2015/02/20 23:31:06 jsg Exp $
.include "../config.mk"

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
	-I${MESA}/.. \
	-I${MESA}/main \
	-I${MESA}/math \
	-I${MESA}/tnl \
	-I${GLSL} \
	-I${GALLIUM}/auxiliary \
	-I${.OBJDIR}/shader/slang \
	-I${.CURDIR}/../../generated \
	-I${.CURDIR}/../../generated/main \
	-I${X11BASE}/include \
	-D_REENTRANT \
	-DIN_DRI_DRIVER

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
	es1_conversion.c

# main
MAIN_SOURCES = \
	api_arrayelt.c \
	api_exec.c \
	api_loopback.c \
	api_validate.c \
	accum.c \
	arbprogram.c \
	atifragshader.c \
	attrib.c \
	arrayobj.c \
	blend.c \
	blit.c \
	bufferobj.c \
	buffers.c \
	clear.c \
	clip.c \
	colortab.c \
	compute.c \
	condrender.c \
	context.c \
	convolve.c \
	cpuinfo.c \
	debug.c \
	depth.c \
	dlist.c \
	drawpix.c \
	drawtex.c \
	enable.c \
	errors.c \
	eval.c \
	execmem.c \
	extensions.c \
	fbobject.c \
	feedback.c \
	ffvertex_prog.c \
	fog.c \
	formatquery.c \
	formats.c \
	format_pack.c \
	format_unpack.c \
	framebuffer.c \
	get.c \
	genmipmap.c \
	getstring.c \
	glformats.c \
	hash.c \
	hash_table.c \
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
        objectlabel.c \
	pack.c \
	pbo.c \
	performance_monitor.c \
	pipelineobj.c \
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
	set.c \
	shaderapi.c \
	shaderimage.c \
	shaderobj.c \
	shared.c \
	state.c \
	stencil.c \
	syncobj.c \
	texcompress.c \
	texcompress_cpal.c \
	texcompress_rgtc.c \
	texcompress_s3tc.c \
	texcompress_fxt1.c \
	texcompress_etc.c \
	texenv.c \
	texformat.c \
	texgen.c \
	texgetimage.c \
	teximage.c \
	texobj.c \
	texparam.c \
	texstate.c \
	texstorage.c \
	texstore.c \
        textureview.c \
	texturebarrier.c \
	transformfeedback.c \
	uniforms.c \
	varray.c \
	vdpau.c \
	version.c \
	viewport.c \
	vtxfmt.c \
	enums.c \
	${MAIN_ES_SOURCES}

MAIN_CXX_SOURCES = \
	ff_fragment_shader.cpp \
	shader_query.cpp \
	uniform_query.cpp

# math
MATH_SOURCES= \
	m_debug_clip.c \
	m_debug_norm.c \
	m_debug_xform.c \
	m_eval.c \
	m_matrix.c \
	m_translate.c \
	m_vector.c

MATH_XFORM_SOURCES = \
	m_xform.c

# swrast
SWRAST_SOURCES= \
	s_aaline.c \
	s_aatriangle.c \
	s_alpha.c \
	s_atifragshader.c \
	s_bitmap.c \
	s_blend.c \
	s_blit.c \
	s_clear.c \
	s_copypix.c \
	s_context.c \
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
	s_zoom.c

# swrast_setup
SWRAST_SETUP_SOURCES= \
	ss_context.c \
	ss_triangle.c

# tnl
TNL_SOURCES = \
	t_context.c \
	t_pipeline.c \
	t_draw.c \
	t_rasterpos.c \
	t_vb_program.c \
	t_vb_render.c \
	t_vb_texgen.c \
	t_vb_texmat.c \
	t_vb_vertex.c \
	t_vb_fog.c \
	t_vb_light.c \
	t_vb_normals.c \
	t_vb_points.c \
	t_vp_build.c \
	t_vertex.c \
	t_vertex_sse.c \
	t_vertex_generic.c

VBO_SOURCES = \
	vbo_context.c \
	vbo_exec.c \
	vbo_exec_api.c \
	vbo_exec_array.c \
	vbo_exec_draw.c \
	vbo_exec_eval.c \
	vbo_noop.c \
	vbo_primitive_restart.c \
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
	st_atom_array.c \
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
	st_cb_bitmap.c \
	st_cb_blit.c \
	st_cb_bufferobjects.c \
	st_cb_clear.c \
	st_cb_condrender.c \
	st_cb_flush.c \
	st_cb_msaa.c \
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
	st_texture.c \
	st_vdpau.c

STATETRACKER_CXX_SOURCES = \
	st_glsl_to_tgsi.cpp

PROGRAM_SOURCES = \
	arbprogparse.c \
	hash_table.c \
	lex.yy.c \
	program.c \
	program_parse.tab.c \
	program_parse_extra.c \
	prog_cache.c \
	prog_execute.c \
	prog_hash_table.c \
	prog_instruction.c \
	prog_noise.c \
	prog_opt_constant_fold.c \
	prog_optimize.c \
	prog_parameter.c \
	prog_parameter_layout.c \
	prog_print.c \
	prog_statevars.c \
	programopt.c \
	register_allocate.c \
	symbol_table.c

PROGRAM_CXX_SOURCES = \
	string_to_uint_map.cpp

SHADER_CXX_SOURCES = \
	ir_to_mesa.cpp \
	sampler.cpp

.if ${MACHINE_ARCH} == i386 || ${MACHINE_ARCH} == amd64
ASM_C_SOURCES= \
	common_x86.c \
	x86_xform.c \
	3dnow.c \
	sse.c \
	x86sse.c \
	x86-64.c
.endif

.if ${MACHINE_ARCH} == i386
ASM_SOURCES= \
	common_x86_asm.S \
	x86_xform2.S \
	x86_xform3.S \
	x86_xform4.S \
	x86_cliptest.S \
	mmx_blend.S \
	3dnow_xform1.S \
	3dnow_xform2.S \
	3dnow_xform3.S \
	3dnow_xform4.S \
	3dnow_normal.S \
	sse_xform1.S \
	sse_xform2.S \
	sse_xform3.S \
	sse_xform4.S \
	sse_normal.S \
	read_rgba_span_x86.S
.endif
.if ${MACHINE_ARCH} == amd64
ASM_SOURCES =		\
	xform4.S
.endif
.if ${MACHINE_ARCH} == sparc
ASM_C_SOURCES= \
	sparc.c
SPARC_SOURCES =			\
	clip.S			\
	norm.S			\
	xform.S
.endif

COMMON_DRIVER_SOURCES = \
	driverfuncs.c		\
	meta.c

MESA_SOURCES = \
	$(MAIN_SOURCES)		\
	$(MATH_SOURCES)		\
	$(MATH_XFORM_SOURCES)	\
	$(VBO_SOURCES)		\
	$(TNL_SOURCES)		\
	$(PROGRAM_SOURCES)	\
	$(SWRAST_SOURCES)	\
	$(SWRAST_SETUP_SOURCES) \
	$(COMMON_DRIVER_SOURCES) \
	$(ASM_C_SOURCES)

MESA_CXX_SOURCES = \
	$(MAIN_CXX_SOURCES) \
	$(PROGRAM_CXX_SOURCES) \
	$(SHADER_CXX_SOURCES)

MESAGALLIUM_SOURCES = \
	${MAIN_SOURCES}		\
	${MATH_SOURCES}		\
	${VBO_SOURCES}		\
	${PROGRAM_SOURCES}	\
	${STATETRACKER_SOURCES} \
	${ASM_C_SOURCES}

MESAGALLIUM_CXX_SOURCES = \
	${MAIN_CXX_SOURCES} \
	${SHADER_CXX_SOURCES} \
	${PROGRAM_CXX_SOURCES} \
	${STATETRACKER_CXX_SOURCES}

OBJS+=  ${SRCS:N*.h:R:S/$/.so/g}

all: lib${LIB}_pic.a

obj: _xenocara_obj

install:
	@echo "Not installing libmesa"

clean: 
	rm -f lib${LIB}_pic.a ${OBJS} $(CLEANFILES)

cleandir:	clean

.SUFFIXES:
.SUFFIXES: .a .c .cpp .o .S .s .so

.c.so:
	@echo "${COMPILE.c} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.c} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

.if ${MACHINE_ARCH} == alpha
# remove optimization flags to prevent gcc 4.2.1 ICE
prog_execute.so: prog_execute.c
	@echo "${COMPILE.c} -fno-tree-ter ${PICFLAG} -DPIC ${.ALLSRC} -o ${.TARGET}"
	@${COMPILE.c} -fno-tree-ter ${PICFLAG} -DPIC ${.ALLSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o
.endif

.cpp.so:
	@echo "${COMPILE.cc} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.cc} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

.S.so .s.so:
	@echo "${CPP} -DPIC ${CPPFLAGS} ${CFLAGS:M-[ID]*} ${AINC} ${.IMPSRC} | \
	    ${AS} ${ASPICFLAG} -o ${.TARGET}"
	@${CPP} -DPIC ${CPPFLAGS} ${CFLAGS:M-[ID]*} ${AINC} ${.IMPSRC} | \
	    ${AS} ${ASPICFLAG} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o


lib${LIB}_pic.a:	${OBJS} ${DPADD}
	@rm -f lib${LIB}_pic.a
	@${AR} cq lib${LIB}_pic.a `${LORDER} ${OBJS} | tsort -q`
	${RANLIB} lib${LIB}_pic.a

.if ${MACHINE_ARCH} == i386 || ${MACHINE_ARCH} == amd64
gen_matypes: gen_matypes.c
	$(CC) $(CPPFLAGS) $(CFLAGS) ${MESA}/x86/gen_matypes.c -o gen_matypes


matypes.h: mtypes.h t_context.h gen_matypes
	./gen_matypes > matypes.h

CLEANFILES+= gen_matypes matypes.h

xform4.so: matypes.h
common_x86_asm.so: matypes.h
3dnow_normal.so: matypes.h
3dnow_xform1.so: matypes.h
3dnow_xform2.so: matypes.h
3dnow_xform3.so: matypes.h
3dnow_xform4.so: matypes.h
mmx_blend.so: matypes.h
sse_normal.so: matypes.h
sse_xform1.so: matypes.h
sse_xform2.so: matypes.h
sse_xform3.so: matypes.h
sse_xform4.so: matypes.h
x86_cliptest.so: matypes.h
x86_xform2.so: matypes.h
x86_xform3.so: matypes.h
x86_xform4.so: matypes.h
.endif

.if ${MACHINE_ARCH} == i386 || ${MACHINE_ARCH} == amd64
.PATH: ${MESA}/x86
.PATH: ${MESA}/x86/rtasm
.PATH: ${MESA}/x86-64
.endif
.if ${MACHINE_ARCH} == powerpc
.PATH: ${MESA}/ppc
.endif
.if ${MACHINE_ARCH} == sparc
.PATH: ${MESA}/sparc
.endif
.PATH: ${MAPI}/glapi
.PATH: ${MESA}/main
.PATH: ${MESA}/../util
.PATH: ${MESA}/math
.PATH: ${MESA}/swrast
.PATH: ${MESA}/swrast_setup
.PATH: ${MESA}/tnl
.PATH: ${MESA}/state_tracker
.PATH: ${MESA}/program
.PATH: ${MESA}/vbo
.PATH: ${MESA}/drivers/common
.PATH: ${.CURDIR}/..
.PATH: ${.CURDIR}/../../generated
.PATH: ${.CURDIR}/../../generated/main
.PATH: ${.CURDIR}/../../generated/program
