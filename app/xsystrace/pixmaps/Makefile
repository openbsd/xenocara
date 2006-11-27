#	$OpenBSD: Makefile,v 1.1 2006/11/27 20:19:57 matthieu Exp $
.include <bsd.own.mk>

PIXMAPS= deny-always.xpm deny.xpm logo.xpm permit-always.xpm \
	permit.xpm skull.xpm

install:
	@for f in ${PIXMAPS}; do \
		echo "${INSTALL} ${INSTALL_COPY} $$f \
			${DESTDIR}${INCSDIR}/X11/pixmaps" ; \
		${INSTALL} ${INSTALL_COPY} ${.CURDIR}/$$f \
			${DESTDIR}${INCSDIR}/X11/pixmaps ; \
	done 

.include <bsd.own.mk>
.include <bsd.xorg.mk>
	
