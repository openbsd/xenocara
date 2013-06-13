# $OpenBSD: Makefile.inc,v 1.2 2013/06/13 10:39:28 jsg Exp $

GALLIUM=	${.CURDIR}/../../../../../dist/Mesa/src/gallium
MESA=		${.CURDIR}/../../../../../dist/Mesa/src/mesa

CPPFLAGS+= \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/include 

all: lib${LIB}_pic.a

obj: _xenocara_obj

install:
	@echo "Not installing lib${LIB}"

clean: 
	rm -f lib${LIB}_pic.a ${OBJS}

cleandir:	clean

.PATH: ${GALLIUM}/drivers/${LIB}

.include "../Makefile.inc"
