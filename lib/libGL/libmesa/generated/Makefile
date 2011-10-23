# $OpenBSD: Makefile,v 1.1 2011/10/23 13:57:36 matthieu Exp $

MESA=	${.CURDIR}/../../../../dist/Mesa/src/mesa

PYTHON=		python${PYTHON_VERSION}
PYTHON_FLAGS=	-t -O -O

GENERATED= api_exec_es1.c api_exec_es2.c

all: ${GENERATED}

obj:

depend:

install:

clean distclean:
	rm -f ${GENERATED}

api_exec_es1.c: APIspec.xml es_generator.py APIspecutil.py APIspec.py
	$(PYTHON) $(PYTHON_FLAGS) ${MESA}/main/es_generator.py -S ${MESA}/main/APIspec.xml -V GLES1.1 > $@

api_exec_es2.c: APIspec.xml es_generator.py APIspecutil.py APIspec.py
	$(PYTHON) $(PYTHON_FLAGS) ${MESA}/main/es_generator.py -S ${MESA}/main/APIspec.xml -V GLES2.0 > $@

.SUFFIXES: .py .xml

.PATH:	${MESA}/main

.include <bsd.xorg.mk>
