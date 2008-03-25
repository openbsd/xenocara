# $OpenBSD: Makefile,v 1.6 2008/03/25 23:41:50 matthieu Exp $
.include <bsd.xconf.mk>

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
