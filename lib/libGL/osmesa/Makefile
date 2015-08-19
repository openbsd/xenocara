# $OpenBSD: Makefile,v 1.1 2015/08/19 23:00:12 matthieu Exp $
.include <bsd.xconf.mk>
.include "${.CURDIR}/../config.mk"

LIB= OSMesa

MESA=   ${.CURDIR}/../../../dist/Mesa/src/mesa
MAPI=   ${.CURDIR}/../../../dist/Mesa/src/mapi
OSMESA=	${MESA}/drivers/osmesa
MESA_INCLUDE=   ${.CURDIR}/../../../dist/Mesa/include

OSMESA_VERSION=	8

SRCS= osmesa.c

NOPROFILE=

PKGCONGIG= osmesa.pc

obj: _xenocara_obj

CPPFLAGS += \
	-I. \
	-I${MESA_INCLUDE} \
	-I${MESA} \
	-I${MAPI} \
	-I${.CURDIR}/../generated

LDADD=	../mesa/libmesa/libmesa_pic.a \
	../glsl/libglsl/libglsl_pic.a \
	-L${X11BASE}/lib -lglapi -lpthread -lstdc++ -lm

osmesa.pc: ${OSMESA}/osmesa.pc.in
	sed -e 's,@prefix@,${X11BASE},' \
	    -e 's,@exec_prefix@,${X11BASE},' \
	    -e 's,@libdir@,${LIBDIR},' \
	    -e 's,@includedir@,${INCSDIR},' \
	    -e 's,@OSMESA_VERSION@,$(OSMESA_VERSION),' \
	    -e 's,@OSMESA_PC_REQ@,,' \
	    -e 's,@OSMESA_LIB@,${LIB},' \
	    -e 's,@OSMESA_PC_LIB_PRIV@,-lm -lpthread,' \
	< ${OSMESA}/osmesa.pc.in > osmesa.pc

beforedepend: osmesa.pc

afterinstall: osmesa.pc
	$(INSTALL_DATA) osmesa.pc ${DESTDIR}${LIBDIR}/pkgconfig

CLEANFILES+= osmesa.pc

.include <bsd.lib.mk>
.include <bsd.xorg.mk>


.PATH: ${MESA}/drivers/osmesa
