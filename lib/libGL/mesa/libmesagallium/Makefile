# $OpenBSD: Makefile,v 1.1 2012/08/26 18:46:42 mpi Exp $

LIB=	mesagallium

CPPFLAGS+= \
	-I${MESA}/drivers/dri/common \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/include \
	-I${GALLIUM}/state_trackers/dri/common \
	-DHAVE_POSIX_MEMALIGN \
	-D__NOT_HAVE_DRM_H

SRCS=	${MESAGALLIUM_SOURCES}	\
	${MESAGALLIUM_CXX_SOURCES} \
	${ASM_SOURCES} \
	_atexit.c

.include <bsd.xorg.mk>
