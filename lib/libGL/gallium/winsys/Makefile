# $OpenBSD: Makefile,v 1.3 2015/07/29 05:49:16 mpi Exp $

.if ${MACHINE} == i386 || ${MACHINE} == amd64 || \
    ${MACHINE} == macppc || ${MACHINE} == sparc64
SUBDIR= radeon
.endif

build depend all install clean cleandir: _SUBDIRUSE

.include <bsd.xorg.mk>
