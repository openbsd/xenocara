# $OpenBSD: Makefile,v 1.1 2007/08/25 19:17:16 matthieu Exp $

.include <bsd.own.mk>

NOOBJ=

ICONS=\
	xterm-color_32x32.xpm \
	xterm-color_48x48.xpm \
	xterm_32x32.xpm \
	xterm_48x48.xpm

depend all clean cleandir:

install:
	$(INSTALL_DATA) $(ICONS) $(DESTDIR)$(INCSDIR)/X11/pixmaps

.include <bsd.xorg.mk>
