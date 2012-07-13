# $OpenBSD: Makefile,v 1.1.1.1 2012/07/13 12:43:34 mpi Exp $

AUX=	${.CURDIR}/../../../../../../dist/Mesa/src/gallium/auxiliary
DEV=	${.CURDIR}/../../../../../../dist/Mesa/src/gallium/drivers

PYTHON=		python${PYTHON_VERSION}
PYTHON_FLAGS=	-t -O -O

GENERATED=	lp_tile_soa.c	

all: ${GENERATED}

obj:

depend:

install:

clean distclean:
	rm -f ${GENERATED}

lp_tile_soa.c: lp_tile_soa.py u_format_parse.py u_format_pack.py u_format.csv
	$(PYTHON) $(PYTHON_FLAGS) ${DEV}/llvmpipe/lp_tile_soa.py ${AUX}/util/u_format.csv > $@

.SUFFIXES: .py .csv

.PATH: ${DEV}/llvmpipe
.PATH: ${AUX}/util

.include <bsd.xorg.mk>
