# $OpenBSD: Makefile,v 1.3 2013/06/13 10:39:28 jsg Exp $
.include <bsd.xconf.mk>

SUBDIR= rbug softpipe trace

.if ${XENOCARA_BUILD_GALLIUM} == "llvm"
SUBDIR+= llvmpipe
.endif

.if ${XENOCARA_BUILD_GALLIUM:L} == "yes" || ${XENOCARA_BUILD_GALLIUM} == "llvm" &&
    (${MACHINE} == i386 || ${MACHINE} == amd64 || ${MACHINE} == macppc)
SUBDIR+= r300
.endif

build depend all install clean cleandir: _SUBDIRUSE

.include <bsd.xorg.mk>
