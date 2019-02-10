# $OpenBSD: Makefile,v 1.17 2019/02/10 23:07:47 matthieu Exp $

.include <bsd.xconf.mk>

SUBDIR= bigreqsproto compositeproto dmxproto damageproto \
	fixesproto fontsproto glproto inputproto \
	kbproto pmproto xineramaproto presentproto randrproto \
	recordproto renderproto resourceproto scrnsaverproto \
	videoproto x11proto xcb-proto xcmiscproto xextproto \
	xf86bigfontproto xf86dgaproto \
	xf86vidmodeproto

.if ${XENOCARA_BUILD_DRI:L} == "yes"
SUBDIR+= xf86driproto dri2proto dri3proto
.endif

.include <bsd.subdir.mk>

build: _SUBDIRUSE
