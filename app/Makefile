# $OpenBSD: Makefile,v 1.27 2010/07/23 12:23:25 jakemsr Exp $
.include <bsd.xconf.mk>

SUBDIR= bdftopcf mkfontscale mkfontdir \
	appres beforelight bitmap editres fonttosfnt fslsfonts \
	fstobdf	iceauth ico listres luit oclock \
	rgb scripts sessreg setxkbmap showfont \
	smproxy twm video viewres x11perf xauth xbacklight xbiff xcalc \
	xclipboard xclock xcmsdb xcompmgr xconsole  \
	xdm xdpyinfo xedit xev xeyes xf86dga \
	xfd xfontsel xfsinfo xfwp xgamma xgc xhost \
	xinit xinput xkbcomp xkbevd xkbprint xkbutils xkill xload xlogo \
	xlsatoms xlsclients xlsfonts xmag xman xmessage \
	xmodmap xmore xpr \
	xprop xrandr xrdb xrefresh xset \
	xsetroot xsm xstdcmap xvidtune xvinfo xwd xwininfo \
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
