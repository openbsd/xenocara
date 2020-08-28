#	$OpenBSD: Makefile,v 1.2 2020/08/28 16:07:19 jsg Exp $

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
		-I${MESA_SRC}/src/loader

LDADD+= ${.CURDIR}/../libmesa_util/${__objdir}/libmesa_util.a \
	${.CURDIR}/../libxmlconfig/${__objdir}/libxmlconfig.a \
	${.CURDIR}/../libloader/${__objdir}/libloader.a \
	-lz -lm -lexpat

.if ${XENOCARA_BUILD_DRI:L} == "yes"
LDADD+= -L${X11BASE}/lib -ldrm
.endif

obj: _xenocara_obj

beforeinstall:
	rm -f ${DESTDIR}${LIBDIR}/lib${LIB}.la

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/gbm/main
.PATH:	${MESA_SRC}/src/gbm/backends/dri
