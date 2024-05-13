# $OpenBSD: Makefile,v 1.1 2024/05/13 19:16:27 matthieu Exp $

PACKAGE_VERSION = 0.18.1
LIB = pciaccess

TOP = ${.CURDIR}/..

NOPROFILE =

SRCS= \
	common_bridge.c \
	common_iterator.c \
	common_init.c \
	common_interface.c \
	common_io.c \
	common_capability.c \
	common_device_name.c \
	common_map.c \
	openbsd_pci.c

CPPFLAGS += \
	-DHAVE_CONFIG_H \
	-DPCIIDS_PATH=\"${X11BASE}/share\" \
	-I$(TOP)/mk \
	-I$(TOP)/src \
	-I$(TOP)/include

CFLAGS += -std=c99

EXTRA_PKGCONFIG_SUBST= "-e s/@PCIACCESS_LIBS@//"
PKGCONFIG = pciaccess.pc

beforeinstall:
	@cmp -s ${DESTDIR}${X11BASE}/include/pciaccess.h \
		$(TOP)/include/pciaccess.h || \
	echo "${INSTALL_DATA} $(TOP)/include/pciaccess.h \
		${DESTDIR}${X11BASE}/include/pciaccess.h" && \
	${INSTALL_DATA} $(TOP)/include/pciaccess.h \
		${DESTDIR}${X11BASE}/include/pciaccess.h

obj: _xenocara_obj

.PATH: $(TOP)
.PATH: $(TOP)/src

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
