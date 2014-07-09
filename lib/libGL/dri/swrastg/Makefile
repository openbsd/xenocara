#	$OpenBSD: Makefile,v 1.10 2014/07/09 21:32:40 jsg Exp $

LIB=	swrast_dri.so

CPPFLAGS= \
	-I${GALLIUM}/include \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/drivers \
	-I${GALLIUM}/winsys/sw/dri \
	-I${GALLIUM}/state_trackers/dri/common \
	-DHAVE_POSIX_MEMALIGN

SWRAST_DRIVERS= \
	../../gallium/drivers/trace/libtrace_pic.a \
	../../gallium/drivers/rbug/librbug_pic.a \
	../../gallium/drivers/softpipe/libsoftpipe_pic.a

DRIVER_DEFINES = \
	-D__NOT_HAVE_DRM_H \
	-DGALLIUM_SOFTPIPE \
	-DGALLIUM_RBUG \
	-DGALLIUM_TRACE

WINSYS_SOURCES = \
	dri_sw_winsys.c

DRISW_SOURCES = \
        dri_context.c \
        dri_screen.c \
        dri_drawable.c \
        drisw.c

SWRAST_COMMON_SOURCES = \
	utils.c \
	dri_util.c \
	xmlconfig.c

DRIVER_SOURCES = \
	swrast_drm_api.c

SRCS = \
	${WINSYS_SOURCES} \
	${DRISW_SOURCES} \
	${SWRAST_COMMON_SOURCES} \
	${DRIVER_SOURCES}

LDADD=	../../mesa/libmesagallium/libmesagallium_pic.a \
	../../glsl/libglsl/libglsl_pic.a \
	${SWRAST_DRIVERS} \
	../../gallium/libgallium/libgallium_pic.a \
	-L${X11BASE}/lib -lexpat -lm -lstdc++ -lc
DPADD=	../../mesa/libmesagallium/libmesagallium_pic.a

.include "../../gallium/Makefile.inc"
.include <bsd.xorg.mk>

.PATH: ${GALLIUM}/state_trackers/dri/common
.PATH: ${GALLIUM}/state_trackers/dri/sw
.PATH: ${GALLIUM}/targets/dri-swrast
.PATH: ${GALLIUM}/winsys/sw/dri
