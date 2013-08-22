# $OpenBSD: bsd.xconf.mk,v 1.27 2013/08/22 19:26:51 miod Exp $

# Shared libs?
.if ${MACHINE} == "vax"
XENOCARA_HAVE_SHARED_LIBS?=no
.else
XENOCARA_HAVE_SHARED_LIBS?=yes
.endif

# Build GL libs and apps?
.if ${MACHINE} == "octeon"
XENOCARA_BUILD_GL?=no
XENOCARA_BUILD_DRI?=no
.else
XENOCARA_BUILD_GL?=${XENOCARA_HAVE_SHARED_LIBS:L}
XENOCARA_BUILD_DRI?=${XENOCARA_HAVE_SHARED_LIBS:L}
.endif
# Gallium requires gcc 4 __sync_and_fetch() support
.if ${MACHINE_ARCH} == "arm" || ${MACHINE_ARCH} == "hppa" || \
    ${MACHINE_ARCH} == "hppa64" || ${MACHINE_ARCH} == "m68k" || \
    ${MACHINE_ARCH} == "m88k" || ${MACHINE_CPU} == "mips64" || \
    ${MACHINE_ARCH} == "sh" || ${MACHINE_ARCH} == "sparc"
XENOCARA_BUILD_GALLIUM?=no
.else
XENOCARA_BUILD_GALLIUM?=${XENOCARA_HAVE_SHARED_LIBS:L}
.endif

# Build pixman?
.if ${MACHINE} == "vax"
XENOCARA_BUILD_PIXMAN?=no
.else
XENOCARA_BUILD_PIXMAN?=yes
.endif
