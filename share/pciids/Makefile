#	$OpenBSD: Makefile,v 1.2 2008/11/08 14:57:54 matthieu Exp $

.include <bsd.own.mk>

all: pci.ids

install:
	${INSTALL_DATA} -c pci.ids ${DESTDIR}${X11BASE}/share/

pci.ids: ${BSDSRCDIR}/sys/dev/pci/pcidevs
	perl ${.CURDIR}/pcidevs2pciids.pl < ${BSDSRCDIR}/sys/dev/pci/pcidevs \
		> pci.ids

clean:
	rm -f pci.ids

.include <bsd.xorg.mk>

