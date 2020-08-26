#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:41 jsg Exp $

LIB=	vulkan_util

NOPROFILE=

SRCS=	vk_debug_report.c \
	vk_format.c \
	vk_util.c

SRCS+=	vk_enum_to_str.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/vulkan/util \
		-I${MESA_SRC}/src/gallium/include \
		-DVK_USE_PLATFORM_XCB_KHR \
		-DVK_USE_PLATFORM_XLIB_KHR \
		-DVK_USE_PLATFORM_DISPLAY_KHR \
		-DVK_USE_PLATFORM_XLIB_XRANDR_EXT

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/vulkan/util
.PATH: ${MESA_SRC}/generated/src/vulkan/util
