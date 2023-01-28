#	$OpenBSD: Makefile,v 1.5 2023/01/28 08:56:33 jsg Exp $

LIB=	aco

NOPROFILE=

SRCS=	aco_dead_code_analysis.cpp \
	aco_dominance.cpp \
	aco_instruction_selection.cpp \
	aco_instruction_selection_setup.cpp \
	aco_interface.cpp \
	aco_ir.cpp \
	aco_assembler.cpp \
	aco_form_hard_clauses.cpp \
	aco_insert_exec_mask.cpp \
	aco_insert_NOPs.cpp \
	aco_insert_waitcnt.cpp \
	aco_reduce_assign.cpp \
	aco_register_allocation.cpp \
	aco_live_var_analysis.cpp \
	aco_lower_phis.cpp \
	aco_lower_to_cssa.cpp \
	aco_lower_to_hw_instr.cpp \
	aco_optimizer.cpp \
	aco_optimizer_postRA.cpp \
	aco_opt_value_numbering.cpp \
	aco_print_asm.cpp \
	aco_print_ir.cpp \
	aco_reindex_ssa.cpp \
	aco_scheduler.cpp \
	aco_spill.cpp \
	aco_ssa_elimination.cpp \
	aco_statistics.cpp \
	aco_validate.cpp

SRCS+=	aco_opcodes.cpp

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS} ${PICFLAG} -fno-exceptions -fno-rtti

LLVM_CXXFLAGS!= llvm-config --cxxflags
CXXFLAGS+=	${LLVM_CXXFLAGS}

CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/amd \
		-I${MESA_SRC}/src/amd/common \
		-I${MESA_SRC}/src/amd/compiler \
		-I${MESA_SRC}/src/amd/llvm \
		-I${MESA_SRC}/src/compiler \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/generated/src/amd \
		-I${MESA_SRC}/generated/src/amd/common \
		-I${MESA_SRC}/generated/src/amd/compiler \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/amd/compiler
.PATH: ${MESA_SRC}/generated/src/amd/compiler
