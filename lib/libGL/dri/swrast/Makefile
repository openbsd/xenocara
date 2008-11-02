#	$OpenBSD: Makefile,v 1.1 2008/11/02 15:01:32 matthieu Exp $

LIB=	swrast_dri.so

SWRAST_COMMON_SOURCES = \
        driverfuncs.c \
        utils.c

DRIVER_SOURCES = \
	swrast.c \
	swrast_span.c

SRCS = \
	$(SWRAST_COMMON_SOURCES) \
	$(DRIVER_SOURCES)

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/swrast
