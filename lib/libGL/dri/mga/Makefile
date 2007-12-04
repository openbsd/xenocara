#	$OpenBSD: Makefile,v 1.1 2007/12/04 22:00:27 matthieu Exp $

LIB=	mga_dri.so

DRIVER_SOURCES = \
	mgadd.c \
	mgaioctl.c \
	mgarender.c \
	mgastate.c \
	mgatris.c \
	mgapixel.c \
	mgaspan.c \
	mgatex.c \
	mgatexmem.c \
	mga_texstate.c \
	mga_texcombine.c \
	mgavb.c \
	mga_xmesa.c

SRCS = \
	$(COMMON_SOURCES) \
	$(DRIVER_SOURCES) 

DRIVER_DEFINES = -I$(MESA)/drivers/dri/mga/server

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/mga
