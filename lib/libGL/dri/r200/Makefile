# $OpenBSD: Makefile,v 1.7 2015/01/25 15:09:12 jsg Exp $

MESA=${.CURDIR}/../../../../dist/Mesa/src/mesa
.include "${MESA}/drivers/dri/r200/Makefile.sources"

LIB=	r200_dri.so

SRCS = \
	$(COMMON_SOURCES) \
	$(R200_C_FILES)

DRIVER_DEFINES = -DRADEON_R200 \
	-DHAVE_LIBDRM_RADEON \
	-DRADEON_COMMON=1 -DRADEON_COMMON_FOR_R200 \
	-I$(MESA)/drivers/dri/r200 \
	-I$(MESA)/drivers/dri/r200/server \
	-I$(MESA)/drivers/dri/radeon \
	-I$(MESA)/drivers/dri/radeon/server

.include <bsd.xorg.mk>

LDADD+=	-ldrm_radeon

.PATH: $(MESA)/drivers/dri/r200
.PATH: $(MESA)/drivers/dri/radeon
