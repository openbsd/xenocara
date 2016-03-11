# $OpenBSD: bsd.xconf.mk,v 1.33 2016/03/11 13:09:42 okan Exp $

# Build GL libs and apps?
.if ${MACHINE_ARCH} == "m88k"
XENOCARA_BUILD_GL?=no
XENOCARA_BUILD_DRI?=no
.else
XENOCARA_BUILD_GL?=yes
XENOCARA_BUILD_DRI?=yes
.endif

# Build pixman?
XENOCARA_BUILD_PIXMAN?=yes
