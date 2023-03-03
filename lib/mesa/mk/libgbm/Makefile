#	$OpenBSD: Makefile,v 1.9 2023/03/03 06:32:18 jsg Exp $

.include <bsd.xconf.mk>

LIB=	gbm

SRCS=	backend.c \
	gbm.c

.if ${XENOCARA_BUILD_DRI:L} == "yes"
SRCS+=	gbm_dri.c
.endif

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gbm/main \
		-I${MESA_SRC}/src/gbm/backends/dri \
		-I${MESA_SRC}/src/loader \
		-DDEFAULT_BACKENDS_PATH=\"${X11BASE}/lib/gbm\"

LDADD+=	-Wl,--as-needed -Wl,--start-group \
	${.CURDIR}/../libloader/${__objdir}/libloader.a \
	${.CURDIR}/../libmesa_util/${__objdir}/libmesa_util.a \
	${.CURDIR}/../libmesa_format/${__objdir}/libmesa_format.a \
	${.CURDIR}/../libmesa_util_sse41/${__objdir}/libmesa_util_sse41.a \
	${.CURDIR}/../libmesa_util_c11/${__objdir}/libmesa_util_c11.a \
	${.CURDIR}/../libxmlconfig/${__objdir}/libxmlconfig.a \
	-Wl,--gc-sections \
	-lz -lm -lexpat -lpthread

.if ${XENOCARA_BUILD_DRI:L} == "yes"
LDADD+= -L${X11BASE}/lib -ldrm
.endif

LDADD+=	-Wl,--end-group

obj: _xenocara_obj

beforeinstall:
	rm -f ${DESTDIR}${LIBDIR}/lib${LIB}.la

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/gbm/main
.PATH:	${MESA_SRC}/src/gbm/backends/dri
