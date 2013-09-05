#	$OpenBSD: Makefile,v 1.1 2013/09/05 15:11:58 jsg Exp $

LIB=	radeonsi_dri.so

CPPFLAGS= \
	-I${GALLIUM}/include \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/drivers \
	-I${GALLIUM}/drivers/radeonsi \
	-I${GALLIUM}/winsys \
	-I${GALLIUM}/state_trackers/dri/common

DRIVER_LIBS= \
	../../gallium/drivers/trace/libtrace_pic.a \
	../../gallium/drivers/rbug/librbug_pic.a \
	../../gallium/drivers/radeonsi/libradeonsi_pic.a

DRIVER_DEFINES = \
	-DGALLIUM_RBUG \
	-DGALLIUM_TRACE

WINSYS_SOURCES = \
	radeon_drm_bo.c \
	radeon_drm_cs.c \
	radeon_drm_cs_dump.c \
	radeon_drm_winsys.c

DRIDRM_SOURCES = \
	dri_context.c \
	dri_screen.c \
	dri_drawable.c \
	dri2.c

COMMON_GALLIUM_SOURCES = \
	utils.c \
	vblank.c \
	dri_util.c \
	xmlconfig.c

DRIVER_SOURCES = \
	target.c \

SRCS = \
	${WINSYS_SOURCES} \
	${DRIDRM_SOURCES} \
	${COMMON_GALLIUM_SOURCES} \
	${DRIVER_SOURCES}

LDADD=	../../mesa/libmesagallium/libmesagallium_pic.a \
	../../glsl/libglsl/libglsl_pic.a \
	${DRIVER_LIBS} \
	../../gallium/libgallium/libgallium_pic.a \
	-L${X11BASE}/lib -lexpat -lm -lstdc++ -lc -ldrm_radeon -lxcb-dri2
DPADD=	../../mesa/libmesagallium/libmesagallium_pic.a

.include "../../gallium/Makefile.inc"
.include <bsd.xorg.mk>

.PATH: ${GALLIUM}/state_trackers/dri/common
.PATH: ${GALLIUM}/state_trackers/dri/drm
.PATH: ${GALLIUM}/targets/dri-radeonsi
.PATH: ${GALLIUM}/winsys/radeon/drm
