# $OpenBSD: Makefile,v 1.4 2008/03/25 23:41:50 matthieu Exp $
.include <bsd.xconf.mk>

PROG=	xtsscale
MAN=	xtsscale.1

CPPFLAGS+=	-I${X11BASE}/include -I${X11BASE}/include/freetype2
LDADD+=		-L${X11BASE}/lib -lXft -lXrender -lX11 -lXau \
		-lXdmcp -lfontconfig -lexpat -lfreetype -lz

MANDIR=		${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
