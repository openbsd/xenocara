#	$OpenBSD: Makefile,v 1.6 2014/01/11 12:39:14 miod Exp $

LIB=	swrast_dri.so

.if ${MACHINE_ARCH} == "m68k" || ${MACHINE_ARCH} == "sparc"
PICFLAG = -fPIC
.endif


SWRAST_COMMON_SOURCES = \
        driverfuncs.c \
        utils.c \
	drisw_util.c

DRIVER_SOURCES = \
	swrast.c

SRCS = \
	$(SWRAST_COMMON_SOURCES) \
	$(DRIVER_SOURCES)

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/swrast
