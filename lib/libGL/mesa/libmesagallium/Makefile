# $OpenBSD: Makefile,v 1.3 2012/12/04 14:06:32 mpi Exp $

LIB=	mesagallium

CPPFLAGS+= \
	-I${MESA}/drivers/dri/common \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/include \
	-D__NOT_HAVE_DRM_H

SRCS=	${MESAGALLIUM_SOURCES}	\
	${MESAGALLIUM_CXX_SOURCES} \
	${ASM_SOURCES} \
	_atexit.c

.include <bsd.xorg.mk>
