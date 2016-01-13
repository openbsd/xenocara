# $OpenBSD: Makefile,v 1.27 2016/01/13 20:40:08 matthieu Exp $
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
LDADD+=		-L${X11BASE}/lib -lXaw -lXpm -lXt -lSM -lICE -lXmu \
		-lXft -lXrender -lXinerama -lX11 -lxcb -lXext -lXau -lXdmcp \
		-lfontconfig -lexpat -lfreetype -lutil -ltermcap -lz

SRCS=	TekPrsTbl.c Tekproc.c VTPrsTbl.c button.c cachedGCs.c \
	charclass.c xtermcap.c charproc.c charsets.c cursor.c \
	data.c doublechr.c fontutils.c input.c linedata.c main.c menu.c \
	misc.c precompose.c print.c ptydata.c wcwidth.c screen.c \
	scrollback.c scrollbar.c tabs.c trace.c util.c version.c xstrings.c

MANDIR=		${X11BASE}/man/man

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

PATCH_NUM != sed -n '/XTERM_PATCH/s/[^0-9]*//gp' ${.CURDIR}/version.h
PATCH_YMD != sed -n '/XTERM_DATE/s,[^0-9/.-]*,,gp' ${.CURDIR}/version.h

.man.1:
	sed -e 's#__vendorversion__#"X Window System"#g' \
	    -e 's#__mansuffix__#1#g' \
	    -e 's#__miscmansuffix__#7#g' \
	    -e 's#__apploaddir__#${X11BASE}/share/X11/app-defaults#g' \
	    -e 's%__app_version__%Patch\ \#'$(PATCH_NUM)% \
	    -e 's%__app_date__%'${PATCH_YMD}% \
	    -e s%__default_termname__%xterm% \
	    -e s%__default_termid__%vt100% \
	    -e s%__alt_sends_esc__%false% \
	    -e s%__meta_sends_esc__%false% \
	    -e s%__backarrow_is_bs__%true% \
	    -e s%__backarrow_is_erase__%true% \
	    -e s%__delete_is_del__%true% \
	    -e s%__default_class__%XTerm% \
	    -e s%__pixmapsdir__%${X11BASE}/include/X11/pixmaps/% \
	    -e s%/etc/utmp%/var/run/utmp%g \
	    -e s%/etc/wtmp%/var/run/wtmp%g \
	     < $< > $@

CLEANFILES+=	${MAN} builtin_icons.h

afterinstall:
	${INSTALL} ${INSTALL_COPY} -m 755 ${.CURDIR}/uxterm \
		${DESTDIR}${BINDIR}
	${INSTALL} ${INSTALL_COPY} -m 755 ${.CURDIR}/koi8rxterm \
		${DESTDIR}${BINDIR}
	${INSTALL_DATA} ${.CURDIR}/XTerm.ad \
		${DESTDIR}${X11BASE}/share/X11/app-defaults/XTerm
	${INSTALL_DATA} ${.CURDIR}/XTerm-col.ad \
		${DESTDIR}${X11BASE}/share/X11/app-defaults/XTerm-color
	${INSTALL_DATA} ${.CURDIR}/UXTerm.ad \
		${DESTDIR}${X11BASE}/share/X11/app-defaults/UXTerm
	${INSTALL_DATA} ${.CURDIR}/KOI8RXTerm.ad \
		${DESTDIR}${X11BASE}/share/X11/app-defaults/KOI8RXTerm


obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
