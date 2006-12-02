# $Xenocara$
.include <bsd.own.mk>
X11BASE?= /usr/X11R6

PROG=	wm2
SRCS=	Border.C Buttons.C Client.C Events.C Main.C Manager.C Rotated.C

CPPFLAGS+=	-I${X11BASE}/include
LDADD+=		-L${X11BASE}/lib -lXext -lX11
MANDIR=		${X11BASE}/man/cat

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
