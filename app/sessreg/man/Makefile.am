
appmandir = $(APP_MAN_DIR)
appman_PRE = sessreg.man
appman_DATA = $(appman_PRE:man=$(APP_MAN_SUFFIX))

EXTRA_DIST = $(appman_PRE) filenames.sed.c
CLEANFILES = $(appman_DATA) filenames.sed
SUFFIXES = .$(APP_MAN_SUFFIX) .man

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)
filenames.sed: filenames.sed.c
	$(AM_V_GEN)$(CPP) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	    $(AM_CPPFLAGS) $(CPPFLAGS) $(srcdir)/filenames.sed.c | \
	    $(SED) -n -e '/s|__/ p' -e '/^\/__/ p' > $@

# String replacements in MAN_SUBSTS now come from xorg-macros.m4 via configure
MAN_SUBSTS += -f filenames.sed

sessreg.$(APP_MAN_SUFFIX): filenames.sed

.man.$(APP_MAN_SUFFIX):
	$(AM_V_GEN)$(SED) $(MAN_SUBSTS) < $< > $@
