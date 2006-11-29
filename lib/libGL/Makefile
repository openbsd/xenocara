# $Xenocara$

all: ${OBJS}
LIB=	GL

MESA=	${.CURDIR}/../../dist/Mesa/src/mesa
GL=	${.CURDIR}/../../dist/Mesa/src/GL
GLX=	${.CURDIR}/../../dist/Mesa/src/glx/x11
MESA_INCLUDE=	${.CURDIR}/../../dist/Mesa/include

INCSDIR=	${X11BASE}/include/GL

CPP=	cpp -notraditional
DEBUG=	

CPPFLAGS+= \
	-I. \
	-I${MESA_INCLUDE} \
	-I${MESA} \
	-I${MESA}/main \
	-I${MESA}/math \
	-I${MESA}/glapi \
	-I${MESA}/tnl \
	-I${MESA}/shader \
	-I${MESA}/shader/grammar \
	-I${MESA}/shader/slang \
	-I${MESA}/shader/slang/OSDependent/Linux \
	-I${MESA}/shader/slang/OGLCompilersDLL \
	-I${MESA}/x86 \
	-I${MESA}/drivers/dri/common \
	-I${GL}/dri \
	-I${GLX} \
	-I${X11BASE}/include \
	-DCSRG_BASED  \
	-DUSE_GAS  \
	-DGLXEXT  \
	-DGLX_USE_MESA  \
	-DXF86VIDMODE 

.if ${MACHINE_ARCH} == "i386"
CPPFLAGS += \
	-DUSE_X86_ASM \
	-DUSE_MMX_ASM \
	-DUSE_3DNOW_ASM \
	-DUSE_SSE_ASM
.elif ${MACHINE_ARCH} == "amd64"
CPPFLAGS += -D__GLX_ALIGN64
.endif

INCS= \
	gl.h \
	glext.h \
	glx.h \
	glxext.h \
	osmesa.h

# main
MAIN_SOURCES= \
	accum.c \
	api_arrayelt.c \
	api_loopback.c \
	api_noop.c \
	api_validate.c \
	arrayobj.c  \
	attrib.c \
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
	feedback.c \
	fbobject.c \
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
	mm.c \
	occlude.c \
	pixel.c \
	points.c \
	polygon.c \
	rastpos.c \
	renderbuffer.c \
	state.c \
	stencil.c \
	texcompress.c \
	texcompress_fxt1.c \
	texcompress_s3tc.c \
	texenvprogram.c \
	texformat.c \
	teximage.c \
	texobj.c \
	texrender.c \
	texstate.c \
	texstore.c \
	varray.c \
	vtxfmt.c

GLAPI_SOURCES= \
	dispatch.c \
	glapi.c \
	glthread.c

.if ${MACHINE_ARCH} == "i386"
GLAPI_SOURCES+= \
	glapi_x86.S 
.endif

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

# array_cache
ARRAY_CACHE_SOURCES= \
	ac_context.c \
	ac_import.c


# swrast
SWRAST_SOURCES= \
	s_aaline.c \
	s_aatriangle.c \
	s_accum.c \
	s_alpha.c \
	s_arbshader.c \
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
	s_imaging.c \
	s_lines.c \
	s_logic.c \
	s_masking.c \
	s_nvfragprog.c \
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

# tnl
TNL_SOURCES= \
	t_array_api.c \
	t_array_import.c \
	t_context.c \
	t_pipeline.c \
	t_save_api.c \
	t_save_loopback.c \
	t_save_playback.c \
	t_vb_arbprogram.c \
	t_vb_arbprogram_sse.c \
	t_vb_arbshader.c\
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
	t_vtx_api.c \
	t_vtx_generic.c \
	t_vtx_x86.c \
	t_vtx_eval.c \
	t_vtx_exec.c 

# shader
SHADER_SOURCES=	\
	arbprogparse.c \
	arbprogram.c \
	atifragshader.c \
	grammar_mesa.c \
	nvfragparse.c \
	nvprogram.c \
	nvvertexec.c \
	nvvertparse.c \
	program.c \
	shaderobjects.c \
	shaderobjects_3dlabs.c

# shader/slang C
SLANG_C_SOURCES=	\
	atom.c \
	cpp.c \
	cppstruct.c \
	memory.c \
	scanner.c \
	symbols.c \
	tokens.c

# shader/slang CPP
SLANG_CPP_SOURCES= \
	slang_mesa.cpp	\
	Gen_glslang.cpp	\
	Gen_glslang_tab.cpp	\
	InfoSink.cpp	\
	Initialize.cpp	\
	Intermediate.cpp	\
	intermOut.cpp	\
	IntermTraverse.cpp	\
	parseConst.cpp	\
	ParseHelper.cpp	\
	PoolAlloc.cpp	\
	QualifierAlive.cpp	\
	RemoveTree.cpp	\
	ShaderLang.cpp	\
	SymbolTable.cpp	\
	Initialisation.cpp	\
	ossource.cpp

# shader/slang
SLANG_SOURCES= \
	slang_analyse.c	\
	slang_assemble.c	\
	slang_assemble_assignment.c	\
	slang_assemble_conditional.c	\
	slang_assemble_constructor.c	\
	slang_assemble_typeinfo.c	\
	slang_compile.c	\
	slang_compile_function.c	\
	slang_compile_operation.c	\
	slang_compile_struct.c	\
	slang_compile_variable.c	\
	slang_execute.c	\
	slang_export.c	\
	slang_library_noise.c	\
	slang_library_texsample.c	\
	slang_link.c	\
	slang_preprocess.c	\
	slang_storage.c	\
	slang_utility.c

.if ${MACHINE_ARCH} == "i386"
# x86
ASM_C_SOURCES= \
	common_x86.c \
	x86.c \
	3dnow.c \
	sse.c \
	x86sse.c \
	slang_execute_x86.c

X86_SOURCES= \
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
	read_rgba_span_x86.S \
	t_vtx_x86_gcc.S
.endif

GLX_SOURCES= \
	  glcontextmodes.c \
	  clientattrib.c \
	  compsize.c \
	  glxeval.c \
	  glxcmds.c \
	  glxext.c \
	  glxextensions.c \
	  indirect.c \
	  indirect_init.c \
	  indirect_size.c \
	  indirect_window_pos.c \
	  indirect_transpose_matrix.c \
	  indirect_vertex_array.c \
	  indirect_vertex_program.c \
	  glxpixel.c \
	  pixelstore.c \
	  render2.c \
	  renderpix.c \
	  single2.c \
	  singlepix.c \
	  vertarr.c \
	  xfont.c \
	  glx_pbuffer.c \
	  glx_query.c \
	  glx_texture_compression.c \
	  dri_glx.c

SRCS = 	$(MAIN_SOURCES)		\
	$(MATH_SOURCES)		\
	$(ARRAY_CACHE_SOURCES)	\
	$(TNL_SOURCES)		\
	$(SHADER_SOURCES)	\
	$(SWRAST_SOURCES)	\
	$(SWRAST_SETUP_SOURCES)	\
	$(ASM_C_SOURCES)	\
	$(SLANG_SOURCES)	\
	$(X86_SOURCES)		\
	$(GLAPI_SOURCES)	\
	$(GLX_SOURCES)

#	$(SLANG_C_SOURCES)


includes:
	cd ${MESA_INCLUDE}/GL; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/GL/$$i || \
		${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 \
		$$i ${DESTDIR}${INCSDIR}/GL"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

gen_matypes: gen_matypes.c
	$(CC) $(CPPFLAGS) $(CFLAGS) ${MESA}/x86/gen_matypes.c -o gen_matypes

matypes.h: mtypes.h t_context.h gen_matypes
	./gen_matypes > matypes.h

CLEANFILES+= gen_matypes matypes.h

common_x86_asm.o: matypes.h
3dnow_normal.o: matypes.h
3dnow_xform1.o: matypes.h
3dnow_xform2.o: matypes.h
3dnow_xform3.o: matypes.h
3dnow_xform4.o: matypes.h
mmx_blend.o: matypes.h
sse_normal.o: matypes.h
sse_xform1.o: matypes.h
sse_xform2.o: matypes.h
sse_xform3.o: matypes.h
sse_xform4.o: matypes.h
x86_cliptest.o: matypes.h
x86_xform2.o: matypes.h
x86_xform3.o: matypes.h
x86_xform4.o: matypes.h

NOPROFILE=

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.if ${MACHINE_ARCH} == powerpc
PICFLAG = -fPIC
.endif

# name conflicts
glxeval.c: ${GLX}/eval.c
	ln -s ${GLX}/eval.c $@

glxpixel.c: ${GLX}/pixel.c
	ln -s ${GLX}/pixel.c $@

CLEANFILES+= glxeval.c glxpixel.c

.PATH: ${MESA}/x86
.PATH: ${MESA}/x86/rtasm
.PATH: ${MESA}/array_cache
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
.PATH: ${MESA}/drivers/dri/common
.PATH: ${GLX}

.SUFFIXES: .cpp

.cpp.o:
	@echo "${COMPILE.cc} ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.cc} ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

.cpp.go:
	@echo "${COMPILE.cc} -g ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.cc} -g ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

.cpp.po:
	@echo "${COMPILE.cc} -p ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.cc} -p ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

.cpp.so:
	@echo "${COMPILE.cc} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.cc} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

