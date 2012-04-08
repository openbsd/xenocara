# $OpenBSD: Makefile,v 1.5 2012/04/08 09:48:18 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	dpmsconst.h dpmsproto.h ge.h geproto.h \
		lbx.h lbxproto.h \
		mitmiscconst.h mitmiscproto.h multibufconst.h multibufproto.h \
		secur.h securproto.h shapeconst.h shapeproto.h \
		shm.h shmproto.h syncconst.h syncproto.h \
		ag.h agproto.h cup.h cupproto.h dbe.h \
		dbeproto.h EVI.h EVIproto.h \
		xtestext1proto.h xtestext1const.h xtestconst.h xtestproto.h
PKGCONFIG=	xextproto.pc
SPECS=		appendix.xml appgrp.xml dbe.xml dpms.xml evi.xml geproto.xml \
		lbx.xml multibuf.xml security.xml shape.xml shm.xml sync.xml \
		tog-cup.xml xtest.xml

afterinstall:
	@for i in ${SPECS}; do \
		echo installing $$i in ${X11BASE}/share/doc/xextproto;\
		${INSTALL_DATA} ${.CURDIR}/specs/$$i \
			${DESTDIR}${X11BASE}/share/doc/xextproto;\
	done

.include <bsd.xorg.mk>
