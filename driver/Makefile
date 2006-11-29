# $OpenBSD: Makefile,v 1.4 2006/11/29 14:38:08 matthieu Exp $

INPUT_DRV= \
	xf86-input-acecad \
	xf86-input-digitaledge \
	xf86-input-elographics xf86-input-fpit \
	xf86-input-hyperpen xf86-input-jamstudio xf86-input-joystick \
	xf86-input-keyboard xf86-input-magellan xf86-input-magictouch \
	xf86-input-mouse xf86-input-mutouch \
	xf86-input-palmax xf86-input-spaceorb \
	xf86-input-summa xf86-input-tek4957 xf86-input-void

# Broken
# xf86-input-calcomp  xf86-input-citron  xf86-input-dmc
# xf86-input-dynapro xf86-input-elo2300 xf86-input-microtouch 
# xf86-input-penmount

.if ${MACHINE} == i386 || ${MACHINE} == amd64
INPUT_DRV+=xf86-input-vmmouse
.endif

INPUT_DRV+=openbsd-input-usbtablet

.if ${MACHINE} == zaurus
INPUT_DRV+=openbsd-input-ws
.endif

VIDEO_DRV= \
	xf86-video-apm xf86-video-ark xf86-video-ati xf86-video-chips  \
	xf86-video-cirrus xf86-video-cyrix xf86-video-dummy \
	xf86-video-glint xf86-video-i128 xf86-video-mga \
	xf86-video-neomagic \
	xf86-video-nv xf86-video-rendition xf86-video-s3 \
	xf86-video-s3virge xf86-video-savage xf86-video-siliconmotion \
	xf86-video-sis xf86-video-tdfx xf86-video-trident \
	xf86-video-tseng xf86-video-vesa xf86-video-vga  \
	xf86-video-wsfb

# uhg needs libdrm ? 
# xf86-video-via

.if ${MACHINE} == i386
VIDEO_DRV+= xf86-video-i740 xf86-video-nsc xf86-video-voodoo
.endif

.if ${MACHINE} == i386 || ${MACHINE} == amd64
VIDEO_DRV+= xf86-video-intel xf86-video-vmware
.endif

SUBDIR= ${INPUT_DRV} ${VIDEO_DRV}

.include <bsd.subdir.mk>

build: _SUBDIRUSE
update: _SUBDIRUSE

