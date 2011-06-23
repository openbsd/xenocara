# $OpenBSD: Makefile,v 1.9 2011/06/23 22:48:59 naddy Exp $

.include "../Makefile.inc"

PROG= fvwm
SRCS= add_window.c bindings.c borders.c \
	builtins.c colormaps.c colors.c complex.c decorations.c \
	events.c focus.c functions.c fvwm.c fvwmdebug.c icons.c \
	menus.c misc.c modconf.c module.c move.c \
	placement.c read.c resize.c style.c \
	virtual.c windows.c

CPPFLAGS+= -DFVWM_MODULEDIR=\"$(FVWMLIBDIR)\" \
	-DFVWMRC=\".fvwmrc\" \
	-DFVWM_CONFIGDIR=\"$(FVWMLIBDIR)\"

LDADD+= -lXpm -lXt -lICE -lSM -lXext -lX11 -lxcb -lXdmcp -lXau

fvwm.1: fvwm2.1
	sed -e "s,__projectroot__,${X11BASE}," ${.CURDIR}/fvwm2.1 > fvwm.1

CLEANFILES= fvwm.1

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
