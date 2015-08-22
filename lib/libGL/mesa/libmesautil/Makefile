# $OpenBSD: Makefile,v 1.2 2015/08/22 00:50:54 jsg Exp $

UTIL=${.CURDIR}/../../../../dist/Mesa/src/util
.include "${UTIL}/Makefile.sources"

LIB=	mesautil

CPPFLAGS+= \
	-I${X11BASE}/include/libdrm \
	-I${.CURDIR}/../generated \
	-I${.CURDIR}/../generated/main \
	-I${UTIL} \
	-DGLX_INDIRECT_RENDERING \
	-DGLX_DIRECT_RENDERING \
	-DUSE_EXTERNAL_DXTN_LIB=1 \

SRCS=	${MESA_UTIL_FILES} \
	${MESA_UTIL_GENERATED_FILES}

.include <bsd.xorg.mk>
.PATH: ${UTIL}
.PATH: ${.CURDIR}/../../generated/mesa_util
