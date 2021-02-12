#	$OpenBSD: Makefile,v 1.2 2021/02/12 01:16:05 jsg Exp $

.include "../config.mk"

PC_FILES= libdrm.pc libdrm_amdgpu.pc libdrm_radeon.pc

.if ${WITH_INTEL} == "yes"
PC_FILES+= libdrm_intel.pc
.endif

CLEANFILES+=${PC_FILES}

all: ${PC_FILES}
${PC_FILES}:

libdrm.pc:
	/bin/sh ${.CURDIR}/gen_libdrm.sh -c ${.CURDIR} -o ${.OBJDIR}

libdrm_amdgpu.pc:
	/bin/sh ${.CURDIR}/gen_libdrm_amdgpu.sh -c ${.CURDIR} -o ${.OBJDIR}

libdrm_intel.pc:
	/bin/sh ${.CURDIR}/gen_libdrm_intel.sh -c ${.CURDIR} -o ${.OBJDIR}

libdrm_radeon.pc:
	/bin/sh ${.CURDIR}/gen_libdrm_radeon.sh -c ${.CURDIR} -o ${.OBJDIR}

install:
.for PC in ${PC_FILES}
	${INSTALL} ${INSTALL_COPY} -o root -g ${SHAREGRP} \
	    -m ${SHAREMODE} ${.OBJDIR}/${PC} ${DESTDIR}${X11BASE}/lib/pkgconfig/
.endfor

obj: _xenocara_obj
	
.include <bsd.xconf.mk>
.include <bsd.prog.mk>
.include <bsd.xorg.mk>
