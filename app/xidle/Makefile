# $OpenBSD: Makefile,v 1.5 2010/11/27 14:25:47 matthieu Exp $
.include <bsd.xconf.mk>

PROG=	xidle

CPPFLAGS+= -I${X11BASE}/include
CFLAGS+= -Wall
.if ${XENOCARA_BUILD_XCB:L} != "no"
LIBXCB=	-lxcb
.endif
LDADD+=	-L${X11BASE}/lib -lXss -lXext -lX11 ${LIBXCB} -lXau -lXdmcp

MANDIR=	${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
