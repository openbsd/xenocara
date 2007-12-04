#	$OpenBSD: Makefile,v 1.1 2007/12/04 22:00:28 matthieu Exp $

LIB=	sis_dri.so

DRIVER_SOURCES = \
	sis6326_state.c \
	sis6326_clear.c \
	sis_alloc.c \
	sis_clear.c \
	sis_context.c \
	sis_dd.c \
	sis_fog.c \
	sis_lock.c \
	sis_screen.c \
	sis_span.c \
	sis_state.c \
	sis_stencil.c \
	sis_tex.c \
	sis_texstate.c \
	sis_tris.c

SRCS = \
	$(COMMON_SOURCES) \
	$(DRIVER_SOURCES)

DRIVER_DEFINES = -I$(MESA)/drivers/dri/sis/server

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/sis
