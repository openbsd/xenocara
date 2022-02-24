#	$OpenBSD: Makefile,v 1.2 2022/02/24 02:29:49 jsg Exp $

LIB=	loader

NOPROFILE=

SRCS=	loader_dri_helper.c \
	loader.c \
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
