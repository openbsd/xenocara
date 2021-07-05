MDOCS=	drmAvailable 3 drmHandleEvent 3 drmModeGetResources 3 \
	drm 7 drm-kms 7 drm-memory 7

beforeinstall:
.for n s in ${MDOCS}
	${INSTALL} ${INSTALL_COPY} -o ${MANOWN} -g ${MANGRP} -m ${MANMODE} \
		${.CURDIR}/${n}.${s} ${DESTDIR}${MANDIR}${s}
.endfor

# maintainer target, not used duing build or install
mdoc:
.for n s in ${MDOCS}
	rst2man ${DRM_SRC}/man/${n}.${s}.rst> ${.CURDIR}/${n}.${s}
.endfor

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>

.PHONY: mdoc
