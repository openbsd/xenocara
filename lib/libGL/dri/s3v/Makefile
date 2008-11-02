#	$OpenBSD: Makefile,v 1.1 2008/11/02 15:01:32 matthieu Exp $

LIB=	s3v_dri.so

DRIVER_SOURCES = \
	s3v_context.c \
	s3v_dd.c \
	s3v_inithw.c \
	s3v_lock.c \
	s3v_render.c \
	s3v_screen.c \
	s3v_span.c \
	s3v_state.c \
	s3v_tex.c \
	s3v_texmem.c \
	s3v_texstate.c \
	s3v_tris.c \
	s3v_vb.c \
	s3v_xmesa.c

SRCS = \
	$(COMMON_SOURCES) \
	$(DRIVER_SOURCES) 

DRIVER_DEFINES = -I$(MESA)/drivers/dri/s3v/server

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/s3v
