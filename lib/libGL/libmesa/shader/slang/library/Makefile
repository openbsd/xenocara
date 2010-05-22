#	$OpenBSD: Makefile,v 1.1 2010/05/22 20:14:15 matthieu Exp $

TOP = ${.CURDIR}/../../../../../../dist/Mesa

GLSL_CL = ../../../../glsl/apps/compile
d = ${TOP}/src/mesa/shader/slang/library

#
# builtin library sources
#

BUILTIN = slang_common_builtin_gc.h slang_core_gc.h slang_fragment_builtin_gc.h slang_vertex_builtin_gc.h

BUILTIN += slang_120_core_gc.h slang_builtin_120_common_gc.h slang_builtin_120_fragment_gc.h



all: ${BUILTIN}

clean:
	rm -f *_gc.h

install:

slang_120_core_gc.h: ${d}/slang_120_core.gc
	$(GLSL_CL) fragment ${d}/slang_120_core.gc slang_120_core_gc.h

slang_builtin_120_common_gc.h: ${d}/slang_builtin_120_common.gc
	$(GLSL_CL) fragment ${d}/slang_builtin_120_common.gc slang_builtin_120_common_gc.h

slang_builtin_120_fragment_gc.h: ${d}/slang_builtin_120_fragment.gc
	$(GLSL_CL) fragment ${d}/slang_builtin_120_fragment.gc slang_builtin_120_fragment_gc.h

slang_common_builtin_gc.h: ${d}/slang_common_builtin.gc
	$(GLSL_CL) fragment ${d}/slang_common_builtin.gc slang_common_builtin_gc.h

slang_core_gc.h: ${d}/slang_core.gc
	$(GLSL_CL) fragment ${d}/slang_core.gc slang_core_gc.h

slang_fragment_builtin_gc.h: ${d}/slang_fragment_builtin.gc
	$(GLSL_CL) fragment ${d}/slang_fragment_builtin.gc slang_fragment_builtin_gc.h

slang_vertex_builtin_gc.h: ${d}/slang_vertex_builtin.gc
	$(GLSL_CL) vertex ${d}/slang_vertex_builtin.gc slang_vertex_builtin_gc.h
.include <bsd.xorg.mk>
