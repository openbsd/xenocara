#	$OpenBSD: Makefile,v 1.6 2015/02/22 09:30:33 jsg Exp $

LIB=	radeonsi_dri.so

CPPFLAGS= \
	-I${GALLIUM}/include \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/drivers \
	-I${GALLIUM}/drivers/radeonsi \
	-I${GALLIUM}/winsys \
	-I${GALLIUM}/winsys/radeon/drm \
	-I${GALLIUM}/state_trackers/dri/common

DRIVER_LIBS= \
	../../gallium/drivers/trace/libtrace_pic.a \
	../../gallium/drivers/rbug/librbug_pic.a \
	../../gallium/drivers/radeonsi/libradeonsi_pic.a \
	../../gallium/drivers/radeon/libradeon_pic.a \
	../../gallium/winsys/radeon/drm/libradeonwinsys_pic.a

DRIVER_DEFINES = \
	-DGALLIUM_RADEONSI \
	-DGALLIUM_RBUG \
	-DGALLIUM_TRACE \
	-DDRI_TARGET

DRIDRM_SOURCES = \
	dri_context.c \
	dri_drawable.c \
	dri_query_renderer.c \
	dri_screen.c \
	dri2.c

COMMON_GALLIUM_SOURCES = \
	utils.c \
	vblank.c \
	dri_util.c \
	xmlconfig.c

GALLIUM_DRI_SOURCES = \
	drm_target.c

SRCS = \
	${DRIDRM_SOURCES} \
	${COMMON_GALLIUM_SOURCES} \
	${GALLIUM_DRI_SOURCES}

LDADD=	\
	../../mesa/libmesagallium/libmesagallium_pic.a \
	../../glsl/libglsl/libglsl_pic.a \
	${DRIVER_LIBS} \
	../../gallium/libgallium/libgallium_pic.a \
	-L${X11BASE}/lib -lexpat -lm -lstdc++ -lc -ldrm_radeon -lxcb-dri2

.include "../../gallium/Makefile.inc"
.include <bsd.xorg.mk>

.PATH: ${GALLIUM}/state_trackers/dri
.PATH: ${GALLIUM}/state_trackers/dri/common
.PATH: ${GALLIUM}/state_trackers/dri/drm
.PATH: ${GALLIUM}/targets/radeonsi/common
.PATH: ${GALLIUM}/winsys/radeon/drm
