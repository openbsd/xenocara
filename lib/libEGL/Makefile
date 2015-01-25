# $OpenBSD: Makefile,v 1.11 2015/01/25 15:09:12 jsg Exp $
.include <bsd.xconf.mk>

LIB=	EGL

MESA=	${.CURDIR}/../../dist/Mesa/src/mesa
EGL=	${.CURDIR}/../../dist/Mesa/src/egl
GBM=	${.CURDIR}/../../dist/Mesa/src/gbm
LOADER=	${.CURDIR}/../../dist/Mesa/src/loader
MESA_INCLUDE=	${.CURDIR}/../../dist/Mesa/include

MESA_MAJOR=	10
MESA_MINOR=	4
MESA_TINY=	3

DRI_DRIVER_INSTALL_DIR=	${X11BASE}/lib/modules/dri
EGL_DRIVER_SEARCH_DIR=	${X11BASE}/lib/modules/egl


INCSDIR=	${X11BASE}/include/EGL
KHRINCSDIR=	${X11BASE}/include/KHR

INCS= \
	egl.h \
	eglext.h \
	eglextchromium.h \
	eglmesaext.h \
	eglplatform.h

KHRINCS= \
	khrplatform.h

EGL_SOURCES= \
	eglapi.c \
	eglarray.c \
	eglconfig.c \
	eglcontext.c \
	eglcurrent.c \
	egldisplay.c \
	egldriver.c \
	eglfallbacks.c \
	eglglobals.c \
	eglimage.c \
	egllog.c \
	eglmisc.c \
	eglmode.c \
	eglscreen.c \
	eglstring.c \
	eglsurface.c \
	eglsync.c \
	eglcompiler.h \
	eglconfig.h \
	eglcontext.h \
	eglcurrent.h \
	egldefines.h \
	egldisplay.h \
	egldriver.h \
	eglglobals.h \
	eglimage.h \
	egllog.h \
	eglmisc.h \
	eglmode.h \
	eglmutex.h \
	eglscreen.h \
	eglstring.h \
	eglsurface.h \
	eglsync.h

DRI2_SOURCES= \
	egl_dri2.c \
	platform_x11.c

DRM_SOURCES= \
	platform_drm.c

LOADER_SOURCES= \
	loader.c \
	pci_id_driver_map.c

CFLAGS+= -std=c99 -fvisibility=hidden

CPPFLAGS+= \
	-I${MESA_INCLUDE} \
	-I${EGL}/main \
	-I${LOADER} \
	-I${X11BASE}/include \
	-I/usr/include/dev/pci/drm \
	-DHAVE_PTHREAD \
	-DHAVE_X11_PLATFORM \
	-D_EGL_NATIVE_PLATFORM=_EGL_PLATFORM_X11 \
	-D_EGL_BUILT_IN_DRIVER_GLX \
	-D_EGL_OS_UNIX=1 \
	-DHAVE_SHARED_GLAPI \
	-D_EGL_DRIVER_SEARCH_DIR=\"${EGL_DRIVER_SEARCH_DIR}\" \
	-DDEFAULT_DRIVER_DIR=\"${DRI_DRIVER_INSTALL_DIR}\"

SRCS= \
	${EGL_SOURCES} ${LOADER_SOURCES}

.if ${XENOCARA_BUILD_DRI:L} == "yes"
SRCS+=	${DRI2_SOURCES} ${DRM_SOURCES}
CPPFLAGS+=	-D_EGL_BUILT_IN_DRIVER_DRI2 -DHAVE_XCB_DRI2 -DHAVE_LIBDRM \
		-I${GBM}/main -I${GBM}/backends/dri -DHAVE_DRM_PLATFORM
LDADD=	-L${X11BASE}/lib -lX11-xcb -lX11 -lxcb-dri2 -lxcb-xfixes -lxcb-render \
	-lxcb-shape -lxcb -lgbm -lglapi -lpthread -ldrm
.else
LDADD=	-L${X11BASE}/lib -lX11-xcb -lX11 -lxcb-xfixes -lxcb-render \
	-lxcb-shape -lxcb -lglapi -lpthread
.endif

includes: _SUBDIRUSE
	cd ${MESA_INCLUDE}/EGL; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/EGL/$$i || \
		${INSTALL_DATA}	$$i ${DESTDIR}${INCSDIR}/EGL"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done
	cd ${MESA_INCLUDE}/KHR; for i in ${KHRINCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/KHR/$$i || \
		${INSTALL_DATA}	$$i ${DESTDIR}${INCSDIR}/KHR"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

NOPROFILE=

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

egl.pc: ${EGL}/main/egl.pc.in Makefile
	sed -e 's,@prefix@,$(X11BASE),' \
	    -e 's,@libdir@,${LIBDIR},' \
	    -e 's,@includedir@,${INCSDIR},' \
	    -e 's,@GL_PC_REQ_PRIV@,libdrm >= 2.4.38 x11 xext xdamage xfixes x11-xcb xcb-glx >= 1.8.1 xcb-dri2 >= 1.8 xxf86vm,' \
	    -e 's,@GL_PC_LIB_PRIV@,-lm -lpthread,' \
	    -e 's,@GL_PC_CFLAGS@,,' \
	    -e 's,@GL_LIB@,GL,' \
	    -e 's,@PACKAGE_VERSION@,$(MESA_MAJOR).$(MESA_MINOR).$(MESA_TINY),' \
	    -e 's,@GLX_TLS@,no,' \
	< ${EGL}/main/egl.pc.in > egl.pc

afterinstall: egl.pc
	$(INSTALL) -c -m 644 -o root -g wheel egl.pc \
		${DESTDIR}${LIBDIR}/pkgconfig

CLEANFILES+= egl.pc

.PATH: ${EGL}
.PATH: ${EGL}/main
.PATH: ${EGL}/drivers/dri2
.PATH: ${EGL}/drivers/glx
.PATH: ${LOADER}
