# $OpenBSD: Makefile,v 1.1 2007/04/09 19:11:43 matthieu Exp $

PROG=	resize
MAN=	resize.1

VPATH=	${.CURDIR}/..

CPPFLAGS+=	-I${.CURDIR}/.. -I${X11BASE}/include \
		-I${X11BASE}/include/freetype2 \
		-DHAVE_CONFIG_H -DCSRG_BASED \
		-DFUNCPROTO=15 -DNARROWPROTO -DXFREE86_FT2 -DUTMP
LDADD+=		-L${X11BASE}/lib -lXaw -lXt -lSM -lICE \
		-lXmu -lXft -lXrender -lX11 -lXext -lXau -lXdmcp \
		-lfontconfig -lexpat -lfreetype -lutil -ltermcap -lz

X11BASE?=	/usr/X11R6
MANDIR=		${X11BASE}/man/cat

SRCS=		resize.c xstrings.c

resize.1: resize.man
	sed -e 's#__vendorversion__#"X Window System"#g' \
	    -e 's#__miscmansuffix__#1#g' \
            -e 's#__apploaddir__#${X11ETC}/app-defaults#g' \
	< ${.CURDIR}/../resize.man > $@

CLEANFILES+=	resize.1

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
