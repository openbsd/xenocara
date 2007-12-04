LIB=	i810_dri.so

DRIVER_SOURCES = \
	i810context.c \
	i810ioctl.c \
	i810render.c \
	i810screen.c \
	i810span.c \
	i810state.c \
	i810tex.c \
	i810texmem.c \
	i810texstate.c \
	i810tris.c \
	i810vb.c

SRCS= ${DRIVER_SOURCES} ${COMMON_SOURCES}

DRIVER_DEFINES= -I$(MESA)/drivers/dri/i810/server

.include <bsd.xorg.mk>

.PATH: ${MESA}/drivers/dri/i810
