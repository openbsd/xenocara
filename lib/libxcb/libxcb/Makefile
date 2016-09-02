#	$OpenBSD: Makefile,v 1.7 2016/09/02 10:09:43 matthieu Exp $

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

NEEDED=		pthread-stubs xau >= 0.99.2 xdmcp

CFLAGS+=	--std=c99

XCBPROTO_VERSION!=	pkg-config --modversion xcb-proto

EXTRA_PKGCONFIG_SUBST=	'-e s,@NEEDED@,${NEEDED},g' \
			'-e s,@XCBPROTO_VERSION@,${XCBPROTO_VERSION},g' \
			    '-e s,@LIBS@,,g' \

LDADD=		`pkg-config --libs "${NEEDED}"`

install-ext:
	@echo installing ${EXTHEADERS} in ${INCSDIR}/${HEADER_SUBDIR}
	cd ${.CURDIR}/../src; for i in ${EXTHEADERS}; do \
	   cmp -s $$i ${DESTDIR}${INCSDIR}/${HEADERS_SUBDIR}$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/${HEADERS_SUBDIR}$$i;\
	done

realinstall: install-ext

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
