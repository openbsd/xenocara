# $OpenBSD: Makefile,v 1.2 2026/03/12 05:10:55 jsg Exp $
.include <bsd.xconf.mk>

LIB=	amdgpu_drv
MAN=	amdgpu.4
MANDIR=	${X11BASE}/man/man

NOPROFILE=

SRCS=	amdgpu_bo_helper.c amdgpu_dri2.c amdgpu_dri3.c amdgpu_drm_queue.c \
	amdgpu_glamor.c amdgpu_glamor_wrappers.c amdgpu_kms.c amdgpu_misc.c \
	amdgpu_pixmap.c amdgpu_present.c amdgpu_probe.c amdgpu_sync.c \
	amdgpu_video.c drmmode_display.c

CFLAGS+=	-Wall
CPPFLAGS+=	-I${.CURDIR} -I${X11BASE}/include -I${X11BASE}/include/xorg \
		-I${X11BASE}/include/pixman-1 -I${X11BASE}/include/libdrm
CPPFLAGS+=	-DHAVE_CONFIG_H

amdgpu.4:	man/amdgpu.man
	sed -e 's,@vendorversion@,xf86-video-amdgpu 25.0.0,' \
	    ${.CURDIR}/man/amdgpu.man > $@

install: maninstall
	${INSTALL} ${INSTALL_COPY} \
	    -o ${BINOWN} -g ${BINGRP} -m 644 \
	    ${.CURDIR}/conf/10-amdgpu.conf \
	    ${DESTDIR}${X11BASE}/share/X11/xorg.conf.d
	${INSTALL} ${INSTALL_COPY} ${INSTALL_STRIP} \
	    -o ${LIBOWN} -g ${LIBGRP} -m ${LIBMODE} \
	    ${LIB}.so ${DESTDIR}${X11BASE}/lib/modules/drivers

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

LDADD+=	-L${X11BASE}/lib -ldrm -ldrm_amdgpu -lgbm

all:	${LIB}.so

${LIB}.so: ${SOBJS} ${DPADD}
	${CC} -shared ${PICFLAG} -o ${.TARGET} \
		`echo ${SOBJS} | tr ' ' '\n' | sort -R` ${LDADD}

CLEANFILES+= ${LIB}.so amdgpu.4

.PATH: ${.CURDIR}/src
