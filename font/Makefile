# $OpenBSD: Makefile,v 1.3 2007/06/01 01:05:25 matthieu Exp $

# util must come 1st, alias must come last
SUBDIR= util encodings \
	adobe-100dpi adobe-75dpi adobe-utopia-100dpi adobe-utopia-75dpi	\
	adobe-utopia-type1 arabic-misc bh-100dpi bh-75dpi \
	bh-lucidatypewriter-100dpi bh-lucidatypewriter-75dpi bh-ttf \
	bh-type1 bitstream-100dpi bitstream-75dpi bitstream-speedo \
	bitstream-type1 cronyx-cyrillic cursor-misc daewoo-misc dec-misc \
	ibm-type1 isas-misc jis-misc micro-misc misc-cyrillic \
	misc-ethiopic misc-meltho misc-misc mutt-misc schumacher-misc \
	screen-cyrillic sony-misc sun-misc winitzki-cyrillic \
	xfree86-type1 ${EXTRAS} \
	alias

EXTRAS=	bitstream-vera-ttf

afterinstall:
	@if test -z "$(DESTDIR)"; then \
		echo "running fc-cache";\
		fc-cache -s -v ;\
	fi

.include <bsd.subdir.mk>

build: _SUBDIRUSE

