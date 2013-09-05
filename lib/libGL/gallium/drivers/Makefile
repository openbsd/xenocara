# $OpenBSD: Makefile,v 1.6 2013/09/05 15:11:59 jsg Exp $
.include <bsd.xconf.mk>

SUBDIR= rbug softpipe trace

.if ${XENOCARA_BUILD_GALLIUM} == "llvm"
SUBDIR+= llvmpipe radeonsi
.endif

.if ${XENOCARA_BUILD_GALLIUM:L} == "yes" || ${XENOCARA_BUILD_GALLIUM} == "llvm" &&
    (${MACHINE} == i386 || ${MACHINE} == amd64 || ${MACHINE} == macppc || ${MACHINE} == sparc64)
SUBDIR+= r300 r600
.endif

build depend all install clean cleandir: _SUBDIRUSE

.include <bsd.xorg.mk>
