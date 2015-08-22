# $OpenBSD: Makefile,v 1.5 2015/08/22 00:50:54 jsg Exp $
.include <bsd.xconf.mk>

LIB=	glapi

MESA=	${.CURDIR}/../../dist/Mesa/src/mesa
MAPI=	${.CURDIR}/../../dist/Mesa/src/mapi
GL=	${.CURDIR}/../../dist/Mesa/Mesa/src/GL
MESA_INCLUDE=	${.CURDIR}/../../dist/Mesa/include

INCSDIR=	${X11BASE}/include/GL

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
	-I${X11BASE}/include/libdrm \
	-DGLX_INDIRECT_RENDERING \
	-DPTHREADS \
	-DHAVE_PTHREAD \
	-DHAVE_ALIAS \
	-D_REENTRANT \
	-DMAPI_MODE_GLAPI \
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

MAPI_GLAPI_SOURCES = \
	u_current.c \
	u_execmem.c

MAPI_SOURCES = \
	entry.c \
	mapi_glapi.c \
	stub.c \
	table.c \
	$(MAPI_GLAPI_SOURCES)

SRCS =  $(MAPI_SOURCES) \
	$(MAPI_GLAPI_SOURCES)

includes: _SUBDIRUSE
	cd ${MESA_INCLUDE}/GL; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/GL/$$i || \
		${INSTALL_DATA}	$$i ${DESTDIR}${INCSDIR}/GL"; \
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
.PATH: ${MESA}/main
.PATH: ${MAPI}/glapi
.PATH: ${MAPI}
.PATH: ${.CURDIR}/generated/glapi
