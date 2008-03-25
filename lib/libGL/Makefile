# $OpenBSD: Makefile,v 1.18 2008/03/25 23:41:50 matthieu Exp $
.include <bsd.xconf.mk>

.if ${XENOCARA_BUILD_DRI:L} == "yes"
SUBDIR=libmesa dri
.endif

LIB=	GL

MESA=	${.CURDIR}/../../dist/Mesa/src/mesa
GL=	${.CURDIR}/../../dist/Mesa/src/GL
GLX=	${.CURDIR}/../../dist/Mesa/src/glx/x11
MESA_INCLUDE=	${.CURDIR}/../../dist/Mesa/include

MESA_MAJOR=	7
MESA_MINOR=	0
MESA_TINY=	2

DRI_DRIVER_INSTALL_DIR=	${X11BASE}/lib/modules/dri

DRI_DRIVER_SEARCH_DIR=	$(DRI_DRIVER_INSTALL_DIR)

INCSDIR=	${X11BASE}/include/GL

.if ${XENOCARA_BUILD_DRI:L} == "yes"
LDADD=		-L${X11BASE}/lib -lX11 -lXext -lXxf86vm -lXdamage -lXfixes -ldrm
.else
LDADD=		-L${X11BASE}/lib -lXext -lX11
.endif

CPP=	cpp -notraditional
DEBUG?=

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
	-I${GLX} \
	-I${X11BASE}/include \
	-I${X11BASE}/include/drm \
	-DGLX_INDIRECT_RENDERING \
	-DPTHREADS \
	-DHAVE_ALIAS \
	-D_REENTRANT

.if ${XENOCARA_BUILD_DRI:L} == "yes"
CPPFLAGS+=	-DGLX_DIRECT_RENDERING
.endif

.if ${MACHINE_ARCH} == "i386"
CPPFLAGS += \
	-DUSE_X86_ASM \
	-DUSE_MMX_ASM \
	-DUSE_3DNOW_ASM \
	-DUSE_SSE_ASM
.elif ${MACHINE_ARCH} == "amd64"
CPPFLAGS += -D__GLX_ALIGN64 -DUSE_X86_64_ASM
.endif

INCS= \
	gl.h \
	glext.h \
	glx.h \
	glxext.h \
	osmesa.h

GLAPI_SOURCES= \
	dispatch.c \
	glapi.c \
	glthread.c \
	$(ASM_API)

.if ${MACHINE_ARCH} == i386
ASM_API = glapi_x86.S
.endif

.if ${MACHINE_ARCH} == amd64
ASM_API = glapi_x86-64.S
.endif

.if ${MACHINE_ARCH} == sparc
SPARC_API = glapi_sparc.S
.endif

COMMON_DRIVER_SOURCES = \
	driverfuncs.c

X11_DRIVER_SOURCES = 	\
	glxapi.c	\
	fakeglx.c	\
	xfonts.c	\
	xm_api.c	\
	xm_buffer.c	\
	xm_dd.c	\
	xm_glide.c	\
	xm_line.c	\
	xm_span.c	\
	xm_tri.c

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
	dri_glx.c \
	XF86dri.c

SRCS = $(GLX_SOURCES)	\
	$(GLAPI_SOURCES) 

includes:
	cd ${MESA_INCLUDE}/GL; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/GL/$$i || \
		${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 \
		$$i ${DESTDIR}${INCSDIR}/GL"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

NOPROFILE=

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.if ${MACHINE_ARCH} == powerpc || ${MACHINE_ARCH} == sparc
PICFLAG = -fPIC
.endif

# name conflicts
glxeval.c: ${GLX}/eval.c
	ln -s ${GLX}/eval.c $@

glxpixel.c: ${GLX}/pixel.c
	ln -s ${GLX}/pixel.c $@

CLEANFILES+= glxeval.c glxpixel.c

gl.pc: ${MESA}/gl.pc.in Makefile
	sed -e 's,@INSTALL_DIR@,$(X11BASE),' \
	    -e 's,@LIB_DIR@,lib,' \
	    -e 's,@VERSION@,$(MESA_MAJOR).$(MESA_MINOR).$(MESA_TINY),' \
	< ${MESA}/gl.pc.in > gl.pc

afterinstall: gl.pc
	$(INSTALL) -c -m 644 -o root -g wheel gl.pc \
		${DESTDIR}${LIBDIR}/pkgconfig

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
.PATH: ${MESA}/drivers/x11
.PATH: ${GLX}

.include <bsd.subdir.mk>
