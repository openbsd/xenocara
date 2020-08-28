#	$OpenBSD: Makefile,v 1.2 2020/08/28 16:07:18 jsg Exp $

LIB=	GLESv1_CM

SRCS=	entry.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DMAPI_MODE_BRIDGE \
		-DMAPI_ABI_HEADER=\"${MESA_SRC}/generated/src/mapi/es1api/glapi_mapi_tmp.h\"

LDADD+= -L${.CURDIR}/../libglapi/${__objdir} -lglapi \
	-L${X11BASE}/lib -ldrm -lm

obj: _xenocara_obj

beforeinstall:
	rm -f ${DESTDIR}${LIBDIR}/lib${LIB}.la

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/mapi
