# $OpenBSD: Makefile,v 1.2 2013/09/05 15:12:09 jsg Exp $

LIB=	mesa

CFLAGS+= -std=c99

CPPFLAGS+= \
	-I/usr/include/dev/pci/drm \
	-I${.CURDIR}/../generated \
	-I${.CURDIR}/../generated/main \
	-DGLX_INDIRECT_RENDERING \
	-DGLX_DIRECT_RENDERING \
	-DUSE_EXTERNAL_DXTN_LIB=1 \

SRCS=	${MESA_SOURCES}	\
	${MESA_CXX_SOURCES} \
	${ASM_SOURCES} \
	_atexit.c

.include <bsd.xorg.mk>
