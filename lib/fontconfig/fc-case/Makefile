#	$OpenBSD: Makefile,v 1.1 2013/03/09 14:34:44 matthieu Exp $

FONTCONFIG=	${.CURDIR}/../../../dist/fontconfig

PROG=	fc-case

afterdepend: fccase.h

fccase.h: ${PROG}
	./${PROG} ${FONTCONFIG}/fc-case/CaseFolding.txt \
	< ${FONTCONFIG}/fc-case/fccase.tmpl.h > fccase.h

obj:	_xenocara_obj

.PATH: ${FONTCONFIG}/fc-case

CLEANFILES = fccase.h

NOMAN=

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
