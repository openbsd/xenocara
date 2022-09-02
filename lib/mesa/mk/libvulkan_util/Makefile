#	$OpenBSD: Makefile,v 1.4 2022/09/02 05:46:41 jsg Exp $

LIB=	vulkan_util

NOPROFILE=

SRCS=	vk_alloc.c \
	vk_format.c \
	vk_util.c

SRCS+=	vk_dispatch_table.c \
	vk_enum_to_str.c \
	vk_extensions.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/vulkan/util \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/util \
		-I${MESA_SRC}/generated/src/vulkan/util \
		-I${MESA_SRC}/generated/src/compiler/nir \
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
