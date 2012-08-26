#	$OpenBSD: Makefile,v 1.4 2012/08/26 18:46:42 mpi Exp $

LIB=	swrast_dri.so

GALLIUM=	${.CURDIR}/../../../../dist/Mesa/src/gallium

CPPFLAGS= \
	-I${GALLIUM}/include \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/drivers \
	-I${GALLIUM}/winsys/sw/dri

DRIVER_LIBS= \
	../../gallium/drivers/trace/libtrace_pic.a \
	../../gallium/drivers/rbug/librbug_pic.a \
	../../gallium/drivers/softpipe/libsoftpipe_pic.a

.ifdef XENOCARA_HAVE_LLVM
DRIVER_LIBS+=	../../gallium/drivers/llvmpipe/libllvmpipe_pic.a
.endif

DRIVER_DEFINES = \
	-D__NOT_HAVE_DRM_H \
	-DGALLIUM_SOFTPIPE \
	-DGALLIUM_RBUG \
	-DGALLIUM_TRACE

SWRAST_COMMON_SOURCES = \
	utils.c \
	drisw_util.c \
	xmlconfig.c

DRIVER_SOURCES = \
	swrast_drm_api.c

SRCS = \
	${SWRAST_COMMON_SOURCES} \
	${DRIVER_SOURCES}

LDADD=	../../mesa/libmesagallium/libmesagallium_pic.a \
	../../glsl/libglsl/libglsl_pic.a \
	${DRIVER_LIBS} \
	../../gallium/libgallium/libgallium_pic.a \
	-L${X11BASE}/lib -lexpat -lm -lstdc++ -lc -lpthread
DPADD=	../../mesa/libmesagallium/libmesagallium_pic.a

.include "../../gallium/Makefile.inc"
.include <bsd.xorg.mk>

.PATH: ${GALLIUM}/targets/dri-swrast
