# $OpenBSD: Makefile,v 1.4 2015/02/20 23:31:05 jsg Exp $

MESA=		${.CURDIR}/../../../dist/Mesa/src/mesa
GLAPI=		${.CURDIR}/../../../dist/Mesa/src/mapi/glapi
MAPI=		${.CURDIR}/../../../dist/Mesa/src/mapi
XMLPOOL=	${MESA}/drivers/dri/common/xmlpool
AUX=		${.CURDIR}/../../../dist/Mesa/src/gallium/auxiliary
DEV=		${.CURDIR}/../../../dist/Mesa/src/gallium/drivers
GLSL=		${.CURDIR}/../../../dist/Mesa/src/glsl
COMP=		${.CURDIR}/../glsl/builtin_compiler/obj

# py-libxml required as well...
PYTHON=		python${PYTHON_VERSION}
PYTHON_FLAGS=	-t -O -O
FLEX=		gflex
BISON=		bison

# src/glx
GEN_GLX= \
	glx/indirect.c glx/indirect.h \
	glx/indirect_init.c \
	glx/indirect_size.c glx/indirect_size.h

# src/mapi/glapi
GEN_GLAPI= \
	glapi/glapi_x86.S glapi/glapi_x86-64.S glapi/glapi_sparc.S \
	glapi/glapi_gentable.c glapi/glapitable.h glapi/glapitemp.h \
	glapi/glprocs.h glapi/glapi_mapi_tmp.h

# src/mesa/drivers/dri/common/xmlpool/
GEN_XMLPOOL= \
	xmlpool/options.h

# src/gallium/auxiliary/util/
GEN_UTIL= \
	util/u_indices_gen.c util/u_unfilled_gen.c util/u_format_srgb.c \
	util/u_format_table.c

# src/glsl
YACC_GLSL= glsl/glsl_lexer.cpp glsl/glsl_parser.cpp
GEN_GLSL= \
	${YACC_GLSL}

# src/glsl/glcpp
YACC_GLCPP= glsl/glcpp/glcpp-lex.c glsl/glcpp/glcpp-parse.c
GEN_GLCPP= ${YACC_GLCPP}

# src/mesa/main
GEN_MAIN= \
	main/api_exec.c main/enums.c main/get_hash.h main/git_sha1.h \
	main/dispatch.h main/remap_helper.h

# src/mesa/program
YACC_PROG= program/lex.yy.c program/program_parse.tab.c
GEN_PROG= ${YACC_PROG}

GENERATED= \
	${GEN_GLX} ${GEN_GLAPI} ${GEN_XMLPOOL} ${GEN_UTIL} \
	${GEN_GLSL} ${GEN_GLCPP} ${GEN_MAIN} ${GEN_PROG}

all: ${GENERATED}

yacc: ${YACC_GLSL} ${YACC_GLCPP} ${YACC_PROG}

clean-yacc:
	rm -f ${YACC_GLSL} ${YACC_GLCPP} ${YACC_PROG}

obj:

depend:

install:

clean distclean:
	rm -f ${GENERATED}

glx/indirect.c: gl_API.xml glX_proto_send.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/glX_proto_send.py \
	    -f ${GLAPI}/gen/gl_API.xml -m proto \
	    | indent -i4 -nut -br -brs -npcs -ce -TGLubyte -TGLbyte -TBool > $@

glx/indirect.h: gl_API.xml glX_proto_send.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/glX_proto_send.py \
	    -f ${GLAPI}/gen/gl_API.xml -m init_h > $@

glx/indirect_init.c: gl_API.xml glX_proto_send.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/glX_proto_send.py \
	    -f ${GLAPI}/gen/gl_API.xml -m init_c > $@

glx/indirect_size.h: gl_API.xml glX_proto_size.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/glX_proto_size.py \
	    -f ${GLAPI}/gen/gl_API.xml -m size_h --only-set \
	    -h _INDIRECT_SIZE_H_ \
	    | indent -i4 -nut -br -brs -npcs -ce -TGLubyte -TGLbyte -TBool > $@

glx/indirect_size.c: gl_API.xml glX_proto_size.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/glX_proto_size.py \
	    -f ${GLAPI}/gen/gl_API.xml -m size_c --only-set \
	    | indent -i4 -nut -br -brs -npcs -ce -TGLubyte -TGLbyte -TBool > $@

glapi/glapi_x86.S: gl_and_es_API.xml gl_x86_asm.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/gl_x86_asm.py \
	    -f ${GLAPI}/gen/gl_and_es_API.xml > $@

glapi/glapi_x86-64.S: gl_and_es_API.xml gl_x86-64_asm.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/gl_x86-64_asm.py \
	    -f ${GLAPI}/gen/gl_and_es_API.xml > $@
 
glapi/glapi_sparc.S: gl_and_es_API.xml gl_SPARC_asm.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/gl_SPARC_asm.py \
	    -f ${GLAPI}/gen/gl_and_es_API.xml > $@

glapi/glapi_gentable.c: gl_and_es_API.xml gl_gentable.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/gl_gentable.py \
	    -f ${GLAPI}/gen/gl_and_es_API.xml > $@

glapi/glapitable.h: gl_and_es_API.xml gl_table.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/gl_table.py \
	    -f ${GLAPI}/gen/gl_and_es_API.xml > $@

glapi/glapitemp.h: gl_and_es_API.xml gl_apitemp.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/gl_apitemp.py \
	    -f ${GLAPI}/gen/gl_and_es_API.xml > $@

glapi/glprocs.h: gl_and_es_API.xml gl_procs.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/gl_procs.py \
	    -c -f ${GLAPI}/gen/gl_and_es_API.xml > $@

glapi/glapi_mapi_tmp.h: gl_and_es_API.xml mapi_abi.py
	$(PYTHON) $(PYTHON_FLAGS) ${MAPI}/mapi_abi.py \
	    --printer glapi --mode lib ${GLAPI}/gen/gl_and_es_API.xml > $@

xmlpool/options.h: gen_xmlpool.py t_options.h
	$(PYTHON) $(PYTHON_FLAGS) ${XMLPOOL}/gen_xmlpool.py \
	    ${XMLPOOL}/t_options.h ${XMLPOOL}/ de es nl fr sv > $@

util/u_indices_gen.c: u_indices_gen.py
	${PYTHON} ${PYTHON_FLAGS} ${AUX}/indices/u_indices_gen.py > $@

util/u_unfilled_gen.c: u_unfilled_gen.py
	${PYTHON} ${PYTHON_FLAGS} ${AUX}/indices/u_unfilled_gen.py > $@

util/u_format_srgb.c: ${AUX}/util/u_format_srgb.py
	${PYTHON} ${PYTHON_FLAGS} ${AUX}/util/u_format_srgb.py > $@

util/u_format_table.c: u_format_table.py u_format_pack.py u_format_parse.py u_format.csv
	${PYTHON} ${PYTHON_FLAGS} ${AUX}/util/u_format_table.py ${AUX}/util/u_format.csv > $@

glsl/glsl_lexer.cpp: ${GLSL}/glsl_lexer.ll
	$(FLEX) -o $@ ${GLSL}/glsl_lexer.ll

glsl/glsl_parser.cpp: ${GLSL}/glsl_parser.yy
	$(BISON) -o $@ -p "_mesa_glsl_" --defines=glsl/glsl_parser.h ${GLSL}/glsl_parser.yy

glsl/glcpp/glcpp-lex.c: ${GLSL}/glcpp/glcpp-lex.l
	$(FLEX) -o $@ ${GLSL}/glcpp/glcpp-lex.l

glsl/glcpp/glcpp-parse.c: ${GLSL}/glcpp/glcpp-parse.y
	$(BISON) -o $@ -p "glcpp_parser_" --defines=glsl/glcpp/glcpp-parse.h ${GLSL}/glcpp/glcpp-parse.y

program/lex.yy.c: ${MESA}/program/program_lexer.l
	$(FLEX) --never-interactive -o $@ ${MESA}/program/program_lexer.l

program/program_parse.tab.c: ${MESA}/program/program_parse.y
	$(BISON) -p "_mesa_program_" -v -d --output=program/program_parse.tab.c ${MESA}/program/program_parse.y

main/api_exec.c: gl_and_es_API.xml gl_genexec.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/gl_genexec.py \
	    -f ${GLAPI}/gen/gl_and_es_API.xml > $@

main/enums.c: gl_and_es_API.xml gl_enums.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/gl_enums.py \
	    -f ${GLAPI}/gen/gl_and_es_API.xml > $@

main/get_hash.h: get_hash_generator.py
	$(PYTHON) $(PYTHON_FLAGS) ${MESA}/main/get_hash_generator.py \
	    -f ${GLAPI}/gen/gl_and_es_API.xml > $@

main/git_sha1.h:
	touch $@

main/dispatch.h: gl_and_es_API.xml gl_table.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/gl_table.py \
	    -f ${GLAPI}/gen/gl_and_es_API.xml -m remap_table > $@

main/remap_helper.h: gl_and_es_API.xml remap_helper.py
	$(PYTHON) $(PYTHON_FLAGS) ${GLAPI}/gen/remap_helper.py \
	    -f ${GLAPI}/gen/gl_and_es_API.xml > $@

.SUFFIXES: .py .xml

.PATH:	${MESA}/main
.PATH:	${GLAPI}/gen
.PATH:	${MAPI}
.PATH:	${XMLPOOL}
.PATH:	${AUX}/indices
.PATH:	${AUX}/util

.include <bsd.xorg.mk>
