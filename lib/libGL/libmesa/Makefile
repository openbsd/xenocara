# $OpenBSD: Makefile,v 1.12 2011/11/29 14:57:54 naddy Exp $

MESA=	${.CURDIR}/../../../dist/Mesa/src/mesa
MAPI=	${.CURDIR}/../../../dist/Mesa/src/mapi
GLSL=	${.CURDIR}/../../../dist/Mesa/src/glsl

MESA_INCLUDE=	${.CURDIR}/../../../dist/Mesa/include

CPP=	cpp -notraditional

CPPFLAGS+= \
	-I. \
	-I${MAPI} \
	-I${MESA_INCLUDE} \
	-I${MESA_INCLUDE}/GL/internal \
	-I${MESA} \
	-I${MESA}/main \
	-I${MESA}/math \
	-I${MESA}/tnl \
	-I${GLSL} \
	-I${.OBJDIR}/shader/slang \
	-I${X11BASE}/include \
	-I/usr/include/dev/pci/drm \
	-DGLX_INDIRECT_RENDERING \
	-DGLX_DIRECT_RENDERING \
	-DUSE_EXTERNAL_DXTN_LIB=1 \
	-DPTHREADS \
	-DHAVE_ALIAS \
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
	scissor.c \
	shaderapi.c \
	shaderobj.c \
	shared.c \
	state.c \
	stencil.c \
	syncobj.c \
	texcompress.c \
	texcompress_s3tc.c \
	texcompress_fxt1.c \
	texenv.c \
	texenvprogram.c \
	texfetch.c \
	texformat.c \
	texgen.c \
	texgetimage.c \
	teximage.c \
	texobj.c \
	texpal.c \
	texparam.c \
	texrender.c \
	texstate.c \
	texstore.c \
	transformfeedback.c \
	uniforms.c \
	varray.c \
	version.c \
	viewport.c \
	vtxfmt.c \
	$(MAIN_ES_SOURCES)

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
	s_accum.c \
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
	s_readpix.c \
	s_span.c \
	s_stencil.c \
	s_texcombine.c \
	s_texfilter.c \
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
	vbo_rebase.c \
	vbo_split.c \
	vbo_split_copy.c \
	vbo_split_inplace.c \
	vbo_save.c \
	vbo_save_api.c \
	vbo_save_draw.c \
	vbo_save_loopback.c 

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
	x86_xform.c \
	3dnow.c \
	sse.c \
	x86sse.c

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
ASM_C_SOURCES= \
	x86-64.c

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
.if ${MACHINE_ARCH} == powerpc
ASM_C_SOURCES= \
	common_ppc.c
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
	$(SHADER_CXX_SOURCES)

SRCS=	${MESA_SOURCES}	\
	${MESA_CXX_SOURCES} \
	${ASM_SOURCES} \
	_atexit.c

OBJS+=  ${SRCS:N*.h:R:S/$/.so/g}

LIB=	libmesa

all: ${LIB}_pic.a

obj: _xenocara_obj

install:
	@echo "Not installing libmesa"

clean: 
	rm -f ${LIB}_pic.a ${OBJS} $(CLEANFILES)

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
	@echo "${COMPILE.c:N-O*} ${PICFLAG} -DPIC ${.ALLSRC} -o ${.TARGET}"
	@${COMPILE.c:N-O*} ${PICFLAG} -DPIC ${.ALLSRC} -o ${.TARGET}.o
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


${LIB}_pic.a:	${OBJS} $(DPADD)
	@rm -f ${LIB}_pic.a
	@${AR} cq ${LIB}_pic.a `${LORDER} ${OBJS} | tsort -q`
	${RANLIB} ${LIB}_pic.a

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

.if ${MACHINE_ARCH} == i386
.PATH: ${MESA}/x86
.PATH: ${MESA}/x86/rtasm
.endif
.if ${MACHINE_ARCH} == powerpc
.PATH: ${MESA}/ppc
.endif
.if ${MACHINE_ARCH} == sparc
.PATH: ${MESA}/sparc
.endif
.if ${MACHINE_ARCH} == amd64
.PATH: ${MESA}/x86-64
.PATH: ${MESA}/x86
.endif
.PATH: ${MAPI}/glapi
.PATH: ${MESA}/main
.PATH: ${MESA}/math
.PATH: ${MESA}/swrast
.PATH: ${MESA}/swrast_setup
.PATH: ${MESA}/tnl
.PATH: ${MESA}/program
.PATH: ${MESA}/vbo
.PATH: ${MESA}/drivers/common
.PATH: ${.CURDIR}/generated

.include <bsd.xorg.mk>

.if ${MACHINE_ARCH} == powerpc || ${MACHINE_ARCH} == sparc
PICFLAG=-fPIC
.endif
