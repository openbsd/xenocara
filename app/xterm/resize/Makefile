# $OpenBSD: Makefile,v 1.8 2021/01/02 10:24:36 matthieu Exp $

PROG=	resize
MAN=	resize.1

VPATH=	${.CURDIR}/..

CPPFLAGS+=	-I${.CURDIR}/.. -I${X11BASE}/include \
		-I${X11BASE}/include/freetype2 \
		-DHAVE_CONFIG_H -DUTMP

X11BASE?=	/usr/X11R6
MANDIR=		${X11BASE}/man/man

SRCS=		resize.c version.c xstrings.c

PATCH_NUM != sed -n '/XTERM_PATCH/s/[^0-9]*//gp' ${.CURDIR}/../version.h
PATCH_YMD != sed -n '/XTERM_DATE/s,[^0-9/.-]*,,gp' ${.CURDIR}/../version.h


resize.1: resize.man
	sed -e 's#__vendorversion__#"X Window System"#g' \
	    -e 's#__mansuffix__#1#g' \
	    -e 's#__miscmansuffix__#7#g' \
	    -e 's#__apploaddir__#${X11ETC}/app-defaults#g' \
	    -e s%__default_termname__%xterm% \
	    -e 's%__app_version__%Patch\ \#'$(PATCH_NUM)% \
	    -e 's%__app_date__%'${PATCH_YMD}% \
	    < ${.CURDIR}/../resize.man > $@

CLEANFILES+=	resize.1

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
