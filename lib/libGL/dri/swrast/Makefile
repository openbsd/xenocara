#	$OpenBSD: Makefile,v 1.2 2008/11/05 07:18:25 todd Exp $

LIB=	swrast_dri.so

.if ${MACHINE} == "sparc"
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
