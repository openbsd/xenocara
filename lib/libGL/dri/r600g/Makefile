#	$OpenBSD: Makefile,v 1.2 2013/08/12 04:16:52 jsg Exp $

LIB=	r600_dri.so

CPPFLAGS= \
	-I${GALLIUM}/include \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/drivers \
	-I${GALLIUM}/drivers/r600 \
	-I${GALLIUM}/winsys \
	-I${GALLIUM}/state_trackers/dri/common

DRIVER_LIBS= \
	../../gallium/drivers/trace/libtrace_pic.a \
	../../gallium/drivers/rbug/librbug_pic.a \
	../../gallium/drivers/r600/libr600_pic.a

DRIVER_DEFINES = \
	-DGALLIUM_RBUG \
	-DGALLIUM_TRACE

WINSYS_SOURCES = \
	bof.c \
	evergreen_hw_context.c \
	radeon_bo.c \
	radeon_pciid.c \
	r600_bo.c \
	r600_drm.c \
	r600_hw_context.c \
	r600_bomgr.c

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
	-L${X11BASE}/lib -lexpat -lm -lstdc++ -lc
DPADD=	../../mesa/libmesagallium/libmesagallium_pic.a

.include "../../gallium/Makefile.inc"
.include <bsd.xorg.mk>

.PATH: ${GALLIUM}/state_trackers/dri/common
.PATH: ${GALLIUM}/state_trackers/dri/drm
.PATH: ${GALLIUM}/targets/dri-r600
.PATH: ${GALLIUM}/winsys/r600/drm
