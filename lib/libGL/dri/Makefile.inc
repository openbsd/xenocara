# $OpenBSD: Makefile.inc,v 1.5 2008/03/25 23:41:50 matthieu Exp $
.include <bsd.xconf.mk>

MESA= ${.CURDIR}/../../../../dist/Mesa/src/mesa
TOP= ${.CURDIR}/../../../../dist/Mesa

DRI_DRIVER_INSTALL_DIR=	${X11BASE}/lib/modules/dri

COMMON_SOURCES = \
	driverfuncs.c \
	utils.c \
	texmem.c \
	vblank.c \
	dri_util.c \
	xmlconfig.c \
	drirenderbuffer.c

COMMON_BM_SOURCES = \
	dri_bufmgr.c \
	dri_drmpool.c

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

LDADD=	../../libmesa/libmesa_pic.a -L${X11BASE}/lib -lX11 -lexpat -ldrm -lm
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






