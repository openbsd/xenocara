#	$OpenBSD: Makefile,v 1.3 2025/06/05 14:18:40 jsg Exp $

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
.if ${MACHINE_ARCH} == "i386"
CPPFLAGS+=	-DBLAKE3_NO_SSE2 \
		-DBLAKE3_NO_SSE41 \
		-DBLAKE3_NO_AVX2 \
		-DBLAKE3_NO_AVX512
.endif

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/util/blake3
