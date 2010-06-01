# $OpenBSD: Makefile,v 1.4 2010/06/01 20:00:22 matthieu Exp $

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

afterinstall: geproto.txt
	${INSTALL_DATA} ${.CURDIR}/geproto.txt \
		${DESTDIR}${X11BASE}/share/doc/xextproto

.include <bsd.xorg.mk>
