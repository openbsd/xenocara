# $OpenBSD: Makefile,v 1.4 2013/06/17 23:30:25 jsg Exp $
.include <bsd.xconf.mk>

SUBDIR= rbug softpipe trace

.if ${XENOCARA_BUILD_GALLIUM} == "llvm"
SUBDIR+= llvmpipe
.endif

.if ${XENOCARA_BUILD_GALLIUM:L} == "yes" || ${XENOCARA_BUILD_GALLIUM} == "llvm" &&
    (${MACHINE} == i386 || ${MACHINE} == amd64 || ${MACHINE} == macppc)
SUBDIR+= r300 r600
.endif

build depend all install clean cleandir: _SUBDIRUSE

.include <bsd.xorg.mk>
