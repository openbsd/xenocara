# $OpenBSD: Makefile,v 1.1 2015/04/15 07:58:58 jsg Exp $

EPOXY=	${.CURDIR}/../../../dist/libepoxy

PYTHON=		python${PYTHON_VERSION}
PYTHON_FLAGS=	-t -O -O

GENERATED= \
	src/gl_generated_dispatch.c \
	src/glx_generated_dispatch.c \
	src/egl_generated_dispatch.c

all: ${GENERATED}

obj:

depend:

install:

clean distclean:
	rm -f ${GENERATED}

src/gl_generated_dispatch.c: registry/gl.xml
	$(PYTHON) $(PYTHON_FLAGS) ${EPOXY}/src/gen_dispatch.py \
	    --dir ${.CURDIR} ${EPOXY}/registry/gl.xml

src/glx_generated_dispatch.c: registry/glx.xml
	$(PYTHON) $(PYTHON_FLAGS) ${EPOXY}/src/gen_dispatch.py \
	    --dir ${.CURDIR} ${EPOXY}/registry/glx.xml

src/egl_generated_dispatch.c: registry/egl.xml
	$(PYTHON) $(PYTHON_FLAGS) ${EPOXY}/src/gen_dispatch.py \
	    --dir ${.CURDIR} ${EPOXY}/registry/egl.xml

.SUFFIXES: .py .xml

.PATH:	${EPOXY}
.PATH:	${EPOXY}/src

.include <bsd.xorg.mk>
