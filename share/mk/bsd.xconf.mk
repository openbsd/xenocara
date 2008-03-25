# $OpenBSD: bsd.xconf.mk,v 1.1 2008/03/25 23:41:50 matthieu Exp $

# Shared libs?
.if ${MACHINE} == landisk || ${MACHINE} == "mvme88k" || ${MACHINE} == "vax"
XENOCARA_HAVE_SHARED_LIBS?=no
.else
XENOCARA_HAVE_SHARED_LIBS?=yes
.endif

# Build DRI?
.if ${MACHINE} == "amd64" || ${MACHINE} == "i386"
XENOCARA_BUILD_DRI?=yes
.else
XENOCARA_BUILD_DRI?=no
.endif

# Build GL libs and apps
.if ${MACHINE} == "landisk" || ${MACHINE_ARCH} == "m88k" || ${MACHINE} == "sgi" || ${MACHINE} == vax
XENOCARA_BUILD_GL?=no
.else
XENOCARA_BUILD_GL?=yes
.endif
