#	$OpenBSD: Makefile.inc,v 1.7 2016/10/09 09:54:56 matthieu Exp $

XKC_TOP?=	../../..
XKB_DIR=	${.CURDIR}/${XKC_TOP}/dist/xkeyboard-config
XKB_BASE=	${DESTDIR}${X11BASE}/share/X11/xkb

INSTALL_DIR=	${INSTALL} -d -o root -g wheel -m ${DIRMODE}

all:	_SUBDIRUSE

.if defined(DATA)
install:	${DATA} install-files _SUBDIRUSE
	test -z ${XKB_BASE}/${XKB_SUBDIR} || ${INSTALL_DIR} ${XKB_BASE}/${XKB_SUBDIR}
	@for f in ${DATA}; do \
		echo ${INSTALL_DATA} ${XKB_DIR}/${XKB_SUBDIR}/$$f ${XKB_BASE}/${XKB_SUBDIR}/$$f; \
		${INSTALL_DATA} ${XKB_DIR}/${XKB_SUBDIR}/$$f ${XKB_BASE}/${XKB_SUBDIR}/$$f; \
	done
.else
install:	install-files _SUBDIRUSE
.endif

.if !target(intall-files)
install-files:
.endif

.if !target(clean)
clean:
.endif

.PATH: ${XKB_DIR}/${XKB_SUBDIR}
