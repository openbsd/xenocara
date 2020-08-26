#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:37 jsg Exp $

LIB=	gallium_dri

NOPROFILE=

SRCS=	target.c

GALLIUM_DRIVERS=	swrast kms_swrast

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/state_trackers/dri \
		-I${MESA_SRC}/src/gallium/drivers \
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
	libmesa_gallium \
	libmesa_common \
	libglsl \
	libglcpp \
	libmesa_util \
	libmesa_format \
	libnir \
	libcompiler \
	libdricommon \
	libmegadriver_stub \
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
	libradeonsi
.endif

.if ${WITH_GALLIUM_IRIS} == "yes"
SLIBS+= libiris_gen80 \
	libiris_gen90 \
	libiris_gen100 \
	libiris_gen110 \
	libiris_gen120 \
	libintel_compiler \
	libintel_dev \
	libisl \
	libisl_gen40 \
	libisl_gen50 \
	libisl_gen60 \
	libisl_gen70 \
	libisl_gen75 \
	libisl_gen80 \
	libisl_gen90 \
	libisl_gen100 \
	libisl_gen110 \
	libisl_gen120 \
	libisl_tiled_memcpy \
	libisl_tiled_memcpy_sse41 \
	libblorp \
	libintel_perf \
	libintel_common \
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
LDADD+=		-lelf -lLLVM -lexpat -lz -lm
LDADD+=		-L${X11BASE}/lib -ldrm

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
