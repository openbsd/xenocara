#	$OpenBSD: Makefile,v 1.2 2009/05/17 20:20:23 matthieu Exp $

LIB=	trident_dri.so

DRIVER_SOURCES = \
	trident_context.c \
	trident_state.c \
	trident_vb.c \
	trident_tris.c

SRCS = \
	$(COMMON_SOURCES) \
	$(DRIVER_SOURCES)

DRIVER_DEFINES = -I$(MESA)/drivers/dri/trident/server

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/trident
