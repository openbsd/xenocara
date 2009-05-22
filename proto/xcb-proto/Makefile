#	$OpenBSD: Makefile,v 1.2 2009/05/22 20:29:06 matthieu Exp $

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
PYTHONDIR=	${X11BASE}/lib/python${PYTHON_VERSION}/site-packages

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

.if defined(XENOCARA_REBUILD_XCB) && ${XENOCARA_REBUILD_XCB:L} == "yes"

XCBGEN_SRCS= \
	__init__.py \
	error.py \
	expr.py \
	matcher.py \
	state.py \
	xtypes.py

install-py:
	mkdir -p ${DESTDIR}${PYTHONDIR}/xcbgen
	cd ${.CURDIR}/xcbgen ; for i in ${XCBGEN_SRCS}; do \
		${INSTALL_DATA} $$i ${DESTDIR}${PYTHONDIR}/xcbgen; \
	done
	cd ${DESTDIR}${PYTHONDIR}/xcbgen; \
	env PYTHON=python${PYTHON_VERSION} ${SHELL} ${.CURDIR}/py-compile \
	--basedir ${PYTHONDIR}/xcbgen ${XCBGEN_SRCS}
	
realinstall: install-py

.endif

.include <bsd.xorg.mk>
