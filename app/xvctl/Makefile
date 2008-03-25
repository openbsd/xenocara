# $OpenBSD: Makefile,v 1.4 2008/03/25 23:41:50 matthieu Exp $
.include <bsd.xconf.mk>

PROG=xvctl
MAN=xvctl.1

CPPFLAGS+= -I${X11BASE}/include
LDADD+=-L${X11BASE}/lib -lXv -lXext -lX11 -lXau -lXdmcp

MANDIR= ${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
