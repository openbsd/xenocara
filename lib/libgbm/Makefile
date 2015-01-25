# $OpenBSD: Makefile,v 1.3 2015/01/25 15:09:14 jsg Exp $
.include <bsd.xconf.mk>

LIB=	gbm

MESA=	${.CURDIR}/../../dist/Mesa/src/mesa
GBM=	${.CURDIR}/../../dist/Mesa/src/gbm
LOADER=	${.CURDIR}/../../dist/Mesa/src/loader

MESA_INCLUDE=		${.CURDIR}/../../dist/Mesa/include
DEFAULT_DRIVER_DIR=     ${X11BASE}/lib/modules/dri

MESA_MAJOR=	10
MESA_MINOR=	4
MESA_TINY=	3

INCSDIR=	${X11BASE}/include

INCS= \
	gbm.h

GBM_SOURCES= \
	gbm.c \
	backend.c \
	gbm_dri.c \

LOADER_SOURCES= \
	loader.c \
	pci_id_driver_map.c

CFLAGS+= -std=c99 -fvisibility=hidden

CPPFLAGS+= \
	-I${MESA_INCLUDE} \
	-I${GBM}/main \
	-I${LOADER} \
	-I${X11BASE}/include \
	-I/usr/include/dev/pci/drm \
	-DHAVE_PTHREAD \
	-D_OS_UNIX=1 \
	-DMODULEDIR=\"${X11BASE}/lib/modules/gbm\" \
	-DDEFAULT_DRIVER_DIR=\"${DEFAULT_DRIVER_DIR}\"

SRCS= \
	${GBM_SOURCES} \
	${LOADER_SOURCES}

LDADD=	-L${X11BASE}/lib -lpthread -ldrm

includes: _SUBDIRUSE
	cd ${GBM}/main; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL_DATA}	$$i ${DESTDIR}${INCSDIR}"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

NOPROFILE=

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

gbm.pc: ${GBM}/main/gbm.pc.in Makefile
	sed -e 's,@prefix@,$(X11BASE),' \
	    -e 's,@exec_prefix@,${X11BASE},' \
	    -e 's,@libdir@,${LIBDIR},' \
	    -e 's,@includedir@,${INCSDIR},' \
	    -e 's,@GBM_PC_REQ_PRIV@,,' \
	    -e 's,@GBM_PC_LIB_PRIV@,-m -lpthread,' \
	    -e 's,@GBM_PC_CFLAGS@,,' \
	    -e 's,@VERSION@,$(MESA_MAJOR).$(MESA_MINOR).$(MESA_TINY),' \
	< ${GBM}/main/gbm.pc.in > gbm.pc

afterinstall: gbm.pc
	$(INSTALL) -c -m 644 -o root -g wheel gbm.pc \
		${DESTDIR}${LIBDIR}/pkgconfig

CLEANFILES+= gbm.pc

.PATH: ${GBM}
.PATH: ${GBM}/main
.PATH: ${GBM}/backends/dri
.PATH: ${LOADER}
