#	$OpenBSD: Makefile.inc,v 1.2 2010/08/25 17:44:24 todd Exp $


.if ${MACHINE_ARCH} == powerpc || ${MACHINE_ARCH} == sparc
PICFLAG=-fPIC
.endif

OBJS = ${C_SOURCES:.c=.so}

CPPFLAGS = \
	 -I${.CURDIR} ${LIBRARY_INCLUDES}

.if defined(LIB)
all: ${LIB}_pic.a
.endif

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
