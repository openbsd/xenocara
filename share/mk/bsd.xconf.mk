# $OpenBSD: bsd.xconf.mk,v 1.9 2009/05/22 17:58:45 matthieu Exp $

# Shared libs?
.if ${MACHINE} == "mvme88k" || ${MACHINE} == "vax"
XENOCARA_HAVE_SHARED_LIBS?=no
.else
XENOCARA_HAVE_SHARED_LIBS?=yes
.endif

# Build GL libs and apps
.if ${MACHINE_ARCH} == "m88k" || ${MACHINE} == vax
XENOCARA_BUILD_GL?=no
XENOCARA_BUILD_DRI?=no
.else
XENOCARA_BUILD_GL?=yes
XENOCARA_BUILD_DRI?=yes
.endif

# Build XCB
XENOCARA_BUILD_XCB?=no
