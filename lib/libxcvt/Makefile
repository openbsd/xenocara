# $OpenBSD: Makefile,v 1.4 2023/09/06 20:36:50 matthieu Exp $

SUBDIR=	 lib cvt

INCS=	libxcvt.h libxcvt_mode.h

PACKAGE_VERSION= 	0.1.2
PKGCONFIG=              libxcvt.pc
EXTRA_PKGCONFIG_SUBST=	-e 's/@VERSION@/${PACKAGE_VERSION}/'
obj: _xenocara_obj


includes: _SUBDIRUSE
	cd ${.CURDIR}/include/libxcvt; for i in ${INCS}; do \
            j="cmp -s $$i ${DESTDIR}${INCSDIR}/libxcvt/$$i || \
                ${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/libxcvt"; \
                echo "\tinstalling $$i"; \
                eval "$$j"; \
        done

beforeinstall: includes 

.include <bsd.subdir.mk>
.include <bsd.xorg.mk>
