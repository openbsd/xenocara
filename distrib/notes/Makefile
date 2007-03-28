# $OpenBSD: Makefile,v 1.2 2007/03/28 00:22:04 todd Exp $

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

.include <bsd.xorg.mk>
