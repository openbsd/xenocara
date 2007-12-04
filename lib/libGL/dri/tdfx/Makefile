#	$OpenBSD: Makefile,v 1.1 2007/12/04 22:00:28 matthieu Exp $

LIB=	tdfx_dri.so

DRIVER_SOURCES = \
	tdfx_context.c \
	tdfx_dd.c \
	tdfx_lock.c \
	tdfx_pixels.c \
	tdfx_render.c \
	tdfx_screen.c \
	tdfx_span.c \
	tdfx_state.c \
	tdfx_tex.c \
	tdfx_texman.c \
	tdfx_texstate.c \
	tdfx_tris.c \
	tdfx_vb.c

SRCS = \
	$(COMMON_SOURCES) \
	$(DRIVER_SOURCES)

DRIVER_DEFINES = -I$(MESA)/drivers/dri/tdfx/server

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/tdfx
