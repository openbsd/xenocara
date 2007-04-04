# $OpenBSD: Makefile,v 1.4 2007/04/04 22:33:46 david Exp $

SUBDIR= freetype expat fontconfig libxtrans libXau libXdmcp \
	libX11  libXext  libXrender libXft libdmx libfontenc \
	libFS libICE liblbxutil liboldX libSM libXt \
	libXmu libXpm libXp libXaw libXfixes libXcomposite \
	libXdamage libXcursor libXevie libXfont libXfontcache \
	libXi libXinerama libxkbfile libxkbui libXprintUtil \
	libXprintAppUtil libXrandr libXRes libXScrnSaver libXTrap \
	libXtst libXv libXvMC libXxf86dga libXxf86misc libXxf86vm

SUBDIR+= libGL libGLU libGLw

DRM_ARCHES= alpha amd64 i386 macppc sparc64

.for a in $(DRM_ARCHES)
SUBDIR_$a+= libdrm
.endfor
SUBDIR+= $(SUBDIR_$(MACHINE))

.include <bsd.subdir.mk>

build: _SUBDIRUSE
