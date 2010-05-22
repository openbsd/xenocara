#	$OpenBSD: Makefile,v 1.4 2010/05/22 20:14:15 matthieu Exp $

LIB=	r300_dri.so

CS_SOURCES = \
	radeon_cs_space_drm.c \
	radeon_bo.c \
	radeon_cs.c 

COMMON_SOURCES = \
	driverfuncs.c \
	mm.c \
	utils.c \
	texmem.c \
	vblank.c \
	xmlconfig.c \
	dri_util.c

RADEON_COMMON_SOURCES = \
	radeon_bo_legacy.c \
	radeon_buffer_objects.c \
	radeon_common_context.c \
	radeon_common.c \
	radeon_cs_legacy.c \
	radeon_dma.c \
	radeon_debug.c \
	radeon_fbo.c \
	radeon_lock.c \
	radeon_mipmap_tree.c \
	radeon_span.c \
	radeon_queryobj.c \
	radeon_texture.c \
	radeon_tex_copy.c

DRIVER_SOURCES = \
		 radeon_screen.c \
		 r300_blit.c \
		 r300_context.c \
		 r300_draw.c \
		 r300_cmdbuf.c \
		 r300_state.c \
		 r300_render.c \
		 r300_tex.c \
		 r300_texstate.c \
		 r300_vertprog.c \
		 r300_fragprog_common.c \
		 r300_shader.c \
		 radeon_mesa_to_rc.c \
		 r300_emit.c \
		 r300_swtcl.c \
		 $(RADEON_COMMON_SOURCES) \
		 $(EGL_SOURCES) \
		 $(CS_SOURCES)

COMPILER_SOURCES = \
		radeon_code.c \
		radeon_compiler.c \
		radeon_program.c \
		radeon_program_print.c \
		radeon_opcodes.c \
		radeon_program_alu.c \
		radeon_program_pair.c \
		radeon_pair_translate.c \
		radeon_pair_schedule.c \
		radeon_pair_regalloc.c \
		radeon_dataflow.c \
		radeon_dataflow_deadcode.c \
		radeon_dataflow_swizzles.c \
		r3xx_fragprog.c \
		r300_fragprog.c \
		r300_fragprog_swizzle.c \
		r300_fragprog_emit.c \
		r500_fragprog.c \
		r500_fragprog_emit.c \
		r3xx_vertprog.c \
		r3xx_vertprog_dump.c \
		memory_pool.c

SRCS = $(COMMON_SOURCES) $(DRIVER_SOURCES) $(COMPILER_SOURCES)

DRIVER_DEFINES = -DRADEON_R300 \
	-DCOMPILE_R300 -DR200_MERGED=0 \
	-DRADEON_COMMON=1 -DRADEON_COMMON_FOR_R300 \
	-I$(MESA)/drivers/dri/r300 \
	-I$(MESA)/drivers/dri/r300/server \
	-I$(MESA)/drivers/dri/radeon \
	-I$(MESA)/drivers/dri/radeon/server

.include <bsd.xorg.mk>

.PATH: $(MESA)/drivers/dri/r300
.PATH: $(MESA)/drivers/dri/r300/compiler
.PATH: $(MESA)/drivers/dri/radeon
