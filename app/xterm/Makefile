# $OpenBSD: Makefile,v 1.24 2014/05/09 19:56:40 matthieu Exp $
.include <bsd.xconf.mk>

SUBDIR=	icons resize

PROG=	xterm
MAN=	xterm.1 uxterm.1 koi8rxterm.1

BINGRP=		utmp
BINMODE=	2755

CPPFLAGS+=	-I. -I${.CURDIR} -I${X11BASE}/include \
		-I${X11BASE}/include/freetype2 \
		-DHAVE_CONFIG_H -DCSRG_BASED \
		-DFUNCPROTO=15 -DNARROWPROTO -DXFREE86_FT2 -DUTMP \
		-DDEF_ALLOW_FONT=False -DDEF_ALLOW_TCAP=False \
		-DDEF_ALLOW_WINDOW=False
LDADD+=		-L${X11BASE}/lib -lXaw -lXpm -lXt -lSM -lICE \
		-lXmu -lXft -lXrender -lX11 -lxcb -lXext -lXau -lXdmcp \
		-lfontconfig -lexpat -lfreetype -lutil -ltermcap -lz

SRCS=	TekPrsTbl.c Tekproc.c VTPrsTbl.c button.c cachedGCs.c \
	charclass.c xtermcap.c charproc.c charsets.c cursor.c \
	data.c doublechr.c fontutils.c input.c linedata.c main.c menu.c \
	misc.c precompose.c print.c ptydata.c wcwidth.c screen.c \
	scrollback.c scrollbar.c tabs.c trace.c util.c version.c xstrings.c

MANDIR=		${X11BASE}/man/man
XETC?=		/etc/X11

.if ${MACHINE_ARCH} == "sh"
charproc.o:	charproc.c
	${CC} -O1 -fno-stack-protector -c ${CPPFLAGS} ${.CURDIR}/charproc.c

screen.o:	screen.c
	${CC} -O1 -fno-stack-protector -c ${CPPFLAGS} ${.CURDIR}/screen.c
.endif

misc.o:	builtin_icons.h

# do this to quiet gcc -Wcast-qual warnings 
builtin_icons.h :
	@echo "#if OPT_BUILTIN_XPMS" >$@
	@echo "#include <icons/mini.xterm.xpms>" >>$@
	@echo "#include <icons/filled-xterm.xpms>" >>$@
	@echo "#include <icons/xterm.xpms>" >>$@
	@echo "#include <icons/xterm-color.xpms>" >>$@
	@echo "#else" >>$@
	@sed -e 's/static char \* /static const char * /' $(.CURDIR)/icons/mini.xterm_48x48.xpm >>$@
	@echo "#endif" >>$@
	@echo "made $@"

beforedepend: builtin_icons.h

.SUFFIXES: .man .1

.man.1:
	sed -e 's#__vendorversion__#"X Window System"#g' \
	    -e 's#__mansuffix__#1#g' \
	    -e 's#__miscmansuffix__#7#g' \
	    -e 's#__apploaddir__#${X11ETC}/app-defaults#g' \
	    < $< > $@

CLEANFILES+=	${MAN} builtin_icons.h

afterinstall:
	${INSTALL} ${INSTALL_COPY} -m 755 ${.CURDIR}/uxterm \
		${DESTDIR}${BINDIR}
	${INSTALL} ${INSTALL_COPY} -m 755 ${.CURDIR}/koi8rxterm \
		${DESTDIR}${BINDIR}
	${INSTALL_DATA} ${.CURDIR}/XTerm.ad \
		${DESTDIR}${XETC}/app-defaults/XTerm
	${INSTALL_DATA} ${.CURDIR}/XTerm-col.ad \
		${DESTDIR}${XETC}/app-defaults/XTerm-color
	${INSTALL_DATA} ${.CURDIR}/UXTerm.ad \
		${DESTDIR}${XETC}/app-defaults/UXTerm
	${INSTALL_DATA} ${.CURDIR}/KOI8RXTerm.ad \
		${DESTDIR}${XETC}/app-defaults/KOI8RXTerm


obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
