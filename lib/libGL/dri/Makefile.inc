# $OpenBSD: Makefile.inc,v 1.7 2008/11/02 15:01:32 matthieu Exp $
.include <bsd.xconf.mk>

MESA= ${.CURDIR}/../../../../dist/Mesa/src/mesa
TOP= ${.CURDIR}/../../../../dist/Mesa

DRI_DRIVER_INSTALL_DIR=	${X11BASE}/lib/modules/dri

COMMON_SOURCES = \
	utils.c \
	texmem.c \
	vblank.c \
	dri_util.c \
	xmlconfig.c \
	drirenderbuffer.c

COMMON_BM_SOURCES = \
	dri_bufmgr.c \
        dri_bufmgr_fake.c

CFLAGS+= -DIN_DRI_DRIVER -DGLX_DIRECT_RENDERING -DPTHREADS $(INCLUDES) $(DRIVER_DEFINES)

INCLUDES = \
	-I$(TOP)/src/mesa/drivers/dri/common \
	-I$(TOP)/include \
	-I$(TOP)/include/GL/internal \
	-I$(TOP)/src/mesa \
	-I$(TOP)/src/mesa/main \
	-I$(TOP)/src/mesa/glapi \
	-I$(TOP)/src/mesa/math \
	-I$(TOP)/src/mesa/transform \
	-I$(TOP)/src/mesa/shader \
	-I$(TOP)/src/mesa/swrast \
	-I$(TOP)/src/mesa/swrast_setup \
	-I$(TOP)/src/egl/main \
	-I$(TOP)/src/egl/drivers/dri \
	-I$(X11BASE)/include \
	-I$(X11BASE)/include/drm

.if ${MACHINE_ARCH} == "i386"
CPPFLAGS += \
	-DUSE_X86_ASM \
	-DUSE_MMX_ASM \
	-DUSE_3DNOW_ASM \
	-DUSE_SSE_ASM
.elif ${MACHINE_ARCH} == "amd64"
CPPFLAGS += -DUSE_X86_64_ASM
.endif

LDADD=	../../libmesa/libmesa_pic.a -L${X11BASE}/lib -lX11 -lexpat -ldrm -lm
DPADD=	../../libmesa/libmesa_pic.a

.PATH: ${MESA}/drivers/dri/common
.PATH: ${MESA}/drivers/common

.SUFFIXES:
.SUFFIXES: .a .c .o .S .s .so

.c.so:
	@echo "${COMPILE.c} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.c} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

all: ${LIB}

OBJS+=  ${SRCS:N*.h:R:S/$/.so/g}

${LIB}:	${OBJS} $(DPADD)
	${CC} -shared ${PICFLAG} -o $@ `${LORDER} ${OBJS}|tsort -q` ${LDADD}

clean:
	rm -f ${LIB} ${OBJS}

cleandir:	clean

install: $(LIB)
	${INSTALL} ${INSTALL_COPY} -o ${LIBOWN} -g ${LIBGRP} -m ${LIBMODE} \
	${LIB} ${DESTDIR}${DRI_DRIVER_INSTALL_DIR}

.include <bsd.dep.mk>






