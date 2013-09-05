# $OpenBSD: Makefile,v 1.6 2013/09/05 15:12:08 jsg Exp $
.include <bsd.xconf.mk>

MESA=	${.CURDIR}/../../../../dist/Mesa/src/mesa
MAPI=	${.CURDIR}/../../../../dist/Mesa/src/mapi
GLSL=	${.CURDIR}/../../../../dist/Mesa/src/glsl

MESA_INCLUDE=	${.CURDIR}/../../../../dist/Mesa/include


LIBGLCPP_SOURCES = \
	glcpp-lex.c \
	glcpp-parse.c \
	pp.c

GLCPP_SOURCES = \
	$(LIBGLCPP_SOURCES) \
	ralloc.c \
	glcpp.c

C_SOURCES = \
	strtod.c \
	ralloc.c \
	$(LIBGLCPP_SOURCES)

#	builtin_stubs.cpp is used instead of builtin_function.cpp
#	when bootstrapping builtin_compiler
CXX_SOURCES = \
	ast_array_index.cpp \
	ast_expr.cpp \
	ast_function.cpp \
	ast_to_hir.cpp \
	ast_type.cpp \
	builtin_function.cpp \
	builtin_types.cpp \
	builtin_variables.cpp \
	glsl_lexer.cpp \
	glsl_parser.cpp \
	glsl_parser_extras.cpp \
	glsl_symbol_table.cpp \
	glsl_types.cpp \
	hir_field_selection.cpp \
	ir.cpp \
	ir_basic_block.cpp \
	ir_builder.cpp \
	ir_clone.cpp \
	ir_constant_expression.cpp \
	ir_expression_flattening.cpp \
	ir_function.cpp \
	ir_function_can_inline.cpp \
	ir_function_detect_recursion.cpp \
	ir_hierarchical_visitor.cpp \
	ir_hv_accept.cpp \
	ir_import_prototypes.cpp \
	ir_print_visitor.cpp \
	ir_reader.cpp \
	ir_rvalue_visitor.cpp \
	ir_set_program_inouts.cpp \
	ir_validate.cpp \
	ir_variable_refcount.cpp \
	link_functions.cpp \
	link_interface_blocks.cpp \
	link_uniform_block_active_visitor.cpp \
	link_uniform_blocks.cpp \
	link_uniform_initializers.cpp \
	link_uniforms.cpp \
	link_varyings.cpp \
	linker.cpp \
	loop_analysis.cpp \
	loop_controls.cpp \
	loop_unroll.cpp \
	lower_clip_distance.cpp \
	lower_discard.cpp \
	lower_discard_flow.cpp \
	lower_if_to_cond_assign.cpp \
	lower_instructions.cpp \
	lower_jumps.cpp \
	lower_mat_op_to_vec.cpp \
	lower_named_interface_blocks.cpp \
	lower_noise.cpp \
	lower_output_reads.cpp \
	lower_packed_varyings.cpp \
	lower_packing_builtins.cpp \
	lower_texture_projection.cpp \
	lower_texture_projection.cpp \
	lower_ubo_reference.cpp \
	lower_variable_index_to_cond_assign.cpp \
	lower_vec_index_to_cond_assign.cpp \
	lower_vec_index_to_swizzle.cpp \
	lower_vector.cpp \
	lower_vector_insert.cpp \
	main.cpp \
	opt_algebraic.cpp \
	opt_array_splitting.cpp \
	opt_constant_folding.cpp \
	opt_constant_propagation.cpp \
	opt_constant_variable.cpp \
	opt_copy_propagation.cpp \
	opt_copy_propagation_elements.cpp \
	opt_dead_builtin_varyings.cpp \
	opt_dead_code.cpp \
	opt_dead_code_local.cpp \
	opt_dead_functions.cpp \
	opt_flatten_nested_if_blocks.cpp \
	opt_flip_matrices.cpp \
	opt_function_inlining.cpp \
	opt_if_simplification.cpp \
	opt_noop_swizzle.cpp \
	opt_redundant_jumps.cpp \
	opt_structure_splitting.cpp \
	opt_swizzle_swizzle.cpp \
	opt_tree_grafting.cpp \
	s_expression.cpp \
	standalone_scaffolding.cpp \
	test.cpp \
	test_optpass.cpp

LIB=	libglsl

SRCS=	\
	$(C_SOURCES) \
	$(CXX_SOURCES) \

OBJS+= $(C_SOURCES:.c=.so) $(CXX_SOURCES:.cpp=.so)

INCLUDES= \
	-I${X11BASE}/include/GL \
	-I$(GLSL) \
	-I$(GLSL)/glcpp \
	-I${.CURDIR}/../../generated/glsl \
	-I${.CURDIR}/../../generated/glsl/glcpp \
	-I$(MESA) \
	-I$(MAPI) \
	-I$(MESA_INCLUDE)

CPPFLAGS+= $(INCLUDES) $(DEFINES)

CFLAGS+= -std=c99

all: ${LIB}_pic.a

obj:	_xenocara_obj

install:
	@echo "Not installing libglsl"

clean:
	rm -f ${LIB}_pic.a ${OBJS}

cleandir: clean

# Tweak suffixes: only .cpp is used for C++,
# ignore .l and .y to avoid rebuilding cpp-{lex,parse}.c
# which need bison/recent flex.
.SUFFIXES:
.SUFFIXES: .a .so .c .cpp

.c.so:
	@echo "${COMPILE.c} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.c} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

.cpp.so:
	@echo "${COMPILE.cc} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.cc} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${LIB}_pic.a:	${OBJS} $(DPADD)
	@rm -f ${LIB}_pic.a
	@${AR} cq ${LIB}_pic.a `${LORDER} ${OBJS} | tsort -q`
	${RANLIB} ${LIB}_pic.a

.include <bsd.xorg.mk>

.PATH: ${GLSL}
.PATH.cpp: ${GLSL}
.PATH: ${GLSL}/glcpp
.PATH: ${GLSL}/builtin_compiler
.PATH: ${.CURDIR}/../../generated/glsl
.PATH: ${.CURDIR}/../../generated/glsl/glcpp

.if ${MACHINE_ARCH} == powerpc || ${MACHINE_ARCH} == sparc
PICFLAG=-fPIC
.endif
