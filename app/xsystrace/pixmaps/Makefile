#	$OpenBSD: Makefile,v 1.2 2007/03/30 17:24:13 aanriot Exp $
.include <bsd.own.mk>

PIXMAPS= deny-always.xpm deny.xpm logo.xpm permit-always.xpm \
	permit.xpm skull.xpm

install:
	@for f in ${PIXMAPS}; do \
		echo "${INSTALL_DATA} $$f \
			${DESTDIR}${INCSDIR}/X11/pixmaps" ; \
		${INSTALL_DATA} ${.CURDIR}/$$f \
			${DESTDIR}${INCSDIR}/X11/pixmaps ; \
	done 

.include <bsd.own.mk>
.include <bsd.xorg.mk>
	
