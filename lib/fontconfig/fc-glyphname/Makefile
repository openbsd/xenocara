#	$OpenBSD: Makefile,v 1.1 2013/03/09 14:34:44 matthieu Exp $

FONTCONFIG=	${.CURDIR}/../../../dist/fontconfig

PROG=	fc-glyphname

afterdepend: fcglyphname.h

fcglyphname.h: ${PROG}
	./${PROG} ${FONTCONFIG}/fc-glyphname/zapfdingbats.txt \
	< ${FONTCONFIG}/fc-glyphname/fcglyphname.tmpl.h > fcglyphname.h

obj:	_xenocara_obj

.PATH: ${FONTCONFIG}/fc-glyphname

CLEANFILES = fcglyphname.h

NOMAN=

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
