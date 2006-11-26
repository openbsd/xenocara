# $OpenBSD: Makefile,v 1.1.1.1 2006/11/26 10:59:22 matthieu Exp $

.include <bsd.own.mk>
X11BASE?=	/usr/X11R6

PROG=xvctl
MAN=xvctl.1

CPPFLAGS+= -I${X11BASE}/include
LDADD+=-L${X11BASE}/lib -lXv -lXext -lX11

MANDIR= ${X11BASE}/man/cat

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
