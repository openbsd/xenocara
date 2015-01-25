# $OpenBSD: Makefile.inc,v 1.4 2015/01/25 15:09:13 jsg Exp $

GALLIUM=	${.CURDIR}/../../../../../dist/Mesa/src/gallium
MESA=		${.CURDIR}/../../../../../dist/Mesa/src/mesa

CPPFLAGS+= \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/include \
	-I${.CURDIR}/../../../../../dist/Mesa/include \
	-I${.CURDIR}/../../../../../dist/Mesa/src \
	-I${X11BASE}/include

all: lib${LIB}_pic.a

obj: _xenocara_obj

install:
	@echo "Not installing lib${LIB}"

clean: 
	rm -f lib${LIB}_pic.a ${OBJS}

cleandir:	clean

.PATH: ${GALLIUM}/drivers/${LIB}

.include "../Makefile.inc"
