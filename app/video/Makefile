# $OpenBSD: Makefile,v 1.6 2011/06/23 22:48:59 naddy Exp $
.include <bsd.xconf.mk>

PROG=	video

CPPFLAGS+= -I${X11BASE}/include

LDADD+= -L${X11BASE}/lib -lXv -lXext -lX11 -lxcb -lXau -lXdmcp
MANDIR=	${X11BASE}/man/man

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
