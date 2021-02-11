#	$OpenBSD: Makefile,v 1.1 2021/02/11 10:34:51 jsg Exp $

.include "config.mk"

SUBDIR=		include man pkgconfig \
		libdrm libdrm_amdgpu libdrm_radeon

.if ${WITH_INTEL} == "yes" || make(obj)
SUBDIR+=	libdrm_intel
.endif

.include <bsd.subdir.mk>
