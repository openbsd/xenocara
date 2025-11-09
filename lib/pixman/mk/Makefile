# $OpenBSD: Makefile,v 1.3 2025/11/09 15:54:47 matthieu Exp $

.include <bsd.own.mk>

PACKAGE_VERSION = 0.46.4
LIB = pixman-1

TOP = ${.CURDIR}/..

NOPROFILE =

SRCS = \
  pixman.c \
  pixman-access-accessors.c \
  pixman-access.c \
  pixman-arm.c \
  pixman-bits-image.c \
  pixman-combine-float.c \
  pixman-combine32.c \
  pixman-conical-gradient.c \
  pixman-edge-accessors.c \
  pixman-edge.c \
  pixman-fast-path.c \
  pixman-filter.c \
  pixman-general.c \
  pixman-glyph.c \
  pixman-gradient-walker.c \
  pixman-image.c \
  pixman-implementation.c \
  pixman-linear-gradient.c \
  pixman-matrix.c \
  pixman-mips.c \
  pixman-noop.c \
  pixman-ppc.c \
  pixman-radial-gradient.c \
  pixman-region16.c \
  pixman-region32.c \
  pixman-region64f.c \
  pixman-riscv.c \
  pixman-solid-fill.c \
  pixman-timer.c \
  pixman-trap.c \
  pixman-utils.c \
  pixman-x86.c


CPPFLAGS += \
	-DHAVE_CONFIG_H \
	-I${_SRCDIR}

CFLAGS += -std=gnu99 -fno-strict-aliasing -fvisibility=hidden -ftrapping-math \
	-Wall -Winvalid-pch -Winline -Wundef -Wno-unused-local-typedefs

.if ${MACHINE_ARCH} == "amd64" || ${MACHINE_ARCH} == "i386"
SRCS += \
  pixman-mmx.c \
  pixman-sse2.c \
  pixman-ssse3.c

CFLAGS_pixman-mmx.so = -mmmx
CFLAGS_pixman-mmx.o = -mmmx

CFLAGS_pixman-sse2.so = -msse2
CFLAGS_pixman-sse2.o = -msse2

CFLAGS_pixman-ssse3.so = -mssse3
CFLAGS_pixman-ssse3.o = -mssse3
.endif

.if ${MACHINE_ARCH} == "mips64" || ${MACHINE_ARCH} == "mips64el"
SRCS += 
.endif

.if ${MACHINE_ARCH} == "arm"
SRCS += pixman-arm-simd-asm.S pixman-arm-neon-asm.S
.endif

.if ${MACHINE_ARCH} == "powerpc" || ${MACHINE_ARCH} == "powerpc64"
SRCS += pixman-vmx.c
CFLAGS_pixman-vmx.so = -maltivec
CFLAGS_pixman-vmx.o = -maltivec
.endif

CFLAGS += $(CFLAGS_$@)

LDADD += -Wl,--allow-shlib-undefined \
	-lm -lpthread

PKGCONFIG = pixman-1.pc

HEADERS =  pixman-version.h
HEADERS_SUBDIR = pixman-1/

beforeinstall:
	@cmp -s ${DESTDIR}${X11BASE}/include/$(HEADERS_SUBDIR)pixman.h \
		$(TOP)/pixman/pixman.h || \
	echo "${INSTALL_DATA} $(TOP)/pixman/pixman.h \
		${DESTDIR}${X11BASE}/include/$(HEADERS_SUBDIR)pixman.h" && \
	${INSTALL_DATA} $(TOP)/pixman/pixman.h \
		${DESTDIR}${X11BASE}/include/$(HEADERS_SUBDIR)pixman.h

obj: _xenocara_obj

.PATH: $(TOP)
.PATH: $(TOP)/pixman

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
