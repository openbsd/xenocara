# $OpenBSD: Makefile,v 1.1 2006/11/26 18:03:16 matthieu Exp $

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
		echo "${INSTALL} -c ${.CURDIR}/$$f ${DESTDIR}${fontdir}" ; \
		${INSTALL} -c ${.CURDIR}/$$f ${DESTDIR}${fontdir} ; \
	done
	@rm -f ${DESTDIR}${fontdir}/fonts.scale
	mkfontscale ${DESTDIR}${fontdir}
	@rm -f ${DESTDIR}${fontdir}/fonts.dir
	mkfontdir ${DESTDIR}${fontdir}
	fc-cache -v ${DESTDIR}${fontdir}

.include <bsd.xorg.mk>
