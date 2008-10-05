#	$OpenBSD: Makefile,v 1.1 2008/10/05 08:06:07 matthieu Exp $

.include <bsd.own.mk>

all: pci.ids

install:
	${INSTALL_DATA} -c pci.ids ${X11BASE}/share/

pci.ids: ${BSDSRCDIR}/sys/dev/pci/pcidevs
	perl ${.CURDIR}/pcidevs2pciids.pl < ${BSDSRCDIR}/sys/dev/pci/pcidevs \
		> pci.ids

clean:
	rm -f pci.ids

.include <bsd.xorg.mk>

	
