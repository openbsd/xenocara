#	$OpenBSD: Makefile,v 1.1 2025/06/05 14:18:50 jsg Exp $

LIB=	vpe

NOPROFILE=

SRCS=	color_gamma.c \
	color_bg.c \
	vpe_scl_filters.c \
	background.c \
	vpe_visual_confirm.c \
	mpc.c \
	config_writer.c \
	color_gamut.c \
	vpelib.c \
	3dlut_builder.c \
	geometric_scaling.c \
	color_test_values.c \
	resource.c \
	color_table.c \
	color.c \
	color_cs.c \
	common.c \
	shaper_builder.c \
	custom_fp16.c \
	custom_float.c \
	conversion.c \
	fixpt31_32.c \
	vector.c \
	vpe10_plane_desc_writer.c \
	vpe10_vpe_desc_writer.c \
	vpe10_cm_common.c \
	vpe10_dpp.c \
	vpe10_resource.c \
	vpe10_mpc.c \
	vpe10_cmd_builder.c \
	vpe10_dpp_dscl.c \
	vpe10_dpp_cm.c \
	vpe10_opp.c \
	vpe10_background.c \
	vpe10_cdc_fe.c \
	vpe10_cdc_be.c \
	vpe10_vpec.c \
	vpe10_config_writer.c \
	vpe11_cmd_builder.c \
	vpe11_resource.c \
	vpe11_vpe_desc_writer.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/amd/vpelib/inc \
		-I${MESA_SRC}/src/amd/vpelib/src/core/inc \
		-I${MESA_SRC}/src/amd/vpelib/src/utils/inc \
		-I${MESA_SRC}/src/amd/vpelib/src/chip \
		-I${MESA_SRC}/src/amd/vpelib/src/chip/vpe10/inc \
		-I${MESA_SRC}/src/amd/vpelib/src/chip/vpe11/inc \
		-DLITTLEENDIAN_CPU \
		-DVPE_BUILD_1_0 \
		-DVPE_BUILD_1_X \
		-DVPE_BUILD_1_1

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/amd/vpelib/src/chip/vpe10
.PATH: ${MESA_SRC}/src/amd/vpelib/src/chip/vpe11
.PATH: ${MESA_SRC}/src/amd/vpelib/src/core
.PATH: ${MESA_SRC}/src/amd/vpelib/src/utils
