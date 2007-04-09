.if exists(Makefile.am)
realinstall:
	exec ${MAKE_ENV} ${MAKE} install-strip
.endif
