#	$OpenBSD: Makefile.inc,v 1.1 2020/08/26 06:02:36 jsg Exp $

# bsd.lib.mk and bsd.xorg.mk both include Makefile.inc
.if !defined(MESA_MK_INC)

MESA_SRC?=	${.CURDIR}/../..

MESA_V!=	cat ${MESA_SRC}/VERSION

CPPFLAGS+=	-I${MESA_SRC}/include \
		-I${MESA_SRC}/src \
		-I${X11BASE}/include \
		-I${X11BASE}/include/libdrm \
		-DPACKAGE_VERSION=\"${MESA_V}\"

.include "config.mk"

.endif
MESA_MK_INC=Done
