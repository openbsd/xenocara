# $OpenBSD: Makefile,v 1.14 2009/05/22 17:58:45 matthieu Exp $

.include <bsd.xconf.mk>

.if ${XENOCARA_BUILD_XCB:L} == "yes"
XCB_LIBS=	libpthread-stubs libxcb
.endif

SUBDIR= freetype fontconfig libxtrans libXau libXdmcp ${XCB_LIBS}\
	libX11  libXext  libXrender libXft libdmx libfontenc \
	libFS libICE liblbxutil libSM libXt \
	libXmu libXpm libXp libXaw libXfixes libXcomposite \
	libXdamage libXcursor libXevie libXfont libXfontcache \
	libXi libXinerama libxkbfile libxkbui libXprintUtil \
	libXprintAppUtil libXrandr libXRes libXScrnSaver libXTrap \
	libXtst libXv libXvMC libXxf86dga libXxf86misc libXxf86vm \
	libpciaccess pixman

# libdrm is needed by the via driver built on i386 only
.if ${XENOCARA_BUILD_DRI:L} == "yes" || ${MACHINE} == "i386"
SUBDIR+= libdrm
.endif

.if ${XENOCARA_BUILD_GL:L} == "yes"
SUBDIR+= libGL libGLU libGLw
.endif

.include <bsd.subdir.mk>

build: _SUBDIRUSE
