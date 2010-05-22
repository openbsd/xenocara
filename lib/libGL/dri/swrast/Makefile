#	$OpenBSD: Makefile,v 1.3 2010/05/22 20:14:15 matthieu Exp $

LIB=	swrast_dri.so

.if ${MACHINE_ARCH} == "sparc"
PICFLAG = -fPIC
.endif


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
