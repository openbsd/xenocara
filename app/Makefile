# $OpenBSD: Makefile,v 1.15 2008/03/25 23:41:50 matthieu Exp $
.include <bsd.xconf.mk>

SUBDIR= bdftopcf mkfontscale mkfontdir \
	appres beforelight bitmap editres fonttosfnt fslsfonts \
	fstobdf	iceauth ico lbxproxy listres luit oclock \
	proxymngr rgb rstart scripts sessreg setxkbmap showfont \
	smproxy twm viewres x11perf xauth xbacklight xbiff xcalc \
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
.if ${XENOCARA_HAVE_SHARED_LIBS:L} == "yes"
SUBDIR += xfs
.endif

# GL
.if ${XENOCARA_BUILD_GL:L} == "yes"
SUBDIR +=   xdriinfo glxgears glxinfo
.endif

# extras
SUBDIR += cwm fvwm ssh-askpass xterm xidle xlockmore xsystrace \
	  xtsscale xvctl

.include <bsd.subdir.mk>

build:	_SUBDIRUSE
