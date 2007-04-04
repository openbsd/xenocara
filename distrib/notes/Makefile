# $OpenBSD: Makefile,v 1.4 2007/04/04 15:24:27 aanriot Exp $

FILE= README.${MACHINE}

sets=xbase xfont xshare xserv

all: ${FILE}.generated

${FILE}.generated: ${FILE} m4.common
	m4 ${.CURDIR}/m4.common ${.CURDIR}/${FILE} > ${FILE}.generated

install: ${FILE}.generated
	${INSTALL} ${INSTALL_COPY} -o root -g wheel -m 444 ${FILE}.generated \
		${DESTDIR}/usr/X11R6/README
.for set in ${sets}
	if [ -d ${DESTDIR}/var/db/pkg/${set}-${OSREV} ]; then \
		${INSTALL} ${INSTALL_COPY} -o root -g wheel -m 444 \
			DESCR.${set} \
			${DESTDIR}/var/db/pkg/${set}-${OSREV}/+DESCR; \
		${INSTALL} ${INSTALL_COPY} -o root -g wheel -m 755 \
			DEINSTALL \
			${DESTDIR}/var/db/pkg/${set}-${OSREV}/+DEINSTALL; \
	fi
.endfor

clean:
	rm -f *.generated

distclean: clean

includes:
	@echo No includes here.

.include <bsd.xorg.mk>
