# $OpenBSD: Makefile,v 1.26 2010/05/08 14:15:29 matthieu Exp $
.include <bsd.xconf.mk>

.if ${XENOCARA_BUILD_DRI:L} == "yes"
SUBDIR=libmesa dri
.endif

LIB=	GL

MESA=	${.CURDIR}/../../dist/Mesa/src/mesa
GL=	${.CURDIR}/../../dist/Mesa/src/GL
GLX=	${.CURDIR}/../../dist/Mesa/src/glx/x11
MESA_INCLUDE=	${.CURDIR}/../../dist/Mesa/include
MESA_DRI=	${.CURDIR}/../../dist/Mesa/src/mesa/drivers/dri

MESA_MAJOR=	7
MESA_MINOR=	4
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
	-I${MESA}/glapi \
	-I${GLX} \
	-I${X11BASE}/include \
	-I/usr/include/dev/pci/drm \
	-DGLX_INDIRECT_RENDERING \
	-DPTHREADS \
	-DHAVE_ALIAS \
	-D_REENTRANT

.if ${XENOCARA_BUILD_DRI:L} == "yes"
CPPFLAGS+=	-DGLX_DIRECT_RENDERING -DXF86VIDMODE \
	-DDEFAULT_DRIVER_DIR=\"${DRI_DRIVER_INSTALL_DIR}\"
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
	glapi_getproc.c \
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

GLX_SOURCES= \
	glcontextmodes.c \
	clientattrib.c \
	compsize.c \
	glxeval.c \
	glxcmds.c \
	glxcurrent.c \
	glxext.c \
	glxextensions.c \
	indirect.c \
	indirect_init.c \
	indirect_size.c \
	indirect_window_pos.c \
	indirect_texture_compression.c \
	indirect_transpose_matrix.c \
	indirect_vertex_array.c \
	indirect_vertex_program.c \
	glxpixel.c \
	glxpixelstore.c \
	render2.c \
	renderpix.c \
	single2.c \
	singlepix.c \
	vertarr.c \
	xfont.c \
	glx_pbuffer.c \
	glx_query.c \
	drisw_glx.c \
	dri_common.c \
	dri_glx.c \
	XF86dri.c \
	glxhash.c \
	dri2_glx.c \
	dri2.c

SRCS = $(GLX_SOURCES)	\
	$(GLAPI_SOURCES) 

includes: _SUBDIRUSE
	cd ${MESA_INCLUDE}/GL; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/GL/$$i || \
		${INSTALL_DATA}	$$i ${DESTDIR}${INCSDIR}/GL"; \
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

glxpixelstore.c: ${GLX}/pixelstore.c
	ln -s ${GLX}/pixelstore.c $@

CLEANFILES+= glxeval.c glxpixel.c glxpixelstore.c

gl.pc: ${MESA}/gl.pc.in Makefile
	sed -e 's,@INSTALL_DIR@,$(X11BASE),' \
	    -e 's,@INSTALL_LIB_DIR@,${LIBDIR},' \
	    -e 's,@INSTALL_INC_DIR@,${INCSDIR},' \
	    -e 's,@GL_PC_REQ_PRIV@,libdrm >= 2.4.3 dri2proto >= 1.99.3 x11 xext xxf86vm xdamage xfixes,' \
	    -e 's,@GL_PC_LIB_PRIV@,-L${LIBDIR} -lXxf86vm -lXext -lXdamage -lXfixes -lX11 -lXau -lXdmcp,' \
	    -e 's,@GL_PC_CFLAGS@,-I${INCSDIR},' \
	    -e 's,@VERSION@,$(MESA_MAJOR).$(MESA_MINOR).$(MESA_TINY),' \
	< ${MESA}/gl.pc.in > gl.pc

.if ${XENOCARA_BUILD_DRI:L} == "yes"
DRI_PC=dri.pc

dri.pc: ${MESA_DRI}/dri.pc.in Makefile
	sed -e 's,@INSTALL_DIR@,${X11BASE},' \
	    -e 's,@INSTALL_LIB_DIR@,${LIBDIR},' \
	    -e 's,@INSTALL_INC_DIR@,${INCSDIR},' \
	    -e 's,@DRI_DRIVER_DIR@,${DRI_DRIVER_INSTALL_DIR},' \
	    -e 's,@VERSION@,$(MESA_MAJOR).$(MESA_MINOR).$(MESA_TINY),' \
	    -e 's,@DRI_PC_REQ_PRIV@,,' \
	< ${MESA_DRI}/dri.pc.in > dri.pc
.endif

afterinstall: gl.pc ${DRI_PC}
	$(INSTALL) -c -m 644 -o root -g wheel gl.pc \
		${DESTDIR}${LIBDIR}/pkgconfig
.if ${XENOCARA_BUILD_DRI:L} == "yes"
	$(INSTALL) -c -m 644 -o root -g wheel dri.pc \
		${DESTDIR}${LIBDIR}/pkgconfig
.endif

CLEANFILES+= gl.pc ${DRI_PC}

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
.PATH: ${MESA}/main
.PATH: ${MESA}/glapi
.PATH: ${GLX}

.include <bsd.subdir.mk>
