SUBDIRS =
noinst_LTLIBRARIES = liblegacy.la

NULL:=#

liblegacy_la_SOURCES = legacy.h
liblegacy_la_LIBADD =

if ! KMS_ONLY
SUBDIRS += i810
liblegacy_la_SOURCES +=
liblegacy_la_LIBADD += \
		       i810/liblegacy-i810.la \
		       $(NULL)
endif

EXTRA_DIST = README
