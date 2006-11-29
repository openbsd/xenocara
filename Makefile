# $OpenBSD: Makefile,v 1.2 2006/11/29 17:42:27 matthieu Exp $
.include <bsd.own.mk>

X11BASE?=	/usr/X11R6

SUBDIR= proto data/bitmaps lib app data/xkbdata xserver driver doc util
.ifndef NOFONTS
SUBDIR+= font
.endif
.ifmake(install)
SUBDIR+= share/mk
.endif

NOOBJS=

build: beforebuild _SUBDIRUSE

bootstrap:
	${SUDO} ${MAKE} distrib-dirs
	cd ${.CURDIR}/share/mk \
		&& ${SUDO} ${MAKE} install

beforebuild: bootstrap
	cd ${.CURDIR}/util/macros \
		&& ${MAKE} -f Makefile.bsd-wrapper \
		&& ${SUDO} ${MAKE} -f Makefile.bsd-wrapper install
	${SUDO} ${MAKE} includes

beforeinstall:
	${MAKE} distrib-dirs
	${MAKE} includes

release:

distrib-dirs:
	if [ ! -d ${DESTDIR}${X11BASE}/. ]; then \
		${INSTALL} -d -o root -g wheel -m 755 ${DESTDIR}${X11BASE}/; \
	fi
	mtree -qdef ${.CURDIR}/etc/mtree/BSD.x11.dist \
		-p ${DESTDIR}${X11BASE}/ -U
	if [ ! -d ${DESTDIR}${X11ETC}/. ]; then \
		${INSTALL} -d -o root -g wheel -m 755 ${DESTDIR}${X11ETC}/; \
	fi
	mtree -qdef ${.CURDIR}/etc/mtree/BSD.etc-x11.dist \
		-p ${DESTDIR}${X11ETC}/ -U

update: _SUBDIRUSE

.PHONY: all build beforeinstall install afterinstall release clean cleandir

.include <bsd.subdir.mk>
.include <bsd.xorg.mk>
