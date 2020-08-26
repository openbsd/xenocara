#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:37 jsg Exp $

LIB=	glapi

SRCS=	entry.c \
	mapi_glapi.c \
	stub.c \
	table.c

SRCS+=	u_current.c \
	u_execmem.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DMAPI_MODE_GLAPI \
		-DMAPI_ABI_HEADER=\"${MESA_SRC}/generated/src/mapi/shared-glapi/glapi_mapi_tmp.h\"

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/mapi
