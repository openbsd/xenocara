#	$OpenBSD: Makefile,v 1.2 2023/01/28 08:56:34 jsg Exp $

LIB=	crocus

NOPROFILE=

SRCS=	crocus_batch.c \
	crocus_blit.c \
	crocus_bufmgr.c \
	crocus_clear.c \
	crocus_context.c \
	crocus_draw.c \
	crocus_fence.c \
	crocus_fine_fence.c \
	crocus_formats.c \
	crocus_monitor.c \
	crocus_perf.c \
	crocus_performance_query.c \
	crocus_pipe_control.c \
	crocus_program.c \
	crocus_program_cache.c \
	crocus_resolve.c \
	crocus_resource.c \
	crocus_screen.c \
	crocus_disk_cache.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${C_SSE2_ARGS}
CPPFLAGS+=	-DGALLIUM_CROCUS \
		-I${MESA_SRC}/src/gallium/drivers/crocus \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/intel \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/drivers/crocus
