#	$OpenBSD: Makefile.inc,v 1.3 2016/03/13 20:23:35 matthieu Exp $

X11BASE ?=	/usr/X11R6

FC_CACHEDIR =	/var/cache/fontconfig
BASECONFIGDIR =	/etc/fonts
MANDIR =	${X11BASE}/man/man

CFLAGS +=	-Wall -Wpointer-arith -Wstrict-prototypes \
		-Wmissing-prototypes -Wmissing-declarations \
		-Wnested-externs -fno-strict-aliasing
CFLAGS +=	-DHAVE_CONFIG_H
CFLAGS +=	-I${FONTCONFIG} \
		-I${X11BASE}/include -I${X11BASE}/include/freetype2 \
		-I${.CURDIR}/../src -I${FONTCONFIG}/src \
		-I${.CURDIR}/../common \
		-I${.CURDIR}

.if ${MACHINE_ARCH} == "alpha" || ${MACHINE_ARCH} == "amd64" || \
    ${MACHINE_ARCH} == "i386" || ${MACHINE_ARCH} == "mips64" || \
    ${MACHINE_ARCH} == "mips64el" || ${MACHINE_ARCH} == "powerpc" || \
    ${MACHINE_ARCH} == "sparc64"
CFLAGS +=	-DHAVE_INTEL_ATOMIC_PRIMITIVES=1
.endif

NOPROFILE =
