# $OpenBSD: Makefile,v 1.5 2007/04/08 21:53:14 matthieu Exp $
.include <bsd.own.mk>
X11BASE?=	/usr/X11R6
DIST=		${XSRCDIR}/dist/Mesa
GLXINFO=	${DIST}/progs/xdemos
MANDIR=		${X11BASE}/man/cat

PROG=	glxinfo

CPPFLAGS+=	-I${X11BASE}/include
LDADD+=		-L${X11BASE}/lib -lGL -lXext -lX11 -lXau -lXdmcp -lm

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>

.PATH: ${GLXINFO}
