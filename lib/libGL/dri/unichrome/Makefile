#	$OpenBSD: Makefile,v 1.1 2007/12/04 22:00:28 matthieu Exp $

LIB=	unichrome_dri.so

DRIVER_SOURCES = \
	via_context.c \
	via_fb.c \
	via_tex.c \
	via_ioctl.c \
	via_memcpy.c \
	via_render.c \
	via_screen.c \
	via_span.c \
	via_state.c \
	via_texcombine.c \
	via_tris.c 

SRCS = \
	$(COMMON_SOURCES) \
	$(DRIVER_SOURCES)

DRIVER_DEFINES = -I$(MESA)/drivers/dri/unichrome/server \
		-I$(MESA)/drivers/dri/unichrome

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/unichrome

