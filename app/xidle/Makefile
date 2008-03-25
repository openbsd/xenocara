# $OpenBSD: Makefile,v 1.4 2008/03/25 23:41:50 matthieu Exp $
.include <bsd.xconf.mk>

PROG=	xidle

CPPFLAGS+= -I${X11BASE}/include
CFLAGS+= -Wall
LDADD+=	-L${X11BASE}/lib -lXss -lXext -lX11 -lXau -lXdmcp

MANDIR=	${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
