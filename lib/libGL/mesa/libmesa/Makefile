# $OpenBSD: Makefile,v 1.1 2012/08/26 18:46:42 mpi Exp $

LIB=	mesa

CPPFLAGS+= \
	-I/usr/include/dev/pci/drm \
	-DGLX_INDIRECT_RENDERING \
	-DGLX_DIRECT_RENDERING \
	-DUSE_EXTERNAL_DXTN_LIB=1 \

SRCS=	${MESA_SOURCES}	\
	${MESA_CXX_SOURCES} \
	${ASM_SOURCES} \
	_atexit.c

.include <bsd.xorg.mk>
