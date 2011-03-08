# $OpenBSD: bsd.xconf.mk,v 1.18 2011/03/08 20:48:59 matthieu Exp $

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
.if ${MACHINE} == "vax"
XENOCARA_BUILD_PIXMAN?=no
.else
XENOCARA_BUILD_PIXMAN?=yes
.endif

# Build xkeyboard-config?
XENOCARA_USE_XKEYBOARD_CONFIG?=no
