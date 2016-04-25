# $OpenBSD: Makefile,v 1.34 2016/04/25 19:25:31 matthieu Exp $
.include <bsd.xconf.mk>

SUBDIR= bdftopcf mkfontscale mkfontdir \
	appres beforelight bitmap editres fonttosfnt fslsfonts \
	fstobdf	iceauth ico listres luit oclock \
	rgb scripts sessreg setxkbmap showfont \
	smproxy twm viewres x11perf xauth xbacklight xbiff xcalc \
	xclipboard xclock xcmsdb xcompmgr xconsole  \
	xdm xdpyinfo xedit xev xeyes xf86dga \
	xfd xfontsel xfsinfo xfwp xgamma xgc xhost \
	xinit xinput xkbcomp xkbevd xkbprint xkbutils xkill xload xlogo \
	xlsatoms xlsclients xlsfonts xmag xman xmessage \
	xmodmap xmore xpr \
	xprop xrandr xrdb xrefresh xset \
	xsetroot xsm xstdcmap xvidtune xvinfo xwd xwininfo \
	xwud

# GL
.if ${XENOCARA_BUILD_GL:L} == "yes"
SUBDIR +=   xdriinfo glxgears glxinfo
.endif

# extras
SUBDIR += cwm fvwm ssh-askpass video xterm xidle xlockmore \
	  xtsscale xvctl

.include <bsd.subdir.mk>

build:	_SUBDIRUSE
