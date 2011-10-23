#	$OpenBSD: Makefile,v 1.4 2011/10/23 13:57:36 matthieu Exp $

LIB=	swrast_dri.so

.if ${MACHINE_ARCH} == "sparc"
PICFLAG = -fPIC
.endif


SWRAST_COMMON_SOURCES = \
        driverfuncs.c \
        utils.c \
	drisw_util.c

DRIVER_SOURCES = \
	swrast.c \
	swrast_span.c

SRCS = \
	$(SWRAST_COMMON_SOURCES) \
	$(DRIVER_SOURCES)

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/swrast
