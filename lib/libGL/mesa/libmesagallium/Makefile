# $OpenBSD: Makefile,v 1.2 2012/11/28 13:53:16 mpi Exp $

LIB=	mesagallium

CPPFLAGS+= \
	-I${MESA}/drivers/dri/common \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/include \
	-DHAVE_POSIX_MEMALIGN \
	-D__NOT_HAVE_DRM_H

SRCS=	${MESAGALLIUM_SOURCES}	\
	${MESAGALLIUM_CXX_SOURCES} \
	${ASM_SOURCES} \
	_atexit.c

.include <bsd.xorg.mk>
