# $OpenBSD: Makefile.inc,v 1.2 2012/11/11 12:09:09 mpi Exp $

CPP=	cpp -notraditional

OBJS+=  ${SRCS:N*.h:R:S/$/.so/g}

.SUFFIXES:
.SUFFIXES: .a .c .cpp .o .so

.c.so:
	@echo "${COMPILE.c} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.c} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

.cpp.so:
	@echo "${COMPILE.cc} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.cc} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

lib${LIB}_pic.a: ${OBJS} ${DPADD}
	@rm -f lib${LIB}_pic.a
	@${AR} cq lib${LIB}_pic.a `${LORDER} ${OBJS} | tsort -q`
	${RANLIB} lib${LIB}_pic.a


.ifdef XENOCARA_HAVE_LLVM
LLVM_VERSION=	0x301	# 3.1 
LLVM_CONFIG?=	/usr/local/bin/llvm-config

LLVM_CFLAGS!=	${LLVM_CONFIG} --cflags
LLVM_CPPFLAGS!=	${LLVM_CONFIG} --cxxflags
LLVM_LIBDIR!=	${LLVM_CONFIG} --libdir
LLVM_LIBS!=	${LLVM_CONFIG} --libs engine

CFLAGS+=	${LLVM_CFLAGS} -std=gnu99
CXXFLAGS=	${LLVM_CPPFLAGS}
LDADD+=		-L${LLVM_LIBDIR} ${LLVM_LIBS}

CPPFLAGS+= \
	-DGALLIUM_LLVMPIPE \
	-DHAVE_LLVM=${LLVM_VERSION}
.endif


.if ${MACHINE_ARCH} == powerpc || ${MACHINE_ARCH} == sparc
PICFLAG=-fPIC
.endif
