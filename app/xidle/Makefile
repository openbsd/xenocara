# $OpenBSD: Makefile,v 1.1.1.1 2006/11/26 10:57:44 matthieu Exp $

.include <bsd.own.mk>
X11BASE?=	/usr/X11R6

PROG=	xidle

CPPFLAGS+= -I${X11BASE}/include
CFLAGS+= -Wall
LDADD+=	-L${X11BASE}/lib -lXss -lXext -lX11

MANDIR=	${X11BASE}/man/cat

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
