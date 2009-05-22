#	$OpenBSD: Makefile,v 1.1 2009/05/22 16:06:29 matthieu Exp $

PKGCONFIG=	xcb-proto.pc

FILES= \
	xcb.xsd \
	xproto.xml \
	bigreq.xml \
	composite.xml \
	damage.xml \
	dpms.xml \
	glx.xml \
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
	xf86dri.xml \
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
	'-e s,@pythondir@,$${prefix}/lib/python${PYTHON_VERSION}/site-packages,g'

realinstall:
	@for i in ${FILES}; do \
	    j="cmp -s ${.CURDIR}/src/$$i ${DESTDIR}${XCBDATADIR}/$$i || \
		${INSTALL_DATA} ${.CURDIR}/src/$$i \
			${DESTDIR}${XCBDATADIR}"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

.include <bsd.xorg.mk>
