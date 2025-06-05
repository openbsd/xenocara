#	$OpenBSD: Makefile,v 1.11 2025/06/05 14:18:42 jsg Exp $

.include <bsd.xconf.mk>

LIB=	gbm

SRCS=	backend.c \
	gbm.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gbm/main \
		-I${MESA_SRC}/src/gbm/backends/dri \
		-I${MESA_SRC}/src/loader \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/frontends/dri \
		-I${MESA_SRC}/generated/src \
		-DDEFAULT_BACKENDS_PATH=\"${X11BASE}/lib/gbm\"

LDADD+=	-Wl,--as-needed -Wl,--start-group \
	${.CURDIR}/../libloader/${__objdir}/libloader.a \
	${.CURDIR}/../libmesa_util/${__objdir}/libmesa_util.a \
	${.CURDIR}/../libmesa_util_sse41/${__objdir}/libmesa_util_sse41.a \
	${.CURDIR}/../libmesa_util_c11/${__objdir}/libmesa_util_c11.a \
	${.CURDIR}/../libxmlconfig/${__objdir}/libxmlconfig.a \
	-Wl,--gc-sections \
	-lz -lexpat -lxcb -lxcb-randr

.if ${XENOCARA_BUILD_DRI:L} == "yes"
LDADD+= -L${X11BASE}/lib -ldrm \
	-L${.CURDIR}/../libgallium_dri/${__objdir} -lgallium_dri
.endif

LDADD+=	-Wl,--end-group

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/gbm/main
