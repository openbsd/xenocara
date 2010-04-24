# $OpenBSD: Makefile,v 1.10 2010/04/24 20:11:12 matthieu Exp $

.include <bsd.xconf.mk>

SUBDIR= bigreqsproto compositeproto dmxproto damageproto \
	fixesproto fontcacheproto fontsproto glproto inputproto \
	kbproto pmproto xineramaproto randrproto \
	recordproto renderproto resourceproto scrnsaverproto \
	videoproto x11proto xcmiscproto xextproto \
	xf86bigfontproto xf86dgaproto xf86miscproto \
	xf86vidmodeproto 

.if ${XENOCARA_BUILD_DRI:L} == "yes"
SUBDIR+= xf86driproto dri2proto
.endif

.if ${XENOCARA_BUILD_XCB:L} == "yes"
SUBDIR+= xcb-proto
.endif

# deprecated
# evieproto  printproto trapproto 

.include <bsd.subdir.mk>

build: _SUBDIRUSE
