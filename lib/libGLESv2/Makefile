# $OpenBSD: Makefile,v 1.9 2015/01/25 15:09:14 jsg Exp $
.include <bsd.xconf.mk>

LIB=	GLESv2

MESA=	${.CURDIR}/../../dist/Mesa/src/mesa
MAPI=	${.CURDIR}/../../dist/Mesa/src/mapi
GL=	${.CURDIR}/../../dist/Mesa/Mesa/src/GL
MESA_INCLUDE=	${.CURDIR}/../../dist/Mesa/include

MESA_MAJOR=	10
MESA_MINOR=	4
MESA_TINY=	3

INCSDIR=	${X11BASE}/include/GLES2
INCSDIR3=	${X11BASE}/include/GLES3

.if ${XENOCARA_BUILD_DRI:L} == "yes"
LDADD=	-L${X11BASE}/lib -ldrm -lm -lpthread -lglapi
.else
LDADD=	-L${X11BASE}/lib -lm -lpthread -lglapi
.endif

CPP=	cpp -notraditional
DEBUG?=

CFLAGS+= -std=c99

CPPFLAGS+= \
	-I. \
	-I${MESA_INCLUDE} \
	-I${MESA_INCLUDE}/GL/internal \
	-I${MESA} \
	-I${MAPI} \
	-I${MAPI}/glapi \
	-I${.CURDIR}/generated \
	-I${X11BASE}/include \
	-I/usr/include/dev/pci/drm \
	-DGLX_INDIRECT_RENDERING \
	-DPTHREADS \
	-DHAVE_PTHREAD \
	-DHAVE_ALIAS \
	-D_REENTRANT \
	-DMAPI_MODE_BRIDGE \
	-DMAPI_ABI_HEADER=\"${.CURDIR}/generated/glapi/glapi_mapi_tmp.h\"

.if ${XENOCARA_BUILD_DRI:L} == "yes"
CPPFLAGS+= \
	-DGLX_DIRECT_RENDERING
.endif

.if ${MACHINE_ARCH} == "i386"
CPPFLAGS += \
	-DUSE_X86_ASM \
	-DUSE_MMX_ASM \
	-DUSE_3DNOW_ASM \
	-DUSE_SSE_ASM
.elif ${MACHINE_ARCH} == "amd64"
CPPFLAGS += -D__GLX_ALIGN64 -DUSE_X86_64_ASM
.endif

INCS= \
	gl2.h \
	gl2ext.h \
	gl2platform.h

INCS3= \
	gl3.h \
	gl31.h \
	gl3ext.h \
	gl3platform.h

MAPI_SOURCES = \
	entry.c

SRCS =  $(MAPI_SOURCES)

includes: _SUBDIRUSE
	cd ${MESA_INCLUDE}/GLES2; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/GLES2/$$i || \
		${INSTALL_DATA}	$$i ${DESTDIR}${INCSDIR}/GLES2"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done
	cd ${MESA_INCLUDE}/GLES3; for i in ${INCS3}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/GLES3/$$i || \
		${INSTALL_DATA}	$$i ${DESTDIR}${INCSDIR}/GLES3"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

NOPROFILE=

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.if ${MACHINE_ARCH} == "powerpc" || ${MACHINE_ARCH} == "sparc"
PICFLAG = -fPIC
.endif

glesv2.pc: ${MAPI}/es2api/glesv2.pc.in Makefile
	sed -e 's,@prefix@,$(X11BASE),' \
	    -e 's,@libdir@,${LIBDIR},' \
	    -e 's,@includedir@,${INCSDIR},' \
	    -e 's,@GLESv2_PC_LIB_PRIV@,-lm -lpthread,' \
	    -e 's,@VERSION@,$(MESA_MAJOR).$(MESA_MINOR).$(MESA_TINY),' \
	< ${MAPI}/es2api/glesv2.pc.in > glesv2.pc

afterinstall: glesv2.pc
	$(INSTALL) -c -m 644 -o root -g wheel glesv2.pc \
		${DESTDIR}${LIBDIR}/pkgconfig

CLEANFILES+= glesv2.pc


.PATH: ${MESA}/main
.PATH: ${MAPI}/glapi
.PATH: ${MAPI}
.PATH: ${.CURDIR}/generated/glapi
