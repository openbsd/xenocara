# $OpenBSD: Makefile.inc,v 1.1.1.1 2012/07/13 12:43:34 mpi Exp $

GALLIUM=	${.CURDIR}/../../../../../dist/Mesa/src/gallium

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
