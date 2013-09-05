# $OpenBSD: Makefile,v 1.4 2013/09/05 15:12:09 jsg Exp $

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
	${ASM_SOURCES} \
	_atexit.c

.include <bsd.xorg.mk>
