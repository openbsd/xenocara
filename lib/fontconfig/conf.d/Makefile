#	$OpenBSD: Makefile,v 1.2 2013/03/16 11:17:57 matthieu Exp $

FONTCONFIG =	${.CURDIR}/../../../dist/fontconfig
CONFIGDIR =	/etc/fonts/conf.d
CONFAVAILDIR=	/etc/fonts/conf.avail

DOC_FILES= \
	README

CONF_LINKS = \
	10-autohint.conf \
	10-scale-bitmap-fonts.conf \
	20-unhint-small-vera.conf \
	30-lucida-aliases.conf \
	30-urw-aliases.conf \
	30-metric-aliases.conf \
	31-nonmst.conf \
	40-nonlatin.conf \
	45-latin.conf \
	49-sansserif.conf \
	50-user.conf \
	51-local.conf \
	60-latin.conf \
	65-fonts-persian.conf \
	65-nonlatin.conf \
	69-unifont.conf \
	80-delicious.conf \
	90-synthetic.conf

AVAIL_FILES =				\
	10-autohint.conf		\
	10-no-sub-pixel.conf		\
	10-scale-bitmap-fonts.conf	\
	10-sub-pixel-bgr.conf		\
	10-sub-pixel-rgb.conf		\
	10-sub-pixel-vbgr.conf		\
	10-sub-pixel-vrgb.conf		\
	10-unhinted.conf		\
	11-lcdfilter-default.conf	\
	11-lcdfilter-legacy.conf	\
	11-lcdfilter-light.conf		\
	20-unhint-small-vera.conf	\
	25-unhint-nonlatin.conf		\
	30-lucida-aliases		\
	30-urw-aliases.conf		\
	30-metric-aliases.conf		\
	31-nonmst.conf			\
	40-nonlatin.conf		\
	45-latin.conf			\
	49-sansserif.conf		\
	50-user.conf			\
	51-local.conf			\
	60-latin.conf			\
	65-fonts-persian.conf		\
	65-khmer.conf			\
	65-nonlatin.conf		\
	69-unifont.conf			\
	70-no-bitmaps.conf		\
	70-yes-bitmaps.conf		\
	80-delicious.conf		\
	90-synthetic.conf

all:

install:
	@for i in $(AVAIL_FILES); do				\
	   echo ${INSTALL_DATA} $$i $(DESTDIR)${CONFAVAILDIR}; \
	   ${INSTALL_DATA} ${FONTCONFIG}/conf.d/$$i $(DESTDIR)${CONFAVAILDIR};	\
	done
	@(echo cd $(DESTDIR)$(CONFIGDIR);			\
	  cd $(DESTDIR)$(CONFIGDIR);				\
	  for i in $(CONF_LINKS); do				\
	    echo rm $$i";" ln -s $(CONFAVAILDIR)/$$i .;	\
	    rm -f $$i;						\
	    ln -s $(CONFAVAILDIR)/$$i .;			\
	  done)

NOOBJ=

.include <bsd.own.mk>
.include <bsd.xorg.mk>
