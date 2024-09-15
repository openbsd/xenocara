#	$OpenBSD: Makefile,v 1.2 2024/09/15 10:56:40 jsg Exp $

LIB=	blake3

NOPROFILE=

SRCS=	blake3.c \
	blake3_dispatch.c \
	blake3_portable.c

.if ${MACHINE_ARCH} == "amd64"
SRCS+=	blake3_sse2_x86-64_unix.S \
	blake3_sse41_x86-64_unix.S \
	blake3_avx2_x86-64_unix.S \
	blake3_avx512_x86-64_unix.S
.elif ${MACHINE_ARCH} == "aarch64"
SRCS+=	blake3_neon.c
.endif

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/util/blake3
