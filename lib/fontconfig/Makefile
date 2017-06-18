#	$OpenBSD: Makefile,v 1.5 2017/06/18 21:09:07 espie Exp $

FONTCONFIG =	${.CURDIR}/../../dist/fontconfig
FC_DEFAULT_FONTS = ${X11BASE}/lib/X11/fonts
FC_FONTPATH = "<dir>/usr/local/share/fonts</dir>"
CONFIGDIR = conf.d
FC_CACHEDIR = /var/cache/fontconfig

# Respect order:
SUBDIR =	fc-case 

# fc-lang depends on fc-case
SUBDIR += 	fc-lang fc-glyphname 

# src requires fc-case fc-lang and fc-glyphname
SUBDIR +=	src \
		fc-cache fc-cat fc-list fc-match fc-pattern fc-query \
		fc-scan fc-validate conf.d pc doc

.ifmake	install
SUBDIR += fontconfig
.endif

obj:	_xenocara_obj

all:	fonts.conf _SUBDIRUSE

install: _SUBDIRUSE
	${INSTALL_DATA} fonts.conf ${DESTDIR}/etc/fonts
	${INSTALL_DATA} ${FONTCONFIG}/fonts.dtd ${DESTDIR}/etc/fonts

fonts.conf:	${FONTCONFIG}/fonts.conf.in
	@sed -e s,@FC_DEFAULT_FONTS@,${FC_DEFAULT_FONTS}, \
		-e s,@FC_FONTPATH@,${FC_FONTPATH}, \
		-e s,@CONFIGDIR@,${CONFIGDIR}, \
		-e s,@FC_CACHEDIR@,${FC_CACHEDIR}, \
		< ${FONTCONFIG}/fonts.conf.in > $@

cleandir clean: _SUBDIRUSE
	rm -f fonts.conf 

.include <bsd.subdir.mk>
.include <bsd.xorg.mk>
