# $OpenBSD: Makefile,v 1.2 2006/12/02 16:28:48 matthieu Exp $

.include <bsd.own.mk>
X11BASE?=	/usr/X11R6

PROG=xvctl
MAN=xvctl.1

CPPFLAGS+= -I${X11BASE}/include
LDADD+=-L${X11BASE}/lib -lXv -lXext -lX11

MANDIR= ${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
