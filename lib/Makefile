# $OpenBSD: Makefile,v 1.5 2007/05/12 14:24:59 matthieu Exp $

SUBDIR= freetype expat fontconfig libxtrans libXau libXdmcp \
	libX11  libXext  libXrender libXft libdmx libfontenc \
	libFS libICE liblbxutil liboldX libSM libXt \
	libXmu libXpm libXp libXaw libXfixes libXcomposite \
	libXdamage libXcursor libXevie libXfont libXfontcache \
	libXi libXinerama libxkbfile libxkbui libXprintUtil \
	libXprintAppUtil libXrandr libXRes libXScrnSaver libXTrap \
	libXtst libXv libXvMC libXxf86dga libXxf86misc libXxf86vm

.if ${MACHINE} != "sgi" && ${MACHINE} != "vax"
SUBDIR+= libGL libGLU libGLw
.endif

DRM_ARCHES= alpha amd64 i386 macppc sparc64

.for a in $(DRM_ARCHES)
SUBDIR_$a+= libdrm
.endfor
SUBDIR+= $(SUBDIR_$(MACHINE))

.include <bsd.subdir.mk>

build: _SUBDIRUSE
