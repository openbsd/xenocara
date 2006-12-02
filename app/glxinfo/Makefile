# $Xenocara$
.include <bsd.own.mk>
X11BASE?=	/usr/X11R6
DIST=		${XENOCARA_TOP}/dist/Mesa
GLXINFO=	${DIST}/progs/xdemos
MANDIR=		${X11BASE}/man/cat

PROG=	glxinfo

CPPFLAGS+=	-I${X11BASE}/include
LDADD+=		-L${X11BASE}/lib -lGL -lXext -lX11 -lm

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>

.PATH: ${GLXINFO}
