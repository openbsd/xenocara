# $OpenBSD: Makefile,v 1.4 2016/11/19 08:43:19 tb Exp $

FONT_FILES= \
	DejaVuMathTeXGyre.ttf \
	DejaVuSans-Bold.ttf \
	DejaVuSans-BoldOblique.ttf \
	DejaVuSans-ExtraLight.ttf \
	DejaVuSans-Oblique.ttf \
	DejaVuSans.ttf \
	DejaVuSansCondensed-Bold.ttf \
	DejaVuSansCondensed-BoldOblique.ttf \
	DejaVuSansCondensed-Oblique.ttf \
	DejaVuSansCondensed.ttf \
	DejaVuSansMono-Bold.ttf \
	DejaVuSansMono-BoldOblique.ttf \
	DejaVuSansMono-Oblique.ttf \
	DejaVuSansMono.ttf \
	DejaVuSerif-Bold.ttf \
	DejaVuSerif-BoldItalic.ttf \
	DejaVuSerif-Italic.ttf \
	DejaVuSerif.ttf \
	DejaVuSerifCondensed-Bold.ttf \
	DejaVuSerifCondensed-BoldItalic.ttf \
	DejaVuSerifCondensed-Italic.ttf \
	DejaVuSerifCondensed.ttf

CONFIG_FILES= \
	20-unhint-small-dejavu-sans-mono.conf \
	20-unhint-small-dejavu-sans.conf \
	20-unhint-small-dejavu-serif.conf

BINGRP=	wheel

fontdir=	${X11BASE}/lib/X11/fonts/TTF
fontconfigdir=	/etc/fonts

depend:

all:

clean:

cleandir:

install:
	@for f in ${FONT_FILES}; do \
		echo "${INSTALL_DATA} -c ${.CURDIR}/ttf/$$f ${DESTDIR}${fontdir}" ; \
		${INSTALL_DATA} -c ${.CURDIR}/ttf/$$f ${DESTDIR}${fontdir} ; \
	done
	@for f in ${CONFIG_FILES}; do \
		echo ${INSTALL_DATA} -c ${.CURDIR}/fontconfig/$$f \
			${DESTDIR}${fontconfigdir}/conf.avail ; \
		${INSTALL_DATA} -c ${.CURDIR}/fontconfig/$$f \
			${DESTDIR}${fontconfigdir}/conf.avail ; \
		( cd ${DESTDIR}${fontconfigdir}/conf.d ;  \
		  rm -f $$f ; \
		  ln -s ../conf.avail/$$f ; \
		  chown -h ${BINOWN}:bin $$f ) ; \
	done
	@rm -f ${DESTDIR}${fontdir}/fonts.scale
	mkfontscale ${DESTDIR}${fontdir}
	chown root:wheel ${DESTDIR}${fontdir}/fonts.scale
	chmod 644 ${DESTDIR}${fontdir}/fonts.scale
	@rm -f ${DESTDIR}${fontdir}/fonts.dir
	mkfontdir ${DESTDIR}${fontdir}
	chown root:wheel ${DESTDIR}${fontdir}/fonts.dir
	chmod 644 ${DESTDIR}${fontdir}/fonts.dir

.include <bsd.xorg.mk>
