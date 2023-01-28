#	$OpenBSD: Makefile,v 1.2 2023/01/28 08:56:35 jsg Exp $

LIB=	mesa

NOPROFILE=

SRCS=	accum.c \
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
	compute.c \
	condrender.c \
	conservativeraster.c \
	context.c \
	copyimage.c \
	cpuinfo.c \
	debug.c \
	debug_output.c \
	depth.c \
	dlist.c \
	draw.c \
	draw_validate.c \
	drawpix.c \
	drawtex.c \
	enable.c \
	errors.c \
	es1_conversion.c \
	eval.c \
	extensions.c \
	extensions_table.c \
	externalobjects.c \
	fbobject.c \
	feedback.c \
	ff_fragment_shader.cpp \
	ffvertex_prog.c \
	fog.c \
	format_utils.c \
	formatquery.c \
	formats.c \
	framebuffer.c \
	genmipmap.c \
	get.c \
	getstring.c \
	glformats.c \
	glspirv.c \
	glthread.c \
	glthread_bufferobj.c \
	glthread_draw.c \
	glthread_get.c \
	glthread_list.c \
	glthread_shaderobj.c \
	glthread_varray.c \
	hash.c \
	hint.c \
	image.c \
	light.c \
	lines.c \
	matrix.c \
	mipmap.c \
	multisample.c \
	objectlabel.c \
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
	shader_query.cpp \
	shaderapi.c \
	shaderimage.c \
	shaderobj.c \
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
	es1_conversion.c \
	m_debug_clip.c \
	m_debug_norm.c \
	m_debug_xform.c \
	m_eval.c \
	m_matrix.c \
	m_vector.c \
	arbprogparse.c \
	link_program.cpp \
	prog_cache.c \
	prog_instruction.c \
	prog_parameter.c \
	prog_parameter_layout.c \
	prog_print.c \
	prog_statevars.c \
	prog_to_nir.c \
	program.c \
	program_parse_extra.c \
	programopt.c \
	symbol_table.c \
	st_atifs_to_nir.c \
	st_atom.c \
	st_atom_array.cpp \
	st_atom_atomicbuf.c \
	st_atom_blend.c \
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
	st_cb_clear.c \
	st_cb_copyimage.c \
	st_cb_drawpixels.c \
	st_cb_drawtex.c \
	st_cb_eglimage.c \
	st_cb_feedback.c \
	st_cb_flush.c \
	st_cb_rasterpos.c \
	st_cb_readpixels.c \
	st_cb_texture.c \
	st_context.c \
	st_copytex.c \
	st_debug.c \
	st_draw.c \
	st_draw_feedback.c \
	st_draw_hw_select.c \
	st_extensions.c \
	st_format.c \
	st_gen_mipmap.c \
	st_glsl_to_ir.cpp \
	st_glsl_to_nir.cpp \
	st_interop.c \
	st_manager.c \
	st_nir_builtins.c \
	st_nir_lower_builtin.c \
	st_nir_lower_tex_src_plane.c \
	st_pbo.c \
	st_pbo_compute.c \
	st_program.c \
	st_sampler_view.c \
	st_scissor.c \
	st_shader_cache.c \
	st_texture.c \
	st_vdpau.c \
	vbo_context.c \
	vbo_exec.c \
	vbo_exec_api.c \
	vbo_exec_draw.c \
	vbo_exec_eval.c \
	vbo_minmax_index.c \
	vbo_noop.c \
	vbo_save.c \
	vbo_save_api.c \
	vbo_save_draw.c \
	vbo_save_loopback.c \
	common_x86.c

.if ${MACHINE_ARCH} == "i386"
SRCS+=	common_x86_asm.S \
	read_rgba_span_x86.S
.endif

SRCS+=	lex.yy.c \
	program_parse.tab.c \
	api_exec_init.c \
	enums.c \
	format_fallback.c \
	unmarshal_table.c

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
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/util \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/mesa \
		-I${MESA_SRC}/generated/src/mesa/main \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-I${MESA_SRC}/generated/src/mapi/glapi/gen

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/mesa/program
.PATH:	${MESA_SRC}/src/mesa/main
.PATH:	${MESA_SRC}/src/mesa/math
.PATH:	${MESA_SRC}/src/mesa/state_tracker
.PATH:	${MESA_SRC}/src/mesa/vbo
.PATH:	${MESA_SRC}/src/mesa/x86
.PATH:	${MESA_SRC}/src/mesa/x86-64
.PATH:	${MESA_SRC}/src/mesa/sparc
.PATH:	${MESA_SRC}/generated/src/mesa
.PATH:	${MESA_SRC}/generated/src/mesa/program
.PATH:	${MESA_SRC}/generated/src/mapi/glapi/gen
