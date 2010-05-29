# $OpenBSD: Makefile,v 1.19 2010/05/29 17:25:11 matthieu Exp $

.include <bsd.xconf.mk>

.if ${XENOCARA_BUILD_XCB:L} == "yes"
XCB_LIBS= \
	libpthread-stubs \
	libxcb/libxcb \
	libxcb/libxcb-composite \
	libxcb/libxcb-damage \
	libxcb/libxcb-dpms \
	libxcb/libxcb-dri2 \
	libxcb/libxcb-glx \
	libxcb/libxcb-randr \
	libxcb/libxcb-record \
	libxcb/libxcb-render \
	libxcb/libxcb-res \
	libxcb/libxcb-screensaver \
	libxcb/libxcb-shape \
	libxcb/libxcb-shm \
	libxcb/libxcb-sync \
	libxcb/libxcb-xevie \
	libxcb/libxcb-xf86dri \
	libxcb/libxcb-xfixes \
	libxcb/libxcb-xinerama \
	libxcb/libxcb-xprint \
	libxcb/libxcb-xtest \
	libxcb/libxcb-xv \
	libxcb/libxcb-xvmc \
	xcb-util/atom \
	xcb-util/aux \
	xcb-util/event \
	xcb-util/property \
	xcb-util/icccm \
	xcb-util/image \
	xcb-util/keysyms \
	xcb-util/renderutil \
	xcb-util/reply
.endif

SUBDIR= freetype fontconfig libxtrans libXau libXdmcp ${XCB_LIBS}\
	libX11  libXext  libXrender libXft libdmx libfontenc \
	libFS libICE libSM libXt \
	libXmu libXpm libXaw libXfixes libXcomposite \
	libXdamage libXcursor libXfont libXfontcache \
	libXi libXinerama libxkbfile libxkbui  \
	libXrandr libXRes libXScrnSaver \
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
