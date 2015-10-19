includes: ${INCS} incinstall _SUBDIRUSE

incinstall:
	for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/$$i || \
		${INSTALL_DATA} $$i ${DESTDIR}${INCSDIR}"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

.if !target(_SUBDIRUSE)
_SUBDIRUSE:
.endif
