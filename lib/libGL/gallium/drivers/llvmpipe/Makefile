# $OpenBSD: Makefile,v 1.3 2015/01/25 15:09:13 jsg Exp $

GALLIUM=${.CURDIR}/../../../../../dist/Mesa/src/gallium
.include "${GALLIUM}/drivers/llvmpipe/Makefile.sources"

LIB = llvmpipe

SRCS = \
	${C_SOURCES}

CPPFLAGS+= -I${GALLIUM}/drivers/llvmpipe

lp_test_sincos.o: sse_mathfun.h

.include <bsd.xorg.mk>
