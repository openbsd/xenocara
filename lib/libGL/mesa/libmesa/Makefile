# $OpenBSD: Makefile,v 1.5 2015/08/22 00:50:54 jsg Exp $

LIB=	mesa

CPPFLAGS+= \
	-I${X11BASE}/include/libdrm \
	-I${.CURDIR}/../generated \
	-I${.CURDIR}/../generated/main \
	-DGLX_INDIRECT_RENDERING \
	-DGLX_DIRECT_RENDERING \
	-DUSE_EXTERNAL_DXTN_LIB=1 \

SRCS=	${MESA_SOURCES}	\
	${MESA_CXX_SOURCES} \
	${ASM_SOURCES}

.include <bsd.xorg.mk>
