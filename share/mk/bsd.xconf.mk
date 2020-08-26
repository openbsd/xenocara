# $OpenBSD: bsd.xconf.mk,v 1.36 2020/08/26 06:25:26 jsg Exp $

# Build GL libs and apps?
.if ${MACHINE_ARCH} == "alpha" || ${MACHINE_ARCH} == "hppa" || \
    ${MACHINE_ARCH} == "m88k" || ${MACHINE_ARCH} == "sh"
XENOCARA_BUILD_GL?=no
XENOCARA_BUILD_DRI?=no
.else
XENOCARA_BUILD_GL?=yes
XENOCARA_BUILD_DRI?=yes
.endif

# Build DRI3 support?
.if ${XENOCARA_BUILD_DRI:L} == "no" || ${MACHINE_ARCH} == "hppa" || \
    ${MACHINE_ARCH} == "sh"
XENOCARA_BUILD_DRI3?=no
.else
XENOCARA_BUILD_DRI3?=yes
.endif
