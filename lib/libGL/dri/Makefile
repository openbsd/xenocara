#	$OpenBSD: Makefile,v 1.15 2013/06/13 10:39:28 jsg Exp $
.include <bsd.xconf.mk>

SUBDIR= swrast

.if ${MACHINE} == i386 || ${MACHINE} == amd64
SUBDIR+= i810 i915 i965 radeon r200 r300 r600
.elif ${MACHINE} == macppc
SUBDIR+= radeon r200 r300
.endif

.if ${XENOCARA_BUILD_GALLIUM:L} == "yes" || ${XENOCARA_BUILD_GALLIUM} == "llvm"
SUBDIR+=swrastg
.if ${MACHINE} == i386 || ${MACHINE} == amd64 || ${MACHINE} == macppc
SUBDIR+= r300g
.endif
.endif

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


