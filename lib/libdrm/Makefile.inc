# $OpenBSD: Makefile.inc,v 1.5 2013/03/25 02:32:20 jsg Exp $

PACKAGE_VERSION=	2.4.42

NOPROFILE=

includes: _SUBDIRUSE
	cd ${.CURDIR}; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done
