#	$OpenBSD: Makefile,v 1.3 2009/05/23 08:44:32 matthieu Exp $

LIB=		xcb

HEADERS=	xcb.h \
		xcbext.h \

EXTHEADERS=	xproto.h \
		bigreq.h \
		xc_misc.h

_SRCDIR=	${LIBXCB}/src

SRCS= \
	xcb_conn.c \
	xcb_out.c \
	xcb_in.c \
	xcb_ext.c \
	xcb_xid.c \
	xcb_list.c \
	xcb_util.c \
	xcb_auth.c \
	xproto.c \
	bigreq.c \
	xc_misc.c

PKGCONFIG=	xcb.pc

EXTRA_PKGCONFIG_SUBST=	 '-e s,@NEEDED@,pthread-stubs xau >= 0.99.2 xdmcp,g' \
			    '-e s,@LIBS@,,g' \

install-ext:
	@echo installing ${EXTHEADERS} in ${INCSDIR}/${HEADER_SUBDIR}
	cd ${.CURDIR}/../src; for i in ${EXTHEADERS}; do \
	   cmp -s $$i ${DESTDIR}${INCSDIR}/${HEADERS_SUBDIR}$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/${HEADERS_SUBDIR}$$i;\
	done

realinstall: install-ext

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
