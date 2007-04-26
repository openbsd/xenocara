# $OpenBSD: Makefile,v 1.1 2007/04/26 11:55:28 bernd Exp $
.include <bsd.own.mk>

PROG=		xmlwf
SRCS=		xmlwf.c xmlfile.c codepage.c unixfilemap.c
CFLAGS+=	-I${.CURDIR}/../ -I${.CURDIR}/../lib/
CPPFLAGS+=	-DHAVE_EXPAT_CONFIG_H
LDFLAGS+=	-L${.CURDIR}/../obj -lexpat

X11BASE?=	/usr/X11R6
MANDIR=		${X11BASE}/man/cat
VPATH=		${.CURDIR}/../doc

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
