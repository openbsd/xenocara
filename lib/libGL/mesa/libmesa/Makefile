# $OpenBSD: Makefile,v 1.4 2014/07/09 21:32:41 jsg Exp $

LIB=	mesa

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
