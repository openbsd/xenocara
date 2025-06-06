#	$OpenBSD: Makefile,v 1.14 2025/06/06 09:04:08 jsg Exp $

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
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/frontends/dri \
		-I${MESA_SRC}/src/x11 \
		-I${MESA_SRC}/generated/src \
		-D_EGL_NATIVE_PLATFORM=_EGL_PLATFORM_X11 \
		-DGALLIUM_SOFTPIPE

LDADD+= -Wl,--as-needed -Wl,--allow-shlib-undefined -Wl,--start-group

.if ${XENOCARA_BUILD_DRI:L} == "yes"
LDADD+= ${.CURDIR}/../libloader/${__objdir}/libloader.a \
	${.CURDIR}/../libloader_x11/${__objdir}/libloader_x11.a \
	${.CURDIR}/../libpipe_loader_dynamic/${__objdir}/libpipe_loader_dynamic.a \
	${.CURDIR}/../libxmlconfig/${__objdir}/libxmlconfig.a
.endif

LDADD+= -L${.CURDIR}/../libgbm/${__objdir} -lgbm \
	-L${.CURDIR}/../libglapi/${__objdir} -lglapi \
	${.CURDIR}/../libmesa_util/${__objdir}/libmesa_util.a \
	${.CURDIR}/../libmesa_util_sse41/${__objdir}/libmesa_util_sse41.a \
	${.CURDIR}/../libmesa_util_c11/${__objdir}/libmesa_util_c11.a \
	-Wl,-Bsymbolic -Wl,--gc-sections \
	-lz -lm -lpthread \
	-L${X11BASE}/lib -lX11-xcb -lX11 -lxcb -lxcb-randr \
	-lxcb-xfixes -lxcb-shm

.if ${XENOCARA_BUILD_DRI:L} == "yes"
LDADD+= -ldrm -lxcb-dri2 -lexpat \
	-L${.CURDIR}/../libgallium_dri/${__objdir} -lgallium_dri
.endif

.if ${XENOCARA_BUILD_DRI3:L} == "yes"
LDADD+= -lxcb-dri3 -lxcb-present -lxcb-sync -lxshmfence
.endif

LDADD+= -Wl,--end-group

NOPROFILE=

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/egl/main
.PATH:	${MESA_SRC}/src/egl/drivers/dri2
