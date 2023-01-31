#	$OpenBSD: Makefile,v 1.4 2023/01/31 03:29:36 jsg Exp $

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
