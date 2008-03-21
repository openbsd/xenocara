# $OpenBSD: Makefile,v 1.8 2008/03/21 21:07:48 oga Exp $

SUBDIR= freetype fontconfig libxtrans libXau libXdmcp \
	libX11  libXext  libXrender libXft libdmx libfontenc \
	libFS libICE liblbxutil liboldX libSM libXt \
	libXmu libXpm libXp libXaw libXfixes libXcomposite \
	libXdamage libXcursor libXevie libXfont libXfontcache \
	libXi libXinerama libxkbfile libxkbui libXprintUtil \
	libXprintAppUtil libXrandr libXRes libXScrnSaver libXTrap \
	libXtst libXv libXvMC libXxf86dga libXxf86misc libXxf86vm

DRM_ARCHES= alpha amd64 i386 macppc sparc64

.for a in $(DRM_ARCHES)
SUBDIR_$a+= libdrm
.endfor
SUBDIR+= $(SUBDIR_$(MACHINE))

.if ${MACHINE} != "sgi" && ${MACHINE} != "vax"
SUBDIR+= libGL libGLU libGLw
.endif

# Gcc ICE on sh
.if ${MACHINE_ARCH} != "sh"
SUBDIR+= pixman
.endif

.include <bsd.subdir.mk>

build: _SUBDIRUSE
