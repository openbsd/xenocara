# $OpenBSD: Makefile,v 1.3 2007/06/01 01:05:25 matthieu Exp $

FONT_FILES= \
	Vera.ttf \
	VeraBI.ttf \
	VeraBd.ttf \
	VeraIt.ttf \
	VeraMoBI.ttf \
	VeraMoBd.ttf \
	VeraMoIt.ttf \
	VeraMono.ttf \
	VeraSe.ttf \
	VeraSeBd.ttf \

fontdir=	${X11BASE}/lib/X11/fonts/TTF

depend:

all:

clean:

cleandir:

install:
	@for f in ${FONT_FILES}; do \
		echo "${INSTALL_DATA} -c ${.CURDIR}/$$f ${DESTDIR}${fontdir}" ; \
		${INSTALL_DATA} -c ${.CURDIR}/$$f ${DESTDIR}${fontdir} ; \
	done
	@rm -f ${DESTDIR}${fontdir}/fonts.scale
	mkfontscale ${DESTDIR}${fontdir}
	@rm -f ${DESTDIR}${fontdir}/fonts.dir
	mkfontdir ${DESTDIR}${fontdir}

.include <bsd.xorg.mk>
