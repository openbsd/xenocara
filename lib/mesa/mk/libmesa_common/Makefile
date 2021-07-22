#	$OpenBSD: Makefile,v 1.3 2021/07/22 11:10:12 jsg Exp $

LIB=	mesa_common

NOPROFILE=

SRCS=	arbprogparse.c \
	ir_to_mesa.cpp \
	prog_cache.c \
	prog_execute.c \
	prog_instruction.c \
	prog_noise.c \
	prog_opt_constant_fold.c \
	prog_optimize.c \
	prog_parameter.c \
	prog_parameter_layout.c \
	prog_print.c \
	program.c \
	programopt.c \
	program_parse_extra.c \
	prog_statevars.c \
	symbol_table.c \
	prog_to_nir.c \
	accum.c \
	api_arrayelt.c \
	arbprogram.c \
	arrayobj.c \
	atifragshader.c \
	attrib.c \
	barrier.c \
	bbox.c \
	blend.c \
	blit.c \
	bufferobj.c \
	buffers.c \
	clear.c \
	clip.c \
	colortab.c \
	compute.c \
	condrender.c \
	conservativeraster.c \
	context.c \
	convolve.c \
	copyimage.c \
	cpuinfo.c \
	debug.c \
	debug_output.c \
	depth.c \
	dlist.c \
	draw.c \
	drawpix.c \
	drawtex.c \
	draw_validate.c \
	enable.c \
	errors.c \
	eval.c \
	execmem.c \
	extensions.c \
	extensions_table.c \
	externalobjects.c \
	fbobject.c \
	feedback.c \
	ff_fragment_shader.cpp \
	ffvertex_prog.c \
	fog.c \
	formatquery.c \
	formats.c \
	format_utils.c \
	framebuffer.c \
	get.c \
	genmipmap.c \
	getstring.c \
	glformats.c \
	glspirv.c \
	glthread.c \
	glthread_bufferobj.c \
	glthread_draw.c \
	glthread_get.c \
	glthread_shaderobj.c \
	glthread_varray.c \
	hash.c \
	hint.c \
	histogram.c \
	image.c \
	light.c \
	lines.c \
	matrix.c \
	mipmap.c \
	multisample.c \
	objectlabel.c \
	objectpurge.c \
	pack.c \
	pbo.c \
	performance_monitor.c \
	performance_query.c \
	pipelineobj.c \
	pixel.c \
	pixelstore.c \
	pixeltransfer.c \
	points.c \
	polygon.c \
	program_binary.c \
	program_resource.c \
	querymatrix.c \
	queryobj.c \
	rastpos.c \
	readpix.c \
	remap.c \
	renderbuffer.c \
	robustness.c \
	samplerobj.c \
	scissor.c \
	shaderapi.c \
	shaderimage.c \
	shaderobj.c \
	shader_query.cpp \
	shared.c \
	spirv_extensions.c \
	state.c \
	stencil.c \
	syncobj.c \
	texcompress.c \
	texcompress_astc.cpp \
	texcompress_bptc.c \
	texcompress_cpal.c \
	texcompress_etc.c \
	texcompress_fxt1.c \
	texcompress_rgtc.c \
	texcompress_s3tc.c \
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
	texturebindless.c \
	textureview.c \
	transformfeedback.c \
	uniform_query.cpp \
	uniforms.c \
	varray.c \
	vdpau.c \
	version.c \
	viewport.c \
	vtxfmt.c \
	es1_conversion.c \
	m_debug_clip.c \
	m_debug_norm.c \
	m_debug_xform.c \
	m_eval.c \
	m_matrix.c \
	m_translate.c \
	m_vector.c \
	vbo_context.c \
	vbo_exec_api.c \
	vbo_exec.c \
	vbo_exec_draw.c \
	vbo_exec_eval.c \
	vbo_minmax_index.c \
	vbo_noop.c \
	vbo_save_api.c \
	vbo_save.c \
	vbo_save_draw.c \
	vbo_save_loopback.c \
	common_x86.c

.if ${MACHINE_ARCH} == "i386"
SRCS+=	common_x86_asm.S \
	x86_xform2.S \
	x86_xform3.S \
	x86_xform4.S \
	x86_cliptest.S \
	mmx_blend.S \
	3dnow_xform1.S \
	3dnow_xform2.S \
	3dnow_xform3.S \
	3dnow_xform4.S \
	sse_xform1.S \
	sse_xform2.S \
	sse_xform3.S \
	sse_xform4.S \
	sse_normal.S \
	read_rgba_span_x86.S
.elif ${MACHINE_ARCH} == "amd64"
SRCS+=		xform4.S
CPPFLAGS+=	-I${MESA_SRC}/src/mesa/x86-64
.elif ${MACHINE_ARCH} == "sparc64"
SRCS+=		sparc_clip.S \
		norm.S \
		xform.S
CPPFLAGS+=	-I${MESA_SRC}/src/mesa/sparc
.endif

SRCS+=	format_pack.c

SRCS+=	lex.yy.c \
	program_parse.tab.c \
	api_exec.c \
	enums.c \
	format_fallback.c

.for INDEX in 0 1 2 3 4 5 6 7
SRCS+=	marshal_generated${INDEX}.c
.endfor

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mesa/program \
		-I${MESA_SRC}/src/mesa/main \
		-I${MESA_SRC}/src/mesa/math \
		-I${MESA_SRC}/src/mesa/vbo \
		-I${MESA_SRC}/src/mesa/x86 \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/mesa \
		-I${MESA_SRC}/generated/src/mesa/main \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/mesa/program
.PATH:	${MESA_SRC}/src/mesa/main
.PATH:	${MESA_SRC}/src/mesa/math
.PATH:	${MESA_SRC}/src/mesa/vbo
.PATH:	${MESA_SRC}/src/mesa/x86
.PATH:	${MESA_SRC}/src/mesa/x86-64
.PATH:	${MESA_SRC}/src/mesa/sparc
.PATH:	${MESA_SRC}/generated/src/mesa
.PATH:	${MESA_SRC}/generated/src/mesa/program
.PATH:	${MESA_SRC}/generated/src/mapi/glapi/gen
