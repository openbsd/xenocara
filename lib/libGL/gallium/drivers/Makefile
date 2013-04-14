# $OpenBSD: Makefile,v 1.2 2013/04/14 10:23:04 mpi Exp $
.include <bsd.xconf.mk>

SUBDIR= rbug softpipe trace

.if ${XENOCARA_BUILD_GALLIUM} == "llvm"
SUBDIR+= llvmpipe
.endif

build depend all install clean cleandir: _SUBDIRUSE

.include <bsd.xorg.mk>
