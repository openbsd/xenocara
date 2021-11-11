#	$OpenBSD: Makefile,v 1.1 2021/11/11 08:46:46 matthieu Exp $

FONTCONFIG=	${.CURDIR}/../../../dist/fontconfig

PROG=	fc-conflist
LDADD=	-L../src -L${X11BASE}/lib -lfontconfig -lfreetype -lexpat -lz

obj:	_xenocara_obj

.PATH: ${FONTCONFIG}/fc-conflist

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
