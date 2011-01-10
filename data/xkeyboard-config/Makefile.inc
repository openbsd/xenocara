#	$OpenBSD: Makefile.inc,v 1.3 2011/01/10 14:42:48 shadchin Exp $

TOP?=		../../..
XKB_DIR=	${.CURDIR}/${TOP}/dist/xkeyboard-config
XKB_BASE=	${DESTDIR}${X11BASE}/share/X11/xkb
XKBCOMP=	xkbcomp

INSTALL_DIR=	${INSTALL} -d -o ${SHAREOWN} -g ${SHAREGRP} -m ${DIRMODE}

.if !target(all)
.if defined(DIR_DATA)
all:	_SUBDIRUSE ${DIR_DATA}
.else
all:	_SUBDIRUSE
.endif
.endif

.if defined(DIR_DATA)
${DIR_DATA}:	${DATA}
	rm -f $@
	cd ${XKB_DIR}/${XKB_SUBDIR} && ${XKBCOMP} -lfhlpR -o ${.OBJDIR}/$@ '*'
.endif

.if defined(DATA)
install:	${DATA} install-dir install-files _SUBDIRUSE
	test -z ${XKB_BASE}/${XKB_SUBDIR} || ${INSTALL_DIR} ${XKB_BASE}/${XKB_SUBDIR}
	@for f in ${DATA}; do \
		echo installing "$$f" ; \
		cmp -s ${XKB_DIR}/${XKB_SUBDIR}/$$f ${XKB_BASE}/${XKB_SUBDIR}/$$f || \
		${INSTALL_DATA} ${XKB_DIR}/${XKB_SUBDIR}/$$f ${XKB_BASE}/${XKB_SUBDIR}/$$f; \
	done
.else
install:	install-dir install-files _SUBDIRUSE
.endif

.if defined(DIR_DATA)
install-dir:	${DIR_DATA}
	test -z ${XKB_BASE} || ${INSTALL_DIR} ${XKB_BASE}
	${INSTALL_DATA} ${DIR_DATA} ${XKB_BASE}
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
