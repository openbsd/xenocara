# $OpenBSD: Makefile,v 1.7 2014/07/12 12:32:45 matthieu Exp $

FILE= README.${MACHINE}

all: ${FILE}.generated

${FILE}.generated: ${FILE} m4.common
	m4 ${.CURDIR}/m4.common ${.CURDIR}/${FILE} > ${FILE}.generated

install: ${FILE}.generated
	${INSTALL_DATA} ${FILE}.generated ${DESTDIR}/usr/X11R6/README

clean:
	rm -f *.generated

distclean: clean

includes:
	@echo No includes here.

.include <bsd.xorg.mk>
