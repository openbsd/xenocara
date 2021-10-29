# $OpenBSD: Makefile,v 1.10 2021/10/29 15:57:09 matthieu Exp $
.include <bsd.xconf.mk>

EPOXY=	${.CURDIR}/../../dist/libepoxy

LIB=	epoxy

EPOXY_MAJOR=	1
EPOXY_MINOR=	5
EPOXY_TINY=	9

INCSDIR=	${X11BASE}/include/epoxy

CFLAGS+= -std=gnu99

.if ${MACHINE_ARCH} == "mips64el" || ${MACHINE_ARCH} == "mips64"
PICFLAG+= -mxgot
.endif

CPPFLAGS+= \
	-I${.CURDIR} \
	-I${.CURDIR}/generated/include \
	-I${EPOXY}/include \
	-I${EPOXY}/src \
	-I${X11BASE}/include

SRCS=	dispatch_common.c \
	dispatch_glx.c \
	dispatch_egl.c \
	gl_generated_dispatch.c \
	glx_generated_dispatch.c \
	egl_generated_dispatch.c

INCS=	common.h \
	gl.h \
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

beforeinstall: includes 

NOPROFILE=

PKGCONFIG=		epoxy.pc
PACKAGE_VERSION=	$(EPOXY_MAJOR).$(EPOXY_MINOR).$(EPOXY_TINY)
EXTRA_PKGCONFIG_SUBST=	-e 's,@DLOPEN_LIBS@,,' \
			 -e 's,@GL_REQS@,gl egl,' \
			 -e 's,@epoxy_has_glx@,1,' \
			 -e 's,@epoxy_has_egl@,1,' \
			 -e 's,@epoxy_has_wgl@,0,'

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${.CURDIR}/generated/src
.PATH: ${EPOXY}
.PATH: ${EPOXY}/src
