#	$OpenBSD: Makefile,v 1.10 2015/01/25 15:09:12 jsg Exp $

MESA=${.CURDIR}/../../../../dist/Mesa/src/mesa
.include "${MESA}/drivers/dri/i965/Makefile.sources"

LIB=	i965_dri.so

SRCS = \
	$(COMMON_SOURCES) \
	$(MINIGLX_SOURCES) \
	$(i965_FILES)

DRIVER_DEFINES = \
	-I$(MESA)/drivers/dri/i965 \
	-DFEATURE_ES1=1 -DFEATURE_ES2=1

.include <bsd.xorg.mk>

LDADD+=	-ldrm_intel

.PATH: $(MESA)/drivers/dri/i965
