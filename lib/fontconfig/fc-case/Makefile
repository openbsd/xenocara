#	$OpenBSD: Makefile,v 1.3 2021/11/11 08:46:46 matthieu Exp $

FONTCONFIG=	${.CURDIR}/../../../dist/fontconfig


all:

regen: fccase.h

fccase.h: CaseFolding.txt fccase.tmpl.h fc-case.py
	python${PYTHON_VERSION} ${FONTCONFIG}/fc-case/fc-case.py \
	${FONTCONFIG}/fc-case/CaseFolding.txt \
	--template ${FONTCONFIG}/fc-case/fccase.tmpl.h \
		--output $@

obj:

.PATH: ${FONTCONFIG}/fc-case


NOMAN=

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
