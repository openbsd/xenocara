.include <bsd.xconf.mk>

PYTHON=		python3
FLEX=		flex
BISON=		bison
GLSLV=		glslangValidator
MESA_CLC=	../build/src/compiler/clc/mesa_clc
VTN_BINDGEN=	../build/src/compiler/spirv/vtn_bindgen
MESA_SRC=	..

all:
	mkdir -p src/amd/common
	mkdir -p src/amd/compiler
	mkdir -p src/amd/vulkan/bvh
	mkdir -p src/amd/vulkan/layers
	mkdir -p src/compiler/glsl/glcpp
	mkdir -p src/compiler/nir
	mkdir -p src/compiler/spirv
	mkdir -p src/gallium/auxiliary
	mkdir -p src/gallium/drivers/r300
	mkdir -p src/gallium/drivers/r600
	mkdir -p src/gallium/drivers/radeonsi
	mkdir -p src/gallium/targets/dri
	mkdir -p src/gallium/targets/dril
	mkdir -p src/gallium/targets/va
	mkdir -p src/intel/compiler/elk
	mkdir -p src/intel/dev
	mkdir -p src/intel/ds
	mkdir -p src/intel/genxml
	mkdir -p src/intel/isl
	mkdir -p src/intel/perf
	mkdir -p src/intel/shaders
	mkdir -p src/intel/vulkan/bvh
	mkdir -p src/intel/vulkan/shaders
	mkdir -p src/intel/vulkan_hasvk
	mkdir -p src/mapi/es1api
	mkdir -p src/mapi/es2api
	mkdir -p src/mapi/glapi/gen
	mkdir -p src/mapi/shared-glapi
	mkdir -p src/mesa/main
	mkdir -p src/mesa/program
	mkdir -p src/util/format
	mkdir -p src/vulkan/runtime/bvh
	mkdir -p src/vulkan/runtime/radix_sort/shaders
	mkdir -p src/vulkan/util
	mkdir -p src/vulkan/wsi
	${PYTHON} ${MESA_SRC}/src/mapi/new/gen_gldispatch_mapi.py \
	    glesv1 ${MESA_SRC}/src/mapi/glapi/registry/gl.xml \
	    > src/mapi/es1api/glapi_mapi_tmp.h
	${PYTHON} ${MESA_SRC}/src/mapi/new/gen_gldispatch_mapi.py \
	    glesv2 ${MESA_SRC}/src/mapi/glapi/registry/gl.xml \
	    > src/mapi/es2api/glapi_mapi_tmp.h
	${PYTHON} ${MESA_SRC}/src/mapi/mapi_abi.py --printer glapi \
	    ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mapi/glapi/gen/glapi_mapi_tmp.h
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/api_exec_decl_h.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mapi/glapi/gen/api_exec_decl.h
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/api_save_h.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mapi/glapi/gen/api_save.h
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/api_save_init_h.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mapi/glapi/gen/api_save_init.h
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/api_beginend_init_h.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mapi/glapi/gen/api_beginend_init.h
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/gl_unmarshal_table.py \
	    ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mapi/glapi/gen/unmarshal_table.c 8
.for INDEX in 0 1 2 3 4 5 6 7
# XXX GEN 8 SIZEOF_POINTER
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/gl_marshal.py \
	    ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    ${INDEX} 8 8 \
	    > src/mapi/glapi/gen/marshal_generated${INDEX}.c
.endfor
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
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/api_hw_select_init_h.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_API.xml \
	    > src/mapi/glapi/gen/api_hw_select_init.h
	${PYTHON} ${MESA_SRC}/src/compiler/builtin_types_h.py \
	    src/compiler/builtin_types.h
	${PYTHON} ${MESA_SRC}/src/compiler/builtin_types_c.py \
	    src/compiler/builtin_types.c
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
	${PYTHON} ${MESA_SRC}/src/intel/dev/gen_wa_helpers.py \
	    ${MESA_SRC}/src/intel/dev/mesa_defs.json \
	    src/intel/dev/intel_wa.h \
	    src/intel/dev/intel_wa.c
.for GEN in 40 45 50 60 70 75 80 90 110 120 125 200 300
	${PYTHON} ${MESA_SRC}/src/intel/genxml/gen_pack_header.py \
	    ${MESA_SRC}/src/intel/genxml/gen${GEN}.xml \
	    --engines=render,blitter,video \
	    > src/intel/genxml/gen${GEN}_pack.h
.endfor
.for GEN in 125 200 300
	${PYTHON} ${MESA_SRC}/src/intel/genxml/gen_pack_header.py \
	    ${MESA_SRC}/src/intel/genxml/gen${GEN}_rt.xml \
	    --engines=render,blitter,video \
	    > src/intel/genxml/gen${GEN}_rt_pack.h
.endfor
.for GEN in 40 45 50 60 70 75 80 90 110 120 125 200 300
	${PYTHON} ${MESA_SRC}/src/intel/genxml/gen_pack_header.py \
	    ${MESA_SRC}/src/intel/genxml/gen${GEN}.xml \
	    --engines=render,blitter,video --opencl --repack \
	    --include-symbols 3DSTATE_DS,3DSTATE_HS,3DSTATE_INDEX_BUFFER,3DSTATE_VERTEX_BUFFERS,3DPRIMITIVE,3DPRIMITIVE_EXTENDED,MI_ARB_CHECK,MI_BATCH_BUFFER_START,VERTEX_BUFFER_STATE  \
	    > src/intel/genxml/gen${GEN}_cl_pack.h
.endfor
.for GEN in 125 200 300
	${PYTHON} ${MESA_SRC}/src/intel/genxml/gen_pack_header.py \
	    ${MESA_SRC}/src/intel/genxml/gen${GEN}_rt.xml \
	    --engines=render,blitter,video --opencl --repack \
	    --include-symbols 3DSTATE_DS,3DSTATE_HS,3DSTATE_INDEX_BUFFER,3DSTATE_VERTEX_BUFFERS,3DPRIMITIVE,3DPRIMITIVE_EXTENDED,MI_ARB_CHECK,MI_BATCH_BUFFER_START,VERTEX_BUFFER_STATE \
	    > src/intel/genxml/gen${GEN}_rt_cl_pack.h
.endfor
.for GEN in 80 90 110 120 125 200 300
	${MESA_CLC} ${MESA_SRC}/src/intel/shaders/libintel_shaders.h \
	    ${MESA_SRC}/src/intel/shaders/generate.cl \
	    ${MESA_SRC}/src/intel/shaders/generate_draws.cl \
	    ${MESA_SRC}/src/intel/shaders/generate_draws_iris.cl \
	    ${MESA_SRC}/src/intel/shaders/query_copy.cl \
	    ${MESA_SRC}/src/intel/shaders/util.cl \
	    -o src/intel/shaders/intel_gfx${GEN}_shaders.spv \
	    -- -DNDEBUG=1 -DGFX_VERx10=${GEN} \
	    -I${MESA_SRC}/src/intel/shaders \
	    -I${MESA_SRC}/src/compiler/libcl \
	    -I${MESA_SRC}/src \
	    -I${MESA_SRC}/src/intel \
	    -Isrc/intel \
	    -I${MESA_SRC}/src/intel/genxml
	${VTN_BINDGEN} \
	    src/intel/shaders/intel_gfx${GEN}_shaders.spv \
	    src/intel/shaders/intel_gfx${GEN}_shaders_binding.h
.endfor
	${PYTHON} ${MESA_SRC}/src/intel/shaders/spv2hex.py \
	    src/intel/shaders/intel_gfx80_shaders.spv \
	    --output src/intel/shaders/intel_gfx80_shaders_spv.h \
	    --prefix gfx8_shaders_spv
	${PYTHON} ${MESA_SRC}/src/intel/shaders/spv2hex.py \
	    src/intel/shaders/intel_gfx90_shaders.spv \
	    --output src/intel/shaders/intel_gfx90_shaders_spv.h \
	    --prefix gfx9_shaders_spv
	${PYTHON} ${MESA_SRC}/src/intel/shaders/spv2hex.py \
	    src/intel/shaders/intel_gfx110_shaders.spv \
	    --output src/intel/shaders/intel_gfx110_shaders_spv.h \
	    --prefix gfx11_shaders_spv
	${PYTHON} ${MESA_SRC}/src/intel/shaders/spv2hex.py \
	    src/intel/shaders/intel_gfx120_shaders.spv \
	    --output src/intel/shaders/intel_gfx120_shaders_spv.h \
	    --prefix gfx12_shaders_spv
	${PYTHON} ${MESA_SRC}/src/intel/shaders/spv2hex.py \
	    src/intel/shaders/intel_gfx125_shaders.spv \
	    --output src/intel/shaders/intel_gfx125_shaders_spv.h \
	    --prefix gfx125_shaders_spv
	${PYTHON} ${MESA_SRC}/src/intel/shaders/spv2hex.py \
	    src/intel/shaders/intel_gfx200_shaders.spv \
	    --output src/intel/shaders/intel_gfx200_shaders_spv.h \
	    --prefix gfx20_shaders_spv
	${PYTHON} ${MESA_SRC}/src/intel/shaders/spv2hex.py \
	    src/intel/shaders/intel_gfx300_shaders.spv \
	    --output src/intel/shaders/intel_gfx300_shaders_spv.h \
	    --prefix gfx30_shaders_spv
	${PYTHON} ${MESA_SRC}/src/intel/dev/intel_device_info_gen_h.py \
	    src/intel/dev/intel_device_info_gen.h
	${PYTHON} ${MESA_SRC}/src/intel/genxml/gen_bits_header.py \
	    ${MESA_SRC}/src/intel/genxml/gen40.xml \
	    ${MESA_SRC}/src/intel/genxml/gen45.xml \
	    ${MESA_SRC}/src/intel/genxml/gen50.xml \
	    ${MESA_SRC}/src/intel/genxml/gen60.xml \
	    ${MESA_SRC}/src/intel/genxml/gen70.xml \
	    ${MESA_SRC}/src/intel/genxml/gen75.xml \
	    ${MESA_SRC}/src/intel/genxml/gen80.xml \
	    ${MESA_SRC}/src/intel/genxml/gen90.xml \
	    ${MESA_SRC}/src/intel/genxml/gen110.xml \
	    ${MESA_SRC}/src/intel/genxml/gen120.xml \
	    ${MESA_SRC}/src/intel/genxml/gen125.xml \
	    ${MESA_SRC}/src/intel/genxml/gen200.xml \
	    ${MESA_SRC}/src/intel/genxml/gen300.xml \
	    ${MESA_SRC}/src/intel/genxml/gen125_rt.xml \
	    ${MESA_SRC}/src/intel/genxml/gen200_rt.xml \
	    ${MESA_SRC}/src/intel/genxml/gen300_rt.xml \
	    -o src/intel/genxml/genX_bits.h \
	    --include-symbols \
	    'MI_BATCH_BUFFER_START::Batch Buffer Start Address,MI_REPORT_PERF_COUNT::Memory Address,MI_STORE_DATA_IMM::Address,MI_STORE_DATA_IMM::Immediate Data,MI_STORE_REGISTER_MEM::Memory Address,3DSTATE_DEPTH_BUFFER::Surface Base Address,3DSTATE_DEPTH_BUFFER::Surface Pitch,3DSTATE_STENCIL_BUFFER::Surface Base Address,3DSTATE_STENCIL_BUFFER::Surface Pitch,3DSTATE_HIER_DEPTH_BUFFER::Surface Base Address,3DSTATE_HIER_DEPTH_BUFFER::Surface Pitch,3DSTATE_CLEAR_PARAMS,3DSTATE_SO_BUFFER::Surface Base Address,3DSTATE_SO_BUFFER::Stream Offset,3DSTATE_CPSIZE_CONTROL_BUFFER::Surface Base Address,3DSTATE_CPSIZE_CONTROL_BUFFER::Surface Pitch,RENDER_SURFACE_STATE::Surface Base Address,RENDER_SURFACE_STATE::Surface Pitch,RENDER_SURFACE_STATE::Auxiliary Surface Base Address,RENDER_SURFACE_STATE::Auxiliary Surface Pitch,RENDER_SURFACE_STATE::Clear Value Address,RENDER_SURFACE_STATE::Red Clear Color,RENDER_SURFACE_STATE::Green Clear Color,RENDER_SURFACE_STATE::Blue Clear Color,RENDER_SURFACE_STATE::Alpha Clear Color,RENDER_SURFACE_STATE::Width,RENDER_SURFACE_STATE::Height,RENDER_SURFACE_STATE::Depth,RENDER_SURFACE_STATE::Surface Type,RENDER_SURFACE_STATE::Render Target View Extent,CLEAR_COLOR,VERTEX_BUFFER_STATE::Buffer Starting Address,CPS_STATE,RT_DISPATCH_GLOBALS::Hit Group Table,RT_DISPATCH_GLOBALS::Miss Group Table,RT_DISPATCH_GLOBALS::Callable Group Table,RT_DISPATCH_GLOBALS::Launch Width,RT_DISPATCH_GLOBALS::Launch Height,RT_DISPATCH_GLOBALS::Launch Depth'
	${PYTHON} ${MESA_SRC}/src/gallium/drivers/r300/compiler/r300_nir_algebraic.py \
	    -p ${MESA_SRC}/src/compiler/nir/ \
	    src/gallium/drivers/r300/r300_nir_algebraic.c
	${PYTHON} ${MESA_SRC}/src/gallium/drivers/r600/egd_tables.py \
	    ${MESA_SRC}/src/gallium/drivers/r600/evergreend.h \
	    > src/gallium/drivers/r600/egd_tables.h
	${PYTHON} ${MESA_SRC}/src/gallium/drivers/radeonsi/si_tracepoints.py \
	    -p ${MESA_SRC}/src/util/perf/ \
	    -C src/gallium/drivers/radeonsi/si_tracepoints.c \
	    --perfetto-hdr src/gallium/drivers/radeonsi/si_tracepoints_perfetto.h \
	    -H src/gallium/drivers/radeonsi/si_tracepoints.h
	${PYTHON} ${MESA_SRC}/src/amd/registers/makeregheader.py \
	    ${MESA_SRC}/src/amd/registers/gfx6.json \
	    ${MESA_SRC}/src/amd/registers/gfx7.json \
	    ${MESA_SRC}/src/amd/registers/gfx8.json \
	    ${MESA_SRC}/src/amd/registers/gfx81.json \
	    ${MESA_SRC}/src/amd/registers/gfx9.json \
	    ${MESA_SRC}/src/amd/registers/gfx940.json \
	    ${MESA_SRC}/src/amd/registers/gfx10.json \
	    ${MESA_SRC}/src/amd/registers/gfx103.json \
	    ${MESA_SRC}/src/amd/registers/gfx11.json \
	    ${MESA_SRC}/src/amd/registers/gfx115.json \
	    ${MESA_SRC}/src/amd/registers/gfx12.json \
	    ${MESA_SRC}/src/amd/registers/pkt3.json \
	    ${MESA_SRC}/src/amd/registers/gfx10-rsrc.json \
	    ${MESA_SRC}/src/amd/registers/gfx11-rsrc.json \
	    ${MESA_SRC}/src/amd/registers/gfx12-rsrc.json \
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
	    ${MESA_SRC}/src/amd/registers/gfx940.json \
	    ${MESA_SRC}/src/amd/registers/gfx10.json \
	    ${MESA_SRC}/src/amd/registers/gfx103.json \
	    ${MESA_SRC}/src/amd/registers/gfx11.json \
	    ${MESA_SRC}/src/amd/registers/gfx115.json \
	    ${MESA_SRC}/src/amd/registers/gfx12.json \
	    ${MESA_SRC}/src/amd/registers/pkt3.json \
	    ${MESA_SRC}/src/amd/registers/gfx10-rsrc.json \
	    ${MESA_SRC}/src/amd/registers/gfx11-rsrc.json \
	    ${MESA_SRC}/src/amd/registers/gfx12-rsrc.json \
	    ${MESA_SRC}/src/amd/registers/registers-manually-defined.json \
	    > src/amd/common/sid_tables.h
	${PYTHON} ${MESA_SRC}/src/amd/common/gfx10_format_table.py \
	    ${MESA_SRC}/src/util/format/u_format.yaml \
	    ${MESA_SRC}/src/amd/registers/gfx10-rsrc.json \
	    ${MESA_SRC}/src/amd/registers/gfx11-rsrc.json \
	    > src/amd/common/gfx10_format_table.c
	${PYTHON} ${MESA_SRC}/src/util/xxd.py \
	    ${MESA_SRC}/src/compiler/glsl/bc1.glsl \
	    src/compiler/glsl/bc1_glsl.h -n bc1_source
	${PYTHON} ${MESA_SRC}/src/util/xxd.py \
	    ${MESA_SRC}/src/compiler/glsl/bc4.glsl \
	    src/compiler/glsl/bc4_glsl.h -n bc4_source
	${PYTHON} ${MESA_SRC}/src/util/xxd.py \
	    ${MESA_SRC}/src/compiler/glsl/etc2_rgba_stitch.glsl \
	    src/compiler/glsl/etc2_rgba_stitch_glsl.h -n etc2_rgba_stitch_source
	${PYTHON} ${MESA_SRC}/src/util/xxd.py \
	    ${MESA_SRC}/src/compiler/glsl/astc_decoder.glsl \
	    src/compiler/glsl/astc_glsl.h -n astc_source
	${PYTHON} ${MESA_SRC}/src/util/xxd.py \
	    ${MESA_SRC}/src/compiler/glsl/float64.glsl \
	    src/compiler/glsl/float64_glsl.h -n float64_source
	${PYTHON} ${MESA_SRC}/src/util/xxd.py \
	    ${MESA_SRC}/src/compiler/glsl/CrossPlatformSettings_piece_all.glsl \
	    src/compiler/glsl/cross_platform_settings_piece_all.h \
	    -n cross_platform_settings_piece_all_header
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
	    --out src/compiler/nir/nir_opt_algebraic.c
	${PYTHON} ${MESA_SRC}/src/compiler/spirv/spirv_info_gen.py \
	    --json ${MESA_SRC}/src/compiler/spirv/spirv.core.grammar.json \
	    --out-h src/compiler/spirv/spirv_info.h \
	    --out-c src/compiler/spirv/spirv_info.c
	${PYTHON} ${MESA_SRC}/src/compiler/spirv/vtn_gather_types_c.py \
	    ${MESA_SRC}/src/compiler/spirv/spirv.core.grammar.json \
	    src/compiler/spirv/vtn_gather_types.c
	${PYTHON} ${MESA_SRC}/src/compiler/spirv/vtn_generator_ids_h.py \
	    ${MESA_SRC}/src/compiler/spirv/spir-v.xml \
	    src/compiler/spirv/vtn_generator_ids.h
	${PYTHON} ${MESA_SRC}/src/gallium/auxiliary/indices/u_indices_gen.py \
	    src/gallium/auxiliary/u_indices_gen.c
	${PYTHON} ${MESA_SRC}/src/gallium/auxiliary/util/u_tracepoints.py \
	    -p ${MESA_SRC}/src/util/perf/ \
	    -C src/gallium/auxiliary/u_tracepoints.c
	${PYTHON} ${MESA_SRC}/src/gallium/auxiliary/util/u_tracepoints.py \
	    -p ${MESA_SRC}/src/util/perf/ \
	    -H src/gallium/auxiliary/u_tracepoints.h
	${PYTHON} ${MESA_SRC}/src/gallium/auxiliary/driver_trace/enums2names.py \
	    ${MESA_SRC}/src/gallium/include/pipe/p_defines.h \
	    ${MESA_SRC}/src/gallium/include/pipe/p_video_enums.h \
	    ${MESA_SRC}/src/util/blend.h \
	    -C src/gallium/auxiliary/tr_util.c \
	    -H src/gallium/auxiliary/tr_util.h \
	    -I tr_util.h
	${PYTHON} ${MESA_SRC}/src/gallium/auxiliary/indices/u_unfilled_gen.py \
	    src/gallium/auxiliary/u_unfilled_gen.c
	echo '#define MESA_GIT_SHA1 ""' > src/git_sha1.h
	${PYTHON} ${MESA_SRC}/src/intel/compiler/brw_nir_trig_workarounds.py \
	    -p ${MESA_SRC}/src/compiler/nir \
	    > src/intel/compiler/brw_nir_trig_workarounds.c
	${PYTHON} ${MESA_SRC}/src/intel/ds/intel_tracepoints.py \
	    -p ${MESA_SRC}/src/util/perf \
	    --utrace-hdr src/intel/ds/intel_tracepoints.h \
	    --perfetto-hdr src/intel/ds/intel_tracepoints_perfetto.h \
	    --utrace-src src/intel/ds/intel_tracepoints.c
	${PYTHON} ${MESA_SRC}/src/util/gen_zipped_xml_file.py \
	    ${MESA_SRC}/src/intel/genxml/gen40.xml \
	    ${MESA_SRC}/src/intel/genxml/gen45.xml \
	    ${MESA_SRC}/src/intel/genxml/gen50.xml \
	    ${MESA_SRC}/src/intel/genxml/gen60.xml \
	    ${MESA_SRC}/src/intel/genxml/gen70.xml \
	    ${MESA_SRC}/src/intel/genxml/gen75.xml \
	    ${MESA_SRC}/src/intel/genxml/gen80.xml \
	    ${MESA_SRC}/src/intel/genxml/gen90.xml \
	    ${MESA_SRC}/src/intel/genxml/gen110.xml \
	    ${MESA_SRC}/src/intel/genxml/gen120.xml \
	    ${MESA_SRC}/src/intel/genxml/gen125.xml \
	    ${MESA_SRC}/src/intel/genxml/gen200.xml \
	    ${MESA_SRC}/src/intel/genxml/gen300.xml \
	    > src/intel/genxml/genX_xml.h
	${PYTHON} ${MESA_SRC}/src/intel/isl/gen_format_layout.py \
	    --csv ${MESA_SRC}/src/intel/isl/isl_format_layout.csv \
	    --out src/intel/isl/isl_format_layout.c
	${PYTHON} ${MESA_SRC}/src/intel/perf/gen_perf.py \
	    --code src/intel/perf/intel_perf_metrics.c \
	    --header src/intel/perf/intel_perf_metrics.h \
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
	    ${MESA_SRC}/src/intel/perf/oa-adl.xml \
	    ${MESA_SRC}/src/intel/perf/oa-acmgt1.xml \
	    ${MESA_SRC}/src/intel/perf/oa-acmgt2.xml \
	    ${MESA_SRC}/src/intel/perf/oa-acmgt3.xml \
	    ${MESA_SRC}/src/intel/perf/oa-mtlgt2.xml \
	    ${MESA_SRC}/src/intel/perf/oa-mtlgt3.xml \
	    ${MESA_SRC}/src/intel/perf/oa-arlgt1.xml \
	    ${MESA_SRC}/src/intel/perf/oa-arlgt2.xml \
	    ${MESA_SRC}/src/intel/perf/oa-lnl.xml \
	    ${MESA_SRC}/src/intel/perf/oa-bmg.xml
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_entrypoints_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --proto --weak \
	    --out-h src/intel/vulkan/anv_entrypoints.h \
	    --out-c src/intel/vulkan/anv_entrypoints.c \
	    --prefix anv \
	    --beta false \
	    --device-prefix gfx9 \
	    --device-prefix gfx11 \
	    --device-prefix gfx12 \
	    --device-prefix gfx125 \
	    --device-prefix gfx20 \
	    --device-prefix gfx30 \
	    --device-prefix anv_doom64 \
	    --device-prefix anv_hitman3 \
	    --device-prefix anv_android \
	    --device-prefix anv_rmv
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_entrypoints_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --proto --weak \
	    --out-h src/intel/vulkan_hasvk/anv_entrypoints.h \
	    --out-c src/intel/vulkan_hasvk/anv_entrypoints.c \
	    --prefix anv \
	    --beta false \
	    --device-prefix gfx7 \
	    --device-prefix gfx75 \
	    --device-prefix gfx8 \
	    --device-prefix doom64
	${PYTHON} ${MESA_SRC}/src/util/glsl2spirv.py \
	    ${MESA_SRC}/src/compiler/glsl/float64.glsl \
	    src/intel/vulkan/shaders/float64_spv.h \
	    ${GLSLV} \
	    --create-entry main \
	    --vn float64_spv_source \
	    --glsl-version 450 \
	    -Olib
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/api_exec_init.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mapi/glapi/gen/api_exec_init.c
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/gl_enums.py \
	    -f ${MESA_SRC}/src/mapi/glapi/registry/gl.xml \
	    > src/mapi/glapi/gen/enums.c
	${PYTHON} ${MESA_SRC}/src/mesa/main/format_fallback.py \
	    ${MESA_SRC}/src/mesa/main/formats.csv \
	    src/mesa/format_fallback.c
	${PYTHON} ${MESA_SRC}/src/mesa/main/format_info.py \
	    ${MESA_SRC}/src/mesa/main/formats.csv \
	    > src/mesa/format_info.h
	${PYTHON} ${MESA_SRC}/src/mesa/main/get_hash_generator.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    > src/mesa/get_hash.h
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/gl_table.py \
	    -f ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    -m dispatch > src/mesa/main/dispatch.h
	${PYTHON} ${MESA_SRC}/src/mapi/glapi/gen/gl_marshal_h.py \
	    ${MESA_SRC}/src/mapi/glapi/gen/gl_and_es_API.xml \
	    8 \
	    > src/mesa/main/marshal_generated.h
	${FLEX} -o src/mesa/program/lex.yy.c \
	    ${MESA_SRC}/src/mesa/program/program_lexer.l
	${BISON} -Wno-deprecated -o src/mesa/program/program_parse.tab.c \
	    --defines=src/mesa/program/program_parse.tab.h \
	    ${MESA_SRC}/src/mesa/program/program_parse.y
	${PYTHON} ${MESA_SRC}/src/util/format/u_format_table.py \
	    ${MESA_SRC}/src/util/format/u_format.yaml \
	    > src/util/format/u_format_table.c
	${PYTHON} ${MESA_SRC}/src/util/format/u_format_table.py \
	    ${MESA_SRC}/src/util/format/u_format.yaml \
	    --enums > src/util/format/u_format_gen.h
	${PYTHON} ${MESA_SRC}/src/util/format/u_format_table.py \
	    ${MESA_SRC}/src/util/format/u_format.yaml \
	    --header > src/util/format/u_format_pack.h
	${PYTHON} ${MESA_SRC}/src/vulkan/util/gen_enum_to_str.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --outdir src/vulkan/util \
	    --beta false
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
	    --beta false \
	    --prefix radv \
	    --device-prefix sqtt \
	    --device-prefix rra \
	    --device-prefix rmv \
	    --device-prefix ctx_roll \
	    --device-prefix metro_exodus \
	    --device-prefix rage2 \
	    --device-prefix quantic_dream \
	    --device-prefix annotate
	${GLSLV} -V -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    --target-env spirv1.5 -x \
	    -o src/vulkan/runtime/bvh/lbvh_main.spv.h \
	    ${MESA_SRC}/src/vulkan/runtime/bvh/lbvh_main.comp --quiet
	${GLSLV} -V -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    --target-env spirv1.5 -x \
	    -o src/vulkan/runtime/bvh/leaf.spv.h \
	    ${MESA_SRC}/src/vulkan/runtime/bvh/leaf.comp --quiet \
	    -DALWAYS_ACTIVE=0
	${GLSLV} -V -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    --target-env spirv1.5 -x \
	    -o src/vulkan/runtime/bvh/leaf_always_active.spv.h \
	    ${MESA_SRC}/src/vulkan/runtime/bvh/leaf.comp --quiet \
	    -DALWAYS_ACTIVE=1
	${GLSLV} -V -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    --target-env spirv1.5 -x \
	    -o src/vulkan/runtime/bvh/morton.spv.h \
	    ${MESA_SRC}/src/vulkan/runtime/bvh/morton.comp --quiet
	${GLSLV} -V -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    --target-env spirv1.5 -x \
	    -o src/vulkan/runtime/bvh/ploc_internal.spv.h \
	    ${MESA_SRC}/src/vulkan/runtime/bvh/ploc_internal.comp --quiet
	${GLSLV} -V -I${MESA_SRC}/src/amd/vulkan/bvh \
	    -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    --target-env spirv1.5 -x \
	    -o src/amd/vulkan/bvh/copy.spv.h \
	    ${MESA_SRC}/src/amd/vulkan/bvh/copy.comp \
	    --depfile src/amd/vulkan/bvh/copy.spv.h.d --quiet
	${GLSLV} -V -I${MESA_SRC}/src/amd/vulkan/bvh \
	    -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    --target-env spirv1.5 -x \
	    -o src/amd/vulkan/bvh/encode.spv.h \
	    ${MESA_SRC}/src/amd/vulkan/bvh/encode.comp --quiet -DCOMPACT=0
	${GLSLV} -V -I${MESA_SRC}/src/amd/vulkan/bvh \
	    -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    --target-env spirv1.5 -x \
	    -o src/amd/vulkan/bvh/encode_compact.spv.h \
	    ${MESA_SRC}/src/amd/vulkan/bvh/encode.comp --quiet -DCOMPACT=1
	${GLSLV} -V -I${MESA_SRC}/src/amd/vulkan/bvh \
	    -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    --target-env spirv1.5 -x \
	    -o src/amd/vulkan/bvh/header.spv.h \
	    ${MESA_SRC}/src/amd/vulkan/bvh/header.comp --quiet
	${GLSLV} -V -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    --target-env spirv1.5 -x \
	    -o src/amd/vulkan/bvh/update.spv.h \
	    ${MESA_SRC}/src/amd/vulkan/bvh/update.comp \
	    --depfile src/amd/vulkan/bvh/update.spv.h.d --quiet
	${GLSLV} -V -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    --target-env spirv1.5 -x \
	    -o src/vulkan/runtime/bvh/lbvh_generate_ir.spv.h \
	    ${MESA_SRC}/src/vulkan/runtime/bvh/lbvh_generate_ir.comp --quiet
.for SHADER in init fill histogram prefix scatter_0_even scatter_0_odd scatter_1_even scatter_1_odd
	${GLSLV} -V \
	    --target-env spirv1.5 -x \
	    -o src/vulkan/runtime/radix_sort/shaders/${SHADER}.comp.spv.h  \
	    ${MESA_SRC}/src/vulkan/runtime/radix_sort/shaders/${SHADER}.comp \
	    -DRS_KEYVAL_DWORDS=2 --quiet
.endfor
	${GLSLV} -V -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    -I${MESA_SRC}/src/intel/vulkan/bvh \
	    --target-env spirv1.5 -x \
	    -o src/intel/vulkan/bvh/copy.spv.h \
	    ${MESA_SRC}/src/intel/vulkan/bvh/copy.comp --quiet
	${GLSLV} -V -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    -I${MESA_SRC}/src/intel/vulkan/bvh \
	    --target-env spirv1.5 -x \
	    -o src/intel/vulkan/bvh/encode.spv.h \
	    ${MESA_SRC}/src/intel/vulkan/bvh/encode.comp --quiet
	${GLSLV} -V -I${MESA_SRC}/src/vulkan/runtime/bvh \
	    -I${MESA_SRC}/src/intel/vulkan/bvh \
	    --target-env spirv1.5 -x \
	    -o src/intel/vulkan/bvh/header.spv.h \
	    ${MESA_SRC}/src/intel/vulkan/bvh/header.comp --quiet

	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_icd_gen.py \
	    --api-version 1.4 \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --lib-path libvulkan_radeon.so \
	    --out src/amd/vulkan/radeon_icd.json
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_icd_gen.py \
	    --api-version 1.4 \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --lib-path libvulkan_intel.so \
	    --out src/intel/vulkan/intel_icd.json
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_icd_gen.py \
	    --api-version 1.3 \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --lib-path libvulkan_intel_hasvk.so \
	    --out src/intel/vulkan_hasvk/intel_hasvk_icd.json
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_entrypoints_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --proto --weak \
	    --out-h src/vulkan/wsi/wsi_common_entrypoints.h \
	    --out-c src/vulkan/wsi/wsi_common_entrypoints.c \
	    --prefix wsi \
	    --beta false
	${PYTHON} ${MESA_SRC}/src/vulkan/runtime/vk_format_info_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --out-c src/vulkan/runtime/vk_format_info.c \
	    --out-h src/vulkan/runtime/vk_format_info.h
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_entrypoints_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --proto --weak \
	    --out-h src/vulkan/runtime/vk_common_entrypoints.h \
	    --out-c src/vulkan/runtime/vk_common_entrypoints.c \
	    --prefix vk_common \
	    --beta false
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_entrypoints_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --proto --weak \
	    --out-h src/vulkan/runtime/vk_cmd_enqueue_entrypoints.h \
	    --out-c src/vulkan/runtime/vk_cmd_enqueue_entrypoints.c \
	    --prefix vk_cmd_enqueue \
	    --prefix vk_cmd_enqueue_unless_primary \
	    --beta false
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_dispatch_trampolines_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --out-c src/vulkan/runtime/vk_dispatch_trampolines.c \
	    --out-h src/vulkan/runtime/vk_dispatch_trampolines.h \
	    --beta false
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_physical_device_features_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --out-c src/vulkan/runtime/vk_physical_device_features.c \
	    --out-h src/vulkan/runtime/vk_physical_device_features.h \
	    --beta false
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_physical_device_properties_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --out-c src/vulkan/runtime/vk_physical_device_properties.c \
	    --out-h src/vulkan/runtime/vk_physical_device_properties.h \
	    --beta false
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_cmd_queue_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --out-c src/vulkan/runtime/vk_cmd_queue.c \
	    --out-h src/vulkan/runtime/vk_cmd_queue.h \
	    --beta false
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_dispatch_table_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --out-c src/vulkan/util/vk_dispatch_table.c \
	    --out-h src/vulkan/util/vk_dispatch_table.h \
	    --beta false
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_extensions_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --out-c src/vulkan/util/vk_extensions.c \
	    --out-h src/vulkan/util/vk_extensions.h
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_struct_type_cast_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --outdir src/vulkan/util \
	    --beta false
	${PYTHON} ${MESA_SRC}/src/util/driconf_static.py \
	    ${MESA_SRC}/src/util/00-mesa-defaults.conf \
	    ${MESA_SRC}/src/util/00-radv-defaults.conf \
	    src/util/driconf_static.h
	${GLSLV} -V -S comp -x \
	    -o src/vulkan/runtime/astc_spv.h \
	    ${MESA_SRC}/src/compiler/glsl/astc_decoder.glsl \
	    --quiet
	sed -e 's/@nouveau_drm_screen_create@//' \
	    -e 's/@radeon_drm_winsys_create@/radeon_drm_winsys_create;/' \
	    -e 's/@amdgpu_winsys_create@/amdgpu_winsys_create;/' \
	    -e 's/@fd_drm_screen_create_renderonly@//' \
	    -e 's/@ac_init_shared_llvm_once@/ac_init_shared_llvm_once;/' \
	    -e 's/@va_driver_init@/__vaDriverInit_*_*;/' \
	    -e 's/@vdp_imp_device_create_x11@//' \
	    < ${MESA_SRC}/src/gallium/targets/dri/dri.sym.in \
	    > src/gallium/targets/dri/dri.sym
	cp ${MESA_SRC}/src/gallium/targets/dril/dri.sym.in \
	    src/gallium/targets/dril/dri.sym
	sed -e 's/@radeon_drm_winsys_create@/radeon_drm_winsys_create;/' \
	    -e 's/@amdgpu_winsys_create@/amdgpu_winsys_create;/' \
	    -e 's/@ac_init_shared_llvm_once@/ac_init_shared_llvm_once;/' \
	    < ${MESA_SRC}/src/gallium/targets/va/va.sym.in \
	    > src/gallium/targets/va/va.sym
	${PYTHON} ${MESA_SRC}/src/amd/vulkan/layers/radv_annotate_layer_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --out-c src/amd/vulkan/layers/radv_annotate_layer.c \
	    --beta false
	${PYTHON} ${MESA_SRC}/src/intel/compiler/brw_device_sha1_gen_c.py \
	    --outdir src/intel/compiler
	${PYTHON} ${MESA_SRC}/src/intel/compiler/brw_nir_lower_fsign.py \
	    -p ${MESA_SRC}/src/compiler/nir/ \
	    > src/intel/compiler/brw_nir_lower_fsign.c
	${PYTHON} ${MESA_SRC}/src/intel/compiler/elk/elk_nir_trig_workarounds.py \
	    -p ${MESA_SRC}/src/compiler/nir/ \
	    > src/intel/compiler/elk/elk_nir_trig_workarounds.c
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_physical_device_spirv_caps_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --out-c src/vulkan/runtime/vk_physical_device_spirv_caps.c \
	    --beta false
	${PYTHON} ${MESA_SRC}/src/vulkan/util/vk_synchronization_helpers_gen.py \
	    --xml ${MESA_SRC}/src/vulkan/registry/vk.xml \
	    --out-c src/vulkan/runtime/vk_synchronization_helpers.c \
	    --beta false

clean:
	rm -f src/amd/common/amdgfxregs.h
	rm -f src/amd/common/gfx10_format_table.c
	rm -f src/amd/common/sid_tables.h
	rm -f src/amd/compiler/aco_builder.h
	rm -f src/amd/compiler/aco_opcodes.cpp
	rm -f src/amd/compiler/aco_opcodes.h
	rm -f src/amd/vulkan/bvh/copy.spv.h
	rm -f src/amd/vulkan/bvh/copy.spv.h.d
	rm -f src/amd/vulkan/bvh/encode.spv.h
	rm -f src/amd/vulkan/bvh/encode_compact.spv.h
	rm -f src/amd/vulkan/bvh/header.spv.h
	rm -f src/amd/vulkan/bvh/update.spv.h
	rm -f src/amd/vulkan/bvh/update.spv.h.d
	rm -f src/amd/vulkan/layers/radv_annotate_layer.c
	rm -f src/amd/vulkan/radeon_icd.json
	rm -f src/amd/vulkan/radv_entrypoints.c
	rm -f src/amd/vulkan/radv_entrypoints.h
	rm -f src/compiler/builtin_types.c
	rm -f src/compiler/builtin_types.h
	rm -f src/compiler/glsl/astc_glsl.h
	rm -f src/compiler/glsl/bc1_glsl.h
	rm -f src/compiler/glsl/bc4_glsl.h
	rm -f src/compiler/glsl/cross_platform_settings_piece_all.h
	rm -f src/compiler/glsl/etc2_rgba_stitch_glsl.h
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
	rm -f src/compiler/spirv/spirv_info.h
	rm -f src/compiler/spirv/vtn_gather_types.c
	rm -f src/compiler/spirv/vtn_generator_ids.h
	rm -f src/gallium/auxiliary/tr_util.c
	rm -f src/gallium/auxiliary/tr_util.h
	rm -f src/gallium/auxiliary/u_indices_gen.c
	rm -f src/gallium/auxiliary/u_tracepoints.c
	rm -f src/gallium/auxiliary/u_tracepoints.h
	rm -f src/gallium/auxiliary/u_unfilled_gen.c
	rm -f src/gallium/drivers/r300/r300_nir_algebraic.c
	rm -f src/gallium/drivers/r600/egd_tables.h
	rm -f src/gallium/drivers/radeonsi/si_tracepoints.c
	rm -f src/gallium/drivers/radeonsi/si_tracepoints.h
	rm -f src/gallium/drivers/radeonsi/si_tracepoints_perfetto.h
	rm -f src/gallium/targets/dri/dri.sym
	rm -f src/gallium/targets/dril/dri.sym
	rm -f src/gallium/targets/va/va.sym
	rm -f src/git_sha1.h
	rm -f src/intel/compiler/brw_device_sha1_gen.c
	rm -f src/intel/compiler/brw_nir_lower_fsign.c
	rm -f src/intel/compiler/brw_nir_trig_workarounds.c
	rm -f src/intel/compiler/elk/elk_nir_trig_workarounds.c
	rm -f src/intel/dev/intel_device_info_gen.h
	rm -f src/intel/dev/intel_wa.c
	rm -f src/intel/dev/intel_wa.h
	rm -f src/intel/ds/intel_tracepoints.c
	rm -f src/intel/ds/intel_tracepoints.h
	rm -f src/intel/ds/intel_tracepoints_perfetto.h
	rm -f src/intel/genxml/genX_bits.h
	rm -f src/intel/genxml/genX_xml.h
.for GEN in 40 45 50 60 70 75 80 90 110 120 125 200 300
	rm -f src/intel/genxml/gen${GEN}_pack.h
	rm -f src/intel/genxml/gen${GEN}_cl_pack.h
.endfor
.for GEN in 125 200 300
	rm -f src/intel/genxml/gen${GEN}_rt_pack.h
	rm -f src/intel/genxml/gen${GEN}_rt_cl_pack.h
.endfor
	rm -f src/intel/isl/isl_format_layout.c
	rm -f src/intel/perf/intel_perf_metrics.c
	rm -f src/intel/perf/intel_perf_metrics.h
.for GEN in 80 90 110 120 125 200 300
	rm -f src/intel/shaders/intel_gfx${GEN}_shaders.spv
	rm -f src/intel/shaders/intel_gfx${GEN}_shaders_spv.h
	rm -f src/intel/shaders/intel_gfx${GEN}_shaders_binding.h
.endfor
	rm -f src/intel/vulkan/anv_entrypoints.c
	rm -f src/intel/vulkan/anv_entrypoints.h
	rm -f src/intel/vulkan/bvh/copy.spv.h
	rm -f src/intel/vulkan/bvh/encode.spv.h
	rm -f src/intel/vulkan/bvh/header.spv.h
	rm -f src/intel/vulkan/intel_icd.json
	rm -f src/intel/vulkan/shaders/float64_spv.h
	rm -f src/intel/vulkan_hasvk/anv_entrypoints.c
	rm -f src/intel/vulkan_hasvk/anv_entrypoints.h
	rm -f src/intel/vulkan_hasvk/intel_hasvk_icd.json
	rm -f src/mapi/es1api/glapi_mapi_tmp.h
	rm -f src/mapi/es2api/glapi_mapi_tmp.h
	rm -f src/mapi/glapi/gen/api_beginend_init.h
	rm -f src/mapi/glapi/gen/api_exec_decl.h
	rm -f src/mapi/glapi/gen/api_exec_init.c
	rm -f src/mapi/glapi/gen/api_hw_select_init.h
	rm -f src/mapi/glapi/gen/api_save.h
	rm -f src/mapi/glapi/gen/api_save_init.h
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
	rm -f src/mapi/glapi/gen/unmarshal_table.c
	rm -f src/mapi/shared-glapi/glapi_mapi_tmp.h
	rm -f src/mesa/format_fallback.c
	rm -f src/mesa/format_info.h
	rm -f src/mesa/get_hash.h
	rm -f src/mesa/main/dispatch.h
	rm -f src/mesa/main/marshal_generated.h
	rm -f src/mesa/main/remap_helper.h
	rm -f src/mesa/program/lex.yy.c
	rm -f src/mesa/program/program_parse.tab.c
	rm -f src/mesa/program/program_parse.tab.h
	rm -f src/util/driconf_static.h
	rm -f src/util/format/u_format_gen.h
	rm -f src/util/format/u_format_pack.h
	rm -f src/util/format/u_format_table.c
	rm -f src/util/format_srgb.c
	rm -f src/vulkan/runtime/astc_spv.h
	rm -f src/vulkan/runtime/bvh/lbvh_generate_ir.spv.h
	rm -f src/vulkan/runtime/bvh/lbvh_main.spv.h
	rm -f src/vulkan/runtime/bvh/leaf.spv.h
	rm -f src/vulkan/runtime/bvh/leaf_always_active.spv.h
	rm -f src/vulkan/runtime/bvh/morton.spv.h
	rm -f src/vulkan/runtime/bvh/ploc_internal.spv.h
	rm -f src/vulkan/runtime/radix_sort/shaders/fill.comp.spv.h
	rm -f src/vulkan/runtime/radix_sort/shaders/histogram.comp.spv.h
	rm -f src/vulkan/runtime/radix_sort/shaders/init.comp.spv.h
	rm -f src/vulkan/runtime/radix_sort/shaders/prefix.comp.spv.h
	rm -f src/vulkan/runtime/radix_sort/shaders/scatter_0_even.comp.spv.h
	rm -f src/vulkan/runtime/radix_sort/shaders/scatter_0_odd.comp.spv.h
	rm -f src/vulkan/runtime/radix_sort/shaders/scatter_1_even.comp.spv.h
	rm -f src/vulkan/runtime/radix_sort/shaders/scatter_1_odd.comp.spv.h
	rm -f src/vulkan/runtime/vk_cmd_enqueue_entrypoints.c
	rm -f src/vulkan/runtime/vk_cmd_enqueue_entrypoints.h
	rm -f src/vulkan/runtime/vk_cmd_queue.c
	rm -f src/vulkan/runtime/vk_cmd_queue.h
	rm -f src/vulkan/runtime/vk_common_entrypoints.c
	rm -f src/vulkan/runtime/vk_common_entrypoints.h
	rm -f src/vulkan/runtime/vk_dispatch_trampolines.c
	rm -f src/vulkan/runtime/vk_dispatch_trampolines.h
	rm -f src/vulkan/runtime/vk_format_info.c
	rm -f src/vulkan/runtime/vk_format_info.h
	rm -f src/vulkan/runtime/vk_physical_device_features.c
	rm -f src/vulkan/runtime/vk_physical_device_features.h
	rm -f src/vulkan/runtime/vk_physical_device_properties.c
	rm -f src/vulkan/runtime/vk_physical_device_properties.h
	rm -f src/vulkan/runtime/vk_physical_device_spirv_caps.c
	rm -f src/vulkan/runtime/vk_synchronization_helpers.c
	rm -f src/vulkan/util/vk_dispatch_table.c
	rm -f src/vulkan/util/vk_dispatch_table.h
	rm -f src/vulkan/util/vk_enum_defines.h
	rm -f src/vulkan/util/vk_enum_to_str.c
	rm -f src/vulkan/util/vk_enum_to_str.h
	rm -f src/vulkan/util/vk_extensions.c
	rm -f src/vulkan/util/vk_extensions.h
	rm -f src/vulkan/util/vk_struct_type_cast.h
	rm -f src/vulkan/wsi/wsi_common_entrypoints.c
	rm -f src/vulkan/wsi/wsi_common_entrypoints.h
