#	$OpenBSD: Makefile,v 1.1 2007/12/04 22:00:27 matthieu Exp $

LIB=	r128_dri.so

DRIVER_SOURCES = \
	r128_context.c \
	r128_lock.c \
	r128_state.c \
	r128_texstate.c \
	r128_dd.c \
	r128_screen.c \
	r128_tex.c \
	r128_tris.c \
	r128_ioctl.c \
	r128_span.c \
	r128_texmem.c

SRCS = \
	$(COMMON_SOURCES) \
	$(DRIVER_SOURCES) 

DRIVER_DEFINES= -I$(MESA)/drivers/dri/r128/server

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/r128
