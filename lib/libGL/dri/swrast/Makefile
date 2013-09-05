#	$OpenBSD: Makefile,v 1.5 2013/09/05 15:11:58 jsg Exp $

LIB=	swrast_dri.so

.if ${MACHINE_ARCH} == "sparc"
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
