#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:40 jsg Exp $

.include <bsd.xconf.mk>

LIB=	loader_dri3_helper

NOPROFILE=

.if ${XENOCARA_BUILD_DRI3:L} == "yes"
SRCS=	loader_dri3_helper.c
.else
SRCS=
.endif

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/loader

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/loader
