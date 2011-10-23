# $OpenBSD: Makefile,v 1.1 2011/10/23 13:57:36 matthieu Exp $
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
	glcpp.c

C_SOURCES = \
	strtod.c \
	ralloc.c \
	$(LIBGLCPP_SOURCES)

CXX_SOURCES = \
	ast_expr.cpp \
	ast_function.cpp \
	ast_to_hir.cpp \
	ast_type.cpp \
	builtin_function.cpp \
	glsl_lexer.cpp \
	glsl_parser.cpp \
	glsl_parser_extras.cpp \
	glsl_symbol_table.cpp \
	glsl_types.cpp \
	hir_field_selection.cpp \
	ir.cpp \
	ir_basic_block.cpp \
	ir_clone.cpp \
	ir_constant_expression.cpp \
	ir_expression_flattening.cpp \
	ir_function.cpp \
	ir_function_can_inline.cpp \
	ir_hierarchical_visitor.cpp \
	ir_hv_accept.cpp \
	ir_import_prototypes.cpp \
	ir_print_visitor.cpp \
	ir_reader.cpp \
	ir_rvalue_visitor.cpp \
	ir_set_program_inouts.cpp \
	ir_validate.cpp \
	ir_variable.cpp \
	ir_variable_refcount.cpp \
	link_functions.cpp \
	linker.cpp \
	loop_analysis.cpp \
	loop_controls.cpp \
	loop_unroll.cpp \
	lower_discard.cpp \
	lower_if_to_cond_assign.cpp \
	lower_instructions.cpp \
	lower_jumps.cpp \
	lower_mat_op_to_vec.cpp \
	lower_noise.cpp \
	lower_texture_projection.cpp \
	lower_variable_index_to_cond_assign.cpp \
	lower_vec_index_to_cond_assign.cpp \
	lower_vec_index_to_swizzle.cpp \
	lower_vector.cpp \
	main.cpp \
	opt_algebraic.cpp \
	opt_constant_folding.cpp \
	opt_constant_propagation.cpp \
	opt_constant_variable.cpp \
	opt_copy_propagation.cpp \
	opt_dead_code.cpp \
	opt_dead_code_local.cpp \
	opt_dead_functions.cpp \
	opt_discard_simplification.cpp \
	opt_function_inlining.cpp \
	opt_if_simplification.cpp \
	opt_noop_swizzle.cpp \
	opt_redundant_jumps.cpp \
	opt_structure_splitting.cpp \
	opt_swizzle_swizzle.cpp \
	opt_tree_grafting.cpp \
	s_expression.cpp

LIB=	glsl

SRCS=	\
	$(C_SOURCES) \
	$(CXX_SOURCES)

OBJS+= $(CXX_SOURCES:.cpp=.o)

INCLUDES= \
	-I${X11BASE}/include/GL \
	-I$(GLSL) \
	-I$(GLSL)/glcpp \
	-I$(MESA) \
	-I$(MAPI) \
	-I$(MESA_INCLUDE)

CPPFLAGS+= $(INCLUDES) $(DEFINES)

NOPROFILE=

obj:	_xenocara_obj

install:
	@echo "Not installing libglsl"

.include <bsd.lib.mk>

# Tweak suffixes: only .cpp is used for C++, 
# ignore .l and .y to avoid rebuilding cpp-{lex,parse}.c
# which need bison/recent flex.
.SUFFIXES:
.SUFFIXES: .out .o .go .po .so .S .s .c .cc .cpp .f  .ln .m4 .m

.cpp.o:
	@echo "${COMPILE.cc} ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.cc} ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

.cpp.so:
	@echo "${COMPILE.cc} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.cc} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

.include <bsd.xorg.mk>

.PATH: ${GLSL}
.PATH.cpp: ${GLSL}
.PATH: ${GLSL}/glcpp
