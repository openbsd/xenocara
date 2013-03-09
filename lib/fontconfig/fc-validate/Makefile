#	$OpenBSD: Makefile,v 1.1 2013/03/09 14:34:45 matthieu Exp $

FONTCONFIG=	${.CURDIR}/../../../dist/fontconfig

PROG=	fc-validate
LDADD=	-L../src -L${X11BASE}/lib -lfontconfig -lfreetype -lexpat -lz -lpthread-stubs

obj:	_xenocara_obj

.PATH: ${FONTCONFIG}/fc-validate

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
