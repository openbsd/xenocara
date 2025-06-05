#	$OpenBSD: Makefile,v 1.7 2025/06/05 14:18:42 jsg Exp $

LIB=	glapi

SRCS=	entry.c \
	u_current.c \
	glapi.c \
	stub.c \
	table.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mapi/shared-glapi \
		-I${MESA_SRC}/src/mapi \
		-DMAPI_MODE_GLAPI \
		-DMAPI_ABI_HEADER=\"${MESA_SRC}/generated/src/mapi/shared-glapi/glapi_mapi_tmp.h\"

LDADD+=	-Wl,--as-needed -Wl,--allow-shlib-undefined -Wl,--start-group \
	${.CURDIR}/../libmesa_util/${__objdir}/libmesa_util.a \
	${.CURDIR}/../libmesa_util_c11/${__objdir}/libmesa_util_c11.a \
	-Wl,--gc-sections -lpthread -lm -Wl,--end-group

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/mapi
.PATH: ${MESA_SRC}/src/mapi/shared-glapi
