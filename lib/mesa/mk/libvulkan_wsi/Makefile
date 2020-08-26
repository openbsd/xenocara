#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:41 jsg Exp $

LIB=	vulkan_wsi

NOPROFILE=

SRCS=	wsi_common.c \
	wsi_common_x11.c \
	wsi_common_display.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/vulkan/wsi \
		-I${MESA_SRC}/src/vulkan/util \
		-DVK_USE_PLATFORM_XCB_KHR \
		-DVK_USE_PLATFORM_XLIB_KHR \
		-DVK_USE_PLATFORM_DISPLAY_KHR \
		-DVK_USE_PLATFORM_XLIB_XRANDR_EXT

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/vulkan/wsi
