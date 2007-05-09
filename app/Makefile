# $OpenBSD: Makefile,v 1.9 2007/05/09 06:19:30 matthieu Exp $

.if ${MACHINE} != landisk
MKFONTSCALE= mkfontscale
.endif

SUBDIR= bdftopcf $(MKFONTSCALE)  mkfontdir \
	appres beforelight bitmap editres fonttosfnt fslsfonts \
	fstobdf	iceauth ico lbxproxy listres luit oclock \
	proxymngr rgb rstart scripts sessreg setxkbmap showfont \
	smproxy twm viewres x11perf xauth xbiff xcalc \
	xclipboard xclock xcmsdb xconsole  \
	xdm xdpyinfo xedit xev xeyes xf86dga \
	xfd  xfindproxy xfontsel xfsinfo xfwp xgamma xgc xhost \
	xinit xkbcomp xkbevd xkbprint xkbutils xkill xload xlogo \
	xlsatoms xlsclients xlsfonts xmag xman xmessage \
	xmodmap xmore xphelloworld xplsprinters xpr xprehashprinterlist \
	xprop xrandr xrdb xrefresh xset xsetmode xsetpointer \
	xsetroot xsm xstdcmap xtrap xvidtune xvinfo xwd xwininfo \
	xwud

# Font server doesn't link on architectures with static only libraries
.if ${MACHINE} != landisk && ${MACHINE} != "vax"
SUBDIR += xfs
.endif

# GL
.if ${MACHINE} != "sgi" && ${MACHINE} != "vax"
SUBDIR +=   xdriinfo glxgears glxinfo
.endif

# extras
SUBDIR += cwm fvwm ssh-askpass xterm xidle xlockmore xsystrace xvctl

.include <bsd.subdir.mk>

build:	_SUBDIRUSE
