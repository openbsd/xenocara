# $OpenBSD: Makefile,v 1.18 2016/10/10 13:27:14 matthieu Exp $

DEBUG?=

LIB= GLw

MESA_MAJOR=	7
MESA_MINOR=	11
MESA_TINY=	2

CPPFLAGS += -I${X11BASE}/include \
	-I${.CURDIR} -I${.CURDIR}/GL

SRCS= \
	GLwDrawA.c \
	GLwDAUtil.c \
	GLwM1DrawA.c \
	GLwM2DrawA.c

INCS= \
	GLwDrawA.h \
	GLwDrawAP.h \
	GLwMDrawA.h \
	GLwMDrawAP.h

obj: _xenocara_obj

includes:
	cd ${.CURDIR}/GL; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/GL/$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/GL"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

beforeinstall: includes

NOPROFILE=

PKGCONFIG=	glw.pc
PACKAGE_VERSION=$(MESA_MAJOR).$(MESA_MINOR).$(MESA_TINY)
EXTRA_PKGCONFIG_SUBST= \
	-e 's,@INSTALL_DIR@,$(X11BASE),' \
	-e 's,@LIB_DIR@,lib,' \
	-e 's,@VERSION@,$(PACKAGE_VERSION),' \

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

