# $OpenBSD: Makefile,v 1.2 2007/04/14 16:11:13 matthieu Exp $

SUBDIR=	resize

PROG=	xterm
MAN=	xterm.1

BINGRP=		utmp
BINMODE=	2755

CPPFLAGS+=	-I${.CURDIR} -I${X11BASE}/include \
		-I${X11BASE}/include/freetype2 \
		-DHAVE_CONFIG_H -DCSRG_BASED \
		-DFUNCPROTO=15 -DNARROWPROTO -DXFREE86_FT2 -DUTMP
LDADD+=		-L${X11BASE}/lib -lXaw -lXt -lSM -lICE \
		-lXmu -lXft -lXrender -lX11 -lXext -lXau -lXdmcp \
		-lfontconfig -lexpat -lfreetype -lutil -ltermcap -lz

SRCS=	TekPrsTbl.c  charproc.c	  fontutils.c  precompose.c scrollbar.c \
	Tekproc.c    charsets.c	  input.c      print.c	    tabs.c \
	VTPrsTbl.c   cursor.c	  main.c       ptydata.c    util.c \
	button.c     data.c	  menu.c       wcwidth.c \
	charclass.c  doublechr.c  misc.c       screen.c	    xstrings.c

X11BASE?=	/usr/X11R6
MANDIR=		${X11BASE}/man/cat
XETC?=		/etc/X11

xterm.1: xterm.man
	sed -e 's#__vendorversion__#"X Window System"#g' \
	    -e 's#__miscmansuffix__#1#g' \
            -e 's#__apploaddir__#${X11ETC}/app-defaults#g' \
	< ${.CURDIR}/xterm.man > $@

CLEANFILES+=	xterm.1

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
