# $OpenBSD: Makefile,v 1.12 2008/11/02 15:18:49 matthieu Exp $

.include <bsd.xconf.mk>

SUBDIR= freetype fontconfig libxtrans libXau libXdmcp \
	libX11  libXext  libXrender libXft libdmx libfontenc \
	libFS libICE liblbxutil liboldX libSM libXt \
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
