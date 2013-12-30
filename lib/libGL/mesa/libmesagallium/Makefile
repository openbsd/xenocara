# $OpenBSD: Makefile,v 1.5 2013/12/30 18:32:48 matthieu Exp $

LIB=	mesagallium

CPPFLAGS+= \
	-I${MESA}/drivers/dri/common \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/include \
	-I${.CURDIR}/../generated \
	-I${.CURDIR}/../generated/main \
	-D__NOT_HAVE_DRM_H

SRCS=	${MESAGALLIUM_SOURCES}	\
	${MESAGALLIUM_CXX_SOURCES} \
	${ASM_SOURCES}

.include <bsd.xorg.mk>
