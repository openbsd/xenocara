# $OpenBSD: Makefile,v 1.3 2010/09/25 02:58:00 jakemsr Exp $
.include <bsd.xconf.mk>

PROG=	video

CPPFLAGS+= -I${X11BASE}/include
LDADD+= -L${X11BASE}/lib -lXv -lXext -lX11 -lXau -lXdmcp
MANDIR=	${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
