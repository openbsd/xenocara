#	$OpenBSD: Makefile,v 1.18 2025/01/04 15:22:55 matthieu Exp $

FONTCONFIG =	${.CURDIR}/../../../dist/fontconfig
CONFIGDIR =	/etc/fonts/conf.d
CONFAVAILDIR=	/etc/fonts/conf.avail
CONFAVAILREL=	../conf.avail

DOC_FILES= \
	README

CONF_LINKS = \
	10-hinting-slight.conf \
	10-scale-bitmap-fonts.conf \
	11-lcdfilter-default.conf \
	20-unhint-small-vera.conf \
	30-lucida-aliases.conf \
	30-metric-aliases.conf \
	31-nonmst.conf \
	40-nonlatin.conf \
	45-generic.conf \
	45-latin.conf \
	48-spacing.conf \
	49-sansserif.conf \
	50-user.conf \
	51-local.conf \
	59-latin-obsd.conf \
	60-generic.conf \
	60-latin.conf \
	65-fonts-persian.conf \
	65-nonlatin.conf \
	69-unifont.conf \
	80-delicious.conf \
	90-synthetic.conf

AVAIL_FILES =				\
	10-autohint.conf		\
	10-hinting-full.conf		\
	10-hinting-medium.conf		\
	10-hinting-none.conf		\
	10-hinting-slight.conf		\
	10-no-antialias.conf		\
	10-scale-bitmap-fonts.conf	\
	10-sub-pixel-bgr.conf		\
	10-sub-pixel-none.conf		\
	10-sub-pixel-rgb.conf		\
	10-sub-pixel-vbgr.conf		\
	10-sub-pixel-vrgb.conf		\
	10-unhinted.conf		\
	10-yes-antialias.conf		\
	11-lcdfilter-default.conf	\
	11-lcdfilter-legacy.conf	\
	11-lcdfilter-light.conf		\
	11-lcdfilter-none.conf		\
	20-unhint-small-vera.conf	\
	25-unhint-nonlatin.conf		\
	30-lucida-aliases.conf		\
	30-metric-aliases.conf		\
	31-nonmst.conf			\
	40-nonlatin.conf		\
	45-generic.conf			\
	45-latin.conf			\
	48-spacing.conf			\
	49-sansserif.conf		\
	59-latin-obsd.conf		\
	50-user.conf			\
	51-local.conf			\
	60-generic.conf			\
	60-latin.conf			\
	65-fonts-persian.conf		\
	65-khmer.conf			\
	65-nonlatin.conf		\
	69-unifont.conf			\
	70-no-bitmaps.conf		\
	70-yes-bitmaps.conf		\
	80-delicious.conf		\
	90-synthetic.conf

all cleandir:

install:
	@for i in ${AVAIL_FILES}; do				\
	   echo ${INSTALL_DATA} $$i ${DESTDIR}${CONFAVAILDIR}; \
	   ${INSTALL_DATA} ${FONTCONFIG}/conf.d/$$i ${DESTDIR}${CONFAVAILDIR};	\
	done
	${INSTALL_DATA} ${FONTCONFIG}/conf.d/README ${DESTDIR}/${CONFIGDIR}
	@(echo cd ${DESTDIR}${CONFIGDIR};			\
	  cd ${DESTDIR}${CONFIGDIR};				\
	  for i in ${CONF_LINKS}; do				\
	    echo rm $$i";" ln -s ${CONFAVAILREL}/$$i . ";"	\
	      chown -h ${BINOWN}:${BINGRP} $$i;			\
	    rm -f $$i;						\
	    ln -s ${CONFAVAILREL}/$$i .;			\
	    chown -h ${BINOWN}:${BINGRP} $$i;			\
	  done)

NOOBJ=

.include <bsd.own.mk>
.include <bsd.xorg.mk>
