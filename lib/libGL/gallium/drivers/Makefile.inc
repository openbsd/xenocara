# $OpenBSD: Makefile.inc,v 1.3 2013/09/05 15:11:59 jsg Exp $

GALLIUM=	${.CURDIR}/../../../../../dist/Mesa/src/gallium
MESA=		${.CURDIR}/../../../../../dist/Mesa/src/mesa

CPPFLAGS+= \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/include \
	-I${.CURDIR}/../../../../../dist/Mesa/include \
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
