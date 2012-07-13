# $OpenBSD: Makefile,v 1.1.1.1 2012/07/13 12:43:30 mpi Exp $

AUX=	${.CURDIR}/../../../../../dist/Mesa/src/gallium/auxiliary

PYTHON=		python${PYTHON_VERSION}
PYTHON_FLAGS=	-t -O -O

GENERATED=	u_indices_gen.c u_unfilled_gen.c u_format_srgb.c \
		u_format_table.c u_half.c

all: ${GENERATED}

obj:

depend:

install:

clean distclean:
	rm -f ${GENERATED}

u_indices_gen.c: u_indices_gen.py
	${PYTHON} ${PYTHON_FLAGS} ${AUX}/indices/u_indices_gen.py > $@

u_unfilled_gen.c: u_unfilled_gen.py
	${PYTHON} ${PYTHON_FLAGS} ${AUX}/indices/u_unfilled_gen.py > $@

u_format_srgb.c: ${AUX}/util/u_format_srgb.py
	${PYTHON} ${PYTHON_FLAGS} ${AUX}/util/u_format_srgb.py > $@

u_format_table.c: u_format_table.py u_format_pack.py u_format_parse.py u_format.csv
	${PYTHON} ${PYTHON_FLAGS} ${AUX}/util/u_format_table.py ${AUX}/util/u_format.csv > $@

u_half.c: ${AUX}/util/u_half.py
	${PYTHON} ${PYTHON_FLAGS} ${AUX}/util/u_half.py > $@

.SUFFIXES: .py

.PATH: ${AUX}/indices
.PATH: ${AUX}/util

.include <bsd.xorg.mk>
