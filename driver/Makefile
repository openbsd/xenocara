# $OpenBSD: Makefile,v 1.79 2021/06/25 19:32:01 matthieu Exp $

INPUT_DRV_DEFAULT= \
	xf86-input-keyboard \
	xf86-input-mouse \
	xf86-input-usbtablet \
	xf86-input-void \
	xf86-input-ws

INPUT_DRV_COMMON = ${INPUT_DRV_DEFAULT} \
	xf86-input-acecad xf86-input-elographics xf86-input-fpit \
	xf86-input-hyperpen xf86-input-joystick xf86-input-mutouch \
	xf86-input-penmount

INPUT_DRV_alpha=

INPUT_DRV_amd64= ${INPUT_DRV_COMMON} xf86-input-synaptics xf86-input-vmmouse

INPUT_DRV_hppa= ${INPUT_DRV_DEFAULT}

INPUT_DRV_i386= ${INPUT_DRV_COMMON} xf86-input-synaptics xf86-input-vmmouse

INPUT_DRV_landisk=

INPUT_DRV_loongson= ${INPUT_DRV_DEFAULT}

INPUT_DRV_luna88k= xf86-input-keyboard xf86-input-mouse xf86-input-void \
	xf86-input-ws

INPUT_DRV_macppc= ${INPUT_DRV_COMMON}

INPUT_DRV_octeon=

INPUT_DRV_riscv64= ${INPUT_DRV_DEFAULT}

INPUT_DRV_sparc64= ${INPUT_DRV_COMMON}

.if defined(INPUT_DRV_${MACHINE})
INPUT_DRV= ${INPUT_DRV_${MACHINE}}
.else
INPUT_DRV= ${INPUT_DRV_DEFAULT}
.endif

VIDEO_DRV_alpha=

VIDEO_DRV_amd64= \
	xf86-video-amdgpu xf86-video-apm \
	xf86-video-ast xf86-video-ati \
	xf86-video-cirrus xf86-video-dummy \
	xf86-video-intel xf86-video-mach64 \
	xf86-video-mga \
	xf86-video-nv xf86-video-openchrome xf86-video-r128 \
	xf86-video-savage xf86-video-siliconmotion \
	xf86-video-vesa xf86-video-vmware \
	xf86-video-wsfb xf86-video-wsudl

VIDEO_DRV_arm64= \
	xf86-video-amdgpu xf86-video-ati xf86-video-wsfb xf86-video-wsudl

VIDEO_DRV_armv7= \
	xf86-video-wsfb	xf86-video-wsudl

VIDEO_DRV_hppa= \
	xf86-video-wsfb \
	xf86-video-wsudl

VIDEO_DRV_i386= \
	${VIDEO_DRV_amd64} \
	xf86-video-geode

VIDEO_DRV_landisk=

VIDEO_DRV_loongson= \
	xf86-video-ati xf86-video-wsfb

VIDEO_DRV_luna88k= \
	xf86-video-wsfb

VIDEO_DRV_macppc= \
	xf86-video-ati xf86-video-mach64 xf86-video-nv xf86-video-r128 \
	xf86-video-wsfb xf86-video-wsudl

VIDEO_DRV_octeon=

VIDEO_DRV_powerpc64= \
	xf86-video-ati xf86-video-wsfb xf86-video-wsudl

VIDEO_DRV_riscv64= \
	xf86-video-amdgpu xf86-video-ati xf86-video-wsfb xf86-video-wsudl

VIDEO_DRV_sparc64= \
	xf86-video-ati xf86-video-mach64 xf86-video-r128 \
	xf86-video-sunffb \
	xf86-video-wildcatfb xf86-video-wsfb

.if defined(VIDEO_DRV_${MACHINE})
VIDEO_DRV= ${VIDEO_DRV_${MACHINE}}
.else
VIDEO_DRV= xf86-video-wsfb
.endif

SUBDIR= ${INPUT_DRV} ${VIDEO_DRV}

.include <bsd.subdir.mk>

build: _SUBDIRUSE
