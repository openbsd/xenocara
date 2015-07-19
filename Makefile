# $OpenBSD: Makefile,v 1.66 2015/07/19 10:44:05 matthieu Exp $
.include <bsd.own.mk>
.include <bsd.xconf.mk>

LOCALAPPD=/usr/local/lib/X11/app-defaults
LOCALAPPX=/usr/local/lib/X11
REALAPPD=/etc/X11/app-defaults
XCONFIG=${XSRCDIR}/etc/X11.${MACHINE}/xorg.conf
RM?=rm

.if ${MACHINE_ARCH} != "vax"
XSERVER= xserver
.endif

.if defined(XENOCARA_BUILD_PIXMAN)
.if ${XENOCARA_BUILD_PIXMAN:L} == "yes" && \
    ${XENOCARA_BUILD_GL:L} == "yes" && \
    ${XENOCARA_HAVE_SHARED_LIBS:L} == "yes"
XSERVER+= kdrive
.endif
.endif

SUBDIR= proto font/util data/bitmaps lib app data \
	${XSERVER} driver util doc
.ifndef NOFONTS
SUBDIR+= font
.endif
SUBDIR+= distrib/notes

NOOBJ=

.if defined(DESTDIR)
build:
	@echo "Cannot run ${MAKE} build with DESTDIR set"
	@exit 2
.else
build: 
	exec ${SUDO} ${MAKE} bootstrap-root
	cd util/macros && exec ${MAKE} -f Makefile.bsd-wrapper
	exec ${SUDO} ${MAKE} beforebuild
	exec ${MAKE} realbuild
	exec ${SUDO} ${MAKE} afterbuild
.endif

realbuild: _SUBDIRUSE
	# that's all folks

bootstrap:
	exec ${SUDO} ${MAKE} bootstrap-root

bootstrap-root:
	exec ${MAKE} distrib-dirs
	exec ${MAKE} install-mk

beforeinstall beforebuild:
	cd util/macros && exec ${MAKE} -f Makefile.bsd-wrapper install
	exec ${MAKE} includes

afterinstall afterbuild:
	exec ${MAKE} fix-appd
	/usr/sbin/makewhatis -Qv ${DESTDIR}/usr/X11R6/man
	touch ${DESTDIR}/usr/share/sysmerge/xetcsum
	cd ${DESTDIR}/ && \
		sort ${.CURDIR}/distrib/sets/lists/xetc/{mi,md.${MACHINE}} | \
		xargs sha256 -h ${DESTDIR}/usr/share/sysmerge/xetcsum || true
	cd distrib/sets && exec ${MAKE}

install-mk:
	cd share/mk && exec ${MAKE} X11BASE=${X11BASE} install

fix-appd:
	# Make sure /usr/local/lib/X11/app-defaults is a link
	if [ ! -L $(DESTDIR)${LOCALAPPD} ]; then \
	    if [ -d $(DESTDIR)${LOCALAPPD} ]; then \
		mv $(DESTDIR)${LOCALAPPD}/* $(DESTDIR)${REALAPPD} || true; \
		rmdir $(DESTDIR)${LOCALAPPD}; \
	    fi; \
	    mkdir -p ${DESTDIR}${LOCALAPPX}; \
	    ln -s ${REALAPPD} ${DESTDIR}${LOCALAPPD}; \
	fi

font-cache:
	cd font/alias && exec ${MAKE} -f Makefile.bsd-wrapper afterinstall

.if ! ( defined(DESTDIR) && defined(RELEASEDIR) )
release:
	@echo You must set DESTDIR and RELEASEDIR for a release.; exit 255
.else
release: sha

sha: release-clean release-install dist hash

hash: dist
	-cd ${RELEASEDIR}; \
		cksum -a sha256 x*tgz > SHA256

.ORDER: release-clean release-install dist hash
.endif

release-clean:
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
	@exec ${MAKE} bootstrap-root
.if ${MACHINE} == zaurus
	@if [ -f $(DESTDIR)/etc/X11/xorg.conf ]; then \
	 echo "Not overwriting existing" $(DESTDIR)/etc/X11/xorg.conf; \
	else set -x; \
	 ${INSTALL} ${INSTALL_COPY} -o root -g wheel -m 644 \
		${XCONFIG} ${DESTDIR}/etc/X11 ; \
	fi
.endif
	@exec ${MAKE} install

dist-rel:
	${MAKE} RELEASEDIR=`pwd`/rel DESTDIR=`pwd`/dest dist 2>&1 | tee distlog

dist:
	cd distrib/sets && \
		env MACHINE=${MACHINE} ksh ./maketars ${OSrev} ${OSREV} && \
		{ env MACHINE=${MACHINE} ksh ./checkflist ${OSREV} || true ; }

checkdist:
	@cd distrib/sets && \
		{ env MACHINE=${MACHINE} ksh ./checkflist ${OSREV} || true ; }

distrib-dirs:
.if defined(DESTDIR) && ${DESTDIR} != ""
	# running mtree under ${DESTDIR}
	mtree -qdef /etc/mtree/BSD.x11.dist -p ${DESTDIR} -U
.else
	# running mtree
	mtree -qdef /etc/mtree/BSD.x11.dist -p / -U
.endif


.PHONY: all build beforeinstall install afterinstall release clean cleandir \
	dist distrib-dirs fix-appd beforebuild bootstrap afterbuild realbuild \
	install-mk bootstrap-root

.include <bsd.subdir.mk>
.include <bsd.xorg.mk>
