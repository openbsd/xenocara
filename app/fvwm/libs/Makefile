# $OpenBSD: Makefile,v 1.2 2006/11/27 19:56:35 matthieu Exp $

.include "../Makefile.inc"

.PATH: $(DIST)/libs

LIB=	fvwm
SRCS=   ClientMsg.c GetFont.c Grab.c ModParse.c ModParse.h \
        Module.c Parse.c Picture.c Strings.c System.c \
        envvar.c fvwmlib.h safemalloc.c wild.c \
        lang-strings.h XResource.c debug.c ColorUtils.c

DEBUGLIBS=	no
NOPROFILE=	yes
NOPIC=		yes

install:

.include <bsd.lib.mk>
.include <bsd.xorg.mk>