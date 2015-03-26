# $OpenBSD: Makefile.inc,v 1.15 2015/03/26 06:00:06 jsg Exp $

PACKAGE_VERSION=	2.4.60

NOPROFILE=

includes: _SUBDIRUSE
	cd ${.CURDIR}; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done
