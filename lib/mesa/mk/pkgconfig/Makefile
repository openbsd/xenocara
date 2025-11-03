#	$OpenBSD: Makefile,v 1.2 2025/11/03 04:48:25 jsg Exp $

PC_FILES= dri.pc egl.pc gbm.pc gl.pc glesv1_cm.pc glesv2.pc
CLEANFILES+=${PC_FILES}

all: ${PC_FILES}
${PC_FILES}:

dri.pc:
	/bin/sh ${.CURDIR}/gen_dri.sh -c ${.CURDIR} -o ${.OBJDIR}

egl.pc:
	/bin/sh ${.CURDIR}/gen_egl.sh -c ${.CURDIR} -o ${.OBJDIR}

gbm.pc:
	/bin/sh ${.CURDIR}/gen_gbm.sh -c ${.CURDIR} -o ${.OBJDIR}

gl.pc:
	/bin/sh ${.CURDIR}/gen_gl.sh -c ${.CURDIR} -o ${.OBJDIR}

glesv1_cm.pc:
	/bin/sh ${.CURDIR}/gen_glesv1_cm.sh -c ${.CURDIR} -o ${.OBJDIR}

glesv2.pc:
	/bin/sh ${.CURDIR}/gen_glesv2.sh -c ${.CURDIR} -o ${.OBJDIR}

install:
.for PC in ${PC_FILES}
	${INSTALL} ${INSTALL_COPY} -o root -g ${SHAREGRP} \
	    -m ${SHAREMODE} ${.OBJDIR}/${PC} ${DESTDIR}${X11BASE}/lib/pkgconfig/
.endfor

obj: _xenocara_obj
	
.include <bsd.xconf.mk>
.include <bsd.prog.mk>
.include <bsd.xorg.mk>
