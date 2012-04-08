# $OpenBSD: Makefile,v 1.3 2012/04/08 09:46:58 matthieu Exp $

HEADERS_SUBDIR=	X11/
HEADERS=	ap_keysym.h DECkeysym.h HPkeysym.h keysymdef.h keysym.h \
		Sunkeysym.h Xalloca.h Xarch.h Xatom.h Xdefs.h XF86keysym.h \
		Xfuncs.h X.h Xmd.h Xosdefs.h Xos.h Xos_r.h Xproto.h \
		Xprotostr.h Xthreads.h Xw32defs.h XWDFile.h Xwindows.h \
		Xwinsock.h
GENERATED_HEADERS= Xfuncproto.h Xpoll.h
PKGCONFIG=	xproto.pc
SIADDRESSES=	IPv6.txt README hostname.txt localuser.txt
SPECS=		encoding.xml glossary.xml keysyms.xml \
		sect1-9.xml x11protocol.xml

all: ${GENERATED_HEADERS}

Xfuncproto.h: Xfuncproto.h.in
	@sed -e 's,^#undef NARROWPROTO,#define NARROWPROTO,' \
	    -e 's,^#undef FUNCPROTO,#define FUNCPROTO 15,' \
	    ${.CURDIR}/Xfuncproto.h.in > $@

Xpoll.h: Xpoll.h.in
	@sed -e 's#@USE_FDS_BITS@#fds_bits#g' ${.CURDIR}/Xpoll.h.in > $@

install-gh:  ${GENERATED_HEADERS}
	@echo installing ${GENERATED_HEADERS} in ${INCSDIR}/${HEADERS_SUBDIR}
	@for i in ${GENERATED_HEADERS}; do \
	    cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}/${HEADERS_SUBDIR}$$i;\
	done

afterinstall:
	@for i in ${SPECS}; do \
		echo installing $$i in ${X11BASE}/share/doc/x11proto; \
		${INSTALL_DATA} ${.CURDIR}/specs/$$i \
			${DESTDIR}${X11BASE}/share/doc/x11proto;\
	done
	@for i in ${SIADDRESSES}; do \
		echo installing $$i in ${X11BASE}/share/doc/x11proto/SIAddresses; \
		${INSTALL_DATA} ${.CURDIR}/specs/SIAddresses/$$i \
			${DESTDIR}${X11BASE}/share/doc/x11proto/SIAddresses;\
	done

clean-gh:
	rm -f ${GENERATED_HEADERS}

realinstall:	install-gh

clean:	clean-gh

.include <bsd.xorg.mk>
