# $OpenBSD: Makefile,v 1.6 2009/12/01 21:20:56 matthieu Exp $
.include <bsd.xconf.mk>

PROG=	xtsscale
MAN=	xtsscale.1

CPPFLAGS+=	-I${X11BASE}/include -I${X11BASE}/include/freetype2 \
		-I${.CURDIR}/../../driver/xf86-input-ws/include
LDADD+=		-L${X11BASE}/lib -lXft -lXi -lXrender -lX11 -lXau \
		-lXdmcp -lfontconfig -lexpat -lfreetype -lz

CFLAGS+=	-Wall
MANDIR=		${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
