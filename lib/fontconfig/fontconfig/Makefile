#	$OpenBSD: Makefile,v 1.2 2025/11/16 16:43:35 matthieu Exp $

FONTCONFIG = ${.CURDIR}/../../../dist/fontconfig
INCS = fcfreetype.h fcprivate.h

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
	${INSTALL_DATA} fontconfig.h ${DESTDIR}${INCSDIR}/fontconfig

.include <bsd.xorg.mk>
