# $OpenBSD: Makefile,v 1.7 2007/04/27 18:19:00 bernd Exp $

.if ${MACHINE} != landisk
MKFONTSCALE= mkfontscale
.endif

SUBDIR= bdftopcf $(MKFONTSCALE)  mkfontdir \
	appres beforelight bitmap editres fonttosfnt fslsfonts \
	fstobdf	iceauth ico lbxproxy listres luit oclock \
	proxymngr rgb rstart scripts sessreg setxkbmap showfont \
	smproxy twm viewres x11perf xauth xbiff xcalc \
	xclipboard xclock xcmsdb xconsole  \
	xdm xdpyinfo  xdriinfo xedit xev xeyes xf86dga \
	xfd  xfindproxy xfontsel xfs xfsinfo xfwp xgamma xgc xhost \
	xinit xkbcomp xkbevd xkbprint xkbutils xkill xload xlogo \
	xlsatoms xlsclients xlsfonts xmag xman xmessage \
	xmodmap xmore xphelloworld xplsprinters xpr xprehashprinterlist \
	xprop xrandr xrdb xrefresh xset xsetmode xsetpointer \
	xsetroot xsm xstdcmap xtrap xvidtune xvinfo xwd xwininfo \
	xwud

# GL
SUBDIR += glxgears glxinfo

# extras
SUBDIR += cwm fvwm ssh-askpass wm2 xterm xidle xlockmore xsystrace xvctl

.include <bsd.subdir.mk>

build:	_SUBDIRUSE
