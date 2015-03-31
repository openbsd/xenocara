#	$OpenBSD: Makefile,v 1.3 2015/03/31 09:42:56 ajacoutot Exp $

FONTCONFIG =	${.CURDIR}/../../dist/fontconfig
FC_DEFAULT_FONTS = ${X11BASE}/lib/X11/fonts
FC_FONTPATH = "<dir>/usr/local/share/fonts</dir>"
CONFIGDIR = conf.d
FC_CACHEDIR = /var/cache/fontconfig

SUBDIR =	fc-case fc-lang fc-glyphname src \
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

clean: _SUBDIRUSE
	rm -f fonts.conf 

.include <bsd.subdir.mk>
.include <bsd.xorg.mk>
