#	$OpenBSD: Makefile,v 1.2 2022/09/02 05:46:40 jsg Exp $

LIB=	r300

NOPROFILE=

SRCS=	r300_blit.c \
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
	r300_screen_buffer.c \
	r300_screen.c \
	r300_state.c \
	r300_state_derived.c \
	r300_texture.c \
	r300_texture_desc.c \
	r300_tgsi_to_rc.c \
	r300_transfer.c \
	r300_vs.c \
	r300_vs_draw.c \
	memory_pool.c \
	r300_fragprog.c \
	r300_fragprog_emit.c \
	r300_fragprog_swizzle.c \
	r3xx_fragprog.c \
	r3xx_vertprog.c \
	r3xx_vertprog_dump.c \
	r500_fragprog.c \
	r500_fragprog_emit.c \
	radeon_code.c \
	radeon_compiler.c \
	radeon_compiler_util.c \
	radeon_dataflow.c \
	radeon_dataflow_deadcode.c \
	radeon_dataflow_swizzles.c \
	radeon_emulate_branches.c \
	radeon_inline_literals.c \
	radeon_list.c \
	radeon_opcodes.c \
	radeon_optimize.c \
	radeon_pair_dead_sources.c \
	radeon_pair_regalloc.c \
	radeon_pair_schedule.c \
	radeon_pair_translate.c \
	radeon_program_alu.c \
	radeon_program.c \
	radeon_program_pair.c \
	radeon_program_print.c \
	radeon_program_tex.c \
	radeon_remove_constants.c \
	radeon_rename_regs.c \
	radeon_variable.c \
	radeon_vert_fc.c

SRCS+=	r300_nir_algebraic.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DGALLIUM_R300 \
		-I${MESA_SRC}/src/gallium/drivers/r300 \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/drivers/r300
.PATH: ${MESA_SRC}/src/gallium/drivers/r300/compiler
.PATH: ${MESA_SRC}/generated/src/gallium/drivers/r300
