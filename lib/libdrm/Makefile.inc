# $OpenBSD: Makefile.inc,v 1.3 2012/11/27 14:37:08 mpi Exp $

PACKAGE_VERSION=	2.4.31

includes: _SUBDIRUSE
	cd ${.CURDIR}; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done
