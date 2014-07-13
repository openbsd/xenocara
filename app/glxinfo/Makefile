# $OpenBSD: Makefile,v 1.9 2014/07/13 09:47:57 jsg Exp $
.include <bsd.xconf.mk>

MANDIR=		${X11BASE}/man/man

PROG=	glxinfo
SRCS=	glinfo_common.c glxinfo.c

CPPFLAGS+=	-I${X11BASE}/include
LDADD+=		-L${X11BASE}/lib -lGL -lXext -lX11 -lXau -lXdmcp -lm

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
