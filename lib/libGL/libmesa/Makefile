# $OpenBSD: Makefile,v 1.5 2008/11/02 15:01:32 matthieu Exp $

MESA=	${.CURDIR}/../../../dist/Mesa/src/mesa
MESA_INCLUDE=	${.CURDIR}/../../../dist/Mesa/include

CPP=	cpp -notraditional

CPPFLAGS+= \
	-I. \
	-I${MESA_INCLUDE} \
	-I${MESA_INCLUDE}/GL/internal \
	-I${MESA} \
	-I${MESA}/main \
	-I${MESA}/math \
	-I${MESA}/glapi \
	-I${MESA}/tnl \
	-I${MESA}/shader \
	-I${MESA}/shader/grammar \
	-I${MESA}/shader/slang \
	-I${X11BASE}/include \
	-I${X11BASE}/include/drm \
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

# main
MAIN_SOURCES= \
	api_arrayelt.c \
	api_loopback.c \
	api_noop.c \
	api_validate.c \
	accum.c \
	attrib.c \
	arrayobj.c \
	blend.c \
	bufferobj.c \
	buffers.c \
	clip.c \
	colortab.c \
	context.c \
	convolve.c \
	debug.c \
	depth.c \
	depthstencil.c \
	dlist.c \
	drawpix.c \
	enable.c \
	enums.c \
	eval.c \
	execmem.c \
	extensions.c \
	fbobject.c \
	feedback.c \
	fog.c \
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
	pixel.c \
	points.c \
	polygon.c \
	queryobj.c \
	rastpos.c \
	rbadaptors.c \
	renderbuffer.c \
	shaders.c \
	state.c \
	stencil.c \
	texcompress.c \
	texcompress_s3tc.c \
	texcompress_fxt1.c \
	texenvprogram.c \
	texformat.c \
	teximage.c \
	texobj.c \
	texrender.c \
	texstate.c \
	texstore.c \
	varray.c \
	vtxfmt.c

# math
MATH_SOURCES= \
	m_debug_clip.c \
	m_debug_norm.c \
	m_debug_xform.c \
	m_eval.c \
	m_matrix.c \
	m_translate.c \
	m_vector.c \
	m_xform.c

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

# tnl
TNL_SOURCES= \
	t_context.c \
	t_pipeline.c \
	t_draw.c \
	t_vb_program.c \
	t_vb_render.c \
	t_vb_texgen.c \
	t_vb_texmat.c \
	t_vb_vertex.c \
	t_vb_cull.c \
	t_vb_fog.c \
	t_vb_light.c \
	t_vb_normals.c \
	t_vb_points.c \
	t_vp_build.c \
	t_vertex.c \
	t_vertex_sse.c \
	t_vertex_generic.c \

# shader
SHADER_SOURCES=	\
	arbprogparse.c \
	arbprogram.c \
	atifragshader.c \
	grammar_mesa.c \
	nvfragparse.c \
	nvprogram.c \
	nvvertparse.c \
	program.c \
	prog_debug.c \
	prog_execute.c \
	prog_instruction.c \
	prog_parameter.c \
	prog_print.c \
	prog_statevars.c \
	prog_uniform.c \
	programopt.c \
	shader_api.c \

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
	s_buffers.c \
	s_copypix.c \
	s_context.c \
	s_depth.c \
	s_drawpix.c \
	s_feedback.c \
	s_fog.c \
	s_fragprog.c \
	s_imaging.c \
	s_lines.c \
	s_logic.c \
	s_masking.c \
	s_points.c \
	s_readpix.c \
	s_span.c \
	s_stencil.c \
	s_texcombine.c \
	s_texfilter.c \
	s_texstore.c \
	s_triangle.c \
	s_zoom.c

# swrast_setup
SWRAST_SETUP_SOURCES= \
	ss_context.c \
	ss_triangle.c

COMMON_DRIVER_SOURCES = \
	driverfuncs.c

# shader/slang
SLANG_SOURCES= \
	slang_builtin.c	\
	slang_codegen.c	\
	slang_compile.c	\
	slang_compile_function.c	\
	slang_compile_operation.c	\
	slang_compile_struct.c	\
	slang_compile_variable.c	\
	slang_emit.c	\
	slang_ir.c	\
	slang_label.c	\
	slang_library_noise.c	\
	slang_link.c	\
	slang_log.c	\
	slang_mem.c	\
	slang_preprocess.c	\
	slang_print.c	\
	slang_simplify.c	\
	slang_storage.c	\
	slang_typeinfo.c	\
	slang_vartable.c	\
	slang_utility.c

.if ${MACHINE_ARCH} == i386
ASM_C_SOURCES= \
	common_x86.c \
	x86.c \
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

SOLO_SOURCES = \
	$(MAIN_SOURCES)		\
	$(MATH_SOURCES)		\
	$(VBO_SOURCES)		\
	$(TNL_SOURCES)		\
	$(SHADER_SOURCES)	\
	$(SWRAST_SOURCES)	\
	$(SWRAST_SETUP_SOURCES) \
	$(COMMON_DRIVER_SOURCES) \
	$(ASM_C_SOURCES)	\
	$(SLANG_SOURCES)

SRCS=	${SOLO_SOURCES}	\
	${ASM_SOURCES}

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
.SUFFIXES: .a .c .o .S .s .so

.c.so:
	@echo "${COMPILE.c} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.c} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}.o
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
.PATH: ${MESA}/glapi
.PATH: ${MESA}/main
.PATH: ${MESA}/math
.PATH: ${MESA}/swrast
.PATH: ${MESA}/swrast_setup
.PATH: ${MESA}/tnl
.PATH: ${MESA}/shader
.PATH: ${MESA}/shader/grammar
.PATH: ${MESA}/shader/slang
.PATH: ${MESA}/shader/slang/MachineIndependent
.PATH: ${MESA}/shader/slang/MachineIndependent/preprocessor
.PATH: ${MESA}/shader/slang/OGLCompilersDLL
.PATH: ${MESA}/shader/slang/OSDependent/Linux
.PATH: ${MESA}/vbo
.PATH: ${MESA}/drivers/common

.include <bsd.xorg.mk>

# Prevent a gcc ICE
.if ${MACHINE_ARCH} == mips64
s_copypix.so: s_copypix.c
	${CC} -c -O0 ${CPPFLAGS} -fpic -DPIC $<
.endif

