#	$OpenBSD: Makefile,v 1.1 2013/03/09 14:34:46 matthieu Exp $

FONTCONFIG = ${.CURDIR}/../../../dist/fontconfig
INCS = fcfreetype.h fcprivate.h  fontconfig.h

obj: _xenocara_obj

all:
	@echo done

install:
	@for i in ${INCS}; do \
            j="cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
                ${INSTALL_DATA} ${FONTCONFIG}/fontconfig/$$i \
			${DESTDIR}${INCSDIR}/fontconfig"; \
                echo "\tinstalling $$i"; \
                eval "$$j"; \
        done

.include <bsd.xorg.mk>
