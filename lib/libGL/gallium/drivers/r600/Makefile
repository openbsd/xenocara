# $OpenBSD: Makefile,v 1.1 2013/06/17 23:30:25 jsg Exp $

LIB = r600

SRCS = \
	r600_asm.c \
	r600_blit.c \
	r600_buffer.c \
	r600_pipe.c \
	r600_query.c \
	r600_resource.c \
	r600_shader.c \
	r600_state.c \
	r600_texture.c \
	r700_asm.c \
	evergreen_state.c \
	eg_asm.c \
	r600_translate.c \
	r600_state_common.c

CPPFLAGS+= -I${MESA}/../../include \
	   -I${MESA} -I${X11BASE}/include -I/usr/include/dev/pci/drm
CFLAGS+= -std=gnu99

.include <bsd.xorg.mk>

.PATH: ${MESA}/program
