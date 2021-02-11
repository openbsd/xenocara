#	$OpenBSD: Makefile.inc,v 1.1 2021/02/11 10:34:51 jsg Exp $

# bsd.lib.mk and bsd.xorg.mk both include Makefile.inc
.if !defined(DRM_MK_INC)

DRM_SRC?=	${.CURDIR}/../..

CPPFLAGS+=	-I${DRM_SRC} \
		-I${DRM_SRC}/include/drm

.include "config.mk"

.endif
DRM_MK_INC=Done
