#	$OpenBSD: Makefile,v 1.3 2023/01/28 08:56:33 jsg Exp $

.include "../Makefile.inc"

KHR_FILES=	khrplatform.h
GLES_FILES=	egl.h \
		gl.h \
		glext.h \
		glplatform.h
GLES2_FILES=	gl2.h \
		gl2ext.h \
		gl2platform.h
GLES3_FILES=	gl3.h \
		gl31.h \
		gl32.h \
		gl3ext.h \
		gl3platform.h
GL_FILES=	gl.h \
		glcorearb.h \
		glext.h \
		glx.h \
		glxext.h \
		osmesa.h \
		internal/dri_interface.h
EGL_FILES=	egl.h \
		eglext.h \
		eglplatform.h \
		eglmesaext.h \
		eglext_angle.h
VULKAN_FILES=
GBM_FILES=	gbm.h

includes:
	cd ${MESA_SRC}/include/KHR && for i in ${KHR_FILES}; do \
	    cmp -s $$i ${DESTDIR}${X11BASE}/include/KHR/$$i || \
	    ${INSTALL} ${INSTALL_COPY} -m 444 $$i ${DESTDIR}${X11BASE}/include/KHR/$$i; \
	done
	cd ${MESA_SRC}/include/GLES && for i in ${GLES_FILES}; do \
	    cmp -s $$i ${DESTDIR}${X11BASE}/include/GLES/$$i || \
	    ${INSTALL} ${INSTALL_COPY} -m 444 $$i ${DESTDIR}${X11BASE}/include/GLES/$$i; \
	done
	cd ${MESA_SRC}/include/GLES2 && for i in ${GLES2_FILES}; do \
	    cmp -s $$i ${DESTDIR}${X11BASE}/include/GLES2/$$i || \
	    ${INSTALL} ${INSTALL_COPY} -m 444 $$i ${DESTDIR}${X11BASE}/include/GLES2/$$i; \
	done
	cd ${MESA_SRC}/include/GLES3 && for i in ${GLES3_FILES}; do \
	    cmp -s $$i ${DESTDIR}${X11BASE}/include/GLES3/$$i || \
	    ${INSTALL} ${INSTALL_COPY} -m 444 $$i ${DESTDIR}${X11BASE}/include/GLES3/$$i; \
	done
	cd ${MESA_SRC}/include/GL && for i in ${GL_FILES}; do \
	    cmp -s $$i ${DESTDIR}${X11BASE}/include/GL/$$i || \
	    ${INSTALL} ${INSTALL_COPY} -m 444 $$i ${DESTDIR}${X11BASE}/include/GL/$$i; \
	done
	cd ${MESA_SRC}/include/EGL && for i in ${EGL_FILES}; do \
	    cmp -s $$i ${DESTDIR}${X11BASE}/include/EGL/$$i || \
	    ${INSTALL} ${INSTALL_COPY} -m 444 $$i ${DESTDIR}${X11BASE}/include/EGL/$$i; \
	done
.if ${WITH_INTEL_VK} == "yes"
	cd ${MESA_SRC}/include/vulkan && for i in ${VULKAN_FILES}; do \
	    cmp -s $$i ${DESTDIR}${X11BASE}/include/vulkan/$$i || \
	    ${INSTALL} ${INSTALL_COPY} -m 444 $$i ${DESTDIR}${X11BASE}/include/vulkan/$$i; \
	done
.endif
	cd ${MESA_SRC}/src/gbm/main && for i in ${GBM_FILES}; do \
	    cmp -s $$i ${DESTDIR}${X11BASE}/include/$$i || \
	    ${INSTALL} ${INSTALL_COPY} -m 444 $$i ${DESTDIR}${X11BASE}/include/$$i; \
	done

beforeinstall: includes

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
