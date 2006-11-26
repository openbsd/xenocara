# $Xenocara: Makefile,v 1.1 2006/04/17 17:51:34 matthieu Exp $

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