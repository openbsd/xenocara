.include <bsd.xconf.mk>

PYTHON=		python3
FLEX=		flex
BISON=		bison
MESA_SRC=	..

all:
	mkdir -p src/amd/common
	mkdir -p src/amd/compiler
	mkdir -p src/amd/vulkan
	mkdir -p src/compiler/glsl/glcpp
	mkdir -p src/compiler/nir
	mkdir -p src/compiler/spirv
	mkdir -p src/gallium/auxiliary
	mkdir -p src/gallium/drivers/iris
	mkdir -p src/gallium/drivers/r600
	mkdir -p src/gallium/drivers/radeonsi
	mkdir -p src/intel/compiler
	mkdir -p src/intel/genxml
	mkdir -p src/intel/isl
	mkdir -p src/intel/perf
	mkdir -p src/intel/vulkan
	mkdir -p src/mapi/es1api
	mkdir -p src/mapi/es2api
	mkdir -p src/mapi/glapi/gen
	mkdir -p src/mapi/shared-glapi
	mkdir -p src/mesa/main
	mkdir -p src/mesa/program
	mkdir -p src/util/format
	mkdir -p src/vulkan/util
	${PYTHON} ${MESA_SRC}/src/mapi/new/gen_gldispatch_mapi.py \
	    glesv1 ${MESA_SRC}/src/mapi/glapi/registry/gl.xml \
	    > src/mapi/es1api/glapi_mapi_tmp.h
	${PYTHON} ${MESA_SRC}/src/mapi/new/gen_gldispatch_mapi.py \
	    glesv2 ${MESA_SRC}/src/mapi/glapi/registry/gl.xml \
	    > src/mapi/es2api/glapi_mapi_tmp.h
	${PYTHON} ${MESA_SRC}/src/mapi/mapi_abi.py --printer glapi \
	    ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mapi/glapi/gen/glapi_mapi_tmp.h
	${PYTHON} ${MESA_SRC}/src/mapi/mapi_abi.py --printer shared-glapi \
	    ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mapi/shared-glapi/glapi_mapi_tmp.h
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/glX_proto_send.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_API.xml -m proto \
	    > src/mapi/glapi/gen/indirect.c
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/glX_proto_send.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_API.xml -m init_h \
	    > src/mapi/glapi/gen/indirect.h
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/glX_proto_send.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_API.xml -m init_c \
	    > src/mapi/glapi/gen/indirect_init.c
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/glX_proto_size.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_API.xml \
	    --only-set -m size_c \
	    > src/mapi/glapi/gen/indirect_size.c
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/glX_proto_size.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_API.xml \
	    --only-set -m size_h --header-tag _INDIRECT_SIZE_H_ \
	    > src/mapi/glapi/gen/indirect_size.h
	${PYTHON} ${MESA_SRC}/src/util/format_srgb.py \
	    > src/util/format_srgb.c
	${PYTHON} ${MESA_SRC}/src/compiler/nir/nir_opcodes_h.py \
	    > src/compiler/nir/nir_opcodes.h
	${PYTHON} ${MESA_SRC}/src/compiler/nir/nir_builder_opcodes_h.py \
	    > src/compiler/nir/nir_builder_opcodes.h
	${PYTHON} ${MESA_SRC}/src/compiler/nir/nir_intrinsics_h.py \
	    --outdir src/compiler/nir
	${PYTHON} ${MESA_SRC}/src/compiler/nir/nir_intrinsics_indices_h.py \
	    --outdir src/compiler/nir
	${PYTHON} ${MESA_SRC}/src/compiler/glsl/ir_expression_operation.py \
	    enum > src/compiler/ir_expression_operation.h
.for GEN in 4 45 5 6 7 75 8 9 11 12 125
	${PYTHON} ${MESA_SRC}/src/intel/genxml/gen_pack_header.py \
	    ${MESA_SRC}/src/intel/genxml/gen${GEN}.xml \
	    > src/intel/genxml/gen${GEN}_pack.h
.endfor
	${PYTHON} ${MESA_SRC}/src/intel/genxml/gen_pack_header.py \
	    ${MESA_SRC}/src/intel/genxml/gen_rt.xml \
	    > src/intel/genxml/gen_rt_pack.h
	${PYTHON} ${MESA_SRC}/src/intel/genxml/gen_bits_header.py \
	    ${MESA_SRC}/src/intel/genxml/gen4.xml \
	    ${MESA_SRC}/src/intel/genxml/gen45.xml \
	    ${MESA_SRC}/src/intel/genxml/gen5.xml \
	    ${MESA_SRC}/src/intel/genxml/gen6.xml \
	    ${MESA_SRC}/src/intel/genxml/gen7.xml \
	    ${MESA_SRC}/src/intel/genxml/gen75.xml \
	    ${MESA_SRC}/src/intel/genxml/gen8.xml \
	    ${MESA_SRC}/src/intel/genxml/gen9.xml \
	    ${MESA_SRC}/src/intel/genxml/gen11.xml \
	    ${MESA_SRC}/src/intel/genxml/gen12.xml \
	    ${MESA_SRC}/src/intel/genxml/gen125.xml \
	    -o src/intel/genxml/genX_bits.h
	${PYTHON} ${MESA_SRC}/src/gallium/drivers/r600/egd_tables.py \
	    ${MESA_SRC}/src/gallium/drivers/r600/evergreend.h \
	    > src/gallium/drivers/r600/egd_tables.h
	${PYTHON} ${MESA_SRC}/src/gallium/drivers/r600/sfn/sfn_nir_algebraic.py \
	    -p ${MESA_SRC}/src/compiler/nir/ \
	    > src/gallium/drivers/r600/sfn_nir_algebraic.c
	${PYTHON} ${MESA_SRC}/src/amd/registers/makeregheader.py \
	    ${MESA_SRC}/src/amd/registers/gfx6.json \
	    ${MESA_SRC}/src/amd/registers/gfx7.json \
	    ${MESA_SRC}/src/amd/registers/gfx8.json \
	    ${MESA_SRC}/src/amd/registers/gfx81.json \
	    ${MESA_SRC}/src/amd/registers/gfx9.json \
	    ${MESA_SRC}/src/amd/registers/gfx10.json \
	    ${MESA_SRC}/src/amd/registers/gfx103.json \
	    ${MESA_SRC}/src/amd/registers/pkt3.json \
	    ${MESA_SRC}/src/amd/registers/gfx10-rsrc.json \
	    ${MESA_SRC}/src/amd/registers/registers-manually-defined.json \
	    --sort address --guard AMDGFXREGS_H \
	    > src/amd/common/amdgfxregs.h
	${PYTHON} ${MESA_SRC}/src/amd/common/sid_tables.py \
	    ${MESA_SRC}/src/amd/common/sid.h \
	    ${MESA_SRC}/src/amd/registers/gfx6.json \
	    ${MESA_SRC}/src/amd/registers/gfx7.json \
	    ${MESA_SRC}/src/amd/registers/gfx8.json \
	    ${MESA_SRC}/src/amd/registers/gfx81.json \
	    ${MESA_SRC}/src/amd/registers/gfx9.json \
	    ${MESA_SRC}/src/amd/registers/gfx10.json \
	    ${MESA_SRC}/src/amd/registers/gfx103.json \
	    ${MESA_SRC}/src/amd/registers/pkt3.json \
	    ${MESA_SRC}/src/amd/registers/gfx10-rsrc.json \
	    ${MESA_SRC}/src/amd/registers/registers-manually-defined.json \
	    > src/amd/common/sid_tables.h
	${PYTHON} ${MESA_SRC}/src/amd/common/gfx10_format_table.py \
	    ${MESA_SRC}/src/util/format/u_format.csv \
	    ${MESA_SRC}/src/amd/registers/gfx10-rsrc.json \
	    > src/amd/common/gfx10_format_table.c
	${PYTHON} ${MESA_SRC}/src/util/xxd.py \
	    ${MESA_SRC}/src/compiler/glsl/float64.glsl \
	    src/compiler/glsl/float64_glsl.h -n float64_source
	${FLEX} -o src/compiler/glsl/glcpp/glcpp-lex.c \
	    ${MESA_SRC}/src/compiler/glsl/glcpp/glcpp-lex.l
	${BISON} -Wno-deprecated -o src/compiler/glsl/glcpp/glcpp-parse.c \
	    -p glcpp_parser_ --defines=src/compiler/glsl/glcpp/glcpp-parse.h \
	    ${MESA_SRC}/src/compiler/glsl/glcpp/glcpp-parse.y
	${FLEX} -o src/compiler/glsl/glsl_lexer.cpp \
	    ${MESA_SRC}/src/compiler/glsl/glsl_lexer.ll
	${BISON} -Wno-deprecated -o src/compiler/glsl/glsl_parser.cpp \
	    -p _mesa_glsl_ --defines=src/compiler/glsl/glsl_parser.h \
	    ${MESA_SRC}/src/compiler/glsl/glsl_parser.yy
	${PYTHON} ${MESA_SRC}/src/compiler/glsl/ir_expression_operation.py \
	    constant > src/compiler/glsl/ir_expression_operation_constant.h
	${PYTHON} ${MESA_SRC}/src/compiler/glsl/ir_expression_operation.py \
	    strings > src/compiler/glsl/ir_expression_operation_strings.h
	${PYTHON} ${MESA_SRC}/src/compiler/nir/nir_constant_expressions.py \
	    > src/compiler/nir/nir_constant_expressions.c
	${PYTHON} ${MESA_SRC}/src/compiler/nir/nir_intrinsics_c.py \
	    --outdir src/compiler/nir
	${PYTHON} ${MESA_SRC}/src/compiler/nir/nir_opcodes_c.py \
	    > src/compiler/nir/nir_opcodes.c
	${PYTHON} ${MESA_SRC}/src/compiler/nir/nir_opt_algebraic.py \
	    > src/compiler/nir/nir_opt_algebraic.c
	${PYTHON} ${MESA_SRC}/src/compiler/spirv/spirv_info_c.py \
	    ${MESA_SRC}/src/compiler/spirv/spirv.core.grammar.json \
	    src/compiler/spirv/spirv_info.c
	${PYTHON} ${MESA_SRC}/src/compiler/spirv/vtn_gather_types_c.py \
	    ${MESA_SRC}/src/compiler/spirv/spirv.core.grammar.json \
	    src/compiler/spirv/vtn_gather_types.c
	${PYTHON} ${MESA_SRC}/src/compiler/spirv/vtn_generator_ids_h.py \
	    ${MESA_SRC}/src/compiler/spirv/spir-v.xml \
	    src/compiler/spirv/vtn_generator_ids.h
	${PYTHON} ${MESA_SRC}/src/gallium/auxiliary/indices/u_indices_gen.py \
	    > src/gallium/auxiliary/u_indices_gen.c
	${PYTHON} ${MESA_SRC}/src/gallium/auxiliary/util/u_tracepoints.py \
	    -p ${MESA_SRC}/src/gallium/auxiliary/util/ \
	    -C src/gallium/auxiliary/u_tracepoints.c
	${PYTHON} ${MESA_SRC}/src/gallium/auxiliary/util/u_tracepoints.py \
	    -p ${MESA_SRC}/src/gallium/auxiliary/util/ \
	    -H src/gallium/auxiliary/u_tracepoints.h
	${PYTHON} ${MESA_SRC}/src/gallium/auxiliary/indices/u_unfilled_gen.py \
	    > src/gallium/auxiliary/u_unfilled_gen.c
	echo '#define MESA_GIT_SHA1 ""' > src/git_sha1.h
	${PYTHON} ${MESA_SRC}/src/intel/compiler/brw_nir_trig_workarounds.py \
	    -p ${MESA_SRC}/src/compiler/nir \
	    > src/intel/compiler/brw_nir_trig_workarounds.c
	${PYTHON} ${MESA_SRC}/src/intel/genxml/gen_zipped_file.py \
	    ${MESA_SRC}/src/intel/genxml/gen4.xml \
	    ${MESA_SRC}/src/intel/genxml/gen45.xml \
	    ${MESA_SRC}/src/intel/genxml/gen5.xml \
	    ${MESA_SRC}/src/intel/genxml/gen6.xml \
	    ${MESA_SRC}/src/intel/genxml/gen7.xml \
	    ${MESA_SRC}/src/intel/genxml/gen75.xml \
	    ${MESA_SRC}/src/intel/genxml/gen8.xml \
	    ${MESA_SRC}/src/intel/genxml/gen9.xml \
	    ${MESA_SRC}/src/intel/genxml/gen11.xml \
	    ${MESA_SRC}/src/intel/genxml/gen12.xml \
	    ${MESA_SRC}/src/intel/genxml/gen125.xml \
	    > src/intel/genxml/genX_xml.h
	${PYTHON} ${MESA_SRC}/src/intel/isl/gen_format_layout.py \
	    --csv ${MESA_SRC}/src/intel/isl/isl_format_layout.csv \
	    --out src/intel/isl/isl_format_layout.c
	${PYTHON} ${MESA_SRC}/src/intel/perf/gen_perf.py \
	    --code src/intel/perf/gen_perf_metrics.c \
	    --header src/intel/perf/gen_perf_metrics.h \
	    ${MESA_SRC}/src/intel/perf/oa-hsw.xml \
	    ${MESA_SRC}/src/intel/perf/oa-bdw.xml \
	    ${MESA_SRC}/src/intel/perf/oa-chv.xml \
	    ${MESA_SRC}/src/intel/perf/oa-sklgt2.xml \
	    ${MESA_SRC}/src/intel/perf/oa-sklgt3.xml \
	    ${MESA_SRC}/src/intel/perf/oa-sklgt4.xml \
	    ${MESA_SRC}/src/intel/perf/oa-kblgt2.xml \
	    ${MESA_SRC}/src/intel/perf/oa-kblgt3.xml \
	    ${MESA_SRC}/src/intel/perf/oa-cflgt2.xml \
	    ${MESA_SRC}/src/intel/perf/oa-cflgt3.xml \
	    ${MESA_SRC}/src/intel/perf/oa-bxt.xml \
	    ${MESA_SRC}/src/intel/perf/oa-glk.xml \
	    ${MESA_SRC}/src/intel/perf/oa-icl.xml \
	    ${MESA_SRC}/src/intel/perf/oa-ehl.xml \
	    ${MESA_SRC}/src/intel/perf/oa-tglgt1.xml \
	    ${MESA_SRC}/src/intel/perf/oa-tglgt2.xml \
	    ${MESA_SRC}/src/intel/perf/oa-rkl.xml \
	    ${MESA_SRC}/src/intel/perf/oa-dg1.xml \
	    ${MESA_SRC}/src/intel/perf/oa-adl.xml
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_entrypoints_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --proto --weak \
	    --out-h src/intel/vulkan/anv_entrypoints.h \
	    --out-c src/intel/vulkan/anv_entrypoints.c \
	    --prefix anv \
	    --device-prefix gfx7 \
	    --device-prefix gfx75 \
	    --device-prefix gfx8 \
	    --device-prefix gfx9 \
	    --device-prefix gfx11 \
	    --device-prefix gfx12 \
	    --device-prefix gfx125
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/gl_genexec.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mapi/glapi/gen/api_exec.c
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/gl_enums.py \
	    -f ${MESA_SRC}/src/mapi/glapi/registry/gl.xml \
	    > src/mapi/glapi/gen/enums.c
.for INDEX in 0 1 2 3 4 5 6 7
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/gl_marshal.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    -i ${INDEX} -n 8 \
	    > src/mapi/glapi/gen/marshal_generated${INDEX}.c
.endfor
	${PYTHON} ${MESA_SRC}/src/mesa/main/format_fallback.py \
	    ${MESA_SRC}/src/mesa/main/formats.csv \
	    src/mesa/format_fallback.c
	${PYTHON} ${MESA_SRC}/src/mesa/main/format_info.py \
	    ${MESA_SRC}/src/mesa/main/formats.csv \
	    > src/mesa/format_info.h
	${PYTHON} ${MESA_SRC}/src/mesa/main/format_pack.py \
	    ${MESA_SRC}/src/mesa/main/formats.csv \
	    > src/mesa/format_pack.c
	${PYTHON} ${MESA_SRC}/src/mesa/main/get_hash_generator.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mesa/get_hash.h
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/gl_table.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    -m remap_table > src/mesa/main/dispatch.h
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/gl_marshal_h.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mesa/main/marshal_generated.h
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/remap_helper.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mesa/main/remap_helper.h
	${FLEX} -o src/mesa/program/lex.yy.c \
	    ${MESA_SRC}/src/mesa/program/program_lexer.l
	${BISON} -Wno-deprecated -o src/mesa/program/program_parse.tab.c \
	    --defines=src/mesa/program/program_parse.tab.h \
	    ${MESA_SRC}/src/mesa/program/program_parse.y
	${PYTHON} ${MESA_SRC}/src/util/format/u_format_table.py \
	    ${MESA_SRC}/src/util/format/u_format.csv \
	    > src/util/format/u_format_table.c
	${PYTHON} ${MESA_SRC}/src/util/format/u_format_table.py \
	    ${MESA_SRC}/src/util/format/u_format.csv \
	    --header > src/util/format/u_format_pack.h
	${PYTHON} ${MESA_SRC}/src/vulkan/util/gen_enum_to_str.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --outdir src/vulkan/util
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/gl_table.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mapi/glapi/glapitable.h
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/gl_apitemp.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mapi/glapi/glapitemp.h
	${PYTHON} ${MESA_SRC}/src/amd/compiler/aco_opcodes_cpp.py \
	    > src/amd/compiler/aco_opcodes.cpp
	${PYTHON} ${MESA_SRC}/src/amd/compiler/aco_opcodes_h.py \
	    > src/amd/compiler/aco_opcodes.h
	${PYTHON} ${MESA_SRC}/src/amd/compiler/aco_builder_h.py \
	    > src/amd/compiler/aco_builder.h
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_entrypoints_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --proto --weak \
	    --out-h src/amd/vulkan/radv_entrypoints.h \
	    --out-c src/amd/vulkan/radv_entrypoints.c \
	    --prefix radv \
	    --device-prefix sqtt
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_icd_gen.py \
	    --api-version 1.2 \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --lib-path ${X11BASE}/lib/libvulkan_radeon.so \
	    --out src/amd/vulkan/radeon_icd.json
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_icd_gen.py \
	    --api-version 1.2 \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --lib-path ${X11BASE}/lib/libvulkan_intel.so \
	    --out src/intel/vulkan/intel_icd.json
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_entrypoints_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --proto --weak \
	    --out-h src/vulkan/util/vk_common_entrypoints.h \
	    --out-c src/vulkan/util/vk_common_entrypoints.c \
	    --prefix vk_common
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_dispatch_table_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --out-c src/vulkan/util/vk_dispatch_table.c \
	    --out-h src/vulkan/util/vk_dispatch_table.h
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_extensions_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --out-c src/vulkan/util/vk_extensions.c \
	    --out-h src/vulkan/util/vk_extensions.h
	${PYTHON} ${MESA_SRC}/src/util/driconf_static.py \
	    ${MESA_SRC}/src/util/00-mesa-defaults.conf \
	    src/util/driconf_static.h

clean:
	rm -f src/amd/common/amdgfxregs.h
	rm -f src/amd/common/gfx10_format_table.c
	rm -f src/amd/common/sid_tables.h
	rm -f src/amd/compiler/aco_builder.h
	rm -f src/amd/compiler/aco_opcodes.cpp
	rm -f src/amd/compiler/aco_opcodes.h
	rm -f src/amd/vulkan/radeon_icd.json
	rm -f src/amd/vulkan/radv_entrypoints.c
	rm -f src/amd/vulkan/radv_entrypoints.h
	rm -f src/compiler/glsl/float64_glsl.h
	rm -f src/compiler/glsl/glcpp/glcpp-lex.c
	rm -f src/compiler/glsl/glcpp/glcpp-parse.c
	rm -f src/compiler/glsl/glcpp/glcpp-parse.h
	rm -f src/compiler/glsl/glsl_lexer.cpp
	rm -f src/compiler/glsl/glsl_parser.cpp
	rm -f src/compiler/glsl/glsl_parser.h
	rm -f src/compiler/glsl/ir_expression_operation_constant.h
	rm -f src/compiler/glsl/ir_expression_operation_strings.h
	rm -f src/compiler/ir_expression_operation.h
	rm -f src/compiler/nir/nir_builder_opcodes.h
	rm -f src/compiler/nir/nir_constant_expressions.c
	rm -f src/compiler/nir/nir_intrinsics.c
	rm -f src/compiler/nir/nir_intrinsics.h
	rm -f src/compiler/nir/nir_intrinsics_indices.h
	rm -f src/compiler/nir/nir_opcodes.c
	rm -f src/compiler/nir/nir_opcodes.h
	rm -f src/compiler/nir/nir_opt_algebraic.c
	rm -f src/compiler/spirv/spirv_info.c
	rm -f src/compiler/spirv/vtn_gather_types.c
	rm -f src/compiler/spirv/vtn_generator_ids.h
	rm -f src/gallium/auxiliary/u_indices_gen.c
	rm -f src/gallium/auxiliary/u_tracepoints.c
	rm -f src/gallium/auxiliary/u_tracepoints.h
	rm -f src/gallium/auxiliary/u_unfilled_gen.c
	rm -f src/gallium/drivers/r600/egd_tables.h
	rm -f src/gallium/drivers/r600/sfn_nir_algebraic.c
	rm -f src/git_sha1.h
	rm -f src/intel/compiler/brw_nir_trig_workarounds.c
.for GEN in 4 45 5 6 7 75 8 9 11 12 125
	rm -f src/intel/genxml/gen${GEN}_pack.h
.endfor
	rm -f src/intel/genxml/genX_bits.h
	rm -f src/intel/genxml/genX_xml.h
	rm -f src/intel/genxml/gen_rt_pack.h
	rm -f src/intel/isl/isl_format_layout.c
	rm -f src/intel/perf/gen_perf_metrics.c
	rm -f src/intel/perf/gen_perf_metrics.h
	rm -f src/intel/vulkan/anv_entrypoints.c
	rm -f src/intel/vulkan/anv_entrypoints.h
	rm -f src/intel/vulkan/intel_icd.json
	rm -f src/mapi/es1api/glapi_mapi_tmp.h
	rm -f src/mapi/es2api/glapi_mapi_tmp.h
	rm -f src/mapi/glapi/gen/api_exec.c
	rm -f src/mapi/glapi/gen/enums.c
	rm -f src/mapi/glapi/gen/glapi_mapi_tmp.h
	rm -f src/mapi/glapi/gen/indirect.c
	rm -f src/mapi/glapi/gen/indirect.h
	rm -f src/mapi/glapi/gen/indirect_init.c
	rm -f src/mapi/glapi/gen/indirect_size.c
	rm -f src/mapi/glapi/gen/indirect_size.h
.for INDEX in 0 1 2 3 4 5 6 7
	rm -f src/mapi/glapi/gen/marshal_generated${INDEX}.c
.endfor
	rm -f src/mapi/glapi/glapitable.h
	rm -f src/mapi/glapi/glapitemp.h
	rm -f src/mapi/shared-glapi/glapi_mapi_tmp.h
	rm -f src/mesa/format_fallback.c
	rm -f src/mesa/format_info.h
	rm -f src/mesa/format_pack.c
	rm -f src/mesa/get_hash.h
	rm -f src/mesa/main/dispatch.h
	rm -f src/mesa/main/marshal_generated.h
	rm -f src/mesa/main/remap_helper.h
	rm -f src/mesa/program/lex.yy.c
	rm -f src/mesa/program/program_parse.tab.c
	rm -f src/mesa/program/program_parse.tab.h
	rm -f src/util/driconf_static.h
	rm -f src/util/format/u_format_pack.h
	rm -f src/util/format/u_format_table.c
	rm -f src/util/format_srgb.c
	rm -f src/vulkan/util/vk_common_entrypoints.c
	rm -f src/vulkan/util/vk_common_entrypoints.h
	rm -f src/vulkan/util/vk_dispatch_table.c
	rm -f src/vulkan/util/vk_dispatch_table.h
	rm -f src/vulkan/util/vk_enum_to_str.c
	rm -f src/vulkan/util/vk_enum_to_str.h
	rm -f src/vulkan/util/vk_extensions.c
	rm -f src/vulkan/util/vk_extensions.h
