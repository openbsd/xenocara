#	$OpenBSD: Makefile,v 1.1 2007/12/04 22:00:27 matthieu Exp $

LIB=	radeon_dri.so

DRIVER_SOURCES = \
	radeon_context.c \
	radeon_ioctl.c \
	radeon_lock.c \
	radeon_screen.c \
	radeon_state.c \
	radeon_state_init.c \
	radeon_tex.c \
	radeon_texmem.c \
	radeon_texstate.c \
	radeon_tcl.c \
	radeon_swtcl.c \
	radeon_span.c \
	radeon_maos.c \
	radeon_sanity.c 

SRCS = \
	$(COMMON_SOURCES) \
	$(DRIVER_SOURCES) 

DRIVER_DEFINES = -DRADEON_COMMON=0 \
	-I$(MESA)/drivers/dri/radeon/server


.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/radeon
