# $OpenBSD: Makefile,v 1.1 2008/03/25 23:28:20 matthieu Exp $

HEADERS_SUBDIR=	X11/
HEADERS=	ap_keysym.h DECkeysym.h HPkeysym.h keysymdef.h keysym.h \
		Sunkeysym.h Xalloca.h Xarch.h Xatom.h Xdefs.h XF86keysym.h \
		Xfuncs.h X.h Xmd.h Xosdefs.h Xos.h Xos_r.h Xproto.h \
		Xprotostr.h Xthreads.h Xw32defs.h XWDFile.h Xwindows.h \
		Xwinsock.h
GENERATED_HEADERS= Xfuncproto.h Xpoll.h
PKGCONFIG=	xproto.pc

all:: ${GENERATED_HEADERS}

Xfuncproto.h: Xfuncproto.h.in
	@sed -e 's,^#undef NARROWPROTO,#define NARROWPROTO,' \
	    -e 's,^#undef FUNCPROTO,#define FUNCPROTO 15,' \
	    ${.CURDIR}/Xfuncproto.h.in > $@

Xpoll.h: Xpoll.h.in
	@sed -e 's#@USE_FDS_BITS@#fds_bits#g' ${.CURDIR}/Xpoll.h.in > $@

install::  ${GENERATED_HEADERS}
	@echo installing ${GENERATED_HEADERS} in ${INCSDIR}/${HEADERS_SUBDIR}
	@for i in ${GENERATED_HEADERS}; do \
	    cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/${HEADERS_SUBDIR}$$i;\
	done

clean::
	rm -f ${GENERATED_HEADERS}

.include <bsd.xorg.mk>
