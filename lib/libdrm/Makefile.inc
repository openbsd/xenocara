# $OpenBSD: Makefile.inc,v 1.6 2013/06/20 09:55:30 jsg Exp $

PACKAGE_VERSION=	2.4.45

NOPROFILE=

includes: _SUBDIRUSE
	cd ${.CURDIR}; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done
