#	$OpenBSD: Makefile,v 1.2 2017/06/18 21:09:07 espie Exp $

FONTCONFIG=	${.CURDIR}/../../../dist/fontconfig

PROG=	fc-case

afterdepend: fccase.h
all: fccase.h

fccase.h: ${PROG}
	./${PROG} ${FONTCONFIG}/fc-case/CaseFolding.txt \
	< ${FONTCONFIG}/fc-case/fccase.tmpl.h > fccase.h

obj:	_xenocara_obj

.PATH: ${FONTCONFIG}/fc-case

CLEANFILES = fccase.h

NOMAN=

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
