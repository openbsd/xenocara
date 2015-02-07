# $OpenBSD: Makefile.inc,v 1.14 2015/02/07 01:34:35 jsg Exp $

PACKAGE_VERSION=	2.4.59

NOPROFILE=

includes: _SUBDIRUSE
	cd ${.CURDIR}; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done
