# $OpenBSD: Makefile,v 1.6 2013/03/09 14:45:39 matthieu Exp $

PROG=	resize
MAN=	resize.1

VPATH=	${.CURDIR}/..

CPPFLAGS+=	-I${.CURDIR}/.. -I${X11BASE}/include \
		-I${X11BASE}/include/freetype2 \
		-DHAVE_CONFIG_H -DCSRG_BASED \
		-DFUNCPROTO=15 -DNARROWPROTO -DXFREE86_FT2 -DUTMP

X11BASE?=	/usr/X11R6
MANDIR=		${X11BASE}/man/man

SRCS=		resize.c version.c xstrings.c

resize.1: resize.man
	sed -e 's#__vendorversion__#"X Window System"#g' \
	    -e 's#__mansuffix__#1#g' \
	    -e 's#__miscmansuffix__#7#g' \
	    -e 's#__apploaddir__#${X11ETC}/app-defaults#g' \
	    < ${.CURDIR}/../resize.man > $@

CLEANFILES+=	resize.1

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
