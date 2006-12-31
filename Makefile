# $OpenBSD: Makefile,v 1.13 2006/12/31 10:52:07 matthieu Exp $
.include <bsd.own.mk>

LOCALAPPD=/usr/local/lib/X11/app-defaults
LOCALAPPX=/usr/local/lib/X11
REALAPPD=/etc/X11/app-defaults

SUBDIR= proto data/bitmaps lib app data/xkbdata xserver driver util doc
.ifndef NOFONTS
SUBDIR+= font
.endif
.ifmake(install)
SUBDIR+= share/mk
.endif

NOOBJ=

build: beforebuild _SUBDIRUSE

bootstrap:
	${SUDO} ${MAKE} distrib-dirs
	cd ${.CURDIR}/share/mk \
		&& ${SUDO} ${MAKE} X11BASE=${X11BASE} install

beforebuild: bootstrap
	cd ${.CURDIR}/util/macros \
		&& ${MAKE} -f Makefile.bsd-wrapper \
		&& ${SUDO} ${MAKE} -f Makefile.bsd-wrapper install
	${SUDO} ${MAKE} includes

beforeinstall:
	${MAKE} distrib-dirs
	${MAKE} includes

afterinstall: fix-appd
	cd distrib/notes; ${MAKE} install
	/usr/libexec/makewhatis ${DESTDIR}/usr/X11R6/man

realinstall: _SUBDIRUSE

fix-appd:
	# Make sure /usr/local/lib/X11/app-defaults is a link
	if [ ! -L $(DESTDIR)${LOCALAPPD} ]; then \
	    if [ -d $(DESTDIR)${LOCALAPPD} ]; then \
		mv $(DESTDIR)${LOCALAPPD}/* $(DESTDIR)${REALAPPD}; \
		rmdir $(DESTDIR)${LOCALAPPD}; \
	    fi; \
	    mkdir -p ${DESTDIR}${LOCALAPPX}; \
	    ln -s ${REALAPPD} ${DESTDIR}${LOCALAPPD}; \
	fi

release: release-clean distrib-dirs release-install dist

release-clean:
.if ! ( defined(DESTDIR) && defined(RELEASEDIR) )
	@echo You must set DESTDIR and RELEASEDIR for a release.; exit 255
.endif
	${RM} -rf ${DESTDIR}/usr/X11R6/* ${DESTDIR}/usr/X11R6/.[a-zA-Z0-9]*
	${RM} -rf ${DESTDIR}/var/cache/*
	${RM} -rf ${DESTDIR}/etc/X11/*
	${RM} -rf ${DESTDIR}/etc/fonts/*
	@if [ -d ${DESTDIR}/usr/X11R6 ] && [ "`cd ${DESTDIR}/usr/X11R6;ls`" ]; then \
		echo "Files found in ${DESTDIR}/usr/X11R6:"; \
		(cd ${DESTDIR}/usr/X11R6;/bin/pwd;ls -a); \
		echo "Cleanup before proceeding."; \
		exit 255; \
	fi

release-install:
	@${MAKE} install
.if ${MACHINE} == alpha || ${MACHINE} == hp300 || ${MACHINE} == mac68k || \
    ${MACHINE} == macppc || ${MACHINE} == sparc || ${MACHINE} == vax || \
    ${MACHINE} == zaurus
	@if [ -f $(DESTDIR)/etc/X11/xorg.conf ]; then \
	 echo "Not overwriting existing" $(DESTDIR)/etc/X11/xorg.conf; \
	else set -x; \
	 ${INSTALL} ${INSTALL_COPY} -o root -g wheel -m 644 \
		${XCONFIG} ${DESTDIR}/etc/X11 ; \
	fi
.endif

dist-rel:
	${MAKE} RELEASEDIR=`pwd`/rel DESTDIR=`pwd`/dest dist 2>&1 | tee distlog

dist:
	cd distrib/sets && \
		env MACHINE=${MACHINE} ksh ./maketars ${OSrev} ${OSREV} && \
		(env MACHINE=${MACHINE} ksh ./checkflist ${OSREV} || true)


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

.PHONY: all build beforeinstall install afterinstall release clean cleandir \
	distrib-dirs fix-appd

.include <bsd.subdir.mk>
.include <bsd.xorg.mk>
