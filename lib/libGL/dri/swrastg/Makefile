#	$OpenBSD: Makefile,v 1.12 2015/02/20 23:31:05 jsg Exp $
.include <bsd.xconf.mk>

LIB=	swrast_dri.so

CPPFLAGS= \
	-I${GALLIUM}/include \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/drivers \
	-I${GALLIUM}/winsys \
	-I${GALLIUM}/winsys/sw/dri \
	-I${GALLIUM}/state_trackers/dri/common \
	-DHAVE_POSIX_MEMALIGN

DRIVER_LIBS= \
	../../gallium/drivers/trace/libtrace_pic.a \
	../../gallium/drivers/rbug/librbug_pic.a \
	../../gallium/drivers/softpipe/libsoftpipe_pic.a

DRIVER_DEFINES = \
	-DGALLIUM_RBUG \
	-DGALLIUM_TRACE \
	-DGALLIUM_SOFTPIPE \
	-DDRI_TARGET

.if ${XENOCARA_BUILD_GALLIUM} == "llvm"
DRIVER_DEFINES+= -DGALLIUM_LLVMPIPE
DRIVER_LIBS+=	../../gallium/drivers/llvmpipe/libllvmpipe_pic.a
.endif

WINSYS_SOURCES = \
	dri_sw_winsys.c

DRISW_SOURCES = \
	dri_context.c \
	dri_drawable.c \
	dri_screen.c \
	drisw.c

SWRAST_COMMON_SOURCES = \
	utils.c \
	dri_util.c \
	xmlconfig.c

GALLIUM_DRI_SOURCES = \
	swrast_drm_api.c

SRCS = \
	${WINSYS_SOURCES} \
	${DRISW_SOURCES} \
	${SWRAST_COMMON_SOURCES} \
	${GALLIUM_DRI_SOURCES}

LDADD=	\
	../../mesa/libmesagallium/libmesagallium_pic.a \
	../../glsl/libglsl/libglsl_pic.a \
	${DRIVER_LIBS} \
	../../gallium/libgallium/libgallium_pic.a \
	-L${X11BASE}/lib -lexpat -lm -lstdc++ -lc -lxcb-dri2

.include "../../gallium/Makefile.inc"
.include <bsd.xorg.mk>

.PATH: ${GALLIUM}/state_trackers/dri/common
.PATH: ${GALLIUM}/state_trackers/dri/sw
.PATH: ${GALLIUM}/targets/dri-swrast
.PATH: ${GALLIUM}/winsys/sw/dri
