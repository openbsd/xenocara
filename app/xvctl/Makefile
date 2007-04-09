# $OpenBSD: Makefile,v 1.3 2007/04/09 15:48:56 matthieu Exp $

.include <bsd.own.mk>
X11BASE?=	/usr/X11R6

PROG=xvctl
MAN=xvctl.1

CPPFLAGS+= -I${X11BASE}/include
LDADD+=-L${X11BASE}/lib -lXv -lXext -lX11 -lXau -lXdmcp

MANDIR= ${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
