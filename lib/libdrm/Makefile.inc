# $OpenBSD: Makefile.inc,v 1.12 2014/05/13 05:44:06 jsg Exp $

PACKAGE_VERSION=	2.4.54

NOPROFILE=

includes: _SUBDIRUSE
	cd ${.CURDIR}; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done
