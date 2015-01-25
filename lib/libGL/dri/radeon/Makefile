#	$OpenBSD: Makefile,v 1.7 2015/01/25 15:09:12 jsg Exp $

MESA=${.CURDIR}/../../../../dist/Mesa/src/mesa
.include "${MESA}/drivers/dri/radeon/Makefile.sources"

LIB=	radeon_dri.so

SRCS = \
	$(COMMON_SOURCES) \
	$(RADEON_C_FILES)

DRIVER_DEFINES = -DRADEON_R100 -DRADEON_COMMON=0 \
	-DHAVE_LIBDRM_RADEON \
	-I$(MESA)/drivers/dri/radeon/server \
	-I$(MESA)/drivers/dri/radeon

.include <bsd.xorg.mk>

LDADD+=	-ldrm_radeon

.PATH: $(MESA)/drivers/dri/radeon
