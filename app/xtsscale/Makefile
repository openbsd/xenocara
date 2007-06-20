# $OpenBSD: Makefile,v 1.2 2007/06/20 21:46:22 matthieu Exp $

PROG=	xtsscale
MAN=	xtsscale.1

CPPFLAGS+=	-I${X11BASE}/include
LDADD+=		-L${X11BASE}/lib -lX11 -lXau -lXdmcp

X11BASE?=	/usr/X11R6
MANDIR=		${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
