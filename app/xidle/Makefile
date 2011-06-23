# $OpenBSD: Makefile,v 1.7 2011/06/23 22:48:59 naddy Exp $
.include <bsd.xconf.mk>

PROG=	xidle

CPPFLAGS+= -I${X11BASE}/include
CFLAGS+= -Wall
LDADD+=	-L${X11BASE}/lib -lXss -lXext -lX11 -lxcb -lXau -lXdmcp

MANDIR=	${X11BASE}/man/man

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
