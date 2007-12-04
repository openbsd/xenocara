#	$OpenBSD: Makefile,v 1.1 2007/12/04 22:00:28 matthieu Exp $

LIB=	savage_dri.so

DRIVER_SOURCES = \
	savage_xmesa.c \
	savagedd.c \
	savagestate.c \
	savagetex.c \
	savagetris.c \
	savagerender.c \
	savageioctl.c \
	savagespan.c

SRCS = \
	$(COMMON_SOURCES) \
	$(DRIVER_SOURCES) 

DRIVER_DEFINES = -I$(MESA)/drivers/dri/savage/server

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/savage
