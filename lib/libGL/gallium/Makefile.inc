# $OpenBSD: Makefile.inc,v 1.9 2015/01/25 15:09:13 jsg Exp $
.include <bsd.xconf.mk>
.include "../config.mk"

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

CPPFLAGS+=-DPIPE_SEARCH_DIR=\"${X11BASE}/lib/modules/gallium-pipe\" \
	-DGALLIUM_STATIC_TARGETS=1

.if ${XENOCARA_BUILD_GALLIUM} == "llvm"

LLVM_CONFIG?=	/usr/local/bin/llvm-config
LLVM_V!=	${LLVM_CONFIG} --version | sed 's/svn.*//g'
LLVM_V_MAJOR!=	echo ${LLVM_V} | cut -d. -f 1
LLVM_V_MINOR!=	echo ${LLVM_V} | cut -d. -f 2
LLVM_V_PATCH!=	echo ${LLVM_V} | cut -d. -f 3

.if empty(LLVM_V_PATCH)
LLVM_V_PATCH=0
.endif

LLVM_VERSION_INT="${LLVM_V_MAJOR}0${LLVM_V_MINOR}"

# ugh...
ALLVM_CFLAGS!=		${LLVM_CONFIG} --cflags
ALLVM_CPPFLAGS!=	${LLVM_CONFIG} --cxxflags
BLLVM_CFLAGS=	${ALLVM_CFLAGS:M-[IDf]*}
BLLVM_CPPFLAGS=	${ALLVM_CPPFLAGS:M-[IDf]*}
LLVM_CFLAGS=	${BLLVM_CFLAGS:S/-DNDEBUG//}
LLVM_CPPFLAGS=	${BLLVM_CPPFLAGS:S/-DNDEBUG//}
LLVM_LIBDIR!=	${LLVM_CONFIG} --libdir
LLVM_LIBS!=	${LLVM_CONFIG} --libs engine bitwriter mcjit r600 bitreader ipo asmparser

CFLAGS+=	${LLVM_CFLAGS}
CXXFLAGS=	${LLVM_CPPFLAGS}
LDADD+=		-L${LLVM_LIBDIR} ${LLVM_LIBS}

CPPFLAGS+= \
	-DGALLIUM_LLVMPIPE \
	-DHAVE_LLVM=0x0${LLVM_VERSION_INT} -DLLVM_VERSION_PATCH=${LLVM_V_PATCH}

SWRAST_DRIVERS+= ../../gallium/drivers/llvmpipe/libllvmpipe_pic.a

# for r600 and radeonsi
LOCALBASE?=	/usr/local
LDADD+=		${LOCALBASE}/lib/libelf.a
.endif
