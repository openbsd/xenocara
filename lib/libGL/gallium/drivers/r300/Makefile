# $OpenBSD: Makefile,v 1.4 2014/07/09 21:32:40 jsg Exp $

LIB = r300

PROGRAM_SOURCES = \
	register_allocate.c

COMPILER_SOURCES = \
	radeon_code.c \
	radeon_compiler.c \
	radeon_compiler_util.c \
	radeon_emulate_branches.c \
	radeon_emulate_loops.c \
	radeon_inline_literals.c \
	radeon_program.c \
	radeon_program_print.c \
	radeon_opcodes.c \
	radeon_program_alu.c \
	radeon_program_pair.c \
	radeon_program_tex.c \
	radeon_pair_translate.c \
	radeon_pair_schedule.c \
	radeon_pair_regalloc.c \
	radeon_pair_dead_sources.c \
	radeon_dataflow.c \
	radeon_dataflow_deadcode.c \
	radeon_dataflow_swizzles.c \
	radeon_list.c \
	radeon_optimize.c \
	radeon_remove_constants.c \
	radeon_rename_regs.c \
	radeon_vert_fc.c \
	radeon_variable.c \
	r3xx_fragprog.c \
	r300_fragprog.c \
	r300_fragprog_swizzle.c \
	r300_fragprog_emit.c \
	r500_fragprog.c \
	r500_fragprog_emit.c \
	r3xx_vertprog.c \
	r3xx_vertprog_dump.c \
	memory_pool.c \
	${PROGRAM_SOURCES}

SRCS = \
	r300_blit.c \
	r300_chipset.c \
	r300_context.c \
	r300_debug.c \
	r300_emit.c \
	r300_flush.c \
	r300_fs.c \
	r300_hyperz.c \
	r300_query.c \
	r300_render.c \
	r300_render_stencilref.c \
	r300_render_translate.c \
	r300_resource.c \
	r300_screen.c \
	r300_screen_buffer.c \
	r300_state.c \
	r300_state_derived.c \
	r300_vs.c \
	r300_vs_draw.c \
	r300_texture.c \
	r300_texture_desc.c \
	r300_tgsi_to_rc.c \
	r300_transfer.c \
	${COMPILER_SOURCES}

CPPFLAGS+= -I${.CURDIR}/compiler -I${MESA}/../../include \
	   -I${MESA} -I${MESA}/../glsl -I${MESA}/../mapi \
	   -I${MESA}/program

.include <bsd.xorg.mk>

.PATH: ${MESA}/program
.PATH: ${MESA}/../gallium/drivers/r300/compiler
