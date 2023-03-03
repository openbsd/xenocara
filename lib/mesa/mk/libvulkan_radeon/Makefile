#	$OpenBSD: Makefile,v 1.10 2023/03/03 06:32:18 jsg Exp $

LIB=	vulkan_radeon

NOPROFILE=

SRCS=	radv_metro_exodus.c \
	radv_rra_layer.c \
	radv_sqtt_layer.c \
	radv_null_bo.c \
	radv_null_cs.c \
	radv_null_winsys.c \
	radv_acceleration_structure.c \
	radv_android.c \
	radv_cmd_buffer.c \
	radv_debug.c \
	radv_device.c \
	radv_descriptor_set.c \
	radv_device_generated_commands.c \
	radv_formats.c \
	radv_image.c \
	radv_meta.c \
	radv_meta_blit.c \
	radv_meta_blit2d.c \
	radv_meta_buffer.c \
	radv_meta_bufimage.c \
	radv_meta_clear.c \
	radv_meta_copy.c \
	radv_meta_copy_vrs_htile.c \
	radv_meta_dcc_retile.c \
	radv_meta_decompress.c \
	radv_meta_etc_decode.c \
	radv_meta_fast_clear.c \
	radv_meta_fmask_copy.c \
	radv_meta_fmask_expand.c \
	radv_meta_resolve.c \
	radv_meta_resolve_cs.c \
	radv_meta_resolve_fs.c \
	radv_nir_apply_pipeline_layout.c \
	radv_nir_lower_abi.c \
	radv_nir_lower_ray_queries.c \
	radv_nir_lower_ycbcr_textures.c \
	radv_perfcounter.c \
	radv_pipeline.c \
	radv_pipeline_cache.c \
	radv_pipeline_rt.c \
	radv_rra.c \
	radv_rt_common.c \
	radv_sdma_copy_image.c \
	radv_shader.c \
	radv_shader_args.c \
	radv_shader_info.c \
	radv_spm.c \
	radv_sqtt.c \
	radv_query.c \
	radv_wsi.c \
	si_cmd_buffer.c \

SRCS+=	radv_amdgpu_bo.c \
	radv_amdgpu_cs.c \
	radv_amdgpu_surface.c \
	radv_amdgpu_winsys.c

# with_llvm
SRCS+=	radv_llvm_helper.cpp \
	radv_nir_to_llvm.c

# from src/amd/vulkan/radix_sort/meson.build
SRCS+=	barrier.c \
	util.c \
	radix_sort_vk.c \
	radv_radix_sort.c \

SRCS+=	radv_entrypoints.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}

LLVM_CXXFLAGS!= llvm-config --cxxflags
CXXFLAGS+=	${LLVM_CXXFLAGS}

CPPFLAGS+=	-I${MESA_SRC}/src/amd/vulkan \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/vulkan/runtime \
		-I${MESA_SRC}/src/vulkan/util \
		-I${MESA_SRC}/src/vulkan/wsi \
		-I${MESA_SRC}/src/amd/common \
		-I${MESA_SRC}/src/amd/compiler \
		-I${MESA_SRC}/src/amd/llvm \
		-I${MESA_SRC}/src/compiler \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/util \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-I${MESA_SRC}/generated/src/amd/common \
		-I${MESA_SRC}/generated/src/amd/vulkan \
		-I${MESA_SRC}/generated/src/vulkan/runtime \
		-I${MESA_SRC}/generated/src/vulkan/util \
		-I${MESA_SRC}/generated/src/vulkan/wsi \
		-DVK_USE_PLATFORM_XCB_KHR \
		-DVK_USE_PLATFORM_XLIB_KHR \
		-DVK_USE_PLATFORM_DISPLAY_KHR \
		-DVK_USE_PLATFORM_XLIB_XRANDR_EXT

.if "${MACHINE_ARCH}" == "amd64"
VKARCH=x86_64
.else
VKARCH=${MACHINE_ARCH}
.endif

install: beforeinstall
	${INSTALL} ${INSTALL_COPY} ${INSTALL_STRIP} \
	    -o ${LIBOWN} -g ${LIBGRP} -m ${LIBMODE} \
	    lib${LIB}.so ${DESTDIR}${X11BASE}/lib
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m ${BINMODE} \
	    ${MESA_SRC}/generated/src/amd/vulkan/radeon_icd.json \
	    ${DESTDIR}${X11BASE}/share/vulkan/icd.d/radeon_icd.${VKARCH}.json

obj: _xenocara_obj

beforeinstall:
	rm -f ${DESTDIR}${LIBDIR}/lib${LIB}.la

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

SLIBS=	libamd_common \
	libamd_common_llvm \
	libaddrlib \
	libvulkan_wsi \
	libxmlconfig \
	libmesa_util \
	libmesa_format \
	libmesa_util_sse41 \
	libmesa_util_c11 \
	libaco \
	libnir \
	libcompiler

LDADD+=		-Wl,--as-needed -Wl,--allow-shlib-undefined -Wl,--start-group

LDADD+=		-Wl,--whole-archive
LDADD+=		${.CURDIR}/../libvulkan_util/${__objdir}/libvulkan_util.a
LDADD+=		${.CURDIR}/../libvulkan_runtime/${__objdir}/libvulkan_runtime.a
LDADD+=		-Wl,--no-whole-archive

.for slib in ${SLIBS}
LDADD+=		${.CURDIR}/../${slib}/${__objdir}/${slib}.a
.endfor

LDADD+=		-lelf -lLLVM -lpthread -lexpat -lz -lm
LDADD+=		-L${X11BASE}/lib -ldrm -ldrm_amdgpu \
		-lxcb-dri3 -lxcb-xfixes -lxcb-randr -lXrandr -lxcb -lX11-xcb \
		-lX11 -lxcb-dri2 -lxcb-present -lxcb-xfixes -lxcb-sync \
		-lxcb-randr -lxcb-shm -lxshmfence

LDADD+=		-Wl,--gc-sections \
		-Wl,--version-script ${MESA_SRC}/src/amd/vulkan/vulkan.sym \
		-Wl,--end-group

all:	lib${LIB}.so

lib${LIB}.so: ${SOBJS} ${DPADD}
	${CXX} -shared -Wl,-soname,lib${LIB}.so ${PICFLAG} -o ${.TARGET} \
	    `echo ${SOBJS} | tr ' ' '\n' | sort -R` ${LDADD}

CLEANFILES+= lib${LIB}.so

.PATH: ${MESA_SRC}/src/amd/vulkan
.PATH: ${MESA_SRC}/src/amd/vulkan/layers
.PATH: ${MESA_SRC}/src/amd/vulkan/winsys/amdgpu
.PATH: ${MESA_SRC}/src/amd/vulkan/winsys/null
.PATH: ${MESA_SRC}/src/amd/vulkan/radix_sort
.PATH: ${MESA_SRC}/src/amd/vulkan/radix_sort/common
.PATH: ${MESA_SRC}/src/amd/vulkan/radix_sort/common/vk
.PATH: ${MESA_SRC}/generated/src/amd/vulkan
