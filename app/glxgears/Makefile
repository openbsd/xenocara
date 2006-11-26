# $Xenocara$
.include <bsd.own.mk>
X11BASE?=	/usr/X11R6
DIST=		${XENOCARA_TOP}/dist/Mesa
GLXGEARS=	${DIST}/progs/xdemos
MANDIR=		${X11BASE}/man/cat

PROG=	glxgears

CPPFLAGS+=	-I${X11BASE}/include
LDADD+=		-L${X11BASE}/lib -lGL -lXext -lX11 -lm

.include <bsd.prog.mk>
.include <bsd.xorg.mk>

.PATH: ${GLXGEARS}

