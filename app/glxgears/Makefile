# $OpenBSD: Makefile,v 1.8 2011/06/23 22:48:59 naddy Exp $
.include <bsd.xconf.mk>

MANDIR=		${X11BASE}/man/man

PROG=	glxgears

CPPFLAGS+=	-I${X11BASE}/include
LDADD+=		-L${X11BASE}/lib -lGL -lXext -lX11 -lXau -lXdmcp -lm

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
