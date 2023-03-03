#	$OpenBSD: Makefile,v 1.6 2023/03/03 06:32:18 jsg Exp $

.include "config.mk"

SUBDIR=		include pkgconfig \
		libcompiler libdri \
		libgallium libgalliumvl \
		libglapi_static libglcpp libglsl \
		libloader \
		libmesa libmesa_format \
		libmesa_util libmesa_util_c11 \
		libnir libpipe_loader_static libsoftpipe \
		libmesa_util_sse41 \
		libswdri libswkmsdri \
		libws_null libwsw libxmlconfig

.if ${XENOCARA_BUILD_DRI3:L} == "yes" || make(obj)
SUBDIR+=	libloader_dri3_helper
.endif

.if ${WITH_SSE41} == "yes" || make(obj)
SUBDIR+=	libmesa_sse41
.endif

.if ${WITH_AMD_VK} == "yes" || ${WITH_GALLIUM_RADEONSI} == "yes" || make(obj)
SUBDIR+=	libaddrlib libamd_common libamd_common_llvm libamdgpuwinsys
.endif

.if ${WITH_GALLIUM_CROCUS} == "yes" || ${WITH_GALLIUM_IRIS} == "yes" || \
    ${WITH_INTEL_VK} == "yes" || make(obj)
SUBDIR+=	libblorp libintel_common libintel_compiler libintel_dev \
		libintel-driver-ds \
		libintel_perf libisl libisl_tiled_memcpy \
		libisl_tiled_memcpy_sse41
.for GFX in 40 50 60 70 75 80 90 110 120 125
SUBDIR+=	libisl_gfx${GFX}
.endfor
.endif

.if ${WITH_GALLIUM_R300} == "yes" || ${WITH_GALLIUM_R600} == "yes" || \
    ${WITH_GALLIUM_RADEONSI} == "yes" || make(obj)
SUBDIR+=	libradeonwinsys
.endif

.if ${WITH_AMD_VK} == "yes" || ${WITH_INTEL_VK} == "yes" || make(obj)
SUBDIR+=	libvulkan_runtime libvulkan_util libvulkan_wsi
.endif

.if ${WITH_LLVM} == "yes" || make(obj)
SUBDIR+=	libllvmpipe
.endif

.if ${WITH_AMD_VK} == "yes" || make(obj)
SUBDIR+=	libaco
SUBDIR+=	libvulkan_radeon
.endif

.if ${WITH_INTEL_VK} == "yes" || make(obj)
SUBDIR+=	libanv_common  libanv_hasvk_common
.for GEN in 70 75 80 90 110 120 125
SUBDIR+=	libanv_gen${GEN}
.endfor
SUBDIR+=	libvulkan_intel libvulkan_intel_hasvk
.endif

.if ${WITH_GALLIUM_I915} == "yes" || make(obj)
SUBDIR+=	libi915 libi915drm
.endif

.if ${WITH_GALLIUM_CROCUS} == "yes" || make(obj)
SUBDIR+=	libcrocus libcrocuswinsys
.for GEN in 40 45 50 60 70 75 80
SUBDIR+=	libcrocus_per_hw_ver${GEN}
.endfor
.endif

.if ${WITH_GALLIUM_IRIS} == "yes" || make(obj)
SUBDIR+=	libiris libiriswinsys
.for GEN in 80 90 110 120 125
SUBDIR+=	libiris_gen${GEN}
.endfor
.endif

.if ${WITH_GALLIUM_R300} == "yes" || make(obj)
SUBDIR+=	libr300
.endif

.if ${WITH_GALLIUM_R600} == "yes" || make(obj)
SUBDIR+=	libr600
.endif

.if ${WITH_GALLIUM_RADEONSI} == "yes" || make(obj)
.for GEN in 6 7 8 9 10 103 11
SUBDIR+=	libradeonsi_gfx${GEN}
.endfor
SUBDIR+=	libradeonsi
.endif

# installed libs
SUBDIR+=	libglapi libgbm libEGL libGL libGLESv1_CM libGLESv2 libOSMesa

SUBDIR+=	libgallium_dri

.include <bsd.subdir.mk>
