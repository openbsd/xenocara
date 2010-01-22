# $OpenBSD: Makefile,v 1.9 2010/01/22 07:47:54 matthieu Exp $
.include <bsd.xconf.mk>

PROG=	xtsscale
MAN=	xtsscale.1

.if ${XENOCARA_BUILD_XCB:L} == "yes"
LIBXCB=		-lxcb
.endif

CPPFLAGS+=	-I${X11BASE}/include -I${X11BASE}/include/freetype2 \
		-I${.CURDIR}/../../driver/xf86-input-ws/include
LDADD+=		-L${X11BASE}/lib -lXft -lXi -lXrender -lXrandr -lXext -lX11 \
		${LIBXCB} -lXau -lXdmcp -lfontconfig -lexpat -lfreetype -lz

CFLAGS+=	-Wall
MANDIR=		${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
