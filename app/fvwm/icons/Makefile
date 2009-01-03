# $OpenBSD: Makefile,v 1.5 2009/01/03 17:55:00 matthieu Exp $

.include "../Makefile.inc"

NOOBJ= Yes

BITMAPS= \
	fvwm.bitmap \
	ccode.icon \
	hcode.icon \
	ocode.icon \
	prog.icon \
	default.xbm

PIXMAPS= \
	Jlock.xpm Jnews.xpm Jrecycle.xpm Jxlock.xpm Monitor.xpm \
	Mosaic.xpm arrdown2.xpm arrows2.xpm arrup2.xpm bomb.xpm \
	button-close.xpm button-horiz.xpm button-max.xpm button-vert.xpm \
	button.xpm clamp.xpm colormap.xpm datebook.xpm desk.xpm \
	dialog_box.xpm doomface.xpm editres.xpm eps.xpm exit.xpm \
	flow_chart.xpm folder2.xpm folders.xpm fvwm.xpm fvwm2.xpm \
	fvwm2_big.xpm fvwm3.xpm gnu-animal.xpm graphs.xpm jball.xpm \
	jbomb.xpm jbook1.xpm jbook2.xpm jcalc.xpm jclock.xpm jf.xpm \
	jgraph.xpm jline.xpm jlower.xpm jmag.xpm jmail.xpm jmove.xpm \
	jpaint.xpm jraise.xpm jresize.xpm jterm.xpm jwindow.xpm jx.xpm \
	mag_glass.xpm mail1.xpm mail2.xpm map.xpm math4.xpm \
	mini.audiovol.xpm mini.blah.xpm mini.bomb.xpm mini.checkmark.xpm \
	mini.cut.xpm mini.destroy.xpm mini.directory.xpm mini.dirlink.xpm \
	mini.excl.xpm mini.exit.xpm mini.eye.xpm mini.file.xpm \
	mini.filelink.xpm mini.fvwm.xpm mini.iconify.xpm mini.letter.xpm \
	mini.lower.xpm mini.maximize-horiz.xpm mini.maximize-vert.xpm \
	mini.maximize.xpm mini.maximize2.xpm mini.move.xpm \
	mini.netscape.xpm mini.rainbow.xpm mini.raise.xpm \
	mini.refresh.xpm mini.resize.xpm mini.resize2.xpm \
	mini.stick.xpm mini.stick2.xpm mini.stickpressed.xpm \
	mini.winXX-close.xpm mini.winXX-maximize.xpm \
	mini.winXX-minimize.xpm mini.xarchie.xpm mini.xbiff.xpm \
	mini.xboing.xpm mini.xlock.xpm mini.xmag.xpm mini.xman.xpm \
	mini.xpaint.xpm mini.xpm mini.xterm.xpm mini.zircon.xpm \
	page.xpm page2.xpm ps.xpm question.xpm rbomb.xpm rcalc.xpm \
	rterm.xpm side.fvwm2.xpm term.xpm tiff2.xpm toolbox.xpm \
	unknown1.xpm wierd_page3.xpm word_processor.xpm world.xpm \
	xcalc.xpm xemacs.xpm xlock.xpm xman.xpm xpaint.xpm \
	xterm-axp.xpm xterm-blank.xpm xterm-dec.xpm xterm-linux.xpm \
	xterm-sgi.xpm xterm-sol.xpm xterm-sun.xpm xterm.xpm \
	xv.xpm xv2.xpm


depend:

all:

install:
	@for f in ${BITMAPS}; do \
		echo "${INSTALL_DATA} $$f \
			${DESTDIR}${INCSDIR}/X11/bitmaps" ; \
		${INSTALL_DATA} ${.CURDIR}/$$f \
			${DESTDIR}${INCSDIR}/X11/bitmaps ; \
	done 
	@for f in ${PIXMAPS}; do \
		echo "${INSTALL_DATA} $$f \
			${DESTDIR}${INCSDIR}/X11/pixmaps" ; \
		${INSTALL_DATA} ${.CURDIR}/$$f \
			${DESTDIR}${INCSDIR}/X11/pixmaps ; \
	done 

clean:

cleandir:

.include <bsd.own.mk>
.include <bsd.xorg.mk>
