# $OpenBSD: Makefile,v 1.3 2013/12/30 18:32:48 matthieu Exp $

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
	${ASM_SOURCES}

.include <bsd.xorg.mk>
