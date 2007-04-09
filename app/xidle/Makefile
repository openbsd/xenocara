# $OpenBSD: Makefile,v 1.3 2007/04/09 11:14:25 matthieu Exp $

.include <bsd.own.mk>
X11BASE?=	/usr/X11R6

PROG=	xidle

CPPFLAGS+= -I${X11BASE}/include
CFLAGS+= -Wall
LDADD+=	-L${X11BASE}/lib -lXss -lXext -lX11 -lXau -lXdmcp

MANDIR=	${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
