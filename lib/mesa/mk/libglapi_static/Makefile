#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:37 jsg Exp $

LIB=	glapi_static

NOPROFILE=

SRCS=	entry.c

.include "../Makefile.inc"

CPPFLAGS+=	-DMAPI_MODE_BRIDGE \
		-DMAPI_ABI_HEADER=\"${MESA_SRC}/generated/src/mapi/glapi/gen/glapi_mapi_tmp.h\"

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/mapi
