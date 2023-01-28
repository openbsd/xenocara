#	$OpenBSD: Makefile,v 1.6 2023/01/28 08:56:34 jsg Exp $

.include <bsd.xconf.mk>

LIB=	glsl

NOPROFILE=

# files_libglsl
SRCS=	ast_array_index.cpp \
	ast_expr.cpp \
	ast_function.cpp \
	ast_to_hir.cpp \
	ast_type.cpp \
	builtin_functions.cpp \
	builtin_types.cpp \
	builtin_variables.cpp \
	generate_ir.cpp \
	gl_nir_lower_atomics.c \
	gl_nir_lower_images.c \
	gl_nir_lower_buffers.c \
	gl_nir_lower_packed_varyings.c \
	gl_nir_lower_samplers.c \
	gl_nir_lower_samplers_as_deref.c \
	gl_nir_lower_xfb_varying.c \
	gl_nir_link_atomics.c \
	gl_nir_link_uniform_blocks.c \
	gl_nir_link_uniform_initializers.c \
	gl_nir_link_uniforms.c \
	gl_nir_link_varyings.c \
	gl_nir_link_xfb.c \
	gl_nir_linker.c \
	gl_nir_opt_dead_builtin_varyings.c \
	glsl_parser_extras.cpp \
	glsl_symbol_table.cpp \
	glsl_to_nir.cpp \
	hir_field_selection.cpp \
	ir_array_refcount.cpp \
	ir_basic_block.cpp \
	ir_builder.cpp \
	ir_clone.cpp \
	ir_constant_expression.cpp \
	ir.cpp \
	ir_equals.cpp \
	ir_expression_flattening.cpp \
	ir_function_can_inline.cpp \
	ir_function_detect_recursion.cpp \
	ir_function.cpp \
	ir_hierarchical_visitor.cpp \
	ir_hv_accept.cpp \
	ir_print_visitor.cpp \
	ir_reader.cpp \
	ir_rvalue_visitor.cpp \
	ir_validate.cpp \
	ir_variable_refcount.cpp \
	linker.cpp \
	linker_util.cpp \
	link_functions.cpp \
	link_interface_blocks.cpp \
	link_uniforms.cpp \
	link_uniform_block_active_visitor.cpp \
	link_uniform_blocks.cpp \
	link_varyings.cpp \
	lower_blend_equation_advanced.cpp \
	lower_builtins.cpp \
	lower_discard.cpp \
	lower_discard_flow.cpp \
	lower_distance.cpp \
	lower_instructions.cpp \
	lower_int64.cpp \
	lower_jumps.cpp \
	lower_mat_op_to_vec.cpp \
	lower_named_interface_blocks.cpp \
	lower_packing_builtins.cpp \
	lower_precision.cpp \
	lower_subroutine.cpp \
	lower_tess_level.cpp \
	lower_vec_index_to_cond_assign.cpp \
	lower_vector_derefs.cpp \
	lower_vector_insert.cpp \
	opt_algebraic.cpp \
	opt_constant_folding.cpp \
	opt_constant_propagation.cpp \
	opt_constant_variable.cpp \
	opt_copy_propagation_elements.cpp \
	opt_dead_builtin_variables.cpp \
	opt_dead_code.cpp \
	opt_dead_code_local.cpp \
	opt_dead_functions.cpp \
	opt_flatten_nested_if_blocks.cpp \
	opt_flip_matrices.cpp \
	opt_function_inlining.cpp \
	opt_if_simplification.cpp \
	opt_minmax.cpp \
	opt_rebalance_tree.cpp \
	opt_tree_grafting.cpp \
	propagate_invariance.cpp \
	s_expression.cpp \
	string_to_uint_map.cpp \
	serialize.cpp \
	shader_cache.cpp

# glsl_parser, glsl_lexer_cpp
SRCS+=	glsl_parser.cpp \
	glsl_lexer.cpp

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/compiler \
		-I${MESA_SRC}/src/compiler/glsl \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/glsl \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/compiler/glsl
.PATH:	${MESA_SRC}/generated/src/compiler/glsl
