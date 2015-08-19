# $OpenBSD: Makefile,v 1.46 2015/08/19 23:00:12 matthieu Exp $
.include <bsd.xconf.mk>
.include "config.mk"

.if ${XENOCARA_BUILD_GALLIUM:L} == "yes" || ${XENOCARA_BUILD_GALLIUM} == "llvm"
SUBDIR+=gallium
MAKE_FLAGS+= XENOCARA_BUILD_GALLIUM=${XENOCARA_BUILD_GALLIUM}
.endif

.if ${XENOCARA_BUILD_DRI:L} == "yes"
SUBDIR+=glsl mesa dri osmesa
.endif

LIB=	GL

MESA=	${.CURDIR}/../../dist/Mesa/src/mesa
MAPI=	${.CURDIR}/../../dist/Mesa/src/mapi
GL=	${.CURDIR}/../../dist/Mesa/Mesa/src/GL
GLX=	${.CURDIR}/../../dist/Mesa/src/glx
LOADER=	${.CURDIR}/../../dist/Mesa/src/loader
MESA_INCLUDE=	${.CURDIR}/../../dist/Mesa/include
MESA_DRI=	${.CURDIR}/../../dist/Mesa/src/mesa/drivers/dri

DRI_DRIVER_INSTALL_DIR=	${X11BASE}/lib/modules/dri

DRI_DRIVER_SEARCH_DIR=	$(DRI_DRIVER_INSTALL_DIR)

INCSDIR=	${X11BASE}/include/GL

.if ${XENOCARA_BUILD_DRI:L} == "yes"
LDADD=		-L${X11BASE}/lib -lX11 -lXext -lXxf86vm -lXdamage -lXfixes \
		-lxcb -lX11-xcb -lxcb-dri2 -lxcb-glx -ldrm -lstdc++ -lm -lc
# XXX link libGL with -lpthread to be able to dlopen() the llvm-pipe module
LDADD+=		-lpthread
.else
LDADD=		-L${X11BASE}/lib -lX11 -lXext -lxcb -lX11-xcb -lxcb-glx
.endif

LDADD+=		-lglapi

CPPFLAGS+= \
	-I. \
	-I${MESA_INCLUDE} \
	-I${MESA_INCLUDE}/GL/internal \
	-I${MESA} \
	-I${MAPI} \
	-I${MAPI}/glapi \
	-I${GLX} \
	-I${LOADER} \
	-I${X11BASE}/include \
	-I/usr/include/dev/pci/drm \
	-I${.CURDIR}/generated \
	-I${.CURDIR}/generated/glx \
	-DGLX_INDIRECT_RENDERING \
	-D_REENTRANT \
	-DHAVE_SHARED_GLAPI \
	-DGLX_SHARED_GLAPI \
	-DMAPI_MODE_BRIDGE \
	-DMAPI_ABI_HEADER=\"${.CURDIR}/generated/glapi/glapi_mapi_tmp.h\"

.if ${XENOCARA_BUILD_DRI:L} == "yes"
CPPFLAGS+= \
	-DGLX_DIRECT_RENDERING -DXF86VIDMODE \
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
	gl_mangle.h \
	glext.h \
	glx.h \
	glx_mangle.h \
	glxext.h \
	osmesa.h

GLX_SOURCES= \
	clientattrib.c \
	clientinfo.c \
	compsize.c \
	create_context.c \
	glxeval.c \
	glxconfig.c \
	glxcmds.c \
	glxcurrent.c \
	glxext.c \
	glxextensions.c \
	indirect.c \
	indirect_glx.c \
	indirect_init.c \
	indirect_window_pos.c \
	indirect_size.c \
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
	glx_error.c \
	glx_pbuffer.c \
	glx_query.c \
	drisw_glx.c \
	dri_common.c \
	dri_glx.c \
	XF86dri.c \
	glxhash.c \
	dri2_glx.c \
	dri2.c \
	dri2_query_renderer.c \
	query_renderer.c \
	applegl_glx.c

LOADER_SOURCES = \
	loader.c \
	pci_id_driver_map.c

MAPI_BRIDGE_SOURCES = \
	entry.c

SRCS =	$(GLX_SOURCES) \
	$(GLAPI_SOURCES) \
	$(MAPI_BRIDGE_SOURCES) \
	$(LOADER_SOURCES)

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

# name conflicts
glxeval.c: ${GLX}/eval.c
	ln -s ${GLX}/eval.c $@

glxpixel.c: ${GLX}/pixel.c
	ln -s ${GLX}/pixel.c $@

glxpixelstore.c: ${GLX}/pixelstore.c
	ln -s ${GLX}/pixelstore.c $@

CLEANFILES+= glxeval.c glxpixel.c glxpixelstore.c

gl.pc: ${MESA}/gl.pc.in Makefile
	sed -e 's,@prefix@,$(X11BASE),' \
	    -e 's,@libdir@,${LIBDIR},' \
	    -e 's,@includedir@,${INCSDIR},' \
	    -e 's,@GL_PC_REQ_PRIV@,libdrm >= 2.4.38 x11 xext xdamage xfixes x11-xcb xcb-glx >= 1.8.1 xcb-dri2 >= 1.8 xxf86vm,' \
	    -e 's,@GL_PC_LIB_PRIV@,-lm -lpthread,' \
	    -e 's,@GL_PC_CFLAGS@,,' \
	    -e 's,@GL_LIB@,GL,' \
	    -e 's,@PACKAGE_VERSION@,$(MESA_MAJOR).$(MESA_MINOR).$(MESA_TINY),' \
	    -e 's,@GLX_TLS@,no,' \
	< ${MESA}/gl.pc.in > gl.pc

.if ${XENOCARA_BUILD_DRI:L} == "yes"
DRI_PC=dri.pc

dri.pc: ${MESA_DRI}/dri.pc.in Makefile
	sed -e 's,@prefix@,${X11BASE},' \
	    -e 's,@exec_prefix@,${X11BASE},' \
	    -e 's,@libdir@,${LIBDIR},' \
	    -e 's,@includedir@,${INCSDIR},' \
	    -e 's,@DRI_DRIVER_INSTALL_DIR@,${DRI_DRIVER_INSTALL_DIR},' \
	    -e 's,@VERSION@,$(MESA_MAJOR).$(MESA_MINOR).$(MESA_TINY),' \
	    -e 's,@DRI_PC_REQ_PRIV@,libdrm >= 2.4.38,' \
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
.PATH: ${MESA}/main
.PATH: ${MAPI}/glapi
.PATH: ${MAPI}
.PATH: ${GLX}
.PATH: ${LOADER}
.PATH: ${.CURDIR}/generated
.PATH: ${.CURDIR}/generated/glapi
.PATH: ${.CURDIR}/generated/glx

.include <bsd.subdir.mk>
