# $OpenBSD: Makefile,v 1.65 2016/02/20 11:07:10 matthieu Exp $

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

INPUT_DRV_luna88k= ${INPUT_DRV_DEFAULT}

INPUT_DRV_macppc= ${INPUT_DRV_COMMON}

INPUT_DRV_octeon=

INPUT_DRV_sgi= ${INPUT_DRV_DEFAULT}

INPUT_DRV_sparc64= ${INPUT_DRV_COMMON}

INPUT_DRV_vax=

INPUT_DRV_zaurus= ${INPUT_DRV_DEFAULT}

.if defined(INPUT_DRV_${MACHINE})
INPUT_DRV= ${INPUT_DRV_${MACHINE}}
.else
INPUT_DRV= ${INPUT_DRV_DEFAULT}
.endif

VIDEO_DRV_alpha=

VIDEO_DRV_amd64= \
	xf86-video-apm xf86-video-ark xf86-video-ast xf86-video-ati \
	xf86-video-chips xf86-video-cirrus xf86-video-dummy xf86-video-glint \
	xf86-video-i128 xf86-video-intel xf86-video-mach64 \
	xf86-video-mga xf86-video-neomagic \
	xf86-video-nv xf86-video-openchrome xf86-video-r128 \
	xf86-video-rendition xf86-video-s3 \
	xf86-video-s3virge xf86-video-savage xf86-video-siliconmotion \
	xf86-video-sis xf86-video-tdfx xf86-video-trident \
	xf86-video-tseng xf86-video-vesa xf86-video-vmware \
	xf86-video-wsfb xf86-video-wsudl

VIDEO_DRV_armish= \
	xf86-video-wsfb	xf86-video-wsudl

VIDEO_DRV_armv7= \
	xf86-video-wsfb	xf86-video-wsudl

VIDEO_DRV_hppa= \
	xf86-video-wsfb \
	xf86-video-wsudl

VIDEO_DRV_i386= \
	${VIDEO_DRV_amd64} \
	xf86-video-geode \
	xf86-video-i740 \
	xf86-video-voodoo

VIDEO_DRV_landisk=

VIDEO_DRV_loongson= \
	xf86-video-wsfb

VIDEO_DRV_luna88k= \
	xf86-video-wsfb

VIDEO_DRV_macppc= \
	xf86-video-ati xf86-video-mach64 xf86-video-nv xf86-video-r128 \
	xf86-video-wsfb xf86-video-wsudl

VIDEO_DRV_octeon=

VIDEO_DRV_sgi= xf86-video-wsfb

VIDEO_DRV_sparc64= \
	xf86-video-ati xf86-video-mach64 xf86-video-r128 xf86-video-glint \
	xf86-video-sunffb \
	xf86-video-wildcatfb xf86-video-wsfb

VIDEO_DRV_vax=

.if defined(VIDEO_DRV_${MACHINE})
VIDEO_DRV= ${VIDEO_DRV_${MACHINE}}
.else
VIDEO_DRV= xf86-video-wsfb
.endif

SUBDIR= ${INPUT_DRV} ${VIDEO_DRV}

.include <bsd.subdir.mk>

build: _SUBDIRUSE
