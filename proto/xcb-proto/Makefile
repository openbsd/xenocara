#	$OpenBSD: Makefile,v 1.9 2022/07/18 06:21:45 matthieu Exp $

PKGCONFIG=	xcb-proto.pc

FILES= \
	xcb.xsd \
	xproto.xml \
	bigreq.xml \
	composite.xml \
	damage.xml \
	dbe.xml \
	dpms.xml \
	dri2.xml \
	dri3.xml \
	ge.xml \
	glx.xml \
	present.xml \
	randr.xml \
	record.xml \
	render.xml \
	res.xml \
	screensaver.xml \
	shape.xml \
	shm.xml \
	sync.xml \
	xc_misc.xml \
	xevie.xml \
	xkb.xml \
	xf86dri.xml \
	xf86vidmode.xml \
	xfixes.xml \
	xinerama.xml \
	xinput.xml \
	xprint.xml \
	xselinux.xml \
	xtest.xml \
	xv.xml \
	xvmc.xml

XCBDATADIR=	${X11BASE}/share/xcb

EXTRA_PKGCONFIG_SUBST=	'-e s,@xcbincludedir@,$${datadir}/xcb,g' \
	'-e /^pythondir=/d'

realinstall:
	@for i in ${FILES}; do \
	    j="cmp -s ${.CURDIR}/src/$$i ${DESTDIR}${XCBDATADIR}/$$i || \
		${INSTALL_DATA} ${.CURDIR}/src/$$i \
			${DESTDIR}${XCBDATADIR}"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

.include <bsd.xorg.mk>
