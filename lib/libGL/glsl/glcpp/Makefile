# $OpenBSD: Makefile,v 1.1 2011/10/23 13:57:36 matthieu Exp $
.include <bsd.xconf.mk>

MESA=	${.CURDIR}/../../../../dist/Mesa/src/mesa
MAPI=	${.CURDIR}/../../../../dist/Mesa/src/mapi
GLSL=	${.CURDIR}/../../../../dist/Mesa/src/glsl

MESA_INCLUDE=	${.CURDIR}/../../../../dist/Mesa/include


GLCPP_SOURCES = \
	hash_table.c \
	glcpp.c

INCLUDES= \
	-I${X11BASE}/include/GL \
	-I$(GLSL) \
	-I$(GLSL)/glcpp \
	-I$(MESA) \
	-I$(MAPI) \
	-I$(MESA_INCLUDE)

CPPFLAGS+= $(INCLUDES) $(DEFINES)

SRCS= 	$(GLCPP_SOURCES)

OBJS=	$(GLCPP_SOURCES:.c=.o) \

PROG=	glcpp

LDADD=	../libglsl/libglsl.a

all: ${PROG}

clean:
	rm -f ${OBJS} ${PROG}

${PROG}: ${LIBCRT0} ${OBJS} ${LIBC} ${CRTBEGIN} ${CRTEND} ${DPADD}
	${CXX} ${LDFLAGS} ${LDSTATIC} -o ${.TARGET} ${OBJS} ${LDADD}

.include <bsd.xorg.mk>

.PATH: ${GLSL}
.PATH: ${GLSL}/glcpp
.PATH: ${MESA}/program
