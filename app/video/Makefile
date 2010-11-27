# $OpenBSD: Makefile,v 1.4 2010/11/27 14:25:47 matthieu Exp $
.include <bsd.xconf.mk>

PROG=	video

CPPFLAGS+= -I${X11BASE}/include

.if ${XENOCARA_BUILD_XCB:L} != "no"
LIBXCB=	-lxcb
.endif

LDADD+= -L${X11BASE}/lib -lXv -lXext -lX11 ${LIBXCB} -lXau -lXdmcp
MANDIR=	${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
