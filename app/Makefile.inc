.if exists(Makefile.am)
realinstall:
	exec ${MAKE_ENV} ${MAKE} install-strip \
	INSTALL_STRIP_PROGRAM="/usr/bin/install -s -c -o root -g bin"
.endif
