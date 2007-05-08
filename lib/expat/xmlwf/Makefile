# $OpenBSD: Makefile,v 1.2 2007/05/08 21:07:55 matthieu Exp $
.include <bsd.own.mk>

PROG=		xmlwf
SRCS=		xmlwf.c xmlfile.c codepage.c unixfilemap.c
CFLAGS+=	-I${.CURDIR}/../ -I${.CURDIR}/../lib/
CPPFLAGS+=	-DHAVE_EXPAT_CONFIG_H
LDFLAGS+=	-L${.CURDIR}/../obj
LDADD+=		-lexpat

X11BASE?=	/usr/X11R6
MANDIR=		${X11BASE}/man/cat
VPATH=		${.CURDIR}/../doc

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
