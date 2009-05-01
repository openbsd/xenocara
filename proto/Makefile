# $OpenBSD: Makefile,v 1.7 2009/05/01 15:43:44 matthieu Exp $

.include <bsd.xconf.mk>

SUBDIR= bigreqsproto compositeproto dmxproto damageproto evieproto \
	fixesproto fontcacheproto fontsproto glproto inputproto \
	kbproto pmproto xineramaproto printproto randrproto \
	recordproto renderproto resourceproto scrnsaverproto \
	trapproto videoproto x11proto xcmiscproto xextproto \
	xf86bigfontproto xf86dgaproto xf86miscproto \
	xf86vidmodeproto 

.if ${XENOCARA_BUILD_DRI:L} == "yes"
SUBDIR+= xf86driproto dri2proto
.endif

.include <bsd.subdir.mk>

build: _SUBDIRUSE
