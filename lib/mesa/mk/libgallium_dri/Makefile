#	$OpenBSD: Makefile,v 1.8 2023/03/03 06:32:18 jsg Exp $

LIB=	gallium_dri

NOPROFILE=

SRCS=	target.c

GALLIUM_DRIVERS=	swrast kms_swrast

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/src/gallium/frontends/dri \
		-I${MESA_SRC}/src/gallium/winsys \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mesa/drivers/dri/common \
		-I${MESA_SRC}/generated/src/util \
		-I${MESA_SRC}/generated/src/gallium/drivers \
		-DGALLIUM_SOFTPIPE

.if ${WITH_GALLIUM_R300} == "yes"
GALLIUM_DRIVERS+=	r300
CPPFLAGS+=	-DGALLIUM_R300
.endif

.if ${WITH_GALLIUM_R600} == "yes"
GALLIUM_DRIVERS+=	r600
CPPFLAGS+=	-DGALLIUM_R600
.endif

.if ${WITH_GALLIUM_RADEONSI} == "yes"
GALLIUM_DRIVERS+=	radeonsi
CPPFLAGS+=	-DGALLIUM_RADEONSI
.endif

.if ${WITH_GALLIUM_I915} == "yes"
GALLIUM_DRIVERS+=	i915
CPPFLAGS+=	-DGALLIUM_I915
.endif

.if ${WITH_GALLIUM_CROCUS} == "yes"
GALLIUM_DRIVERS+=	crocus
CPPFLAGS+=	-DGALLIUM_CROCUS
.endif

.if ${WITH_GALLIUM_IRIS} == "yes"
GALLIUM_DRIVERS+=	iris
CPPFLAGS+=	-DGALLIUM_IRIS
.endif

.if ${WITH_LLVM} == "yes"
CPPFLAGS+=	-DGALLIUM_LLVMPIPE
.endif

install:
	${INSTALL} ${INSTALL_COPY} ${INSTALL_STRIP} \
	    -o ${LIBOWN} -g ${LIBGRP} -m ${LIBMODE} \
	    lib${LIB}.so ${DESTDIR}${X11BASE}/lib/modules/dri
.for driver in ${GALLIUM_DRIVERS}
	ln -f ${DESTDIR}${X11BASE}/lib/modules/dri/lib${LIB}.so \
	    ${DESTDIR}${X11BASE}/lib/modules/dri/${driver}_dri.so
.endfor
	rm ${DESTDIR}${X11BASE}/lib/modules/dri/lib${LIB}.so

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

SLIBS=	libdri \
	libmesa \
	libglsl \
	libglcpp \
	libmesa_util \
	libmesa_format \
	libmesa_util_sse41 \
	libmesa_util_c11 \
	libnir \
	libcompiler \
	libgalliumvl \
	libgallium \
	libpipe_loader_static \
	libloader \
	libxmlconfig \
	libws_null \
	libwsw \
	libswdri \
	libswkmsdri \
	libsoftpipe

.if ${WITH_SSE41} == "yes"
SLIBS+=	libmesa_sse41
.endif

.if ${WITH_GALLIUM_R300} == "yes" || ${WITH_GALLIUM_R600} == "yes" || \
    ${WITH_GALLIUM_RADEONSI} == "yes"
SLIBS+=	libradeonwinsys
.endif

.if ${WITH_GALLIUM_R300} == "yes"
SLIBS+=	libr300
.endif

.if ${WITH_GALLIUM_R600} == "yes"
SLIBS+=	libr600
.endif

.if ${WITH_GALLIUM_RADEONSI} == "yes"
SLIBS+= libamdgpuwinsys \
	libaddrlib \
	libamd_common \
	libamd_common_llvm \
	libradeonsi_gfx6 \
	libradeonsi_gfx7 \
	libradeonsi_gfx8 \
	libradeonsi_gfx9 \
	libradeonsi_gfx10 \
	libradeonsi_gfx103 \
	libradeonsi_gfx11 \
	libradeonsi
.endif

.if ${WITH_GALLIUM_I915} == "yes"
SLIBS+=	libi915 \
	libi915drm
.endif

.if ${WITH_GALLIUM_CROCUS} == "yes"
SLIBS+=	libcrocus_per_hw_ver40 \
	libcrocus_per_hw_ver45 \
	libcrocus_per_hw_ver50 \
	libcrocus_per_hw_ver60 \
	libcrocus_per_hw_ver70 \
	libcrocus_per_hw_ver75 \
	libcrocus_per_hw_ver80 \
	libcrocus \
	libcrocuswinsys
.endif

.if ${WITH_GALLIUM_IRIS} == "yes"
SLIBS+= libiris_gen80 \
	libiris_gen90 \
	libiris_gen110 \
	libiris_gen120 \
	libiris_gen125 \
	libintel_compiler \
	libintel_dev \
	libisl \
	libisl_gfx40 \
	libisl_gfx50 \
	libisl_gfx60 \
	libisl_gfx70 \
	libisl_gfx75 \
	libisl_gfx80 \
	libisl_gfx90 \
	libisl_gfx110 \
	libisl_gfx120 \
	libisl_gfx125 \
	libisl_tiled_memcpy \
	libisl_tiled_memcpy_sse41 \
	libblorp \
	libintel_perf \
	libintel_common \
	libintel-driver-ds \
	libiris \
	libiriswinsys
.endif

.if ${WITH_LLVM} == "yes"
SLIBS+=	libllvmpipe
.endif

LDADD+=		-Wl,--as-needed -Wl,--allow-shlib-undefined -Wl,--start-group
.for slib in ${SLIBS}
LDADD+=		${.CURDIR}/../${slib}/${__objdir}/${slib}.a
.endfor

LDADD+=		-L${.CURDIR}/../libglapi/${__objdir} -lglapi
LDADD+=		-lelf -lLLVM -lexpat -lz -lm -lpthread
LDADD+=		-L${X11BASE}/lib -ldrm

.if ${WITH_GALLIUM_I915} == "yes"
LDADD+=		-ldrm_intel
.endif

.if ${WITH_GALLIUM_R300} == "yes" || ${WITH_GALLIUM_R600} == "yes"
LDADD+=		-ldrm_radeon
.endif

.if ${WITH_GALLIUM_RADEONSI} == "yes"
LDADD+=		-ldrm_amdgpu
.endif

LDADD+=		${BUILD_ID_SHA1} -Wl,--gc-sections \
		-Wl,--version-script ${MESA_SRC}/src/gallium/targets/dri/dri.sym
.if ${WITH_LD_DYNAMIC_LIST} == "yes"
LDADD+=		-Wl,--dynamic-list ${MESA_SRC}/src/gallium/targets/dri-vdpau.dyn
.endif
LDADD+=		-Wl,--end-group

all:	lib${LIB}.so

lib${LIB}.so: ${SOBJS} ${DPADD}
	${CXX} -shared -Wl,-soname,lib${LIB}.so ${PICFLAG} -o ${.TARGET} \
	    `echo ${SOBJS} | tr ' ' '\n' | sort -R` ${LDADD}

CLEANFILES+= lib${LIB}.so

.PATH: ${MESA_SRC}/src/gallium/targets/dri
