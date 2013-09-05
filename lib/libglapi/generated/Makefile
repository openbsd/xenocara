# $OpenBSD: Makefile,v 1.1 2013/09/05 15:11:56 jsg Exp $

MESA=		${.CURDIR}/../../../dist/Mesa/src/mesa
MAPI=		${.CURDIR}/../../../dist/Mesa/src/mapi
GLAPI=		${.CURDIR}/../../../dist/Mesa/src/mapi/glapi

PYTHON=		python${PYTHON_VERSION}
PYTHON_FLAGS=	-t -O -O

# src/mapi/glapi
GEN_GLAPI= \
	glapi/glapi_mapi_tmp.h

GENERATED=${GEN_GLAPI}


all: ${GENERATED}

obj:

depend:

install:

clean distclean:
	rm -f ${GENERATED}

glapi/glapi_mapi_tmp.h: gl_and_es_API.xml mapi_abi.py
	$(PYTHON) $(PYTHON_FLAGS) ${MAPI}/mapi_abi.py \
	    --mode lib --printer shared-glapi ${GLAPI}/gen/gl_and_es_API.xml > $@

.SUFFIXES: .py .xml

.PATH:	${GLAPI}/gen
.PATH:	${MAPI}

.include <bsd.xorg.mk>
