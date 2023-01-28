#	$OpenBSD: Makefile,v 1.3 2023/01/28 08:56:33 jsg Exp $

LIB=	GLESv2

SRCS=	entry.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DMAPI_MODE_BRIDGE \
		-DMAPI_ABI_HEADER=\"${MESA_SRC}/generated/src/mapi/es2api/glapi_mapi_tmp.h\"

LDADD+= -L${.CURDIR}/../libglapi/${__objdir} -lglapi \
	${.CURDIR}/../libmesa_util_c11/${__objdir}/libmesa_util_c11.a \
	-L${X11BASE}/lib -ldrm -lm

obj: _xenocara_obj

beforeinstall:
	rm -f ${DESTDIR}${LIBDIR}/lib${LIB}.la

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/mapi
