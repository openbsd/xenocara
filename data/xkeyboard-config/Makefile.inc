#	$OpenBSD: Makefile.inc,v 1.2 2009/06/06 19:44:01 matthieu Exp $

TOP?=		../../..
XKB_DIR=	${.CURDIR}/${TOP}/dist/xkeyboard-config
XKB_BASE=	${DESTDIR}${X11BASE}/share/X11/xkb
XKBCOMP=	xkbcomp

.if !target(all)
.if defined(DIR_DATA)
all:	 _SUBDIRUSE ${DIR_DATA}
.else
all:	 _SUBDIRUSE
.endif
.endif

${DIR_DATA}: ${DATA}
	rm -f $@
	cd ${.CURDIR} && ${XKBCOMP} -lfhlpR -o ${.OBJDIR}/$@ '*'

install:	${DATA} install-dir install-files _SUBDIRUSE
	@for f in ${DATA}; do \
	echo installing "$$f" ; \
	cmp -s ${XKB_DIR}/${XKB_SUBDIR}/$$f ${XKB_BASE}/${XKB_SUBDIR}/$$f || ${INSTALL_DATA} ${XKB_DIR}/${XKB_SUBDIR}/$$f ${XKB_BASE}/${XKB_SUBDIR}/$$f; done

.if defined(DIR_DATA)
install-dir: ${DIR_DATA}
	${INSTALL_DATA} ${DIR_DATA} ${XKB_BASE}/${XKB_SUBDIR}
.else
install-dir:
.endif

.if !target(intall-files)
install-files:
.endif

.if defined(DIR_DATA) && !target(clean)
clean:
	rm -f ${DIR_DATA}
.endif

.PATH: ${XKB_DIR}/${XKB_SUBDIR}
