# $OpenBSD: bsd.xconf.mk,v 1.34 2016/04/01 03:15:15 jsg Exp $

# Build GL libs and apps?
.if ${MACHINE_ARCH} == "m88k"
XENOCARA_BUILD_GL?=no
XENOCARA_BUILD_DRI?=no
.else
XENOCARA_BUILD_GL?=yes
XENOCARA_BUILD_DRI?=yes
.endif
