# $OpenBSD: Makefile,v 1.3 2007/08/31 21:17:33 matthieu Exp $

PROG=	xtsscale
MAN=	xtsscale.1

CPPFLAGS+=	-I${X11BASE}/include -I${X11BASE}/include/freetype2
LDADD+=		-L${X11BASE}/lib -lXft -lXrender -lX11 -lXau \
		-lXdmcp -lfontconfig -lexpat -lfreetype -lz

X11BASE?=	/usr/X11R6
MANDIR=		${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
