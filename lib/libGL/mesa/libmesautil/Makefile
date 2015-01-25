# $OpenBSD: Makefile,v 1.1 2015/01/25 15:09:14 jsg Exp $

UTIL=${.CURDIR}/../../../../dist/Mesa/src/util
.include "${UTIL}/Makefile.sources"

LIB=	mesautil

CPPFLAGS+= \
	-I/usr/include/dev/pci/drm \
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
