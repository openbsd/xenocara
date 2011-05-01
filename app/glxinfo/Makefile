# $OpenBSD: Makefile,v 1.7 2011/05/01 15:28:37 matthieu Exp $
.include <bsd.xconf.mk>

MANDIR=		${X11BASE}/man/cat

PROG=	glxinfo

CPPFLAGS+=	-I${X11BASE}/include
LDADD+=		-L${X11BASE}/lib -lGL -lXext -lX11 -lXau -lXdmcp -lm

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
