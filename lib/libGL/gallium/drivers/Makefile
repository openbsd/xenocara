# $OpenBSD: Makefile,v 1.1.1.1 2012/07/13 12:43:34 mpi Exp $

SUBDIR= rbug softpipe trace

.ifdef XENOCARA_HAVE_LLVM
SUBDIR+= llvmpipe
.endif

build depend all install clean cleandir: _SUBDIRUSE

.include <bsd.xorg.mk>
