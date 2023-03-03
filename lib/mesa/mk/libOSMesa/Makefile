#	$OpenBSD: Makefile,v 1.8 2023/03/03 06:32:18 jsg Exp $

LIB=	OSMesa

SRCS=	target.c \
	osmesa.c	# libosmesa_st

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/src/gallium/winsys \
		-I${MESA_SRC}/generated/src/mapi/glapi \
		-DGALLIUM_SOFTPIPE -DGALLIUM_LLVMPIPE

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

SLIBS=	libmesa \
	libmesa_util \
	libmesa_format \
	libmesa_util_sse41 \
	libmesa_util_c11 \
	libglsl \
	libglcpp \
	libnir \
	libcompiler \
	libgallium \
	libws_null \
	libsoftpipe

.if ${WITH_SSE41} == "yes"
SLIBS+=	libmesa_sse41
.endif

.if ${WITH_LLVM} == "yes"
SLIBS+= libllvmpipe
.endif

LDADD+=		-Wl,--as-needed -Wl,--allow-shlib-undefined -Wl,--start-group

LDADD+=		-Wl,--whole-archive
LDADD+=		${.CURDIR}/../libglapi_static/${__objdir}/libglapi_static.a
LDADD+=		-Wl,--no-whole-archive

.for slib in ${SLIBS}
LDADD+=		${.CURDIR}/../${slib}/${__objdir}/${slib}.a
.endfor

LDADD+= -L${.CURDIR}/../libglapi/${__objdir} -lglapi

LDADD+=	-Wl,--gc-sections
LDADD+=	-Wl,--version-script ${MESA_SRC}/src/gallium/targets/osmesa/osmesa.sym
LDADD+=	-lLLVM -lm -lz -lpthread
LDADD+=	-L${X11BASE}/lib -ldrm
LDADD+=	-Wl,--end-group

beforeinstall:
	rm -f ${DESTDIR}${LIBDIR}/lib${LIB}.la

.PATH:	${MESA_SRC}/src/gallium/targets/osmesa
.PATH:	${MESA_SRC}/src/gallium/frontends/osmesa
