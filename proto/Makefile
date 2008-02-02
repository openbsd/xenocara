# $OpenBSD: Makefile,v 1.4 2008/02/02 17:19:57 matthieu Exp $

.include <bsd.own.mk>

SUBDIR= bigreqsproto compositeproto dmxproto damageproto evieproto \
	fixesproto fontcacheproto fontsproto glproto inputproto \
	kbproto pmproto xineramaproto printproto randrproto \
	recordproto renderproto resourceproto scrnsaverproto \
	trapproto videoproto x11proto xcmiscproto xextproto \
	xf86bigfontproto xf86dgaproto xf86miscproto \
	xf86rushproto xf86vidmodeproto

.if defined(XENOCARA_BUILD_DRI) && ${XENOCARA_BUILD_DRI:L} == "yes"
SUBDIR+= xf86driproto
.endif

.include <bsd.subdir.mk>

build: _SUBDIRUSE
