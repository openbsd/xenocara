#	$OpenBSD: Makefile,v 1.1 2013/03/09 14:34:44 matthieu Exp $

FONTCONFIG=	${.CURDIR}/../../../dist/fontconfig

PROG=	fc-cat
LDADD=	-L../src -L${X11BASE}/lib -lfontconfig -lfreetype -lexpat -lz -lpthread-stubs

obj:	_xenocara_obj

.PATH: ${FONTCONFIG}/fc-cat

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
