# $OpenBSD: Makefile,v 1.1 2007/12/04 22:00:27 matthieu Exp $

LIB=	r200_dri.so

DRIVER_SOURCES = r200_context.c \
		 r200_ioctl.c \
		 r200_lock.c \
		 r200_state.c \
		 r200_state_init.c \
		 r200_cmdbuf.c \
		 r200_pixel.c \
		 r200_tex.c \
		 r200_texmem.c \
		 r200_texstate.c \
		 r200_tcl.c \
		 r200_swtcl.c \
		 r200_span.c \
		 r200_maos.c \
		 r200_sanity.c \
		 r200_fragshader.c \
		 r200_vertprog.c \
		 radeon_screen.c \
		 $(EGL_SOURCES)

SRCS = $(COMMON_SOURCES) $(DRIVER_SOURCES)

DRIVER_DEFINES = -DRADEON_COMMON=1 -DRADEON_COMMON_FOR_R200 \
	-I$(MESA)/drivers/dri/r200 \
	-I$(MESA)/drivers/dri/r200/server \
	-I$(MESA)/drivers/dri/radeon \
	-I$(MESA)/drivers/dri/radeon/server

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/r200
.PATH: $(MESA)/drivers/dri/radeon



