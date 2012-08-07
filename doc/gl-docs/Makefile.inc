# $OpenBSD: Makefile.inc,v 1.1 2012/08/07 21:10:35 matthieu Exp $

all:

cleandir:

.SUFFIXES: .3 .3gl

.for page source in $(MAN)
_INST_MAN = ${DESTDIR}${MANDIR}$(page:E)/$(page:T)
${_INST_MAN}: ${source}
	${INSTALL} ${INSTALL_COPY} -o ${MANOWN} -g ${MANGRP} -m ${MANMODE} \
	  ${.ALLSRC} ${.TARGET}

install: ${_INST_MAN}

.PHONY: ${_INST_MAN}
.endfor

install:
.for lnk file in $(LINKS)
	@l=${DESTDIR}${MANDIR}${lnk:E}/${lnk}; \
	t=${DESTDIR}${MANDIR}${file:E}/${file}; \
	echo $$t -\> $$l; \
	rm -f $$t; ln $$l $$t;
.endfor

NOOBJ=
