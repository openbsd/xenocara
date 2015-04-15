# $OpenBSD: Makefile,v 1.28 2015/04/15 09:39:53 ajacoutot Exp $

.include <bsd.xconf.mk>

XCB_LIBS= \
	libxcb/libxcb \
	libxcb/libxcb-composite \
	libxcb/libxcb-damage \
	libxcb/libxcb-dpms \
	libxcb/libxcb-dri2 \
	libxcb/libxcb-dri3 \
	libxcb/libxcb-glx \
	libxcb/libxcb-present \
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
	libxcb/libxcb-xkb \
	libxcb/libxcb-xprint \
	libxcb/libxcb-xtest \
	libxcb/libxcb-xv \
	libxcb/libxcb-xvmc \
	xcb-util \
	xcb-util-image \
	xcb-util-renderutil \
	xcb-util-cursor \
	xcb-util-keysyms \
	xcb-util-wm 

SUBDIR= libpthread-stubs \
	freetype fontconfig libxtrans libXau libXdmcp ${XCB_LIBS}\
	libX11  libXext  libXrender libXft libdmx libfontenc \
	libFS libICE libSM libXt \
	libXmu libXpm libXaw libXfixes libXcomposite \
	libXdamage libXcursor libXfont libXfontcache \
	libXi libXinerama libxkbfile libxkbui  \
	libXrandr libXRes libXScrnSaver \
	libXtst libXv libXvMC libXxf86dga libXxf86misc libXxf86vm \
	libpciaccess

.if ${XENOCARA_BUILD_PIXMAN:L} == "yes"
SUBDIR+= pixman
.endif

.if ${XENOCARA_BUILD_GL:L} == "yes"
SUBDIR+= libdrm libgbm
SUBDIR+= libglapi libGL libEGL libGLESv1_CM libGLESv2
SUBDIR+= libGLU libGLw libtxc_dxtn
SUBDIR+= libepoxy
.endif

.include <bsd.subdir.mk>

build: _SUBDIRUSE
