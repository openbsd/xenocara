#	$OpenBSD: Makefile,v 1.5 2023/11/02 04:53:32 jsg Exp $

LIB=	vulkan_wsi

NOPROFILE=

SRCS=	wsi_common.c \
	wsi_common_drm.c \
	wsi_common_x11.c \
	wsi_common_headless.c \
	wsi_common_display.c

SRCS+=	wsi_common_entrypoints.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/vulkan/wsi \
		-I${MESA_SRC}/src/vulkan/runtime \
		-I${MESA_SRC}/src/vulkan/util \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/generated/src/vulkan/runtime \
		-I${MESA_SRC}/generated/src/vulkan/util \
		-I${MESA_SRC}/generated/src/vulkan/wsi \
		-DVK_USE_PLATFORM_XCB_KHR \
		-DVK_USE_PLATFORM_XLIB_KHR \
		-DVK_USE_PLATFORM_DISPLAY_KHR \
		-DVK_USE_PLATFORM_XLIB_XRANDR_EXT

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/vulkan/wsi
.PATH: ${MESA_SRC}/generated/src/vulkan/wsi
