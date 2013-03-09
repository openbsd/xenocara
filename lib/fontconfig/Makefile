#	$OpenBSD: Makefile,v 1.1 2013/03/09 14:34:40 matthieu Exp $

SUBDIR =	fc-case fc-lang fc-glyphname src \
		fc-cache fc-cat fc-list fc-match fc-pattern fc-query \
		fc-scan fc-validate conf.d pc doc

.ifmake	install
SUBDIR += fontconfig
.endif

obj:	_xenocara_obj

.include <bsd.subdir.mk>
.include <bsd.xorg.mk>
