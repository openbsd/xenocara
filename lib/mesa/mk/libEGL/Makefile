#	$OpenBSD: Makefile,v 1.10 2023/03/03 06:32:18 jsg Exp $

.include <bsd.xconf.mk>

LIB=	EGL

SRCS=	eglapi.c \
	eglarray.c \
	eglconfigdebug.c \
	eglconfig.c \
	eglcontext.c \
	eglcurrent.c \
	egldevice.c \
	egldisplay.c \
	eglglobals.c \
	eglimage.c \
	egllog.c \
	eglsurface.c \
	eglsync.c 

.if ${XENOCARA_BUILD_DRI:L} == "yes"
SRCS+=	egl_dri2.c \
	platform_device.c \
	platform_drm.c \
	platform_surfaceless.c \
	platform_x11.c
.endif

.if ${XENOCARA_BUILD_DRI3:L} == "yes"
SRCS+=	platform_x11_dri3.c
.endif

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/egl/main \
		-I${MESA_SRC}/src/loader \
		-I${MESA_SRC}/src/gbm/main \
		-I${MESA_SRC}/src/gbm/backends/dri \
		-D_EGL_NATIVE_PLATFORM=_EGL_PLATFORM_X11

LDADD+= -Wl,--as-needed -Wl,--allow-shlib-undefined -Wl,--start-group \
	${.CURDIR}/../libmesa_util/${__objdir}/libmesa_util.a \
	${.CURDIR}/../libmesa_format/${__objdir}/libmesa_format.a \
	${.CURDIR}/../libmesa_util_sse41/${__objdir}/libmesa_util_sse41.a \
	${.CURDIR}/../libmesa_util_c11/${__objdir}/libmesa_util_c11.a \
	-Wl,--gc-sections \
	-lz -lm -lpthread \
	-L${X11BASE}/lib -lX11-xcb -lX11 -lxcb -lxcb-randr -lxcb-xfixes

.if ${XENOCARA_BUILD_DRI:L} == "yes"
LDADD+= ${.CURDIR}/../libloader/${__objdir}/libloader.a \
	${.CURDIR}/../libxmlconfig/${__objdir}/libxmlconfig.a \
	-ldrm -lxcb-dri2 -lexpat
.endif

.if ${XENOCARA_BUILD_DRI3:L} == "yes"
LDADD+= ${.CURDIR}/../libloader_dri3_helper/${__objdir}/libloader_dri3_helper.a \
	-lxcb-dri3 -lxcb-present -lxcb-sync -lxshmfence
.endif

LDADD+= -L${.CURDIR}/../libgbm/${__objdir} -lgbm \
	-L${.CURDIR}/../libglapi/${__objdir} -lglapi \
	-Wl,--end-group

obj: _xenocara_obj

beforeinstall:
	rm -f ${DESTDIR}${LIBDIR}/lib${LIB}.la

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/egl/main
.PATH:	${MESA_SRC}/src/egl/drivers/dri2
