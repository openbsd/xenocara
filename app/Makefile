# $OpenBSD: Makefile,v 1.36 2018/12/21 14:37:24 matthieu Exp $
.include <bsd.xconf.mk>

SUBDIR= bdftopcf mkfontscale mkfontdir \
	appres beforelight bitmap editres fonttosfnt \
	iceauth ico listres luit oclock \
	rgb sessreg setxkbmap \
	smproxy twm viewres x11perf xauth xbacklight xbiff xcalc \
	xclipboard xclock xcmsdb xcompmgr xconsole  \
	xenodm xdpyinfo xedit xev xeyes xf86dga \
	xfd xfontsel xgamma xgc xhost \
	xinit xinput xkbcomp xkbevd xkbprint xkbutils xkill xload xlogo \
	xlsatoms xlsclients xlsfonts xmag xman xmessage \
	xmodmap xmore xpr \
	xprop xrandr xrdb xrefresh xset \
	xsetroot xsm xvidtune xvinfo xwd xwininfo \
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
