# $OpenBSD: Makefile,v 1.1.1.1 2006/11/27 11:25:45 matthieu Exp $

FILE= README.${MACHINE}

sets=xbase xfont xshare xserv

all: ${FILE}.generated

${FILE}.generated: ${FILE} m4.common
	m4 m4.common ${FILE} > ${FILE}.generated

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
