#	$OpenBSD: Makefile,v 1.6 2021/11/11 08:46:45 matthieu Exp $

FONTCONFIG =	${.CURDIR}/../../dist/fontconfig

# the TAB below is required
FC_DEFAULT_FONTS = "	<dir>${X11BASE}/lib/X11/fonts</dir>"
FC_FONTPATH = "<dir>/usr/local/share/fonts</dir>"
CONFIGDIR = conf.d
FC_CACHEDIR = /var/cache/fontconfig
TEMPLATEDIR = conf.avail

# Respect order:
SUBDIR =	fc-case 

# fc-lang depends on fc-case
SUBDIR += 	fc-lang 

# src requires fc-case fc-lang and fc-glyphname
SUBDIR +=	src \
		fc-cache fc-cat fc-conflist fc-list fc-match \
		fc-pattern fc-query fc-scan fc-validate conf.d pc doc

.ifmake	install
SUBDIR += fontconfig
.endif

obj:	_xenocara_obj

all:	fonts.conf _SUBDIRUSE

install: _SUBDIRUSE
	${INSTALL_DATA} fonts.conf ${DESTDIR}/etc/fonts
	${INSTALL_DATA} ${FONTCONFIG}/fonts.dtd ${DESTDIR}/etc/fonts

fonts.conf:	${FONTCONFIG}/fonts.conf.in Makefile
	@sed -e s,@FC_DEFAULT_FONTS@,${FC_DEFAULT_FONTS}, \
		-e s,@FC_FONTPATH@,${FC_FONTPATH}, \
		-e s,@CONFIGDIR@,${CONFIGDIR}, \
		-e s,@FC_CACHEDIR@,${FC_CACHEDIR}, \
		< ${FONTCONFIG}/fonts.conf.in > $@

cleandir clean: _SUBDIRUSE
	rm -f fonts.conf 

.include <bsd.subdir.mk>
.include <bsd.xorg.mk>
