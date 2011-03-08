# $OpenBSD: Makefile,v 1.5 2011/03/08 20:48:59 matthieu Exp $
.include <bsd.xconf.mk>

PROG=	video

CPPFLAGS+= -I${X11BASE}/include

LDADD+= -L${X11BASE}/lib -lXv -lXext -lX11 -lxcb -lXau -lXdmcp
MANDIR=	${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
