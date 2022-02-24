#	$OpenBSD: Makefile,v 1.3 2022/02/24 02:29:49 jsg Exp $

LIB=	mesa_format

NOPROFILE=

SRCS=	u_format.c \
	u_format_bptc.c \
	u_format_etc.c \
	u_format_fxt1.c \
	u_format_latc.c \
	u_format_other.c \
	u_format_rgtc.c \
	u_format_s3tc.c \
	u_format_tests.c \
	u_format_unpack_neon.c \
	u_format_yuv.c \
	u_format_zs.c

SRCS+=	u_format_table.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/util/format \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/util/format

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/util/format
.PATH:	${MESA_SRC}/generated/src/util/format
