#	$OpenBSD: Makefile,v 1.17 2013/08/12 04:16:52 jsg Exp $
.include <bsd.xconf.mk>

SUBDIR= swrast

.if ${MACHINE} == i386 || ${MACHINE} == amd64
SUBDIR+= i810 i915 i965 radeon r200
.elif ${MACHINE} == macppc || ${MACHINE} == sparc64
SUBDIR+= radeon r200
.endif

.if ${XENOCARA_BUILD_GALLIUM} == "llvm"
SUBDIR+=swrastg
.endif

.if ${XENOCARA_BUILD_GALLIUM:L} == "yes" || ${XENOCARA_BUILD_GALLIUM} == "llvm"
.if ${MACHINE} == i386 || ${MACHINE} == amd64 || ${MACHINE} == macppc || ${MACHINE} == sparc64
SUBDIR+= r300g r600g
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


