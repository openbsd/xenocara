#	$OpenBSD: Makefile,v 1.1 2010/05/22 20:14:15 matthieu Exp $

TOP=	${.CURDIR}/../../../../dist/Mesa

LIBS = \
	../pp/libglslpp_pic.a \
	../cl/libglslcl_pic.a

SOURCES = \
	compile.c \
	process.c \
	purify.c \
	tokenise.c \
	version.c

APPS = $(SOURCES:%.c=%)

CPPFLAGS = -I${.CURDIR}

.SUFFIXES:
.SUFFIXES: .c

.c:
	${CC} ${LDFLAGS} -o ${.TARGET} ${.IMPSRC} ${LIBS}

all: ${APPS}

install:

clean:
	rm -f ${APPS}

cleandir: clean

.include <bsd.xorg.mk>

.PATH: ${TOP}/src/glsl/apps
