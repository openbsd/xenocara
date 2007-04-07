.if exists(Makefile.am)
install:
	exec ${MAKE_ENV} ${MAKE} install-strip
.endif
