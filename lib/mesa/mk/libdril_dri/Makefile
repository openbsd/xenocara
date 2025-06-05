#	$OpenBSD: Makefile,v 1.1 2025/06/05 14:18:41 jsg Exp $

LIB=	dril_dri

NOPROFILE=

SRCS=	dril_target.c

GALLIUM_DRIVERS=	swrast kms_swrast

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/generated/src

.if ${WITH_GALLIUM_R300} == "yes"
GALLIUM_DRIVERS+=	r300
.endif

.if ${WITH_GALLIUM_R600} == "yes"
GALLIUM_DRIVERS+=	r600
.endif

.if ${WITH_GALLIUM_RADEONSI} == "yes"
GALLIUM_DRIVERS+=	radeonsi
.endif

.if ${WITH_GALLIUM_I915} == "yes"
GALLIUM_DRIVERS+=	i915
.endif

.if ${WITH_GALLIUM_CROCUS} == "yes"
GALLIUM_DRIVERS+=	crocus
.endif

.if ${WITH_GALLIUM_IRIS} == "yes"
GALLIUM_DRIVERS+=	iris
.endif

install:
	${INSTALL} ${INSTALL_COPY} ${INSTALL_STRIP} \
	    -o ${LIBOWN} -g ${LIBGRP} -m ${LIBMODE} \
	    lib${LIB}.so ${DESTDIR}${X11BASE}/lib/modules/dri
.for driver in ${GALLIUM_DRIVERS}
	ln -f ${DESTDIR}${X11BASE}/lib/modules/dri/lib${LIB}.so \
	    ${DESTDIR}${X11BASE}/lib/modules/dri/${driver}_dri.so
.endfor
#	rm ${DESTDIR}${X11BASE}/lib/modules/dri/lib${LIB}.so

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

SLIBS=	libmesa_util \
	libmesa_util_sse41 \
	libblake3 \
	libmesa_util_c11 \
	libnir \
	libcompiler \
	libgallium

LDADD+=		-Wl,--as-needed -Wl,--allow-shlib-undefined -Wl,--start-group

.for slib in ${SLIBS}
LDADD+=		${.CURDIR}/../${slib}/${__objdir}/${slib}.a
.endfor

LDADD+=		-L${.CURDIR}/../libglapi/${__objdir}
LDADD+=		-lelf -lLLVM -lexpat -lz -lm -lpthread
LDADD+=		-L${X11BASE}/lib -ldrm \
		-L${.CURDIR}/../libgbm/${__objdir} -lgbm

LDADD+=		${BUILD_ID_SHA1} -Wl,--gc-sections \
		-Wl,--version-script ${MESA_SRC}/generated/src/gallium/targets/dril/dri.sym
.if ${WITH_LD_DYNAMIC_LIST} == "yes"
LDADD+=		-Wl,--dynamic-list ${MESA_SRC}/src/gallium/targets/dri.dyn
.endif
LDADD+=		-Wl,--end-group

all:	lib${LIB}.so

lib${LIB}.so: ${SOBJS} ${DPADD}
	${CXX} -shared -Wl,-soname,lib${LIB}.so ${PICFLAG} -o ${.TARGET} \
	    `echo ${SOBJS} | tr ' ' '\n' | sort -R` ${LDADD}

CLEANFILES+= lib${LIB}.so

.PATH: ${MESA_SRC}/src/gallium/targets/dril
