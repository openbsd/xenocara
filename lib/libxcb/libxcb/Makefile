#	$OpenBSD: Makefile,v 1.2 2009/05/22 16:53:05 matthieu Exp $

LIB=		xcb

HEADERS=	xcb.h \
		xcbext.h

_SRCDIR=	${LIBXCB}/src

SRCS= \
	xcb_conn.c \
	xcb_out.c \
	xcb_in.c \
	xcb_ext.c \
	xcb_xid.c \
	xcb_list.c \
	xcb_util.c \
	xcb_auth.c

PKGCONFIG=	xcb.pc

EXTRA_PKGCONFIG_SUBST=	 '-e s,@NEEDED@,pthread-stubs xau >= 0.99.2 xdmcp,g' \
			    '-e s,@LIBS@,,g' \

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
