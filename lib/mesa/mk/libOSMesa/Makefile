#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:36 jsg Exp $

LIB=	OSMesa

SRCS=	osmesa.c

LDADD+=	${.CURDIR}/../libmesa_classic/${__objdir}/libmesa_classic.a \
	-Wl,--whole-archive ${.CURDIR}/../libglapi_static/${__objdir}/libglapi_static.a \
	-Wl,--no-whole-archive
LDADD+= -L${.CURDIR}/../libglapi/${__objdir} -lglapi \
	-lm -lz

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/generated/src/mapi/glapi

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/mesa/drivers/osmesa
