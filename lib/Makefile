# $OpenBSD: Makefile,v 1.41 2018/09/11 19:34:56 matthieu Exp $

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
	libxcb/libxcb-xf86dri \
	libxcb/libxcb-xfixes \
	libxcb/libxcb-xinerama \
	libxcb/libxcb-xinput \
	libxcb/libxcb-xkb \
	libxcb/libxcb-xtest \
	libxcb/libxcb-xv \
	libxcb/libxcb-xvmc \
	xcb-util \
	xcb-util-image \
	xcb-util-renderutil \
	xcb-util-cursor \
	xcb-util-keysyms \
	xcb-util-wm \
	xcb-util-xrm

SUBDIR= freetype fontconfig libxtrans libXau libXdmcp ${XCB_LIBS}\
	libX11  libXext  libXrender libXft libdmx libfontenc \
	libFS libICE libSM libXt \
	libXmu libXpm libXaw libXfixes libXcomposite \
	libXdamage libXcursor libXfont2  \
	libXi libXinerama libxkbfile libxkbui \
	libXrandr libXpresent libXRes libXScrnSaver \
	libXtst libXv libXvMC libXxf86dga libXxf86vm \
	libpciaccess pixman

.if ${XENOCARA_BUILD_DRI3:L} == "yes"
SUBDIR+= libxshmfence
.endif

.if ${XENOCARA_BUILD_GL:L} == "yes"
SUBDIR+= libdrm
SUBDIR+= mesa
SUBDIR+= libGLU libGLw libtxc_dxtn
SUBDIR+= libepoxy
.endif

.include <bsd.subdir.mk>

build: _SUBDIRUSE
