#	$OpenBSD: Makefile,v 1.2 2017/06/18 21:09:07 espie Exp $

FONTCONFIG=	${.CURDIR}/../../../dist/fontconfig

PROG=	fc-glyphname

afterdepend: fcglyphname.h
all: fcglyphname.h

fcglyphname.h: ${PROG}
	./${PROG} ${FONTCONFIG}/fc-glyphname/zapfdingbats.txt \
	< ${FONTCONFIG}/fc-glyphname/fcglyphname.tmpl.h > fcglyphname.h

obj:	_xenocara_obj

.PATH: ${FONTCONFIG}/fc-glyphname

CLEANFILES = fcglyphname.h

NOMAN=

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
