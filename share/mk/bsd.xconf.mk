# $OpenBSD: bsd.xconf.mk,v 1.22 2013/02/09 21:48:07 brad Exp $

# Shared libs?
.if ${MACHINE} == "vax"
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
