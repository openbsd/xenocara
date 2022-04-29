#	$OpenBSD: Makefile.inc,v 1.3 2022/04/29 10:16:21 jsg Exp $

# bsd.lib.mk and bsd.xorg.mk both include Makefile.inc
.if !defined(DRM_MK_INC)

DRM_SRC?=	${.CURDIR}/../..

CPPFLAGS+=	-I${DRM_SRC} \
		-I${DRM_SRC}/include/drm \
		-I${DRM_SRC}/generated

.include "config.mk"

.endif
DRM_MK_INC=Done
