#	$OpenBSD: Makefile,v 1.1 2021/02/11 10:34:51 jsg Exp $

.include "../Makefile.inc"

BASE_FILES=	libsync.h \
		xf86drm.h \
		xf86drmMode.h
DRM_FILES=	amdgpu_drm.h \
		drm.h \
		drm_fourcc.h \
		drm_mode.h \
		drm_sarea.h \
		i915_drm.h \
		mach64_drm.h \
		mga_drm.h \
		msm_drm.h \
		nouveau_drm.h \
		qxl_drm.h \
		r128_drm.h \
		radeon_drm.h \
		savage_drm.h \
		sis_drm.h \
		tegra_drm.h \
		vc4_drm.h \
		via_drm.h \
		virtgpu_drm.h \
		vmwgfx_drm.h
RADEON_FILES=	r600_pci_ids.h \
		radeon_bo.h \
		radeon_bo_gem.h \
		radeon_bo_int.h \
		radeon_cs.h \
		radeon_cs_gem.h \
		radeon_cs_int.h \
		radeon_surface.h
INTEL_FILES=	intel_aub.h \
		intel_bufmgr.h \
		intel_debug.h
AMDGPU_FILES=	amdgpu.h

includes:
	cd ${DRM_SRC} && for i in ${BASE_FILES}; do \
	    cmp -s $$i ${DESTDIR}${X11BASE}/include/$$i || \
	    ${INSTALL} ${INSTALL_COPY} -m 444 $$i ${DESTDIR}${X11BASE}/include/$$i; \
	done
	cd ${DRM_SRC}/include/drm && for i in ${DRM_FILES}; do \
	    cmp -s $$i ${DESTDIR}${X11BASE}/include/libdrm/$$i || \
	    ${INSTALL} ${INSTALL_COPY} -m 444 $$i ${DESTDIR}${X11BASE}/include/libdrm/$$i; \
	done
	cd ${DRM_SRC}/radeon && for i in ${RADEON_FILES}; do \
	    cmp -s $$i ${DESTDIR}${X11BASE}/include/libdrm/$$i || \
	    ${INSTALL} ${INSTALL_COPY} -m 444 $$i ${DESTDIR}${X11BASE}/include/libdrm/$$i; \
	done
	cd ${DRM_SRC}/amdgpu && for i in ${AMDGPU_FILES}; do \
	    cmp -s $$i ${DESTDIR}${X11BASE}/include/libdrm/$$i || \
	    ${INSTALL} ${INSTALL_COPY} -m 444 $$i ${DESTDIR}${X11BASE}/include/libdrm/$$i; \
	done
.if ${WITH_INTEL} == "yes"
	cd ${DRM_SRC}/intel && for i in ${INTEL_FILES}; do \
	    cmp -s $$i ${DESTDIR}${X11BASE}/include/libdrm/$$i || \
	    ${INSTALL} ${INSTALL_COPY} -m 444 $$i ${DESTDIR}${X11BASE}/include/libdrm/$$i; \
	done
.endif

beforeinstall: includes

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
