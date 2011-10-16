#	$OpenBSD: Makefile.inc,v 1.5 2011/10/16 17:18:56 shadchin Exp $

XKC_TOP?=	../../..
XKB_DIR=	${.CURDIR}/${XKC_TOP}/dist/xkeyboard-config
XKB_BASE=	${DESTDIR}${X11BASE}/share/X11/xkb
XKBCOMP=	xkbcomp

INSTALL_DIR=	${INSTALL} -d -o root -g wheel -m ${DIRMODE}

all:	_SUBDIRUSE

.if defined(DATA)
install:	${DATA} install-files _SUBDIRUSE
	test -z ${XKB_BASE}/${XKB_SUBDIR} || ${INSTALL_DIR} ${XKB_BASE}/${XKB_SUBDIR}
	@for f in ${DATA}; do \
		echo ${INSTALL_DATA} ${XKB_DIR}/${XKB_SUBDIR}/$$f ${XKB_BASE}/${XKB_SUBDIR}/$$f; \
		${INSTALL_DATA} ${XKB_DIR}/${XKB_SUBDIR}/$$f ${XKB_BASE}/${XKB_SUBDIR}/$$f; \
	done
.if defined(DIR_DATA)
	cd ${XKB_BASE}/${XKB_SUBDIR} && ${XKBCOMP} -lfhlpR -o ${.OBJDIR}/${DIR_DATA} '*'
	${INSTALL_DATA} ${DIR_DATA} ${XKB_BASE}
.endif
.else
install:	install-files _SUBDIRUSE
.endif

.if !target(intall-files)
install-files:
.endif

.if defined(DIR_DATA) && !target(clean)
clean:
	rm -f ${DIR_DATA}
.endif

.PATH: ${XKB_DIR}/${XKB_SUBDIR}
