#	$OpenBSD: Makefile.inc,v 1.1 2010/05/22 20:14:15 matthieu Exp $


.if ${MACHINE_ARCH} == powerpc || ${MACHINE_ARCH} == sparc
PICFLAG=-fPIC
.endif

OBJS = ${C_SOURCES:.c=.so}

CPPFLAGS = \
	 -I${.CURDIR} ${LIBRARY_INCLUDES}

all: ${LIB}_pic.a

${LIB}_pic.a:	${OBJS} $(DPADD)
	@rm -f ${LIB}_pic.a
	@${AR} cq ${LIB}_pic.a `${LORDER} ${OBJS} | tsort -q`
	${RANLIB} ${LIB}_pic.a

clean:
	rm -f ${LIB}_pic.a ${OBJS}

cleandir: clean

install:
	@echo "noting to install"

.SUFFIXES:
.SUFFIXES: .a .c .o .so

.c.so:
	${COMPILE.c} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o


.include <bsd.dep.mk>
