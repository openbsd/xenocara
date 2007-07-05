# $OpenBSD: Makefile,v 1.6 2007/07/05 22:09:06 matthieu Exp $

SUBDIR=	resize

PROG=	xterm
MAN=	xterm.1tbl

BINGRP=		utmp
BINMODE=	2755

CPPFLAGS+=	-I${.CURDIR} -I${X11BASE}/include \
		-I${X11BASE}/include/freetype2 \
		-DHAVE_CONFIG_H -DCSRG_BASED \
		-DFUNCPROTO=15 -DNARROWPROTO -DXFREE86_FT2 -DUTMP
LDADD+=		-L${X11BASE}/lib -lXaw -lXpm -lXt -lSM -lICE \
		-lXmu -lXft -lXrender -lX11 -lXext -lXau -lXdmcp \
		-lfontconfig -lexpat -lfreetype -lutil -ltermcap -lz

SRCS=	TekPrsTbl.c Tekproc.c VTPrsTbl.c button.c cachedGCs.c \
	charclass.c xtermcap.c charproc.c charsets.c cursor.c \
	data.c doublechr.c fontutils.c input.c main.c menu.c \
	misc.c precompose.c print.c ptydata.c wcwidth.c screen.c \
	scrollbar.c tabs.c util.c xstrings.c

X11BASE?=	/usr/X11R6
MANDIR=		${X11BASE}/man/cat
XETC?=		/etc/X11

xterm.1tbl: xterm.man
	sed -e 's#__vendorversion__#"X Window System"#g' \
	    -e 's#__miscmansuffix__#1#g' \
            -e 's#__apploaddir__#${X11ETC}/app-defaults#g' \
	< ${.CURDIR}/xterm.man > $@

CLEANFILES+=	xterm.1tbl

afterinstall:
	${INSTALL} ${INSTALL_COPY} -m 755 ${.CURDIR}/uxterm \
		${DESTDIR}${BINDIR}
	${INSTALL} ${INSTALL_COPY} -m 644 ${.CURDIR}/XTerm.ad \
		${DESTDIR}${XETC}/app-defaults/XTerm
	${INSTALL} ${INSTALL_COPY} -m 644 ${.CURDIR}/XTerm-col.ad \
		${DESTDIR}${XETC}/app-defaults/XTerm-color
	${INSTALL} ${INSTALL_COPY} -m 644 ${.CURDIR}/UXTerm.ad \
		${DESTDIR}${XETC}/app-defaults/UXTerm

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
