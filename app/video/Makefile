# $OpenBSD: Makefile,v 1.2 2010/07/23 19:46:00 ckuethe Exp $
.include <bsd.xconf.mk>

PROG=	video

CPPFLAGS+= -I${X11BASE}/include
LDADD+= -L${X11BASE}/lib -lX11 -lXv
MANDIR=	${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
