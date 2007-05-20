# $OpenBSD: Makefile,v 1.1 2007/05/20 14:43:12 robert Exp $

PROG=	xtsscale
MAN=	xtsscale.1

CPPFLAGS+=	-I${X11BASE}/include
LDADD+=		-L${X11BASE}/lib -lX11

X11BASE?=	/usr/X11R6
MANDIR=		${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
