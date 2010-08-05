# $OpenBSD: Makefile,v 1.6 2010/08/05 06:34:19 matthieu Exp $

FILE= README.${MACHINE}

sets=xbase xfont xshare xserv

all: ${FILE}.generated

${FILE}.generated: ${FILE} m4.common
	m4 ${.CURDIR}/m4.common ${.CURDIR}/${FILE} > ${FILE}.generated

install: ${FILE}.generated
	${INSTALL_DATA} ${FILE}.generated ${DESTDIR}/usr/X11R6/README
.for set in ${sets}
	if [ -d ${DESTDIR}/var/db/pkg/${set}-${OSREV} ]; then \
		${INSTALL_DATA} DESCR.${set} \
			${DESTDIR}/var/db/pkg/${set}-${OSREV}/+DESCR; \
		${INSTALL_DATA} DEINSTALL \
			${DESTDIR}/var/db/pkg/${set}-${OSREV}/+DEINSTALL; \
	fi
.endfor

clean:
	rm -f *.generated

distclean: clean

includes:
	@echo No includes here.

.include <bsd.xorg.mk>
