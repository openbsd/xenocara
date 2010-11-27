# $OpenBSD: Makefile,v 1.10 2010/11/27 14:25:47 matthieu Exp $
.include <bsd.xconf.mk>

PROG=	xsystrace
MAN=	xsystrace.1

.ifmake install
SUBDIR=		pixmaps
.endif

CPPFLAGS+=	-I${X11BASE}/include
.if ${XENOCARA_BUILD_XCB:L} != "no"
LIBXCB=		-lxcb
.endif
LDADD+=		-L${X11BASE}/lib -lXaw -lXt -lSM -lICE -lXmu -lXpm -lX11 ${LIBXCB} -lXext -lXau -lXdmcp

SRCS=		main.c interface.c callbacks.c policy.c

afterinstall:
	${INSTALL_DATA} \
		${.CURDIR}/Xsystrace.ad ${DESTDIR}/etc/X11/app-defaults/Xsystrace

MANDIR=	${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
.include <bsd.subdir.mk>
