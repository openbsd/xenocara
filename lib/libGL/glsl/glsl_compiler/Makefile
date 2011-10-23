# $OpenBSD: Makefile,v 1.1 2011/10/23 13:57:36 matthieu Exp $
.include <bsd.xconf.mk>

MESA=	${.CURDIR}/../../../../dist/Mesa/src/mesa
MAPI=	${.CURDIR}/../../../../dist/Mesa/src/mapi
GLSL=	${.CURDIR}/../../../../dist/Mesa/src/glsl

MESA_INCLUDE=	${.CURDIR}/../../../../dist/Mesa/include

GLSL2_C_SOURCES = \
	hash_table.c \
	symbol_table.c
GLSL2_CXX_SOURCES = \
	main.cpp

INCLUDES= \
	-I${X11BASE}/include/GL \
	-I$(GLSL) \
	-I$(GLSL)/glcpp \
	-I$(MESA) \
	-I$(MAPI) \
	-I$(MESA_INCLUDE)

CPPFLAGS+= $(INCLUDES) $(DEFINES)

SRCS= 	$(GLSL2_C_SOURCES) \
	$(GLSL2_CXX_SOURCES)

OBJS=	$(GLSL2_C_SOURCES:.c=.o) \
	$(GLSL2_CXX_SOURCES:.cpp=.o)

PROG=	glsl_compiler

LDADD=	../libglsl/libglsl.a

all: ${PROG}

clean:
	rm -f ${OBJS} ${PROG}

.SUFFIXES: .cpp

.cpp.o:
	@echo "${COMPILE.cc} ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.cc} ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${PROG}: ${LIBCRT0} ${OBJS} ${LIBC} ${CRTBEGIN} ${CRTEND} ${DPADD}
	${CXX} ${LDFLAGS} ${LDSTATIC} -o ${.TARGET} ${OBJS} ${LDADD}

.include <bsd.xorg.mk>

.PATH: ${GLSL}
.PATH: ${MESA}/program
