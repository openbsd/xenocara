#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:40 jsg Exp $

LIB=	loader

NOPROFILE=

SRCS=	loader.c \
	pci_id_driver_map.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DUSE_DRICONF \
		-DDEFAULT_DRIVER_DIR=\"${X11BASE}/lib/modules/dri\" \
		-DPREFER_IRIS \
		-I${MESA_SRC}/src/loader \
		-I${MESA_SRC}/generated/src/util

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/loader
