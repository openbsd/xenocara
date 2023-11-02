#	$OpenBSD: Makefile,v 1.5 2023/11/02 04:53:31 jsg Exp $

LIB=	glapi

SRCS=	entry.c \
	u_current.c \
	u_execmem.c \
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

beforeinstall:
	rm -f ${DESTDIR}${LIBDIR}/lib${LIB}.la

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/mapi
.PATH: ${MESA_SRC}/src/mapi/shared-glapi
