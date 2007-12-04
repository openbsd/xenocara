#	$OpenBSD: Makefile,v 1.1 2007/12/04 22:00:27 matthieu Exp $

LIB=	mach64_dri.so

DRIVER_SOURCES = \
	mach64_context.c \
	mach64_ioctl.c \
	mach64_screen.c \
	mach64_span.c \
	mach64_state.c \
	mach64_tex.c \
	mach64_texmem.c \
	mach64_texstate.c \
	mach64_tris.c \
	mach64_vb.c \
	mach64_dd.c \
	mach64_lock.c

SRCS = \
	$(COMMON_SOURCES) \
	$(DRIVER_SOURCES)

DRIVER_DEFINES= -I$(MESA)/drivers/dri/mach64/server

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/mach64
