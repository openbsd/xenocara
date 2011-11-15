
if INSTALL_XDMSHELL
bin_PROGRAMS = xdmshell
else
noinst_PROGRAMS = xdmshell
endif

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CFLAGS = 			\
	$(CWARNFLAGS)		\
	$(XDMSHELL_CFLAGS)	\
	-DBINDIR="\"$(bindir)\""

xdmshell_SOURCES = xdmshell.c

if LINT
ALL_LINT_FLAGS=$(LINT_FLAGS) $(DEFS) $(DEFAULT_INCLUDES) \
		$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS)

lint:
	$(LINT) $(ALL_LINT_FLAGS) $(xdmshell_SOURCES)
endif LINT

