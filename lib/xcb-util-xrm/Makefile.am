ACLOCAL_AMFLAGS = -I m4

MAINTAINERCLEANFILES = ChangeLog INSTALL Makefile.in

TEST_LIBS = $(shell pkg-config --libs x11 x11-xcb xcb xcb-aux)

.PHONY: ChangeLog INSTALL

INSTALL:
	$(INSTALL_CMD)  

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog INSTALL

EXTRA_DIST = autogen.sh xcb-xrm.pc.in include/xcb_xrm.h include/database.h
EXTRA_DIST += include/entry.h include/externals.h include/match.h
EXTRA_DIST += include/resource.h include/util.h
EXTRA_DIST += tests/tests_utils.h tests/tests_database_runner.sh
EXTRA_DIST += tests/resources/1/xresources1 tests/resources/1/xresources2
EXTRA_DIST += tests/resources/1/sub/xresources3
EXTRA_DIST += tests/resources/2/xenvironment tests/resources/2/.Xresources
EXTRA_DIST += tests/resources/3/loop.xresources

lib_LTLIBRARIES = libxcb-xrm.la

xcbinclude_HEADERS = include/xcb_xrm.h

AM_CFLAGS = $(CWARNFLAGS)

libxcb_xrm_la_SOURCES = src/database.c src/resource.c src/entry.c src/match.c src/util.c
libxcb_xrm_la_CPPFLAGS = -I$(srcdir)/include/ $(XCB_CFLAGS) $(XCB_AUX_CFLAGS)
libxcb_xrm_la_LIBADD = $(XCB_LIBS) $(XCB_AUX_LIBS) -lm
libxcb_xrm_la_LDFLAGS = -version-info 0:0:0 -no-undefined -export-symbols-regex '^xcb_xrm_'

pkgconfig_DATA = xcb-xrm.pc

DIRECT_TESTS = tests/tests_parser tests/tests_match
TESTS = $(DIRECT_TESTS) tests/tests_database_runner.sh
check_PROGRAMS = $(DIRECT_TESTS) tests/tests_database

tests_tests_parser_SOURCES = tests/tests_utils.c tests/tests_parser.c
tests_tests_parser_CPPFLAGS = -I$(srcdir)/include/ $(XCB_CFLAGS)
tests_tests_parser_LDADD = libxcb-xrm.la $(XCB_LIBS)
tests_tests_parser_LDFLAGS = $(TEST_LIBS)

tests_tests_match_SOURCES = tests/tests_utils.c tests/tests_match.c
tests_tests_match_CPPFLAGS = -I$(srcdir)/include/ $(XCB_CFLAGS)
tests_tests_match_LDADD = libxcb-xrm.la $(XCB_LIBS)
tests_tests_match_LDFLAGS = $(TEST_LIBS)

tests_tests_database_SOURCES = tests/tests_utils.c tests/tests_database.c
tests_tests_database_CPPFLAGS = -I$(srcdir)/include/ $(XCB_CFLAGS)
tests_tests_database_LDADD = libxcb-xrm.la $(XCB_LIBS)
tests_tests_database_LDFLAGS = $(TEST_LIBS)
