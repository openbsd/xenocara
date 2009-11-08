bin_PROGRAMS = xcompmgr
man_MANS = xcompmgr.1

AM_CFLAGS = $(CWARNFLAGS) $(XCOMPMGR_CFLAGS)
xcompmgr_LDADD = $(XCOMPMGR_LIBS) -lm

EXTRA_DIST = \
	autogen.sh \
	ChangeLog \
	${man_MANS}

MAINTAINERCLEANFILES=ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog

