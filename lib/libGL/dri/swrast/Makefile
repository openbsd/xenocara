#	$OpenBSD: Makefile,v 1.9 2015/02/20 23:31:05 jsg Exp $

LIB=	swrast_dri.so

.if ${MACHINE_ARCH} == "sparc"
PICFLAG = -fPIC
.endif

SWRAST_COMMON_SOURCES = \
        driverfuncs.c \
	meta_blit.c \
	meta_generate_mipmap.c \
	meta.c \
	dri_util.c \
        utils.c \
	xmlconfig.c \
	megadriver_stub.c

DRIVER_SOURCES = \
	swrast.c

SRCS = \
	$(SWRAST_COMMON_SOURCES) \
	$(DRIVER_SOURCES)

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/swrast
