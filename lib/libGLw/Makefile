# $Xenocara$

DIST=	${.CURDIR}/../../dist/Mesa
GLW=	${DIST}/src/glw

LIB= GLw

CPPFLAGS += -I${DIST}/include \
	-I${X11BASE}/include

SRCS= \
	GLwDrawA.c

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${GLW}
