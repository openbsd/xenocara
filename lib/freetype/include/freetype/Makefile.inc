includes: ${INCS} incinstall _SUBDIRUSE

incinstall:
	@cd ${DIST}; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 \
		$$i ${DESTDIR}${INCSDIR}"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

.if !target(_SUBDIRUSE)
_SUBDIRUSE:
.endif
