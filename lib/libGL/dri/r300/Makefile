#	$OpenBSD: Makefile,v 1.2 2008/11/02 15:01:32 matthieu Exp $

LIB=	r300_dri.so

COMMON_SOURCES = \
	utils.c \
	texmem.c \
	vblank.c \
	dri_util.c \
	xmlconfig.c \
	drirenderbuffer.c

DRIVER_SOURCES = \
		 radeon_screen.c \
		 radeon_context.c \
		 radeon_ioctl.c \
		 radeon_lock.c \
		 radeon_span.c \
		 radeon_state.c \
		 r300_mem.c \
		 \
		 r300_context.c \
		 r300_ioctl.c \
		 r300_cmdbuf.c \
		 r300_state.c \
		 r300_render.c \
		 r300_texmem.c \
		 r300_tex.c \
		 r300_texstate.c \
		 radeon_program.c \
		 radeon_program_alu.c \
	         radeon_program_pair.c \
		 radeon_nqssadce.c \
		 r300_vertprog.c \
		 r300_fragprog.c \
		 r300_fragprog_swizzle.c \
		 r300_fragprog_emit.c \
		 r500_fragprog.c \
		 r500_fragprog_emit.c \
		 r300_shader.c \
		 r300_emit.c \
		 r300_swtcl.c

SRCS = $(COMMON_SOURCES) $(DRIVER_SOURCES)

DRIVER_DEFINES = -DCOMPILE_R300 -DR200_MERGED=0 \
	-DRADEON_COMMON=1 -DRADEON_COMMON_FOR_R300 \
	-I$(MESA)/drivers/dri/r300 \
	-I$(MESA)/drivers/dri/r300/server \
	-I$(MESA)/drivers/dri/radeon \
	-I$(MESA)/drivers/dri/radeon/server

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/r300
.PATH: $(MESA)/drivers/dri/radeon

