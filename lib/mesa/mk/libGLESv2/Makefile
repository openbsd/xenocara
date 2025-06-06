#	$OpenBSD: Makefile,v 1.6 2025/06/06 09:04:08 jsg Exp $

LIB=	GLESv2

SRCS=	entry.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DMAPI_MODE_BRIDGE \
		-DMAPI_ABI_HEADER=\"${MESA_SRC}/generated/src/mapi/es2api/glapi_mapi_tmp.h\"

LDADD+= -L${.CURDIR}/../libglapi/${__objdir} -lglapi \
	${.CURDIR}/../libmesa_util_c11/${__objdir}/libmesa_util_c11.a \
	-L${X11BASE}/lib -ldrm -lm -lpthread \
	-L${.CURDIR}/../libgallium_dri/${__objdir} -lgallium_dri

NOPROFILE=

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/mapi
