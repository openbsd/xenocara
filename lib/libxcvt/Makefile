# $OpenBSD: Makefile,v 1.2 2021/10/27 16:24:02 matthieu Exp $

SUBDIR=	 lib cvt

INCS=	libxcvt.h libxcvt_mode.h

PACKAGE_VERSION= 	0.1.1
PKGCONFIG=              libxcvt.pc

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
