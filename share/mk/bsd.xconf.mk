# $OpenBSD: bsd.xconf.mk,v 1.21 2012/03/08 07:29:29 matthieu Exp $

# Shared libs?
.if ${MACHINE_ARCH} == "m88k" || ${MACHINE} == "vax"
XENOCARA_HAVE_SHARED_LIBS?=no
.else
XENOCARA_HAVE_SHARED_LIBS?=yes
.endif

# Build GL libs and apps?
XENOCARA_BUILD_GL?=${XENOCARA_HAVE_SHARED_LIBS:L}
XENOCARA_BUILD_DRI?=${XENOCARA_HAVE_SHARED_LIBS:L}

# Build pixman?
.if ${MACHINE} == "vax" || ${MACHINE} == "landisk"
XENOCARA_BUILD_PIXMAN?=no
.else
XENOCARA_BUILD_PIXMAN?=yes
.endif
