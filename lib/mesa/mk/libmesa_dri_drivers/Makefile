#	$OpenBSD: Makefile,v 1.2 2021/07/22 11:10:12 jsg Exp $

.include "../Makefile.inc"

LIB=	mesa_dri_drivers

NOPROFILE=

SRCS=

DRI_DRIVERS=

.if ${WITH_DRI_I915} == "yes"
DRI_DRIVERS+=	i915
.endif

.if ${WITH_DRI_I965} == "yes"
DRI_DRIVERS+=	i965
.endif

.if ${WITH_DRI_R100} == "yes"
DRI_DRIVERS+=	r100
.endif

.if ${WITH_DRI_R200} == "yes"
DRI_DRIVERS+=	r200
.endif

CPPFLAGS+=

all:	lib${LIB}.so

.include <bsd.dep.mk>

SLIBS=	libmegadriver_stub \
	libdricommon \
	libmesa_classic \
	libmesa_common \
	libglsl \
	libglcpp \
	libmesa_util \
	libmesa_format \
	libnir \
	libcompiler \
	libxmlconfig

.if ${WITH_SSE41} == "yes"
SLIBS+=	libmesa_sse41
.endif

.if ${WITH_DRI_I915} == "yes"
SLIBS+=	libi915
.endif

.if ${WITH_DRI_I965} == "yes"
SLIBS+= libi965_gen40 \
	libi965_gen45 \
	libi965_gen50 \
	libi965_gen60 \
	libi965_gen70 \
	libi965_gen75 \
	libi965_gen80 \
	libi965_gen90 \
	libi965_gen110 \
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
	libintel_compiler \
	libblorp \
	libintel_perf \
	libintel_common \
	libi965
.endif

.if ${WITH_DRI_R100} == "yes"
SLIBS+=	libr100
.endif

.if ${WITH_DRI_R200} == "yes"
SLIBS+=	libr200
.endif

LDADD+=		-Wl,--as-needed -Wl,--allow-shlib-undefined -Wl,--start-group

LDADD+=		-Wl,--whole-archive
.for driver in ${DRI_DRIVERS}
LDADD+=		${.CURDIR}/../lib${driver}/${__objdir}/lib${driver}.a
.endfor
LDADD+=		-Wl,--no-whole-archive

.for slib in ${SLIBS}
LDADD+=		${.CURDIR}/../${slib}/${__objdir}/${slib}.a
.endfor

LDADD+=		-L${.CURDIR}/../libglapi/${__objdir} -lglapi
LDADD+=		-lexpat -lz -lm
LDADD+=		-L${X11BASE}/lib -ldrm

.if ${WITH_DRI_I915} == "yes" || ${WITH_DRI_I965} == "yes"
LDADD+=		-ldrm_intel
.endif

.if ${WITH_DRI_R100} == "yes" || ${WITH_DRI_R200} == "yes"
LDADD+=		-ldrm_radeon
.endif

LDADD+=		${BUILD_ID_SHA1} -Wl,-Bsymbolic -Wl,--gc-sections \
		-Wl,--end-group

lib${LIB}.so: ${DPADD}
	${CXX} -shared -Wl,-soname,lib${LIB}.so ${PICFLAG} -o ${.TARGET} \
	    ${LDADD}

install: _SUBDIRUSE
	${INSTALL} ${INSTALL_COPY} ${INSTALL_STRIP} \
	    -o ${LIBOWN} -g ${LIBGRP} -m ${LIBMODE} \
	    lib${LIB}.so ${DESTDIR}${X11BASE}/lib/modules/dri
.for driver in ${DRI_DRIVERS}
.if "${driver}" == "r100"
	ln -f ${DESTDIR}${X11BASE}/lib/modules/dri/lib${LIB}.so \
	    ${DESTDIR}${X11BASE}/lib/modules/dri/radeon_dri.so
.else
	ln -f ${DESTDIR}${X11BASE}/lib/modules/dri/lib${LIB}.so \
	    ${DESTDIR}${X11BASE}/lib/modules/dri/${driver}_dri.so
.endif
.endfor
	rm ${DESTDIR}${X11BASE}/lib/modules/dri/lib${LIB}.so

clean: _SUBDIRUSE
	rm -f lib${LIB}.so

.include <bsd.xorg.mk>
