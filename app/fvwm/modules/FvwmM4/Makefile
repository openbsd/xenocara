# $Xenocara: Makefile,v 1.1 2006/04/17 20:16:24 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmM4

PROG=	FvwmM4
SRCS=	FvwmM4.c

CPPFLAGS+= -DFVWM_MODULEDIR=\"$(FVWMLIBDIR)\" \
	-DFVWM_CONFIGDIR=\"$(FVWMLIBDIR)\" \

LDADD+= -lX11
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
