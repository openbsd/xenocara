# $OpenBSD: bsd.xconf.mk,v 1.8 2008/11/02 15:20:26 matthieu Exp $

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
