# $OpenBSD: Makefile,v 1.2 2015/04/22 17:31:47 miod Exp $
.include <bsd.xconf.mk>

EPOXY=	${.CURDIR}/../../dist/libepoxy

LIB=	epoxy

EPOXY_MAJOR=	1
EPOXY_MINOR=	2
EPOXY_TINY=	0

INCSDIR=	${X11BASE}/include/epoxy

CFLAGS+= -std=gnu99

CPPFLAGS+= \
	-I${.CURDIR}/generated/include \
	-I${EPOXY}/include \
	-I${EPOXY}/src \
	-I${X11BASE}/include \
	-DPLATFORM_HAS_GLX \
	-DPLATFORM_HAS_EGL

SRCS=	dispatch_common.c \
	dispatch_glx.c \
	dispatch_egl.c \
	gl_generated_dispatch.c \
	glx_generated_dispatch.c \
	egl_generated_dispatch.c

INCS=	gl.h \
	glx.h \
	egl.h

GINCS=	gl_generated.h \
	glx_generated.h \
	egl_generated.h

includes: _SUBDIRUSE
	cd ${EPOXY}/include/epoxy; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/epoxy/$$i || \
		${INSTALL_DATA}	$$i ${DESTDIR}${INCSDIR}/epoxy"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done
	cd ${.CURDIR}/generated/include/epoxy; for i in ${GINCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/epoxy/$$i || \
		${INSTALL_DATA}	$$i ${DESTDIR}${INCSDIR}/epoxy"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

NOPROFILE=

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.if ${MACHINE_ARCH} == "sparc"
PICFLAG = -fPIC
.endif

epoxy.pc: ${EPOXY}/epoxy.pc.in Makefile
	sed -e 's,@prefix@,$(X11BASE),' \
	    -e 's,@exec_prefix@,$(X11BASE),' \
	    -e 's,@libdir@,${LIBDIR},' \
	    -e 's,@includedir@,${INCSDIR},' \
	    -e 's,@DLOPEN_LIBS@,,' \
	    -e 's,@PACKAGE_VERSION@,$(EPOXY_MAJOR).$(EPOXY_MINOR),' \
	< ${EPOXY}/epoxy.pc.in > epoxy.pc

afterinstall: epoxy.pc
	$(INSTALL) -c -m 644 -o root -g wheel epoxy.pc \
		${DESTDIR}${LIBDIR}/pkgconfig

CLEANFILES+= epoxy.pc

.PATH: ${EPOXY}/src
.PATH: ${.CURDIR}/generated/src
