#	$OpenBSD: Makefile,v 1.5 2023/11/02 04:53:31 jsg Exp $

.include <bsd.xconf.mk>

LIB=	dri

NOPROFILE=

SRCS=	dri_context.c \
	dri_drawable.c \
	dri_helpers.c \
	dri_query_renderer.c \
	dri_screen.c \
	dri_util.c

.if ${XENOCARA_BUILD_DRI:L} == "yes"
SRCS+=	drisw.c \
	dri2.c
.endif

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DGALLIUM_SOFTPIPE \
		-I${MESA_SRC}/src/gallium/frontends/dri \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mesa/drivers/dri/common \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/generated/src

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/frontends/dri
