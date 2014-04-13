# $OpenBSD: Makefile,v 1.13 2014/04/13 11:04:26 matthieu Exp $

.include <bsd.xconf.mk>

SUBDIR= bigreqsproto compositeproto dmxproto damageproto \
	fixesproto fontcacheproto fontsproto glproto inputproto \
	kbproto pmproto xineramaproto presentproto randrproto \
	recordproto renderproto resourceproto scrnsaverproto \
	videoproto x11proto xcb-proto xcmiscproto xextproto \
	xf86bigfontproto xf86dgaproto xf86miscproto \
	xf86vidmodeproto

.if ${XENOCARA_BUILD_DRI:L} == "yes"
SUBDIR+= xf86driproto dri2proto
.endif

.include <bsd.subdir.mk>

build: _SUBDIRUSE
