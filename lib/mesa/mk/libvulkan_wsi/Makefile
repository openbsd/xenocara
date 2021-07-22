#	$OpenBSD: Makefile,v 1.2 2021/07/22 11:10:12 jsg Exp $

LIB=	vulkan_wsi

NOPROFILE=

SRCS=	wsi_common.c \
	wsi_common_drm.c \
	wsi_common_x11.c \
	wsi_common_display.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/vulkan/wsi \
		-I${MESA_SRC}/src/vulkan/util \
		-I${MESA_SRC}/generated/src/vulkan/util \
		-DVK_USE_PLATFORM_XCB_KHR \
		-DVK_USE_PLATFORM_XLIB_KHR \
		-DVK_USE_PLATFORM_DISPLAY_KHR \
		-DVK_USE_PLATFORM_XLIB_XRANDR_EXT

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/vulkan/wsi
