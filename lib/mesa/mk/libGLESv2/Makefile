#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:36 jsg Exp $

LIB=	GLESv2

SRCS=	entry.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DMAPI_MODE_BRIDGE \
		-DMAPI_ABI_HEADER=\"${MESA_SRC}/generated/src/mapi/es2api/glapi_mapi_tmp.h\"

LDADD+= -L${.CURDIR}/../libglapi/${__objdir} -lglapi \
	-L${X11BASE}/lib -ldrm -lm

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/mapi
