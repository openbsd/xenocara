# $OpenBSD: Makefile,v 1.9 2008/03/25 23:41:50 matthieu Exp $

.include <bsd.xconf.mk>

SUBDIR= freetype fontconfig libxtrans libXau libXdmcp \
	libX11  libXext  libXrender libXft libdmx libfontenc \
	libFS libICE liblbxutil liboldX libSM libXt \
	libXmu libXpm libXp libXaw libXfixes libXcomposite \
	libXdamage libXcursor libXevie libXfont libXfontcache \
	libXi libXinerama libxkbfile libxkbui libXprintUtil \
	libXprintAppUtil libXrandr libXRes libXScrnSaver libXTrap \
	libXtst libXv libXvMC libXxf86dga libXxf86misc libXxf86vm

.if ${XENOCARA_BUILD_DRI:L} == "yes"
SUBDIR+= libdrm
.endif

.if ${XENOCARA_BUILD_GL:L} == "yes"
SUBDIR+= libGL libGLU libGLw
.endif

# Gcc ICE on sh
.if ${MACHINE_ARCH} != "sh"
SUBDIR+= pixman
.endif

.include <bsd.subdir.mk>

build: _SUBDIRUSE
