#	$OpenBSD: Makefile,v 1.9 2010/05/22 20:14:14 matthieu Exp $

SUBDIR= swrast

.if ${MACHINE} == i386 || ${MACHINE} == amd64
SUBDIR+= i810 i915 i965 \
	 radeon r200 r300
# not ready
# mach64 mga r128  s3v savage sis tdfx trident
.endif
#.if ${MACHINE} == sparc64
#SUBDIR+= mach64 r128 r200 r300 radeon
#.endif

build depend all install clean cleandir: _SUBDIRUSE

MESA_INCLUDE=	${.CURDIR}/../../../dist/Mesa/include

INCS = \
	dri_interface.h

includes:
	cd ${MESA_INCLUDE}/GL/internal; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/GL/internal/$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/GL/internal"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

.include <bsd.xorg.mk>


