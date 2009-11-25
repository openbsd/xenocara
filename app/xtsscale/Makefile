# $OpenBSD: Makefile,v 1.5 2009/11/25 18:30:13 matthieu Exp $
.include <bsd.xconf.mk>

PROG=	xtsscale
MAN=	xtsscale.1

CPPFLAGS+=	-I${X11BASE}/include -I${X11BASE}/include/freetype2 \
		-I${X11BASE}/include/xorg 
LDADD+=		-L${X11BASE}/lib -lXft -lXi -lXrender -lX11 -lXau \
		-lXdmcp -lfontconfig -lexpat -lfreetype -lz

CFLAGS+=	-Wall
MANDIR=		${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
